/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BattleSoccer.cpp"
*
*/
CBattleSoccerMgr::CBattleSoccerMgr()
{
	this->SetState_None();
}

CBattleSoccerMgr::~CBattleSoccerMgr()
{

}

void CBattleSoccerMgr::Update()
{
	switch ( this->GetState() )
	{
	case BATTLE_SOCCER_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case BATTLE_SOCCER_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case BATTLE_SOCCER_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case BATTLE_SOCCER_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

void CBattleSoccerMgr::SetState_None()
{
	this->SetState(BATTLE_SOCCER_STATE_NONE);
	this->GetTime()->Start(0);
	this->GetBallMoveTime()->Reset();
	this->SetBallMove(false);
	
	GUILD_WAR_TEAM_LOOP(i)
	{
		this->GetTeam(i)->Reset();
	}
}
	
void CBattleSoccerMgr::SetState_StandBy()
{
	this->SetState(BATTLE_SOCCER_STATE_STANDBY);
	this->GetTime()->Start(sGameServer->GetGuildWarMaxWaitInterval() + (5 * IN_MILLISECONDS));
	this->GetBallMoveTime()->Reset();
	this->SetBallMove(false);
}
	
void CBattleSoccerMgr::SetState_Playing()
{
	this->SetState(BATTLE_SOCCER_STATE_PLAYING);
	this->GetTime()->Start(sGameServer->GetBattleSoccerDuration());
	this->GetBallMoveTime()->Reset();
	this->SetBallMove(false);

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->GetPlayer(h)->GetPlayer();

			if ( !pPlayer )
				continue;

			sGuildWarMgr->SendStart(pPlayer, this->GetTeam(oposite)->GetGuild()->GetName(), GUILD_WAR_TYPE_SOCCER, i);
			sGuildWarMgr->SendScore(pPlayer, 0, 0);
			pPlayer->TeleportToLocation(WORLD_STADIUM, g_BattleSoccerStartZone[i].x1 + h, g_BattleSoccerStartZone[i].y1, -1, pPlayer->GetInstance());
		}

		pTeam->GetGuild()->SendNoticeArgToMembers(NOTICE_GLOBAL, "Battle Soccer against %s started!!", this->GetTeam(oposite)->GetGuild()->GetName());
	}
}
	
void CBattleSoccerMgr::SetState_Playend()
{
	this->SetState(BATTLE_SOCCER_STATE_PLAYEND);
	this->GetTime()->Start(10 * IN_MILLISECONDS);
	this->GetBallMoveTime()->Reset();
	this->SetBallMove(false);
}

void CBattleSoccerMgr::ProcState_None()
{

}
	
void CBattleSoccerMgr::ProcState_StandBy()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_None();
	}
}
	
void CBattleSoccerMgr::ProcState_Playing()
{
	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->GetPlayer(h)->GetPlayer();

			if ( !pPlayer )
				continue;

			if ( pPlayer->GetRegenStatus() != REGEN_NONE )
				continue;

			if ( !pPlayer->Object::IsPlaying() )
			{
				pTeam->GetPlayer(h)->SetPlayer(nullptr);

				if ( pTeam->GetPlayer(h)->IsMaster() )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}

				continue;
			}

			if ( pPlayer->GetWorldId() != WORLD_STADIUM || 
				((pPlayer->GetX() < g_BattleSoccerFullGround.x1 || pPlayer->GetX() > g_BattleSoccerFullGround.x2) ||
				 (pPlayer->GetY() < g_BattleSoccerFullGround.y1 || pPlayer->GetY() > g_BattleSoccerFullGround.y2)) )
			{
				sGuildWarMgr->SendEnd(pPlayer, pTeam->GetGuild()->GetName(), 7);

				pTeam->GetPlayer(h)->SetPlayer(nullptr);

				if ( pTeam->GetPlayer(h)->IsMaster() )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}

				continue;
			}

			if ( pTeam->GetPartyID() != pPlayer->GetPartyID() )
			{
				if ( pTeam->GetPlayer(h)->IsMaster() )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}
				else
				{
					pTeam->GetPlayer(h)->SetPlayer(nullptr);

					sGuildWarMgr->SendEnd(pPlayer, pTeam->GetGuild()->GetName(), 7);
					pPlayer->TeleportToGate(17);
				}

				continue;
			}
		}
	}

	bool send_score = false;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		if ( pTeam->GetScore() != pTeam->GetGuild()->GetWarData()->GetScore() )
		{
			pTeam->SetScore(pTeam->GetGuild()->GetWarData()->GetScore());
			send_score = true;
		}
	}

	if ( send_score )
	{
		this->SendScore();
	}

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		if ( pTeam->GetScore() >= sGameServer->GetBattleSoccerMaxPoints() )
		{
			int32 add_score = 1;

			if ( this->GetTeam(oposite)->GetScore() == 0 )
			{
				add_score = 3;
			}
			else if ( this->GetTeam(oposite)->GetScore() <= 10 )
			{
				add_score = 2;
			}

			this->Finish(BATTLE_SOCCER_END_REASON_SCORE, i, oposite, add_score);
			return;
		}
	}

	if ( !this->IsBallMove() && this->GetBallMoveTime()->Elapsed(10 * IN_MILLISECONDS) )
	{
		this->SetBallMove(true);
	}
	
	if ( this->GetTime()->Elapsed() )
	{
		this->Finish(BATTLE_SOCCER_END_REASON_TIME_OUT, GUILD_WAR_TEAM_MAX, GUILD_WAR_TEAM_MAX, -1);
	}
}
	
void CBattleSoccerMgr::ProcState_Playend()
{
	if ( this->GetTime()->Elapsed() )
	{
		this->MovePlayersOut();
		this->SetState_None();
	}
}

void CBattleSoccerMgr::UpdateBall(Monster* Ball)
{
	if ( this->GetState() != BATTLE_SOCCER_STATE_PLAYING )
		return;

	int16 x = Ball->GetX();
	int16 y = Ball->GetY();

	GUILD_WAR_TEAM_LOOP(i)
	{
		if ( (x >= g_BattleSoccerZones[i].x1 && x <= g_BattleSoccerZones[i].x2) &&
			 (y >= g_BattleSoccerZones[i].y1 && y <= g_BattleSoccerZones[i].y2) )
		{
			Ball->Kill();

			this->GetTeam(i)->GetGuild()->GetWarData()->IncreaseScore(sGameServer->GetBattleSoccerGoalPoints());
		}
	}
}

bool CBattleSoccerMgr::IsInField(Player const* pPlayer) const
{
	if ( !pPlayer )
		return false;

	Guild const* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return false;

	if ( this->GetState() != BATTLE_SOCCER_STATE_PLAYING &&
		 this->GetState() != BATTLE_SOCCER_STATE_PLAYEND )
		return false;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam const* pTeam = this->GetTeam(i);

		if ( pTeam->GetGuild() != pGuild )
			continue;

		PARTY_LOOP(h)
		{
			if ( pTeam->GetPlayer(h)->GetPlayer() == pPlayer )
				return true;
		}
	}

	return false;
}

bool CBattleSoccerMgr::GetRespawnLocation(int16 & x, int16 & y, Player* pPlayer)
{
	if ( !this->IsInField(pPlayer) )
		return false;

	int16 px = g_BattleSoccerGround.x2 - g_BattleSoccerGround.x1;
	int16 py = g_BattleSoccerGround.y2 - g_BattleSoccerGround.y1;

	if ( px < 1 )
		px = 1;

	if ( py < 1 )
		py = 1;
		
	x = g_BattleSoccerGround.x1 + Random(px);
	y = g_BattleSoccerGround.y1 + Random(py);
	return true;
}

bool CBattleSoccerMgr::IsAttackAllowed(Player* pPlayer01, Player* pPlayer02)
{
	if ( !pPlayer01 || !pPlayer02 )
		return true;

	Guild* pGuild01 = pPlayer01->GuildGet();
	Guild* pGuild02 = pPlayer02->GuildGet();

	if ( !pGuild01 || !pGuild02 )
		return true;

	uint8 Team01 = GUILD_WAR_TEAM_MAX;
	uint8 Team02 = GUILD_WAR_TEAM_MAX;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam const* pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		if ( pTeam->GetGuild() == pGuild01 )
		{
			PARTY_LOOP(h)
			{
				if ( pTeam->GetPlayer(h)->GetPlayer() == pPlayer01 )
				{
					Team01 = i;
				}
			}
		}
		if ( pTeam->GetGuild() == pGuild02 )
		{
			PARTY_LOOP(h)
			{
				if ( pTeam->GetPlayer(h)->GetPlayer() == pPlayer02 )
				{
					Team02 = i;
				}
			}
		}
	}

	if ( Team01 == GUILD_WAR_TEAM_MAX && Team02 == GUILD_WAR_TEAM_MAX )
	{
		return true;
	}

	if ( Team01 == GUILD_WAR_TEAM_MAX && Team02 != GUILD_WAR_TEAM_MAX )
	{
		return false;
	}

	if ( Team01 != GUILD_WAR_TEAM_MAX && Team02 == GUILD_WAR_TEAM_MAX )
	{
		return false;
	}

	if ( Team01 == Team02 )
	{
		return false;
	}
	
	if ( this->GetState() == BATTLE_SOCCER_STATE_PLAYING )
	{
		return true;
	}

	return false;
}

void CBattleSoccerMgr::MovePlayersOut()
{
	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = this->GetTeam(i);

		if ( !pTeam )
		{
			continue;
		}

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->GetPlayer(h)->GetPlayer();

			if ( !pPlayer )
				continue;

			if ( !pPlayer->IsPlaying() || pPlayer->GetWorldId() != WORLD_STADIUM || pPlayer->IsServerQuit() )
				continue;

			pPlayer->TeleportToGate(17);
		}

		pTeam->Reset();
	}
}

void CBattleSoccerMgr::ProcessRequest(Player* pPlayer, const char * guild)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->CanInteract() )
		return;

	if ( !sGameServer->IsBattleSoccerEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Battle soccer is disabled.");
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in guild to start a battle soccer.");
		return;
	}

	if ( pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your guild is currently declaring a war.");
		return;
	}

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be guild master to start a battle soccer.");
		return;
	}

	Party* pParty01 = pPlayer->GetParty();

	if ( !pParty01 )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start a battle soccer.");
		return;
	}

	if ( !this->PartySuccess(pParty01, pGuild) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}
	
	Guild* pGuildTarget = sGuildMgr->GetGuild(guild);

	if ( !pGuildTarget )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find guild %s.", guild);
		return;
	}

	if ( pGuild == pGuildTarget )
	{
		return;
	}

	if ( pGuildTarget->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target guild is currently declaring a war.");
		return;
	}

	if ( pGuild->GetAlliance() && pGuild->GetAlliance() == pGuildTarget->GetAlliance() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't declare war to a guild that belogs to the same alliance.");
		return;
	}

	Player* pTarget = sObjectMgr->FindPlayerByGUID(pGuildTarget->GetID());

	if ( !Player::IsPlayerBasicState(pTarget, false) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target guild master is not available.");
		return;
	}

	Party* pParty02 = pTarget->GetParty();

	if ( !pParty02 )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in party to start a battle soccer.");
		return;
	}

	if ( !this->PartySuccess(pParty02, pGuildTarget) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	if ( this->GetState() != BATTLE_SOCCER_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Battle soccer field is currently in use.");
		return;
	}

	this->SetTeamData(pGuild, pGuildTarget);

	sGuildWarMgr->StartWarRequestResult(pPlayer, 1);

	pGuildTarget->GetWarData()->StartRequest(pGuild, GUILD_WAR_TYPE_SOCCER, sGameServer->GetGuildWarMaxWaitInterval());
	pGuild->GetWarData()->StartRequest(pGuildTarget, GUILD_WAR_TYPE_SOCCER, sGameServer->GetGuildWarMaxWaitInterval());

	pPlayer->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pTarget, 0, sGameServer->GetGuildWarMaxWaitInterval());
	pTarget->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pPlayer, 0, sGameServer->GetGuildWarMaxWaitInterval());

	sGuildWarMgr->StartWarRequestSend(pTarget, pGuild->GetName(), GUILD_WAR_TYPE_SOCCER);
}

bool CBattleSoccerMgr::PartySuccess(Party* pParty, Guild* pGuild)
{
	PARTY_LOOP(i)
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
		{
			return false;
		}

		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if ( !Player::IsPlayerBasicState(pMember, false) )
		{
			return false;
		}

		if ( pMember->GetRegenStatus() != REGEN_NONE )
		{
			return false;
		}

		if ( pMember->GetWorldId() != WORLD_STADIUM )
		{
			return false;
		}

		if ( pMember->GetInterfaceState()->GetID() != InterfaceData::None )
		{
			return false;
		}

		if ( pMember->GuildGet() != pGuild )
		{
			return false;
		}
	}

	return true;
}

void CBattleSoccerMgr::ProcessTeams(Player* pPlayer01, Player* pPlayer02, Guild* pGuild01, Guild* pGuild02)
{
	if ( this->GetState() != BATTLE_SOCCER_STATE_STANDBY )
		return;

	if ( this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetGuild() != pGuild01 || this->GetTeam(GUILD_WAR_TEAM_RED)->GetGuild() != pGuild02 )
		return;

	Party* pParty01 = pPlayer01->GetParty();
	Party* pParty02 = pPlayer02->GetParty();

	if ( !pParty01 )
	{
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start battlesoccer.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s is not in party.", pPlayer01->GetName());
		return;
	}

	if ( !pParty02 )
	{
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start battlesoccer.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s is not in party.", pPlayer02->GetName());
		return;
	}

	if ( !this->PartySuccess(pParty01, pGuild01) )
	{
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");

		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	if ( !this->PartySuccess(pParty02, pGuild02) )
	{
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");

		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	this->GetTeam(GUILD_WAR_TEAM_BLUE)->SetPartyID(pParty01->GetID());
	this->GetTeam(GUILD_WAR_TEAM_RED)->SetPartyID(pParty02->GetID());

	PARTY_LOOP(i)
	{
		this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(i)->Reset();
		this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(i)->SetPlayer(pParty01->GetMember(i)->GetPlayer());

		if ( this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(i)->GetPlayer() )
		{
			this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(i)->SetMaster(this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(i)->GetPlayer()->GuildGetRanking() == GUILD_RANK_MASTER ? true: false);
		}
		
		this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(i)->Reset();
		this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(i)->SetPlayer(pParty02->GetMember(i)->GetPlayer());

		if ( this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(i)->GetPlayer() )
		{
			this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(i)->SetMaster(this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(i)->GetPlayer()->GuildGetRanking() == GUILD_RANK_MASTER ? true: false);
		}
	}

	pGuild01->GetWarData()->SetTeam(GUILD_WAR_TEAM_BLUE);
	pGuild02->GetWarData()->SetTeam(GUILD_WAR_TEAM_RED);

	pGuild01->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);
	pGuild02->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);

	this->SetState_Playing();
}

void CBattleSoccerMgr::SetTeamData(Guild* pGuild01, Guild* pGuild02)
{
	this->SetState_StandBy();

	this->GetTeam(GUILD_WAR_TEAM_BLUE)->Reset();
	this->GetTeam(GUILD_WAR_TEAM_BLUE)->SetGuild(pGuild01);

	this->GetTeam(GUILD_WAR_TEAM_RED)->Reset();
	this->GetTeam(GUILD_WAR_TEAM_RED)->SetGuild(pGuild02);
}

void CBattleSoccerMgr::Finish(uint8 reason, uint8 winner, uint8 looser, int32 score)
{
	if ( score == -1 )
	{
		if ( winner == GUILD_WAR_TEAM_MAX || looser == GUILD_WAR_TEAM_MAX )
		{
			if ( this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetScore() < this->GetTeam(GUILD_WAR_TEAM_RED)->GetScore() )
			{
				winner = GUILD_WAR_TEAM_RED;
				looser = GUILD_WAR_TEAM_BLUE;
			}
			else if ( this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetScore() > this->GetTeam(GUILD_WAR_TEAM_RED)->GetScore() )
			{
				winner = GUILD_WAR_TEAM_BLUE;
				looser = GUILD_WAR_TEAM_RED;
			}
		}

		if ( winner != GUILD_WAR_TEAM_MAX && looser != GUILD_WAR_TEAM_MAX )
		{
			if ( this->GetTeam(winner)->GetScore() < this->GetTeam(looser)->GetScore() )
			{
				score = 1;
			}
			else if ( this->GetTeam(winner)->GetScore() > this->GetTeam(looser)->GetScore() )
			{
				score = 3;
			}
			else
			{
				score = 2;
			}
		}
	}

	if ( score != -1 && winner != GUILD_WAR_TEAM_MAX )
		sServerLink->GuildScore(this->GetTeam(winner)->GetGuild()->GetID(), score);

	if ( winner == GUILD_WAR_TEAM_MAX || looser == GUILD_WAR_TEAM_MAX ) // Empatados
	{
		PARTY_LOOP(h)
		{
			sGuildWarMgr->SendEnd(this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(h)->GetPlayer(), this->GetTeam(GUILD_WAR_TEAM_RED)->GetGuild()->GetName(), 7);
			sGuildWarMgr->SendEnd(this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(h)->GetPlayer(), this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetGuild()->GetName(), 7);
		}
	}
	else
	{
		PARTY_LOOP(h)
		{
			sGuildWarMgr->SendEnd(this->GetTeam(winner)->GetPlayer(h)->GetPlayer(), this->GetTeam(looser)->GetGuild()->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 2: 1);
			sGuildWarMgr->SendEnd(this->GetTeam(looser)->GetPlayer(h)->GetPlayer(), this->GetTeam(winner)->GetGuild()->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 3: 0);
		}
	}
								
	this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetGuild()->GetWarData()->Reset();
	this->GetTeam(GUILD_WAR_TEAM_RED)->GetGuild()->GetWarData()->Reset();

	this->SetState_Playend();
}

void CBattleSoccerMgr::SendScore()
{
	PARTY_LOOP(h)
	{
		sGuildWarMgr->SendScore(this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetPlayer(h)->GetPlayer(), this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetScore(), this->GetTeam(GUILD_WAR_TEAM_RED)->GetScore());
		sGuildWarMgr->SendScore(this->GetTeam(GUILD_WAR_TEAM_RED)->GetPlayer(h)->GetPlayer(), this->GetTeam(GUILD_WAR_TEAM_RED)->GetScore(), this->GetTeam(GUILD_WAR_TEAM_BLUE)->GetScore());
	}
}