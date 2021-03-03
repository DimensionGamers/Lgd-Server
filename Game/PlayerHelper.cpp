/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerHelper.cpp"
*
*/

void Player::HelperSendSettings()
{
	HELPER_SETTINGS_SEND pMsg;
	pMsg.item_pick_flag = this->GetHelper()->GetItemPickFlags();
	pMsg.range = this->GetHelper()->GetHuntingRange() | (this->GetHelper()->GetPickRange() << 4);
	pMsg.movement_distance = this->GetHelper()->GetMovementTime();
	pMsg.primary_skill = this->GetHelper()->GetAttackSkill(0);
	pMsg.secondary_skill_1 = this->GetHelper()->GetAttackSkill(1);
	pMsg.secondary_delay_1 = this->GetHelper()->GetDelay(0);
	pMsg.secondary_skill_2 = this->GetHelper()->GetAttackSkill(2);
	pMsg.secondary_delay_2 = this->GetHelper()->GetDelay(1);
	pMsg.time_space_casting = this->GetHelper()->GetTimeSpaceCasting();
	pMsg.buff_skill[0] = this->GetHelper()->GetBuffSkill(0);
	pMsg.buff_skill[1] = this->GetHelper()->GetBuffSkill(1);
	pMsg.buff_skill[2] = this->GetHelper()->GetBuffSkill(2);
	pMsg.heal_percent = this->GetHelper()->GetAutoPotPercent() / 10;
	pMsg.heal_percent |= (this->GetHelper()->GetAutoHealPercent() / 10) << 4;
	pMsg.heal_percent |= (this->GetHelper()->GetDrainLifePercent() / 10) << 8;
	pMsg.heal_percent |= (this->GetHelper()->GetPartyHealPercent() / 10) << 12;
	pMsg.opt_flags_1 = this->GetHelper()->GetOptionFlags(0);
	pMsg.opt_flags_2 = this->GetHelper()->GetOptionFlags(1);

	for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i )
	{
		memcpy(pMsg.item_name[i], this->GetHelper()->item_name[i], HELPER_ITEM_LENGTH);
	}

	pMsg.buff_item[0] = this->GetHelper()->GetItemBuff(0);
	pMsg.buff_item[1] = this->GetHelper()->GetItemBuff(1);
	pMsg.buff_item[2] = this->GetHelper()->GetItemBuff(2);

	this->sendPacket((uint8*)&pMsg,pMsg.h.get_size());
}

void Player::HelperSaveSettings(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	//this->GetHelper()->Reset();

	POINTER_PCT_LOG(HELPER_SETTINGS_SAVE, lpMsg, Packet, 0);

	this->GetHelper()->SetItemPickFlags(lpMsg->item_pick_flag);
	this->GetHelper()->SetHuntingRange(lpMsg->range & 0x0F);
	this->GetHelper()->SetPickRange(lpMsg->range >> 4 & 0x0F);
	this->GetHelper()->SetMovementTime(lpMsg->movement_distance);
	this->GetHelper()->SetAttackSkill(0, lpMsg->primary_skill);
	this->GetHelper()->SetAttackSkill(1, lpMsg->secondary_skill_1);
	this->GetHelper()->SetAttackSkill(2, lpMsg->secondary_skill_2);
	this->GetHelper()->SetDelay(0, lpMsg->secondary_delay_1);
	this->GetHelper()->SetDelay(1, lpMsg->secondary_delay_2);
	this->GetHelper()->SetTimeSpaceCasting(lpMsg->time_space_casting);
	this->GetHelper()->SetBuffSkill(0, lpMsg->buff_skill[0]);
	this->GetHelper()->SetBuffSkill(1, lpMsg->buff_skill[1]);
	this->GetHelper()->SetBuffSkill(2, lpMsg->buff_skill[2]);
	this->GetHelper()->SetAutoPotPercent(10 * (lpMsg->heal_percent & 0xF));
	this->GetHelper()->SetAutoHealPercent(10 * ((lpMsg->heal_percent >> 4) & 0xF));
	this->GetHelper()->SetDrainLifePercent(10 * ((lpMsg->heal_percent >> 8) & 0xF));
	this->GetHelper()->SetPartyHealPercent(10 * ((lpMsg->heal_percent >> 12) & 0xF));

	this->GetHelper()->SetOptionFlags(0, lpMsg->opt_flags_1);
	this->GetHelper()->SetOptionFlags(1, lpMsg->opt_flags_2);

	for (uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i)
	{
		memcpy(this->GetHelper()->item_name[i], lpMsg->item_name[i], HELPER_ITEM_LENGTH);
	}

	this->GetHelper()->SetItemBuff(0, lpMsg->buff_item[0]);
	this->GetHelper()->SetItemBuff(1, lpMsg->buff_item[1]);
	this->GetHelper()->SetItemBuff(2, lpMsg->buff_item[2]);

	if (this->HelperSettingsUpdate())
	{
		this->HelperSendSettings();
	}

	this->GetHelper()->SetLoaded(true);
	//this->GetHelper()->SetStarted(false);
}

void Player::HelperOpenRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->GetHelper()->IsLoaded() )
	{
		return;
	}

	POINTER_PCT_LOG(HELPER_OPEN, lpMsg, Packet, 0);

	if ( lpMsg->result == 0x00 ) // Open
	{
		if ( this->GetHelper()->IsStarted() )
		{
			return;
		}

		this->HelperStart();
	}
	else
	{
		this->GetHelper()->SetStarted(false);
		this->HelperOpenResult(0x01);
	}

	// Result 0 -> %d zen spent
	// Result 1 -> Ni idea ( probablemente cortar el Helper )
	// Result 2 -> %d zen is not suficient to run mu helper
}

void Player::HelperOpenResult(uint8 result, uint32 zen, uint16 spent_time)
{
	HELPER_OPEN_RESULT pMsg(result,zen,spent_time);
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::HelperStart()
{
	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_HELPER) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Helper can't be used on this map.");
		return false;
	}

	if ( this->IsInSafeZone() )
	{
		return false;
	}

	if ( character_helper const* helper_stage = sCharacterBase->GetHelperStage(this->GetClass(), 1) )
	{
		uint32 cost = helper_stage->GetCost() * this->GetTotalLevel();

		if ( this->MoneyHave(cost) )
		{
			this->MoneyReduce(cost);
			this->GetHelper()->SetStageTick(GetTickCount());
			this->GetHelper()->SetCostTick(GetTickCount());
			this->GetHelper()->SetStage(1);
			this->GetHelper()->SetStarted(true);
			this->GetHelper()->SetX(this->GetX());
			this->GetHelper()->SetY(this->GetY());
			this->GetTimer(PLAYER_TIMER_HELPER_LABYRINTH)->Start();
			this->HelperOpenResult(0x00, cost, 0);

			sHuntingRecord->Start(this);

			sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_ACHIEVE_HELPER_MISSION, 0, true, false);

			return true;
		}
		else
		{
			sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_ACHIEVE_HELPER_MISSION, 0, true);

			this->HelperOpenResult(0x02);
			return false;
		}
	}

	return false;
}

void Player::HelperStop(uint8 reason)
{
	if ( !this->GetHelper()->IsStarted() )
	{
		return;
	}

	sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_ACHIEVE_HELPER_MISSION, 0, true);

	this->GetHelper()->SetStarted(false);
	this->HelperOpenResult(reason);
}

void Player::HelperUpdate()
{
	if (this->GetRegenStatus() == REGEN_NONE && !this->IsTeleporting() && !this->IsDelayedTeleport() && this->IsDelayedTeleportHelper())
	{
		if (!this->GetHelper()->IsStarted())
		{
			this->HelperStart();
		}

		this->SetDelayedTeleportHelper(false);
	}
	

	if ( !this->GetHelper()->IsStarted() )
	{
		return;
	}

	character_helper const* helper_data = sCharacterBase->GetHelperStage(this->GetClass(), this->GetHelper()->GetStage());

	if ( !helper_data )
	{
		this->HelperStop(0x01);
		return;
	}

	uint32 cur_time = GetTickCount();

	if ( this->GetTimer(PLAYER_TIMER_HELPER_LABYRINTH)->Elapsed(IN_MILLISECONDS) )
	{
		sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_ACHIEVE_HELPER_MISSION, 1);
	}

	if ( (cur_time - this->GetHelper()->GetCostTick()) > helper_data->GetCostInterval() )
	{
		this->GetHelper()->SetCostTick(cur_time);

		uint32 cost = helper_data->GetCost() * this->GetTotalLevel();

		if ( this->MoneyHave(cost) )
		{
			this->HelperOpenResult(0x00, cost, (cur_time - this->GetHelper()->GetStageTick()) / (MINUTE * IN_MILLISECONDS));
			this->MoneyReduce(cost);
		}
		else
		{
			this->HelperStop(0x02);
			return;
		}
	}

	if ( (cur_time - this->GetHelper()->GetStageTick()) > helper_data->GetDuration() )
	{
		if ( character_helper const* helper_stage = sCharacterBase->GetHelperStage(this->GetClass(), helper_data->GetNextStage()) )
		{
			this->GetHelper()->SetStageTick(GetTickCount());
			this->GetHelper()->SetCostTick(GetTickCount());
			this->GetHelper()->SetStage(helper_stage->GetStage());
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Official MU Helper Stage changed.");
		}
		else
		{
			this->HelperStop(0x01);
			return;
		}
	}

	if (!this->GetHelper()->IsOffline() && this->IsClientMinimized())
	{
		this->HelperAutoloot();
	}
}

bool Player::HelperSettingsUpdate()
{
	bool update = false;

	if ( !sGameServer->IsHelperLootEnabled() )
	{
		if ( this->GetHelper()->GetItemPickFlags() )
		{
			this->GetHelper()->SetItemPickFlags(0);
			update = true;
		}

		for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i )
		{
			if ( strlen(this->GetHelper()->item_name[i]) )
			{
				update = true;
			}

			memset(this->GetHelper()->item_name[i], 0, HELPER_ITEM_LENGTH);
		}

		if ( this->GetHelper()->IsOptPickSelectedItems() )
		{
			this->GetHelper()->ClearFlag(0, OPT_PICK_SELECTED_ITEMS);
			update = true;
		}

		if ( this->GetHelper()->IsOptPickAllNearItems() )
		{
			this->GetHelper()->ClearFlag(0, OPT_PICK_ALL_NEAR_ITEMS);
			update = true;
		}

		this->GetHelper()->SetPickRange(1);
	}

	if ( !sGameServer->IsHelperRepairEnabled() )
	{
		if ( this->GetHelper()->IsOptRepairItems() )
		{
			this->GetHelper()->ClearFlag(0, OPT_REPAIR_ITEMS);
			update = true;
		}
	}

	if ( !sGameServer->IsHelperPotionEnabled() )
	{
		if ( this->GetHelper()->IsAutoPot() )
		{
			this->GetHelper()->ClearFlag(0, OPT_AUTOPOT);
			update = true;
		}
	}

	return update;
}

bool Player::HelperCanLoot(WorldItem const* pWorldItem)
{
	if ( !pWorldItem )
		return false;

	///- Si el helper está configurado para agarrar todos los items, entonces no lo proceso
	if (this->GetHelper()->IsOptPickAllNearItems())
	{
		if (pWorldItem->item.GetItem() != ITEMGET(14, 15) && (sGameServer->IsOfflineAttackZenLoot() && this->GetHelper()->IsOffline()))
		{
			return false;
		}

		return true;
	}

	///- Si el helper está configurado para agarrar solo algunos items, entonces lo proceso
	if ( this->GetHelper()->IsOptPickSelectedItems() )
	{
		if ( pWorldItem->item.GetItem() == ITEMGET(14, 15) )
		{
			if ( this->GetHelper()->IsItemPickFlag(ITEM_PICK_ZEN) || sGameServer->IsOfflineAttackZenLoot() )
				return true;
		}

		if ( sGameServer->IsOfflineAttackZenLoot() && this->GetHelper()->IsOffline())
		{
			return false;
		}

		if ( pWorldItem->item.IsJewel() )
		{
			if ( this->GetHelper()->IsItemPickFlag(ITEM_PICK_JEWEL) )
				return true;
		}

		if (pWorldItem->item.GetExe())
		{
			if ( this->GetHelper()->IsItemPickFlag(ITEM_PICK_EXE) )
				return true;
		}

		if ( pWorldItem->item.IsAncient() )
		{
			if ( this->GetHelper()->IsItemPickFlag(ITEM_PICK_ANCIENT) )
				return true;
		}

		if ( sItemMgr->GetItemInventoryType(pWorldItem->item.GetItem()) == ITEM_INVENTORY_TYPE_EVENT )
		{
			if ( this->GetHelper()->IsPickEventItem() )
			{
				return true;
			}
		}

		StringLower(name_lower, pWorldItem->item.GetName());

		if ( sGameServer->IsOfflineAttackDetailAutoloot() )
		{
			if ( pWorldItem->item.GetLevel() > 0 )
			{
				item_template const* item_info = sItemMgr->GetItem(pWorldItem->item.GetItem());

				if ( item_info && !item_info->IsLevelData(pWorldItem->item.GetLevel()) )
				{
					name_lower.append("+");
					name_lower.append(std::to_string(pWorldItem->item.GetLevel()));
				}
			}

			if ( pWorldItem->item.GetSkill() )
			{
				name_lower.append("+skill");
			}

			if ( pWorldItem->item.GetOption() )
			{
				name_lower.append("+option");
			}

			if ( pWorldItem->item.GetLuck() )
			{
				name_lower.append("+luck");
			}

			uint8 socket_count = pWorldItem->item.GetSocketCount();

			if ( pWorldItem->item.IsPentagramItem() || (socket_count > 0 && pWorldItem->item.GetMaxSocket() > 0) )
			{
				name_lower.append("+socket");
				name_lower.append(std::to_string(socket_count));
			}
		}
		
		if ( this->GetHelper()->IsItemPickFlag(ITEM_PICK_SELECTED) )
		{
			for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i )
			{
				if ( strlen(this->GetHelper()->item_name[i]) <= 0 )
					continue;

				StringLower(item_lower, this->GetHelper()->item_name[i]);

				std::string text_part;
				std::istringstream stream(item_lower.c_str());
				bool founded = true;
				int32 count = 50;

				while (stream)
				{
					stream >> text_part;

					if ( FindString<std::string>(name_lower, text_part) == size_t(-1) )
					{
						founded = false;
					}

					text_part = "";
					--count;

					if ( count <= 0 )
					{
						break;
					}
				}

				if ( !founded )
				{
					continue;
				}

				/*if ( FindString<std::string>(name_lower, item_lower) == size_t(-1) )
				{
					continue;
				}*/

				return true;
			}
		}
	}

	return false;
}

void Player::HelperAutoloot()
{
	if ( !this->GetHelper()->IsStarted() )
	{
		return;
	}

	if ( !sGameServer->IsHelperAutoLootEnabled() )
	{
		return;
	}

	if ( !sGameServer->IsHelperLootEnabled() )
	{
		return;
	}

	///- Si no tiene ninguna opción de loot, entonces no lo proceso
	if ( !this->GetHelper()->IsOptPickAllNearItems() && !this->GetHelper()->IsOptPickSelectedItems() )
	{
		return;
	}

	ViewportDataList const& viewport = this->GetAllViewport();
	ViewportData const* data = nullptr;
	Object* pObject = nullptr;
	WorldItem const* pWorldItem = nullptr;
	
	for ( ViewportDataList::const_iterator it = viewport.begin(); it != viewport.end(); ++it )
	{
		data = it->second;

		if ( !data ) ///- Si la data no existe por algún motivo, no lo proceso
			continue;

		if ( data->GetStatus() != Viewport::STATUS_READY ) ///- Solamente proceso objetos que estén listos
			continue;

		pObject = data->GetObjectData();

		if ( !pObject ) ///- Si el objeto no existe, no lo proceso
			continue;

		pWorldItem = pObject->ToWorldItem();

		if ( !pWorldItem ) ///- Solamente tengo que procesar objetos que sean items
			continue;

		if ( !IN_RANGE(this, pWorldItem, this->GetHelper()->GetPickRange()) )
			continue;

		if ( this->HelperCanLoot(pWorldItem) )
		{
			item_template const* item_info = sItemMgr->GetItem(pWorldItem->item.GetItem());

			std::string name_lower(pWorldItem->item.GetName());

			if ( item_info && !item_info->IsLevelData(pWorldItem->item.GetLevel()) )
			{
				name_lower.append(" +");
				name_lower.append(std::to_string(pWorldItem->item.GetLevel()));
			}

			bool jewel = pWorldItem->item.IsJewel();
			uint8 ancient = pWorldItem->item.GetAncient();
			uint8 excellent = pWorldItem->item.GetExe();

			switch ( sItemMgr->GetItemInventoryType(pWorldItem->item.GetItem()) )
			{
			case ITEM_INVENTORY_TYPE_NORMAL:
				{
					ITEM_GET pMsg;
					pMsg.h.set(0x26, sizeof(ITEM_GET));
					pMsg.SetItem(pWorldItem->GetEntry());

					if ( this->ItemGet((uint8*)&pMsg, true) )
					{
						return;
					}
				} break;

			case ITEM_INVENTORY_TYPE_EVENT:
				{
					PMSG_EVENT_ITEM_GET_RECV pMsg;
					pMsg.index[0] = SET_NUMBERH(pWorldItem->GetEntry());
					pMsg.index[1] = SET_NUMBERL(pWorldItem->GetEntry());

					if ( sEventInventory->CGEventItemGetRecv(this, (uint8*)&pMsg) )
					{
						return;
					}
				} break;

			case ITEM_INVENTORY_TYPE_MUUN:
				{
					PMSG_MUUN_ITEM_GET_RECV pMsg;
					pMsg.index[0] = SET_NUMBERH(pWorldItem->GetEntry());
					pMsg.index[1] = SET_NUMBERL(pWorldItem->GetEntry());

					if ( sMuunSystem->CGMuunItemGetRecv(this, (uint8*)&pMsg) )
					{
						return;
					}
				} break;
			}
		}
	}
}

void Player::HelperPetZenLoot()
{
	Item const* pGuardian = this->GetInventory()->GetItem(GUARDIAN);

	if ( !pGuardian )
	{
		return;
	}

	if ( !pGuardian->IsItem() )
	{
		return;
	}

	if ( pGuardian->GetItem() != PET_PANDA &&
		 pGuardian->GetItem() != PET_SKELETON &&
		 pGuardian->GetItem() != PET_RUDOLF &&
		 pGuardian->GetItem() != PET_UNICORN )
	{
		return;
	}

	ViewportDataList const& viewport = this->GetAllViewport();
	for ( ViewportDataList::const_iterator it = viewport.begin(); it != viewport.end(); ++it )
	{
		ViewportData const* data = it->second;

		if ( !data ) ///- Si la data no existe por algún motivo, no lo proceso
			continue;

		if ( data->GetStatus() != Viewport::STATUS_READY ) ///- Solamente proceso objetos que estén listos
			continue;

		Object* pObject = data->GetObjectData();

		if ( !pObject ) ///- Si el objeto no existe, no lo proceso
			continue;

		WorldItem const* pWorldItem = pObject->ToWorldItem();

		if ( !pWorldItem ) ///- Solamente tengo que procesar objetos que sean items
			continue;

		if ( !IN_RANGE(this, pWorldItem, 4) )
			continue;

		if ( pWorldItem->item.GetItem() == ITEMGET(14, 15) )
		{
			ITEM_GET pMsg;
			pMsg.h.set(0x26, sizeof(ITEM_GET));
			pMsg.SetItem(pWorldItem->GetEntry());

			if ( this->ItemGet((uint8*)&pMsg, true) )
			{
				break;
			}
		}
	}
}

void Player::HelperOffline()
{
	if ( !this->GetHelper()->IsOffline() )
	{
		return;
	}

	if ( sGameServer->GetOfflineAttackTime() > 0 )
	{
		if ((MyGetTickCount() - this->GetHelper()->GetStartedTime()) > sGameServer->GetOfflineAttackTime())
		{
			this->HelperStop(0x01);
			this->GetHelper()->SetOffline(false);
			this->SetAutoLoginInfo(0);
			return;
		}
	}

	if ( sGameServer->IsOfflineCloseWhenWrongLevel() )
	{
		if ( sGameServer->GetOfflineAttackMinLevel() > 0 && this->GetTotalLevel() < sGameServer->GetOfflineAttackMinLevel() )
		{
			this->HelperStop(0x01);
			this->GetHelper()->SetOffline(false);
			this->SetAutoLoginInfo(0);
			return;
		}

		if ( sGameServer->GetOfflineAttackMaxLevel() > 0 && this->GetTotalLevel() > sGameServer->GetOfflineAttackMaxLevel() )
		{
			this->HelperStop(0x01);
			this->GetHelper()->SetOffline(false);
			this->SetAutoLoginInfo(0);
			return;
		}
	}

	if ( this->IsInSafeZone() )
	{
		if ( sGameServer->GetOfflineDieDisconnectTime() > 0 )
		{
			if ( (GetTickCount() - this->GetHelper()->GetDieTime()) > sGameServer->GetOfflineDieDisconnectTime() )
			{
				this->HelperStop(0x01);
				this->GetHelper()->SetOffline(false);
				this->SetAutoLoginInfo(0);
			}
		}

		return;
	}

	if ( this->GetHelper()->IsDisconencted() )
	{
		if ( sGameServer->GetOfflineDisconnectTime() > 0 )
		{
			if ( (GetTickCount() - this->GetHelper()->GetDisconnectTime()) > sGameServer->GetOfflineDisconnectTime() )
			{
				this->HelperStop(0x01);
				this->GetHelper()->SetOffline(false);
				this->SetAutoLoginInfo(0);
				return;
			}
		}
	}

	if ( !sGameServer->IsOfflineAttack() )
	{
		this->HelperStop(0x01);
		this->GetHelper()->SetOffline(false);
		this->SetAutoLoginInfo(0);
		return;
	}

	if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_OFFLINE_ATTACK) )
	{
		this->HelperStop(0x01);
		this->GetHelper()->SetOffline(false);
		this->SetAutoLoginInfo(0);
		return;
	}

	if ( !this->IsLive() )
	{
		return;
	}

	if ( !this->GetHelper()->IsStarted() )
	{
		this->HelperStart();
		return;
	}

	this->HelperOfflineHP();

	if (this->GetTimer(PLAYER_TIMER_OFFATTACK_UPDATE)->Elapsed(500))
	{
		this->HelperOfflineReloadArrow();
		this->HelperAutoloot();
		this->HelperPetZenLoot();
		this->HelperOfflineRepair();
	}

	if (GetTickCount() < this->GetHelper()->GetSkillTime())
	{
		return;
	}
	
	this->GetHelper()->SetSkillID(0);

	if ( !this->GetPathData()->IsStartEnd() )
	{
		this->GetHelper()->SetTarget(nullptr);
		this->GetHelper()->SetTargetRange(0);
	}

	if ( Unit* pTarget = this->GetHelper()->GetTarget() )
	{
		if ( !pTarget->SameDimension(this) ||
			 !pTarget->IsLive() ||
			 !pTarget->IsPlaying() ||
			 pTarget->IsTeleporting() ||
			 pTarget->IsInSafeZone() )
		{
			this->GetHelper()->SetTarget(nullptr);
			this->GetHelper()->SetTargetRange(0);

			if ( this->GetPathData()->IsStartEnd() )
			{
				this->ClearPathData(false);
			}
		}
	}

	if ( this->GetCombo()->GetProgressIndex() != -1 )
	{
		if ( this->GetCombo()->GetTime()->Elapsed() )
		{
			this->GetCombo()->Init();
		}
	}

	if ( this->GetHelper()->IsOriginalPosition() && !this->GetPathData()->IsStartEnd() && sGameServer->IsOfflineMove() )
	{
		if ( (GetTickCount() - this->GetHelper()->GetAttackTime()) > 3000 )
		{
			if ( (GetTickCount() - this->GetHelper()->GetOriginalPositionTime()) > (this->GetHelper()->GetMovementTime() * IN_MILLISECONDS) )
			{
				this->GetHelper()->SetOriginalPositionTime(GetTickCount());

				if ( this->GetHelper()->GetX() != this->GetX() ||
					 this->GetHelper()->GetY() != this->GetY() )
				{
					if ( World* pWorld = this->GetWorld() )
					{
						this->SetTempX(this->GetHelper()->GetX());
						this->SetTempY(this->GetHelper()->GetY());
						pWorld->GetPathFinderManager()->FindPath(this);
					}
				}
			}
		}
	}

	this->HelperOfflineSupport();
	this->HelperOfflineAttack();
}

void Player::HelperOfflineHP()
{
	if (this->GetHelper()->IsAutoPot())
	{
		if (this->PowerGetPercent(POWER_LIFE) < this->GetHelper()->GetAutoPotPercent())
		{
			int32 counter = 0;
			uint8 hp_potion = 0;

			do
			{
				counter++;

				if (counter > 20)
				{
					break;
				}

				hp_potion = this->HelperOfflineHPSlot();

				if (hp_potion == uint8(-1))
				{
					break;
				}

				USE_ITEM pMsg(hp_potion, 0, 0);
				this->ItemUse((uint8*)&pMsg);
			} while (this->PowerGetPercent(POWER_LIFE) < this->GetHelper()->GetAutoPotPercent());
		}
	}

	if (this->GetClass() == Character::FAIRY_ELF && this->GetHelper()->IsAutoHeal())
	{
		if (this->PowerGetPercent(POWER_LIFE) < this->GetHelper()->GetAutoHealPercent())
		{
			this->HelperOfflineSkillUse(this->MagicGetByBase(SKILL_HEAL), this);
		}
	}

	if (this->GetClass() == Character::SUMMONER && this->GetHelper()->IsDrainLife())
	{
		if (this->PowerGetPercent(POWER_LIFE) < this->GetHelper()->GetDrainLifePercent())
		{
			this->HelperOfflineSkillUse(this->MagicGetByBase(SKILL_DRAIN_LIFE), this->HelperOfflineGetRandomTarget(SKILL_DRAIN_LIFE, false));
		}
	}

	if (this->GetClass() == Character::FAIRY_ELF && this->GetHelper()->IsOptParty() && this->GetHelper()->IsOptReferenceOfPartyHeal())
	{
		this->HelperOfflineSkillUse(this->MagicGetByBase(SKILL_HEAL), this->HelperOfflineGetRandomPartyMember(true, SKILL_HEAL));
	}
}

uint8 Player::HelperOfflineHPSlot() const
{
	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		Item const* pItem = this->GetInventory()->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( pItem->GetItem() == ITEMGET(14, 0) ||
			 pItem->GetItem() == ITEMGET(14, 1) ||
			 pItem->GetItem() == ITEMGET(14, 2) ||
			 pItem->GetItem() == ITEMGET(14, 3) )
		{
			return i;
		}
	}

	return -1;
}

bool Player::HelperOfflineMP(Skill * pSkill, skill_template const* skill_info)
{
	int32 consume_mana = this->SkillManaUse(pSkill, skill_info);

	if ( this->PowerGet(POWER_MANA) >= consume_mana )
	{
		return true;
	}

	int32 counter = 0;
	uint8 mana_potion = 0;

	do
	{
		counter++;

		if ( counter > 100 )
		{
			return false;
		}

		mana_potion = this->HelperOfflineMP();

		if ( mana_potion == uint8(-1) )
		{
			return false;
		}

		USE_ITEM pMsg(mana_potion, 0, 0);
		this->ItemUse((uint8*)&pMsg);
	}
	while ( consume_mana > this->PowerGet(POWER_MANA) );

	return true;
}

uint8 Player::HelperOfflineMP() const
{
	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->GetItem(i)->IsItem() )
			continue;

		if ( !this->GetInventory()->CanUseExpandedSlot(i) ) 
			continue;

		if ( this->GetInventory()->GetItem(i)->GetItem() == ITEMGET(14, 4) ||
			 this->GetInventory()->GetItem(i)->GetItem() == ITEMGET(14, 5) ||
			 this->GetInventory()->GetItem(i)->GetItem() == ITEMGET(14, 6) )
		{
			return i;
		}
	}

	if ( this->GetHelper()->IsUseEliteManaPotion() )
	{
		use_inventory_loop(i)
		{
			if ( !this->GetInventory()->GetItem(i)->IsItem() )
				continue;

			if ( !this->GetInventory()->CanUseExpandedSlot(i) ) 
				continue;

			if ( this->GetInventory()->GetItem(i)->GetItem() == ITEMGET(14, 71) )
			{
				return i;
			}
		}
	}

	return -1;
}

void Player::HelperOfflineSupport()
{
	if ( this->GetHelper()->IsBuffItem() && sGameServer->IsOfflineAttackBuffItem() )
	{
		for ( int32 i = 0; i < 3; ++i )
		{
			if ( buff_template const* buff_info = sSkillMgr->GetBuffItem(this->GetHelper()->GetItemBuff(i)) )
			{
				if ( item_template const* item_info = sItemMgr->GetItem(this->GetHelper()->GetItemBuff(i)) )
				{
					if ( !this->HasBuff(buff_info->GetBuff()) )
					{
						uint8 buff_item_slot = this->ItemFind(this->GetHelper()->GetItemBuff(i), 0);

						if ( buff_item_slot != uint8(-1) )
						{
							this->AddItemBuff(this->GetHelper()->GetItemBuff(i), 0, BUFF_FLAG_DB_STORE, this);

							if ( item_info->GetStackData()->GetMaxStack() > 0 )
							{
								this->DecreaseItemDurabilityByUse(buff_item_slot, 1.0f);
							}
							else
							{
								this->ClearItem(buff_item_slot);
							}
						}
					}
				}
			}
		}
	}

	if ( this->GetHelper()->IsBuffDuration() )
	{
		for (int32 i = 0; i < 3; ++i)
		{
			if (skill_template const* skill_info = sSkillMgr->GetSkill(this->GetHelper()->GetBuffSkill(i)))
			{
				if (skill_info->GetMountCheck() == 2 && this->HasMount())
				{
					continue;
				}

				if (!this->HasBuff(skill_info->GetBuffIcon()))
				{
					this->HelperOfflineSkillUse(this->MagicGetByBase(this->GetHelper()->GetBuffSkill(i)), this);
				}
			}
		}

		Party* pParty = this->GetParty();

		if ( !pParty )
		{
			return;
		}
	
		if ( this->GetHelper()->IsOptParty() && this->GetHelper()->IsBuffDurationForAllParty() )
		{
			if ( this->GetHelper()->GetCurrentPartyMember() < MAX_PARTY_MEMBERS )
			{
				Player* pPlayer = pParty->GetMember(this->GetHelper()->GetCurrentPartyMember())->GetPlayer();

				if (pPlayer && pPlayer != this && pPlayer->IsLive() && this->SameDimension(pPlayer))
				{
					uint16 skill_base = sSkillMgr->GetSkillBaseSkill(this->GetHelper()->GetBuffSkill(this->GetHelper()->GetCurrentPartySkill()));

					if (skill_base != SKILL_BERSERKER &&
						skill_base != SKILL_RED_ARMOR_IGNORE &&
						skill_base != SKILL_INFINITY_ARROW &&
						skill_base != SKILL_CIRCLE_SHIELD &&
						skill_base != SKILL_OBSIDIAN &&
						skill_base != SKILL_WRATH &&
						skill_base != SKILL_BURST &&
						skill_base != SKILL_GREATER_FORTITUDE &&
						skill_base != SKILL_FITNESS &&
						skill_base != SKILL_DEFENSE_SUCCESS_RATE &&
						skill_base != SKILL_CRITICAL_DAMAGE)
					{
						this->HelperOfflineSkillUse(this->MagicGetByBase(this->GetHelper()->GetBuffSkill(this->GetHelper()->GetCurrentPartySkill())), pPlayer);
					}
				}

				this->GetHelper()->IncreaseCurrentPartySkill(1);

				if ( this->GetHelper()->GetCurrentPartySkill() >= 3 )
				{
					this->GetHelper()->SetCurrentPartySkill(0);
					this->GetHelper()->IncreaseCurrentPartyMember(1);
				}
			}
			else
			{
				if ( (GetTickCount() - this->GetHelper()->GetBuffCastingTime()) > (this->GetHelper()->GetTimeSpaceCasting() * IN_MILLISECONDS) )
				{
					this->GetHelper()->SetCurrentPartySkill(0);
					this->GetHelper()->SetCurrentPartyMember(0);
					this->GetHelper()->SetBuffCastingTime(GetTickCount());
				}
			}
		}
	}
}
	
void Player::HelperOfflineAttack()
{
	int32 monsters_within_range = this->HelperOfflineGetMonsterWithinHuntingRange();

	for ( int32 i = 0; i < 2; ++i )
	{
		if ( !sSkillMgr->IsSkill(this->GetHelper()->GetAttackSkill(i + 1)) )
		{
			continue;
		}

		if ( sSkillMgr->GetSkillBaseSkill(this->GetHelper()->GetAttackSkill(i + 1)) == SKILL_HEAL )
		{
			continue;
		}

		if ( this->GetHelper()->IsOptCombo() )
		{
			if ( this->GetCombo()->GetProgressIndex() == 0 ||
				 this->GetCombo()->GetProgressIndex() == 1 )
			{
				if ( this->GetCombo()->GetSkill(this->GetCombo()->GetProgressIndex()) != this->GetHelper()->GetAttackSkill(i + 1) )
				{
					this->HelperOfflineSkillUse(this->MagicGet(this->GetHelper()->GetAttackSkill(i + 1)), this->HelperOfflineGetRandomTarget(this->GetHelper()->GetAttackSkill(i + 1), this->GetHelper()->IsSkillConditionMobAttackingMe(i)));
				}
			}

			continue;
		}

		if ( !this->GetHelper()->IsSkillDelay(i) &&
			 !this->GetHelper()->IsSkillCondition(i) )
		{
			continue;	
		}

		if ( this->GetHelper()->IsSkillCondition(i) )
		{
			if ( this->GetHelper()->IsSkillConditionMobMoreThan5(i) )
			{
				if ( monsters_within_range <= 5 )
				{
					continue;
				}
			}
			else if ( this->GetHelper()->IsSkillConditionMobMoreThan4(i) )
			{
				if ( monsters_within_range <= 4 )
				{
					continue;
				}
			}
			else if ( this->GetHelper()->IsSkillConditionMobMoreThan3(i) )
			{
				if ( monsters_within_range <= 3 )
				{
					continue;
				}
			}
			else
			{
				if ( monsters_within_range <= 2 )
				{
					continue;
				}
			}
		}

		if (this->GetHelper()->IsSkillDelay(i))
		{
			if ((GetTickCount() - this->GetHelper()->GetSkillAttackDelay(i)) < (this->GetHelper()->GetDelay(i) * IN_MILLISECONDS))
			{
				continue;
			}
		}

		if (this->HelperOfflineSkillUse(this->MagicGet(this->GetHelper()->GetAttackSkill(i + 1)), this->HelperOfflineGetRandomTarget(this->GetHelper()->GetAttackSkill(i + 1), this->GetHelper()->IsSkillConditionMobAttackingMe(i))))
		{
			this->GetHelper()->SetSkillAttackDelay(i, GetTickCount());
		}
	}

	if ( !this->GetHelper()->IsOptCombo() || this->GetCombo()->GetProgressIndex() == -1 )
	{
		if (sSkillMgr->GetSkillBaseSkill(this->GetHelper()->GetAttackSkill(0)) != SKILL_HEAL)
		{
			this->HelperOfflineSkillUse(this->MagicGet(this->GetHelper()->GetAttackSkill(0)), this->HelperOfflineGetRandomTarget(this->GetHelper()->GetAttackSkill(0), false));
		}
	}

	if ( this->GetPathData()->IsStartEnd() )
	{
		return;
	}

	if ( this->GetHelper()->IsUseRegularAttack() && !this->GetHelper()->GetSkillID() )
	{
		Monster* pMonster = this->HelperOfflineGetRandomTarget(this->GetHelper()->GetAttackSkill(0), false);

		if ( pMonster )
		{
			int32 distance = Util::Distance(this->GetX(), this->GetY(), pMonster->GetX(), pMonster->GetY());
			
			if ( distance > 2 )
			{
				if ( sGameServer->IsOfflineMove() )
				{
					this->HelperOfflineMoveToTarget(pMonster, this->GetHelper()->GetHuntingRange());

					if ( this->GetTempX() != this->GetX() ||
						 this->GetTempY() != this->GetY() )
					{
						if ( this->CheckWall(this->GetTempX(), this->GetTempY()) )
						{
							this->SetDirection(GetPathPacketDirPos(pMonster->GetX() - this->GetX(), pMonster->GetY() - this->GetY()));

							if ( World* pWorld = this->GetWorld() )
							{
								if ( pWorld->GetPathFinderManager()->FindPath(this) )
								{
									this->GetHelper()->SetTarget(pMonster);
									this->GetHelper()->SetTargetRange(2);
								}
							}
						}
					}
				}
				return;
			}

			this->GetHelper()->SetAttackTime(GetTickCount());

			ATTACK_REQUEST pMsg;
			pMsg.h.set(HEADCODE_ATTACK_NORMAL, sizeof(ATTACK_REQUEST));
			pMsg.SetTarget(pMonster->GetEntry());
			pMsg.action = 120;
			pMsg.dir_dis = GetPathPacketDirPos(pMonster->GetX() - this->GetX(), pMonster->GetY() - this->GetY());
			this->NormalAttack((uint8*)&pMsg);
		}
	}
}

void Player::HelperOfflineRepair()
{
	if (this->GetHelper()->IsOptRepairItems())
	{
		wear_inventory_loop(i)
		{
			this->HelperOfflineRepair(i);
		}

		this->HelperOfflineRepair(EARRING_01);
		this->HelperOfflineRepair(EARRING_02);

		use_inventory_loop(i)
		{
			this->HelperOfflineRepair(i);
		}
	}

	if (this->GetHelper()->IsFlag(1, HELPER_OPTION_REPAIR_MUUN))
	{
		for (int32 i = 0; i < MUUN_INVENTORY_WEAR_SIZE; ++i)
		{
			auto pItem = this->GetMuunInventory()->GetItem(i);

			if (!pItem || !pItem->IsItem() || pItem->GetDurability() > 1)
			{
				continue;
			}

			uint8 jewel = this->ItemFind(JEWEL::LIFE, -1);

			if (jewel == (uint8)-1)
			{
				break;
			}

			if (sMuunSystem->CharacterUseJewelOfLife(this, jewel, i))
			{
				this->ItemDeleteByUse(jewel);
			}
		}
	}
}

void Player::HelperOfflineRepair(uint8 slot)
{
	Item const* pItem = this->GetInventory()->GetItem(slot);

	if (!pItem)
	{
		return;
	}

	if (!pItem->IsItem())
	{
		return;
	}

	if (pItem->GetExpireDate())
	{
		return;
	}

	if (pItem->GetDurabilityOriginal() <= 0.0f)
	{
		return;
	}

	if (pItem->GetDurability() == pItem->GetDurabilityBase())
	{
		return;
	}

	if (!sItemMgr->IsItemFlag(pItem->GetItem(), pItem->GetLevel(), ITEM_FLAG_ALLOW_REPAIR))
	{
		return;
	}

	if (pItem->IsInventoryActive() && pItem->GetDurability() > 1)
	{
		return;
	}

	if (pItem->GetKind2() == ItemKind::GUARDIAN_MOUNT && !this->GetHelper()->IsFlag(1, HELPER_OPTION_REPAIR_GUARDIAN))
	{
		return;
	}

	this->ItemRepair(slot, 0);
}

bool Player::HelperOfflineSkillUse(Skill* pSkill, Unit* pTarget)
{
	if (!this->HelperOfflineSpeed())
	{
		return false;
	}

	if (this->GetHelper()->GetSkillID())
	{
		return false;
	}

	if (this->GetPathData()->IsStartEnd())
	{
		return false;
	}

	if ((MyGetTickCount() - this->GetHelper()->GetLastMoveTime()) < 1000)
	{
		return false;
	}

	if (GetTickCount() < this->GetHelper()->GetSkillTime())
	{
		return false;
	}

	if (!pSkill || !pTarget)
	{
		return false;
	}

	if (!this->CheckSkillCooldown(pSkill->GetSkill()))
	{
		return false;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(pSkill->GetSkill());

	if (!skill_info)
	{
		return false;
	}

	if (!this->HelperOfflineMP(pSkill, skill_info))
	{
		return false;
	}

	int32 distance = Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY());
	int32 max_distance = SkillHandler::GetSkillRange(this, pSkill->GetSkill());

	if (max_distance <= 0)
	{
		max_distance = 1;
	}

	if (this->GetHelper()->IsUseSkillClosely())
	{
		max_distance = 2;
	}

	if (distance > max_distance)
	{
		if (sGameServer->IsOfflineMove())
		{
			this->HelperOfflineMoveToTarget(pTarget, max_distance);

			if (this->GetTempX() != this->GetX() ||
				this->GetTempY() != this->GetY())
			{
				if (this->CheckWall(this->GetTempX(), this->GetTempY()))
				{
					this->SetDirection(GetPathPacketDirPos(pTarget->GetX() - this->GetX(), pTarget->GetY() - this->GetY()));

					if (World* pWorld = this->GetWorld())
					{
						if (pWorld->GetPathFinderManager()->FindPath(this))
						{
							this->GetHelper()->SetTarget(pTarget);
							this->GetHelper()->SetTargetRange(max_distance);
						}
					}
				}
			}
		}

		return false;
	}

	if (!this->CheckWall(pTarget->GetX(), pTarget->GetY()))
	{
		return false;
	}

	this->GetHelper()->SetAttackTime(GetTickCount());
	this->GetHelper()->SetSkillID(pSkill->GetSkill());

	if (!skill_info->animation)
	{
		NORMAL_MAGIC_ATTACK pMsg;
		pMsg.h.set(HEADCODE_NORMAL_MAGIC_ATTACK, sizeof(NORMAL_MAGIC_ATTACK));
		pMsg.MagicH = SET_NUMBERH(pSkill->GetSkill());
		pMsg.MagicL = SET_NUMBERL(pSkill->GetSkill());
		pMsg.TargetH = SET_NUMBERH(pTarget->GetEntry());
		pMsg.TargetL = SET_NUMBERL(pTarget->GetEntry());
		pMsg.attack_time = getMSTime();
		this->NormalMagicAttack((uint8*)&pMsg);
	}
	else
	{
		DURATION_MAGIC_ATTACK pMsg;
		pMsg.h.set(HEADCODE_DURATION_MAGIC_ATTACK, sizeof(DURATION_MAGIC_ATTACK));
		pMsg.SkillH = SET_NUMBERH(pSkill->GetSkill());
		pMsg.SkillL = SET_NUMBERL(pSkill->GetSkill());
		pMsg.TargetH = SET_NUMBERH(pTarget->GetEntry());
		pMsg.TargetL = SET_NUMBERL(pTarget->GetEntry());
		pMsg.Dir = this->HelperGetAngle(pTarget) - 127;//GetPathPacketDirPos(pTarget->GetX() - this->GetX(), pTarget->GetY() - this->GetY());
		pMsg.Dis = 0;
		pMsg.target_pos = this->HelperGetAngle(pTarget);
		pMsg.x = pTarget->GetX();
		pMsg.y = pTarget->GetY();
		pMsg.attack_time = getMSTime();
		this->DurationMagicAttack((uint8*)&pMsg);
	}

	auto pAttackData = sSkillMgr->GetSkillAttackTime(this, sSkillMgr->GetSkillBaseSkill(pSkill->GetSkill()));

	if (pAttackData && pAttackData->GetRealTime() > 0)
	{
		this->GetHelper()->SetSkillTime(GetTickCount() + pAttackData->GetRealTime());
	}
	else if (skill_info->GetAttackDelay() > 0)
	{
		this->GetHelper()->SetSkillTime(GetTickCount() + skill_info->GetAttackDelay());
	}
	else if (sGameServer->GetOfflineSkillTime() > 0)
	{
		this->GetHelper()->SetSkillTime(GetTickCount() + sGameServer->GetOfflineSkillTime());
	}
	else
	{
		this->GetHelper()->SetSkillTime(GetTickCount() + 500);
	}

	return true;
}

int32 Player::HelperOfflineGetMonsterWithinHuntingRange()
{
	int32 monsters_within_range = 0;

	for ( ViewportDataList::const_iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); ++it )
	{
		ViewportData const* pData = it->second;

		if ( !pData )
		{
			continue;
		}

		Object* pObject = pData->GetObjectData();

		if ( !pObject )
		{
			continue;
		}

		Monster* pMonster = pObject->ToCreature();

		if ( !pMonster )
		{
			continue;
		}

		if ( !this->AttackAllowed(pMonster) )
		{
			continue;
		}

		int32 distance = Util::Distance(this->GetX(), this->GetY(), pMonster->GetX(), pMonster->GetY());

		if ( distance > this->GetHelper()->GetHuntingRange() )
		{
			continue;
		}

		++monsters_within_range;
	}

	return monsters_within_range;
}

Monster* Player::HelperOfflineGetRandomTarget(uint16 skill, bool attacking_me)
{
	int32 min_distance = 0;
	Monster* pVictim = nullptr;

	Skill* pSkill = this->MagicGetByBase(skill);

	/*int32 attack_range = sSkillMgr->GetSkillRange(pSkill ? pSkill->GetSkill() : 0);

	if ( attack_range == 0 )
	{
		attack_range = 2;
	}*/

	for ( ViewportDataList::const_iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); ++it )
	{
		ViewportData const* pData = it->second;

		if ( !pData )
		{
			continue;
		}

		Object* pObject = pData->GetObjectData();

		if ( !pObject )
		{
			continue;
		}

		Monster* pMonster = pObject->ToCreature();

		if ( !pMonster )
		{
			continue;
		}

		if ( !this->AttackAllowed(pMonster) )
		{
			continue;
		}

		if (sGameServer->IsOfflineAttackMonsterAICheck())
		{
			if (pMonster->GetAI() && !pMonster->GetAI()->EnableAttack(this, nullptr))
			{
				continue;
			}
		}

		if ( !this->CheckWall(pMonster) )
		{
			continue;
		}

		if ( attacking_me && pMonster->GetTarget() != this )
		{
			continue;
		}

		Unit* pSummoner = pMonster->GetSummoner();

		if ( pSummoner && pSummoner != pMonster && pSummoner->IsPlayer() )
		{
			continue;
		}

		int32 distance = Util::Distance(this->GetX(), this->GetY(), pMonster->GetX(), pMonster->GetY());

		//if ( distance > attack_range || distance > this->GetHelper()->GetHuntingRange() )
		if ( distance > this->GetHelper()->GetHuntingRange() )
		{
			continue;
		}

		if ( min_distance == 0 || distance < min_distance )
		{
			min_distance = distance;
			pVictim = pMonster;
		}
	}

	return pVictim;
}

Player* Player::HelperOfflineGetRandomPartyMember(bool heal, uint16 skill)
{
	Party* pParty = this->GetParty();

	if (!pParty)
	{
		return nullptr;
	}

	Skill* pSkill = this->MagicGetByBase(skill);

	if (!pSkill)
	{
		return nullptr;
	}

	int32 attack_range = SkillHandler::GetSkillRange(this, pSkill->GetSkill());

	if (attack_range <= 0)
	{
		return nullptr;
	}

	int32 min_distance = 0;
	Player* pPlayer = nullptr;

	Unit* pMembers[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	this->GetPartyMembers(pMembers, count, attack_range);

	if (count == 0)
	{
		return nullptr;
	}

	for (int32 i = 0; i < count; ++i)
	{
		Unit* pUnit = pMembers[i];

		if (!pUnit)
		{
			continue;
		}

		if (heal && pUnit == this)
		{
			continue;
		}

		Player* pMember = pUnit->ToPlayer();

		if (!pMember)
		{
			continue;
		}

		if (!pMember->IsLive())
		{
			continue;
		}

		if (heal)
		{
			if (pMember->PowerGetPercent(POWER_LIFE) >= this->GetHelper()->GetPartyHealPercent())
			{
				continue;
			}
		}

		int32 distance = Util::Distance(this->GetX(), this->GetY(), pMember->GetX(), pMember->GetY());

		if (min_distance == 0 || distance < min_distance)
		{
			min_distance = distance;
			pPlayer = pMember;
		}
	}

	return pPlayer;
}

bool Player::HelperOfflineSpeed()
{
	return true;
}

void Player::HelperOfflineMoveToTarget(Unit* pUnit, int32 range)
{
	this->SetTempX(this->GetX());
	this->SetTempY(this->GetY());

	if ( !pUnit )
	{
		return;
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	if ( this->HasRestrictionBuff() )
	{
		return;
	}

	if ( pUnit->IsTeleporting() )
	{
		return;
	}

	int16 tpx = pUnit->GetX();
	int16 tpy = pUnit->GetY();

	int16 mtx = tpx;
	int16 mty = tpy;

	int32 dis = range;

	if ( this->GetX() < mtx )
	{
		tpx -= dis;
	}

	if ( this->GetX() > mtx )
	{
		tpx += dis;
	}

	if ( this->GetY() < mty )
	{
		tpy -= dis;
	}

	if ( this->GetY() > mty )
	{
		tpy += dis;
	}

	int16 searchp = GetPathPacketDirPos(pUnit->GetX() - tpx, pUnit->GetY() - tpy) * 2;

	WorldGrid const& grid = pWorld->GetGrid(tpx, tpy);

	if ( !grid.attribute )
	{
		PATH_LOOP
		{
			mtx = pUnit->GetX() + Path::Table[searchp];
			mty = pUnit->GetY() + Path::Table[searchp + 1];

			WorldGrid const& verify_grid = pWorld->GetGrid(mtx, mty);

			if ( !verify_grid.attribute )
			{
				this->SetTempX(mtx);
				this->SetTempY(mty);
				return;
			}

			Path::FixSearch(searchp);
		}

		this->SetTempX(tpx);
		this->SetTempY(tpy);
		return;
	}
}

void Player::HelperOfflineReloadArrow()
{
	auto pWeapon01 = this->GetInventory()->GetItem(WEAPON_01);
	auto pWeapon02 = this->GetInventory()->GetItem(WEAPON_02);

	if (pWeapon02->GetKind2() == ItemKind::QUIVER)
	{
		return;
	}

	if (pWeapon01->GetKind2() == ItemKind::BOW || pWeapon01->GetKind2() == ItemKind::CROSSBOW)
	{
		if (pWeapon02->IsItem())
		{
			return;
		}

		if (pWeapon01->GetKind2() == ItemKind::BOW)
		{
			this->EquipmentAutochange(WEAPON_02, ITEMGET(4, 15), -1);
		}
		else if (pWeapon01->GetKind2() == ItemKind::CROSSBOW)
		{
			this->EquipmentAutochange(WEAPON_02, ITEMGET(4, 7), -1);
		}
	}
}

uint8 Player::HelperGetAngle(Unit* pTarget)
{
	int32 angle = this->GetAngle(pTarget->GetX(), pTarget->GetY());

	uint8 mangle = angle * 255 / 360;

	this->GetHelper()->SetAttackAngle(mangle);

	return mangle;
}

bool Player::IsHelperSkill(uint16 skill) const
{
	for (int32 i = 0; i < 3; ++i)
	{
		uint16 base_skill = sSkillMgr->GetSkillBaseSkill(this->GetHelper()->GetAttackSkill(i));

		if (base_skill == skill)
		{
			return true;
		}
	}

	return false;
}

void Player::HandleHelperPlusData(uint8 * Packet)
{

}

void Player::HandleHelperPlusRun(uint8 * Packet)
{

}