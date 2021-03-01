LastManStanding::LastManStanding()
{
	this->SetState_None();
}

LastManStanding::~LastManStanding()
{
	
}

void LastManStanding::Start(int32 notify)
{
	if ( !sGameServer->IsLastManStandingEnabled() )
	{
		return;
	}

	if ( this->GetState() != LAST_MAN_STANDING_STATE_NONE )
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
	
void LastManStanding::Update()
{
	if ( !sGameServer->IsLastManStandingEnabled() )
	{
		return;
	}

	switch ( this->GetState() )
	{
	case LAST_MAN_STANDING_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case LAST_MAN_STANDING_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case LAST_MAN_STANDING_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case LAST_MAN_STANDING_STATE_STANDBY:
		{
			this->ProcState_Standby();
		} break;

	case LAST_MAN_STANDING_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case LAST_MAN_STANDING_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void LastManStanding::SetState_None()
{
	this->SetState(LAST_MAN_STANDING_STATE_NONE);
	this->SetType(LAST_MAN_STANDING_TYPE_LEVEL);
	this->GetTime()->Reset();
	this->SetCurrentMinute(-1);
	this->SetCurrentSecond(-1);
	this->SetNotifyTime(0);

	this->MovePlayersOut();

	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::SetState_Notify()
{
	this->SetState(LAST_MAN_STANDING_STATE_NOTIFY);
	this->SetType(LAST_MAN_STANDING_TYPE_LEVEL);
	this->GetTime()->Start(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);
	this->SetCurrentSecond(-1);

	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::SetState_Open()
{
	this->SetState(LAST_MAN_STANDING_STATE_OPEN);
	this->SetType(LAST_MAN_STANDING_TYPE_LEVEL);
	this->GetTime()->Start(5 * MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);

	sServerLink->NoticeSendNormal(NOTICE_GLOBAL, "[Last Man Standing] is open!");

	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::SetState_Standby()
{
	this->SetState(LAST_MAN_STANDING_STATE_STANDBY);
	this->SetType(LAST_MAN_STANDING_TYPE_LEVEL);
	this->GetTime()->Start(MINUTE * IN_MILLISECONDS);
	this->SetCurrentMinute(-1);
	this->SetCurrentSecond(-1);

	sServerLink->NoticeSendNormal(NOTICE_GLOBAL, "[Last Man Standing] has been closed.");

	this->SetInstance();

	sObjectMgr->SendNoticeToMap(WORLD_LAST_MAN_STANDING, NOTICE_NORMAL_BLUE, "[Last Man Standing] battle will start in 60 second(s).");

	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::SetState_Playing()
{
	this->SetState(LAST_MAN_STANDING_STATE_PLAYING);
	this->GetTime()->Start(sGameServer->GetLastManStandingDuration());
	this->SetCurrentSecond(-1);
	
	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::SetState_End()
{
	this->SetState(LAST_MAN_STANDING_STATE_END);
	this->GetTime()->Start(30 * IN_MILLISECONDS);

	sServerLink->EventStateUpdate(EVENT_LAST_MAN_STANDING, this->GetState(), 0);

	sLog->outInfo("last_man_standing", "%s", __FUNCTION__);
}

void LastManStanding::ProcState_None()
{

}

void LastManStanding::ProcState_Notify()
{
	this->SendRemainMinutes("[Last Man Standing] is open after %d mins.", 5, 15);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Open();
	}
}

void LastManStanding::ProcState_Open()
{
	this->SendRemainMinutes("[Last Man Standing] is closed after %d mins.", 1, 5);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Standby();
	}
}

void LastManStanding::ProcState_Standby()
{
	if ( (this->GetTime()->GetRemain() <= 30000) && (this->GetTime()->GetRemain() >= 0) )
	{
		int32 remian_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		if ( this->GetCurrentSecond() != remian_seconds )
		{
			this->SetCurrentSecond(remian_seconds);

			sObjectMgr->SendNoticeToMap(WORLD_LAST_MAN_STANDING, NOTICE_GLOBAL, "[Last Man Standing] battle will start in %d second(s).", remian_seconds + 1);
		}
	}
	
	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void LastManStanding::ProcState_Playing()
{
	this->CheckWinner();

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_End();
	}
}

void LastManStanding::ProcState_End()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_None();
	}
}

void LastManStanding::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
{
	if ( !this->IsMinuteElapsed(period) )
	{
		return;
	}

	if ( max_minute != -1 && (this->GetCurrentMinute() + 1) > max_minute )
	{
		return;
	}

	sServerLink->NoticeSend(NOTICE_GLOBAL, message.c_str(), this->GetCurrentMinute() + 1);
}

void LastManStanding::MovePlayersOut()
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_LAST_MAN_STANDING )
		{
			continue;
		}

		pPlayer->EventResetData();
		pPlayer->MoveToGate(503);
	}
}

void LastManStanding::PlayerKill(Player* pPlayer)
{
	pPlayer->AddEventScore(1, pPlayer->GetEventGround(), sGameServer->GetLastManStandingKillScore());
}

struct LastManStandingCount
{
	LastManStandingCount()
	{
		this->count = 0;
		this->pPlayer = 0;
	}

	int32 count;
	Player* pPlayer;
};

void LastManStanding::CheckWinner()
{
	std::map<int32, LastManStandingCount> PlayerInstanceMap;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_LAST_MAN_STANDING )
		{
			continue;
		}

		if ( pPlayer->GetEventId() != EVENT_LAST_MAN_STANDING )
		{
			continue;
		}

		LastManStandingCount & data = PlayerInstanceMap[pPlayer->GetEventGround()];
		++data.count;
		data.pPlayer = pPlayer;
	}

	for ( std::map<int32, LastManStandingCount>::const_iterator it = PlayerInstanceMap.begin(); it != PlayerInstanceMap.end(); ++it )
	{
		Player* pPlayer = it->second.pPlayer;

		if (it->second.count == 1 && pPlayer)
		{
			pPlayer->AddEventScore(1, pPlayer->GetEventGround(), sGameServer->GetLastManStandingWinScore());

			switch ( this->GetType() )
			{
			case LAST_MAN_STANDING_TYPE_NORMAL:
				{
					sServerLink->NoticeSend(NOTICE_GLOBAL, "[Last Man Standing] %s is the winner.", pPlayer->GetName());

					sLog->outInfo("last_man_standing", "%s --  %u/%s is the winner.", __FUNCTION__, pPlayer->GetGUID(), pPlayer->GetName());
				} break;

			case LAST_MAN_STANDING_TYPE_LEVEL:
				{
					sServerLink->NoticeSend(NOTICE_GLOBAL, "[Last Man Standing] %s is the level %d-%d winner.", pPlayer->GetName(), (pPlayer->GetEventGround() * sGameServer->GetLastManStandingLevelRange()) + 1, (pPlayer->GetEventGround() * sGameServer->GetLastManStandingLevelRange()) + sGameServer->GetLastManStandingLevelRange());

					sLog->outInfo("last_man_standing", "%s --  %u/%s is the level %d-%d winner.", __FUNCTION__, pPlayer->GetGUID(), pPlayer->GetName(), (pPlayer->GetEventGround() * sGameServer->GetLastManStandingLevelRange()) + 1, (pPlayer->GetEventGround() * sGameServer->GetLastManStandingLevelRange()) + sGameServer->GetLastManStandingLevelRange());
				} break;

			case LAST_MAN_STANDING_TYPE_RACE:
				{
					sServerLink->NoticeSend(NOTICE_GLOBAL, "[Last Man Standing] %s is the %s class winner.", pPlayer->GetName(), CharacterToName[pPlayer->GetClass()]);

					sLog->outInfo("last_man_standing", "%s -- %u/%s is the %s class winner.", __FUNCTION__, pPlayer->GetGUID(), pPlayer->GetName(), CharacterToName[pPlayer->GetClass()]);
				} break;
			}

			std::string itembag = "LastManStanding_" + std::to_string(pPlayer->GetEventGround());

			sItemBagMgr->RunItemBag(pPlayer, itembag, Item(), true, GremoryCaseReward::EVENT);

			pPlayer->EventResetData();
		}
	}
}

void LastManStanding::EnterRequest(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetTotalLevel() < sGameServer->GetLastManStandingMinLevel() )
	{
		pPlayer->SendMessageBox(0, "Last Man Standing", "Min level required %d", sGameServer->GetLastManStandingMinLevel());
		return;
	}

	if ( sGameServer->GetLastManStandingMaxPerPC() > 0 )
	{
		int32 count = 0;
		PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
		{
			Player* pCharacter = it->second;

			if ( !pCharacter )
			{
				continue;
			}

			if ( pCharacter->GetWorldId() != WORLD_LAST_MAN_STANDING )
			{
				continue;
			}

			if ( pCharacter->GetAccountData()->GetDiskSerial() == pPlayer->GetAccountData()->GetDiskSerial() )
			{
				++count;
			}
		}

		if ( count >= sGameServer->GetLastManStandingMaxPerPC() )
		{
			pPlayer->SendMessageBox(0, "Last Man Standing", "Max allowed characters per pc %d", sGameServer->GetLastManStandingMaxPerPC());
			return;
		}
	}

	sLog->outInfo("last_man_standing", "%s --  %s", __FUNCTION__, pPlayer->BuildLog().c_str());
	
	pPlayer->MoveToGate(LAST_MAN_STANDING_GATE);

	pPlayer->ClearAllBuff(0);
}

void LastManStanding::SetInstance()
{
	//this->SetType(Random<uint8>(LAST_MAN_STANDING_TYPE_MAX));

	if ( sGameServer->IsLastManStandingClassic() )
	{
		this->SetType(LAST_MAN_STANDING_TYPE_NORMAL);
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_LAST_MAN_STANDING )
		{
			continue;
		}

		switch ( this->GetType() )
		{
		case LAST_MAN_STANDING_TYPE_NORMAL:
			{
				pPlayer->SetInstance(-1);

				pPlayer->SendNotice(NOTICE_GLOBAL, "[Last Man Standing] Normal Battle");
			} break;

		case LAST_MAN_STANDING_TYPE_LEVEL:
			{
				pPlayer->SetInstance((pPlayer->GetTotalLevel() - 1) / sGameServer->GetLastManStandingLevelRange());

				pPlayer->SendNotice(NOTICE_GLOBAL, "[Last Man Standing] Level Battle");
			} break;

		case LAST_MAN_STANDING_TYPE_RACE:
			{
				pPlayer->SetInstance(pPlayer->GetClass());

				pPlayer->SendNotice(NOTICE_GLOBAL, "[Last Man Standing] Race Battle");
			} break;
		}

		pPlayer->SetEventId(EVENT_LAST_MAN_STANDING);
		pPlayer->SetEventGround(pPlayer->GetInstance());
	}
}