/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterAggro.cpp"
*
*/

void Monster::CalculateMaxAttacker()
{
	if ( this->IsLive() )
	{
		return;
	}

	this->SetMaxAttacker(nullptr);
	this->SetMaxAttackerGUID(0);
	this->ResetMaxAttackerName();

	DamageDataList const& damage_data = this->GetThreatManager()->GetDamageData();

	if ( damage_data.empty() )
	{
		return;
	}

	int64 max_damage = -1;

	for ( DamageDataList::const_iterator it = damage_data.begin(); it != damage_data.end(); ++it )
	{
		DamageData const* data = (*it);

		if ( !data )
		{
			continue;
		}

		if ( (GetTickCount() - data->GetLastAttackTime()) > (MINUTE * IN_MILLISECONDS) )
		{
			continue;
		}

		if ( data->GetTotalDamage() <= max_damage )
		{
			continue;
		}

		this->SetMaxAttackerGUID(data->GetGuid());
		this->ResetMaxAttackerName();
		this->SetMaxAttackerName(data->GetName());

		Player* pPlayer = sObjectMgr->FindPlayerByGUID(data->GetGuid());

		if ( pPlayer )
		{
			this->SetMaxAttacker(pPlayer);
		}

		max_damage = data->GetTotalDamage();
	}
}

Player * Monster::GetMaxAggro()
{
	uint32 guid = GetThreatManager()->GetMostHatedID();
	
	return guid ? sObjectMgr->FindPlayerByGUID(guid): nullptr;
}

void Monster::KillExperience()
{
	Player* pMaxAttacker = this->GetMaxAttacker();

	if ( !pMaxAttacker )
	{
		return;
	}

	DamageDataList const& damage_data = this->GetThreatManager()->GetDamageData();

	if ( damage_data.empty() )
	{
		return;
	}

	Player* pPlayer = nullptr;
	Party* pParty = pMaxAttacker->GetParty();
	int64 damage_deal = 0;
	DamageData const* data = nullptr;

	for ( DamageDataList::const_iterator it = damage_data.begin(); it != damage_data.end(); ++it )
	{
		data = *it;

		if ( !data )
			continue;

		if ( !data->GetTotalDamage() )
			continue;

		if ( data->GetGuid() != pMaxAttacker->GetGUID() )
		{
			if ( !pParty )
				continue;

			if ( !pParty->IsMember(data->GetGuid()) )
				continue;
		}

		if ( !(pPlayer = sObjectMgr->FindPlayerByGUID(data->GetGuid()) ) )
			continue;

		if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() )
			continue;

		if ( !this->SameDimension(pPlayer) )
			continue;

		damage_deal += data->GetTotalDamage();
	}

	if ( pParty )
	{
		pMaxAttacker->GivePartyExperience(this, damage_deal);
	}
	else
	{
		pMaxAttacker->GiveSingleExperience(this, damage_deal);
	}
}

void Monster::DropItem()
{
	if ( this->GetAI() && this->GetAI()->Drop() )
	{
		return;
	}

	Player* pMaxAttacker = this->GetMaxAttacker();

	if (pMaxAttacker)
	{
		if (pMaxAttacker->QuestEvolutionMonsterDrop(this))
		{
			return;
		}

		if (pMaxAttacker->QuestMUObjectiveItemDrop(this))
		{
			return;
		}
	}

	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MONSTER, this, this->GetItemBag()) == ITEM_BAG_RESULT_SUCCESS )
	{
		return;
	}

	if (sItemBagMgr->ExecuteItemBag(ITEM_BAG_MONSTER_ALL, this, this->GetItemBag()) == ITEM_BAG_RESULT_SUCCESS)
	{
		return;
	}

	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_WORLD, this, "") == ITEM_BAG_RESULT_SUCCESS )
	{
		return;
	}

	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_WORLD_2, this, "") == ITEM_BAG_RESULT_SUCCESS )
	{
		return;
	}

	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MONSTER_RATE, this, "") == ITEM_BAG_RESULT_SUCCESS )
	{
		return;
	}

	if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_EVENT, this, "") == ITEM_BAG_RESULT_SUCCESS )
	{
		return;
	}

	if ( !pMaxAttacker )
	{
		return;
	}

	if ( sArkaWar->ItemDrop(pMaxAttacker, this) )
	{
		return;
	}

	if ( this->GetLevel() <= 20 && (Random<uint16>(10000) < 2000) )
	{
		sItemMgr->ItemSerialCreate(pMaxAttacker, this->GetWorldId(), this->GetX(), this->GetY(), Item(ITEMGET(14, 0), 0, 1.0f), false);
	}

	int32 item_rate = this->GetItemRate();

	if ( item_rate > 0 )
	{
		//int32 ext_drop_per = rand()%2000;
		//int32 ext_drop_per = Random(sGameServer->item_drop_rate_exe.get());
		int32 ext_drop_per = sGameServer->item_drop_rate_exe.get();
		int32 item_drop_per = sGameServer->item_drop_rate_normal.get();

		int32 add_rate = static_cast<int32>(pMaxAttacker->GetEffect(BUFF_OPTION_INCREASE_DROP_RATE));

		if ( sHappyHour->GetState() == HAPPY_HOUR_STATE_START )
		{
			add_rate += sGameServer->happy_hour_drop_add.get();
		}

		add_rate += sCharacterBase->GetBonus(pMaxAttacker, true);

		if ( add_rate > 0 )
		{
			item_drop_per += item_drop_per * add_rate / 100; //ItemDropPer = ItemDropPer * iDropRate / 100; //Increase Item Drop
			//ext_drop_per = Random((sGameServer->item_drop_rate_exe.get() / add_rate) * 100);
			ext_drop_per -= add_rate * sGameServer->item_drop_rate_exe.get() / 100; //ExtDropPer = rand()%((2000/iDropRate)*100); //Increase Exc Drop
		}

		if ( pMaxAttacker->HasBuff(BUFF_FEEL_TIRED) )
		{
			item_drop_per = item_drop_per * 50 / 100;
			//ext_drop_per = Random(sGameServer->item_drop_rate_exe.get() * 150 / 100);
			ext_drop_per = ext_drop_per * 150 / 100;
		}
		if ( pMaxAttacker->HasBuff(BUFF_FEEL_EXHAUSTED) )
		{
			item_drop_per = 0;
			ext_drop_per = 0;
		}

		ext_drop_per = Random(ext_drop_per);
		bool item_drop = false;
		Item item;
		item.clear();

		if ( ext_drop_per == 1 )
		{
			if ( Random(item_rate) < item_drop_per )
			{
				item_drop = sMonsterMgr->GenerateItem(item, this->GetLevel() - 25, true);
			}
		}
		else
		{
			if ( Random(item_rate) < item_drop_per )
			{
				if ( sItemMgr->RandomItemDrop(this) )
				{
					return;
				}

				item_drop = sMonsterMgr->GenerateItem(item, this->GetLevel(), false);
			}
		}

		item_template const* item_info = nullptr;

		if ( item_drop )
		{
			if ( item_info = sItemMgr->GetItem(item.GetItem()) )
			{
				if ( item_info->GetMaxSocket() > 0 )
				{
					if ( this->GetWorldId() != WORLD_RAKLION && this->GetWorldId() != WORLD_RAKLION_BOSS )
					{
						item_drop = false;
					}
				}

				if ( !AllowItemDropInWorld(this->GetWorldId(), item) )
				{
					item_drop = false;
				}
			}
			else
			{
				item_drop = false;
			}
		}

		if ( item_drop )
		{
			float durability = item.GetDurability();

			if ( ext_drop_per == 1 )
			{
				durability = sItemMgr->CalculateDurability(item.GetItem(), item.GetLevel(), 1, 0, item_info);
			}
			else
			{
				durability = sItemMgr->CalculateDurability(item.GetItem(), item.GetLevel(), 0, 0, item_info);
			}

			item.SetSkill(Random<uint8>(100) < sGameServer->item_drop_skill_rate_normal.get());
			item.SetLuck(Random<uint8>(100) < sGameServer->item_drop_luck_rate_normal.get());
			uint8 option_rate = Random<uint8>(100);
			uint8 option = Random<uint8>(3);
			item.ResetSocket(SOCKET_SLOT_NONE);

			if ( item_info->GetMaxSocket() > 0 )
			{
				for ( uint8 i = 0; i < item_info->GetMaxSocket(); i++ )
				{
					if (roll_chance_i(sGameServer->GetItemDropSocketRate(i), 10000))
					{
						item.SetSocket(i, SOCKET_SLOT_EMPTY);
					}
					else
					{
						break;
					}
				}
			}

			if ( ext_drop_per == 1 )
			{
				item.SetSkill(1);
				item.SetLuck(Random<uint8>(100) < sGameServer->item_drop_luck_rate_exe.get());
				item.SetExe(GetRandomExcOption(sGameServer->GetRandomItemDropExeOption()));
				item.ResetSocket(SOCKET_SLOT_NONE);
				item.SetLevel(0);

				if ( item_info->GetMaxSocket() > 0 )
				{
					for ( uint8 i = 0; i < item_info->GetMaxSocket(); ++i )
					{
						if (roll_chance_i(sGameServer->GetItemDropSocketRateExe(i), 10000))
						{
							item.SetSocket(i, SOCKET_SLOT_EMPTY);
						}
						else
						{
							break;
						}
					}
				}
			}

			item.SetOption(option == 0 && option_rate < 4 ? 3:
						   option == 1 && option_rate < 8 ? 2:
						   option == 2 && option_rate < 12 ? 1: 0);

			LOG_JEWEL_DROP(pMaxAttacker, this->GetWorldId(), this->GetX(), this->GetY(), &item);
				
			sItemMgr->ItemSerialCreate(pMaxAttacker, this->GetWorldId(), this->GetX(), this->GetY(), item, false);
			return;
		}
	}

	this->DropZen();
}

void Monster::DropZen()
{
	if ( DS_MAP_RANGE(this->GetWorldId()) )
	{
		return;
	}

	Player* pMaxAttacker = this->GetMaxAttacker();
	World* pWorld = this->GetWorld();

	if ( !pWorld || !pMaxAttacker )
	{
		return;
	}

	if ( this->GetZen() < 1 || this->GetZenRate() <= 0 )
	{
		return;
	}

	int32 zen_rate = this->GetZenRate();

	if ( zen_rate < 1 )
	{
		zen_rate = 1;
	}

	if ( RANDOM(zen_rate) < 10 )
	{
		int32 zen_drop = this->GetZen() + (this->GetZen() * pMaxAttacker->GetData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN) / 100) + 7;

		pWorld->AddZen(this, this->GetX(), this->GetY(), zen_drop);
	}

	if ( RANDOM(400) == 1 )
	{
		for ( uint8 i = 0; i < 4; i++ )
		{
			int16 x = this->GetX() - 2 + RANDOM(3);
			int16 y = this->GetY() - 2 + RANDOM(3);

			FIX_COORD(x);
			FIX_COORD(y);

			pWorld->AddZen(this, x, y, this->GetZen());
		}
	}
}