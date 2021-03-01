/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChaosCastleSurvival.cpp"
*
*/

ChaosCastleSurvivalMgr::ChaosCastleSurvivalMgr(): EventCommonMgr(MAX_CHAOS_CASTLE_SURVIVAL_STAGE)
{
	this->GetGround()->SetGround(0);
	this->GetGround()->SetName("ChaosCastleSurvival");
	this->GetGround()->SetEvent(EVENT_CHAOS_CASTLE_SURVIVAL);
	this->GetGround()->SetEnterGate(443);
	this->GetGround()->SetRespawnGate(CHAOS_CASTLE_GATE);
	this->GetGround()->SetWorldID(WORLD_CHAOS_CASTLE_SURVIVAL);
	this->GetGround()->SetState_None();

	this->SetStage(CHAOS_CASTLE_SURVIVAL_STAGE_NONE);
	this->SetState_None();
}

ChaosCastleSurvivalMgr::~ChaosCastleSurvivalMgr()
{
	this->GetGround()->item_list.clear();

	for ( int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_STAGE; ++i )
	{
		LIST_CLEAR(ChaosCastleSurvivalScheduleList::iterator, this->m_StageTime[i]);
	}
}

void ChaosCastleSurvivalMgr::LoadChaosCastleSetting()
{
	for ( int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_STAGE; ++i )
	{
		LIST_CLEAR(ChaosCastleSurvivalScheduleList::iterator, this->m_StageTime[i]);
	}

	if ( !sGameServer->IsChaosCastleSurvivalOfTheFittestEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Chaos Castle Survival Settings...");

	ReadScript file("EventData/ChaosCastleSurvival.txt", "rb");

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
					int32 min_players = file.GetInt32();
					file.GetToken();	int32 max_players = file.GetInt32();
					file.GetToken();	uint32 required_zen = file.GetUInt32();
					file.GetToken();	bool pk_check = file.GetBool();
					file.GetToken();	uint8 pk_level_max = file.GetUInt8();
					file.GetToken();	int32 experience_user = file.GetInt32();
					file.GetToken();	int32 experience_monster = file.GetInt32();
					file.GetToken();	bool elf_soldier = file.GetBool();

					this->GetGround()->GetSettings()->SetMinPlayers(min_players);
					this->GetGround()->GetSettings()->SetMaxPlayers(max_players);
					this->GetGround()->GetSettings()->SetRequiredZen(required_zen);
					this->GetGround()->GetSettings()->SetPKCheck(pk_check);
					this->GetGround()->GetSettings()->SetPKLevelMax(pk_level_max);
					this->GetGround()->GetSettings()->SetExperienceUser(experience_user);
					this->GetGround()->GetSettings()->SetExperienceMonster(experience_monster);
					this->GetGround()->GetSettings()->SetRemoveElfSoldierBuff(elf_soldier);
				} break;

			case 1:
				{
					this->SetPreliminary(0, file.GetUInt8());

					file.GetToken();	this->SetPreliminary(1, file.GetUInt8());
					file.GetToken();	this->SetPreliminary(2, file.GetUInt8());
					file.GetToken();	this->SetSemiFinal(0, file.GetUInt8());
					file.GetToken();	this->SetSemiFinal(1, file.GetUInt8());
					file.GetToken();	this->SetFinal(file.GetUInt8());
				} break;

			case 2:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	uint8 hour = file.GetUInt8();
					file.GetToken();	uint8 minute = file.GetUInt8();

					if ( id < MAX_CHAOS_CASTLE_SURVIVAL_STAGE )
					{
						ChaosCastleSurvivalSchedule* pData = new ChaosCastleSurvivalSchedule;
						pData->SetHour(hour);
						pData->SetMinute(minute);

						this->m_StageTime[id].push_back(pData);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Chaos Castle Survival Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ChaosCastleSurvivalMgr::Start(uint32 duration, int32 notify)
{
	if ( this->GetStage() != CHAOS_CASTLE_SURVIVAL_STAGE_PRELIMINARY &&
		 this->GetStage() != CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL &&
		 this->GetStage() != CHAOS_CASTLE_SURVIVAL_STAGE_FINAL )
	{
		return;
	}

	EventCommonMgr::Start(duration, notify);
}

void ChaosCastleSurvivalMgr::StartGround(uint32 duration)
{
	if ( !sGameServer->IsChaosCastleSurvivalOfTheFittestEnabled() )
	{
		return;
	}

	this->GetGround()->SetDuration(duration);
}

void ChaosCastleSurvivalMgr::Update()
{
	this->GetGround()->UpdateTime();
}

void ChaosCastleSurvivalMgr::SetState_None()
{
	this->SetState(EVENT_MGR_STATE_NONE);
	this->SetMessageSent(EVENT_MESSAGE_OPEN_CHAOS_CASTLE_SURVIVAL, false);
	this->RunTime(HOUR * IN_MILLISECONDS);
	
	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleSurvivalMgr] SetState_None()");
}
		
void ChaosCastleSurvivalMgr::SetState_Notify()
{
	this->SetState(EVENT_MGR_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleSurvivalMgr] SetState_Notify()");
}
		
void ChaosCastleSurvivalMgr::SetState_Open()
{
	this->GetGround()->SetState_Open();

	this->SetState(EVENT_MGR_STATE_OPEN);
	this->RunTime(EVENT_MGR_OPEN_TIME * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleSurvivalMgr] SetState_Open()");
}
		
void ChaosCastleSurvivalMgr::SetState_Playing()
{
	this->SetState(EVENT_MGR_STATE_PLAYING);
	this->RunTime(0);
	this->GetGround()->SetState_StandBy();

	sLog->outInfo(LOG_CHAOS_CASTLE,"[ChaosCastleSurvivalMgr] SetState_Playing()");
}

void ChaosCastleSurvivalMgr::ProcState_None()
{
	if ( this->TimePassed(true) || (this->GetStage() != this->GetDay()) )
	{
		this->UpdateRanking();
	}
}
	
void ChaosCastleSurvivalMgr::ProcState_Notify()
{
	if ( this->TimePassed() )
		this->SetState_Open();
}
		
void ChaosCastleSurvivalMgr::ProcState_Open()
{
	this->SendMessageID(EVENT_MESSAGE_OPEN_CHAOS_CASTLE_SURVIVAL);

	this->SendRemainMinutes("%d minutes left before Chaos Castle Survival of the Fittest starts!", 1, 5);

	if ( this->TimePassed() )
		this->SetState_Playing();
}

void ChaosCastleSurvivalMgr::ProcState_Playing()
{
	if ( this->GetGround()->GetState() != CHAOS_CASTLE_STATE_NONE )
	{
		return;
	}

	sChaosCastleSurvivalMgr->UpdateRanking();

	this->SetState_None();
}

void ChaosCastleSurvivalMgr::DataRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_SCHEDULE_HEAD, head, buffer, 0);
	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_SCHEDULE_BODY, body, buffer, sizeof(CHAOS_CASTLE_SURVIVAL_SCHEDULE_HEAD));
	head->count = 0;
	head->type = this->GetStage();
	head->preliminary[0] = this->GetPreliminary(0);
	head->preliminary[1] = this->GetPreliminary(1);
	head->preliminary[2] = this->GetPreliminary(2);
	head->semi_final[0] = this->GetSemiFinal(0);
	head->semi_final[1] = this->GetSemiFinal(1);
	head->final = this->GetFinal();

	for ( int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_STAGE; ++i )
	{
		for ( ChaosCastleSurvivalScheduleList::const_iterator it = this->m_StageTime[i].begin(); it != this->m_StageTime[i].end(); ++it )
		{
			ChaosCastleSurvivalSchedule const* pData = (*it);

			if ( !pData )
			{
				continue;
			}

			body[head->count].hour = pData->GetHour();
			body[head->count].minute = pData->GetMinute();
			body[head->count].preliminary = i == 0 ? 1 : 0;
			body[head->count].semi_final = i == 1 ? 2 : 0;
			body[head->count].final = i == 2 ? 3 : 0;

			++head->count;
		}
	}

	head->h.set(HEADCODE_CHAOS_CASTLE, SUBCODE_CHAOS_CASTLE_SURVIVAL_DATA, sizeof(CHAOS_CASTLE_SURVIVAL_SCHEDULE_HEAD) + (head->count * sizeof(CHAOS_CASTLE_SURVIVAL_SCHEDULE_BODY)));

	pPlayer->sendPacket(buffer, head->h.get_size());

	pPlayer->GetInterfaceState()->Set(InterfaceData::ChaosCastleSurvivalUI, nullptr);
}

void ChaosCastleSurvivalMgr::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ChaosCastleSurvivalUI, 0) )
	{
		this->EnterResult(pPlayer, 0x06);
		return;
	}

	/*
	result 0 -> OK
	result 1 -> Don't have required level
	result 2 -> After %d minutes
	result 3 -> Can't participate because ranking is not enught
	result 4 -> you canceled your entry
	result 5 -> there are too many players
	result 6 -> you cannot use this
	result 7 -> you are short of zen
	result 8 -> can't enter PK
	result 9 -> you can't use this
	result 10 -> this is not your day to enter
	result 11 -> you cannot enter survival of the fittest from this area
	result 12 -> cannot use this
	result 13 -> cannot use this
	result 14 -> not PK
	result 15 -> cannot enter during battle
	result 16 -> cannot enter during guild war
	result 17 -> cannot enter using mount
	result 18 -> cannot enter using transformation ring
	result 19 -> cant enter from this area
	*/

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		this->EnterResult(pPlayer, 0x0B);
		return;
	}

	if ( !pPlayer->IsInSafeZone() )
	{
		this->EnterResult(pPlayer, 0x13);
		return;
	}

	if ( this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_NONE )
	{
		this->EnterResult(pPlayer, 0x0A);
		return;
	}

	if ( this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL ||
		 this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_FINAL )
	{
		if ( !this->IsInRanking(pPlayer) )
		{
			this->EnterResult(pPlayer, 0x03);
			return;
		}
	}

	if ( this->GetState() == EVENT_MGR_STATE_PLAYING )
	{
		this->EnterResult(pPlayer, 0x0F);
		return;
	}

	if ( this->GetState() != EVENT_MGR_STATE_OPEN || !CHAOS_CASTLE_SURVIVAL_GROUND_RANGE(this->GetCurrentGround()) )
	{
		this->EnterResult(pPlayer, 0x02, sEventMgr->GetNextRemainTime(EVENT_CHAOS_CASTLE_SURVIVAL));
		return;
	}

	if ( sEventMgr->GetEventLevel(EVENT_CHAOS_CASTLE_SURVIVAL, pPlayer) == uint8(-1) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( pGuild && pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
	{
		this->EnterResult(pPlayer, 0x10);
		return;
	}

	if ( pPlayer->GetInventory()->GetItem(RING_01)->IsTransformationRing() || 
		 pPlayer->GetInventory()->GetItem(RING_02)->IsTransformationRing() )
	{
		this->EnterResult(pPlayer, 0x12);
		return;
	}

	if (pPlayer->HasMount())
	{
		this->EnterResult(pPlayer, 0x11);
		return;
	}

	if ( this->GetGround()->GetSettings()->IsPKCheck() && (pPlayer->GetPKLevel() >= this->GetGround()->GetSettings()->GetPKLevelMax()) )
	{
		this->EnterResult(pPlayer, 0x08);
		return;
	}

	int32 current_players = this->GetGround()->GetPlayersCount(true);

	if ( current_players >= this->GetGround()->GetPlayerSize() )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !pPlayer->MoneyHave(this->GetGround()->GetSettings()->GetRequiredZen()) )
	{
		this->EnterResult(pPlayer, 0x07);
		return;
	}

	if (sGameServer->GetChaosCastleSurvivalMaxPerPC() > 0)
	{
		if (this->GetGround()->GetPlayersCountByPC(pPlayer) >= sGameServer->GetChaosCastleSurvivalMaxPerPC())
		{
			this->EnterResult(pPlayer, 0x06);
			pPlayer->SendMessageBox(0, "Chaos Castle Survival", "You can only use %d character per PC.", sGameServer->GetChaosCastleSurvivalMaxPerPC());
			return;
		}
	}

	pPlayer->GetInterfaceState()->Reset();
	
	if ( !this->GetGround()->AddPlayer(pPlayer) )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !pPlayer->MoveToGate(this->GetGround()->GetEnterGate()) )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	pPlayer->MoneyReduce(this->GetGround()->GetSettings()->GetRequiredZen());

	if ( this->GetGround()->GetSettings()->IsRemoveElfSoldierBuff() )
	{
		pPlayer->RemoveBuff(BUFF_ELF_SOLDIER);
	}

	pPlayer->GetInterfaceState()->Reset();
	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have entered Chaos Castle Survival.");
	this->EnterResult(pPlayer, 0);
}

void ChaosCastleSurvivalMgr::EnterResult(Player* pPlayer, uint8 result, uint16 remain_time)
{
	CHAOS_CASTLE_SURVIVAL_ENTER_RESULT pMsg(result, remain_time);
	pPlayer->SEND_PCT(pMsg);
}

void ChaosCastleSurvivalMgr::RankingRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !pPlayer->IsPlaying() )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ChaosCastleSurvivalUI, 0) )
	{
		return;
	}

	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_RANKING_REQUEST, lpMsg, Packet, 0);

	uint8 buffer[4096];
	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_RANKING_HEAD, head, buffer, 0);
	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_RANKING_BODY, body, buffer, sizeof(CHAOS_CASTLE_SURVIVAL_RANKING_HEAD));
	head->result = 0;
	head->count = 0;
	uint8 type = lpMsg->type;

	/*
	result 1 -> No list available
	result 2 -> Can't see list at this time
	*/

	ChaosCastleSurvivalStage current_day = this->GetDay();

	if ( current_day == CHAOS_CASTLE_SURVIVAL_STAGE_NONE )
	{
		head->result = 2;
	}
	else
	{
		if ( (current_day != CHAOS_CASTLE_SURVIVAL_STAGE_PRELIMINARY || lpMsg->type != 1) &&
			 (current_day != CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL || lpMsg->type != 2) &&
			 (current_day != CHAOS_CASTLE_SURVIVAL_STAGE_FINAL || lpMsg->type != 2) )
		{
			head->result = 2;
		}
	}
		
	if ( head->result != 2 )
	{
		for ( int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i )
		{
			if ( !this->GetRanking(i)->GetID() )
			{
				continue;
			}

			body[head->count].rank = head->count + 1;
			body[head->count].score = this->GetRanking(i)->GetScore();
			memcpy(body[head->count].name, this->GetRanking(i)->GetName(), MAX_CHARACTER_LENGTH + 1);
			body[head->count].unk2 = 0;
			++head->count;
		}

		head->result = head->count > 0 ? 0: 1;
	}

	head->h.set(HEADCODE_CHAOS_CASTLE, SUBCODE_CHAOS_CASTLE_SURVIVAL_RANKING, sizeof(CHAOS_CASTLE_SURVIVAL_RANKING_HEAD) + (head->count * sizeof(CHAOS_CASTLE_SURVIVAL_RANKING_BODY)));
	pPlayer->sendPacket(buffer, head->h.get_size());
}

void ChaosCastleSurvivalMgr::Close(Player* pPlayer, uint8 * Packet)
{
	if ( !pPlayer->IsPlaying() )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ChaosCastleSurvivalUI, 0) )
	{
		return;
	}

	pPlayer->GetInterfaceState()->Reset();
}

void ChaosCastleSurvivalMgr::PositionSet(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_CHAOS_CASTLE_SURVIVAL )
	{
		return;
	}

	World* pWorld = pPlayer->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	POINTER_PCT(CHAOS_CASTLE_SURVIVAL_POSITION, lpMsg, Packet, 0);

	WorldGrid const& attr = pWorld->GetGrid(lpMsg->x, lpMsg->y);

	if ( attr.IsLocked_1() )
	{
		return;
	}

	pPlayer->PositionSend(lpMsg->x, lpMsg->y);
	//pPlayer->SetBlowTime(GetTickCount());
}

bool ChaosCastleSurvivalMgr::IsInRanking(Player* pPlayer)
{
	for ( int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i )
	{
		if ( this->GetRanking(i)->GetID() == pPlayer->GetGUID() )
		{
			return true;
		}
	}

	return false;
}

ChaosCastleSurvivalStage ChaosCastleSurvivalMgr::GetDay() const
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	
	if ( sysTime.wDay == this->GetPreliminary(0) || sysTime.wDay == this->GetPreliminary(1) || sysTime.wDay == this->GetPreliminary(2) )
	{
		return CHAOS_CASTLE_SURVIVAL_STAGE_PRELIMINARY;
	}

	if ( sysTime.wDay == this->GetSemiFinal(0) || sysTime.wDay == this->GetSemiFinal(1) )
	{
		return CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL;
	}

	if ( sysTime.wDay == this->GetFinal() )
	{
		return CHAOS_CASTLE_SURVIVAL_STAGE_FINAL;
	}

	return CHAOS_CASTLE_SURVIVAL_STAGE_NONE;
}

bool Sort(ChaosCastleSurvivalRanking & pPlayer1, ChaosCastleSurvivalRanking & pPlayer2)
{
	return (pPlayer1.GetScore() >= pPlayer2.GetScore());
}

void ChaosCastleSurvivalMgr::UpdateRanking()
{
	///- Calculo el día actual
	ChaosCastleSurvivalStage old_stage = this->GetStage();
	this->SetStage(this->GetDay());

	for (int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i)
	{
		this->GetRanking(i)->Reset();
	}

	///- Genero el Ranking correspondiente
	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_SELECT);

	ChaosCastleSurvivalRankingList sort_list;
	int32 count = 0;
	int32 max_count = this->GetCurrentRankSize();

	if (this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_NONE && old_stage == CHAOS_CASTLE_SURVIVAL_STAGE_FINAL)
	{
		max_count = 1;
	}

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			ChaosCastleSurvivalRanking AddPlayer;
			AddPlayer.SetID(reader.GetUInt32());
			AddPlayer.SetName(reader.GetString().c_str());
			AddPlayer.SetScore(reader.GetInt32());
			sort_list.push_back(AddPlayer);
			++count;

			if (count >= max_count)
			{
				break;
			}
		} while (result->NextRow());
	}

	std::sort(sort_list.begin(), sort_list.end(), Sort);

	for (int32 i = 0; i < sort_list.size(); ++i)
	{
		this->GetRanking(i)->SetID(sort_list[i].GetID());
		this->GetRanking(i)->SetScore(sort_list[i].GetScore());
		this->GetRanking(i)->SetName(sort_list[i].GetName());
	}

	///- Si no estoy dentro de las fechas entonces limpio el Ranking
	if (this->GetStage() == CHAOS_CASTLE_SURVIVAL_STAGE_NONE)
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_DELETE);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}

	if (old_stage != this->GetStage())
	{
		if (old_stage == CHAOS_CASTLE_SURVIVAL_STAGE_PRELIMINARY || old_stage == CHAOS_CASTLE_SURVIVAL_STAGE_SEMI_FINAL)
		{
			for (int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i)
			{
				this->GetRanking(i)->SetScore(0);
			}

			SQLTransaction trans = MuDatabase.BeginTransaction();
			trans->Append(MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_INIT));
			MuDatabase.CommitTransaction(trans);
		}
		else if (old_stage == CHAOS_CASTLE_SURVIVAL_STAGE_FINAL)
		{
			for (int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i)
			{
				this->GetRanking(i)->Reset();
			}
		}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_DELETE);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);

		for (int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i)
		{
			if (this->GetRanking(i)->GetID())
			{
				sChaosCastleSurvivalMgr->AddScore(this->GetRanking(i)->GetID(), this->GetRanking(i)->GetScore());
			}
		}
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_EVENT_STAGE_UPDATE);
	stmt->setUInt8(EVENT_CHAOS_CASTLE_SURVIVAL);
	stmt->setUInt8(this->GetStage());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void ChaosCastleSurvivalMgr::AddScore(uint32 char_id, int32 score)
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_CHAOS_CASTLE_SURVIVAL_RANKING_INSERT);
	stmt->setUInt32(char_id);
	stmt->setInt64(score);
	stmt->setInt64(score);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void ChaosCastleSurvivalMgr::OnPlayerConnect(uint32 id, const char * name)
{
	for (int32 i = 0; i < MAX_CHAOS_CASTLE_SURVIVAL_RANK; ++i)
	{
		if (this->GetRanking(i)->GetID() == id)
		{
			this->GetRanking(i)->ResetName();
			this->GetRanking(i)->SetName(name, MAX_CHARACTER_LENGTH);
		}
	}
}