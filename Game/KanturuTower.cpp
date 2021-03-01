/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "KanturuTower.cpp"
*
*/

KanturuTower::KanturuTower()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_NONE);
	this->SetEntrance(false);
	this->Init();
}

KanturuTower::~KanturuTower()
{

}

void KanturuTower::Init()
{
	for ( uint8 i = 0; i < KANTURU_TOWER_OF_REFINEMENT_MAX; ++i )
	{
		this->GetStateData(i)->ResetTimeInfo();
	}
}

void KanturuTower::Load(const char * filename)
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
			case 5:
				{
					uint8 state = file.GetUInt8();

					file.GetToken(); int32 condition = file.GetInt32();
					file.GetToken(); int32 value = file.GetInt32();

					if ( state < KANTURU_TOWER_OF_REFINEMENT_MAX )
					{
						this->GetStateData(state)->SetStateInfo(state);
						this->GetStateData(state)->SetCondition(condition);

						if ( condition == 1 )
							value *= IN_MILLISECONDS;

						this->GetStateData(state)->SetValue(value);
					}
				} break;
			}
		}
	}
}

void KanturuTower::Update()
{
	this->CheckStateTime();

	switch ( this->GetState() )
	{
	case KANTURU_TOWER_OF_REFINEMENT_NONE:
		{
			this->ProcState_None();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION:
		{
			this->ProcState_Revitalization();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_NOTIFY_1:
		{
			this->ProcState_Notify_1();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_CLOSE:
		{
			this->ProcState_Close();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_NOTIFY_2:
		{
			this->ProcState_Notify_2();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_END:
		{
			this->ProcState_End();
		} break;
	}
}

void KanturuTower::CheckStateTime()
{
	if ( this->GetStateData(this->GetState()) && this->GetStateData(this->GetState())->GetCondition() == 1 && this->GetStateData(this->GetState())->Passed() )
	{
		this->SetStateNext();
	}
}

void KanturuTower::ChangeState(uint8 state)
{
	if ( state >= KANTURU_TOWER_OF_REFINEMENT_MAX )
	{
		state = KANTURU_TOWER_OF_REFINEMENT_NONE;
	}

	this->GetStateData(state)->Start();

	switch ( state )
	{
	case KANTURU_TOWER_OF_REFINEMENT_NONE:
		{
			this->SetState_None();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION:
		{
			this->SetState_Revitalization();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_NOTIFY_1:
		{
			this->SetState_Notify_1();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_CLOSE:
		{
			this->SetState_Close();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_NOTIFY_2:
		{
			this->SetState_Notify_2();
		} break;

	case KANTURU_TOWER_OF_REFINEMENT_END:
		{
			this->SetState_End();
		} break;
	}

	sKanturuMgr->StateChange();
	sKanturuMgr->InitUser(false);
}

int32 KanturuTower::GetRemainTime() const
{
	KanturuStateInfo const* pKanturuStateInfo = this->GetStateData(this->GetState());

	if ( !pKanturuStateInfo )
	{
		return 0;
	}

	return pKanturuStateInfo->GetRemainTime();
}

void KanturuTower::SetState_None()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_NONE);
	this->SetEntrance(false);

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_None()");
}

void KanturuTower::SetState_Revitalization()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION);
	this->SetEntrance(true);

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_Revitalization()");
}

void KanturuTower::SetState_Notify_1()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_NOTIFY_1);
	this->SetEntrance(true);
	sKanturuMgr->sendNoticeToPlayersOnMap(true, "Will lead to tower of the refinement road after 1 hour again seal.");

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_Notify_1()");
}

void KanturuTower::SetState_Close()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_CLOSE);
	this->SetEntrance(false);
	sKanturuMgr->sendNoticeToPlayersOnMap(true, "Because the unlucky strength transmits to tower of the refinement, after 20 minutes will force to return the ridge security area.");

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_Close()");
}

void KanturuTower::SetState_Notify_2()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_NOTIFY_2);
	this->SetEntrance(false);

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_Notify_2()");
}

void KanturuTower::SetState_End()
{
	this->SetState(KANTURU_TOWER_OF_REFINEMENT_END);
	this->SetEntrance(false);

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer || !pPlayer->Object::IsPlaying() )
			continue;

		if (  pPlayer->GetWorldId() != WORLD_KANTURU_BOSS )
			continue;

		pPlayer->MoveToGate(KANTURU_GATE_TOWER_OUT);
	}

	sLog->outInfo(LOG_KANTURU, "[ Tower ] SetState_End()");
}

void KanturuTower::ProcState_None()
{

}
	
void KanturuTower::ProcState_Revitalization()
{

}
	
void KanturuTower::ProcState_Notify_1()
{

}
	
void KanturuTower::ProcState_Close()
{

}
	
void KanturuTower::ProcState_Notify_2()
{

}
	
void KanturuTower::ProcState_End()
{

}