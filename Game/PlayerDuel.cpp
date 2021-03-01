/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerDuel.cpp"
*
*/

void Player::DuelRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !sGameServer->IsDuelEnabled() )
	{
		return;
	}

	POINTER_PCT_LOG(DUEL_REQUEST, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		return;
	}

	if ( !this->CanInteract(pPlayer, true) )
	{
		return;
	}

	if ( !pPlayer->IsFlag(CHARACTER_FLAG_REQUEST) )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are busy right now.");
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target is busy right now.");
		return;
	}
	
	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_DUEL) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to send duel request here.");
		return;
	}

	if ( this->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request in Castle Siege Battle.");
		return;
	}

	if ( !this->SameDimension(pPlayer) )
	{
		return;
	}

	if ( Util::Distance(this->GetX(), this->GetY(), pPlayer->GetX(), pPlayer->GetY()) > 5 )
	{
		return;
	}

	if ( Guild* pGuild = this->GuildGet() )
	{
		if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request while in Guild War.");
			return;
		}
	}

	if ( Guild* pGuild = pPlayer->GuildGet() )
	{
		if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target use is currently in Guild War.");
			return;
		}
	}

	if ( this->GetTotalLevel() < sGameServer->dueling_min_level.get() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Open only for level %d or higher.", sGameServer->dueling_min_level.get());
		return;
	}

	if ( pPlayer->GetTotalLevel() < sGameServer->dueling_min_level.get() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Open only for level %d or higher.", sGameServer->dueling_min_level.get());
		return;
	}

	if ( this->IsDueling() || pPlayer->IsDueling() )
	{
		return;
	}

	if ( !sGameServer->IsDuelClassic() )
	{
		if ( !this->MoneyHave(sGameServer->GetDuelRequiredZen()) )
		{
			this->DuelResult(0x1E, pPlayer->GetEntry(), pPlayer->GetName());
			return;
		}

		if ( !pPlayer->MoneyHave(sGameServer->GetDuelRequiredZen()) )
		{
			this->DuelResult(0x21, pPlayer->GetEntry(), pPlayer->GetName());
			return;
		}
	}

	if ( this->IsInSelfDefense() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request while autodefense is active.");
		return;
	}

	if ( pPlayer->IsInSelfDefense() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target user has autodefense activated.");
		return;
	}

	if ( sGameServer->IsDuelClassic() )
	{
		this->GetInterfaceState()->Set(InterfaceData::Duel, pPlayer, 0, sGameServer->dueling_max_wait_interval.get());
		this->SetDuelClassic(true);
		this->SetDuelWorld(this->GetWorldId());
		this->SetDuelTarget(pPlayer);
	
		pPlayer->GetInterfaceState()->Set(InterfaceData::Duel, this, 0, sGameServer->dueling_max_wait_interval.get());
		pPlayer->SetDuelClassic(true);
		pPlayer->SetDuelWorld(pPlayer->GetWorldId());
		pPlayer->SetDuelTarget(this);

		this->SendNotice(NOTICE_NORMAL_BLUE, "You have requested duel to %s", pPlayer->GetName());
	
		DUEL_REQUEST_SEND pMsg(this->GetEntry(), this->GetName());
		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
	else
	{
		DuelRoom * room = sDuelMgr->GetFreeRoom();

		if ( !room )
		{
			this->DuelResult(0x10, pPlayer->GetEntry(), pPlayer->GetName());
			return;
		}

		this->GetInterfaceState()->Set(InterfaceData::Duel, pPlayer, 0, sGameServer->dueling_max_wait_interval.get());
		this->SetDuelRoom(room->GetRoomID());
		this->SetDuelClassic(false);

		pPlayer->GetInterfaceState()->Set(InterfaceData::Duel, this, 0, sGameServer->dueling_max_wait_interval.get());
		pPlayer->SetDuelRoom(room->GetRoomID());
		pPlayer->SetDuelClassic(false);

		room->SetDuelers(this, pPlayer);
		room->SetState_Requested();

		this->SendNotice(NOTICE_NORMAL_BLUE, "You have requested duel to %s", pPlayer->GetName());
	
		DUEL_REQUEST_SEND pMsg(this->GetEntry(), this->GetName());
		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::DuelAnswer(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	POINTER_PCT_LOG(DUEL_ANSWER, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceSharedCheck(this, pPlayer, InterfaceData::Duel, 0) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel answer has failed.");
		return;
	}

	if ( this->IsDuelClassic() != pPlayer->IsDuelClassic() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel Error.");
		return;
	}

	if ( !this->IsDuelClassic() && this->GetDuelRoom() != pPlayer->GetDuelRoom() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel room error.");
		return;
	}

	this->GetInterfaceState()->Reset();
	pPlayer->GetInterfaceState()->Reset();

	if ( this->IsDuelClassic() )
	{
		if ( lpMsg->result == 1 )
		{
			this->DuelResult(0, pPlayer->GetEntry(), pPlayer->GetName());
			pPlayer->DuelResult(0, this->GetEntry(), this->GetName());

			this->SetDueling(true);
			this->SetDuelScore(0);

			pPlayer->SetDueling(true);
			pPlayer->SetDuelScore(0);

			this->GetTimer(PLAYER_TIMER_DUEL_TIME)->Start();
			pPlayer->GetTimer(PLAYER_TIMER_DUEL_TIME)->Start();
		}
		else
		{
			pPlayer->DuelResult(0xF,this->GetEntry(),this->GetName());

			this->SetDuelClassic(false);
			this->SetDuelTarget(nullptr);

			pPlayer->SetDuelClassic(false);
			pPlayer->SetDuelTarget(nullptr);
		}
	}
	else
	{
		DuelRoom * room = sDuelMgr->GetRoom(this->GetDuelRoom());

		if ( !room )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel room error.");
			return;
		}

		if ( lpMsg->result == 1 )
		{
			this->SetDueling(true);
			this->SetDuelScore(0);

			pPlayer->SetDueling(true);
			pPlayer->SetDuelScore(0);

			this->DuelResult(0, pPlayer->GetEntry(), pPlayer->GetName());
			pPlayer->DuelResult(0, this->GetEntry(), this->GetName());
			sDuelMgr->GetRoom(this->GetDuelRoom())->SetState_Started();
		}
		else
		{
			pPlayer->DuelResult(0xF,this->GetEntry(),this->GetName());
		}
	}
}

void Player::DuelResult(uint8 result, uint16 target, const char * name)
{
	DUEL_RESULT pMsg(result, target, name);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::DuelEndRequest(uint8 * Packet)
{
	if ( !this->IsPlaying() )
	{
		return;
	}

	if ( !this->IsDueling() )
	{
		return;
	}

	if ( !this->IsDuelClassic() && this->GetWorldId() != WORLD_VULCANUS_ROOM )
	{
		return;
	}

	if ( !this->IsDuelClassic() && !DUEL_ROOM_RANGE(this->GetDuelRoom()) )
	{
		return;
	}
	
	if ( this->IsDuelClassic() )
	{
		sDuelMgr->ClassicCancel(this, this->GetDuelTarget());

		PLAYER_POINTER(this)->UpdateStatistic(STATISTIC_DUEL_LOOSED, 1);
	}
	else
	{
		DuelRoom* pRoom = sDuelMgr->GetRoom(this->GetDuelRoom());

		if ( !pRoom )
		{
			return;
		}

		if ( pRoom->GetStatus() != DUEL_ROOM_STARTED )
		{
			return;
		}

		if ( pRoom->GetDueler(0)->GetPlayer() == this )
		{
			pRoom->GetDueler(0)->SetCancelled(true);
		}
		else if ( pRoom->GetDueler(1)->GetPlayer() == this )
		{
			pRoom->GetDueler(1)->SetCancelled(true);
		}
	}
}

void Player::DuelSpectatorRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::DuelGate )
	{
		return;
	}

	if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		return;
	}

	POINTER_PCT_LOG(DUEL_SPECTATOR, lpMsg, Packet, 0);

	if ( !DUEL_ROOM_RANGE(lpMsg->room) )
	{
		return;
	}

	if ( sDuelMgr->GetRoom(lpMsg->room)->GetStatus() != DUEL_ROOM_STARTED )
	{
		return;
	}

	sDuelMgr->GetRoom(lpMsg->room)->AddSpectator(this);
}

void Player::DuelSpectatorRemove(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT_LOG(DUEL_SPECTATOR_REMOVE, lpMsg, Packet, 0);

	if ( !DUEL_ROOM_RANGE(lpMsg->room) )
	{
		return;
	}

	if ( this->GetWorldId() != WORLD_VULCANUS_ROOM || this->GetDuelRoom() != lpMsg->room )
	{
		return;
	}

	sDuelMgr->GetRoom(lpMsg->room)->DelSpectator(this);
}

bool Player::IsOnDuel() const
{
	if ( !this->IsDueling() )
	{
		return false;
	}

	if ( !this->IsDuelClassic() )
	{
		if ( this->GetWorldId() != WORLD_VULCANUS_ROOM )
		{
			return false;
		}

		if ( !DUEL_ROOM_RANGE(this->GetDuelRoom()) )
		{
			return false;
		}

		DuelRoom* pRoom = sDuelMgr->GetRoom(this->GetDuelRoom());

		if ( !pRoom )
		{
			return false;
		}

		return pRoom->GetDueler(0)->GetPlayer() == this || pRoom->GetDueler(1)->GetPlayer() == this;
	}
	else
	{
		return true;
	}
}

bool Player::IsOnDuel(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !this->IsOnDuel() || !pPlayer->IsOnDuel() )
	{
		return false;
	}

	if ( this->IsDuelClassic() != pPlayer->IsDuelClassic() )
	{
		return false;
	}

	if ( this->IsDuelClassic() && (this->GetDuelTarget() != pPlayer || pPlayer->GetDuelTarget() != this) )
	{
		return false;
	}

	if ( !this->IsDuelClassic() )
	{
		if ( this->GetWorldId() != WORLD_VULCANUS_ROOM )
		{
			return false;
		}

		if ( !DUEL_ROOM_RANGE(this->GetDuelRoom()) )
		{
			return false;
		}

		if ( pPlayer->GetDuelRoom() != this->GetDuelRoom() )
		{
			return false;
		}

		if ( sDuelMgr->GetRoom(this->GetDuelRoom())->GetStatus() != DUEL_ROOM_STARTED )
		{
			return false;
		}
	}

	return true;
}

void Player::UpdateDuelScore()
{
	if ( this->IsDuelClassic() )
	{
		sDuelMgr->ClassicScore(this, this->GetDuelTarget());
	}
	else
	{
		sDuelMgr->GetRoom(this->GetDuelRoom())->UpdateScore(this);
	}
}

bool Player::IsSpectator() const
{
	if ( this->GetWorldId() != WORLD_VULCANUS_ROOM )
	{
		return false;
	}

	if ( !DUEL_ROOM_RANGE(this->GetDuelRoom()) )
	{
		return false;
	}
	
	for ( uint8 i = 0; i < MAX_DUEL_SPECTATORS; ++i )
	{
		if ( sDuelMgr->GetRoom(this->GetDuelRoom())->GetSpectator(i) == this )
		{
			return true;
		}
	}

	return false;
}