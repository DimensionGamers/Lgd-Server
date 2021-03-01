/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuMaya.cpp"
*
*/

KanturuMaya::KanturuMaya()
{
	this->SetState(KANTURU_MAYA_NONE);
	this->SetEntrance(false);
	this->Init();
}

KanturuMaya::~KanturuMaya()
{

}

void KanturuMaya::Init()
{
	for ( uint8 i = 0; i < KANTURU_MAYA_MAX; ++i )
	{
		this->GetStateData(i)->ResetTimeInfo();
	}

	for ( uint8 i = 0; i < KANTURU_MAYA_SCENE_MAX; ++i )
	{
		this->GetSceneData(i)->ResetTimeInfo();
	}

	this->SetSuccess(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_NONE);
	this->SetSceneSuccess(false);
	this->SetEntrance(false);
}

void KanturuMaya::Load(const char * filename)
{
	this->Init();

	ReadScript file(filename, "r");
	
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
			case 2:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_MAYA_SCENE_MAX )
					{
						this->GetSceneData(state)->SetStateInfo(state);
						this->GetSceneData(state)->SetCondition(condition);
						this->GetSceneData(state)->SetValue(value * IN_MILLISECONDS);
					}
				} break;

			case 3:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_MAYA_MAX )
					{
						this->GetStateData(state)->SetStateInfo(state);
						this->GetStateData(state)->SetCondition(condition);
						this->GetStateData(state)->SetValue(value * IN_MILLISECONDS);
					}
				} break;
			}
		}
	}
}

void KanturuMaya::Update()
{
	this->CheckStateTime();
	this->UserUpdate();
	sKanturuMgr->UpdateUser();

	switch ( this->GetState() )
	{
	case KANTURU_MAYA_NONE:
		{
			this->ProcState_None();
		} break;

	case KANTURU_MAYA_STANDBY_1:
		{
			this->ProcState_StandBy_1();
		} break;

	case KANTURU_MAYA_NOTIFY_1:
		{
			this->ProcState_Notify_1();
		} break;

	case KANTURU_MAYA_START_1:
		{
			this->ProcState_Start_1();
		} break;

	case KANTURU_MAYA_1:
		{
			this->ProcState_Maya_1();
		} break;

	case KANTURU_MAYA_END_1:
		{
			this->ProcState_End_1();
		} break;

	case KANTURU_MAYA_ENDCYCLE_1:
		{
			this->ProcState_EndCycle_1();
		} break;

	case KANTURU_MAYA_STANDBY_2:
		{
			this->ProcState_StandBy_2();
		} break;

	case KANTURU_MAYA_START_2:
		{
			this->ProcState_Start_2();
		} break;

	case KANTURU_MAYA_2:
		{
			this->ProcState_Maya_2();
		} break;

	case KANTURU_MAYA_END_2:
		{
			this->ProcState_End_2();
		} break;

	case KANTURU_MAYA_ENDCYCLE_2:
		{
			this->ProcState_EndCycle_2();
		} break;

	case KANTURU_MAYA_STANDBY_3:
		{
			this->ProcState_StandBy_3();
		} break;

	case KANTURU_MAYA_START_3:
		{
			this->ProcState_Start_3();
		} break;

	case KANTURU_MAYA_3:
		{
			this->ProcState_Maya_3();
		} break;

	case KANTURU_MAYA_END_3:
		{
			this->ProcState_End_3();
		} break;

	case KANTURU_MAYA_ENDCYCLE_3:
		{
			this->ProcState_EndCycle_3();
		} break;

	case KANTURU_MAYA_END:
		{
			this->ProcState_End();
		} break;

	case KANTURU_MAYA_ENDCYCLE:
		{
			this->ProcState_EndCycle();
		} break;
	}
}

void KanturuMaya::CheckStateTime()
{
	if ( this->GetStateData(this->GetState()) && this->GetStateData(this->GetState())->GetCondition() == 1 && this->GetStateData(this->GetState())->Passed() )
	{
		this->SetStateNext();
	}

	if ( this->GetSceneState() == -1 )
		return;

	KanturuStateInfo const* pKanturuStateInfo = this->GetSceneData(this->GetSceneState());

	if ( !pKanturuStateInfo )
	{
		return;
	}

	if ( pKanturuStateInfo->GetCondition() != -1 )
		return;

	if ( this->IsSceneSuccess() )
		return;

	if ( pKanturuStateInfo->Passed() )
	{
		if ( this->GetState() != KANTURU_MAYA_END )
		{
			this->SetSceneSuccess(false);
			this->SetSuccess(false);

			sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Failed. TimeOut at state %d", this->GetState());
			this->ChangeState(KANTURU_MAYA_END);
		}
	}
}

void KanturuMaya::ChangeState(uint8 state)
{
	if ( state >= KANTURU_MAYA_MAX )
	{
		state = KANTURU_MAYA_NONE;
	}

	this->GetStateData(state)->Start();

	switch ( state )
	{
	case KANTURU_MAYA_NONE:
		{
			this->SetState_None();
		} break;

	case KANTURU_MAYA_STANDBY_1:
		{
			this->SetState_StandBy_1();
		} break;

	case KANTURU_MAYA_NOTIFY_1:
		{
			this->SetState_Notify_1();
		} break;

	case KANTURU_MAYA_START_1:
		{
			this->SetState_Start_1();
		} break;

	case KANTURU_MAYA_1:
		{
			this->SetState_Maya_1();
		} break;

	case KANTURU_MAYA_END_1:
		{
			this->SetState_End_1();
		} break;

	case KANTURU_MAYA_ENDCYCLE_1:
		{
			this->SetState_EndCycle_1();
		} break;

	case KANTURU_MAYA_STANDBY_2:
		{
			this->SetState_StandBy_2();
		} break;

	case KANTURU_MAYA_START_2:
		{
			this->SetState_Start_2();
		} break;

	case KANTURU_MAYA_2:
		{
			this->SetState_Maya_2();
		} break;

	case KANTURU_MAYA_END_2:
		{
			this->SetState_End_2();
		} break;

	case KANTURU_MAYA_ENDCYCLE_2:
		{
			this->SetState_EndCycle_2();
		} break;

	case KANTURU_MAYA_STANDBY_3:
		{
			this->SetState_StandBy_3();
		} break;

	case KANTURU_MAYA_START_3:
		{
			this->SetState_Start_3();
		} break;

	case KANTURU_MAYA_3:
		{
			this->SetState_Maya_3();
		} break;

	case KANTURU_MAYA_END_3:
		{
			this->SetState_End_3();
		} break;

	case KANTURU_MAYA_ENDCYCLE_3:
		{
			this->SetState_EndCycle_3();
		} break;

	case KANTURU_MAYA_END:
		{
			this->SetState_End();
		} break;

	case KANTURU_MAYA_ENDCYCLE:
		{
			this->SetState_EndCycle();
		} break;
	}

	sKanturuMgr->AddMonster();
	sKanturuMgr->StateChange();
}

int32 KanturuMaya::GetRemainTime() const
{
	KanturuStateInfo const* pKanturuStateInfo = this->GetStateData(this->GetState());

	if ( !pKanturuStateInfo )
	{
		return 0;
	}

	return pKanturuStateInfo->GetRemainTime();
}

void KanturuMaya::SetState_None()
{
	this->SetState(KANTURU_MAYA_NONE);
	this->SetEntrance(false);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_None()");
}
	
void KanturuMaya::SetState_StandBy_1()
{
	this->SetState(KANTURU_MAYA_STANDBY_1);
	sKanturuMgr->InitUser();
	this->SetEntrance(true);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_StandBy_1()");
}
	
void KanturuMaya::SetState_Notify_1()
{
	this->SetState(KANTURU_MAYA_NOTIFY_1);
	this->SetEntrance(false);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Notify_1()");
}
	
void KanturuMaya::SetState_Start_1()
{
	this->SetState(KANTURU_MAYA_START_1);

	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_0);
	this->SetSceneSuccess(false);

	if ( this->GetSceneData(this->GetSceneState()) )
	{
		this->GetSceneData(this->GetSceneState())->Start();

		if ( this->GetSceneData(this->GetSceneState())->GetCondition() == 1 )
		{
			sKanturuMgr->SendTime(this->GetSceneData(this->GetSceneState())->GetValue());
		}
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Start_1()");
}
	
void KanturuMaya::SetState_Maya_1()
{
	this->SetState(KANTURU_MAYA_1);
	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneSuccess(false);
	this->m_maya_tick.set(0);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Maya_1()");
}
	
void KanturuMaya::SetState_End_1()
{
	this->SetState(KANTURU_MAYA_END_1);
	this->SetSceneState(KANTURU_MAYA_SCENE_NONE);
	this->SetEntrance(false);
	sKanturuMgr->SendSuccess(this->IsSuccess());

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_End_1()");
}
	
void KanturuMaya::SetState_EndCycle_1()
{
	this->SetState(KANTURU_MAYA_ENDCYCLE_1);
	this->SetEntrance(false);
	sKanturuMgr->sendNoticeToPlayersOnMap(true, "1 minute(s) left before proceed to next battle.");

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_EndCycle_1()");
}
	
void KanturuMaya::SetState_StandBy_2()
{
	this->SetState(KANTURU_MAYA_STANDBY_2);
	this->SetEntrance(true);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_StandBy_2()");
}

void KanturuMaya::SetState_Start_2()
{
	this->SetState(KANTURU_MAYA_START_2);
	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_1);
	this->SetSceneSuccess(false);

	if ( this->GetSceneData(this->GetSceneState()) )
	{
		this->GetSceneData(this->GetSceneState())->Start();

		if ( this->GetSceneData(this->GetSceneState())->GetCondition() == 1 )
		{
			sKanturuMgr->SendTime(this->GetSceneData(this->GetSceneState())->GetValue());
		}
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Start_2()");
}
	
void KanturuMaya::SetState_Maya_2()
{
	this->SetState(KANTURU_MAYA_2);
	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneSuccess(false);
	this->m_maya_tick.set(0);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Maya_2()");
}
	
void KanturuMaya::SetState_End_2()
{
	this->SetState(KANTURU_MAYA_END_2);
	this->SetEntrance(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_NONE);
	sKanturuMgr->SendSuccess(this->IsSuccess());

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_End_2()");
}
	
void KanturuMaya::SetState_EndCycle_2()
{
	this->SetState(KANTURU_MAYA_ENDCYCLE_2);
	this->SetEntrance(false);
	sKanturuMgr->sendNoticeToPlayersOnMap(true, "1 minute(s) left before proceed to next battle.");

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_EndCycle_2()");
}
	
void KanturuMaya::SetState_StandBy_3()
{
	this->SetState(KANTURU_MAYA_STANDBY_3);
	this->SetEntrance(true);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_StandBy_3()");
}
	
void KanturuMaya::SetState_Start_3()
{
	this->SetState(KANTURU_MAYA_START_3);
	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_2);
	this->SetSceneSuccess(false);

	if ( this->GetSceneData(this->GetSceneState()) )
	{
		this->GetSceneData(this->GetSceneState())->Start();

		if ( this->GetSceneData(this->GetSceneState())->GetCondition() == 1 )
		{
			sKanturuMgr->SendTime(this->GetSceneData(this->GetSceneState())->GetValue());
		}
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Start_3()");
}
	
void KanturuMaya::SetState_Maya_3()
{
	this->SetState(KANTURU_MAYA_3);
	this->SetEntrance(false);
	this->SetSuccess(false);
	this->SetSceneSuccess(false);
	this->m_maya_tick.set(0);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_Maya_3()");
}
	
void KanturuMaya::SetState_End_3()
{
	this->SetState(KANTURU_MAYA_END_3);
	this->SetEntrance(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_NONE);
	sKanturuMgr->SendSuccess(this->IsSuccess());

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_End_3()");
}
	
void KanturuMaya::SetState_EndCycle_3()
{
	this->SetState(KANTURU_MAYA_ENDCYCLE_3);
	this->SetEntrance(false);

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_EndCycle_3()");
}
	
void KanturuMaya::SetState_End()
{
	this->SetState(KANTURU_MAYA_END);
	this->SetEntrance(false);
	this->SetSceneState(KANTURU_MAYA_SCENE_NONE);

	if ( this->IsSuccess() )
	{
		sKanturuMgr->SendNotice("Defeat the Nightmare that controls the Maya to gain entrance to the Refinery.");
	}
	else
	{
		sKanturuMgr->SendSuccess(this->IsSuccess());
		sKanturuMgr->SendNotice("Because will limit the time crossed, will force to return the ridge security area.");
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_End()");
}
	
void KanturuMaya::SetState_EndCycle()
{
	this->SetState(KANTURU_MAYA_ENDCYCLE);
	this->SetEntrance(false);

	if ( !this->IsSuccess() )
	{
		sKanturuMgr->MoveToGate(KANTURU_GATE_OUT);
		sKanturuMgr->InitUser();
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] SetState_EndCycle()");
}

void KanturuMaya::ProcState_None()
{

}
	
void KanturuMaya::ProcState_StandBy_1()
{

}
	
void KanturuMaya::ProcState_Notify_1()
{

}
	
void KanturuMaya::ProcState_Start_1()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		this->SetSuccess(true);
		sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		this->SetStateNext();
	}
	else
	{
		sKanturuMgr->MonsterAndUserNotify();
	}
}
	
void KanturuMaya::ProcState_Maya_1()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		if ( this->m_maya_tick == 0 )
		{
			this->SetSuccess(true);
			this->SetSceneSuccess(true);
			this->m_maya_tick.set(GetTickCount());
			sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		}

		if ( GetTickCount() > ( this->m_maya_tick.get() + 3000) )
		{
			this->SetStateNext();
		}
	}

	sKanturuMgr->MonsterAndUserNotify();
}
	
void KanturuMaya::ProcState_End_1()
{

}
	
void KanturuMaya::ProcState_EndCycle_1()
{

}
	
void KanturuMaya::ProcState_StandBy_2()
{

}

void KanturuMaya::ProcState_Start_2()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		this->SetSuccess(true);
		sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		this->SetStateNext();
	}
	else
	{
		sKanturuMgr->MonsterAndUserNotify();
	}
}
	
void KanturuMaya::ProcState_Maya_2()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		if ( this->m_maya_tick == 0 )
		{
			this->SetSuccess(true);
			this->SetSceneSuccess(true);
			this->m_maya_tick.set(GetTickCount());
			sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		}

		if ( GetTickCount() > ( this->m_maya_tick.get() + 3000) )
		{
			this->SetStateNext();
		}
	}

	sKanturuMgr->MonsterAndUserNotify();
}
	
void KanturuMaya::ProcState_End_2()
{

}
	
void KanturuMaya::ProcState_EndCycle_2()
{

}
	
void KanturuMaya::ProcState_StandBy_3()
{

}

void KanturuMaya::ProcState_Start_3()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		this->SetSuccess(true);
		sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		this->SetStateNext();
	}
	else
	{
		sKanturuMgr->MonsterAndUserNotify();
	}
}
	
void KanturuMaya::ProcState_Maya_3()
{
	if ( sKanturuMgr->GetMonsterCount() <= 0 )
	{
		if ( this->m_maya_tick == 0 )
		{
			this->SetSuccess(true);
			this->SetSceneSuccess(true);
			this->m_maya_tick.set(GetTickCount());
			sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Success. State %d", this->GetState());
		}

		if ( GetTickCount() > ( this->m_maya_tick.get() + 3000) )
		{
			this->SetStateNext();
		}
	}

	sKanturuMgr->MonsterAndUserNotify();
}
	
void KanturuMaya::ProcState_End_3()
{

}
	
void KanturuMaya::ProcState_EndCycle_3()
{

}
	
void KanturuMaya::ProcState_End()
{

}
	
void KanturuMaya::ProcState_EndCycle()
{

}

void KanturuMaya::UserUpdate()
{
	if ( this->GetState() == KANTURU_MAYA_NOTIFY_1 || this->GetState() == KANTURU_MAYA_START_1 || this->GetState() == KANTURU_MAYA_1 ||
		 this->GetState() == KANTURU_MAYA_START_2 || this->GetState() == KANTURU_MAYA_2 ||
		 this->GetState() == KANTURU_MAYA_START_3 || this->GetState() == KANTURU_MAYA_3 )
	{
		if ( sKanturuMgr->GetUserCount() <= 0 )
		{
			this->SetSuccess(false);
			this->SetSceneSuccess(false);
			sLog->outInfo(LOG_KANTURU, "[ Battle Maya ] Failed. No users at state %d", this->GetState());
			this->ChangeState(KANTURU_MAYA_END);
		}
	}
}