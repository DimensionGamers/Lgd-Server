/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerCharacter.cpp"
*
*/

void Player::ClearCharacter()
{
	this->SetPartyID(-1);
	this->SetGUID(0);
	this->ResetName();
	this->SetSlot(0);
	this->SetDBClass(0);
	this->SetClass(0);
	this->ResetChangeUP(0);
	this->SetAuthority(0);

	this->ResetStat(0);
	this->ResetAddStat(0);

	this->SetFruitPointsAdd(0);
	this->SetFruitPointsDec(0);

	for ( int32 i = 0; i < LEVEL_DATA_MAX; ++i )
	{
		this->GetLevelData(i)->Reset();
	}
	
	this->MoneySet(0);

	Util::memset<uint8, uint8>(this->preview, 0, MAX_PREVIEW_DATA);
	this->SetFullAncient(false);

	this->SetPKLevel(0);
	this->SetPKCount(0);
	this->SetPKPoints(0);

	this->GetInventory()->Clear();
	this->GetTransactionInventory()->Clear();
	this->GetTradeInventory()->Clear();
	this->GetMixInventory()->Clear();
	this->GetMixRecoveryInventory()->Clear();

	this->GetInterfaceState()->Reset();

	this->quest_evolution.clear();

	this->SetQuestEvolutionSend(false);

	this->SetSkin(uint16(-1));

	FRIEND_LOOP(i)
	{
		this->GetFriend(i)->Reset();
	}
	
	MAIL_LOOP(i)
	{
		this->GetMailData(i)->Reset();
	}

	this->ResetPowerFillCount(0);
	
	POWER_LOOP(i)
	{
		this->GetPowerAutorecuperationTime(i)->Start();
	}

	this->SetTransaction(TRANSACTION_TYPE_NONE);

	this->SetGuildID(0);

	this->GetPersonalStore()->Reset();

	this->SetLastDurationSkill(0);
	this->SetLastDurationSkillTime(0);
	this->SetLastDurationSkillCount(0);
	this->SetLastDurationSkillX(0);
	this->SetLastDurationSkillY(0);

	this->GetHelper()->Reset();

	this->SetStaminaReduction(0);

	this->ResetItemDurabilityReduction(0.0f);

	this->SetMuteTime(time(nullptr));
	this->SetAdministratorFlag(0);

	this->TalkingGuild.set(0);
	this->TalkingAlliance.set(0);

	this->GetDarkSpirit()->Reset();

	this->SetAccumulatedDamage(0);
	this->SetAccumulatedTime(0);
	this->SetBookHitCount(0);

	MAP_CLEAR(SelfDefenseDataMap::iterator, this->self_defense_map);

	this->GetTalismanResurrection()->Reset();
	this->GetTalismanMobility()->Reset();

	this->SetFirstTime(false);

	for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; ++i )
	{
		this->GetRestriction(i)->Reset();
	}

	this->ResetAleTime(0);
	this->ResetAleEffect(0);
	this->ResetAleItem(0);
	this->ResetAleLevel(0);

	STATISTIC_LOOP(i)
	{
		this->GetStatisticData(i)->Reset();
	}

	this->m_item_delay.clear();

	this->ResetData(0);
	this->ResetDataFloat(0.0f);

	memset(this->requested_friend, 0, MAX_CHARACTER_LENGTH + 1);

	this->SetSantaClausGiftDate(0);
	this->SetSantaClausGiftRequest(0);

	this->SetEventAnimation(false);

	this->m_gen_victim_map.clear();
	this->SetFlag(CHARACTER_FLAG_REQUEST | CHARACTER_FLAG_POST);
	this->SetAdminPanelFlag(0);
	
	this->SetPostTime(0);

	for ( uint32 i = 0; i < MULTI_ATTACK_MAX_INFO; ++i )
	{
		this->GetMultiAttack(i)->Reset();
	}

	this->SetMultiAttackCount(0);

	this->GetDurationMagicKey()->Reset();
	this->SetAttackStandby(0);
	this->SetCrywolfScore(0);
	this->SetGoblinPointCount(0);
	this->SetKanturuAllowed(false);
	this->SetCurrentTarget(nullptr);

	this->GetGameOption()->Reset();

	this->SetGoblinPoints(0);
	this->SetCastleSiegeMoneyRemove(false);

	this->SetLastItemUpdate(0);
	
	this->SetRageFighterSkill(0);
	this->SetRageFighterSkillTarget(-1);
	this->SetRageFighterSkillCount(0);
	this->SetRageFighterSkillMaxCount(0);
	this->SetInChallenge(false);

	this->SetKickTime(0);

	this->ResetCurrentCharacterSearch();

	this->SetShopTime(0);

	this->SetPostCount(0);
	this->SetPostDay(0);
	this->SetPostMonth(0);

	this->ResetAntiSpamMessage();

	for ( int32 i = 0; i < PLAYER_TIMER_MAX; ++i )
	{
		if ( i == PLAYER_TIMER_PACKET ||
			 i == PLAYER_TIMER_WRONG_SERIAL ||
			 i == PLAYER_TIMER_WRONG_DECRYPT )
		{
			continue;
		}

		this->GetTimer(i)->Start();
	}

	this->ResetCounter(0);
	this->ResetDate(0);

	this->SetArmorSetBonus(false);

	MAP_CLEAR(PlayerSkillTreeMap::iterator, this->skill_tree_map);
	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_list);
	LIST_CLEAR(PlayerMajesticSkillTreeList::iterator, this->majestic_skill_tree_stat_list);
	this->SetMajesticAddStatPoint(false);
	this->SetMajesticStatPoints(0);
	this->ResetMajesticStat(0);

	SKILL_LOOP(i)
	{
		this->GetTransactionSkill(i)->Clear();
	}

	this->SetTransactionRuud(0);
	this->SetTransactionMoney(0);
	this->SetTransactionCredits(0);

	this->SetUseGuildMatching(false);
	this->SetUseGuildMatchingJoin(false);

	this->GetPentagramOption()->Reset();
	this->GetPentagramJewelOption()->Reset();

	for ( int32 i = 0; i < MAX_PENTAGRAM_JEWEL_INFO; ++i )
	{
		this->GetPentagramJewelInfoInventory(i)->Clear();
		this->GetPentagramJewelInfoWarehouse(i)->Clear();
	}

	this->GetEventInventory()->Clear();
	this->GetTransactionEventInventory()->Clear();

	this->SetMuunItemStatus(false);
	this->GetMuunInventory()->Clear();

	this->SetDisplayMuunSlot(0);

	this->SetMiniMapState(false);
	this->SetMiniMapWorld(-1);

	this->GetMuRoomyInfo()->Init();

	this->SetPacketResult(0);
	this->SetPacketData1(0);
	this->SetPacketData2(0);
	this->SetPacketData3(0);

	this->chat_block_list.clear();

	this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->Clear();
	this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->Clear();

	this->SetRuudMoney(0);

	this->ResetSpecializationStat(0);
	this->ResetBaseStat(0);
	this->ResetEventNotification(0);

	//this->SetEvomon(false);
	this->SetEvomonPortal(nullptr);
	this->SetEvomonTime(0);

	this->SetMuunTarget(nullptr);

	this->SetCurrentGate(0);
	this->SetDelayedTeleportGate(0);

	this->SetCoordinateAttribute(0);

	this->SetMixCompleted(false);

	this->GetNpcSell()->Clear();

	this->SetWalkSpeedCount(0);

	this->skill_cooldown.clear();

	this->SetMiningStatus(0);

	this->SetHuntingRecordActive(false);

	for ( HuntingRecordDataMap::iterator it = this->hunting_record_data.begin(); it != this->hunting_record_data.end(); ++it )
	{
		LIST_CLEAR(HuntingRecordDataList::iterator, it->second);
	}

	this->hunting_record_data.clear();

	this->GetCurrentHuntingData()->Reset();
	this->SetHuntingRecordWorld(0);
	this->SetHuntingRecordTarget(-1);
	this->SetHuntingRecordVisible(false);

	this->ResetAncientEffect(0);

	this->SetDarkLordPetSlot(-1);
	this->SetDarkLordPetType(-1);
	this->SetDarkLordPetFlag(-1);

	this->SetGameOptionSaved(false);

	this->SetGiftQuery(false);
	this->SetMailQuery(false);

	this->SetPartyMatchingID(PARTY_NULL);
	this->SetPartyMatchingAssist(false);

	this->SetDueling(false);
	this->SetDuelScore(0);
	this->SetDuelRoom(0);
	this->SetDuelSpectatorActive(false);
	this->SetDuelTarget(nullptr);
	this->SetDuelClassic(false);
	this->SetDuelWorld(0);

	this->SetGuildMatchingUpdateTime(0);

	this->GetLabyrinthData()->Init();

	MAP_CLEAR(PlayerEventEnterCountMap::iterator, this->event_enter_count_map);

	this->SetMiniBombState(MINI_BOMB_GAME_START);
	this->SetMiniBombCurrentScore(0);
	this->SetMiniBombFinalScore(0);
	this->SetMiniBombTotalScore(0);
	this->SetMiniBombDate(0);

	for ( int32 i = 0; i < MINI_BOMB_SIZE; ++i )
	{
		this->GetMiniBombData(i)->Reset();
	}
	
	LIST_CLEAR(CashShopItemDataList::iterator, this->m_CashShopItemList);
	LIST_CLEAR(CashShopItemDataList::iterator, this->m_CashShopGiftList);

	this->ResetCashShopGiftName();
	this->ResetCashShopGiftMessage();
	this->SetCashShopGiftPackage(0);
	this->SetCashShopGiftOption(0);

	for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
	{
		this->GetWarpFavoriteList(i)->Reset();
	}

	this->SetQuestGuidedID(0);
	this->SetQuestGuidedMove(false);
	MAP_CLEAR(PlayerQuestGuidedMap::iterator, this->quest_guided_map);

	this->SetWorldState(-1);

	for ( MajesticDataMap::iterator itr = this->majestic_data_map.begin(); itr != this->majestic_data_map.end(); ++itr )
	{
		itr->second.clear();
	}

	this->majestic_data_map.clear();

	this->SetJewelBingoState(JEWEL_BINGO_STATE_NONE);
	this->SetJewelBingoBox(0);
	this->SetJewelBingoCount(0);
	this->SetJewelBingoCurrentJewel(0);
	this->SetJewelBingoScore1(0);
	this->SetJewelBingoScore2(0);
	this->SetJewelBingoScore3(0);
	this->SetJewelBingoDate(0);

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		this->GetJewelBingoGrid(i)->Reset();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		this->GetJewelBingoJewel(i)->Reset();
	}

	for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
	{
		this->GetJewelBingoMatching(i)->Reset();
	}

	this->SetNumericBaseballState(0);
	this->SetNumericBaseballScore(0);
	this->ResetNumericBaseballNumber(0xFF);
	this->ResetNumericBaseballUsedNumber(0xFF);
	this->ResetNumericBaseballStrike(0);
	this->ResetNumericBaseballBall(0);
	this->SetNumericBaseballCurrentNumber(0);

	this->SetTrackingID(0);

	this->SetSentMapState(false);

	this->SetCurrentTargetPlayer(-1);
	this->SetDelayedTeleportHelper(false);

	this->SetCreateDate(0);

	this->SetClientMinimized(false);
	this->SetSendBuff(true);

	MAP_CLEAR(SkillTimeMap::iterator, this->m_SkillTimeMap);

	this->SetCashShopDiscountWC(0);
	this->SetCashShopDiscountGP(0);
	this->SetCashShopDiscountDate(0);

	MAP_CLEAR(SkillCPSMap::iterator, this->m_SkillCPSMap);

	this->m_AncientSetDataMap.clear();

	this->SetVehiculeType(0);

	MAP_CLEAR(QuestMUDataMap::iterator, this->m_QuestMUDataMap);
	this->SetQuestMUAutoMoving(0);

	MAP_CLEAR(MonsterSoulDataMap::iterator, this->m_MonsterSoulDataMap);

	this->SetVisibleMount(-1);

	this->SetSummonerDebuff(nullptr);
	this->SetMajesticDebuff(nullptr);
	this->SetMajesticDebuffTick(MyGetTickCount());

	this->SetAccountDataLoaded(false);

	this->SetLastSkillTime(0);
	this->SetLastSkillChecked(0);

	this->GetACPathData()->Reset();

	this->Unit::Init();
}

void Player::CalculateCharacter()
{
	CharacterBaseData const* base_data = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !base_data )
	{
		return;
	}

	Item const* Right = this->GetInventory()->GetItem(WEAPON_01);
	Item const* Left = this->GetInventory()->GetItem(WEAPON_02);
	Item const* Wings = this->GetInventory()->GetItem(WINGS);
	Item const* Helm = this->GetInventory()->GetItem(HELM);
	Item const* Armor = this->GetInventory()->GetItem(ARMOR);
	Item const* Pants = this->GetInventory()->GetItem(PANTS);
	Item const* Gloves = this->GetInventory()->GetItem(GLOVES);
	Item const* Boots = this->GetInventory()->GetItem(BOOTS);
	Item const* Ring01 = this->GetInventory()->GetItem(RING_01);
	Item const* Ring02 = this->GetInventory()->GetItem(RING_02);
	Item const* Pendant = this->GetInventory()->GetItem(PENDANT);
	Item const* Helper = this->GetInventory()->GetItem(GUARDIAN);
	Item const* Earring01 = this->GetInventory()->GetItem(EARRING_01);
	Item const* Earring02 = this->GetInventory()->GetItem(EARRING_02);
	
	Item const* Pentagram = this->GetInventory()->GetItem(PENTAGRAM_SLOT);

	bool item_update = false;

	wear_inventory_loop(i)
	{
		this->GetInventory()->GetItem(i)->SetValid(this->GetInventory()->GetItem(i)->IsItem() && (this->GetInventory()->GetItem(i)->GetDurability() >= 1.0f));
	}

	this->GetInventory()->GetItem(PENTAGRAM_SLOT)->SetValid(this->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsItem());
	this->GetInventory()->GetItem(EARRING_01)->SetValid(this->GetInventory()->GetItem(EARRING_01)->IsItem() && this->GetInventory()->GetItem(EARRING_01)->GetDurability() >= 1);
	this->GetInventory()->GetItem(EARRING_02)->SetValid(this->GetInventory()->GetItem(EARRING_02)->IsItem() && this->GetInventory()->GetItem(EARRING_02)->GetDurability() >= 1);

	use_inventory_loop(i)
	{
		Item* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsInventoryActive())
		{
			continue;
		}

		if (pItem->IsMount() || pItem->GetItem() == PET_DARK_RAVEN)
		{
			pItem->SetValid(pItem->GetDurability() >= 1.0f);
		}
	}

	Util::memset(this->power_add, 0, POWER_MAX);
	this->ResetIntData(0);
	this->ResetFloatData(0.0f);
	this->ResetData(0);
	this->ResetItemDurabilityReduction(0.0f);
	this->ResetResistance(0);
	this->ResetResistanceAdd(0);
	this->ResetDataFloat(0.0f);
	
	this->SetStaminaReduction(0.0f);
	this->SetForceUpgrade(false);

	this->SetArmorSetBonus(false);

	this->SetElementalAttribute(ELEMENTAL_ATTRIBUTE_NONE);
	this->SetElementalPattern(0);

	this->GetPentagramOption()->Reset();
	this->GetPentagramJewelOption()->Reset();

	this->ResetBaseStat(0);
	this->ResetSpecializationStat(0);
	this->ResetAncientEffect(0);

	for ( MajesticDataMap::iterator itr = this->majestic_data_map.begin(); itr != this->majestic_data_map.end(); ++itr )
	{
		itr->second.clear();
	}

	this->majestic_data_map.clear();

	do
	{
		item_update = false;

		this->ResetAddStat(0);
		this->SetFullAncient(false);

		this->CalculateItemStat();
		this->CalculateAncientItemStat();
		this->SetAncientMastery(true);
		this->ApplyBuffStatEffect();
		this->ApplySkillTree(true);
		this->ApplyMajesticSkillTree(true);
		this->ApplyMajesticStats(true);
		this->CalculateSocketItem(true);
		this->ApplyMasteryBonus(true);
		this->ApplyWingGradedOption(true);
		this->ApplyWingErrtelOption(true);
		this->ApplyEarringOption(EARRING_01, true);
		this->ApplyEarringOption(EARRING_02, true);
		this->ApplyEarringBonus(true);
		
		sMuunSystem->CalcMuunOption(this, true);
		
		wear_inventory_loop(i)
		{
			if ( !this->GetInventory()->GetItem(i)->IsItem() )
			{
				continue;
			}

			if ( !this->GetInventory()->GetItem(i)->IsValid() )
			{
				continue;
			}

			this->GetInventory()->GetItem(i)->SetValid(this->ValidItem(this->GetInventory()->GetItem(i)));

			if ( !this->GetInventory()->GetItem(i)->IsValid() )
			{
				item_update = true;
			}
		}

		if ( this->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsItem() && this->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsValid() )
		{
			this->GetInventory()->GetItem(PENTAGRAM_SLOT)->SetValid(this->ValidItem(this->GetInventory()->GetItem(PENTAGRAM_SLOT)));

			if ( !this->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsValid() )
			{
				item_update = true;
			}
		}

		if (this->GetInventory()->GetItem(EARRING_01)->IsItem() && this->GetInventory()->GetItem(EARRING_01)->IsValid())
		{
			this->GetInventory()->GetItem(EARRING_01)->SetValid(this->ValidItem(this->GetInventory()->GetItem(EARRING_01)));

			if (!this->GetInventory()->GetItem(EARRING_01)->IsValid())
			{
				item_update = true;
			}
		}

		if (this->GetInventory()->GetItem(EARRING_02)->IsItem() && this->GetInventory()->GetItem(EARRING_02)->IsValid())
		{
			this->GetInventory()->GetItem(EARRING_02)->SetValid(this->ValidItem(this->GetInventory()->GetItem(EARRING_02)));

			if (!this->GetInventory()->GetItem(EARRING_02)->IsValid())
			{
				item_update = true;
			}
		}

		use_inventory_loop(i)
		{
			Item* pItem = this->GetInventory()->GetItem(i);

			if (!pItem || !pItem->IsItem() || !pItem->IsInventoryActive() || !pItem->IsValid())
			{
				continue;
			}

			if (pItem->IsMount() || pItem->GetItem() == PET_DARK_RAVEN)
			{
				pItem->SetValid(this->ValidItem(pItem));

				if (!pItem->IsValid())
				{
					item_update = true;
				}
			}
		}
	}
	while ( item_update );

	/*
	0x00A928D8 -> Calculate item
	0x00576570 -> print item required stat
	0x00A934FE -> Calculate item required stat
	
	
	0x00CC2104
	0x00CCE557
	0x00CC016C
	0x00CD1D22 -> Summoner WIZ
	0x00CD250E -> Summoner CURSE
	0x00CBE8C9
	0x00865D08 -> ItemToolTipText
	0x0085D1F9 -> Item Buffer
	0x00A93F7E -> Assign item option
	0x0085D34C
	*/

	sCharacterBase->CalculateStatFormula(this);
	sCharacterBase->CalculateSpecialization(this);

	this->CalculateMaxHP();
	this->CalculateMaxMana();
	this->CalculateMaxStamina();

	this->DarkSpiritCalculate();

	if (Wings->IsValid())
	{
		int32 physical_option = Wings->GetOptionValue(ItemOption::PHYSICAL_DAMAGE);
		int32 wizardry_option = Wings->GetOptionValue(ItemOption::WIZARDRY_DAMAGE);
		int32 curse_option = Wings->GetOptionValue(ItemOption::CURSE_DAMAGE);

		physical_option -= physical_option * Wings->GetDurabilityCurrentStatus();
		wizardry_option -= wizardry_option * Wings->GetDurabilityCurrentStatus();
		curse_option -= curse_option * Wings->GetDurabilityCurrentStatus();

		if (physical_option < 0)
		{
			physical_option = 0;
		}

		if (wizardry_option < 0)
		{
			wizardry_option = 0;
		}

		if (curse_option < 0)
		{
			curse_option = 0;
		}

		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, physical_option);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, physical_option);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, physical_option);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, physical_option);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, wizardry_option);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, wizardry_option);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, curse_option);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, curse_option);

		if (Wings->GetItem() == ITEMGET(12, 268))
		{
			int32 value = 100 + (Wings->GetLevel() * 5);

			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		}
		else if (Wings->GetItem() == ITEMGET(12, 480))
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 300);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 300);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 300);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 300);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 300);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 300);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, 300);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, 300);
		}
		else if (Wings->IsWingLvl4())
		{
			int32 value = sFormulaMgr->GetOrCreateValue(5, 0, Wings->GetLevel());

			if (Wings->IsWingType(1))
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			}

			if (Wings->IsWingType(2))
			{
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			}

			if (Wings->IsWingType(3))
			{
				this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
				this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
			}
		}
	}

	if (Right->IsValid())
	{
		int32 add_damage = Right->GetOptionValue(ItemOption::PHYSICAL_DAMAGE);
		int32 add_wizardry_damage = Right->GetOptionValue(ItemOption::WIZARDRY_DAMAGE);
		int32 add_curse_damage = Right->GetOptionValue(ItemOption::CURSE_DAMAGE);
		add_damage -= add_damage * Right->GetDurabilityCurrentStatus();
		add_wizardry_damage -= add_wizardry_damage * Right->GetDurabilityCurrentStatus();
		add_curse_damage -= add_curse_damage * Right->GetDurabilityCurrentStatus();

		int32 min_damage = Right->GetMinDamage();
		int32 max_damage = Right->GetMaxDamage();

		min_damage -= min_damage * Right->GetDurabilityCurrentStatus();
		max_damage -= max_damage * Right->GetDurabilityCurrentStatus();

		if (min_damage < 0)
		{
			min_damage = 0;
		}

		if (max_damage < 0)
		{
			max_damage = 0;
		}

		if (add_damage < 0)
		{
			add_damage = 0;
		}

		if (add_wizardry_damage < 0)
		{
			add_wizardry_damage = 0;
		}

		if (add_curse_damage < 0)
		{
			add_curse_damage = 0;
		}

		if (Right->GetKind2() == ItemKind::STAFF || Right->GetKind2() == ItemKind::STICK || Right->GetKind2() == ItemKind::BOOK || Right->GetKind2() == ItemKind::RUNIC_MACE || Right->GetKind2() == ItemKind::MAGIC_GUN)
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, min_damage + add_damage);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, max_damage + add_damage);
		}
		else
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, min_damage + add_damage);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, max_damage + add_damage);
		}

		if (Right->GetKind2() == ItemKind::MAGIC_SWORD)
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_damage);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_damage);
		}
		else
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_wizardry_damage);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_wizardry_damage);
		}

		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, add_curse_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, add_curse_damage);

		///- APPLY EXCELLENT
		///- APPLY UNK
		///- APPLY 380
		///- APPLY HARMONY

		this->SetForceUpgrade(Right->IsForceUpgrade());
	}

	if (Left->IsValid())
	{
		int32 add_damage = Left->GetOptionValue(ItemOption::PHYSICAL_DAMAGE);
		int32 add_wizardry_damage = Left->GetOptionValue(ItemOption::WIZARDRY_DAMAGE);
		int32 add_curse_damage = Left->GetOptionValue(ItemOption::CURSE_DAMAGE);
		add_damage -= add_damage * Left->GetDurabilityCurrentStatus();
		add_wizardry_damage -= add_wizardry_damage * Left->GetDurabilityCurrentStatus();
		add_curse_damage -= add_curse_damage * Left->GetDurabilityCurrentStatus();

		int32 min_damage = Left->GetMinDamage();
		int32 max_damage = Left->GetMaxDamage();

		min_damage -= min_damage * Left->GetDurabilityCurrentStatus();
		max_damage -= max_damage * Left->GetDurabilityCurrentStatus();

		if (min_damage < 0)
		{
			min_damage = 0;
		}

		if (max_damage < 0)
		{
			max_damage = 0;
		}

		if (add_damage < 0)
		{
			add_damage = 0;
		}

		if (add_wizardry_damage < 0)
		{
			add_wizardry_damage = 0;
		}

		if (add_curse_damage < 0)
		{
			add_curse_damage = 0;
		}

		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, min_damage + add_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, max_damage + add_damage);

		if (Left->GetKind2() == ItemKind::MAGIC_SWORD)
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_damage);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_damage);
		}
		else
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_wizardry_damage);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_wizardry_damage);
		}

		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, add_curse_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, add_curse_damage);

		///- APPLY EXCELLENT

		if (Left->GetKind2() == ItemKind::SHIELD)
		{
			/*
			v22 = GetItemInfo(pItem);
              *((_WORD *)v62 + 192) += sub_CC3732(v114, (_WORD *)(v22 + 672), (int **)&pItem, 3);
              v23 = GetItemInfo(pItem);
              *((_WORD *)v62 + 193) += sub_CC3732(v109, (_WORD *)(v23 + 674), (int **)&pItem, 3);
              v24 = GetItemInfo(pItem);
              *((_WORD *)v62 + 194) += sub_CC3732(v106, (_WORD *)(v24 + 676), (int **)&pItem, 3);
			*/
		}
		else
		{
			/*
			v25 = i;
              v26 = GetItemInfo(pItem);
              LOWORD(v114) = sub_CC3732(v114, (_WORD *)(v26 + 672), (int **)&pItem, v25);
              v27 = i;
              v28 = GetItemInfo(pItem);
              LOWORD(v109) = sub_CC3732(v109, (_WORD *)(v28 + 674), (int **)&pItem, v27);
              v29 = i;
              v30 = GetItemInfo(pItem);
              LOWORD(v106) = sub_CC3732(v106, (_WORD *)(v30 + 676), (int **)&pItem, v29);
			*/
		}

		///- APPLY 380
		///- APPLY HARMONY

		if (Left->GetKind2() == ItemKind::BOOK)
		{
			if (Left->GetLevel() <= 9)
			{
				this->SetBookHitCount(5);
			}
			else if (Left->GetLevel() <= 13)
			{
				this->SetBookHitCount(7);
			}
			else
			{
				this->SetBookHitCount(9);
			}
		}

		if (Left->GetKind2() == ItemKind::SHIELD)
		{
			this->IncreaseData(PLAYER_DATA_SHIELD_PROTECTION_VALUE, Left->GetDefense() * 10);
		}
	}

	

	/*
	APPLY PENDANT EXCELLENT VALUES
	*/

	

	if( Pentagram->IsItem() && Pentagram->IsPentagramItem() && Pentagram->IsValid() )
	{
		this->SetElementalAttribute(Pentagram->GetSocketBonus() % 16);
		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, Pentagram->GetDefense());
		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, Pentagram->GetMinDamage());
		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, Pentagram->GetMaxDamage());

		if ( (this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_FIRE && this->HasBuff(BUFF_FIRE_TOWER)) ||
			 (this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_WATER && this->HasBuff(BUFF_WATER_TOWER)) ||
			 (this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_EARTH && this->HasBuff(BUFF_EARTH_TOWER)) ||
			 (this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_WIND && this->HasBuff(BUFF_WIND_TOWER)) ||
			 (this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_DARK && this->HasBuff(BUFF_DARKNESS_TOWER)) )
		{
			this->GetPentagramOption()->IncreaseMulPentagramDamage(5);
			this->GetPentagramOption()->IncreaseMulPentagramDefense(5);
		}

		int32 base_imprint = 9 + (Pentagram->GetLevel() * 3);
		this->SetData(PLAYER_DATA_ELEMENTAL_IMPRINT, base_imprint);

		SOCKET_SLOT_LOOP(i)
		{
			if (Pentagram->GetSocket(i) == SOCKET_SLOT_EMPTY || Pentagram->GetSocket(i) == SOCKET_SLOT_NONE)
			{
				continue;
			}

			auto pErrtelData = sPentagramSystem->GetPentagramJewelInfo(this, Pentagram->GetSocket(i), PENTAGRAM_JEWEL_TYPE_INVENTORY);

			if (!pErrtelData)
			{
				continue;
			}

			if (pErrtelData->GetItemType() == 222 || pErrtelData->GetItemType() == 232 || pErrtelData->GetItemType() == 242 || pErrtelData->GetItemType() == 252)
			{
				continue;
			}

			uint8 errtel_rank = pErrtelData->GetRank();

			float add_value = 0;
			if (errtel_rank >= 1 && errtel_rank <= 5)
			{
				add_value = 0.05f + (0.05f * errtel_rank);
			}

			if (i == 4)
			{
				this->IncreaseData(PLAYER_DATA_ELEMENTAL_IMPRINT, std::floor((add_value + 0.25f) * base_imprint));
			}
			else
			{
				this->IncreaseData(PLAYER_DATA_ELEMENTAL_IMPRINT, std::floor((add_value + 0.1f) * base_imprint));
			}
		}
	}

	this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, base_data->GetAutomaticPowerRecovery(POWER_LIFE));
	this->IncreaseDataFloat(PLAYER_FLOAT_MP_RECOVERY_RATE, base_data->GetAutomaticPowerRecovery(POWER_MANA));
	this->IncreaseDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE, base_data->GetAutomaticPowerRecovery(POWER_STAMINA));

	if ( Right->IsValid() && Left->IsValid() )
	{
		if ( (Left->GetItem() == ITEMGET(4, 7) || Left->GetItem() == ITEMGET(4, 15)) && Left->GetLevel() >= 1 )
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN) * (2 * Left->GetLevel() + 1) / 100);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX) * (2 * Left->GetLevel() + 1) / 100);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 1);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 1);
		}
	}

	

	this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, this->GetAleEffect(ALE_SPEED));
	this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, this->GetAleEffect(ALE_SPEED));

	bool right = false;
	bool left = false;

	if ( Right->IsValid() && Right->GetItem() != ITEMGET(4, 7) && Right->GetItem() != ITEMGET(4, 15) && Right->GetItem() >= ITEMGET(0, 0) && Right->GetItem() < ITEMGET(6, 0) )
	{
		right = true;
	}
	if ( Left->IsValid() && Left->GetItem() != ITEMGET(4, 7) && Left->GetItem() != ITEMGET(4, 15) && Left->GetItem() >= ITEMGET(0, 0) && Left->GetItem() < ITEMGET(6, 0) )
	{
		left = true;
	}

	if ( right && left )
	{
		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, (Right->GetAttackSpeed() + Left->GetAttackSpeed()) / 2);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, (Right->GetAttackSpeed() + Left->GetAttackSpeed()) / 2);
	}
	else if ( right )
	{
		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, Right->GetAttackSpeed());
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, Right->GetAttackSpeed());
	}
	else if ( left )
	{
		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, Left->GetAttackSpeed());
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, Left->GetAttackSpeed());
	}

	if ( Gloves->IsValid() )
	{
		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, Gloves->GetAttackSpeed());
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, Gloves->GetAttackSpeed());
	}

	wear_inventory_loop(i)
	{
		const auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsValid() || pItem->IsExpired())
		{
			continue;
		}

		this->IncreaseIntData(UNIT_INT_DEFENSE, pItem->GetDefense());
		this->IncreaseIntData(UNIT_INT_DEFENSE, pItem->GetOptionValue(ItemOption::DEFENSE));

		this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, pItem->GetDefenseSuccessRate());
		this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, pItem->GetOptionValue(ItemOption::DEFENSE_SUCCESS_RATE));

		this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, pItem->CriticalDamageRate());

		this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * pItem->GetOptionValue(ItemOption::MAX_MANA) / 100, false);
		this->PowerIncreseAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * pItem->GetOptionValue(ItemOption::MAX_STAMINA) / 100, false);
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, pItem->GetOptionValue(ItemOption::HP_RECOVERY));
	}

	Item const* pDarkHorse = this->GetActiveInventoryItem(PET_DARK_HORSE);
	
	if (pDarkHorse && pDarkHorse->IsValid() && !pDarkHorse->IsExpired())
	{
		this->IncreaseIntData(UNIT_INT_DEFENSE, this->GetOnlyEquipmentStat(AGILITY) / 20 + pDarkHorse->GetOptionValue(ItemOption::DARK_HORSE_DEFENSE));
	}

	this->SetArmorSetBonus(true);

	for ( uint8 i = HELM; i <= BOOTS; i++ )
	{
		if ( i == HELM && this->GetClass() == Character::MAGIC_GLADIATOR )
			continue;

		if ( i == GLOVES && this->GetClass() == Character::RAGE_FIGHTER )
			continue;

		if ( !this->GetInventory()->GetItem(i)->IsItem() || !this->GetInventory()->GetItem(i)->IsValid() )
		{
			this->SetArmorSetBonus(false);
			break;
		}
	}

	int32 level_10_count = 0;
	int32 level_11_count = 0;
	int32 level_12_count = 0;
	int32 level_13_count = 0;
	int32 level_14_count = 0;
	int32 level_15_count = 0;

	if ( this->IsArmorSetBonus() )
	{
		uint16 set_index = Armor->GetItemIndex();

		if ( this->GetClass() == Character::MAGIC_GLADIATOR )
		{
			this->SetArmorSetBonus(Armor->IsExclusiveClass(this->GetClass()) &&
								   Pants->IsExclusiveClass(this->GetClass()) &&
								   Gloves->IsExclusiveClass(this->GetClass()) &&
								   Boots->IsExclusiveClass(this->GetClass()));
		}
		if ( this->GetClass() == Character::RAGE_FIGHTER )
		{
			this->SetArmorSetBonus(Helm->IsExclusiveClass(this->GetClass()) &&
								   Armor->IsExclusiveClass(this->GetClass()) &&
								   Pants->IsExclusiveClass(this->GetClass()) &&
								   Boots->IsExclusiveClass(this->GetClass()));
		}

		if ( this->GetClass() == Character::MAGIC_GLADIATOR || this->GetClass() == Character::RAGE_FIGHTER )
		{
			level_15_count++;
		}

		for ( uint8 i = HELM; i <= BOOTS; i++ )
		{
			if ( this->GetClass() == Character::MAGIC_GLADIATOR && i == HELM )
				continue;

			if ( this->GetClass() == Character::RAGE_FIGHTER && i == GLOVES )
				continue;

			if ( set_index != this->GetInventory()->GetItem(i)->GetItemIndex() )
			{
				this->SetArmorSetBonus(false);
			}

			if ( this->GetInventory()->GetItem(i)->GetLevel() == 15 )
			{
				level_15_count++;
			}
			else if ( this->GetInventory()->GetItem(i)->GetLevel() == 14 )
			{
				level_14_count++;
			}
			else if ( this->GetInventory()->GetItem(i)->GetLevel() == 13 )
			{
				level_13_count++;
			}
			else if ( this->GetInventory()->GetItem(i)->GetLevel() == 12 )
			{
				level_12_count++;
			}
			else if ( this->GetInventory()->GetItem(i)->GetLevel() == 11 )
			{
				level_11_count++;
			}
			else if ( this->GetInventory()->GetItem(i)->GetLevel() == 10 )
			{
				level_10_count++;
			}
		}

		if ( this->IsArmorSetBonus() )
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, this->GetIntData(UNIT_INT_DEFENSE_RATE) / 10);
		}
	}

	if ( (level_15_count + level_14_count + level_13_count + level_12_count + level_11_count + level_10_count) >= 5 && this->IsArmorSetBonus() )
	{
		if ( level_15_count == 5 )
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 30);
		}
		else if ( level_14_count == 5 || (level_14_count + level_15_count) == 5 )
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 25);
		}
		else if ( level_13_count == 5 || (level_13_count + level_14_count + level_15_count) == 5 )
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 20);
		}
		else if ( level_12_count == 5 || (level_12_count + level_13_count + level_14_count + level_15_count) == 5 )
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 15);
		}
		else if ( level_11_count == 5 || (level_11_count + level_12_count + level_13_count + level_14_count + level_15_count) == 5 )
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 10);
		}
		else if ( level_10_count == 5 || (level_10_count + level_11_count + level_12_count + level_13_count + level_14_count + level_15_count) == 5)
		{
			this->PercentIntData(UNIT_INT_DEFENSE, 5);
		}
	}

	//this->SetIntData(UNIT_INT_DEFENSE, this->GetIntData(UNIT_INT_DEFENSE) * 10 / 20);

	this->ApplyBuffNonStatEffect();

	this->CalculateExeItem(); 
	this->Calculate380Item();
	this->CalculateHarmonyItem();

	if (Earring01->IsValid())
	{
		int32 min_damage = Earring01->GetMinDamage();
		int32 max_damage = Earring01->GetMaxDamage();

		min_damage -= min_damage * Earring01->GetDurabilityCurrentStatus();
		max_damage -= max_damage * Earring01->GetDurabilityCurrentStatus();

		if (min_damage < 0)
		{
			min_damage = 0;
		}

		if (max_damage < 0)
		{
			max_damage = 0;
		}

		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, max_damage);

		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, Earring01->GetDefense());

		this->ApplyEarringOption(EARRING_01, false);
	}

	if (Earring02->IsValid())
	{
		int32 min_damage = Earring02->GetMinDamage();
		int32 max_damage = Earring02->GetMaxDamage();

		min_damage -= min_damage * Earring02->GetDurabilityCurrentStatus();
		max_damage -= max_damage * Earring02->GetDurabilityCurrentStatus();

		if (min_damage < 0)
		{
			min_damage = 0;
		}

		if (max_damage < 0)
		{
			max_damage = 0;
		}

		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, max_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, min_damage);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, max_damage);

		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, Earring02->GetDefense());

		this->ApplyEarringOption(EARRING_02, false);
	}

	this->ApplyEarringBonus(false);

	this->CalculateSocketItem(false);

	this->CalculateAncientItem();
		
	this->ApplySkillTree(false);

	auto pBerserker = this->GetBuffByGroup(BUFF_BERSERKER);
	if (pBerserker)
	{
		float power_value = 0;

		if (Left && Left->IsValid() && Left->GetKind2() == ItemKind::BOOK && Left->GetCursePower() > 0)
		{
			float power = Left->GetCursePower() / 100.0f;
			power -= power * Left->GetDurabilityCurrentStatus();

			if (power < 0)
			{
				power = 0;
			}

			power_value = this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX) * power;
		}

		float add_power = (this->GetEnergy() / 3.5f) + (power_value - (power_value * 0.86f))*(power_value * 0.002f);
		if (add_power < 0)
		{
			add_power = 0;
		}


		float damage_add = (this->GetEnergy() / 3.5f) + add_power;
		damage_add += this->MagicGetValue(469);

		if (damage_add < 0)
		{
			damage_add = 0;
		}

		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, damage_add);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, damage_add);

		if (pBerserker->GetID() == 81 || pBerserker->GetID() == 150 || pBerserker->GetID() == 152)
		{
			float defense_decrease = (this->GetEnergy() / 200) * 0.01f;
			this->ReduceIntData(UNIT_INT_DEFENSE, (this->GetAgility() / 3) * defense_decrease);
		}
	}

	auto pDarknesss = this->GetBuffByGroup(301);
	if (pDarknesss)
	{
		float power_value = 0;

		if (Right && Right->IsValid() && Right->GetKind2() == ItemKind::STICK && Right->GetMagicPower() > 0)
		{
			float power = Right->GetCursePower() / 100.0f;
			power -= power * Right->GetDurabilityCurrentStatus();

			if (power < 0)
			{
				power = 0;
			}

			power_value = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX) * power;
		}

		float add_power = (power_value - (power_value * 0.91f))*(power_value * 0.002f);
		if (add_power < 0)
		{
			add_power = 0;
		}

		float damage_add = (this->GetEnergy() / 3.5f) + add_power;
		damage_add += this->MagicGetValue(770);

		if (damage_add < 0)
		{
			damage_add = 0;
		}

		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, damage_add);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, damage_add);

		float defense_increase = this->GetEnergy() / 5;
		defense_increase += this->MagicGetValue(771);

		this->IncreaseIntData(UNIT_INT_DEFENSE, defense_increase);
	}

	this->ApplyMajesticStats(false);

	this->ApplyStats1();

	this->SetAncientMastery(false);
	this->ApplyMajesticSkillTree(false);

	sPentagramSystem->ApplyPentagramOption(this);
	sPentagramSystem->ApplyErrtelOption(this);
	sMuunSystem->CalcMuunOption(this, false);

	this->ApplyMasteryBonus(false);
	this->ApplyWingGradedOption(false);
	this->ApplyWingErrtelOption(false);
		
	if ( this->GetData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT) > sGameServer->GetShieldDefenseRateLimit() )
	{
		this->SetData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT, sGameServer->GetShieldDefenseRateLimit());
	}

	if ( Left->IsValid() && Left->GetKind2() == ItemKind::SHIELD )
	{
		this->IncreaseData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE, Left->GetDefense() * this->GetData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT) / 100);

		this->IncreaseIntData(UNIT_INT_DEFENSE, this->GetData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE));
		this->IncreaseIntData(UNIT_INT_DEFENSE, this->GetEffect(BUFF_OPTION_SOLID_PROTECTION_SHIELD_DEFENSE));

		if (Left->GetKind2() == ItemKind::SHIELD && (Left->GetKind1() == ItemKind1::MASTERY_ARMOR_1 || Left->GetKind1() == ItemKind1::MASTERY_ARMOR_2))
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE_ADD, Left->GetDefense() * Left->GetMasteryShieldDefense());
		}
	}

	this->CalculateMaxShield();
	
	for ( uint8 i = 0; i < Element::MAX; i++ )
	{
		this->IncreaseResistance(i, GET_MAX_RESISTANCE(Ring01->GetResistance(i), Ring02->GetResistance(i), Pendant->GetResistance(i)));
	}

	if (this->GetClass() == Character::SLAYER)
	{
		if (Left && Left->IsValid() && Left->GetKind2() == ItemKind::SHORT_SWORD)
		{
			if (Right && Right->IsValid() && Right->GetKind2() == ItemKind::SHORT_SWORD)
			{
				int32 add_value = 0;
				if (Right->GetItem() == Left->GetItem())
				{
					add_value = 65;
				}
				else
				{
					add_value = 55;
				}

				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN) / 100);
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX) / 100);
			}
			else
			{
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN));
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
			}
		}
		else
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
		}
	}
	else if (this->GetClass() == Character::RUNE_WIZARD)
	{
		if (Right && Right->IsValid())
		{
			if (Right->GetKind2() == ItemKind::RUNIC_MACE)
			{
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN));
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
			}
			else
			{
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
			}
		}
		else
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
		}
	}
	else if (this->GetClass() == Character::GROW_LANCER || this->GetClass() == Character::GUN_CRUSHER)
	{
		if (Left && Left->IsValid() && (Left->GetKind1() == ItemKind1::WEAPON || Left->GetKind1() == ItemKind1::MASTERY_WEAPON))
		{
			if (Right && Right->IsValid() && (Right->GetKind1() == ItemKind1::WEAPON || Right->GetKind1() == ItemKind1::MASTERY_WEAPON))
			{
				int32 add_value = 0;
				if (Right->GetItem() == Left->GetItem())
				{
					add_value = 65;
				}
				else
				{
					add_value = 55;
				}

				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN) / 100);
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX) / 100);
			}
			else
			{
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN));
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
			}
		}
		else
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
		}
	}
	else
	{
		if (Right && Right->IsValid() && (Right->GetKind2() == ItemKind::CROSSBOW || Right->GetKind2() == ItemKind::BOW))
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
		}
		else if (!Right || !Right->IsValid())
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
		}
		else if (Right->GetItem() < ITEMGET(6, 0))
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX));
		}
		else
		{
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN));
			this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
		}

		if (this->GetClass() == Character::DARK_KNIGHT || this->GetClass() == Character::MAGIC_GLADIATOR)
		{
			if (Left && Left->IsValid() && !Left->IsTwoHand() && Left->GetKind2() != ItemKind::SHIELD)
			{
				if (Right && Right->IsValid() && !Right->IsTwoHand() && Right->GetKind2() != ItemKind::SHIELD)
				{
					int32 add_value = 0;
					if (Right->GetItem() == Left->GetItem())
					{
						add_value = 65;
					}
					else
					{
						add_value = 55;
					}

					this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN) / 100);
					this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX) / 100 + add_value * this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX) / 100);
				}
			}
		}
		else if (this->GetClass() == Character::RAGE_FIGHTER)
		{
			if (Right && Right->IsValid() && Right->GetItem() >= ITEMGET(0, 0) && Right->GetItem() < ITEMGET(3, 0) &&
				Left && Left->IsValid() && Left->GetItem() >= ITEMGET(0, 0) && Left->GetItem() < ITEMGET(3, 0))
			{
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, 60 * (this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN) + this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN)) / 100);
				this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, 65 * (this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX) + this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX)) / 100);
			}
		}
	}

	this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetAleEffect(ALE_DAMAGE));
	this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetAleEffect(ALE_DAMAGE));
	this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, this->GetAleEffect(ALE_DAMAGE));
	this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, this->GetAleEffect(ALE_DAMAGE));
	this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, this->GetAleEffect(ALE_DAMAGE));
	this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, this->GetAleEffect(ALE_DAMAGE));

	for ( uint8 i = 0; i < POWER_MAX; i++ )
	{
		if ( this->PowerGet(i) > this->PowerGetTotal(i) )
		{
			this->PowerSet(i, this->PowerGetTotal(i));
		}
	}

	for ( uint8 i = 0; i < POWER_MAX; i++ )
	{
		if ( this->PowerGet(i) < 0 )
		{
			this->PowerSet(i, 0);
		}
	}

	if ( this->GetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE) > static_cast<float>(sGameServer->GetExcellentDamageRateLimit()) )
	{
		this->SetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, static_cast<float>(sGameServer->GetExcellentDamageRateLimit()));
	}

	if ( this->GetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE) > static_cast<float>(sGameServer->GetCriticalDamageRateLimit()) )
	{
		this->SetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, static_cast<float>(sGameServer->GetCriticalDamageRateLimit()));
	}

	if ( this->GetIntData(UNIT_INT_DAMAGE_DECREASE) > static_cast<float>(sGameServer->GetDecreaseDamageRateLimit()) )
	{
		this->SetIntData(UNIT_INT_DAMAGE_DECREASE, static_cast<float>(sGameServer->GetDecreaseDamageRateLimit()));
	}

	if ( this->GetFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE) > static_cast<float>(sGameServer->GetReflectDamageRateLimit()) )
	{
		this->SetFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, static_cast<float>(sGameServer->GetReflectDamageRateLimit()));
	}

	if ( this->GetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE) > static_cast<float>(sGameServer->GetIgnoreDefenseRateLimit()) )
	{
		this->SetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, static_cast<float>(sGameServer->GetIgnoreDefenseRateLimit()));
	}

	int32 speed_reduction = this->GetEffect(BUFF_OPTION_ELITE_ATTACK_SPEED_REDUCTION);

	if (speed_reduction > 0)
	{
		this->ReduceIntData(UNIT_INT_ATTACK_SPEED, speed_reduction);

		if (this->GetIntData(UNIT_INT_ATTACK_SPEED) <= 0)
		{
			this->SetIntData(UNIT_INT_ATTACK_SPEED, 1);
		}

		this->ReduceIntData(UNIT_INT_MAGIC_SPEED, speed_reduction);

		if (this->GetIntData(UNIT_INT_MAGIC_SPEED) <= 0)
		{
			this->SetIntData(UNIT_INT_MAGIC_SPEED, 1);
		}
	}

	if ( this->GetIntData(UNIT_INT_ATTACK_SPEED) > sGameServer->GetMaxAttackSpeed(this->GetClass()) )
	{
		this->SetIntData(UNIT_INT_ATTACK_SPEED, sGameServer->GetMaxAttackSpeed(this->GetClass()));
	}

	if ( this->GetIntData(UNIT_INT_MAGIC_SPEED) > sGameServer->GetMaxAttackSpeed(this->GetClass()) )
	{
		this->SetIntData(UNIT_INT_MAGIC_SPEED, sGameServer->GetMaxAttackSpeed(this->GetClass()));
	}

	if (this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_HP) > 0)
	{
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_HP, this->PowerGetTotal(POWER_LIFE) / this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_HP));
	}

	if (this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP) > 0)
	{
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_MP, this->PowerGetTotal(POWER_MANA) / this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP));
	}

	if (this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_SD) > 0)
	{
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_SD, this->PowerGetTotal(POWER_SHIELD) / this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_SD));
	}

	if (this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_AG) > 0)
	{
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_AG, this->PowerGetTotal(POWER_STAMINA) / this->GetDataFloat(PLAYER_FLOAT_MONSTER_DIE_AG));
	}

	if (this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN) > this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX))
	{
		this->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX));
	}

	if (this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN) > this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX))
	{
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX));
	}

	if (this->GetIntData(UNIT_INT_CURSE_DAMAGE_MIN) > this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX))
	{
		this->SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX));
	}

	this->SendStats();
	this->UpdatePowers(POWER_LIFE, false);
	this->UpdatePowers(POWER_LIFE, true);
	this->UpdatePowers(POWER_MANA, false);
	this->UpdatePowers(POWER_MANA, true);
	
	this->LimitMaxStaminaReduction(100.0f);

	if (this->IsAdministrator())
	{
		/*if (this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN) > 0 || this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Damage Left: %d - %d", this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN), this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX));
		}*/

		if (this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN) > 0 || this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Damage: %d - %d", this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN), this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX));
		}

		if (this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN) > 0 || this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Wizardry Damage: %d - %d", this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN), this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX));
		}

		if (this->GetIntData(UNIT_INT_CURSE_DAMAGE_MIN) > 0 || this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Curse Damage: %d - %d", this->GetIntData(UNIT_INT_CURSE_DAMAGE_MIN), this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX));
		}

		if (this->GetIntData(UNIT_INT_ATTACK_RATE) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Rate: %d", this->GetIntData(UNIT_INT_ATTACK_RATE));
		}

		if (this->GetIntData(UNIT_INT_ATTACK_RATE_PVP) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Rate PvP: %d", this->GetIntData(UNIT_INT_ATTACK_RATE_PVP));
		}

		if (this->GetIntData(UNIT_INT_DEFENSE) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Defense: %d", this->GetIntData(UNIT_INT_DEFENSE));
		}

		if (this->GetIntData(UNIT_INT_DEFENSE_RATE) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Defense Rate: %d", this->GetIntData(UNIT_INT_DEFENSE_RATE));
		}

		if (this->GetIntData(UNIT_INT_DEFENSE_RATE_PVP) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Defense Rate PvP: %d", this->GetIntData(UNIT_INT_DEFENSE_RATE_PVP));
		}
	}
}

void Player::CalculateItemStat()
{
	wear_inventory_loop(i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem->IsValid() ||
			!pItem->IsAncient() ||
			pItem->GetDurability() < 1.0f)
		{
			continue;
		}

		if (pItem->GetAncientAttribute() >= 0 && pItem->GetAncientAttribute() < MAX_STAT_TYPE)
		{
			this->IncreaseAddStat(pItem->GetAncientAttribute(), pItem->GetAncientAttributeValue());
		}
		else if (pItem->GetAncientAttribute() == MAX_STAT_TYPE)
		{
			this->IncreaseAddStat(STRENGTH, pItem->GetAncientAttributeValue());
			this->IncreaseAddStat(AGILITY, pItem->GetAncientAttributeValue());
			this->IncreaseAddStat(VITALITY, pItem->GetAncientAttributeValue());
			this->IncreaseAddStat(ENERGY, pItem->GetAncientAttributeValue());

			if (this->GetClass() == Character::DARK_LORD)
			{
				this->IncreaseAddStat(LEADERSHIP, pItem->GetAncientAttributeValue());
			}
		}
	}
}

bool Player::ValidItem(Item const* item)
{
	if (!item->IsItem())
	{
		return false;
	}

	if (!item->IsPentagramItem() && item->GetDurability() < 1.0f)
	{
		return false;
	}

	const auto pItemInfo = sItemMgr->GetItem(item->GetItem());
	if (!pItemInfo)
	{
		return false;
	}

	if (!item->IsRequiredClass(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)))
	{
		return false;
	}

	for (uint8 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		if (sGameServer->IsOnlyBaseStatForEquipment())
		{
			if (pItemInfo->IsMasteryWeaponDK() && i == ENERGY && this->GetOnlyEquipmentStat(STRENGTH) >= item->GetRequiredStat(STRENGTH))
			{
				continue;
			}

			if (pItemInfo->IsMasteryWeaponDK() && i == STRENGTH && this->GetOnlyEquipmentStat(ENERGY) >= item->GetRequiredStat(ENERGY))
			{
				continue;
			}

			if (this->GetOnlyEquipmentStat(i) < item->GetRequiredStat(i))
			{
				return false;
			}
		}
		else
		{
			if (pItemInfo->IsMasteryWeaponDK() && i == ENERGY && this->GetTotalStat(STRENGTH) >= item->GetRequiredStat(STRENGTH))
			{
				continue;
			}

			if (pItemInfo->IsMasteryWeaponDK() && i == STRENGTH && this->GetTotalStat(ENERGY) >= item->GetRequiredStat(ENERGY))
			{
				continue;
			}

			if (this->GetTotalStat(i) < item->GetRequiredStat(i))
			{
				return false;
			}
		}
	}

	if (this->GetTotalLevel() < item->GetRequiredLevel())
	{
		return false;
	}

	return true;
}

void Player::Calculate380Item()
{
	wear_inventory_loop(i)
	{
		const auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsValid() || !pItem->Get380())
		{
			continue;
		}

		const auto pItemAddOption = sItemMgr->GetItemAddOption(pItem->GetItem());

		if (!pItemAddOption)
		{
			continue;
		}

		for (uint8 h = 0; h < MAX_ITEM_ADD_OPTION_EFFECT; ++h)
		{
			const auto value = pItemAddOption->GetValue(h);

			switch (pItemAddOption->GetEffect(h))
			{
			case EFFECT_380_ATTACK_RATE_PVP:
				this->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, value);
				break;

			case EFFECT_380_ATTACK_PVP:
				this->IncreaseData(PLAYER_DATA_380_PVP_DAMAGE, value);
				break;

			case EFFECT_380_DEFENSE_RATE_PVP:
				this->IncreaseIntData(UNIT_INT_DEFENSE_RATE_PVP, value);
				break;

			case EFFECT_380_DEFENSE_PVP:
				this->IncreaseIntData(UNIT_INT_DEFENSE_ADD_PVP, value);
				break;

			case EFFECT_380_INCREASE_MAX_LIFE:
				this->PowerIncreseAdd(POWER_LIFE, value, false);
				break;

			case EFFECT_380_INCREASE_MAX_SHIELD:
				this->PowerIncreseAdd(POWER_SHIELD, value, false);
				break;

			case EFFECT_380_SHIELD_REFILL_ON:
				this->IncreaseData(PLAYER_DATA_SHIELD_REFILL_ON, value);
				break;

			case EFFECT_380_SHIELD_REFILL_ADD:
				this->IncreaseDataFloat(PLAYER_FLOAT_SD_RECOVERY_RATE, value);
				break;
			}
		}
	}
}

void Player::CalculateHarmonyItem()
{
	wear_inventory_loop(i)
	{
		const auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsValid() || !pItem->IsHarmonyUpgrade())
		{
			continue;
		}

		const auto option = pItem->GetHarmonyOption();
		const auto level = pItem->GetHarmonyLevel();

		if (!option || level >= MAX_ITEM_HARMONY_LEVEL)
		{
			continue;
		}

		if (pItem->GetLevel() < level)
		{
			continue;
		}

		const auto type = pItem->GetHarmonyItem();

		const auto pHarmonyData = sItemMgr->GetHarmony(type, option);

		if (!pHarmonyData)
		{
			pItem->SetHarmony(HARMONY_EFFECT_NONE);
			pItem->AlterDurability(3, 0.0f);
			continue;
		}

		const auto value = pHarmonyData->GetValue(level);

		switch (type)
		{
		case HARMONY_ITEM_WEAPON:
			switch (option)
			{
			case HARMONY_WEAPON_ATTACK_DAMAGE_MIN:
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				break;

			case HARMONY_WEAPON_ATTACK_DAMAGE_MAX:
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
				break;

			case HARMONY_WEAPON_ATTACK_DAMAGE:
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
				break;

			case HARMONY_WEAPON_CRITICAL_DAMAGE:
				this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
				break;

			case HARMONY_WEAPON_SKILL_DAMAGE:
				this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
				break;

			case HARMONY_WEAPON_ATTACK_RATE_PVP:
				this->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, value);
				break;

			case HARMONY_WEAPON_DECREASE_SHIELD_RATE:
				this->IncreaseData(PLAYER_DATA_DECREASE_DAMAGE_TO_SD_RATE, value);
				break;

			case HARMONY_WEAPON_IGNORE_SHIELD_RATE:
				this->IncreaseData(PLAYER_DATA_IGNORE_SD_RATE, value);
				break;
			}
			break;

		case HARMONY_ITEM_STAFF:
			switch (option)
			{
			case HARMONY_STAFF_MAGIC_DAMAGE:
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
				this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
				break;

			case HARMONY_STAFF_SKILL_DAMAGE:
				this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
				break;

			case HARMONY_STAFF_CRITICAL_DAMAGE:
				this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
				break;

			case HARMONY_STAFF_DECREASE_SHIELD_RATE:
				this->IncreaseData(PLAYER_DATA_DECREASE_DAMAGE_TO_SD_RATE, value);
				break;

			case HARMONY_STAFF_ATTACK_RATE_PVP:
				this->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, value);
				break;

			case HARMONY_STAFF_IGNORE_SHIELD_RATE:
				this->IncreaseData(PLAYER_DATA_IGNORE_SD_RATE, value);
				break;
			}
			break;

		case HARMONY_ITEM_DEFENSE:
			switch (option)
			{
			case HARMONY_DEFENSE_DEFENSE:
				this->IncreaseIntData(UNIT_INT_DEFENSE, value);
				break;

			case HARMONY_DEFENSE_MAX_STAMINA:
				this->PowerIncreseAdd(POWER_STAMINA, value, false);
				break;

			case HARMONY_DEFENSE_MAX_LIFE:
				this->PowerIncreseAdd(POWER_LIFE, value, false);
				break;

			case HARMONY_DEFENSE_LIFE_RECOVERY:
				this->IncreaseData(PLAYER_DATA_HP_RECOVERY_ADD, value);
				break;

			case HARMONY_DEFENSE_MANA_RECOVERY:
				this->IncreaseData(PLAYER_DATA_MP_RECOVERY_ADD, value);
				break;

			case HARMONY_DEFENSE_DEFENSE_RATE_PVP:
				this->IncreaseIntData(UNIT_INT_DEFENSE_RATE_PVP, value);
				break;

			case HARMONY_DEFENSE_DAMAGE_DECREASE:
				this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, value);
				break;

			case HARMONY_DEFENSE_INCREASE_SHIELD_RATE:
				this->IncreaseData(PLAYER_DATA_INCREASE_DAMAGE_TO_SD_RATE, value);
				break;
			}
			break;
		}
	}
}

void Player::CalculateExeItem()
{
	wear_inventory_loop(i)
	{
		auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem->IsValid() || !pItem->IsExcellent())
		{
			continue;
		}

		auto pItemInfo = sItemMgr->GetItem(pItem->GetItem());
		if (!pItemInfo)
		{
			continue;
		}

		auto excellent_type = pItem->GetExcellentType();

		switch (excellent_type)
		{
		case 0:
			if (pItem->GetExe() & ExeOption::INCREASE_ZEN_AFTER_KILL_MONSTER)
			{
				this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 30);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_DEFENSE_SUCCESS_RATE)
			{
				this->PercentIntData(UNIT_INT_DEFENSE_RATE, 10);
			}

			if (pItem->GetExe() & ExeOption::DAMAGE_REFLECT)
			{
				this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, 5);
			}

			if (pItem->GetExe() & ExeOption::DAMAGE_DECREASE)
			{
				this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, 4);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_MAX_MANA)
			{
				this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * 4 / 100, false);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_MAX_HP)
			{
				this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * 4 / 100, false);
			}
			break;

		case 1:
			if (pItem->GetExe() & ExeOption::INCREASE_MANA_RECOVERY_AFTER_KILL_MONSTER)
			{
				this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP, 8);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_HP_RECOVERY_AFTER_KILL_MONSTER)
			{
				this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_HP, 8);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_ATTACK_SPEED)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 7);
				this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 7);
			}

			if (pItem->GetExe() & ExeOption::INCREASE_DAMAGE_PER_LEVEL)
			{
				if (pItem->GetKind2() == ItemKind::STAFF || pItem->GetKind2() == ItemKind::STICK || pItem->GetKind2() == ItemKind::RUNIC_MACE || pItem->GetKind2() == ItemKind::WIZARD_PENDANT || pItem->GetKind2() == ItemKind::MAGIC_GUN)
				{
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, this->GetTotalLevel() / 20);
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, this->GetTotalLevel() / 20);
				}
				else if (pItem->GetKind2() == ItemKind::BOOK)
				{
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, this->GetTotalLevel() / 20);
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, this->GetTotalLevel() / 20);
				}
				else
				{
					if (i == WEAPON_01)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, this->GetTotalLevel() / 20);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, this->GetTotalLevel() / 20);
					}
					else if (i == WEAPON_02)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, this->GetTotalLevel() / 20);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, this->GetTotalLevel() / 20);
					}
					else
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, this->GetTotalLevel() / 20);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, this->GetTotalLevel() / 20);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, this->GetTotalLevel() / 20);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, this->GetTotalLevel() / 20);
					}
				}
			}

			if (pItem->GetExe() & ExeOption::INCREASE_DAMAGE_PERCENT)
			{
				if (pItem->GetKind2() == ItemKind::STAFF || pItem->GetKind2() == ItemKind::STICK || pItem->GetKind2() == ItemKind::RUNIC_MACE || pItem->GetKind2() == ItemKind::WIZARD_PENDANT || pItem->GetKind2() == ItemKind::MAGIC_GUN)
				{
					this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 2);
					this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 2);
				}
				else if (pItem->GetKind2() == ItemKind::BOOK)
				{
					this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MIN, 2);
					this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MAX, 2);
				}
				else
				{
					if (i == WEAPON_01)
					{
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 2);
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 2);
					}
					else if (i == WEAPON_02)
					{
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 2);
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 2);
					}
					else
					{
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 2);
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 2);
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 2);
						this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 2);
					}
				}
			}

			if (pItem->GetExe() & ExeOption::INCREASE_EXE_DAMAGE_RATE)
			{
				this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, 10);
			}
			break;

		case 2:
			if (pItem->GetExe() & ExeOption::WingIncreaseMaxHP)
			{
				this->PowerIncreseAdd(POWER_LIFE, 50 + (pItem->GetLevel() * 5), false);
			}

			if (pItem->GetExe() & ExeOption::WingIncreaseMaxMana)
			{
				this->PowerIncreseAdd(POWER_MANA, 50 + (pItem->GetLevel() * 5), false);
			}

			if (pItem->GetExe() & ExeOption::WingIgnoreEnemyDefense)
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 3);
			}
			break;

		case 3:
			if (pItem->GetExe() & ExeOption::WingThirdIgnoreEnemyDefense)
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 5);
			}

			if (pItem->GetExe() & ExeOption::WingThirdReflectTotalEnemyDamage)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE, 5);
			}

			if (pItem->GetExe() & ExeOption::WingThirdTotalyHPRecovery)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY, 5);
			}

			if (pItem->GetExe() & ExeOption::WingThirdTotalyManaRecovery)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_MANA_RECOVERY, 5);
			}
			break;

		case 4:
			if (pItem->GetExe() & ExeOption::WingMonsterIgnoreEnemyDefense)
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 3);
			}

			if (pItem->GetExe() & ExeOption::WingMonsterTotalyHPRecovery)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY, 5);
			}
			break;

		case 5:
			if (pItem->GetExe() & 1)
			{
				this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, 4);
			}

			if (pItem->GetExe() & 2)
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 4);
			}

			if (pItem->GetExe() & 4)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY, 4);
			}
			break;

		case 9:
			if (pItem->GetExe() & 1)
			{
				this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, 6);
			}

			if (pItem->GetExe() & 2)
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, 6);
			}

			if (pItem->GetExe() & 4)
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY, 6);
			}
			break;

		case 6:
			if (pItem->GetExe() & 1)
			{
				int32 add_value = ((float)pItemInfo->GetLevel() / 3.5f);

				this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, add_value);
			}

			if (pItem->GetExe() & 2)
			{
				this->PercentIntData(UNIT_INT_DEFENSE_RATE, 10);
			}

			if (pItem->GetExe() & 4)
			{
				this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, 5);
			}

			if (pItem->GetExe() & 8)
			{
				int32 add_value = ((float)pItemInfo->GetLevel() * 0.3f);

				this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE_ADD, add_value);
			}

			if (pItem->GetExe() & 16)
			{
				this->PowerIncreseAdd(POWER_MANA, pItemInfo->GetLevel() + (pItemInfo->GetLevel() / 10), false);
			}

			if (pItem->GetExe() & 32)
			{
				this->PowerIncreseAdd(POWER_LIFE, pItemInfo->GetLevel() + (pItemInfo->GetLevel() / 10), false);
			}
			break;

		case 8:
			if (pItem->GetExe() & 1)
			{
				this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, (this->GetTotalLevel() / 20) * ((float)pItemInfo->GetLevel() / 100.0f));
			}

			if (pItem->GetExe() & 2)
			{
				this->PercentIntData(UNIT_INT_DEFENSE_RATE, 10);
			}

			if (pItem->GetExe() & 4)
			{
				this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, 5);
			}

			if (pItem->GetExe() & 8)
			{
				int32 add_value = ((float)pItemInfo->GetLevel() * 0.3f);

				this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE_ADD, add_value);
			}

			if (pItem->GetExe() & 16)
			{
				this->PowerIncreseAdd(POWER_MANA, pItemInfo->GetLevel() + (pItemInfo->GetLevel() / 10), false);
			}

			if (pItem->GetExe() & 32)
			{
				this->PowerIncreseAdd(POWER_LIFE, pItemInfo->GetLevel() + (pItemInfo->GetLevel() / 10), false);
			}
			break;

		case 7:
			if (pItem->GetExe() & 1)
			{
				this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP, 8);
			}

			if (pItem->GetExe() & 2)
			{
				this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_HP, 8);
			}

			if (pItem->GetExe() & 4)
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 7);
				this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 7);
			}

			if (pItem->GetExe() & 8)
			{
				int32 add_value = ((float)pItemInfo->GetLevel() / 3.5f);

				if (pItem->GetKind2() == ItemKind::STAFF || pItem->GetKind2() == ItemKind::STICK || pItem->GetKind2() == ItemKind::RUNIC_MACE || pItem->GetKind2() == ItemKind::MAGIC_GUN)
				{
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_value);
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_value);
				}
				else if (pItem->GetKind2() == ItemKind::BOOK)
				{
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, add_value);
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, add_value);
				}
				else
				{
					if (i == WEAPON_01)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, add_value);
					}
					else if (i == WEAPON_02)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, add_value);
					}
					else
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, add_value);
					}
				}
			}

			if (pItem->GetExe() & 16)
			{
				int32 add_value = (this->GetTotalLevel() / 20) * ((float)pItemInfo->GetLevel() / 100.0f);

				if (pItem->GetKind2() == ItemKind::STAFF || pItem->GetKind2() == ItemKind::STICK || pItem->GetKind2() == ItemKind::RUNIC_MACE || pItem->GetKind2() == ItemKind::MAGIC_GUN)
				{
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_value);
					this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_value);
				}
				else if (pItem->GetKind2() == ItemKind::BOOK)
				{
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, add_value);
					this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, add_value);
				}
				else
				{
					if (i == WEAPON_01)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, add_value);
					}
					else if (i == WEAPON_02)
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, add_value);
					}
					else
					{
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, add_value);
						this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, add_value);
					}
				}
			}

			if (pItem->GetExe() & 32)
			{
				this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, 10);
			}
			break;
		}
	}
}

void Player::CalculateSocketItem(bool flag)
{
	uint8 socket_count[MAX_SOCKET_TYPE];
	memset(socket_count, 0, MAX_SOCKET_TYPE);

	wear_inventory_loop(i)
	{
		const auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsValid() || pItem->IsExpired() || pItem->GetKind3() != ItemKind3::SOCKET)
		{
			continue;
		}

		SOCKET_SLOT_LOOP(n)
		{
			if (pItem->GetSocket(n) != SOCKET_SLOT_EMPTY && pItem->GetSocket(n) != SOCKET_SLOT_NONE)
			{
				this->ApplySocketEffect(flag, pItem->GetSocket(n), socket_count);
			}
		}

		if (pItem->GetSocketBonus() != 0xFF)
		{
			this->ApplySocketBonus(flag, pItem->GetSocketBonus());
		}
	}

	this->ApplySocketPackage(flag, socket_count);
}

void Player::ApplySocketEffect(bool flag, SocketDataType socket, uint8 * socket_count)
{
	uint8 index = socket % MAX_SOCKET_OPTION;
	uint8 level = socket / MAX_SOCKET_OPTION;

	if ( level >= MAX_SOCKET_LEVEL )
	{
		return;
	}

	item_socket_seed const* seed = sItemMgr->GetSocketSeed(index);

	if ( !seed )
	{
		return;
	}

	if ( seed->GetType() <= 0 )
	{
		return;
	}

	socket_count[seed->GetType() - 1]++;
	uint32 value = seed->GetLevel(level);

	if ( !flag )
	{
		if ( index == 34 ||
			 index == 35 ||
			 index == 36 ||
			 index == 37 )
		{
			return;
		}
	}
	else
	{
		if ( index != 34 &&
			 index != 35 &&
			 index != 36 &&
			 index != 37 )
		{
			return;
		}
	}

	switch(index)
	{
	case 0:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, ((this->GetTotalLevel() / 20) * value) / 10);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, ((this->GetTotalLevel() / 20) * value) / 10);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, ((this->GetTotalLevel() / 20) * value) / 10);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, ((this->GetTotalLevel() / 20) * value) / 10);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, ((this->GetTotalLevel() / 20) * value) / 10);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, ((this->GetTotalLevel() / 20) * value) / 10);
		} break;

	case 1:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, value);
		} break;

	case 2:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case 3:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		} break;

	case 4:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case 5:
		{
			this->IncreaseStaminaReduction(value);
		} break;

	case 10:
		{
			this->PercentIntData(UNIT_INT_DEFENSE_RATE, value);
		} break;

	case 11:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case 12:
		{
			this->IncreaseData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT, value);
		} break;

	case 13:
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, value);
		} break;

	case 14:
		{
			this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, value);
		} break;

	case 16:
		{
			   this->IncreaseData(PLAYER_DATA_MONSTER_DIE_HP, value);
		} break;

	case 17:
		{
			   this->IncreaseData(PLAYER_DATA_MONSTER_DIE_MP, value);
		} break;

	case 18:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case 19:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_RATE, value);
		} break;

	case 20:
		{
		} break;

	case 21:
		{
			this->IncreaseData(PLAYER_DATA_HP_RECOVERY_ADD, value);
		} break;

	case 22:
		{
			this->PowerIncreseAdd(POWER_LIFE, value, false);
		} break;

	case 23:
		{
			this->PowerIncreseAdd(POWER_MANA, value, false);
		} break;

	case 24:
		{
			this->IncreaseData(PLAYER_DATA_MP_RECOVERY_ADD, value);
		} break;

	case 25:
		{
			this->PowerIncreseAdd(POWER_STAMINA, value, false);
		} break;

	case 26:
		{
			this->IncreaseData(PLAYER_DATA_AG_RECOVERY_ADD, value);
		} break;

	case 29:
		{
			this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case 30:
		{
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

	case 31:
		{
			this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case 32:
		{
			this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

	case 34:
		{
			this->IncreaseAddStat(STRENGTH, value);
		} break;

	case 35:
		{
			this->IncreaseAddStat(AGILITY, value);
		} break;

	case 36:
		{
			this->IncreaseAddStat(VITALITY, value);
		} break;

	case 37:
		{
			this->IncreaseAddStat(ENERGY, value);
		} break;
	}
}

void Player::ApplySocketBonus(bool flag, uint8 socket_bonus)
{
	if ( flag )
	{
		return;
	}

	item_socket_bonus const* bonus = sItemMgr->GetSocketBonus(socket_bonus);

	if ( !bonus )
		return;

	uint32 value = bonus->GetValue();

	switch(socket_bonus)
	{
	case 0:
	case 6:
	case 12:
	case 18:
	case 24:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		} break;

	case 1:
	case 7:
	case 13:
	case 19:
	case 25:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case 2:
	case 8:
	case 14:
	case 20:
	case 26:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case 3:
	case 9:
	case 15:
	case 21:
	case 27:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case 4:
	case 10:
	case 16:
	case 22:
	case 28:
	case 30:
	case 32:
	case 34:
	case 36:
	case 38:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case 5:
	case 11:
	case 17:
	case 23:
	case 29:
	case 31:
	case 33:
	case 35:
	case 37:
	case 39:
		{
			this->PowerIncreseAdd(POWER_LIFE, value, false);
		} break;
	}
}

void Player::ApplySocketPackage(bool flag, uint8 * socket_count)
{
	if ( flag )
	{
		return;
	}

	sItemMgr->GetSocketPackage(this, socket_count);
}

void Player::CalculateAncientItemStat()
{
	this->m_AncientSetDataMap.clear();

	uint8 RightSetOptionIndex = -1;
	uint8 RightRingSetOptionIndex = -1;

	wear_inventory_loop(i)
	{
		if (!this->GetInventory()->GetItem(i)->IsValid() ||
			!this->GetInventory()->GetItem(i)->GetAncientType())
		{
			continue;
		}

		item_set_type const* ancient_data = sItemMgr->GetItemSetType(this->GetInventory()->GetItem(i)->GetItem());

		if (!ancient_data)
		{
			continue;
		}

		uint8 ancient = this->GetInventory()->GetItem(i)->GetAncientType();
		uint8 ancient_option = ancient_data->GetAncientID(ancient - 1);

		if (i == WEAPON_01)
		{
			RightSetOptionIndex = ancient_option;
		}
		else if (i == WEAPON_02)
		{
			if (RightSetOptionIndex == ancient_option)
			{
				if (this->GetInventory()->GetItem(WEAPON_01)->GetItem() == this->GetInventory()->GetItem(WEAPON_02)->GetItem() &&
					this->GetInventory()->GetItem(WEAPON_01)->GetAncientType() == this->GetInventory()->GetItem(WEAPON_02)->GetAncientType())
				{
					continue;
				}
			}
		}

		if (i == RING_01)
		{
			RightRingSetOptionIndex = ancient_option;
		}
		else if (i == RING_02)
		{
			if (RightRingSetOptionIndex == ancient_option)
			{
				if (this->GetInventory()->GetItem(RING_01)->GetItem() == this->GetInventory()->GetItem(RING_02)->GetItem() &&
					this->GetInventory()->GetItem(RING_01)->GetAncientType() == this->GetInventory()->GetItem(RING_02)->GetAncientType())
				{
					continue;
				}
			}
		}

		AncientSetData & data = this->m_AncientSetDataMap[ancient_option];

		data.SetID(ancient_option);
		data.IncreaseParts(1);
	}

	uint8 option[MAX_ITEM_ANCIENT_OPTION_FULL];
	int32 value[MAX_ITEM_ANCIENT_OPTION_FULL];

	for (int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
	{
		option[i] = -1;
		value[i] = 0;
	}

	for (AncientSetDataMap::iterator it = this->m_AncientSetDataMap.begin(); it != this->m_AncientSetDataMap.end(); ++it)
	{
		AncientSetData & data = it->second;

		uint8 option_id = data.GetID();
		int32 option_count = data.GetParts();

		if (option_count >= 2)
		{
			for (int32 tablecnt = 0; tablecnt < (option_count - 1); ++tablecnt)
			{
				if (this->GetAncientOption(option_id, tablecnt, option, value))
				{
					this->SetAncientOptionStat(option[0], value[0]);
					this->SetAncientOptionStat(option[1], value[1]);
				}
			}

			if (this->GetAncientFullOption(option_id, option_count, option, value))
			{
				data.SetFull(true);
				this->SetFullAncient(true);

				for (int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
				{
					this->SetAncientOptionStat(option[i], value[i]);
				}
			}
		}
	}
}

void Player::CalculateAncientItem()
{
	uint8 option[MAX_ITEM_ANCIENT_OPTION_FULL];
	int32 value[MAX_ITEM_ANCIENT_OPTION_FULL];

	for (int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
	{
		option[i] = -1;
		value[i] = 0;
	}

	for (AncientSetDataMap::iterator it = this->m_AncientSetDataMap.begin(); it != this->m_AncientSetDataMap.end(); ++it)
	{
		AncientSetData & data = it->second;

		uint8 option_id = data.GetID();
		int32 option_count = data.GetParts();

		if (option_count >= 2)
		{
			for (int32 tablecnt = 0; tablecnt < (option_count - 1); ++tablecnt)
			{
				if (this->GetAncientOption(option_id, tablecnt, option, value))
				{
					this->SetAncientOption(option[0], value[0]);
					this->SetAncientOption(option[1], value[1]);
				}
			}

			if (this->GetAncientFullOption(option_id, option_count, option, value))
			{
				for (int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
				{
					this->SetAncientOption(option[i], value[i]);
				}
			}
		}
	}
}

bool Player::GetAncientOption(uint8 option_id, int32 OptTableNum, uint8 * option, int32 * value)
{
	option[0] = -1;
	value[0] = 0;
	option[1] = -1;
	value[1] = 0;

	item_set_option const* option_data = sItemMgr->GetItemSetOption(option_id);

	if ( !option_data )
	{
		return false;
	}

	if ( OptTableNum >= option_data->GetOptionCount() )
	{
		return false;
	}

	if ( OptTableNum >= MAX_ITEM_ANCIENT_OPTION )
	{
		if ( OptTableNum <= (MAX_ITEM_ANCIENT_OPTION + 1) )
		{
			option[0] = option_data->GetExtraOptionTable(OptTableNum - MAX_ITEM_ANCIENT_OPTION);
			value[0] = option_data->GetExtraOptionTableValue(OptTableNum - MAX_ITEM_ANCIENT_OPTION);
		}
	}
	else
	{
		option[0] = option_data->option_table[OptTableNum][0];
		value[0] = option_data->option_table_value[OptTableNum][0];
		option[1] = option_data->option_table[OptTableNum][1];
		value[1] = option_data->option_table_value[OptTableNum][1];
	}

	return true;
}

void Player::SetAncientOptionStat(uint8 option, int32 value)
{
	switch ( option )
	{
	case ANCIENT_EFFECT_STRENGTH:
		{
			this->IncreaseAddStat(STRENGTH, value);
		} break;

	case ANCIENT_EFFECT_AGILITY:
		{
			this->IncreaseAddStat(AGILITY, value);
		} break;

	case ANCIENT_EFFECT_VITALITY:
		{
			this->IncreaseAddStat(VITALITY, value);
		} break;

	case ANCIENT_EFFECT_ENERGY:
		{
			this->IncreaseAddStat(ENERGY, value);
		} break;

	case ANCIENT_EFFECT_LEADERSHIP:
		{
			if (this->GetClass() == Character::DARK_LORD)
			{
				this->IncreaseAddStat(LEADERSHIP, value);
			}
		} break;

	case ANCIENT_EFFECT_INCREASE_ALL_STATS:
		{
			this->IncreaseAddStat(STRENGTH, value);
			this->IncreaseAddStat(AGILITY, value);
			this->IncreaseAddStat(VITALITY, value);
			this->IncreaseAddStat(ENERGY, value);

			if ( this->GetClass() == Character::DARK_LORD )
			{
				this->IncreaseAddStat(LEADERSHIP, value);
			}
		} break;
	}
}

void Player::SetAncientOption(uint8 option, int32 value)
{
	if ( !CHECK_RANGE(option, ANCIENT_EFFECT_MAX) )
	{
		return;
	}

	this->IncreaseAncientEffect(option, value);

	switch ( option )
	{
	case ANCIENT_EFFECT_ATTACK_DAMAGE_MIN:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
		} break;

	case ANCIENT_EFFECT_ATTACK_DAMAGE_MAX:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		} break;

	case ANCIENT_EFFECT_MAGIC_DAMAGE:
		{
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case ANCIENT_EFFECT_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_ADD, value);
		} break;

	case ANCIENT_EFFECT_ATTACK_RATE:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_RATE, value);
		} break;

	case ANCIENT_EFFECT_DEFENSE:
		{
			//this->IncreaseIntData(UNIT_INT_DEFENSE, value * 10 / 20);
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case ANCIENT_EFFECT_MAX_LIFE:
		{
			this->PowerIncreseAdd(POWER_LIFE, value, false);
		} break;

	case ANCIENT_EFFECT_MAX_MANA:
		{
			this->PowerIncreseAdd(POWER_MANA, value, false);
		} break;

	case ANCIENT_EFFECT_MAX_STAMINA:
		{
			this->PowerIncreseAdd(POWER_STAMINA, value, false);
		} break;

	case ANCIENT_EFFECT_STAMINA_VALUE:
		{
			this->IncreaseData(PLAYER_DATA_AG_RECOVERY_ADD, value);
		} break;

	case ANCIENT_EFFECT_CRITICAL_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

	case ANCIENT_EFFECT_CRITICAL_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case ANCIENT_EFFECT_EXCELLENT_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

	case ANCIENT_EFFECT_EXCELLENT_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case ANCIENT_EFFECT_SKILL_DAMAGE_ADD:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case ANCIENT_EFFECT_DOUBLE_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
		} break;

	case ANCIENT_EFFECT_IGNORE_DEFENSE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
		} break;

	case ANCIENT_EFFECT_SHIELD_DEFENSE:
		{
			this->IncreaseData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE, value);
		} break;

	case ANCIENT_EFFECT_TWO_HAND_SWORD_DAMAGE:
		{
			if ( (this->GetInventory()->GetItem(WEAPON_01)->IsValid() && this->GetInventory()->GetItem(WEAPON_01)->IsTwoHand()) || 
				 (this->GetInventory()->GetItem(WEAPON_02)->IsValid() && this->GetInventory()->GetItem(WEAPON_02)->IsTwoHand() ) )
			{
				this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
				this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
				this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
				this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			}
		} break;

	case ANCIENT_EFFECT_INCREASE_DEFENSE_BASE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, value);
		} break;

	case ANCIENT_EFFECT_DAMAGE_2:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		} break;

	case ANCIENT_EFFECT_MAGIC_DAMAGE_2:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case ANCIENT_EFFECT_TRIPLE_DAMAGE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_TRIPLE_DAMAGE_RATE, value);
		} break;

	case ANCIENT_EFFECT_WIZARDRY_CURSE:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		} break;
	}
}

bool Player::GetAncientFullOption(uint8 option_id, int32 count, uint8* option, int32* value)
{
	for ( int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i )
	{
		option[i] = -1;
		value[i] = 0;
	}

	item_set_option const* option_data = sItemMgr->GetItemSetOption(option_id);

	if ( !option_data )
	{
		return false;
	}

	if ( option_data->GetOptionCount() >= count )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i )
	{
		option[i] = option_data->GetFullOptionTable(i);
		value[i] = option_data->GetFullOptionTableValue(i);
	}

	return true;
}

void Player::SetAncientMastery(bool stat)
{
	uint8 option[MAX_ITEM_ANCIENT_OPTION_FULL];
	int32 value[MAX_ITEM_ANCIENT_OPTION_FULL];

	for (int32 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
	{
		option[i] = -1;
		value[i] = 0;
	}

	wear_inventory_loop(i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem)
		{
			continue;
		}

		if (!pItem->IsValid() || !pItem->GetAncientType())
		{
			continue;
		}

		item_set_type const* pAncientData = sItemMgr->GetItemSetType(pItem->GetItem());

		if (!pAncientData)
		{
			continue;
		}

		uint8 ancient = pItem->GetAncientType();
		uint8 ancient_option = pAncientData->GetAncientID(ancient - 1);

		AncientSetData & data = this->m_AncientSetDataMap[ancient_option];

		uint8 option_id = data.GetID();
		int32 option_count = data.GetParts();

		if (option_count >= 2)
		{
			for (int32 tablecnt = 0; tablecnt < (option_count - 1); ++tablecnt)
			{
				if (this->GetAncientOption(option_id, tablecnt, option, value))
				{
					this->SetAncientMastery(stat, option[0], value[0], pItem->GetLevel());
					this->SetAncientMastery(stat, option[1], value[1], pItem->GetLevel());
				}
			}

			if (this->GetAncientFullOption(option_id, option_count, option, value))
			{
				for (int32 n = 0; n < MAX_ITEM_ANCIENT_OPTION_FULL; ++n)
				{
					this->SetAncientMastery(stat, option[n], value[n], pItem->GetLevel());
				}
			}
		}
	}
}

void Player::SetAncientMastery(bool stat, uint8 option_id, int32 value_id, int32 level)
{
	if (option_id != ANCIENT_EFFECT_MASTERY_ATTACK_POWER &&
		option_id != ANCIENT_EFFECT_MASTERY_WIZARDY &&
		option_id != ANCIENT_EFFECT_MASTERY_BASIC_DEFENSE &&
		option_id != ANCIENT_EFFECT_MASTERY_EXCELLENT_DAMAGE &&
		option_id != ANCIENT_EFFECT_MASTERY_CRITICAL_DAMAGE &&
		option_id != ANCIENT_EFFECT_MASTERY_MAXIMUM_HP &&
		option_id != ANCIENT_EFFECT_MASTERY_SKILL_DAMAGE &&
		option_id != ANCIENT_EFFECT_MASTERY_STRENGTH &&
		option_id != ANCIENT_EFFECT_MASTERY_AGILITY &&
		option_id != ANCIENT_EFFECT_MASTERY_ENERGY &&
		option_id != ANCIENT_EFFECT_MASTERY_STATS &&
		option_id != ANCIENT_EFFECT_MASTERY_WIZARDRY_CURSE &&
		option_id != ANCIENT_EFFECT_MASTERY_DAMAGE_REDUCTION &&
		option_id != ANCIENT_EFFECT_MASTERY_ELEMENTAL_DEFENSE)
	{
		return;
	}

	if (stat)
	{
		if (option_id != ANCIENT_EFFECT_MASTERY_STRENGTH &&
			option_id != ANCIENT_EFFECT_MASTERY_AGILITY &&
			option_id != ANCIENT_EFFECT_MASTERY_ENERGY &&
			option_id != ANCIENT_EFFECT_MASTERY_STATS)
		{
			return;
		}
	}
	else
	{
		if (option_id == ANCIENT_EFFECT_MASTERY_STRENGTH ||
			option_id == ANCIENT_EFFECT_MASTERY_AGILITY ||
			option_id == ANCIENT_EFFECT_MASTERY_ENERGY ||
			option_id == ANCIENT_EFFECT_MASTERY_STATS)
		{
			return;
		}
	}

	int32 value = sItemMgr->GetItemSetValue(value_id, level);

	switch (option_id)
	{
	case ANCIENT_EFFECT_MASTERY_ATTACK_POWER:
	{
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
		this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_WIZARDY:
	{
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_BASIC_DEFENSE:
	{
		this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_EXCELLENT_DAMAGE:
	{
		this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_CRITICAL_DAMAGE:
	{
		this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_MAXIMUM_HP:
	{
		this->PowerIncreseAdd(POWER_LIFE, value, false);
	} break;

	case ANCIENT_EFFECT_MASTERY_SKILL_DAMAGE:
	{
		this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_STRENGTH:
	{
		this->IncreaseAddStat(STRENGTH, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_AGILITY:
	{
		this->IncreaseAddStat(AGILITY, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_ENERGY:
	{
		this->IncreaseAddStat(ENERGY, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_STATS:
	{
		this->IncreaseAddStat(STRENGTH, value);
		this->IncreaseAddStat(AGILITY, value);
		this->IncreaseAddStat(VITALITY, value);
		this->IncreaseAddStat(ENERGY, value);

		if (this->GetClass() == Character::DARK_LORD)
		{
			this->IncreaseAddStat(LEADERSHIP, value);
		}
	} break;

	case ANCIENT_EFFECT_MASTERY_WIZARDRY_CURSE:
	{
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_DAMAGE_REDUCTION:
	{
		this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE_ADD, value);
	} break;

	case ANCIENT_EFFECT_MASTERY_ELEMENTAL_DEFENSE:
	{
		this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, value);
	} break;
	}
}

int32 Player::GetSpecialItemExperience()
{
	Item const* Pet = this->GetInventory()->GetItem(GUARDIAN);

	if ( !Pet->IsValid() )
	{
		return 0;
	}

	if ( Pet->GetItem() == ITEMGET(13, 80) ||
		 Pet->GetItem() == ITEMGET(13, 123) )
	{
		if ( this->GetInventory()->IsWearingRing(ITEMGET(13, 76), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 77), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 78), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 122), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 163), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 164), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 165), 0) ||
			 this->GetInventory()->IsWearingRing(ITEMGET(13, 268), 0) )
		{
			return sGameServer->GetPetComboExperience();
		}
	}

	return 0;
}

void Player::ApplyStats1()
{
	const auto pPendant = this->GetInventory()->GetItem(PENDANT);
	const auto pRing01 = this->GetInventory()->GetItem(RING_01);
	const auto pRing02 = this->GetInventory()->GetItem(RING_02);
	const auto pHelper = this->GetInventory()->GetItem(GUARDIAN);

	int32 add_physical_damage_min = 0;
	int32 add_physical_damage_max = 0;
	int32 add_defense = 0;
	int32 add_wizardry_damage_min = 0;
	int32 add_wizardry_damage_max = 0;
	int32 add_curse_damage_min = 0;
	int32 add_curse_damage_max = 0;

	uint16 ring_item_01 = -1;
	uint16 ring_item_02 = -1;

	if (pRing01 && pRing01->IsItem() && pRing01->IsValid() && !pRing01->IsExpired())
	{
		ring_item_01 = pRing01->GetItem();
	}

	if (pRing02 && pRing02->IsItem() && pRing02->IsValid() && !pRing02->IsExpired())
	{
		ring_item_02 = pRing02->GetItem();
	}

	if (ring_item_01 == ITEMGET(13, 41) || ring_item_02 == ITEMGET(13, 41))// Transfer Christmas Ring
	{
		add_physical_damage_min += 20;
		add_physical_damage_max += 20;

		add_wizardry_damage_min += 20;
		add_wizardry_damage_max += 20;

		add_curse_damage_min += 20;
		add_curse_damage_max += 20;
	}

	if (ring_item_01 == ITEMGET(13, 76) || ring_item_02 == ITEMGET(13, 76) ||
		ring_item_01 == ITEMGET(13, 77) || ring_item_02 == ITEMGET(13, 77) ||
		ring_item_01 == ITEMGET(13, 78) || ring_item_02 == ITEMGET(13, 78) ||
		ring_item_01 == ITEMGET(13, 166) || ring_item_02 == ITEMGET(13, 166) ||
		ring_item_01 == ITEMGET(13, 471) || ring_item_02 == ITEMGET(13, 471))
	{
		add_physical_damage_min += 30;
		add_physical_damage_max += 30;

		add_wizardry_damage_min += 30;
		add_wizardry_damage_max += 30;

		add_curse_damage_min += 30;
		add_curse_damage_max += 30;
	}

	if (ring_item_01 == ITEMGET(13, 77) || ring_item_02 == ITEMGET(13, 77) ||
		ring_item_01 == ITEMGET(13, 78) || ring_item_02 == ITEMGET(13, 78) ||
		ring_item_01 == ITEMGET(13, 166) || ring_item_02 == ITEMGET(13, 166))
	{
		this->IncreaseData(PLAYER_DATA_ADD_EXPERIENCE_RATE, 10);
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 50);
	}

	if (ring_item_01 == ITEMGET(13, 76) || ring_item_02 == ITEMGET(13, 76))
	{
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 50);
	}

	if (ring_item_01 == 6778 || ring_item_02 == 6778 || ring_item_01 == 7128 || ring_item_02 == 7128)// Skeleton Rings
	{
		add_physical_damage_min += 40;
		add_physical_damage_max += 40;

		add_wizardry_damage_min += 40;
		add_wizardry_damage_max += 40;

		add_curse_damage_min += 40;
		add_curse_damage_max += 40;
	}

	if (ring_item_01 == 6819 || ring_item_02 == 6819 || ring_item_01 == 7129 || ring_item_02 == 7129)// Robot Knight Rings
	{
		add_physical_damage_min += 30;
		add_physical_damage_max += 30;

		add_wizardry_damage_min += 30;
		add_wizardry_damage_max += 30;

		add_curse_damage_min += 30;
		add_curse_damage_max += 30;
	}

	if (ring_item_01 == 6820 || ring_item_02 == 6820 || ring_item_01 == 7130 || ring_item_02 == 7130)// Mini Robot Ring
	{
		add_physical_damage_min += 30;
		add_physical_damage_max += 30;

		add_wizardry_damage_min += 30;
		add_wizardry_damage_max += 30;

		add_curse_damage_min += 30;
		add_curse_damage_max += 30;

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 7);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 7);
	}

	if (ring_item_01 == 6821 || ring_item_02 == 6821 || ring_item_01 == 7131 || ring_item_02 == 7131)// Mage Ring
	{
		add_physical_damage_min += 40;
		add_physical_damage_max += 40;

		add_wizardry_damage_min += 40;
		add_wizardry_damage_max += 40;

		add_curse_damage_min += 40;
		add_curse_damage_max += 40;
	}

	if (ring_item_01 == 6924 || ring_item_02 == 6924)
	{
		add_physical_damage_min += 70;
		add_physical_damage_max += 70;

		add_wizardry_damage_min += 70;
		add_wizardry_damage_max += 70;

		add_curse_damage_min += 70;
		add_curse_damage_max += 70;

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
		this->PowerIncreseAdd(POWER_LIFE, 50, false);
	}

	if (ring_item_01 == 6695 || ring_item_02 == 6695)
	{
		add_defense += this->GetIntData(UNIT_INT_DEFENSE) / 10;
		this->PowerIncreseAdd(POWER_LIFE, this->GetTotalLevel(), false);
	}

	if (ring_item_01 == 6819 || ring_item_02 == 6819 || ring_item_01 == 7129 || ring_item_02 == 7129)
	{
		add_defense += 10;
	}

	if (ring_item_01 == 6924 || ring_item_02 == 6924)
	{
		add_defense += 50;
	}

	if (ring_item_01 == 6609 || ring_item_02 == 6609)
	{
		add_physical_damage_min += 20;
		add_physical_damage_max += 20;

		add_wizardry_damage_min += 20;
		add_wizardry_damage_max += 20;

		add_curse_damage_min += 20;
		add_curse_damage_max += 20;
	}

	if (ring_item_01 == 6610 || ring_item_02 == 6610)
	{
		add_defense += 20;
	}

	use_inventory_loop(i)
	{
		const auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsValid() || !pItem->IsInventoryActive())
		{
			continue;
		}

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, pItem->GetAttackSpeed());
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, pItem->GetAttackSpeed());

		if (pItem->GetItem() == PET_FENRIR && pItem->GetExe() == 4)
		{
			add_physical_damage_min += this->GetTotalLevel() / 12;
			add_physical_damage_max += this->GetTotalLevel() / 12;

			add_wizardry_damage_min += this->GetTotalLevel() / 25;
			add_wizardry_damage_max += this->GetTotalLevel() / 25;

			add_curse_damage_min += this->GetTotalLevel() / 25;
			add_curse_damage_max += this->GetTotalLevel() / 25;

			this->PowerIncreseAdd(POWER_LIFE, this->GetTotalLevel() / 2, false);
			this->PowerIncreseAdd(POWER_MANA, this->GetTotalLevel() / 2, false);
			continue;
		}

		if (pItem->GetItem() == PET_DINORANT)
		{
			this->PowerIncreseAdd(POWER_STAMINA, pItem->GetOptionValue(ItemOption::DINORANT_STAMINA), false);
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, pItem->GetOptionValue(ItemOption::ATTACK_SPEED));
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, pItem->GetOptionValue(ItemOption::ATTACK_SPEED));
			continue;
		}

		if (pItem->GetKind1() == ItemKind1::GUARDIAN_MOUNT)
		{
			const auto pItemInfo = sItemMgr->GetItem(pItem->GetItem());
			if (!pItemInfo)
			{
				continue;
			}

			for (uint8 n = 0; n < MAX_SOCKET_SLOT; ++n)
			{
				item_guardian_option_value const* pData = sItemMgr->GetGuardianOptionValue(pItemInfo->GetLevel(), pItem->GetSocket(n));

				if (!pData)
				{
					continue;
				}

				int32 total_level = this->GetTotalLevel();

				if (pData->GetCondition() != -1 && total_level > pData->GetCondition())
				{
					total_level = pData->GetCondition();
				}

				float formula_value = sFormulaMgr->GetOrCreateValue(8, pData->GetFormulaData(), pItem->GetLevel());

				switch (pData->GetID())
				{
				case 0:
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, formula_value);
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, formula_value);
					break;

				case 1:
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, formula_value);
					break;

				case 2:
					add_defense += (total_level / 20) + formula_value;
					break;

				case 3:
					add_physical_damage_min += (total_level / 20) + formula_value;
					add_physical_damage_max += (total_level / 20) + formula_value;
					add_wizardry_damage_min += (total_level / 20) + formula_value;
					add_wizardry_damage_max += (total_level / 20) + formula_value;
					add_curse_damage_min += (total_level / 20) + formula_value;
					add_curse_damage_max += (total_level / 20) + formula_value;
					break;

				case 4:
					this->IncreaseDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_HP_RECOVERY, formula_value);
					break;
				}
			}

			if (pItem->GetSocket(4) != SOCKET_SLOT_NONE)
			{
				item_guardian_elite_option_value const* pData = sItemMgr->GetGuardianEliteOptionValue(pItemInfo->GetLevel(), pItem->GetSocket(4));

				if (!pData)
				{
					return;
				}

				for (int32 n = 0; n < MAX_GUARDIAN_ELITE_OPTIONS; ++n)
				{
					float value = sFormulaMgr->GetOrCreateValue(8, pData->GetFormulaData(n), pItem->GetLevel());

					if (value <= 0)
					{
						continue;
					}

					int32 value_int = static_cast<int32>(value);

					switch (pData->GetID(n))
					{
					case 0:
						this->IncreaseIntData(UNIT_INT_DEBUFF_BLEEDING_RESISTANCE, value_int);
						break;

					case 1:
						this->IncreaseIntData(UNIT_INT_DEBUFF_POISON_RESISTANCE, value_int);
						break;

					case 2:
						this->IncreaseIntData(UNIT_INT_DEBUFF_DAMAGE_REDUCTION_RESISTANCE, value_int);
						break;

					case 3:
						this->IncreaseIntData(UNIT_INT_DEBUFF_ATTACK_SPEED_REDUCTION_RESISTANCE, value_int);
						break;

					case 4:
						this->IncreaseIntData(UNIT_INT_DEBUFF_DEFENSE_REDUCTION_RESISTANCE, value_int);
						break;
					}
				}
			}
		}
	}

	this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, add_physical_damage_min);
	this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, add_physical_damage_max);
	this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, add_physical_damage_min);
	this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, add_physical_damage_max);
	this->IncreaseIntData(UNIT_INT_DEFENSE, add_defense);
	this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, add_wizardry_damage_min);
	this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, add_wizardry_damage_max);
	this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, add_curse_damage_min);
	this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, add_curse_damage_max);

	if (ring_item_01 == ITEMGET(13, 20))// Wizard Ring
	{
		int32 value1 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
		int32 value2 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX);
		int32 value3 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN);
		int32 value4 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX);
		int32 value5 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
		int32 value6 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);

		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value1, 10);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value2, 10);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value3, 10);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value4, 10);
		pRing01->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value5, 10);
		pRing01->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value6, 10);

		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value1);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value2);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value3);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value4);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value5);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value6);

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
	}

	if (ring_item_02 == ITEMGET(13, 20))// Wizard Ring
	{
		int32 value1 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
		int32 value2 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX);
		int32 value3 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN);
		int32 value4 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX);
		int32 value5 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
		int32 value6 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);

		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value1, 10);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value2, 10);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value3, 10);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value4, 10);
		pRing02->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value5, 10);
		pRing02->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value6, 10);

		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value1);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value2);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value3);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value4);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value5);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value6);

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
	}

	if (ring_item_01 == ITEMGET(13, 107))// Wizard Ring
	{
		int32 value1 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
		int32 value2 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX);
		int32 value3 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN);
		int32 value4 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX);
		int32 value5 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
		int32 value6 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);

		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value1, 15);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value2, 15);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value3, 15);
		pRing01->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value4, 15);
		pRing01->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value5, 15);
		pRing01->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value6, 15);

		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value1);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value2);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value3);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value4);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value5);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value6);

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
	}

	if (ring_item_02 == ITEMGET(13, 107))// Wizard Ring
	{
		int32 value1 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
		int32 value2 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX);
		int32 value3 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN);
		int32 value4 = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX);
		int32 value5 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
		int32 value6 = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);

		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value1, 15);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value2, 15);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value3, 15);
		pRing02->ApplyOption(ItemOption::PHYSICAL_DAMAGE_MUL, value4, 15);
		pRing02->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value5, 15);
		pRing02->ApplyOption(ItemOption::WIZARDRY_DAMAGE_MUL, value6, 15);

		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value1);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value2);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value3);
		this->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value4);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value5);
		this->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value6);

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
	}

	if (pHelper && pHelper->IsItem() && pHelper->IsValid() && !pHelper->IsExpired())
	{
		if (pHelper->GetItem() == 6720)
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 40);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 40);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 40);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 40);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 40);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 40);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MIN, 40);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MAX, 40);
		}

		if (pHelper->GetItem() == 6779 || pHelper->GetItem() == 7126)
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 20);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 20);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 20);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 20);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 20);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 20);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MIN, 20);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MAX, 20);
		}

		if (pHelper->GetItem() == 6657)
		{
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 30);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 30);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 30);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 30);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 30);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 30);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MIN, 30);
			this->PercentIntData(UNIT_INT_CURSE_DAMAGE_MAX, 30);
		}

		if (pHelper->GetItem() == PET_PANDA || pHelper->GetItem() == ITEMGET(13, 469))
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, 50);
			this->IncreaseData(PLAYER_DATA_ADD_EXPERIENCE_RATE, 10);
		}

		if (pHelper->GetItem() == PET_SPIRIT_OF_GUARDIAN)
		{
			this->PowerIncreseAdd(POWER_LIFE, 50, false);
		}
	}

	if (ring_item_01 != 6825 && ring_item_02 != 6825)
	{
		if (ring_item_01 == 6826 || ring_item_02 == 6826)
		{
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 15);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 15);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 15);
			this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 15);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 15);
			this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 15);

			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
		}
	}
	else
	{
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 10);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 10);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, 10);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, 10);
		this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MIN, 10);
		this->PercentIntData(UNIT_INT_MAGIC_DAMAGE_MAX, 10);

		this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 10);
		this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 10);
	}

	switch (ring_item_01)
	{
	case ITEMGET(13, 109):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * 4 / 100, false);
		break;

	case ITEMGET(13, 110):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * 4 / 100, false);
		break;

	case ITEMGET(13, 111):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 30);
		break;

	case ITEMGET(13, 112):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, 4);
		break;

	case ITEMGET(13, 173):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_DOUBLE_DAMAGE_RATE, (pRing01->GetExe() ? 3 : 1) + pRing01->GetLevel());
		break;

	case ITEMGET(13, 174):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_STUN, (pRing01->GetExe() ? 3 : 1) + pRing01->GetLevel());
		break;

	case ITEMGET(13, 175):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE, (pRing01->GetExe() ? 3 : 1) + pRing01->GetLevel());
		break;

	case ITEMGET(13, 176):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE, (pRing01->GetExe() ? 3 : 1) + pRing01->GetLevel());
		break;
	}

	switch (ring_item_02)
	{
	case ITEMGET(13, 109):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * 4 / 100, false);
		break;

	case ITEMGET(13, 110):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * 4 / 100, false);
		break;

	case ITEMGET(13, 111):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 30);
		break;

	case ITEMGET(13, 112):
		this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
		this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, 4);
		break;

	case ITEMGET(13, 173):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_DOUBLE_DAMAGE_RATE, (pRing02->GetExe() ? 3 : 1) + pRing02->GetLevel());
		break;

	case ITEMGET(13, 174):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_STUN, (pRing02->GetExe() ? 3 : 1) + pRing02->GetLevel());
		break;

	case ITEMGET(13, 175):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE, (pRing02->GetExe() ? 3 : 1) + pRing02->GetLevel());
		break;

	case ITEMGET(13, 176):
		this->IncreaseFloatData(UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE, (pRing02->GetExe() ? 3 : 1) + pRing02->GetLevel());
		break;
	}

	if (pPendant && pPendant->IsItem() && pPendant->IsValid())
	{
		switch (pPendant->GetItem())
		{
		case ITEMGET(13, 171):
		case ITEMGET(20, 41):
			this->IncreaseFloatData(UNIT_FLOAT_RESIST_IGNORE_DEFENSE, (pPendant->GetExe() ? 3 : 1) + pPendant->GetLevel());
			break;

		case ITEMGET(13, 172):
		case ITEMGET(20, 42):
			this->IncreaseFloatData(UNIT_FLOAT_RESIST_IGNORE_SD, (pPendant->GetExe() ? 3 : 1) + pPendant->GetLevel());
			break;

		case ITEMGET(13, 113):
			this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, 10);
			break;

		case ITEMGET(13, 114):
			this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, 7);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, 7);
			break;

		case ITEMGET(13, 115):
			this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, 3);
			this->IncreaseDataFloat(PLAYER_FLOAT_MONSTER_DIE_MP, 8);
			break;
		}
	}
}

void Player::ApplyMasteryBonus(bool stat)
{
	wear_inventory_loop(i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsValid())
		{
			continue;
		}

		if (!pItem->IsMysteriousStoneItem())
		{
			continue;
		}

		if (pItem->GetSocketBonus() == SOCKET_SLOT_NONE)
		{
			continue;
		}

		item_mastery_bonus const* pData = sItemMgr->GetMasteryBonus(pItem->GetItem() >= ITEMGET(7, 0) && pItem->GetItem() < ITEMGET(12, 0) ? 1 : 2, pItem->GetSocketBonus());

		if (!pData)
		{
			continue;
		}

		switch (pData->GetType())
		{
		case 1:
			{
				switch (pData->GetID())
				{
				case 1:
				case 2:
				case 3:
					{
						if (!stat)
						{
							this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE_ADD, pData->GetValue());
						}
					} break;
				}
			} break;

		case 2:
			{
				switch (pData->GetID())
				{
				case 1:
				case 2:
				case 3:
					{
						if (stat)
						{
							this->IncreaseAddStat(STRENGTH, pData->GetValue());
							this->IncreaseAddStat(AGILITY, pData->GetValue());
							this->IncreaseAddStat(VITALITY, pData->GetValue());
							this->IncreaseAddStat(ENERGY, pData->GetValue());

							if (this->GetClass() == Character::DARK_LORD)
							{
								this->IncreaseAddStat(LEADERSHIP, pData->GetValue());
							}
						}
					} break;
				}
			} break;
		}
	}
}

void Player::ApplyWingGradedOption(bool stat)
{
	Item const* pWings = this->GetInventory()->GetItem(WINGS);

	if (!pWings || !pWings->IsItem() || !pWings->IsValid() || !pWings->IsWingLvl4())
	{
		return;
	}

	for (int32 i = 0; i < (MAX_SOCKET_SLOT - 1); ++i)
	{
		if (pWings->GetSocket(i) == SOCKET_SLOT_NONE)
		{
			continue;
		}

		uint8 id = pWings->GetSocket(i) / 16;
		uint8 level = pWings->GetSocket(i) % 16;

		int32 value = sItemMgr->GetWingGradedOptionValue(id, level);

		if (value <= 0)
		{
			continue;
		}

		if (stat)
		{
			if (id != 7 && id != 8 && id != 9 && id != 10)
			{
				continue;
			}
		}
		else
		{
			if (id == 7 || id == 8 || id == 9 || id == 10)
			{
				continue;
			}
		}

		switch (id)
		{
		case 0:
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_MANA_RECOVERY, value);
			} break;

		case 1:
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY, value);
			} break;

		case 2:
			{
				this->IncreaseFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE, value);
			} break;

		case 3:
			{
				this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
			} break;

		case 4:
			{
				this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
				this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, value);
			} break;

		case 5:
			{
				this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
			} break;

		case 6:
			{
				this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, value);
			} break;

		case 7:
			{
				this->IncreaseAddStat(STRENGTH, value);
			} break;

		case 8:
			{
				this->IncreaseAddStat(VITALITY, value);
			} break;

		case 9:
			{
				this->IncreaseAddStat(ENERGY, value);
			} break;

		case 10:
			{
				this->IncreaseAddStat(AGILITY, value);
			} break;
		}
	}
}

void Player::ApplyWingErrtelOption(bool stat)
{
	Item const* pPentagram = this->GetInventory()->GetItem(PENTAGRAM_SLOT);

	if (!pPentagram || !pPentagram->IsItem() || !pPentagram->IsValid())
	{
		return;
	}

	if (stat)
	{
		return;
	}

	Item const* pWings = this->GetInventory()->GetItem(WINGS);

	if (!pWings || !pWings->IsItem() || !pWings->IsValid() || !pWings->IsWingLvl4())
	{
		return;
	}

	if (pWings->GetSocket(4) != SOCKET_SLOT_NONE)
	{
		uint8 id = pWings->GetSocket(4) / 16;
		uint8 level = pWings->GetSocket(4) % 16;

		int32 value = sItemMgr->GetWingSocketOptionValue(1, id, level);

		if (value > 0)
		{
			switch (id)
			{
			case 0:
				{
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, value);
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, value);
				} break;

			case 1:
				{
					  this->IncreaseIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, value);
				} break;

			case 2:
				{
					  this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, value);
				} break;

			case 3:
				{
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, value);
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, value);
				} break;

			case 4:
				{
					this->PercentIntData(UNIT_INT_ELEMENTAL_DEFENSE, value);
				} break;

			case 5:
				{
					  this->IncreaseIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, value);
				} break;

			case 6:
				{
					  this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, value);
				} break;
			}
		}
	}

	if (pWings->GetSocketBonus() != 0xFF)
	{
		uint8 id = pWings->GetSocketBonus() / 16;
		uint8 level = pWings->GetSocketBonus() % 16;

		int32 value = sItemMgr->GetWingSocketOptionValue(0, id, level);

		if (value > 0)
		{
			switch (id)
			{
			case 0:
				{
					this->IncreaseIntData(UNIT_INT_ELEMENTAL_DEFENSE, value);
				} break;
			}
		}
	}
}

void Player::ApplyEarringOption(uint8 slot, bool stat)
{
	Item const* pItem = this->GetInventory()->GetItem(slot);

	if (!pItem || !pItem->IsItem() || !pItem->IsValid())
	{
		return;
	}

	item_earring const* pEarringData = sItemMgr->GetEarringData(pItem->GetItem());

	if (!pEarringData)
	{
		return;
	}

	SOCKET_SLOT_LOOP(i)
	{
		if (pItem->GetSocket(i) == SOCKET_SLOT_NONE)
		{
			continue;
		}

		this->ApplyEarringOption(pItem->GetLevel(), pEarringData->GetGroup(), pItem->GetSocket(i) / MAX_EARRING_OPTION_LEVEL, pItem->GetSocket(i) % MAX_EARRING_OPTION_LEVEL, stat);
	}
}

void Player::ApplyEarringOption(uint8 level, uint8 group, uint8 option, uint8 value, bool stat)
{
	if (stat)
	{
		if (option != 0 &&
			option != 1 &&
			option != 2 &&
			option != 3 &&
			option != 4 &&
			option != 24)
		{
			return;
		}
	}
	else
	{
		if (option == 0 ||
			option == 1 ||
			option == 2 ||
			option == 3 ||
			option == 4 ||
			option == 24)
		{
			return;
		}
	}

	item_earring_option_value const* pEarringValue = sItemMgr->GetEarringOptionValue(value, group);

	if (!pEarringValue)
	{
		return;
	}

	switch (option)
	{
	case 0:
		{
			this->IncreaseAddStat(STRENGTH, pEarringValue->GetValue());
		} break;

	case 1:
		{
			this->IncreaseAddStat(AGILITY, pEarringValue->GetValue());
		} break;

	case 2:
		{
			this->IncreaseAddStat(ENERGY, pEarringValue->GetValue());
		} break;

	case 3:
		{
			this->IncreaseAddStat(VITALITY, pEarringValue->GetValue());
		} break;

	case 4:
		{
			if (this->GetClass() == Character::DARK_LORD)
			{
				this->IncreaseAddStat(LEADERSHIP, pEarringValue->GetValue());
			}
		} break;

	case 5:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, pEarringValue->GetValue());
		} break;

	case 6:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, pEarringValue->GetValue());
		} break;

	case 7:
		{
			//7	"Increases wizardry"	2
			  ///- TODO:
		} break;

	case 8:
		{
			//8	"Increases damage"	2
			  ///- TODO:
		} break;

	case 9:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, pEarringValue->GetValue());
		} break;

	case 10:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, pEarringValue->GetValue());
		} break;

	case 11:
		{
			this->PowerIncreseAdd(POWER_LIFE, pEarringValue->GetValue(), false);
		} break;

	case 12:
		{
			this->PowerIncreseAdd(POWER_MANA, pEarringValue->GetValue(), false);
		} break;

	case 13:
		{
			this->PowerIncreseAdd(POWER_STAMINA, pEarringValue->GetValue(), false);
		} break;

	case 14:
		{
			this->IncreaseDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE, pEarringValue->GetValue());
		} break;

	case 15:
		{
			this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, pEarringValue->GetValue());
		} break;

	case 16:
		{
			this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, pEarringValue->GetValue());
		} break;

	case 17:
		{
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, pEarringValue->GetValue());
		} break;

	case 18:
		{
			this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, pEarringValue->GetValue());
		} break;

	case 19:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, pEarringValue->GetValue());
		} break;

	case 20:
		{
			this->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, pEarringValue->GetValue());
		} break;

	case 21:
		{
			this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, pEarringValue->GetValue());
		} break;

	case 22:
		{
			this->IncreaseData(PLAYER_DATA_SHIELD_DEFENSE_INCREASE_PERCENT, pEarringValue->GetValue());
		} break;

	case 23:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, pEarringValue->GetValue());
		} break;

	case 26:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pEarringValue->GetValue());
		} break;

	case 27:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pEarringValue->GetValue());
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pEarringValue->GetValue());
		} break;

	case 24:
		{
			this->IncreaseAddStat(STRENGTH, pEarringValue->GetValue());
			this->IncreaseAddStat(AGILITY, pEarringValue->GetValue());
			this->IncreaseAddStat(ENERGY, pEarringValue->GetValue());
			this->IncreaseAddStat(VITALITY, pEarringValue->GetValue());

			if (this->GetClass() == Character::DARK_LORD)
			{
				this->IncreaseAddStat(LEADERSHIP, pEarringValue->GetValue());
			}
		} break;

	case 28:
		{
			   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
			   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
			   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
			   this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
			   this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
			   this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
		} break;

	case 29:
		{
			   this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
		} break;

	case 30:
		{
			   this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, sFormulaMgr->GetOrCreateValue(9, pEarringValue->GetValue(), level));
		} break;
	}
}

void Player::ApplyEarringBonus(bool stat)
{
	Item const* pItem01 = this->GetInventory()->GetItem(EARRING_01);
	Item const* pItem02 = this->GetInventory()->GetItem(EARRING_02);

	if (!pItem01 || !pItem01->IsItem() || !pItem01->IsValid())
	{
		return;
	}

	if (!pItem02 || !pItem02->IsItem() || !pItem02->IsValid())
	{
		return;
	}

	item_earring const* pEarring01 = sItemMgr->GetEarringData(pItem01->GetItem());
	item_earring const* pEarring02 = sItemMgr->GetEarringData(pItem02->GetItem());

	if (!pEarring01 || !pEarring02)
	{
		return;
	}

	if (pEarring01->GetGroup() != pEarring02->GetGroup())
	{
		return;
	}

	item_earring_bonus const* pBonus = sItemMgr->GetEarringBonus(pEarring01->GetLink(), pEarring02->GetLink());

	if (!pBonus)
	{
		return;
	}

	for (int32 i = 0; i < MAX_EARRING_BONUS; ++i)
	{
		this->ApplyEarringOption(0, pEarring01->GetGroup(), pBonus->GetOption(i), pBonus->GetValue(i), stat);
	}
}

void Player::CalculateMaxHP()
{
	CharacterBaseData const* base_data = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !base_data )
		return;

	this->PowerSetMax(POWER_LIFE, base_data->GetLife() + 
					 ((this->GetTotalLevel() - 1) * base_data->GetLevelToLife()) + 
					 ((this->GetTotalStat(VITALITY) - base_data->GetStat(VITALITY)) * base_data->GetVitalityToLife()));
}

void Player::CalculateMaxMana()
{
	CharacterBaseData const* base_data = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !base_data )
		return;

	this->PowerSetMax(POWER_MANA, base_data->GetMana() + 
					 ((this->GetTotalLevel() - 1) * base_data->GetLevelToMana()) + 
					 ((this->GetTotalStat(ENERGY) - base_data->GetStat(ENERGY)) * base_data->GetEnergyToMana()));
}

void Player::CalculateMaxShield()
{
	int32 iExpressionA = this->GetTotalStat(STRENGTH) + this->GetTotalStat(AGILITY) + this->GetTotalStat(VITALITY) + this->GetTotalStat(ENERGY) + this->GetTotalStat(LEADERSHIP);
	int32 iExpressionB = (this->GetTotalLevel() * this->GetTotalLevel()) / 30;
	this->PowerSetMax(POWER_SHIELD, (iExpressionA * 12 / 10) + iExpressionB + this->GetIntData(UNIT_INT_DEFENSE));
}

void Player::CalculateMaxStamina()
{
	float stamina = 0;

	switch (this->GetClass())
	{
	case Character::DARK_WIZARD:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.2f) + (this->GetTotalStat(AGILITY) * 0.4f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 0.2f);
	} break;

	case Character::DARK_KNIGHT:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.15f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 1.0f);
	} break;

	case Character::FAIRY_ELF:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.3f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 0.2f);
	} break;

	case Character::MAGIC_GLADIATOR:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.2f) + (this->GetTotalStat(AGILITY) * 0.25f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 0.15f);
	} break;

	case Character::DARK_LORD:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.3f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.1f) + (this->GetTotalStat(ENERGY) * 0.15f) + (this->GetTotalStat(LEADERSHIP) * 0.3f);
	} break;

	case Character::SUMMONER:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.3f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 0.2f);
	} break;

	case Character::RAGE_FIGHTER:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.15f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 1.0f);
	} break;

	case Character::GROW_LANCER:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.15f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 1.0f);
	} break;

	case Character::RUNE_WIZARD:
	{
		stamina = (this->GetTotalStat(STRENGTH) / 5) + (this->GetTotalStat(AGILITY) / 5) + (this->GetTotalStat(VITALITY) / 3) + (this->GetTotalStat(ENERGY) * 0.75f);
	} break;

	case Character::SLAYER:
	{
		stamina = (this->GetTotalStat(STRENGTH) * 0.15f) + (this->GetTotalStat(AGILITY) * 0.2f) + (this->GetTotalStat(VITALITY) * 0.3f) + (this->GetTotalStat(ENERGY) * 1.0f);
	} break;
	
	case Character::GUN_CRUSHER:
	{
		stamina = (this->GetTotalStat(STRENGTH) / 5) + (this->GetTotalStat(AGILITY) / 5) + (this->GetTotalStat(VITALITY) / 3) + (this->GetTotalStat(ENERGY) * 0.75f);
	} break;
	}

	this->PowerSetMax(POWER_STAMINA, static_cast<int32>(stamina));
}

void Player::SendStats(bool flag)
{
	this->GetTimer(PLAYER_TIMER_SPEED_SEND)->Start();

	CHARACTER_ATTACK_SPEED_SEND pMsg(this->GetIntData(UNIT_INT_ATTACK_SPEED), this->GetIntData(UNIT_INT_MAGIC_SPEED));
	this->sendPacket(MAKE_PCT(pMsg));

	if ( !flag )
	{
		return;
	}

	CHARACTER_NEW_STAT pMsg3;

	int32 str = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_STRENGTH1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_STRENGTH2));
	int32 agi = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_DEXTERITY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_DEXTERITY2));
	int32 vit = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_VITALITY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_VITALITY2));
	int32 ene = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_ENERGY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_ENERGY2));
	int32 cmd = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_LEADERSHIP));

	pMsg3.strength = this->GetStat(STRENGTH) + str;
	pMsg3.strength_add = this->GetAddStat(STRENGTH) - str;
	pMsg3.agility = this->GetStat(AGILITY) + agi;
	pMsg3.agility_add = this->GetAddStat(AGILITY) - agi;
	pMsg3.vitality = this->GetStat(VITALITY) + vit;
	pMsg3.vitality_add = this->GetAddStat(VITALITY) - vit;
	pMsg3.energy = this->GetStat(ENERGY) + ene;
	pMsg3.energy_add = this->GetAddStat(ENERGY) - ene;
	pMsg3.leadership = this->GetStat(LEADERSHIP) + cmd;
	pMsg3.leadership_add = this->GetAddStat(LEADERSHIP) - cmd;

	// TODO: Completar los datos
	pMsg3.sd_recovery_when_attacked = 0;
	pMsg3.sd_ignore_rate = this->GetData(PLAYER_DATA_IGNORE_SD_RATE);
	pMsg3.sd_recovery_when_attack = 0;
	pMsg3.zen_obtained_rate = this->GetData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN);
	pMsg3.defense_ignore_rate = this->GetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE);
	pMsg3.hp_recovery_rate = this->GetDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE);
	pMsg3.mp_recovery_rate = this->GetDataFloat(PLAYER_FLOAT_MP_RECOVERY_RATE);
	pMsg3.stun_rate = this->GetFloatData(UNIT_FLOAT_STUN_RATE);
	pMsg3.stun_resistance_rate = this->GetFloatData(UNIT_FLOAT_RESIST_STUN);
	pMsg3.shield_absorb = this->GetDataFloat(PLAYER_DATA_SHIELD_PROTECTION);
	pMsg3.monster_attack_hp_recovery = this->GetData(PLAYER_DATA_MONSTER_DIE_HP);
	pMsg3.monster_attack_mp_recovery = this->GetData(PLAYER_DATA_MONSTER_DIE_MP);
	pMsg3.monster_attack_sd_recovery = this->GetData(PLAYER_DATA_MONSTER_DIE_SD);
	pMsg3.sd_recovery_rate = this->GetDataFloat(PLAYER_FLOAT_SD_RECOVERY_RATE);
	pMsg3.restore_all_mp_rate = this->GetFloatData(UNIT_FLOAT_TOTAL_MANA_RECOVERY) + this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_MP_RECOVERY);
	pMsg3.restore_all_hp_rate = this->GetFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY) + this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_HP_RECOVERY);
	pMsg3.restore_all_sd_rate = this->GetFloatData(UNIT_FLOAT_TOTAL_SHIELD_RECOVERY) + this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_SD_RECOVERY);
	pMsg3.unk_data_1 = 0;
	pMsg3.unk_data_2 = 0;
	pMsg3.ag_recovery_rate = this->GetDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE);
	pMsg3.damage_absorb_rate = this->GetAbsorbDamageRate();

	pMsg3.shield_block_rate = this->GetDataFloat(PLAYER_DATA_SHIELD_BLOCK);
	pMsg3.weapon_block_rate = this->GetDataFloat(PLAYER_DATA_WEAPON_BLOCK);
	pMsg3.hp_absorb_rate = this->GetDataFloat(PLAYER_DATA_ABSORB_HP);
	pMsg3.sd_absorb = this->GetDataFloat(PLAYER_DATA_ABSORB_SD);
	pMsg3.return_damage_rate = this->GetFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE);
	pMsg3.damage_increase_rate = this->GetIncreaseDamageRate();
	pMsg3.mp_use_reduction_rate = this->GetDataFloat(PLAYER_FLOAT_MANA_REDUCTION);
	pMsg3.critical_damage_rate = this->GetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE);
	pMsg3.excellent_damage_rate = this->GetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE);
	pMsg3.double_damage_rate = this->GetFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE);
	pMsg3.damage_reduction_rate = this->GetIntData(UNIT_INT_DAMAGE_DECREASE);
	pMsg3.damage_reflect_rate = this->GetFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE);
	pMsg3.ag_use_reduction_rate = this->GetStaminaReduction();
	pMsg3.excellent_damage_increase = this->GetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD);
	pMsg3.critical_damage_increase = this->GetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD);
	pMsg3.skill_attack_damage_increase = this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE) + this->GetData(PLAYER_DATA_MAJESTIC_SKILL_DAMAGE);
	pMsg3.basic_defense = this->GetIntData(UNIT_INT_DEFENSE_BASE);
	pMsg3.triple_damage_rate = this->GetFloatData(UNIT_FLOAT_TRIPLE_DAMAGE_RATE);
	pMsg3.damage_reduction_amount = this->GetIntData(UNIT_INT_DAMAGE_DECREASE_ADD);
	
	this->sendPacket(MAKE_PCT(pMsg3));

	CHARACTER_ELEMENTAL_DATA pMsg4;

	this->GetPreviewElementalDamage(pMsg4.elemental_damage_min, pMsg4.elemental_damage_max, pMsg4.elemental_pvp_damage_min, pMsg4.elemental_pvp_damage_max);
	this->GetPreviewElementalDefense(pMsg4.elemental_defense, pMsg4.elemental_pvp_defense);
	this->GetPreviewElementalAttackSuccessRate(pMsg4.elemental_attack_success_rate, pMsg4.elemental_pvp_attack_success_rate);
	this->GetPreviewElementalDefenseSuccessRate(pMsg4.elemental_defense_success_rate, pMsg4.elemental_pvp_defense_success_rate);

	pMsg4.data_1 = 0;
	pMsg4.data_2 = 0;
	pMsg4.data_3 = 0;
	pMsg4.data_4 = 0;
	pMsg4.data_5 = 0;
	pMsg4.data_6 = 0;
	pMsg4.data_7 = 0;
	pMsg4.data_8 = 0;
	pMsg4.critical_elemental_dmg_rate = this->GetPentagramOption()->GetAddElementalCriticalDamageRate();
	pMsg4.increase_damage_pve = 0;
	pMsg4.increase_damage_pvp = 0;
	pMsg4.absorb_damage_pve = 0;
	pMsg4.absorb_damage_pvp = 0;
	pMsg4.absorb_shield = this->GetPentagramJewelOption()->GetAddElementalShieldAbsorbRate(); // Absorb shield
	pMsg4.absorb_hp = this->GetPentagramJewelOption()->GetAddElementalLifeAbsorbRate(); // Absorb hp
	pMsg4.bleeding_dmg = this->GetPentagramJewelOption()->GetElementalBleeding(); // Bleeding Dmg
	pMsg4.paralyzing = this->GetPentagramJewelOption()->GetElementalParalysisRate(); // Paralyzing ( sec )
	pMsg4.bind = this->GetPentagramJewelOption()->GetElementalBindingRate(); // Bind ( sec )
	pMsg4.punish = this->GetPentagramJewelOption()->GetElementalPunishRate(); // Punish ( add dmg )
	pMsg4.blind = this->GetPentagramJewelOption()->GetElementalBlindness(); // Blind ( redd atk )
	pMsg4.res_to_str_elem = 0; // Res to Str Elem
	pMsg4.res_to_elem_dmg = 0; // Res to Elem Dmg
	pMsg4.elemental_add_attack_damage = this->GetPentagramOption()->GetAddElementalAttackTransferRate();
	pMsg4.elemental_add_defense = this->GetPentagramOption()->GetAddElementalDefenseTransferRate();

	this->SEND_PCT(pMsg4);

	/*
	 1 = Attack Power
	 2 = Attack Rate
	 3 = Attack Rate PVP
	 4 = Defense
	 5 = 
	 6 = Defense Rate
	 7 = Defense Rate PVP
	 8 =
	 9 = Magic Damage
	 10 = Curse Damage
	*/

	CHARACTER_SPECIALIZATION_STAT pMsg5;

	int32 spec_count = 0;
	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MIN) > 0 || this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MAX) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 1;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MIN);
		pMsg5.data[spec_count].spl_value2 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MAX);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 2;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE_PVP) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 3;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE_PVP);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 4;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 6;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE_PVP) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 7;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE_PVP);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MIN) > 0 || this->GetSpecializationStat(CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MAX) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 9;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MIN);
		pMsg5.data[spec_count].spl_value2 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MAX);
		spec_count++;
	}

	if ( this->GetSpecializationStat(CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MIN) > 0 || this->GetSpecializationStat(CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MAX) > 0 && spec_count < 5 )
	{
		pMsg5.data[spec_count].type = 10;
		pMsg5.data[spec_count].spl_value1 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MIN);
		pMsg5.data[spec_count].spl_value2 = this->GetSpecializationStat(CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MAX);
		spec_count++;
	}

	spec_count = 0;

	this->SEND_PCT(pMsg5);

	CHARACTER_STAT_DATA pMsg6;
	pMsg6.strength = this->GetStat(STRENGTH) + str;
	pMsg6.strength_add = this->GetAddStat(STRENGTH) - str;
	pMsg6.agility = this->GetStat(AGILITY) + agi;
	pMsg6.agility_add = this->GetAddStat(AGILITY) - agi;
	pMsg6.vitality = this->GetStat(VITALITY) + vit;
	pMsg6.vitality_add = this->GetAddStat(VITALITY) - vit;
	pMsg6.energy = this->GetStat(ENERGY) + ene;
	pMsg6.energy_add = this->GetAddStat(ENERGY) - ene;
	pMsg6.leadership = this->GetStat(LEADERSHIP) + cmd;
	pMsg6.leadership_add = this->GetAddStat(LEADERSHIP) - cmd;

	this->SEND_PCT(pMsg6);

	this->AttackSpeedSend();

	this->SendExperienceEvent();
}

bool Player::IsMaster() const
{
	return this->GetChangeUP(1) == 1;
}

bool Player::IsMajestic() const
{
	return this->GetChangeUP(2) == 1;
}

void Player::CalculateNextExperience()
{
	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( this->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( this->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	this->GetLevelData(level_type)->SetNextExperience(sCharacterBase->GetCharacterExperience(level_type, this->GetLevelData(level_type)->GetLevel() + 1));
}

int64 Player::GetNextExperience(int16 add_level) const
{
	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( this->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( this->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	return sCharacterBase->GetCharacterExperience(level_type, this->GetLevelData(level_type)->GetLevel() + add_level);
}

void Player::AutoRefill()
{
	POWER_LOOP(i)
	{
		this->IncreasePowerFillCount(i, 1);
	}

	if ( this->GetPowerFillCount(POWER_LIFE) > (this->IsRest() ? sGameServer->GetAutoRecuperationCountRest(POWER_LIFE): sGameServer->GetAutoRecuperationCount(POWER_LIFE)) )
	{
		this->SetPowerFillCount(POWER_LIFE, 0);

		if ( this->PowerGet(POWER_LIFE) < this->PowerGetTotal(POWER_LIFE) )
		{
			float rate = this->GetDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE);

			if ( sGameServer->IsAutoRecuperationEnabled(POWER_LIFE) && this->GetTotalLevel() <= sGameServer->GetAutoRecuperationMaxLevel(POWER_LIFE) )
			{
				rate += this->GetPowerAutorecuperationTime(POWER_LIFE)->GetElapsed() > 5000 ? 5 : 0;
			}

			int32 value = this->PowerGetTotal(POWER_LIFE) * rate / 100;

			value += this->GetData(PLAYER_DATA_HP_RECOVERY_ADD);

			if ( this->HasBuff(BUFF_PARALYSIS) )
			{
				value -= value * sGameServer->GetRadianceParalysisEffect() / 100;
			}

			if ( value < 0 )
			{
				value = 0;
			}

			this->PowerIncrease(POWER_LIFE, value, true);
		}
	}

	if ( this->GetPowerFillCount(POWER_SHIELD) > (this->IsRest() ? sGameServer->GetAutoRecuperationCountRest(POWER_SHIELD): sGameServer->GetAutoRecuperationCount(POWER_SHIELD)) )
	{
		this->SetPowerFillCount(POWER_SHIELD, 0);

		if ( this->PowerGet(POWER_SHIELD) < this->PowerGetTotal(POWER_SHIELD) )
		{
			float rate = this->GetDataFloat(PLAYER_FLOAT_SD_RECOVERY_RATE);

			if ( this->GetData(PLAYER_DATA_SHIELD_REFILL_ON) != 0 || this->IsInSafeZone() )
			{
				if ( sGameServer->IsAutoRecuperationEnabled(POWER_SHIELD) && this->GetTotalLevel() <= sGameServer->GetAutoRecuperationMaxLevel(POWER_SHIELD) )
				{
					rate += this->GetPowerAutorecuperationTime(POWER_SHIELD)->GetElapsed() > 5000 ? 5 : 0;
				}
			}

			int32 value = this->PowerGetTotal(POWER_SHIELD) * rate / 100;

			value += this->GetData(PLAYER_DATA_SD_RECOVERY_ADD);

			if ( this->HasBuff(BUFF_PARALYSIS) )
			{
				value -= value * sGameServer->GetRadianceParalysisEffect() / 100;
			}

			if ( value < 0 )
			{
				value = 0;
			}

			this->PowerIncrease(POWER_SHIELD, value, true);
		}
	}

	if ( this->GetPowerFillCount(POWER_MANA) > (this->IsRest() ? sGameServer->GetAutoRecuperationCountRest(POWER_MANA): sGameServer->GetAutoRecuperationCount(POWER_MANA)) )
	{
		this->SetPowerFillCount(POWER_MANA, 0);

		if ( this->PowerGet(POWER_MANA) < this->PowerGetTotal(POWER_MANA) )
		{
			float rate = this->GetDataFloat(PLAYER_FLOAT_MP_RECOVERY_RATE);

			if ( sGameServer->IsAutoRecuperationEnabled(POWER_MANA) && this->GetTotalLevel() <= sGameServer->GetAutoRecuperationMaxLevel(POWER_MANA) )
			{
				rate += this->GetPowerAutorecuperationTime(POWER_MANA)->GetElapsed() > 5000 ? 3 : 0;
			}

			int32 value = this->PowerGetTotal(POWER_MANA) * rate / 100;

			value += this->GetData(PLAYER_DATA_MP_RECOVERY_ADD);

			if ( this->HasBuff(BUFF_PARALYSIS) )
			{
				value -= value * sGameServer->GetRadianceParalysisEffect() / 100;
			}

			if ( value < 0 )
			{
				value = 0;
			}
			
			this->PowerIncrease(POWER_MANA, value, true);
		}
	}

	if ( this->GetPowerFillCount(POWER_STAMINA) > (this->IsRest() ? sGameServer->GetAutoRecuperationCountRest(POWER_STAMINA): sGameServer->GetAutoRecuperationCount(POWER_STAMINA)) )
	{
		this->SetPowerFillCount(POWER_STAMINA, 0);

		if ( this->PowerGet(POWER_STAMINA) < this->PowerGetTotal(POWER_STAMINA) )
		{
			float rate = this->GetDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE);

			if ( sGameServer->IsAutoRecuperationEnabled(POWER_STAMINA) && this->GetTotalLevel() <= sGameServer->GetAutoRecuperationMaxLevel(POWER_STAMINA) )
			{
				rate += this->GetPowerAutorecuperationTime(POWER_STAMINA)->GetElapsed() > 5000 ? 3 : 0;
			}

			int32 value = this->PowerGetTotal(POWER_STAMINA) * rate / 100;

			value += this->GetData(PLAYER_DATA_AG_RECOVERY_ADD);

			if ( this->HasBuff(BUFF_PARALYSIS) )
			{
				value -= value * sGameServer->GetRadianceParalysisEffect() / 100;
			}

			if ( value < 0 )
			{
				value = 0;
			}

			this->PowerIncrease(POWER_STAMINA, value, true);
		}
	}
}

bool Player::IsSpecialCharacter()
{
	return (this->GetClass() == Character::MAGIC_GLADIATOR || 
			this->GetClass() == Character::DARK_LORD ||
			this->GetClass() == Character::RAGE_FIGHTER ||
			this->GetClass() == Character::GROW_LANCER);
}

void Player::UpdateInterface()
{
	if ( this->GetInterfaceState()->GetID() == InterfaceData::None )
		return;

	if ( this->GetInterfaceState()->GetID() == InterfaceData::DuelGate )
	{
		sDuelMgr->SendDuelRoomStatus(this);
	}

	if ( this->GetInterfaceState()->GetState() )
		return;

	if ( !this->GetInterfaceState()->GetTime() )
		return;

	if ( GetTickCount() < this->GetInterfaceState()->GetTime() )
		return;
	
	if ( this->GetInterfaceState()->GetID() == InterfaceData::Party )
	{
		this->PartyRequestResult(PARTY_REQUEST_FAILED);
	}
	else if ( this->GetInterfaceState()->GetID() == InterfaceData::Trade )
	{
		this->TradeResult(TRADE_OVER_TIME_AUTO_CANCEL);
	}
	else if ( this->GetInterfaceState()->GetID() == InterfaceData::Duel )
	{
		this->SetDueling(false);
		this->SetDuelScore(0);
		this->SetDuelRoom(0);
		this->SetDuelClassic(false);
		this->SetDuelTarget(nullptr);
	}
	else if ( this->GetInterfaceState()->GetID() == InterfaceData::GuildWarRequest )
	{
		sGuildWarMgr->StartWarRequestResult(this, 0);
	}

	this->GetInterfaceState()->Reset();
}

void Player::ClearItem(uint8 slot, uint8 send)
{
	this->GetInventory()->ItemSet(slot, 0);
	this->GetInventory()->GetItem(slot)->clear(false);
	this->SendInventory(slot, send, true);
}

uint8 Player::GetManaPotionSlot()
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

	return -1;
}

void Player::DecreaseItemDurabilityByUse(uint8 position, float durability)
{
	if ( !this->GetInventory()->GetItem(position)->IsItem() )
		return;

	this->GetInventory()->GetItem(position)->AlterDurability(0, durability);

	if ( this->GetInventory()->GetItem(position)->GetDurability() > 0.0f )
	{
		this->SendItemDurability(position, 1);
	}
	else
	{
		this->ClearItem(position);
	}
}

void Player::ItemDelete(uint16 item, uint8 level, int32 count)
{
	if ( count <= 0 )
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(item);

	if ( !item_info )
	{
		return;
	}

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		Item * pItem = this->GetInventory()->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( !pItem->IsItem() )
		{
			continue;
		}

		if (pItem->GetItem() == item && (pItem->GetLevel() == level || level == uint8(-1)))
		{
			if (item_info->GetStackData()->GetMaxStack() > 0)
			{
				if (pItem->GetDurability() > count)
				{
					pItem->AlterDurability(0, count);
					this->SendItemDurability(i, 1);
					count = 0;
				}
				else
				{
					count -= pItem->GetDurability();
					this->ClearItem(i);
				}
			}
			else
			{
				this->ClearItem(i);
				--count;
			}

			if (count <= 0)
			{
				break;
			}
		}
	}
}

void Player::ItemDeleteByUse(uint8 slot)
{
	if (!inventory_range(slot))
	{
		return;
	}

	Item* pItem = this->GetInventory()->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		return;
	}

	item_template const* pItemInfo = sItemMgr->GetItem(pItem->GetItem());

	if (!pItemInfo)
	{
		return;
	}

	if (pItemInfo->GetStackData()->GetMaxStack() > 0)
	{
		pItem->AlterDurability(0, 1);

		if (pItem->GetDurability() > 0.0f)
		{
			this->SendItemDurability(slot, 1);
			this->SendInventory(slot);
		}
		else
		{
			this->ClearItem(slot);
		}
	}
	else
	{
		this->ClearItem(slot);
	}
}

bool Player::UpgradeItemCommon(uint8 source_slot, uint8 target_slot)
{
	if ( !use_inventory_range(source_slot) ||
		 !use_inventory_range(target_slot) )
		return false;

	if ( !this->GetInventory()->GetItem(source_slot)->IsItem() ||
		 !this->GetInventory()->GetItem(target_slot)->IsItem() )
		 return false;

	if ( !this->GetInventory()->CanUseExpandedSlot(source_slot) ||
		 !this->GetInventory()->CanUseExpandedSlot(target_slot) )
		 return false;

	return true;
}

void Player::UpgradeItemSuccess(uint8 source_slot, uint8 target_slot)
{
	this->ItemDeleteByUse(source_slot);

	this->SendInventory(target_slot);
}

bool Player::UpgradeItemUsingBless(uint8 source_slot, uint8 target_slot)
{
	if ( !this->UpgradeItemCommon(source_slot, target_slot) )
		return false;

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	Item * source_item = this->GetInventory()->GetItem(source_slot);
	
	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if ( !item_info )
		return false;

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_BLESS) )
		return false;

	if ( target_item->GetExpireDate() > 0 )
		return false;

	if( target_item->GetItem() == ITEMGET(0, 41) ) // Pick Axe
	{
		if (target_item->GetDurability() >= item_info->GetDurability())
		{
			return false;
		}

		target_item->AlterDurability(1, 4);

		goto upgrade_success;
	}
	else if( target_item->GetItem() == ITEMGET(13, 268) ) // Dark Transformation Ring
	{
		if (target_item->GetDurability() >= item_info->GetDurability())
		{
			return false;
		}

		target_item->AlterDurability(1, 50);

		goto upgrade_success;
	}
	else if (item_info->GetKind2() == ItemKind::GUARDIAN_MOUNT) // Ghost Horse
	{
		if (target_item->GetDurability() >= item_info->GetDurability())
		{
			return false;
		}

		target_item->AlterDurability(2, item_info->GetDurability());

		goto upgrade_success;
	}
	else if ( target_item->GetItem() == PET_FENRIR )
	{
		if (target_item->GetDurability() >= item_info->GetDurability())
		{
			return false;
		}

		if (roll_chance_i(sGameServer->GetFenrirRepairRate(), 10000))
		{
			float add_durability = static_cast<float>(RANDOM(150) + 50);

			target_item->AlterDurability(1, add_durability);

			this->SendNotice(NOTICE_NORMAL_BLUE,"[FENRIR REPAIR] It's life Successfully Regenerated (%d/255).", int32(target_item->GetDurability()));
		}
		else
		{
			this->SendNotice(NOTICE_NORMAL_BLUE,"[FENRIR REPAIR] It's life Failed to Regenerate (%d).", int32(target_item->GetDurability()));
		}

		goto upgrade_success;
	}

	if ( target_item->GetLevel() >= 6 )
	{
		return false;
	}

	target_item->IncreaseLevel(1);

	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

upgrade_success:	

	sLog->outInfo("jewel", "[ Jewel of Bless ] Success: [Source: %d (%u/ %u/ %u) / Target: %d (%d / %d) (%u/ %u/ %u)] --- %s", 
		source_slot, source_item->GetSerialServer(), source_item->GetSerial(), source_item->GetSerialShop(), 
		target_slot, target_item->GetItem(), target_item->GetLevel(), target_item->GetSerialServer(), target_item->GetSerial(), target_item->GetSerialShop(), 
		this->BuildLog().c_str());

	this->UpgradeItemSuccess(source_slot, target_slot);
	
	return true;
}
	
bool Player::UpgradeItemUsingSoul(uint8 source_slot, uint8 target_slot)
{
	if (!sGameServer->IsJewelOfSoulEnabled())
		return false;

	if ( !this->UpgradeItemCommon(source_slot, target_slot) )
		return false;

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	Item * source_item = this->GetInventory()->GetItem(source_slot);
	
	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if ( !item_info )
		return false;

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_SOUL) )
		return false;

	if ( target_item->GetLevel() >= 9 )
		return false;

	int32 success = sGameServer->GetJewelOfSoulRate();

	if (target_item->GetLuck())
	{
		success += sGameServer->GetJewelOfSoulRateLuck();
	}

	bool is_success = false;

	if (roll_chance_i(success))
	{
		is_success = true;
		target_item->IncreaseLevel(1);
	}
	else
	{
		if ( target_item->GetLevel() >= 7 )
		{
			target_item->SetLevel(0);
		}
		else if ( target_item->GetLevel() > 0 )
		{
			target_item->ReduceLevel(1);
		}
	}

	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

	sLog->outInfo("jewel", "[ Jewel of Soul ] %s - Rate: %d: [Source: %d (%u/ %u/ %u) / Target: %d (%d / %d) (%u/ %u/ %u)] --- %s", 
		is_success ? "Success" : "Fail", success,
		source_slot, source_item->GetSerialServer(), source_item->GetSerial(), source_item->GetSerialShop(), 
		target_slot, target_item->GetItem(), target_item->GetLevel(), target_item->GetSerialServer(), target_item->GetSerial(), target_item->GetSerialShop(), 
		this->BuildLog().c_str());

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}
	
bool Player::UpgradeItemUsingLife(uint8 source_slot, uint8 target_slot)
{
	if (!sGameServer->IsJewelOfLifeEnabled())
	{
		return false;
	}

	if (!this->UpgradeItemCommon(source_slot, target_slot))
	{
		return false;
	}

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	
	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if (!item_info)
	{
		return false;
	}

	if (!item_info->IsFlag(ITEM_FLAG_ALLOW_LIFE))
	{
		return false;
	}

	if (target_item->GetOption() >= sGameServer->GetJewelOfLifeMax())
	{
		return false;
	}

	if (target_item->GetOption() == 0)
	{
		if (target_item->IsWingLvl2())
		{
			target_item->AndExe(~0x20);

			if (RANDOM(2))
			{
				target_item->OrExe(0x20);
			}
		}
		else if (target_item->IsWingLvl3() || target_item->IsWingLvl4())
		{
			target_item->AndExe(~0x10);
			target_item->AndExe(~0x20);

			switch (Random(3))
			{
			case 1:
				{
					target_item->OrExe(0x10);
				} break;

			case 2:
				{
					target_item->OrExe(0x20);
				} break;

			default:
				{
				} break;
			}
		}
		else if (target_item->IsWingMonster())
		{
			target_item->AndExe(~0x10);

			if (RANDOM(2))
			{
				target_item->OrExe(0x10);
			}
		}
	}
	
	int32 rate = Random(sGameServer->jewel_of_life_max_rate.get());
	int32 success = sGameServer->jewel_of_life_rate_normal.get();

	if (target_item->GetExe())
	{
		success = sGameServer->jewel_of_life_rate_exe.get();
	}

	if (target_item->IsAncient())
	{
		success = sGameServer->jewel_of_life_rate_ancient.get();
	}

	if (target_item->IsSocket())
	{
		success = sGameServer->jewel_of_life_rate_socket.get();
	}

	if (rate < success)
	{
		target_item->SetOption(target_item->GetOption() + 1);
	}
	else
	{
		target_item->SetOption(0);
	}

	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}

bool Player::UpgradeItemUsingHarmony(uint8 source_slot, uint8 target_slot)
{
	if ( !sGameServer->IsHarmonyEnabled() )
	{
		return false;
	}

	if ( !this->UpgradeItemCommon(source_slot, target_slot) )
	{
		return false;
	}

	Item * target_item = this->GetInventory()->GetItem(target_slot);

	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if ( !item_info )
	{
		return false;
	}

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_HARMONY) )
	{
		return false;
	}

	if ( target_item->IsHarmonyUpgrade() )
	{
		return false;
	}
	
	if ( target_item->IsAncient() && !sGameServer->IsHarmonyOnAncient() )
	{
		return false;
	}

	HarmonyItemType type = target_item->GetHarmonyItem();

	if ( type == HARMONY_ITEM_NONE )
	{
		return false;
	}

	item_harmony_data const* harmony_data = sItemMgr->GetRandomHarmonyOption(target_item);

	if ( !harmony_data )
	{
		return false;
	}
	
	if ( RANDOM(100) >= sGameServer->harmony_strengthen_rate.get() )
	{
		this->SendNotice(NOTICE_NORMAL_BLUE, "Regeneration gem strengthening defeat.");
		goto harmony_clear_item;
	}

	target_item->SetHarmony(HARMONY_EFFECT_NONE);
	target_item->OrHarmony(harmony_data->GetOption() << 4);
	target_item->OrHarmony(harmony_data->GetMinLevel() & 0x0F);

	this->SendNotice(NOTICE_NORMAL_BLUE, "The regeneration gem strengthens successfully.");
	
	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

harmony_clear_item:
	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}
	
bool Player::UpgradeItemUsingRefineStone(uint8 source_slot, uint8 target_slot)
{
	if ( !sGameServer->IsHarmonyEnabled() )
	{
		return false;
	}

	if ( !this->UpgradeItemCommon(source_slot,target_slot) )
	{
		return false;
	}

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	Item * source_item = this->GetInventory()->GetItem(source_slot);
	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if ( !item_info )
	{
		return false;
	}

	if ( !item_info->IsFlag(ITEM_FLAG_ALLOW_HARMONY) )
	{
		return false;
	}

	if ( !target_item->IsHarmonyUpgrade() )
	{
		return false;
	}

	uint8 option_level = target_item->GetHarmonyLevel();

	if ( option_level >= target_item->GetLevel() || option_level >= 13 )
	{
		if ( option_level == 13 )
		{
			this->SendNotice(NOTICE_NORMAL_BLUE, "Already strengthened for the fiercest value, was unable to continue to strengthen.");
		}
		else
		{
			this->SendNotice(NOTICE_NORMAL_BLUE, "Because the goods rank too is low, is unable to continue to strengthen.");
		}

		return false;
	}

	if ( target_item->GetHarmonyItem() == HARMONY_ITEM_WEAPON )
	{
		if ( target_item->GetHarmonyOption() == HARMONY_WEAPON_ATTACK_DAMAGE_MIN )
		{
			uint8 next_option_level = option_level + 1;

			if ( item_harmony_data const* item_data = sItemMgr->GetHarmony(target_item->GetHarmonyItem(), target_item->GetHarmonyOption()) )
			{
				int32 value = item_data->GetValue(next_option_level);

				if ( (target_item->GetMinDamage() + value) > target_item->GetMaxDamage() )
				{
					this->SendNotice(NOTICE_NORMAL_BLUE, "Already strengthened for the fiercest value, was unable to continue to strengthen.");
					return false;
				}
			}
		}
	}

	int32 success_rate = RANDOM(100);
	int32 rate = sGameServer->harmony_strengthen_update_normal_rate.get();

	if ( source_item->GetItem() == JEWEL::HIGH_REFINE_STONE )
		rate = sGameServer->harmony_strengthen_update_exe_rate.get();

	item_harmony_data const* item_data = sItemMgr->GetHarmony(target_item->GetHarmonyItem(), target_item->GetHarmonyOption());

	if ( !item_data )
	{
		return false;
	}

	if ( success_rate >= rate )
	{
		uint8 next_option_level = item_data->GetMinLevel();
		target_item->SetHarmony(target_item->GetHarmony() & 0xF0);
		target_item->OrHarmony(next_option_level & 0x0F);

		this->SendNotice(NOTICE_NORMAL_BLUE, "Strengthened evolution gem defeat.");
	}
	else
	{
		uint8 next_option_level = option_level + 1;
		target_item->SetHarmony(target_item->GetHarmony() & 0xF0);
		target_item->OrHarmony(next_option_level & 0x0F);

		this->SendNotice(NOTICE_NORMAL_BLUE, "The strengthened evolution gem is successful.");
	}

	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}

bool Player::UpgradeItemUsingExtension(uint8 source_slot, uint8 target_slot)
{
	if ( !this->UpgradeItemCommon(source_slot,target_slot) )
		return false;

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	
	if ( !sItemMgr->IsLuckyItem(target_item->GetItem()) )
	{
		return false;
	}

	float durability = sItemMgr->CalculateDurability(target_item->GetItem(), target_item->GetLevel(), target_item->IsExcellent(), target_item->IsAncient());
	target_item->AlterDurability(2, durability);
	target_item->SetDurabilityState(0);

	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}
	
bool Player::UpgradeItemUsingElevation(uint8 source_slot, uint8 target_slot)
{
	if ( !this->UpgradeItemCommon(source_slot,target_slot) )
		return false;

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	
	if ( !sItemMgr->IsLuckyItem(target_item->GetItem()) )
	{
		return false;
	}

	HarmonyItemType type = target_item->GetHarmonyItem();

	if ( type == HARMONY_ITEM_NONE )
		return false;

	item_harmony_data const* harmony_data = sItemMgr->GetRandomHarmonyOption(target_item);

	if ( !harmony_data )
		return false;
	
	target_item->SetHarmony(HARMONY_EFFECT_NONE);
	target_item->OrHarmony(harmony_data->GetOption() << 4);
	target_item->OrHarmony(harmony_data->GetMinLevel() & 0x0F);

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "The regeneration gem strengthens successfully.");
	
	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}

bool Player::UpgradeItemUsingTradeableSeal(uint8 source_slot, uint8 target_slot)
{
	if ( !this->UpgradeItemCommon(source_slot, target_slot) )
		return false;

	Item * target_item = this->GetInventory()->GetItem(target_slot);
	
	if ( !target_item->IsPentagramItem() || target_item->GetDurability() >= 255.0f )
		return false;

	target_item->AlterDurability(1, 1.0f);
	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);
	return true;
}

bool Player::UpgradeItemUsingMysteriousStone(uint8 source_slot, uint8 target_slot)
{
	if (!this->UpgradeItemCommon(source_slot, target_slot))
	{
		return false;
	}

	Item * target_item = this->GetInventory()->GetItem(target_slot);

	item_template const* item_info = sItemMgr->GetItem(target_item->GetItem());

	if (!item_info)
	{
		return false;
	}

	if (!item_info->IsFlag(ITEM_FLAG_ALLOW_MYSTERIOUS_STONE))
	{
		return false;
	}

	bool modified = false;

	if (target_item->GetItem() < ITEMGET(6, 0))
	{
		if (!target_item->GetExe())
		{
			return false;
		}

		if (!sItemMgr->IsMysteriousStoneMax(1, target_item))
		{
			item_mysterious_stone const* pMysteriousData = sItemMgr->GetRandomMysteriousStone(1);

			if (pMysteriousData)
			{
				target_item->SetExe(pMysteriousData->GetExcellent());
				target_item->SetLuck(pMysteriousData->GetLuck());
				modified = true;
			}
		}

		if (!sItemMgr->IsMasteryBonusMax(2, target_item->GetSocketBonus()))
		{
			target_item->SetSocketBonus(sItemMgr->GetRandomMasteryBonus(2));
			modified = true;
		}
	}
	else if (target_item->GetItem() < ITEMGET(7, 0))
	{
		if (!target_item->GetExe())
		{
			return false;
		}

		if (!sItemMgr->IsMysteriousStoneMax(2, target_item))
		{
			item_mysterious_stone const* pMysteriousData = sItemMgr->GetRandomMysteriousStone(2);

			if (pMysteriousData)
			{
				target_item->SetExe(pMysteriousData->GetExcellent());
				target_item->SetLuck(pMysteriousData->GetLuck());
				modified = true;
			}
		}

		if (!sItemMgr->IsMasteryBonusMax(1, target_item->GetSocketBonus()))
		{
			target_item->SetSocketBonus(sItemMgr->GetRandomMasteryBonus(1));
			modified = true;
		}
	}
	else if (target_item->GetItem() < ITEMGET(12, 0))
	{
		if (target_item->GetAncient())
		{
			uint8 ancient = IS_ANCIENT(target_item->GetAncient());

			if (ancient == 0)
			{
				return false;
			}

			if (!sItemMgr->IsMysteriousStoneMax(0, target_item))
			{
				item_mysterious_stone const* pMysteriousData = sItemMgr->GetRandomMysteriousStone(0);

				if (pMysteriousData)
				{
					target_item->SetAncient(ancient);
					target_item->OrAncient(pMysteriousData->GetAncient());
					target_item->SetLuck(pMysteriousData->GetLuck());
					modified = true;
				}
			}
		}

		if (!sItemMgr->IsMasteryBonusMax(1, target_item->GetSocketBonus()))
		{
			target_item->SetSocketBonus(sItemMgr->GetRandomMasteryBonus(1));
			modified = true;
		}
	}

	if (!modified)
	{
		return false;
	}

	target_item->AlterDurability(3, 0.0f);
	target_item->Convert(true);

	this->UpgradeItemSuccess(source_slot, target_slot);

	return true;
}

void Player::ItemRepairRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		this->ItemRepairResult();
		return;
	}

	POINTER_PCT_LOG(ITEM_REPAIR, lpMsg, Packet, 0);

	if (this->GetInterfaceState()->GetID() == InterfaceData::PetTrainer)
	{
		if (lpMsg->slot == 1)
		{
			Item const* pItem = this->GetActiveInventoryItem(PET_DARK_RAVEN);

			if (!pItem)
			{
				this->ItemRepairResult();
				return;
			}

			lpMsg->slot = pItem->GetOwnerSlot();
			lpMsg->repair_way = 1;
		}
	}

	if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		this->ItemRepairResult();
		return;
	}

	if ( lpMsg->repair_way == 1 ) // Manual Repair
	{
		if ( !sGameServer->IsCharacterRepairButtonEnabled() )
		{
			this->ItemRepairResult();
			return;
		}

		if ( this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < sGameServer->GetCharacterRepairButtonMinLevel() )
		{
			this->ItemRepairResult();
			return;
		}
	}
	else // Npc Repair
	{
		if ( this->GetInterfaceState()->GetID() == InterfaceData::None )
		{
			this->ItemRepairResult();
			return;
		}

		if ( !this->GetInterfaceState()->GetTarget() )
		{
			this->ItemRepairResult();
			return;
		}

		Monster* pNpc = this->GetInterfaceState()->GetTarget()->ToCreature();

		if ( !pNpc )
		{
			this->ItemRepairResult();
			return;
		}
	}

	if ( lpMsg->slot == 0xFF ) // Repair All
	{
		inventory_loop(i, 0, use_inventory_size)
		{
			this->ItemRepairSingle(i, lpMsg->repair_way);
		}

		this->ItemRepairSingle(EARRING_01, lpMsg->repair_way);
		this->ItemRepairSingle(EARRING_02, lpMsg->repair_way);

		this->CalculateCharacter();
		return;
	}

	if (!use_inventory_range(lpMsg->slot) && lpMsg->slot >= PLAYER_MAX_EQUIPMENT && lpMsg->slot != EARRING_01 && lpMsg->slot != EARRING_02)
	{
		this->ItemRepairResult();
		return;
	}

	if ( !this->GetInventory()->GetItem(lpMsg->slot)->IsItem() )
	{
		this->ItemRepairResult();
		return;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(lpMsg->slot) )
	{
		this->ItemRepairResult();
		return;
	}

	if ( !this->GetInventory()->GetItem(lpMsg->slot)->IsFlag(ITEM_FLAG_ALLOW_REPAIR) )
	{
		this->ItemRepairResult();
		return;
	}

	if ( this->GetInventory()->GetItem(lpMsg->slot)->GetDurabilityOriginal() <= 0.0f )
	{
		this->ItemRepairResult();
		return;
	}

	this->ItemRepair(lpMsg->slot, lpMsg->repair_way);

	//if ( lpMsg->slot < PLAYER_MAX_EQUIPMENT || lpMsg->slot == PENTAGRAM_SLOT )
	//{
		this->CalculateCharacter();
	//}
}

void Player::ItemRepairSingle(uint8 slot, uint8 repair_way)
{
	if (!this->GetInventory()->GetItem(slot)->IsItem())
	{
		return;
	}

	if (!this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_ALLOW_REPAIR))
	{
		return;
	}

	if (this->GetInventory()->GetItem(slot)->GetDurabilityOriginal() <= 0.0f)
	{
		return;
	}

	this->ItemRepair(slot, repair_way);
}

void Player::ItemRepair(uint8 slot, uint8 repair_way, bool autorepair)
{
	Item * item = this->GetInventory()->GetItem(slot);

	int32 repair_cost = item->GetRepairZen(repair_way);

	if ( repair_cost <= 0 )
	{
		if ( !autorepair )
		{
			this->ItemRepairResult();
		}
		return;
	}

	if ( this->IsAdministrator() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Repair Cost: %d", repair_cost);
	}

	repair_cost -= (repair_cost * this->GetDataFloat(PLAYER_DATA_DECREASE_REPAIR_FEE)) / 100;

	if ( !this->MoneyHave(repair_cost) )
	{
		if ( !autorepair )
		{
			this->ItemRepairResult();
		}
		return;
	}
	
	if ( !autorepair )
	{
		this->MoneyReduce(repair_cost, false);
	}
	else
	{
		this->MoneyReduce(repair_cost);
	}

	item->SetDurability(item->GetDurabilityBase());
	item->SetDurabilityState(0.0f);
	item->Convert(true);

	this->SendItemDurability(slot, 0);

	if ( item->GetItem() == PET_DARK_HORSE )
	{
		this->PetInfoResult(1, 0, slot);
	}
	else if ( item->GetItem() == PET_DARK_RAVEN )
	{
		this->PetInfoResult(0, 0, slot);
	}

	if ( !autorepair )
	{
		this->ItemRepairResult(this->MoneyGet());
	}
	else
	{
		this->CalculateCharacter();
	}
}

void Player::ItemRepairResult(uint32 zen)
{
	ITEM_REPAIR_RESULT pMsg(zen);
	this->sendPacket(MAKE_PCT(pMsg));
}

// Hago un respaldo del Inventario
bool Player::BeginTransaction()
{
	if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
		return false;

	this->GetTransactionInventory()->Assign(this->GetInventory());
	this->GetTransactionEventInventory()->Assign(this->GetEventInventory());
	SKILL_LOOP(i)
	{
		this->GetTransactionSkill(i)->SetSkill(this->GetSkill(i)->GetSkill());
		this->GetTransactionSkill(i)->SetLevel(this->GetSkill(i)->GetLevel());
	}

	this->SetTransactionRuud(this->GetRuudMoney());
	this->SetTransactionMoney(this->MoneyGet());
	this->SetTransactionCredits(this->GetCredits());

	this->SetTransaction(TRANSACTION_TYPE_BEGIN);
	return true;
}

// No realizo ningun cambio, dejo el inventario como esta
bool Player::CommitTransaction()
{
	if ( this->GetTransaction() != TRANSACTION_TYPE_BEGIN )
		return false;

	this->SetTransaction(TRANSACTION_TYPE_COMMIT);
	return true;
}

// Devuelvo el inventario a su estado original
bool Player::RollBackTransaction()
{
	if ( this->GetTransaction() != TRANSACTION_TYPE_BEGIN )
		return false;

	this->GetInventory()->Assign(this->GetTransactionInventory());
	this->GetEventInventory()->Assign(this->GetTransactionEventInventory());

	SKILL_LOOP(i)
	{
		this->GetSkill(i)->SetSkill(this->GetTransactionSkill(i)->GetSkill());
		this->GetSkill(i)->SetLevel(this->GetTransactionSkill(i)->GetLevel());
	}

	if ( sGameServer->IsTransactionRollbackCurrency() )
	{
		this->SetRuudMoney(this->GetTransactionRuud());
		this->SetCredits(this->GetTransactionCredits());
		this->MoneySet(this->GetTransactionMoney());
	}

	this->SetTransaction(TRANSACTION_TYPE_ROLLBACK);
	return true;
}

void Player::CancelAllActivities()
{
	if ( this->GetInterfaceState()->GetID() == InterfaceData::Warehouse )
	{
		this->WarehouseClose();
	}
	else if ( this->GetInterfaceState()->GetID() == InterfaceData::Trade )
	{
		if ( this->GetInterfaceState()->GetState() == 1 )
		{
			this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeCancel();
			this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeResult(0);
			this->TradeCancel();
			this->TradeResult(0);
		}
	}
	else if ( this->GetInterfaceState()->IsCommonMix() || this->GetInterfaceState()->GetID() == InterfaceData::PetTrainer )
	{
		this->ChaosMixCloseResult();
		this->MixSafeItemRecoverAttempt();
	}

	if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
		this->RollBackTransaction();

	this->GetInterfaceState()->Reset();
}

void Player::TeleportRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_TELEPORT) )
	{
		return;
	}

	POINTER_PCT_LOG(TELEPORT_REQUEST, lpMsg, Packet, 0);

	TeleportRequest(sTeleport->GetTeleport(lpMsg->id), lpMsg->event_id);
}

void Player::TeleportRequest(TeleportData const* teleportData, uint8 eventId)
{
	if (!teleportData)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Invalid teleport location.");
		return;
	}

	if (this->GetTotalLevel() < teleportData->MinLevel)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required level %d to move to %s.", teleportData->MinLevel, teleportData->Name.c_str());
		return;
	}

	auto price = teleportData->Zen;

	if (this->IsHighMurder())
		price *= sGameServer->GetPlayerPKWarpCostIncrease();

	if (!this->MoneyHave(price))
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need %u zen to move to %s.", price, teleportData->Name.c_str());
		return;
	}

	if (this->IsInSelfDefense())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't teleport while self defense is active.");
		return;
	}

	auto gate = teleportData->Gate;

	switch (eventId)
	{
	case EVENT_NOTIFICATION_CHAOS_CASTLE:
		gate = 503;
		break;

	case EVENT_NOTIFICATION_BLOOD_CASTLE:
		gate = BLOOD_CASTLE_GATE;
		break;

	case EVENT_NOTIFICATION_DEVIL_SQUARE:
		gate = DEVIL_SQUARE_GATE;
		break;

	case EVENT_NOTIFICATION_ILLUSION_TEMPLE:
		gate = 506;
		break;

	case EVENT_NOTIFICATION_DOPPELGANGER:
		gate = 507;
		break;
	}

	WARP_RESULT pMsg;
	SendPacket(&pMsg);

	if (MoveToGate(gate))
		MoneyReduce(price);
}

void Player::TeleportRequestSecond(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		//this->TeleportResult(0, false);
		return;
	}

	if (this->HasTeleportRestrictionBuff())
	{
		this->TeleportResult(0,false);
		return;
	}

	if ( CC_MAP_RANGE(this->GetWorldId()) )
	{
		if ( this->GetEventId() == EVENT_CHAOS_CASTLE && CHAOS_CASTLE_GROUND_RANGE(this->GetEventGround()) )
		{
			if ( sChaosCastleMgr->GetGround(this->GetEventGround())->GetState() == CHAOS_CASTLE_STATE_PLAYEND )
			{
				this->TeleportResult(0,false);
				return;
			}
		}
		else if ( this->GetEventId() == EVENT_CHAOS_CASTLE_SURVIVAL )
		{
			if ( sChaosCastleSurvivalMgr->GetGround()->GetState() == CHAOS_CASTLE_STATE_PLAYEND )
			{
				this->TeleportResult(0,false);
				return;
			}
		}
	}

	if ( this->GetNova()->IsRunning() )
	{
		this->RunNova();
		this->TeleportResult(0,false);
		return;
	}

	POINTER_PCT_LOG(SKILL_TELEPORT_REQUEST, lpMsg, Packet, 0);

	if (lpMsg->gate == 0)
	{
		if (this->GetTeleportStatus() != TELEPORT_NONE)
		{
			//this->TeleportResult(0, false);
			return;
		}

		if (IF_MAP_RANGE(this->GetWorldId()) && sGameServer->IsImperialFortressTeleportCheck())
		{
			this->TeleportResult(0, false);
			return;
		}

		if (sDungeon->IsDungeon(this->GetWorldId()))
		{
			this->TeleportResult(0, false);
			return;
		}

		Skill * mSkill = this->MagicGet(SKILL_TELEPORT);

		if (!mSkill)
		{
			this->TeleportResult(0, false);
			return;
		}

		int16 x = lpMsg->x;
		int16 y = lpMsg->y;

		if (x == this->GetX() && y == this->GetY())
		{
			this->TeleportResult(0, false);
			return;
		}

		if (this->HasTeleportCoordinateBuff())
		{
			x = this->GetX();
			y = this->GetY();
		}

		if (!this->TeleportAreaCheck(x, y))
		{
			this->TeleportResult(0, false);
			return;
		}

		World* pWorld = this->GetWorld();

		if (!pWorld)
		{
			this->TeleportResult(0, false);
			return;
		}

		if (this->GetWorldId() == WORLD_CASTLE_SIEGE)
		{
			if (!sCastleSiege->CheckTeleportMagicAxisY(this->GetY(), x, y))
			{
				y = this->GetY();
			}
		}

		skill_template const* skill_info = sSkillMgr->GetSkill(SKILL_TELEPORT);

		if (!skill_info)
		{
			this->TeleportResult(0, false);
			return;
		}

		int32 manause = this->SkillManaUse(mSkill, skill_info);
		int32 staminause = this->SkillStaminaUse(mSkill, skill_info);

		if (!this->PowerHas(POWER_MANA, manause) || !this->PowerHas(POWER_STAMINA, staminause))
		{
			this->TeleportResult(0, false);
			return;
		}

		if (sGameServer->IsTeleportFix())
		{
			NORMAL_MAGIC_ATTACK_RESULT pMsg(SKILL_TELEPORT, this->GetEntry(), this->GetEntry());

			this->sendPacket(MAKE_PCT(pMsg));
			this->sendPacket_viewport(MAKE_PCT(pMsg));
		}
		else
		{
			this->MagicAttackSend(ENTRY(this), SKILL_TELEPORT);
		}

		this->PowerReduce(POWER_MANA, manause);
		this->PowerReduce(POWER_STAMINA, staminause);
		this->ManaSend();

		this->SkillTeleportUse(x, y);
		sDungeonRace->ResetPlayer(this);
		sLosttowerRace->ResetPlayer(this);
		return;
	}
	else
	{
		/*if ( GateData const* pGate = sTeleport->GetGate(lpMsg->gate) )
		{
			if ( pGate->flag != GATE_FLAG_TELEPORT )
			{
				return;
			}

			if ( pGate->world != this->GetWorldId() )
			{
				return;
			}

			if( this->GetX() < (pGate->GetMinX() - 5) || this->GetX() > (pGate->GetMaxX() + 5) || this->GetY() < (pGate->GetMinY() - 5) || this->GetY() > (pGate->GetMaxY() + 5))
			{
				return;
			}

			int32 level = pGate->min_level;

			if( pGate->min_level != -1 && this->GetTotalLevel() < level )
			{
				this->TeleportResult(1, false);
				return;
			}

			this->MoveToGate(lpMsg->gate);
		}
		else
		{*/
			this->TeleportResult(1, false);
		//}
	}
}

void Player::TeleportRequestTarget(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( this->IsInSafeZone() )
	{
		return;
	}

	if ( this->HasBuff(BUFF_STUN) || this->HasBuff(BUFF_SLEEP) || this->HasBuff(BUFF_EARTH_PRISON))
	{
		return;
	}

	POINTER_PCT_LOG(TELEPORT_REQUEST_TARGET, lpMsg, Packet, 0);

	Player * mPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !Player::IsPlayerBasicState(mPlayer, false) )
	{
		return;
	}

	if ( !this->SameDimension(mPlayer) )
	{
		return;
	}

	if ( this->GetPartyID() == PARTY_NULL )
	{
		return;
	}

	if ( mPlayer->GetPartyID() != this->GetPartyID() )
	{
		return;
	}

	if ( !mPlayer->TeleportAreaCheck(lpMsg->x, lpMsg->y) )
	{
		return;
	}

	if ( sCrywolf->IsInAltar(mPlayer) )
	{
		return;
	}

	if (sDungeon->IsDungeon(this->GetWorldId()))
	{
		return;
	}

	Skill * mSkill = this->MagicGet(SKILL_MASS_TELEPORT);

	if ( !mSkill )
	{
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(mSkill->GetSkill());

	if ( !skill_info )
	{
		return;
	}

	int32 manause = this->SkillManaUse(mSkill, skill_info);
	int32 staminause = this->SkillStaminaUse(mSkill, skill_info);

	if ( !this->PowerHas(POWER_MANA, manause) || !this->PowerHas(POWER_STAMINA, staminause) )
	{
		return;
	}

	this->PowerReduce(POWER_MANA, manause);
	this->PowerReduce(POWER_STAMINA, staminause);
	this->ManaSend();

	this->MagicAttackSend(ENTRY(mPlayer), SKILL_MASS_TELEPORT);
	mPlayer->SkillTeleportUse(lpMsg->x, lpMsg->y);
	sDungeonRace->ResetPlayer(mPlayer);
	sLosttowerRace->ResetPlayer(mPlayer);
}

void Player::TeleportResult(uint16 id, bool regen)
{
	if ( regen )
	{
		TELEPORT_RESULT pMsg(id, this->GetRegenLocation()->GetDisplayWorld(), this->GetRegenLocation()->GetX(), this->GetRegenLocation()->GetY(), this->GetRegenLocation()->GetDirection());
		this->sendPacket(MAKE_PCT(pMsg));
	}
	else
	{
		TELEPORT_RESULT pMsg(id, this->GetDisplayWorld(), this->GetX(), this->GetY(), this->GetDirection());
		this->sendPacket(MAKE_PCT(pMsg));
	}
}

bool Player::MoveToGate(uint16 gateId)
{
	this->HelperStop(0x01);

	if (!this->IsLive() || this->GetRegenStatus() != REGEN_NONE)
		return false;

	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_TELEPORT))
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to teleport.");
		return false;
	}

	if (this->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are busy to teleport.");
		return false;
	}

	auto gate = sTeleport->GetGate(gateId, true);
	if (!gate)
		return false;

	if (IF_MAP_RANGE(this->GetWorldId()) && IF_MAP_RANGE(gate->MapId))
	{
		if (!sImperialFortressMgr->UpdatePlayerGate(this, gateId))
			return false;
	}

	if (!this->CanEnterWorld(gate->MapId, gate->X1, gate->Y1, gate->X2, gate->Y2, true))
		return false;

	if (this->GetTotalLevel() < gate->MinLevel)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Minimum level required to teleport: %d", gate->MinLevel);
		return false;
	}

	int16 tmp_x = gate->X1;
	int16 tmp_y = gate->Y1;
	uint16 tmp_map = gate->MapId;
	uint8 tmp_direction = gate->Direction;
	int32 tmp_instance = this->GetInstance();

	this->GetValidCoordinates(gate->Id, tmp_map, tmp_x, tmp_y);
	this->CancelDelayedTeleport();

	if (AW_MAP_RANGE(tmp_map))
	{
		if (sProtectorOfAcheron->GetState() >= PROTECTOR_OF_ACHERON_STATE_PLAYING)
		{

		}
		else
		{
			if (sArkaWar->GetState() == ARKA_WAR_STATE_STANDBY)
			{
				if (gateId != 426)
					return false;
			}
			else if (sArkaWar->GetState() >= ARKA_WAR_STATE_PLAYING)
			{
				if (!sArkaWar->GetGatePosition(this, gateId, tmp_map, tmp_x, tmp_y))
					return false;
			}
			else
				return false;
		}
	}

	this->StartRegen(tmp_map, tmp_x, tmp_y, tmp_direction, tmp_instance);

	this->SetCurrentGate(gateId);

	sArkaWar->SendOccupyZone(this, gateId);

	sDungeonRace->PlayerInGate(this, gateId);
	sLosttowerRace->PlayerInGate(this, gateId);

	return true;
}

void Player::TeleportToLocation(uint16 world, int16 x, int16 y, uint8 direction, int32 instance)
{
	this->StartRegen(world, x, y, direction, instance);
}

void Player::TeleportToLocation(uint16 world)
{
	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
	{
		return;
	}

	int16 x = 0;
	int16 y = 0;
	pWorld->GetRespawn(world, x, y);
	this->StartRegen(world, x, y, this->GetDirection(), this->GetInstance());
}

void Player::TeleportToLocation()
{
	this->StartRegen(this->GetLastLocation()->GetWorldId(), this->GetLastLocation()->GetX(), this->GetLastLocation()->GetY(), this->GetLastLocation()->GetDirection(), this->GetLastLocation()->GetInstance());
}

void Player::TeleportToDelayed()
{
	this->StartRegen(this->GetDelayedLocation()->GetWorldId(), this->GetDelayedLocation()->GetX(), this->GetDelayedLocation()->GetY(), this->GetDelayedLocation()->GetDirection(), this->GetDelayedLocation()->GetInstance());
}

void Player::TeleportToGate(uint16 gate)
{
	uint16 world = 0;
	int16 x = 130;
	int16 y = 130;

	this->GetValidCoordinates(gate, world, x, y);

	this->StartRegen(world, x, y, this->GetDirection(), this->GetInstance());
}

void Player::StartRegen(uint16 world, int16 x, int16 y, uint8 direction, int32 instance, uint16 gate_id)
{
	this->CancelAllActivities();

	if ( this->GetWorldId() != world )
	{
		uint16 server_move = sWorldMgr->AllowMoveToWorld(this, world);

		if ( server_move != sGameServer->GetServerCode() )
		{
			//if ( server_move == uint16(-1) )
			//	return;

			sAuthServer->PlayerServerMoveRequest(this, server_move, world, x, y);
			return;
		}
	}

	if (CC_MAP_RANGE(world))
	{
		this->SetVisibleMount(-1);
	}

	this->SetMiniMapState(false);
	this->SetSentMapState(false);
	//this->SetMiniMapWorld(-1);

	this->SetState(OBJECT_STATE_TELEPORTING);

	this->GetRegenLocation()->SetWorldId(world);
	this->GetRegenLocation()->SetX(x);
	this->GetRegenLocation()->SetY(y);
	this->GetRegenLocation()->SetDirection(direction);
	this->GetRegenLocation()->SetInstance(instance);
	
	sTeleport->SendBattleZone(this, this->GetRegenLocation()->GetWorldId());
	
	sLog->outInfo(LOG_PLAYER, "Teleport From [World:%d - %d/%d - Instance:%d] To [World:%d - %d/%d - Instance:%d] -- %s",
		this->GetWorldId(), this->GetX(), this->GetY(), this->GetInstance(), world, x, y, instance, this->BuildLog().c_str());

	this->UpdateLastLocation();

	this->GetLocation()->Set(this->GetRegenLocation());
	this->SetTX(this->GetX());
	this->SetTY(this->GetY());
	this->SetWalkSpeedCount(0);
	this->GetACPathData()->Reset();

	this->SetCurrentGate(0);
	this->SetWorldState(-1);

	this->ClearPathData();
	this->SetRegenStatus(REGEN_STANDBY);
	
	this->RestoreEventAnimation();

	//this->ViewportDeleteSend();

	if ( world != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
	{
		this->TeleportResult(1);
	}

	if ( world != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
	{
		//this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
		this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);

		this->ViewportClear();
	}
	
	this->SendExperienceEvent();
}

void Player::MoveFail()
{
	this->StartRegen(this->GetWorldId(), this->GetX(), this->GetY(), this->GetDirection(), this->GetInstance());
}

void Player::DataLoadingConfirm()
{
	if ( this->GetRegenStatus() == REGEN_STANDBY )
		this->SetRegenStatus(REGEN_READY);
}

void Player::UpdateStatus()
{
	if ( this->GetTeleportStatus() == TELEPORT_NONE && this->GetViewState() == VIEW_STATE_TELEPORT )
	{
		this->SetViewState(VIEW_STATE_NONE);
	}

	if (!this->IsRest() && (this->GetViewState() == VIEW_STATE_ACTION_1 || this->GetViewState() == VIEW_STATE_ACTION_2 || this->GetViewState() == VIEW_STATE_ACTION_3))
	{
		this->SetViewState(VIEW_STATE_NONE);
	}

	this->UpdateTeleport();

	if ( this->GetState() == OBJECT_STATE_STAND_BY )
	{
		this->SetState(OBJECT_STATE_READY);
	}

	if ( this->GetRegenStatus() == REGEN_STARTED )
	{
		this->SetRegenStatus(REGEN_NONE);
	}
	
	if ( this->GetRegenStatus() == REGEN_STANDBY && this->GetHelper()->IsOffline() )
	{
		this->SetRegenStatus(REGEN_READY);
	}

	if ( this->GetRegenStatus() == REGEN_STANDBY && this->GetPersonalStore()->IsOpen() && this->GetPersonalStore()->IsOff() )
	{
		this->SetRegenStatus(REGEN_READY);
	}

	if ( this->GetRegenStatus() == REGEN_READY )
	{
		this->SetState(OBJECT_STATE_STAND_BY);
		this->SetRegenStatus(REGEN_STARTED);
		this->SetX(this->GetRegenLocation()->GetX());
		this->SetY(this->GetRegenLocation()->GetY());
		this->SetWorldId(this->GetRegenLocation()->GetWorldId());
		this->SetTX(this->GetX());
		this->SetTY(this->GetY());
		this->SetWalkSpeedCount(0);
		this->GetACPathData()->Reset();
		this->CreateFrustrum();

		this->SetViewState(VIEW_STATE_NONE);
		this->SetRest(0);

		sWorldMgr->SendUrukMountainZone(this);
		sWorldMgr->SendFereaZone(this);
		sSwampOfDarkness->SendState(this);

		if (this->IsQuestGuidedMove())
		{
			this->QuestGuidedWalk();

			this->SetQuestGuidedMove(false);
		}

		if (this->GetQuestMUAutoMoving() != 0)
		{
			this->SendQuestMUAutoMove();

			this->SetQuestMUAutoMoving(0);
		}

		if ( this->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
		{
			//this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
		}
	}

	if ( this->GetRegenStatus() == REGEN_NONE )
	{
		if ( this->GetWorldId() != WORLD_LAST_MAN_STANDING && 
			 this->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE && 
			 this->GetWorldId() != WORLD_PLACE_OF_QUALIFICATION && 
			 !sDungeon->IsDungeon(this->GetWorldId()) &&
			 !IF_MAP_RANGE(this->GetWorldId()))
		{
			this->SetInstance(-1);
		}
	}

	for ( UnitPunishMap::const_iterator itr = this->punish_map.begin(); itr != this->punish_map.end(); )
	{
		if ( (MyGetTickCount() > (itr->second + sGameServer->GetSkillPunishFrequencyPVP())) )
		{
			this->punish_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}

void Player::LoadDefaultSkill()
{
	if ( this->GetClass() >= Character::MAX_CLASS )
		return;

	CharacterSkillBaseList const& base_list = sCharacterBase->char_skill_base[this->GetClass()];

	for ( CharacterSkillBaseList::const_iterator it = base_list.begin(); it != base_list.end(); ++it )
	{
		this->MagicAdd(*it, 0);
	}
}

void Player::GiveSingleExperience(Monster * mMonster, int32 damage)
{
	if (sGameServer->GetTimeCheckLevelMax() != -1 && this->GetTotalLevel() > sGameServer->GetTimeCheckLevelMax())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't get more experience on this server.");
		return;
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return;

	if (!pWorld->flagHas(WORLD_FLAG_NO_LEVEL_CHECK))
	{
		if (this->IsMajestic() && mMonster->GetLevel() < sGameServer->GetMinMonsterLevelMajestic())
		{
			return;
		}

		if (this->IsMaster() && mMonster->GetLevel() < sGameServer->GetMinMonsterLevelMaster())
		{
			return;
		}
	}

	this->UpdateGoblinPoints(mMonster);

	int32 level = (mMonster->GetLevel() + 25) * mMonster->GetLevel() / 3;

	if ( (mMonster->GetLevel() + 10) < this->GetTotalLevel() )
		level = ((mMonster->GetLevel() + 10) * level) / this->GetTotalLevel();

	if ( mMonster->GetLevel() >= 65 )
		level += (mMonster->GetLevel() / 4) * (mMonster->GetLevel() - 64);

	int32 max_level = 0;

	if(level > 0)
		max_level = level / 2;
	else
		level = 0;

	int64 experience = level + Random(max_level);

	experience = damage * experience / mMonster->PowerGetTotal(POWER_LIFE);

	CharacterExperienceTable const* pCharacterExperienceTable = sCharacterBase->GetCharacterExperienceTable(this);

	if (pCharacterExperienceTable)
	{
		experience *= pCharacterExperienceTable->GetExperienceRate();
	}
	else
	{
		if (this->IsMajestic())
		{
			experience *= sGameServer->GetMajesticExperience();
		}
		else if (this->IsMaster())
		{
			experience *= sGameServer->GetMasterExperience();
		}
		else
		{
			experience *= sGameServer->GetNormalExperience();
		}
	}

	experience = experience * pWorld->GetExperienceRate() / 100;
	
	if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
	{
		experience = experience * sGameServer->GetCrywolfPenaltyExperienceGain() / 100;
	}

	this->ApplyBuffExperience(experience, 1);

	sCharacterBase->ApplyExperienceAdjust(this, experience);

	int32 m_zen = 0;

	if ( this->IsMajestic() )
	{
		m_zen = experience * sGameServer->GetMajesticZen() / 100;
	}
	else if ( this->IsMaster() )
	{
		m_zen = experience * sGameServer->GetMasterZen() / 100;
	}
	else
	{
		m_zen = experience * sGameServer->GetNormalZen() / 100;
	}

	m_zen = m_zen * pWorld->GetZenRate() / 100;

	if ( m_zen < 0 )
		m_zen = 0;

	mMonster->IncreaseZen(m_zen);

	this->GiveExperience(experience, false, true, 1);
}

void Player::GivePartyExperience(Monster * mMonster, int32 damage)
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return;

	bool CharacterBonus[Character::MAX_CLASS] = {false, false, false, false, false, false, false, false, false, false, false};
	int32 total_level = 0;
	int32 party_level = 0;
	uint8 viewplayer = 0;
	int32 viewpercent = 100;
	int32 level = ( mMonster->GetLevel() + 25) * mMonster->GetLevel() / 3;

	Party* pParty = this->GetParty();
	Player* pPlayer = nullptr;

	if ( !pParty )
		return;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			continue;

		pPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !this->SameDimension(pPlayer) )
			continue;

		if ( !IN_RANGE(this, pPlayer, MAX_PARTY_MONSTER_KILL_DISTANCE) )
			continue;

		viewplayer++;
		total_level += pPlayer->GetTotalLevel();
		CharacterBonus[pPlayer->GetClass()] = true;
	}

	if ( !viewplayer )
	{
		return;
	}

	if( sCharacterBase->IsExperienceBonus(CharacterBonus) )
	{
		viewpercent = sGameServer->GetPartyBonusExperience(viewplayer - 1);
	}
	else
	{
		viewpercent = sGameServer->GetPartyExperience(viewplayer - 1);
	}

	party_level = total_level / viewplayer;

	if ( (mMonster->GetLevel() + 10) < party_level )
	{
		level = ((mMonster->GetLevel() + 10) * level) / party_level;
	}

	if ( mMonster->GetLevel() >= 65 )
	{
		level += (mMonster->GetLevel() / 4) * (mMonster->GetLevel() - 64);
	}

	int32 max_level = 0;

	if(level > 0)
		max_level = level / 2;
	else
		level = 0;

	int64 experience = 0;
	int64 total_experience = level + Random(max_level);

	total_experience = damage * total_experience / mMonster->PowerGetTotal(POWER_LIFE);

	total_experience = (total_experience * (viewpercent / viewplayer)) / 100;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			continue;

		pPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !this->SameDimension(pPlayer) )
			continue;

		if ( !IN_RANGE(this, pPlayer, MAX_PARTY_MONSTER_KILL_DISTANCE) )
			continue;

		if (!pWorld->flagHas(WORLD_FLAG_NO_LEVEL_CHECK))
		{
			if (pPlayer->IsMajestic() && mMonster->GetLevel() < sGameServer->GetMinMonsterLevelMajestic())
			{
				continue;
			}

			if (pPlayer->IsMaster() && mMonster->GetLevel() < sGameServer->GetMinMonsterLevelMaster())
			{
				continue;
			}
		}

		if (sGameServer->GetTimeCheckLevelMax() != -1 && pPlayer->GetTotalLevel() > sGameServer->GetTimeCheckLevelMax())
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't get more experience on this server.");
			continue;
		}

		pPlayer->UpdateGoblinPoints(mMonster);

		//experience = (total_experience * viewpercent * pPlayer->GetTotalLevel()) / total_level / 100;

		//experience = (total_experience * (viewpercent / viewplayer)) / 100;
		experience = total_experience;

		CharacterExperienceTable const* pCharacterExperienceTable = sCharacterBase->GetCharacterExperienceTable(pPlayer);

		if (pCharacterExperienceTable)
		{
			experience *= pCharacterExperienceTable->GetExperienceRate();
		}
		else
		{
			if (pPlayer->IsMajestic())
			{
				experience *= sGameServer->GetMajesticExperience();
			}
			else if (pPlayer->IsMaster())
			{
				experience *= sGameServer->GetMasterExperience();
			}
			else
			{
				experience *= sGameServer->GetNormalExperience();
			}
		}

		experience = experience * pWorld->GetExperienceRate() / 100;

		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
		{
			experience = experience * sGameServer->GetCrywolfPenaltyExperienceGain() / 100;
		}

		pPlayer->ApplyBuffExperience(experience, viewplayer);

		sCharacterBase->ApplyExperienceAdjust(pPlayer, experience);

		int32 m_zen = 0;

		if ( pPlayer->IsMajestic() )
		{
			m_zen = experience * sGameServer->GetMajesticZen() / 100;
		}
		else if ( pPlayer->IsMaster() )
		{
			m_zen = experience * sGameServer->GetMasterZen() / 100;
		}
		else
		{
			m_zen = experience * sGameServer->GetNormalZen() / 100;
		}

		m_zen = m_zen * pWorld->GetZenRate() / 100;

		if ( m_zen < 0 )
			m_zen = 0;

		mMonster->IncreaseZen(m_zen);

		pPlayer->GiveExperience(experience, false, true, 1);
	}
}

void Player::GiveExperience(int64 & experience, bool apply_buff, bool send, uint8 type)
{
	if ( apply_buff )
	{
		this->ApplyBuffExperience(experience, 1);
	}

	if ( experience <= 0 )
	{
		experience = 0;
		return;
	}

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_GAIN_EXPERIENCE) )
	{
		experience = 0;
		return;
	}

	//sLog->outInfo(LOG_PLAYER, "GiveExperience() %s - Gain experience: %I64d - %s", this->BuildLog().c_str(), experience, this->BuildLocationLog().c_str());

	this->GivePetExperience(experience);

	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( this->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( this->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	if ( this->GetLevelData(level_type)->GetLevel() >= sGameServer->GetMaxLevel(level_type) )
	{
		experience = 0;
		return;
	}

	if ( type == 1 )
	{
		sHuntingRecord->Update(this, 0, 0, 0, 0, experience);

		if (this->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE)
		{
			this->GetLabyrinthData()->IncreaseEarnedExperience(experience);
		}
	}

	if ( (this->GetLevelData(level_type)->GetExperience() + experience) < this->GetLevelData(level_type)->GetNextExperience() )
	{
		this->GetLevelData(level_type)->IncreaseExperience(experience);
		goto end_send;
	}

	int16 level = this->GetTotalLevel();

	while(true)
	{
		this->GetLevelData(level_type)->IncreaseLevel(1);

		this->LevelUpAddPoints();

		experience -= (this->GetLevelData(level_type)->GetNextExperience() - this->GetLevelData(level_type)->GetExperience());

		//this->GetLevelData(level_type)->SetExperience(this->GetLevelData(level_type)->GetNextExperience());
		this->GetLevelData(level_type)->SetExperience(0);

		sLog->outInfo(LOG_PLAYER, "%s %s - Level: %d - %s", this->IsMaster() ? "MasterLevelUp()": "LevelUp()", this->BuildLog().c_str(), this->GetLevelData(level_type)->GetLevel(), this->BuildLocationLog().c_str());

		this->CalculateNextExperience();

		/*if ( this->IsMaster() )
		{
			this->MasterSendLevelUp();
		}
		else
		{
			this->NormalSendLevelUp();
		}*/

		if ( this->GetLevelData(level_type)->GetLevel() >= sGameServer->GetMaxLevel(level_type) )
		{
			experience = 0;
			break;
		}
		
		//if ( (this->GetLevelData(level_type)->GetExperience() + experience) < this->GetLevelData(level_type)->GetNextExperience() )
		if ( experience < this->GetLevelData(level_type)->GetNextExperience() )
		{
			if ( level_type == LEVEL_DATA_NORMAL )
			{
				this->GetLevelData(level_type)->IncreaseExperience(experience);
			}
			break;
		}
	}

	this->SetDate(PLAYER_DATE_LEVEL_UP, time(nullptr));

	sCharacterBase->LevelUpReward(this, level);

	if ( this->IsMajestic() )
	{
		this->MasterSendLevelUp();
		this->MajesticSkillTreeSendPoints();
	}
	else if ( this->IsMaster() )
	{
		this->MasterSendLevelUp();
	}
	else
	{
		this->NormalSendLevelUp();
	}

	if ( experience < 0 )
	{
		experience = 0;
	}
	
	if ( !this->IsMaster() )
	{
		this->CheckAddInfinityArrow();
	}

	this->CalculateCharacter();

	POWER_LOOP(i)
	{
		this->PowerSet(i, this->PowerGetTotal(i));
	}

	this->ManaSend();
	this->LifeSend();

	this->MasterSendStatus();

	this->LogLevelUp();
	this->ElfSoldierBuffUP(true);
	this->UpdateQuestMULevel();

	if ( level_type == LEVEL_DATA_MASTER || level_type == LEVEL_DATA_MAJESTIC )
	{
		this->GetLevelData(level_type)->IncreaseExperience(experience);
	}

end_send:
	if ( send )
	{
		this->KillExperienceSend(uint16(-1), experience);
	}
}

void Player::GivePetExperience(int64 experience)
{
	if ( this->GetClass() != Character::DARK_LORD )
	{
		return;
	}

	Item * Mount = this->GetActiveInventoryItem(PET_DARK_HORSE);

	if (Mount && Mount->IsValid())
	{
		int64 add_experiences = experience * 0.15;

		if (Mount->GivePetExperience(add_experiences))
		{
			Mount->SetValid(this->ValidItem(Mount));

			if (!Mount->IsValid())
			{
				Mount->PetLevelDown(add_experiences);
				this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Exp is unobtainable because you are unable to command the Horse");
				this->PetInfoResult(1, 0, Mount->GetOwnerSlot());
			}
			else
			{
				this->PetInfoLevelUp(Mount->GetOwnerSlot(), 1);
			}
		}
		else
		{
			this->PetInfoResult(1, 0, Mount->GetOwnerSlot());
		}
	}

	Item * Raven = this->GetActiveInventoryItem(PET_DARK_RAVEN);

	if (Raven && Raven->IsValid())
	{
		int64 add_experiences = experience * 0.15;

		if (Raven->GivePetExperience(add_experiences))
		{
			Raven->SetValid(this->ValidItem(Raven));

			if (!Raven->IsValid())
			{
				Raven->PetLevelDown(add_experiences);
				this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Exp is unobtainable because you are unable to command the Raven");
				this->PetInfoResult(0, 0, Raven->GetOwnerSlot());
			}
			else
			{
				this->PetInfoLevelUp(Raven->GetOwnerSlot(), 0);
			}
		}
		else
		{
			this->PetInfoResult(0, 0, Raven->GetOwnerSlot());
		}
	}
}

void Player::LevelUpAddPoints()
{
	CharacterBaseData const* base_info = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !base_info )
		return;

	if ( this->IsMajestic() )
	{
		this->GetLevelData(LEVEL_DATA_MAJESTIC)->IncreasePoints(base_info->GetMajesticLevelUpPoints());
	}
	else if ( this->IsMaster() )
	{
		this->GetLevelData(LEVEL_DATA_MASTER)->IncreasePoints(base_info->GetMasterLevelUpPoints());
	}
	else
	{
		this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(base_info->GetLevelUpPoints());

		if ( this->QuestEvolutionGetState(2) == QUEST_EVOLUTION_STATE_COMPLETE )
		{
			this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(base_info->GetPlusLevelUpPoints());
		}
	}
}

void Player::KillExperienceSend(uint16 target, int64 experience, uint16 damage, uint8 flag)
{
	if ( experience <= 0 )
	{
		return;
	}

	KILL_EXPERIENCE_SEND pMsg(-1, static_cast<uint32>(experience), damage, flag);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::ApplyBuffExperience(int64 & experience, uint8 count)
{
	int32 experience_rate = 100;

	experience_rate += this->GetData(PLAYER_DATA_ADD_EXPERIENCE_RATE);

	if ( count > 1 )
	{
		experience_rate += static_cast<int32>(this->GetEffect(BUFF_OPTION_INCREASE_EXPERIENCE_PARTY) * (count - 1));
	}

	experience_rate += this->GetSpecialItemExperience();

	if ( sHappyHour->GetState() == HAPPY_HOUR_STATE_START )
	{
		experience_rate += sGameServer->happy_hour_experience_add.get();
	}

	experience_rate += sCharacterBase->GetBonus(this);

	experience_rate += sWorldMgr->GetWorldExperienceRate(this->GetWorldId(), count);

	if ( this->HasBuff(BUFF_FEEL_TIRED) || this->HasBuff(BUFF_FEEL_EXHAUSTED) )
	{
		experience_rate = 50;
	}

	if ( this->GetEffect(BUFF_OPTION_EXPERIENCE_DISABLED) > 0.0f )
	{
		experience_rate = 0;
	}

	experience = experience * experience_rate / 100;
	
	sVipMgr->ApplyVipExperience(this, experience);
}

void Player::LogLevelUp()
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_QUERY_LEVEL_UP);

	stmt->setUInt32(this->GetGUID());
	stmt->setString(this->GetName());
	stmt->setUInt16(this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	stmt->setUInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
	stmt->setUInt16(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());
	stmt->setUInt16(this->GetWorldId());
	stmt->setInt16(this->GetX());
	stmt->setInt16(this->GetY());
	stmt->setInt8(this->GetEventGround());

	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void Player::MonsterDiePlus(Unit * mMonster)
{
	if (!mMonster || !mMonster->IsCreature() || !mMonster->ToCreature()->IsMonster())
	{
		return;
	}

	this->PowerIncrease(POWER_LIFE, mMonster->GetTotalLevel());
	this->PowerIncrease(POWER_LIFE, this->GetData(PLAYER_DATA_MONSTER_DIE_HP));
	this->PowerIncrease(POWER_MANA, this->GetData(PLAYER_DATA_MONSTER_DIE_MP));
	this->PowerIncrease(POWER_SHIELD, this->GetData(PLAYER_DATA_MONSTER_DIE_SD));
	this->PowerIncrease(POWER_STAMINA, this->GetData(PLAYER_DATA_MONSTER_DIE_AG));

	this->ManaSend();
	this->LifeSend();
}

void Player::SendSummonedHP(int32 hp, int32 max_hp)
{
	if ( !this->GetSummoned() )
		return;

	SUMMONED_HP pMsg(hp * 100 / max_hp);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::IncreaseAggroAround(int64 value)
{
	Object * mUnit;

	VIEWPORT_LOOP(mUnit)

		if ( !mUnit->IsCreature() || !mUnit->ToCreature()->IsMonster() )
			continue;

		if ( mUnit->ToUnit()->IsSummoned() && mUnit->ToUnit()->GetSummoner()->IsPlayer() )
			continue;

		if ( Party* pParty = this->GetParty() )
		{
			pParty->IncreaseAggroAround(this, mUnit->ToCreature(), value);
		}
		else
		{
			mUnit->ToCreature()->GetThreatManager()->AddThreat(this, value);
		}

	VIEWPORT_CLOSE
}

void Player::UpdateAutorecuperation()
{
	POWER_LOOP(i)
	{
		this->GetPowerAutorecuperationTime(i)->Start();
	}
}

void Player::UpdateMovement()
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	if ( this->HasRestrictionBuff() )
	{
		if ( this->GetPathData()->GetCount() > 0 )
		{
			this->ClearPathData(false);
		}

		return;
	}

	if ( this->GetHelper()->IsStarted() && this->GetHelper()->IsOffline() )
	{
		if ( Unit* pUnit = this->GetHelper()->GetTarget() )
		{
			if ( IN_RANGE(this, pUnit, this->GetHelper()->GetTargetRange()) )
			{
				this->ClearPathData(false);
				return;
			}
		}
	}

	if ( this->GetPathData()->GetCount() > 0 )
	{
		this->SetIntData(UNIT_INT_MOVE_SPEED, sGameServer->GetPlayerWalkSpeed());

		if ( this->GetPathData()->GetCurrent() >= 14 )
		{
			return;
		}

		uint32 MoveTime = this->GetIntData(UNIT_INT_MOVE_SPEED) + (this->HasSlowdownBuff() ? 300: 0);

		PathPosition const* Position = this->GetPathData()->GetPosition(this->GetPathData()->GetCurrent());
	
		if( (Position->GetDir() % 2) == 0 )
		{
			MoveTime = (uint32)((double)(MoveTime) * 1.3f);
		}

		if ( this->HasBuff(BUFF_PARALYZE) || this->HasBuff(BUFF_PARALYZE_IMPROVED) )
		{
			MoveTime += MoveTime * 50 / 100;
		}
	
		if( this->GetMovePathTime()->Elapsed(MoveTime) )
		{
			int16 nextX = Position->GetX();
			int16 nextY = Position->GetY();

			WorldGrid const& grid = pWorld->GetGrid(nextX, nextY);

			if( grid.IsLocked_1() || grid.IsLocked_2() )
			{
				sLog->outWarn(LOG_PLAYER, "[ MOVEMENT ] %s Map[%d]-(%d,%d) (%d,%d) Can not Move Position Attr[%d]", 
					this->BuildLog().c_str(), this->GetWorldId(), nextX, nextY, this->GetX(), this->GetY(), grid.attribute);

				this->ClearPathData(true);
				return;
			}

			if (this->IsAdministrator())
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Walking Path :: X: %d - Y: %d", nextX, nextY);
			}

			this->SetX(nextX);
			this->SetY(nextY);
			this->SetDirection(Position->GetDir());
			this->GetPathData()->IncreaseCurrent(1);

			this->GetACPathData()->SetLoaded(true);
			this->GetACPathData()->SetX(nextX);
			this->GetACPathData()->SetY(nextY);

			if ( this->GetHelper()->IsStarted() && this->GetHelper()->IsOffline() )
			{
				this->MoveSend(this->GetX(), this->GetY(), this->GetDirection() << 4);
				this->GetHelper()->SetLastMoveTime(MyGetTickCount());
			}

			if ( this->GetPathData()->GetCurrent() >= this->GetPathData()->GetCount() )
			{
				this->GetPathData()->SetCurrent(0);
				this->GetPathData()->SetCount(0);
				this->GetPathData()->SetStartEnd(false);
			}
		}
	}
	else
	{
		if ( this->GetPathData()->IsStartEnd() )
		{
			this->GetPathData()->SetStartEnd(false);
		}
	}
}

uint8 Player::GetKalimaLevelEntrance()
{
	uint8 return_level = -1;

	for ( uint8 i = 0; i < KALIMA_LEVEL; ++i )
	{
		if ( this->GetTotalLevel() >= g_KalimaLevel[i][this->GetClass()] )
			return_level = i;
	}

	return return_level;
}

bool Player::CreateKalimaGate(uint8 level, int16 x, int16 y)
{
	if ( level == 0 || level > KALIMA_LEVEL )
		return false;

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	if ( !pWorld->flagHas(WORLD_FLAG_ALLOW_KALIMA_GATE) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't summon a Kalima gate within this zone.");
		return false;
	}

	if ( this->IsInSafeZone() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't summon a Kalima gate within safe zone.");
		return false;
	}

	if ( sObjectMgr->ExistKalimaGate(this) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You already possess a Kalima gate.");
		return false;
	}

	uint8 entrance_level = this->GetKalimaLevelEntrance();

	if ( entrance_level == uint8(-1) || entrance_level < (level-1) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You don't have enough level to summon this Kalima gate.");
		return false;
	}

	entrance_level = level - 1;

	WorldGrid const& attr = pWorld->GetGrid(x, y);

	if ( attr.attribute != 0 )
	{
		if ( !this->GetKalimaGateRandomLocation(x, y) )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't find correct zone to summon Kalima gate.");
			return false;
		}
	}

	int32 monster_class = 152 + entrance_level;

	Monster * mMonster = sObjectMgr->MonsterTryAdd(monster_class, this->GetWorldId());

	if ( !mMonster )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't summon Kalima gate.");
		return false;
	}

	mMonster->SetLevel(level);
	mMonster->SetWorldId(this->GetWorldId());
	mMonster->SetBasicLocation(x, y, x, y);
	mMonster->SetInstance(this->GetInstance());
	mMonster->SetDirection(1);
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetSummoner(this);
	mMonster->SetSummonTick(MyGetTickCount());
	mMonster->AddToWorld();

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Kalima Gate created");

	return true;
}

bool Player::GetKalimaGateRandomLocation(int16 & x, int16 & y)
{
	int32 count = 100;
	int16 ix = x;
	int16 iy = y;

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	while ( count-- > 0 )
	{
		x = Random(6) * (((Random(2) == 0) ? -1 : 1)) + ix;
		y = Random(6) * (((Random(2) == 0) ? -1 : 1)) + iy;

		if ( pWorld->GetGrid(x, y).attribute == 0 )
			return true;
	}

	return false;
}

void Player::PetInfoRequest(uint8 * Packet)
{
	if ( !this->IsPlaying() )
		return;

	POINTER_PCT_LOG(PET_INFO_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->pet > 1 || lpMsg->type > 6 )
		return;

	this->SetDarkLordPetSlot(lpMsg->slot);
	this->SetDarkLordPetType(lpMsg->pet);
	this->SetDarkLordPetFlag(lpMsg->type);

	this->PetInfoResult(lpMsg->pet, lpMsg->type, lpMsg->slot);
}

void Player::PetInfoResult(uint8 pet, uint8 type, uint8 slot)
{
	if ( this->GetDarkLordPetSlot() != slot )
	{
		return;
	}

	if ( this->GetDarkLordPetType() != pet )
	{
		return;
	}

	if ( this->GetDarkLordPetFlag() != type )
	{
		return;
	}

	PET_INFO_RESULT pMsg(pet,type,slot);
	pMsg.experience = 0;
	pMsg.level = 0;
	pMsg.durability = 0;
	Item const* item = nullptr;

	if ( type == 0 ) // inventory
	{
		if (slot == 0x0B)
		{
			return;
		}

		if ( inventory_range(slot) )
		{
			item = this->GetInventory()->GetItem(slot);
		}
	}
	else if ( type == 1 ) // warehouse
	{
		if ( warehouse_range(slot) )
		{
			if ( this->GetInterfaceState()->GetID() == InterfaceData::Warehouse )
			{
				item = this->GetWarehouse()->GetItem(slot);
			}
		}
	}
	else if ( type == 2 ) // trade
	{
		if ( trade_range(slot) )
		{
			if ( this->GetInterfaceState()->GetID() == InterfaceData::Trade && this->GetInterfaceState()->GetState() == 1 )
			{
				item = this->GetTradeInventory()->GetItem(slot);
			}
		}
	}
	else if ( type == 3 ) // trade target
	{
		if ( trade_range(slot) )
		{
			if ( this->GetInterfaceState()->GetID() == InterfaceData::Trade && this->GetInterfaceState()->GetState() == 1 && this->GetInterfaceState()->GetTarget() )
			{
				Player * mPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer();

				if ( mPlayer->GetInterfaceState()->GetID() == InterfaceData::Trade && mPlayer->GetInterfaceState()->GetState() == 1 && mPlayer->GetInterfaceState()->GetTarget() == this )
				{
					item = mPlayer->GetTradeInventory()->GetItem(slot);
				}
			}
		}
	}
	else if ( type == 4 ) // chaos
	{
		if ( chaos_box_range(slot) )
		{
			if ( this->GetInterfaceState()->IsCommonMix() || this->GetInterfaceState()->GetID() == InterfaceData::PetTrainer )
			{
				item = this->GetMixInventory()->GetItem(slot);
			}
		}
	}
	else if ( type == 5 ) // personal shop
	{
		if ( inventory_range(slot) )
		{
		}
	}

	if ( item && item->IsItem() )
	{
		pMsg.experience = item->pet_dark.experience;
		pMsg.level = item->pet_dark.level;
		pMsg.durability = item->GetDurability();
	}

	this->SendPacket(&pMsg);
}

void Player::PetInfoLevelUp(uint8 slot, uint8 pet)
{
	PET_INFO_RESULT pMsg(pet, 0xFE, slot);
	pMsg.experience = this->GetInventory()->GetItem(slot)->pet_dark.experience;
	pMsg.level = this->GetInventory()->GetItem(slot)->pet_dark.level;
	pMsg.durability = this->GetInventory()->GetItem(slot)->GetDurability();

	this->SendPacket(&pMsg);
}

bool Player::CreateMercenary(uint8 level, int16 x, int16 y)
{
	if ( this->GetWorldId() != WORLD_CASTLE_SIEGE )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Soldiers can be summoned only in Castle Siege map.");
		return false;
	}

	if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Soldiers can be summoned only after the Siege begins.");
		return false;
	}

	if ( !this->isCastleSiegeDefender() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Only the Defensive Alliance can summon Soldiers.");
		return false;
	}

	Guild* pGuild = this->GuildGet();

	if ( !pGuild )
		return false;

	if ( pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(this) != GUILD_RANK_ASISTANT )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Only the Guild Master and Assistant Guild Master can summon Soldiers.");
		return false;
	}

	if ( mercenary_count > 100 )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "The maximum number of Soldiers has been exceeded.");
		return false;
	}

	Monster * mMonster = sObjectMgr->MonsterTryAdd((level==0)?286:287, this->GetWorldId());

	if ( !mMonster )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Soldier summon failed!.");
		return false;
	}

	mMonster->SetWorldId(this->GetWorldId());
	mMonster->SetBasicLocation(x,y,x,y);
	mMonster->SetInstance(this->GetInstance());
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetMoveDistance(0);
	mMonster->SetDirection(1);
	mMonster->SetCastleSiegeJoinSide(1);
	mMonster->AddToWorld();
			
	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Soldier summon succeeded!.");

	mercenary_count++;
	
	return true;
}

bool Player::CreateLifeStone()
{
	if ( this->GetWorldId() != WORLD_CASTLE_SIEGE )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Life Stone can be summoned only in Castle Siege map");
		return false;
	}

	if ( sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't summon the Life Stone after the Siege begins");
		return false;
	}

	Guild* pGuild = this->GuildGet();

	if (!pGuild)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Only the Guild Master can summon Life Stone");
		return false;
	}

	if (pGuild->GetMemberRanking(this) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(this) != GUILD_RANK_ASISTANT)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Only the Guild Master can summon Life Stone");
		return false;
	}

	if ( !this->isCastleSiegeAttacker() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can only be used by the Offensive Alliance");
		return false;
	}

	if ( pGuild->GetLifeStone() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Life Stone has already been summoned");
		return false;
	}

	if ( this->IsInDragonTower() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't summon the Life Stone inside the Dragon Tower");
		return false;
	}

	Monster * mMonster = sObjectMgr->MonsterTryAdd(278, this->GetWorldId());

	if ( !mMonster )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Life Stone summon failed!");
		return false;
	}

	mMonster->SetWorldId(this->GetWorldId());
	mMonster->SetBasicLocation(this->GetX(), this->GetY(), this->GetX(), this->GetY());
	mMonster->SetInstance(this->GetInstance());
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
	mMonster->SetDespawnTime(10 * MINUTE * IN_MILLISECONDS);
	mMonster->SetDespawnTick(GetTickCount());
	mMonster->SetMoveDistance(0);
	mMonster->SetDirection(1);
	mMonster->SetCastleSiegeJoinSide(this->GetCastleSiegeJoinSide());
	mMonster->AddToWorld();

	pGuild->SetLifeStone(mMonster);

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Life Stone summon succeeded!");
	return true;
}

void Player::CommandSend(uint8 type, uint8 cmd1, uint8 cmd2)
{
	SERVER_COMMAND pMsg(type,cmd1,cmd2);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::DamagePet(int32 damage)
{
	if ( damage <= 0 )
		return;

	Item * pet = this->GetInventory()->GetItem(GUARDIAN);

	if ( !pet->IsItem() || pet->GetDurability() < 1.0f )
		return;

	if ( pet->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION) )
		return;

	if ( !pet->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
		return;

	if ( pet->GetExpireDate() )
		return;

	float fN = 10.0f;
	float fdamage = damage;
	bool send_dur = false;

	if ( this->GetItemDurabilityReduction(pet->GetDurabilityReduction()) > 0.0f )
	{
		fN = this->GetItemDurabilityReduction(pet->GetDurabilityReduction());
	}

	if ( pet->GetItem() == PET_GUARDIAN_ANGEL )
	{
		fdamage = ((damage * 3) / 10) / fN;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_SATAN )
	{
		fdamage = ((damage * 2) / 10) / fN;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_DEMON )
	{
		fdamage = ((damage * 3) / 10) / 20.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_SPIRIT_OF_GUARDIAN )
	{
		fdamage = ((damage * 2) / 10) / 20.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_RUDOLF )
	{
		fdamage = ((damage * 3) / 10) / 10.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_PANDA )
	{
		fdamage = (damage / 20) / 20.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_UNICORN )
	{
		fdamage = (damage / 20) / 20.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if ( pet->GetItem() == PET_SKELETON )
	{
		fdamage = (damage / 20) / 10.0f;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}

	if ( !send_dur )
	{
		return;
	}

	this->SendItemDurability(GUARDIAN, 0);

	if ( pet->GetDurability() < 1.0f )
	{
		uint16 item_tmp = pet->GetItem();

		this->ClearItem(GUARDIAN);
		
		if ( this->IsWorldFlag(WORLD_FLAG_ALLOW_AUTO_PET_CHANGE) )
		{
			if ( item_tmp == PET_SATAN || item_tmp == PET_GUARDIAN_ANGEL )
			{
				this->EquipmentAutochange(GUARDIAN, item_tmp, 0);
			}
		}

		this->PreviewSend();
		this->CalculateCharacter();
	}
}

void Player::DamageMount(int32 damage)
{
	if (damage <= 0)
	{
		return;
	}

	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item * pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || !pItem->IsMount() || pItem->GetDurability() < 1)
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		this->DamageMount(damage, pItem);
	}
}

void Player::DamageMount(int32 damage, Item* pet)
{
	if (pet->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION))
	{
		return;
	}

	if (!pet->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE))
	{
		return;
	}

	if (pet->GetExpireDate())
	{
		return;
	}

	uint8 slot = pet->GetOwnerSlot();

	float fN = 10.0f;
	float fdamage = damage;
	bool send_dur = false;

	if (this->GetItemDurabilityReduction(pet->GetDurabilityReduction()) > 0.0f)
	{
		fN = this->GetItemDurabilityReduction(pet->GetDurabilityReduction());
	}

	if (pet->GetItem() == PET_UNIRIA)
	{
		fdamage = ((damage) / 10) / fN;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if (pet->GetItem() == PET_DINORANT)
	{
		fdamage = ((damage) / 20) / fN;
		pet->AlterDurability(0, fdamage);
		send_dur = true;
	}
	else if (pet->GetItem() == PET_DARK_HORSE)
	{
		fdamage = (((damage * 2) / 10) / fN) + 1;
		pet->IncreaseDurabilityState(fdamage);
		pet->SetState(ITEM_CHANGED);

		float max_dur_state = pet->GetDurabilityResistance() + (pet->GetDurabilityResistance() * this->MagicGetValue(SKILL_TREE_PET_REDUCTION_HP) / 100.0f);

		if (pet->GetDurabilityState() > max_dur_state)
		{
			pet->AlterDurability(0, 1.0f);
			pet->SetDurabilityState(0);

			if (pet->GetDurability() < 1.0f)
			{
				int32 pet_experience = pet->pet_dark.experience;

				if (pet->PetExeDownPercent(10))
				{
					this->PetInfoLevelUp(slot, 1);
				}
			}

			this->SendItemDurability(slot, 0);
			this->PetInfoResult(1, 0, slot);
		}

		return;
	}
	else if (pet->GetItem() == PET_FENRIR)
	{
		fdamage = ((damage * 2 / 10.0f) / fN) + 1.0f;

		float iMaxDurSmall = pet->GetDurabilityResistance();

		pet->IncreaseDurabilityState(fdamage);
		pet->SetState(ITEM_CHANGED);

		if (pet->GetDurabilityState() > iMaxDurSmall)
		{
			pet->AlterDurability(0, 1.0f);
			pet->SetDurabilityState(0.0f);
			this->SendItemDurability(slot, 0);
		}

		return;
	}

	if (!send_dur)
	{
		return;
	}

	this->SendItemDurability(slot, 0);

	if (pet->GetDurability() < 1.0f)
	{
		this->InventoryItemUse(0xFF, slot);
		this->ClearItem(slot);
	}
}

void Player::ApplyWingDamage(int32 & damage, uint16 skill)
{
	if ( damage <= 0 )
	{
		return;
	}

	if ( !this->GetInventory()->GetItem(WINGS)->IsValid() )
	{
		return;
	}

	if ( !this->GetInventory()->GetItem(WINGS)->IsMiniWing() ) ///- Solamente reduzco hp cuando usan alas normales
	{
		switch ( this->GetClass() )
		{
		case Character::DARK_WIZARD:
		case Character::FAIRY_ELF:
		case Character::SUMMONER:
			{
				this->PowerReduce(POWER_LIFE, 1, true);
			} break;

		case Character::RAGE_FIGHTER:
			{
				if( skill == 260 || skill == 261 || skill == 270 || skill == 264 )
				{
					this->PowerReduce(POWER_LIFE, 2, true);
				}
				else if( skill == 262 || skill == 269 || skill == 263 )
				{
					this->PowerReduce(POWER_LIFE, 4, true);
				}
				else if( skill == 265 )
				{
					this->PowerReduce(POWER_LIFE, 100, true);
				}
				else
				{
					this->PowerReduce(POWER_LIFE, 3, true);
				}
			} break;

		default:
			{
				this->PowerReduce(POWER_LIFE, 3, true);
			} break;
		}
	}

	if ( this->PowerGet(POWER_LIFE) <= 0 )
	{
		return;
	}

	int32 inc = this->GetWingDamage();

	if ( inc <= 0 )
	{
		return;
	}

	damage += damage * inc / 100;

	if ( damage < 0 )
	{
		damage = 0;
	}
}

void Player::ApplyFenrirEquipmentReduction()
{
	uint8 slot = Random<uint8>(5) + 2;
	Item * equipment = this->GetInventory()->GetItem(slot);

	if ( !equipment->IsItem() )
		return;

	if ( equipment->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION) )
		return;

	if ( !equipment->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
		return;

	int32 value = 50 + (7 * this->GetItemDurabilityReduction(equipment->GetDurabilityReduction()));

	limitmin(value, 1);

	float durability = equipment->GetDurability() * value / 100.0f;

	equipment->AlterDurability(2, durability);

	this->SendItemDurability(slot, 0);
}

void Player::ItemDurabilityDown(DurabilityState state, uint8 slot)
{
	if ( state == DURABILITY_STATE_NONE )
		return;

	if ( this->GetInventory()->GetItem(slot)->GetDurability() < 1.0f && this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_DURABILITY_DESTROY) )
	{
		this->ClearItem(slot);
		state = DURABILITY_STATE_CHANGED;
	}
	else
	{
		this->SendItemDurability(slot, 0);
	}
	
	if ( state == DURABILITY_STATE_CHANGED )
	{
		this->CalculateCharacter();
	}
}

void Player::WeaponDurabilityDown(Unit* pTarget, bool melee)
{
	if ( this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION) )
	{
		return;
	}

	int32 target_defense = pTarget->GetIntData(UNIT_INT_DEFENSE);
	Item * Right = this->GetInventory()->GetItem(WEAPON_01);
	Item * Left = this->GetInventory()->GetItem(WEAPON_02);

	if( melee )
	{
		if ( (this->GetClass() == Character::DARK_KNIGHT || 
			  this->GetClass() == Character::MAGIC_GLADIATOR || 
			  this->GetClass() == Character::DARK_LORD || 
			  this->GetClass() == Character::RAGE_FIGHTER || 
			  this->GetClass() == Character::GROW_LANCER ||
			  this->GetClass() == Character::SLAYER) &&
			  (Right->GetItem() >= 0 && Right->GetItem() < ITEMGET(4, 0) && Left->GetItem() >= 0 && Left->GetItem() < ITEMGET(4, 0)) )
		{
			if ( Random(2) == 0 )
			{
				this->ItemDurabilityDown(Left->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Left->GetDurabilityReduction())), WEAPON_02);
			}
			else
			{
				this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
			}
		}
		else if( Left->GetKind2() == ItemKind::BOW )
		{
			this->ItemDurabilityDown(Left->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Left->GetDurabilityReduction())), WEAPON_02);
		}
		else if( Right->GetKind2() == ItemKind::CROSSBOW )
		{
			this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
		}
		else if( Right->GetItem() >= ITEMGET(0, 0) && Right->GetItem() < ITEMGET(6, 0) && Right->GetItem() != ITEMGET(0, 41) )
		{
			this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
		}
	}
	else
	{
		if ( pTarget->IsCreature() )
		{
			target_defense = pTarget->GetIntData(UNIT_INT_DEFENSE_MAGIC);
		}

		if ( this->GetClass() == Character::SUMMONER )
		{
			if( Right->GetItem() >= ITEMGET(5, 0) && Right->GetItem() < ITEMGET(6, 0) && Left->GetItem() >= ITEMGET(5, 0) && Left->GetItem() < ITEMGET(6, 0) )
			{
				if(rand()%2==0)
				{
					this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
				}
				else
				{
					this->ItemDurabilityDown(Left->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Left->GetDurabilityReduction())), WEAPON_02);
				}
				return;
			}
		
			if( Right->GetItem() >= ITEMGET(5, 0) && Right->GetItem() < ITEMGET(6, 0) )
			{
				this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
				return;
			}
		
			if( Left->GetItem() >= ITEMGET(5, 0) && Left->GetItem() < ITEMGET(6, 0) )
			{
				this->ItemDurabilityDown(Left->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Left->GetDurabilityReduction())), WEAPON_02);
				return;
			}
		}
		else
		{
			if( Right->GetItem() >= ITEMGET(5, 0) && Right->GetItem() < ITEMGET(6, 0) )
			{
				this->ItemDurabilityDown(Right->DurabilityDownWeapon(target_defense, this->GetItemDurabilityReduction(Right->GetDurabilityReduction())), WEAPON_01);
			}
		}
	}
}

void Player::DecreaseWeaponDurabilityInCastle(float value)
{
	Item * Right = this->GetInventory()->GetItem(WEAPON_01);
	Item * Left = this->GetInventory()->GetItem(WEAPON_02);

	bool right_durability_down = false;
	bool left_durability_down = false;

	if (this->GetClass() == Character::DARK_KNIGHT || this->GetClass() == Character::MAGIC_GLADIATOR || this->GetClass() == Character::DARK_LORD || this->GetClass() == Character::RAGE_FIGHTER || this->GetClass() == Character::SLAYER)
	{
		if ( Right->GetItem() >= ITEMGET(0,0) && Right->GetItem() < ITEMGET(4,0) && Left->GetItem() >= ITEMGET(0,0) && Left->GetItem() < ITEMGET(4,0) )
		{
			right_durability_down = true;
			left_durability_down = true;
		}
	}

	if (Right->GetKind2() == ItemKind::CROSSBOW || Right->GetKind2() == ItemKind::BOW)
	{
		right_durability_down = true;
	}
	if ( Right->GetItem() >= ITEMGET(0,0) && Right->GetItem() < ITEMGET(4,0) )
	{
		right_durability_down = true;
	}
	if ( Right->GetItem() >= ITEMGET(5,0) && Right->GetItem() < ITEMGET(6,0) )
	{
		right_durability_down = true;
	}

	if ( !Right->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
	{
		right_durability_down = false;
	}
	if ( !Left->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
	{
		left_durability_down = false;
	}

	if ( right_durability_down )
	{
		uint8 ret = Right->DurabilityDownSimple(value);

		if ( ret )
		{
			this->SendItemDurability(WEAPON_01, 0);

			if ( ret == 1 )
			{
				this->CalculateCharacter();
			}
		}
	}

	if ( left_durability_down )
	{
		uint8 ret = Left->DurabilityDownSimple(value);

		if ( ret )
		{
			this->SendItemDurability(WEAPON_02, 0);

			if ( ret == 1 )
			{
				this->CalculateCharacter();
			}
		}
	}
}

void Player::UpdateDurabilityTime()
{
	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( !this->IsLive() )
	{
		return;
	}

	bool recalculate_character = false;
	bool weared_item = false;
	uint8 ret = 0;

	uint16 pet_item = -1;
	uint8 pet_level = 0;
	
	wear_inventory_loop(i)
	{
		Item* item = this->GetInventory()->GetItem(i);

		ret = item->ProcessDurabilityTime(this);

		if (item->GetDurability() < 1.0f && item->IsFlag(ITEM_FLAG_DURABILITY_DESTROY))
		{
			if (i == GUARDIAN)
			{
				pet_item = item->GetItem();
				pet_level = item->GetLevel();
			}

			this->ClearItem(i);
		}
		else if (ret)
		{
			this->SendItemDurability(i, 0);
		}

		if (ret)
		{
			recalculate_character = true;
			weared_item = true;
		}
	}

	{
		Item* item = this->GetInventory()->GetItem(EARRING_01);

		ret = item->ProcessDurabilityTime(this);

		if (item->GetDurability() < 1.0f && item->IsFlag(ITEM_FLAG_DURABILITY_DESTROY))
		{
			this->ClearItem(EARRING_01);
		}
		else if (ret)
		{
			this->SendItemDurability(EARRING_01, 0);
		}

		if (ret)
		{
			recalculate_character = true;
			weared_item = true;
		}
	}

	{
		Item* item = this->GetInventory()->GetItem(EARRING_02);

		ret = item->ProcessDurabilityTime(this);

		if (item->GetDurability() < 1.0f && item->IsFlag(ITEM_FLAG_DURABILITY_DESTROY))
		{
			this->ClearItem(EARRING_02);
		}
		else if (ret)
		{
			this->SendItemDurability(EARRING_02, 0);
		}

		if (ret)
		{
			recalculate_character = true;
			weared_item = true;
		}
	}

	use_inventory_loop(i)
	{
		Item* item = this->GetInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( !item->IsInventoryActive() )
		{
			continue;
		}

		if ( item->GetExpireDate() )
		{
			continue;
		}

		ret = item->ProcessDurabilityTime(this);

		if ( item->GetDurability() < 1.0f && item->IsFlag(ITEM_FLAG_DURABILITY_DESTROY) )
		{
			this->InventoryItemUse(0xFF, i);
			this->ClearItem(i);
		}
		else if ( ret )
		{
			this->SendItemDurability(i, 0);

			if (item->GetDurability() < 1.0f)
			{
				this->InventoryItemUse(0xFF, i);
			}
		}

		if (ret)
		{
			recalculate_character = true;
			weared_item = true;
		}
	}

	bool update_item = false;

	if ( this->GetLastItemUpdate() != sItemMgr->GetLastItemUpdate() )
	{
		this->SetLastItemUpdate(sItemMgr->GetLastItemUpdate());
		update_item = true;
	}

	full_inventory_loop(i)
	{
		Item* item = this->GetInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if ( item->IsExpired() )
		{
			if ( i == GUARDIAN )
			{
				pet_item = item->GetItem();
				pet_level = item->GetLevel();
			}

			sMonsterSoul->ExpireSoulConverter(this, item);

			this->InventoryItemUse(0xFF, i);
			this->ClearItem(i);
			recalculate_character = true;

			if ( i < PLAYER_MAX_EQUIPMENT || i == PENTAGRAM_SLOT )
			{
				weared_item = true;
			}

			continue;
		}

		if ( update_item )
		{
			item->Convert(true);

			this->SendInventory(i);
		}
	}

	if ( pet_item != uint16(-1) )
	{
		if ( this->IsWorldFlag(WORLD_FLAG_ALLOW_AUTO_PET_CHANGE) )
		{
			if ( pet_item == PET_SATAN || pet_item == PET_GUARDIAN_ANGEL )
			{
				this->EquipmentAutochange(GUARDIAN, pet_item, pet_level);
			}
		}
	}

	if ( weared_item )
		sItemMgr->IsUsingTransformationItem(this);

	if ( recalculate_character )
		this->CalculateCharacter();

	if ( weared_item )
		this->PreviewSend();
}

void Player::RandomArmorDurabilityDown(Unit* mAttacker, int32 damage)
{
	uint8 slot = Random<uint8>(5) + 2;

	if ( !this->GetInventory()->GetItem(slot)->IsItem() )
		 return;

	if ( !this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
		return;

	if ( this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION) )
		return;

	Item * item = this->GetInventory()->GetItem(slot);
	
	this->ItemDurabilityDown(item->DurabilityDownArmor(static_cast<float>(damage), this->GetItemDurabilityReduction(item->GetDurabilityReduction())), slot);
}

void Player::RandomArmorDurabilityDownPercent(Unit* mAttacker, int32 percent)
{
	uint8 slot = Random<uint8>(5) + 2;

	if ( !this->GetInventory()->GetItem(slot)->IsItem() )
		 return;

	if ( !this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_DURABILITY_DAMAGE) )
		return;

	if ( this->GetInventory()->GetItem(slot)->IsFlag(ITEM_FLAG_TALISMAN_ITEM_PROTECTION) && this->HasBuff(BUFF_TALISMAN_OF_ITEM_PROTECTION) )
		return;

	Item * item = this->GetInventory()->GetItem(slot);

	this->ItemDurabilityDown(item->DurabilityDownArmor(item->GetDurability() * percent / 100, this->GetItemDurabilityReduction(item->GetDurabilityReduction())), slot);
}

void Player::Respawn()
{
	this->GetTalismanResurrection()->Set(this->GetLocation());

	this->CalculateCharacter();
	
	for ( uint8 i = 0; i < POWER_MAX; ++i )
	{
		this->PowerSet(i, this->PowerGetTotal(i));
	}

	this->SetMiniMapState(false);
	this->SetSentMapState(false);
	//this->SetMiniMapWorld(-1);

	this->SetTeleportStatus(TELEPORT_NONE);
	this->SetViewState(VIEW_STATE_NONE);
	this->SelfDefenseClear();
	this->ViewportClear();
	this->UpdateAutorecuperation();
	this->DarkSpiritSetMode(DARK_SPIRIT_MODE_NORMAL, nullptr);
	this->UpdateLastLocation();
	this->GenerateRespawnLocation();
	this->SetKiller(nullptr);
	this->SetCurrentGate(0);
	
	uint16 server_move = sWorldMgr->AllowMoveToWorld(this, this->GetWorldId());

	if ( server_move != sGameServer->GetServerCode() )
	{
		//if ( server_move == uint16(-1) )
		//	return;

		sAuthServer->PlayerServerMoveRequest(this, server_move, this->GetWorldId(), this->GetX(), this->GetY());
		return;
	}

	if ( AW_MAP_RANGE(this->GetWorldId()) && sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING )
	{
		this->AddBuff(BUFF_RESURRECTION_PENALTY, 90, BUFF_FLAG_DB_STORE, this);
	}
	
	this->ClearPathData();
	this->SetTX(this->GetX());
	this->SetTY(this->GetY());
	this->CreateFrustrum();
	this->RestoreEventAnimation();

	this->punish_map.clear();

	this->SetDeadStatus(DEAD_NONE);
	this->SetState(OBJECT_STATE_STAND_BY);
	this->SetLive(true);

	PLAYER_RESPAWN pMsg;
	pMsg.x = this->GetX();
	pMsg.y = this->GetY();
	pMsg.world = this->GetDisplayWorld();
	pMsg.direction = this->GetDirection();

	POWER_LOOP(i)
	{
		pMsg.power[i] = GET_MAX_WORD_VALUE(this->PowerGet(i));
	}

	pMsg.zen = this->MoneyGet();

	if ( this->IsMajestic() )
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience(), pMsg.experience);
	}
	else if ( this->IsMaster() )
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MASTER)->GetExperience(), pMsg.experience);
	}
	else
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience() + sCharacterBase->GetCharacterExperience(LEVEL_DATA_NORMAL, this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel(), true), pMsg.experience);
	}

	pMsg.labyrinth_id = this->GetLabyrinthData()->GetID();

	pMsg.ViewHP = this->PowerGet(POWER_LIFE);
	pMsg.ViewMP = this->PowerGet(POWER_MANA);
	pMsg.ViewSD = this->PowerGet(POWER_SHIELD);
	pMsg.ViewAG = this->PowerGet(POWER_STAMINA);

	this->sendPacket(MAKE_PCT(pMsg));

	this->ManaSend(true);
	this->ManaSend(false);

	this->LifeSend(true);
	this->LifeSend(false);

	this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);

	sWorldMgr->SendUrukMountainZone(this);
	sWorldMgr->SendFereaZone(this);
	sSwampOfDarkness->SendState(this);

	sLabyrinthDimensions->Respawn(this);
	
	//sMiniMap->GCMiniMapInfoSend(this);

	sTeleport->SendBattleZone(this, this->GetWorldId());

	this->SendExperienceEvent();
}

void Player::GenerateRespawnLocation(bool from_select)
{
	int16 x = this->GetX();
	int16 y = this->GetY();
	World* pWorld = this->GetWorld();
	int32 instance = -1;
	uint16 world = this->GetWorldId();
	Guild* pGuild = this->GuildGet();

	SafeAssert(pWorld, "pWorld == nullptr");

	if ( !from_select && pGuild && this->IsInGuildWar() )
	{
		if ( pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL )
		{
			pWorld->GetRandomLocation(x, y, 18);	
		}
		else if ( pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_SOCCER )
		{
			sBattleSoccerMgr->GetRespawnLocation(x, y, this);
		}
	}
	else if ( this->IsDueling() && (this->IsDuelClassic() || (!this->IsDuelClassic() && this->GetWorldId() == WORLD_VULCANUS_ROOM)) )
	{
		sDuelMgr->GenerateRespawn(this, world, x, y);
	}
	else if ( world == WORLD_CASTLE_SIEGE )
	{
		if ( !pGuild || !pGuild->LifeStoneTeleport(this, x, y) )
		{
			this->GetValidCoordinates(this->isCastleSiegeDefender() ? 105: 100, world, x, y);
		}
	}
	else if ( world == WORLD_CRYWOLF_FIRST_ZONE )
	{
		if ( sCrywolf->GetState() == CRYWOLF_STATE_START )
		{
			this->GetValidCoordinates(118, world, x, y);
		}
		else
		{
			this->GetValidCoordinates(258, world, x, y);
		}
	}
	else if ( world == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
	{
		world = g_LabyrinthOfDimensionsWorldData[0].world;
		x = g_LabyrinthOfDimensionsWorldData[0].x;
		y = g_LabyrinthOfDimensionsWorldData[0].y;
		instance = this->GetGUID();
	}
	else if (sDungeon->IsDungeon(world))
	{
		sDungeon->GetDungeonRespawn(this, world, x, y);
	}
	else
	{
		pWorld->GetRespawn(world, x, y);
	}

	if (IF_MAP_RANGE(pWorld))
	{
		ImperialFortressPlayer* pPlayer = sImperialFortressMgr->FindPlayer(this);
		if (pPlayer && sGameServer->IsImperialFortressMoveToPrevious())
		{
			world = pPlayer->GetWorld();
			x = pPlayer->GetX();
			y = pPlayer->GetY();
		}
	}

	this->SetTX(x);
	this->SetTY(y);
	this->SetX(x);
	this->SetY(y);
	this->SetWorldId(world);
	this->SetInstance(instance);
	this->SetWalkSpeedCount(0);
	this->GetACPathData()->Reset();
}

void Player::LocationChange()
{
	if (this->GetNova()->IsRunning())
	{
		this->RunNova();
	}

	if (this->IsDelayedTeleport() && !this->GetHelper()->IsStarted())
	{
		this->CancelDelayedTeleport();
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Delayed teleport canceled.");
	}
}

void Player::UpdateDelayedTeleport()
{
	if ( !this->IsDelayedTeleport() )
		return;

	this->ReduceDelayedTeleportTime(1);

	if ( this->GetDelayedTeleportTime() > 0 )
	{
		if ( this->GetDelayedTeleportTime() <= 5 )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Will be summoned in %d seconds.", this->GetDelayedTeleportTime());
		}
	}
	else
	{
		this->SetDelayedTeleportTime(0);
		this->SetDelayedTeleport(false);

		bool summon_world = this->SummonCheckWorld();
		bool busy = this->IsBusy();
		bool equipment = this->CanEnterWorld(this->GetDelayedLocation()->GetWorldId(), this->GetDelayedLocation()->GetX(), this->GetDelayedLocation()->GetY(), this->GetDelayedLocation()->GetX(), this->GetDelayedLocation()->GetY());

		if ( !summon_world || busy || !equipment )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Summoning Canceled.");
		}
		else
		{
			sDungeonRace->ResetPlayer(this);
			sLosttowerRace->ResetPlayer(this);

			if ( this->GetHelper()->IsOffline() )
			{
				this->GetHelper()->SetX(this->GetDelayedLocation()->GetX());
				this->GetHelper()->SetY(this->GetDelayedLocation()->GetY());
			}

			if ( this->GetWorldId() == this->GetDelayedLocation()->GetWorldId() )
			{
				this->MagicAttackSend(ENTRY(this), SKILL_TELEPORT);
				this->SkillTeleportUse(this->GetDelayedLocation()->GetX(), this->GetDelayedLocation()->GetY());
				this->SetInstance(this->GetDelayedLocation()->GetInstance());
			}
			else
			{
				this->TeleportToDelayed();
			}

			this->SetCurrentGate(this->GetDelayedTeleportGate());
		}
	}
}

void Player::InventoryItemUse(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_LOG(INVENTORY_ITEM_USE, lpMsg, Packet, 0);

	this->InventoryItemUse(lpMsg->use_type, lpMsg->slot, true);
}

bool Player::InventoryItemUse(uint8 use_type, uint8 slot, bool from_packet)
{
	if (!use_inventory_range(slot))
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	/*0xFE = Apply ... 0xFF = Remove*/
	if (use_type != 0xFE && use_type != 0xFF)
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	Item * pItem = this->GetInventory()->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

	if (!item_info)
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	if (!item_info->IsFlag(ITEM_FLAG_INVENTORY_ITEM))
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	buff_template const* buff = sSkillMgr->GetBuffItem(pItem->GetItem());

	if (!buff)
	{
		this->InventoryItemUseResult(slot, -1);
		return false;
	}

	if (pItem->IsMount() || pItem->GetItem() == PET_DARK_RAVEN)
	{
		if (sGameServer->GetInventoryMountTime() > 0 && from_packet && !this->GetTimer(PLAYER_TIMER_NORMAL_MOUNT)->Elapsed(sGameServer->GetInventoryMountTime()))
		{
			sLog->outInfo("antihack", "[Inventory Mount]: [%u][%s][%s]", sGameServer->GetServerCode(), this->BuildLog().c_str(), this->BuildLocationLog().c_str());
			return false;
		}

		if (use_type == 0xFE)
		{
			pItem->SetValid(this->ValidItem(pItem));

			if (!pItem->IsValid())
			{
				return false;
			}
		}
	}

	if (use_type == 0xFE)
	{
		this->InventoryItemClearGroup(pItem->GetItem());
	}
	else
	{
		this->RemoveBuff(buff);
	}

	pItem->SetInventoryActive(use_type == 0xFE ? true : false);
	pItem->SetState(ITEM_CHANGED);

	if (use_type == 0xFE)
	{
		if (pItem->IsMount())
		{
			if (pItem->GetSkill())
			{
				int32 result = this->MagicAddWeapon(pItem->GetSkillID(), 0);

				if (result != -1)
				{
					this->SendSkill(result, this->GetSkill(result)->GetSkill(), this->GetSkill(result)->GetLevel());
				}
			}

			this->PreviewMake();
		}
		else if (pItem->GetItem() == PET_DARK_RAVEN)
		{
			this->PreviewMake();
		}

		this->AddBuff(buff->GetBuff(),
			BuffEffect(BuffOption(buff->GetEffect(0)), buff->GetValue(0)),
			BuffEffect(BuffOption(buff->GetEffect(1)), buff->GetValue(1)),
			0, BUFF_FLAG_CONSTANT, this);
	}
	else
	{
		if (pItem->IsMount())
		{
			this->CalculateCharacter();

			this->PreviewMake();
		}
	}

	this->SendEquipment(slot);

	this->InventoryItemUseResult(slot, use_type);
	return true;
}

void Player::InventoryItemClearGroup(uint16 item)
{
	const auto pItemInfo = sItemMgr->GetItem(item);
	if (!pItemInfo)
	{
		return;
	}

	const auto pBuffInfo = sSkillMgr->GetBuffItem(item);
	if (!pBuffInfo)
	{
		return;
	}

	use_inventory_loop(i)
	{
		auto pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		buff_template const* pBuffInfoTmp = sSkillMgr->GetBuffItem(this->GetInventory()->GetItem(i)->GetItem());

		if (!pBuffInfoTmp)
		{
			continue;
		}

		if ((pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT && pItem->GetKind2() == ItemKind::GUARDIAN_MOUNT) ||
			(pBuffInfo->GetGroup() == pBuffInfoTmp->GetGroup()))
		{
			this->RemoveBuff(pBuffInfoTmp);

			this->GetInventory()->GetItem(i)->SetInventoryActive(false);
			this->GetInventory()->GetItem(i)->SetState(ITEM_CHANGED);
			this->InventoryItemUseResult(i, 0xFF);
		}
	}
}

void Player::InventoryItemUseResult(uint8 slot, uint8 use_type)
{
	INVENTORY_ITEM_USE_ANSWER pMsg(slot, use_type);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::InventoryItemSend()
{
	use_inventory_loop(i)
	{
		Item * pItem = this->GetInventory()->GetItem(i);

		if (!pItem->IsItem())
		{
			continue;
		}

		if (!sItemMgr->IsItemFlag(pItem->GetItem(), pItem->GetLevel(), ITEM_FLAG_INVENTORY_ITEM))
		{
			continue;
		}

		this->InventoryItemUseResult(i, 0xFF);

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		if (!this->InventoryItemUse(0xFE, i))
		{
			pItem->SetInventoryActive(false);
			pItem->SetState(ITEM_CHANGED);
		}
	}
}

bool Player::hasMoonstoneRing()
{
	if ( this->GetInventory()->GetItem(RING_01)->IsValid() &&
		 this->GetInventory()->GetItem(RING_01)->GetItem() == ITEMGET(13,38) )
	{
		return true;
	}

	if ( this->GetInventory()->GetItem(RING_02)->IsValid() &&
		 this->GetInventory()->GetItem(RING_02)->GetItem() == ITEMGET(13,38) )
	{
		return true;
	}

	return false;
}

void Player::AddNormalPoints(int32 points, uint8 stat)
{
	if ( stat == LEADERSHIP && this->GetClass() != Character::DARK_LORD )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Only available for Dark Lord");
		return;
	}

	if ( points <= 0)
		return;

	if ( points > this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints() )
		points = this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints();

	if ( this->GetStat(stat) + points > sGameServer->GetMaxStat(stat) )
		points = sGameServer->GetMaxStat(stat) - this->GetStat(stat);

	if ( points <= 0 )
		return;

	this->GetLevelData(LEVEL_DATA_NORMAL)->ReducePoints(points);
	this->IncreaseStat(stat, points);

	//this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added %d points. Left %d points to add", points, this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints());
	this->NormalSendLevelUp(false);


	this->CalculateCharacter();
}

void Player::EventGetRemainTime(uint8 * Packet)
{
	if ( !this->IsPlaying() )
		return;

	POINTER_PCT(EVENT_REMAIN_TIME_REQUEST, lpMsg, Packet, 0);
	
	EVENT_REMAIN_TIME_RESULT pMsg(lpMsg->event_id);

	uint32 remain_time = 0;

	if ( sEventMgr->GetEventRemainTimeData(lpMsg->event_id, remain_time, pMsg.entered_user, this) )
	{
		switch ( lpMsg->event_id )
		{
		case 1:
		case 2:
		case 5:
			{
				pMsg.remain_time = remain_time;
			} break;

		case 4:
			{
				pMsg.remain_time = SET_NUMBERH(remain_time);
				pMsg.remain_time_low = SET_NUMBERL(remain_time);
			} break;
		}

		this->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::EventResetData()
{
	this->SetEventId(-1);
	this->SetEventGround(-1);
	this->SetEventScore(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// SEND TO CLIENT ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::SendJoinInfo()
{
	sTeleport->SendBattleZone(this, this->GetWorldId());

	///- CHANGELOG 03/10
	if ( sGameServer->IsArkaWarEnabled() )
	{
		sArkaWar->SendPlayInfo(this);
		sArkaWar->SendState(this);
	}

	CHARACTER_JOIN_RESULT pMsg;
	pMsg.x = this->GetX();
	pMsg.y = this->GetY();
	pMsg.world = this->GetDisplayWorld();
	pMsg.direction = this->GetDirection();

	if ( this->IsMajestic() )
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience(), pMsg.experience);
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetNextExperience(), pMsg.next_experience);
	}
	else if ( this->IsMaster() )
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MASTER)->GetExperience(), pMsg.experience);
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_MASTER)->GetNextExperience(), pMsg.next_experience);
	}
	else
	{
		Util::ConvertToByte(this->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience() + sCharacterBase->GetCharacterExperience(LEVEL_DATA_NORMAL, this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel(), true), pMsg.experience);
		Util::ConvertToByte(sCharacterBase->GetCharacterExperience(LEVEL_DATA_NORMAL, this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() + 1, true), pMsg.next_experience);
	}

	pMsg.normal_points = this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints();
	pMsg.strength = this->GetStat(STRENGTH);
	pMsg.agility = this->GetStat(AGILITY);
	pMsg.vitality = this->GetStat(VITALITY);
	pMsg.energy = this->GetStat(ENERGY);
	pMsg.leadership = this->GetStat(LEADERSHIP);
	pMsg.zen = this->MoneyGet();
	pMsg.life = GET_MAX_WORD_VALUE(this->PowerGet(POWER_LIFE));
	pMsg.life_max = GET_MAX_WORD_VALUE(this->PowerGetTotal(POWER_LIFE));
	pMsg.mana = GET_MAX_WORD_VALUE(this->PowerGet(POWER_MANA));
	pMsg.mana_max = GET_MAX_WORD_VALUE(this->PowerGetTotal(POWER_MANA));
	pMsg.shield = GET_MAX_WORD_VALUE(this->PowerGet(POWER_SHIELD));
	pMsg.shield_max = GET_MAX_WORD_VALUE(this->PowerGetTotal(POWER_SHIELD));
	pMsg.stamina = GET_MAX_WORD_VALUE(this->PowerGet(POWER_STAMINA));
	pMsg.stamina_max = GET_MAX_WORD_VALUE(this->PowerGetTotal(POWER_STAMINA));

	if ((this->GetWorldId() == WORLD_LAST_MAN_STANDING) || (this->GetWorldId() == WORLD_SPECIAL_EVENT_MAP && sGameServer->IsSpecialMapPVP()))
	{
		pMsg.PkLevel = PK_STATUS_MURDER_LVL_3;
	}
	else
	{
		pMsg.PkLevel = this->GetPKLevel();
	}

	pMsg.authority = this->GetAuthority();

	pMsg.add_point = this->GetFruitPointsAdd();
	pMsg.add_point_max = sCharacterBase->GetStatFruitMaxAdd(this->GetClass(), this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	pMsg.minus_point = this->GetFruitPointsDec();
	pMsg.minus_point_max = sCharacterBase->GetStatFruitMaxDec(this->GetClass(), this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	pMsg.ExpandedInventory = this->GetInventory()->GetExpanded();
	pMsg.r_money = this->GetRuudMoney();

	pMsg.unk1 = 0;
	pMsg.unk2[0] = 0;
	pMsg.unk2[1] = 0;
	pMsg.unk2[2] = 0;

	pMsg.ViewHP = this->PowerGet(POWER_LIFE);
	pMsg.ViewMP = this->PowerGet(POWER_MANA);
	pMsg.ViewSD = this->PowerGet(POWER_SHIELD);
	pMsg.ViewAG = this->PowerGet(POWER_STAMINA);
	pMsg.ViewMaxHP = this->PowerGetTotal(POWER_LIFE);
	pMsg.ViewMaxMP = this->PowerGetTotal(POWER_MANA);
	pMsg.ViewMaxSD = this->PowerGetTotal(POWER_SHIELD);
	pMsg.ViewMaxAG = this->PowerGetTotal(POWER_STAMINA);

	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendJoinInfo(uint8 result)
{
	/*if ( result == 0 )
	{:
		this->EnterWorldSuccess(nullptr);
	}

	return;*/

	//CHARACTER_JOIN_INFO pMsg(result);
	//this->sendPacket((uint8*)&pMsg, pMsg.h.size);
}

void Player::SendInventory()
{
	uint8 buffer[8192];
	POINTER_PCT(INVENTORY_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(INVENTORY_LIST_BODY, body, buffer, sizeof(INVENTORY_LIST_HEAD));
	head->count = 0;

	full_inventory_loop(i)
	{
		if ( !this->GetInventory()->GetItem(i)->IsItem() )
			continue;

		body[head->count].slot = i;
		this->GetInventory()->GetItem(i)->ConvertToBuffer(body[head->count].item);

		++head->count;
	}

	head->h.setE(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_ITEM, sizeof(INVENTORY_LIST_HEAD) + sizeof(INVENTORY_LIST_BODY) * head->count);

	this->sendPacket(buffer, head->h.get_size());

	this->CashShopSendExpireItem();
}

void Player::SendSkill()
{
	uint8 buffer[1000];
	POINTER_PCT(SKILL_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SKILL_LIST_BODY, body, buffer, sizeof(SKILL_LIST_HEAD));
	head->count = 0;

	SKILL_LOOP(i)
	{
		if (!this->GetSkill(i)->GetSkill())
		{
			continue;
		}

		auto & data = body[head->count++];

		data.position = i;
		data.skill = this->GetSkill(i)->GetSkill();
		data.level = this->GetSkill(i)->GetLevel() << 3;
		data.level |= data.skill & 0x07;
	}

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SKILL, sizeof(SKILL_LIST_HEAD)+sizeof(SKILL_LIST_BODY)* head->count);
	head->mode = 0;

	this->sendPacket(buffer, head->h.get_size());

	SKILL_LOOP(i)
	{
		if (!this->GetSkill(i)->GetSkill())
		{
			continue;
		}

		const auto pSkillTree = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), this->GetSkill(i)->GetSkill());
		if (!pSkillTree)
		{
			continue;
		}

		this->SendSkill(i, this->GetSkill(i)->GetSkill(), this->GetSkill(i)->GetLevel());
	}
}

void Player::SendSkill(uint8 slot, uint16 skill, uint8 level, bool add)
{
	uint8 buffer[1000];
	POINTER_PCT(SKILL_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SKILL_LIST_BODY, body, buffer, sizeof(SKILL_LIST_HEAD));
	head->count = add ? 0xFE: 0xFF;
	body[0].position = slot;
	body[0].skill = skill;
	body[0].level = level << 3;
	body[0].level |= skill & 0x07;

	head->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SKILL, sizeof(SKILL_LIST_HEAD) + sizeof(SKILL_LIST_BODY));
	head->mode = 0;

	this->sendPacket(buffer, head->h.get_size());
}

void Player::SendGameOption()
{
	GAME_OPTION pMsg;

	pMsg.game_option = this->GetGameOption()->GetOption();
	pMsg.chat_window = this->GetGameOption()->GetChatWindow();
	pMsg.key_q = this->GetGameOption()->GetKeyQ();
	pMsg.key_w = this->GetGameOption()->GetKeyW();
	pMsg.key_e = this->GetGameOption()->GetKeyE();
	pMsg.key_r = this->GetGameOption()->GetKeyR();
	pMsg.qwer_level = this->GetGameOption()->GetQWERLevel();

	for (int32 i = 0; i < SKILL_KEY_COUNT; ++i)
	{
		pMsg.skill_key[(2 * i)] = SET_NUMBERH(this->GetGameOption()->GetSkillKey(i));
		pMsg.skill_key[(2 * i) + 1] = SET_NUMBERL(this->GetGameOption()->GetSkillKey(i));
	}

	pMsg.additional_options = this->GetGameOption()->GetAdditionalOptions();

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		pMsg.linked_button[i] = this->GetGameOption()->GetButtonLink(i);
	}

	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::SendFriendList()
{
	uint8 buffer[8192];
	POINTER_PCT(FRIEND_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(FRIEND_LIST_BODY, body, buffer, sizeof(FRIEND_LIST_HEAD));
	head->friend_count = 0;

	FRIEND_LOOP(i)
	{
		if ( strlen(this->GetFriend(i)->GetName()) > 0 )
		{
			memcpy(body[head->friend_count].name, this->GetFriend(i)->GetName(), MAX_CHARACTER_LENGTH);
			body[head->friend_count].server = this->GetFriend(i)->GetServer();
			head->friend_count++;
		}
	}

	head->h.set(HEADCODE_FRIEND_LIST_SEND, sizeof(FRIEND_LIST_HEAD) + (head->friend_count * sizeof(FRIEND_LIST_BODY)));
	head->mail_count = this->MailGetCount();
	head->mail_total = MAIL_MAX;

	this->sendPacket(buffer, head->h.get_size());
}

void Player::MoneySend()
{
	ZEN_SEND pMsg(this->MoneyGet());
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::MoneySend(uint32 _zen)
{
	ZEN_SEND pMsg(_zen);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::NormalSendLevelUp(bool effect)
{
	NORMAL_LEVELUP_SEND pMsg;
	pMsg.level = this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();
	pMsg.points = this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints();

	POWER_LOOP(i)
		pMsg.power_max[i] = GET_MAX_WORD_VALUE(this->PowerGetTotal(i));

	pMsg.add_point = this->GetFruitPointsAdd();
	pMsg.add_point_max = sCharacterBase->GetStatFruitMaxAdd(this->GetClass(), this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	pMsg.minus_point = this->GetFruitPointsDec();
	pMsg.minus_point_max = sCharacterBase->GetStatFruitMaxDec(this->GetClass(), this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	pMsg.ViewMaxHP = this->PowerGetTotal(POWER_LIFE);
	pMsg.ViewMaxMP = this->PowerGetTotal(POWER_MANA);
	pMsg.ViewMaxSD = this->PowerGetTotal(POWER_SHIELD);
	pMsg.ViewMaxAG = this->PowerGetTotal(POWER_STAMINA);
	this->sendPacket(MAKE_PCT(pMsg));

	if ( effect )
	{
		this->EffectSend(0x10);
	}
}

void Player::Summon(uint32 id, uint32 time)
{

}

void Player::EnterWorldSuccess(uint8 * Packet)
{
	this->SendInventory();
	this->SendSkill();

	this->InventoryItemSend();

	this->SendGameOption();
	this->SendFriendList();
	this->FriendMailList();
	this->HelperSendSettings();
	this->MasterSendStatus();
	this->MasterSkillSend();
	this->QuestEvolutionSendInfo();
	this->CashShopSendDate();
	this->ChatBlockList(1, 1);

	sMuunSystem->GCMuunItemListSend(this);
	sMuunSystem->GCMuunItemExpireSend(this);

	sEventInventory->GCEventItemListSend(this);
	sEventInventory->GCEventInventoryEnableSend(this);

	sPentagramSystem->GCPentagramJewelInfoSend(this, PENTAGRAM_JEWEL_TYPE_INVENTORY);
	sPentagramSystem->GCPentagramJewelInfoSend(this, PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

	for (int32 i = 0; i < GREMORY_CASE_TYPE_MAX; ++i)
	{
		sGremoryCase->ItemListSend(this, i);
	}
	
	this->SendPCRoom();

	sServerLink->CharacterOnOff(this, 1);

	if ( !sItemMgr->IsUsingTransformationItem(this) )
	{
		this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
	}

	this->GenSendInfo(true, true);

	sPartyMgr->OnLogin(this);

	this->SendNoticeNormal(NOTICE_GLOBAL, sGameServer->GetWelcomeMessage().c_str());

	this->SetFirstTime(false);

	sVipMgr->SendVipData(this);

	sGuildMgr->SendGuildNotice(this);

	this->ManaSend(true);
	this->ManaSend(false);

	this->LifeSend(true);
	this->LifeSend(false);

	for ( int32 i = 0; i < EVENT_NOTIFICATION_MAX; ++i )
	{
		this->SendEventNotification(i, 0);
	}

	sPentagramSystem->FixPentagram(this);

	this->CalculateCharacter();

	//sMiniMap->GCMiniMapInfoSend(this);
	sGameServer->SendNotification(this);

	this->GetAccountData()->SetCloseGame(false);

	sGameServer->SendOffsetData(this);
	sGameServer->SendOffsetFPS(this);

	this->SendExperienceEvent();

	this->SendEventEnterCount();

	this->WarpFavoriteListSend();
	
	this->QuestGuidedSetStarting();
	this->QuestGuidedSend();

	this->SendQuestMU();
	this->UpdateQuestMULevel();

	this->MajesticSkillTreeSend();
	this->MajesticSkillTreeStatSend();

	this->SendServerList();

	sMonsterSoul->SendStatus(this);

	this->ResetMount(-1);

	sWorldMgr->SendFereaZone(this);
	sWorldMgr->SendUrukMountainZone(this);
	sSwampOfDarkness->SendState(this);

	sCashShopMgr->SendUpdatedData(this);

	sTeleport->SendMonsterInfo(this);
	this->SendAntiMacro();

	this->GenUpdateLevel();

	this->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->SetTimer(0);

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized to use this account.");
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Use /auth <security_code> to authorize it.");
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Example /auth 012345678912");
	}

	this->SetAutoLoginInfo(0);

	if ( this->IsAutoLogin() )
	{
		switch ( this->GetAutoLoginType() )
		{
		case 1: ///- OffStore
		case 2:
			{
				PERSONAL_STORE_STATE pMsg1;
				memcpy(pMsg1.seller, this->GetName(), MAX_CHARACTER_LENGTH);
				memcpy(pMsg1.description, this->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH);

				this->HandlePersonalStoreState((uint8*)&pMsg1);

				if (this->GetPersonalStore()->IsOpen())
				{
					this->GetPersonalStore()->SetOff(true);
					this->GetPersonalStore()->SetOffTime(MyGetTickCount());

					this->UpdateAccountStatusType(1);
				}
			} break;

		case 3: ///- OffAttack
			{
				if ( this->HelperStart() )
				{
					this->GetHelper()->SetStartedTime(MyGetTickCount());
					this->GetHelper()->SetOffline(true);

					this->UpdateAccountStatusType(2);
				}
			} break;
		}
	}

	this->SetAutoLogin(false);

	sLog->outInfo(LOG_PLAYER, "%s -- %s -- %s", __FUNCTION__, this->BuildLog().c_str(), this->BuildLocationLog().c_str());
}

void Player::UpdateStatistic(uint32 statistic_id, uint64 count)
{
	if ( statistic_id >= STATISTIC_MAX )
		return;

	this->GetStatisticData(statistic_id)->IncreaseCount(count);
}

uint64 Player::GetStatistic(uint32 statistic_id)
{
	if ( statistic_id >= STATISTIC_MAX )
		return 0;

	return this->GetStatisticData(statistic_id)->GetCount();
}

void Player::ReduceStatistic(uint32 statistic_id, uint64 count)
{
	if ( statistic_id >= STATISTIC_MAX )
		return;

	StatisticData * statistic_data = this->GetStatisticData(statistic_id);

	if ( statistic_data->GetCount() >= count )
	{
		statistic_data->ReduceCount(count);
	}
	else
	{
		statistic_data->SetCount(0);
	}
}

bool Player::IsActionAllowed(uint32 action, Unit* pTarget, bool notify)
{
	if ( this->HasRestriction(action, notify) )
		return false;

	if ( sGameServer->IsMACRestriction(action, this) )
		return false;

	switch ( action )
	{
	case PlayerAction::PLAYER_ACTION_TRADE:
		{
			if ( !Player::IsPlayerBasicState(this) )
			{
				return false;
			}

			if ( !sGameServer->trade_enabled )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Trade is disabled.");
				this->TradeRequestResult(0, "");
				return false;
			}

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				this->TradeRequestResult(0, "");
				return false;
			}

			if ( this->IsAdministrator() && !this->IsAdministratorFlag(ADMIN_FLAG_TRADE) )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You don't have permission to trade.");
				this->TradeRequestResult(0, "");
				return false;
			}

			if ( !pTarget )
			{
				this->TradeRequestResult(0, "");
				return false;
			}

			if ( !this->CanInteract(pTarget, true) )
			{
				this->TradeRequestResult(0, "");
				return false;
			}

			Player* pPlayer = pTarget->ToPlayer();

			if ( !Player::IsPlayerBasicState(pPlayer, false) )
			{
				this->TradeRequestResult(0, "");
				return false;
			}

			if ( pPlayer->HasRestriction(PlayerAction::PLAYER_ACTION_TRADE) )
			{
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( sGameServer->IsMACRestriction(PlayerAction::PLAYER_ACTION_TRADE, pPlayer) )
			{
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( !pPlayer->IsAuthorizationEnabled() )
			{
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( pPlayer->IsAdministrator() && !pPlayer->IsAdministratorFlag(ADMIN_FLAG_TRADE) )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s doesn't have permission to trade.", pTarget->GetName());
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( !pPlayer->IsFlag(CHARACTER_FLAG_REQUEST) )
			{
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( !this->IsWorldFlag(WORLD_FLAG_ALLOW_EXCHANGE) )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Trade is not allowed on this map.");
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( this->GetInterfaceState()->GetID() != InterfaceData::None || pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You and %s need to be free to trade.", pTarget->GetName());
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( this->GetPersonalStore()->IsBusy() || pPlayer->GetPersonalStore()->IsBusy() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You and %s need to be free to trade.", pTarget->GetName());
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}

			if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN || pPlayer->GetTransaction() == TRANSACTION_TYPE_BEGIN )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You and %s need to be free to trade.", pTarget->GetName());
				this->TradeRequestResult(0, pTarget->GetName());
				return false;
			}
		} break;

	case PlayerAction::PLAYER_ACTION_TRADE_ANSWER:
		{
			if ( !pTarget || !pTarget->IsPlaying() || !pTarget->IsLive() || !pTarget->IsPlayer() )
			{
				return false;
			}

			if ( pTarget->ToPlayer()->HasRestriction(PlayerAction::PLAYER_ACTION_TRADE) )
			{
				return false;
			}

			if ( sGameServer->IsMACRestriction(PlayerAction::PLAYER_ACTION_TRADE, pTarget->ToPlayer()) )
			{
				return false;
			}

			if ( !Player::InterfaceSharedCheck(this, pTarget->ToPlayer(), InterfaceData::Trade, 0) )
			{
				return false;
			}

			if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN || pTarget->ToPlayer()->GetTransaction() == TRANSACTION_TYPE_BEGIN )
			{
				return false;
			}
	
			if ( Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) > 2 )
			{
				return false;
			}

			if ( this->IsServerQuit() || pTarget->ToPlayer()->IsServerQuit() )
			{
				return false;
			}
		} break;

	case PlayerAction::PLAYER_ACTION_PARTY:
		{
			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_DUEL:
		{
			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_DROP:
		{
			if ( !this->IsPlaying() )
				return false;

			if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
				return false;

			if ( this->GetInterfaceState()->GetID() != InterfaceData::None && !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
				return false;

			//if ( this->GetPersonalStore()->IsBusy() )
			//	return false;

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_LOOT:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
				return false;

			if ( !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) && this->GetInterfaceState()->GetID() != InterfaceData::None )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->IsSpectator() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_USE:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetInterfaceState()->GetID() != InterfaceData::None && !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
				return false;

			if ( this->IsSpectator() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_MOVE:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_BUY_NPC:
		{
			if ( !Player::IsPlayerBasicState(this) )
			{
				return false;
			}

			if ( !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) && this->GetInterfaceState()->GetID() != InterfaceData::MixRecovery)
				return false;

			if ( !this->GetInterfaceState()->GetTarget() || !this->GetInterfaceState()->GetTarget()->IsCreature() || !this->GetInterfaceState()->GetTarget()->ToCreature()->IsNpc() )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetShopTime() == 0 )
			{
				this->SetShopTime(1);
			}

			if ( this->GetShopTime() > 60 )
			{
				this->GetInterfaceState()->Reset();
				return false;
			}
		} break;

	case PlayerAction::PLAYER_ACTION_ITEM_SELL_NPC:
		{
			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}

			if ( !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
				return false;

			if ( !this->GetInterfaceState()->GetTarget() || !this->GetInterfaceState()->GetTarget()->IsCreature() || !this->GetInterfaceState()->GetTarget()->ToCreature()->IsNpc() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_TELEPORT:
		{
			if ( this->IsServerQuit() )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_GAIN_EXPERIENCE:
		{
		} break;

	case PlayerAction::PLAYER_ACTION_NPC_INTERACTION:
		{
			if ( !Player::IsPlayerBasicState(this) )
			{
				return false;
			}

			if ( !Player::IsPlayerTransactionFree(this) )
			{
				return false;
			}

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
			{
				return false;
			}

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
			{
				return false;
			}
		} break;

	case PlayerAction::PLAYER_ACTION_ATTACK_REQUEST:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( this->IsTeleporting() )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
				return false;

			if ( this->IsSpectator() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_MAGIC_REQUEST:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
				return false;

			if ( this->IsSpectator() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_CANCEL_MAGIC:
		{
			if ( !this->IsPlaying() )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
				return false;

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_WHISPER:
		{
			if ( !this->ChatCanSpeak() )
				return false;

			if ( !this->Object::IsPlaying() )
				return false;

			if ( this->IsAdministrator() && !this->IsAuthorizationEnabled() )
				return false;

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
				return false;

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_SPEAK:
		{
			if ( !this->ChatCanSpeak() )
				return false;

			if ( !this->Object::IsPlaying() )
				return false;

			if ( this->GetAuthority() == AUTHORITY_CODE_BANNED )
				return false;

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_CASH_SHOP_USE:
		{
			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}

			if ( this->IsWorldFlag(WORLD_FLAG_DISABLE_CASH_SHOP) )
			{
				this->SendMessageBox(1, "Cash Shop", "Can't use Cash Shop on this zone.");
				return false;
			}
		} break;

	case PlayerAction::PLAYER_ACTION_GUILD_JOIN:
		{
			if ( !this->PacketTimeCheck() )
				return false;

			if ( !this->IsPlaying() )
				return false;

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}

			if ( this->IsServerQuit() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_RESET:
		{
			if ( !Player::IsPlayerBasicState(this, false) )
			{
				return false;
			}

			if ( !this->IsAuthorizationEnabled() )
			{
				return false;
			}

			if ( this->GetInterfaceState()->GetID() != InterfaceData::None || this->GetPersonalStore()->IsBusy() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "[ RESET ]: You need to close any menu.");
				return false;
			}

			if ( !this->CanInteract() )
				return false;
		} break;

	case PlayerAction::PLAYER_ACTION_GUILD_CREATE:
		{
			if ( !Player::IsPlayerBasicState(this) )
			{
				return false;
			}

			if ( this->GetInterfaceState()->GetID() != InterfaceData::Guild )
				return false;

			if ( !this->IsAuthorizationEnabled() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
				return false;
			}
		} break;
	}

	return true;
}

void Player::NpcInteractionBarrackTeleport()
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !sGameServer->IsBarracksMoveEnabled() )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_WEREWOLF, 0);
		return;
	}

	if ( this->GetTotalLevel() < sGameServer->GetBarracksMoveMinLevel() )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_WEREWOLF, 0);
		return;
	}

	if ( !this->MoneyHave(sGameServer->GetBarracksMoveCost()) )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_WEREWOLF, 0);
		return;
	}

	uint8 state = this->QuestEvolutionGetState(5);

	if ( state != QUEST_EVOLUTION_STATE_ACCEPTED && state != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_WEREWOLF, 0);
		return;
	}

	this->MoneyReduce(sGameServer->GetBarracksMoveCost());

	if ( sGameServer->IsBarracksMoveParty() )
	{
		Unit* pMember[MAX_PARTY_MEMBERS];
		uint8 count = 0;

		this->GetPartyMembers(pMember, count, 10);

		if ( count > MAX_PARTY_MEMBERS )
			count = 1;

		for ( uint8 i = 0; i < count; ++i )
		{
			if ( !pMember[i] )
			{
				continue;
			}

			Player* pPlayer = pMember[i]->ToPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetTotalLevel() < sGameServer->GetBarracksMoveMinLevel() )
				continue;

			pPlayer->MoveToGate(256);
		}
	}
	else
	{
		this->MoveToGate(256);
	}
}	

void Player::NpcInteractionRefugeTeleport()
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !sGameServer->IsRefugeMoveEnabled() )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_GATE_KEEPER, 0);
		return;
	}

	if ( this->GetTotalLevel() < sGameServer->GetRefugeMoveMinLevel() )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_GATE_KEEPER, 0);
		return;
	}

	if ( !this->MoneyHave(sGameServer->GetRefugeMoveCost()) )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_GATE_KEEPER, 0);
		return;
	}

	uint8 state = this->QuestEvolutionGetState(6);

	if ( state != QUEST_EVOLUTION_STATE_ACCEPTED && state != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		this->NpcInteractionResult(SUBCODE_NPC_INTERACTION_GATE_KEEPER, 0);
		return;
	}

	this->MoneyReduce(sGameServer->GetRefugeMoveCost());

	if ( sGameServer->IsRefugeMoveParty() )
	{
		Unit* pMember[MAX_PARTY_MEMBERS];
		uint8 count = 0;

		this->GetPartyMembers(pMember, count, 10);

		if ( count > MAX_PARTY_MEMBERS )
			count = 1;

		for ( uint8 i = 0; i < count; i++ )
		{
			if ( !pMember[i] )
			{
				continue;
			}

			Player* pPlayer = pMember[i]->ToPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetTotalLevel() < sGameServer->GetRefugeMoveMinLevel() )
			{
				continue;
			}

			pPlayer->MoveToGate(257);
		}
	}
	else
	{
		this->MoveToGate(257);
	}
}

void Player::NpcInteractionResult(uint8 npc, uint8 result)
{
	NPC_INTERACTION pMsg(npc, result);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag)
{
	if ( !pAttacker )
	{
		return;
	}

	Unit * pAttackerMaster = pAttacker->GetSummoner();

	if ( !pAttackerMaster )
	{
		return;
	}

	Player* pPlayer = pAttackerMaster->ToPlayer();

	this->HelperStop(0x01);
	this->SetKillCount(0);
	this->KillCountSend();

	if ( pPlayer && this != pAttackerMaster )
	{
		if ( Party* pParty = pPlayer->GetParty() )
		{
			pParty->SendKillDamage(pPlayer, this, damage, shield_damage, damage_flag);
		}

		if ( CC_MAP_RANGE(pAttackerMaster->GetWorldId()) )
		{
			if ( pAttackerMaster->GetEventId() == EVENT_CHAOS_CASTLE && CHAOS_CASTLE_GROUND_RANGE(pAttackerMaster->GetEventGround()) )
			{
				sChaosCastleMgr->GetGround(pAttackerMaster->GetEventGround())->KillPlayer(pPlayer);
			}
			else if ( pAttackerMaster->GetEventId() == EVENT_CHAOS_CASTLE_SURVIVAL )
			{
				sChaosCastleSurvivalMgr->GetGround()->KillPlayer(pPlayer);
			}
		}

		sEventMgr->CastleSiegeKill(this, pPlayer);
		sEventMgr->ArkaWarKill(this, pPlayer);

		sLog->outInfo(LOG_PLAYER, "[%s][%s] Killed [%s][%s]", pPlayer->BuildLog().c_str(), pPlayer->BuildLocationLog().c_str(), this->BuildLog().c_str(), this->BuildLocationLog().c_str());
	}
	else
	{
		sEventMgr->CastleSiegeKill(this, nullptr);
		sEventMgr->ArkaWarKill(this, nullptr);

		sLog->outInfo(LOG_PLAYER, "[%s][%s] Killed [%s][%s]", this->BuildLog().c_str(), this->BuildLocationLog().c_str(), this->BuildLog().c_str(), this->BuildLocationLog().c_str());
	}

	Player::PKPlayerMurder(this, pAttackerMaster, damage_flag);
	this->OnDie(pAttackerMaster);
}

void Player::UpdateWrongWorld()
{
	if ( !this->IsLive() )
		return;

	if ( this->GetRegenStatus() != REGEN_NONE )
	{
		this->HelperStop(0x01);
		//return;
	}

	if ( this->GetRegenStatus() == REGEN_STANDBY ||
		 this->GetRegenStatus() == REGEN_READY )
		return;

	if ( this->GetPartyID() != PARTY_NULL && !this->IsWorldFlag(WORLD_FLAG_ALLOW_PARTY) )
	{
		this->PartyRemove();
	}

	if ( this->GetPartyID() != PARTY_NULL && this->GetWorldId() == WORLD_LAST_MAN_STANDING )
	{
		this->PartyRemove();
	}

	if ( this->GetPersonalStore()->IsOpen() && !this->IsWorldFlag(WORLD_FLAG_ALLOW_EXCHANGE) )
	{
		this->PersonalStoreClose(!this->GetPersonalStore()->IsOff());
	}

	if ( this->GetHelper()->IsStarted() && !this->IsWorldFlag(WORLD_FLAG_ALLOW_HELPER) )
	{
		this->HelperStop(0x01);
	}

	if ( this->GetSummoned() && !this->IsWorldFlag(WORLD_FLAG_ALLOW_SUMMON_MONSTER) )
	{
		this->SendSummonedHP(0, 60);
		this->KillSummoned();
	}

	if ( this->IsDuelSpectatorActive() && !this->IsSpectator() )
	{
		this->SetDuelSpectatorActive(false);
		this->RemoveBuff(BUFF_DUEL_WATCHER);
		this->RemoveBuff(BUFF_TRANSPARENCY);
	}

	if ( !this->IsAdministrator() )
	{
		if ( this->GetWorldId() == WORLD_VULCANUS_ROOM )
		{
			DuelRoom const* pRoom = sDuelMgr->GetRoom(this->GetDuelRoom());

			if ( !pRoom )
			{
				this->TeleportToLocation(WORLD_VULCANUS);
				return;
			}

			if ( !this->IsSpectator()  )
			{
				if ( pRoom->GetDueler(0)->GetPlayer() != this && 
					 pRoom->GetDueler(1)->GetPlayer() != this)
				{
					this->TeleportToLocation(WORLD_VULCANUS);
					return;
				}
			}
		}
	}

	if ( this->IsDueling() && !this->IsDuelClassic() && this->GetWorldId() != WORLD_VULCANUS_ROOM )
	{
		this->SetDueling(false);
	}
	
	if ( this->IsDueling() && this->IsDuelClassic() )
	{
		sDuelMgr->ClassicUpdate(this);
	}

	if ( sWorldBoss->GetState() != WORLD_BOSS_STATE_PLAYING )
	{
		this->SetCounter(PLAYER_COUNTER_WORLD_BOSS, 0);
	}

	if ( this->GetMiniBombState() != MINI_BOMB_GAME_START && (!sMiniBomb->IsActive() || !sGameServer->IsMiniBombEnabled()) )
	{
		this->SetMiniBombState(MINI_BOMB_GAME_START);
		this->SetMiniBombCurrentScore(0);
		this->SetMiniBombTotalScore(0);
	}

	if ( this->GetTimer(PLAYER_TIMER_WRONG_LOCATION)->Elapsed(IN_MILLISECONDS) && this->GetRegenStatus() == REGEN_NONE && this->IsWorldFlag(WORLD_FLAG_ALLOW_WRONG_LOCATION_CHECK) )
	{
		WorldGrid const& grid = this->GetGrid();

		if ( grid.IsLocked_1() || grid.IsLocked_2() )
		{
			this->TeleportToGate(17);
			return;
		}
	}

	if ( this->IsAdministrator() )
		return;

	if ( this->GetWorldId() == WORLD_KANTURU_BOSS )
	{
		if ( this->GetEventId() != EVENT_KANTURU && this->GetRegenStatus() == REGEN_NONE )
		{
			this->MoveToGate(KANTURU_GATE_OUT);
			return;
		}
	}

	/*if ( this->GetWorldId() == WORLD_SWAMP_OF_CALMNESS )
	{
		if ( this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < sGameServer->GetMaxLevel(LEVEL_DATA_NORMAL) )
		{
			this->MoveToGate(17);
			return;
		}
	}*/
	
	if (this->GetWorldId() == WORLD_ICARUS || this->GetWorldId() == WORLD_KANTURU_BOSS || this->GetWorldId() == WORLD_SCARLET_ICARUS)
	{
		/*if ( this->IsActiveInventoryItem(PET_UNIRIA) )
		{
			this->UnMountBuff();
		}*/

		if ( this->GetInventory()->GetItem(WINGS)->IsItem() )
		{
			return;
		}

		if (this->HasMount())
		{
			return;
		}

		use_inventory_loop(i)
		{
			if ( !this->GetInventory()->CanUseExpandedSlot(i) )
			{
				continue;
			}

			if ( !this->GetInventory()->GetItem(i)->IsItem() )
			{
				continue;
			}

			if ( this->GetInventory()->GetItem(i)->GetItem() != PET_DINORANT )
			{
				continue;
			}

			this->InventoryItemUse(0xFE, i);
			return;
		}
		
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to wear Wings, Dinorant or Fenrir to stay in Icarus.");
		this->MoveToGate(22);
	}
	else if ( this->GetWorldId() == WORLD_RAKLION_BOSS )
	{
		if ( sRaklion->GetState() == RAKLION_STATE_NONE ||
			 sRaklion->GetState() == RAKLION_STATE_CLOSED )
		{
			this->MoveToGate(287);
		}
	}
	else if ( this->GetWorldId() == WORLD_LAND_OF_TRIALS )
	{
		if ( (!sCastleSiege->IsHuntEnabled() && !sCastleSiege->CastleOwnerMember(this) && !sCastleSiege->CastleOwnerUnionMember(this)) ||
			 sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			this->MoveToGate(106);
		}
	}
	else if ( this->GetWorldId() == WORLD_ATLANS )
	{
		/*if ( this->IsActiveInventoryItem(PET_UNIRIA) ||
			 this->IsActiveInventoryItem(PET_DINORANT) )
		{
			this->UnMountBuff();			
		}*/
	}
	else if ( this->GetWorldId() == WORLD_CASTLE_SIEGE )
	{
		if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			if ( this->GetHelper()->IsStarted() )
				this->HelperStop(0x01);

			if ( !this->GetCastleSiegeJoinSide() && this->IsInDragonTower() )
				this->MoveToGate(103);
		}
	}
}

bool Player::SummonPersonalMerchant()
{
	Monster * mMonster = sObjectMgr->MonsterTryAdd(1600, this->GetWorldId());

	if ( !mMonster )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't summon Personal Merchant.");
		return false;
	}

	mMonster->SetWorldId(this->GetWorldId());
	mMonster->SetBasicLocation(this->GetX(), this->GetY(), this->GetX(), this->GetY());
	mMonster->SetInstance(this->GetInstance());
	mMonster->SetDirection(this->GetDirection());
	mMonster->SetNpcFunction(sGameServer->GetPersonalMerchantShop().c_str());
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetSummoner(this);
	mMonster->SetSummonTick(MyGetTickCount());
	mMonster->AddToWorld();

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Personal Merchant created.");
	return true;
}
	
bool Player::SummonPersonalWarehouse()
{
	Monster * mMonster = sObjectMgr->MonsterTryAdd(1602, this->GetWorldId());

	if ( !mMonster )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Couldn't summon Personal Warehouse.");
		return false;
	}

	mMonster->SetWorldId(this->GetWorldId());
	mMonster->SetBasicLocation(this->GetX(), this->GetY(), this->GetX(), this->GetY());
	mMonster->SetInstance(this->GetInstance());
	mMonster->SetDirection(this->GetDirection());
	mMonster->SetNpcFunction("npc_talk_warehouse");
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetSummoner(this);
	mMonster->SetSummonTick(MyGetTickCount());
	mMonster->AddToWorld();

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Personal Warehouse created.");
	return true;
}

void Player::FinalClose()
{
	sServerLink->CharacterOnOff(this, 0);

	this->CancelAllActivities();

	this->SaveCharacter();
}

bool Player::WearItem(Item const* mItem, uint8 slot)
{
	if ( mItem->GetSlot() == uint8(-1) )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(mItem->GetItem());

	if (!item_info)
	{
		return false;
	}

	for ( uint8 i = 0; i < MAX_STAT_TYPE; ++i )
	{
		if (sGameServer->IsOnlyBaseStatForEquipment())
		{
			if (item_info->IsMasteryWeaponDK() && i == ENERGY && this->GetOnlyEquipmentStat(STRENGTH) >= mItem->GetRequiredStat(STRENGTH))
			{
				continue;
			}

			if (item_info->IsMasteryWeaponDK() && i == STRENGTH && this->GetOnlyEquipmentStat(ENERGY) >= mItem->GetRequiredStat(ENERGY))
			{
				continue;
			}

			if (this->GetOnlyEquipmentStat(i) < mItem->GetRequiredStat(i))
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required %s: %d", StatName[i], mItem->GetRequiredStat(i));
				return false;
			}
		}
		else
		{
			if (item_info->IsMasteryWeaponDK() && i == ENERGY && this->GetTotalStat(STRENGTH) >= mItem->GetRequiredStat(STRENGTH))
			{
				continue;
			}

			if (item_info->IsMasteryWeaponDK() && i == STRENGTH && this->GetTotalStat(ENERGY) >= mItem->GetRequiredStat(ENERGY))
			{
				continue;
			}

			if (this->GetTotalStat(i) < mItem->GetRequiredStat(i))
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required %s: %d", StatName[i], mItem->GetRequiredStat(i));
				return false;
			}
		}
	}
	
	if ( this->GetTotalLevel() < mItem->GetRequiredLevel() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required Level: %d", mItem->GetRequiredLevel());
		return false;
	}

	if ( !mItem->IsRequiredClass(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong character class.");
		return false;
	}

	if( mItem->GetSlot() != slot && slot != WEAPON_01 && slot != WEAPON_02 && slot != RING_01 && slot != RING_02 )
	{
		return false;
	}

	if( mItem->GetSlot() != slot && ((slot == WEAPON_01 || slot == WEAPON_02) && mItem->GetSlot() != WEAPON_01 && mItem->GetSlot() != WEAPON_02) )
	{
		return false;
	}

	if( mItem->GetSlot() != slot && ((slot == RING_01 || slot == RING_02) && mItem->GetSlot() != RING_01 && mItem->GetSlot() != RING_02) )
	{
		return false;
	}

	if (this->GetClass() == Character::GROW_LANCER && item_info->GetKind2() == ItemKind::LANCE && slot == WEAPON_02 && item_info->IsExclusiveClass(this->GetClass()))
	{
		return false;
	}

	auto weapon_kind_01 = this->GetInventory()->GetItem(WEAPON_01)->GetKind2();
	auto weapon_kind_02 = this->GetInventory()->GetItem(WEAPON_02)->GetKind2();

	if( slot == WEAPON_01 && this->GetInventory()->GetItem(WEAPON_02)->IsItem() )
	{
		if ( this->GetInventory()->GetItem(WEAPON_02)->IsTwoHand() )
		{
			return false;
		}

		if ( mItem->IsTwoHand() )
		{
			if (item_info->GetKind2() != ItemKind::BOW && item_info->GetKind2() != ItemKind::CROSSBOW)
			{
				return false;
			}

			if (weapon_kind_02 != ItemKind::ARROW && weapon_kind_02 != ItemKind::BOLT &&  weapon_kind_02 != ItemKind::QUIVER)
			{
				return false;
			}
		}
	}

	if( slot == WEAPON_02 && this->GetInventory()->GetItem(WEAPON_01)->IsItem() )
	{
		if ( mItem->IsTwoHand() )
		{
			return false;
		}

		if ( this->GetInventory()->GetItem(WEAPON_01)->IsTwoHand() && weapon_kind_01 != ItemKind::BOW && weapon_kind_01 != ItemKind::CROSSBOW)
		{
			return false;
		}

		if (weapon_kind_01 == ItemKind::BOW || weapon_kind_01 == ItemKind::CROSSBOW)
		{
			if (item_info->GetKind2() != ItemKind::ARROW && item_info->GetKind2() != ItemKind::BOLT &&  item_info->GetKind2() != ItemKind::QUIVER)
			{
				return false;
			}
		}
	}

	if ( sGameServer->GetMaxSocketWear() > 0 )
	{
		if ( slot == WEAPON_02 && mItem->GetMaxSocket() > 0 && this->GetInventory()->GetItem(WEAPON_01)->IsItem() && this->GetInventory()->GetItem(WEAPON_01)->GetMaxSocket() > 0 )
		{
			if ( (mItem->GetSocketCount() + this->GetInventory()->GetItem(WEAPON_01)->GetSocketCount()) > sGameServer->GetMaxSocketWear() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You have exceeded the maximum socket slot that can be worn on both hands. (Up to 6 socket slots)");
				return false;
			}
		}

		if ( slot == WEAPON_01 && mItem->GetMaxSocket() > 0 && this->GetInventory()->GetItem(WEAPON_02)->IsItem() && this->GetInventory()->GetItem(WEAPON_02)->GetMaxSocket() > 0 )
		{
			if ( (mItem->GetSocketCount() + this->GetInventory()->GetItem(WEAPON_02)->GetSocketCount()) > sGameServer->GetMaxSocketWear() )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You have exceeded the maximum socket slot that can be worn on both hands. (Up to 6 socket slots)");
				return false;
			}
		}
	}

	if( CC_MAP_RANGE(this->GetWorldId()) || IT_MAP_RANGE(this->GetWorldId()) )
	{
		if ( mItem->IsTransformationRing() )
		{
			return false;
		}
	}

	if ( slot == RING_01 || slot == RING_02 ) // Verificar que no se puedan usar 2 rings de transformación
	{
		if ( mItem->GetSlot() != RING_01 && mItem->GetSlot() != RING_02 )
		{
			return false;
		}

		if ( (slot == RING_01 && mItem->IsTransformationRing() && this->GetInventory()->GetItem(RING_02)->IsTransformationRing()) ||
			 (slot == RING_02 && mItem->IsTransformationRing() && this->GetInventory()->GetItem(RING_01)->IsTransformationRing()) )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Cannot be equipped while another Transformation Ring is equipped.");
			return false;
		}
	}

	this->PrintItemData(mItem);

	return true;
}

void Player::WearedItemApplySkill()
{
	SKILL_LOOP(i)
	{
		Skill* pSkill = this->GetSkill(i);

		if (!pSkill)
		{
			continue;
		}

		if (!pSkill->IsWeapon())
		{
			continue;
		}

		uint16 skill = pSkill->GetSkill();
		uint8 level = pSkill->GetLevel();

		int32 result = this->MagicRemove(pSkill->GetSkill());
		if (result != -1)
		{
			this->SendSkill(result, skill, level, false);
		}
	}

	wear_inventory_loop(i)
	{
		if ( !this->GetInventory()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( this->GetInventory()->GetItem(i)->GetSkill() )
		{
			this->MagicAddWeapon(this->GetInventory()->GetItem(i)->GetSkillID(), 0);
		}

		if ( this->GetInventory()->GetItem(i)->IsBlessedArchangelWeapon() )
		{
			this->MagicAddWeapon(SKILL_ARCHANGELS_WILL, 0);
		}
	}
}

void Player::WearedItemRemove(uint8 slot)
{
	if( !INVENTORY_WEAR_RANGE(slot) )
	{
		return;
	}

	if( slot == RING_01 || slot == RING_02 )
	{
		sItemMgr->IsUsingTransformationItem(this);
	}

	this->CalculateCharacter();
	this->PreviewSend();
	this->SendEquipment(slot);
}

void Player::PreviewMake()
{
	Util::memset<uint8, uint8>(this->preview, 0, MAX_PREVIEW_DATA);

	// Set Class
	this->preview[CS_CLASS].set(this->GetDBClass() & ~7);

	///- 2nd Class
	if ( this->GetChangeUP(0) != 0 )
	{
		this->preview[CS_CLASS].op_or(8);
	}
	///- 3th Class
	if ( this->GetChangeUP(1) != 0 )
	{
		this->preview[CS_CLASS].op_or(4);
	}
	///- 4th Class
	if ( this->GetChangeUP(2) != 0 )
	{
		//this->preview[CS_CLASS].op_and(~4);
		this->preview[CS_CLASS].op_or(2);
	}

	// Set Right Hand Item
	this->preview[CS_WEAPON1_DATA].op_or(DBI_GET_TYPE(this->GetInventory()->item[0].GetItem()));
	this->preview[CS_WEAPON1_TYPE].set(DBI_GET_INDEX(this->GetInventory()->item[0].GetItem()));
			
	// Set Left Hand Item
	this->preview[CS_WEAPON2_DATA].op_or(DBI_GET_TYPE(this->GetInventory()->item[1].GetItem()));
	this->preview[CS_WEAPON2_TYPE].set(DBI_GET_INDEX(this->GetInventory()->item[1].GetItem()));

	// Set Helmet
	this->preview[CS_HELMET1].op_or(CS_SET_HELMET1(this->GetInventory()->item[2].GetItem()));
	this->preview[CS_HELMET2].op_or(CS_SET_HELMET2(this->GetInventory()->item[2].GetItem()));
	this->preview[CS_HELMET3].op_or(CS_SET_HELMET3(this->GetInventory()->item[2].GetItem()));

	// Set Armor
	this->preview[CS_ARMOR1].op_or(CS_SET_ARMOR1(this->GetInventory()->item[3].GetItem()));
	this->preview[CS_ARMOR2].op_or(CS_SET_ARMOR2(this->GetInventory()->item[3].GetItem()));
	this->preview[CS_ARMOR3].op_or(CS_SET_ARMOR3(this->GetInventory()->item[3].GetItem()));

	// Set Pants
	this->preview[CS_PANTS1].op_or(CS_SET_PANTS1(this->GetInventory()->item[4].GetItem()));
	this->preview[CS_PANTS2].op_or(CS_SET_PANTS2(this->GetInventory()->item[4].GetItem()));
	this->preview[CS_PANTS3].op_or(CS_SET_PANTS3(this->GetInventory()->item[4].GetItem()));

	// Set Gloves
	this->preview[CS_GLOVES1].op_or(CS_SET_GLOVES1(this->GetInventory()->item[5].GetItem()));
	this->preview[CS_GLOVES2].op_or(CS_SET_GLOVES2(this->GetInventory()->item[5].GetItem()));
	this->preview[CS_GLOVES3].op_or(CS_SET_GLOVES3(this->GetInventory()->item[5].GetItem()));

	// Set Boots
	this->preview[CS_BOOTS1].op_or(CS_SET_BOOTS1(this->GetInventory()->item[6].GetItem()));
	this->preview[CS_BOOTS2].op_or(CS_SET_BOOTS2(this->GetInventory()->item[6].GetItem()));
	this->preview[CS_BOOTS3].op_or(CS_SET_BOOTS3(this->GetInventory()->item[6].GetItem()));

	int32 index = 0;
	Item const* pWings = this->GetInventory()->GetItem(WINGS);

	// Set Wings
	switch (pWings->GetItem())
	{
	case ITEMGET(12, 0):
	case ITEMGET(12, 1):
	case ITEMGET(12, 2):
		{
			this->preview[16].op_or(0x04 * ((pWings->GetItem() - ITEMGET(12, 0)) + 1));
		} break;

	case ITEMGET(12, 3): // ELF Lvl 2
	case ITEMGET(12, 424): // ELF Lvl 2
		{
			this->preview[16].op_or(16);
		} break;

	case ITEMGET(12, 4): // DW Lvl 2
	case ITEMGET(12, 422): // DW Lvl 2
		{
			this->preview[16].op_or(20);
		} break;

	case ITEMGET(12, 5): // DK Lvl 2
	case ITEMGET(12, 423): // DK Lvl 2
		{
			this->preview[16].op_or(24);
		} break;

	case ITEMGET(12, 6): // MG Lvl 2
	case ITEMGET(12, 425): // MG Lvl 2
		{
			this->preview[16].op_or(28);
		} break;

	case ITEMGET(12, 36): // DK Lvl 3
	case ITEMGET(12, 431): // DK Lvl 3
		{
			this->preview[16].op_or(0);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 37): // DW Lvl 3
	case ITEMGET(12, 430): // DW Lvl 3
		{
			this->preview[16].op_or(4);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 38): // ELF Lvl 3
	case ITEMGET(12, 432): // ELF Lvl 3
		{
			this->preview[16].op_or(8);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 39): // MG Lvl 3
	case ITEMGET(12, 433): // MG Lvl 3
		{
			this->preview[16].op_or(12);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 40): // DL Lvl 3
	case ITEMGET(12, 434): // DL Lvl 3
		{
			this->preview[16].op_or(16);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 41): // SUM Lvl 1
		{
			this->preview[16].op_or(20);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 42): // SUM Lvl 2
	case ITEMGET(12, 427): // SUM Lvl 2
		{
			this->preview[16].op_or(24);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 43): // SUM Lvl 3
	case ITEMGET(12, 435): // SUM Lvl 3
		{
			this->preview[16].op_or(28);
			this->preview[9].op_or(1);
		} break;

	case ITEMGET(12, 49): // RF Lvl 2
	case ITEMGET(12, 428): // RF Lvl 2
		{
			this->preview[16].op_or(0);
			this->preview[9].op_or(2);
		} break;

	case ITEMGET(12, 50): // RF Lvl 3
	case ITEMGET(12, 436): // RF Lvl 3
		{
			this->preview[16].op_or(4);
			this->preview[9].op_or(2);
		} break;

	case ITEMGET(12, 130): // Small Cape of Lord
	case ITEMGET(12, 131): // Small Wings of Misery
	case ITEMGET(12, 132): // Small Wings of Elf
	case ITEMGET(12, 133): // Small Wings of Heaven
	case ITEMGET(12, 134): // Small Wings of Satan
	case ITEMGET(12, 135): // Small Cloak of Warrior
		{
			this->preview[16].op_or(0x04 * ((pWings->GetItem() - ITEMGET(12, 130) + 2)));
			this->preview[9].op_or(2);
		} break;

	case ITEMGET(12, 262):
	case ITEMGET(12, 263):
	case ITEMGET(12, 264):
	case ITEMGET(12, 265):
	case ITEMGET(12, 267):
		{
			this->preview[16].op_or(0x04 * (pWings->GetItem() - ITEMGET(12, 262)));
			this->preview[9].op_or(3);
		} break;

	case ITEMGET(13, 30): // DL Lvl 2
	case ITEMGET(12, 426): // DL Lvl 2
		{
			this->preview[16].op_or(24);
			this->preview[9].op_or(3);
		} break;

	case ITEMGET(12, 269): // GL Lvl 2
	case ITEMGET(12, 429): // GL Lvl 2
		{
			this->preview[16].op_or(28);
			this->preview[9].op_or(3);
		} break;

	case ITEMGET(12, 270): // GL Lvl 3
	case ITEMGET(12, 437): // GL Lvl 3
		{
			if (pWings->GetLevel() == 15)
			{
				this->preview[16].op_or(8); // 34
				this->preview[9].op_or(4);
			}
			else
			{
				this->preview[16].op_or(0); // 32
				this->preview[9].op_or(4);
			}
		} break;

	case ITEMGET(12, 278):
		{
			this->preview[16].op_or(4);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 268):
	case ITEMGET(12, 266):
		{
			this->preview[16].op_or(12);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 279):
	case ITEMGET(12, 284):
		{
			this->preview[16].op_or(16);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 280):
	case ITEMGET(12, 285):
		{
			this->preview[16].op_or(20);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 281):
	case ITEMGET(12, 286):
		{
			this->preview[16].op_or(24);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 282):
	case ITEMGET(12, 287):
		{
			this->preview[16].op_or(28);
			this->preview[9].op_or(4);
		} break;

	case ITEMGET(12, 414):
		{
			this->preview[16].op_or(0);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 415):
		{
			this->preview[16].op_or(4);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 416):
		{
			this->preview[16].op_or(8);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 417):
		{
			this->preview[16].op_or(12);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 418):
		{
			this->preview[16].op_or(16);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 419):
		{
			this->preview[16].op_or(20);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 420):
		{
			this->preview[16].op_or(24);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 421):
		{
			this->preview[16].op_or(28);
			this->preview[9].op_or(5);
		} break;

	case ITEMGET(12, 438):
		{
			this->preview[16].op_or(0);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 439):
		{
			this->preview[16].op_or(4);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 440):
		{
			this->preview[16].op_or(8);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 441):
		{
			this->preview[16].op_or(12);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 442):
		{
			this->preview[16].op_or(16);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 443):
		{
			this->preview[16].op_or(20);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 444):
		{
			this->preview[16].op_or(24);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 445):
		{
			this->preview[16].op_or(28);
			this->preview[9].op_or(6);
		} break;

	case ITEMGET(12, 467):
	case ITEMGET(12, 468):
		{
			this->preview[16].op_or(0);
			this->preview[9].op_or(7);
		} break;

	case ITEMGET(12, 469):
	{
		this->preview[16].op_or(4);
		this->preview[9].op_or(7);
	} break;

	case ITEMGET(12, 472):
	case ITEMGET(12, 473):
	{
							 this->preview[16].op_or(16);
							 this->preview[9].op_or(7);
	} break;

	case ITEMGET(12, 474):
	{
							 this->preview[16].op_or(20);
							 this->preview[9].op_or(7);
	} break;

	case ITEMGET(12, 480):
	{
							 this->preview[16].op_or(12);
							 this->preview[9].op_or(7);
	} break;

	default:
		{
			index |= 0;
		} break;
	}

	int32 levelindex = CS_SET_SMALLLEVEL_RH(sItemMgr->LevelSmallConvert(this->GetInventory()->item[0].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_LH(sItemMgr->LevelSmallConvert(this->GetInventory()->item[1].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(sItemMgr->LevelSmallConvert(this->GetInventory()->item[2].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(sItemMgr->LevelSmallConvert(this->GetInventory()->item[3].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(sItemMgr->LevelSmallConvert(this->GetInventory()->item[4].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(sItemMgr->LevelSmallConvert(this->GetInventory()->item[5].GetLevel()));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(sItemMgr->LevelSmallConvert(this->GetInventory()->item[6].GetLevel()));

	this->preview[CS_LEVEL1].set(CS_SET_SMALLLEVEL1(levelindex));
	this->preview[CS_LEVEL2].set(CS_SET_SMALLLEVEL2(levelindex));
	this->preview[CS_LEVEL3].set(CS_SET_SMALLLEVEL3(levelindex));

	// Set Excellent Items
	this->preview[CS_EXE_EFFECT].set(0);
	if (this->GetInventory()->item[2].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x80);
	if (this->GetInventory()->item[3].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x40);
	if (this->GetInventory()->item[4].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x20);
	if (this->GetInventory()->item[5].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x10);
	if (this->GetInventory()->item[6].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x08);
	if (this->GetInventory()->item[0].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x04);
	if (this->GetInventory()->item[1].GetExe()) this->preview[CS_EXE_EFFECT].op_or(0x02);

	// Set Set Items
	this->preview[CS_ANC_EFFECT].set(0);
	if ( this->GetInventory()->item[2].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x80);
	if ( this->GetInventory()->item[3].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x40);
	if ( this->GetInventory()->item[4].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x20);
	if ( this->GetInventory()->item[5].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x10);
	if ( this->GetInventory()->item[6].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x08);
	if ( this->GetInventory()->item[0].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x04);
	if ( this->GetInventory()->item[1].IsAncient() ) this->preview[CS_ANC_EFFECT].op_or(0x02);

	if ( this->IsFullAncient() )
	{
		this->preview[CS_ANC_EFFECT].op_or(0x01);
	}

	// Pets Set
	switch ( this->GetInventory()->item[8].GetItem() )
	{
	case ITEMGET(13,0):
	case ITEMGET(13,1):
		{
			index |= CS_SET_HELPER(this->GetInventory()->item[8].GetItem());
		} break;

	case ITEMGET(13,80): // Panda Pet
		{
			this->preview[16].op_or(224);
		} break;

	case ITEMGET(13,106): // Unicorn
		{
			this->preview[16].op_or(160);
		} break;

	case ITEMGET(13,67): // Rudolf Pet
		{
			this->preview[16].op_or(128);
		} break;

	case ITEMGET(13,123): // Skeleton Pet
		{
			this->preview[16].op_or(96);
		} break;

	case ITEMGET(13,65): // Spirit of Guardian
		{	
			this->preview[16].op_or(64);
		} break;

	case ITEMGET(13,64): // Demon Pet
		{
			this->preview[16].op_or(32);
		} break;

	default:
		{
			index |= 3;
		} break;
	}

	Item const* pMount = this->GetActiveInventoryItem(this->GetVisibleMount());
	
	if (pMount)
	{
		if (pMount->GetItem() == PET_DARK_HORSE)
		{
			this->preview[12].op_or(1);
		}
		else if (pMount->GetItem() == PET_FENRIR)
		{
			if (pMount->FenrirIncLastDamage() > 0)
			{
				this->preview[16].op_or(0x01); // Black Fenrir
			}

			if (pMount->FenrirDecLastDamage() > 0)
			{
				this->preview[16].op_or(0x02); // Blue Fenrir
			}

			if (pMount->FenrirIllusion() > 0)
			{
				this->preview[17].op_or(1); // Illusion Fenrir
			}
		}
	}

	this->preview[5].op_or(index);
	this->preview[18].set(pWings->GetLevel());
}

void Player::PreviewSend()
{
	this->PreviewMake();

	PREVIEW_SEND pMsg(this->GetEntry());
	Util::memcpy(pMsg.preview, this->preview, MAX_PREVIEW_DATA);

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Player::SantaClausGift()
{
	if ( !this->IsPlaying() )
		return;

	if ( !this->GetSantaClausGiftRequest() )
	{
		this->CommandSend(16, 3, 0);
		return;
	}

	this->SetSantaClausGiftRequest(0);
	
	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, this, sGameServer->GetSantaVillageSantaClausItemBag()) == ITEM_BAG_RESULT_SUCCESS )
	{
		this->SetSantaClausGiftDate(time(nullptr));
	}
}
	
void Player::SnowmanMove()
{
	this->MoveToGate(22);
}

bool Player::SummonCheckWorld()
{
	return this->IsWorldFlag(WORLD_FLAG_ALLOW_SUMMON);
}

bool Player::IsBusy()
{
	return this->GetInterfaceState()->GetID() != InterfaceData::None || this->GetPersonalStore()->IsOpen();
}

void Player::Autosave()
{
	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( this->IsServerQuit() )
	{
		return;
	}

	if ( this->IsAutoLogin() )
	{
		return;
	}

	if ( this->GetAccountData()->GetCloseCount() > 0 )
	{
		return;
	}

	if ( !this->GetPersonalStore()->IsBusy() )
	{
		uint32 save_time = sGameServer->GetAutoSaveTime();

		if ( (this->GetHelper()->IsOffline()) || (this->GetPersonalStore()->IsOff()) )
		{
			save_time = sGameServer->GetAutoSaveOfflineTime();
		}

		if ( this->GetTimer(PLAYER_TIMER_SAVE_DB)->Elapsed(save_time) )
		{
			this->SaveCharacter();
		}

		this->CashShopCoinUpdate();

		if ( this->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->GetTimer() == 0 || this->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->Elapsed(sGameServer->GetCashShopGiftUpdateTime()) )
		{
			this->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->Start();
			this->CashShopGiftUpdate();
		}
	}

	if ( this->GetTimer(PLAYER_TIMER_MAIL)->GetTimer() == 0 || this->GetTimer(PLAYER_TIMER_MAIL)->Elapsed(sGameServer->GetFriendMailUpdateInterval()) )
	{
		this->GetTimer(PLAYER_TIMER_MAIL)->Start();
		this->MailNewUpdate();
	}
}

void Player::PositionSet(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( this->IsTeleporting() )
	{
		return;
	}

	if ( this->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	POINTER_PCT_LOG(POSITION_SET, lpMsg, Packet, 0);

	if (this->GetVehiculeType() == 0)
	{
		int32 distance = Util::Distance(this->GetX(), this->GetY(), lpMsg->x, lpMsg->y);

		if (distance > 15)
		{
			sLog->outError("antihack", "%s -- Wrong Distance %d [%u/%u -- %u/%u] %s", __FUNCTION__, distance, this->GetX(), this->GetY(), lpMsg->x, lpMsg->y, this->BuildLog().c_str());
			return;
		}

		if (sGameServer->IsBloodCastleSkillRushCheck())
		{
			if (BC_MAP_RANGE(this->GetWorldId()))
			{
				if (this->GetLastDurationSkill() == SKILL_RUSH || this->GetLastDurationSkill() == SKILL_RUSH_KNIGHT)
				{
					lpMsg->x = this->GetX();
					lpMsg->y = this->GetY();
				}
			}
		}
	}

	this->SetX(lpMsg->x);
	this->SetY(lpMsg->y);
	this->SetWalkSpeedCount(0);
	this->GetACPathData()->Reset();

	this->PositionSend(this->GetX(), this->GetY());
}

int32 Player::MagicAddWeapon(uint16 skill, uint8 level)
{
	if ( !sSkillMgr->IsSkillRequiredClass(skill, this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
	{
		return -1;
	}

	this->GetMasterSkillWeapon(skill, level);

	return this->MagicAdd(skill, level, true);
}

void Player::MagicDelWeapon(uint16 skill, uint8 level)
{
	this->GetMasterSkillWeapon(skill, level);

	int32 result = this->MagicRemove(skill);

	if ( result != -1 )
	{
		this->SendSkill(result, skill, level, false);
	}
}

bool Player::MagicFindSkillTree(uint16 skill) const
{
	skill_template const* skill_info = sSkillMgr->GetSkill(skill);

	if ( !skill_info )
	{
		return false;
	}

	skill_template const* skill_info_tmp = nullptr;

	SKILL_LOOP(i)
	{
		if ( !(skill_info_tmp = sSkillMgr->GetSkill(this->GetSkill(i)->GetSkill())) )
		{
			continue;
		}

		if ( skill_info_tmp->GetBaseSkill() != skill_info->GetBaseSkill() )
		{
			continue;
		}

		return true;
	}

	return false;
}

float Player::MagicGetValue(uint16 skill, bool majestic)
{
	if (majestic)
	{
		MajesticSkill const* pSkill = this->GetMajesticSkillTreeSkill(skill);

		if (!pSkill)
		{
			return 0.0f;
		}

		skill_tree_majestic * skill_info = sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSection(), pSkill->GetID());

		if (skill_info)
		{
			return skill_info->GetValue(pSkill->GetLevel());
		}
	}
	else
	{
		Skill const* pSkill = this->GetSkillTreeSkill(skill);

		if (!pSkill)
		{
			return 0.0f;
		}

		if (pSkill->GetLevel() < 1 || pSkill->GetLevel() > 20)
		{
			return 0.0f;
		}

		SkillTree* pSkillTree = sSkillMgr->GetSkillTree(skill, this->GetClass());

		if (pSkillTree)
		{
			return pSkillTree->GetValue(pSkill->GetLevel());
		}
	}

	return 0.0f;
}

Skill const* Player::MagicGetByBase(uint16 skill) const
{
	skill_template const* skill_info = sSkillMgr->GetSkill(skill);

	if ( !skill_info )
	{
		return nullptr;
	}

	skill_template const* skill_info_tmp = nullptr;

	SKILL_LOOP(i)
	{
		if ( !(skill_info_tmp = sSkillMgr->GetSkill(this->GetSkill(i)->GetSkill())) )
		{
			continue;
		}

		if ( skill_info_tmp->GetBaseSkill() != skill_info->GetBaseSkill() )
		{
			continue;
		}

		return this->GetSkill(i);
	}

	return nullptr;
}
	
Skill * Player::MagicGetByBase(uint16 skill)
{
	skill_template const* skill_info = sSkillMgr->GetSkill(skill);

	if ( !skill_info )
	{
		return nullptr;
	}

	skill_template const* skill_info_tmp = nullptr;

	SKILL_LOOP(i)
	{
		if ( !(skill_info_tmp = sSkillMgr->GetSkill(this->GetSkill(i)->GetSkill())) )
		{
			continue;
		}

		if ( skill_info_tmp->GetBaseSkill() != skill_info->GetBaseSkill() )
		{
			continue;
		}

		return this->GetSkill(i);
	}

	return nullptr;
}

int32 Player::GetHorseDamage() const
{
	Item const* Mount = this->GetActiveInventoryItem(PET_DARK_HORSE);

	if (!Mount || !Mount->IsValid())
	{
		return 0;
	}

	//Horse bonus dmg = 100+horseLvl*10+lvl*2.5+str/10+cmd/5
	return (this->GetTotalStat(STRENGTH) / 10) + (this->GetTotalStat(LEADERSHIP) / 5) + (Mount->pet_dark.level * 10);
}

void Player::MoneySet(uint32 ammount, bool send)
{
	this->zen.set(ammount);

	if ( send )
		this->MoneySend();
}

bool Player::MoneyReachMaximum(uint32 ammount) const
{
	return (this->MoneyGet() + ammount) > sGameServer->GetMaxCharacterZen();
}

void Player::MoneyAdd(uint32 ammount, bool send)
{
	if ( ammount )
	{
		if ( this->MoneyReachMaximum(ammount) )
		{
			this->MoneySet(sGameServer->GetMaxCharacterZen());
		}
		else
		{
			this->MoneySet(this->MoneyGet() + ammount);
		}
	}

	if ( send )
		this->MoneySend();
}

void Player::MoneyReduce(uint32 ammount, bool send)
{
	if ( ammount )
	{
		if ( this->MoneyGet() < ammount )
		{
			this->MoneySet(0);
		}
		else
		{
			this->MoneySet(this->MoneyGet() - ammount);
		}
	}

	if ( send )
		this->MoneySend();
}

bool Player::MoneyHave(uint32 ammount) const
{
	if ( !ammount )
		return true;

	return this->MoneyGet() >= ammount;
}

void Player::TownPortalScrollUse(uint8 slot)
{
	if ( this->GetRegenStatus() != REGEN_NONE || !this->IsLive() )
	{
		return;
	}

	if ( this->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE )
	{
		return;
	}

	auto pWorld = this->GetWorld();
	if (!pWorld)
	{
		return;
	}

	this->ClearItem(slot);

	if (IF_MAP_RANGE(this->GetWorldId()))
	{
		ImperialFortressPlayer* pPlayer = sImperialFortressMgr->FindPlayer(this);
		if (pPlayer && sGameServer->IsImperialFortressMoveToPrevious())
		{
			this->TeleportToLocation(pPlayer->GetWorld(), pPlayer->GetX(), pPlayer->GetY(), -1, -1);
			return;
		}
	}
	else if (this->GetWorldId() == WORLD_AIDA)
	{
		this->MoveToGate(27);
		return;
	}
	else if (this->GetWorldId() == WORLD_CRYWOLF_FIRST_ZONE)
	{
		this->MoveToGate(17);
		return;
	}
	else if (this->GetWorldId() == 30)
	{
		if (sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START)
		{
			if (this->isCastleSiegeDefender())
			{
				this->MoveToGate(101);
				return;
			}
			else
			{
				this->MoveToGate(100);
				return;
			}
		}
		else
		{
			if (sCastleSiege->CastleOwnerMember(this) || sCastleSiege->CastleOwnerUnionMember(this))
			{
				this->MoveToGate(101);
				return;
			}
			else
			{
				this->MoveToGate(100);
				return;
			}
		}
	}

	this->MoveToGate(pWorld->GetCurrentGate());
}

void Player::OnDie(Unit* pUnit)
{
	if ( this->GetHelper()->IsOffline() )
	{
		this->GetHelper()->SetDieTime(GetTickCount());
	}

	if ( this->GetCounter(PLAYER_COUNTER_WORLD_BOSS) > 0 )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "World Boss kill points reseted.");
		this->SetCounter(PLAYER_COUNTER_WORLD_BOSS, 0);
	}
	
	if ( BC_MAP_RANGE(this->GetWorldId()) || DS_MAP_RANGE(this->GetWorldId()) )
	{
		return;
	}

	if ( this->GetWorldId() == WORLD_LABYRINTH_OF_DIMENSIONS_BATTLE ||
		 this->GetWorldId() == WORLD_PLACE_OF_QUALIFICATION )
	{
		return;
	}

	if ( this->HasBuff(BUFF_TALISMAN_OF_GUARDIAN) )
	{
		return;
	}
	
	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( this->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( this->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	LevelData* pLevelData = this->GetLevelData(level_type);

	int32 decrease_experience_rate = 0;
	int32 decrease_money_rate = 0;
	int32 decrease_experience_ammount = 0;
	int32 decrease_money_ammount = 0;
	
	if ( !pUnit->IsPlayer() )
	{
		if ( this->GetTotalLevel() < 11 )
		{
			decrease_experience_rate = 0;
		}
		else if ( this->GetTotalLevel() <= 150 )
		{
			if ( this->GetPKLevel() == PK_STATUS_COMMONER )
				decrease_experience_rate = 3;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_1 )
				decrease_experience_rate = 5;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_2 )
				decrease_experience_rate = 10;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_3 )
				decrease_experience_rate = 20;
		}
		else if ( this->GetTotalLevel() <= 220 )
		{
			if ( this->GetPKLevel() == PK_STATUS_COMMONER )
				decrease_experience_rate = 2;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_1 )
				decrease_experience_rate = 5;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_2 )
				decrease_experience_rate = 10;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_3 )
				decrease_experience_rate = 20;
		}
		else
		{
			if ( this->GetPKLevel() == PK_STATUS_COMMONER )
				decrease_experience_rate = 1;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_1 )
				decrease_experience_rate = 5;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_2 )
				decrease_experience_rate = 10;
			else if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_3 )
				decrease_experience_rate = 20;
		}
	
		if ( this->GetPKLevel() == PK_STATUS_COMMONER ||
			 this->GetPKLevel() == PK_STATUS_MURDER_LVL_1 ||
			 this->GetPKLevel() == PK_STATUS_MURDER_LVL_2 || 
			 this->GetPKLevel() == PK_STATUS_MURDER_LVL_3 )
		{
			decrease_money_rate = 4;
		}
	}

	if ( sGameServer->IsCharacterDieExperienceDecrease() )
	{
		if ( pLevelData->GetLevel() < sGameServer->GetMaxLevel(level_type) )
		{
			//int64 experience_min = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel());
			int64 experience_min = 0;
			int64 experience_max = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel() + 1);

			if ( decrease_experience_rate > 0 )
			{
				int64 decrease_experience = ((experience_max - experience_min) * decrease_experience_rate) / 100;
				int64 sub_experience = pLevelData->GetExperience() - decrease_experience;

				if ( sub_experience < experience_min )
				{
					sub_experience = experience_min;
				}

				pLevelData->SetExperience(sub_experience);
			}

			if ( decrease_experience_ammount > 0 )
			{
				int64 sub_experience = pLevelData->GetExperience() - decrease_experience_ammount;
				
				if ( sub_experience < experience_min )
				{
					sub_experience = experience_min;
				}

				pLevelData->SetExperience(sub_experience);
			}
		}
	}

	if ( sGameServer->IsCharacterDieZenDecrease() )
	{
		if ( decrease_money_rate > 0 )
		{
			this->MoneyReduce(this->MoneyGet() * decrease_money_rate / 100);
			this->GetWarehouse()->MoneyReduce(this->GetWarehouse()->GetZen() * decrease_money_rate / 100);
		}

		if ( decrease_money_ammount > 0 )
		{
			this->MoneyReduce(decrease_money_ammount);
			this->GetWarehouse()->MoneyReduce(decrease_money_ammount);
		}
	}
}

void Player::LorenMarketTeleport()
{
	if ( !this->IsPlaying() )
		return;

	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
		return;

	if ( this->GetPersonalStore()->IsBusy() )
		return;

	if ( this->GetWorldId() != WORLD_LOREN_MARKET )
	{
		this->MoveToGate(333);
	}
	else
	{
		this->MoveToGate(17);
	}
}

void Player::SendEventAnimation(uint8 event_id, uint8 state)
{
	EVENT_STATE pMsg(event_id, state);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::RestoreEventAnimation()
{
	this->SetEventAnimation(EVENT_STATE_RED_DRAGON, false);
	this->SendEventAnimation(EVENT_STATE_RED_DRAGON, 0);
	this->SetEventAnimation(EVENT_STATE_GOLDEN_DRAGON, false);
	this->SendEventAnimation(EVENT_STATE_GOLDEN_DRAGON, 0);
}

void Player::SendWeather(uint8 weather)
{
	WEATHER_UPDATE pMsg(weather);
	this->SEND_PCT(pMsg);
}

void Player::SendWeather()
{
	if ( World* pWorld = this->GetWorld() )
	{
		this->SendWeather(pWorld->GetWeatherConverted());
	}
}

//void Player::AdminPanelUpdate(uint8 * Packet)
//{
//	POINTER_PCT_LOG(PANEL_ADMIN_UPDATE, lpMsg, Packet, 0);
//
//	PANEL_ADMIN_UPDATE pMsg(0, 0);
//
//	if ( !this->IsAdministrator() || !this->IsAdministratorFlag(ADMIN_FLAG_PANEL) || !this->IsAuthorized() )
//	{
//		this->SEND_PCT(pMsg);
//		return;
//	}
//
//	switch ( lpMsg->type )
//	{
//	case 0:
//		{
//			pMsg.type = 1;
//			this->SEND_PCT(pMsg);
//		} break;
//
//	case 1:
//		{
//			this->SetAdminPanelFlag(lpMsg->flags);
//		} break;
//	}
//
//	if ( !this->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE) )
//	{
//		if ( !this->HasBuff(BUFF_INVISIBILITY) )
//		{
//			this->AddBuff(BUFF_INVISIBILITY, -10, BUFF_FLAG_CONSTANT, this);
//			this->ViewportDeleteSend();
//		}
//	}
//	else
//	{
//		if ( this->HasBuff(BUFF_INVISIBILITY) )
//		{
//			this->RemoveBuff(BUFF_INVISIBILITY);
//			this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
//		}
//	}
//
//	sServerLink->CharacterOnOff(this, 2);
//}

void Player::SendDataSingle(Object* pObject)
{
	if ( !pObject )
		return;

	if ( Unit* pUnit = pObject->ToUnit() )
	{

	}
}

void Player::CheckAddInfinityArrow()
{
	if ( this->GetClass() == Character::FAIRY_ELF && this->GetChangeUP(0) == 1 )
	{
		skill_template const* skill_info = sSkillMgr->GetSkill(SKILL_INFINITY_ARROW);

		if ( skill_info && this->GetTotalLevel() >= skill_info->GetRequiredLevel() )
		{
			if ( !this->MagicFindSkillTree(SKILL_INFINITY_ARROW) )
			{
				int32 result = this->MagicAdd(SKILL_INFINITY_ARROW, 0);

				if ( result != -1 )
				{
					this->SendSkill(result, this->GetSkill(result)->GetSkill(), this->GetSkill(result)->GetLevel());
				}
			}
		}
	}

	if ( this->GetClass() == Character::GROW_LANCER && this->QuestEvolutionGetState(3) == QUEST_EVOLUTION_STATE_COMPLETE )
	{
		skill_template const* skill_info = sSkillMgr->GetSkill(SKILL_CIRCLE_SHIELD);

		if ( skill_info && this->GetTotalLevel() >= skill_info->GetRequiredLevel() )
		{
			if ( !this->MagicFindSkillTree(SKILL_CIRCLE_SHIELD) )
			{
				int32 result = this->MagicAdd(SKILL_CIRCLE_SHIELD, 0);

				if ( result != -1 )
				{
					this->SendSkill(result, this->GetSkill(result)->GetSkill(), this->GetSkill(result)->GetLevel());
				}
			}
		}
	}
}

void Player::SendEventEnterCount()
{
	EVENT_ENTER_COUNT pMsgMax(0);
	pMsgMax.BloodCastleCount = sEventMgr->GetEventEnterCount(EVENT_BLOOD_CASTLE);
	pMsgMax.DevilSquareCount = sEventMgr->GetEventEnterCount(EVENT_DEVIL_SQUARE);
	pMsgMax.ChaosCastleCount = sEventMgr->GetEventEnterCount(EVENT_CHAOS_CASTLE);
	pMsgMax.DoppelgangerCount = sEventMgr->GetEventEnterCount(EVENT_DOPPELGANGER);
	pMsgMax.IllusionTempleCount = sEventMgr->GetEventEnterCount(EVENT_ILLUSION_TEMPLE);
	pMsgMax.ImperialFortressCount = sEventMgr->GetEventEnterCount(EVENT_IMPERIAL_FORTRESS);
	this->SEND_PCT(pMsgMax);

	EVENT_ENTER_COUNT pMsgCur(1);
	pMsgCur.BloodCastleCount = this->GetEventEnterCount(EVENT_BLOOD_CASTLE);
	pMsgCur.DevilSquareCount = this->GetEventEnterCount(EVENT_DEVIL_SQUARE);
	pMsgCur.ChaosCastleCount = this->GetEventEnterCount(EVENT_CHAOS_CASTLE);
	pMsgCur.DoppelgangerCount = this->GetEventEnterCount(EVENT_DOPPELGANGER);
	pMsgCur.IllusionTempleCount = this->GetEventEnterCount(EVENT_ILLUSION_TEMPLE);
	pMsgCur.ImperialFortressCount = this->GetEventEnterCount(EVENT_IMPERIAL_FORTRESS);
	this->SEND_PCT(pMsgCur);
}

bool Player::IsWearingPet(uint16 pet)
{
	Item const* pItem = this->GetInventory()->GetItem(GUARDIAN);

	if ( !pItem->IsItem() || !pItem->IsValid() )
		return false;

	if ( pItem->GetItem() != pet )
		return false;

	return true;
}

void Player::UpdateGoblinPoints(Monster* pMonster)
{
	if ( !sGameServer->IsGoblinPointEnabled() )
		return;

	if ( this->IsInSafeZone() )
		return;

	if ( sGameServer->IsGoblinPointLevelAllowed(this, pMonster) )
	{
		this->GetTimer(PLAYER_TIMER_GOBLIN_POINT)->Start();
	}
}

void Player::UpdateGoblinPoints()
{
	if ( !sGameServer->IsGoblinPointEnabled() )
		return;

	if ( this->IsInSafeZone() )
		return;

	if ( this->GetTimer(PLAYER_TIMER_GOBLIN_POINT)->GetElapsed() > (sGameServer->GetGoblinPointKillTime() * IN_MILLISECONDS) )
		return;

	if ( this->GetHelper()->IsOffline() && !sGameServer->IsOfflineAttackGoblinPointsGain() )
	{
		return;
	}

	this->IncreaseGoblinPointCount(1);

	if ( this->GetHelper()->IsOffline() )
	{
		if ( this->GetGoblinPointCount() < sGameServer->GetGoblinPointRequiredTimeOffline() )
		{
			return;
		}
	}
	else
	{
		if ( this->GetGoblinPointCount() < sGameServer->GetGoblinPointRequiredTime() )
		{
			return;
		}
	}
	
	this->SetGoblinPointCount(0);
	this->IncreaseGoblinPoints(1);
}

void Player::EquipmentAutochange(uint8 target_slot, uint16 item, uint8 level)
{
	uint8 slot = uint8(-1);

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
			continue;

		if ( !this->GetInventory()->GetItem(i)->IsItem() )
			continue;

		if ( this->GetInventory()->GetItem(i)->GetItem() != item )
			continue;

		if ( level == uint8(-1) || this->GetInventory()->GetItem(i)->GetLevel() == level )
		{
			slot = i;
			break;
		}
	}

	if ( slot != uint8(-1) )
	{
		uint8 item_info[MAX_ITEM_INFO];
		this->GetInventory()->GetItem(slot)->ConvertToBuffer(item_info);
		uint8 result = this->MoveItemToInventoryFromInventory(move_item_inventory, slot, move_item_inventory, target_slot, item_info);
					
		this->ItemMoveResult(result, target_slot, item_info);
		this->CashShopSendExpireItem(target_slot);
		this->ClearItem(slot, 0);
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "The equipment has been changed.");
	}
}

void Player::AccumulateDamage(int32 & damage)
{
	if ( this->HasBuff(BUFF_POTION_OF_BLESS) )
	{
		damage += damage * 20 / 100;
	}
	else if ( this->HasBuff(BUFF_POTION_OF_SOUL) )
	{
		damage = damage;
	}
	else
	{
		this->IncreaseAccumulatedDamage(damage);

		if ( this->GetAccumulatedDamage() > 100 )
		{
			this->DecreaseWeaponDurabilityInCastle();
			this->SetAccumulatedDamage(0);
		}

		damage = damage * 5 / 100;
	}
}

void Player::PVPDamageReduction(int32 & damage, Player* pPlayer)
{
	if ( this->IsOnDuel(pPlayer) )
	{
		damage = damage * sGameServer->GetDuelingDamage() / 100;
	}
	else if ( CC_MAP_RANGE(this->GetWorldId()) )
	{
		damage = damage * 50 / 100;
	}
	else if ( this->GetWorldId() == WORLD_CASTLE_SIEGE )
	{
		if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			if ( this->GetCastleSiegeJoinSide() == pPlayer->GetCastleSiegeJoinSide() )
			{
				damage = damage * sGameServer->GetCastleSiegeReduceDamageSameSide() / 100;
			}
			else
			{
				damage = damage * sGameServer->GetCastleSiegeReduceDamageDifSide() / 100;
			}
		}
	}
	else if ( AW_MAP_RANGE(this->GetWorldId()) )
	{
		if ( sArkaWar->GetState() == ARKA_WAR_STATE_STANDBY ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_RESULT ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_CLOSE ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_END )
		{
			if ( this->GetGuildID() == pPlayer->GetGuildID() )
			{
				damage = damage * sGameServer->GetArkaWarDamageReductionAlly() / 100;
			}
			else
			{
				damage = damage * sGameServer->GetArkaWarDamageReductionEnemy() / 100;
			}
		}
	}
	else if ( this->GetWorldId() == WORLD_LAST_MAN_STANDING )
	{
		damage = damage * sGameServer->GetLastManStandingDamageReduction() / 100;
	}
}

void Player::SendWorldAttribute(uint8 type, uint8 attribute, uint8 count, bool apply, int16 const* data)
{
	uint8 buffer[255];
	POINTER_PCT(WORLD_ATTRIBUTE_SET_HEAD, head, buffer, 0);
	POINTER_PCT(WORLD_ATTRIBUTE_SET_BODY, body, buffer, sizeof(WORLD_ATTRIBUTE_SET_HEAD));
	uint8 size = sizeof(WORLD_ATTRIBUTE_SET_HEAD) + sizeof(WORLD_ATTRIBUTE_SET_BODY) * (count * 2);
	head->h.c = 0xC1;
	head->h.headcode = HEADCODE_WORLD_ATTRIBUTE;
	head->h.size = size;
	head->type = type;
	head->count = count;
	head->attr = attribute;
	head->map_set_type = apply ? 0: 1;

	for ( int32 i = 0; i < count; ++i )
	{
		body[i * 2].x = data[i * 4 + 0];
		body[i * 2].y = data[i * 4 + 1];
		body[i * 2 + 1].x = data[i * 4 + 2];
		body[i * 2 + 1].y = data[i * 4 + 3];
	}

	sendPacket(buffer, size);
}

void Player::UpdatePowers(uint8 type, bool max)
{
	if (type == POWER_LIFE || type == POWER_SHIELD)
	{
		this->LifeSend(max);
	}
	else if (type == POWER_MANA || type == POWER_STAMINA)
	{
		this->ManaSend(max);
	}
}

void Player::LifeSend(bool max)
{
	if (max)
	{
		LIFE_SEND pMsg(0xFE, this->PowerGetTotal(POWER_LIFE), this->PowerGetTotal(POWER_SHIELD));
		this->SendPacket(&pMsg);
	}
	else
	{
		LIFE_SEND pMsg(0xFF, this->PowerGet(POWER_LIFE), this->PowerGet(POWER_SHIELD));
		this->SendPacket(&pMsg);
	}
}

void Player::ManaSend(bool max)
{
	if (max)
	{
		MANA_SEND pMsg(0xFE, this->PowerGetTotal(POWER_MANA), this->PowerGetTotal(POWER_STAMINA));
		this->SendPacket(&pMsg);
	}
	else
	{
		MANA_SEND pMsg(0xFF, this->PowerGet(POWER_MANA), this->PowerGet(POWER_STAMINA));
		this->SendPacket(&pMsg);
	}
}

ViewportData* Player::GetViewportItemByID(uint16 id)
{
	ViewportData* data = nullptr;
	Object* pObject = nullptr;

	for ( ViewportDataList::iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); ++it )
	{
		data = it->second;

		if ( !data )
			continue;

		if ( data->GetStatus() == Viewport::STATUS_NONE )
			continue;

		pObject = data->GetObjectData();

		if ( !pObject )
			continue;

		if ( pObject->GetType() != OBJECT_TYPE_WORLD_ITEM )
			continue;

		if ( pObject->GetEntry() == id )
			return data;
	}

	return nullptr;
}

bool Player::CanEnterWorld(uint16 world, int16 x1, int16 y1, int16 x2, int16 y2, bool send_message)
{
	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
		return false;

	if ( !this->IsAdministrator() )
	{
		if ( pWorld->flagHas(WORLD_FLAG_BATTLE_ZONE) && !this->GetGen()->IsFamily() )
		{
			if ( send_message )
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This zone is only available for Gen Members.");
			return false;
		}

		Item* pRing01 = this->GetInventory()->GetItem(RING_01);
		Item* pRing02 = this->GetInventory()->GetItem(RING_02);
		Item* pGuardian = this->GetInventory()->GetItem(GUARDIAN);

		switch ( pWorld->GetEntry() )
		{
		case WORLD_ICARUS:
		case WORLD_SCARLET_ICARUS:
			{
				if (!this->GetInventory()->GetItem(WINGS)->IsItem() && !this->HasMount())
				{
					if ( send_message )
						this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't warp to Icarus without wearing Wings, Dinorant or Fenrir.");
					return false;
				}

				if ( (pRing01->IsItem() && pRing01->IsTransformationRing(false)) ||
					 (pRing02->IsItem() && pRing02->IsTransformationRing(false)) )
				{
					if ( send_message )
						this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't warp to Icarus while wearing a Transformation Ring.");
					return false;
				}
			} break;

		case WORLD_RAKLION_BOSS:
			{
				if ( !sRaklion->EnterAllowed(this) )
				{
					if ( send_message )
						this->SendNotice(CUSTOM_MESSAGE_ID_RED, "The Hatchery Gate is closed, you can not enter.");
					return false;
				}
			} break;

		case WORLD_CASTLE_SIEGE:
			{
				if ( IsInDragonTower(x1, y1, x2, y2) )
				{
					if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
					{
						if ( !this->GetCastleSiegeJoinSide() )
						{
							if ( send_message )
								this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to enter here.");
							return false;
						}

						if ( sCastleSiege->GuardianStatueExist() && this->GetCastleSiegeJoinSide() != CASTLE_SIEGE_JOIN_SIDE_DEFENSE )
						{
							if ( send_message )
								this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to enter here.");
							return false;
						}
					}
					else if ( !sCastleSiege->CastleOwnerMember(this) && !sCastleSiege->CastleOwnerUnionMember(this) )
					{
						return false;
					}
				}
			} break;

		case WORLD_NIXIES_LAKE:
			{
				if (sNixiesLake->IsInSanctuary(world, x1, y1, x2, y2) && !sNixiesLake->IsPlayerAllowed(this))
				{
					if (send_message)
					{

					}
				}
			} break;

		case WORLD_SWAMP_OF_DARKNESS:
			{
				if (sSwampOfDarkness->IsBossZone(world, x1, y1, x2, y2))
				{
					if (sSwampOfDarkness->GetState() == SWAMP_OF_DARKNESS_STATE_NONE)
					{
						return false;
					}

					if (!sSwampOfDarkness->IsAllowedToEnter(this))
					{
						if (send_message)
						{
							SWAMP_OF_DARKNESS_BOSS_ENTER_RESULT pMsg;
							pMsg.result = 2;
							this->SEND_PCT(pMsg);
						}

						return false;
					}
				}
			} break;
		}
	}

	if (this->GetHelper()->IsOffline())
	{
		if (!sGameServer->IsOfflineAttackWorld(this, world))
		{
			return false;
		}

		if (!sGameServer->IsOfflineAttackZone(this, world, x1, y1))
		{
			return false;
		}

		if (!sGameServer->IsOfflineAttackZone(this, world, x2, y2))
		{
			return false;
		}
	}

	return true;
}

void Player::OpenSealedBoxAttempt(uint8 slot)
{
	Item const* pItem = this->GetInventory()->GetItem(slot);

	if ( !pItem )
	{
		//this->ItemUseResult();
		return;
	}

	uint8 type = pItem->GetItem() == ITEMGET(14, 121) ? 0: 1;
	uint8 key_slot = -1;
	uint16 key = type == 0 ? ITEMGET(14, 113) : ITEMGET(14, 112);

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		pItem = this->GetInventory()->GetItem(i);

		if ( !pItem || !pItem->IsItem() )
			continue;

		if ( pItem->GetItem() != key )
			continue;

		key_slot = i;
		break;
	}

	if ( key_slot == uint8(-1) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s key required to open this box.", type == 0 ? "Golden" : "Silver");
		//this->ItemUseResult();
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(type == 0 ? ITEMGET(14, 123) : ITEMGET(14, 124));

	if ( !item_info )
	{
		//this->ItemUseResult();
		return;
	}

	if ( !this->GetInventory()->IsEmptySpace(item_info->GetX(), item_info->GetY()) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "There are no free slots to unlock box.");
		//this->ItemUseResult();
		return;
	}

	this->ClearItem(slot);
	this->ClearItem(key_slot);

	sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(item_info->GetItem()));

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Box unlocked.");
}

void Player::UpdateRestriction()
{
	for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; ++i )
	{
		if ( !this->GetRestriction(i)->IsActive() )
			continue;

		if ( this->GetRestriction(i)->GetTime() < time(nullptr) )
		{
			this->GetRestriction(i)->Reset();
		}
	}
}

bool Player::CanInteract(Unit* pTarget, bool target)
{
	if (sGameServer->IsShutdownCheck() && sGameServer->IsShutdown())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't interact with npc or players while server shutdown is active.");
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't perform certain actions while server shutdown is active.");
		return false;
	}

	if ( target )
	{
		if ( !pTarget || !pTarget->IsPlaying() || !pTarget->IsLive() )
			return false;

		if ( this == pTarget)
			return false;

		if ( !this->SameDimension(pTarget) )
			return false;
	}

	return true;
}

void Player::ProcessAttackSafe()
{
	if ( !sGameServer->IsAttackSafeEnabled() )
		return;

	this->IncreaseCounter(PLAYER_COUNTER_ATTACK_SAFE, 1);

	if ( this->GetCounter(PLAYER_COUNTER_ATTACK_SAFE) >= sGameServer->GetAttackSafeCount() )
	{
		sLog->outInfo("antihack", "%s - %s", __FUNCTION__, this->BuildLog().c_str());

		if ( sGameServer->IsAttackSafeKick() )
		{
			KICK_PLAYER(this, "Attack Safe");

			sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Attack Safe");
		}
	}
}

void Player::UpdateAleTime()
{
	bool update = false;

	for ( int32 i = 0; i < ALE_MAX; ++i )
	{
		if ( this->GetAleEffect(i) <= 0 )
			continue;

		if ( GetTickCount() > this->GetAleTime(i) )
		{
			this->SetAleEffect(i, 0);
			update = true;
		}
	}

	if ( update )
	{
		this->CalculateCharacter();
	}
}

void Player::LogoutEventRemove()
{
	if ( BC_MAP_RANGE(this->GetWorldId()) && this->GetEventId() == EVENT_BLOOD_CASTLE )
	{
		BloodCastle * Ground = sBloodCastleMgr->GetGround(this->GetEventGround());

		if ( Ground )
		{
			Ground->RemovePlayer(this);
		}

		return;
	}

	if ( DS_MAP_RANGE(this->GetWorldId()) && this->GetEventId() == EVENT_DEVIL_SQUARE )
	{
		DevilSquare * Ground = sDevilSquareMgr->GetGround(this->GetEventGround());

		if ( Ground )
		{
			Ground->RemovePlayer(this);
		}

		return;
	}

	if ( CC_MAP_RANGE(this->GetWorldId()) )
	{
		if ( this->GetEventId() == EVENT_CHAOS_CASTLE && CHAOS_CASTLE_GROUND_RANGE(this->GetEventGround()) )
		{
			ChaosCastle * Ground = sChaosCastleMgr->GetGround(this->GetEventGround());

			if ( Ground )
			{
				Ground->RemovePlayer(this);
			}
		}
		else if ( this->GetEventId() == EVENT_CHAOS_CASTLE_SURVIVAL )
		{
			sChaosCastleSurvivalMgr->GetGround()->RemovePlayer(this);
		}

		return;
	}

	if ( DG_MAP_RANGE(this->GetWorldId()) )
	{
		sDoppelganger->RemovePlayer(this->GetGUID());
	}

	sTormentedSquareSurvival->RemovePlayer(this);

	if ( TS_MAP_RANGE(this->GetWorldId()) )
	{
		sTormentedSquare->RemovePlayer(this);
	}
}

bool Player::HasSkillRequiredStat(skill_template const* skill_info)
{
	if ( !skill_info )
	{
		return false;
	}

	if ( skill_info->GetLearnItem() != uint16(-1) )
	{
		Item item = Item(skill_info->GetLearnItem(), skill_info->GetLearnItemLevel());
		item.Convert();
	
		for ( uint8 i = 0; i < MAX_STAT_TYPE; ++i )
		{
			if ( this->GetTotalStat(i) < item.GetRequiredStat(i) )
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required %d %s.", item.GetRequiredStat(i), StatName[i]);
				return false;
			}
		}
		
		if ( this->GetTotalLevel() < item.GetRequiredLevel() )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required level %d.", item.GetRequiredLevel());
			return false;
		}

		if ( !item.IsRequiredClass(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong character class.");
			return false;
		}
	}

	if ( sGameServer->IsSkillEnergyCheck() )
	{
		int32 required_energy = skill_info->GetRequiredStat(ENERGY);

		if ( this->GetTotalStat(ENERGY) < required_energy )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required energy check %d.", required_energy);
			return false;
		}
	}

	if ( sGameServer->IsSkillLeadershipCheck() )
	{
		int32 required_leadership = skill_info->GetRequiredStat(LEADERSHIP);

		if ( this->GetTotalStat(LEADERSHIP) < required_leadership )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required leadership check %d.", required_leadership);
			return false;
		}
	}

	if ( this->GetTotalLevel() < skill_info->GetRequiredLevel() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required level check %d.", skill_info->GetRequiredLevel());
		return false;
	}

	if ( !sSkillMgr->IsSkillRequire(this, skill_info->GetID()) )
	{
		return false;
	}

	return true;
}


void Player::ChangeSkinOption(uint8 * Packet)
{
	if ( !this->IsPlaying() )
		return;

	POINTER_PCT(OPTION_CHANGE_SKIN, lpMsg, Packet, 0);

	this->GetGameOption()->SetChangeSkin(lpMsg->change_skin);

	this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
}

void Player::MuunUpdate()
{
	bool calculate_character = false;

	for( int32 n = 0; n < MUUN_INVENTORY_SIZE; ++n )
	{
		if ( !this->GetMuunInventory()->GetItem(n)->IsItem() )
		{
			continue;
		}

		if ( !this->GetMuunInventory()->GetItem(n)->GetExpireDate() )
		{
			continue;
		}

		if ( this->GetMuunInventory()->GetItem(n)->IsExpired() && !this->GetMuunInventory()->GetItem(n)->IsExpiredStatus() )
		{
			this->GetMuunInventory()->GetItem(n)->SetExpiredStatus(true);

			sMuunSystem->GCMuunItemExpireUpdate(this, n);

			if ( MUUN_INVENTORY_WEAR_RANGE(n) )
			{
				calculate_character = true;
			}
		}
	}

	for( int32 n = 0; n < MUUN_INVENTORY_WEAR_SIZE; ++n )
	{
		if ( !this->GetMuunInventory()->GetItem(n)->IsItem() ||
			 !this->GetMuunInventory()->GetItem(n)->IsMuunItem() )
		{
			continue;
		}

		bool muun_active = true;

		if ( this->GetMuunInventory()->GetItem(n)->GetDurability() <= 0.0f )
		{
			muun_active = false;
		}

		if ( !sMuunSystem->CheckSpecialOption(this, sMuunSystem->GetMuunSystemInfo(this->GetMuunInventory()->GetItem(n)->GetItem())) )
		{
			muun_active = false;
		}

		if ( !muun_active )
		{
			if( this->IsMuunItemStatus(n) )
			{
				sMuunSystem->GCMuunItemStatusSend(this, n, 0);
				this->SetMuunItemStatus(n, false);
				calculate_character = true;
			}
		}
		else
		{
			if( !this->IsMuunItemStatus(n) )
			{
				sMuunSystem->GCMuunItemStatusSend(this, n, 1);
				this->SetMuunItemStatus(n, true);
				calculate_character = true;
			}
		}
	}

	if ( calculate_character )
	{
		this->CalculateCharacter();
	}

	sMuunSystem->MuunAttack(this);
}

uint8 Player::GetFreeInventorySlots() const
{
	uint8 count = 0;

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( this->GetInventory()->item_map[i - PLAYER_MAX_EQUIPMENT] == 0 )
		{
			++count;
		}
	}

	return count;
}

bool Player::CheckItemMoveToInventory(Item const* pItem, uint8 slot)
{
	if ( !pItem->IsItem() )
	{
		return false;
	}

	if ( sItemMgr->GetItemInventoryType(pItem->GetItem()) != ITEM_INVENTORY_TYPE_NORMAL )
	{
		return false;
	}

	if( !INVENTORY_WEAR_RANGE(slot) && slot != PENTAGRAM_SLOT ) // TEST
	{
		return true;
	}

	if ( !this->WearItem(pItem, slot) )
	{
		return false;
	}

	return true;
}

bool Player::CheckItemMoveToTrade(Item const* pItem)
{
	if( !pItem->IsItem() )
	{
		return false;
	}

	if ( !sItemMgr->IsItemFlag(pItem->GetItem(), pItem->GetLevel(), ITEM_FLAG_ALLOW_EXCHANGE) )
	{
		return false;
	}

	if ( pItem->GetExpireDate() > 0 )
	{
		return false;
	}

	if ( pItem->IsDBFlag(ITEM_DB_FLAG_EXCHANGE) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item can't be traded.");
		return false;
	}

	if (pItem->common.time > time(nullptr))
	{
		this->SendMessageBox(1, "Trade", "You will be able to trade this item in %s", secsToTimeString(pItem->common.time - time(nullptr)).c_str());
		return false;
	}

	return true;
}

bool Player::CheckItemMoveToVault(Item const* pItem)
{
	if( !pItem->IsItem() )
	{
		return false;
	}

	if ( !sItemMgr->IsItemFlag(pItem->GetItem(), pItem->GetLevel(), ITEM_FLAG_ALLOW_WAREHOUSE) )
	{
		return false;
	}

	if ( sItemMgr->GetItemInventoryType(pItem->GetItem()) != ITEM_INVENTORY_TYPE_NORMAL )
	{
		return false;
	}

	if ( pItem->GetExpireDate() > 0 )
	{
		return false;
	}

	if ( pItem->IsDBFlag(ITEM_DB_FLAG_WAREHOUSE) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "This item can't be stored.");
		return false;
	}

	return true;
}

bool Player::CheckItemMoveToChaos(Item const* pItem, uint8 target)
{
	if( !pItem->IsItem() )
	{
		return false;
	}

	if ( pItem->GetExpireDate() > 0 )
	{
		return false;
	}

	if ( this->IsMixCompleted() )
	{
		return false;
	}

	if (sGameServer->IsLuckyItemMixCheck())
	{
		if (target == move_item_chaos_machine && sItemMgr->IsLuckyItem(pItem->GetItem()))
		{
			return false;
		}
	}

	if (pItem->IsDBFlag(ITEM_DB_FLAG_UPGRADE))
	{
		this->SendMessageBox(0, "Chaos Box", "This item can't be upgraded.");
		return false;
	}

	return true;
}

uint8 Player::MoveItem(Inventory, Inventory)
{
	if ( !inventory_range(source_slot) )
	{
		return uint8(-1);
	}

	if ( !inventory_range(target_slot) )
	{
		return uint8(-1);
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(source_slot) )
	{
		return uint8(-1);
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(target_slot) )
	{
		return uint8(-1);
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO INVENTORY FROM INVENTORY") )
	{
		return uint8(-1);
	}

	if ( pSourceItem->IsInventoryActive() )
	{
		return uint8(-1);
	}

	if ( !this->CheckItemMoveToInventory(pSourceItem, target_slot) )
	{
		return uint8(-1);
	}

	if( this->InventoryAddItemStack(source_slot, target_slot) )
	{
		return uint8(-1);
	}

	this->GetInventory()->StartTmp();
	this->GetInventory()->ItemSet(source_slot, 0);

	if( this->GetInventory()->AddItem(this->GetInventory()->item[source_slot], target_slot) == 0xFF )
	{
		this->GetInventory()->EndTmp();
		return uint8(-1);
	}

	MoveItemSuccess(Inventory, Inventory);

	if (sMonsterSoul->IsSoulConverter(this->GetInventory()->GetItem(target_slot)))
	{
		sMonsterSoul->SendItemMove(this, source_slot, target_slot);
	}

	this->WearedItemRemove(source_slot);
	this->WearedItemRemove(target_slot);

	return target;
}

uint8 Player::MoveItem(Trade, Trade)
{
	if( !trade_range(source_slot) )
	{
		return 0xFF;
	}

	if( !trade_range(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetTradeInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO TRADE FROM TRADE") )
	{
		return uint8(-1);
	}

	this->GetTradeInventory()->StartTmp();
	this->GetTradeInventory()->ItemSet(source_slot, 0);

	sLog->outInfo(LOG_TRADE, "[ ITEM MOVE ][ TRADE -> TRADE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetTradeInventory()->AddItem(this->GetTradeInventory()->item[source_slot], target_slot) == 0xFF )
	{
		this->GetTradeInventory()->EndTmp();
		return 0xFF;
	}

	MoveItemSuccess(TradeInventory, TradeInventory);

	this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeItemDelete(source_slot);
	
	this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeItemAdd(target_slot, item_info);

	return 1;
}

uint8 Player::MoveItem(Trade, Inventory)
{
	if ( !inventory_range(source_slot) )
	{
		return 0xFF;
	}

	if( !trade_range(target_slot) )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(source_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToTrade(pSourceItem) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO TRADE FROM INVENTORY") )
	{
		return uint8(-1);
	}

	if ( pSourceItem->IsInventoryActive() )
	{
		return uint8(-1);
	}

	sLog->outInfo(LOG_TRADE, "[ ITEM MOVE ][ INVENTORY -> TRADE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetTradeInventory()->AddItem(this->GetInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(TradeInventory, Inventory);

	this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeItemAdd(target_slot, item_info);

	this->WearedItemRemove(source_slot);

	sPentagramSystem->GCPentagramJewelTradeInfoSend(this, this->GetInterfaceState()->GetTarget()->ToPlayer(), this->GetTradeInventory()->GetItem(target_slot));

	return target;
}

uint8 Player::MoveItem(Inventory, Trade)
{
	if( !trade_range(source_slot) )
	{
		return 0xFF;
	}

	if( !inventory_range(target_slot) )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetTradeInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO INVENTORY FROM TRADE") )
	{
		return 0xFF;
	}

	if ( !this->CheckItemMoveToInventory(pSourceItem, target_slot) )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_TRADE, "[ ITEM MOVE ][ TRADE -> INVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetInventory()->AddItem(this->GetTradeInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(Inventory, TradeInventory);

	if ( Player* pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer() )
	{
		this->GetTradeInventory()->SetButtonEnabled(false);
		//this->TradeOkTime = GetTickCount();
		this->TradeButtonOKSend(0);
		pPlayer->GetTradeInventory()->SetButtonEnabled(false);
		//pPlayer->TradeOkTime = GetTickCount();
		pPlayer->TradeButtonOKSend(2);
		pPlayer->TradeItemDelete(source_slot);
	}

	this->WearedItemRemove(target_slot);

	return target;
}

uint8 Player::MoveItem(ChaosBox, ChaosBox)
{
	if ( !chaos_box_range(source_slot) )
	{
		return 0xFF;
	}

	if ( !chaos_box_range(target_slot) )
	{
		return 0xFF;
	}

	if ( this->IsMixCompleted() )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetMixInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO CHAOSBOX FROM CHAOSBOX") )
	{
		return 0xFF;
	}

	this->GetMixInventory()->StartTmp();
	this->GetMixInventory()->ItemSet(source_slot, 0);

	sLog->outInfo(LOG_CHAOS_MIX, "[ ITEM MOVE ][ CHAOSBOX -> CHAOSBOX ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetMixInventory()->AddItem(this->GetMixInventory()->item[source_slot], target_slot) == 0xFF )
	{
		this->GetMixInventory()->EndTmp();
		return 0xFF;
	}

	MoveItemSuccess(MixInventory, MixInventory);

	return target;
}

uint8 Player::MoveItem(ChaosBox, Inventory)
{
	if( !inventory_range(source_slot) )
	{
		return 0xFF;
	}

	if ( !chaos_box_range(target_slot) )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(source_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToChaos(pSourceItem, target) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO CHAOSBOX FROM INVENTORY") )
	{
		return 0xFF;
	}

	if ( pSourceItem->IsInventoryActive() )
	{
		return uint8(-1);
	}

	if ( this->GetInterfaceState()->GetID() == InterfaceData::Lucky )
	{
		chaos_box_loop(i)
		{
			if ( this->GetMixInventory()->GetItem(i)->IsItem() )
			{
				return uint8(-1);
			}
		}
	}

	sLog->outInfo(LOG_CHAOS_MIX, "[ ITEM MOVE ][ INVENTORY -> CHAOSBOX ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetMixInventory()->AddItem(this->GetInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(MixInventory, Inventory);

	this->WearedItemRemove(source_slot);

	return target;
}

uint8 Player::MoveItem(Inventory, ChaosBox)
{
	if ( !chaos_box_range(source_slot) )
	{
		return 0xFF;
	}

	if( !inventory_range(target_slot) )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetMixInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO INVENTORY FROM CHAOSBOX") )
	{
		return 0xFF;
	}

	if( !this->CheckItemMoveToInventory(pSourceItem, target_slot) )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_CHAOS_MIX, "[ ITEM MOVE ][ CHAOSBOX -> INVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetInventory()->AddItem(this->GetMixInventory()->item[source_slot], target_slot) == 0xFF)
	{
		return 0xFF;
	}

	MoveItemSuccess(Inventory, MixInventory);

	this->WearedItemRemove(target_slot);

	return target;
}

uint8 Player::MoveItem(Warehouse, Warehouse)
{
	if( !warehouse_range(source_slot) )
	{
		return 0xFF;
	}

	if( !warehouse_range(target_slot) )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(source_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(target_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetWarehouse()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO WAREHOUSE FROM WAREHOUSE") )
	{
		return 0xFF;
	}

	this->GetWarehouse()->StartTmp();
	this->GetWarehouse()->ItemSet(source_slot, 0);

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ WAREHOUSE -> WAREHOUSE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetWarehouse()->AddItem(this->GetWarehouse()->item[source_slot], target_slot) == 0xFF )
	{
		this->GetWarehouse()->EndTmp();
		return 0xFF;
	}

	MoveItemSuccess(Warehouse, Warehouse);

	return target;
}

uint8 Player::MoveItem(Warehouse, Inventory)
{
	if( !inventory_range(source_slot) )
	{
		return 0xFF;
	}

	if( !warehouse_range(target_slot) )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(source_slot) )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(target_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToVault(pSourceItem) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO WAREHOUSE FROM INVENTORY") )
	{
		return 0xFF;
	}

	if ( pSourceItem->IsInventoryActive() )
	{
		return uint8(-1);
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ INVENTORY -> WAREHOUSE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);
	
	if( this->GetWarehouse()->AddItem(this->GetInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(Warehouse, Inventory);

	this->WearedItemRemove(source_slot);

	sPentagramSystem->MovePentagramToWarehouseFromInventory(this, this->GetWarehouse()->GetItem(target_slot));

	return target;
}

uint8 Player::MoveItem(Inventory, Warehouse)
{
	if ( !warehouse_range(source_slot) )
	{
		return 0xFF;
	}

	if ( !inventory_range(target_slot) )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(source_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	if ( !this->GetInventory()->CanUseExpandedSlot(target_slot) )
	{
		return 0xFF;
	}

	uint32 take_price = this->WarehouseGetUsedHowMuch();

	if ( !this->MoneyHave(take_price) && !this->GetWarehouse()->MoneyHave(take_price) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need %u Zen to use your vault.", take_price);
		return 0xFF;
	}

	Item * pSourceItem = this->GetWarehouse()->GetItem(source_slot);
	
	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO INVENTORY FROM WAREHOUSE") )
	{
		return 0xFF;
	}

	if( !this->CheckItemMoveToInventory(pSourceItem, target_slot) )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ WAREHOUSE -> INVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetInventory()->AddItem(this->GetWarehouse()->item[source_slot], target_slot) == 0xFF)
	{
		return 0xFF;
	}

	MoveItemSuccess(Inventory, Warehouse);

	if ( this->MoneyHave(take_price) )
	{
		this->MoneyReduce(take_price, true);
	}
	else
	{
		this->GetWarehouse()->MoneyReduce(take_price);
		this->WarehouseMoneyInOutResult(1, this->GetWarehouse()->GetZen(), this->MoneyGet());
	}

	this->WearedItemRemove(target_slot);

	sPentagramSystem->MovePentagramToInventoryFromWarehouse(this, this->GetInventory()->GetItem(target_slot));

	return target;
}

uint8 Player::MoveItem(Trade, EventInventory)
{
	if( !EVENT_INVENTORY_RANGE(source_slot) )
	{
		return 0xFF;
	}

	if( !trade_range(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetEventInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToTrade(pSourceItem) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO TRADE FROM EVENTINVENTORY") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ EVENTINVENTORY -> TRADE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetTradeInventory()->AddItem(*pSourceItem, target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(TradeInventory, EventInventory);

	this->GetInterfaceState()->GetTarget()->ToPlayer()->TradeItemAdd(target_slot, item_info);

	return 1;
}

uint8 Player::MoveItem(EventInventory, Trade)
{
	if( !trade_range(source_slot) )
	{
		return 0xFF;
	}

	if( !EVENT_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetTradeInventory()->GetItem(source_slot);

	if ( sItemMgr->GetItemInventoryType(pSourceItem->GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO EVENTINVENTORY FROM TRADE") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ TRADE -> EVENTINVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetEventInventory()->AddItem(this->GetTradeInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(EventInventory, TradeInventory);

	if ( Player* pPlayer = this->GetInterfaceState()->GetTarget()->ToPlayer() )
	{
		this->GetTradeInventory()->SetButtonEnabled(false);
		//this->TradeOkTime = GetTickCount();
		this->TradeButtonOKSend(0);
		pPlayer->GetTradeInventory()->SetButtonEnabled(false);
		//pPlayer->TradeOkTime = GetTickCount();
		pPlayer->TradeButtonOKSend(2);
		pPlayer->TradeItemDelete(source_slot);
	}

	return target;
}

uint8 Player::MoveItem(EventInventory, EventInventory)
{
	if( !EVENT_INVENTORY_RANGE(source_slot) )
	{
		return 0xFF;
	}

	if( !EVENT_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetEventInventory()->GetItem(source_slot);

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO EVENTINVENTORY FROM EVENTINVENTORY") )
	{
		return 0xFF;
	}

	if( sEventInventory->EventInventoryAddItemStack(this, source_slot, target_slot) )
	{
		return 0xFF;
	}

	this->GetEventInventory()->StartTmp();
	this->GetEventInventory()->ItemSet(source_slot, 0);

	if( this->GetEventInventory()->AddItem(this->GetEventInventory()->item[source_slot], target_slot) == 0xFF )
	{
		this->GetEventInventory()->EndTmp();
		return 0xFF;
	}

	MoveItemSuccess(EventInventory, EventInventory);

	return target;
}

uint8 Player::MoveItem(ChaosBox, EventInventory)
{
	if( !EVENT_INVENTORY_RANGE(source_slot) )
	{
		return 0xFF;
	}

	if( !chaos_box_range(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetEventInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToChaos(pSourceItem, target) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO CHAOSBOX FROM EVENTINVENTORY") )
	{
		return 0xFF;
	}

	if ( pSourceItem->IsInventoryActive() )
	{
		return uint8(-1);
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ EVENTINVENTORY -> CHAOSBOX ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetMixInventory()->AddItem(*pSourceItem, target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(MixInventory, EventInventory);

	return target;
}

uint8 Player::MoveItem(EventInventory, ChaosBox)
{
	if( !chaos_box_range(source_slot) )
	{
		return 0xFF;
	}

	if( !EVENT_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetMixInventory()->GetItem(source_slot);

	if ( sItemMgr->GetItemInventoryType(pSourceItem->GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO EVENTINVENTORY FROM CHAOSBOX") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ CHAOSBOX -> EVENTINVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetEventInventory()->AddItem(this->GetMixInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(EventInventory, MixInventory);

	return target;
}

uint8 Player::MoveItem(EventInventory, Inventory)
{
	if( !inventory_range(source_slot) )
	{
		return 0xFF;
	}

	if( !EVENT_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);

	if ( sItemMgr->GetItemInventoryType(pSourceItem->GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO EVENTINVENTORY FROM INVENTORY") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ INVENTORY -> EVENTINVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetEventInventory()->AddItem(this->GetInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(EventInventory, Inventory);

	this->WearedItemRemove(source_slot);

	return target;
}

uint8 Player::MoveItem(Warehouse, EventInventory)
{
	if( !EVENT_INVENTORY_RANGE(source_slot) )
	{
		return 0xFF;
	}

	if( !warehouse_range(target_slot) )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(target_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	Item * pSourceItem = this->GetEventInventory()->GetItem(source_slot);

	if( !this->CheckItemMoveToVault(pSourceItem) )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO WAREHOUSE FROM EVENTINVENTORY") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ EVENTINVENTORY -> WAREHOUSE ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);
	
	if( this->GetWarehouse()->AddItem(this->GetEventInventory()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(Warehouse, EventInventory);

	return target;
}
	
uint8 Player::MoveItem(EventInventory, Warehouse)
{
	if( !warehouse_range(source_slot) )
	{
		return 0xFF;
	}

	if( !EVENT_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(source_slot) && this->GetWarehouse()->GetExpanded() < 1 )
	{
		return 0xFF;
	}

	uint32 take_price = this->WarehouseGetUsedHowMuch();

	if ( !this->MoneyHave(take_price) && !this->GetWarehouse()->MoneyHave(take_price) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need %u Zen to use your vault.", take_price);
		return 0xFF;
	}

	Item * pSourceItem = this->GetWarehouse()->GetItem(source_slot);

	if ( sItemMgr->GetItemInventoryType(pSourceItem->GetItem()) != ITEM_INVENTORY_TYPE_EVENT )
	{
		return 0xFF;
	}

	if ( Player::TransactionSerialFind(this, pSourceItem->GetSerialServer(), pSourceItem->GetSerial(), "MOVE TO EVENTINVENTORY FROM WAREHOUSE") )
	{
		return 0xFF;
	}

	sLog->outInfo(LOG_PLAYER, "[ ITEM MOVE ][ WAREHOUSE -> EVENTINVENTORY ]: %s - ItemData: %s - To :%u", this->BuildLog().c_str(), pSourceItem->BuildLog(source_slot).c_str(), target_slot);

	if( this->GetEventInventory()->AddItem(this->GetWarehouse()->item[source_slot], target_slot) == 0xFF )
	{
		return 0xFF;
	}

	MoveItemSuccess(EventInventory, Warehouse);

	if ( this->MoneyHave(take_price) )
	{
		this->MoneyReduce(take_price, true);
	}
	else
	{
		this->GetWarehouse()->MoneyReduce(take_price);
		this->WarehouseMoneyInOutResult(1, this->GetWarehouse()->GetZen(), this->MoneyGet());
	}

	return target;
}

uint8 Player::MoveItem(MuunInventory, MuunInventory)
{
	if( !MUUN_INVENTORY_RANGE(source_slot) )
	{
		return 0xFF;
	}

	if( !MUUN_INVENTORY_RANGE(target_slot) )
	{
		return 0xFF;
	}

	if( !sMuunSystem->CheckItemMoveToMuunInventory(this, this->GetMuunInventory()->GetItem(source_slot), target_slot) )
	{
		return 0xFF;
	}

	if( this->GetMuunInventory()->AddItem(*this->GetMuunInventory()->GetItem(source_slot), target_slot) == 0xFF )
	{
		return 0xFF;
	}

	this->GetMuunInventory()->DeleteItem(source_slot);

	if( MUUN_INVENTORY_WEAR_RANGE(source_slot) )
	{
		this->PreviewMake();
		sMuunSystem->GCMuunItemChangeSend(this, source_slot, true);
		sMuunSystem->GCMuunItemStatusSend(this, source_slot, 0);
		this->SetMuunItemStatus(source_slot, 0);
	}

	if( MUUN_INVENTORY_WEAR_RANGE(target_slot) )
	{
		this->PreviewMake();
		sMuunSystem->GCMuunItemChangeSend(this, target_slot, true);
	}

	if ( MUUN_INVENTORY_WEAR_RANGE(source_slot) || MUUN_INVENTORY_WEAR_RANGE(target_slot) )
	{
		this->CalculateCharacter();
	}

	return target;
}

void Player::GetPreviewElementalDamage(int32 & damage_min, int32 & damage_max, int32 & pvp_damage_min, int32 & pvp_damage_max)
{
	damage_min = this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN);
	damage_max = this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX);

	Item const* pPentagram = this->GetInventory()->GetItem(PENTAGRAM_SLOT);

	damage_min += (((!pPentagram->IsValid()) ? 0 : pPentagram->GetMinDamage()) * this->GetPentagramOption()->GetMulPentagramDamage()) / 100;
	damage_max += (((!pPentagram->IsValid()) ? 0 : pPentagram->GetMaxDamage()) * this->GetPentagramOption()->GetMulPentagramDamage()) / 100;

	damage_min += this->GetPentagramJewelOption()->GetAddElementalDamage();
	damage_max += this->GetPentagramJewelOption()->GetAddElementalDamage();

	pvp_damage_min = damage_min;
	pvp_damage_max = damage_max;

	pvp_damage_min += this->GetPentagramJewelOption()->GetAddElementalDamagePvP();
	pvp_damage_max += this->GetPentagramJewelOption()->GetAddElementalDamagePvP();

	pvp_damage_min += (pvp_damage_min * this->GetPentagramJewelOption()->GetMulElementalDamagePvP()) / 100;
	pvp_damage_max += (pvp_damage_max * this->GetPentagramJewelOption()->GetMulElementalDamagePvP()) / 100;

	damage_min += this->GetPentagramJewelOption()->GetAddElementalDamagePvM();
	damage_max += this->GetPentagramJewelOption()->GetAddElementalDamagePvM();

	damage_min += (damage_min * this->GetPentagramJewelOption()->GetMulElementalDamagePvM()) / 100;
	damage_max += (damage_max * this->GetPentagramJewelOption()->GetMulElementalDamagePvM()) / 100;
}

void Player::GetPreviewElementalDefense(int32 & defense, int32 & pvp_defense)
{
	defense = this->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE);

	Item const* pPentagram = this->GetInventory()->GetItem(PENTAGRAM_SLOT);

	defense += (((!pPentagram->IsValid()) ? 0 : pPentagram->GetDefense()) * this->GetPentagramOption()->GetMulPentagramDefense()) / 100;

	defense += (this->GetIntData(UNIT_INT_DEFENSE) * this->GetPentagramOption()->GetAddElementalDefenseTransferRate()) / 100;

	defense += this->GetPentagramJewelOption()->GetAddElementalDefense();

	pvp_defense = defense;

	pvp_defense += this->GetPentagramJewelOption()->GetAddElementalDefensePvP();

	defense += this->GetPentagramJewelOption()->GetAddElementalDefensePvM();
}
	
void Player::GetPreviewElementalAttackSuccessRate(int32 & attack_success_rate, int32 & pvp_attack_success_rate)
{
	attack_success_rate = this->GetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE);

	attack_success_rate += (attack_success_rate * this->GetFloatData(UNIT_FLOAT_ELEMENTAL_ATTACK_SUCCESS_RATE_PERCENT)) / 100;

	pvp_attack_success_rate = attack_success_rate;

	attack_success_rate += this->GetData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVM);
	pvp_attack_success_rate += this->GetData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVP);

	attack_success_rate -= (attack_success_rate * this->GetBuffEffect(BUFF_BLINDNESS)) / 100;
	pvp_attack_success_rate -= (pvp_attack_success_rate * this->GetBuffEffect(BUFF_BLINDNESS)) / 100;

	if (attack_success_rate < 0)
	{
		attack_success_rate = 0;
	}

	if (pvp_attack_success_rate < 0)
	{
		pvp_attack_success_rate = 0;
	}
}
	
void Player::GetPreviewElementalDefenseSuccessRate(int32 & defense_success_rate, int32 & pvp_defense_success_rate)
{
	defense_success_rate = this->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE);

	defense_success_rate += (defense_success_rate * this->GetFloatData(UNIT_FLOAT_ELEMENTAL_DEFENSE_SUCCESS_RATE_PERCENT)) / 100;

	defense_success_rate = ((defense_success_rate < 0) ? 0 : defense_success_rate);

	pvp_defense_success_rate = defense_success_rate;

	defense_success_rate += this->GetData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVM);
	pvp_defense_success_rate += this->GetData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVP);
}

bool Player::InventoryAddItemStack(uint8 source_slot, uint8 target_slot)
{
	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);
	Item * pTargetItem = this->GetInventory()->GetItem(target_slot);

	if ( !pSourceItem->IsItem() ||
		 !pTargetItem->IsItem() )
	{
		return false;
	}

	if ( pSourceItem->GetItem() != pTargetItem->GetItem() ||
		 pSourceItem->GetLevel() != pTargetItem->GetLevel() ||
		 pSourceItem->GetSocketBonus() != pTargetItem->GetSocketBonus() )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(pSourceItem->GetItem());

	if ( !item_info )
	{
		return false;
	}

	int32 MaxStack = item_info->GetStackData()->GetMaxStack();

	if ( MaxStack <= 0 )
	{
		if ( sGameServer->IsJoinItemsWithExpireTime() )
		{
			if ( pSourceItem->GetExpireDate() > 0 && pTargetItem->GetExpireDate() > 0 )
			{
				time_t source_time = pSourceItem->GetRemainTime();
				time_t target_time = pTargetItem->GetRemainTime();

				target_time += source_time;

				pTargetItem->SetExpireDate(time(nullptr) + target_time);
				pTargetItem->SetState(ITEM_CHANGED);

				this->ClearItem(source_slot, 0);

				return true;
			}
		}

		return false;
	}

	if ( pTargetItem->GetDurability() >= MaxStack )
	{
		return false;
	}

	int32 AddDur = (int32)(MaxStack - pTargetItem->GetDurability());
	AddDur = ((AddDur > pSourceItem->GetDurability())?(int32)pSourceItem->GetDurability():AddDur);

	pSourceItem->AlterDurability(0, AddDur);
	pTargetItem->AlterDurability(1, AddDur);

	this->ItemMoveResult(-1);

	if ( pTargetItem->GetDurability() == MaxStack && item_info->GetStackData()->GetConvertedItem() )
	{
		this->ClearItem(target_slot, 1);

		sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(item_info->GetStackData()->GetConvertedItem()->GetItem(), pSourceItem->GetLevel(), 1, 0, 0, 0, 0, 0, nullptr, pSourceItem->GetSocketBonus()));
	}
	else
	{
		this->SendItemDurability(target_slot, 0);
	}

	if ( pSourceItem->GetDurability() < 1.0f )
	{
		this->ClearItem(source_slot, 0);
	}
	else
	{
		this->SendItemDurability(source_slot, 0);
	}

	return true;
}

bool Player::InventoryAddItemStackOnBuy(Item & item)
{
	if ( !item.IsItem() || item.GetDurability() <= 0.0f )
	{
		return false;
	}

	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if ( !item_info )
	{
		return false;
	}

	if ( !item_info->GetStackData()->IsStackOnLoot() )
	{
		return false;
	}

	int32 MaxStack = item_info->GetStackData()->GetMaxStack();

	if ( MaxStack <= 0 )
	{
		return false;
	}

	switch ( item_info->GetInventoryType() )
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
		{
			use_inventory_loop(i)
			{
				if ( !this->GetInventory()->CanUseExpandedSlot(i) )
				{
					continue;
				}

				if ( !this->GetInventory()->GetItem(i)->IsItem() )
				{
					continue;
				}

				if ( this->GetInventory()->GetItem(i)->GetItem() != item.GetItem() ||
					 this->GetInventory()->GetItem(i)->GetLevel() != item.GetLevel() ||
					 this->GetInventory()->GetItem(i)->GetSocketBonus() != item.GetSocketBonus() )
				{
					continue;
				}

				if ( this->GetInventory()->GetItem(i)->GetDurability() < MaxStack )
				{
					int32 AddDur = (int32)(MaxStack - this->GetInventory()->GetItem(i)->GetDurability());

					AddDur = ((AddDur>item.GetDurability())?(int32)item.GetDurability():AddDur);

					item.AlterDurability(0, AddDur);
					this->GetInventory()->GetItem(i)->AlterDurability(1, AddDur);

					this->QuestMUObjectiveItemGet(item);
			
					if ( this->GetInventory()->GetItem(i)->GetDurability() == MaxStack && item_info->GetStackData()->GetConvertedItem() )
					{
						this->ClearItem(i, 1);

						sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(item_info->GetStackData()->GetConvertedItem()->GetItem(), item.GetLevel(), 1, 0, 0, 0, 0, 0, nullptr, item.GetSocketBonus()));

						if( item.GetDurability() < 1.0f )
						{
							return true;
						}
					}
					else
					{
						this->SendItemDurability(i, 0);

						if( item.GetDurability() < 1.0f )
						{
							return true;
						}
					}
				}
			}
		} break;
	}

	return false;
}

void Player::OffensiveFullRestore()
{
	if (roll_chance_f(this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_HP_RECOVERY)))
	{
		this->PowerSet(POWER_LIFE, this->PowerGetTotal(POWER_LIFE), true);
	}

	if (roll_chance_f(this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_MP_RECOVERY)))
	{
		this->PowerSet(POWER_MANA, this->PowerGetTotal(POWER_MANA), true);
	}

	if (roll_chance_f(this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_SD_RECOVERY)))
	{
		this->PowerSet(POWER_SHIELD, this->PowerGetTotal(POWER_SHIELD), true);
	}

	if (roll_chance_f(this->GetDataFloat(PLAYER_DATA_OFFENSIVE_TOTAL_AG_RECOVERY)))
	{
		this->PowerSet(POWER_STAMINA, this->PowerGetTotal(POWER_STAMINA), true);
	}
}

bool Player::PotionUse(uint16 item, uint8 level, uint16 value)
{
	int32 add_life = 0;
	int32 add_mana = 0;
	int32 add_shield = 0;
	int32 add_stamina = 0;
	//uint32 statistic_id = STATISTIC_LIFE_POTIONS;

	switch ( item )
	{
	case ITEMGET(14, 0):
		{
			add_life = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() * 2);
			if ( add_life < 0 ) { add_life = 0; }
			add_life += this->PowerGetTotal(POWER_LIFE) * (10 + (level * 5)) / 100;
		} break;

	case ITEMGET(14, 1):
		{
			add_life = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() * 2);
			if ( add_life < 0 ) { add_life = 0; }
			add_life += this->PowerGetTotal(POWER_LIFE) * (20 + (level * 5)) / 100;
		} break;

	case ITEMGET(14, 2):
		{
			add_life = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() * 2);
			if ( add_life < 0 ) { add_life = 0; }
			add_life += this->PowerGetTotal(POWER_LIFE) * (30 + (level * 5)) / 100;
		} break;

	case ITEMGET(14, 3):
		{
			add_life = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() * 2);
			if ( add_life < 0 ) { add_life = 0; }
			add_life += this->PowerGetTotal(POWER_LIFE) * (40 + (level * 5)) / 100;
		} break;
		
	case ITEMGET(14, 4):
		{
			add_mana = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
			if ( add_mana < 0 ) { add_mana = 0; }
			add_mana += this->PowerGetTotal(POWER_MANA) * (20 + (level * 5)) / 100;
			//statistic_id = STATISTIC_MANA_POTIONS;
		} break;

	case ITEMGET(14, 5):
		{
			add_mana = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
			if ( add_mana < 0 ) { add_mana = 0; }
			add_mana += this->PowerGetTotal(POWER_MANA) * (30 + (level * 5)) / 100;
			//statistic_id = STATISTIC_MANA_POTIONS;
		} break;

	case ITEMGET(14, 6):
		{
			add_mana = (value * 10) - (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
			if ( add_mana < 0 ) { add_mana = 0; }
			add_mana += this->PowerGetTotal(POWER_MANA) * (40 + (level * 5)) / 100;
			//statistic_id = STATISTIC_MANA_POTIONS;
		} break;

	case ITEMGET(14, 38):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 5 / 100;
			add_life = this->PowerGetTotal(POWER_LIFE) * 10 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 39):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 10 / 100;
			add_life = this->PowerGetTotal(POWER_LIFE) * 25 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 40):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 20 / 100;
			add_life = this->PowerGetTotal(POWER_LIFE) * 45 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 35):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 25 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 36):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 35 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 37):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 45 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 133):
		{
			add_shield = this->PowerGetTotal(POWER_SHIELD) * 65 / 100;
			//statistic_id = STATISTIC_SHIELD_POTIONS;
		} break;

	case ITEMGET(14, 70):
		{
			add_life = this->PowerGetTotal(POWER_LIFE) * 100 / 100;
		} break;

	case ITEMGET(14, 71):
		{
			add_mana = this->PowerGetTotal(POWER_MANA) * 100 / 100;
		} break;

	case ITEMGET(14, 94):
		{
			add_life = this->PowerGetTotal(POWER_LIFE) * 65 / 100;
		} break;

	default:
		{
			return false;
		} break;
	}

	if ( this->HasBuff(BUFF_PARALYSIS) )
	{
		add_life -= add_life * sGameServer->GetRadianceParalysisEffect() / 100;
		add_mana -= add_mana * sGameServer->GetRadianceParalysisEffect() / 100;
		add_shield -= add_shield * sGameServer->GetRadianceParalysisEffect() / 100;
		add_stamina -= add_stamina * sGameServer->GetRadianceParalysisEffect() / 100;
	}

	if ( add_life > 0 )
	{
		int32 heal_value = (this->PowerGet(POWER_LIFE) + add_life) > this->PowerGetTotal(POWER_LIFE) ? this->PowerGetTotal(POWER_LIFE) - this->PowerGet(POWER_LIFE): add_life;

		if ( heal_value < 0 )
		{
			heal_value = 0;
		}

		sHuntingRecord->Update(this, 0, 0, 0, heal_value, 0);

		sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_NOT_HP, 0, true);

		this->PowerIncrease(POWER_LIFE, add_life);
	}

	if ( add_mana > 0 )
	{
		this->PowerIncrease(POWER_MANA, add_mana);

		sLabyrinthDimensions->UpdateMission(this, LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_KILL_MONSTERS_NOT_MP, 0, true);
	}

	if ( add_shield > 0 )
	{
		this->PowerIncrease(POWER_SHIELD, add_shield);
	}

	if ( add_stamina > 0 )
	{
		this->PowerIncrease(POWER_STAMINA, add_stamina);
	}

	if (add_life > 0 || add_shield > 0)
	{
		this->LifeSend();
	}

	if (add_mana > 0 || add_stamina > 0)
	{
		this->ManaSend();
	}

	if ( add_shield > 0 )
	{
		this->EffectSend(3);
	}

	//this->UpdateStatistic(statistic_id, 1);

	return true;
}

bool Player::ExperiencePotionUse(uint16 item, uint8 level)
{
	int64 experience = 0;
	uint8 type = LEVEL_DATA_NORMAL;

	switch ( item )
	{
	case ITEMGET(14, 269): // Normal 3.000.000
		{
			experience = 3000000;
		} break;

	case ITEMGET(14, 270): // Normal 5.000.000
		{
			experience = 5000000;
		} break;

	case ITEMGET(14, 271): // Normal 8.000.000
		{
			experience = 8000000;
		} break;

	case ITEMGET(14, 272): // Normal 10.000.000
		{
			experience = 10000000;
		} break;

	case ITEMGET(14, 273): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 10000000;
		} break;

	case ITEMGET(14, 274): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 20000000;
		} break;

	case ITEMGET(14, 275): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 30000000;
		} break;

	case ITEMGET(14, 276): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 40000000;
		} break;

	case ITEMGET(14, 279): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 50000000;
		} break;

	case ITEMGET(14, 280): // Master 
		{
			type = LEVEL_DATA_MASTER;
			experience = 100000000;
		} break;
	}

	if ( experience <= 0 )
	{
		return false;
	}

	if ( type == LEVEL_DATA_MASTER && this->IsMajestic() )
	{
		type = LEVEL_DATA_MAJESTIC;
	}

	if ( type == LEVEL_DATA_MAJESTIC && !this->IsMajestic() )
	{
		return false;
	}

	if ( type == LEVEL_DATA_MASTER && !this->IsMaster() )
	{
		return false;
	}

	if ( type == LEVEL_DATA_NORMAL && this->IsMaster() )
	{
		return false;
	}

	if ( this->GetLevelData(type)->GetLevel() >= sGameServer->GetMaxLevel(type) )
	{
		return false;
	}

	int64 diff = this->GetLevelData(type)->GetNextExperience() - this->GetLevelData(type)->GetExperience();

	if ( diff < experience )
	{
		experience = diff;
	}

	if ( experience <= 0 )
	{
		experience = 1;
	}

	this->GiveExperience(experience, false, true);

	return true;
}

void Player::OpenBoxRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( this->GetPersonalStore()->IsBusy() )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( this->GetTransaction() == TRANSACTION_TYPE_BEGIN )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->OpenBoxResult(0xFF);
		return;
	}

	POINTER_PCT_LOG(OPEN_BOX_REQUEST, lpMsg, Packet, 0);

	/*
	result 2 -> OK Inventario
	result 3 -> OK Event Inventario
	result 0xFD -> Full ZEN
	result 0xFE -> FUll Inventory
	result 0xFF -> Unable to use
	*/

	Item * pItem = nullptr;
	uint8 type = lpMsg->type;

	if ( lpMsg->type == 0x00 )
	{
		if ( !inventory_range(lpMsg->inventory_slot) )
		{
			this->OpenBoxResult(0xFF);
			return;
		}

		pItem = this->GetInventory()->GetItem(lpMsg->inventory_slot);
	}
	else if ( lpMsg->type == 0x15 )
	{
		if ( !EVENT_INVENTORY_RANGE(lpMsg->inventory_slot) )
		{
			this->OpenBoxResult(0xFF);
			return;
		}

		pItem = this->GetEventInventory()->GetItem(lpMsg->inventory_slot);
	}
	else
	{
		type = -1;
	}

	if ( type == uint8(-1) )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( !pItem )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( !pItem->IsItem() )
	{
		this->OpenBoxResult(0xFF);
		return;
	}

	if ( pItem->GetItem() == ITEMGET(13, 66) )
	{
		if (sGameServer->IsSantaVillageEnabled() && (sEventMgr->IsSeasonEventOn(sGameServer->GetSantaVillageSeason()) || !sGameServer->IsSantaVillageCheckSeason()))
		{
			this->TeleportToLocation(WORLD_SANTA_VILLAGE, 220, 20, this->GetDirection(), this->GetInstance());
			pItem->AlterDurability(0, 1.0f);

			if ( pItem->GetDurability() <= 0.0f )
			{
				if ( type == 0x00 )
				{
					this->ClearItem(lpMsg->inventory_slot);
				}
				else if ( type == 0x15 )
				{
					this->GetEventInventory()->DeleteItem(lpMsg->inventory_slot);
					sEventInventory->GCEventItemDeleteSend(this, lpMsg->inventory_slot);
				}
			}
			else
			{
				if ( type == 0x00 )
				{
					this->SendItemDurability(lpMsg->inventory_slot, 1);
				}
				else if ( type == 0x15 )
				{
					sEventInventory->GCEventItemDurSend(this, lpMsg->inventory_slot, pItem->GetDurability());
				}
			}
		}
		else
		{
			this->OpenBoxResult(0xFF);
			this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Santa Village is not avaliable.");
		}

		return;
	}

	if ( !this->GetInventory()->IsEmptySpace(4, 4) )
	{
		this->OpenBoxResult(0xFE);
		return;
	}

	uint8 result = sItemBagMgr->ExecuteItemBag(ITEM_BAG_INVENTORY_ITEM, this, "", pItem->GetItem(), pItem->GetLevel(), 0, 0, pItem->GetSerialServer(), pItem->GetSerial(), pItem->GetName());

	if ( result == ITEM_BAG_RESULT_SUCCESS )
	{
		if ( type == 0x00 )
		{
			this->ClearItem(lpMsg->inventory_slot);
		}
		else if ( type == 0x15 )
		{
			this->GetEventInventory()->DeleteItem(lpMsg->inventory_slot);
			sEventInventory->GCEventItemDeleteSend(this, lpMsg->inventory_slot);
		}
	}
	else
	{
		this->OpenBoxResult(0xFF);
	}
}

void Player::OpenBoxResult(uint8 result, uint16 item)
{
	OPEN_BOX_RESULT pMsg(result);
	pMsg.slot = item;
	this->SEND_PCT(pMsg);
}

void Player::ChatBlockAdd(uint8 * Packet)
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

	POINTER_PCT_LOG(CHAT_BLOCK_ADD, lpMsg, Packet, 0);

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(name_safe, MAX_CHARACTER_LENGTH + 1);
	strcpy(name_safe, name);

	if ( !this->ChatBlockAdd(name_safe) )
	{
		this->ChatBlockList(2, 3);
		return;
	}

	this->ChatBlockList(2, 1);
}

bool Player::ChatBlockAdd(const char * name)
{
	std::string name_safe = name;

	strToLower(name_safe);

	for ( PlayerChatBlockList::const_iterator it = this->chat_block_list.begin(); it != this->chat_block_list.end(); ++it )
	{
		std::string list_name = (*it);

		strToLower(list_name);

		if ( name_safe == list_name )
		{
			return false;
		}
	}

	this->chat_block_list.insert(name);
	return true;
}
	
void Player::ChatBlockDelete(uint8 * Packet)
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

	POINTER_PCT_LOG(CHAT_BLOCK_DELETE, lpMsg, Packet, 0);

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE(name_safe, MAX_CHARACTER_LENGTH + 1);
	strcpy(name_safe, name);

	PlayerChatBlockList::const_iterator it = this->chat_block_list.find(name_safe);

	if ( it != this->chat_block_list.end() )
	{
		this->chat_block_list.erase(it);
		this->ChatBlockList(3, 1);
	}
	else
	{
		this->ChatBlockList(3, 2);
	}
}
	
void Player::ChatBlockList(uint8 type, uint8 result)
{
	uint8 buffer[8092];
	POINTER_PCT(CHAT_BLOCK_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(CHAT_BLOCK_LIST_BODY, body, buffer, sizeof(CHAT_BLOCK_LIST_HEAD));
	head->count = 0;
	head->type = type;
	head->result = result;
	head->action = 0;

	if ( result == 1 || type == 1 )
	{
		for ( PlayerChatBlockList::const_iterator it = this->chat_block_list.begin(); it != this->chat_block_list.end(); ++it )
		{
			body[head->count].position = head->count;
			memset(body[head->count].name, 0, MAX_CHARACTER_LENGTH + 1);
			strcpy(body[head->count].name, (*it).c_str());
			body[head->count].next = 1;

			++head->count;
		}
	}

	if ( head->count > 0 )
	{
		body[head->count - 1].next = 0;
		head->action = 1;
	}

	head->h.set(0x6E, 0x00, sizeof(CHAT_BLOCK_LIST_HEAD) + (head->count * sizeof(CHAT_BLOCK_LIST_BODY)));

	this->sendPacket(buffer, head->h.get_size());
}

bool Player::ChatBlockFind(const char * name) const
{
	std::string lower_name = name;
	strToLower(lower_name);

	for ( PlayerChatBlockList::const_iterator it = this->chat_block_list.begin(); it != this->chat_block_list.end(); ++it )
	{
		std::string cur_name = *it;
		strToLower(cur_name);

		if ( lower_name == cur_name )
		{
			return true;
		}
	}

	return false;
}

void Player::ItemDisassemble(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( this->GetPersonalStore()->IsBusy() )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::None )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(ITEM_DISASSEMBLE, lpMsg, Packet, 0);

	if ( !use_inventory_range(lpMsg->slot) )
	{
		this->ItemDisassembleResult(0);
		return;
	}

	Item const* pItem = this->GetInventory()->GetItem(lpMsg->slot);

	if ( !pItem )
	{
		this->ItemDisassembleResult(0);
		return;
	}

	if ( !pItem->IsItem() )
	{
		this->ItemDisassembleResult(0);
		return;
	}

	if ( pItem->IsAncient() )
	{
		if ( sItemMgr->IsLuckyItem(pItem->GetItem()) )
		{
			this->ItemDisassembleResult(0);
			return;
		}

		item_set_type const* pItemSetType = sItemMgr->GetItemSetType(pItem->GetItem());

		if ( !pItemSetType )
		{
			this->ItemDisassembleResult(0);
			return;
		}

		uint8 ancient_id = pItem->GetAncientType();

		if ( ancient_id == 0 )
		{
			this->ItemDisassembleResult(0);
			return;
		}

		item_set_option const* pItemSetOption = sItemMgr->GetItemSetOption(pItemSetType->GetAncientID(ancient_id - 1));

		if ( !pItemSetOption )
		{
			this->ItemDisassembleResult(0);
			return;
		}

		if ( !pItemSetOption->IsDisassemble() )
		{
			this->ItemDisassembleResult(0);
			return;
		}
	}
	else 
	{
		if (pItem->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			sLog->outInfo(LOG_ITEMS, "[%s][%s][%s]", __FUNCTION__, this->BuildLog().c_str(), pItem->BuildLog(lpMsg->slot).c_str());

			this->ClearItem(lpMsg->slot);

			Item item;
			if (sItemBagMgr->RunItemBag(this, sGameServer->GetGuardianDisassembleItemBag(), item))
			{
				this->ItemDisassembleResult(1, item.GetItem(), 1);
			}
			else
			{
				this->ItemDisassembleResult(0);
			}

			return;

		}
		else if (pItem->GetKind2() != ItemKind::ERRTEL)
		{
			this->ItemDisassembleResult(0);
			return;
		}
	}

	sLog->outInfo(LOG_ITEMS, "[%s][%s][%s]", __FUNCTION__, this->BuildLog().c_str(), pItem->BuildLog(lpMsg->slot).c_str());

	this->ClearItem(lpMsg->slot);
	sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(ITEMGET(14, 290), 0, 1.0f));

	this->ItemDisassembleResult(1, ITEMGET(14, 290), 1);
}
	
void Player::ItemDisassembleResult(uint8 result, uint32 item, uint32 count)
{
	DISSASEMBLE_RESULT pMsg;
	pMsg.result = result;
	pMsg.item = item;
	pMsg.count = count;
	this->SEND_PCT(pMsg);
}

void Player::SendEventNotification(uint8 event_id, uint8 open)
{
	EVENT_NOTIFICATION pMsg(event_id, open);
	this->SEND_PCT(pMsg);
}

void Player::AcheronWarp()
{
	if ( !this->IsPlaying() )
	{
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::AcheronEntrance )
	{
		return;
	}

	this->GetInterfaceState()->Reset();

	use_inventory_loop(i)
	{
		if ( !this->GetInventory()->CanUseExpandedSlot(i) )
		{
			continue;
		}

		if ( this->GetInventory()->GetItem(i)->IsItem() &&
			 this->GetInventory()->GetItem(i)->GetItem() == ITEMGET(13, 146) )
		{
			this->ClearItem(i);
			this->MoveToGate(417);
			return;
		}
	}

	ACHERON_WARP_RESULT pMsg(1);
	this->SEND_PCT(pMsg);
}

void Player::SendRuudMoney()
{
	RUUD_SEND pMsg(this->GetRuudMoney());
	this->SEND_PCT(pMsg);
}

void Player::SendRuudMoney(uint32 add)
{
	RUUD_SEND pMsg(this->GetRuudMoney());
	pMsg.add_ruud = add;
	pMsg.add = 1;
	this->SEND_PCT(pMsg);
}

bool Player::HasMount() const
{
	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		if (pItem->IsMount())
		{
			return true;
		}
	}

	return false;
}

bool Player::IsActiveInventoryItem(uint16 item) const
{
	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || pItem->GetItem() != item)
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		return true;
	}

	return false;
}

Item* Player::GetActiveInventoryItem(uint16 item)
{
	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item * pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || pItem->GetItem() != item)
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		return pItem;
	}

	return nullptr;
}

Item const* Player::GetActiveInventoryItem(uint16 item) const
{
	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem() || pItem->GetItem() != item)
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		return pItem;
	}

	return nullptr;
}

Item const* Player::GetActiveGuardianMount() const
{
	for (int32 i = 0; i < use_inventory_size; ++i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		if (!pItem->IsInventoryActive())
		{
			continue;
		}

		if (pItem->GetKind2() != ItemKind::GUARDIAN_MOUNT)
		{
			continue;
		}

		return pItem;
	}

	return nullptr;
}

void Player::SendMount(Player* pPlayer)
{
	MUUN_MOUNT_SINGLE pMsg;
	pMsg.SetIndex(this->GetEntry());
	pMsg.SetMount(this->GetVisibleMount());

	Item const* pItem = this->GetActiveInventoryItem(this->GetVisibleMount());

	if (pItem)
	{
		if (pItem->GetItem() == PET_FENRIR)
		{
			pMsg.data = pItem->GetExe();
		}
	}

	if (pPlayer)
	{
		pPlayer->SEND_PCT(pMsg);
	}
	else
	{
		this->SEND_PCT(pMsg);
		this->SEND_PCT_VP(pMsg);
	}
}

void Player::ResetMount(uint16 item)
{
	if (this->GetVisibleMount() == item)
	{
		this->SetVisibleMount(-1);

		this->SendMount();
	}
}

void Player::AssignMount()
{
	if (this->IsActiveInventoryItem(PET_DARK_HORSE))
	{
		this->SetVisibleMount(PET_DARK_HORSE);
	}
	else if (Item const* pGuardianItem = this->GetActiveGuardianMount())
	{
		this->SetVisibleMount(pGuardianItem->GetItem());
	}
	else
	{
		for (int32 i = 0; i < use_inventory_size; ++i)
		{
			Item const* pItem = this->GetInventory()->GetItem(i);

			if (!pItem || !pItem->IsItem())
			{
				continue;
			}

			if (!pItem->IsInventoryActive())
			{
				continue;
			}

			if (pItem->GetItem() != PET_UNIRIA &&
				pItem->GetItem() != PET_DINORANT &&
				pItem->GetItem() != PET_FENRIR)
			{
				continue;
			}

			this->SetVisibleMount(pItem->GetItem());
			break;
		}
	}
}

uint8 Player::AddItem(Item const& item, uint8 & inventory, uint8 slot, AddItemSend send)
{
	switch ( sItemMgr->GetItemInventoryType(item.GetItem()) )
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
		{
			inventory = ITEM_INVENTORY_TYPE_NORMAL;

			if ( slot == uint8(-1) )
			{
				slot = this->GetInventory()->AddItem(item);
			}
			else
			{
				slot = this->GetInventory()->AddItem(item, slot);
			}

			if ( slot != uint8(-1) )
			{
				this->GetInventory()->GetItem(slot)->Convert();

				if ( !this->IsFirstTime() && (send == ADD_ITEM_SEND_OK) )
				{
					this->SendInventory(slot);
				}
			}
		} break;

	case ITEM_INVENTORY_TYPE_EVENT:
		{
			inventory = ITEM_INVENTORY_TYPE_EVENT;

			if ( slot == uint8(-1) )
			{
				slot = this->GetEventInventory()->AddItem(item);
			}
			else
			{
				slot = this->GetEventInventory()->AddItem(item, slot);
			}

			if ( slot != uint8(-1) )
			{
				this->GetEventInventory()->GetItem(slot)->Convert();

				if ( !this->IsFirstTime() && (send == ADD_ITEM_SEND_OK) )
				{
					sEventInventory->GCEventItemModifySend(this, slot);
				}
			}
		} break;

	case ITEM_INVENTORY_TYPE_MUUN:
		{
			inventory = ITEM_INVENTORY_TYPE_MUUN;

			if ( slot == uint8(-1) )
			{
				slot = this->GetMuunInventory()->AddItem(item);
			}
			else
			{
				slot = this->GetMuunInventory()->AddItem(item, slot);
			}

			if ( slot != uint8(-1) )
			{
				this->GetMuunInventory()->GetItem(slot)->Convert();

				if ( !this->IsFirstTime() && (send == ADD_ITEM_SEND_OK) )
				{
					sMuunSystem->GCMuunItemModifySend(this, slot);
				}
			}
		} break;
	}

	return slot;
}

uint8 Player::AddItem(Item const& item, uint8 slot, AddItemSend send)
{
	uint8 inventory = 0;
	return this->AddItem(item, inventory, slot, send);
}

bool Player::IsOffline() const
{
	if ( this->GetPersonalStore()->IsOff() )
	{
		return true;
	}

	if ( this->GetHelper()->IsOffline() )
	{
		return true;
	}

	return false;
}

bool Player::IsBuffAllowed(uint16 id) const
{
	return true;	
}

void Player::UpdateBuffID()
{
	this->UpdateBuffID(40, 101); // Asce
	this->UpdateBuffID(41, 102); // Wealth
	this->UpdateBuffID(44, 204); // Quick
	this->UpdateBuffID(45, 196); // Defense
	this->UpdateBuffID(46, 200); // Wrath
	this->UpdateBuffID(47, 197); // Wizardy
	this->UpdateBuffID(48, 198); // Health
	this->UpdateBuffID(49, 199); // Mana
	this->UpdateBuffID(89, 202); // Battle
	this->UpdateBuffID(90, 203); // Strength
	this->UpdateBuffID(121, 201); // Healing
}
	
void Player::UpdateBuffID(uint16 normal_id, uint16 master_id)
{
	BuffData const* pBuffData = this->GetBuff(master_id);

	if ( pBuffData )
	{
		buff_template const* buff_data = sSkillMgr->GetBuff(normal_id);

		if ( buff_data )
		{
			this->AddBuff(normal_id, 
				BuffEffect(buff_data->GetEffect(0), buff_data->GetValue(0)), 
				BuffEffect(buff_data->GetEffect(1), buff_data->GetValue(1)), 
				pBuffData->GetDuration(), pBuffData->GetFlag(), this);
		}
	}
}

void Player::UpdateCoordinateAttribute()
{
	if ( !sGameServer->IsAntiHackCoordinateVerify() )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( this->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	WorldGrid const& attr = this->GetGrid();

	if ( attr.attribute != this->GetCoordinateAttribute() )
	{
		this->SetCoordinateAttribute(attr.attribute);
		this->GetTimer(PLAYER_TIMER_COORDINATE_ATTRIBUTE)->Start();
	}
	else
	{
		if ( (this->GetCoordinateAttribute() & 4) == 4 || (this->GetCoordinateAttribute() & 8) == 8 )
		{
			if ( this->GetTimer(PLAYER_TIMER_COORDINATE_ATTRIBUTE)->Elapsed(sGameServer->GetAntiHackCoordinateVerifyTime()) )
			{
				this->TeleportToLocation(sGameServer->GetDefaultWorld(), sGameServer->GetDefaultX(), sGameServer->GetDefaultY(), -1, this->GetInstance());
			}
		}
	}
}

void Player::AddItemToSoldList(Item const& item, uint32 price)
{
	if ( item.GetExpireDate() > 0 )
	{
		return;
	}

	this->GetNpcSell()->AddItem(item, price);
}

void Player::ItemSoldListView()
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if (this->GetInterfaceState()->GetID() == InterfaceData::MixRecovery)
	{
		//this->SendCancelRecoveryMixItemList();
		return;
	}

	if ( !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(SOLD_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SOLD_ITEM_LIST_BODY, body, buffer, sizeof(SOLD_ITEM_LIST_HEAD));
	head->countH = 0;
	head->countL = 0;
	uint16 count = 0;

	time_t cur_time = time(nullptr);

	for ( int32 i = 0; i < MAX_ITEM_SOLD; ++i )
	{
		if ( !this->GetNpcSell()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( this->GetNpcSell()->GetItem(i)->IsExpired() )
		{
			continue;
		}

		body[count].time = this->GetNpcSell()->GetItem(i)->GetExpireDate() - cur_time;
		this->GetNpcSell()->GetItem(i)->ConvertToBuffer(body[count].item_info);
		body[count].quantity = 1;
		body[count].serial = this->GetNpcSell()->GetItem(i)->GetSerial();
		body[count].unk = i;
		++count;
	}

	head->h.set(0x6F, 0x00, sizeof(SOLD_ITEM_LIST_HEAD) + (sizeof(SOLD_ITEM_LIST_BODY) * count));
	head->countH = SET_NUMBERH(count);
	head->countL = SET_NUMBERL(count);

	this->sendPacket(buffer, head->h.get_size());
}

void Player::ItemSoldListClose()
{

}

void Player::ItemSoldRecover(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT(SOLD_ITEM_RECOVER, lpMsg, Packet, 0);

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if (this->GetInterfaceState()->GetID() == InterfaceData::MixRecovery)
	{
		//this->CancelRecoveryMixItem(lpMsg->serial);
		return;
	}

	if ( !sShopMgr->IsShop(this->GetInterfaceState()->GetID()) )
	{
		return;
	}

	uint8 cur_slot = MAX_ITEM_SOLD;

	for ( int32 i = 0; i < MAX_ITEM_SOLD; ++i )
	{
		if ( !this->GetNpcSell()->GetItem(i)->IsItem() )
		{
			continue;
		}

		if ( this->GetNpcSell()->GetItem(i)->GetSerial() == lpMsg->serial )
		{
			cur_slot = i;
			break;
		}
	}

	if ( cur_slot >= MAX_ITEM_SOLD )
	{
		return;
	}

	SOLD_ITEM_RECOVER_RESULT pMsg;
	pMsg.result = 0;

	if ( !this->MoneyHave(this->GetNpcSell()->GetItem(cur_slot)->GetPersonalStorePrice()) )
	{
		pMsg.result = 2;
		this->SEND_PCT(pMsg);
		return;
	}

	Item add_item = *this->GetNpcSell()->GetItem(cur_slot);
	add_item.SetExpireDate(0);
	add_item.SetPersonalStorePrice(0);

	uint8 result = this->AddItem(add_item, -1, ADD_ITEM_SEND_OK);

	if ( result == uint8(-1) )
	{
		pMsg.result = 1;
		this->SEND_PCT(pMsg);
		return;
	}

	this->MoneyReduce(this->GetNpcSell()->GetItem(cur_slot)->GetPersonalStorePrice());

	this->GetNpcSell()->DeleteItem(cur_slot);

	this->SEND_PCT(pMsg);

	this->ItemSoldListView();

	/*
	C2 00 07 6F 00 01 00 
	
	C3 13 BD 9E 09 BD BF 08 EF E2 DE D5 31 EB 3E 64 3B EE 01
	
	C1 05 6F 02 00 
	*/
}

float Player::GetWingAbsorb() const
{
	Item const* pItem = this->GetInventory()->GetItem(WINGS);

	if (!pItem || !pItem->IsItem() || !pItem->IsValid())
	{
		return 0;
	}

	float value = sItemMgr->GetWingAbsorb(pItem->GetItem(), pItem->GetLevel());

	if (value < 0)
	{
		value = 0;
	}

	if (value > 100)
	{
		value = 100;
	}

	return value;
}

float Player::GetWingDamage() const
{
	Item const* pItem = this->GetInventory()->GetItem(WINGS);

	if (!pItem || !pItem->IsItem() || !pItem->IsValid())
	{
		return 0;
	}

	float value = sItemMgr->GetWingDamage(pItem->GetItem(), pItem->GetLevel());

	if (value < 0)
	{
		value = 0;
	}

	if (value > 100)
	{
		value = 100;
	}

	return value;
}

float Player::GetMountAbsorb() const
{
	float value = 0;

	Item const* pFenrir = this->GetActiveInventoryItem(PET_FENRIR);
	Item const* pDinorant = this->GetActiveInventoryItem(PET_DINORANT);

	if (pDinorant && pDinorant->IsValid())
	{
		value += 10 + pDinorant->GetOptionValue(ItemOption::DINORANT_DEFENSE);
	}

	if (pFenrir && pFenrir->IsValid())
	{
		value += pFenrir->FenrirDecLastDamage();
	}

	Item const* pDarkHorse = this->GetActiveInventoryItem(PET_DARK_HORSE);

	if (pDarkHorse && pDarkHorse->IsValid())
	{
		value += (pDarkHorse->pet_dark.level + 30) / 2;
	}

	if (value < 0)
	{
		value = 0;
	}

	if (value > 100)
	{
		value = 100;
	}

	return value;
}

float Player::GetMountDamage() const
{
	float value = 0;

	Item const* pFenrir = this->GetActiveInventoryItem(PET_FENRIR);
	Item const* pDinorant = this->GetActiveInventoryItem(PET_DINORANT);

	if (pDinorant && pDinorant->IsValid())
	{
		value += 15;
	}

	if (pFenrir && pFenrir->IsValid())
	{
		value += pFenrir->FenrirIncLastDamage();
	}

	if (value < 0)
	{
		value = 0;
	}

	if (value > 100)
	{
		value = 100;
	}

	return value;
}

float Player::GetPetAbsorb() const
{
	Item const* pItem = this->GetInventory()->GetItem(GUARDIAN);

	if (!pItem || !pItem->IsValid())
	{
		return 0;
	}

	float value = 0;

	switch (pItem->GetItem())
	{
	case PET_GUARDIAN_ANGEL:
		{
			value = 20;
		} break;

	case PET_SPIRIT_OF_GUARDIAN:
		{
			value = 30;
		} break;
	}

	if (value < 0)
	{
		value = 0;
	}

	if (value > 100)
	{
		value = 100;
	}

	return value;
}

void Player::AddEventScore(uint8 type, int32 level, int32 score)
{
	if ( score <= 0 )
	{
		return;
	}

	switch ( type )
	{
	case 0:
		{
			this->UpdateStatistic(STATISTIC_PVE_SCORE, score);
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added +%d PVE Points", score);
		} break;

	case 1:
		{
			this->UpdateStatistic(STATISTIC_PVP_SCORE, score);
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added +%d PVP Points", score);
		} break;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_SCORE);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, type);
	stmt->setInt32(2, level);
	stmt->setInt32(3, score);
	stmt->setInt32(4, score);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void Player::AttackSpeedSend()
{
	ATTACK_SPEED_DATA pMsg6(this->GetIntData(UNIT_INT_ATTACK_SPEED), this->GetIntData(UNIT_INT_MAGIC_SPEED));
	this->SEND_PCT(pMsg6);
}

void Player::AttackSpeedRecv(uint8 * Packet)
{
	if ( !this->IsPlaying() )
	{
		return;
	}

	POINTER_PCT(ATTACK_SPEED_DATA, lpMsg, Packet, 0);

	uint8 attack_speed[4];
	uint8 magic_speed[4];

	memcpy(attack_speed, lpMsg->attack_speed, 4);
	memcpy(magic_speed, lpMsg->magic_speed, 4);

	attack_speed[0] -= 0x30;
	attack_speed[1] -= 0x31;
	attack_speed[2] -= 0x32;
	attack_speed[3] -= 0x33;

	magic_speed[0] -= 0x34;
	magic_speed[1] -= 0x35;
	magic_speed[2] -= 0x36;
	magic_speed[3] -= 0x37;

	uint32 a_speed = MAKE_NUMBERDW(MAKE_NUMBERW(attack_speed[0], attack_speed[1]), MAKE_NUMBERW(attack_speed[2], attack_speed[3]));
	uint32 m_speed = MAKE_NUMBERDW(MAKE_NUMBERW(magic_speed[0], magic_speed[1]), MAKE_NUMBERW(magic_speed[2], magic_speed[3]));

	if ( this->GetIntData(UNIT_INT_ATTACK_SPEED) != a_speed || this->GetIntData(UNIT_INT_MAGIC_SPEED) != m_speed )
	{
		sLog->outError("antihack", "%s -- %s -- [%u / %u] [%u / %u]", __FUNCTION__, this->BuildLog().c_str(),
			this->GetIntData(UNIT_INT_ATTACK_SPEED), a_speed,
			this->GetIntData(UNIT_INT_MAGIC_SPEED), m_speed);
	}
}

void Player::HackCheckVerify(uint8 * Packet, uint8 id)
{
	if ( !this->IsPlaying() )
	{
		return;
	}

	switch ( id )
	{
	case 0:
		{
		} break;

	case 1:
		{
			POINTER_PCT(HACK_CHECK_MODIFIED_BYTE_OFFSET, lpMsg, Packet, 0);

			sLog->outError("antihack", "HACK_CHECK_MODIFIED_BYTE_OFFSET(%x + %02X / %02X) -- %s", lpMsg->offset, lpMsg->move, lpMsg->value, this->BuildLog().c_str());

			this->IncreaseCounter(PLAYER_COUNTER_HACK_CHECK, 1);

			sGameServer->LogAntiHack(this, ANTIHACK_ID_BYTE_OFFSET, "HACK_CHECK_MODIFIED_BYTE_OFFSET(%x + %02X / %02X) -- Count: %d", lpMsg->offset, lpMsg->move, lpMsg->value, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
		} break;

	case 2:
		{
			POINTER_PCT(HACK_CHECK_MODIFIED_DOUBLE_OFFSET, lpMsg, Packet, 0);

			sLog->outError("antihack", "HACK_CHECK_MODIFIED_DOUBLE_OFFSET(%x / %f) -- %s", lpMsg->offset, lpMsg->value, this->BuildLog().c_str());

			this->IncreaseCounter(PLAYER_COUNTER_HACK_CHECK, 1);

			sGameServer->LogAntiHack(this, ANTIHACK_ID_DOUBLE_OFFSET, "HACK_CHECK_MODIFIED_DOUBLE_OFFSET(%x / %f) -- Count: %d", lpMsg->offset, lpMsg->value, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
		} break;

	case 3:
		{
			POINTER_PCT(HACK_CHECK_HOOKED_FUNCTION, lpMsg, Packet, 0);

			sLog->outError("antihack", "HACK_CHECK_HOOKED_FUNCTION(%u / %02X) -- %s", lpMsg->id, lpMsg->data, this->BuildLog().c_str());

			this->IncreaseCounter(PLAYER_COUNTER_HACK_CHECK, 1);

			//if ( !this->IsWhiteList() )
			//{
				sGameServer->LogAntiHack(this, ANTIHACK_ID_HOOKED_FUNCTION, "HACK_CHECK_HOOKED_FUNCTION(%u / %02X) -- Count: %d", lpMsg->id, lpMsg->data, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
			//}
		} break;

	case 4:
		{
			POINTER_PCT(HACK_CHECK_FRAME_COUNT, lpMsg, Packet, 0);

			if ( lpMsg->count >= sGameServer->GetHackCheckFrameCount() )
			{
				sLog->outError("antihack", "HACK_CHECK_FRAME_COUNT(%d) -- %s", lpMsg->count, this->BuildLog().c_str());

				this->IncreaseCounter(PLAYER_COUNTER_HACK_CHECK, 1);

				sGameServer->LogAntiHack(this, ANTIHACK_ID_FRAME_COUNT, "HACK_CHECK_FRAME_COUNT(%d) -- Count: %d", lpMsg->count, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
			}
		} break;

	case 5:
		{
		} break;
	}
}

void Player::HackCheckVerify()
{
	if ( !sGameServer->IsHackCheckEnabled() )
	{
		return;
	}

	if ( this->GetHelper()->IsStarted() && this->GetHelper()->IsOffline() )
	{
		return;
	}

	if ( this->GetPersonalStore()->IsOpen() && this->GetPersonalStore()->IsOff() )
	{
		return;
	}

	if ( sGameServer->GetHackCheckResetTime() > 0 && this->GetTimer(PLAYER_TIMER_HACK_CHECK_INIT)->Elapsed(sGameServer->GetHackCheckResetTime()) )
	{
		this->SetCounter(PLAYER_COUNTER_HACK_CHECK, 0);
	}

	/*if ( this->IsWhiteList() )
	{
		return;
	}*/

	if ( (sGameServer->GetHackCheckCount() > 0) && (this->GetCounter(PLAYER_COUNTER_HACK_CHECK) > sGameServer->GetHackCheckCount()) )
	{
		if ( sGameServer->IsHackCheckBan() )
		{
			this->SetAuthority(1);
		}

		if ( sGameServer->IsHackCheckFullBan() )
		{
			sAuthServer->AccountCompleteBan(this->GetAccountData()->GetIP(), this->GetAccountData()->GetMac(), this->GetAccountData()->GetDiskSerial(), 7);
		}

		if ( sGameServer->IsHackCheckKick() )
		{
			KICK_PLAYER(this, "Hack Check Verify");

			sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Hack Check Verify");
		}
	}
}

bool Player::IsSkillCooldown(uint16 skill)
{
	auto cooldown = this->GetSkillCooldown(skill);

	auto itr = this->skill_cooldown.find(skill);

	if (itr == this->skill_cooldown.end())
	{
		this->skill_cooldown[skill].Reset();
		return true;
	}
	else
	{
		return itr->second.Elapsed(cooldown);
	}
}

bool Player::CheckSkillCooldown(uint16 skill)
{
	const auto cooldown = this->GetSkillCooldown(skill);

	if (cooldown <= 0)
	{
		return true;
	}

	auto itr = this->skill_cooldown.find(skill);

	if (itr != this->skill_cooldown.end())
	{
		return itr->second.GetElapsed() >= cooldown;
	}

	return true;
}

uint32 Player::GetSkillCooldown(uint16 skill)
{
	const auto skill_info = sSkillMgr->GetSkill(skill);

	if (!skill_info)
	{
		return 0;
	}

	uint32 cooldown = skill_info->GetCooldown();
	uint32 reduce_cooldown = 0;

	switch (skill_info->GetBaseSkill())
	{
	case SKILL_WRATH:
	{
		reduce_cooldown = this->MagicGetValue(709) * IN_MILLISECONDS;
	} break;

	case SKILL_BURST:
	{
		reduce_cooldown = this->GetMagicTreeLevel(701) * IN_MILLISECONDS;
	} break;

	case SKILL_BURST_RW:
	{
		reduce_cooldown = this->GetMagicTreeLevel(766) * IN_MILLISECONDS;
	} break;

	case SKILL_HASTE_RW:
	{
		reduce_cooldown = this->GetMagicTreeLevel(769) * IN_MILLISECONDS;
	} break;

	case SKILL_DEMOLISH:
	{
		reduce_cooldown = this->GetMagicTreeLevel(787) * IN_MILLISECONDS;
	} break;
	}

	if (reduce_cooldown > cooldown)
	{
		cooldown = 0;
	}
	else
	{
		cooldown -= reduce_cooldown;
	}

	if (!this->GetHelper()->IsOffline())
	{
		if (cooldown > 1000)
		{
			cooldown -= 1000;
		}
		else
		{
			cooldown = 0;
		}
	}

	return cooldown;
}

void Player::SendExperienceEvent()
{
	EXPERIENCE_EVENT pMsg;
	pMsg.experience = 0;
	
	if ( sHappyHour->GetState() == HAPPY_HOUR_STATE_START )
	{
		pMsg.experience += sGameServer->happy_hour_experience_add.get();
	}

	pMsg.experience += sCharacterBase->GetBonus(this);
	
	this->SEND_PCT(pMsg);
}

void Player::SendMessageBox(uint8 type, const char * caption, const char * message, ...)
{
	ARG(buffer, message);

	MESSAGE_BOX_SEND pMsg;
	strcpy(pMsg.caption, caption);
	memcpy(pMsg.message, buffer, 150);
	pMsg.type = type;

	this->SEND_PCT(pMsg);
}

bool Player::InmuneToRadiance() const
{
	if ( this->IsDebuffInmune(true) )
	{
		return true;
	}

	return false;
}

bool Player::InmuneToPunish(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return false;
	}

	if ( !sGameServer->GetSkillPunishFrequencyPVP() )
	{
		return false;
	}

	UnitPunishMap::const_iterator itr = this->punish_map.find(pPlayer->GetGUID());

	if ( itr == this->punish_map.end() )
	{
		this->punish_map[pPlayer->GetGUID()] = MyGetTickCount();
		return false;
	}

	return true;
}

void Player::ApplyRadianceDebuff(Unit* pTarget, uint16 effect, int32 & punish_damage, int32 count)
{
	if (!pTarget)
	{
		return;
	}

	if (pTarget->InmuneToRadiance())
	{
		return;
	}

	Player* pVictim = pTarget->ToPlayer();
	Monster* pMonster = pTarget->ToCreature();

	if (count <= 1)
	{
		if ((this->GetPentagramJewelOption()->GetElementalBleeding() > 0) && roll_chance_i(sGameServer->GetRadianceBleedingRate()) && !pTarget->HasBuff(BUFF_HEMORRAGE))
		{
			pTarget->AddBuff(BUFF_HEMORRAGE, BuffEffect(BUFF_OPTION_NONE, this->GetPentagramJewelOption()->GetElementalBleeding()), BuffEffect(BUFF_OPTION_NONE, this->GetElementalAttribute()), sGameServer->GetRadianceBleedingDuration(), 0, this);
			return;
		}

		if (pVictim && (this->GetPentagramJewelOption()->GetElementalParalysisRate() > 0) && roll_chance_i(sGameServer->GetRadianceParalysisRate()) && !pTarget->HasBuff(BUFF_PARALYSIS))
		{
			pTarget->AddBuff(BUFF_PARALYSIS, this->GetPentagramJewelOption()->GetElementalParalysisRate(), 0, this);
			return;
		}

		if ((this->GetPentagramJewelOption()->GetElementalBindingRate() > 0) && roll_chance_i(this->GetPentagramJewelOption()->GetElementalBindingRate()) && !pTarget->HasBuff(BUFF_BINDING))
		{
			pTarget->AddBuff(BUFF_BINDING, sGameServer->GetRadianceBindingDuration(), 0, this);
			return;
		}

		if ((this->GetPentagramJewelOption()->GetElementalBlindness() > 0) && roll_chance_i(sGameServer->GetRadianceBlindingRate()) && !pTarget->HasBuff(BUFF_BLINDNESS))
		{
			pTarget->AddBuff(BUFF_BLINDNESS, BuffEffect(BUFF_OPTION_NONE, this->GetPentagramJewelOption()->GetElementalBlindness()), sGameServer->GetRadianceBlindingDuration(), 0, this);
			return;
		}
	}

	if (!sGameServer->IsSkillPunishFirstHit() || count <= 1)
	{
		if ((this->GetPentagramJewelOption()->GetElementalPunishRate() > 0) && ((pMonster && roll_chance_i(sGameServer->GetSkillPunishRatePVM())) || (pVictim && roll_chance_i(sGameServer->GetSkillPunishRatePVP()))) && (effect == ELEMENTAL_DAMAGE_TYPE_CRITICAL) && !pTarget->InmuneToPunish(this))
		{
			punish_damage = pTarget->PowerGetTotal(POWER_LIFE) * this->GetPentagramJewelOption()->GetElementalPunishRate() / 100;

			if (punish_damage > sGameServer->GetSkillPunishMaxDamage())
			{
				punish_damage = sGameServer->GetSkillPunishMaxDamage();
			}

			//pTarget->PowerReduce(POWER_LIFE, punish_damage, true);
			this->DealDamage(pTarget, punish_damage, 0, 0, false, MONSTER_THREAT_DAMAGE_NORMAL, 1);
			return;
		}
	}
}

void Player::CreateDarkAncientItem(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (!Player::IsPlayerTransactionFree(this))
	{
		return;
	}

	POINTER_PCT(DARK_ANCIENT_ITEM_CRAFT, lpMsg, Packet, 0);

	if (!use_inventory_range(lpMsg->slot))
	{
		return;
	}

	Item const* pItem = this->GetInventory()->GetItem(lpMsg->slot);

	if (!pItem)
	{
		return;
	}

	if (lpMsg->h.subcode == 0)
	{
		DARK_ANCIENT_ITEM_CREATE pMsg;

		item_mix_lucky_data const* pData = sItemMgr->GetRandomItemMixLucky(pItem->GetItem(), pItem->GetLevel(), this);

		if (!pData)
		{
			pMsg.result = 4;
			this->SEND_PCT(pMsg);
			return;
		}

		if (!this->GetInventory()->IsEmptySpace(pData->GetItem()))
		{
			pMsg.result = 1;
			this->SEND_PCT(pMsg);
			return;
		}

		this->ClearItem(lpMsg->slot);

		uint8 ancient = sItemMgr->GetRandomOptionAncient(pData->GetItem());

		if (!IS_ANCIENT(ancient))
		{
			ancient = ANCIENT_TYPE_1 | ANCIENT_OPTION_1;
		}

		sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(pData->GetItem(), Random(MAX_ITEM_LEVEL + 1), 0.0f, 0, 0, 0, 0, ancient), -1, 1);
	}
	else if (lpMsg->h.subcode == 1)
	{
		DARK_ANCIENT_ITEM_DESTROY pMsg;

		if (!this->GetInventory()->IsEmptySpace(1, 1))
		{
			pMsg.result = 1;
			this->SEND_PCT(pMsg);
			return;
		}

		int32 success_rate = pItem->GetDurability() == 255.0f ? 60 : 10;

		this->ClearItem(lpMsg->slot);

		if (roll_chance_i(success_rate))
		{
			sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(JEWEL::EXTENSION));
		}

		this->SEND_PCT(pMsg);
	}
}

uint8 Player::GetMatchingClass() const
{
	uint8 db_class = this->GetClass();
	if ( this->GetChangeUP(0) )
	{
		db_class |= 16;
	}
	if ( this->GetChangeUP(1) )
	{
		db_class |= 32;
	}

	if ( this->GetChangeUP(2) )
	{
		db_class |= 64;
	}

	return db_class;
}

uint8 Player::ItemFind(uint16 item, uint8 level, bool consume)
{
	item_template const* item_info = sItemMgr->GetItem(item);

	if ( !item_info )
	{
		return uint8(-1);
	}

	switch ( item_info->GetInventoryType() )
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
		{
			use_inventory_loop(i)
			{
				Item const* pItem = this->GetInventory()->GetItem(i);

				if ( !pItem )
				{
					continue;
				}

				if ( !pItem->IsItem() )
				{
					continue;
				}

				if ( pItem->GetItem() == item && pItem->GetLevel() == level )
				{
					if ( consume )
					{
						if ( item_info->GetStackData()->GetMaxStack() > 0 )
						{
							this->DecreaseItemDurabilityByUse(i, 1.0f);
						}
						else
						{
							this->ClearItem(i);
						}
					}

					return i;
				}
			}
		} break;

	case ITEM_INVENTORY_TYPE_EVENT:
		{
			EVENT_INVENTORY_LOOP(i)
			{
				Item const* pItem = this->GetEventInventory()->GetItem(i);

				if ( !pItem )
				{
					continue;
				}

				if ( !pItem->IsItem() )
				{
					continue;
				}

				if ( pItem->GetItem() == item && pItem->GetLevel() == level )
				{
					if ( consume )
					{
						if ( item_info->GetStackData()->GetMaxStack() > 0 )
						{
							sEventInventory->DecreaseItemDurabilityByUse(this, i, 1.0f);
						}
						else
						{
							this->GetEventInventory()->DeleteItem(i);
							sEventInventory->GCEventItemDeleteSend(this, i);
						}
					}

					return i;
				}
			}
		} break;
	}
	
	return uint8(-1);
}

uint8 Player::FindExpirableItem(uint16 item, uint8 level) const
{
	item_template const* item_info = sItemMgr->GetItem(item);

	if (!item_info)
	{
		return uint8(-1);
	}

	switch (item_info->GetInventoryType())
	{
	case ITEM_INVENTORY_TYPE_NORMAL:
		{
			wear_inventory_loop(i)
			{
				if (this->GetInventory()->IsExpirableItem(i, item, level))
				{
					return i;
				}
			}

			if (this->GetInventory()->IsExpirableItem(EARRING_01, item, level))
			{
				return EARRING_01;
			}

			if (this->GetInventory()->IsExpirableItem(EARRING_02, item, level))
			{
				return EARRING_02;
			}

			use_inventory_loop(i)
			{
				if (this->GetInventory()->IsExpirableItem(i, item, level))
				{
					return i;
				}
			}
		} break;

	case ITEM_INVENTORY_TYPE_EVENT:
		{
			EVENT_INVENTORY_LOOP(i)
			{
				Item const* pItem = this->GetEventInventory()->GetItem(i);

				if (!pItem)
				{
					continue;
				}

				if (!pItem->IsItem())
				{
					continue;
				}

				if (!pItem->GetExpireDate())
				{
					continue;
				}

				if (pItem->GetItem() == item && pItem->GetLevel() == level)
				{
					return i;
				}
			}
		} break;
	}

	return uint8(-1);
}

uint8 Player::GetEventEnterCount(uint8 event_id) const
{
	PlayerEventEnterCountMap::const_iterator itr = this->event_enter_count_map.find(event_id);

	if ( itr != this->event_enter_count_map.end() )
	{
		return itr->second->GetCount();
	}
	else
	{
		return 0;
	}
}

void Player::UpdateEventEnterCount()
{
	Custom::SystemTimer m_system_time = Custom::SystemTimer();
	bool update = false;

	for ( PlayerEventEnterCountMap::iterator itr = this->event_enter_count_map.begin(); itr != this->event_enter_count_map.end(); )
	{
		PlayerEventEnterCount * pData = itr->second;

		if ( !pData )
		{
			update = true;
			this->event_enter_count_map.erase(itr++);
			continue;
		}

		if ( pData->GetDay() != m_system_time.GetDay() )
		{
			update = true;
			delete itr->second;
			this->event_enter_count_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

	if ( update )
	{
		this->SendEventEnterCount();

		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Event enter count has been reseted.");
	}
}

int32 Player::GetDarkHorseAbsorb() const
{
	Item const* pItem = this->GetActiveInventoryItem(PET_DARK_HORSE);

	if (!pItem)
	{
		return 0;
	}

	if (!pItem->IsValid())
	{
		return 0;
	}

	return ((pItem->pet_dark.level + 30) / 2);
}

void Player::WarpFavoriteListSave(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false, false) )
	{
		return;
	}

	POINTER_PCT_LOG(WARP_FAVORITE_LIST, lpMsg, Packet, 0);

	for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
	{
		this->GetWarpFavoriteList(i)->SetData(lpMsg->data[i].data);
		this->GetWarpFavoriteList(i)->SetWarpID(lpMsg->data[i].warp_id);
	}
}
	
void Player::WarpFavoriteListSend()
{
	WARP_FAVORITE_LIST pMsg;

	for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
	{
		pMsg.data[i].data = this->GetWarpFavoriteList(i)->GetData();
		pMsg.data[i].warp_id = this->GetWarpFavoriteList(i)->GetWarpID();

		if ( this->GetWarpFavoriteList(i)->GetData() != uint16(-1) )
		{
			pMsg.count++;
		}
	}

	this->SEND_PCT(pMsg);
}

void Player::CentQuestStartRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !Player::IsPlayerTransactionFree(this) )
	{
		return;
	}

	if ( this->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_SAFE )
	{
		return;
	}

	if ( this->QuestEvolutionGetState(6) != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		return;
	}

	POINTER_PCT_LOG(CENT_QUEST_START, lpMsg, Packet, 0);

	int32 instance = this->QuestEvolutionGetInstance();

	if ( instance != -1 )
	{
		Quest4thInstanceData * pData = sQuestMgr->GetQuest4thInstanceData(instance);

		if ( !pData )
		{
			return;
		}

		if ( !this->MoveToGate(540) )
		{
			return;
		}

		this->SetInstance(instance);

		if ( pData->GetQuestID() == 8 &&  pData->GetState() == 3 )
		{
			QUEST_SURVIVAL_TIMER pMsg;
			pMsg.time = pData->GetTime()->GetRemain();
			pMsg.type = 5;
			pMsg.increase = 0;

			this->SEND_PCT(pMsg);
		}
	}
	else
	{
		Quest4thInstanceData * pData = sQuestMgr->Get4thQuestInstance();

		if ( !pData )
		{
			return;
		}

		pData->SetPartyID(this->GetPartyID());
		sQuestMgr->Change4thQuestInstanceState(pData->GetInstance(), 0, 0);
		
		Unit* pMember[MAX_PARTY_MEMBERS];
		uint8 count = 0;

		this->GetPartyMembers(pMember, count, 15);

		if ( count > MAX_PARTY_MEMBERS )
		{
			count = 1;
		}

		for ( uint8 i = 0; i < count; ++i )
		{
			if ( !pMember[i] )
			{
				continue;
			}

			Player* pPlayer = pMember[i]->ToPlayer();

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetWorldId() != WORLD_LABYRINTH_OF_DIMENSIONS_SAFE )
			{
				return;
			}

			if ( pPlayer->QuestEvolutionGetState(6) != QUEST_EVOLUTION_STATE_COMPLETE )
			{
				continue;
			}

			if ( pPlayer->MoveToGate(540) )
			{
				pPlayer->SetInstance(pData->GetInstance());
			}
		}
	}
}

void Player::CentQuestMoveRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !Player::IsPlayerTransactionFree(this) )
	{
		return;
	}

	if ( this->QuestEvolutionGetState(6) != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		return;
	}

	this->MoveToGate(537);
}

void Player::CentQuestSpawnMonster(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( this->QuestEvolutionGetState(6) != QUEST_EVOLUTION_STATE_COMPLETE )
	{
		return;
	}

	if ( this->GetWorldId() != WORLD_PLACE_OF_QUALIFICATION )
	{
		return;
	}

	Quest4thInstanceData const* pData = sQuestMgr->GetQuest4thInstanceData(this->GetInstance());

	if ( !pData )
	{
		return;
	}

	if ( pData->GetState() != 1 )
	{
		return;
	}
	
	uint8 quest_id = this->QuestEvolutionGetState(7) == QUEST_EVOLUTION_STATE_ACCEPTED ? 7 :
					 this->QuestEvolutionGetState(8) == QUEST_EVOLUTION_STATE_ACCEPTED ? 8 :
					 this->QuestEvolutionGetState(9) == QUEST_EVOLUTION_STATE_ACCEPTED ? 9 :
					 0;

	if ( quest_id == 0 )
	{
		return;
	}

	sQuestMgr->Change4thQuestInstanceState(pData->GetInstance(), 2, quest_id);
}

void Player::TrackingUpdate()
{
	if ( !this->IsAdministrator() )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		return;
	}

	if ( !this->GetTrackingID() )
	{
		return;
	}

	if ( this->GetRegenStatus() == REGEN_STANDBY )
	{
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayerByGUID(this->GetTrackingID());

	if ( !pPlayer )
	{
		this->SetTrackingID(0);

		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Tracking player disconnected");
		return;
	}

	if ( pPlayer->IsServerQuit() )
	{
		this->SetTrackingID(0);

		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Tracking player moved to server %u", pPlayer->GetDestServer());
		return;
	}

	if ( this->GetWorldId() != pPlayer->GetWorldId() )
	{
		this->TeleportToObject(pPlayer);
	}
	else if ( this->GetX() != pPlayer->GetX() || this->GetY() != pPlayer->GetY() )
	{
		this->PositionSend(pPlayer->GetX(), pPlayer->GetY());
	}
}

void Player::UpdateMapState()
{
	if ( this->GetRegenStatus() != REGEN_NONE )
	{
		return;
	}

	if ( this->IsSentMapState() )
	{
		return;
	}

	World const* pWorld = this->GetWorld();

	if ( pWorld )
	{
		for ( WorldAreaAttributeList::const_iterator itr = pWorld->m_attribute.begin(); itr != pWorld->m_attribute.end(); ++itr )
		{
			WorldAreaAttribute const* pData = *itr;

			if ( !pData )
			{
				continue;
			}

			if ( pData->IsSend() )
			{
				int16 coord[4] = { pData->GetX1(), pData->GetY1(), pData->GetX2(), pData->GetY2() };
				this->SendWorldAttribute(0, pData->GetAttribute(), 1, pData->IsApply(), coord);
			}
		}
	}
	
	this->SetSentMapState(true);
}

void Player::CancelMovement()
{
	this->SetRest(1);
	this->ClearPathData(true);

	ACTION_RESULT pAction(this->GetEntry(), 0, this->GetDirection(), 0x7A);
	this->sendPacket((uint8*)&pAction, pAction.h.size);
}

void Player::ItemSplit(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this))
	{
		return;
	}

	if (!Player::IsPlayerTransactionFree(this))
	{
		return;
	}

	POINTER_PCT_LOG(ITEM_SPLIT_REQUEST, lpMsg, Packet, 0);

	ITEM_SPLIT_RESULT pMsg;

	if (!sGameServer->IsItemSplitEnabled())
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if (lpMsg->amount <= 0)
	{
		this->SEND_PCT(pMsg);
		return;
	}

	Item * pItem = nullptr;

	switch (lpMsg->type)
	{
	case 0:
		{
			if (!use_inventory_range(lpMsg->slot))
			{
				this->SEND_PCT(pMsg);
				return;
			}

			pItem = this->GetInventory()->GetItem(lpMsg->slot);
		} break;

	case 1:
		{
			if (!EVENT_INVENTORY_RANGE(lpMsg->slot))
			{
				this->SEND_PCT(pMsg);
				return;
			}

			pItem = this->GetEventInventory()->GetItem(lpMsg->slot);
		} break;

	default:
		{
			this->SEND_PCT(pMsg);
			return;
		} break;
	}

	if (!pItem)
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if (!pItem->IsItem())
	{
		this->SEND_PCT(pMsg);
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

	if (!item_info)
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if (!item_info->GetStackData()->GetMaxStack())
	{
		this->SEND_PCT(pMsg);
		return;
	}

	if (static_cast<uint8>(pItem->GetDurability()) <= lpMsg->amount)
	{
		this->SEND_PCT(pMsg);
		return;
	}

	switch (lpMsg->type)
	{
	case 0:
		{
			if (!this->GetInventory()->IsEmptySpace(item_info->GetX(), item_info->GetY()))
			{
				pMsg.result = 6;
				this->SEND_PCT(pMsg);
				return;
			}

			//this->SEND_PCT(pMsg);

			pItem->AlterDurability(0, lpMsg->amount);
			this->SendItemDurability(lpMsg->slot, 0);
		} break;

	case 1:
		{
			if (!this->GetEventInventory()->IsEmptySpace(item_info->GetX(), item_info->GetY()))
			{
				pMsg.result = 6;
				this->SEND_PCT(pMsg);
				return;
			}

			//this->SEND_PCT(pMsg);

			pItem->AlterDurability(0, lpMsg->amount);
			sEventInventory->GCEventItemDurSend(this, lpMsg->slot, static_cast<uint8>(pItem->GetDurability()));
		} break;
	}
		
	sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(pItem->GetItem(), pItem->GetLevel(), lpMsg->amount), -1, 3);
}

bool Player::FillMagicSolution(uint8 source_slot, uint8 target_slot)
{
	if (!use_inventory_range(source_slot) || !use_inventory_range(target_slot))
	{
		return false;
	}

	Item * pSourceItem = this->GetInventory()->GetItem(source_slot);
	Item * pTargetItem = this->GetInventory()->GetItem(target_slot);

	if (!pSourceItem || !pSourceItem->IsItem())
	{
		return false;
	}

	if (!pTargetItem || !pTargetItem->IsItem())
	{
		return false;
	}

	uint16 create_item = -1;

	switch (pSourceItem->GetItem())
	{
	case ITEMGET(12, 446):
		{
			if (pTargetItem->GetItem() != ITEMGET(12, 407))
			{
				return false;
			}

			create_item = ITEMGET(12, 408);
		} break;

	case ITEMGET(12, 447):
		{
			if (pTargetItem->GetItem() != ITEMGET(12, 409))
			{
				return false;
			}

			create_item = ITEMGET(12, 410);
		} break;

	case ITEMGET(12, 448):
		{
			if (pTargetItem->GetItem() != ITEMGET(12, 411))
			{
				return false;
			}

			create_item = ITEMGET(12, 412);
		} break;
	}

	if (create_item == uint16(-1))
	{
		return false;
	}

	if (pTargetItem->GetDurability() >= 255.0f)
	{
		this->ClearItem(target_slot, 1);
		sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(create_item));
		return false;
	}

	int32 AddDur = (int32)(255.0f - pTargetItem->GetDurability());
	AddDur = ((AddDur > pSourceItem->GetDurability()) ? (int32)pSourceItem->GetDurability() : AddDur);

	pSourceItem->AlterDurability(0, AddDur);
	pTargetItem->AlterDurability(1, AddDur);

	if (pTargetItem->GetDurability() >= 255.0f)
	{
		this->ClearItem(target_slot, 1);
		sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(create_item));
	}
	else
	{
		this->SendItemDurability(target_slot, 0);
	}

	if (pSourceItem->GetDurability() < 1.0f)
	{
		this->ItemUseResult();
		this->SendInventory(source_slot);
		this->ClearItem(source_slot);
		return true;
	}
	else
	{
		this->SendItemDurability(source_slot, 1);
		this->SendInventory(source_slot);
	}
	
	return false;
}

void Player::PlayerRightClick(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	this->SetCurrentTargetPlayer(-1);

	POINTER_PCT_LOG(PLAYER_RIGHT_CLICK, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->index);

	if (!pPlayer)
	{
		sLog->outError("antihack", "%s(Wrong index :%u) - %s", __FUNCTION__, lpMsg->index, this->BuildLog().c_str());
		return;
	}

	if (pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING)
	{
		sLog->outError("antihack", "%s(Index not playing :%u) - %s", __FUNCTION__, lpMsg->index, this->BuildLog().c_str());
		return;
	}

	this->SetCurrentTargetPlayer(lpMsg->index);

	HUNTING_RECORD_SET_VISIBLE pMsg(pPlayer->IsHuntingRecordVisible() ? 1 : 0);
	this->SendPacket(&pMsg);
}

void Player::ClientMinimize(uint8 * Packet)
{
	if (!this->IsPlaying())
	{
		return;
	}

	POINTER_PCT_LOG(CLIENT_MINIMIZE, lpMsg, Packet, 0);

	this->SetClientMinimized(lpMsg->minimize ? true : false);
}

void Player::WingSocketOptionAdd(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (this->GetPersonalStore()->IsBusy())
	{
		return;
	}

	if (!Player::InterfaceCheck(this, InterfaceData::Adniel, 0))
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(WING_SOCKET_OPTION_ADD, lpMsg, Packet, 0);

	MixHandler(this, 5).WingSocketMix(0, lpMsg->slot, 15);
}

void Player::WingSocketOptionAddResult(uint8 result)
{
	WING_SOCKET_OPTION_ADD_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}

void Player::WingSocketOptionUpgrade(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (this->GetPersonalStore()->IsBusy())
	{
		return;
	}

	if (!Player::InterfaceCheck(this, InterfaceData::Adniel, 0))
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(WING_SOCKET_OPTION_UPGRADE, lpMsg, Packet, 0);

	MixHandler(this, 5).WingSocketMix(1, lpMsg->slot, 11);
}

void Player::WingSocketOptionUpgradeResult(uint8 result, uint8 slot)
{
	WING_SOCKET_OPTION_UPGRADE_RESULT pMsg(result);
	pMsg.slot = slot;

	this->SEND_PCT(pMsg);
}

void Player::CheckSkill(int32 count)
{
	if (!sGameServer->IsSkillCheckEnabled())
	{
		return;
	}

	std::set<uint16> skill;
	bool update = false;

	SKILL_LOOP(i)
	{
		Skill* pSkill = this->GetSkill(i);

		if (!pSkill)
		{
			continue;
		}
		///- Reviso si es un skill del Tree Normal
		if (sSkillMgr->GetSkillTree(pSkill->GetSkill(), this->GetClass()))
		{
			Skill* pTreeSkill = this->GetSkillTreeSkill(pSkill->GetSkill());

			if (!pTreeSkill) ///- Si no existe en mi lista del Tree Normal entonces lo borro
			{
				uint16 base = sSkillMgr->GetSkillBaseSkill(pSkill->GetSkill());

				if (base != pSkill->GetSkill())
				{
					skill.insert(base);
				}

				pSkill->Clear();
				update = true;
			}
			else
			{
				pSkill->SetLevel(pTreeSkill->GetLevel());
			}
		}
		///- Reviso si es un skill del Tree Majestic
		else if (sSkillMgr->GetSkillTreeMajestic(this->GetClass(), pSkill->GetSkill())) ///- Si no existe en mi lista del Tree Majestic entonces lo borro
		{
			MajesticSkill* pTreeSkill = this->GetMajesticSkillTreeSkill(pSkill->GetSkill());

			if (!pTreeSkill)
			{
				uint16 base = sSkillMgr->GetSkillBaseSkill(pSkill->GetSkill());

				if (base != pSkill->GetSkill())
				{
					skill.insert(base);
				}

				pSkill->Clear();
				update = true;
			}
			else
			{
				pSkill->SetLevel(pTreeSkill->GetLevel());
			}
		}
	}

	for (std::set<uint16>::const_iterator itr = skill.begin(); itr != skill.end(); ++itr)
	{
		if (this->MagicGetByBase(*itr))
		{
			continue;
		}

		int32 result = this->MagicAdd(*itr, 0);

		if (result != -1)
		{
			//this->SendSkill(result, *itr, 0);
		}
	}

	if (update && count <= 3)
	{
		this->CheckSkill(count + 1);
	}
}

float Player::GetAbsorbDamageRate() const
{
	float value = 100 * (1 - (this->GetMountAbsorb() / 100.0f)) * (1 - (this->GetWingAbsorb() / 100.0f)) * (1 - (this->GetEffect(BUFF_OPTION_MANA_SHIELD) / 100.0f)) * (1 - (this->GetPetAbsorb() / 100.0f));

	float result = 100 - ceil(value);

	if (result < 0)
	{
		result = 0;
	}

	if (result >= 100)
	{
		result = 99;
	}

	return result;
}

float Player::GetIncreaseDamageRate() const
{
	float value = 100 * (1 - (this->GetMountDamage() / 100.0f)) * (1 - (this->GetWingDamage() / 100.0f));

	float result = 100 - ceil(value);

	if (result < 0)
	{
		result = 0;
	}

	if (result >= 100)
	{
		result = 99;
	}

	return result;
}

void Player::ReduceAttackHP(uint16 skill)
{
	Item const* pWings = this->GetInventory()->GetItem(WINGS);

	if (pWings && pWings->IsItem())
	{
		if (!pWings->IsMiniWing()) ///- Solamente reduzco hp cuando usan alas normales
		{
			switch (this->GetClass())
			{
			case Character::DARK_WIZARD:
			case Character::FAIRY_ELF:
			case Character::SUMMONER:
				{
					this->PowerReduce(POWER_LIFE, 1, true);
				} break;

			case Character::RAGE_FIGHTER:
				{
					if (skill == 260 || skill == 261 || skill == 270 || skill == 264)
					{
						this->PowerReduce(POWER_LIFE, 2, true);
					}
					else if (skill == 262 || skill == 269 || skill == 263)
					{
						this->PowerReduce(POWER_LIFE, 4, true);
					}
					else if (skill == 265)
					{
						this->PowerReduce(POWER_LIFE, 100, true);
					}
					else
					{
						this->PowerReduce(POWER_LIFE, 3, true);
					}
				} break;

			default:
				{
					this->PowerReduce(POWER_LIFE, 3, true);
				} break;
			}
		}
	}

	if (this->IsActiveInventoryItem(PET_DINORANT))
	{
		this->PowerReduce(POWER_LIFE, 1, true);
	}

	Item const* pHelper = this->GetInventory()->GetItem(GUARDIAN);

	if (pHelper && pHelper->IsItem())
	{
		switch (pHelper->GetItem())
		{
		case PET_SATAN:
			{
				this->PowerReduce(POWER_LIFE, 3, true);
			} break;
		}
	}
}

bool Player::IsItemEquipped() const
{
	wear_inventory_loop(i)
	{
		Item const* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		return true;
	}

	if (this->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsItem())
	{
		return true;
	}

	if (this->GetInventory()->GetItem(EARRING_01)->IsItem())
	{
		return true;
	}

	if (this->GetInventory()->GetItem(EARRING_02)->IsItem())
	{
		return true;
	}

	return false;
}

void Player::SendPCRoom()
{
	PACKET_PC_ROOM_OPEN pMsg;

	this->SEND_PCT(pMsg);
}

void Player::EarringUpgrade(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (this->GetPersonalStore()->IsBusy())
	{
		return;
	}

	/*if (!Player::InterfaceCheck(this, InterfaceData::ChaosMachine, 0))
	{
		return;
	}*/

	if (!this->IsAuthorizationEnabled())
	{
		this->SendMessageBox(0, "Unauthorized access", "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_CHECK(EARRING_UPGRADE, lpMsg, Packet, 0);

	MixHandler(this, 6).EarringMix(0, lpMsg->slot, MAX_EARRING_UPGRADE_SLOTS);
}

void Player::EarringUpgradeResult(uint8 result)
{
	EARRING_UPGRADE_RESULT pMsg;
	pMsg.result = result;

	this->SendPacket(&pMsg);
}

void Player::SendServerList()
{
	/*if (!Player::IsPlayerBasicState(this, false, false))
	{
		sLog->outInfo("root", "SendServerList(2)");
		return;
	}*/

	uint8 buffer[4092];
	POINTER_PCT(PACKET_CHANNEL_HEAD, head, buffer, 0);
	POINTER_PCT(PACKET_CHANNEL_BODY, body, buffer, sizeof(PACKET_CHANNEL_HEAD));
	head->count = 0;
	head->data = 0;

	if (/*sGameServer->GetServerType() == SERVER_TYPE_NORMAL && sGameServer->IsFlag(SERVER_FLAG_DISPLAY) && */sGameServer->IsChannelChangeEnabled())
	{
		for (ChannelDataMap::const_iterator itr = sConnectServer->m_ChannelDataMap.begin(); itr != sConnectServer->m_ChannelDataMap.end(); ++itr)
		{
			ChannelData const* pData = itr->second;

			if (!pData)
			{
				continue;
			}

			body[head->count].server = pData->GetServer() + MAX_SERVER_PER_GROUP;
			body[head->count].data1 = sGameServer->GetServerType() == SERVER_TYPE_NORMAL && sGameServer->IsFlag(SERVER_FLAG_DISPLAY) ? 32 : 0;
			body[head->count].data2 = 0;
			body[head->count].type = pData->GetType();
			body[head->count].gold = pData->GetGold();
			++head->count;
		}
	}

	head->Set(0xEC, 0x57, sizeof(PACKET_CHANNEL_HEAD) + (head->count * sizeof(PACKET_CHANNEL_BODY)));
	this->sendPacket(buffer, head->GetSize());
}

void Player::ServerWarp(uint8 * Packet)
{
	if (sGameServer->GetServerType() != SERVER_TYPE_NORMAL)
	{
		return;
	}

	PACKET_CHANNEL_WARP_RESULT pMsg;

	if (!sGameServer->IsChannelChangeEnabled())
	{
		this->SendPacket(&pMsg);
		return;
	}

	if (!Player::IsPlayerBasicState(this, false))
	{
		this->SendPacket(&pMsg);
		return;
	}

	POINTER_PCT_CHECK(PACKET_CHANNEL_WARP, lpMsg, Packet, 0);

	if (this->IsInSelfDefense())
	{
		pMsg.result = 5;
		this->SendPacket(&pMsg);
		return;
	}

	uint16 server = lpMsg->server - MAX_SERVER_PER_GROUP;

	if (!sConnectServer->IsServerAvailable(server))
	{
		this->SendPacket(&pMsg);
		return;
	}

	if ((server / MAX_SERVER_PER_GROUP) != sGameServer->GetServerGroup())
	{
		this->SendPacket(&pMsg);
		return;
	}
	
	sAuthServer->PlayerServerMoveRequest(this, server, this->GetWorldId(), this->GetX(), this->GetY(), true);
}

void Player::SendAntiMacro()
{
	PACKET_ANTI_MACRO pMsg;
	pMsg.enabled = sGameServer->IsAntiMacroEnabled() ? 1 : 0;
	pMsg.time = sGameServer->GetAntiMacroTime();
	pMsg.combo_enabled = sGameServer->IsAntiMacroComboEnabled() ? 1 : 0;
	pMsg.combo_time = sGameServer->GetAntiMacroComboTime();

	this->SendPacket(&pMsg);
}

void Player::AntiMacroDetect(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PACKET_ANTI_MACRO_DETECT, lpMsg, Packet, 0);

	switch (lpMsg->type)
	{
	case 0:
		{
			sLog->outInfo("antihack", "[Type: 0 - ANTI_MACRO_POTION] [%s] [%s]", this->BuildLog().c_str(), this->BuildLocationLog().c_str());
			//sGameServer->LogAntiHack(this, ANTIHACK_ID_MACRO_POTION, "ANTI_MACRO_POTION(%d) -- Count: %d", lpMsg->count, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
		} break;

	case 1:
		{
			sLog->outInfo("antihack", "[Type: 1 - ANTI_MACRO_SKILL] [%s] [%s]", this->BuildLog().c_str(), this->BuildLocationLog().c_str());
			//sGameServer->LogAntiHack(this, ANTIHACK_ID_MACRO_SKILL, "ANTI_MACRO_SKILL(%d) -- Count: %d", lpMsg->count, this->GetCounter(PLAYER_COUNTER_HACK_CHECK));
		} break;
	}
	
}

void Player::AddRecoveryMixItem(Item const* pItem)
{
	Item item = *pItem;

	int64 current_time = time(nullptr);

	item.SetState(ITEM_CHANGED);
	item.SetExpireDate(time(nullptr) + sGameServer->GetMixRecoveryTime());
	
	if (item.common.time > current_time)
	{
		item.common.time = current_time - item.common.time;
		item.common.attempts += 1;
	}
	else
	{
		item.common.attempts = 1;
		item.common.time = 0;
	}

	while (true)
	{
		uint8 result = this->GetMixRecoveryInventory()->AddItem(item);

		if (result != uint8(-1))
		{
			this->LogRecoveryMixItem(this->GetMixRecoveryInventory()->GetItem(result), 0);
			break;
		}

		this->RemoveOldRecoveryMixItem();
	}
}

void Player::RemoveOldRecoveryMixItem()
{
	time_t oldest_time = 0;
	uint8 slot = -1;

	for (uint8 i = 0; i < max_shop_item; ++i)
	{
		Item const* pItem = this->GetMixRecoveryInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		if (oldest_time == 0 || pItem->GetExpireDate() < oldest_time)
		{
			oldest_time = pItem->GetExpireDate();
			slot = i;
		}
	}

	if (slot == uint8(-1))
	{
		return;
	}

	this->GetMixRecoveryInventory()->DeleteItem(slot);
}

void Player::AttemptRecoveryMixItem(uint8 slot)
{
	if (!sGameServer->IsMixRecoveryEnabled())
	{
		this->SendMessageBox(1, "Mix Item Recovery", "Item recovery has been disabled.");
		this->ItemBuyResult();
		return;
	}

	if (this->GetInterfaceState()->GetID() != InterfaceData::MixRecovery)
	{
		this->ItemBuyResult();
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->ItemBuyResult();
		return;
	}

	Item* pItem = this->GetMixRecoveryInventory()->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		this->SendMessageBox(1, "Mix Item Recovery", "Can't find item.");
		this->ItemBuyResult();
		return;
	}

	int32 price = (pItem->common.talisman * sGameServer->GetMixRecoveryPriceWC()) * pItem->common.attempts;

	if (pItem->IsExpired())
	{
		price = 0;
	}

	if (this->GetCredits() < price)
	{
		this->SendMessageBox(1, "Mix Item Recovery", "You need %d wcoins to recover your item.", price);
		this->ItemBuyResult();
		return;
	}

	uint8 result = this->AddItem(*pItem, -1, ADD_ITEM_SEND_NO);

	if (result == uint8(-1))
	{
		this->SendMessageBox(1, "Mix Item Recovery", "You don't have enough space on inventory.");
		this->ItemBuyResult();
		return;
	}

	this->LogRecoveryMixItem(pItem, 1);

	this->GetMixRecoveryInventory()->DeleteItem(slot);

	Item* pNewItem = this->GetInventory()->GetItem(result);

	pNewItem->SetExpireDate(0);
	pNewItem->common.talisman = 0;
	pNewItem->common.time += time(nullptr);
	pNewItem->common.time += sGameServer->GetMixRecoveryFailTime();

	pNewItem->UpdateMixRecovery();

	this->SendInventory(result);

	this->ReduceCredits(price);

	this->SendRecoveryMixItemList(nullptr, true);
}

void Player::LogRecoveryMixItem(Item const* pItem, uint8 type)
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(LOG_STATEMENT_MIX_RECOVERY);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());
	stmt->setString(2, this->GetAccountData()->GetAccount());
	stmt->setUInt32(3, this->GetGUID());
	stmt->setString(4, this->GetName());
	stmt->setString(5, this->BuildLogDB());
	stmt->setUInt8(6, type);
	stmt->setString(7, pItem->BuildLogDB(-1, true));
	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void Player::UpdateRecoveryMix()
{
	for (int32 i = 0; i < inventory_size; ++i)
	{
		Item* pItem = this->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		pItem->UpdateMixRecovery();
	}
}

void Player::SendRecoveryMixItemList(Monster* pMonster, bool renew)
{
	uint8 buffer[4092];
	POINTER_PCT(SHOP_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SHOP_ITEM_LIST_BODY, body, buffer, sizeof(SHOP_ITEM_LIST_HEAD));
	head->count = 0;

	uint8 buffer_misc[4092];
	POINTER_PCT(SHOP_MISC_HEAD, head_misc, buffer_misc, 0);
	POINTER_PCT(SHOP_MISC_BODY, body_misc, buffer_misc, sizeof(SHOP_MISC_HEAD));
	head_misc->count = 0;
	head_misc->type = SHOP_TYPE_WCOIN;

	for (uint8 i = 0; i < max_shop_item; ++i)
	{
		Item const* pItem = this->GetMixRecoveryInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		body[head->count].slot = i;
		pItem->ConvertToBuffer(body[head->count].item_info);
		++head->count;

		body_misc[head_misc->count].slot = i;
		body_misc[head_misc->count].price = (pItem->common.talisman * sGameServer->GetMixRecoveryPriceWC()) * pItem->common.attempts;

		if (pItem->IsExpired())
		{
			body_misc[head_misc->count].price = 1;
		}

		++head_misc->count;
	}

	if (!renew)
	{
		this->GetInterfaceState()->Set(InterfaceData::MixRecovery, pMonster);
		this->SetShopTime(0);
		pMonster->AddTalkReference(this);
	}

	TALK_TO_NPC_RESULT pMsg(0);
	this->sendPacket(MAKE_PCT(pMsg));

	head_misc->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MISC_SHOP, sizeof(SHOP_MISC_HEAD) + (head_misc->count * sizeof(SHOP_MISC_BODY)));
	this->sendPacket(buffer_misc, head_misc->h.get_size());

	head->type = 0x00;
	head->h.set(0x31, sizeof(SHOP_ITEM_LIST_HEAD) + (head->count * sizeof(SHOP_ITEM_LIST_BODY)));
	this->sendPacket(buffer, head->h.get_size());
}

bool Player::IsSkillCPS(uint16 skill, uint32 time)
{
	if (this->GetHelper()->IsStarted() && this->GetHelper()->IsOffline())
	{
		return true;
	}

	SkillCPS* pSkillCPS = this->GetSkillCPS(skill);

	if (!pSkillCPS)
	{
		pSkillCPS = new SkillCPS();
		pSkillCPS->SetTime(time);
		pSkillCPS->SetCount(0);
		this->m_SkillCPSMap[skill] = pSkillCPS;
	}
	else
	{
		uint32 old_time = pSkillCPS->GetTime();
		pSkillCPS->SetTime(time);

		int64 diff = time - old_time;

		if (this->IsAdministrator())
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Skill: %u - Speed: %d - Time: %I64d", skill, this->GetIntData(UNIT_INT_ATTACK_SPEED), diff);
		}

		SkillAttackTime const* pAttackTime = sSkillMgr->GetSkillAttackTime(this, skill);

		if (!pAttackTime || pAttackTime->GetTime() <= 0)
		{
			return true;
		}

		if (diff <= 0)
		{
			if (sGameServer->IsAttackTimeNullKick())
			{
				KICK_PLAYER(this, "AttackTime Null");
			}

			return false;
		}

		if ((old_time + pAttackTime->GetTime()) > time)
		{
			pSkillCPS->IncreaseCount(1);

			if (sGameServer->GetAntiHackSkillLogCount() > 0 && pSkillCPS->GetCount() >= sGameServer->GetAntiHackSkillLogCount())
			{
				sGameServer->LogAntiHack(this, ANTIHACK_ID_ATTACK_TIME, "Skill: %u - Speed: %d - Step: %d - Count: %d - OldTime: %u - Time: %u - Diff: %I64d", skill, this->GetIntData(UNIT_INT_ATTACK_SPEED), pAttackTime->GetStep(), pSkillCPS->GetCount(), old_time, time, diff);
			}

			if (sGameServer->GetAntiHackSkillCount() > 0 && pSkillCPS->GetCount() >= sGameServer->GetAntiHackSkillCount())
			{
				if (sGameServer->IsAntiHackSkillKick())
				{
					KICK_PLAYER(this, "AntiHack Skill");

					sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "AntiHack Skill");
				}
			}

			if (pSkillCPS->GetCount() >= sGameServer->GetAntiHackSkillCountContinue())
			{
				return false;
			}

			return sGameServer->IsAntiHackSkillContinue();
		}

		pSkillCPS->SetCount(0);
	}

	return true;
}

SkillCPS* Player::GetSkillCPS(uint16 skill)
{
	SkillCPSMap::const_iterator itr = this->m_SkillCPSMap.find(skill);

	if (itr != this->m_SkillCPSMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

SkillCPS const* Player::GetSkillCPS(uint16 skill) const
{
	SkillCPSMap::const_iterator itr = this->m_SkillCPSMap.find(skill);

	if (itr != this->m_SkillCPSMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

int32 Player::GetSkillCPSCount(uint16 skill) const
{
	SkillCPS const* pSkillCPS = this->GetSkillCPS(skill);

	if (pSkillCPS)
	{
		return pSkillCPS->GetCount();
	}

	return 0;
}

void Player::KuberaMineCart(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	if (this->GetWorldId() != WORLD_KUBERA_MINE_1 &&
		this->GetWorldId() != WORLD_KUBERA_MINE_2 &&
		this->GetWorldId() != WORLD_KUBERA_MINE_3 &&
		this->GetWorldId() != WORLD_KUBERA_MINE_4 &&
		this->GetWorldId() != WORLD_KUBERA_MINE_5)
	{
		return;
	}

	POINTER_PCT_CHECK(KUBERA_MINE_CART, lpMsg, Packet, 0);

	this->SetVehiculeType(lpMsg->type);
}

void Player::PetTrainerMix1(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(PET_TRAINER_MIX_1, lpMsg, Packet, 0);

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");

		this->SendPetTrainerMix1Result(lpMsg->type, 1);
		return;
	}

	MixHandler(this, 7).PetTrainerMix(lpMsg->type, lpMsg->id, lpMsg->main_material, lpMsg->materials, MAX_PET_TRAINER_MIX_SLOTS);
}

void Player::PetTrainerMix2(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false, false))
	{
		return;
	}

	if (!Player::InterfaceCheck(this, InterfaceData::PetTrainer, 0))
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");

		this->SendPetTrainerMix2Result(1);
		return;
	}

	POINTER_PCT_CHECK(PET_TRAINER_MIX_2, lpMsg, Packet, 0);

	Item* pGuardian = this->GetInventory()->GetItem(lpMsg->horse_slot);
	Item* pMaterial = this->GetInventory()->GetItem(lpMsg->material_slot);

	if (!pGuardian || !pGuardian->IsItem())
	{
		this->SendPetTrainerMix2Result(1);
		return;
	}

	if (!pMaterial || !pMaterial->IsItem())
	{
		this->SendPetTrainerMix2Result(1);
		return;
	}

	if (pGuardian->GetKind2() != ItemKind::GUARDIAN_MOUNT)
	{
		this->SendPetTrainerMix2Result(1);
		return;
	}

	if (pMaterial->GetItem() != ITEMGET(14, 470))
	{
		this->SendPetTrainerMix2Result(1);
		return;
	}

	this->ClearItem(lpMsg->material_slot);

	RandomValue<uint8> m_RandomValue(0);

	ItemGuardianEliteOptionValueList const& list = sItemMgr->GetGuardianEliteOptionValueList();
	for (ItemGuardianEliteOptionValueList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
	{
		item_guardian_elite_option_value const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		m_RandomValue.AddValue(pData->GetGroup(), 0);
	}
	
	pGuardian->SetSocket(4, m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM));
	pGuardian->Convert(true);

	this->SendInventory(lpMsg->horse_slot);

	this->SendPetTrainerMix2Result(0);
}

void Player::SendPetTrainerMix1Result(uint8 type, uint8 result)
{
	PET_TRAINER_MIX_1_RESULT pMsg;
	pMsg.type = type;
	pMsg.result = result;

	this->SendPacket(&pMsg);
}

void Player::SendPetTrainerMix2Result(uint8 result)
{
	PET_TRAINER_MIX_2_RESULT pMsg;
	pMsg.result = result;

	this->SendPacket(&pMsg);
}

MonsterSoulData* Player::GetMonsterSoulData(uint32 id)
{
	MonsterSoulDataMap::iterator itr = this->m_MonsterSoulDataMap.find(id);

	if (itr != this->m_MonsterSoulDataMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

void Player::RemoveMonsterSoulData(uint32 id)
{
	MonsterSoulDataMap::iterator itr = this->m_MonsterSoulDataMap.find(id);

	if (itr != this->m_MonsterSoulDataMap.end())
	{
		delete itr->second;
		this->m_MonsterSoulDataMap.erase(itr);
	}
}

uint8 Player::GetMagicTreeLevel(uint16 skill) const
{
	Skill const* pSkill = this->GetSkillTreeSkill(skill);

	if (pSkill)
	{
		return pSkill->GetLevel();
	}

	MajesticSkill const* pMajesticSkill = this->GetMajesticSkillTreeSkill(skill);

	if (pMajesticSkill)
	{
		return pMajesticSkill->GetLevel();
	}

	return 0;
}

void Player::SwitchModeTurnOn(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this))
	{
		return;
	}

	SWITCH_MODE_ON pMsg;

	if (!this->HasBuff(BUFF_SWITCH_MODE))
	{
		pMsg.result = 1;
		this->SendPacket(&pMsg);
		return;
	}

	this->SendPacket(&pMsg);
}

void Player::ItemEnhancementRequest(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	if (!this->IsAuthorizationEnabled())
	{
		this->SendMessageBox(0, "Unauthorized access", "You are not authorized for this action.");
		return;
	}

	if (this->GetInterfaceState()->GetID() != InterfaceData::None && this->GetTransaction() == TRANSACTION_TYPE_BEGIN)
	{
		//this->SetAuthority(1);
		//KICK_PLAYER(this, "Dupe Attempt");

		//sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Dupe Attempt");
		return;
	}

	POINTER_PCT(ITEM_ENHANCEMENT_REQUEST, lpMsg, Packet, 0);

	ITEM_ENHANCEMENT_RESULT pMsg;

	sLog->outPacket("chaosmix", Packet, lpMsg->GetSize(), "%s => %s", __FUNCTION__, this->BuildLog().c_str());

	if (!use_inventory_range(lpMsg->item.slot))
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	/*if (lpMsg->item.data <= 0)
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}*/

	Item* pItem = this->GetInventory()->GetItem(lpMsg->item.slot);

	if (!pItem || !pItem->IsItem())
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (!pItem->IsFlag(ITEM_FLAG_ALLOW_BLESS))
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (pItem->GetLevel() >= MAX_ITEM_LEVEL)
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	int32 success_rate = 0;
	int32 price = 0;
	Item* pTalismanChaos = nullptr;
	Item* pTalismanLuck = nullptr;
	int32 talisman_rate = 0;
	uint8 talisman_chaos_slot = -1;
	uint8 talisman_luck_slot = -1;

	std::map<uint16, uint8> MaterialList;

	if (pItem->GetLevel() < 6)
	{
		if (lpMsg->materials[0].count == 1)
		{
			if (this->GetInventory()->GetItemCount(lpMsg->materials[0].item, 0) >= lpMsg->materials[0].count)
			{
				if (lpMsg->materials[0].item == ITEMGET(14, 13))
				{
					success_rate = 100;
					MaterialList[lpMsg->materials[0].item] = lpMsg->materials[0].count;
				}
				else if (lpMsg->materials[0].item == ITEMGET(14, 14) && sGameServer->IsJewelOfSoulEnabled())
				{
					success_rate = sGameServer->GetJewelOfSoulRate();

					if (pItem->GetLuck() != 0)
					{
						success_rate += sGameServer->GetJewelOfSoulRateLuck();
					}

					MaterialList[lpMsg->materials[0].item] = lpMsg->materials[0].count;
				}
			}
		}
	}
	else if (pItem->GetLevel() < 9)
	{
		if (lpMsg->materials[0].count == 1 && lpMsg->materials[0].item == ITEMGET(14, 14) && sGameServer->IsJewelOfSoulEnabled())
		{
			if (this->GetInventory()->GetItemCount(lpMsg->materials[0].item, 0) >= lpMsg->materials[0].count)
			{
				success_rate = sGameServer->GetJewelOfSoulRate();

				if (pItem->GetLuck() != 0)
				{
					success_rate += sGameServer->GetJewelOfSoulRateLuck();
				}

				MaterialList[lpMsg->materials[0].item] = lpMsg->materials[0].count;
			}
		}
	}
	else
	{
		int32 enhance_level = (pItem->GetLevel() - 8);

		if (lpMsg->materials[0].count == 1 && lpMsg->materials[0].item == ITEMGET(12, 15) &&
			lpMsg->materials[1].count == enhance_level && lpMsg->materials[1].item == ITEMGET(14, 13) &&
			lpMsg->materials[2].count == enhance_level && lpMsg->materials[2].item == ITEMGET(14, 14))
		{
			if (this->GetInventory()->GetItemCount(lpMsg->materials[0].item, 0) >= lpMsg->materials[0].count &&
				this->GetInventory()->GetItemCount(lpMsg->materials[1].item, 0) >= lpMsg->materials[1].count &&
				this->GetInventory()->GetItemCount(lpMsg->materials[2].item, 0) >= lpMsg->materials[2].count)
			{
				price = 2000000 * enhance_level;
				success_rate = sGameServer->GetItemUpgradeSuccessRate(enhance_level - 1);

				MaterialList[lpMsg->materials[0].item] = lpMsg->materials[0].count;
				MaterialList[lpMsg->materials[1].item] = lpMsg->materials[1].count;
				MaterialList[lpMsg->materials[2].item] = lpMsg->materials[2].count;

				if (pItem->GetLuck() != 0)
				{
					success_rate += 25;
				}

				for (int32 i = 0; i < 2; ++i)
				{
					int32 current_slot = 3 + i;

					ITEM_ENHANCEMENT_DATA const& material = lpMsg->materials[current_slot];

					if (material.count >= 1 && use_inventory_range(material.slot))
					{
						Item* pMaterial = this->GetInventory()->GetItem(material.slot);

						if (!pMaterial)
						{
							continue;
						}

						if (pMaterial->GetItem() != material.item)
						{
							continue;
						}

						if (pItem->IsPentagramItem())
						{
							if (pMaterial->GetItem() == ITEMGET(14, 190))
							{
								pTalismanChaos = pMaterial;
								talisman_chaos_slot = material.slot;
							}
							else if (pMaterial->GetItem() == ITEMGET(14, 189))
							{
								pTalismanLuck = pMaterial;
								talisman_rate = 10;
								talisman_luck_slot = material.slot;
							}
						}
						else
						{
							if (pMaterial->GetItem() == ITEMGET(14, 96))
							{
								pTalismanChaos = pMaterial;
								talisman_chaos_slot = material.slot;
							}
							else if (pMaterial->GetItem() == ITEMGET(14, 53))
							{
								pTalismanLuck = pMaterial;
								talisman_rate = pTalismanLuck->GetDurability();
								talisman_luck_slot = material.slot;
							}
						}
					}
				}
			}
		}
	}

	if (success_rate <= 0)
	{
		pMsg.result = 3;
		this->SendPacket(&pMsg);
		return;
	}

	if (pTalismanLuck)
	{
		if (talisman_rate > 10)
		{
			talisman_rate = 10;
		}

		success_rate += talisman_rate;
	}

	if (success_rate > 100)
	{
		success_rate = 100;
	}

	if (price > 0)
	{
		if (!this->MoneyHave(price))
		{
			pMsg.result = 2;
			this->SendPacket(&pMsg);
			return;
		}

		this->MoneyReduce(price);
	}

	for (std::map<uint16, uint8>::const_iterator itr = MaterialList.begin(); itr != MaterialList.end(); ++itr)
	{
		this->ItemDelete(itr->first, 0, itr->second);
	}

	uint8 talisman = 0;
	
	if (pTalismanChaos)
	{
		talisman |= 1;
		this->ClearItem(talisman_chaos_slot);
	}
	else
	{
		if (pItem->GetLevel() >= 9)
		{
			pItem->common.talisman += 1;
		}
	}

	if (pTalismanLuck)
	{
		talisman |= 2;
		this->ClearItem(talisman_luck_slot);
	}

	if (roll_chance_i(success_rate))
	{
		pMsg.result = 0;

		this->ItemEnhancementLog(pItem, success_rate, price, true, talisman);

		pItem->IncreaseLevel(1);
		pItem->AlterDurability(3, 0.0f);
		pItem->Convert();

		this->SendInventory(lpMsg->item.slot);
	}
	else
	{
		this->ItemEnhancementLog(pItem, success_rate, price, false, talisman);

		if (pItem->GetLevel() > 0)
		{
			if (pItem->GetLevel() <= 6)
			{
				pItem->ReduceLevel(1);
				pItem->AlterDurability(3, 0.0f);
				pItem->Convert();

				this->SendInventory(lpMsg->item.slot);
			}
			else if (pItem->GetLevel() <= 8 || pTalismanChaos)
			{
				pItem->common.talisman = 0;

				pItem->SetLevel(0);
				pItem->AlterDurability(3, 0.0f);
				pItem->Convert();

				this->SendInventory(lpMsg->item.slot);
			}
			else
			{
				sPentagramSystem->DelAllPentagramJewelInfo(this, pItem, 0);

				pItem->SetLevel(0);
				pItem->AlterDurability(3, 0.0f);
				pItem->Convert();

				this->AddRecoveryMixItem(pItem);

				this->ClearItem(lpMsg->item.slot);
			}
		}

		pMsg.result = 1;
	}

	this->SendPacket(&pMsg);
}

void Player::ItemEnhancementLog(Item* pItem, int32 success_rate, int32 price, bool success, uint8 talisman)
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_MIX);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());
	stmt->setString(2, this->GetAccountData()->GetAccount());
	stmt->setUInt32(3, this->GetGUID());
	stmt->setString(4, this->GetName());
	stmt->setString(5, this->BuildLogDB());
	stmt->setUInt16(6, -1);
	stmt->setString(7, "[ Item Upgrade ] => " + std::to_string(pItem->GetLevel()));
	stmt->setInt32(8, success_rate);
	stmt->setUInt32(9, price);
	stmt->setBool(10, success);
	stmt->setString(11, pItem->BuildLogDB(-1, true));

	std::string talisman_data;
	if ((talisman & 1) != 0)
	{
		talisman_data = "Talisman of Chaos Assembly - ";
	}

	if ((talisman & 2) != 0)
	{
		talisman_data = "Talisman of Luck";
	}

	stmt->setString(12, talisman_data);

	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);
}

void Player::PrintItemData(Item const* pItem)
{
	if (!this->IsAdministrator())
	{
		return;
	}

	if (!pItem || !pItem->IsItem())
	{
		return;
	}

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s", pItem->GetName());

	for (uint8 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		if (pItem->GetRequiredStat(i) > 0)
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Required %s: %d", StatName[i], pItem->GetRequiredStat(i));
		}
	}

	if (pItem->GetRequiredLevel() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Required Level: %d", pItem->GetRequiredLevel());
	}

	if (pItem->GetMinDamage() > 0 || pItem->GetMaxDamage() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Damage: %u - %u", pItem->GetMinDamage(), pItem->GetMaxDamage());
	}

	if (pItem->GetMagicPower() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Magic Power: %u", pItem->GetMagicPower());
	}

	if (pItem->GetCursePower() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Curse Power: %u", pItem->GetCursePower());
	}

	if (pItem->GetPetPower() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Pet Power: %u", pItem->GetPetPower());
	}

	if (pItem->GetDefense() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Defense: %u", pItem->GetDefense());
	}

	if (pItem->GetDefenseSuccessRate() > 0)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Defense Success Rate: %u", pItem->GetDefenseSuccessRate());
	}

	for (auto & itr : pItem->m_OptionData)
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Option %u - %u", itr.first, itr.second);
	}
}