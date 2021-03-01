CommonRace::CommonRace()
{
	this->SetGateReached(0);
}

CommonRace::~CommonRace()
{
	this->Clear();
}

void CommonRace::Clear()
{
	LIST_CLEAR(CommonRaceGateList::iterator, this->gate_list);
}

void CommonRace::Start(int32 duration, int32 notify)
{
	if ( this->GetState() != COMMON_RACE_STATE_NONE )
		return;

	this->SetDuration(duration);
	this->SetNotifyTime(notify);

	if ( notify > 0 )
	{
		this->SetState_Notify();
	}
	else
	{
		this->SetState_Playing();
	}
}

void CommonRace::Update()
{
	switch ( this->GetState() )
	{
	case COMMON_RACE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case COMMON_RACE_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case COMMON_RACE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;
	}
}

void CommonRace::SetState_None()
{
	this->SetState(COMMON_RACE_STATE_NONE);
	this->RunTime(0);
	this->SetGateReached(0);
	this->player_data.clear();

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "%s race finished.", this->GetRaceName().c_str());

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_None()", this->GetRaceName().c_str());
}
		
void CommonRace::SetState_Notify()
{
	this->SetState(COMMON_RACE_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);
	this->SetGateReached(0);
	this->player_data.clear();

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_Notify()", this->GetRaceName().c_str());
}
		
void CommonRace::SetState_Playing()
{
	this->SetState(COMMON_RACE_STATE_PLAYING);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);
	this->SetGateReached(0);
	this->player_data.clear();

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "%s race started.", this->GetRaceName().c_str());

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_Playing()", this->GetRaceName().c_str());
}

void CommonRace::ProcState_None()
{

}
		
void CommonRace::ProcState_Notify()
{
	this->SendRemainMinutes("%d minute(s) left before " + this->GetRaceName() + " race starts.", 5, 15);
	
	if ( this->TimePassed() )
		this->SetState_Playing();
}

void CommonRace::ProcState_Playing()
{
	this->SendRemainMinutes("%d minute(s) left before " + this->GetRaceName() + " race ends.", 5, 10);

	if ( this->TimePassed() )
		this->SetState_None();
}

void CommonRace::PlayerInGate(Player* pPlayer, int32 gate)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( this->GetState() != COMMON_RACE_STATE_PLAYING )
	{
		return;
	}

	CommonRacePlayer & PlayerData = this->player_data[pPlayer->GetGUID()];

	for ( size_t i = 0; i < this->gate_list.size(); ++i )
	{
		if ( this->gate_list[i]->GetGate() != gate )
			continue;

		if ( PlayerData.GateList.size() == i )
		{
			if ( !PlayerData.IsParticipating() )
			{
				PlayerData.SetStartedTime(time(nullptr));
			}

			PlayerData.SetParticipating(true);
			PlayerData.GateList.push_back(gate);
		
			if ( this->GetGateReached() == i )
			{
				this->IncreaseGateReached(1);

				if ( this->GetGateReached() == this->gate_list.size() )
				{
					sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ %s race ] Finished! %s is the winner",
						this->GetRaceName().c_str(), pPlayer->GetName());

					if ( this->GetRaceName() == "Dungeon" )
					{
						sItemBagMgr->RunItemBag(pPlayer, sGameServer->GetDungeonRaceReward(), Item(), true, GremoryCaseReward::EVENT);
					}
					else if ( this->GetRaceName() == "LostTower" )
					{
						sItemBagMgr->RunItemBag(pPlayer, sGameServer->GetLosttowerRaceReward(), Item(), true, GremoryCaseReward::EVENT);
					}

					SQLTransaction trans = MuDatabase.BeginTransaction();

					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_RACE);
					stmt->setUInt32(0, pPlayer->GetGUID());
					stmt->setString(1, this->GetRaceName());
					stmt->setString(2, TimeToTimestampStr(PlayerData.GetStartedTime()));
					trans->Append(stmt);

					MuDatabase.CommitTransaction(trans);

					this->SetState_None();
				}
				else
				{
					sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ %s race ] %s reached gate number %d",
						this->GetRaceName().c_str(), pPlayer->GetName(), i + 1);
				}
			}

			return;
		}

		break;
	}

	this->ResetPlayer(pPlayer);
}

void CommonRace::ResetPlayer(Player* pPlayer)
{
	if ( this->GetState() != COMMON_RACE_STATE_PLAYING )
		return;

	CommonRacePlayer & PlayerData = this->player_data[pPlayer->GetGUID()];

	if ( PlayerData.IsParticipating() )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have been disqualified from the race.");
	}

	PlayerData.SetParticipating(false);
	PlayerData.GateList.clear();
}