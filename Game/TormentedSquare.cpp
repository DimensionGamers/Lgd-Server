TormentedSquareGround::TormentedSquareGround()
{

}

TormentedSquareGround::~TormentedSquareGround()
{

}

void TormentedSquareGround::Update()
{
	switch ( this->GetState() )
	{
	case TORMENTED_SQUARE_GROUND_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case TORMENTED_SQUARE_GROUND_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case TORMENTED_SQUARE_GROUND_STATE_STANDBY:
		{
			this->ProcState_Standby();
		} break;

	case TORMENTED_SQUARE_GROUND_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case TORMENTED_SQUARE_GROUND_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void TormentedSquareGround::SetState_None()
{
	this->SetState(TORMENTED_SQUARE_GROUND_STATE_NONE);
	this->SetStage(0);
	this->SetSecondTick(GetTickCount());
	this->GetTime()->Reset();
	this->SetRankingSent(false);
	this->PlayerMoveOut();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareGround::SetState_Open()
{
	this->SetState(TORMENTED_SQUARE_GROUND_STATE_OPEN);
	this->SetStage(0);
	this->SetSecondTick(GetTickCount());
	this->SetRankingSent(false);
	this->GetTime()->Reset();

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareGround::SetState_Standby()
{
	this->SetState(TORMENTED_SQUARE_GROUND_STATE_STANDBY);
	this->IncreaseStage(1);
	this->GetStageStandbyTime();
	this->SetRankingSent(false);
	this->SendNotification(4);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareGround::SetState_Playing()
{
	this->SetState(TORMENTED_SQUARE_GROUND_STATE_PLAYING);
	this->SetRankingSent(false);
	this->GetStageTime();
	this->AddMonster();
	this->SendNotification(2);
	
	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareGround::SetState_End()
{
	this->SetState(TORMENTED_SQUARE_GROUND_STATE_END);
	this->GetTime()->Start(TORMENTED_SQUARE_GROUND_END_TIME * IN_MILLISECONDS);
	this->SendNotification(3);
	this->SetRankingSent(false);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquareGround::ProcState_None()
{

}

void TormentedSquareGround::ProcState_Open()
{
	this->UpdateTeamQueue();
}

void TormentedSquareGround::ProcState_Standby()
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

void TormentedSquareGround::ProcState_Playing()
{
	this->PlayerUpdate();

	if ( this->SecondPassed() )
	{
		this->SendTime();
	}

	if ( this->GetTime()->Elapsed() )
	{
		if ( this->GetStage() >= MAX_TORMENTED_SQUARE_ROUND )
		{
			this->SetState_End();
		}
		else
		{
			this->SetState_Standby();
		}
	}
}

void TormentedSquareGround::ProcState_End()
{
	if ( this->GetTime()->GetElapsed() > 5000 && !this->IsRankingSent() )
	{
		this->SetRankingSent(true);
		this->CalculateTeamScore();
	}

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_None();
	}
}

void TormentedSquareGround::UpdateTeamQueue()
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

		if ( (GetTickCount() - this->GetTeam(h)->GetQueueTime()) > 40000 )
		{
			this->GetTeam(h)->Reset();
			continue;
		}
	}

	this->PlayerUpdate();
}

void TormentedSquareGround::GetStageStandbyTime()
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_ROUND )
	{
		this->GetTime()->Start(this->GetStageData(this->GetStage() - 1)->GetStandBy() * IN_MILLISECONDS);
	}
	else
	{
		this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	}
}

void TormentedSquareGround::GetStageTime()
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_ROUND )
	{
		this->GetTime()->Start(this->GetStageData(this->GetStage() - 1)->GetDuration() * IN_MILLISECONDS);
	}
	else
	{
		this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	}
}

int32 TormentedSquareGround::GetStageScore() const
{
	if ( this->GetStage() > 0 && this->GetStage() <= MAX_TORMENTED_SQUARE_ROUND )
	{
		return this->GetStageData(this->GetStage() - 1)->GetScore();
	}
	else
	{
		return 0;
	}
}

void TormentedSquareGround::AddMonster()
{
	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_TORMENTED_SQUARE);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (event_monster->tormented_square.ground != GetGround())
			continue;

		if (event_monster->tormented_square.stage != (GetStage() - 1))
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnType(event_monster->tormented_square.boss != 0 ? GAME_OBJECT_RESPAWN_DELETE : GAME_OBJECT_RESPAWN_NORMAL);
			monster->SetEventGround(event_monster->tormented_square.ground);
			monster->SetEventStage(event_monster->tormented_square.stage);
			monster->SetMoveDistance(60);
			monster->AddToWorld();

			if (monster->GetClass() == 709 ||
				monster->GetClass() == 710 ||
				monster->GetClass() == 711 ||
				monster->GetClass() == 712)
			{
				TORMENTED_SQUARE_CHAOS_GOBLIN pMsg;
				this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
			}
		}
	}
}

int32 TormentedSquareGround::PlayerGetCount() const
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

void TormentedSquareGround::PlayerUpdate()
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

void TormentedSquareGround::PlayerAddScore(Player* pKiller, bool boss)
{
	if ( !pKiller )
	{
		return;
	}

	TormentedSquareTeam * pTeam = nullptr;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		if ( pTeam )
		{
			break;
		}

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			if ( this->GetTeam(h)->GetPlayer(i)->GetGUID() == pKiller->GetGUID() )
			{
				pTeam = this->GetTeam(h);
				break;
			}
		}
	}

	if ( !pTeam )
	{
		return;
	}
		
	int32 count = 0;

	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		if ( !pTeam->GetPlayer(i)->GetGUID() )
		{
			continue;
		}

		++count;
	}

	if ( count <= 0 || count > pTeam->GetPlayerSize() )
	{
		return;
	}

	int32 add_score = boss ? g_TormentedSquarePoints[count - 1].boss_points : g_TormentedSquarePoints[count - 1].monster_points;

	pTeam->IncreaseScore(add_score);

	for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
	{
		if ( pTeam->GetPlayer(i)->GetGUID() == pKiller->GetGUID() )
		{
			pTeam->GetPlayer(i)->IncreaseScore(add_score);
			break;
		}
	}
}

void TormentedSquareGround::PlayerMoveOut()
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

void TormentedSquareGround::SendPacket(uint8 * Packet, uint16 size)
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

void TormentedSquareGround::SendTeamInfo()
{
	uint8 buffer[8192];
	POINTER_PCT(TORMENTED_SQUARE_TEAM_HEAD, head, buffer, 0);
	POINTER_PCT(TORMENTED_SQUARE_TEAM_BODY, body, buffer, sizeof(TORMENTED_SQUARE_TEAM_HEAD));
	head->unk1[0] = 0xFF;
	head->unk1[1] = 0xFF;
	head->unk1[2] = 0xFF;
	head->count = 0;
	head->unk2 = 0;
	head->unk3[0] = 0xFF;
	head->unk3[1] = 0xFF;
	head->unk3[2] = 0xFF;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		body[head->count].id = h;
		body[head->count].unk[0] = 3;
		/*body[head->count].unk[0] = 0;
		body[head->count].unk[1] = 0;
		body[head->count].unk[2] = 0;
		body[head->count].unk[3] = 0;
		body[head->count].unk[4] = 0;*/

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			memset(body[head->count].name[i], 0, MAX_CHARACTER_LENGTH + 1);

			Player* pPlayer = this->GetTeam(h)->GetPlayer(i)->GetPlayer();

			if ( pPlayer )
			{
				memcpy(body[head->count].name[i], pPlayer->GetName(), MAX_CHARACTER_LENGTH);
			}
		}

		++head->count;
	}

	head->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_PLAY_RANKING, sizeof(TORMENTED_SQUARE_TEAM_HEAD) + (head->count * sizeof(TORMENTED_SQUARE_TEAM_BODY)));

	this->SendPacket(buffer, head->h.get_size());
}

void TormentedSquareGround::SendNotification(uint8 id)
{
	TORMENTED_SQUARE_NOTIFICATION pMsg(id);

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void TormentedSquareGround::SendTime()
{
	TORMENTED_SQUARE_TIME pMsg;
	pMsg.remain_time = this->GetTime()->GetRemain() / IN_MILLISECONDS;
	pMsg.unk1 = 0;
	pMsg.unk2 = 0;
	
	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		pMsg.score[h] = this->GetTeam(h)->GetScore();
	}

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

uint8 TormentedSquareGround::GetEmptyTeam() const
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

bool Sort(TormentedSquareTeam* pTeam01, TormentedSquareTeam* pTeam02)
{
	if ( !pTeam01 )
	{
		return false;
	}

	if ( !pTeam02 )
	{
		return true;
	}
	
	return (pTeam01->GetScore() >= pTeam02->GetScore());
}

void TormentedSquareGround::CalculateTeamScore()
{
	int32 team_count = 0;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		int32 count = 0;

		for ( int32 i = 0; i < this->GetTeam(h)->GetPlayerSize(); ++i )
		{
			if ( !this->GetTeam(h)->GetPlayer(i)->GetGUID() )
			{
				continue;
			}

			++count;
		}

		if ( count <= 0 || count > this->GetTeam(h)->GetPlayerSize() )
		{
			continue;
		}

		this->GetTeam(h)->IncreaseScore(g_TormentedSquarePoints[count - 1].final_points);

		++team_count;
	}

	if ( team_count <= 0 || team_count > this->GetTeamSize() )
	{
		return;
	}

	int32 start_reward = this->GetTeamSize() - team_count;

	if ( start_reward < 0 )
	{
		start_reward = 0;
	}

	std::vector<TormentedSquareTeam*> team_list;

	for ( int32 h = 0; h < this->GetTeamSize(); ++h )
	{
		if ( !this->GetTeam(h)->IsInUse() )
		{
			continue;
		}

		team_list.push_back(this->GetTeam(h));
	}

	std::sort(team_list.begin(), team_list.end(), Sort);

	uint8 buffer[8192];
	POINTER_PCT(TORMENTED_SQUARE_RANKING_HEAD, head, buffer, 0);
	POINTER_PCT(TORMENTED_SQUARE_RANKING_BODY, body, buffer, sizeof(TORMENTED_SQUARE_RANKING_HEAD));
	head->count = 0;
	head->type = 0;

	for ( std::vector<TormentedSquareTeam*>::const_iterator it = team_list.begin(); it != team_list.end(); ++it )
	{
		TormentedSquareTeam* pTeam = *it;

		if ( !pTeam )
		{
			continue;
		}

		body[head->count].ranking = head->count;
		body[head->count].id = 0;
		memcpy(body[head->count].team_name, pTeam->GetPlayer(0)->GetName(), MAX_CHARACTER_LENGTH + 1);
		++head->count;
	}

	head->h.set(HEADCODE_TORMENTED_SQUARE, SUBCODE_TORMENTED_SQUARE_FINAL_RANKING, sizeof(TORMENTED_SQUARE_RANKING_HEAD) + (head->count * sizeof(TORMENTED_SQUARE_RANKING_BODY)));

	for ( int32 i = 0; i < head->count; ++i )
	{
		STRING_SAFE(team_name, MAX_CHARACTER_LENGTH + 1);
		memcpy(team_name, body[i].team_name, MAX_CHARACTER_LENGTH);

		for ( std::vector<TormentedSquareTeam*>::const_iterator it = team_list.begin(); it != team_list.end(); ++it )
		{
			if ( !memcmp((*it)->GetPlayer(0)->GetName(), team_name, MAX_CHARACTER_LENGTH) )
			{
				for ( int32 h = 0; h < (*it)->GetPlayerSize(); ++h )
				{
					if ( !(*it)->GetPlayer(h)->GetGUID() )
					{
						continue;
					}

					Player* pPlayer = (*it)->GetPlayer(h)->GetPlayer();

					if ( !pPlayer )
					{
						continue;
					}

					memcpy(body[i].team_name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
					pPlayer->sendPacket(buffer, head->h.get_size());

					if ( this->GetReward(start_reward + i) != 0 )
					{
						sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(this->GetReward(start_reward + i)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::TORMENTED_SQUARE_BATTLE);
					}
				}

				break;
			}
		}

		memcpy(body[i].team_name, team_name, MAX_CHARACTER_LENGTH);
	}

	team_list.clear();



	//this->SendPacket(buffer, head->h.get_size());
}

TormentedSquare::TormentedSquare()
{
	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		this->GetGround(i)->SetGround(i);
		this->GetGround(i)->SetRespawnGate(TORMENTED_SQUARE_GATE);
		this->GetGround(i)->SetWorldID(g_TormentedSquareZone[i].world);
		this->GetGround(i)->SetEnterGate(g_TormentedSquareZone[i].gate);

		this->GetGround(i)->CreateTeam(TORMENTED_SQUARE_MAX_TEAM);

		for ( int32 h = 0; h < this->GetGround(i)->GetTeamSize(); ++h )
		{
			this->GetGround(i)->GetTeam(h)->CreatePlayer(MAX_PARTY_MEMBERS);
			this->GetGround(i)->GetTeam(h)->Reset();
		}
	}

	this->SetState_None();
}

TormentedSquare::~TormentedSquare()
{
	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		this->GetGround(i)->EraseTeam();
	}
}

void TormentedSquare::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Tormented Square Stage...");

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		for ( int32 h = 0; h < MAX_TORMENTED_SQUARE_ROUND; ++h )
		{
			this->GetGround(i)->GetStageData(h)->Reset();
		}
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_tormented_square_stage");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			int32 loop = 0;

			uint8 ground = fields[loop++].GetUInt8();
			uint8 stage = fields[loop++].GetUInt8();

			if ( ground < MAX_TORMENTED_SQUARE_GROUND && stage < MAX_TORMENTED_SQUARE_ROUND )
			{
				this->GetGround(ground)->GetStageData(stage)->SetStandBy(fields[loop++].GetInt32());
				this->GetGround(ground)->GetStageData(stage)->SetDuration(fields[loop++].GetInt32());
				this->GetGround(ground)->GetStageData(stage)->SetScore(0);

				count++;
			}
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u tormented square stage definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void TormentedSquare::LoadRanking()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Tormented Square Ranking...");

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		this->GetGround(i)->ResetReward(0);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM event_tormented_square_ranking");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			int32 loop = 0;

			uint8 ground = fields[loop++].GetUInt8();
			uint8 rank = fields[loop++].GetUInt8(); 

			if ( ground < MAX_TORMENTED_SQUARE_GROUND && rank < TORMENTED_SQUARE_MAX_TEAM )
			{
				this->GetGround(ground)->SetReward(rank, fields[loop++].GetUInt16());

				count++;
			}
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u tormented square ranking definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void TormentedSquare::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_NOT_OPEN);
		return;
	}

	POINTER_PCT(TORMENTED_SQUARE_ENTER, lpMsg, Packet, 0);

	uint8 enter_ground = this->GetEnterGround(pPlayer->GetTotalLevel(), pPlayer->IsMaster());

	if ( (enter_ground != lpMsg->ground) || (enter_ground >= MAX_TORMENTED_SQUARE_GROUND) )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_OK);
		return;
	}

	if ( this->IsIn(pPlayer, false) )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_OK);
		return;
	}

	int32 add_count = 0;

	Party* pParty = pPlayer->GetParty();

	if ( pParty )
	{
		if ( pParty->GetLeader() != pPlayer )
		{
			this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_PARTY_LEADER);
			return;
		}

		for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		{
			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( !pMember )
			{
				continue;
			}

			if ( this->IsIn(pMember, false) )
			{
				this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_OK);
				return;
			}

			if ( pMember->GetInterfaceState()->GetID() != InterfaceData::None )
			{
				this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_TEAM_MEMBER_JOINED_ANOTHER_ACTIVITY);
				return;
			}

			uint8 member_enter_ground = this->GetEnterGround(pMember->GetTotalLevel(), pMember->IsMaster());

			if ( member_enter_ground != enter_ground )
			{
				this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_TEAM_MEMBER_WRONG_LEVEL);
				return;
			}

			add_count++;	
		}
	}
	else
	{
		add_count = 1;
	}

	if ( (this->GetGround(lpMsg->ground)->PlayerGetCount() + add_count) > TORMENTED_SQUARE_MAX_PLAYER )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_MAXIMUM_PLAYERS);
		return;
	}

	uint8 team = this->GetGround(enter_ground)->GetEmptyTeam();

	if ( team == uint8(-1) )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_MAXIMUM_PLAYERS);
		return;
	}

	this->GetGround(enter_ground)->GetTeam(team)->SetInUse(true);
	this->GetGround(enter_ground)->GetTeam(team)->SetQueued(true);
	this->GetGround(enter_ground)->GetTeam(team)->SetQueueTime(GetTickCount());
	this->GetGround(enter_ground)->GetTeam(team)->SetPartyID(pPlayer->GetPartyID());

	if ( pParty )
	{
		for ( int32 i = 0; i < this->GetGround(enter_ground)->GetTeam(team)->GetPlayerSize(); ++i )
		{
			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if ( !pMember )
			{
				continue;
			}

			this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(i)->SetQueueState(TORMENTED_SQUARE_QUEUE_STATE_STANDBY);
			this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(i)->SetGUID(pMember->GetGUID());
			this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(i)->SetName(pMember->GetName());
			this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(i)->SetPlayer(pMember);

			this->EnterResult(pMember, TORMENTED_SQUARE_RESULT_QUEUE, this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->GetName());

			TORMENTED_SQUARE_NOTIFICATION pMsg(0);
			pMember->SEND_PCT(pMsg);

			sLog->outInfo("tormentedsquare", "%s -- [%u][%s] -- Ground[%u] Team[%u] Slot[%u]", __FUNCTION__, pMember->GetEntry(), pMember->GetName(), enter_ground, team, i);
		}
	}
	else
	{
		this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->SetQueueState(TORMENTED_SQUARE_QUEUE_STATE_STANDBY);
		this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->SetGUID(pPlayer->GetGUID());
		this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->SetName(pPlayer->GetName());
		this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->SetPlayer(pPlayer);

		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_QUEUE, this->GetGround(enter_ground)->GetTeam(team)->GetPlayer(0)->GetName());

		TORMENTED_SQUARE_NOTIFICATION pMsg(0);
		pPlayer->SEND_PCT(pMsg);

		sLog->outInfo("tormentedsquare", "%s -- [%u][%s] -- Ground[%u] Team[%u] Slot[%u]", __FUNCTION__, pPlayer->GetEntry(), pPlayer->GetName(), enter_ground, team, 0);
	}
}

void TormentedSquare::EnterResult(Player* pPlayer, uint8 result, const char * team)
{
	TORMENTED_SQUARE_ENTER_RESULT pMsg(result, team);
	pPlayer->SEND_PCT(pMsg);
}

void TormentedSquare::QueueResult(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN )
	{
		this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_NOT_OPEN);
		return;
	}

	POINTER_PCT(TORMENTED_SQUARE_QUEUE, lpMsg, Packet, 0);

	for ( int32 n = 0; n < MAX_TORMENTED_SQUARE_GROUND; ++n )
	{
		for ( int32 h = 0; h < this->GetGround(n)->GetTeamSize(); ++h )
		{
			if ( !this->GetGround(n)->GetTeam(h)->IsInUse() )
			{
				continue;
			}

			if ( !this->GetGround(n)->GetTeam(h)->IsQueued() )
			{
				continue;
			}

			for ( int32 i = 0; i < this->GetGround(n)->GetTeam(h)->GetPlayerSize(); ++i )
			{
				if ( this->GetGround(n)->GetTeam(h)->GetPlayer(i)->GetPlayer() == pPlayer &&
					 this->GetGround(n)->GetTeam(h)->GetPlayer(i)->GetGUID() == pPlayer->GetGUID() )
				{
					if ( this->GetGround(n)->GetTeam(h)->GetPlayer(i)->GetQueueState() == TORMENTED_SQUARE_QUEUE_STATE_STANDBY )
					{
						this->GetGround(n)->GetTeam(h)->GetPlayer(i)->SetQueueState(lpMsg->result == 0 ? TORMENTED_SQUARE_QUEUE_STATE_ACCEPT: TORMENTED_SQUARE_QUEUE_STATE_CANCEL);
					}

					this->SendQueueResult(pPlayer, 0, this->GetGround(n)->GetTeam(h)->GetPlayer(0)->GetName());

					this->QueueResultProcess(n, this->GetGround(n)->GetTeam(h), this->GetGround(n)->GetTeam(h)->GetPlayer(i)->GetQueueState());
					return;
				}
			}
		}
	}
}

void TormentedSquare::SendQueueResult(Player* pPlayer, uint8 result, const char * team)
{
	TORMENTED_SQUARE_QUEUE_RESULT pMsg(result, team);
	pPlayer->SEND_PCT(pMsg);
}

void TormentedSquare::QueueResultProcess(uint8 ground, TormentedSquareTeam * pTeam, TormentedSquareQueueState state)
{
	if ( !pTeam )
	{
		return;
	}

	if ( ground >= MAX_TORMENTED_SQUARE_GROUND )
	{
		return;
	}

	if ( state != TORMENTED_SQUARE_QUEUE_STATE_ACCEPT )
	{
		for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
		{
			Player* pPlayer = pTeam->GetPlayer(i)->GetPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			this->EnterResult(pPlayer, TORMENTED_SQUARE_RESULT_TEAM_MEMBER_NOT_ACCEPTED);
		}

		pTeam->Reset();
	}
	else
	{
		bool move = true;

		for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
		{
			if ( !pTeam->GetPlayer(i)->GetGUID() )
			{
				continue;
			}

			if ( pTeam->GetPlayer(i)->GetQueueState() == TORMENTED_SQUARE_QUEUE_STATE_NONE )
			{
				continue;
			}

			if ( pTeam->GetPlayer(i)->GetQueueState() != TORMENTED_SQUARE_QUEUE_STATE_ACCEPT )
			{
				move = false;
			}
		}

		if ( move )
		{
			pTeam->SetQueued(false);

			for ( int32 i = 0; i < pTeam->GetPlayerSize(); ++i )
			{
				Player* pPlayer = pTeam->GetPlayer(i)->GetPlayer();

				if ( !pPlayer )
				{
					continue;
				}

				pPlayer->SetEventId(EVENT_TORMENTED_SQUARE);
				pPlayer->SetEventGround(ground);
				pPlayer->MoveToGate(this->GetGround(ground)->GetEnterGate());
			}
		}
	}
}

uint8 TormentedSquare::GetEnterGround(int16 level, bool master)
{
	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		if ( master != g_TormentedSquareLevel[i].master )
		{
			continue;
		}

		if ( level >= g_TormentedSquareLevel[i].min_level && level <= g_TormentedSquareLevel[i].max_level )
		{
			return i;
		}
	}

	return MAX_TORMENTED_SQUARE_GROUND;
}

void TormentedSquare::Start(int32 notify)
{
	if ( !sGameServer->IsTormentedSquareEnabled() )
	{
		return;
	}

	if ( this->GetState() != TORMENTED_SQUARE_STATE_NONE )
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
	
void TormentedSquare::Update()
{
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

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		this->GetGround(i)->Update();
	}
}

void TormentedSquare::SetState_None()
{
	this->SetState(TORMENTED_SQUARE_STATE_NONE);
	this->GetTime()->Reset();
	this->SetCurrentMinute(-1);
	this->SetNotifyTime(0);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquare::SetState_Notify()
{
	this->SetState(TORMENTED_SQUARE_STATE_NOTIFY);
	this->GetTime()->Start(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquare::SetState_Open()
{
	this->SetState(TORMENTED_SQUARE_STATE_OPEN);
	this->GetTime()->Start(TORMENTED_SQUARE_OPEN_TIME * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		this->GetGround(i)->SetState_Open();
	}

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquare::SetState_Playing()
{
	this->SetState(TORMENTED_SQUARE_STATE_PLAYING);

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		if ( this->GetGround(i)->PlayerGetCount() >= TORMENTED_SQUARE_MIN_PLAYER )
		{
			for ( int32 h = 0; h < this->GetGround(i)->GetTeamSize(); ++h )
			{
				if ( !this->GetGround(i)->GetTeam(h)->IsInUse() )
				{
					continue;
				}

				if ( !this->GetGround(i)->GetTeam(h)->IsQueued() )
				{
					continue;
				}

				this->GetGround(i)->GetTeam(h)->Reset();
			}

			this->GetGround(i)->SetState_Standby();
			this->GetGround(i)->SendTeamInfo();
			this->GetGround(i)->SendNotification(1);
		}
		else
		{
			this->GetGround(i)->SetState_None();
		}
	}

	sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "[Tormented Square] has been closed.");

	sLog->outInfo("tormentedsquare", "%s", __FUNCTION__);
}

void TormentedSquare::ProcState_None()
{

}

void TormentedSquare::ProcState_Notify()
{
	this->SendRemainMinutes("[Tormented Square] is open after %d mins.", 5, 15);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Open();
	}
}

void TormentedSquare::ProcState_Open()
{
	this->SendRemainMinutes("[Tormented Square] is closed after %d mins.", 1, 5);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void TormentedSquare::ProcState_Playing()
{
	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != TORMENTED_SQUARE_GROUND_STATE_NONE )
		{
			return;
		}
	}

	this->SetState_None();
}

void TormentedSquare::RemovePlayer(Player* pPlayer)
{
	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		for ( int32 h = 0; h < this->GetGround(i)->GetTeamSize(); ++h )
		{
			for ( int32 n = 0; n < this->GetGround(i)->GetTeam(h)->GetPlayerSize(); ++n )
			{
				if ( this->GetGround(i)->GetTeam(h)->GetPlayer(n)->GetGUID() == pPlayer->GetGUID() )
				{
					this->GetGround(i)->GetTeam(h)->GetPlayer(n)->Reset();
					return;
				}
			}
		}
	}
}
	
bool TormentedSquare::IsIn(Player* pPlayer, bool add)
{
	if ( this->GetState() != TORMENTED_SQUARE_STATE_OPEN &&
		 this->GetState() != TORMENTED_SQUARE_STATE_PLAYING )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != TORMENTED_SQUARE_GROUND_STATE_STANDBY &&
			 this->GetGround(i)->GetState() != TORMENTED_SQUARE_GROUND_STATE_PLAYING )
		{
			continue;
		}

		for ( int32 h = 0; h < this->GetGround(i)->GetTeamSize(); ++h )
		{
			for ( int32 n = 0; n < this->GetGround(i)->GetTeam(h)->GetPlayerSize(); ++n )
			{
				if ( this->GetGround(i)->GetTeam(h)->GetPlayer(n)->GetGUID() == pPlayer->GetGUID() )
				{
					if ( add )
					{
						this->GetGround(i)->GetTeam(h)->GetPlayer(n)->SetPlayer(pPlayer);
				
						pPlayer->SetEventId(EVENT_TORMENTED_SQUARE);
						pPlayer->SetEventGround(i);
					}
					return true;
				}
			}
		}
	}

	return false;
}

void TormentedSquare::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
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