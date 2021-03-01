/*
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/asio/basic_socket.hpp>

using boost::asio::ip::tcp;

struct EventState
{
	EventState()
	{
		this->SetState(0);
		this->SetOccupationState(0);
	}

	DECLARE_ENUM(uint8, State);
	DECLARE_ENUM(uint8, OccupationState);
};

std::map<uint8, EventState> EventStateMap;

ServerSocket::ServerSocket(tcp::socket&& socket): Socket(std::move(socket))
{
	this->SetServerCode(-1);	
}

void ServerSocket::Start()
{
    AsyncRead();
}

void ServerSocket::ReadHandler()
{
    if (!IsOpen())
		return;

	MessageBuffer& packet = GetReadBuffer();
    while ( packet.GetActiveSize() > 0 )
    {
		uint8 cmd = packet.GetReadPointer()[0];
        auto itr = Handlers.find(cmd);
        if (itr == Handlers.end())
        {
            packet.Reset();
            break;
        }

        uint16 size = itr->second.packetSize;
        if (packet.GetActiveSize() < size)
            break;

		CUSTOM_PACKET_HEAD * head = (CUSTOM_PACKET_HEAD*)packet.GetReadPointer();

		sServerSocketMgr.AddPacket(new ServerPacketData(shared_from_this(), cmd, packet.GetReadPointer(), head->size));	

		packet.ReadCompleted(head->size);
    }

    AsyncRead();
}

void ServerSocket::HandleHeadcodeOnConnect(uint8 * Packet)
{
	POINTER_PCT(SL_ON_CONNECT, lpMsg, Packet, 0);

	this->SetServerCode(lpMsg->h.server);

	this->QueuePacket(Packet, lpMsg->h.get_size());

	for ( std::map<uint8, EventState>::const_iterator it = EventStateMap.begin(); it != EventStateMap.end(); ++it )
	{
		SL_EVENT_STATE_UPDATE pMsg(it->first, it->second.GetState(), it->second.GetOccupationState());
		this->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
	}
}

void ServerSocket::HandleHeadcodeCommon(uint8 * Packet)
{
	POINTER_PCT(CUSTOM_PACKET_HEAD, lpMsg, Packet, 0);

	sServerSocketMgr.SendPacketAll(Packet, lpMsg->size);
}

void ServerSocket::HandleWhisper(uint8 * Packet)
{
	POINTER_PCT(SL_WHISPER, lpMsg, Packet, 0);

	SL_WHISPER pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.entry = lpMsg->entry;
	pMsg.guid = lpMsg->guid;
	memcpy(pMsg.name, lpMsg->name, MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.target_name, lpMsg->target_name, MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.message, lpMsg->message, MAX_CHAT_LENGTH + 1);
	pMsg.result = 0;

	if ( CharacterDataPtr pCharacterData = sCharacterMgr->GetCharacterData(lpMsg->target_name) )
	{
		uint32 target_guid = pCharacterData->GetID();

		if ( target_guid != lpMsg->guid )
		{
			if ( pCharacterData->GetAuthority() != AUTHORITY_CODE_ADMINISTRATOR || (pCharacterData->GetAdminPanelFlags() & ADMIN_PANEL_WHISPER) )
			{
				
			}
			else
			{
				pMsg.result = 2;
			}
		}
		else
		{
			pMsg.result = 3;
		}
	}
	else
	{
		pMsg.result = 1;
	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void ServerSocket::CharacterCreateRequest(uint8 * Packet)
{
	POINTER_PCT(SL_CHARACTER_CREATE_REQUEST, lpMsg, Packet, 0);

	CharacterCreateDataPtr add_data(new CharacterCreateData(lpMsg->entry, lpMsg->character_class, lpMsg->character_name));
	add_data->ms_time = lpMsg->ms_time;
	add_data->account_id = lpMsg->account_id;
	add_data->server = lpMsg->h.server;
	add_data->socket = shared_from_this();
	add_data->strength = lpMsg->strength;
	add_data->agility = lpMsg->agility;
	add_data->vitality = lpMsg->vitality;
	add_data->energy = lpMsg->energy;
	add_data->leadership = lpMsg->leadership;
	add_data->life = lpMsg->life;
	add_data->mana = lpMsg->mana;
	add_data->points = lpMsg->points;
	add_data->level = lpMsg->level;
	add_data->world = lpMsg->world;
	add_data->x = lpMsg->x;
	add_data->y = lpMsg->y;
	add_data->slot = lpMsg->slot;

	sCharacterCreateQueue->Queue(add_data);
}

void ServerSocket::CharacterDeleteRequest(uint8 * Packet)
{
	POINTER_PCT(SL_CHARACTER_DELETE_REQUEST, lpMsg, Packet, 0);
	SL_CHARACTER_DELETE_RESULT pMsg;
	pMsg.result = CHARACTER_DELETE_RESULT_ERROR;
	pMsg.entry = lpMsg->entry;
	pMsg.ms_time = lpMsg->ms_time;
	pMsg.h.server = lpMsg->h.server;
	memcpy(pMsg.character_name, lpMsg->character_name, MAX_CHARACTER_LENGTH + 1);

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_SELECT_GUID_BY_NAME);
	stmt->setString(0, lpMsg->character_name);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
		Field* fields = result->Fetch();
		uint32 tmp_id = fields[0].GetUInt32();

		SQLTransaction trans = MuDatabase.BeginTransaction();
	
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_SKILL);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_QUEST_EVOLUTION);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_MAIL);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS_KILL);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GAMEOPTION);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_FRIEND);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_HELPER);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_RESTRICTION);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_STATISTICS);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_BUFF);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_ITEM_DELAY);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INFO);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_KICK_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_CHAT_BLOCK);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_PENTAGRAM_DELETE);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(0);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_INVENTORY);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_MUUN);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_EVENT_INVENTORY);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_SELL_NPC);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_GREMORY_CASE_INV);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_GREMORY_CASE_MOBILE);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_INVENTORY);
		stmt->setUInt32(tmp_id);
		stmt->setUInt8(ITEM_BOX_GREMORY_CASE_PS);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_DELETE_SCORE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DATE_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_QUEST_MU_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_SOUL_DELETE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DELETE_INSERT);
		stmt->setUInt32(lpMsg->account_id);
		stmt->setUInt32(tmp_id);
		stmt->setString(lpMsg->character_name);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_REMOVE);
		stmt->setUInt32(tmp_id);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);

		pMsg.result = CHARACTER_DELETE_RESULT_SUCCESS;

		sGuildMatching->RemoveGuildMatchingJoinInfo(tmp_id);
		sArkaWar->MemberRemove(tmp_id);
	}

	this->QueuePacket((uint8*)&pMsg, pMsg.h.size);
}

void ServerSocket::CharacterLoginLogout(uint8 * Packet)
{
	this->HandleHeadcodeCommon(Packet);

	sCharacterMgr->LoginLogout(Packet, shared_from_this());
}

/*******************************************
				GUILD
*******************************************/

//void ServerSocket::GuildListRequest(uint8 * Packet)
//{
//	sGuildMgr->ListRequest(shared_from_this());
//}

//void ServerSocket::GuildMemberListRequest(uint8 * Packet)
//{
//	sGuildMgr->MemberListRequest(Packet, shared_from_this());
//}

void ServerSocket::GuildCreateRequest(uint8 * Packet)
{
	sGuildMgr->CreateRequest(Packet, shared_from_this());
}

void ServerSocket::GuildRemoveRequest(uint8 * Packet)
{
	sGuildMgr->RemoveRequest(Packet);
}

void ServerSocket::GuildMemberAddRequest(uint8 * Packet)
{
	sGuildMgr->MemberAddRequest(Packet, shared_from_this());
}
	
void ServerSocket::GuildMemberDelRequest(uint8 * Packet)
{
	sGuildMgr->MemberDelRequest(Packet, shared_from_this());
}

void ServerSocket::GuildMemberStatusRequest(uint8 * Packet)
{
	sGuildMgr->MemberStatusRequest(Packet);
}

void ServerSocket::GuildRelationshipRequest(uint8 * Packet)
{
	sGuildMgr->RelationshipRequest(Packet);
}

void ServerSocket::GuildScore(uint8 * Packet)
{
	sGuildMgr->Score(Packet);
}

void ServerSocket::GuildNotice(uint8 * Packet)
{
	sGuildMgr->Notice(Packet);
}

/*******************************************
				CASTLE SIEGE
*******************************************/

void ServerSocket::CastleSiegeLoadData(uint8 * Packet)
{
	sCastleSiege->LoadData(shared_from_this());
}

void ServerSocket::CastleSiegeLoadNpc(uint8 * Packet)
{
	sCastleSiege->LoadNpc(shared_from_this());
}

void ServerSocket::CastleSiegeLoadRegisteredGuild(uint8 * Packet)
{
	sCastleSiege->LoadGuildRegister(shared_from_this());
}

void ServerSocket::CastleSiegeSaveNpc(uint8 * Packet)
{
	sCastleSiege->SaveNpc(Packet);
}

void ServerSocket::CastleSiegeClearGuild(uint8 * Packet)
{
	sCastleSiege->ClearGuild();
}

void ServerSocket::CastleSiegeInsertFinalGuild(uint8 * Packet)
{
	sCastleSiege->InsertFinalGuild(Packet);
}

void ServerSocket::CastleSiegeSaveTax(uint8 * Packet)
{
	sCastleSiege->SaveTaxRate(Packet);
}

void ServerSocket::CastleSiegeSaveMoney(uint8 * Packet)
{
	sCastleSiege->UpdateMoney(Packet);
}

void ServerSocket::CastleSiegeSaveOwnerStatus(uint8 * Packet)
{
	sCastleSiege->SaveOwnerStatus(Packet);
}

void ServerSocket::CastleSiegeInsertRegisteredGuild(uint8 * Packet)
{
	sCastleSiege->InsertRegisteredGuild(Packet);
}

void ServerSocket::CastleSiegeDeleteRegisteredGuild(uint8 * Packet)
{
	sCastleSiege->DeleteRegisteredGuild(Packet);
}

void ServerSocket::CastleSiegeUpdateRegisteredGuild(uint8 * Packet)
{
	sCastleSiege->UpdateRegisteredGuild(Packet);
}

void ServerSocket::CastleSiegeRegisteredGuild(uint8 * Packet)
{
	sCastleSiege->RegisteredGuild(Packet, shared_from_this());
}
	
void ServerSocket::CastleSiegeRegisteredGuildAll(uint8 * Packet)
{
	sCastleSiege->RegisteredGuildAll(Packet, shared_from_this());
}

void ServerSocket::AdminCommand(uint8 * Packet)
{
	POINTER_PCT(SL_ADMIN_COMMAND, lpMsg, Packet, 0);

	SL_ADMIN_COMMAND pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.type = lpMsg->type;
	pMsg.sub_type = lpMsg->sub_type;
	pMsg.entry = lpMsg->entry;
	pMsg.guid = lpMsg->guid;
	memcpy(pMsg.target_name, lpMsg->target_name, MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.target_account, lpMsg->target_account, MAX_ACCOUNT_LENGTH + 1);
	pMsg.time = lpMsg->time;
	pMsg.result = 0;

	uint32 target_guid = 0;

	if ( lpMsg->type == 0 ||
		 lpMsg->type == 1 ||
		 lpMsg->type == 2 ||
		 lpMsg->type == 3 )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_SELECT);
		stmt->setString(0, lpMsg->target_name);

		PreparedQueryResult result = MuDatabase.Query(stmt);

		if ( result )
		{
			Field* fields = result->Fetch();
			target_guid = fields[0].GetUInt32();
		}

		if ( !target_guid )
		{
			pMsg.result = 1;
		}
	}

	if ( !pMsg.result )
	{
		switch ( lpMsg->type )
		{
		case 0:
			{
				if ( lpMsg->time > 0 )
				{
					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_KICK);
					stmt->setInt64(0, lpMsg->time + ::time(nullptr));
					stmt->setUInt32(1, target_guid);
					MuDatabase.Execute(stmt);
				}
			} break;

		case 1:
			{
				if ( lpMsg->time > 0 )
				{
					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_RESTRICTION);
					stmt->setUInt32(0, target_guid);
					stmt->setUInt32(1, lpMsg->sub_type);
					stmt->setInt64(2, lpMsg->time + ::time(nullptr));
					stmt->setString(3, "");
					stmt->setString(4, "");
					MuDatabase.Execute(stmt);
				}
			} break;

		case 2:
			{
				if ( lpMsg->time > 0 )
				{
					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_MUTE);
					stmt->setInt64(0, lpMsg->time + ::time(nullptr));
					stmt->setUInt32(1, target_guid);
					MuDatabase.Execute(stmt);
				}
			} break;

		case 3:
			{
				PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_PENALIZATION_CHARACTER_BAN);
				stmt->setUInt8(0, 1);
				stmt->setUInt32(1, target_guid);
				MuDatabase.Execute(stmt);
			} break;
		}
	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void ServerSocket::AdminCommandLocation(uint8 * Packet)
{
	POINTER_PCT(SL_ADMIN_COMMAND_LOCATION, lpMsg, Packet, 0);

	SL_ADMIN_COMMAND_LOCATION pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.type = lpMsg->type;
	pMsg.entry = lpMsg->entry;
	pMsg.guid = lpMsg->guid;
	memcpy(pMsg.target_name, lpMsg->target_name, MAX_CHARACTER_LENGTH + 1);
	pMsg.world = lpMsg->world;
	pMsg.x = lpMsg->x;
	pMsg.y = lpMsg->y;
	pMsg.direction = lpMsg->direction;
	pMsg.server = lpMsg->server;
	pMsg.online = lpMsg->online;
	pMsg.result = 0;

	uint32 target_guid = 0;

	if ( lpMsg->type == 0 ||
		 lpMsg->type == 1 ||
		 lpMsg->type == 2 )
	{

	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void ServerSocket::CrywolfDataRequest(uint8 * Packet)
{
	sCrywolf->GetStatus(shared_from_this());	
}
	
void ServerSocket::CrywolfDataSave(uint8 * Packet)
{
	sCrywolf->UpdateStatus(Packet);
}

void ServerSocket::GuildMatchingList(uint8 * Packet)
{
	sGuildMatching->GuildMatchingListRecv(Packet, shared_from_this());
}

void ServerSocket::GuildMatchingListSearch(uint8 * Packet)
{
	sGuildMatching->GuildMatchingListSearchRecv(Packet, shared_from_this());
}
	
void ServerSocket::GuildMatchingInsert(uint8 * Packet)
{
	sGuildMatching->GuildMatchingInsertRecv(Packet, shared_from_this());
}
	
void ServerSocket::GuildMatchingCancel(uint8 * Packet)
{
	sGuildMatching->GuildMatchingCancelRecv(Packet, shared_from_this());
}
		
void ServerSocket::GuildMatchingJoinInsert(uint8 * Packet)
{
	sGuildMatching->GuildMatchingJoinInsertRecv(Packet, shared_from_this());
}

void ServerSocket::GuildMatchingJoinCancel(uint8 * Packet)
{
	sGuildMatching->GuildMatchingJoinCancelRecv(Packet, shared_from_this());
}
		
void ServerSocket::GuildMatchingJoinAccept(uint8 * Packet)
{
	sGuildMatching->GuildMatchingJoinAcceptRecv(Packet, shared_from_this());
}
	
void ServerSocket::GuildMatchingJoinList(uint8 * Packet)
{
	sGuildMatching->GuildMatchingJoinListRecv(Packet, shared_from_this());
}

void ServerSocket::GuildMatchingJoinInfo(uint8 * Packet)
{
	sGuildMatching->GuildMatchingJoinInfoRecv(Packet, shared_from_this());
}
	
void ServerSocket::GuildMatchingInsertSave(uint8 * Packet)
{
	sGuildMatching->GuildMatchingInsertSaveRecv(Packet, shared_from_this());
}

void ServerSocket::EventStateUpdate(uint8 * Packet)
{
	POINTER_PCT(SL_EVENT_STATE_UPDATE, lpMsg, Packet, 0);

	EventState & EventStateData = EventStateMap[lpMsg->event_id];
	EventStateData.SetState(lpMsg->state);
	EventStateData.SetOccupationState(lpMsg->occupation_state);

	this->HandleHeadcodeCommon(Packet);
}

void ServerSocket::ArkaWarMasterRegister(uint8 * Packet)
{
	sArkaWar->MasterRegister(Packet, shared_from_this());
}
	
void ServerSocket::ArkaWarMemberRegister(uint8 * Packet)
{
	sArkaWar->MemberRegister(Packet, shared_from_this());
}
	
void ServerSocket::ArkaWarSignRegister(uint8 * Packet)
{
	sArkaWar->SignRegister(Packet, shared_from_this());
}
	
void ServerSocket::ArkaWarEnter(uint8 * Packet)
{
	sArkaWar->Enter(Packet, shared_from_this());
}
	
void ServerSocket::ArkaWarList(uint8 * Packet)
{
	sArkaWar->GetGuildList(Packet, shared_from_this());
	sArkaWar->GetMemberList(shared_from_this());
}
	
void ServerSocket::ArkaWarClear(uint8 * Packet)
{
	sArkaWar->Clear();
}

void ServerSocket::ArkaWarMemberCount(uint8 * Packet)
{
	sArkaWar->MemberCount(Packet, shared_from_this());
}

void ServerSocket::ArkaWarGuildRequest(uint8 * Packet)
{
	sArkaWar->GuildRequest(Packet, shared_from_this());
}
	
void ServerSocket::ArkaWarGuildSave(uint8 * Packet)
{
	sArkaWar->GuildSave(Packet);
}

void ServerSocket::ArkaWarSignRequest(uint8 * Packet)
{
	sArkaWar->SignRequest(Packet, shared_from_this());
}

void ServerSocket::CharacterSignal(uint8 * Packet)
{
	this->HandleHeadcodeCommon(Packet);
}