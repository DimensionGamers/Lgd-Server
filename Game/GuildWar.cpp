GuildWarMgr::GuildWarMgr()
{

}

GuildWarMgr::~GuildWarMgr()
{
	LIST_CLEAR(GuildWarDataList::iterator, guild_war_list);
}

void GuildWarMgr::Update()
{
	for ( GuildWarDataList::iterator it = this->guild_war_list.begin(); it != this->guild_war_list.end(); ++it )
	{
		if ( (*it)->GetResetTime()->Elapsed() ) 
		{
			(*it)->SetCount(0);
		}
	}
}

void GuildWarMgr::StartWarRequest(Player* pPlayer, const char * guild)
{
	if ( !pPlayer || !pPlayer->IsPlaying() || !pPlayer->IsLive() || pPlayer->IsServerQuit() )
		return;

	if ( !pPlayer->CanInteract() )
		return;

	if ( !sGameServer->IsGuildWarEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild War is disabled.");
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in guild to declare a war.");
		return;
	}

	if ( pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your guild is currently declaring a war.");
		return;
	}

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be guild master to declare a war.");
		return;
	}

	Guild* pGuildTarget = sGuildMgr->GetGuild(guild);

	if ( !pGuildTarget )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find guild %s.", guild);
		return;
	}

	if ( pGuild == pGuildTarget )
		return;

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

	if ( !pTarget || !pTarget->IsPlaying() || !pTarget->IsLive() || pTarget->IsServerQuit() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target guild master is offline.");
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None || pTarget->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You or target guild master is busy.");
		return;
	}

	if ( !this->IsWarAllowed(pGuild, pGuildTarget) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "A war against guild %s is not allowed.", pGuildTarget->GetName());
		return;
	}

	sGuildWarMgr->StartWarRequestResult(pPlayer, 1);

	pGuildTarget->GetWarData()->StartRequest(pGuild, GUILD_WAR_TYPE_NORMAL, sGameServer->GetGuildWarMaxWaitInterval());
	pGuild->GetWarData()->StartRequest(pGuildTarget, GUILD_WAR_TYPE_NORMAL, sGameServer->GetGuildWarMaxWaitInterval());

	pPlayer->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pTarget, 0, sGameServer->GetGuildWarMaxWaitInterval());
	pTarget->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pPlayer, 0, sGameServer->GetGuildWarMaxWaitInterval());

	sGuildWarMgr->StartWarRequestSend(pTarget, pGuild->GetName(), GUILD_WAR_TYPE_NORMAL);
}

void GuildWarMgr::StartWarRequestResult(Player* pPlayer, uint8 result)
{
	GUILD_WAR_REQUEST_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void GuildWarMgr::StartWarRequestSend(Player* pPlayer, const char * guild, GuildWarType type)
{
	GUILD_WAR_REQUEST_SEND pMsg(guild, type);
	pPlayer->SEND_PCT(pMsg);
}

void GuildWarMgr::StartWarResult(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::GuildWarRequest || 
		!pPlayer->GetInterfaceState()->GetTarget() || 
		!pPlayer->GetInterfaceState()->GetTarget()->IsPlayer() )
		return;

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return;

	if ( pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_DECLARE )
		return;

	POINTER_PCT_LOG(GUILD_WAR_REQUEST_RESULT, lpMsg, Packet, 0);

	Player* pTarget = pPlayer->GetInterfaceState()->GetTarget()->ToPlayer();
	Guild* pRequestGuild = pGuild->GetWarData()->GetEnemy();

	if ( !Player::IsPlayerBasicState(pTarget, false) )
	{
		lpMsg->result = 0;
	}
	else if ( pTarget->GetInterfaceState()->GetID() != InterfaceData::GuildWarRequest || pTarget->GetInterfaceState()->GetTarget() != pPlayer )
	{
		lpMsg->result = 0;
	}
	else if ( !pRequestGuild )
	{
		lpMsg->result = 0;
	}
	else if ( pRequestGuild->GetWarData()->GetEnemy() != pGuild || pRequestGuild->GetWarData()->GetState() != GUILD_WAR_STATE_DECLARE )
	{
		lpMsg->result = 0;
	}
	else if ( pRequestGuild->GetMember(0)->GetPlayer() != pTarget )
	{
		lpMsg->result = 0;
	}
	
	if ( lpMsg->result == 0 )
		return;

	pPlayer->GetInterfaceState()->Reset();
	pTarget->GetInterfaceState()->Reset();

	if ( pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_SOCCER )
	{
		sBattleSoccerMgr->ProcessTeams(pTarget, pPlayer, pRequestGuild, pGuild);
	}
	else
	{
		pRequestGuild->GetWarData()->SetTeam(GUILD_WAR_TEAM_BLUE);
		pGuild->GetWarData()->SetTeam(GUILD_WAR_TEAM_RED);

		pRequestGuild->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);
		pGuild->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);

		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			pRequestGuild->GetMember(i)->SetWarInvolved(false);
			pGuild->GetMember(i)->SetWarInvolved(false);
		}

		this->StartWar(pRequestGuild, pGuild);
	}
}

bool GuildWarMgr::IsWarAllowed(Guild* pGuild01, Guild* pGuild02)
{
	if ( sGameServer->GetGuildWarMax() == -1 )
		return true;

	for ( GuildWarDataList::iterator it = this->guild_war_list.begin(); it != this->guild_war_list.end(); ++it )
	{
		if ( ((*it)->GetGuild01() == pGuild01->GetID() && (*it)->GetGuild02() == pGuild02->GetID()) ||
			 ((*it)->GetGuild01() == pGuild02->GetID() && (*it)->GetGuild02() == pGuild01->GetID()) )
		{
			return (*it)->GetCount() < sGameServer->GetGuildWarMax();
		}
	}

	return true;
}

void GuildWarMgr::StartWar(Guild* pGuild01, Guild* pGuild02)
{
	GuildWarData * pData = nullptr;

	for ( GuildWarDataList::iterator it = this->guild_war_list.begin(); it != this->guild_war_list.end(); ++it )
	{
		if ( ((*it)->GetGuild01() == pGuild01->GetID() && (*it)->GetGuild02() == pGuild02->GetID()) ||
			 ((*it)->GetGuild01() == pGuild02->GetID() && (*it)->GetGuild02() == pGuild01->GetID()) )
		{
			pData = *it;
		}
	}

	if ( !pData )
	{
		pData = new GuildWarData(pGuild01->GetID(), pGuild02->GetID());
		pData->GetResetTime()->Start(HOUR * IN_MILLISECONDS);
		pData->GetWarTime()->Start(sGameServer->GetGuildWarIntervalTime());
		this->guild_war_list.push_back(pData);
	}
	
	if ( pData->GetWarTime()->Elapsed() )
	{
		pData->SetCount(1);
	}
	else
	{
		pData->IncreaseCount(1);
	}
}

void GuildWarMgr::EndWar(Guild* pLooser, Guild* pWinner, uint8 reason)
{
	Player* pPlayer = nullptr;
	
	if ( pLooser )
	{
		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			if ( !(pPlayer = pLooser->GetMember(i)->GetPlayer()) )
				continue;

			if ( !pLooser->GetMember(i)->IsWarInvolved() )
				continue;

			this->SendEnd(pPlayer, pWinner ? pWinner->GetName() : pLooser->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 3: 0);
			pLooser->GetMember(i)->SetWarInvolved(false);
		}

		pLooser->GetWarData()->Reset();
	}

	if ( pWinner )
	{
		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			if ( !(pPlayer = pWinner->GetMember(i)->GetPlayer()) )
				continue;

			if ( !pWinner->GetMember(i)->IsWarInvolved() )
				continue;

			this->SendEnd(pPlayer, pLooser ? pLooser->GetName() : pWinner->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 2: 1);
			pWinner->GetMember(i)->SetWarInvolved(false);
		}

		pWinner->GetWarData()->Reset();
	}

	if ( pLooser && pWinner )
	{
		for ( GuildWarDataList::iterator it = this->guild_war_list.begin(); it != this->guild_war_list.end(); ++it )
		{
			if ( ((*it)->GetGuild01() == pLooser->GetID() && (*it)->GetGuild02() == pWinner->GetID()) ||
				 ((*it)->GetGuild01() == pWinner->GetID() && (*it)->GetGuild02() == pLooser->GetID()) )
			{
				(*it)->GetWarTime()->Start(sGameServer->GetGuildWarIntervalTime());
			}
		}
	}
}

void GuildWarMgr::SendStart(Player* pPlayer, const char * guild, uint8 type, uint8 team)
{
	if ( !pPlayer )
		return;

	GUILD_WAR_START pMsg(guild, type, team);
	pPlayer->SEND_PCT(pMsg);
}

void GuildWarMgr::SendScore(Player* pPlayer, int32 score01, int32 score02)
{
	if ( !pPlayer )
		return;

	GUILD_WAR_SCORE pMsg(score01, score02, 0);
	pPlayer->SEND_PCT(pMsg);
}

void GuildWarMgr::SendEnd(Player* pPlayer, const char * guild, uint8 result)
{
	if ( !pPlayer )
		return;

	GUILD_WAR_END pMsg(result, guild);
	pPlayer->SEND_PCT(pMsg);
}

void GuildWarMgr::IncreaseScore(Player* pPlayer, Player* pVictim)
{
	Guild* pGuild01 = pPlayer->GuildGet();
	Guild* pGuild02 = pVictim->GuildGet();

	if ( !pGuild01 || !pGuild02 )
		return;

	uint8 rank = pVictim->GuildGetRanking();
	GuildWarType battle = pGuild01->GetWarData()->GetType();
	int32 add_score = 0;
	
	if ( battle == GUILD_WAR_TYPE_NORMAL )
	{
		add_score = rank == GUILD_RANK_MASTER ? sGameServer->GetGuildWarKillMasterPoints() :
					rank == GUILD_RANK_ASISTANT ? sGameServer->GetGuildWarKillAssistantPoints() :
					rank == GUILD_RANK_BATTLE_MASTER ? sGameServer->GetGuildWarKillBattlePoints() :
					sGameServer->GetGuildWarKillPoints();
	}
	else if ( battle == GUILD_WAR_TYPE_SOCCER )
	{
		add_score = rank == GUILD_RANK_MASTER ? sGameServer->GetBattleSoccerKillMasterPoints() :
					rank == GUILD_RANK_ASISTANT ? sGameServer->GetBattleSoccerKillAssistantPoints() :
					rank == GUILD_RANK_BATTLE_MASTER ? sGameServer->GetBattleSoccerKillBattlePoints() :
					sGameServer->GetBattleSoccerKillPoints();
	}

	Player* pMember = nullptr;

	pGuild01->GetWarData()->IncreaseScore(add_score);

	if ( pGuild01->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL )
	{
		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			if ( !(pMember = pGuild01->GetMember(i)->GetPlayer()) )
				continue;

			if ( !pGuild01->GetMember(i)->IsWarInvolved() )
				continue;

			this->SendScore(pMember, pGuild01->GetWarData()->GetScore(), pGuild02->GetWarData()->GetScore());
		}

		for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
		{
			if ( !(pMember = pGuild02->GetMember(i)->GetPlayer()) )
				continue;

			if ( !pGuild02->GetMember(i)->IsWarInvolved() )
				continue;

			this->SendScore(pMember, pGuild02->GetWarData()->GetScore(), pGuild01->GetWarData()->GetScore());
		}
	}

	if ( pGuild01->GetWarData()->GetScore() >= sGameServer->GetGuildWarMaxPoints() )
	{
		add_score = 1;

		if ( pGuild02->GetWarData()->GetScore() == 0 )
		{
			add_score = 3;
		}
		else if ( pGuild02->GetWarData()->GetScore() <= 10 )
		{
			add_score = 2;
		}

		sServerLink->GuildScore(pGuild01->GetID(), add_score);

		this->EndWar(pGuild02, pGuild01, BATTLE_SOCCER_END_REASON_SCORE);
	}
}