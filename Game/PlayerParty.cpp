/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerParty.cpp"
*
*/

void Player::PartyRequest(uint8 * Packet)
{
	if ( !this->PacketTimeCheck() || !this->IsPlaying() || this->IsServerQuit() )
		return;
	
	if ( !sGameServer->IsPartyEnabled() )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( this->IsAdministrator() && !this->IsAdministratorFlag(ADMIN_FLAG_PARTY) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	POINTER_PCT_LOG(PARTY_REQUEST, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !this->CanInteract(pPlayer, true) )
	{
		this->PartyRequestResult(PARTY_REQUEST_TARGET_OFF);
		return;
	}

	if ( pPlayer->IsServerQuit() )
	{
		this->PartyRequestResult(PARTY_REQUEST_TARGET_OFF);
		return;
	}

	if ( pPlayer->IsAdministrator() && !pPlayer->IsAdministratorFlag(ADMIN_FLAG_PARTY) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( !pPlayer->IsFlag(CHARACTER_FLAG_REQUEST) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( !sGameServer->party_in_battlezone && this->IsWorldFlag(WORLD_FLAG_BATTLE_ZONE) )
	{
		if ( this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily() )
		{
			this->PartyRequestResult(PARTY_REQUEST_BATTLE_ZONE);
			return;
		}
	}

	if ( !sGameServer->party_allow_dif_gens && this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily() )
	{
		this->PartyRequestResult(PARTY_REQUEST_DIF_GENS);
		return;
	}

	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_PARTY) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to make party on this map.");
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( TS_MAP_RANGE(this->GetWorldId()) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}
	///- CHANGELOG: 03/10
	if ( AW_MAP_RANGE(this->GetWorldId()) && (sArkaWar->GetState() == ARKA_WAR_STATE_STANDBY || sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING) && this->GetGuildID() != pPlayer->GetGuildID() )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::None || pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( Party* pParty = this->GetParty() )
	{
		if ( pParty->GetMember(0)->GetGUID() != this->GetGUID() )
		{
			this->PartyRequestResult(PARTY_REQUEST_FAILED);
			return;
		}

		if ( pParty->GetMemberCount() >= MAX_PARTY_MEMBERS )
		{
			this->PartyRequestResult(PARTY_REQUEST_FULL);
			return;
		}
	}

	if ( pPlayer->GetPartyID() != PARTY_NULL )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s is already in a party.", pPlayer->GetName());
		this->PartyRequestResult(PARTY_REQUEST_TARGET_IN_PARTY);
		return;
	}

	int32 sMaxMinLevel[2];

	sMaxMinLevel[0] = std::min(this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel(), pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	sMaxMinLevel[1] = std::max(this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel(), pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());

	if ( (sMaxMinLevel[1] - sMaxMinLevel[0]) > sGameServer->party_max_level_dif.get() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Restricted level for the party is %d", sGameServer->party_max_level_dif.get());
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	this->GetInterfaceState()->Set(InterfaceData::Party, pPlayer, 0, sGameServer->party_max_wait_interval.get());
	pPlayer->GetInterfaceState()->Set(InterfaceData::Party, this, 0, sGameServer->party_max_wait_interval.get());

	PARTY_REQUEST pMsg(this->GetEntry());
	pPlayer->SEND_PCT(pMsg);
}

void Player::PartyRequestResult(uint8 result)
{
	PARTY_REQUEST_RESULT pMsg(result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::PartyRequestAnswer(uint8 * Packet)
{
	if ( !sGameServer->IsPartyEnabled() )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( !this->IsPlaying() || this->IsServerQuit() )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	POINTER_PCT_LOG(PARTY_ANSWER, lpMsg, Packet, 0);

	Player * pPlayer = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if ( !pPlayer || !pPlayer->IsPlaying() || pPlayer->IsServerQuit() )
	{
		this->PartyRequestResult(PARTY_REQUEST_TARGET_OFF);
		return;
	}

	if ( !Player::InterfaceSharedCheck(this, pPlayer, InterfaceData::Party, 0) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		return;
	}

	if ( !this->SameDimension(pPlayer) )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
		goto CLEAR_JUMP;
	}

	if ( !sGameServer->party_in_battlezone && this->IsWorldFlag(WORLD_FLAG_BATTLE_ZONE) )
	{
		if ( this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily() )
		{
			this->PartyRequestResult(PARTY_REQUEST_BATTLE_ZONE);
			goto CLEAR_JUMP;
		}
	}

	if (!sGameServer->party_allow_dif_gens && this->GetGen()->GetFamily() != pPlayer->GetGen()->GetFamily())
	{
		this->PartyRequestResult(PARTY_REQUEST_DIF_GENS);
		goto CLEAR_JUMP;
	}

	if ( TS_MAP_RANGE(this->GetWorldId()) )
	{
		this->PartyRequestResult(PARTY_REQUEST_CANCELED);
		goto CLEAR_JUMP;
	}

	if ( lpMsg->result == 0 )
	{
		this->PartyRequestResult(PARTY_REQUEST_CANCELED);
		goto CLEAR_JUMP;
	}

	Party* pParty = pPlayer->GetParty();

	if ( pParty )
	{
		if ( !pParty->AddMember(this, true) )
		{
			pPlayer->PartyRequestResult(PARTY_REQUEST_FULL);
			this->PartyRequestResult(PARTY_REQUEST_FULL);
			goto CLEAR_JUMP;
		}
	}
	else
	{
		pParty = sPartyMgr->Create(pPlayer, this);
	}

	if ( pParty )
	{
		sPartyMatching->CancelRegister(this, 1, 1);
		sPartyMatching->CancelRegister(pPlayer, 1, 1);

		pParty->ListAllMembers();
	}

CLEAR_JUMP:
	this->GetInterfaceState()->Reset();
	pPlayer->GetInterfaceState()->Reset();
}

void Player::PartyDelete()
{
	PARTY_USER_DELETE pMsg;
	this->SEND_PCT(pMsg);
}

void Player::HandleRemovePartyMember(uint8 * Packet)
{
	if (!this->IsPlaying() || this->IsServerQuit())
	{
		return;
	}

	Party* pParty = this->GetParty();

	if (!pParty)
	{
		return;
	}

	POINTER_PCT_LOG(PARTY_USER_DELETE_REQUEST, lpMsg, Packet, 0);

	if (lpMsg->number >= MAX_PARTY_MEMBERS)
	{
		return;
	}

	auto pMember = pParty->GetMember(lpMsg->number);

	if (pMember->GetStatus() == PARTY_USER_FLAG_NONE)
	{
		return;
	}

	if (pMember->GetPlayer() != this && pParty->GetLeader() != this)
	{
		return;
	}

	pParty->DeleteMember(lpMsg->number);
}

void Player::PartyRemove()
{
	if (Party* pParty = this->GetParty())
	{
		pParty->DeleteMember(pParty->GetMemberPosition(this));
	}
}

void Player::PartyItemGetNotify(WorldItem const* item)
{
	Party* pParty = this->GetParty();

	if ( !pParty )
		return;

	uint32 item_info = item->item.GetItem();

	if ( item->item.GetSkill() )
		item_info |= 0x20000;

	if ( item->item.GetLuck() )
		item_info |= 0x40000;

	if ( item->item.GetOption() )
		item_info |= 0x80000;

	if ( item->item.GetExe() )
		item_info |= 0x100000;

	if ( item->item.GetAncient() )
		item_info |= 0x200000;

	if ( item->item.Get380() )
		item_info |= 0x400000;

	if ( item->item.IsSocket() )
		item_info |= 0x800000;

	uint8 level = item->item.GetLevel();

	PARTY_ITEM_INFO pMsg(this->GetEntry(), item_info, level);

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			 continue;

		Player* pPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::PartySendDamage(Unit* pVictim, int32 damage, int32 shield_damage, uint8 damage_flag)
{
	if ( Party* pParty = this->GetParty() )
	{
		pParty->SendKillDamage(this, pVictim, damage, shield_damage, damage_flag);
	}
}

void Player::PartyLeaderChange(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	POINTER_PCT(PARTY_LEADER_CHANGE, lpMsg, Packet, 0);

	Party* pParty = this->GetParty();

	if ( !pParty )
	{
		return;
	}

	auto pLeaderMember = pParty->GetMember(0);

	if (pLeaderMember->GetGUID() != this->GetGUID())
	{
		return;
	}

	PARTY_LEADER_CHANGE_RESULT pMsg(0);

	PARTY_LOOP(i)
	{
		auto pMember = pParty->GetMember(i);

		if (!memcmp(pMember->GetName(), lpMsg->name, MAX_CHARACTER_LENGTH))
		{
			pLeaderMember->SetAccount(pMember->GetAccount());
			pLeaderMember->SetGUID(pMember->GetGUID());
			pLeaderMember->SetName(pMember->GetName());
			pLeaderMember->SetPlayer(pMember->GetPlayer());
			pLeaderMember->SetLevel(pMember->GetLevel());
			pLeaderMember->SetClass(pMember->GetClass());
			pLeaderMember->SetGenFamily(pMember->GetGenFamily());
			pLeaderMember->SetStatus(pMember->GetStatus());
			pLeaderMember->GetLeftTime()->SetTimer(pMember->GetLeftTime()->GetTimer());
			pLeaderMember->GetLeftTime()->SetDelay(pMember->GetLeftTime()->GetDelay());
			pLeaderMember->SetAssisted(false);

			pMember->SetAccount(this->GetAccountData()->GetGUID());
			pMember->SetGUID(this->GetGUID());
			pMember->SetName(this->GetName());
			pMember->SetPlayer(this);
			pMember->SetLevel(this->GetTotalLevel());
			pMember->SetClass(this->GetDBClass());
			pMember->SetGenFamily(this->GetGen()->GetFamily());
			pMember->SetStatus(PARTY_USER_FLAG_PLAYING);
			pMember->SetAssisted(false);

			pMsg.result = 1;
			this->SendPacket(&pMsg);

			pParty->ListAllMembers();
			pParty->UpdateDB();
			break;
		}
	}
}

Party* Player::GetParty() const
{
	return sPartyMgr->Get(this->GetPartyID());
}

void Player::PartyMove(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this))
	{
		return;
	}

	if (!Player::IsPlayerTransactionFree(this))
	{
		return;
	}

	if (!sGameServer->IsPartyMoveEnabled())
	{
		this->SendMessageBox(1, "Party", "Party move system is disabled.");
		return;
	}

	if (this->IsTeleporting())
	{
		return;
	}

	if (this->GetRegenStatus() != REGEN_NONE)
	{
		return;
	}

	POINTER_PCT_LOG(PARTY_MOVE_REQUEST, lpMsg, Packet, 0);

	if (lpMsg->slot >= MAX_PARTY_MEMBERS)
	{
		this->SendMessageBox(1, "Party", "Can't move. Wrong party member.");
		return;
	}

	Party* pParty = this->GetParty();

	if (!pParty)
	{
		this->SendMessageBox(1, "Party", "Can't move. You are not in Party.");
		return;
	}

	PartyMemberData const* pData = pParty->GetMember(lpMsg->slot);

	if (!pData || pData->GetPlayer() == this)
	{
		return;
	}

	if (pData->GetStatus() != PARTY_USER_FLAG_PLAYING)
	{
		this->SendMessageBox(1, "Party", "Can't move. Wrong party member.");
		return;
	}

	auto pPlayer = pData->GetPlayer();
	if (!pPlayer)
	{
		this->SendMessageBox(1, "Party", "Can't move. Wrong party member.");
		return;
	}

	if (sDungeon->IsDungeon(pPlayer->GetWorldId()))
	{
		return;
	}

	World* pTargetWorld = sWorldMgr->GetWorld(pPlayer->GetWorldId());

	if (!pTargetWorld)
	{
		this->SendMessageBox(1, "Party", "Can't move. Wrong dest world.");
		return;
	}

	if (!pTargetWorld->flagHas(WORLD_FLAG_ALLOW_PARTY_MOVE))
	{
		this->SendMessageBox(1, "Party", "Can't move. Dest world does not allow party move.");
		return;
	}

	int16 move_level = sTeleport->GetMoveLevel(pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY());

	if (this->GetTotalLevel() < move_level || move_level == -1)
	{
		return;
	}

	if (!this->CanEnterWorld(pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetX(), pPlayer->GetY()))
	{
		return;
	}

	if (pPlayer->GetWorldId() != this->GetWorldId())
	{
		if (!this->MoneyHave(sGameServer->GetPartyMoveCost()))
		{
			this->SendMessageBox(1, "Party", "Can't move. You require %u zen.", sGameServer->GetPartyMoveCost());
			return;
		}

		this->MoneyReduce(sGameServer->GetPartyMoveCost());
	}

	this->TeleportToLocation(pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), this->GetDirection(), this->GetInstance());
}

void Player::PartyHelper(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_LOG(PARTY_HELPER_REQUEST, lpMsg, Packet, 0);

	Party* pParty = this->GetParty();

	if (!pParty)
	{
		return;
	}

	Player* pLeader = pParty->GetLeader();
	PartyMemberData * pMemberData = pParty->FindMember(this->GetEntry());

	if (!pMemberData)
	{
		return;
	}

	if (lpMsg->data == 1)
	{
		if (!pLeader)
		{
			return;
		}

		if (pLeader == this)
		{
			return;
		}

		if (this->GetCurrentTargetPlayer() != pLeader->GetEntry())
		{
			return;
		}

		if (!pMemberData->IsAssisted())
		{
			pMemberData->SetAssisted(true);
			pParty->ListAllMembers();
		}
	}
	else
	{
		if (pMemberData->IsAssisted())
		{
			pMemberData->SetAssisted(false);
			pParty->ListAllMembers();
		}
	}
}

void Player::PartySummon(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this))
	{
		return;
	}

	POINTER_PCT_LOG(PARTY_SUMMON_REQUEST, lpMsg, Packet, 0);

	Party* pParty = this->GetParty();

	if (!pParty)
	{
		return;
	}

	Player* pLeader = pParty->GetLeader();

	if (!pLeader)
	{
		return;
	}

	if (pLeader != this)
	{
		return;
	}

	if (pLeader->IsInSafeZone())
	{
		return;
	}

	if (sDungeon->IsDungeon(pLeader->GetWorldId()))
	{
		return;
	}

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; i++)
	{
		if (pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}

		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pMember->IsDelayedTeleport())
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Party summon is currently in progress.");
			return;
		}
	}

	if (!this->IsWorldFlag(WORLD_FLAG_ALLOW_PARTY_MOVE))
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Party Summon is not allowed on this location.");
		return;
	}

	int32 count = 0;
	PARTY_LOOP(i)
	{
		if (pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
		{
			continue;
		}
		
		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pMember == this)
		{
			continue;
		}

		if (sCrywolf->IsInAltar(pMember))
		{
			continue;
		}

		int16 move_level = sTeleport->GetMoveLevel(this->GetWorldId(), this->GetX(), this->GetY());
		bool busy = pMember->IsBusy();
		bool equipment = pMember->CanEnterWorld(this->GetWorldId(), this->GetX(), this->GetY(), this->GetX(), this->GetY());

		if (pMember->GetTotalLevel() >= move_level && move_level != -1 && !busy && equipment)
		{
			int16 x = this->GetX();
			int16 y = this->GetY();
			int32 loop = 50;

			while (loop-- > 0)
			{
				int16 tmp_x = this->GetX() + RANDOM(9) - 4;
				int16 tmp_y = this->GetY() + RANDOM(9) - 4;

				if (this->TeleportAreaCheck(tmp_x, tmp_y))
				{
					x = tmp_x;
					y = tmp_y;
					break;
				}
			}

			pMember->SetDelayedTeleport(true);
			pMember->SetDelayedTeleportTime(7);
			pMember->GetDelayedLocation()->SetX(x);
			pMember->GetDelayedLocation()->SetY(y);
			pMember->GetDelayedLocation()->SetInstance(this->GetInstance());
			pMember->GetDelayedLocation()->SetWorldId(this->GetWorldId());
			pMember->GetDelayedLocation()->SetDirection(pMember->GetDirection());
			pMember->SetDelayedTeleportGate(this->GetCurrentGate());
			pMember->SetDelayedTeleportHelper(pParty->GetMember(i)->IsAssisted() || pMember->GetHelper()->IsStarted());

			pMember->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Summoning in process. It may be cancelled if you move.");
			++count;
		}
		else
		{
			pMember->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot warp.");
		}
	}

	if (count > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%d players will be summoned.", count);
	}
}