/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ItemMgr.cpp"
*
*/
uint8 GetRandomExcOption(uint8 max_options)
{
	limitmax(max_options, MAX_OPTION);

	uint8 btRandOpt;
	uint8 btItemOpts = 0x00;

	for ( uint8 i = 0; i < max_options; ++i )
	{
		while(true)
		{
			btRandOpt = Random<uint8>(MAX_OPTION);

			if (((btItemOpts >> btRandOpt) & 1) == 0)
			{
				btItemOpts |= (1 << btRandOpt);
				break;
			}
		}
	}

	return btItemOpts;
}

void GetRandomExcOption(uint8 & exe, uint8 max_options)
{
	limitmax(max_options, MAX_OPTION);

	uint8 btRandOpt;

	for ( uint8 i = 0; i < max_options; ++i )
	{
		while(true)
		{
			btRandOpt = Random<uint8>(MAX_OPTION);

			if (((exe >> btRandOpt) & 1) == 0)
			{
				exe |= (1 << btRandOpt);
				break;
			}
		}
	}
}

CItemMgr::CItemMgr()
{
	this->SetLastItemUpdate(0);
}

CItemMgr::~CItemMgr()
{
	for ( ItemTemplateMap::iterator it = this->m_item_template.begin(); it != this->m_item_template.end(); ++it )
	{
		MAP_CLEAR(ItemLevelTemplateMap::iterator, it->second->level_data);
	}

	MAP_CLEAR(ItemTemplateMap::iterator, this->m_item_template);

	MAP_CLEAR(ItemSetOptionMap::iterator, this->item_set_option_map);
	MAP_CLEAR(ItemSetTypeMap::iterator, this->m_item_set_type);

	for ( uint8 i = 0; i < HARMONY_ITEM_MAX; ++i )
	{
		LIST_CLEAR(ItemHarmonyDataList::iterator, this->m_item_harmony_list[i]);
	}

	MAP_CLEAR(ItemSocketSeedMap::iterator, this->m_item_socket_seed);
	MAP_CLEAR(ItemSocketBonusMap::iterator, this->m_item_socket_bonus);
	MAP_CLEAR(ItemSocketPackageMap::iterator, this->m_item_socket_package);
	LIST_CLEAR(ItemTransformationList::iterator, this->m_item_transformation);
	LIST_CLEAR(ItemDropList::iterator, this->m_item_drop);
	LIST_CLEAR(ItemMixLuckyList::iterator, this->m_item_mix_lucky);
	LIST_CLEAR(ItemPriceDataList::iterator, this->m_item_price_data_list);
	MAP_CLEAR(ItemAddOptionMap::iterator, this->m_item_add_option);
	MAP_CLEAR(ItemWingValuesMap::iterator, this->m_item_wing_values);
	LIST_CLEAR(ItemReducedDropList::iterator, this->m_item_reduced_drop_list);
	LIST_CLEAR(ItemCreateDataList::iterator, this->m_item_create_data_list);
	MAP_CLEAR(ItemSocketMixMap::iterator, this->m_item_socket_mix);
	MAP_CLEAR(ItemSocketUpgradeMap::iterator, this->m_item_socket_upgrade_map);
	MAP_CLEAR(ItemSocketSeedSphereMap::iterator, this->m_item_socket_seed_sphere_map);
	MAP_CLEAR(ItemFlagDBMap::iterator, this->m_item_flagdb_map);
	MAP_CLEAR(ItemSetValuesMap::iterator, this->m_ItemSetValuesMap);
	MAP_CLEAR(ItemWingGradedOptionMap::iterator, this->m_ItemWingGradedOptionMap);
	LIST_CLEAR(ItemWingSocketOptionList::iterator, this->m_ItemWingSocketOptionList);
	MAP_CLEAR(ItemWingSocketUpgradeMap::iterator, this->m_ItemWingSocketUpgradeMap);
	MAP_CLEAR(ItemEarringMap::iterator, this->m_ItemEarringMap);
	MAP_CLEAR(ItemEarringOptionMap::iterator, this->m_ItemEarringOptionMap);
	LIST_CLEAR(ItemEarringOptionValueList::iterator, this->m_ItemEarringOptionValueList);
	MAP_CLEAR(ItemEarringBonusMap::iterator, this->m_ItemEarringBonusMap);
	MAP_CLEAR(ItemEarringUpgradeMap::iterator, this->m_ItemEarringUpgradeMap);
	LIST_CLEAR(ItemMasteryBonusList::iterator, this->m_ItemMasteryBonusList);
	LIST_CLEAR(ItemMysteriousStoneList::iterator, this->m_ItemMysteriousStoneList);
	LIST_CLEAR(ItemGuardianOptionList::iterator, this->m_ItemGuardianOptionList);
	LIST_CLEAR(ItemGuardianOptionValueList::iterator, this->m_ItemGuardianOptionValueList);
	LIST_CLEAR(ItemGuardianEliteOptionList::iterator, this->m_ItemGuardianEliteOptionList);
	LIST_CLEAR(ItemGuardianEliteOptionValueList::iterator, this->m_ItemGuardianEliteOptionValueList);
	CLEAR_LIST(this->m_HarmonyOptionList);
}

void CItemMgr::LoadItemTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Template...");

	for ( ItemTemplateMap::iterator it = this->m_item_template.begin(); it != this->m_item_template.end(); ++it )
	{
		MAP_CLEAR(ItemLevelTemplateMap::iterator, it->second->level_data);
	}

	MAP_CLEAR(ItemTemplateMap::iterator, this->m_item_template);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_template i WHERE i.disabled = 0");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_template * add_item = new item_template;

			add_item->SetType(fields[loop++].GetUInt8());
			add_item->SetIndex(fields[loop++].GetUInt16());
			add_item->SetName(fields[loop++].GetCString());
			add_item->SetSlot(fields[loop++].GetUInt8());
			add_item->SetX(fields[loop++].GetUInt8());
			add_item->SetY(fields[loop++].GetUInt8());
			add_item->SetFlag(fields[loop++].GetUInt32());
			add_item->SetDropRate(fields[loop++].GetUInt16());
			add_item->SetSkill(fields[loop++].GetUInt16());
			add_item->SetMaxCount(fields[loop++].GetInt16());
			add_item->SetAncientAttribute(fields[loop++].GetUInt8());
			add_item->SetKind1(fields[loop++].GetUInt8());
			add_item->SetKind2(fields[loop++].GetUInt8());
			add_item->SetKind3(fields[loop++].GetUInt8());
			add_item->SetValue(fields[loop++].GetUInt16());
			add_item->SetBuyPrice(fields[loop++].GetUInt32());
			add_item->SetPriceFlags(fields[loop++].GetUInt32());
			add_item->SetLevel(fields[loop++].GetInt32());
			add_item->SetDurability(fields[loop++].GetUInt8());
			add_item->SetDurabilityMagic(fields[loop++].GetUInt8());
			add_item->SetDurabilityResistance(fields[loop++].GetInt32());
			add_item->SetDurabilityTime(fields[loop++].GetInt32());
			add_item->SetDurabilityTimeSafe(fields[loop++].GetInt32());
			add_item->SetDurabilityReduction(fields[loop++].GetUInt8());
			add_item->SetRequiredLevel(fields[loop++].GetInt16());

			for ( uint32 i = 0; i < MAX_STAT_TYPE; ++i )
			{
				add_item->SetRequiredStat(i, fields[loop++].GetUInt32());
			}

			add_item->SetMinDamage(fields[loop++].GetUInt32());
			add_item->SetMaxDamage(fields[loop++].GetUInt32());
			add_item->SetPower(fields[loop++].GetUInt32());
			add_item->SetAttackSpeed(fields[loop++].GetUInt32());
			add_item->SetWalkSpeed(fields[loop++].GetUInt32());
			add_item->SetDefense(fields[loop++].GetUInt32());
			add_item->SetDefenseMagic(fields[loop++].GetUInt32());
			add_item->SetDefenseSuccess(fields[loop++].GetUInt32());

			for ( uint32 i = 0; i < Element::MAX; ++i )
			{
				add_item->SetResistance(i, fields[loop++].GetUInt8());
			}

			for ( uint32 i = 0; i < Character::MAX_CLASS; ++i )
			{
				add_item->SetRequiredClass(i, fields[loop++].GetUInt8());
			}

			add_item->SetHarmonySmeltLevel(fields[loop++].GetInt16());
			add_item->SetUseDelay(fields[loop++].GetUInt32());
			add_item->SetDurationTime(fields[loop++].GetUInt32() * IN_MILLISECONDS);
			add_item->SetLootingTime(fields[loop++].GetUInt32() * IN_MILLISECONDS);
			add_item->SetInventoryType(fields[loop++].GetUInt8());
			add_item->SetMaxSocket(0);
			add_item->GetStackData()->Reset();
			add_item->SetRuudPrice(0);
			add_item->SetQuest(false);
			add_item->SetQuestType(-1);
			add_item->SetQuestIndex(-1);

			if ( add_item->GetDurabilityReduction() >= 4 )
			{
				add_item->SetDurabilityReduction(0);
			}

			this->m_item_template[add_item->GetItem()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemAddOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Add Option...");

	MAP_CLEAR(ItemAddOptionMap::iterator, this->m_item_add_option);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_add_option");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			item_add_option * add_item = new item_add_option;

			add_item->SetItem(reader.ReadItem());
			for (int32 i = 0; i < MAX_ITEM_ADD_OPTION_EFFECT; ++i)
			{
				add_item->SetEffect(i, reader.GetUInt16());
				add_item->SetValue(i, reader.GetUInt16());
			}

			this->m_item_add_option[add_item->GetItem()] = add_item;

			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item add option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemSetType()
{
	sLog->outLoad(true, "Loading Item Set Type...");

	MAP_CLEAR(ItemSetTypeMap::iterator, this->m_item_set_type);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_set_type");

	if ( result )
	{
		do
		{
			FieldReader reader(result->Fetch());

			item_set_type * add_item = new item_set_type;

			add_item->SetType(reader.GetUInt8());
			add_item->SetIndex(reader.GetUInt16());

			for ( uint8 i = 0; i < MAX_ITEM_ANCIENT_TYPE; ++i )
			{
				add_item->SetAncientID(i, reader.GetUInt8());
			}

			this->m_item_set_type[add_item->GetItem()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outLoad(false, ">> Loaded %u item set type definitions", count);
}

void CItemMgr::LoadItemSetOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Set Option...");

	MAP_CLEAR(ItemSetOptionMap::iterator, this->item_set_option_map);
	MAP_CLEAR(ItemSetValuesMap::iterator, this->m_ItemSetValuesMap);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_set_option");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_set_option * add_item = new item_set_option;

			int32 loop = 0;

			add_item->SetGUID(fields[loop++].GetUInt8());
			add_item->SetName(fields[loop++].GetCString());
			add_item->SetOptionCount(0);

			for ( uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION; ++i )
			{
				add_item->option_table[i][0] = fields[loop++].GetUInt8();
				add_item->option_table[i][1] = fields[loop++].GetUInt8();

				if ( add_item->option_table[i][0] != uint8(-1) || add_item->option_table[i][1] != uint8(-1) )
					add_item->IncreaseOptionCount(1);
			}

			for (uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION; ++i)
			{
				add_item->option_table_value[i][0] = fields[loop++].GetInt32();
				add_item->option_table_value[i][1] = fields[loop++].GetInt32();
			}

			add_item->SetExtraOptionTable(0, fields[loop++].GetUInt8());
			add_item->SetExtraOptionTable(1, fields[loop++].GetUInt8());
			add_item->SetExtraOptionTableValue(0, fields[loop++].GetInt32());
			add_item->SetExtraOptionTableValue(1, fields[loop++].GetInt32());

			if ( add_item->GetExtraOptionTable(0) != uint8(-1) )
				add_item->IncreaseOptionCount(1);
			if ( add_item->GetExtraOptionTable(1) != uint8(-1) )
				add_item->IncreaseOptionCount(1);

			for ( uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i )
			{
				add_item->SetFullOptionTable(i, fields[loop++].GetUInt8());
			}

			for (uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
			{
				add_item->SetFullOptionTableValue(i, fields[loop++].GetInt32());
			}

			add_item->SetAncientHeroSoul(fields[loop++].GetBool());
			add_item->SetDisassemble(fields[loop++].GetBool());

			for (uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION; ++i)
			{
				if (add_item->option_table[i][0] >= 120 && add_item->option_table[i][0] < 0xFF)
				{
					this->AddSetOption(add_item->option_table_value[i][0]);
				}

				if (add_item->option_table[i][1] >= 120 && add_item->option_table[i][1] < 0xFF)
				{
					this->AddSetOption(add_item->option_table_value[i][1]);
				}
			}

			for (uint8 i = 0; i < MAX_ITEM_ANCIENT_OPTION_FULL; ++i)
			{
				if (add_item->GetFullOptionTable(i) >= 120 && add_item->GetFullOptionTable(i) < 0xFF)
				{
					this->AddSetOption(add_item->GetFullOptionTableValue(i));
				}
			}

			this->item_set_option_map[add_item->GetGUID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item set option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemHarmony()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Harmony...");

	for ( uint8 i = 0; i < HARMONY_ITEM_MAX; ++i )
	{
		LIST_CLEAR(ItemHarmonyDataList::iterator, this->m_item_harmony_list[i]);
	}

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_harmony");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 type = fields[0].GetUInt8();

			if ( type >= HARMONY_ITEM_MAX )
				continue;

			item_harmony_data * data = new item_harmony_data;
			data->SetType(type);
			data->SetOption(fields[1].GetUInt8());
			data->SetName(fields[2].GetString());
			data->SetRate(fields[3].GetUInt8());
			data->SetMinLevel(fields[4].GetUInt8());

			for ( uint8 i = 0; i < MAX_ITEM_HARMONY_LEVEL; ++i )
			{
				data->SetValue(i, fields[5 + (i * 2)].GetInt32());
				data->SetRequiredZen(i, fields[6 + (i * 2)].GetInt32());
			}

			this->m_item_harmony_list[type].push_back(data);
		
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item harmony definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemHarmonyEnhancement()
{
	sLog->outLoad(true, "Loading Item Harmony Enhancement...");

	CLEAR_LIST(this->m_HarmonyOptionList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_harmony_enhancement");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			HarmonyOption* pData = new HarmonyOption;
			pData->SetType(reader.GetUInt8());
			pData->SetID(reader.GetUInt8());
			pData->SetName(reader.GetString());

			for (int32 i = 0; i < MAX_HARMONY_ENHANCEMENT_LEVEL; ++i)
			{
				pData->SetValue(i, reader.GetInt32());
			}

			pData->SetSeparationValue(reader.GetUInt8());

			this->m_HarmonyOptionList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outLoad(false, ">> Loaded %u item harmony enhancement definitions", count);
}

void CItemMgr::LoadItemSocket()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_template * item_info = this->GetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));

			if ( item_info )
			{
				item_info->SetMaxSocket(fields[2].GetUInt8());

				if ( item_info->GetMaxSocket() > MAX_SOCKET_SLOT )
				{
					item_info->SetMaxSocket(MAX_SOCKET_SLOT);
				}
			}
						
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemSocketSeed()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Seed...");

	MAP_CLEAR(ItemSocketSeedMap::iterator, this->m_item_socket_seed);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_seed");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_socket_seed * add_item = new item_socket_seed;

			add_item->SetGUID(fields[loop++].GetUInt8());
			add_item->SetType(fields[loop++].GetUInt8());
			add_item->SetSubType(fields[loop++].GetUInt8());
			add_item->SetName(fields[loop++].GetString());
			add_item->SetValueType(fields[loop++].GetUInt8());

			for ( uint8 i = 0; i < MAX_SOCKET_LEVEL; ++i )
			{
				add_item->SetLevel(i, fields[loop++].GetUInt32());
			}
	
			this->m_item_socket_seed[add_item->GetGUID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket seed definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CItemMgr::LoadItemSocketBonus()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Bonus...");

	MAP_CLEAR(ItemSocketBonusMap::iterator, this->m_item_socket_bonus);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_bonus");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_socket_bonus * add_item = new item_socket_bonus;

			add_item->SetGUID(fields[loop++].GetUInt8());
			add_item->SetStartItemType(fields[loop++].GetUInt8());
			add_item->SetEndItemType(fields[loop++].GetUInt8());
			add_item->SetName(fields[loop++].GetString());
			add_item->SetValueType(fields[loop++].GetUInt8());
			add_item->SetValue(fields[loop++].GetUInt32());
			add_item->SetSuccessRate(fields[loop++].GetUInt8());

			for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				add_item->SetRequiredSocket(i, fields[loop++].GetUInt8());
			}

			add_item->SetSeedLevelMin(fields[loop++].GetUInt8());
			add_item->SetSeedLevelMax(fields[loop++].GetUInt8());
	
			this->m_item_socket_bonus[add_item->GetGUID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CItemMgr::LoadItemSocketPackage()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Package...");

	MAP_CLEAR(ItemSocketPackageMap::iterator, this->m_item_socket_package);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_package");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_socket_package * add_item = new item_socket_package;

			add_item->SetGUID(fields[0].GetUInt8());
			add_item->SetID(fields[1].GetUInt8());
			add_item->SetName(fields[2].GetString());
			add_item->SetValueType(fields[3].GetUInt8());
			add_item->SetValue(fields[4].GetUInt32());

			for ( uint8 i = 0; i < MAX_SOCKET_TYPE; ++i )
			{
				add_item->SetRequiredType(i, fields[5 + i].GetUInt8());
			}

			this->m_item_socket_package[add_item->GetGUID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket package definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemSocketMix()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Mix...");

	MAP_CLEAR(ItemSocketMixMap::iterator, this->m_item_socket_mix);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_mix");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_socket_mix * add_item = new item_socket_mix;

			add_item->SetRank(fields[0].GetUInt8());
			add_item->SetPrice(fields[1].GetUInt32());
			add_item->SetSuccessRate(fields[2].GetUInt8());
			add_item->SetRequiredRune(fields[3].GetUInt32());

			this->m_item_socket_mix[add_item->GetRank()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket mix definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemSocketUpgrade()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Upgrade...");

	MAP_CLEAR(ItemSocketUpgradeMap::iterator, this->m_item_socket_upgrade_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_upgrade");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_socket_upgrade * add_item = new item_socket_upgrade;

			add_item->SetID(fields[loop++].GetUInt8());
			add_item->SetType(fields[loop++].GetUInt8());
			add_item->SetIndex(fields[loop++].GetUInt16());
			add_item->SetLevelMin(fields[loop++].GetUInt8());
			add_item->SetLevelMax(fields[loop++].GetUInt8());
			add_item->SetSkill(fields[loop++].GetUInt8());
			add_item->SetOption(fields[loop++].GetUInt8());
			add_item->SetSeedSphere(fields[loop++].GetUInt8());
			add_item->SetSocketMin(fields[loop++].GetUInt8());
			add_item->SetPrice(fields[loop++].GetUInt32());
			add_item->SetRate(fields[loop++].GetUInt16());
			add_item->SetUpgradeType(fields[loop++].GetUInt8());
			add_item->SetUpgradeIndex(fields[loop++].GetUInt16());
			add_item->SetUpgradeLevel(fields[loop++].GetUInt8());
			add_item->SetUpgradeSkillRate(fields[loop++].GetUInt16());
			add_item->SetUpgradeLuckRate(fields[loop++].GetUInt16());

			for ( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				add_item->SetUpgradeSocketRate(i, fields[loop++].GetUInt16());
			}
			
			this->m_item_socket_upgrade_map[add_item->GetID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket upgrade definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemSocketSeedSphere()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Socket Seed Sphere...");

	MAP_CLEAR(ItemSocketSeedSphereMap::iterator, this->m_item_socket_seed_sphere_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_socket_seed_sphere");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_socket_seed_sphere * add_item = new item_socket_seed_sphere;

			add_item->SetID(fields[0].GetUInt16());
			add_item->SetAttribute(fields[1].GetUInt8());
			add_item->SetRank(fields[2].GetUInt8());
			add_item->SetNextID(fields[3].GetUInt16());

			this->m_item_socket_seed_sphere_map[add_item->GetID()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item socket seed sphere definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemTransformation()
{
	sLog->outLoad(true, "Loading Item Transformation...");

	LIST_CLEAR(ItemTransformationList::iterator, this->m_item_transformation);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_transformation AS i WHERE i.disabled = 0");

	if ( result )
	{
		do
		{
			FieldReader reader(result->Fetch());

			item_transformation * add_item = new item_transformation;

			add_item->SetType(reader.GetUInt8());
			add_item->SetIndex(reader.GetUInt16());
			add_item->SetLevel(reader.GetUInt8());
			add_item->SetSkin(reader.GetUInt16());
			add_item->SetSpecial(reader.GetBool());
			
			this->m_item_transformation.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outLoad(false, ">> Loaded %u item transformation definitions", count);
}

void CItemMgr::LoadItemDrop()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Drop...");

	LIST_CLEAR(ItemDropList::iterator, this->m_item_drop);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.PQuery("SELECT * FROM item_drop i WHERE i.server = -1 OR i.server = %u", sGameServer->GetServerCode());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_drop_data * add_item = new item_drop_data;

			add_item->SetType(fields[loop++].GetUInt8());
			add_item->SetIndex(fields[loop++].GetUInt16());
			add_item->SetLevelMin(fields[loop++].GetUInt8());
			add_item->SetLevelMax(fields[loop++].GetUInt8());
			add_item->SetDurability(fields[loop++].GetUInt8());
			add_item->SetSkill(fields[loop++].GetUInt8());
			add_item->SetLuck(fields[loop++].GetUInt8());
			add_item->SetOptionMin(fields[loop++].GetUInt8());
			add_item->SetOptionMax(fields[loop++].GetUInt8());
			add_item->SetExcellentMin(fields[loop++].GetUInt8());
			add_item->SetExcellentMax(fields[loop++].GetUInt8());
			add_item->SetAncient(fields[loop++].GetUInt8());

			for ( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
			{
				add_item->SetSocket(i, fields[loop++].GetUInt16());
			}
			add_item->SetWorld(fields[loop++].GetUInt16());
			add_item->SetMonster(fields[loop++].GetUInt16());
			add_item->SetMonsterLevelMin(fields[loop++].GetInt16());
			add_item->SetMonsterLevelMax(fields[loop++].GetInt16());
			add_item->SetEventID(fields[loop++].GetUInt8());
			add_item->SetInvasionID(fields[loop++].GetUInt32());
			add_item->SetSeasonEvent(fields[loop++].GetUInt8());
			add_item->SetRate(fields[loop++].GetUInt16());
		
			this->m_item_drop.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item drop definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemLevelTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Level Template...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_level_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 type = fields[0].GetUInt8();
			uint16 index = fields[1].GetUInt16();
			uint8 level = fields[2].GetUInt8();

			if ( level < 0 || level >= MAX_ITEM_LEVEL )
				continue;

			item_template * item_info = this->GetItem(ITEMGET(type, index));

			if ( !item_info )
				continue;

			item_level_template * add_level = new item_level_template;

			add_level->SetType(type);
			add_level->SetIndex(index);
			add_level->SetLevel(level);
			add_level->SetName(fields[3].GetCString());
			add_level->SetPrice(fields[4].GetUInt32());
			add_level->SetPriceFlag(fields[5].GetUInt32());
			add_level->SetFlags(fields[6].GetUInt32());
			item_info->SetNormalLevel(false);

			item_info->level_data[level] = add_level;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item level template definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemMixLucky()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Mix Lucky Template...");

	LIST_CLEAR(ItemMixLuckyList::iterator, this->m_item_mix_lucky);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mix_lucky_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_mix_lucky_data * add_item = new item_mix_lucky_data;

			add_item->SetTicketType(fields[0].GetUInt8());
			add_item->SetTicketIndex(fields[1].GetUInt16());
			add_item->SetTicketLevel(fields[2].GetUInt8());
			add_item->SetItemType(fields[3].GetUInt8());
			add_item->SetItemIndex(fields[4].GetUInt16());
			
			this->m_item_mix_lucky.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item mix lucky definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemPriceData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Price Data...");

	LIST_CLEAR(ItemPriceDataList::iterator, this->m_item_price_data_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_price");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_price_data * add_item = new item_price_data;

			add_item->SetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));
			add_item->SetLevel(fields[2].GetUInt8());
			add_item->SetBuyPrice(fields[3].GetInt64());
			add_item->SetSellPrice(fields[4].GetInt64());
			
			this->m_item_price_data_list.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item price data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemStack()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Stack...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_stack");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_template * item_info = this->GetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));

			if ( item_info )
			{
				item_info->GetStackData()->SetMaxStack(fields[2].GetUInt8());
				item_info->GetStackData()->SetStackOnLoot(fields[3].GetBool());
				item_info->GetStackData()->SetStackConvertType(fields[4].GetUInt8());
				item_info->GetStackData()->SetStackConvertIndex(fields[5].GetUInt16());
				item_info->GetStackData()->SetConvertedItem(this->GetItem(item_info->GetStackData()->GetConvert()));

				if ( item_info->GetStackData()->GetConvertedItem() == nullptr )
				{
					item_info->GetStackData()->SetStackConvertType(uint8(-1));
					item_info->GetStackData()->SetStackConvertIndex(uint16(-1));
				}

				if ( !item_info->GetStackData()->GetMaxStack() )
				{
					item_info->GetStackData()->Reset();
				}
				else
				{
					if ( item_info->GetDurability() == 0 )
					{
						//item_info->SetDurability(1);
						//sLog->outError("root", "%s -- Item: %u - %s. Wrong Durability.", __FUNCTION__, item_info->GetItem(), item_info->GetName());
					}
				}
			}
						
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item stack definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemQuest()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Quest...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_quest");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_template * item_info = this->GetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));

			if ( item_info )
			{
				item_info->SetQuest(true);
				item_info->SetQuestType(fields[2].GetUInt16());
				item_info->SetQuestIndex(fields[3].GetUInt16());
				count++;
			}
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item quest definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemPriceRuud()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Price Ruud...");

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_price_ruud");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_template * item_info = this->GetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));

			if ( item_info )
			{
				item_info->SetRuudPrice(fields[2].GetUInt32());

				count++;
			}
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item price ruud definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemWingValues()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Wing Values...");

	MAP_CLEAR(ItemWingValuesMap::iterator, this->m_item_wing_values);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_wing_values");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_template * item_info = this->GetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));

			if ( item_info )
			{
				item_wing_values * add_item = new item_wing_values;
				add_item->SetType(fields[0].GetUInt8());
				add_item->SetIndex(fields[1].GetUInt16());
				add_item->SetDamageAdd(fields[2].GetInt32());
				add_item->SetDamageAddLvl(fields[3].GetInt32());
				add_item->SetAbsorbAdd(fields[4].GetInt32());
				add_item->SetAbsorbAddLvl(fields[5].GetInt32());

				this->m_item_wing_values[item_info->GetItem()] = add_item;

				count++;
			}
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item wing values definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemReducedDrop()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Reduced Drop...");

	LIST_CLEAR(ItemReducedDropList::iterator, this->m_item_reduced_drop_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_reduced_drop");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_reduced_drop * add_item = new item_reduced_drop;

			add_item->SetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));
			add_item->SetLevel(fields[2].GetUInt8());
			add_item->SetRate(fields[3].GetUInt16());
			
			this->m_item_reduced_drop_list.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item reduced drop definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemCreateData()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Create Data...");

	LIST_CLEAR(ItemCreateDataList::iterator, this->m_item_create_data_list);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_create_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_create_data * add_item = new item_create_data;

			add_item->SetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));
			add_item->SetLevel(fields[2].GetUInt8());
			
			this->m_item_create_data_list.push_back(add_item);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item create data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemFlagDB()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item FlagDB...");

	MAP_CLEAR(ItemFlagDBMap::iterator, this->m_item_flagdb_map);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_flagdb");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			item_flagdb * add_item = new item_flagdb;

			add_item->SetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));
			add_item->SetFlags(fields[2].GetUInt32());
			
			this->m_item_flagdb_map[add_item->GetItem()] = add_item;
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item flagdb definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemWingGradedOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Wing Graded Option...");

	MAP_CLEAR(ItemWingGradedOptionMap::iterator, this->m_ItemWingGradedOptionMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_wing_graded_option");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_wing_graded_option * pData = new item_wing_graded_option;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			for (int32 i = 0; i < MAX_WING_GRADED_OPTION_LEVEL; ++i)
			{
				pData->SetValue(i, fields[loop++].GetInt32());
			}

			this->m_ItemWingGradedOptionMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item wing graded option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemWingSocketOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Wing Socket Option...");

	LIST_CLEAR(ItemWingSocketOptionList::iterator, this->m_ItemWingSocketOptionList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_wing_socket_option");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_wing_socket_option * pData = new item_wing_socket_option;
			int32 loop = 0;

			pData->SetType(fields[loop++].GetUInt8());
			pData->SetID(fields[loop++].GetUInt8());
			pData->SetRate(fields[loop++].GetInt32());
			for (int32 i = 0; i < MAX_WING_SOCKET_OPTION_LEVEL; ++i)
			{
				pData->SetValue(i, fields[loop++].GetInt32());
			}

			this->m_ItemWingSocketOptionList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item wing socket option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadItemWingSocketUpgrade()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Wing Socket Upgrade...");

	MAP_CLEAR(ItemWingSocketUpgradeMap::iterator, this->m_ItemWingSocketUpgradeMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_wing_socket_upgrade");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_wing_socket_upgrade * pData = new item_wing_socket_upgrade;
			int32 loop = 0;

			pData->SetLevel(fields[loop++].GetUInt8());
			pData->SetRate(fields[loop++].GetInt32());
			
			for (int32 i = 0; i < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++i)
			{
				pData->SetItemType(i, fields[loop++].GetUInt8());
				pData->SetItemIndex(i, fields[loop++].GetUInt16());
				pData->SetItemCount(i, fields[loop++].GetInt32());
			}

			pData->SetZen(fields[loop++].GetInt32());

			this->m_ItemWingSocketUpgradeMap[pData->GetLevel()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item wing socket upgrade definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadEarring()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Earring...");

	MAP_CLEAR(ItemEarringMap::iterator, this->m_ItemEarringMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_earring");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_earring * pData = new item_earring;
			int32 loop = 2;

			pData->SetItem(ITEMGET(fields[0].GetUInt8(), fields[1].GetUInt16()));
			pData->SetGroup(fields[loop++].GetUInt8());
			pData->SetLink(fields[loop++].GetUInt8());

			for (int32 i = 0; i < MAX_EARRING_OPTION; ++i)
			{
				pData->SetOption(i, fields[loop++].GetUInt8());
				pData->SetValue(i, fields[loop++].GetUInt8());
			}

			for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
			{
				pData->SetRate(i, fields[loop++].GetUInt8());
			}

			this->m_ItemEarringMap[pData->GetItem()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item earring definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadEarringOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Earring Option...");

	MAP_CLEAR(ItemEarringOptionMap::iterator, this->m_ItemEarringOptionMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_earring_option");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_earring_option * pData = new item_earring_option;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			pData->SetName(fields[loop++].GetString());
			pData->SetType(fields[loop++].GetUInt8());

			this->m_ItemEarringOptionMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item earring option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadEarringOptionValue()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Earring Option Value...");

	LIST_CLEAR(ItemEarringOptionValueList::iterator, this->m_ItemEarringOptionValueList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_earring_option_value");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_earring_option_value * pData = new item_earring_option_value;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			pData->SetGroup(fields[loop++].GetUInt8());
			pData->SetValue(fields[loop++].GetInt32());

			this->m_ItemEarringOptionValueList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item earring option value definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadEarringBonus()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Earring Bonus...");

	MAP_CLEAR(ItemEarringBonusMap::iterator, this->m_ItemEarringBonusMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_earring_bonus");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_earring_bonus * pData = new item_earring_bonus;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			pData->SetLink1(fields[loop++].GetUInt8());
			pData->SetLink2(fields[loop++].GetUInt8());
			
			for (int32 i = 0; i < MAX_EARRING_BONUS; ++i)
			{
				pData->SetOption(i, fields[loop++].GetUInt8());
				pData->SetValue(i, fields[loop++].GetUInt8());
			}

			this->m_ItemEarringBonusMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item earring bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadEarringUpgrade()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Earring Upgrade...");

	MAP_CLEAR(ItemEarringUpgradeMap::iterator, this->m_ItemEarringUpgradeMap);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_earring_upgrade");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_earring_upgrade * pData = new item_earring_upgrade;
			int32 loop = 0;

			pData->SetID(fields[loop++].GetUInt8());
			pData->SetRate(fields[loop++].GetInt32());

			for (int32 i = 0; i < MAX_EARRING_UPGRADE_MATERIAL; ++i)
			{
				uint8 type = fields[loop++].GetUInt8();
				uint16 index = fields[loop++].GetUInt16();

				pData->SetItem(i, ITEMGET(type, index));
				pData->SetAmount(i, fields[loop++].GetInt32());
			}

			pData->SetPrice(fields[loop++].GetInt32());

			this->m_ItemEarringUpgradeMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item earring upgrade definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadMasteryBonus()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Mastery Bonus...");

	LIST_CLEAR(ItemMasteryBonusList::iterator, this->m_ItemMasteryBonusList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_mastery_bonus");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_mastery_bonus * pData = new item_mastery_bonus;
			pData->SetType(fields[0].GetUInt8());
			pData->SetID(fields[1].GetUInt8());
			pData->SetName(fields[2].GetString());
			pData->SetValue(fields[3].GetInt32());
			pData->SetRate(fields[4].GetUInt16());

			this->m_ItemMasteryBonusList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item mastery bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadMysteriousStone()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Mysterious Stone...");

	LIST_CLEAR(ItemMysteriousStoneList::iterator, this->m_ItemMysteriousStoneList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_mysterious_stone");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_mysterious_stone * pData = new item_mysterious_stone;
			pData->SetType(fields[0].GetUInt8());
			pData->SetID(fields[1].GetUInt8());
			pData->SetLuck(fields[2].GetUInt8());
			pData->SetExcellent(fields[3].GetUInt8());
			pData->SetAncient(fields[4].GetUInt8());
			pData->SetRate(fields[5].GetUInt16());

			this->m_ItemMysteriousStoneList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item mysterious stone definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadGuardianOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Guardian Option...");

	LIST_CLEAR(ItemGuardianOptionList::iterator, this->m_ItemGuardianOptionList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_guardian_option");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_guardian_option * pData = new item_guardian_option;
			pData->SetID(fields[0].GetUInt8());
			pData->SetName(fields[1].GetString());

			this->m_ItemGuardianOptionList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item guardian option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadGuardianOptionValue()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Guardian Option Value...");

	LIST_CLEAR(ItemGuardianOptionValueList::iterator, this->m_ItemGuardianOptionValueList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_guardian_option_value");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_guardian_option_value * pData = new item_guardian_option_value;
			pData->SetType(fields[0].GetUInt8());
			pData->SetID(fields[1].GetUInt8());
			pData->SetFormulaData(fields[2].GetUInt8());
			pData->SetValueType(fields[3].GetUInt8());
			pData->SetCondition(fields[4].GetInt32());

			this->m_ItemGuardianOptionValueList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item guardian option value definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadGuardianEliteOption()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Guardian Elite Option...");

	LIST_CLEAR(ItemGuardianEliteOptionList::iterator, this->m_ItemGuardianEliteOptionList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_guardian_elite_option");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			item_guardian_elite_option * pData = new item_guardian_elite_option;
			pData->SetID(fields[0].GetUInt8());
			pData->SetName(fields[1].GetString());

			this->m_ItemGuardianEliteOptionList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item guardian elite option definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CItemMgr::LoadGuardianEliteOptionValue()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Item Guardian Elite Option Value...");

	LIST_CLEAR(ItemGuardianEliteOptionValueList::iterator, this->m_ItemGuardianEliteOptionValueList);
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM item_guardian_elite_option_value");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			item_guardian_elite_option_value * pData = new item_guardian_elite_option_value;
			pData->SetType(fields[loop++].GetUInt8());
			pData->SetGroup(fields[loop++].GetUInt8());

			for (int32 i = 0; i < MAX_GUARDIAN_ELITE_OPTIONS; ++i)
			{
				pData->SetID(i, fields[loop++].GetUInt8());
				pData->SetFormulaData(i, fields[loop++].GetUInt8());
				pData->SetValueType(i, fields[loop++].GetUInt8());
			}
			
			this->m_ItemGuardianEliteOptionValueList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u item guardian elite option value definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool CItemMgr::IsItem(uint16 item) const
{
	ItemTemplateMap::const_iterator it = this->m_item_template.find(item);

	if ( it != this->m_item_template.end() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint8 CItemMgr::LevelSmallConvert(int32 level)
{
	uint8 return_level = 0;

	switch (level)
	{
	case 15: case 14: {	return_level = 7; } break;
	case 13: case 12: {	return_level = 6; } break;
	case 11: { return_level = 5; } break;
	case 10: case 9: { return_level = 4; } break;
	case 8: case 7: { return_level = 3; } break;
	case 6: case 5: { return_level = 2; } break;
	case 4: case 3: { return_level = 1; } break;
	case 2: case 1: case 0: { return_level = 0; } break;
	}

	return return_level;
}

item_harmony_data * CItemMgr::GetRandomHarmonyOption(Item * item)
{
	uint8 harmony_item = item->GetHarmonyItem();

	if ( harmony_item == HARMONY_ITEM_NONE )
	{
		return nullptr;
	}

	RandomValue<item_harmony_data*> m_randomizer(nullptr);

	for ( ItemHarmonyDataList::const_iterator it = this->m_item_harmony_list[harmony_item].begin(); it != this->m_item_harmony_list[harmony_item].end(); ++it )
	{
		if ( item->GetLevel() < (*it)->GetMinLevel() )
		{
			continue;
		}

		if ( harmony_item == HARMONY_ITEM_WEAPON )
		{
			if ( (*it)->GetOption() == HARMONY_WEAPON_DECREASE_REQUIRED_STRENGTH )
			{
				if ( item->GetRequiredStat(STRENGTH) <= 0 )
				{
					continue;
				}
			}

			if ( (*it)->GetOption() == HARMONY_WEAPON_DECREASE_REQUIRED_AGILITY )
			{
				if ( item->GetRequiredStat(AGILITY) <= 0 )
				{
					continue;
				}
			}
		}

		if ( harmony_item == HARMONY_ITEM_STAFF )
		{
			if ( (*it)->GetOption() == HARMONY_STAFF_DECREASE_REQUIRED_STRENGTH )
			{
				if ( item->GetRequiredStat(STRENGTH) <= 0 )
				{
					continue;
				}
			}

			if ( (*it)->GetOption() == HARMONY_STAFF_DECREASE_REQUIRED_AGILITY )
			{
				if ( item->GetRequiredStat(AGILITY) <= 0 )
				{
					continue;
				}
			}
		}

		m_randomizer.AddValue((*it), (*it)->GetRate());
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_WEIGHT);
}

void CItemMgr::ConvertToBuffer(uint8 * buffer, Item const& item)
{
	this->ConvertToBuffer(buffer, &item);
}

void CItemMgr::ConvertToBuffer(uint8 * buffer, Item const* item)
{
	item->ConvertToBuffer(buffer);
}

item_template const* CItemMgr::GetItem(uint16 id) const
{
	ItemTemplateMap::const_iterator it = this->m_item_template.find(id);

	if ( it != this->m_item_template.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

item_template * CItemMgr::GetItem(uint16 id)
{
	ItemTemplateMap::iterator it = this->m_item_template.find(id);

	if ( it != this->m_item_template.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

item_add_option const* CItemMgr::GetItemAddOption(uint16 item) const
{
	ItemAddOptionMap::const_iterator it = this->m_item_add_option.find(item);

	if ( it != this->m_item_add_option.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

item_set_type const* CItemMgr::GetItemSetType(uint16 item) const
{
	ItemSetTypeMap::const_iterator it = this->m_item_set_type.find(item);

	if ( it != this->m_item_set_type.end() )
		return it->second;

	return nullptr;
}

item_set_option const* CItemMgr::GetItemSetOption(uint8 guid) const
{
	ItemSetOptionMap::const_iterator it = this->item_set_option_map.find(guid);

	if ( it != this->item_set_option_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

item_harmony_data * CItemMgr::GetHarmony(uint8 type, uint8 option) const
{
	if ( type >= HARMONY_ITEM_MAX )
		return nullptr;

	for ( ItemHarmonyDataList::const_iterator it = this->m_item_harmony_list[type].begin(); it != this->m_item_harmony_list[type].end(); ++it )
	{
		if ( (*it)->GetOption() != option )
			continue;

		return (*it);
	}

	return nullptr;
}

item_socket_seed * CItemMgr::GetSocketSeed(uint8 guid) const
{
	ItemSocketSeedMap::const_iterator it = this->m_item_socket_seed.find(guid);

	if ( it != this->m_item_socket_seed.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

item_socket_bonus * CItemMgr::GetSocketBonus(uint8 guid) const
{
	ItemSocketBonusMap::const_iterator it = this->m_item_socket_bonus.find(guid);

	if ( it != this->m_item_socket_bonus.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

uint8 CItemMgr::GetSocketBonus(Item* pItem, uint8 seed_level) const
{
	for ( ItemSocketBonusMap::const_iterator it = this->m_item_socket_bonus.begin(); it != this->m_item_socket_bonus.end(); ++it )
	{
		if ( !this->CheckSocketBonus(pItem, it->second) )
		{
			continue;
		}

		if ( !seed_level || (seed_level >= it->second->GetSeedLevelMin() && seed_level <= it->second->GetSeedLevelMax()) )
		{
			if ( pItem->GetSocketBonus() == it->second->GetGUID() )
			{
				return it->second->GetGUID();
			}
		}
	}
	
	RandomValue<uint8> m_RandomValue(-1);

	for ( ItemSocketBonusMap::const_iterator it = this->m_item_socket_bonus.begin(); it != this->m_item_socket_bonus.end(); ++it )
	{
		if ( !this->CheckSocketBonus(pItem, it->second) )
		{
			continue;
		}

		if ( seed_level >= it->second->GetSeedLevelMin() && seed_level <= it->second->GetSeedLevelMax() )
		{
			if (roll_chance_i(it->second->GetSuccessRate()) || (pItem->GetSocketBonus() == it->second->GetGUID()))
			{
				m_RandomValue.AddValue(it->second->GetGUID(), 0);
			}
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool CItemMgr::CheckSocketBonus(Item* pItem, item_socket_bonus const* pBonus) const
{
	if ( !pBonus )
	{
		return false;
	}

	if ( pItem->GetItemType() < pBonus->GetStartItemType() || pItem->GetItemType() > pBonus->GetEndItemType() )
	{
		return false;
	}

	SOCKET_SLOT_LOOP(i)
	{
		if ( pBonus->GetRequiredSocket(i) == 0 )
		{
			continue;
		}

		if ( pItem->GetSocket(i) == SOCKET_SLOT_NONE || pItem->GetSocket(i) == SOCKET_SLOT_EMPTY )
		{
			return false;
		}

		item_socket_seed const* pSeed = this->GetSocketSeed(pItem->GetSocket(i) % MAX_SOCKET_OPTION);

		if( !pSeed )
		{
			return false;
		}

		if ( pSeed->GetType() != pBonus->GetRequiredSocket(i) )
		{
			return false;
		}
	}

	return true;
}

item_socket_mix const* CItemMgr::GetSocketMix(uint8 rank) const
{
	ItemSocketMixMap::const_iterator it = this->m_item_socket_mix.find(rank);

	if ( it != this->m_item_socket_mix.end() )
	{
		return it->second;
	}
	else
	{	
		return nullptr;
	}
}

item_socket_upgrade const* CItemMgr::GetSocketUpgrade(uint8 id) const
{
	ItemSocketUpgradeMap::const_iterator it = this->m_item_socket_upgrade_map.find(id);

	if ( it != this->m_item_socket_upgrade_map.end() )
	{
		return it->second;
	}
	else
	{	
		return nullptr;
	}
}

item_socket_upgrade const* CItemMgr::GetSocketUpgrade(Item const* pItem) const
{
	for ( ItemSocketUpgradeMap::const_iterator it = this->m_item_socket_upgrade_map.begin(); it != this->m_item_socket_upgrade_map.end(); ++it )
	{
		if ( it->second->GetItem() != pItem->GetItem() )
		{
			continue;
		}

		if ( (pItem->GetLevel() < it->second->GetLevelMin()) || (pItem->GetLevel() > it->second->GetLevelMax()) )
		{
			continue;
		}

		if ( it->second->GetSkill() != 0 && it->second->GetSkill() != pItem->GetSkill() )
		{
			continue;
		}

		if ( pItem->GetOption() < it->second->GetOption() )
		{
			continue;
		}

		if ( pItem->GetSocketCount() < it->second->GetSocketMin() )
		{
			continue;
		}

		return it->second;
	}

	return nullptr;
}

item_socket_seed_sphere const* CItemMgr::GetSocketSeedSphere(uint16 item) const
{
	ItemSocketSeedSphereMap::const_iterator it = this->m_item_socket_seed_sphere_map.find(item);

	if ( it != this->m_item_socket_seed_sphere_map.end() )
	{
		return it->second;
	}
	else
	{	
		return nullptr;
	}
}

uint16 CItemMgr::GetSocketSeedSphere(uint8 type, uint8 level) const
{
	for ( ItemSocketSeedSphereMap::const_iterator it = this->m_item_socket_seed_sphere_map.begin(); it != this->m_item_socket_seed_sphere_map.end(); ++it )
	{
		if ( it->second->GetRank() == level && it->second->GetAttribute() == type )
		{
			return it->second->GetID();
		}
	}

	return -1;
}

void CItemMgr::GetSocketPackage(Player* pPlayer, uint8 * socket_count)
{
	if ( !pPlayer || !socket_count )
	{
		return;
	}

	for ( ItemSocketPackageMap::const_iterator it = this->m_item_socket_package.begin(); it != this->m_item_socket_package.end(); ++it )
	{
		if ( !it->second )
		{
			continue;
		}

		bool apply = true;

		for ( uint8 i = 0; i < MAX_SOCKET_TYPE; ++i )
		{
			if ( it->second->GetRequiredType(i) > socket_count[i] )
			{
				apply = false;
			}
		}

		if ( apply )
		{
			switch(it->second->GetGUID())
			{
			case 0:
				{
					pPlayer->IncreaseFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE, it->second->GetValue());
				} break;

			case 1:
				{
					pPlayer->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, it->second->GetValue());
				} break;
			}
		}
	}
}

SocketDataType CItemMgr::GetSocketByteOption(uint16 seed, uint8 level) const
{
	item_socket_seed_sphere const* pSeedData = this->GetSocketSeedSphere(seed);

	if ( !pSeedData )
	{
		return -1;
	}

	for ( ItemSocketSeedMap::const_iterator it = this->m_item_socket_seed.begin(); it != this->m_item_socket_seed.end(); ++it )
	{
		if ( it->second->GetType() == pSeedData->GetAttribute() && it->second->GetSubType() == level )
		{
			return pSeedData->GetRank() * MAX_SOCKET_OPTION + it->second->GetGUID();
		}
	}

	return -1;
}
	
uint8 CItemMgr::GetRandomOptionAncient(uint16 item)
{
	item_set_type const* item_data = sItemMgr->GetItemSetType(item);

	if ( !item_data )
	{
		return 0;
	}

	RandomValue<uint8> m_randomizer(0);

	for (uint8 i = 0; i < MAX_ITEM_ANCIENT_TYPE; ++i)
	{
		if (item_data->GetAncientID(i) == 0)
		{
			continue;
		}

		m_randomizer.AddValue(i + 1, 0);
	}

	uint8 option = m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);
	uint8 random = RANDOM(uint8(100));

	if ( option == 0 )
	{
		option = ANCIENT_TYPE_1;
	}
	else if ( option == 3 )
	{
		option = ANCIENT_TYPE_3;
	}

	if ( random < 80 )
	{
		option |= ANCIENT_OPTION_1;
	}
	else
	{
		option |= ANCIENT_OPTION_2;
	}

	return option;
}

float CItemMgr::CalculateDurability(uint16 id, uint8 level, uint8 exe, uint8 ancient, item_template const* item_info)
{
	if ( !item_info )
		item_info = this->GetItem(id);

	if ( !item_info )
		return 0.0f;

	if ( item_info->GetDurability() == 0 )
		return 0.0f;

	if( id == ITEMGET(12, 144) || id == ITEMGET(12, 146) || sPentagramSystem->IsPentagramItem(id) ) // Mithril Fragment,Elixir Fragment
	{
		return item_info->GetDurability();
	}

	float durability = item_info->GetDurability();
	
	/*switch ( level )
	{
	case 0: { durability = item_info->GetDurability() + level; } break;
	case 1: { durability = item_info->GetDurability() + level; } break;
	case 2: { durability = item_info->GetDurability() + level; } break;
	case 3: { durability = item_info->GetDurability() + level; } break;
	case 4: { durability = item_info->GetDurability() + level; } break;
	case 5: { durability = item_info->GetDurability() + level*2-4; } break;
	case 6: { durability = item_info->GetDurability() + level*2-4; } break;
	case 7: { durability = item_info->GetDurability() + level*2-4; } break;
	case 8: { durability = item_info->GetDurability() + level*2-4; } break;
	case 9: { durability = item_info->GetDurability() + level*2-4; } break;
	case 10: { durability = item_info->GetDurability() + level*2-3; } break;
	case 11: { durability = item_info->GetDurability() + level*2-1; } break;
	case 12: { durability = item_info->GetDurability() + level*2+2; } break;
	case 13: { durability = item_info->GetDurability() + level*2+6; } break;
	case 14: { durability = item_info->GetDurability() + level*2+11; } break;
	case 15: { durability = item_info->GetDurability() + level*2+17; } break;
	}*/

	for ( int32 i = 0; i < level; ++i )
	{
		if ( i < 14 )
		{
			if ( i < 13 )
			{
				if ( i < 12 )
				{
					if ( i < 11 )
					{
						if ( i < 10 )
						{
							if ( i < 9 )
							{
								if ( i < 4 )
									durability += 1.0f;
								else
									durability += 2.0f;
							}
							else
							{
								durability += 3.0f;
							}
						}
						else
						{
							durability += 4.0f;
						}
					}
					else
					{
						durability += 5.0f;
					}
				}
				else
				{
					durability += 6.0f;
				}
			}
			else
			{
				durability += 7.0f;
			}
		}
		else
		{
			durability += 8.0f;
		}
	}
	
	if (item_info->GetKind2() != ItemKind::WINGS_ANGEL_AND_DEVIL &&
		item_info->GetKind2() != ItemKind::WINGS_CONQUERORS &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_1 &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_2 &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_3 &&
		item_info->GetKind2() != ItemKind::WINGS_LVL_4 &&
		item_info->GetKind2() != ItemKind::MONSTER_WINGS &&
		item_info->GetKind2() != ItemKind::MINI_WINGS &&
		item_info->GetKind2() != ItemKind::CLOAK_LVL_2 &&
		item_info->GetKind2() != ItemKind::LORD_CAPE &&
		item_info->GetKind3() != ItemKind3::ARCHANGEL_WEAPON &&
		item_info->GetKind3() != ItemKind3::BLESSED_ARCHANGEL_WEAPON)
	{
		if ( ancient ) durability += 20;
		if ( exe ) durability += 15;
	}

	if ( durability > MAX_ITEM_DURABILITY )
	{
		durability = MAX_ITEM_DURABILITY;
	}

	return durability;
}

void CItemMgr::GetRandomSocketSeed(uint16 & item, uint8 & level)
{
	RandomValue<item_socket_seed const*> m_RandomValue(nullptr);

	for ( ItemSocketSeedMap::const_iterator it = this->m_item_socket_seed.begin(); it != this->m_item_socket_seed.end(); ++it )
	{
		m_RandomValue.AddValue(it->second, 0);
	}

	item_socket_seed const* pData = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

	if ( !pData )
	{
		return;
	}

	switch ( pData->GetType() )
	{
	case 1: item = ITEMGET(12,60); break;
	case 2: item = ITEMGET(12,61); break;
	case 3: item = ITEMGET(12,62); break;
	case 4: item = ITEMGET(12,63); break;
	case 5: item = ITEMGET(12,64); break;
	case 6: item = ITEMGET(12,65); break;
	}

	level = pData->GetSubType();

	return;
}

void CItemMgr::_insert_serial(SerialCreateData* add_serial)
{
	if (add_serial->type == serial_create_gremory_case && add_serial->item.GetSerial() && add_serial->gremory_case.recovery == 1)
	{
		add_serial->serial = add_serial->item.GetSerial();
		add_serial->serial_cash_shop = add_serial->item.GetSerialShop();
		add_serial->server = add_serial->item.GetSerialServer();
	}
	else
	{
		add_serial->serial = sObjectMgr->GenerateSerial();
		add_serial->serial_cash_shop = 0;

		if (add_serial->type == serial_create_cash_shop)
		{
			add_serial->serial_cash_shop = sObjectMgr->GenerateSerialCashShop();
		}

		add_serial->server = sGameServer->GetServerCode();
	}

	if ( add_serial->item.GetDBFlag() == 0 )
	{
		add_serial->item.SetDBFlag(this->GetItemFlagDB(add_serial->item.GetItem()));
	}

	this->_finally_create_item(add_serial);
}

void CItemMgr::ItemSerialCreate(Unit* owner, world_type world, coord_type x, coord_type y, Item const& item, bool only_owner, bool visible, bool to_queue)
{
	SerialCreateData * add_serial = new SerialCreateData(serial_create_map);
	add_serial->owner = owner;
	add_serial->world = world;
	add_serial->x.set(x);
	add_serial->y.set(y);
	add_serial->only_owner = only_owner;
	add_serial->item.clear();
	add_serial->item = item;
	add_serial->SetVisible(visible);
	add_serial->SetToQueue(to_queue);
	
	this->_insert_serial(add_serial);
}

void CItemMgr::ItemSerialCreateItem(Unit* owner, SerialCreateType type, Item const& item, uint8 slot, uint8 create_type)
{
	SerialCreateData * add_serial = new SerialCreateData(type);
	add_serial->owner = owner;
	add_serial->item = item;
	add_serial->SetSlot(slot);
	add_serial->SetCreateType(create_type);

	this->_insert_serial(add_serial);
}

void CItemMgr::ItemSerialCreateShop(Unit* owner, int32 shop, uint8 slot)
{
	SerialCreateData * add_serial = new SerialCreateData(serial_create_shop);
	add_serial->owner = owner;
	add_serial->shop.id = shop;
	add_serial->SetSlot(slot);
	
	this->_insert_serial(add_serial);
}

void CItemMgr::ItemSerialCreateRuudShop(Unit* owner, int32 shop, uint8 slot)
{
	SerialCreateData * add_serial = new SerialCreateData(serial_create_ruud_shop);
	add_serial->owner = owner;
	add_serial->shop.id = shop;
	add_serial->SetSlot(slot);
	
	this->_insert_serial(add_serial);
}

void CItemMgr::ItemSerialCreateCashShop(Unit* owner, uint32 package, uint32 option, uint32 product, uint32 account_id)
{
	SerialCreateData * add_serial = new SerialCreateData(serial_create_cash_shop);
	add_serial->owner = owner;
	add_serial->cash_shop.package = package;
	add_serial->cash_shop.option = option;
	add_serial->cash_shop.product = product;
	add_serial->cash_shop.account_id = account_id;
	
	this->_insert_serial(add_serial);
}

void CItemMgr::ItemSerialCreateGremoryCase(Unit* owner, Item const& item, uint8 inventory, uint8 type, uint8 recovery)
{
	SerialCreateData * add_serial = new SerialCreateData(serial_create_gremory_case);
	add_serial->owner = owner;
	add_serial->item = item;
	add_serial->gremory_case.inventory = inventory;
	add_serial->gremory_case.type = type;
	add_serial->gremory_case.recovery = recovery;

	this->_insert_serial(add_serial);
}

void CItemMgr::_finally_create_item(SerialCreateData* data)
{
	item_template const* pItemInfo = sItemMgr->GetItem(data->item.GetItem());

	if (pItemInfo)
	{
		if ((sPentagramSystem->IsPentagramItem(data->item.GetItem()) || pItemInfo->GetKind2() == ItemKind::ERRTEL || sPentagramSystem->IsPentagramMithril(data->item.GetItem())) && !data->item.IsValidElementalAttribute())
		{
			data->item.SetSocketBonus(Random(5) + 1);

			if (pItemInfo->GetKind2() == ItemKind::ERRTEL || sPentagramSystem->IsPentagramItem(data->item.GetItem()))
			{
				SOCKET_SLOT_LOOP(i)
				{
					if (data->item.GetSocket(i) != SOCKET_SLOT_NONE)
					{
						data->item.IncreaseSocketBonus(16);
					}
				}
			}
		}

		if (pItemInfo->GetKind2() == ItemKind::ERRTEL && data->item.GetSocket(0) == SOCKET_SLOT_NONE)
		{
			data->item.SetSocket(0, sPentagramSystem->GetRandomErrtelOption(data->item.GetItem(), 1));

			if (data->item.GetSocket(0) != SOCKET_SLOT_NONE)
			{
				data->item.SetSocketBonus((data->item.GetSocketBonus() % 16) + 16);
			}
		}
	}

	sMuunSystem->GetMuunItemSocketOption(data->item);
	sMuunSystem->GetMuunUtilSocketOption(data->item);
	sMuunSystem->SetMuunBonus(data->item);

	this->GenerateEarringOptions(&data->item, 1);
	
	if (pItemInfo)
	{
		if (pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			if (data->item.GetDurability() <= 0)
			{
				data->item.SetDurability(1);
			}
		}

		if (pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			this->GenerateGuardianOptions(&data->item);
		}
	}

	JewelMixData const* pData = sJewelMix->GetJewelData(&data->item);
	if (pData)
	{
		data->item.SetItem(pData->GetItem());
		data->item.SetLevel(0);
		data->item.SetDurability((pData->level.get() + 1) * 10);
	}

	if ( this->IsItemCreateData(data->item.GetItem(), data->item.GetLevel()) )
	{
		SQLTransaction trans = MuLogDatabase.BeginTransaction();

		PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_ITEM_CREATE);

		Unit* pOwner = data->owner;
		Player* pPlayer = pOwner ? pOwner->ToPlayer(): nullptr;

		if ( pPlayer )
		{
			stmt->setUInt32(0, pPlayer->GetAccountData()->GetGUID());
			stmt->setString(1, pPlayer->GetAccountData()->GetAccount());
			stmt->setUInt32(2, pPlayer->GetGUID());
			stmt->setString(3, pPlayer->GetName());
			stmt->setString(4, pPlayer->GetInterfaceState()->GetID());
		}
		else
		{
			stmt->setUInt32(0, 0);
			stmt->setString(1, "");
			stmt->setUInt32(2, 0);
			stmt->setString(3, "");
			stmt->setString(4, "");
		}

		if ( pOwner )
		{
			stmt->setInt16(5, pOwner->GetWorldId());
			stmt->setInt16(6, pOwner->GetX());
			stmt->setInt16(7, pOwner->GetY());
		}
		else
		{
			stmt->setInt16(5, -1);
			stmt->setInt16(6, -1);
			stmt->setInt16(7, -1);
		}

		data->item.Convert();

		stmt->setUInt16(8, data->server);
		stmt->setUInt32(9, data->serial);
		stmt->setUInt32(10, data->serial_cash_shop);
		stmt->setUInt16(11, data->item.GetItem());
		stmt->setUInt8(12, data->item.GetLevel());
		stmt->setString(13, data->item.BuildLog(0).c_str());
		trans->Append(stmt);
		
		MuLogDatabase.CommitTransaction(trans);
	}

	switch ( data->type.get() )
	{
	case serial_create_map:
		{
			this->_serial_create_map(data);
		} break;

	case serial_create_inventory:
		{
			this->_serial_create_inventory(data);
		} break;

	case serial_create_chaos_box:
		{
			this->_serial_create_chaos_box(data);
		} break;

	case serial_create_warehouse:
		{
			this->_serial_create_warehouse(data);
		} break;

	case serial_create_shop:
		{
			this->_serial_create_shop(data);
		} break;

	case serial_create_ruud_shop:
		{
			this->_serial_create_ruud_shop(data);
		} break;

	case serial_create_cash_shop:
		{
			this->_serial_create_cash_shop(data);
		} break;

	case serial_create_pentagram_mix:
		{
			this->_serial_create_pentagram_mix(data);
		} break;

	case serial_create_gremory_case:
		{
			this->_serial_create_gremory_case(data);
		} break;

	case serial_create_inventory_box:
		{
			this->_serial_create_inventory_box(data);
		} break;

	case serial_create_socket_mix:
		{
			this->_serial_create_socket_mix(data);
		} break;
	}

	delete data;
}

void CItemMgr::_serial_create_map(SerialCreateData* data)
{
	World * pWorld = sWorldMgr->GetWorld(data->world);

	if ( !pWorld )
		return;

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();

	pWorld->add_item(new_item, data->owner, data->x.get(), data->y.get(), data->only_owner, data->IsVisible(), data->IsToQueue());
}
	
void CItemMgr::_serial_create_inventory(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
	{
		return;
	}

	Player * owner = data->owner->ToPlayer();

	if ( !owner->Object::IsPlaying() )
	{
		return;
	}

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();

	bool stack = data->GetCreateType() != 3 && owner->InventoryAddItemStackOnBuy(new_item);
	uint8 inventory = 0;
	uint8 item_pos = -1;
	if (!stack)
	{
		item_pos = owner->AddItem(new_item, inventory, data->GetSlot(), ADD_ITEM_SEND_OK);
	}

	if (stack || item_pos != 0xFF)
	{
		owner->QuestMUObjectiveItemGet(new_item);

		if (item_pos != 0xFF)
		{
			switch (data->GetCreateType())
			{
			case 1:
			{
					  DARK_ANCIENT_ITEM_CREATE pMsg;
					  pMsg.slot = item_pos;
					  owner->SEND_PCT(pMsg);
			} break;
			}
		}
	}
	else
	{
		if ( sGameServer->IsFullInventoryToGremoryCase() && new_item.GetExpireDate() <= 0 )
		{
			new_item.SetExpireDate(time(nullptr) + 604800);
			new_item.SetPersonalStorePrice(GremoryCaseReward::EVENT);
			new_item.Convert();

			item_pos = owner->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->AddItem(new_item);

			if ( item_pos != uint8(-1) )
			{
				owner->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(item_pos)->Convert();
		
				sGremoryCase->ItemAdd(owner, GREMORY_CASE_TYPE_CHARACTER, item_pos);
			}
		}
	}
}
	
void CItemMgr::_serial_create_chaos_box(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
		return;

	Player * owner = data->owner->ToPlayer();

	if ( !owner->Object::IsPlaying() )
		return;

	if ( owner->GetInterfaceState()->GetID() != InterfaceData::PetTrainer && !owner->GetInterfaceState()->IsCommonMix() )
		return;

	if ( owner->GetInterfaceState()->GetState() != 1 )
		return;

	uint8 item_info[MAX_ITEM_INFO];

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();
	new_item.ConvertToBuffer(item_info);

	if ( data->GetSlot() == 1 )
	{
		owner->GetMixInventory()->Clear();
	}

	uint8 slot = owner->GetMixInventory()->AddItem(new_item);

	if ( slot != uint8(-1) )
	{
		owner->ChaosMixResult(CHAOS_MIX_RESULT_SUCCESS, item_info, slot);
	}

	if ( data->GetSlot() == 1 )
	{
		owner->GetInterfaceState()->SetState(0);
	}
}
	
void CItemMgr::_serial_create_warehouse(SerialCreateData* data)
{

}
	
void CItemMgr::_serial_create_shop(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
	{
		return;
	}

	Player * mPlayer = data->owner->ToPlayer();

	if ( !mPlayer->Object::IsPlaying() )
	{
		return;
	}

	if ( !sShopMgr->IsShop(mPlayer->GetInterfaceState()->GetID()) )
	{
		mPlayer->ItemBuyResult(0xFF);
		return;
	}

	Shop const* shop = sShopMgr->GetShop(data->shop.id);

	if ( !shop )
	{
		mPlayer->ItemBuyResult(0xFF);
		return;
	}

	if ( !shop_range(data->GetSlot()) )
	{
		mPlayer->ItemBuyResult(0xFF);
		return;
	}

	int32 tax_rate = 0;
	uint32 total_price = shop->GetItem(data->GetSlot())->GetPrice();

	if ( !total_price )
	{
		total_price = shop->GetItem(data->GetSlot())->GetBuyPrice();
	}

	if ( shop->IsFlag(SHOP_FLAG_SIEGE_TAX) )
	{
		tax_rate = total_price * sCastleSiege->GetTaxRateStore(mPlayer) / 100;

		if ( tax_rate < 0 )
			tax_rate = 0;
	}

	total_price += tax_rate;

	switch ( shop->GetType() )
	{
	case SHOP_TYPE_ZEN:
		{
			if ( !mPlayer->MoneyHave(total_price) )
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_RUUD:
	case SHOP_TYPE_RUUD_MISC:
		{
			if ( mPlayer->GetRuudMoney() < total_price )
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_WCOIN:
		{
			if ( mPlayer->GetCredits() < total_price )
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_GOBLIN_POINTS:
		{
			if ( mPlayer->GetGoblinPoints() < total_price )
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_RARE_TICKET:
		{
			if ( (mPlayer->GetInventory()->GetItemCount(ITEMGET(14, 58), 0) < total_price) )
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_BLESS:
		{
			if (mPlayer->GetInventory()->GetItemCount(JEWEL::BLESS, -1) < total_price)
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_SOUL:
		{
			if (mPlayer->GetInventory()->GetItemCount(JEWEL::SOUL, -1) < total_price)
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;

	case SHOP_TYPE_JEWEL_OF_CHAOS:
		{
			if (mPlayer->GetInventory()->GetItemCount(JEWEL::CHAOS, -1) < total_price)
			{
				mPlayer->ItemBuyResult();
				return;
			}
		} break;
	}

	Unit* pTarget = mPlayer->GetInterfaceState()->GetTarget();

	Item new_item = *shop->GetItem(data->GetSlot());
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);

	auto pItemInfo = sItemMgr->GetItem(new_item.GetItem());

	if (pItemInfo)
	{
		if ((sPentagramSystem->IsPentagramItem(new_item.GetItem()) || pItemInfo->GetKind2() == ItemKind::ERRTEL || sPentagramSystem->IsPentagramMithril(new_item.GetItem())) && new_item.GetSocketBonus() == uint8(-1))
		{
			new_item.SetSocketBonus(!sPentagramSystem->IsPentagramMithril(new_item.GetItem()) ? ((Random(5) + 1) | 16) : (Random(5) + 1));
		}
	}

	sMuunSystem->GetMuunItemSocketOption(new_item);
	sMuunSystem->GetMuunUtilSocketOption(new_item);

	new_item.CalculateDurability();
	new_item.Convert();

	uint8 item_pos = -1;
	uint8 inventory = 0;

	if ( !mPlayer->InventoryAddItemStackOnBuy(new_item) )
	{
		inventory = 0;
		item_pos = mPlayer->AddItem(new_item, inventory, -1, ADD_ITEM_SEND_OK);
	
		if ( item_pos == uint8(-1) )
		{
			mPlayer->ItemBuyResult();
			return;
		}
	}

	switch ( shop->GetType() )
	{
	case SHOP_TYPE_ZEN:
		{
			mPlayer->MoneyReduce(total_price);
		} break;

	case SHOP_TYPE_RUUD:
	case SHOP_TYPE_RUUD_MISC:
		{
			mPlayer->ReduceRuudMoney(total_price);
			mPlayer->SendRuudMoney();
		} break;

	case SHOP_TYPE_WCOIN:
		{
			mPlayer->ReduceCredits(total_price);
		} break;

	case SHOP_TYPE_GOBLIN_POINTS:
		{
			mPlayer->ReduceGoblinPoints(total_price);
		} break;

	case SHOP_TYPE_RARE_TICKET:
		{
			mPlayer->ItemDelete(ITEMGET(14, 58), -1, total_price);
		} break;

	case SHOP_TYPE_JEWEL_OF_BLESS:
		{
			mPlayer->ItemDelete(JEWEL::BLESS, -1, total_price);
		} break;

	case SHOP_TYPE_JEWEL_OF_SOUL:
		{
			mPlayer->ItemDelete(JEWEL::SOUL, -1, total_price);
		} break;

	case SHOP_TYPE_JEWEL_OF_CHAOS:
		{
			mPlayer->ItemDelete(JEWEL::CHAOS, -1, total_price);
		} break;
	}
		
	if ( shop->IsFlag(SHOP_FLAG_SIEGE_TAX) )
	{
		sCastleSiege->AddTributeMoney(tax_rate);
	}

	mPlayer->QuestMUObjectiveItemGet(new_item);

	LOG_ITEM_BUY(mPlayer, &new_item, "Bought item on NPC " + std::string(pTarget ? pTarget->GetName(): "") + " - Price " + std::to_string(long long(total_price)) + (shop->GetType() == SHOP_TYPE_WCOIN ? " WCoin" : shop->GetType() == SHOP_TYPE_GOBLIN_POINTS ? " Goblin Points" : " Zen"));

	uint8 item_buffer[MAX_ITEM_INFO];
	new_item.ConvertToBuffer(item_buffer);

	if ( item_pos != uint8(-1) && inventory == ITEM_INVENTORY_TYPE_NORMAL )
	{
		mPlayer->ItemBuyResult(item_pos, item_buffer);
	}
	else
	{
		mPlayer->ItemBuyResult();
	}
}

void CItemMgr::_serial_create_ruud_shop(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
	{
		return;
	}

	Player * pPlayer = data->owner->ToPlayer();

	if ( !pPlayer->Object::IsPlaying() )
	{
		return;
	}

	if ( !sShopMgr->IsShop(pPlayer->GetInterfaceState()->GetID()) )
	{
		sRuudSystem->ItemBuyResult(pPlayer);
		return;
	}

	Shop const* shop = sShopMgr->GetShop(data->shop.id);

	if ( !shop )
	{
		sRuudSystem->ItemBuyResult(pPlayer);
		return;
	}

	if ( !shop_range(data->GetSlot()) )
	{
		sRuudSystem->ItemBuyResult(pPlayer);
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(shop->GetItem(data->GetSlot())->GetItem());

	if ( !item_info )
	{
		sRuudSystem->ItemBuyResult(pPlayer);
		return;
	}

	if ( pPlayer->GetRuudMoney() < item_info->GetRuudPrice() )
	{
		sRuudSystem->ItemBuyResult(pPlayer);
		return;
	}

	Item new_item = *shop->GetItem(data->GetSlot());
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	uint8 excellent = new_item.GetExe();
	if ( excellent != 0 )
	{
		uint8 excellent_count = Random(excellent) + 1;
		if (new_item.GetItem() < ITEMGET(6, 0))
		{
			excellent = 32;
		}
		else
		{
			excellent = 1;
		}

		GetRandomExcOption(excellent, excellent_count);
		new_item.SetExe(excellent);
	}
	new_item.CalculateDurability();
	new_item.Convert();

	if (sMonsterSoul->IsSoulConverter(new_item.GetItem()))
	{
		new_item.SetExpireDate(time(nullptr) + sGameServer->GetMonsterSoulDurationTime());
	}

	uint8 item_pos = pPlayer->GetInventory()->AddItem(new_item);

	if ( item_pos != 0xFF )
	{
		pPlayer->ReduceRuudMoney(item_info->GetRuudPrice());
		pPlayer->SendRuudMoney();

		Unit* pTarget = pPlayer->GetInterfaceState()->GetTarget();

		pPlayer->QuestMUObjectiveItemGet(new_item);

		LOG_ITEM_BUY(pPlayer, pPlayer->GetInventory()->GetItem(item_pos), "Bought item on Ruud NPC " + std::string(pTarget ? pTarget->GetName(): "") + " - Price " + std::to_string(long long(item_info->GetRuudPrice())));

		pPlayer->SendInventory(item_pos);

		sMonsterSoul->SoulConverterPurchase(pPlayer, &new_item);
	}
	else
	{
		sRuudSystem->ItemBuyResult(pPlayer);
	}
}

void CItemMgr::_serial_create_cash_shop(SerialCreateData* data)
{
	if ( !data->owner )
	{
		return;
	}

	Player* pPlayer = data->owner->ToPlayer();

	if ( !pPlayer )
	{
		return;
	}

	sCashShopMgr->CashShopItemBuyCallBack(pPlayer, data);
}

void CItemMgr::_serial_create_pentagram_mix(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
		return;

	Player * owner = data->owner->ToPlayer();

	if ( !owner->Object::IsPlaying() )
		return;

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();
	
	uint8 item_pos = owner->GetInventory()->AddItem(new_item);

	if ( item_pos == 0xFF )
	{
		new_item.SetPersonalStorePrice(GremoryCaseReward::PROVIDE_ELEMENTAL_COMBINATION_SUCCESS_ITEM);

		if ( new_item.GetExpireDate() <= 0 )
		{
			new_item.SetExpireDate(time(nullptr) + 604800);
		}

		item_pos = owner->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->AddItem(new_item);

		if ( item_pos == 0xFF )
		{
			owner->GetInterfaceState()->SetState(0);
			owner->ChaosMixPentagramResult(254, 0);
			return;
		}

		sGremoryCase->ItemAdd(owner, GREMORY_CASE_TYPE_CHARACTER, item_pos);

		owner->GetInterfaceState()->SetState(0);
		owner->ChaosMixPentagramResult(1, 2);
	}
	else
	{
		owner->GetInventory()->GetItem(item_pos)->Convert();
		owner->SendInventory(item_pos);

		owner->GetInterfaceState()->SetState(0);
		owner->ChaosMixPentagramResult(1, 0);
	}
}

void CItemMgr::_serial_create_gremory_case(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
		return;

	Player * owner = data->owner->ToPlayer();

	if ( !data->gremory_case.recovery && !owner->Object::IsPlaying() )
		return;

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.SetPersonalStorePrice(data->gremory_case.type);
	new_item.CalculateDurability();

	uint8 add_type = new_item.raw.data_extra[2];
	new_item.raw.data_extra[2] = 0;

	if ( new_item.GetExpireDate() <= 0 )
	{
		new_item.SetExpireDate(time(nullptr) + 604800);
	}

	new_item.Convert();

	uint8 type = data->gremory_case.inventory;

	uint8 slot = owner->GetGremoryCase(type)->AddItem(new_item);

	if ( slot != uint8(-1) )
	{
		owner->GetGremoryCase(type)->GetItem(slot)->Convert();
		
		sGremoryCase->ItemAdd(owner, type, slot);
	}
	else
	{
		if ( data->gremory_case.recovery )
		{
			sLog->outError(LOG_ITEMS, "%s -- %s -- %s", __FUNCTION__, new_item.BuildLog(0).c_str(), owner->BuildLog().c_str());
		}
	}

	if (sDungeon->IsDungeon(owner->GetWorldId()))
	{
		SQLTransaction trans = MuLogDatabase.BeginTransaction();

		PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(LOG_STATEMENT_REWARD_DUNGEON);
		stmt->setUInt16(0, sGameServer->GetServerCode());
		stmt->setUInt32(1, owner->GetAccountData()->GetGUID());
		stmt->setString(2, owner->GetAccountData()->GetAccount());
		stmt->setUInt32(3, owner->GetGUID());
		stmt->setString(4, owner->GetName());
		stmt->setString(5, owner->BuildLogDB());
		stmt->setUInt8(6, add_type);
		stmt->setString(7, new_item.BuildLogDB(-1, true));
		trans->Append(stmt);

		MuLogDatabase.CommitTransaction(trans);
	}
}

void CItemMgr::_serial_create_inventory_box(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
	{
		return;
	}

	Player * owner = data->owner->ToPlayer();

	if ( !owner->Object::IsPlaying() )
	{
		return;
	}

	OPEN_BOX_RESULT pMsg(0xFF);

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();

	uint8 inventory = 0;
	uint8 item_pos = owner->AddItem(new_item, inventory, data->GetSlot(), ADD_ITEM_SEND_OK);

	if ( item_pos != 0xFF )
	{
		switch ( inventory )
		{
		case ITEM_INVENTORY_TYPE_NORMAL:
			{
				pMsg.result = 2;
				pMsg.slot = owner->GetInventory()->GetItem(item_pos)->GetItem();
			} break;

		case ITEM_INVENTORY_TYPE_EVENT:
			{
				pMsg.result = 3;
				pMsg.slot = owner->GetEventInventory()->GetItem(item_pos)->GetItem();
			} break;

		case ITEM_INVENTORY_TYPE_MUUN:
			{
				pMsg.result = 2;
				pMsg.slot = owner->GetMuunInventory()->GetItem(item_pos)->GetItem();
			} break;
		}

		owner->QuestMUObjectiveItemGet(new_item);
	}

	owner->SEND_PCT(pMsg);
}

void CItemMgr::_serial_create_socket_mix(SerialCreateData* data)
{
	if ( !data->owner || !data->owner->IsPlayer() )
		return;

	Player * owner = data->owner->ToPlayer();

	if ( !owner->Object::IsPlaying() )
		return;

	Item new_item = data->item;
	new_item.SetSerial(data->serial);
	new_item.SetSerialShop(data->serial_cash_shop);
	new_item.SetSerialServer(data->server);
	new_item.CalculateDurability();
	new_item.Convert();
	
	uint8 item_pos = owner->GetInventory()->AddItem(new_item);

	if ( item_pos == 0xFF )
	{
		new_item.SetPersonalStorePrice(GremoryCaseReward::EVENT);

		if ( new_item.GetExpireDate() <= 0 )
		{
			new_item.SetExpireDate(time(nullptr) + 604800);
		}

		item_pos = owner->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->AddItem(new_item);

		if ( item_pos == 0xFF )
		{
			owner->GetInterfaceState()->SetState(0);
			owner->ChaosMixSocketResult(254, 0);
			return;
		}

		sGremoryCase->ItemAdd(owner, GREMORY_CASE_TYPE_CHARACTER, item_pos);

		owner->GetInterfaceState()->SetState(0);
		owner->ChaosMixSocketResult(1, 2);
	}
	else
	{
		owner->GetInventory()->GetItem(item_pos)->Convert();
		owner->SendInventory(item_pos);

		owner->GetInterfaceState()->SetState(0);
		owner->ChaosMixSocketResult(1, 0, item_pos);
	}
}

item_transformation * CItemMgr::GetItemTransformation(uint16 item, uint8 level) const
{
	for (auto const& pData : this->m_item_transformation)
	{
		if (!pData)
		{
			continue;
		}

		if (ITEMGET(pData->GetType(), pData->GetIndex()) != item)
		{
			continue;
		}

		if (pData->GetLevel() != level)
		{
			continue;
		}

		return pData;
	}

	return nullptr;
}

bool CItemMgr::IsUsingTransformationItem(Player* pPlayer)
{
	if ( CC_MAP_RANGE(pPlayer->GetWorldId()) || IT_MAP_RANGE(pPlayer->GetWorldId()) )
	{
		return false;
	}

	item_transformation const* pItemTransformation = nullptr;

	wear_inventory_loop(i)
	{
		if ( !pPlayer->GetInventory()->GetItem(i)->IsItem() )
			continue;

		pItemTransformation = this->GetItemTransformation(pPlayer->GetInventory()->GetItem(i)->GetItem(), pPlayer->GetInventory()->GetItem(i)->GetLevel());

		if ( pItemTransformation )
			break;
	}

	uint16 skin = pItemTransformation ? pItemTransformation->GetSkin(): uint16(-1);

	if ( skin == uint16(-1) )
	{
		pPlayer->GetGameOption()->SetChangeSkin(1);
	}

	if ( pPlayer->GetSkin() == skin )
		return false;
	
	pPlayer->SetSkin(skin);
	pPlayer->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
	return true;
}

bool CItemMgr::RandomItemDrop(Monster* pMonster)
{
	RandomValue<item_drop_data const*> m_randomizer(nullptr);

	for ( ItemDropList::const_iterator it = this->m_item_drop.begin(); it != this->m_item_drop.end(); ++it )
	{
		item_drop_data const* data =  *it;

		if (!data)
		{
			continue;
		}

		if ( data->GetWorld() != pMonster->GetWorldId() && data->GetWorld() != uint16(-1) )
			continue;

		if ( data->GetMonster() != pMonster->GetClass() && data->GetMonster() != uint16(-1) )
			continue;

		if ( data->GetMonsterLevelMin() > pMonster->GetLevel() && data->GetMonsterLevelMin() != -1 )
			continue;

		if ( data->GetMonsterLevelMax() < pMonster->GetLevel() && data->GetMonsterLevelMax() != -1 )
			continue;

		if (!data->GetRate())
		{
			continue;
		}

		if ( !sEventMgr->IsSeasonEventOn(data->GetSeasonEvent()) )
			continue;

		if ( !sEventMgr->IsEventOn(data->GetEventID(), data->GetInvasionID()) )
			continue;
		
		m_randomizer.AddValue(data, 0);
	}

	item_drop_data const* pData = m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);

	if ( !pData )
	{
		return false;
	}

	Player* pPlayer = pMonster->GetMaxAttacker();

	if ( !pPlayer )
	{
		return false;
	}

	uint16 item = pData->GetItem();
	uint8 level = pData->GetRandomLevel();

	if ( pPlayer->GetHelper()->IsOffline() )
	{
		if ( !this->IsItemDrop(item, level) )
		{
			return false;
		}
	}
	
	int32 rate = pData->GetRate();

	int32 add_rate = static_cast<int32>(pPlayer->GetEffect(BUFF_OPTION_INCREASE_DROP_RATE));

	if ( sHappyHour->GetState() == HAPPY_HOUR_STATE_START )
	{
		add_rate += sGameServer->happy_hour_drop_add.get();
	}

	rate += (rate * add_rate) / 100;

	if ( rate > 10000 )
	{
		rate = 10000;
	}

	if (!roll_chance_i(rate, 10000))
	{
		return false;
	}

	INIT_SOCKET_DATA(socket);

	for ( uint8 i = 0; i < MAX_SOCKET_SLOT; ++i )
	{
		if ( Random(10000) < pData->GetSocket(i) )
		{
			socket[i] = SOCKET_SLOT_EMPTY;
		}
		else
		{
			 break;
		}
	}

	Item new_item(item, level, pData->GetDurability(), pData->GetSkill(), pData->GetLuck(), pData->GetRandomOption(),
		GetRandomExcOption(pData->GetRandomExcellent()), pData->GetAncient(), socket);

	new_item.BuildPentagramAttribute(pMonster);

	if ( !AllowItemDropInWorld(pMonster->GetWorldId(), new_item) )
	{
		return false;
	}

	new_item.Convert();

	LOG_JEWEL_DROP(pPlayer, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), &new_item);

	this->ItemSerialCreate(pPlayer, pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), new_item, false);

	return true;
}

bool CItemMgr::IsItemMixLucky(uint16 item, uint8 level) const
{
	for ( ItemMixLuckyList::const_iterator it = this->m_item_mix_lucky.begin(); it != this->m_item_mix_lucky.end(); ++it )
	{
		if ( (*it)->GetTicket() != item )
			continue;

		if ( (*it)->GetTicketLevel() != level )
			continue;

		return true;
	}

	return false;
}
	
item_mix_lucky_data * CItemMgr::GetRandomItemMixLucky(uint16 item, uint8 level, Player* pPlayer)
{
	RandomValue<item_mix_lucky_data*> m_randomizer(nullptr);
	RandomValue<item_mix_lucky_data*> m_exclusive_randomizer(nullptr);

	for ( ItemMixLuckyList::const_iterator it = this->m_item_mix_lucky.begin(); it != this->m_item_mix_lucky.end(); ++it )
	{
		if ( (*it)->GetTicket() != item )
			continue;

		if ( (*it)->GetTicketLevel() != level )
			continue;

		item_template const* item_info = this->GetItem((*it)->GetItem());

		if ( !item_info )
			continue;

		if ( !item_info->is_class_allowed(pPlayer->GetClass(), pPlayer->GetChangeUP(0), pPlayer->GetChangeUP(1), pPlayer->GetChangeUP(2)) )
			continue;

		if ( item_info->IsExclusiveClass(pPlayer->GetClass()) )
		{
			m_exclusive_randomizer.AddValue((*it), 0);
		}

		m_randomizer.AddValue((*it), 0);
	}

	item_mix_lucky_data* LuckyData = m_exclusive_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);

	if ( !LuckyData )
	{
		LuckyData = m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);
	}
	
	return LuckyData;
}

bool CItemMgr::IsLuckyItem(uint16 item) const
{
	for ( ItemMixLuckyList::const_iterator it = this->m_item_mix_lucky.begin(); it != this->m_item_mix_lucky.end(); ++it )
	{
		if ( (*it)->GetItem() == item )
		{
			return true;
		}
	}

	return false;
}

bool CItemMgr::GetRandomSetPart(uint16 index, uint16 & item)
{
	item_template const* item_info = nullptr;
	int32 count = 10;

	while(!item_info && count-- > 0)
	{
		item = ITEMGET((Random(5) + 7), index);
		item_info = this->GetItem(item);
	}

	return item_info ? true: false;
}

struct RandomAncientItem
{
	uint16 item;
	uint8 ancient;
};

bool CItemMgr::GetRandomAncientSetPart(uint8 guid, uint16 & item, uint8 & ancient)
{
	item_set_option const* item_info = this->GetItemSetOption(guid);

	if ( !item_info )
		return false;
	
	std::vector<RandomAncientItem> randomizer;
	RandomAncientItem add_item;

	for ( ItemSetTypeMap::const_iterator it = this->m_item_set_type.begin(); it != this->m_item_set_type.end(); ++it )
	{
		if ( it->second->GetAncientID(0) == guid )
		{
			add_item.item = it->second->GetItem();
			add_item.ancient = ANCIENT_TYPE_1;
			randomizer.push_back(add_item);
		}

		if ( it->second->GetAncientID(1) == guid )
		{
			add_item.item = it->second->GetItem();
			add_item.ancient = ANCIENT_TYPE_2;
			randomizer.push_back(add_item);
		}

		if ( it->second->GetAncientID(2) == guid )
		{
			add_item.item = it->second->GetItem();
			add_item.ancient = ANCIENT_TYPE_3;
			randomizer.push_back(add_item);
		}
	}

	if ( randomizer.empty() )
		return false;

	add_item = randomizer[Random(randomizer.size())];

	item = add_item.item;
	ancient = add_item.ancient;

	return true;
}

uint8 CItemMgr::GetItemMaxStack(uint16 item) const
{
	item_template const* item_info = this->GetItem(item);

	if ( item_info )
	{
		return item_info->GetStackData()->GetMaxStack();
	}
	else
	{
		return 0;
	}
}

bool CItemMgr::IsItemFlag(uint16 item, uint8 level, uint32 flag) const
{
	item_template const* item_info = this->GetItem(item);

	if ( item_info )
	{
		return (item_info->GetFlags(level) & flag) != 0;
	}
	else
	{
		return false;
	}
}

int32 CItemMgr::GetWingDamage(uint16 item, uint8 level) const
{
	ItemWingValuesMap::const_iterator it = this->m_item_wing_values.find(item);

	if ( it != this->m_item_wing_values.end() )
	{
		return it->second->GetDamageAdd() + (it->second->GetDamageAddLvl() * level);
	}
	else
	{
		return 0;
	}
}
	
int32 CItemMgr::GetWingAbsorb(uint16 item, uint8 level) const
{
	ItemWingValuesMap::const_iterator it = this->m_item_wing_values.find(item);

	if ( it != this->m_item_wing_values.end() )
	{
		return it->second->GetAbsorbAdd() + (it->second->GetAbsorbAddLvl() * level);
	}
	else
	{
		return 0;
	}
}

bool CItemMgr::IsItemDrop(uint16 item, uint8 level) const
{
	for ( ItemReducedDropList::const_iterator it = this->m_item_reduced_drop_list.begin(); it != this->m_item_reduced_drop_list.end(); ++it )
	{
		if ( (*it)->GetItem() == item && (*it)->GetLevel() == level )
		{
			return roll_chance_i((*it)->GetRate(), 10000);
		}
	}

	return true;
}

bool CItemMgr::IsItemCreateData(uint16 item, uint8 level) const
{
	for ( ItemCreateDataList::const_iterator it = this->m_item_create_data_list.begin(); it != this->m_item_create_data_list.end(); ++it )
	{
		if ( (*it)->GetItem() == item && (*it)->GetLevel() == level )
		{
			return true;
		}
	}

	return false;
}

bool CItemMgr::IsSeedSphere(uint16 item)
{
	item_socket_seed_sphere const* pSeedSphere = this->GetSocketSeedSphere(item);

	return pSeedSphere != nullptr;
}

uint8 CItemMgr::GetItemInventoryType(uint16 item) const
{
	item_template const* item_info = this->GetItem(item);

	if ( item_info )
	{
		return item_info->GetInventoryType();
	}
	else
	{
		return ITEM_INVENTORY_TYPE_MAX;
	}
}

uint32 CItemMgr::GetItemFlagDB(uint16 item) const
{
	ItemFlagDBMap::const_iterator itr = this->m_item_flagdb_map.find(item);

	if ( itr != this->m_item_flagdb_map.end() )
	{
		return itr->second->GetFlags();
	}
	else
	{
		return 0;
	}
}

int32 CItemMgr::GetItemSetValue(uint16 option, uint8 level) const
{
	if (level < 0 || level > MAX_ITEM_LEVEL)
	{
		return 0;
	}

	ItemSetValuesMap::const_iterator itr = this->m_ItemSetValuesMap.find(option);

	if (itr != this->m_ItemSetValuesMap.end())
	{
		return itr->second->GetValue(level);
	}
	else
	{
		return 0;
	}
}

int32 CItemMgr::GetWingGradedOptionValue(uint8 id, uint8 level) const
{
	if (level < 0 || level >= MAX_WING_GRADED_OPTION_LEVEL)
	{
		return 0;
	}

	ItemWingGradedOptionMap::const_iterator itr = this->m_ItemWingGradedOptionMap.find(id);

	if (itr != this->m_ItemWingGradedOptionMap.end())
	{
		return itr->second->GetValue(level);
	}
	else
	{
		return 0;
	}
}

SocketDataType CItemMgr::GenerateWingGradedOption(SocketDataType const* socket) const
{
	RandomValue<SocketDataType> m_RandomValue(SOCKET_SLOT_NONE);

	for (ItemWingGradedOptionMap::const_iterator itr = this->m_ItemWingGradedOptionMap.begin(); itr != this->m_ItemWingGradedOptionMap.end(); ++itr)
	{
		item_wing_graded_option const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		bool add = true;

		SOCKET_SLOT_LOOP(i)
		{
			if (socket[i] != SOCKET_SLOT_NONE && (socket[i] / 16) == pData->GetID())
			{
				add = false;
			}
		}

		if (!add)
		{
			continue;
		}

		m_RandomValue.AddValue(pData->GetID(), 0);
	}

	SocketDataType value = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

	if (value != SOCKET_SLOT_NONE)
	{
		value *= 16;
		value += Random(MAX_WING_GRADED_OPTION_STEP);
	}

	return value;
}

int32 CItemMgr::GetWingSocketOptionValue(uint8 type, uint8 id, uint8 level) const
{
	if (level < 0 || level >= MAX_WING_SOCKET_OPTION_LEVEL)
	{
		return 0;
	}

	for (ItemWingSocketOptionList::const_iterator itr = this->m_ItemWingSocketOptionList.begin(); itr != this->m_ItemWingSocketOptionList.end(); ++itr)
	{
		item_wing_socket_option const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData->GetValue(level);
		}
	}

	return 0;
}

SocketDataType CItemMgr::GenerateWingSocketOption(uint8 type) const
{
	RandomValue<SocketDataType> m_RandomValue(0);

	for (ItemWingSocketOptionList::const_iterator itr = this->m_ItemWingSocketOptionList.begin(); itr != this->m_ItemWingSocketOptionList.end(); ++itr)
	{
		item_wing_socket_option const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type)
		{
			m_RandomValue.AddValue(pData->GetID() * 16, pData->GetRate());
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);
}

item_wing_socket_upgrade const* CItemMgr::GetWingSocketUpgrade(uint8 level) const
{
	ItemWingSocketUpgradeMap::const_iterator itr = this->m_ItemWingSocketUpgradeMap.find(level);

	if (itr != this->m_ItemWingSocketUpgradeMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

item_earring const* CItemMgr::GetEarringData(uint16 item) const
{
	ItemEarringMap::const_iterator itr = this->m_ItemEarringMap.find(item);

	if (itr != this->m_ItemEarringMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

item_earring_option const* CItemMgr::GetEarringOption(uint8 id) const
{
	ItemEarringOptionMap::const_iterator itr = this->m_ItemEarringOptionMap.find(id);

	if (itr != this->m_ItemEarringOptionMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

item_earring_option_value const* CItemMgr::GetEarringOptionValue(uint8 id, uint8 group) const
{
	for (ItemEarringOptionValueList::const_iterator itr = this->m_ItemEarringOptionValueList.begin(); itr != this->m_ItemEarringOptionValueList.end(); ++itr)
	{
		item_earring_option_value const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetID() == id && pData->GetGroup() == group)
		{
			return pData;
		}
	}

	return nullptr;
}

item_earring_bonus const* CItemMgr::GetEarringBonus(uint8 link_1, uint8 link_2) const
{
	for (ItemEarringBonusMap::const_iterator itr = this->m_ItemEarringBonusMap.begin(); itr != this->m_ItemEarringBonusMap.end(); ++itr)
	{
		item_earring_bonus const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if ((pData->GetLink1() == link_1 && pData->GetLink2() == link_2) ||
			(pData->GetLink1() == link_2 && pData->GetLink2() == link_1))
		{
			return pData;
		}
	}
	
	return nullptr;
}

item_earring_upgrade const* CItemMgr::GetEarringUpgrade(uint8 level) const
{
	ItemEarringUpgradeMap::const_iterator itr = this->m_ItemEarringUpgradeMap.find(level);

	if (itr != this->m_ItemEarringUpgradeMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

void CItemMgr::GenerateEarringOptions(Item* pItem, int32 min_option)
{
	if (!pItem)
	{
		return;
	}

	item_earring const* pEarring = this->GetEarringData(pItem->GetItem());

	if (!pEarring)
	{
		return;
	}

	if (pItem->GetSocketCount() != 0)
	{
		return;
	}

	while (pItem->GetActiveSocketCount() < min_option)
	{
		for (int32 i = 0; i < MAX_SOCKET_SLOT; ++i)
		{
			if (roll_chance_i(pEarring->GetRate(i)))
			{
				RandomValue<uint8> m_RandomOption(-1);

				for (int32 n = 0; n < MAX_EARRING_OPTION; ++n)
				{
					item_earring_option const* pEarringOption = this->GetEarringOption(pEarring->GetOption(n));

					if (!pEarringOption)
					{
						continue;
					}

					bool add = true;

					for (int32 h = 0; h < MAX_SOCKET_SLOT; ++h)
					{
						if (pItem->GetSocket(h) == SOCKET_SLOT_NONE)
						{
							continue;
						}

						if ((pItem->GetSocket(h) / MAX_EARRING_OPTION_LEVEL) == pEarring->GetOption(n))
						{
							add = false;
							break;
						}
					}

					if (add)
					{
						m_RandomOption.AddValue(n, 0);
					}
				}

				uint8 option_slot = m_RandomOption.GetRandomValue(RANDOM_POOL_RANDOM);

				if (option_slot >= 0 && option_slot < MAX_EARRING_OPTION)
				{
					for (int32 h = 0; h < MAX_SOCKET_SLOT; ++h)
					{
						if (pItem->GetSocket(h) == SOCKET_SLOT_NONE)
						{
							pItem->SetSocket(h, pEarring->GetOption(option_slot) * MAX_EARRING_OPTION_LEVEL | pEarring->GetValue(option_slot));
							break;
						}
					}
				}
			}
		}
	}
}

bool CItemMgr::IsMasteryBonusMax(uint8 type, uint8 id) const
{
	uint8 max_id = 0;

	for (ItemMasteryBonusList::const_iterator itr = this->m_ItemMasteryBonusList.begin(); itr != this->m_ItemMasteryBonusList.end(); ++itr)
	{
		item_mastery_bonus const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (pData->GetID() > max_id)
		{
			max_id = pData->GetID();
		}
	}

	return max_id == id;
}

uint8 CItemMgr::GetRandomMasteryBonus(uint8 type) const
{
	RandomValue<uint8> m_RandomValue(-1);

	for (ItemMasteryBonusList::const_iterator itr = this->m_ItemMasteryBonusList.begin(); itr != this->m_ItemMasteryBonusList.end(); ++itr)
	{
		item_mastery_bonus const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type)
		{
			m_RandomValue.AddValue(pData->GetID(), pData->GetRate());
		}
	}

	m_RandomValue.AddValue(-1, 10000);

	return m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);
}

item_mastery_bonus const* CItemMgr::GetMasteryBonus(uint8 type, uint8 id) const
{
	for (ItemMasteryBonusList::const_iterator itr = this->m_ItemMasteryBonusList.begin(); itr != this->m_ItemMasteryBonusList.end(); ++itr)
	{
		item_mastery_bonus const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

bool CItemMgr::IsMysteriousStoneMax(uint8 type, Item const* pItem) const
{
	uint8 max_id = 0;
	uint8 current_id = 0;

	for (ItemMysteriousStoneList::const_iterator itr = this->m_ItemMysteriousStoneList.begin(); itr != this->m_ItemMysteriousStoneList.end(); ++itr)
	{
		item_mysterious_stone const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (pData->GetID() > max_id)
		{
			max_id = pData->GetID();
		}

		if (pData->GetExcellent() != 0 && pItem->GetExe() != pData->GetExcellent())
		{
			continue;
		}

		if (pData->GetAncient() != 0 && (pItem->GetAncient() & pData->GetAncient()) == 0)
		{
			continue;
		}

		if (pData->GetLuck() != pItem->GetLuck())
		{
			continue;
		}

		current_id = pData->GetID();
	}

	return current_id == max_id;
}

item_mysterious_stone const* CItemMgr::GetRandomMysteriousStone(uint8 type) const
{
	RandomValue<item_mysterious_stone const*> m_RandomValue(nullptr);

	for (ItemMysteriousStoneList::const_iterator itr = this->m_ItemMysteriousStoneList.begin(); itr != this->m_ItemMysteriousStoneList.end(); ++itr)
	{
		item_mysterious_stone const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		m_RandomValue.AddValue(pData, pData->GetRate());
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);
}

void CItemMgr::AddSetOption(uint16 id)
{
	ItemSetValuesMap::const_iterator itr = this->m_ItemSetValuesMap.find(id);

	if (itr != this->m_ItemSetValuesMap.end())
	{
		return;
	}

	item_set_values* pData = new item_set_values;
	pData->SetID(id);

	for (int32 i = 0; i < MAX_ITEM_LEVEL + 1; ++i)
	{
		pData->SetValue(i, sFormulaMgr->GetOrCreateValue(4, id, i));
	}

	this->m_ItemSetValuesMap[id] = pData;
}

item_guardian_option_value const* CItemMgr::GetGuardianOptionValue(uint8 type, uint8 id) const
{
	for (ItemGuardianOptionValueList::const_iterator itr = this->m_ItemGuardianOptionValueList.begin(); itr != this->m_ItemGuardianOptionValueList.end(); ++itr)
	{
		item_guardian_option_value const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

item_guardian_elite_option_value const* CItemMgr::GetGuardianEliteOptionValue(uint8 type, uint8 id) const
{
	for (ItemGuardianEliteOptionValueList::const_iterator itr = this->m_ItemGuardianEliteOptionValueList.begin(); itr != this->m_ItemGuardianEliteOptionValueList.end(); ++itr)
	{
		item_guardian_elite_option_value const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetGroup() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

void CItemMgr::GenerateGuardianOptions(Item* pItem, int32 min_options)
{
	if (pItem->GetSocketCount() != 0)
	{
		return;
	}

	int32 options = Random(3) + 1;
	
	if (options < min_options)
	{
		options = min_options;
	}

	for (int32 i = 0; i < options; ++i)
	{
		RandomValue<uint8> m_RandomOption(-1);

		ItemGuardianOptionValueList const& list = this->GetGuardianOptionValueList();
		for (ItemGuardianOptionValueList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			item_guardian_option_value const* pData = *itr;

			if (!pData)
			{
				continue;
			}

			bool add = true;

			for (int32 h = 0; h < 4; ++h)
			{
				if (pItem->GetSocket(h) == SOCKET_SLOT_NONE)
				{
					continue;
				}

				if (pItem->GetSocket(h) == pData->GetID())
				{
					add = false;
					break;
				}
			}

			if (add)
			{
				m_RandomOption.AddValue(pData->GetID(), 0);
			}
		}

		uint8 option_id = m_RandomOption.GetRandomValue(RANDOM_POOL_RANDOM);

		if (option_id != uint8(-1))
		{
			for (int32 h = 0; h < 4; ++h)
			{
				if (pItem->GetSocket(h) == SOCKET_SLOT_NONE)
				{
					pItem->SetSocket(h, option_id);
					break;
				}
			}
		}
	}

	RandomValue<uint8> m_RandomEliteValue(0);

	ItemGuardianEliteOptionValueList const& elite_list = this->GetGuardianEliteOptionValueList();
	for (ItemGuardianEliteOptionValueList::const_iterator itr = elite_list.begin(); itr != elite_list.end(); ++itr)
	{
		item_guardian_elite_option_value const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		m_RandomEliteValue.AddValue(pData->GetGroup(), 0);
	}

	pItem->SetSocket(4, m_RandomEliteValue.GetRandomValue(RANDOM_POOL_RANDOM));
}