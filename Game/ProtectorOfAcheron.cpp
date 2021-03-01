ProtectorOfAcheron::ProtectorOfAcheron()
{
	this->SetState_None();
}

ProtectorOfAcheron::~ProtectorOfAcheron()
{
	LIST_CLEAR(ProtectorOfAcheronBossList::iterator, this->boss_list);
}

void ProtectorOfAcheron::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Protector of Acheron Settings...");

	LIST_CLEAR(ProtectorOfAcheronBossList::iterator, this->boss_list);

	ReadScript file("EventData/ProtectorOfAcheron.txt", "rb");

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
					ProtectorOfAcheronBoss* pData = new ProtectorOfAcheronBoss;

					pData->SetTimeMin(file.GetUInt16());
					file.GetToken();	pData->SetTimeMax(file.GetUInt16());
					file.GetToken();	pData->SetCount(file.GetUInt16());
					file.GetToken();	pData->SetPeriod(file.GetUInt32());
					file.GetToken();	pData->SetMaxCount(file.GetInt32());

					this->boss_list.push_back(pData);
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Protector of Acheron Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ProtectorOfAcheron::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->EnterResult(pPlayer, 0);
		return;
	}

	if ( this->GetState() != PROTECTOR_OF_ACHERON_STATE_PLAYING )
	{
		this->EnterResult(pPlayer, 0);
		return;
	}

	this->EnterResult(pPlayer, 0);
	pPlayer->MoveToGate(426);
}

void ProtectorOfAcheron::EnterResult(Player* pPlayer, uint8 result)
{
	PROTECTOR_OF_ACHERON_ENTER_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void ProtectorOfAcheron::Start()
{
	if ( !sGameServer->IsProtectorOfAcheronEnabled() )
	{
		return;
	}

	if ( this->GetState() != PROTECTOR_OF_ACHERON_STATE_NONE )
	{
		return;
	}

	this->SetState_Playing();
}

void ProtectorOfAcheron::Update()
{
	if ( !sGameServer->IsProtectorOfAcheronEnabled() )
	{
		return;
	}

	switch ( this->GetState() )
	{
	case PROTECTOR_OF_ACHERON_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case PROTECTOR_OF_ACHERON_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case PROTECTOR_OF_ACHERON_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void ProtectorOfAcheron::SetState_None()
{
	this->SetState(PROTECTOR_OF_ACHERON_STATE_NONE);
	this->SetElementalAttribute(ELEMENTAL_ATTRIBUTE_NONE);
	this->SetZone(0);
	this->SetBossCount(0);
	this->GetTime()->Reset();
	this->GetBossTime()->Reset();
	this->MovePlayersOut();

	sLog->outInfo("protectorofacheron", "%s", __FUNCTION__);
}

void ProtectorOfAcheron::SetState_Playing()
{
	this->SetState(PROTECTOR_OF_ACHERON_STATE_PLAYING);
	this->SetBossCount(0);
	this->GetTime()->Start(PROTECTOR_OF_ACHERON_PLAY_TIME);
	this->GetBossTime()->Start();

	PentagramElementalAttribute random_element = Random(ELEMENTAL_ATTRIBUTE_MAX);

	if ( random_element == ELEMENTAL_ATTRIBUTE_NONE )
	{
		random_element = ELEMENTAL_ATTRIBUTE_FIRE;
	}

	this->SetElementalAttribute(random_element);

	this->SetZone(Random(PROTECTOR_OF_ACHERON_MAX_ZONE));

	if ( !this->CreateObelisk() )
	{
		this->SetState_None();
		return;
	}

	sServerLink->NoticeSend(NOTICE_GLOBAL, "Protector of Acheron Started.");

	sServerLink->EventStateUpdate(EVENT_PROTECTOR_OF_ACHERON, PROTECTOR_OF_ACHERON_STATE_PLAYING, 0);

	sLog->outInfo("protectorofacheron", "%s", __FUNCTION__);
}

void ProtectorOfAcheron::SetState_End()
{
	this->SetState(PROTECTOR_OF_ACHERON_STATE_END);
	this->GetTime()->Start(PROTECTOR_OF_ACHERON_END_TIME);

	sServerLink->EventStateUpdate(EVENT_PROTECTOR_OF_ACHERON, PROTECTOR_OF_ACHERON_STATE_END, 0);

	sLog->outInfo("protectorofacheron", "%s", __FUNCTION__);
}

void ProtectorOfAcheron::ProcState_None()
{

}

void ProtectorOfAcheron::ProcState_Playing()
{
	this->BossCreate();

	if ( this->GetTime()->Elapsed() )
	{
		this->SendFail();
		this->SetState_End();
	}
}

void ProtectorOfAcheron::ProcState_End()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_None();
		sServerLink->EventStateUpdate(EVENT_PROTECTOR_OF_ACHERON, PROTECTOR_OF_ACHERON_STATE_NONE, 0);
	}
}

bool ProtectorOfAcheron::CreateObelisk()
{
	Monster* pMonster = sObjectMgr->MonsterTryAdd(g_ProtectorOfAcheronMonster[this->GetZone()][this->GetElementalAttribute()].obelisk, g_ProtectorOfAcheronZone[this->GetZone()].world);

	if ( !pMonster )
	{
		return false;
	}

	pMonster->SetWorldId(g_ProtectorOfAcheronZone[this->GetZone()].world);
	pMonster->SetBasicLocation(g_ProtectorOfAcheronZone[this->GetZone()].x, g_ProtectorOfAcheronZone[this->GetZone()].y, g_ProtectorOfAcheronZone[this->GetZone()].x, g_ProtectorOfAcheronZone[this->GetZone()].y);
	pMonster->SetScriptName("protector_of_acheron_obelisk");
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetEventId(EVENT_PROTECTOR_OF_ACHERON);
	pMonster->AddToWorld();

	return true;
}

void ProtectorOfAcheron::BossCreate()
{
	uint32 period = this->GetBossTime()->GetElapsed();
	uint16 elapsed_time = (this->GetTime()->GetElapsed() / IN_MILLISECONDS) / MINUTE;

	for ( ProtectorOfAcheronBossList::const_iterator it = this->boss_list.begin(); it != this->boss_list.end(); ++it )
	{
		if ( (elapsed_time >= (*it)->GetTimeMin()) && (elapsed_time <= (*it)->GetTimeMax()) )
		{
			if ( period >= (*it)->GetPeriod() )
			{
				this->GetBossTime()->Start();

				for ( int32 i = 0; i < (*it)->GetCount(); ++i )
				{
					Monster* pMonster = sObjectMgr->MonsterTryAdd(g_ProtectorOfAcheronMonster[this->GetZone()][this->GetElementalAttribute()].boss, g_ProtectorOfAcheronZone[this->GetZone()].world);

					if ( pMonster )
					{
						pMonster->SetWorldId(g_ProtectorOfAcheronZone[this->GetZone()].world);
						pMonster->SetBasicLocation(g_ProtectorOfAcheronZone[this->GetZone()].x - 5, g_ProtectorOfAcheronZone[this->GetZone()].y - 5, g_ProtectorOfAcheronZone[this->GetZone()].x + 5, g_ProtectorOfAcheronZone[this->GetZone()].y + 5);
						pMonster->SetMoveDistance(20);
						pMonster->SetScriptName("protector_of_acheron_boss");
						pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
						pMonster->SetEventId(EVENT_PROTECTOR_OF_ACHERON);
						pMonster->AddToWorld();

						this->IncreaseBossCount(1);
					}

					if ( this->GetBossCount() >= (*it)->GetMaxCount() )
					{
						return;
					}
				}
			}

			return;
		}
	}
}

void ProtectorOfAcheron::SendSuccess()
{
	sObjectMgr->SendNoticeToMap(g_ProtectorOfAcheronZone[this->GetZone()].world, NOTICE_GLOBAL, "Obelisk has been destroyed.");
}
	
void ProtectorOfAcheron::SendFail()
{
	sObjectMgr->SendNoticeToMap(g_ProtectorOfAcheronZone[this->GetZone()].world, NOTICE_GLOBAL, "Obelisk hasn't been destroyed.");
}

void ProtectorOfAcheron::MovePlayersOut()
{
	Player* pPlayer = nullptr;
	
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		for ( int32 i = 0; i < PROTECTOR_OF_ACHERON_MAX_ZONE; ++i )
		{
			if ( pPlayer->GetWorldId() == g_ProtectorOfAcheronZone[i].world )
			{
				pPlayer->MoveToGate(27);
				break;
			}
		}
	}
}