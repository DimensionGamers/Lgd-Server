/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "DevilSquare.cpp"
*
*/

DevilSquare::DevilSquare(): EventGroundCommon("DevilSquare", LOG_DEVIL_SQUARE, DS_MAP_RANGE, EVENT_DEVIL_SQUARE)
{
	this->InitRanking();
}

DevilSquare::~DevilSquare()
{
	this->EraseRanking();
}

void DevilSquare::Update()
{
	switch(this->GetState())
	{
	case DEVIL_SQUARE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case DEVIL_SQUARE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case DEVIL_SQUARE_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case DEVIL_SQUARE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case DEVIL_SQUARE_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

void DevilSquare::SetState_None()
{
	this->SetState(DEVIL_SQUARE_STATE_NONE);
	this->MovePlayersOut(DEVIL_SQUARE_GATE);
	this->EraseRanking();
	this->InitMessageSent();
	this->SetDuration(0);
	this->RunTime(0);
	this->ClearMonster();

	LOG_EVENT_STATE();
}

void DevilSquare::SetState_Open()
{
	this->InitPlayers<DevilSquarePlayer>(this->GetSettings()->GetMaxPlayers());

	this->SetState(DEVIL_SQUARE_STATE_OPEN);
	this->RunTime(0);
	
	this->CreateRanking(this->GetSettings()->GetMaxPlayers());

	for ( uint32 i = 0; i < this->GetRankingSize(); ++i )
	{
		this->GetRanking(i)->Reset();
	}

	LOG_EVENT_STATE();
}

void DevilSquare::SetState_StandBy()
{
	this->SetState(DEVIL_SQUARE_STATE_STANDBY);
	this->RunTime(0);

	LOG_EVENT_STATE();
}

void DevilSquare::SetState_Playing()
{
	if ( this->GetPlayersCount(true) < 1 )
	{
		this->SetState_None();
		return;
	}

	this->SetState(DEVIL_SQUARE_STATE_PLAYING);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);
	this->AddMonster();

	LOG_EVENT_STATE();
}

void DevilSquare::SetState_Playend()
{
	this->SetState(DEVIL_SQUARE_STATE_PLAYEND);
	this->RunTime(DEVIL_SQUARE_END_TIME * MINUTE * IN_MILLISECONDS);
	this->CalculatePlayerScore();
	this->ClearMonster();

	LOG_EVENT_STATE();
}

void DevilSquare::ProcState_None()
{

}

void DevilSquare::ProcState_Open()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer(false);
}

void DevilSquare::ProcState_StandBy()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer();
	
	if ( this->TimePassed() )
		this->SetState_Playing();
}

void DevilSquare::ProcState_Playing()
{
	if ( this->SecondPassed() )
	{
		this->UpdatePlayerStatus();

		if ( this->GetPlayersCount(true) < 1 )
		{
			this->SetState_None();
			return;
		}
	}

	this->SendRemainMinutes();
	this->SendMessageID(EVENT_MESSAGE_END_DEVIL_SQUARE);

	if ( this->TimePassed() )
		this->SetState_Playend();
}

void DevilSquare::ProcState_Playend()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer();

	if ( this->TimePassed() )
		this->SetState_None();
}

void DevilSquare::UpdatePlayerStatus()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			this->Log("Removed Player %s : Disconnected.", pPlayer->GetName());

			this->GetPlayer(i)->SetPlayer(nullptr);
			continue;
		}

		if ( !pPlayer->IsLive() )
		{
			this->Log("Removed Player %s : Died.", pPlayer->GetName());

			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			continue;

		if ( !DS_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			this->Log("Removed Player %s : Not in event.", pPlayer->GetName());

			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Failed. Your score is %d. Try again next time", this->GetPlayer(i)->ToDS()->GetScore());
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}
	}
}

void DevilSquare::SendRemainMinutes()
{
	if ( !this->IsMinuteElapsed(5) )
		return;

	if ( this->GetCurrentMinute() <= 0 )
		return;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendNotice(NOTICE_GLOBAL,"[Devil Square] will close after %d minute(s)", this->GetCurrentMinute());
	}
}

bool Sort(DevilSquarePlayer* pPlayer1, DevilSquarePlayer* pPlayer2)
{
	if ( !pPlayer1 || !pPlayer1->GetPlayer() )
		return false;

	if ( !pPlayer2 || !pPlayer2->GetPlayer() )
		return true;
	
	if ( pPlayer1->GetScore() == pPlayer2->GetScore() )
	{
		return (pPlayer1->GetPlayer()->GetTotalLevel() > pPlayer2->GetPlayer()->GetTotalLevel());
	}

	return (pPlayer1->GetScore() > pPlayer2->GetScore());
}

void DevilSquare::BubbleSort()
{
	std::vector<DevilSquarePlayer*> player_list;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		player_list.push_back(this->GetPlayer(i)->ToDS());

		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( pPlayer )
		{
			pPlayer->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_DEVIL_SQUARE_CLEAR);
			pPlayer->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_DEVIL_SQUARE_POINTS, nullptr, this->GetPlayer(i)->ToDS()->GetScore());
		}
	}

	std::sort(player_list.begin(), player_list.end(), Sort);

	for ( int32 i = 0; i < player_list.size(); i++ )
	{
		this->SetRanking(i, *player_list[i]);
	}
}

void DevilSquare::CalculatePlayerScore()
{
	this->BubbleSort();

	uint8 buffer[3000];
	POINTER_PCT(DEVIL_SQUARE_SCORE_HEAD, head, buffer, 0);
	POINTER_PCT(DEVIL_SQUARE_SCORE_BODY, body, buffer, sizeof(DEVIL_SQUARE_SCORE_HEAD));
	uint16 size = sizeof(DEVIL_SQUARE_SCORE_HEAD) + sizeof(DEVIL_SQUARE_SCORE_BODY);
	uint8 count = 1;
	uint8 rank = 0;

	for ( uint32 i = 0; i < this->GetRankingSize(); ++i )
	{
		Player* pPlayer = this->GetRanking(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		memcpy(body[count].name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		body[count].score = this->GetRanking(i)->GetScore();
		body[count].exp = 0;
		body[count].zen = 0;

		if ( devil_square_ranking const* ranking_ = this->GetRankingData(rank) )
		{
			body[count].exp = ranking_->experience.get();
			body[count].zen = ranking_->zen.get();
		}

		this->Log("Ranking (%d - %s): Score: %d / Experience: %d / Zen: %d",
			count, pPlayer->GetName(), body[count].score, body[count].exp, body[count].zen);

		rank++;
		count++;
		size += sizeof(DEVIL_SQUARE_SCORE_BODY);

		if ( count >= MAX_DEVIL_SQUARE_RANK )
			break;
	}

	head->h.c = 0xC1;
	head->h.headcode = 0x93;
	head->h.size = size;
	head->count = count;

	rank = 0;

	for ( uint32 i = 0; i < this->GetRankingSize(); ++i )
	{
		Player* pPlayer = this->GetRanking(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		head->my_rank = rank + 1;

		memcpy(body[0].name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		body[0].score = this->GetRanking(i)->GetScore();
		body[0].exp = 0;
		body[0].zen = 0;
		
		if ( devil_square_ranking const* ranking_ = this->GetRankingData(rank) )
		{
			body[0].exp = ranking_->experience.get();
			body[0].zen = ranking_->zen.get();
		}

		rank++;

		pPlayer->UpdateStatistic(STATISTIC_DEVIL_SQUARE_POINTS, this->GetRanking(i)->GetScore());
		pPlayer->MoneyAdd(body[0].zen);

		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
		{
			body[0].exp = (body[0].exp * sGameServer->GetCrywolfPenaltyExperienceGain()) / 100;
		}

		int64 experience = body[0].exp;

		pPlayer->GiveExperience(experience, true, true);

		pPlayer->sendPacket(buffer,size);

		sEventMgr->GiveContributionReward(pPlayer, EVENT_DEVIL_SQUARE, this->GetGround() + 1, head->my_rank, GremoryCaseReward::DEVIL_SQUARE);

		sEventMgr->AddEventScore(pPlayer->GetGUID(), pPlayer->GetName(), EVENT_DEVIL_SQUARE, this->GetGround(), this->GetRanking(i)->GetScore());
	}
}

devil_square_ranking * DevilSquare::GetRankingData(uint8 position) const
{
	DevilSquareRankingMap::const_iterator it = this->ranking.find(position);

	return (it != this->ranking.end())?it->second:nullptr;
}

void DevilSquare::AddMonster()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_DEVIL_SQUARE )
		{
			continue;
		}

		if ( (*it)->devil_square.ground != this->GetGround() )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetDespawnTime((*it)->devil_square.despawn_time * IN_MILLISECONDS);
			pMonster->SetDespawnTick(MyGetTickCount());
			pMonster->SetDespawnType(MONSTER_DESPAWN_DIE);

			if ( (*it)->devil_square.boss )
			{
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			}

			pMonster->SetEventGround(this->GetGround());
			pMonster->AddToWorld();

			this->Log("Added Monster [%u][%u]", pMonster->GetEntry(), pMonster->GetClass());
		}
	}
}

void DevilSquare::ClearMonster()
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster )
			continue;

		if ( !pMonster->IsPlaying() )
			continue;

		if ( pMonster->GetEventId() != EVENT_DEVIL_SQUARE )
			continue;

		if ( pMonster->GetEventGround() != this->GetGround() )
			continue;

		pMonster->Remove();
	}

	this->Log("Clearing Monsters");
}

DevilSquarePlayer * DevilSquare::FindPlayer(Player* pPlayer)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( this->GetPlayer(i)->GetPlayer() != pPlayer )
			continue;

		return this->GetPlayer(i)->ToDS();
	}

	return nullptr;
}

DevilSquareMgr::DevilSquareMgr(): EventCommonMgr(MAX_DEVIL_SQUARE_GROUND)
{
	FOR_EACH_GROUND(DEVIL_SQUARE)->SetGround(i);
	FOR_EACH_GROUND(DEVIL_SQUARE)->SetState_None();
	this->SetState_None();
}

DevilSquareMgr::~DevilSquareMgr()
{
	for ( uint8 i = 0; i < MAX_DEVIL_SQUARE_GROUND; ++i )
	{
		for ( DevilSquareRankingMap::iterator it = this->GetGround(i)->ranking.begin(); it != this->GetGround(i)->ranking.end(); it++ )
		{
			delete it->second;
		}

		this->GetGround(i)->ranking.clear();
	}
}

void DevilSquareMgr::LoadDevilSquareSetting()
{
	if ( !sGameServer->IsDevilSquareEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Devil Square Settings...");

	ReadScript file("EventData/DevilSquare.txt", "rb");

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
					uint8 ground = file.GetUInt8() - 1;

					file.GetToken();	int32 experience = file.GetInt32();
					file.GetToken();	int32 max_players = file.GetInt32();
					file.GetToken();	bool pk_check = file.GetBool();
					file.GetToken();	uint8 pk_level_max = file.GetUInt8();
					file.GetToken();	bool remove_elf_soldier_buff = file.GetBool();

					if ( ground < MAX_DEVIL_SQUARE_GROUND )
					{
						this->GetGround(ground)->GetSettings()->SetExperience(experience);
						this->GetGround(ground)->GetSettings()->SetMaxPlayers(max_players);
						this->GetGround(ground)->GetSettings()->SetPKCheck(pk_check);
						this->GetGround(ground)->GetSettings()->SetPKLevelMax(pk_level_max);
						this->GetGround(ground)->GetSettings()->SetRemoveElfSoldierBuff(remove_elf_soldier_buff);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Devil Square Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void DevilSquareMgr::LoadDevilSquareRanking()
{
	for ( uint8 i = 0; i < MAX_DEVIL_SQUARE_GROUND; i++ )
	{
		for ( DevilSquareRankingMap::iterator it = this->GetGround(i)->ranking.begin(); it != this->GetGround(i)->ranking.end(); it++ )
		{
			delete it->second;
		}

		this->GetGround(i)->ranking.clear();
	}

	if ( !sGameServer->IsDevilSquareEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Devil Square Ranking...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM event_devil_square_ranking");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 ground = fields[0].GetUInt8();
			uint8 position = fields[1].GetUInt8();
		
			if ( ground < MAX_DEVIL_SQUARE_GROUND )
			{
				devil_square_ranking * add_ranking = new devil_square_ranking;

				add_ranking->experience.set(fields[2].GetInt32());
				add_ranking->zen.set(fields[3].GetUInt32());

				this->GetGround(ground)->ranking[position] = add_ranking;
			}

			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u devil square ranking definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void DevilSquareMgr::StartGround(uint32 duration)
{
	if ( !sGameServer->IsDevilSquareEnabled() )
		return;

	FOR_EACH_GROUND(DEVIL_SQUARE)->SetDuration(duration);
}

void DevilSquareMgr::Update()
{
	FOR_EACH_GROUND(DEVIL_SQUARE)->UpdateTime();
}

void DevilSquareMgr::SetState_None()
{
	this->SetState(EVENT_MGR_STATE_NONE);
	this->SetMessageSent(EVENT_MESSAGE_OPEN_DEVIL_SQUARE, false);
	this->RunTime(0);

	sLog->outInfo(LOG_DEVIL_SQUARE,"[DevilSquareMgr] SetState_None()");
}
		
void DevilSquareMgr::SetState_Notify()
{
	this->SetState(EVENT_MGR_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_DEVIL_SQUARE,"[DevilSquareMgr] SetState_Notify()");
}
		
void DevilSquareMgr::SetState_Open()
{
	FOR_EACH_GROUND(DEVIL_SQUARE)->SetState_Open();

	this->SetState(EVENT_MGR_STATE_OPEN);
	this->RunTime(EVENT_MGR_OPEN_TIME * MINUTE * IN_MILLISECONDS);
	this->SendOpen();

	sLog->outInfo(LOG_DEVIL_SQUARE,"[DevilSquareMgr] SetState_Open()");
}
		
void DevilSquareMgr::SetState_Playing()
{
	this->SetState(EVENT_MGR_STATE_PLAYING);
	this->RunTime(0);
	this->SendStart();

	FOR_EACH_GROUND(DEVIL_SQUARE)->SetState_StandBy();

	sLog->outInfo(LOG_DEVIL_SQUARE,"[DevilSquareMgr] SetState_Playing()");
}

void DevilSquareMgr::ProcState_None()
{

}
	
void DevilSquareMgr::ProcState_Notify()
{
	this->SendRemainMinutes("%d minute(s) left before Devil Square opens", 5, 15);

	if ( this->TimePassed() )
		this->SetState_Open();
}
		
void DevilSquareMgr::ProcState_Open()
{
	if ( this->GetNotificationTime()->Elapsed(EVENT_NOTIFICATION_TIME) )
	{
		sServerLink->EventNotification(EVENT_NOTIFICATION_DEVIL_SQUARE, 1);
	}

	this->SendRemainMinutes("[Devil Square] will start after %d minute(s)", 1, 4);

	this->SendMessageID(EVENT_MESSAGE_OPEN_DEVIL_SQUARE);

	if ( this->TimePassed() )
		this->SetState_Playing();
}

void DevilSquareMgr::ProcState_Playing()
{
	for ( uint8 i = 0; i < MAX_DEVIL_SQUARE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != DEVIL_SQUARE_STATE_NONE )
			return;
	}

	this->SetState_None();
}

void DevilSquareMgr::SendOpen()
{
	sServerLink->EventNotification(EVENT_NOTIFICATION_DEVIL_SQUARE, 1);

	sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "[Devil Square] The gates of Devil Square are open!");
}

void DevilSquareMgr::SendStart()
{
	sServerLink->EventNotification(EVENT_NOTIFICATION_DEVIL_SQUARE, 0);
}

void DevilSquareMgr::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None && pPlayer->GetInterfaceState()->GetID() != InterfaceData::DevilSquareEntrance )
		return;

	POINTER_PCT_LOG(DEVIL_SQUARE_ENTER_REQUEST, lpMsg, Packet, 0);

	uint8 ground = lpMsg->ground;
	uint8 item_slot = lpMsg->item_slot - PLAYER_MAX_EQUIPMENT;

	if ( !use_inventory_range(item_slot) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( ground >= MAX_DEVIL_SQUARE_GROUND )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( this->GetState() != EVENT_MGR_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 0x02);
		return;
	}

	if ( !sEventMgr->CanEnterEvent(pPlayer, EVENT_DEVIL_SQUARE) )
	{
		this->EnterResult(pPlayer, 0x08);
		return;
	}

	if ( this->GetGround(ground)->GetSettings()->IsPKCheck() && (pPlayer->GetPKLevel() >= this->GetGround(ground)->GetSettings()->GetPKLevelMax()) )
	{
		this->EnterResult(pPlayer, 0x06);
		return;
	}

	int32 current_players = this->GetGround(ground)->GetPlayersCount(true);

	if ( current_players >= this->GetGround(ground)->GetPlayerSize() )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !this->EventCommonMgr::HaveTicket(pPlayer->GetInventory()->GetItem(item_slot), ITEMGET(14, 19), ITEMGET(13, 46), ground) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	uint8 real_ground = sEventMgr->GetEventLevel(EVENT_DEVIL_SQUARE, pPlayer);

	if ( real_ground >= MAX_DEVIL_SQUARE_GROUND )
	{
		this->EnterResult(pPlayer, 0x04);
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

	if ( !pPlayer->MoveToGate(g_DevilSquareData[ground].gate) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	this->ConsumeTicket(pPlayer, ITEMGET(14, 19), ITEMGET(13, 46), item_slot);

	if ( this->GetGround(ground)->GetSettings()->IsRemoveElfSoldierBuff() )
	{
		pPlayer->RemoveBuff(BUFF_ELF_SOLDIER);
	}

	sEventMgr->EventEnter(pPlayer, EVENT_DEVIL_SQUARE);
}
	
void DevilSquareMgr::EnterResult(Player* pPlayer, uint8 result)
{
	DEVIL_SQUARE_ENTER_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

bool DevilSquareMgr::IsIn(Player* pPlayer)
{
	if ( this->GetState() != EVENT_MGR_STATE_PLAYING &&
		 this->GetState() != EVENT_MGR_STATE_OPEN )
		return false;

	for ( uint8 i = 0; i < MAX_DEVIL_SQUARE_GROUND; i++ )
	{
		if ( this->GetGround(i)->GetState() == DEVIL_SQUARE_STATE_NONE ||
			 this->GetGround(i)->GetState() == DEVIL_SQUARE_STATE_PLAYEND )
			continue;

		if ( !this->GetGround(i)->IsIn(pPlayer) )
			continue;

		pPlayer->SetEventId(EVENT_DEVIL_SQUARE);
		pPlayer->SetEventGround(i);
		return true;
	}

	return false;
}