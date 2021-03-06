TormentedSquareSurvivalGround::TormentedSquareSurvivalGround()
{
	
}

TormentedSquareSurvivalGround::~TormentedSquareSurvivalGround()
{

}

void TormentedSquareSurvivalGround::Update()
{
	switch ( this->GetState() )
	{
	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_STANDBY1:
		{
			this->ProcState_Standby1();
		} break;

	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_STANDBY2:
		{
			this->ProcState_Standby2();
		} break;

	case TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void TormentedSquareSurvivalGround::SetState_None()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_NONE);
	this->SetStage(0);
	this->SetSecondTick(GetTickCount());
	this->GetTime()->Reset();
	this->SetRankingSent(false);
	this->SetStartMessageSent(false);
	this->PlayerMoveOut();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::SetState_Open()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_OPEN);
	this->SetStage(0);
	this->SetSecondTick(GetTickCount());
	this->SetRankingSent(false);
	this->SetStartMessageSent(false);
	this->GetTime()->Reset();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::SetState_Standby1()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_STANDBY1);
	
	this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	this->SetRankingSent(false);
	
	this->SendTeamList();
	this->SendState(nullptr, 1);

	if ( sTormentedSquareSurvival->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL )
	{
		this->SendTime();
	}

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::SetState_Playing()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_PLAYING);
	this->IncreaseStage(1);
	this->SetRankingSent(false);
	this->GetStageTime();
	this->AddMonster();

	this->SendState(nullptr, 2);
	this->SendTime();	
	
	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::SetState_Standby2()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_STANDBY2);
	this->GetStageStandbyTime();
	this->SetRankingSent(false);
	this->SendState(nullptr, 4);
	this->AddTeamRoundScore();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::SetState_End()
{
	this->SetState(TORMENTED_SQUARE_SURVIVAL_GROUND_STATE_END);
	this->GetTime()->Start(TORMENTED_SQUARE_GROUND_END_TIME * IN_MILLISECONDS);
	this->SetRankingSent(false);
	this->SendState(nullptr, 3);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvivalGround::ProcState_None()
{

}

void TormentedSquareSurvivalGround::ProcState_Open()
{
	this->UpdateTeamQueue();
}

void TormentedSquareSurvivalGround::ProcState_Standby1()
{
	this->PlayerUpdate();

	if ( !this->IsStartMessageSent() && this->GetTime()->GetRemain() <= (20 * IN_MILLISECONDS) )
	{
		this->SetStartMessageSent(true);
		EVENT_MESSAGE_SEND pMsg(EVENT_MESSAGE_START_TORMENTED_SQUARE_SURVIVAL);
		this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void TormentedSquareSurvivalGround::ProcState_Playing()
{
	this->PlayerUpdate();

	if ( this->SecondPassed() )
	{
		this->SendTime();
	}

	if ( this->PlayerGetCount() <= 0 )
	{
		this->SetState_End();
		return;
	}

	if ( this->GetTime()->Elapsed() )
	{
		if ( this->GetStage() >= MAX_TORMENTED_SQUARE_SURVIVAL_ROUND )
		{
			this->AddTeamRoundScore();
			this->SendTime();
			this->SendState(nullptr, 5);
			this->SetState_End();
		}
		else
		{
			this->SetState_Standby2();
		}
	}
}

void TormentedSquareSurvivalGround::ProcState_Standby2()
{
	this->PlayerUpdate();

	if ( this->SecondPassed() )
	{
		this->SendTime();
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void TormentedSquareSurvivalGround::ProcState_End()
{
	if ( this->GetTime()->GetRemain() <= (55 * IN_MILLISECONDS) && !this->IsRankingSent() )
	{
		this->SendScoreResult();
		this->CalculateTeamPosition();

		this->SetRankingSent(true);
	}

	if ( this->GetTime()->Elapsed() )
	{
		sTormentedSquareSurvival->LoadRanking(sTormentedSquareSurvival->GetStage() + 1);
		this->SetState_None();
	}
}

void TormentedSquareSurvivalGround::UpdateTeamQueue()
{
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		if ( !this->GetTeam(h)->IsQueued() )
		{
			continue;
		}

		if ( (GetTickCount() - this->GetTeam(h)->GetQueueTime()) > 35000 )
		{
			this->GetTeam(h)->Reset();
			continue;
		}
	}

	this->PlayerUpdate();
}

void TormentedSquareSurvivalGround::GetStageStandbyTime()
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_SURVIVAL_ROUND )
	{
		this->GetTime()->Start(this->GetStageData(this->GetStage() - 1)->GetStandBy() * IN_MILLISECONDS);
	}
	else
	{
		this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	}
}

void TormentedSquareSurvivalGround::GetStageTime()
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_SURVIVAL_ROUND )
	{
		this->GetTime()->Start(this->GetStageData(this->GetStage() - 1)->GetDuration() * IN_MILLISECONDS);
	}
	else
	{
		this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	}
}

int32 TormentedSquareSurvivalGround::GetStageScore() const
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_SURVIVAL_ROUND )
	{
		return this->GetStageData(this->GetStage() - 1)->GetScore();
	}
	else
	{
		return 0;
	}
}

void TormentedSquareSurvivalGround::AddMonster()
{
	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_TORMENTED_SQUARE_SURVIVAL);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (event_monster->tormented_square_survival.stage != GetStage())
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnType(event_monster->tormented_square_survival.boss != 0 ? GAME_OBJECT_RESPAWN_DELETE : GAME_OBJECT_RESPAWN_NORMAL);
			monster->SetEventGround(0);
			monster->SetEventStage(event_monster->tormented_square_survival.stage);
			monster->SetMoveDistance(60);
			monster->AddAdditionalDataInt(0, event_monster->tormented_square_survival.score);
			monster->AddToWorld();
		}
	}
}

int32 TormentedSquareSurvivalGround::PlayerGetCount() const
{
	int32 count = 0;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			if ( this->GetTeam(h)->GetPlayer(i)->GetGUID() )
			{
				++count;
			}
		}
	}

	return count;
}

void TormentedSquareSurvivalGround::PlayerUpdate()
{
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		if ( this->GetTeam(h)->IsQueued() )
		{
			continue;
		}

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			Player* pPlayer = this->GetTeam(h)->GetPlayer(i)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
			{
				this->GetTeam(h)->GetPlayer(i)->SetPlayer(nullptr);
				continue;
			}

			if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			{
				continue;
			}

			if ( pPlayer->GetWorldId() != this->GetWorldID() )
			{
				pPlayer->EventResetData();
				this->GetTeam(h)->GetPlayer(i)->Reset();
				continue;
			}

			if ( pPlayer->GetPartyID() != this->GetTeam(h)->GetPartyID() )
			{
				pPlayer->EventResetData();
				pPlayer->MoveToGate(this->GetRespawnGate());
				this->GetTeam(h)->GetPlayer(i)->Reset();
				continue;
			}
			
			Party* pParty = pPlayer->GetParty();

			if ( !pParty )
			{
				continue;
			}

			if ( pParty->GetMember(0)->GetGUID() != this->GetTeam(h)->GetPlayer(0)->GetGUID() )
			{
				pPlayer->EventResetData();
				pPlayer->MoveToGate(this->GetRespawnGate());
				this->GetTeam(h)->GetPlayer(i)->Reset();
				continue;
			}
		}

		if ( this->GetTeam(h)->GetPlayerCount() <= 0 )
		{
			this->GetTeam(h)->Reset();
		}
	}
}

void TormentedSquareSurvivalGround::PlayerMoveOut()
{
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			Player* pPlayer = this->GetTeam(h)->GetPlayer(i)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetWorldId() != this->GetWorldID() )
			{
				continue;
			}

			pPlayer->TeleportToGate(this->GetRespawnGate());
			pPlayer->EventResetData();
		}
	}

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			this->GetTeam(h)->GetPlayer(i)->Reset();
		}
	}
}

void TormentedSquareSurvivalGround::SendPacket(uint8 * Packet, uint16 size)
{
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			Player* pPlayer = this->GetTeam(h)->GetPlayer(i)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			pPlayer->sendPacket(Packet, size);
		}
	}
}

void TormentedSquareSurvivalGround::SendTime()
{
	TORMENTED_SQUARE_SURVIVAL_TIME pMsg;
	pMsg.remain_time = this->GetTime()->GetRemain() / IN_MILLISECONDS;
	pMsg.my_team = 0;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		pMsg.team_score[h] = this->GetTeam(h)->GetScore();
	}

	for ( int32 i = 0; i < this->GetTeamSize(); ++i )
	{
		if ( !this->GetTeam(i)->IsInUse() )
		{
			continue;
		}
		
		pMsg.my_team = i;

		for ( int32 h = 0; h < this->GetTeam(i)->GetPlayerSize(); ++h )
		{
			Player* pPlayer = this->GetTeam(i)->GetPlayer(h)->GetPlayer();

			if ( pPlayer )
			{
				pPlayer->SEND_PCT(pMsg);
			}
		}
	}
}

void TormentedSquareSurvivalGround::SendTeamList()
{
	uint8 buffer[8192];
	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_TEAM_HEAD, head, buffer, 0);
	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_TEAM_BODY, body, buffer, sizeof(TORMENTED_SQUARE_SURVIVAL_TEAM_HEAD));
	head->count = 0;
	head->result = 0;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		body[head->count].team_index = h;
		memset(body[head->count].name1, 0, MAX_CHARACTER_LENGTH + 1);
		memset(body[head->count].name2, 0, MAX_CHARACTER_LENGTH + 1);

		memcpy(body[head->count].name1, this->GetTeam(h)->GetPlayer(0)->GetName(), MAX_CHARACTER_LENGTH);
		memcpy(body[head->count].name2, this->GetTeam(h)->GetPlayer(1)->GetName(), MAX_CHARACTER_LENGTH);

		body[head->count].team_level = this->GetTeam(h)->GetLevel();

		++head->count;
	}

	head->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_TEAM_LIST, sizeof(TORMENTED_SQUARE_SURVIVAL_TEAM_HEAD) + (sizeof(TORMENTED_SQUARE_SURVIVAL_TEAM_BODY) * head->count));

	if ( head->count <= 0 )
	{
		head->result = 100;
	}

	this->SendPacket(buffer, head->h.get_size());
}

void TormentedSquareSurvivalGround::SendState(Player* pPlayer, uint8 state)
{
	TORMENTED_SQUARE_SURVIVAL_STATE pMsg;
	pMsg.state = state;

	if ( pPlayer )
	{
		pPlayer->SEND_PCT(pMsg);
	}
	else
	{
		this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
	}
}

void TormentedSquareSurvivalGround::AddTeamRoundScore()
{
	for ( int32 i = 0; i < this->GetTeamSize(); ++i )
	{
		if ( !this->GetTeam(i)->IsInUse() )
		{
			continue;
		}

		this->GetTeam(i)->IncreaseScore(this->GetTeam(i)->GetPlayerCount() * this->GetStageScore());
	}
}

void TormentedSquareSurvivalGround::SendScoreResult(Player* pPlayer)
{
	TORMENTED_SQUARE_SURVIVAL_SCORE_RESULT pMsg;

	for ( int32 i = 0; i < this->GetTeamSize(); ++i )
	{
		if ( !this->GetTeam(i)->IsInUse() )
		{
			continue;
		}

		pMsg.team_score[i] = this->GetTeam(i)->GetScore();
	}

	if ( !pPlayer )
	{
		this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
	}
	else
	{
		if ( sTormentedSquareSurvival->IsIn(pPlayer, false) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

uint8 TormentedSquareSurvivalGround::GetEmptyTeam() const
{
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			return h;
		}
	}

	return -1;
}

bool SortSurvival(TormentedSquareTeam* pTeam01, TormentedSquareTeam* pTeam02)
{
	if ( pTeam01->GetScore() >= pTeam02->GetScore() )
	{
		if ( pTeam01->GetScore() == pTeam02->GetScore() && pTeam01->GetLevel() > pTeam02->GetLevel() )
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void TormentedSquareSurvivalGround::CalculateTeamPosition()
{
	std::vector<TormentedSquareTeam*> team_list;

	for (int32 h = 0; h < this->GetTeamSize(); ++h)
	{
		if (!this->GetTeam(h)->IsInUse())
		{
			continue;
		}

		team_list.push_back(this->GetTeam(h));
	}

	if (team_list.empty())
	{
		return;
	}

	std::sort(team_list.begin(), team_list.end(), SortSurvival);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	for (size_t i = 0; i < team_list.size(); ++i)
	{
		if (team_list[i]->GetScore() <= 0)
		{
			continue;
		}

		if (((sTormentedSquareSurvival->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY) && (i == 0 || i == 1 || i == 2 || i == 3)) ||
			((sTormentedSquareSurvival->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL) && (i == 0 || i == 1)) ||
			((sTormentedSquareSurvival->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL) && (i == 0)))
		{
			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_TEAM_INSERT);
			stmt->setUInt32(team_list[i]->GetPlayer(0)->GetGUID());
			stmt->setUInt32(team_list[i]->GetPlayer(1)->GetGUID());
			stmt->setInt32(team_list[i]->GetScore());
			stmt->setUInt8(sTormentedSquareSurvival->GetStage() + 1);
			stmt->setInt32(team_list[i]->GetScore());
			trans->Append(stmt);
		}
	}

	auto stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
	stmt->setUInt32(team_list[0]->GetPlayer(0)->GetGUID());
	stmt->setUInt16(g_TormentedSquareSurvivalWinnerReward[sTormentedSquareSurvival->GetCurrentGround()]);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt8(-1);
	stmt->setUInt32(0);

	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
	stmt->setUInt32(team_list[0]->GetPlayer(1)->GetGUID());
	stmt->setUInt16(g_TormentedSquareSurvivalWinnerReward[sTormentedSquareSurvival->GetCurrentGround()]);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt8(0);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt16(SOCKET_SLOT_NONE);
	stmt->setUInt8(-1);
	stmt->setUInt32(0);

	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

TormentedSquareSurvival::TormentedSquareSurvival()
{
	this->GetGround()->SetGround(0);
	this->GetGround()->SetRespawnGate(TORMENTED_SQUARE_GATE);
	this->GetGround()->SetWorldID(WORLD_TORMENTED_SQUARE);
	this->GetGround()->SetEnterGate(474);

	this->GetGround()->CreateTeam(TORMENTED_SQUARE_SURVIVAL_MAX_TEAM);

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		this->GetGround()->GetTeam(h)->CreatePlayer(TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE);
		this->GetGround()->GetTeam(h)->Reset();
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i )
	{
		this->GetRankingSemiFinal(i)->Reset();
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i )
	{
		this->GetRankingFinal(i)->Reset();
	}

	this->GetRankingWinner()->Reset();
	this->SetRankingLoaded(false);
	this->SetStage(TORMENTED_SQUARE_SURVIVAL_STAGE_NONE);

	this->SetState_None();
}

TormentedSquareSurvival::~TormentedSquareSurvival()
{
	this->GetGround()->EraseTeam();
}

void TormentedSquareSurvival::Load()
{
	for ( int32 h = 0; h < MAX_TORMENTED_SQUARE_SURVIVAL_ROUND; ++h )
	{
		this->GetGround()->GetStageData(h)->Reset();
	}

	if ( !sGameServer->IsTormentedSquareSurvivalEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Tormented Square Survival Settings...");

	ReadScript file("EventData/TormentedSquareSurvival.txt", "rb");

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
					this->SetPreliminary(0, file.GetUInt8());

					file.GetToken();	this->SetPreliminary(1, file.GetUInt8());
					file.GetToken();	this->SetPreliminary(2, file.GetUInt8());
					file.GetToken();	this->SetPreliminary(3, file.GetUInt8());
					file.GetToken();	this->SetSemiFinal(0, file.GetUInt8());
					file.GetToken();	this->SetSemiFinal(1, file.GetUInt8());
					file.GetToken();	this->SetFinal(file.GetUInt8());
					file.GetToken();	this->SetRewardDayStart(file.GetUInt8());
					file.GetToken();	this->SetRewardDayEnd(file.GetUInt8());
				} break;

			case 1:
				{
					uint8 stage = file.GetUInt8();
					file.GetToken();	int32 standby = file.GetInt32();
					file.GetToken();	int32 duration = file.GetInt32();
					file.GetToken();	int32 score = file.GetInt32();

					if ( stage < MAX_TORMENTED_SQUARE_SURVIVAL_ROUND )
					{
						this->GetGround()->GetStageData(stage)->SetStandBy(standby);
						this->GetGround()->GetStageData(stage)->SetDuration(duration);
						this->GetGround()->GetStageData(stage)->SetScore(score);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Tormented Square Survival Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void TormentedSquareSurvival::LoadRanking(uint8 type)
{
	if (type > TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER)
	{
		return;
	}

	switch (type)
	{
	case TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL:
	{
													   for (int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i)
													   {
														   this->GetRankingSemiFinal(i)->Reset();
													   }
	} break;

	case TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL:
	{
												  for (int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i)
												  {
													  this->GetRankingFinal(i)->Reset();
												  }
	} break;

	case TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER:
	{
												   this->GetRankingWinner()->Reset();
	} break;
	}

	int32 count = 0;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_TEAM_SELECT);
	stmt->setUInt8(type);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_RANKING_DELETE);
	stmt->setUInt8(type);
	trans->Append(stmt);

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			TormentedSquareSurvivalRanking * pRanking = nullptr;

			switch (type)
			{
			case TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL:
			{
															   if (count < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL)
															   {
																   pRanking = this->GetRankingSemiFinal(count);
															   }

															   ++count;
			} break;

			case TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL:
			{
														  if (count < MAX_TORMENTED_SQUARE_RANKING_FINAL)
														  {
															  pRanking = this->GetRankingFinal(count);
														  }

														  ++count;
			} break;

			case TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER:
			{
														   if (count < 1)
														   {
															   pRanking = this->GetRankingWinner();
														   }

														   ++count;
			} break;
			}

			auto char_id_1 = reader.GetUInt32();
			std::string const& name_1 = reader.GetString();
			auto char_id_2 = reader.GetUInt32();
			std::string const& name_2 = reader.GetString();
			auto score = reader.GetInt32();

			if (pRanking)
			{
				pRanking->SetID1(char_id_1);
				pRanking->SetName1(name_1.c_str());
				pRanking->SetID2(char_id_2);
				pRanking->SetName2(name_2.c_str());
				pRanking->SetScore(score);
				pRanking->SetType(type);
				pRanking->SetRank(count);
			}

			stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_RANKING_INSERT);
			stmt->setUInt8(type);
			stmt->setInt32(count);
			stmt->setUInt32(char_id_1);
			stmt->setString(name_1);
			stmt->setUInt32(char_id_2);
			stmt->setString(name_2);
			stmt->setInt32(score);
			trans->Append(stmt);
		} while (result->NextRow());
	}

	MuDatabase.CommitTransaction(trans);
}

TormentedSquareSurvivalStage TormentedSquareSurvival::GetDay() const
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	
	if ( sysTime.wDay == this->GetPreliminary(0) || sysTime.wDay == this->GetPreliminary(1) || sysTime.wDay == this->GetPreliminary(2) || sysTime.wDay == this->GetPreliminary(3) )
	{
		return TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY;
	}

	if ( sysTime.wDay == this->GetSemiFinal(0) || sysTime.wDay == this->GetSemiFinal(1) )
	{
		return TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL;
	}

	if ( sysTime.wDay == this->GetFinal() )
	{
		return TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL;
	}

	if ( sysTime.wDay >= this->GetRewardDayStart() && sysTime.wDay <= this->GetRewardDayEnd() )
	{
		return TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER;
	}

	return TORMENTED_SQUARE_SURVIVAL_STAGE_NONE;
}

void TormentedSquareSurvival::Start(int32 notify)
{
	if ( !sGameServer->IsTormentedSquareSurvivalEnabled() )
	{
		return;
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_NONE )
	{
		return;
	}

	if ( this->GetStage() != TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY &&
		 this->GetStage() != TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL &&
		 this->GetStage() != TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL )
	{
		return;
	}

	this->SetNotifyTime(notify);

	if ( notify > 0 )
	{
		this->SetState_Notify();
	}
	else
	{
		this->SetState_Open();
	}
}
	
void TormentedSquareSurvival::Update()
{
	if ( !sGameServer->IsTormentedSquareSurvivalEnabled() )
	{
		return;
	}

	if ( !this->IsRankingLoaded() )
	{
		this->SetRankingLoaded(true);

		this->LoadRanking(TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL);
		this->LoadRanking(TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL);
		this->LoadRanking(TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER);
	}

	switch ( this->GetState() )
	{
	case TORMENTED_SQUARE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case TORMENTED_SQUARE_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case TORMENTED_SQUARE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case TORMENTED_SQUARE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;
	}

	this->GetGround()->Update();
}

void TormentedSquareSurvival::SetState_None()
{
	this->SetState(TORMENTED_SQUARE_STATE_NONE);
	this->GetTime()->Reset();
	this->SetCurrentMinute(-1);
	this->SetNotifyTime(0);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvival::SetState_Notify()
{
	this->SetState(TORMENTED_SQUARE_STATE_NOTIFY);
	this->GetTime()->Start(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvival::SetState_Open()
{
	this->SetState(TORMENTED_SQUARE_STATE_OPEN);
	this->GetTime()->Start(TORMENTED_SQUARE_OPEN_TIME * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);

	this->GetGround()->SetState_Open();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvival::SetState_Playing()
{
	this->SetState(TORMENTED_SQUARE_STATE_PLAYING);

	if (this->GetGround()->PlayerGetCount() >= (TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE * TORMENTED_SQUARE_SURVIVAL_MIN_TEAM))
	{
		for (int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h)
		{
			if (!this->GetGround()->GetTeam(h)->IsInUse())
			{
				continue;
			}

			if (!this->GetGround()->GetTeam(h)->IsQueued())
			{
				continue;
			}

			this->GetGround()->GetTeam(h)->Reset();
		}

		this->GetGround()->SetState_Standby1();
	}
	else
	{
		if (sTormentedSquareSurvival->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL)
		{
			this->GetGround()->SendScoreResult();
			this->GetGround()->CalculateTeamPosition();
		}

		this->GetGround()->SetState_None();
	}

	sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "[Tormented Square Survival] has been closed.");

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareSurvival::ProcState_None()
{
	if ( this->GetTime()->Elapsed(30 * IN_MILLISECONDS) )
	{
		TormentedSquareSurvivalStage current_stage = this->GetStage();

		this->SetStage(this->GetDay());

		if ( current_stage != this->GetStage() )
		{
			if ( current_stage == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL )
			{
				if ( this->GetRankingWinner()->GetID1() && this->GetRankingWinner()->GetID2() )
				{
					SQLTransaction trans = MuDatabase.BeginTransaction();

					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_TORMENTED_SQUARE_SURVIVAL_HALL_OF_FAME_INSERT);
					stmt->setUInt32(this->GetRankingWinner()->GetID1());
					stmt->setString(this->GetRankingWinner()->GetName1());
					stmt->setUInt32(this->GetRankingWinner()->GetID2());
					stmt->setString(this->GetRankingWinner()->GetName2());
					stmt->setInt32(this->GetRankingWinner()->GetScore());
					trans->Append(stmt);

					MuDatabase.CommitTransaction(trans);
				}
			}

			SQLTransaction trans = MuDatabase.BeginTransaction();

			PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_BATTLE_CORE_EVENT_STAGE_UPDATE);
			stmt->setUInt8(EVENT_TORMENTED_SQUARE_SURVIVAL);
			stmt->setUInt8(this->GetStage());
			trans->Append(stmt);
		
			MuDatabase.CommitTransaction(trans);
		}
	}
}

void TormentedSquareSurvival::ProcState_Notify()
{
	this->SendRemainMinutes("[Tormented Square Survival] is open after %d mins.", 5, 15);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Open();
	}
}

void TormentedSquareSurvival::ProcState_Open()
{
	this->SendRemainMinutes("[Tormented Square Survival] is closed after %d mins.", 1, 5);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void TormentedSquareSurvival::ProcState_Playing()
{
	if ( this->GetGround()->GetState() != TORMENTED_SQUARE_GROUND_STATE_NONE )
	{
		return;
	}

	this->SetState_None();
}

void TormentedSquareSurvival::RemovePlayer(Player* pPlayer)
{
	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		for ( int32 n = 0; n < this->GetGround()->GetTeam(h)->GetPlayerSize(); ++n )
		{
			if ( this->GetGround()->GetTeam(h)->GetPlayer(n)->GetGUID() == pPlayer->GetGUID() )
			{
				this->GetGround()->GetTeam(h)->GetPlayer(n)->Reset();
				return;
			}
		}
	}
}
	
bool TormentedSquareSurvival::IsIn(Player* pPlayer, bool add)
{
	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN &&
		 this->GetState() != TORMENTED_SQUARE_STATE_PLAYING )
	{
		return false;
	}

	if ( this->GetGround()->GetState() != TORMENTED_SQUARE_GROUND_STATE_STANDBY &&
		 this->GetGround()->GetState() != TORMENTED_SQUARE_GROUND_STATE_PLAYING )
	{
		return false;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		for ( int32 n = 0; n < this->GetGround()->GetTeam(h)->GetPlayerSize(); ++n )
		{
			if ( this->GetGround()->GetTeam(h)->GetPlayer(n)->GetGUID() == pPlayer->GetGUID() )
			{
				if ( add )
				{
					this->GetGround()->GetTeam(h)->GetPlayer(n)->SetPlayer(pPlayer);
				
					pPlayer->SetEventId(EVENT_TORMENTED_SQUARE_SURVIVAL);
					pPlayer->SetEventGround(0);
				}
				return true;
			}
		}
	}

	return false;
}

void TormentedSquareSurvival::SendPacket(uint8 * Packet, uint16 size)
{
	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		for ( int32 n = 0; n < this->GetGround()->GetTeam(h)->GetPlayerSize(); ++n )
		{
			Player* pPlayer = this->GetGround()->GetTeam(h)->GetPlayer(n)->GetPlayer();

			if ( pPlayer )
			{
				pPlayer->sendPacket(Packet, size);
			}
		}
	}
}

void TormentedSquareSurvival::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
{
	if ( !this->IsMinuteElapsed(period) )
	{
		return;
	}

	if ( max_minute != -1 && (this->GetCurrentMinute() + 1) > max_minute )
	{
		return;
	}

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, message.c_str(), this->GetCurrentMinute() + 1);
}

void TormentedSquareSurvival::ScheduleRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		return;
	}

	TORMENTED_SQUARE_SURVIVAL_SCHEDULE pMsg;
	pMsg.preliminary[0] = this->GetPreliminary(0);
	pMsg.preliminary[1] = this->GetPreliminary(1);
	pMsg.preliminary[2] = this->GetPreliminary(2);
	pMsg.preliminary[3] = this->GetPreliminary(3);
	pMsg.semi_final[0] = this->GetSemiFinal(0);
	pMsg.semi_final[1] = this->GetSemiFinal(1);
	pMsg.final = this->GetFinal();
	pMsg.current_day = this->GetDay();

	sEventMgr->BuildTormentedSquareSurvivalSchedule(pMsg.preliminary_hour, pMsg.preliminary_minute, TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY);
	sEventMgr->BuildTormentedSquareSurvivalSchedule(pMsg.semi_final_hour, pMsg.semi_final_minute, TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL);
	sEventMgr->BuildTormentedSquareSurvivalSchedule(&pMsg.final_hour, &pMsg.final_minute, TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL);

	pMsg.reward_start_day = this->GetRewardDayStart();
	pMsg.reward_end_day = this->GetRewardDayEnd();

	pPlayer->SEND_PCT(pMsg);
}

void TormentedSquareSurvival::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		return;
	}

	/*
	0 = Success -> Queue
	1 = Require 2 man Party
	2 = Require 2 man Party
	3 = Party Leader
	4 = Require 2 man Party
	5 = Level Req
	6 = Nada
	7 = Transformation Ring
	8 = Not your time to enter
	9 = Not your day to enter
	10 = Team members must be in Loren Market
	11 = Team members is busy
	12 = Pk Can't enter
	13 = Nada
	14 = Can't enter exceeding the member limit
	15 = You cannot enter during battle
	16 = Guild War
	17 = Can't enter using mount
	18 = Your guild is not legible to enter this tournament
	19 = Your guild is not legible to enter this tournament
	20 = Entrance Request Made Already
	21 = Your party has a member that has already qualified for next round
	22 = Your party has a member that has already qualified for next round
	23 = Your party has a member that has already qualified for next round
	24 = You canceled your entry
	*/

	Party* pParty = pPlayer->GetParty();

	if ( !pParty )
	{
		this->EnterResult(pPlayer, 1);
		return;
	}

	if ( pParty->GetLeader() != pPlayer )
	{
		this->EnterResult(pPlayer, 3);
		return;
	}

	if ( pParty->GetMemberCount() != TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE )
	{
		this->EnterResult(pPlayer, 4);
		return;
	}

	for ( int32 i = 0; i < TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE; ++i )
	{
		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if ( !pMember )
		{
			this->EnterResult(pPlayer, 2);
			return;
		}

		if ( pMember->GetWorldId() != WORLD_LOREN_MARKET || pMember->GetRegenStatus() != REGEN_NONE )
		{
			this->EnterResult(pPlayer, 10, pMember->GetName());
			return;
		}

		if ( !Player::IsPlayerTransactionFree(pMember) )
		{
			this->EnterResult(pPlayer, 11, pMember->GetName());

			if ( pMember != pPlayer )
			{
				this->EnterResult(pMember, 11, pMember->GetName());
			}
			return;
		}

		if ( pMember->GetPKLevel() >= 4 )
		{
			this->EnterResult(pPlayer, 12, pMember->GetName());

			if ( pMember != pPlayer )
			{
				this->EnterResult(pMember, 12, pMember->GetName());
			}
			return;
		}

		if ( pMember->GetTotalLevel() < 400 )
		{
			this->EnterResult(pPlayer, 5, pMember->GetName());

			if ( pMember != pPlayer )
			{
				this->EnterResult(pMember, 5, pMember->GetName());
			}

			return;
		}

		if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_PRELIMINARY && (this->IsSemiFinalPlayer(pMember) || this->IsFinalPlayer(pMember)) )
		{
			this->EnterResult(pPlayer, 21, pMember->GetName());

			if ( pMember != pPlayer )
			{
				this->EnterResult(pMember, 21, pMember->GetName());
			}

			return;
		}

		if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL && this->IsFinalPlayer(pMember) )
		{
			this->EnterResult(pPlayer, 22, pMember->GetName());

			if ( pMember != pPlayer )
			{
				this->EnterResult(pMember, 22, pMember->GetName());
			}

			return;
		}
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 8);
		return;
	}

	if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_NONE )
	{
		this->EnterResult(pPlayer, 8);
		return;
	}

	if ( this->IsTeam(pParty) )
	{
		this->EnterResult(pPlayer, 20);
		return;
	}

	this->RemoveTeam(pPlayer);
	this->RemoveTeam(pParty);

	if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL && !this->IsSemiFinalTeam(pParty) )
	{
		this->EnterResult(pPlayer, 18);
		return;
	}

	if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL && !this->IsFinalTeam(pParty) )
	{
		this->EnterResult(pPlayer, 19);
		return;
	}

	uint8 team = this->GetGround()->GetEmptyTeam();

	if ( team == uint8(-1) )
	{
		this->EnterResult(pPlayer, 14);
		return;
	}

	this->GetGround()->GetTeam(team)->SetInUse(true);
	this->GetGround()->GetTeam(team)->SetQueued(true);
	this->GetGround()->GetTeam(team)->SetQueueTime(GetTickCount());
	this->GetGround()->GetTeam(team)->SetPartyID(pPlayer->GetPartyID());

	if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL )
	{
		this->GetGround()->GetTeam(team)->SetScore(1);
	}

	for ( int32 i = 0; i < TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE; ++i )
	{
		Player* pMember = pParty->GetMember(i)->GetPlayer();

		this->GetGround()->GetTeam(team)->GetPlayer(i)->SetQueueState(TORMENTED_SQUARE_QUEUE_STATE_STANDBY);
		this->GetGround()->GetTeam(team)->GetPlayer(i)->SetGUID(pMember->GetGUID());
		this->GetGround()->GetTeam(team)->GetPlayer(i)->SetName(pMember->GetName());
		this->GetGround()->GetTeam(team)->GetPlayer(i)->SetPlayer(pMember);

		if ( this->GetStage() == TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL )
		{
			this->GetGround()->GetTeam(team)->GetPlayer(i)->SetScore(1);
		}

		this->EnterResult(pMember, 0);
	}
}

void TormentedSquareSurvival::EnterResult(Player* pPlayer, uint8 result, const char * name)
{
	if ( !pPlayer )
	{
		return;
	}

	TORMENTED_SQUARE_SURVIVAL_ENTER pMsg;
	pMsg.result = result;
	if ( name )
	{
		memcpy(pMsg.name, name, MAX_CHARACTER_LENGTH);
	}
	pMsg.day = this->GetDay();

	pPlayer->SEND_PCT(pMsg);
}

void TormentedSquareSurvival::EnterAccept(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		return;
	}

	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT, lpMsg, Packet, 0);

	Party* pParty = pPlayer->GetParty();

	if ( !pParty )
	{
		return;
	}

	TormentedSquareTeam * pTeam = this->GetTeam(pParty);

	if ( !pTeam || !pTeam->IsQueued() )
	{
		this->RemoveTeam(pParty);
		return;
	}

	if ( lpMsg->result == 1 )
	{
		this->RemoveTeam(pParty);
		this->EnterAcceptResult(pParty, 24, pPlayer->GetName());
		return;
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN )
	{
		this->RemoveTeam(pParty);
		this->EnterAcceptResult(pParty, 24, pPlayer->GetName());
		return;
	}

	if ( !this->SetTeamQueueOK(pTeam, pPlayer) )
	{
		this->EnterAcceptResult(pPlayer, -1, pPlayer->GetName());
		return;
	}

	if ( !this->IsTeamQueueComplete(pTeam) )
	{
		return;
	}

	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		Player* pMember = pTeam->GetPlayer(i)->GetPlayer();

		if ( !pMember )
		{
			this->RemoveTeam(pParty);
			this->EnterAcceptResult(pParty, 13, pTeam->GetPlayer(i)->GetName());
			return;
		}

		if ( pTeam->GetPlayer(i)->GetGUID() != pParty->GetMember(i)->GetGUID() )
		{
			this->RemoveTeam(pParty);
			this->EnterAcceptResult(pParty, 13, pTeam->GetPlayer(i)->GetName());
			return;
		}

		if ( pMember->GetWorldId() != WORLD_LOREN_MARKET || pMember->GetRegenStatus() != REGEN_NONE )
		{
			this->RemoveTeam(pParty);
			this->EnterAcceptResult(pParty, 13, pTeam->GetPlayer(i)->GetName());
			return;
		}

		if ( !Player::IsPlayerTransactionFree(pMember) )
		{
			this->RemoveTeam(pParty);
			this->EnterAcceptResult(pParty, 11, pTeam->GetPlayer(i)->GetName());
			return;
		}

		if ( pMember->GetPKLevel() >= 4 )
		{
			this->RemoveTeam(pParty);
			this->EnterAcceptResult(pParty, 12, pTeam->GetPlayer(i)->GetName());
			return;
		}
	}

	pTeam->SetQueued(false);

	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		Player* pMember = pTeam->GetPlayer(i)->GetPlayer();

		this->GetGround()->SendState(pMember, 0);

		this->EnterAcceptResult(pMember, 0, pMember->GetName());

		pPlayer->SetEventId(EVENT_TORMENTED_SQUARE_SURVIVAL);
		pPlayer->SetEventGround(this->GetCurrentGround());
		pMember->MoveToGate(this->GetGround()->GetEnterGate());
	}
}

void TormentedSquareSurvival::EnterAcceptResult(Player* pPlayer, uint8 result, const char * name)
{
	if ( !pPlayer )
	{
		return;
	}

	TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT_RESULT pMsg;
	pMsg.result = result;
	if ( name )
	{
		memcpy(pMsg.name, name, MAX_CHARACTER_LENGTH);
	}

	pPlayer->SEND_PCT(pMsg);
}

void TormentedSquareSurvival::EnterAcceptResult(Party* pParty, uint8 result, const char * name)
{
	if ( !pParty )
	{
		return;
	}

	TORMENTED_SQUARE_SURVIVAL_ENTER_ACCEPT_RESULT pMsg;
	pMsg.result = result;
	if ( name )
	{
		memcpy(pMsg.name, name, MAX_CHARACTER_LENGTH);
	}

	for ( int32 i = 0; i < TORMENTED_SQUARE_SURVIVAL_TEAM_SIZE; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
		{
			continue;
		}

		Player* pPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		pPlayer->SEND_PCT(pMsg);
	}
}

void TormentedSquareSurvival::RankingRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_LOREN_MARKET )
	{
		return;
	}

	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_RANKING, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_RANKING_HEAD, head, buffer, 0);
	POINTER_PCT(TORMENTED_SQUARE_SURVIVAL_RANKING_BODY, body, buffer, sizeof(TORMENTED_SQUARE_SURVIVAL_RANKING_HEAD));
	head->count = 0;
	head->result = 0;
	head->type = lpMsg->type;

	switch ( lpMsg->type )
	{
	case TORMENTED_SQUARE_SURVIVAL_STAGE_SEMI_FINAL:
		{
			for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i )
			{
				if ( this->GetRankingSemiFinal(i)->GetRank() > 0 )
				{
					memcpy(body[head->count].name1, this->GetRankingSemiFinal(i)->GetName1(), MAX_CHARACTER_LENGTH + 1);
					memcpy(body[head->count].name2, this->GetRankingSemiFinal(i)->GetName2(), MAX_CHARACTER_LENGTH + 1);
					body[head->count].server_group1 = 0;
					body[head->count].server_group2 = 0;
					body[head->count].rank = this->GetRankingSemiFinal(i)->GetRank();
					body[head->count].type = this->GetRankingSemiFinal(i)->GetType();
					body[head->count].score = this->GetRankingSemiFinal(i)->GetScore();

					body[head->count].enter_day = 0;
					body[head->count].enter_month = 0;
					body[head->count].enter_year = 0;

					++head->count;
				}
			}
		} break;

	case TORMENTED_SQUARE_SURVIVAL_STAGE_FINAL:
		{
			for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i )
			{
				if ( this->GetRankingFinal(i)->GetRank() > 0 )
				{
					memcpy(body[head->count].name1, this->GetRankingFinal(i)->GetName1(), MAX_CHARACTER_LENGTH + 1);
					memcpy(body[head->count].name2, this->GetRankingFinal(i)->GetName2(), MAX_CHARACTER_LENGTH + 1);
					body[head->count].server_group1 = 0;
					body[head->count].server_group2 = 0;
					body[head->count].rank = this->GetRankingFinal(i)->GetRank();
					body[head->count].type = this->GetRankingFinal(i)->GetType();
					body[head->count].score = this->GetRankingFinal(i)->GetScore();

					body[head->count].enter_day = 0;
					body[head->count].enter_month = 0;
					body[head->count].enter_year = 0;

					++head->count;
				}
			}
		} break;

	case TORMENTED_SQUARE_SURVIVAL_STAGE_WINNER:
		{
			if ( this->GetRankingWinner()->GetRank() > 0 )
			{
				memcpy(body[head->count].name1, this->GetRankingWinner()->GetName1(), MAX_CHARACTER_LENGTH + 1);
				memcpy(body[head->count].name2, this->GetRankingWinner()->GetName2(), MAX_CHARACTER_LENGTH + 1);
				body[head->count].server_group1 = 0;
				body[head->count].server_group2 = 0;
				body[head->count].rank = this->GetRankingWinner()->GetRank();
				body[head->count].type = this->GetRankingWinner()->GetType();
				body[head->count].score = this->GetRankingWinner()->GetScore();

				body[head->count].enter_day = 0;
				body[head->count].enter_month = 0;
				body[head->count].enter_year = 0;

				++head->count;
			}
		} break;
	}

	head->h.set(HEADCODE_TORMENTED_SQUARE_SURVIVAL, SUBCODE_TORMENTED_SQUARE_SURVIVAL_RANKING, sizeof(TORMENTED_SQUARE_SURVIVAL_RANKING_HEAD) + (head->count * sizeof(TORMENTED_SQUARE_SURVIVAL_RANKING_BODY)));

	if ( head->count == 0 )
	{
		head->result = 100;
	}

	pPlayer->sendPacket(buffer, head->h.get_size());
}

void TormentedSquareSurvival::RemoveTeam(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		for ( int32 i = 0; i < this->GetGround()->GetTeam(h)->GetPlayerSize(); ++i )
		{
			if ( this->GetGround()->GetTeam(h)->GetPlayer(i)->GetGUID() != pPlayer->GetGUID() )
			{
				continue;
			}

			this->GetGround()->GetTeam(h)->Reset();
			break;
		}
	}
}

void TormentedSquareSurvival::RemoveTeam(Party* pParty)
{
	if ( !pParty )
	{
		return;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		if ( pParty->GetID() == this->GetGround()->GetTeam(h)->GetPartyID() )
		{
			this->GetGround()->GetTeam(h)->Reset();
		}
	}
}

bool TormentedSquareSurvival::IsTeam(Party* pParty)
{
	if ( !pParty )
	{
		return false;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		if ( pParty->GetID() == this->GetGround()->GetTeam(h)->GetPartyID() )
		{
			return true;
		}
	}

	return false;
}

TormentedSquareTeam * TormentedSquareSurvival::GetTeam(Party* pParty)
{
	if ( !pParty )
	{
		return nullptr;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		if ( pParty->GetID() == this->GetGround()->GetTeam(h)->GetPartyID() )
		{
			return this->GetGround()->GetTeam(h);
		}
	}

	return nullptr;
}

bool TormentedSquareSurvival::SetTeamQueueOK(TormentedSquareTeam * pTeam, Player* pPlayer)
{
	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		if ( pTeam->GetPlayer(i)->GetGUID() == pPlayer->GetGUID() )
		{
			if ( pTeam->GetPlayer(i)->GetQueueState() == TORMENTED_SQUARE_QUEUE_STATE_STANDBY )
			{
				pTeam->GetPlayer(i)->SetQueueState(TORMENTED_SQUARE_QUEUE_STATE_ACCEPT);
				return true;
			}
		}
	}

	return false;
}

bool TormentedSquareSurvival::IsTeamQueueComplete(TormentedSquareTeam * pTeam)
{
	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		if ( pTeam->GetPlayer(i)->GetQueueState() != TORMENTED_SQUARE_QUEUE_STATE_ACCEPT )
		{
			return false;
		}
	}

	return true;
}

bool TormentedSquareSurvival::IsSemiFinalTeam(Party* pParty) const
{
	if ( !pParty )
	{
		return false;
	}

	if ( !pParty->GetMember(0)->GetPlayer() ||
		 !pParty->GetMember(1)->GetPlayer() )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i )
	{
		if ( this->GetRankingSemiFinal(i)->GetID1() == pParty->GetMember(0)->GetPlayer()->GetGUID() &&
			 this->GetRankingSemiFinal(i)->GetID2() == pParty->GetMember(1)->GetPlayer()->GetGUID() )
		{
			return true;
		}
	}

	return false;
}
	
bool TormentedSquareSurvival::IsFinalTeam(Party* pParty) const
{
	if ( !pParty )
	{
		return false;
	}

	if ( !pParty->GetMember(0)->GetPlayer() ||
		 !pParty->GetMember(1)->GetPlayer() )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i )
	{
		if ( this->GetRankingFinal(i)->GetID1() == pParty->GetMember(0)->GetPlayer()->GetGUID() &&
			 this->GetRankingFinal(i)->GetID2() == pParty->GetMember(1)->GetPlayer()->GetGUID() )
		{
			return true;
		}
	}

	return false;
}

bool TormentedSquareSurvival::IsSemiFinalPlayer(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i )
	{
		if ( this->GetRankingSemiFinal(i)->GetID1() == pPlayer->GetGUID() ||
			 this->GetRankingSemiFinal(i)->GetID2() == pPlayer->GetGUID() )
		{
			return true;
		}
	}

	return false;
}
	
bool TormentedSquareSurvival::IsFinalPlayer(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i )
	{
		if ( this->GetRankingFinal(i)->GetID1() == pPlayer->GetGUID() ||
			 this->GetRankingFinal(i)->GetID2() == pPlayer->GetGUID() )
		{
			return true;
		}
	}

	return false;
}

void TormentedSquareSurvival::AddScore(Player* pPlayer, int32 score)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 h = 0; h < this->GetGround()->GetTeamSize(); ++h )
	{
		for ( int32 n = 0; n < this->GetGround()->GetTeam(h)->GetPlayerSize(); ++n )
		{
			if ( this->GetGround()->GetTeam(h)->GetPlayer(n)->GetGUID() == pPlayer->GetGUID() &&
				 this->GetGround()->GetTeam(h)->GetPlayer(n)->GetPlayer() == pPlayer )
			{
				this->GetGround()->GetTeam(h)->GetPlayer(n)->IncreaseScore(score);
				this->GetGround()->GetTeam(h)->IncreaseScore(score);
				return;
			}
		}
	}
}

void TormentedSquareSurvival::DropReward(Player* pPlayer, Monster* pMonster)
{
	if ( !pPlayer || !pMonster )
	{
		return;
	}

	if ( this->GetGround()->GetStage() != 5 && this->GetGround()->GetStage() != 10 && this->GetGround()->GetStage() != 15 )
	{
		return;
	}

	if ( roll_chance_i(10) )
	{
		Item item(g_TormentedSquareSurvivalDropReward[this->GetCurrentGround()]);

		sItemMgr->ItemSerialCreate(pPlayer, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), item, false, true, true);
	}
}

void TormentedSquareSurvival::OnPlayerConnect(uint32 id, const char * name)
{
	for (int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_SEMI_FINAL; ++i)
	{
		if (this->GetRankingSemiFinal(i)->GetID1() == id)
		{
			this->GetRankingSemiFinal(i)->ResetName1();
			this->GetRankingSemiFinal(i)->SetName1(name, MAX_CHARACTER_LENGTH);
		}

		if (this->GetRankingSemiFinal(i)->GetID2() == id)
		{
			this->GetRankingSemiFinal(i)->ResetName2();
			this->GetRankingSemiFinal(i)->SetName2(name, MAX_CHARACTER_LENGTH);
		}
	}

	for (int32 i = 0; i < MAX_TORMENTED_SQUARE_RANKING_FINAL; ++i)
	{
		if (this->GetRankingFinal(i)->GetID1() == id)
		{
			this->GetRankingFinal(i)->ResetName1();
			this->GetRankingFinal(i)->SetName1(name, MAX_CHARACTER_LENGTH);
		}

		if (this->GetRankingFinal(i)->GetID2() == id)
		{
			this->GetRankingFinal(i)->ResetName2();
			this->GetRankingFinal(i)->SetName2(name, MAX_CHARACTER_LENGTH);
		}
	}

	if (this->GetRankingWinner()->GetID1() == id)
	{
		this->GetRankingWinner()->ResetName1();
		this->GetRankingWinner()->SetName1(name, MAX_CHARACTER_LENGTH);
	}

	if (this->GetRankingWinner()->GetID2() == id)
	{
		this->GetRankingWinner()->ResetName2();
		this->GetRankingWinner()->SetName2(name, MAX_CHARACTER_LENGTH);
	}
}