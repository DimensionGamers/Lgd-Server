/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "IllusionTemple.cpp"
*
*/

IllusionTemple::IllusionTemple(): EventGroundCommon("IllusionTemple", LOG_ILLUSION_TEMPLE, IT_MAP_RANGE, EVENT_ILLUSION_TEMPLE)
{
}

IllusionTemple::~IllusionTemple()
{
}

void IllusionTemple::Update()
{
	switch(this->GetState())
	{
	case ILLUSION_TEMPLE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case ILLUSION_TEMPLE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case ILLUSION_TEMPLE_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case ILLUSION_TEMPLE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case ILLUSION_TEMPLE_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

BUILD_STATE(IllusionTemple, Set, None)
{
	this->SetState(ILLUSION_TEMPLE_STATE_NONE);
	this->MovePlayersOut(267);
	this->InitMessageSent();
	this->SetDuration(0);
	this->RunTime(0);

	sLog->outInfo(this->GetLogFilter(), "[%s %d] %s", this->GetName().c_str(), this->GetGround() + 1, __FUNCTION__);
}

BUILD_STATE(IllusionTemple, Set, Open)
{
	this->InitPlayers<IllusionTemplePlayer>(this->GetSettings()->GetMaxPlayers());

	this->SetState(ILLUSION_TEMPLE_STATE_OPEN);
	this->RunTime(0);
	
	sLog->outInfo(this->GetLogFilter(), "[%s %d] %s", this->GetName().c_str(), this->GetGround() + 1, __FUNCTION__);
}

BUILD_STATE(IllusionTemple, Set, StandBy)
{
	this->SetState(ILLUSION_TEMPLE_STATE_STANDBY);
	this->RunTime(MINUTE * IN_MILLISECONDS);

	sLog->outInfo(this->GetLogFilter(), "[%s %d] %s", this->GetName().c_str(), this->GetGround() + 1, __FUNCTION__);
}

BUILD_STATE(IllusionTemple, Set, Playing)
{
	if ( this->GetPlayersCount(true) < this->GetSettings()->GetMinPlayers() )
	{
		this->SetState_None();
		return;
	}

	/*AddNpcStatus();
	AddNpcRelicsBox();
	AddMonsterGhost();
	SetStatusRegenTime();

	SendIllusionTempleState(2, -1);*/

	this->SetState(ILLUSION_TEMPLE_STATE_PLAYING);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(this->GetLogFilter(), "[%s %d] %s", this->GetName().c_str(), this->GetGround() + 1, __FUNCTION__);
}

BUILD_STATE(IllusionTemple, Set, Playend)
{
	this->SetState(ILLUSION_TEMPLE_STATE_PLAYEND);
	this->RunTime(MINUTE * IN_MILLISECONDS);

	sLog->outInfo(this->GetLogFilter(), "[%s %d] %s", this->GetName().c_str(), this->GetGround() + 1, __FUNCTION__);
}

BUILD_STATE(IllusionTemple, Proc, None)
{

}

BUILD_STATE(IllusionTemple, Proc, Open)
{
	this->UpdatePlayer();
}

BUILD_STATE(IllusionTemple, Proc, StandBy)
{
	this->UpdatePlayer();

	if ( this->TimePassed() )
		this->SetState_Playing();
}

BUILD_STATE(IllusionTemple, Proc, Playing)
{

}

BUILD_STATE(IllusionTemple, Proc, Playend)
{
	this->UpdatePlayer();

	if ( this->TimePassed() )
		this->SetState_None();
}





IllusionTempleMgr::IllusionTempleMgr(): EventCommonMgr(MAX_ILLUSION_TEMPLE_GROUND)
{
	FOR_EACH_GROUND(ILLUSION_TEMPLE)->SetGround(i);
	FOR_EACH_GROUND(ILLUSION_TEMPLE)->SetState_None();

	this->SetState_None();
}

IllusionTempleMgr::~IllusionTempleMgr()
{

}

void IllusionTempleMgr::LoadIllusionTempleSettings()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Illusion Temple Settings...");

	ReadScript file("EventData/IllusionTemple.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 min_players = file.GetInt32();
					file.GetToken();	int32 max_players = file.GetInt32();
					file.GetToken();	bool pk_check = file.GetBool();
					file.GetToken();	uint8 pk_level_max = file.GetUInt8();

					if ( id < MAX_ILLUSION_TEMPLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetMinPlayers(min_players * 2);
						this->GetGround(id)->GetSettings()->SetMaxPlayers(max_players * 2);
						this->GetGround(id)->GetSettings()->SetPKCheck(pk_check);
						this->GetGround(id)->GetSettings()->SetPKLevelMax(pk_level_max);
					}
				} break;

			case 1:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 default_experience = file.GetInt32();
					file.GetToken();	int32 relic_points_experience = file.GetInt32();
					file.GetToken();	int32 add_kill_points = file.GetInt32();
					file.GetToken();	int32 move_relics_experience = file.GetInt32();
					file.GetToken();	int32 party_user_points_1 = file.GetInt32();
					file.GetToken();	int32 party_user_points_2 = file.GetInt32();
					file.GetToken();	int32 party_user_points_3 = file.GetInt32();
					file.GetToken();	int32 party_user_points_4 = file.GetInt32();
					file.GetToken();	int32 party_user_points_5 = file.GetInt32();

					if ( id < MAX_ILLUSION_TEMPLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetDefaultExperience(default_experience);
						this->GetGround(id)->GetSettings()->SetRelicPointsExperience(relic_points_experience);
						this->GetGround(id)->GetSettings()->SetAddKillPoints(add_kill_points);
						this->GetGround(id)->GetSettings()->SetMoveRelicsExperience(move_relics_experience);
						this->GetGround(id)->GetSettings()->SetPartyUserPoints(0, party_user_points_1);
						this->GetGround(id)->GetSettings()->SetPartyUserPoints(1, party_user_points_2);
						this->GetGround(id)->GetSettings()->SetPartyUserPoints(2, party_user_points_3);
						this->GetGround(id)->GetSettings()->SetPartyUserPoints(3, party_user_points_4);
						this->GetGround(id)->GetSettings()->SetPartyUserPoints(4, party_user_points_5);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Illusion Temple Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void IllusionTempleMgr::StartGround(uint32 duration)
{
	FOR_EACH_GROUND(ILLUSION_TEMPLE)->SetDuration(duration);
}

void IllusionTempleMgr::Update()
{
	FOR_EACH_GROUND(ILLUSION_TEMPLE)->UpdateTime();
}

BUILD_STATE(IllusionTempleMgr, Set, None)
{
	this->SetState(EVENT_MGR_STATE_NONE);
}

BUILD_STATE(IllusionTempleMgr, Set, Notify)
{
	this->SetState(EVENT_MGR_STATE_NOTIFY);
}

BUILD_STATE(IllusionTempleMgr, Set, Open)
{
	this->SetState(EVENT_MGR_STATE_OPEN);
}

BUILD_STATE(IllusionTempleMgr, Set, Playing)
{
	this->SetState(EVENT_MGR_STATE_PLAYING);
}

BUILD_STATE(IllusionTempleMgr, Proc, None)
{

}

BUILD_STATE(IllusionTempleMgr, Proc, Notify)
{

}

BUILD_STATE(IllusionTempleMgr, Proc, Open)
{

}

BUILD_STATE(IllusionTempleMgr, Proc, Playing)
{

}

void IllusionTempleMgr::EnterRequest(Player * pPlayer, uint8 * Packet)
{
	if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() )
		return;

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None && pPlayer->GetInterfaceState()->GetID() != InterfaceData::IllusionTempleEntrance )
		return;

	POINTER_PCT_LOG(ILLUSION_TEMPLE_ENTER_REQUEST, lpMsg, Packet, 0);

	uint8 ground = lpMsg->ground - 1;
	uint8 item_slot = lpMsg->slot;

	if ( !use_inventory_range(item_slot) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( ground >= MAX_ILLUSION_TEMPLE_GROUND )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( (pPlayer->GetPKLevel() >= this->GetGround(ground)->GetSettings()->GetPKLevelMax()) && this->GetGround(ground)->GetSettings()->IsPKCheck() )
	{
		this->EnterResult(pPlayer, 0x07);
		return;
	}

	if ( pPlayer->GetInventory()->GetItem(RING_01)->IsTransformationRing() || 
		 pPlayer->GetInventory()->GetItem(RING_02)->IsTransformationRing() )
	{
		this->EnterResult(pPlayer, 0x08);
		return;
	}

	if ( this->GetState() != EVENT_MGR_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 0x02);
		return;
	}

	int32 current_players = this->GetGround(ground)->GetPlayersCount(true);

	if ( current_players >= this->GetGround(ground)->GetPlayerSize() )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !this->EventCommonMgr::HaveTicket(pPlayer->GetInventory()->GetItem(item_slot), ITEMGET(13, 51), ITEMGET(13, 61), ground) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	uint8 real_ground = sEventMgr->GetEventLevel(EVENT_ILLUSION_TEMPLE, pPlayer);

	if ( real_ground >= MAX_ILLUSION_TEMPLE_GROUND )
	{
		this->EnterResult(pPlayer, 0x03);
		return;
	}

	if ( real_ground != ground )
	{
		if ( ground < real_ground )
		{
			this->EnterResult(pPlayer, 0x03);
			return;
		}

		if ( ground > real_ground )
		{
			this->EnterResult(pPlayer, 0x04);
			return;
		}
	}

	pPlayer->GetInterfaceState()->Reset();

	if ( !this->GetGround(ground)->AddPlayer(pPlayer) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( !pPlayer->MoveToGate(m_IllusionTempleData[ground].gate) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	this->ConsumeTicket(pPlayer, ITEMGET(13, 51), ITEMGET(13, 61), item_slot);
	this->EnterResult(pPlayer, 0x00);
}
	
void IllusionTempleMgr::EnterResult(Player * pPlayer, uint8 result)
{
	ILLUSION_TEMPLE_ENTER_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}
	
bool IllusionTempleMgr::IsIn(Player* pPlayer)
{
	if ( this->GetState() != EVENT_MGR_STATE_PLAYING &&
		 this->GetState() != EVENT_MGR_STATE_OPEN )
		return false;

	for ( uint8 i = 0; i < MAX_ILLUSION_TEMPLE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() == ILLUSION_TEMPLE_STATE_NONE ||
			 this->GetGround(i)->GetState() == ILLUSION_TEMPLE_STATE_PLAYEND )
			continue;

		if ( !this->GetGround(i)->IsIn(pPlayer) )
			continue;

		pPlayer->SetEventId(EVENT_ILLUSION_TEMPLE);
		pPlayer->SetEventGround(i);
		return true;
	}

	return false;
}