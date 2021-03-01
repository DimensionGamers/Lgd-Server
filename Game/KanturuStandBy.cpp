/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuStandBy.cpp"
*
*/
KanturuStandBy::KanturuStandBy()
{
	this->SetState(KANTURU_STANBY_NONE);
	this->SetEntrance(false);
	this->Init();
}

KanturuStandBy::~KanturuStandBy()
{

}

void KanturuStandBy::Init()
{
	for ( uint8 i = 0; i < KANTURU_STANBY_MAX; ++i )
	{
		this->GetStateData(i)->ResetTimeInfo();
	}
}

void KanturuStandBy::Load(const char * filename)
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
			case 1:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_STANBY_MAX )
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

void KanturuStandBy::Update()
{
	this->CheckStateTime();

	switch ( this->GetState() )
	{
	case KANTURU_STANBY_NONE:
		{
			this->ProcState_None();
		} break;

	case KANTURU_STANBY_START:
		{
			this->ProcState_Start();
		} break;

	case KANTURU_STANBY_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case KANTURU_STANBY_END:
		{
			this->ProcState_End();
		} break;
	}
}

void KanturuStandBy::CheckStateTime()
{
	if ( this->GetStateData(this->GetState()) && this->GetStateData(this->GetState())->GetCondition() == 1 && this->GetStateData(this->GetState())->Passed() )
	{
		this->SetStateNext();
	}
}

void KanturuStandBy::ChangeState(uint8 state)
{
	if ( state >= KANTURU_STANBY_MAX )
	{
		state = KANTURU_STANBY_NONE;
	}

	this->GetStateData(state)->Start();

	sKanturuMgr->InitUser();

	switch ( state )
	{
	case KANTURU_STANBY_NONE:
		{
			this->SetState_None();
		} break;

	case KANTURU_STANBY_START:
		{
			this->SetState_Start();
		} break;

	case KANTURU_STANBY_NOTIFY:
		{
			this->SetState_Notify();
		} break;

	case KANTURU_STANBY_END:
		{
			this->SetState_End();
		} break;
	}

	sKanturuMgr->StateChange();
}

int32 KanturuStandBy::GetRemainTime() const
{
	KanturuStateInfo const* pKanturuStateInfo = this->GetStateData(this->GetState());

	if ( !pKanturuStateInfo )
	{
		return 0;
	}

	return pKanturuStateInfo->GetRemainTime();
}

void KanturuStandBy::SetState_None()
{
	this->SetState(KANTURU_STANBY_NONE);

	sLog->outInfo(LOG_KANTURU, "[ Battle StandBy ] SetState_None()");
}
	
void KanturuStandBy::SetState_Start()
{
	this->SetState(KANTURU_STANBY_START);

	sLog->outInfo(LOG_KANTURU, "[ Battle StandBy ] SetState_Start()");
}
	
void KanturuStandBy::SetState_Notify()
{
	this->SetState(KANTURU_STANBY_NOTIFY);
	sKanturuMgr->sendNoticeToPlayersOnMap(true, "1 minute(s) left before Kanturu Gate Core opens.");

	sLog->outInfo(LOG_KANTURU, "[ Battle StandBy ] SetState_Notify()");
}
	
void KanturuStandBy::SetState_End()
{
	this->SetState(KANTURU_STANBY_END);

	sLog->outInfo(LOG_KANTURU, "[ Battle StandBy ] SetState_End()");
}

void KanturuStandBy::ProcState_None()
{

}
	
void KanturuStandBy::ProcState_Start()
{

}
	
void KanturuStandBy::ProcState_Notify()
{

}
	
void KanturuStandBy::ProcState_End()
{

}