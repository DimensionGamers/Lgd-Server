/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerDarkSpirit.cpp"
*
*/

bool Player::DarkSpiritExist(bool run)
{
	if (this->GetClass() != Character::DARK_LORD)
	{
		return false;
	}

	Item const* pItem = this->GetActiveInventoryItem(PET_DARK_RAVEN);

	if (!pItem)
	{
		return false;
	}

	if (run && sGameServer->IsDarkRavenAutorepair() && pItem->GetDurability() <= 0.0f)
	{
		this->ItemRepair(pItem->GetOwnerSlot(), 0, true);
	}

	if (!pItem->IsValid())
	{
		return false;
	}

	return true;
}

void Player::DarkSpiritCalculate()
{
	if (!this->DarkSpiritExist())
	{
		return;
	}

	Item const* raven = this->GetActiveInventoryItem(PET_DARK_RAVEN);

	if (!raven)
	{
		return;
	}

	int32 pet_level = raven->pet_dark.level;
	int32 leadership = this->GetLeadership();

	this->GetDarkSpirit()->SetAttackDamageMin(pet_level * 15 + leadership / 8 + 180);
	this->GetDarkSpirit()->SetAttackDamageMax(pet_level * 15 + leadership / 4 + 200);
	this->GetDarkSpirit()->SetAttackSpeed(pet_level * 4 / 5 + leadership / 50 + 20);
	this->GetDarkSpirit()->SetAttackSuccessRate(pet_level + pet_level / 15 + 1000);

	this->GetDarkSpirit()->SetExcellentDamageRate(3 + (leadership / 200) + (pet_level / 20));
	this->GetDarkSpirit()->SetDoubleDamageRate(2 + (leadership / 300) + (pet_level / 30));
	this->GetDarkSpirit()->SetIgnoreDefenseRate(1 + (leadership / 400) + (pet_level / 40));

	if (pet_level >= 30)
	{
		this->GetDarkSpirit()->SetCriticalDamageRate(30 + (((pet_level - 30) * 90) / 100));
	}
	else
	{
		this->GetDarkSpirit()->SetCriticalDamageRate(pet_level);
	}
}

void Player::DarkSpiritRun()
{
	if ( !this->DarkSpiritExist() )
	{
		return;
	}

	if ( this->IsInSafeZone() )
	{
		return;
	}

	if ( CC_MAP_RANGE(this->GetWorldId()) )
	{
		return;
	}

	if ( this->GetAttackStandby() > 0 )
	{
		return;
	}

	if ( this->IsSpectator() )
	{
		return;
	}

	if ( this->GetDarkSpirit()->GetAttackTick() > GetTickCount() )
	{
		return;
	}

	if ( this->GetHelper()->IsStarted() )
	{
		if ( !this->GetHelper()->IsDarkSpirit() )
		{
			return;
		}

		if ( this->GetHelper()->IsOptDarkSpiritAutoAttack() )
		{
			this->GetDarkSpirit()->SetMode(DARK_SPIRIT_MODE_RANDOM);
		}
		else if ( this->GetHelper()->IsOptDarkSpiritAttackTogether() )
		{
			this->GetDarkSpirit()->SetMode(DARK_SPIRIT_MODE_WITH_MASTER);
		}
		else
		{
			this->GetDarkSpirit()->SetMode(DARK_SPIRIT_MODE_NORMAL);
		}
	}

	this->GetDarkSpirit()->SetAttackTick((GetTickCount() + 1500) - (this->GetDarkSpirit()->GetAttackSpeed() * 10));

	switch ( this->GetDarkSpirit()->GetMode() )
	{
	case DARK_SPIRIT_MODE_NORMAL:
		{
			this->DarkSpiritModeNormal();
		} break;

	case DARK_SPIRIT_MODE_RANDOM:
		{
			this->DarkSpiritModeRandom();
		} break;

	case DARK_SPIRIT_MODE_WITH_MASTER:
		{
			this->DarkSpiritModeWithMaster();
		} break;

	case DARK_SPIRIT_MODE_TARGET:
		{
			this->DarkSpiritModeTarget();
		} break;
	}
}
	
void Player::DarkSpiritModeNormal()
{

}
	
void Player::DarkSpiritModeRandom()
{
	Object * mUnit = nullptr;
	RandomValue<Unit*> m_RandomValue(nullptr);

	VIEWPORT_LOOP(mUnit)

		if ( !mUnit->IsCreature() || !mUnit->IsPlaying() || !mUnit->IsLive() )
		{
			continue;
		}

		if ( !mUnit->ToCreature()->IsMonster() )
		{
			continue;
		}

		if ( !this->AttackAllowed(mUnit->ToUnit(), true) )
		{
			continue;
		}

		if ( !IN_RANGE(this, mUnit, (DARK_SPIRIT_ATTACK_DISTANCE-2)) )
		{
			continue;
		}

		m_RandomValue.AddValue(mUnit->ToUnit(), 0);

	VIEWPORT_CLOSE

	Unit* pUnit = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

	if ( !pUnit )
	{
		return;
	}

	if (roll_chance_i(30))
	{
		this->DarkSpiritRangeAttack(pUnit);
	}
	else
	{
		this->DarkSpiritSendAttack(pUnit, 1, 0);
	}
}
	
void Player::DarkSpiritModeWithMaster()
{
	Unit* pDarkSpiritTarget = this->GetDarkSpirit()->GetTarget();

	if ( !pDarkSpiritTarget )
	{
		return;
	}

	if ( !this->SameDimension(pDarkSpiritTarget) )
	{
		this->GetDarkSpirit()->SetTarget(nullptr);
		return;
	}

	if ( !pDarkSpiritTarget->IsLive() )
	{
		this->GetDarkSpirit()->SetTarget(nullptr);
		return;
	}

	if ( !IN_RANGE(this, pDarkSpiritTarget, DARK_SPIRIT_ATTACK_DISTANCE) )
	{
		return;
	}

	if (roll_chance_i(30))
	{
		this->DarkSpiritRangeAttack(pDarkSpiritTarget);
	}
	else
	{
		this->DarkSpiritSendAttack(pDarkSpiritTarget, 1, 0);
	}
}
	
void Player::DarkSpiritModeTarget()
{
	Unit* pDarkSpiritTarget = this->GetDarkSpirit()->GetTarget();

	if ( !pDarkSpiritTarget )
	{
		return;
	}

	if ( !this->SameDimension(pDarkSpiritTarget) )
	{
		this->GetDarkSpirit()->SetTarget(nullptr);
		this->GetDarkSpirit()->SetMode(DARK_SPIRIT_MODE_NORMAL);
		return;
	}

	if ( !pDarkSpiritTarget->IsLive() )
	{
		this->GetDarkSpirit()->SetTarget(nullptr);
		this->GetDarkSpirit()->SetMode(DARK_SPIRIT_MODE_NORMAL);
		return;
	}

	if ( !IN_RANGE(this, pDarkSpiritTarget, DARK_SPIRIT_ATTACK_DISTANCE) )
	{
		return;
	}

	this->DarkSpiritSendAttack(pDarkSpiritTarget, 1, 1);
}

void Player::DarkSpiritRangeAttack(Unit* mTarget)
{
	this->DarkSpiritSendAttack(mTarget, 0, 0);

	Object * mUnit = nullptr;
	uint8 hit_count = 0;

	VIEWPORT_LOOP(mUnit)

		if ( mTarget == mUnit )
		{
			continue;
		}

		if ( !mUnit->IsCreature() || !mUnit->IsPlaying() || !mUnit->IsLive() )
		{
			continue;
		}

		if ( !mUnit->ToCreature()->IsMonster() )
		{
			continue;
		}

		if ( !this->AttackAllowed(mUnit->ToUnit()) )
		{
			continue;
		}

		if ( !IN_RANGE(mTarget, mUnit, (DARK_SPIRIT_ATTACK_DISTANCE-3)) )
		{
			continue;
		}

		if ( Random(3) == 0 )
		{
			this->DarkSpiritSendAttack(mUnit->ToUnit(), 2, 0);
			hit_count++;
		
			if ( hit_count > 3 )
			{
				break;
			}
		}

	VIEWPORT_CLOSE
}

void Player::DarkSpiritSendAttack(Unit* mTarget, uint8 type, uint8 action)
{
	if ( !mTarget )
	{
		return;
	}

	if ( type != 2 )
	{
		PET_ATTACK_COMMAND pMsg(0, type ? 0 : 1,this->GetEntry(), mTarget->GetEntry());
		this->sendPacket(MAKE_PCT(pMsg));

		Object * mUnit;

		VIEWPORT_LOOP(mUnit)

			if ( !mUnit->IsPlayer() )
			{
				continue;
			}

			mUnit->SEND_PCT(pMsg);
		
		VIEWPORT_CLOSE
		
	}

	this->AddDelayed(DELAYED_DARK_SPIRIT, 600, mTarget, action);
}

void Player::DarkSpiritAttack(Unit* pTarget, uint8 action)
{
	if ( !this->DarkSpiritExist() )
	{
		return;
	}

	if ( !pTarget || pTarget == this )
	{
		return;
	}

	if ( !this->IsLive() || this->IsSpectator() )
	{
		return;
	}

	if ( !this->AttackAllowed(pTarget, true) )
	{
		return;
	}

	PLAYER_POINTER(this)->UpdateAutorecuperation();
	PLAYER_POINTER(pTarget)->UpdateAutorecuperation();
	PLAYER_POINTER(this)->DarkSpiritSetTarget(pTarget);
	this->SetSummonedTarget(pTarget);
	
	this->RemoveBuff(BUFF_TRANSPARENCY);

	if ( pTarget->HasBuff(BUFF_INMUNE_ATTACK) )
	{
		this->MissSend(pTarget, nullptr, false, 0);
		return;
	}

	if ( pTarget->HasBuff(BUFF_ORDER_OF_PROTECTION) )
	{
		this->MissSend(pTarget, nullptr, false, 0);
		return;
	}

	if ( pTarget->IsCreature() )
	{
		if ( pTarget->ToCreature()->GetAI() && !pTarget->ToCreature()->GetAI()->EnableAttack(this, nullptr, 0) )
		{
			return;
		}
	}

	Unit * pAttackMaster = this->GetSummoner();
	Unit * pTargetMaster = pTarget->GetSummoner();

	uint16 damage_type = 0;
	bool miss_all = false;
	uint8 interrupt = 0;
	
	if ( !this->MissCheck(pTarget, nullptr, false, 0, miss_all) )
	{
		return;
	}

	int32 target_defense = this->GetTargetDefense(pTarget, damage_type, 1);
	int32 damage = this->DarkSpiritGetDamage(target_defense, damage_type);

	if( miss_all )
	{
		damage = (damage * 30) / 100;
	}

	if( action != 0 )
	{
		damage = (damage * 60) / 100;
	}

	pTarget->ApplyDamageDecrease(damage);
	
	this->ReduceAttackHP(0);

	if (this->PowerGet(POWER_LIFE) > 0)
	{
		damage += this->GetIncreaseDamageRate() * damage / 100;
	}

	damage -= pTarget->GetAbsorbDamageRate() * damage / 100;

	int32 min_damage = this->GetTotalLevel() / 10;

	if ( damage < min_damage )
	{
		if ( min_damage < 1 )
		{
			min_damage = 1;
		}

		damage = min_damage;
	}

	this->DarkSpiritDamage(damage);

	PLAYER_POINTER(pTarget)->DamagePet(damage);
	PLAYER_POINTER(pTarget)->DamageMount(damage);
		
	if (roll_chance_i(this->GetDarkSpirit()->GetDoubleDamageRate()))
	{
		damage_type |= DAMAGE_PROP_DOUBLE;
		damage += damage;
	}

	if ( pTarget->IsPlayer() )
	{
		this->PVPDamageReduction(damage, pTarget->ToPlayer());
	}
	
	if ( pTarget->IsCreature() )
	{
		if ( pTarget->ToCreature()->IsGuardianStatue() || pTarget->ToCreature()->IsCastleSiegeGate() )
		{
			this->AccumulateDamage(damage);
		}
	}
	
	if ( damage < 0 )
	{
		damage = 0;
	}
	
	int32 damage_shield = this->DealDamage(pTarget, damage, 0);

	if ( pTarget->IsCreature() )
	{
		pTarget->AddDelayed(DELAYED_MONSTER_HIT, 100, this);
	}

	if ( damage > 0 )
	{
		PLAYER_POINTER(pAttackMaster)->SelfDefenseActive(pTargetMaster->ToPlayer());
		PLAYER_POINTER(pTarget)->RandomArmorDurabilityDown(this, damage);
	}

	this->SetRest(0);

	if ( damage > 0 )
	{
		this->ReduceBuffTime(BUFF_ICE_ARROW, 1);
		this->RemoveBuff(BUFF_SLEEP);

		pTarget->ReflectDamage(damage, this);
		pTarget->TotalPowerRecovery();

		if ( Player* pPlayer = pTarget->ToPlayer() )
		{
			if ( Random(4) == 0 )
			{
				if ( !pPlayer->IsActiveInventoryItem(PET_UNIRIA) )
				{
					interrupt = 1;
				}
			}
		}

		this->SendDamage(pTarget, damage, damage_shield, interrupt, damage_type);

		pTarget->Unit::AfterHitCheck(this, damage, damage_shield, damage_type);
	}
	else
	{
		this->SendDamage(pTarget, 0, 0, 0, damage_type);
	}

	if ( this->PowerGet(POWER_LIFE) <= 0 )
	{
		this->Unit::AfterHitCheck(pTarget, 0, 0, 0);
	}
}

void Player::DarkSpiritDamage(int32 damage)
{
	Item * pItem = this->GetActiveInventoryItem(PET_DARK_RAVEN);

	if (!pItem)
	{
		return;
	}

	pItem->IncreaseDurabilityState(damage * 2 / 100);
	pItem->SetState(ITEM_CHANGED);

	float max_dur_state = pItem->GetDurabilityResistance() + (pItem->GetDurabilityResistance() * this->MagicGetValue(SKILL_TREE_PET_REDUCTION_HP) / 100.0f);

	if (pItem->GetDurabilityState() >= max_dur_state)
	{
		pItem->SetDurabilityState(0.0f);
		pItem->AlterDurability(0, 1.0f);

		if (pItem->CheckDurabilityState())
		{
			this->CalculateCharacter();
		}

		/*if ( this->GetInventory()->GetItem(WEAPON_02)->GetDurability() < 1.0f )
		{
		if ( this->GetInventory()->GetItem(WEAPON_02)->PetExeDownPercent(10) )
		{
		this->DarkSpiritCalculate();
		this->PetInfoLevelUp(WEAPON_02,0);
		}
		}*/

		this->SendItemDurability(pItem->GetOwnerSlot(), 0);
		this->PetInfoResult(0, 0, pItem->GetOwnerSlot());
	}
}

int32 Player::DarkSpiritGetDamage(int32 target_defense, uint16 & damage_type)
{
	int32 damage_min = this->GetDarkSpirit()->GetAttackDamageMin();
	int32 damage_max = this->GetDarkSpirit()->GetAttackDamageMax();

	Item const* pLeft = this->GetInventory()->GetItem(WEAPON_01);

	if ( pLeft->IsValid() && pLeft->GetPetPower() )
	{
		int32 rise = (pLeft->GetPetPower() / 2) + (pLeft->GetLevel() * 2);
		rise -= rise * pLeft->GetDurabilityCurrentStatus();

		damage_min += damage_min * rise / 100;
		damage_max += damage_max * rise / 100;
	}

	int32 damage_add = damage_min + (Random(damage_max - damage_min));

	if (roll_chance_i(this->GetDarkSpirit()->GetCriticalDamageRate()))
	{
		damage_add = damage_max;
		damage_type = DAMAGE_TYPE_CRITICAL;
	}

	if (roll_chance_i(this->GetDarkSpirit()->GetExcellentDamageRate()))
	{
		damage_add = (120 * damage_max) / 100;
		damage_type = DAMAGE_TYPE_EXE;
	}

	damage_add -= target_defense;

	if ( damage_add < 0 )
	{
		damage_add = 0;
	}

	return damage_add;
}

void Player::DarkSpiritSetTarget(Unit* mTarget)
{
	if ( this->GetClass() != Character::DARK_LORD )
		return;

	this->GetDarkSpirit()->SetTarget(mTarget);
}

void Player::DarkSpiritSetMode(DarkSpiritMode mode, Unit* mTarget)
{
	if ( !this->DarkSpiritExist() )
		return;

	if ( mode == DARK_SPIRIT_MODE_TARGET && !mTarget )
		return;

	this->GetDarkSpirit()->SetMode(mode);
	this->GetDarkSpirit()->SetTarget(mTarget);
	
	PET_SEND_COMMAND pMsg(0, mode, ENTRY(this->GetDarkSpirit()->GetTarget()));
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::DarkSpiritSetCommand(uint8 * Packet)
{
	if ( !this->IsPlaying() )
		return;

	POINTER_PCT(PET_COMMAND, lpMsg, Packet, 0);

	if ( lpMsg->pet )
		return;

	this->DarkSpiritSetMode(DarkSpiritMode(lpMsg->command), sObjectMgr->GetUnit(lpMsg->GetTarget()));
}