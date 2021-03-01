/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Raklion.cpp"
*
*/

Raklion::Raklion()
{
	this->GetCloseGateTime()->Reset();
	this->SetState_None();
}

Raklion::~Raklion()
{

}

void Raklion::LoadData()
{
	for ( int32 i = 0; i < RAKLION_STATE_MAX; ++i )
		this->GetStateData(i)->Reset();

	if ( !sGameServer->IsRaklionEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Raklion Data...");

	std::string filename("EventData/Raklion.txt");

	ReadScript file(filename.c_str(), "r");
	
	if ( !file.open() )
		return;

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

					file.GetToken(); int32 duration = file.GetInt32();
					file.GetToken(); bool entrance = file.GetBool();
					file.GetToken(); bool pkallowed = file.GetBool();

					if ( state < RAKLION_STATE_MAX )
					{
						this->GetStateData(state)->SetDuration(duration);
						this->GetStateData(state)->SetEntraceAllowed(entrance);
						this->GetStateData(state)->SetPKAllowed(pkallowed);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Raklion Data Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Raklion::Update()
{
	switch ( this->GetState() )
	{
	case RAKLION_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case RAKLION_STATE_CLOSED:
		{
			this->ProcState_Closed();
		} break;

	case RAKLION_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case RAKLION_STATE_STANDBY_1:
		{
			this->ProcState_StandBy_1();
		} break;

	case RAKLION_STATE_STANDBY_2:
		{
			this->ProcState_StandBy_2();
		} break;

	case RAKLION_STATE_KILL_SELUPAN:
		{
			this->ProcState_KillSelupan();
		} break;

	case RAKLION_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void Raklion::ChangeState(RaklionState state)
{
	this->RunTime(this->GetStateData(state)->GetDuration() * IN_MILLISECONDS);
	this->SetState(state);
	this->GetCloseGateTime()->Reset();
		
	switch ( this->GetState() )
	{
	case RAKLION_STATE_NONE:
		{
			this->SetState_None();
		} break;

	case RAKLION_STATE_CLOSED:
		{
			this->SetState_Closed();
		} break;

	case RAKLION_STATE_OPEN:
		{
			this->SetState_Open();
		} break;

	case RAKLION_STATE_STANDBY_1:
		{
			this->SetState_StandBy_1();
		} break;

	case RAKLION_STATE_STANDBY_2:
		{
			this->SetState_StandBy_2();
		} break;

	case RAKLION_STATE_KILL_SELUPAN:
		{
			this->SetState_KillSelupan();
		} break;

	case RAKLION_STATE_END:
		{
			this->SetState_End();
		} break;
	}
}

void Raklion::SetState_None()
{
	this->SetEggCount(0);
	this->SetBossCount(0);
	this->SetSummonCount(0);
	this->SetSummonMaxCount(0);
		
	sLog->outInfo(LOG_RAKLION, "SetState_None()");
}

void Raklion::SetState_Closed()
{
	uint32 closed_time_min = this->GetBossCount() <= 0 ? sGameServer->GetRaklionSuccessTimeMin() : sGameServer->GetRaklionFailTimeMin();
	uint32 closed_time_max = this->GetBossCount() <= 0 ? sGameServer->GetRaklionSuccessTimeMax() : sGameServer->GetRaklionFailTimeMax();

	this->RunTime(closed_time_min + Random(closed_time_max - closed_time_min));

	sLog->outInfo(LOG_RAKLION, "SetState_Closed()");
}

void Raklion::SetState_Open()
{
	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Hatchery Gates are open.");

	sLog->outInfo(LOG_RAKLION, "SetState_Open()");
}

void Raklion::SetState_StandBy_1()
{
	this->SetEggCount(0);
	this->SetBossCount(0);
	this->SetSummonCount(0);
	this->SetSummonMaxCount(0);
	this->AddEggs();

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Hatchery Gates are open. %d minute(s) left to kill eggs.", this->GetRemain() / (MINUTE * IN_MILLISECONDS));
	
	sLog->outInfo(LOG_RAKLION, "SetState_StandBy_1()");
}

void Raklion::SetState_StandBy_2()
{
	sLog->outInfo(LOG_RAKLION, "SetState_StandBy_2()");
}

void Raklion::SetState_KillSelupan()
{
	this->SetBossCount(0);
	this->AddSelupan();
	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Selupan has appeared. The Hatchery Gates will be closed in %d minute(s)", this->GetRemain() / (MINUTE * IN_MILLISECONDS));

	sLog->outInfo(LOG_RAKLION, "SetState_KillSelupan()");
}

void Raklion::SetState_End()
{
	sLog->outInfo(LOG_RAKLION, "SetState_End()");
}

void Raklion::ProcState_None()
{
	if ( !sGameServer->IsRaklionEnabled() )
		return;

	this->ChangeState(RAKLION_STATE_CLOSED);
}

void Raklion::ProcState_Closed()
{
	if ( this->TimePassed() )
	{
		this->ChangeState(RAKLION_STATE_OPEN);
	}
}

void Raklion::ProcState_Open()
{
	if ( this->TimePassed(true) )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Hatchery Gates are open.");
	}
}

void Raklion::ProcState_StandBy_1()
{
	if ( (this->GetRemain() / (MINUTE * IN_MILLISECONDS)) > 5 )
	{
		this->SendRemainMinutes("Hatchery Gates are open. %d minute(s) left to kill Spider Eggs.", 5);
	}
	else
	{
		this->SendRemainMinutes("Hatchery Gates are open. %d minute(s) left to kill Spider Eggs.", 1);
	}
	
	if ( this->TimePassed() || this->GetEggCount() <= 0  )
	{
		this->ChangeState(RAKLION_STATE_STANDBY_2);
	}
}

void Raklion::ProcState_StandBy_2()
{
	if ( this->TimePassed() )
	{
		this->ChangeState(RAKLION_STATE_KILL_SELUPAN);
	}
}

void Raklion::ProcState_KillSelupan()
{
	if ( this->TimePassed() || this->GetBossCount() <= 0 )
	{
		this->ChangeState(RAKLION_STATE_END);
	}
}

void Raklion::ProcState_End()
{
	if ( this->TimePassed() )
	{
		this->ClearMonster();
		this->ChangeState(RAKLION_STATE_NONE);
	}
}

void Raklion::AddEggs()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_RAKLION )
		{
			continue;
		}

		if ( (*it)->GetID() != 460 && 
			 (*it)->GetID() != 461 && 
			 (*it)->GetID() != 462 )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->AddToWorld();
			this->IncreaseEggCount(1);
		}
	}
}

void Raklion::AddSelupan()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_RAKLION )
		{
			continue;
		}

		if ( (*it)->GetID() != 459 )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->AddToWorld();

			this->IncreaseBossCount(1);
		}
	}
}

void Raklion::AddSummoned()
{
	for ( MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it )
	{
		if ( (*it)->GetEventID() != EVENT_RAKLION )
		{
			continue;
		}

		if ( (*it)->GetID() == 459 || 
			 (*it)->GetID() == 460 || 
			 (*it)->GetID() == 461 || 
			 (*it)->GetID() == 462 )
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if ( pMonster )
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			pMonster->AddToWorld();
			this->IncreaseSummonCount(1);

			if ( this->GetSummonMaxCount() > 0 )
			{
				if ( this->GetSummonCount() >= this->GetSummonMaxCount() )
				{
					break;
				}
			}
		}
	}

	if ( this->GetSummonMaxCount() <= 0 )
	{
		this->SetSummonMaxCount(this->GetSummonCount());
	}
}

void Raklion::ClearMonster()
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->GetEventId() != EVENT_RAKLION )
			continue;

		pMonster->Remove();
	}
}

bool Raklion::EnterAllowed(Player* pPlayer)
{
	if ( !this->GetStateData(this->GetState())->IsEntraceAllowed() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Hatchery gates are closed.");
		return false;
	}

	if ( this->GetState() == RAKLION_STATE_OPEN )
	{
		this->ChangeState(RAKLION_STATE_STANDBY_1);
	}

	return true;
}