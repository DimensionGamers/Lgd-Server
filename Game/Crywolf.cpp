/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Crywolf.cpp"
*
*/
Crywolf::Crywolf()
{
	this->SetOccupationState(CRYWOLF_OCCUPATION_SUCCESS);

	this->GetNotifyTime()->Reset();
	this->GetBossTime()->Reset();
	this->SetState_None();
}

Crywolf::~Crywolf()
{
	this->ClearData();
}

void Crywolf::ClearData()
{
	for ( int32 i = 0; i < CRYWOLF_STATE_MAX; ++i )
		this->GetStateData(i)->SetDuration(0);

	for ( int32 i = 0; i < CRYWOLF_MVP_RANK_MAX; ++i )
		this->GetRankingData(i)->Reset();
}

void Crywolf::LoadData()
{
	this->ClearData();

	if ( !sGameServer->IsCrywolfEnabled() )
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Crywolf Data...");

	ReadScript file("EventData/Crywolf.txt", "rb");

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
					uint8 state = file.GetUInt8();

					file.GetToken();
					int32 duration = file.GetInt32();

					if ( state < CRYWOLF_STATE_MAX )
					{
						this->GetStateData(state)->SetDuration(duration * IN_MILLISECONDS);
					}
				} break;

			case 1:
				{
					uint8 rank = file.GetUInt8();

					file.GetToken();
					int32 score = file.GetInt32();

					file.GetToken();
					int32 experience = file.GetInt32();

					if ( rank < CRYWOLF_MVP_RANK_MAX )
					{
						this->GetRankingData(rank)->SetScore(score);
						this->GetRankingData(rank)->SetExperience(experience);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Crywolf Data Loaded...");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Crywolf::AltarContractRequest(uint8 * packet, Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	POINTER_PCT_LOG(CRYWOLF_ALTAR_REQUEST, lpMsg, packet, 0);

	if ( pPlayer->GetClass() != Character::FAIRY_ELF || pPlayer->GetTotalLevel() < sGameServer->crywolf_altar_min_level.get() )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Connection requires a Muse Elf level %d or higher", sGameServer->crywolf_altar_min_level.get());
		this->AltarContractResult(0, 0, lpMsg->GetAltar(), pPlayer);
		return;
	}

	Monster* mMonster = sObjectMgr->FindMonster(lpMsg->GetAltar());

	if ( !mMonster || !mMonster->IsPlaying() )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Connection error 1");
		return;
	}

	if ( !mMonster->AIGroup(CRYWOLF_ALTAR_AI) )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Connection error 2");
		return;
	}

	CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, mMonster->GetAI());

	if ( altar->SetPlayer(pPlayer) )
	{
		this->AltarContractResult(1, altar->GetState(), lpMsg->GetAltar(), pPlayer);
	}
	else
	{
		this->AltarContractResult(0, 0, lpMsg->GetAltar(), pPlayer);
	}
}

void Crywolf::AltarContractResult(uint8 result, uint8 state, uint16 altar, Player* pPlayer)
{
	CRYWOLF_ALTAR_RESULT pMsg(result, state, altar);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::PlusChaosRateRequest(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( this->GetOccupationState() != CRYWOLF_OCCUPATION_SUCCESS )
	{
		return;
	}

	CRYWOLF_BENEFIT_PLUS_CHAOS_RATE pMsg(sGameServer->GetCrywolfBenefitPlusRate());
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::sendStatueAndAltarData()
{
	if ( this->GetState() != CRYWOLF_STATE_READY && this->GetState() != CRYWOLF_STATE_START )
		return;

	static TCType dwTickCount = 0;

	if ( (MyGetTickCount() - dwTickCount) < 2000 )
		return;

	dwTickCount = MyGetTickCount();

	CRYWOLF_STATUE_AND_ALTAR_DATA pMsg;

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->AIGroup("crywolf_statue_ai") )
		{
			CrywolfStatueBasicAI * statue = AI_TO(CrywolfStatueBasicAI, pMonster->GetAI());
			pMsg.statue_hp = statue->GetPercent();
		}

		if ( pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
		{
			CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());
			pMsg.altar[altar->GetId()] = altar->GetContractLeft() | ( altar->GetState() << 4 );
		}
	}

	this->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::SendStateLeftTime()
{
	static TCType dwTickCount = 0;

	if ( (MyGetTickCount() - dwTickCount) < 20000 )
		return;

	dwTickCount = MyGetTickCount();

	CRYWOLF_LEFT_TIME pMsg(
		((this->GetRemain() / IN_MILLISECONDS) / MINUTE) / 24, 
		((this->GetRemain() / IN_MILLISECONDS) / MINUTE));

	this->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::sendMonsterInfo()
{
	static TCType dwTickCount = 0;

	if ( (MyGetTickCount() - dwTickCount) < 5000 )
		return;

	dwTickCount = MyGetTickCount();

	CRYWOLF_MONSTER_INFO pMsg;

	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() || !pMonster->IsLive() )
			continue;

		if ( pMonster->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE || pMonster->GetInstance() != -1 )
			continue;

		if ( pMonster->IsBalgass() )
		{
			pMsg.balgass_hp = int32(pMonster->PowerGet(POWER_LIFE));
		}
		else if ( pMonster->IsDarkElf() )
		{
			pMsg.dark_elf_count++;
		}
	}

	this->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::sendStateInfo()
{
	CRYWOLF_STATE_INFO pMsg(this->GetOccupationState(), this->GetState());

	this->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::sendStateInfo(Player* pPlayer)
{
	CRYWOLF_STATE_INFO pMsg(this->GetOccupationState(), this->GetState());

	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void Crywolf::sendPacket(uint8 * packet, uint16 size)
{
	World* pWorld = sWorldMgr->GetWorld(WORLD_CRYWOLF_FIRST_ZONE);

	if ( !pWorld )
		return;

	Player* pPlayer = nullptr;

	WorldPlayerMap const& wplayers = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(packet, size);
	}
}

void Crywolf::sendNoticeToPlayersOnMap(bool map_only, NoticeType type, const char * notice, ...)
{
	ARG(output_notice, notice);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE && map_only )
			continue;

		pPlayer->SendNoticeNormal(type, output_notice);
	}
}

void Crywolf::removeCommonMonsters()
{
	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->IsWolfAltar() || 
			 pMonster->IsWolfStatue() || 
			 pMonster->IsPriestDevin() || 
			 pMonster->IsWerewolfQuarrel() )
			continue;

		if (  pMonster->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE )
			continue;

		 pMonster->SetInstance(CRYWOLF_COMMON_INSTANCE);
	}
}

void Crywolf::restoreCommonMonsters()
{
	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->IsWolfAltar() || 
			 pMonster->IsWolfStatue() || 
			 pMonster->IsPriestDevin() || 
			 pMonster->IsWerewolfQuarrel() )
			continue;

		if (  pMonster->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE ||  pMonster->GetInstance() != CRYWOLF_COMMON_INSTANCE )
			continue;

		 pMonster->SetInstance(-1);
		 pMonster->Kill();
	}
}

void Crywolf::alterCommonNpc(uint8 value)
{
	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->IsNpc() )
			continue;

		if ( pMonster->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE )
			continue;

		if ( pMonster->IsWolfAltar() || 
			 pMonster->IsWolfStatue() || 
			 pMonster->IsPriestDevin() || 
			 pMonster->IsWerewolfQuarrel() )
			continue;

		if ( value == CRYWOLF_OCCUPATION_FAIL || value == CRYWOLF_OCCUPATION_FIGHT )
		{
			pMonster->SetInstance(CRYWOLF_COMMON_INSTANCE);
		}
		else
		{
			pMonster->SetInstance(-1);
		}
	}
}

void Crywolf::ResetPlayerScore()
{
	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->SetCrywolfScore(0);
	}
}

class CrywolfScoreSort
{
public:
	bool operator()(Player* pPlayer01, Player* pPlayer02)
	{
		SafeRAssert(pPlayer01, "pPlayer01 == nullptr", false);
		SafeRAssert(pPlayer02, "pPlayer02 == nullptr", false);

		bool bResult = pPlayer01->GetCrywolfScore() >= pPlayer02->GetCrywolfScore();

		if ( pPlayer01->GetCrywolfScore() == pPlayer02->GetCrywolfScore() )
		{
			bResult = pPlayer01->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience() < pPlayer02->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience();
		}

		return bResult;
	}
};

void Crywolf::NotifyPersonalRank()
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		if ( pPlayer->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE )
			continue;

		uint8 rank = this->GetRankingPosition(pPlayer);
		int64 experience = this->GetExperience(pPlayer, rank);

		CRYWOLF_PERSONAL_RANK pMsg(rank, experience);
		pPlayer->UpdateStatistic(STATISTIC_CRYWOLF_POINTS, pPlayer->GetCrywolfScore());
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "MvP rank: [%d]", rank);
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "MvP rank: experience points [%d]", static_cast<int32>(experience));
		pPlayer->GiveExperience(experience, true, true);
		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

uint8 Crywolf::GetRankingPosition(Player* pPlayer)
{
	uint8 ranking = 0;

	for ( int32 i = 0; i < CRYWOLF_MVP_RANK_MAX; ++i )
	{
		if ( pPlayer->GetCrywolfScore() < this->GetRankingData(i)->GetScore() )
			break;

		ranking = i;
	}

	return ranking;
}

int64 Crywolf::GetExperience(Player* pPlayer, uint8 ranking)
{
	if ( ranking >= CRYWOLF_MVP_RANK_MAX )
		return 0;

	int64 experience = this->GetRankingData(ranking)->GetExperience();

	if ( this->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL )
	{
		experience = (experience * 10) / 100;
	}

	return experience;
}

void Crywolf::NotifyHeroList()
{
	uint8 buffer[8192];
	POINTER_PCT(CRYWOLF_HERO_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(CRYWOLF_HERO_LIST_BODY, body, buffer, sizeof(CRYWOLF_HERO_LIST_HEAD));
	uint8 count = 0;
	std::set<Player*,CrywolfScoreSort> HeroSet;

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsPlaying() )
			continue;

		if ( !CR_MAP_RANGE(pPlayer->GetWorldId()) )
			continue;

		HeroSet.insert(pPlayer);
	}

	std::set<Player*, CrywolfScoreSort>::iterator _Itor = HeroSet.begin();

	for(int32 j = 0; j < 5 && _Itor != HeroSet.end(); j++, _Itor++)
	{
		Player* pPlayer = (*(_Itor));

		body[j].ranking = count;
		body[j].race = pPlayer->GetClass();
		body[j].score = pPlayer->GetCrywolfScore();
		memcpy(body[j].name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		count++;

		sLog->outInfo(LOG_CRYWOLF, "[Hero List] %s Score(%d) Ranking(%d)",
			pPlayer->BuildLog().c_str(), pPlayer->GetCrywolfScore(), count);

		if ( this->GetOccupationState() != CRYWOLF_OCCUPATION_FAIL )
		{
			if ( body[j].ranking <= 5 )
			{
				sItemMgr->ItemSerialCreate(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), Item(JEWEL::BLESS, 0, 1), true, true, true);
			}
		}
	}

	head->h.set(HEADCODE_CRYWOLF, SUBCODE_CRYWOLF_HERO_LIST, count * sizeof(CRYWOLF_HERO_LIST_BODY) + sizeof(CRYWOLF_HERO_LIST_HEAD));
	head->count = count;
	this->sendPacket(buffer, head->h.get_size());
}

void Crywolf::AddMonster(std::string const& ai_group)
{
	std::vector<int32> ai_group_list;
	ConvertAIGroupList(ai_group, ai_group_list);

	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_CRYWOLF);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (std::find(ai_group_list.begin(), ai_group_list.end(), event_monster->AIGroup) == ai_group_list.end())
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			monster->AddAdditionalDataInt(0, event_monster->crywolf.score);
			monster->AddToWorld();

			sLog->outInfo(LOG_CRYWOLF, "Added Monster [%u][%u]", monster->GetEntry(), monster->GetClass());
		}
	}
}

void Crywolf::ChangeAI(std::string const& ai_group, int32 order)
{
	std::vector<int32> ai_group_list;
	ConvertAIGroupList(ai_group, ai_group_list);

	for ( std::vector<int32>::iterator it = ai_group_list.begin(); it != ai_group_list.end(); ++it )
	{
		sObjectMgr->ChangeAIOrder(*it, order);
	}
}

void Crywolf::UpdateTime()
{
	if ( !sGameServer->IsCrywolfEnabled() )
	{
		return;
	}

	this->sendStatueAndAltarData();

	switch ( this->GetState() )
	{
	case CRYWOLF_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case CRYWOLF_STATE_NOTIFY_1:
		{
			this->ProcState_Notify_1();
		} break;

	case CRYWOLF_STATE_NOTIFY_2:
		{
			this->ProcState_Notify_2();
		} break;

	case CRYWOLF_STATE_READY:
		{
			this->ProcState_Ready();
		} break;

	case CRYWOLF_STATE_START:
		{
			this->ProcState_Start();
		} break;

	case CRYWOLF_STATE_END:
		{
			this->ProcState_End();
		} break;

	case CRYWOLF_STATE_END_CYCLE:
		{
			this->ProcState_EndCycle();
		} break;
	}
}

void Crywolf::SetState_None()
{
	this->SetState(CRYWOLF_STATE_NONE);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	this->SetChangeAI(false);
	this->SetBoss(0);
	this->ResetPlayerScore();
	this->sendStateInfo();

	sLog->outInfo(LOG_CRYWOLF, "SetState_None()");
}

void Crywolf::SetState_Notify_1()
{
	this->SetState(CRYWOLF_STATE_NOTIFY_1);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	sServerLink->NoticeSend(NOTICE_GLOBAL, "Balgass joined forces with Kundun's army and now prepares to attack Crywolf Fortress");

	this->sendStateInfo();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_NOTIFY_1, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_Notify_1()");
}
		
void Crywolf::SetState_Notify_2()
{
	this->SetState(CRYWOLF_STATE_NOTIFY_2);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	this->SetOccupationState(CRYWOLF_OCCUPATION_FIGHT);
	this->alterCommonNpc(CRYWOLF_OCCUPATION_FIGHT);
	this->removeCommonMonsters();
	sWorldMgr->statusChange(WORLD_CRYWOLF_FIRST_ZONE, CRYWOLF_OCCUPATION_FIGHT);

	sServerLink->NoticeSend(NOTICE_GLOBAL, "The army of Balgass is ready to attack Crywolf Fortress");

	this->sendStateInfo();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_NOTIFY_2, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_Notify_2()");
}
	
void Crywolf::SetState_Ready()
{
	this->SetState(CRYWOLF_STATE_READY);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	this->SetOccupationState(CRYWOLF_OCCUPATION_FIGHT);
	this->alterCommonNpc(CRYWOLF_OCCUPATION_FIGHT);
	this->removeCommonMonsters();
	sWorldMgr->statusChange(WORLD_CRYWOLF_FIRST_ZONE, CRYWOLF_OCCUPATION_FIGHT);
	this->altarPrepare();
	this->ResetPlayerScore();

	sServerLink->NoticeSend(NOTICE_GLOBAL, "The army of Balgass is marching towards Crywolf Fortress");

	this->AddMonster("1 2 3 4 6 7 8 9 10 11 12 13");
	this->sendStateInfo();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_READY, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_Ready()");
}
	
void Crywolf::SetState_Start()
{
	this->SetState(CRYWOLF_STATE_START);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	sServerLink->NoticeSend(NOTICE_GLOBAL, "The army of Balgass is marching towards Crywolf Fortress");

	if ( this->altarGetCount() == 0 )
	{
		this->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Without the protection of the Muses, the statue of holy wolf will be stolen");
		this->SetOccupationState(CRYWOLF_OCCUPATION_FAIL);
		this->SetState_End();
		return;
	}

	this->ChangeAI("1 2 3 4 6 7 8 9 10 11 12 13", 1);
	this->SetBoss(0);
	this->SetChangeAI(false);
	this->sendStateInfo();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_START, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_Start()");
}
	
void Crywolf::SetState_End()
{
	this->SetState(CRYWOLF_STATE_END);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	this->SetChangeAI(false);

	if ( this->GetBoss() == 1 )
	{
		this->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Balgass will be alive till the end of the battle");
		this->SetOccupationState(CRYWOLF_OCCUPATION_FAIL);
	}

	this->SetBoss(0);
	
	if ( this->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Defending has failed! The statue of holy wolf has been taken over by Balgass");
		sLog->outInfo(LOG_CRYWOLF, "Failed");
	}
	else
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Defending has been successful! The statue of holy wolf has been protected");
		sLog->outInfo(LOG_CRYWOLF, "Success");
	}

	sServerLink->CrywolfDataSave();

	this->ChangeAI("1 2 3 4 6 7 8 9 10 11 12 13", 0);

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
		{
			CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());
			
			Player* pPlayer = altar->GetPlayer();

			if ( pPlayer )
			{
				pPlayer->IncreaseCrywolfScore(sGameServer->GetCrywolfAltarScore());

				if ( this->GetOccupationState() != CRYWOLF_OCCUPATION_FAIL )
				{
					sItemMgr->ItemSerialCreate(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), Item(JEWEL::BLESS, 0, 1), true, true, true);
				}

				pPlayer->ClearAllBuff(0);
			}

			altar->Reset();
		}
	}

	this->NotifyPersonalRank();
	this->sendStateInfo();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_END, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_End()");
}
	
void Crywolf::SetState_EndCycle()
{
	this->SetState(CRYWOLF_STATE_END_CYCLE);
	this->RunTime(this->GetStateData(this->GetState())->GetDuration());
	this->GetNotifyTime()->Start();
	this->GetBossTime()->Start();

	sWorldMgr->statusChange(WORLD_CRYWOLF_FIRST_ZONE, this->GetOccupationState());
	this->alterCommonNpc(this->GetOccupationState());

	this->restoreCommonMonsters();

	this->NotifyHeroList();
	this->ResetPlayerScore();

	sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_END_CYCLE, this->GetOccupationState());

	sLog->outInfo(LOG_CRYWOLF, "SetState_EndCycle()");
}

void Crywolf::ProcState_None()
{

}
	
void Crywolf::ProcState_Notify_1()
{
	if ( this->IsMinuteElapsed(1) )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Balgass joined forces with Kundun's army and now prepares to attack Crywolf Fortress");
	}

	if ( this->TimePassed() )
	{
		this->SetState_Notify_2();
	}
}
	
void Crywolf::ProcState_Notify_2()
{
	if ( this->IsMinuteElapsed(1) )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Elf's can make contract in the wolf altar in %d minute(s)", (this->GetCurrentMinute() + 1));
	}

	if ( this->TimePassed() )
	{
		this->SetState_Ready();
	}
}
	
void Crywolf::ProcState_Ready()
{
	if ( this->IsMinuteElapsed(1) )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Balgass's army will attack in %d minute(s)", (this->GetCurrentMinute() + 1));
	}

	if ( this->TimePassed() )
		this->SetState_Start();
}
	
void Crywolf::ProcState_Start()
{
	if ( this->altarGetCount() == 0 )
	{
		sLog->outInfo(LOG_CRYWOLF, "ProcState_START() No Contracter Altar");
		this->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Without the protection of the Muses, the statue of holy wolf will be stolen");
		this->SetOccupationState(CRYWOLF_OCCUPATION_FAIL);
		this->SetState_End();
		return;
	}

	this->sendMonsterInfo();
	this->SendStateLeftTime();

	if ( this->GetNotifyTime()->Elapsed(sGameServer->crywolf_aichange_time.get()) && !this->IsChangeAI() )
	{
		this->SetChangeAI(true);
		this->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Balgass's army starts attacking the statue of holy wolf.");
		this->ChangeAI("1 2 3 4 6 7 8 9 10 11 12 13", 2);
	}

	if ( this->GetBossTime()->Elapsed(sGameServer->crywolf_balgass_start_time.get()) && !this->GetBoss() )
	{
		this->SetBoss(1);
		this->sendNoticeToPlayersOnMap(true, NOTICE_GLOBAL, "Balgass is coming! Protect the statue of holy wolf.");
		this->AddMonster("5");
	}

	if ( this->TimePassed() )
		this->SetState_End();
}
	
void Crywolf::ProcState_End()
{
	if ( this->TimePassed() )
		this->SetState_EndCycle();
}
	
void Crywolf::ProcState_EndCycle()
{
	if ( this->TimePassed() )
	{
		this->SetState_None();
		sServerLink->EventStateUpdate(EVENT_CRYWOLF, CRYWOLF_STATE_NONE, this->GetOccupationState());
	}
}

uint8 Crywolf::altarGetCount()
{
	uint8 count = 0;

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
			continue;

		CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());

		if ( altar->GetState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		++count;
	}

	return count;
}

uint32 Crywolf::altarGetHP()
{
	uint32 count = 0;

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
			continue;

		CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());

		if ( altar->GetState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		if ( !altar->GetPlayer() )
		{
			continue;
		}

		count += altar->GetPlayer()->PowerGet(POWER_LIFE);
	}

	return count;
}
	
uint32 Crywolf::altarGetMaxHP()
{
	uint32 count = 0;

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
			continue;

		CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());

		if ( altar->GetState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		if ( !altar->GetPlayer() )
		{
			continue;
		}

		count += altar->GetPlayer()->PowerGetTotal(POWER_LIFE);
	}

	return count;
}

void Crywolf::altarPrepare()
{
	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup(CRYWOLF_ALTAR_AI) )
			continue;

		CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());

		altar->Prepare();
	}
}

bool Crywolf::IsInAltar(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !CR_MAP_RANGE(pPlayer->GetWorldId()) )
	{
		return false;
	}

	if ( sCrywolf->GetState() != CRYWOLF_STATE_READY && sCrywolf->GetState() != CRYWOLF_STATE_START )
	{
		return false;
	}

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( !pMonster->AIGroup(std::string(CRYWOLF_ALTAR_AI)) )
			continue;

		CrywolfAltarBasicAI * altar = AI_TO(CrywolfAltarBasicAI, pMonster->GetAI());

		if ( altar->GetPlayer() == pPlayer )
			return true;
	}

	return false;
}