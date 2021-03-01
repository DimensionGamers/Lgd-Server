/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuNightmare.cpp"
*
*/
KanturuNightmare::KanturuNightmare()
{
	this->SetState(KANTURU_NIGHTMARE_NONE);
	this->SetEntrance(false);
	this->Init();
}

KanturuNightmare::~KanturuNightmare()
{

}

void KanturuNightmare::Init()
{
	for ( uint8 i = 0; i < KANTURU_NIGHTMARE_MAX; ++i )
	{
		this->GetStateData(i)->ResetTimeInfo();
	}

	this->SetSuccess(false);
}

void KanturuNightmare::Load(const char * filename)
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
			case 4:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_NIGHTMARE_MAX )
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

void KanturuNightmare::Update()
{
	this->CheckStateTime();
	this->UserUpdate();
	sKanturuMgr->UpdateUser();

	switch ( this->GetState() )
	{
	case KANTURU_NIGHTMARE_NONE:
		{
			this->ProcState_None();
		} break;

	case KANTURU_NIGHTMARE_IDLE:
		{
			this->ProcState_Idle();
		} break;

	case KANTURU_NIGHTMARE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case KANTURU_NIGHTMARE_START:
		{
			this->ProcState_Start();
		} break;

	case KANTURU_NIGHTMARE_END:
		{
			this->ProcState_End();
		} break;

	case KANTURU_NIGHTMARE_ENDCYCLE:
		{
			this->ProcState_EndCycle();
		} break;
	}
}

void KanturuNightmare::CheckStateTime()
{
	if ( this->GetStateData(this->GetState()) && this->GetStateData(this->GetState())->GetCondition() == 1 && this->GetStateData(this->GetState())->Passed() )
	{
		this->SetStateNext();
	}
}

void KanturuNightmare::ChangeState(uint8 state)
{
	if ( state >= KANTURU_NIGHTMARE_MAX )
	{
		state = KANTURU_NIGHTMARE_NONE;
	}

	this->GetStateData(state)->Start();

	switch ( state )
	{
	case KANTURU_NIGHTMARE_NONE:
		{
			this->SetState_None();
		} break;

	case KANTURU_NIGHTMARE_IDLE:
		{
			this->SetState_Idle();
		} break;

	case KANTURU_NIGHTMARE_NOTIFY:
		{
			this->SetState_Notify();
		} break;

	case KANTURU_NIGHTMARE_START:
		{
			this->SetState_Start();
		} break;

	case KANTURU_NIGHTMARE_END:
		{
			this->SetState_End();
		} break;

	case KANTURU_NIGHTMARE_ENDCYCLE:
		{
			this->SetState_EndCycle();
		} break;
	}

	sKanturuMgr->AddMonster();
	sKanturuMgr->StateChange();
}

int32 KanturuNightmare::GetRemainTime() const
{
	KanturuStateInfo const* pKanturuStateInfo = this->GetStateData(this->GetState());

	if ( !pKanturuStateInfo )
	{
		return 0;
	}

	return pKanturuStateInfo->GetRemainTime();
}

void KanturuNightmare::SetState_None()
{
	this->SetState(KANTURU_NIGHTMARE_NONE);

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_None()");
}

void KanturuNightmare::SetState_Idle()
{
	this->SetState(KANTURU_NIGHTMARE_IDLE);
	this->SetSuccess(false);

	sKanturuMgr->MoveToGate(KANTURU_GATE_NIGHTMARE);

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_Idle()");
}

void KanturuNightmare::SetState_Notify()
{
	this->SetState(KANTURU_NIGHTMARE_NOTIFY);

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_Notify()");
}

void KanturuNightmare::SetState_Start()
{
	this->SetState(KANTURU_NIGHTMARE_START);

	if ( this->GetStateData(this->GetState()) && this->GetStateData(this->GetState())->GetCondition() == 1 )
	{
		sKanturuMgr->SendTime(this->GetStateData(this->GetState())->GetValue());
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_Start()");
}

void KanturuNightmare::SetState_End()
{
	this->SetState(KANTURU_NIGHTMARE_END);
	sKanturuMgr->SendSuccess(this->IsSuccess());

	if ( !this->IsSuccess() )
	{
		sKanturuMgr->SendNotice("Because will limit the time crossed, will force to return the ridge security area.");
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_End()");
}

void KanturuNightmare::SetState_EndCycle()
{
	this->SetState(KANTURU_NIGHTMARE_ENDCYCLE);

	if ( this->IsSuccess() )
	{
		sKanturuMgr->SendNotice("Has defeated the Nightmare Boss. You may now proceed to the Refinery Tower.");
	}
	else
	{
		sKanturuMgr->MoveToGate(KANTURU_GATE_OUT);
		sKanturuMgr->InitUser();
	}

	sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] SetState_EndCycle()");
}

void KanturuNightmare::ProcState_None()
{

}
	
void KanturuNightmare::ProcState_Idle()
{

}
	
void KanturuNightmare::ProcState_Notify()
{

}
	
void KanturuNightmare::ProcState_Start()
{
	sKanturuMgr->MonsterAndUserNotify();

	if ( !sKanturuMgr->IsNightmareAlive() )
	{
		this->SetSuccess(true);
		sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] Success. State %d. Nightmare is Dead", this->GetState());
		this->SetStateNext();
	}
}
	
void KanturuNightmare::ProcState_End()
{

}
	
void KanturuNightmare::ProcState_EndCycle()
{
	
}

void KanturuNightmare::UserUpdate()
{
	if ( this->GetState() == KANTURU_NIGHTMARE_NOTIFY || this->GetState() == KANTURU_NIGHTMARE_START )
	{
		if ( sKanturuMgr->GetUserCount() <= 0 )
		{
			this->SetSuccess(false);
			sLog->outInfo(LOG_KANTURU, "[ Battle Nightmare ] Failed. No users at state %d", this->GetState());
			this->ChangeState(KANTURU_NIGHTMARE_END);
		}
	}
}