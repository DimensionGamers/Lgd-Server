/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Unit.cpp"
*
*/
int32 mercenary_count = 0;

Unit::Unit(uint16 entry): Object(entry)
{
	this->SetConnectStatus(CONNECT_STATUS_NONE);

	Unit::Init(); 
}

Unit::~Unit()
{
	Unit::Init();
}

void Unit::Init()
{
	Util::memset<int32, int32>(this->power, 0, POWER_MAX);
	Util::memset<int32, int32>(this->power_max, 0, POWER_MAX);
	Util::memset<int32, int32>(this->power_add, 0, POWER_MAX);
	
	this->ResetResistance(0);
	this->ResetResistanceAdd(0);
		
	this->SetTX(0);
	this->SetTY(0);
	
	this->SetDelayedTeleport(false);
	this->SetDelayedTeleportTime(0);

	this->GetLastLocation()->Reset();
	this->GetDelayedLocation()->Reset();

	this->ResetIntData(0);

	SKILL_LOOP(i)
		this->GetSkill(i)->Clear();

	BUFF_LOOP(i)
		this->GetBuffData(i)->Reset();

	this->GetRegenLocation()->Reset();

	this->SetRegenStatus(REGEN_NONE);

	this->GetNova()->Reset();

	this->SetTeleportStatus(TELEPORT_NONE);
	this->SetViewState(VIEW_STATE_NONE);

	LIST_CLEAR(DelayedDataList::iterator, this->delayed_data);

	this->SetSummoner(this);
	this->SetSummoned(nullptr);
	
	this->SetRest(0);

	this->SetCastleSiegeJoinSide(0);
	
	this->GetGen()->Reset();

	this->SetKillCount(0);
	this->SetAction(0);
	this->SetForceUpgrade(false);
	this->ResetSkillAngleX(0);
	this->ResetSkillAngleY(0);
	
	this->ClearPathData();
	this->SetKiller(nullptr);
	this->SetTeleportTime(0);
	this->GetMovePathTime()->Reset();
	this->GetMoveLastTime()->Reset();
	this->SetShieldTick(0);

	this->ResetOldPower(0);

	this->SetElementalAttribute(ELEMENTAL_ATTRIBUTE_NONE);
	this->SetElementalPattern(0);
	this->SetMiningIndex(nullptr);
	this->SetMiningStage(0);

	this->ResetFloatData(0.0f);

	this->SetLastAttacker(nullptr);

	this->SetTempX(0);
	this->SetTempY(0);

	this->punish_map.clear();

	this->Object::Init();
}

void Unit::SendCustomObjectData()
{
	if ( Monster* pMonster = this->ToCreature() )
	{
		sMonsterMgr->BuildMonsterCustomData(pMonster, nullptr);
	}
}

void Unit::ActionSend(uint16 target, uint8 action, uint8 direction, bool me)
{
	ACTION_RESULT pMsg(this->GetEntry(), target, direction, action);

	if ( me )
		this->SEND_PCT(pMsg);

	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::PositionSend(coord_type x, coord_type y)
{
	this->SetLocation(x, y, this->GetDirection());
	this->SetTX(x);
	this->SetTY(y);
	this->ClearPathData();

	this->SetRest(0);

	this->CreateFrustrum();

	POSITION_SET_SEND pMsg(this->GetEntry(), x, y);

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::EffectSend(uint8 effect)
{
	SEND_EFFECT pMsg(this->GetEntry(), effect);

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::StateInfoSend(uint16 buff, uint8 state)
{
	SKILL_STATE_SEND pMsg(buff, state, this->GetEntry());

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::StateInfoSendSingle(Player* pPlayer, uint16 buff, uint8 state)
{
	SKILL_STATE_SEND pMsg(buff, state, this->GetEntry());

	if (pPlayer)
	{
		pPlayer->SEND_PCT(pMsg);
	}
}

void Unit::SetLocation(coord_type x, coord_type y, uint8 direction)
{
	this->SetX(x);
	this->SetY(y);
	this->SetDirection(direction);
}

void Unit::UpdateLastLocation()
{
	this->GetLastLocation()->Set(this->GetLocation());
}
	
void Unit::UpdateLastLocation(world_type world, coord_type x, coord_type y, int32 instance, uint8 direction)
{
	this->GetLastLocation()->SetWorldId(world);
	this->GetLastLocation()->SetX(x);
	this->GetLastLocation()->SetY(y);
	this->GetLastLocation()->SetInstance(instance);
	this->GetLastLocation()->SetDirection(direction);
}

void Unit::UpdateDelayedTeleport()
{
	if ( !this->IsDelayedTeleport() )
		return;
	
	this->ReduceDelayedTeleportTime(1);

	if ( this->GetDelayedTeleportTime() > 0 )
		return;

	this->SetDelayedTeleportTime(0);
	this->SetDelayedTeleport(false);

	this->UpdateLastLocation();

	this->ClearPathData();
	this->GetLocation()->Set(this->GetDelayedLocation());
	this->SetTX(this->GetX());
	this->SetTY(this->GetY());
	this->MagicAttackSend(ENTRY(this), SKILL_TELEPORT);
	this->SkillTeleportUse(this->GetX(), this->GetY());
}

void Unit::CancelDelayedTeleport()
{
	this->SetDelayedTeleport(false);
	this->SetDelayedTeleportTime(0);
	this->GetDelayedLocation()->Reset();
	if (Player* pPlayer = this->ToPlayer())
	{
		pPlayer->SetDelayedTeleportHelper(false);
	}
}

int32 Unit::MagicAdd(uint16 skill, uint8 level, bool weapon)
{
	if ( !skill )
	{
		return -1;
	}

	if ( this->MagicGet(skill) )
	{
		return -1;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == 0 )
		{
			this->GetSkill(i)->SetSkill(skill);
			this->GetSkill(i)->SetLevel(level);
			this->GetSkill(i)->SetWeapon(weapon);
			return i;
		}
	}

	return -1;
}

int32 Unit::MagicRemove(uint16 skill)
{
	if ( !skill )
	{
		return -1;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == skill )
		{
			this->GetSkill(i)->Clear();
			return i;
		}
	}

	return -1;
}

bool Unit::MagicFind(uint16 skill, uint8 level) const
{
	if ( !skill )
	{
		return false;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == skill )
		{
			return this->GetSkill(i)->GetLevel() >= level;
		}
	}

	return false;
}

Skill * Unit::MagicGet(uint16 skill)
{
	if ( !skill )
	{
		return nullptr;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == skill )
			return this->GetSkill(i);
	}

	return nullptr;
}

Skill const* Unit::MagicGet(uint16 skill) const
{
	if ( !skill )
	{
		return nullptr;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == skill )
			return this->GetSkill(i);
	}

	return nullptr;
}

Skill * Unit::MagicGet(uint16 skill, int32 & position)
{
	if ( !skill )
	{
		return nullptr;
	}

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == skill )
		{
			position = i;
			return this->GetSkill(i);
		}
	}

	position = -1;
	return nullptr;
}

void Unit::MagicCancel(uint16 skill)
{
	MAGIC_CANCEL pMsg(skill, this->GetEntry());
	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

bool Unit::MoveToGate(uint16 gate_id)
{
	if ( !this->IsLive() || this->GetRegenStatus() != REGEN_NONE )
		return false;

	GateData const* pGate = sTeleport->GetGate(gate_id, true);
	SafeRAssert(pGate, "pGate == nullptr", false);

	coord_type tmp_x = pGate->x1;
	coord_type tmp_y = pGate->y1;
	world_type tmp_world = pGate->world;

	this->GetValidCoordinates(pGate->id, tmp_world, tmp_x, tmp_y);

	this->UpdateLastLocation();
	this->ClearPathData();

	this->SetWorldId(pGate->world);
	this->SetX(tmp_x);
	this->SetY(tmp_y);
	this->SetTX(tmp_x);
	this->SetTY(tmp_y);
	this->SetDirection(pGate->direction);

	this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
	return true;
}

void Unit::GetValidCoordinates(uint16 gate, world_type & world, coord_type & x, coord_type & y)
{
	GateData const* pGate = sTeleport->GetGate(gate);

	if ( !pGate )
	{
		return;
	}

	World* pWorld = sWorldMgr->GetWorld(pGate->world);

	if ( !pWorld )
	{
		return;
	}

	world = pGate->world;
	
	if ( pGate->x1 == pGate->x2 && pGate->y1 == pGate->y2 )
	{
		x = pGate->x1;
		y = pGate->y1;
	}
	else
	{
		pWorld->GetRandomLocation(x, y, pGate->x1, pGate->y1, pGate->x2, pGate->y2);
	}
}

void Unit::TeleportToLocation(world_type world, coord_type x, coord_type y, uint8 direction, int32 instance)
{
	this->UpdateLastLocation();

	this->SetWorldId(world);
	this->SetX(x);
	this->SetY(y);
	this->SetTX(x);
	this->SetTY(y);
	this->SetDirection(direction);
	this->SetInstance(instance);
	this->GetRegenLocation()->Set(this->GetLocation());
}

void Unit::TeleportToLocation(world_type world)
{
	World * pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
	{
		return;
	}

	this->UpdateLastLocation();

	coord_type x = 0;
	coord_type y = 0;
	pWorld->GetRespawn(world, x, y);
	this->SetWorldId(world);
	this->SetX(x);
	this->SetY(y);
	this->SetTX(x);
	this->SetTY(y);
}

void Unit::TeleportToLocation()
{
	this->SetWorldId(this->GetLastLocation()->GetWorldId());
	this->SetX(this->GetLastLocation()->GetX());
	this->SetY(this->GetLastLocation()->GetY());
	this->SetDirection(this->GetLastLocation()->GetDirection());
	this->SetInstance(this->GetLastLocation()->GetInstance());

	this->GetLastLocation()->SetWorldId(-1);
}

void Unit::TeleportToObject(Unit* pUnit)
{
	if ( pUnit )
		this->TeleportToLocation(pUnit->GetWorldId(), pUnit->GetX(), pUnit->GetY(), pUnit->GetDirection(), pUnit->GetInstance());
}

bool Unit::TeleportAreaCheck(coord_type x, coord_type y)
{
	if ( !InRange(x, y, 8) )
		return false;

	if ( this->IsInSafeZone() )
		return false;

	if ( World * pWorld = this->GetWorld() )
	{
		return pWorld->GetGrid(x, y).attribute == 0;
	}

	return false;
}

void Unit::UpdateBuffStatus()
{
	bool update = false;
	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		data = this->GetBuffData(i);

		if ( !data )
			continue;

		if ( data->GetID() == 0 )
			continue;

		if ( data->GetID() == BUFF_ELF_SOLDIER && data->IsFlag(BUFF_FLAG_COMBAT) && sGameServer->IsElfSoldierRemoveOnDie() )
		{
			data->RemoveFlag(BUFF_FLAG_COMBAT);
		}

		if ( data->IsFlag(BUFF_FLAG_CONSTANT) )
		{
			continue;
		}

		this->ApplyDebuffDamage(data);

		time_t remain_time = data->GetRemainTime();

		data->Update(1);
		
		if (remain_time > 0)
		{
			continue;
		}

		this->BuffDataRemove(data);
		update = true;
	}

	if ( update )
	{
		PLAYER_POINTER(this)->CalculateCharacter();
	}
}

void Unit::ApplyDebuffDamage(BuffData * pBuffData) // TODO: Ejecutar cada 500 msec
{
	if ( Monster* pMonster = this->ToCreature() )
	{
		if ( pMonster->GetClass() == 709 ||
			 pMonster->GetClass() == 710 ||
			 pMonster->GetClass() == 711 ||
			 pMonster->GetClass() == 712 )
		{
			return;
		}
	}

	time_t remain_time = pBuffData->GetRemainTime();
	int32 period = -1;
	int32 life_damage = 0;
	int32 shield_damage = 0;
	int32 absorb = 0;
	uint16 effect = 0;
	uint8 special = 0;
	
	switch( pBuffData->GetID() )
	{
	case BUFF_POISON:
		{
			period = 3;
			effect = DAMAGE_TYPE_POISON;
			life_damage = static_cast<int32>(pBuffData->GetEffect(0)->GetValue()) * (this->PowerGet(POWER_LIFE) / 100);
		} break;

	case BUFF_BAT_FLOCK:
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_POISON_ARROW:
	case BUFF_POISON_ARROW_IMPROVED:
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_EXPLOSION:
	//case BUFF_REQUIEM:
	case BUFF_DRAGON_ROAR_ENHANCED:
	case BUFF_CHAIN_DRIVER_ENHANCED:
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_DRAIN_LIFE_ENHANCED:
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
			absorb = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_HEMORRAGE:
		{
			period = 1;
			effect = pBuffData->GetEffect(1)->GetValue();
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_PARALYZE_IMPROVED:
		{
			remain_time *= IN_MILLISECONDS;
			period = 500;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_DEATH_STAB_ENHANCED: // TODO: BloodHowling es cada 0.5 seg
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_POISONING:
	case BUFF_DEADLY_POISONING:
		{
			period = 1;
			effect = DAMAGE_TYPE_POISON;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_BLEEDING:
	case BUFF_HEMORRHAGE:
		{
			period = 1;
			effect = DAMAGE_TYPE_DEBUFF;
			life_damage = pBuffData->GetEffect(0)->GetValue();
		} break;

	case BUFF_ELITE_BLEEDING:
	{
		period = 1;
		effect = DAMAGE_TYPE_DEBUFF;
		life_damage = pBuffData->GetEffect(0)->GetValue();
	} break;

	case BUFF_ELITE_POISON:
	{
		period = 1;
		effect = DAMAGE_TYPE_POISON;
		life_damage = pBuffData->GetEffect(0)->GetValue();
	} break;
	}

	if ( period == -1 )
	{
		return;
	}

	if ( (remain_time % period) )
	{
		return;
	}

	Unit* pCaster = pBuffData->GetCaster();

	if ( pCaster )
	{
		if ( Player* pPlayer = pCaster->ToPlayer() )
		{
			pPlayer->ApplyMajesticDamage(this->ToCreature(), life_damage, false);
		}

		if ( Player* pPlayer = this->ToPlayer() )
		{
			pPlayer->ApplyMajesticDamage(pCaster->ToCreature(), life_damage, true);
		}
	}

	if ( pCaster )
	{
		shield_damage = pCaster->DealDamage(this, life_damage, 0, 0, pBuffData->GetID() == BUFF_HEMORRAGE ? true: false, MONSTER_THREAT_DAMAGE_DEBUFF);
	}
	else
	{
		shield_damage = this->DealDamage(this, life_damage, 0, 0, pBuffData->GetID() == BUFF_HEMORRAGE ? true: false, MONSTER_THREAT_DAMAGE_DEBUFF);
	}

	if ( pCaster )
	{
		pCaster->PowerIncrease(POWER_LIFE, absorb, true);
	}

	if ( pBuffData->GetID() == BUFF_HEMORRAGE )
	{
		if ( pCaster )
		{
			pCaster->SendDamageElemental(this, effect, life_damage + shield_damage);
		}
		else
		{
			this->SendDamageElemental(this, effect, life_damage + shield_damage);
		}
	}
	else
	{
		if ( pCaster )
		{
			pCaster->SendDamage(this, life_damage, shield_damage, 0, effect, special);
		}
		else
		{
			this->SendDamage(this, life_damage, shield_damage, 0, effect, special);
		}
	}

	this->Unit::AfterHitCheck(pCaster ? pCaster: this, life_damage, shield_damage, 0);
}

bool Unit::AddItemBuff(uint16 item, time_t duration, uint8 flags, Unit * Caster, bool group_remove, bool from_use)
{
	buff_template const* item_buff_add = sSkillMgr->GetBuffItem(item);

	if (!item_buff_add)
	{
		return false;
	}

	if (Player* pPlayer = this->ToPlayer())
	{
		if ( from_use && sGameServer->IsHelperBuffItemUseCheck() && pPlayer->GetHelper()->IsStarted() )
		{
			if (this->HasBuff(item_buff_add->GetBuff()))
			{
				return false;
			}
		}
	}

	this->AddBuff(item_buff_add->GetBuff(),
		BuffEffect(item_buff_add->GetEffect(0), item_buff_add->GetValue(0)),
		BuffEffect(item_buff_add->GetEffect(1), item_buff_add->GetValue(1)),
		duration > 0 ? duration : item_buff_add->GetDuration(),
		flags, Caster, group_remove);
	return true;
}

void Unit::AddBuff(uint16 id, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	this->AddBuff(id, BuffEffect(), BuffEffect(), BuffEffect(), BuffEffect(), BuffEffect(), duration, flags, Caster, group_remove);
}

void Unit::AddBuff(uint16 id, BuffEffect effect_1, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	this->AddBuff(id, effect_1, BuffEffect(), BuffEffect(), BuffEffect(), BuffEffect(), duration, flags, Caster, group_remove);
}

void Unit::AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	this->AddBuff(id, effect_1, effect_2, BuffEffect(), BuffEffect(), BuffEffect(), duration, flags, Caster, group_remove);
}

void Unit::AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	this->AddBuff(id, effect_1, effect_2, effect_3, BuffEffect(), BuffEffect(), duration, flags, Caster, group_remove);
}

void Unit::AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, BuffEffect effect_4, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	this->AddBuff(id, effect_1, effect_2, effect_3, effect_4, BuffEffect(), duration, flags, Caster, group_remove);
}

void Unit::AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, BuffEffect effect_4, BuffEffect effect_5, time_t duration, uint8 flags, Unit * Caster, bool group_remove)
{
	if (Player* pPlayer = this->ToPlayer())
	{
		if (!pPlayer->IsSendBuff())
		{
			return;
		}
	}

	if ( id == BUFF_STUN )
	{
		if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_RESIST_STUN)))
		{
			(Caster ? Caster : this)->MissSend(this, nullptr, false, 0, MISS_TYPE_RESIST);
			return;
		}
	}

	this->UpdateOldPower();

	this->RemoveBuff(id, group_remove, true);

	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == 0 )
		{
			data = this->GetBuffData(i);
			break;
		}
	}

	if ( !data )
		return;
	
	data->SetID(id);
	data->SetDisabled(false);
	data->SetCaster(Caster);
	data->SetFlag(flags);
	data->GetEffect(0)->Set(effect_1.GetEffect(), effect_1.GetValue());
	data->GetEffect(1)->Set(effect_2.GetEffect(), effect_2.GetValue());
	data->GetEffect(2)->Set(effect_3.GetEffect(), effect_3.GetValue());
	data->GetEffect(3)->Set(effect_4.GetEffect(), effect_4.GetValue());
	data->GetEffect(4)->Set(effect_5.GetEffect(), effect_5.GetValue());
	data->SetDuration(duration);
	
	BUFF_EFFECT_LOOP(i)
	{
		MONSTER_POINTER(this)->ApplyBuffValues(data->GetEffect(i)->GetEffect(), data->GetEffect(i)->GetValue());
	}

	this->ApplyBuffInstantEffect(id);

	PLAYER_POINTER(this)->SendBuffEffect(data, 0);
	this->StateInfoSend(id, 1);
	PLAYER_POINTER(this)->CalculateCharacter();

	this->ApplyOldPower();
}

void Unit::ApplyBuffValues(uint8 opt, float value)
{
	switch ( opt )
	{
	case BUFF_OPTION_INCREASE_ATTACK_SPEED:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE:
		this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, value);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_INCREASE_DEFENSE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_LIFE:
		{
			this->PowerIncreseAdd(POWER_LIFE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_MANA:
		{
			this->PowerIncreseAdd(POWER_MANA, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_POWER:
		this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_INCREASE_MAGIC_POWER:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_CURSE_DAMAGE:
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_DAMAGE_REFLECTION_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_IGNORE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_MIN_WIZARDY:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		} break;

	case BUFF_OPTION_MAX_WIZARDY:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE_SKILL:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case BUFF_OPTION_GREATER_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_HP_MUL:
		{
			this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100);
		} break;

	case BUFF_OPTION_MP_MUL:
		{
			this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100);
		} break;

	case BUFF_OPTION_SD_MUL:
		{
			this->PowerIncreseAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100);
		} break;

	case BUFF_OPTION_AG_MUL:
		{
			this->PowerIncreseAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100);
		} break;

	case BUFF_OPTION_HP_DIV:
		{
			this->PowerReduceAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100);
		} break;

	case BUFF_OPTION_MP_DIV:
		{
			this->PowerReduceAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100);
		} break;

	case BUFF_OPTION_SD_DIV:
		{
			this->PowerReduceAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100);
		} break;

	case BUFF_OPTION_AG_DIV:
		{
			this->PowerReduceAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100);
		} break;

	case BUFF_OPTION_DEFENSE_REDUCE:
		{
			this->ReduceIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_ATTACK_SPEED_REDUCE:
		{
			this->ReduceIntData(UNIT_INT_ATTACK_SPEED, value);
		} break;

	case BUFF_OPTION_BASIC_DEFENSE:
	{
									  this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, value);
	} break;

	case BUFF_OPTION_DECREASE_MAXIMUM_HP:
	{
		this->PowerReduceAdd(POWER_LIFE, value);
	} break;
	}
}

void Unit::ClearBuffValues(uint8 opt, float value)
{
	switch ( opt )
	{
	case BUFF_OPTION_INCREASE_ATTACK_SPEED:
		{
			this->ReduceIntData(UNIT_INT_ATTACK_SPEED, value);
			this->ReduceIntData(UNIT_INT_MAGIC_SPEED, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE:
		{
			this->ReduceIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, value);
			this->ReduceIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, value);
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			this->ReduceIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
			this->ReduceIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE:
		{
			this->ReduceIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_LIFE:
		{
			this->PowerReduceAdd(POWER_LIFE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_MANA:
		{
			this->PowerReduceAdd(POWER_MANA, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_POWER:
			this->ReduceIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, value);
			this->ReduceIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_INCREASE_MAGIC_POWER:
		{
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_CURSE_DAMAGE:
		this->ReduceIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
		this->ReduceIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE_RATE:
		{
			this->ReduceFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE_RATE:
		{
			this->ReduceFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE:
		{
			this->ReduceIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE:
		{
			this->ReduceIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_DAMAGE_REFLECTION_RATE:
		{
			this->ReduceFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_IGNORE_RATE:
		{
			this->ReduceFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_MIN_WIZARDY:
		{
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		} break;

	case BUFF_OPTION_MAX_WIZARDY:
		{
			this->ReduceIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE_SKILL:
		{
			this->ReduceIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case BUFF_OPTION_GREATER_DAMAGE:
		{
			this->ReduceIntData(UNIT_INT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE:
		{
			this->ReduceIntData(UNIT_INT_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_HP_MUL:
		{
			this->PowerReduceAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100);
		} break;

	case BUFF_OPTION_MP_MUL:
		{
			this->PowerReduceAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100);
		} break;

	case BUFF_OPTION_SD_MUL:
		{
			this->PowerReduceAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100);
		} break;

	case BUFF_OPTION_AG_MUL:
		{
			this->PowerReduceAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100);
		} break;

	case BUFF_OPTION_HP_DIV:
		{
			this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100);
		} break;

	case BUFF_OPTION_MP_DIV:
		{
			this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100);
		} break;

	case BUFF_OPTION_SD_DIV:
		{
			this->PowerIncreseAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100);
		} break;

	case BUFF_OPTION_AG_DIV:
		{
			this->PowerIncreseAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100);
		} break;

	case BUFF_OPTION_DEFENSE_REDUCE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_ATTACK_SPEED_REDUCE:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
		} break;

	case BUFF_OPTION_BASIC_DEFENSE:
	{
									  this->ReduceIntData(UNIT_INT_DEFENSE_BASE, value);
	} break;

	case BUFF_OPTION_DECREASE_MAXIMUM_HP:
	{
		this->PowerIncreseAdd(POWER_LIFE, value);
	} break;
	}
}

bool Unit::HasBuff(uint16 id) const
{
	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == id )
			return true;
	}

	return false;
}

time_t Unit::GetBuffTime(uint16 id)
{
	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == id )
			return this->GetBuffData(i)->GetRemainTime();
	}

	return 0;
}

time_t Unit::GetBuffGroupTime(uint16 id)
{
	buff_template const* buff_info = sSkillMgr->GetBuff(id);
	buff_template const* buff_info_tmp = nullptr;

	if ( !buff_info )
		return this->GetBuffTime(id);

	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == 0 )
			continue;

		buff_info_tmp = sSkillMgr->GetBuff(this->GetBuffData(i)->GetID());

		if ( !buff_info_tmp )
			continue;

		if ( buff_info->GetGroup() != buff_info_tmp->GetGroup() )
			continue;
		
		return this->GetBuffTime(this->GetBuffData(i)->GetID());
	}

	return this->GetBuffTime(id);
}

bool Unit::HasBuffGroup(uint16 id) const
{
	buff_template const* buff_info = sSkillMgr->GetBuff(id);
	buff_template const* buff_info_tmp = nullptr;

	if ( !buff_info )
		return false;

	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == 0 )
			continue;

		buff_info_tmp = sSkillMgr->GetBuff(this->GetBuffData(i)->GetID());

		if ( !buff_info_tmp )
			continue;

		if ( buff_info->GetGroup() != buff_info_tmp->GetGroup() )
			continue;
		
		return true;
	}

	return false;
}

BuffData const* Unit::GetBuffByGroup(uint16 id) const
{
	if (id == 81)
	{
		if (this->HasBuff(81))
		{
			return this->GetBuff(81);
		}
		
		if (this->HasBuff(150))
		{
			return this->GetBuff(150);
		}
		
		if (this->HasBuff(152))
		{
			return this->GetBuff(152);
		}
	}
	else if (id == 301)
	{
		if (this->HasBuff(301))
		{
			return this->GetBuff(301);
		}
		
		if (this->HasBuff(302))
		{
			return this->GetBuff(302);
		}
		
		if (this->HasBuff(303))
		{
			return this->GetBuff(303);
		}
	}
	else
	{
		if (id == 324)
		{
			if (this->HasBuff(324))
			{
				return this->GetBuff(324);
			}

			if (this->HasBuff(328))
			{
				return this->GetBuff(328);
			}

			if (this->HasBuff(327))
			{
				return this->GetBuff(327);
			}
		}
		else if (id == 325)
		{
			if (this->HasBuff(325))
			{
				return this->GetBuff(325);
			}

			if (this->HasBuff(334))
			{
				return this->GetBuff(334);
			}

			if (this->HasBuff(336))
			{
				return this->GetBuff(336);
			}
		}
	}

	return nullptr;
}

void Unit::BuffDataRemove(BuffData * data)
{
	if ( !this->IsPlayer() )
	{
		BUFF_EFFECT_LOOP(i)
			this->ClearBuffValues(data->GetEffect(i)->GetEffect(), data->GetEffect(i)->GetValue());
	}

	PLAYER_POINTER(this)->SendBuffEffect(data, 1);
	this->StateInfoSend(data->GetID(), 0);

	if ( data->GetID() == BUFF_ICE_ARROW )
	{
		this->MagicCancel(SKILL_ICE_ARROW);
	}

	if (Player* pPlayer = this->ToPlayer())
	{
		switch (data->GetID())
		{
		case BUFF_HORN_OF_DINORANT:
		{
			pPlayer->MagicDelWeapon(SKILL_FIRE_BREATH, 0);
		} break;

		case BUFF_HORN_OF_DARK_HORSE:
		{
			pPlayer->MagicDelWeapon(SKILL_EARTHSHAKE, 0);
		} break;

		case BUFF_HORN_OF_FENRIR:
		{
			pPlayer->MagicDelWeapon(SKILL_PLASMA_STORM, 0);
		} break;
		}
	}

	data->Reset();
}

void Unit::buffClearGroup(uint16 group)
{
	buff_template const* buff_info = nullptr;
	bool update = false;

	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == 0 )
			continue;

		buff_info = sSkillMgr->GetBuff(this->GetBuffData(i)->GetID());

		if ( !buff_info || buff_info->GetGroup() != group )
			continue;

		this->BuffDataRemove(this->GetBuffData(i));
		update = true;
	}

	if ( update )
	{
		PLAYER_POINTER(this)->CalculateCharacter();
	}
}

void Unit::RemoveBuff(uint16 id, bool group_remove, bool from_add)
{
	buff_template const* buff_info = sSkillMgr->GetBuff(id);

	if ( !buff_info )
		return;

	this->RemoveBuff(buff_info, group_remove, from_add);
}

void Unit::RemoveBuff(buff_template const * buff_info, bool group_remove, bool from_add)
{
	buff_template const* buff_info_tmp = nullptr;
	bool update = false;

	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		if ( buff_info->GetBuff() != data->GetID() )
		{
			buff_info_tmp = sSkillMgr->GetBuff(data->GetID());

			if ( !buff_info_tmp )
				continue;

			if ( buff_info->GetGroup() != buff_info_tmp->GetGroup() || !group_remove )
				continue;
		}

		this->BuffDataRemove(data);
		update = true;
	}

	if ( from_add )
	{
		return;
	}

	if ( !update )
	{
		return;
	}

	PLAYER_POINTER(this)->CalculateCharacter();
}

void Unit::RemoveNonSelfBuff()
{
	bool update = false;

	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		if ( data->GetCaster() == this )
			continue;

		this->BuffDataRemove(data);
		update = true;
	}

	if ( update )
	{
		PLAYER_POINTER(this)->CalculateCharacter();
	}
}

float Unit::GetBuffEffect(uint16 id) const
{
	float value = 0.0f;

	BUFF_LOOP(i)
	{
		BuffData const* data = this->GetBuffData(i);

		if ( data->GetID() != id )
			continue;

		Player const* pPlayer = this->ToPlayer();

		if ( pPlayer )
		{
			if ( !pPlayer->IsBuffAllowed(data->GetID()) )
			{
				continue;
			}
		}

		BUFF_EFFECT_LOOP(h)
		{
			value += data->GetEffect(h)->GetValue();
		}
	}

	return value;
}

float Unit::GetEffect(uint8 effect) const
{
	float value = 0.0f;

	BUFF_LOOP(i)
	{
		BuffData const* data = this->GetBuffData(i);

		if ( !data->GetID() )
			continue;

		Player const* pPlayer = this->ToPlayer();

		if ( pPlayer )
		{
			if ( !pPlayer->IsBuffAllowed(data->GetID()) )
			{
				continue;
			}
		}

		BUFF_EFFECT_LOOP(h)
		{
			if ( data->GetEffect(h)->GetEffect() == effect )
			{
				value += data->GetEffect(h)->GetValue();
			}
		}
	}

	return value;
}

bool Unit::ReplaceBuff(uint16 id, float value_1, float value_2, float value_3, float value_4, float value_5) const
{
	buff_template const* buff_info = sSkillMgr->GetBuff(id);
	buff_template const* buff_info_tmp = nullptr;

	if ( !buff_info )
		return false;

	BUFF_LOOP(i)
	{
		BuffData const* data = this->GetBuffData(i);

		if ( !data->GetID() )
			continue;

		if ( data->GetID() == id )
		{
			buff_info_tmp = buff_info;
		}
		else
		{
			buff_info_tmp = sSkillMgr->GetBuff(data->GetID());
		}
		
		if ( !buff_info_tmp )
			continue;

		if ( buff_info->GetGroup() != buff_info_tmp->GetGroup() )
			continue;

		if ( data->GetEffect(0)->GetValue() > value_1 )
			return false;

		if ( data->GetEffect(1)->GetValue() > value_2 )
			return false;

		if ( data->GetEffect(2)->GetValue() > value_3 )
			return false;

		if ( data->GetEffect(3)->GetValue() > value_4 )
			return false;

		if ( data->GetEffect(4)->GetValue() > value_5 )
			return false;
	}

	return true;
}

void Unit::ReduceBuffEffect(uint16 id, uint8 effect, float value)
{
	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		data = this->GetBuffData(i);

		if ( data->GetID() != id )
			continue;

		BUFF_EFFECT_LOOP(h)
		{
			if ( data->GetEffect(h)->GetEffect() == effect )
				data->GetEffect(h)->ReduceValue(value);
		}
	}
}

void Unit::IncreaseBuffEffect(uint16 id, uint8 effect, float value)
{
	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		data = this->GetBuffData(i);

		if ( data->GetID() != id )
			continue;

		BUFF_EFFECT_LOOP(h)
		{
			if ( data->GetEffect(h)->GetEffect() == effect )
				data->GetEffect(h)->IncreaseValue(value);
		}
	}
}

void Unit::ReduceBuffTime(uint16 id, time_t time)
{
	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		data = this->GetBuffData(i);

		if ( data->GetID() != id )
			continue;

		if ( data->IsFlag(BUFF_FLAG_EXPIRE) )
			continue;

		data->Update(time);
	}
}

void Unit::SetBuffTime(uint16 id, time_t time)
{
	BuffData * data = nullptr;

	BUFF_LOOP(i)
	{
		data = this->GetBuffData(i);

		if ( data->GetID() != id )
			continue;

		if ( data->IsFlag(BUFF_FLAG_EXPIRE) )
			continue;

		data->SetDuration(time);
	}
}

void Unit::ClearAllBuff(uint8 flag)
{
	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() == 0 )
			continue;

		this->RemoveBuff(this->GetBuffData(i)->GetID());
	}
}

void Unit::ApplyBuffInstantEffect(uint16 id)
{
	switch ( id )
	{
	case BUFF_STUN:
	case BUFF_SLEEP:
	case BUFF_ICE_ARROW:
	case BUFF_EARTH_PRISON:
	case BUFF_ICE_STORM_ENHANCED:
	case BUFF_BINDING:
		{
			if (Player* pPlayer = this->ToPlayer())
			{
				if (sGameServer->IsDebuffStopCheck())
				{
					pPlayer->CancelMovement();
				}
				else
				{
					this->ClearPathData(true);
				}
			}
			else
			{
				this->ClearPathData(true);
			}
		} break;

	case BUFF_EVASION:
		{

		} break;

	case BUFF_IRON_DEFENSE:
	case BUFF_IRON_DEFENSE_IMPROVED:
		{
			this->ClearDebuff(MAX_BUFF);
		} break;

	case BUFF_ERRTEL_IMMUNE_II:
		{
			this->RemoveBuff(BUFF_HEMORRAGE);
			this->RemoveBuff(BUFF_PARALYSIS);
			this->RemoveBuff(BUFF_BINDING);
			this->RemoveBuff(BUFF_BLINDNESS);
		} break;
	}
}

bool Unit::HasRestrictionBuff() const
{
	return this->HasBuff(BUFF_ICE_ARROW) ||
		   this->HasBuff(BUFF_STUN) ||
		   this->HasBuff(BUFF_SLEEP) ||
		   this->HasBuff(BUFF_EARTH_PRISON) ||
		   this->HasBuff(BUFF_ICE_STORM_ENHANCED) ||
		   this->HasBuff(BUFF_BINDING);
}

bool Unit::HasSlowdownBuff() const
{
	return this->HasBuff(BUFF_ICE) ||
		   this->HasBuff(BUFF_COLD) ||
		   this->HasBuff(BUFF_FROZEN_STAB_MASTERED) ||
		   this->HasBuff(BUFF_CHILLING) ||
		   this->HasBuff(BUFF_FREEZING) ||
		   this->HasBuff(BUFF_PARALYSIS);
}

bool Unit::HasTeleportRestrictionBuff() const
{
	for (auto const& id : sGameServer->m_TeleportBuffRestrict)
	{
		if (this->HasBuff(id))
		{
			return true;
		}
	}

	return false;
}

bool Unit::HasTeleportCoordinateBuff() const
{
	for (auto const& id : sGameServer->m_TeleportBuffCoordinate)
	{
		if (this->HasBuff(id))
		{
			return true;
		}
	}

	return false;
}

uint8 Unit::GetActiveBuffCount() const
{
	int32 count = 0;

	BUFF_LOOP(i)
	{
		if ( this->GetBuffData(i)->GetID() != 0 )
		{
			++count;
		}
	}

	return count;
}

bool Unit::AttackAllowed(Unit * mTarget, bool targeted, bool reflect)
{
	if ( !mTarget )
	{
		return false;
	}

	if ( !this->SameDimension(mTarget) )
	{
		return false;
	}

	if ( mTarget->IsInSafeZone() )
	{
		return false;
	}

	if ( !mTarget->IsUnitAttackable() || !mTarget->IsPlaying() || !mTarget->IsLive() )
	{
		return false;
	}

	/*if ( !reflect && mTarget->IsTeleporting() )
	{
		return false;
	}*/
	if ( !reflect && mTarget->IsInTeleportInmuneState() )
	{
		return false;
	}

	if ( !reflect && mTarget->GetRegenStatus() != REGEN_NONE ) // TODO: Quizás haya que sacarlo
	{
		return false;
	}

	if ( mTarget->IsCreature() )
	{
		if ( mTarget->ToCreature()->IsDelayed() )
		{
			return false;
		}

		if ( mTarget->ToCreature()->GetSummonType() == 1 && mTarget->GetSummoner() == this )
		{
			return false;
		}
	}

	Unit * Source = this->GetSummoner();
	Unit * Target = mTarget->GetSummoner();

	SafeRAssert(Source, "Source == nullptr", false);
	SafeRAssert(Target, "Target == nullptr", false);

	if ( Player* pPlayer = Source->ToPlayer() )
	{
		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
		{
			return false;
		}

		if ( pPlayer->IsAdministrator() && !pPlayer->IsAdminPanelFlag(ADMIN_PANEL_ATTACK) )
		{
			return false;
		}

		if ( pPlayer->GetAttackStandby() > 0 )
		{
			return false;
		}

		if ( pPlayer->IsSpectator() )
		{
			return false;
		}

		Guild* pGuild = pPlayer->GuildGet();
		Monster* pMonster = Target->ToCreature();

		if ( pGuild && pMonster )
		{
			if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE && this->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) && pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL )
			{
				return false;
			}
		}

		if ( sWorldBoss->GetState() == WORLD_BOSS_STATE_PLAYING && sWorldBoss->GetBoss() == mTarget && pPlayer->GetCounter(PLAYER_COUNTER_WORLD_BOSS) < sGameServer->GetWorldBossRequiredPointsToAttack() )
		{
			return false;
		}
	}

	if ( Player* pPlayer = Target->ToPlayer() )
	{
		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
		{
			return false;
		}

		if ( pPlayer->IsAdministrator() && !pPlayer->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) )
		{
			return false;
		}

		if ( pPlayer->GetAttackStandby() > 0 )
		{
			return false;
		}

		if ( pPlayer->IsSpectator() )
		{
			return false;
		}

		/*Guild* pGuild = pPlayer->GuildGet();
		Monster* pMonster = Source->ToCreature();

		if ( pGuild && pMonster )
		{
			if ( pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE && this->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) && pGuild->GetWarData()->GetType() == GUILD_WAR_TYPE_NORMAL )
			{
				return false;
			}
		}*/
	}

	if ( Monster* pMonster = Source->ToCreature() )
	{
		if ( pMonster->GetAI() && !pMonster->GetAI()->IsTargetAllowed(Target) )
		{
			return false;
		}
	}

	if ( Source->IsPlayer() && Target->IsPlayer() )
	{
		return Source->ToPlayer()->AttackAllowedToPlayer(Target->ToPlayer(), targeted);
	}

	return true;
}

void Unit::PushBackSimple(Unit* mTarget)
{
	if ( !mTarget )
		return;

	if ( !mTarget->PushBackAllowed() )
		return;

	if (roll_chance_f(mTarget->GetFloatData(UNIT_FLOAT_RESIST_KNOCKBACK)))
	{
		this->MissSend(mTarget, nullptr, false, 0, MISS_TYPE_RESIST);
		return;
	}

	int32 target_direction = 0;

	if( Random(3) == 0 )
	{
		if( this->GetDirection() < 4 ) 
		{
			target_direction = this->GetDirection() + 4;
		}
		else
		{
			target_direction = this->GetDirection() - 4;
		}

		target_direction *= 2;
	}
	else 
	{
		target_direction = this->GetDirection() * 2;		
	}

	limitmin(target_direction, 0);
	limitmax(target_direction, Path::Max - 1);

	coord_type x = mTarget->GetX() + Path::Table[target_direction];
	coord_type y = mTarget->GetY() + Path::Table[target_direction + 1];

	World* pWorld = mTarget->GetWorld();

	if ( !pWorld )
		return;

	WorldGrid const& attr = pWorld->GetGrid(x, y);

	if ( attr.IsSafe() ||
		 attr.IsLocked_1() || 
		 attr.IsLocked_2() || 
		 ((mTarget->GetWorldId() == WORLD_CASTLE_SIEGE || IF_MAP_RANGE(mTarget->GetWorldId())) && attr.IsLocked_3()))
	{
		return;
	}
	
	mTarget->ClearPathData();
	mTarget->PositionSend(x, y);
}
	
void Unit::PushBackCount(Unit* mTarget, uint8 count)
{
	if ( !mTarget )
		return;

	if ( !mTarget->PushBackAllowed() )
		return;

	if (roll_chance_f(mTarget->GetFloatData(UNIT_FLOAT_RESIST_KNOCKBACK)))
	{
		this->MissSend(mTarget, nullptr, false, 0, MISS_TYPE_RESIST);
		return;
	}

	int32 target_direction = GetPathPacketDirPos(mTarget->GetX() - this->GetX(), mTarget->GetY() - this->GetY()) * 2;
	
	coord_type x = mTarget->GetX();
	coord_type y = mTarget->GetY();

	for ( uint8 n = 0; n < count; ++n )
	{
		if( n >= 2 && mTarget->IsPlayer() )
		{
			if ( mTarget->ToPlayer()->GetClass() == Character::DARK_KNIGHT ||
				 mTarget->ToPlayer()->GetClass() == Character::MAGIC_GLADIATOR ||
				 mTarget->ToPlayer()->GetClass() == Character::DARK_LORD )
			{
				mTarget->PushBackCheck(x, y, target_direction);
			}
		}
		else
		{
			mTarget->PushBackCheck(x, y, target_direction);
		}
	}

	mTarget->ClearPathData();
	mTarget->PositionSend(x, y);
}

bool Unit::PushBackAllowed()
{
	if ( !this->IsPlaying() || !this->IsLive() )
		return false;

	if ( !this->IsUnitAttackable() )
		return false;

	if ( this->IsTeleporting() )
		return false;

	if ( Monster* pMonster = this->ToCreature() )
	{
		if ( pMonster->GetAI() && !pMonster->GetAI()->PushBackAllowed() )
			return false;
	}

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( pPlayer->IsAdministrator() )
			return false;

		if ( sCastleSiege->GetCrownUser() == pPlayer )
		{
			if ( pPlayer->IsActiveInventoryItem(PET_DARK_HORSE) )
			{
				return false;
			}
		}

		if ( sCrywolf->IsInAltar(pPlayer) )
		{
			return false;
		}
	}

	if ( this->HasBuff(BUFF_ICE_ARROW) || this->HasBuff(BUFF_STUN) || this->HasBuff(BUFF_RESTRICTION) || this->HasBuff(BUFF_EARTH_PRISON) || this->HasBuff(BUFF_ICE_STORM_ENHANCED) || this->HasBuff(BUFF_BINDING) )
	{
		return false;
	}

	return true;
}

bool Unit::PushBackCheck(coord_type & x, coord_type & y, int32 & direction)
{
	int32 tdir = direction / 2;
	coord_type tx = x + Path::Table[direction];
	coord_type ty = y + Path::Table[direction+1];

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	WorldGrid const& attr = pWorld->GetGrid(tx, ty);

	if ( attr.IsSafe() || 
		 attr.IsLocked_1() || 
		 attr.IsLocked_2() || 
		 ((this->GetWorldId() == WORLD_CASTLE_SIEGE || IF_MAP_RANGE(this->GetWorldId())) && attr.IsLocked_3()) )
	{
		tdir += 4;

		if( tdir > 7 ) tdir -= 8;
		direction = tdir*2;
		return false;
	}

	x = tx;
	y = ty;
	return true;
}

bool Unit::MissCheck(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all)
{
	if ( !pTarget )
	{
		return false;
	}

	if ( this->IsPlayer() && pTarget->IsPlayer() )
	{
		return this->MissCheckPVP(pTarget, pSkill, send, count, miss_all);
	}
	else
	{
		return this->MissCheckNormal(pTarget, pSkill, send, count, miss_all);
	}
}
	
bool Unit::MissCheckNormal(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all)
{
	if ( !pTarget )
	{
		return false;
	}

	int32 attack_rate = this->GetIntData(UNIT_INT_ATTACK_RATE);
	int32 defense_rate = pTarget->GetIntData(UNIT_INT_DEFENSE_RATE);

	defense_rate -= static_cast<int32>(defense_rate * pTarget->GetEffect(BUFF_OPTION_DECREASE_DEFENSE_SUCCESS_RATE) / 100.0f);
	attack_rate -= static_cast<int32>(attack_rate * this->GetEffect(BUFF_OPTION_DECREASE_ATTACK_SUCCESS_RATE) / 100.0f);

	attack_rate -= (attack_rate * this->GetBuffEffect(BUFF_BLINDNESS)) / 100;

	if ( attack_rate <= 0 )
	{
		attack_rate = 1;
	}

	miss_all = (attack_rate < defense_rate);

	bool fail = miss_all ? Random(100) >= 5: Random(attack_rate) < defense_rate;

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->AIGroup("nixies_lake_stone_ai") )
		{
			fail = true;
			miss_all = true;
		}
	}

	if ( fail )
	{
		this->MissSend(pTarget, pSkill, send, count);
		return false;
	}

	return true;
}
	
bool Unit::MissCheckPVP(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all)
{
	if ( !pTarget )
	{
		return false;
	}

	float attack_rate = this->GetIntData(UNIT_INT_ATTACK_RATE_PVP);
	float defense_rate = pTarget->GetIntData(UNIT_INT_DEFENSE_RATE_PVP);

	attack_rate -= (attack_rate * this->GetBuffEffect(BUFF_BLINDNESS)) / 100;

	if ( attack_rate <= 0.0f || defense_rate <= 0.0f || this->GetTotalLevel() <= 0 || pTarget->GetTotalLevel() <= 0 )
	{
		return false;
	}

	float iExpressionA = (( attack_rate * 10000.0f ) / ( attack_rate + defense_rate )) / 10000.0f;
	float iExpressionB = (( this->GetTotalLevel() * 10000.0f ) / ( this->GetTotalLevel() + pTarget->GetTotalLevel() )) / 10000.0f;

	int32 iAttackSuccessRate = 100.0f * iExpressionA * sGameServer->GetSDAttackSuccessRate() * iExpressionB;

	if ( (pTarget->GetTotalLevel() - this->GetTotalLevel()) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if ( (pTarget->GetTotalLevel() - this->GetTotalLevel()) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if ( (pTarget->GetTotalLevel() - this->GetTotalLevel()) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}

	if ( Random(100) > iAttackSuccessRate )
	{
		this->MissSend(pTarget, pSkill, send, count);

		if ( sGameServer->shield_combo_miss_init.get() && this->ToPlayer()->GetCombo()->GetProgressIndex() >= 0 )
			this->ToPlayer()->GetCombo()->Init();

		return false;
	}

	return true;
}

bool Unit::TargetResistance(Unit * mTarget, uint16 skill)
{
	if ( !mTarget )
	{
		return false;
	}

	if ( mTarget->IsDebuffInmune(false) )
	{
		return false;
	}

	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(skill);

	if ( skill_base == SKILL_FIRE_SLASH )
	{
		mTarget->AddBuff(BUFF_DEFENSIBILITY_REDUCTION, BuffEffect(BUFF_OPTION_DECREASE_DEFENSE_PERCENT, 10 + this->MagicGetValue(SKILL_TREE_ADD_FIRE_SLASH_ENHANCED)), 10, 0, this);
	}

	if ( mTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill_base)) )
	{
		return false;
	}

	switch ( skill_base )
	{
	case SKILL_EARTHSHAKE:
		{
			this->PushBackCount(mTarget, 3);
		} break;

	case SKILL_FALLING_SLASH:
	case SKILL_LUNGE:
	case SKILL_UPPERCUT:
	case SKILL_CYCLONE:
	case SKILL_SLASH:
		{
			this->AddDelayed(DELAYED_PUSH_BACK, 150, mTarget);
		} break;

	case SKILL_LIGHTNING:
		{
			this->AddDelayed(DELAYED_PUSH_BACK, 150, mTarget);
		} break;

	case SKILL_POISON:
		{
			if ( mTarget->HasBuff(BUFF_POISON) )
				return false;

			mTarget->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), 20, 0, this);
		} break;

	case SKILL_ICE_ARROW:
		{
			if ( mTarget->HasBuff(BUFF_ICE_ARROW) )
				return false;

			mTarget->AddBuff(sSkillMgr->GetSkillBuffIcon(skill), 7, 0, this);
		} break;

	case SKILL_ICE:
		{
			if ( mTarget->HasBuff(BUFF_ICE) )
				return false;

			mTarget->AddBuff(BUFF_ICE, 10, 0, this);
		} break;

	case SKILL_ICE_STORM:
		{
			if ( mTarget->HasBuff(BUFF_ICE) )
				return false;

			mTarget->AddBuff(BUFF_ICE, 10, 0, this);
		} break;

	case SKILL_DECAY:
		{
			if ( mTarget->HasBuff(BUFF_POISON) )
				return false;

			mTarget->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), 10, 0, this);
		} break;

	case SKILL_STRIKE_OF_DESTRUCTION:
		{
										if (skill != 809 && skill != 1202)
										{
											if (mTarget->HasBuff(BUFF_COLD))
											{
												return false;
											}

											mTarget->AddBuff(BUFF_COLD, 10, 0, this);
										}
		} break;

	case SKILL_EARTH_PRISON:
		{
			if( roll_chance_i(sGameServer->GetSkillEarthPrisonStunRate()) )
			{
				if ( mTarget->HasBuff(BUFF_EARTH_PRISON) )
				{
					return false;
				}

				mTarget->AddBuff(BUFF_EARTH_PRISON, 5, 0, this);
			}
		} break;

	case SKILL_POISON_ARROW:
		{
			int32 poison_arrow_damage_min = 0;
			int32 poison_arrow_damage_max = 0;

			if ( Player* pPlayer = this->ToPlayer() )
			{
				pPlayer->ApplyWeaponDamage(skill, poison_arrow_damage_min, poison_arrow_damage_max);
			}
			else
			{
				poison_arrow_damage_min = this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN);
			}

			if (roll_chance_i(sGameServer->GetSkillPoisonArrowDebuffRate()))
			{
				mTarget->AddBuff(sSkillMgr->GetSkillBuffIcon(skill), BuffEffect(BUFF_OPTION_NONE, poison_arrow_damage_min / 5), 10, 0, this);
			}
		} break;

	case SKILL_FLAME_STRIKE:
		{
			mTarget->AddBuff(BUFF_FLAME_STRIKE, 1, 0, this);
		} break;

	case SKILL_LARGE_RING_BLOWER:
		{
			if( roll_chance_f(5 + this->MagicGetValue(554)) )
			{
				mTarget->AddBuff(BUFF_WEAKNESS, BuffEffect(BUFF_OPTION_WEAKNESS_PERCENT, 5), 10, 0, this);
			}
		} break;

	case SKILL_UPPER_BEAST:
		{
			if( roll_chance_f(5 + this->MagicGetValue(555)) )
			{
				mTarget->AddBuff(BUFF_DEFENSIBILITY_REDUCTION, BuffEffect(BUFF_OPTION_DECREASE_DEFENSE_PERCENT, 10), 10, 0, this);
			}
		} break;

	case SKILL_CHAIN_DRIVE:
		{
			if ( mTarget->HasBuff(BUFF_COLD) )
			{
				return false;
			}

			mTarget->AddBuff(BUFF_COLD, 10, 0, this);
		} break;

	case SKILL_POLLUTION:
		{
			if ( !mTarget->HasBuff(BUFF_ICE) )
			{
				mTarget->AddBuff(BUFF_ICE, 2, 0, this);
			}
		} break;

	case SKILL_LIGHTNING_SHOCK:
		{
			mTarget->AddBuff(BUFF_LIGHTNING_SHOCK, 1, 0, this);
		} break;

	case SKILL_GIGANTIC_STORM:
		{
			mTarget->AddBuff(BUFF_GIGANTIC_STORM, 1, 0, this);
		} break;

	case SKILL_PLASMA_STORM:
		{
			if ( this->IsPlayer() && mTarget->IsPlayer() )
			{
				if ( this->GetCastleSiegeJoinSide() != mTarget->GetCastleSiegeJoinSide() || this->GetCastleSiegeJoinSide() == 0 )
				{
					mTarget->ToPlayer()->ApplyFenrirEquipmentReduction();
				}
			}
		} break;

	case SKILL_PHOENIX_SHOT:
		{
			if ( roll_chance_f(10 + this->MagicGetValue(SKILL_TREE_PHOENIX_SHOT_ENHANCED)) )
			{
				mTarget->AddBuff(BUFF_DECREASE_DEFENSE_SUCCESS_RATE, BuffEffect(BUFF_OPTION_DECREASE_DEFENSE_SUCCESS_RATE, 10), 10, 0, this);
			}

			this->AddDelayed(DELAYED_PUSH_BACK, 150, mTarget);
		} break;

	case SKILL_CLASH:
		{
			this->PushBackCount(mTarget, 2);
		} break;
	}

	return true;
}

bool Unit::ResistElement(uint8 type)
{
	if ( type >= Element::MAX )
	{
		return false;
	}

	uint8 m_resistance = this->GetResistance(type);

	if ( m_resistance == uint8(-1) )
	{
		return true;
	}

	if ( m_resistance == 0 )
	{
		return false;
	}

	if ( this->HasBuff(BUFF_POTION_OF_SOUL) && m_resistance && (type == Element::ICE || type == Element::LIGHTNING) )
	{
		m_resistance += m_resistance * 50 / 100;
	}

	if ( Random(m_resistance) == 0 )
	{
		return false;
	}

	return true;
}

int32 Unit::GetTargetDefense(Unit * mTarget, uint16 & ignore_defense, uint8 attack_type)
{
	if ( !mTarget )
	{
		return 0;
	}
	
	int32 target_defense = mTarget->GetIntData(UNIT_INT_DEFENSE) + mTarget->GetIntData(UNIT_INT_DEFENSE_BASE);

	if (Player* pPlayer = mTarget->ToPlayer())
	{
		if (this->IsPlayer())
		{
			target_defense += pPlayer->GetData(PLAYER_DATA_INCREASE_DEFENSE_PVP);
		}
		else
		{
			target_defense += pPlayer->GetData(PLAYER_DATA_INCREASE_DEFENSE_PVM);
		}
	}

	if ( this->IsPlayer() && mTarget->IsPlayer() )
	{
		//target_defense += mTarget->GetIntData(UNIT_INT_DEFENSE_ADD_PVP) / 2;
		target_defense += mTarget->GetIntData(UNIT_INT_DEFENSE_ADD_PVP);
	}

	if ( Monster* pTargetMonster = mTarget->ToCreature() )
	{
		target_defense += static_cast<int32>(pTargetMonster->GetElementValue(SKILL_ELEMENT_DEFENSE));
	}

	target_defense -= target_defense * mTarget->GetEffect(BUFF_OPTION_DECREASE_DEFENSE_PERCENT) / 100;
	target_defense -= target_defense * mTarget->GetEffect(BUFF_OPTION_INNOVATION_PERCENT) / 100;
	target_defense -= mTarget->GetEffect(BUFF_OPTION_INNOVATION);

	target_defense -= mTarget->GetEffect(BUFF_OPTION_ELITE_DEFENSE_REDUCTION);

	if ( attack_type == 1 && this->IsPlayer() )
	{
		if (roll_chance_f(this->ToPlayer()->GetDarkSpirit()->GetIgnoreDefenseRate() - mTarget->GetFloatData(UNIT_FLOAT_RESIST_IGNORE_DEFENSE)))
		{
			target_defense = mTarget->GetIntData(UNIT_INT_DEFENSE_BASE);
			ignore_defense = DAMAGE_TYPE_IGNORE;
		}
	}
	else if ( attack_type == 0 )
	{
		if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE) - mTarget->GetFloatData(UNIT_FLOAT_RESIST_IGNORE_DEFENSE)))
		{
			target_defense = mTarget->GetIntData(UNIT_INT_DEFENSE_BASE);
			ignore_defense = DAMAGE_TYPE_IGNORE;
		}
	}

	target_defense -= this->GetEffect(BUFF_OPTION_IGNORE_BASE_DEFENSE);

	if ( target_defense < 0 )
	{
		target_defense = 0;
	}

	return target_defense;
}

int32 Unit::GetAttackDamage(int32 target_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget)
{
	int32 damage_min = 0;
	int32 damage_max = 0;
	int32 damage_add = 0;

	if (Player* pPlayer = this->ToPlayer())
	{
		int32 skill_damage_min = 0;
		int32 skill_damage_max = 0;

		if (mSkill)
		{
			this->SkillGetDamage(mSkill->GetSkill(), skill_damage_min, skill_damage_max);

			skill_damage_min += this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE);
			skill_damage_max += this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE);

			if (pTarget->IsPlayer())
			{
				skill_damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP);
				skill_damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP);
			}
			else
			{
				skill_damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM);
				skill_damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM);
			}

			uint8 attribute = sSkillMgr->GetSkillAttribute(mSkill->GetSkill());

			if (attribute != uint8(-1))
			{
				skill_damage_min += this->GetResistanceAdd(attribute);
				skill_damage_max += this->GetResistanceAdd(attribute);
			}

			uint16 base_skill = sSkillMgr->GetSkillBaseSkill(mSkill->GetSkill());

			if (pPlayer->GetClass() == Character::DARK_LORD)
			{
				switch (base_skill)
				{
				case SKILL_ELECTRIC_SPARK:
				{
											 if (Party* pPartySpark = pPlayer->GetParty())
											 {
												 int32 members = pPartySpark->GetPartyMembersWithinDistance(this->GetWorldId(), this->GetInstance(), this->GetX(), this->GetY(), 10);
												 int32 add_skill_damage = (pPlayer->GetTotalStat(LEADERSHIP) / 10) + (members * 50);
												 skill_damage_min += add_skill_damage;
												 skill_damage_max += add_skill_damage;
											 }
				} break;

				case SKILL_EARTHSHAKE:
				{
										 skill_damage_min += pPlayer->GetHorseDamage();
										 skill_damage_max += pPlayer->GetHorseDamage();
				} break;
				}
			}
		}

		pPlayer->ApplyWeaponDamage(mSkill ? mSkill->GetSkill() : 0, damage_min, damage_max);

		damage_min += skill_damage_min;
		damage_max += skill_damage_max;

		if (pTarget->IsPlayer())
		{
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVP);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVP);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVP);
		}
		else
		{
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVM);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVM);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVM);
		}

		if (Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) >= 4)
		{
			damage_min += this->MagicGetValue(SKILL_TREE_ADD_MARKSMAN);
			damage_max += this->MagicGetValue(SKILL_TREE_ADD_MARKSMAN);
		}

		if (Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) <= 2)
		{
			damage_min += this->MagicGetValue(SKILL_TREE_ADD_BATTLE_MIND);
			damage_max += this->MagicGetValue(SKILL_TREE_ADD_BATTLE_MIND);
		}
	}
	else
	{
		damage_min = this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN);
		damage_max = this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX);
	}

	this->ApplyBerserkerDamage(damage_min, damage_max);

	float infinity_arrow_damage = this->GetEffect(BUFF_OPTION_INFINITY_ARROW_DAMAGE);

	if (infinity_arrow_damage > 0)
	{
		damage_min += damage_min * infinity_arrow_damage / 100;
		damage_max += damage_max * infinity_arrow_damage / 100;
	}

	float demolish_value = this->GetEffect(BUFF_OPTION_DEMOLISH_DAMAGE_DECREASE);
	if (demolish_value > 0)
	{
		damage_min -= damage_min * demolish_value / 100;
		damage_max -= damage_max * demolish_value / 100;
	}

	float weakness_value = this->GetEffect(BUFF_OPTION_WEAKNESS_PERCENT);

	if (weakness_value > 0)
	{
		damage_min -= damage_min * weakness_value / 100;
		damage_max -= damage_max * weakness_value / 100;
	}

	weakness_value = this->GetEffect(BUFF_OPTION_WEAKNESS);

	if (weakness_value > 0)
	{
		damage_min -= weakness_value;
		damage_max -= weakness_value;
	}

	damage_add = damage_min + (Random(damage_max - damage_min));

	if ( this->CriticalDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_CRITICAL;
	}

	if ( this->ExcellentDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_EXE;
	}

	damage_add += this->GetIntData(UNIT_INT_DAMAGE_ADD);

	if ( this->IsCreature() )
	{
		damage_add += static_cast<int32>(this->ToCreature()->GetElementValue(SKILL_ELEMENT_ATTACK));
	}

	int32 elite_damage_reduction = pTarget->GetEffect(BUFF_OPTION_ELITE_DAMAGE_REDUCTION);
	if (elite_damage_reduction > 0)
	{
		damage_add -= elite_damage_reduction;
	}

	damage_add -= target_defense;

	limitmin(damage_add, 0);

	return damage_add;
}

int32 Unit::GetMagicDamage(int32 target_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget)
{
	if ( !mSkill )
	{
		return 0;
	}

	if ( this->IsCreature() )
	{
		return this->GetAttackDamage(target_defense, damage_type, mSkill, pTarget);
	}

	int32 damage_min = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MIN);
	int32 damage_max = this->GetIntData(UNIT_INT_MAGIC_DAMAGE_MAX);
	int32 damage_add = 0;
	uint16 base_skill = sSkillMgr->GetSkillBaseSkill(mSkill->GetSkill());

	if ( Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) >= 4 )
	{
		damage_min += this->MagicGetValue(SKILL_TREE_ADD_GRAND_MAGIC_IMPROVED);
		damage_max += this->MagicGetValue(SKILL_TREE_ADD_GRAND_MAGIC_IMPROVED);

		damage_min += this->MagicGetValue(763);
		damage_max += this->MagicGetValue(763);

		damage_min += this->MagicGetValue(SKILL_TREE_ADD_PAIN_OF_CURSE);
		damage_max += this->MagicGetValue(SKILL_TREE_ADD_PAIN_OF_CURSE);
	}

	switch ( base_skill )
	{
	case 220:
	case SKILL_EXPLOSION:
	case SKILL_REQUIEM:
	case SKILL_POLLUTION:
	case SKILL_DEATHSIDE:
		{
			damage_min = this->GetIntData(UNIT_INT_CURSE_DAMAGE_MIN);
			damage_max = this->GetIntData(UNIT_INT_CURSE_DAMAGE_MAX);

			if ( Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) >= 4 )
			{
				damage_min += this->MagicGetValue(SKILL_TREE_ADD_PAIN_OF_CURSE);
				damage_max += this->MagicGetValue(SKILL_TREE_ADD_PAIN_OF_CURSE);
			}
		} break;
	}

	if ( mSkill->GetSkill() == SKILL_NOVA )
	{
		if ( this->GetNova()->GetCount() >= 0 )
		{
			int32 count_damage = (this->GetNova()->GetCount() < Nova::Max)?Nova::Damage[this->GetNova()->GetCount()]:0;

			damage_min += (this->GetStrength() / 2 + count_damage);
			damage_max += (this->GetStrength() / 2 + count_damage);
		}
	}
	else
	{
		this->SkillGetDamage(mSkill->GetSkill(), damage_min, damage_max);
	}

	damage_min += this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE);
	damage_max += this->GetIntData(UNIT_INT_SKILL_ADD_DAMAGE);

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( pTarget->IsPlayer() )
		{
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVP);
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVP);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVP);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVP);
		}
		else
		{
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_SKILL_DAMAGE_PVM);
			damage_min += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVM);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_DAMAGE_PVM);
			damage_max += pPlayer->GetData(PLAYER_DATA_INCREASE_MAX_DAMAGE_PVM);
		}
	}

	uint8 attribute = sSkillMgr->GetSkillAttribute(mSkill->GetSkill());

	if ( attribute < Element::MAX )
	{
		damage_min += this->GetResistanceAdd(attribute);
		damage_max += this->GetResistanceAdd(attribute);
	}

	float infinity_arrow_damage = this->GetEffect(BUFF_OPTION_INFINITY_ARROW_DAMAGE);

	if (infinity_arrow_damage > 0)
	{
		damage_min += damage_min * infinity_arrow_damage / 100;
		damage_max += damage_max * infinity_arrow_damage / 100;
	}

	float demolish_value = this->GetEffect(BUFF_OPTION_DEMOLISH_DAMAGE_DECREASE);
	if (demolish_value > 0)
	{
		damage_min -= damage_min * demolish_value / 100;
		damage_max -= damage_max * demolish_value / 100;
	}

	float weakness_value = this->GetEffect(BUFF_OPTION_WEAKNESS_PERCENT);

	if (weakness_value > 0)
	{
		damage_min -= damage_min * weakness_value / 100;
		damage_max -= damage_max * weakness_value / 100;
	}

	weakness_value = this->GetEffect(BUFF_OPTION_WEAKNESS);

	if (weakness_value > 0)
	{
		damage_min -= weakness_value;
		damage_max -= weakness_value;
	}

	switch ( base_skill )
	{
	case 220:
	case SKILL_EXPLOSION:
	case SKILL_REQUIEM:
	case SKILL_POLLUTION:
	case SKILL_DEATHSIDE:
		{
			if ( Player* pPlayer = this->ToPlayer() )
			{
				pPlayer->ApplyCurseDamage(mSkill ? mSkill->GetSkill(): 0, damage_min, damage_max);
			}
		} break;

	default:
		{
			if ( Player* pPlayer = this->ToPlayer() )
			{
				pPlayer->ApplyStaffDamage(mSkill ? mSkill->GetSkill(): 0, damage_min, damage_max);
			}
		} break;
	}
	
	damage_add = damage_min + Random(damage_max - damage_min);

	if ( this->CriticalDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_CRITICAL;
	}

	if ( this->ExcellentDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_EXE;
	}

	damage_add += this->GetIntData(UNIT_INT_DAMAGE_ADD);

	int32 elite_damage_reduction = pTarget->GetEffect(BUFF_OPTION_ELITE_DAMAGE_REDUCTION);
	if (elite_damage_reduction > 0)
	{
		damage_add -= elite_damage_reduction;
	}

	damage_add -= target_defense;

	limitmin(damage_add, 0);

	return damage_add;
}

int32 Unit::GetFenrirAttackDamage(int32 target_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget)
{
	int32 fenrir_damage = 0;
	int32 damage_add = 0;
	int32 damage_min = 0;
	int32 damage_max = 0;

	if ( Player* pPlayer = this->ToPlayer() )
	{
		fenrir_damage = pPlayer->GetData(PLAYER_DATA_FENRIR_DAMAGE);
	}
	else
	{
		fenrir_damage = this->GetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN);
	}

	if ( fenrir_damage < 0 )
	{
		fenrir_damage = 0;
	}

	this->SkillGetDamage(mSkill->GetSkill(), damage_min, damage_max);

	damage_min += fenrir_damage;
	damage_max += fenrir_damage;

	damage_add = damage_min + Random((damage_max - damage_min) + 1);

	if ( this->CriticalDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_CRITICAL;
	}

	if ( this->ExcellentDamageChance(pTarget, false, damage_add, damage_max) )
	{
		damage_type = DAMAGE_TYPE_EXE;
	}

	damage_add -= target_defense;

	limitmin(damage_add, 0);

	return damage_add;
}

int32 Unit::GetMajesticSpecialDamage(int32 target_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget)
{
	int32 damage_add = 0;
	int32 damage_min = 0;
	int32 damage_max = 0;

	this->SkillGetDamage(mSkill->GetSkill(), damage_min, damage_max);

	damage_add = damage_min;
	damage_type = DAMAGE_TYPE_IGNORE | DAMAGE_PROP_TRIPLE;

	//damage_add -= target_defense;

	limitmin(damage_add, 0);

	return damage_add;
}

void Unit::ApplyBerserkerDamage(int32 & damage_min, int32 & damage_max)
{
	int32 berserker_effect = this->GetBuffEffect(BUFF_NARS_BERSERKER);

	damage_min += berserker_effect;
	damage_max += berserker_effect;
}

void Unit::AttackProc(Unit * pTarget, Skill * mSkill, bool magic_send, int32 damage, uint8 combo, uint16 damage_type, int32 count)
{
	if ( !pTarget )
	{
		return;
	}

	if ( !this->IsLive() || pTarget == this )
	{
		return;
	}

	if ( !this->AttackAllowed(pTarget, true) )
	{
		return;
	}

	if ( !this->ViewportAttack(pTarget) )
	{
		return;
	}

	if (damage <= 0 && sGameServer->IsAttackStunCheck())
	{
		if (this->HasBuff(BUFF_STUN) ||
			this->HasBuff(BUFF_SLEEP))
		{
			return;
		}
	}

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( sGameServer->IsAntiHackCoordinateAttackCheck() )
		{
			WorldGrid const& player_grid = pPlayer->GetGrid();

			if ( player_grid.IsSafe() || player_grid.IsLocked_1() || player_grid.IsLocked_2() )
			{
				sLog->outError("antihack", "%s -- %s attacked on wrong coordinate (%u)", __FUNCTION__, pPlayer->BuildLog().c_str(), player_grid.attribute);
				return;
			}

			if ( pPlayer->GetWorldId() == WORLD_CASTLE_SIEGE && player_grid.IsLocked_3() )
			{
				sLog->outError("antihack", "%s -- %s attacked on wrong coordinate (%u)", __FUNCTION__, pPlayer->BuildLog().c_str(), player_grid.attribute);
				return;
			}
		}
	}

	if ( this->IsCreature() && this->ToCreature()->GetAI() )
	{
		this->ToCreature()->GetAI()->ExtraDamage(pTarget, damage);
	}

	uint16 skill = mSkill ? mSkill->GetSkill() : 0;

	if ( !damage && skill != SKILL_PLASMA_STORM && !this->ConsumeSkillItem() )
	{
		return;
	}

	PLAYER_POINTER(this)->UpdateAutorecuperation();
	PLAYER_POINTER(pTarget)->UpdateAutorecuperation();
	PLAYER_POINTER(this)->DarkSpiritSetTarget(pTarget);
	this->SetSummonedTarget(pTarget);
	PLAYER_POINTER(this)->SetMuunTarget(pTarget);

	Unit * pSourceMaster = this->GetSummoner();
	Unit * pTargetMaster = pTarget->GetSummoner();

	if ( !pSourceMaster || !pTargetMaster )
	{
		return;
	}

	this->RemoveBuff(BUFF_TRANSPARENCY);

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		pMonster->SetLastHitTime(MyGetTickCount());
	}

	if ( Monster* pMonster = this->ToCreature() )
	{
		pMonster->SetLastHitTime(MyGetTickCount());
	}

	if ( pTarget->HasBuff(mSkill ? BUFF_INMUNE_SKILL: BUFF_INMUNE_ATTACK) )
	{
		this->MissSend(pTarget, mSkill, magic_send, count);
		return;
	}

	if ( pTarget->HasBuff(BUFF_ORDER_OF_PROTECTION) )
	{
		this->MissSend(pTarget, mSkill, magic_send, count);
		return;
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetAI() && !pMonster->GetAI()->EnableAttack(this, mSkill, count) )
		{
			return;
		}
	}

	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(skill);
	int8 skill_type = sSkillMgr->GetSkillType(skill);
	bool miss_all = false;
	bool success = false;
	uint8 interrupt = 0;

	if( mSkill && count <= 1 )
	{
		success = this->TargetResistance(pTarget, skill);
	}

	if ( damage == 0 )
	{
		if ( !this->MissCheck(pTarget, mSkill, magic_send, count, miss_all) )
		{
			if ( damage_type != DAMAGE_TYPE_REFLECT )
			{
				this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
			}

			return;
		}

		int32 target_defense = this->GetTargetDefense(pTarget, damage_type, 0);

		if ( Player* pPlayer = this->ToPlayer() )
		{
			if ( skill_base == SKILL_POISON_STORM ||
				 skill_base == SKILL_FROZEN_SLAUGHTER ||
				 skill_base == SKILL_BLEEDING_HEAT || 
				 skill_base == SKILL_SHOCK_BOOM )
			{
				damage = this->GetMajesticSpecialDamage(target_defense, damage_type, mSkill, pTarget);
			}
			else if ( skill_base == SKILL_PLASMA_STORM )
			{
				damage = this->GetFenrirAttackDamage(target_defense, damage_type, mSkill, pTarget);
			}
			else
			{
				switch( skill_type )
				{
				case SkillNone:
				case SkillNormal:
					{
						damage = this->GetAttackDamage(target_defense, damage_type, mSkill, pTarget);

						if ( damage > 0 )
						{
							pPlayer->WeaponDurabilityDown(pTarget, true);
						}
					} break;

				case SkillMagic:
					{
						damage = this->GetMagicDamage(target_defense, damage_type, mSkill, pTarget);

						if ( damage > 0 )
						{
							pPlayer->WeaponDurabilityDown(pTarget, false);
						}
					} break;
				}
			}
		}
		else
		{
			damage = this->GetAttackDamage(target_defense, damage_type, mSkill, pTarget);
		}

		if ( miss_all )
		{
			damage = damage * 30 / 100;
		}

		switch ( skill_base )
		{
		case SKILL_FALLING_SLASH:
		case SKILL_LUNGE:
		case SKILL_UPPERCUT:
		case SKILL_CYCLONE:
		case SKILL_SLASH:
		case SKILL_TWISTING_SLASH:
		case SKILL_RAGEFUL_BLOW:
		case SKILL_DEATH_STAB:
		case SKILL_STRIKE_OF_DESTRUCTION:
		case SKILL_FIRE_BLOW:
		case SKILL_SWORD_BLOW:
		case SKILL_IMPALE:
		case SKILL_POWER_SLASH:
		case SKILL_FLAME_STRIKE:
		case SKILL_CRESCENT_MOON_SLASH:
		case SKILL_FIRE_BREATH:
		case SKILL_FIRE_SLASH:
		case SKILL_SPIRAL_SLASH:
			{
				if (Player * pPlayer = this->ToPlayer())
				{
					if (pPlayer->GetClass() == Character::DARK_KNIGHT)
					{
						if (pPlayer->GetBuffByGroup(325))
						{
							damage = damage * (50 + (this->GetEnergy() / 10)) / 100;
						}
						else
						{
							damage = damage * (200 + (this->GetEnergy() / 10)) / 100;
						}
					}
					else if (pPlayer->GetClass() == Character::SLAYER)
					{
						damage = damage * ((this->GetStrength() / 8) + (this->GetAgility() / 28) + 120) / 100;
					}
					else
					{
						damage = damage * 2;
					}
				}
			} break;

		case SKILL_STARFALL:
		case SKILL_ICE_ARROW:
		case SKILL_PENETRATION:
			{
				damage = damage * 2;
			} break;

		case SKILL_FORCE:
		case SKILL_FIRE_BURST:
		case SKILL_EARTHSHAKE:
		case SKILL_ELECTRIC_SPARK:
		case SKILL_FIRE_BLAST:
		case SKILL_FORCE_WAVE:
			{
				damage = damage * (200 + (this->GetEnergy() / 20)) / 100;
			} break;

		case SKILL_CHAOTIC_DESEIER:
		case SKILL_EVIL_WIND:
			{
				damage = damage * (200 + (this->GetEnergy() / 25)) / 100;
			} break;

		case SKILL_PLASMA_STORM:
			{
				int32 damage_inc = this->GetLevel();

				if ( Player* pPlayer = this->ToPlayer() )
				{
					damage_inc = pPlayer->GetTotalLevel() - 300;
						
					if ( damage_inc < 0 )
						damage_inc = 0;

					damage_inc /= 5;
				}
					
				damage = (damage * (damage_inc + 200)) / 100;
			} break;

		case SKILL_CHAIN_LIGHTNING:
			{
				switch(combo)
				{
				case 1:
					break;
				case 2:
					damage = damage * 70 / 100;
					break;
				case 3:
					damage = damage * 50 / 100;
					break;
				case 4:
					damage = damage * 30 / 100;
					break;
				default:
					return;
				}

				combo = 0; 
			} break;

		/*case SKILL_EXPLOSION:
			{
				if ( pTarget->IsDebuffInmune(false) )
				{
					break;
				}

				if ( pTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill)) )
				{
					break;
				}

				if ( !pTarget->HasBuff(BUFF_EXPLOSION) )
				{
					int32 book_damage = 60 * damage / 100;
					book_damage += book_damage * this->MagicGetValue(SKILL_TREE_FIRE_TOME_MASTERY) / 100;
					pTarget->AddBuff(BUFF_EXPLOSION, BuffEffect(BUFF_OPTION_NONE, book_damage), this->GetBookHitCount(), 0, this, false);
				}
			} break;*/

		/*case SKILL_REQUIEM:
			{
				if ( pTarget->IsDebuffInmune(false) )
				{
					break;
				}

				if ( pTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill)) )
				{
					break;
				}

				if ( !pTarget->HasBuff(BUFF_REQUIEM) )
				{
					int32 book_damage = 60 * damage / 100;

					pTarget->AddBuff(BUFF_REQUIEM, BuffEffect(BUFF_OPTION_NONE, book_damage), this->GetBookHitCount(), 0, this, false);
				}
			} break;*/

		case SKILL_DRAIN_LIFE:
			{
				int32 life_drain = 0;

				if (pTarget->IsPlayer())
				{
					life_drain = (this->GetEnergy() / 23) + (10 * damage / 100);
				}
				else
				{
					life_drain = (this->GetEnergy() / 15) + (pTarget->GetTotalLevel() / 2.5f);
				}

				life_drain += this->MagicGetValue(SKILL_TREE_ADD_DRAIN_LIFE_IMPROVED);

				this->PowerIncrease(POWER_LIFE, life_drain, true);

				if ( pTarget->IsDebuffInmune(false) )
				{
					break;
				}

				if ( pTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill)) )
				{
					break;
				}
			} break;

		case SKILL_HARSH_STRIKE:
			{
				if (count == 1)
				{
					damage = (damage * 1.0) * (this->GetStrength() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RETALIATION_DAMAGE)) / 100;
				}
				else if (count == 2)
				{
					damage = (damage * 1.1) * (this->GetStrength() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RETALIATION_DAMAGE)) / 100;
				}
				else
				{
					damage = (damage * 1.1) * (this->GetStrength() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RETALIATION_DAMAGE)) / 100;
				}
			} break;

		case SKILL_FIRE_SCREAM:
			{
				damage = damage * (200 + (this->GetEnergy() / 20)) / 100;

				SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillAttackFireScreamExplosion(damage);
			} break;

		case SKILL_SPIN_STEP:
			{
				SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillSpinStepExplosion(damage);

				damage = (damage * 1.1f) * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
			} break;

		case SKILL_MAGIC_PIN:
			{
				switch (count)
				{
				case 1:
					{
						damage = (damage * 0.8f) * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
					} break;

				case 2:
					{
						damage = damage * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
					} break;

				case 3:
					{
						damage = (damage * 1.1f) * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
					} break;

				case 4:
					{
						damage = (damage * 1.2f) * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
					} break;

				case 5:
					{
						damage = (damage * 1.3f) * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;
					} break;
				}

				if (count == 1)
				{
					SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillMagicPinExplosion(damage);
				}
			} break;

		case SKILL_SHINING_PEAK:
			{	
				damage = damage * (((this->GetStrength() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RETALIATION_DAMAGE)) + (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE))) * 0.8f) / 100;
				damage = damage / 3;

				switch ( count )
				{
				case 1:
					{
						//this->AddDelayed(DELAYED_ATTACK, (100+(count*100)), pTarget, mSkill->GetSkill(), damage, 0, 0, count + 1);
						this->AddDelayed(DELAYED_ATTACK, (100+(count*100)), pTarget, mSkill->GetSkill(), 0, 0, 0, count + 1);

						//damage = int32(float(damage) * 0.30f);
					} break;

				case 2:
					{
						//this->AddDelayed(DELAYED_ATTACK, (100+(count*100)), pTarget, mSkill->GetSkill(), damage, 0, 0, count + 1);
						this->AddDelayed(DELAYED_ATTACK, (100+(count*100)), pTarget, mSkill->GetSkill(), 0, 0, 0, count + 1);

						//damage = int32(float(damage) * 0.34f);
					} break;

				case 3:
					{
						//damage = int32(float(damage) * 0.36f);
					} break;
				}
			} break;

		case SKILL_BRECHE:
			{
				damage = (damage * 0.5f) * (this->GetStrength() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RETALIATION_DAMAGE)) / 100;
			} break;

		case SKILL_SPIRIT_HOOK:
			{
				damage = (damage + this->GetVitality() / 10) * (this->GetVitality() / 10 + 50) / 100;
			} break;

		case SKILL_ICE_BLOOD:
			{
				damage = damage * 2;

				if (roll_chance_i(6))
				{
					pTarget->AddBuff(BUFF_COLD, 10, 0, this);
				}
			} break;

		case SKILL_FIRE_BLOOD:
			{
				damage = damage * 2;

				if ( pTarget->IsDebuffInmune(false) )
				{
					break;
				}

				if ( pTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill)) )
				{
					break;
				}

				if (roll_chance_i(6))
				{
					int32 debuff_damage = this->GetStrength() / 10;

					pTarget->AddBuff(BUFF_EXPLOSION, BuffEffect(BUFF_OPTION_NONE, debuff_damage), 10, 0, this, false);
				}
			} break;

		case SKILL_LARGE_RING_BLOWER:
		case SKILL_UPPER_BEAST:
		case SKILL_CHARGE:
			{
				damage = damage * (this->GetVitality() / 10 + 50) / 100;
			} break;

		case SKILL_CHAIN_DRIVE:
			{
				damage = (damage + this->GetVitality() / 10) * (this->GetVitality() / 10 + 50) / 100;
			} break;

		case SKILL_DARK_SIDE:
			{
				damage = (this->GetAgility() / 8 + damage + this->GetEnergy() / 10) * (this->GetAgility() / 8 + this->GetEnergy() / 10 + 100) / 100;
			} break;

		case SKILL_DRAGON_LORE:
			{
				damage = (damage + this->GetEnergy() / 10) * (this->GetEnergy() / 10 + 50) / 100;
			} break;

		case SKILL_DRAGON_SLAYER:
			{
				if (pTarget->IsPlayer())
				{
					damage = damage * (this->GetEnergy() / 10 + 50) / 100;
				}
				else
				{
					damage = (damage * (this->GetEnergy() / 10 + 50) / 100 + 100) * 3;
				}
			} break;

		case SKILL_PHOENIX_SHOT:
			{
				damage = damage * (this->GetVitality() / 10 + 200) / 100;
			} break;

		case SKILL_DARK_PHOENIX_SHOT:
			{
				damage = damage * ((this->GetAgility() / 10) + 50) / 100;
			} break;

		case SKILL_POISON_STORM:
			{
				SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillPoisonStormSplash(damage);
			} break;

		case SKILL_FROZEN_SLAUGHTER:
			{
				SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillFrozenSlaughterSplash(damage);
			} break;

		case SKILL_BLEEDING_HEAT:
			{
					SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillBloodyingHitSplash(damage);
			} break;

		case SKILL_MAGIC_ARROW:
			{
				SkillHandler(this, pTarget, mSkill, sSkillMgr->GetSkill(skill)).SkillMagicArrowSplash(damage);
			} break;

		case SKILL_SWORD_INERTIA:
			{
				damage = damage * ((this->GetStrength() / 8) + (this->GetAgility() / 28) + 120) / 100;
				//damage = damage * 0.16f;
			} break;

		case SKILL_BAT_FLOCK:
			{
				damage = damage * ((this->GetStrength() / 8) + (this->GetAgility() / 28) + 120) / 100;
				//damage = damage * 0.5f;

				if (!pTarget->IsDebuffInmune(false) && !pTarget->ResistElement(sSkillMgr->GetSkillAttribute(skill)) && !pTarget->HasBuff(BUFF_BAT_FLOCK))
				{
					pTarget->AddBuff(BUFF_BAT_FLOCK, BuffEffect(BUFF_OPTION_NONE, damage / 10), 3 + this->MagicGetValue(782), 0, this);
				}
			} break;

		case SKILL_PIERCE_ATTACK:
			{
				damage = damage * ((this->GetStrength() / 8) + (this->GetAgility() / 28) + 120) / 100;
			} break;
		}

		if (Player* pPlayer = this->ToPlayer())
		{
			if (!sGameServer->IsMajesticAddDamageNormal())
			{
				damage += pPlayer->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_DAMAGE);
			}

			if (skill != 0)
			{
				damage += pPlayer->GetData(PLAYER_DATA_MAJESTIC_SKILL_DAMAGE);

				skill_template const* pSkillInfo = sSkillMgr->GetSkill(skill);

				if (pSkillInfo)
				{
					if (pTarget->IsCreature())
					{
						if (pSkillInfo->GetPVMDamage() != 0)
						{
							damage = damage * pSkillInfo->GetPVMDamage() / 100;
						}
					}
					else if (pTarget->IsPlayer())
					{
						if (pSkillInfo->GetPVPDamage() != 0)
						{
							damage = damage * pSkillInfo->GetPVPDamage() / 100;
						}
					}
				}
			}
		}

		if (skill_base != SKILL_POISON_STORM &&
			skill_base != SKILL_FROZEN_SLAUGHTER &&
			skill_base != SKILL_BLEEDING_HEAT &&
			skill_base != SKILL_SHOCK_BOOM)
		{
			PLAYER_POINTER(this)->ReduceAttackHP(skill_base);

			if (this->PowerGet(POWER_LIFE) > 0)
			{
				damage += this->GetIncreaseDamageRate() * damage / 100;
			}

			pTarget->ApplyDamageDecrease(damage);

			damage -= pTarget->GetAbsorbDamageRate() * damage / 100;

			int32 min_damage = this->GetTotalLevel() / 10;

			if (damage < min_damage)
			{
				if (min_damage < 1)
				{
					min_damage = 1;
				}

				damage = min_damage;
			}
		}

		PLAYER_POINTER(pTarget)->DamagePet(damage);
		PLAYER_POINTER(pTarget)->DamageMount(damage);

		sMuunSystem->MuunSprite(pTarget->ToPlayer(), damage);

		this->ApplySkillCountEffect(pTarget, skill_base, count, damage_type);

		if ( skill_base != SKILL_POISON_STORM &&
			 skill_base != SKILL_FROZEN_SLAUGHTER &&
			 skill_base != SKILL_BLEEDING_HEAT &&
			 skill_base != SKILL_SHOCK_BOOM )
		{
			if ( this->IsPlayer() && pTarget->IsPlayer() )
			{
				damage += this->ToPlayer()->GetData(PLAYER_DATA_380_PVP_DAMAGE);
			}

			if ( this->IsPlayer() && pTarget->IsPlayer() )
			{
				this->ToPlayer()->PVPDamageReduction(damage, pTarget->ToPlayer());
			}

			if ( Player* pPlayer = this->ToPlayer() )
			{
				pPlayer->ApplyMajesticDamage(pTarget->ToCreature(), damage, false);
			}

			if ( Player* pPlayer = pTarget->ToPlayer() )
			{
				pPlayer->ApplyMajesticDamage(this->ToCreature(), damage, true);
			}

			if ( Player* pPlayer = this->ToPlayer() )
			{
				if ( pTarget->IsPlayer() )
				{
					damage = damage * sGameServer->GetDamageModPVP(pPlayer->GetClass()) / 100;
				}
				else
				{
					damage = damage * sGameServer->GetDamageModPVE(pPlayer->GetClass()) / 100;
				}

				if ( sWorldBoss->GetState() == WORLD_BOSS_STATE_PLAYING && sWorldBoss->GetBoss() == pTarget )
				{
					damage += (damage * (sGameServer->GetWorldBossIncreaseDamagePerPoint() * pPlayer->GetCounter(PLAYER_COUNTER_WORLD_BOSS))) / 100;
				}
			}

			if ( combo )
			{
				int32 combo_damage = this->GetIntData(UNIT_INT_COMBO_DAMAGE);
				damage += combo_damage;
				damage += (combo_damage * this->MagicGetValue(SKILL_TREE_ADD_COMBO_DAMAGE)) / 100;
				damage_type |= DAMAGE_PROP_COMBO;
				//skill = SKILL_COMBO;
			}

			if (this->TripleDamageChance(pTarget, false, damage))
			{
				damage_type |= DAMAGE_PROP_TRIPLE;
			}
			else if ( this->DoubleDamageChance(pTarget, false, damage) )
			{
				damage_type |= DAMAGE_PROP_DOUBLE;
			}
			
			if ( this->IsCreature() )
				MONSTER_AI(this->ToCreature())->FinalDamageMod(damage, false);

			if ( pTarget->IsCreature() )
				MONSTER_AI(pTarget->ToCreature())->FinalDamageMod(damage, true);
		}
	}
	else
	{
		if ( damage_type != DAMAGE_TYPE_REFLECT )
		{
			if ( this->IsPlayer() && pTarget->IsPlayer() )
			{
				this->ToPlayer()->PVPDamageReduction(damage, pTarget->ToPlayer());
			}
		}
	}

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( pTarget->IsCreature() )
		{
			if ( pTarget->ToCreature()->IsGuardianStatue() || pTarget->ToCreature()->IsCastleSiegeGate() )
			{
				this->ToPlayer()->AccumulateDamage(damage);
			}
		}
	}

	if ( damage_type != DAMAGE_TYPE_REFLECT )
	{
		if ( Player* pPlayer = pTarget->ToPlayer() )
		{
			if (roll_chance_f(pPlayer->GetDataFloat(PLAYER_DATA_SHIELD_BLOCK)))
			{
				this->MissSend(pTarget, mSkill, magic_send, count, MISS_TYPE_BLOCK);
				this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
				return;
			}

			if (roll_chance_f(pPlayer->GetDataFloat(PLAYER_DATA_SHIELD_PROTECTION)))
			{
				damage -= pPlayer->GetData(PLAYER_DATA_SHIELD_PROTECTION_VALUE);

				if ( damage <= 0 )
				{
					this->MissSend(pTarget, mSkill, magic_send, count, MISS_TYPE_ABSORB);
					this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
					return;
				}
			}

			if ( skill_type != SkillMagic )
			{
				if (roll_chance_f(pPlayer->GetDataFloat(PLAYER_DATA_WEAPON_BLOCK)))
				{
					this->MissSend(pTarget, mSkill, magic_send, count, MISS_TYPE_PARRY);
					this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
					return;
				}
			}
		}
	}

	if ( damage < 0 )
	{
		damage = 0;
	}

	if ( damage_type != DAMAGE_TYPE_REFLECT )
	{
		if ( !sGameServer->IsAttackDebuffFirstHit() || count <= 1 )
		{
			PLAYER_POINTER(this)->OffensiveFullRestore();
		}

		if ( !sGameServer->IsAttackDebuffFirstHit() || count <= 1 )
		{
			if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_STUN_RATE)))
			{
				if ( !pTarget->IsDebuffInmune(false) && !pTarget->HasBuff(BUFF_STUN) )
				{
					pTarget->AddBuff(BUFF_STUN, 2, 0, this);
				}
			}

			if ( this->HasBuff(BUFF_BLOOD_HOWLING) || this->HasBuff(BUFF_BLOOD_HOWLING_IMPROVED) )
			{
				float rate = sGameServer->GetSkillBloodHowlingRate() + this->MagicGetValue(SKILL_TREE_ADD_BLOOD_HOWLING_IMPROVED);

				if (roll_chance_f(rate))
				{
					if ( !pTarget->IsDebuffInmune(false) )
					{
						int32 howling_damage = sGameServer->GetSkillBloodHowlingDamage();

						howling_damage += howling_damage * this->MagicGetValue(SKILL_TREE_ADD_BLOOD_HOWLING_IMPROVED) / 100;

						pTarget->AddBuff(BUFF_DEATH_STAB_ENHANCED, BuffEffect(BUFF_OPTION_NONE, howling_damage), 10, 0, this);
					}
				}
			}
		}

		if ( pTarget->IsPlayer() )
		{
			int32 reduce_effect = this->GetEffect(BUFF_OPTION_CIRCLE_SHIELD);

			if (reduce_effect > 0 && roll_chance_f(sGameServer->GetSkillCircleShieldReduceAGRate() + this->MagicGetValue(SKILL_TREE_ADD_CIRCLE_SHIELD_MASTERY)))
			{
				pTarget->PowerReduce(POWER_STAMINA, reduce_effect, true);
			}
		}
	}

	if( mSkill && count <= 1 )
	{
		if( magic_send )
		{
			this->MagicAttackSend(ENTRY(pTarget), skill, success);
		}
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetClass() == 709 ||
			 pMonster->GetClass() == 710 ||
			 pMonster->GetClass() == 711 ||
			 pMonster->GetClass() == 712 )
		{
			damage = 1;
		}
	}

	auto pSolidProtection = pTarget->GetSolidProtectionBuff();
	if (pSolidProtection)
	{
		auto damage_transfer = pSolidProtection->FindEffect(BUFF_OPTION_SOLID_PROTECTION_DAMAGE_TRANSFER);
		if (damage_transfer > 0)
		{
			auto pCaster = pSolidProtection->GetCaster();
			if (pCaster && pCaster->IsPlaying() && pCaster->IsLive())
			{
				Player* pCasterPlayer = pCaster->ToPlayer();
				if (pCasterPlayer)
				{
					Player* pPlayer = pTarget->ToPlayer();
					if (pPlayer && pPlayer->GetPartyID() == pCasterPlayer->GetPartyID() && pPlayer->GetPartyID() != PARTY_NULL)
					{
						if (IN_RANGE(pCaster, pPlayer, 15))
						{
							int32 transfer_value = damage * damage_transfer / 100;
							if (transfer_value < 0)
							{
								transfer_value = 0;
							}

							damage -= transfer_value;
							if (damage < 0)
							{
								damage = 0;
							}

							if (transfer_value > 0)
							{
								pCaster->DealDamage(pCaster, transfer_value, 0, skill_base, false, MONSTER_THREAT_DAMAGE_NORMAL);
								pCaster->SendDamage(pCaster, transfer_value, 0, interrupt, damage_type);

								if (pCaster->PowerGet(POWER_LIFE) <= 0)
								{
									pCaster->Unit::AfterHitCheck(pCaster, 0, 0, 0);
								}
							}
						}
					}
				}
			}
		}
	}

	int32 shield_damage = this->DealDamage(pTarget, damage, 0, skill_base, false, damage_type == DAMAGE_TYPE_REFLECT ? MONSTER_THREAT_DAMAGE_REFLECT : MONSTER_THREAT_DAMAGE_NORMAL);

	if ( pTarget->IsCreature() )
	{
		pTarget->AddDelayed(DELAYED_MONSTER_HIT, 100, this);

		if ( pTarget->ToCreature()->IsKundun() )
		{
			if ( !pTarget->ToCreature()->IsDelayed() && Random(15) == 0 )
			{
				pTarget->ToCreature()->SetBattleDelay(10);
				pTarget->ToCreature()->ActionSend(this->GetEntry(), 126, pTarget->GetDirection());
			}
		}
	}

	if ( damage > 0 )
	{
		PLAYER_POINTER(pSourceMaster)->SelfDefenseActive(pTargetMaster->ToPlayer());
		PLAYER_POINTER(pTarget)->RandomArmorDurabilityDown(this, damage);
	}

	this->SetRest(0);

	if ( damage )
	{
		pTarget->ReduceBuffTime(BUFF_ICE_ARROW, 1);
		pTarget->RemoveBuff(BUFF_SLEEP);

		if ( damage_type != DAMAGE_TYPE_REFLECT )
		{
			pTarget->ReflectDamage(damage, this);
			pTarget->TotalPowerRecovery();
		}

		if ( !sGameServer->IsAttackDebuffFirstHit() || count <= 1 )
		{
			if ( Player* pPlayer = this->ToPlayer() )
			{
				int32 add_hp = pPlayer->GetDataFloat(PLAYER_DATA_ABSORB_HP);
				int32 add_sd = pPlayer->GetDataFloat(PLAYER_DATA_ABSORB_SD);

				if ( (Random(100) < sGameServer->GetSkillAbsorbHPRate()) && add_hp > 0 )
				{
					pPlayer->PowerIncrease(POWER_LIFE, add_hp, true);
				}

				if ( (Random(100) < sGameServer->GetSkillAbsorbSDRate()) && add_sd > 0 )
				{
					pPlayer->PowerIncrease(POWER_SHIELD, add_sd, true);
				}

				if (pTarget->IsCreature())
				{
					pPlayer->SpawnMajesticDebuffCircle(skill);
				}
			}
		}

		if ( Player* pPlayer = this->ToPlayer() )
		{
			pPlayer->ApplyMajesticLinkedSplashDamage(pTarget, skill, damage);
		}

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

		int32 send_damage = damage - shield_damage;

		if ( send_damage < 0 )
		{
			send_damage = 0;
		}

		sGameServer->LogDamagePVP(this->ToPlayer(), pTarget->ToPlayer(), send_damage, shield_damage, skill);

		this->SendDamage(pTarget, send_damage, shield_damage, interrupt, damage_type);

		pTarget->Unit::AfterHitCheck(this, send_damage, shield_damage, damage_type);

		if ( damage_type != DAMAGE_TYPE_REFLECT )
		{
			this->AdditionalDamage(pTarget, skill, damage, damage_type);
		}

		if ( damage_type != DAMAGE_TYPE_REFLECT )
		{
			this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
		}
	}
	else
	{
		this->SendDamage(pTarget, 0, 0, 0, damage_type);

		if ( damage_type != DAMAGE_TYPE_REFLECT )
		{
			this->AttackElemental(pTarget, mSkill, magic_send, damage, count, combo);
		}
	}
	
	if ( this->PowerGet(POWER_LIFE) <= 0 )
	{
		this->Unit::AfterHitCheck(pTarget, 0, 0, 0);
	}
}

int32 Unit::DealDamage(Unit* pVictim, int32 damage, int32 shield_damage, uint16 skill, bool elemental, MonsterThreatDamageType type, uint8 extra)
{
	if ( !pVictim )
	{
		return 0;
	}

	limitmin(damage, 0);
	limitmin(shield_damage, 0);
	int32 tmp_damage = damage;

	if ( extra != 0 )
	{
		pVictim->PowerReduce(POWER_LIFE, damage, true);
	}
	else
	{
		if ( this->IsPlayer() )
		{
			if ( pVictim->IsPlayer() )
			{
				int32 tmp_shield_damage = this->GetShieldDamage(pVictim, damage, skill, elemental);

				pVictim->PowerReduce(POWER_SHIELD, tmp_shield_damage, true);

				if ( !elemental )
				{
					pVictim->PowerReduce(POWER_LIFE, damage - tmp_shield_damage, true);
				}

				shield_damage += tmp_shield_damage;
			}
			else if ( pVictim->IsCreature() )
			{
				int32 tmp_shield_damage = this->GetShieldDamageMonster(pVictim, damage);

				pVictim->PowerReduce(POWER_SHIELD, tmp_shield_damage, true);

				tmp_damage = damage - tmp_shield_damage;

				if ( pVictim->PowerGet(POWER_LIFE) <= tmp_damage )
					tmp_damage = pVictim->PowerGet(POWER_LIFE);

				pVictim->PowerReduce(POWER_LIFE, damage - tmp_shield_damage, true);

				shield_damage += tmp_shield_damage;
	
				sHuntingRecord->Update(this->ToPlayer(), !elemental ? damage : 0, elemental ? damage : 0, 0, 0, 0);
	
				sLabyrinthDimensions->UpdateMission(this->ToPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_DO_DAMANGE, tmp_damage);
			}
		}
		else
		{
			pVictim->PowerReduce(POWER_LIFE, damage, true);

			sLabyrinthDimensions->UpdateMission(pVictim->ToPlayer(), LABYRINTH_OF_DIMENSIONS_MISSION_TYPE_TAKE_DAMAGE, damage);
		}
	}

	if ( pVictim->IsCreature() )
	{
		Unit* pSummoner = this->GetSummoner();

		if ( pSummoner && pSummoner->IsPlayer() )
		{
			pVictim->ToCreature()->GetThreatManager()->AddThreat(pSummoner->ToPlayer(), tmp_damage);
			pVictim->ToCreature()->GetThreatManager()->AddDamage(pSummoner->ToPlayer(), type, tmp_damage);
		}
		
		MONSTER_AI(pVictim->ToCreature())->OnBeenAttacked(this);
	}

	return shield_damage;
}

int32 Unit::GetShieldDamage(Unit* pVictim, int32 damage, uint16 skill, bool elemental)
{
	if ( damage <= 0 )
	{
		return 0;
	}

	if ( pVictim->PowerGet(POWER_SHIELD) <= 0 )
	{
		return 0;
	}

	int32 nDecreaseSDRate = 0;

	if ( elemental )
	{
		nDecreaseSDRate = 100;
	}
	else if ( (skill == SKILL_DRAGON_SLAYER) && (Random(100) < (10 + (this->GetEnergy() / 100))) )
	{
		nDecreaseSDRate = 10 + (this->GetEnergy() / 30);

		if( nDecreaseSDRate > 100 )
			nDecreaseSDRate = 100;
	}
	else
	{
		nDecreaseSDRate = sGameServer->shield_damage_to_sd_rate.get();
		nDecreaseSDRate -= this->ToPlayer()->GetData(PLAYER_DATA_DECREASE_DAMAGE_TO_SD_RATE);
		nDecreaseSDRate += pVictim->ToPlayer()->GetData(PLAYER_DATA_INCREASE_DAMAGE_TO_SD_RATE);

		if (roll_chance_f(this->ToPlayer()->GetData(PLAYER_DATA_IGNORE_SD_RATE) - pVictim->GetFloatData(UNIT_FLOAT_RESIST_IGNORE_SD)))
		{
			nDecreaseSDRate = 0;
		}

		limitmin(nDecreaseSDRate, 0);
		limitmax(nDecreaseSDRate, 100);
	}
	
	int32 reduce_shield = damage * nDecreaseSDRate / 100;
	int32 reduce_life = damage - reduce_shield;
	bool shield_broke_animation = false;

	if ( pVictim->PowerGet(POWER_SHIELD) <= reduce_shield )
	{
		reduce_life += (reduce_shield - pVictim->PowerGet(POWER_SHIELD));
		reduce_shield = pVictim->PowerGet(POWER_SHIELD);

		shield_broke_animation = (pVictim->PowerGet(POWER_SHIELD) > 0);
	}

	if ( shield_broke_animation && !CC_MAP_RANGE(pVictim->GetWorldId()) )
	{
		pVictim->EffectSend(17);
	}
	
	return reduce_shield;
}

int32 Unit::GetShieldDamageMonster(Unit* mTarget, int32 damage)
{
	if ( damage <= 0 )
		return 0;

	if ( mTarget->PowerGet(POWER_SHIELD) <= 0 )
		return 0;

	int32 divide_damage_to_sd_rate = 100;

	int32 reduce_shield = damage * divide_damage_to_sd_rate / 100;
	int32 reduce_life = damage - reduce_shield;
	bool shield_broke_animation = false;

	if ( mTarget->PowerGet(POWER_SHIELD) <= reduce_shield )
	{
		reduce_life += reduce_shield - mTarget->PowerGet(POWER_SHIELD);
		reduce_shield = mTarget->PowerGet(POWER_SHIELD);

		shield_broke_animation = (mTarget->PowerGet(POWER_SHIELD) > 0.0f);
	}

	if ( shield_broke_animation && !CC_MAP_RANGE(mTarget->GetWorldId()) )
	{
		mTarget->EffectSend(17);
	}
	
	return reduce_shield;
}

void Unit::MagicAttackSend(uint16 target, uint16 skill, bool success)
{
	NORMAL_MAGIC_ATTACK_RESULT pMsg(skill, this->GetEntry(), target, success);
	
	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::NewMagicAttackSend(uint16 target, uint16 skill, bool success)
{
	NEW_MAGIC_ATTACK_RESULT pMsg(skill, this->GetEntry(), target, success);
	
	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::SendDamage(Unit *mTarget, int32 life_damage, int32 shield_damage, uint8 interrupt, uint16 damage_type, uint8 special)
{
	DAMAGE_SEND pMsg(ENTRY(mTarget), damage_type, life_damage, shield_damage);

	if (interrupt)
	{
		pMsg.m_propTarget[0] &= 0x7F;
		pMsg.m_propTarget[0] |= 0x80;
	}

	pMsg.attribute = special;
	pMsg.ViewHP = mTarget ? mTarget->PowerGet(POWER_LIFE) : 0;
	pMsg.ViewSD = mTarget ? mTarget->PowerGet(POWER_SHIELD) : 0;

	if (mTarget != this && mTarget)
		mTarget->sendPacket(MAKE_PCT(pMsg));

	/*if (Player* pPlayer = this->ToPlayer())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Damage: %d	- Shield: %d	- Type: %u", life_damage, shield_damage, damage_type);
	}*/

	PLAYER_POINTER(this->GetSummoner())->sendPacket(MAKE_PCT(pMsg));
}

void Unit::UpdateDelayed()
{
	if ( this->delayed_data.empty() )
		return;

	if ( this->IsCreature() && this->ToCreature()->IsDelayed() )
		return;

	uint32 cur_time = GetTickCount();
	UnitDelayedData* pDelayed;

	for ( DelayedDataList::iterator it = this->delayed_data.begin(); it != this->delayed_data.end(); )
	{
		pDelayed = *it;

		if ( !pDelayed )
		{
			it = this->delayed_data.erase(it);
			continue;
		}

		if ( !pDelayed->GetID() )
		{
			delete *it;
			it = this->delayed_data.erase(it);
			continue;
		}

		if ( cur_time < pDelayed->GetTime() )
		{
			++it;
			continue;
		}

		switch ( pDelayed->GetID() )
		{
		case DELAYED_ATTACK:
			{
				this->AttackProc(pDelayed->GetUnit(), this->MagicGet(pDelayed->damage.skill), false, pDelayed->damage.damage, pDelayed->damage.combo, pDelayed->damage.damage_type, pDelayed->damage.count);
			} break;

		case DELAYED_MAGIC_ATTACK:
			{
				this->AttackProc(pDelayed->GetUnit(), this->MagicGet(pDelayed->damage.skill), true, pDelayed->damage.damage, pDelayed->damage.combo, pDelayed->damage.damage_type, pDelayed->damage.count);
			} break;

		case DELAYED_MONSTER_DIE_PLUS:
			{
				if ( Player* pPlayer = this->ToPlayer() )
					pPlayer->MonsterDiePlus(pDelayed->GetUnit());
			} break;

		case DELAYED_REFLECT:
			{
				if ( !sGameServer->IsReflectFix() )
				{
					this->AttackProc(pDelayed->GetUnit(), this->MagicGet(pDelayed->damage.skill), false, pDelayed->damage.damage, pDelayed->damage.combo, DAMAGE_TYPE_REFLECT, pDelayed->damage.count);
				}
				else
				{
					this->ProcessReflect(pDelayed->GetUnit(), pDelayed->damage.damage);
				}
			} break;

		case DELAYED_PUSH_BACK:
			{
				if ( pDelayed->GetUnit() )
					this->PushBackSimple(pDelayed->GetUnit());
			} break;

		case DELAYED_PUSH_BACK_COUNT:
			{
				if ( pDelayed->GetUnit() )
					this->PushBackCount(pDelayed->GetUnit(), pDelayed->push_back.count);
			} break;

		case DELAYED_DAMAGE:
			{
				
			} break;

		case DELAYED_DARK_SPIRIT:
			{
				if ( Player* pPlayer = this->ToPlayer() )
					pPlayer->DarkSpiritAttack(pDelayed->GetUnit(), pDelayed->dark_spirit.action);
			} break;

		case DELAYED_BUFF_ADD:
			{
				if ( pDelayed->GetUnit() )
					pDelayed->GetUnit()->AddBuff(pDelayed->buff.id, BuffEffect(pDelayed->buff.effect, pDelayed->buff.value), pDelayed->buff.duration, pDelayed->buff.flag, this);
			} break;

		case DELAYED_BUFF_DEL:
			{
				if ( pDelayed->GetUnit() )
					pDelayed->GetUnit()->RemoveBuff(pDelayed->buff.id);
			} break;

		case DELAYED_MONSTER_DIE:
			{
				if ( this->IsCreature() )
					this->Kill();
			} break;

		case DELAYED_MONSTER_HIT:
			{
				if ( Monster* pMonster = this->ToCreature() )
				{
					pMonster->ProcessHit(pDelayed->GetUnit());
				}
			} break;

		case DELAYED_LIFE_RECOVER:
			{
				if ( Unit* pUnit = pDelayed->GetUnit() )
				{
					pUnit->PowerIncrease(POWER_LIFE, pDelayed->life_recover.value, true);
				}
			} break;

		case DELAYED_ATTACK_RANGE:
			{
				if ( Unit* pUnit = pDelayed->GetUnit() )
				{
					if ( !pDelayed->damage_range.range || (Util::Distance(pDelayed->damage_range.x, pDelayed->damage_range.y, pUnit->GetX(), pUnit->GetY()) <= pDelayed->damage_range.range) )
					{
						this->AttackProc(pUnit, nullptr);

						switch ( pDelayed->damage_range.skill_monster )
						{
						case NIXIES_LAKE_BOSS_SKILL_ID_ENERGY_SPHERE:
							{
								if ( roll_chance_i(75) )
								{
									pUnit->AddBuff(BUFF_WEAKNESS, BuffEffect(BUFF_OPTION_WEAKNESS_PERCENT, 10), 30, 0, this);
									pUnit->AddBuff(BUFF_INNOVATION, BuffEffect(BUFF_OPTION_INNOVATION_PERCENT, 10), 30, 0, this);
								}
							} break;

						case NIXIES_LAKE_BOSS_SKILL_ID_ICE_RAIN:
							{
								if ( roll_chance_i(10) )
								{
									if ( !pUnit->HasBuff(BUFF_ICE_ARROW) )
									{
										pUnit->AddBuff(BUFF_ICE_ARROW, 30, 0, this);
									}
								}
							} break;

						case NIXIES_LAKE_BOSS_SKILL_ID_ICE_BREATH:
							{
								pUnit->AddBuff(BUFF_ICE, 60, 0, this);
							} break;
						}
					}
				}
			} break;

		case DELAYED_MUUN_ATTACK:
			{
				if ( Player* pPlayer = this->ToPlayer() )
				{
					sMuunSystem->ProcessMuunAttack(pPlayer, pDelayed->GetUnit());
				}
			} break;

		case DELAYED_FEREA_MINE_CREATE:
			{
				if ( Monster* pMonster = this->ToCreature() )
				{
					FEREA_MINE pMsg;
					pMsg.x = pDelayed->ferea_mine.x;
					pMsg.y = pDelayed->ferea_mine.y;
					pMsg.type = 1;
					sObjectMgr->sendPacketWorld(WORLD_FEREA, (uint8*)&pMsg, pMsg.h.get_size());

					if ( pDelayed->ferea_mine.count >= 4 )
					{
						this->AddDelayed(DELAYED_FEREA_MINE_EXPLODE, 2000, nullptr, pDelayed->ferea_mine.x, pDelayed->ferea_mine.y, pDelayed->ferea_mine.range);
					}
					else
					{
						this->AddDelayed(DELAYED_FEREA_MINE_CREATE, 2000, nullptr, pDelayed->ferea_mine.x, pDelayed->ferea_mine.y, pDelayed->ferea_mine.range, pDelayed->ferea_mine.count + 1);
					}
				}
			} break;

		case DELAYED_FEREA_MINE_EXPLODE:
			{
				if ( Monster* pMonster = this->ToCreature() )
				{
					FEREA_MINE pMsg;
					pMsg.x = pDelayed->ferea_mine.x;
					pMsg.y = pDelayed->ferea_mine.y;
					pMsg.type = 2;
					sObjectMgr->sendPacketWorld(WORLD_FEREA, (uint8*)&pMsg, pMsg.h.get_size());

					PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
					for ( PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr )
					{
						Player* pPlayer = itr->second;

						if ( !Player::IsPlayerBasicState(pPlayer, false) )
						{
							continue;
						}

						if ( !pMonster->SameDimension(pPlayer) )
						{
							continue;
						}

						if ( Util::Distance(pDelayed->ferea_mine.x, pDelayed->ferea_mine.y, pPlayer->GetX(), pPlayer->GetY()) > pDelayed->ferea_mine.range )
						{
							continue;
						}

						int32 deal_damage = pPlayer->PowerGetTotal(POWER_LIFE) * 95 / 100;

						pPlayer->SendDamage(pPlayer, deal_damage, 0, 0, DAMAGE_TYPE_DEBUFF);

						if ( pPlayer->PowerGet(POWER_LIFE) > (pPlayer->PowerGetTotal(POWER_LIFE) - deal_damage) )
						{
							pPlayer->PowerSet(POWER_LIFE, pPlayer->PowerGetTotal(POWER_LIFE) - deal_damage, true);
						}
					}
				}
			} break;

		case DELAYED_SWAMP_OF_DARKNESS_FLAME:
			{
				if (Monster* pMonster = this->ToCreature())
				{
					PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
					for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
					{
						Player* pPlayer = itr->second;

						if (!Player::IsPlayerBasicState(pPlayer, false))
						{
							continue;
						}

						if (!pMonster->SameDimension(pPlayer))
						{
							continue;
						}

						if (!sSwampOfDarkness->IsBossZone(pPlayer))
						{
							continue;
						}

						int32 distance = Util::Distance(pDelayed->swamp_of_darkness_skill.x, pDelayed->swamp_of_darkness_skill.y, pPlayer->GetX(), pPlayer->GetY());

						if (distance > pDelayed->swamp_of_darkness_skill.range)
						{
							continue;
						}

						if (distance > 10)
						{
							distance = 10;
						}

						int32 percent = 95 - (distance * 10);

						if (percent < 5)
						{
							percent = 5;
						}
						
						int32 deal_damage = pPlayer->PowerGetTotal(POWER_LIFE) * percent / 100;

						pPlayer->SendDamage(pPlayer, deal_damage, 0, 0, DAMAGE_TYPE_DEBUFF);

						if (pPlayer->PowerGet(POWER_LIFE) > (pPlayer->PowerGetTotal(POWER_LIFE) - deal_damage))
						{
							pPlayer->PowerSet(POWER_LIFE, pPlayer->PowerGetTotal(POWER_LIFE) - deal_damage, true);
						}
					}
				}
			} break;

		case DELAYED_SWAMP_OF_DARKNESS_DARK:
			{
				if (Monster* pMonster = this->ToCreature())
				{
					PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
					for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
					{
						Player* pPlayer = itr->second;

						if (!Player::IsPlayerBasicState(pPlayer, false))
						{
							continue;
						}

						if (!pMonster->SameDimension(pPlayer))
						{
							continue;
						}

						if (!sSwampOfDarkness->IsBossZone(pPlayer))
						{
							continue;
						}

						if (Util::Distance(pDelayed->swamp_of_darkness_skill.x, pDelayed->swamp_of_darkness_skill.y, pPlayer->GetX(), pPlayer->GetY()) > pDelayed->swamp_of_darkness_skill.range)
						{
							continue;
						}

						pPlayer->Kill();
					}
				}
			} break;

		case DELAYED_SWAMP_OF_DARKNESS_PET:
			{
				if (Monster* pMonster = this->ToCreature())
				{
					if (pDelayed->swamp_of_darkness_pet.count < 1)
					{
						PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
						for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
						{
							Player* pPlayer = itr->second;

							if (!Player::IsPlayerBasicState(pPlayer, false))
							{
								continue;
							}

							if (!pMonster->SameDimension(pPlayer))
							{
								continue;
							}

							if (!sSwampOfDarkness->IsBossZone(pPlayer))
							{
								continue;
							}

							if (Util::Distance(pDelayed->swamp_of_darkness_pet.x, pDelayed->swamp_of_darkness_pet.y, pPlayer->GetX(), pPlayer->GetY()) > pDelayed->swamp_of_darkness_pet.range)
							{
								continue;
							}

							this->AttackProc(pPlayer, nullptr);
						}

						this->AddDelayed(DELAYED_SWAMP_OF_DARKNESS_PET, 1000, nullptr, pDelayed->swamp_of_darkness_pet.x, pDelayed->swamp_of_darkness_pet.y, pDelayed->swamp_of_darkness_pet.range, pDelayed->swamp_of_darkness_pet.count + 1);
					}
				}
			} break;

		case DELAYED_SWAMP_OF_DARKNESS_WATER:
			{
				if (Monster* pMonster = this->ToCreature())
				{
					PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
					for (PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr)
					{
						Player* pPlayer = itr->second;

						if (!Player::IsPlayerBasicState(pPlayer, false))
						{
							continue;
						}

						if (!pMonster->SameDimension(pPlayer))
						{
							continue;
						}

						if (!sSwampOfDarkness->IsBossZone(pPlayer))
						{
							continue;
						}

						if (Util::Distance(pDelayed->swamp_of_darkness_skill.x, pDelayed->swamp_of_darkness_skill.y, pPlayer->GetX(), pPlayer->GetY()) > pDelayed->swamp_of_darkness_skill.range)
						{
							continue;
						}

						pMonster->AttackProc(pPlayer, nullptr);
					}
				}
			} break;

		case DELAYED_MULTI_SKILL:
			{
									if (pDelayed->GetWorld() == this->GetWorldId())
									{
										SkillHandler(this, pDelayed->GetUnit(), this->MagicGet(pDelayed->multi_skill.skill), nullptr, pDelayed->multi_skill.x, pDelayed->multi_skill.y, pDelayed->multi_skill.count).SkillDelayed();
									}
			} break;

		case DELAYED_MULTI_SKILL_ANGLE:
			{
										  if (pDelayed->GetWorld() == this->GetWorldId())
										  {
											  SkillHandler(this, pDelayed->GetUnit(), this->MagicGet(pDelayed->multi_skill_angle.skill), nullptr, pDelayed->multi_skill_angle.x, pDelayed->multi_skill_angle.y, pDelayed->multi_skill_angle.count, pDelayed->multi_skill_angle.angle).SkillDelayed();
										  }
			} break;
		}

		delete *it;
		it = this->delayed_data.erase(it);
	}
}

void Unit::AddDelayed(DelayedID id, uint32 delay, Unit * mUnit, int32 data_1, int32 data_2, int32 data_3, int32 data_4, int32 data_5)
{
	UnitDelayedData* pDelayed = new UnitDelayedData;
	pDelayed->SetID(id);
	pDelayed->SetTime(GetTickCount() + delay);
	pDelayed->SetUnit(mUnit);
	pDelayed->SetWorld(this->GetWorldId());
	pDelayed->raw.data[0] = data_1;
	pDelayed->raw.data[1] = data_2;
	pDelayed->raw.data[2] = data_3;
	pDelayed->raw.data[3] = data_4;
	pDelayed->raw.data[4] = data_5;
	this->delayed_data.push_back(pDelayed);
}

void Unit::KillSummoned(bool reset)
{
	if ( !this->GetSummoned() )
		return;
	
	if ( !this->GetSummoned()->IsCreature() )
		return;

	if ( this->GetSummoned()->GetSummoner() != this )
		return;

	if ( reset )
		this->GetSummoned()->SetConnectStatus(CONNECT_STATUS_NONE);

	this->SetSummoned(nullptr);
}

void Unit::SetSummonedTarget(Unit* mTarget)
{
	if ( !this->GetSummoned() )
		return;

	if ( !this->GetSummoned()->IsCreature() )
		return;

	if ( this->GetSummoned()->GetSummoner() != this )
		return;

	Monster* mMonster = this->GetSummoned()->ToCreature();

	if ( mMonster->GetTarget() )
		return;

	mMonster->SetTarget(mTarget);
}

uint8 Unit::GetPathPacketDirPos(coord_type px, coord_type py)
{
	uint8 pos = 0;

	if (px <= -1 && py <= -1)
	{
		pos = 0;
	}
	else if (px <= -1 && py == 0)
	{
		pos = 7;
	}
	else if ( px <= -1 && py >= 1)
	{
		pos = 6;
	}
	else if ( px == 0 && py <= -1)
	{
		pos = 1;
	}
	else if ( px == 0 && py >= 1)
	{
		pos = 5;
	}
	else if ( px >= 1 && py <=-1)
	{
		pos = 2;
	}
	else if ( px >= 1 && py == 0)
	{
		pos = 3;
	}
	else if ( px >= 1  && py >= 1)
	{
		pos = 4;
	}

	return pos;
}

bool Unit::IsInDragonTower() const
{
	if ( this->GetWorldId() != WORLD_CASTLE_SIEGE )
		return false;

	return ( (this->GetX() >= 159 && this->GetX() <= 190) && (this->GetY() >= 189 && this->GetY() <= 217) );
}

bool Unit::IsInDragonTower(coord_type x1, coord_type y1, coord_type x2, coord_type y2)
{
	return ( (x1 >= 160 && x2 <= 189) && (y1 >= 190 && y2 <= 217) );
}

void Unit::FireworksSend(uint8 count)
{
	FIREWORK_SEND pMsg(0, this->GetX(), this->GetY());

	for ( uint8 i = 0; i < count; i++ )
	{
		this->sendPacket(MAKE_PCT(pMsg));
		this->sendPacket_viewport(MAKE_PCT(pMsg));
	}
}

void Unit::ApplyDamageDecrease(int32 & damage)
{
	if ( this->GetIntData(UNIT_INT_DAMAGE_DECREASE) )
	{
		damage -= (damage * this->GetIntData(UNIT_INT_DAMAGE_DECREASE)) / 100;
	}

	damage -= this->GetIntData(UNIT_INT_DAMAGE_DECREASE_ADD);

	if ( this->IsUsingShield() )
	{
		damage -= (damage * 50) / 100;
	}
}

void Unit::ReflectDamage(int32 damage, Unit* pVictim)
{
	if ( !pVictim )
	{
		return;
	}
	
	if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_TOTAL_REFLECT_DAMAGE)))
	{
		this->AddDelayed(DELAYED_REFLECT, 10, pVictim, -1, damage);
	}
	else
	{
		int32 damage_reflect = int32(float(damage) * this->GetFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE) / 100.0f);

		if ( damage_reflect > 0 )
		{
			this->AddDelayed(DELAYED_REFLECT, 10, pVictim, -1, damage_reflect);
		}
	}
}

void Unit::TotalPowerRecovery()
{
	if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_TOTAL_LIFE_RECOVERY)))
	{
		this->PowerSet(POWER_LIFE, this->PowerGetTotal(POWER_LIFE), true);
	}

	if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_TOTAL_MANA_RECOVERY)))
	{
		this->PowerSet(POWER_MANA, this->PowerGetTotal(POWER_MANA), true);
	}

	if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_TOTAL_SHIELD_RECOVERY)))
	{
		this->PowerSet(POWER_SHIELD, this->PowerGetTotal(POWER_SHIELD), true);
	}

	if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_TOTAL_STAMINA_RECOVERY)))
	{
		this->PowerSet(POWER_STAMINA, this->PowerGetTotal(POWER_STAMINA), true);
	}
}

void Unit::AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag)
{
	SafeAssert(pAttacker, "pAttacker == nullptr");

	if ( !this->IsPlaying() )
	{
		return;
	}
	
	Unit * pAttackerMaster = pAttacker->GetSummoner();

	if ( this->PowerGet(POWER_LIFE) <= 0 && this->IsLive() )
	{
		this->SetLastAttacker(pAttacker);

		if ( this->IsPlayer() && this->ToPlayer()->IsInChallenge() )
		{
			this->PowerSet(POWER_LIFE, this->PowerGetTotal(POWER_LIFE), true);
			this->PowerSet(POWER_SHIELD, this->PowerGetTotal(POWER_SHIELD), true);
			return;
		}

		this->Die();

		this->SetKiller(pAttacker);
		this->ClearPathData();
		this->DieSend();

		this->AfterHitCheck(pAttacker, damage, shield_damage, damage_flag);

		if ( pAttackerMaster && pAttackerMaster->IsCreature() )
		{
			MONSTER_AI(pAttackerMaster->ToCreature())->OnKillUnit(this);
		}
	}
}

void Unit::Kill()
{
	if ( !this->IsLive() )
	{
		return;
	}

	this->Die();

	this->SetKiller(this);

	POWER_LOOP(i)
	{
		this->PowerSet(i, 0, true);
	}

	this->ClearPathData();
	this->DieSend();
}

void Unit::Hit(Unit* pAttacker, int32 damage, int32 shield_damage, uint16 damage_type, bool damage_send)
{
	SafeAssert(pAttacker, "pAttacker == nullptr");

	this->PowerReduce(POWER_LIFE, damage, true);

	if ( damage_send )
	{
		this->SendDamage(pAttacker, damage, shield_damage, 0, damage_type);
	}

	this->Unit::AfterHitCheck(pAttacker, damage, shield_damage, 0);
}

void Unit::KillCountSend()
{
	KILL_COUNT pMsg(this->GetKillCount());
	this->sendPacket(MAKE_PCT(pMsg));
}

void Unit::KillCountAdd(uint8 count)
{
	if ( (this->GetKillCount() + count) < 255 )
	{
		this->IncreaseKillCount(count);
	}
	else
	{
		this->SetKillCount(255);
	}

	this->KillCountSend();
}

void Unit::KillCountDecrease(uint8 count)
{
	if ( this->GetKillCount() > count )
	{
		this->ReduceKillCount(count);
	}
	else
	{
		this->SetKillCount(0);
	}

	this->KillCountSend();
}

void Unit::KillCountReset()
{
	this->SetKillCount(0);
	this->KillCountSend();
}

void Unit::GenerateRandomLocation(World* pWorld, coord_type &x, coord_type &y, int32 length, uint8 exclude, int32 instance)
{
	if ( !pWorld )
		return;

	int32 count = 10;
	limitmin(length, 1);
	int32 max_length = length * 2;
	bool stand = exclude & WORLD_ATTRIBUTE_STAND;
	exclude &= ~WORLD_ATTRIBUTE_STAND;
	
	while( count-- > 0 )
	{
		coord_type px = (x - length) + Random(max_length);
		coord_type py = (y - length) + Random(max_length);

		if ( !(pWorld->GetGrid(px, py).attribute & exclude) )
		{
			x = px;
			y = py;
			return;
		}
	}
}

int32 Unit::PowerGetTotal(uint8 id) const
{
	return this->PowerGetMax(id) + this->PowerGetAdd(id);
}

int32 Unit::PowerGet(uint8 id) const
{
	return this->power[id].get();
}
		
int32 Unit::PowerGetMax(uint8 id) const
{
	return this->power_max[id].get();
}

int32 Unit::PowerGetAdd(uint8 id) const
{
	return this->power_add[id].get();
}

void Unit::PowerSet(uint8 id, int32 value, bool send)
{
	this->power[id].set(value);

	if ( send )
	{
		this->UpdatePowers(id);
	}
}

void Unit::PowerSetMax(uint8 id, int32 value)
{
	this->power_max[id].set(value);
}
	
void Unit::PowerSetAdd(uint8 id, int32 value)
{
	this->power_add[id].set(value);
}
		
uint8 Unit::PowerGetPercent(uint8 id) const
{
	if (this->PowerGet(id) >= this->PowerGetTotal(id))
	{
		return 100;
	}

	if (this->PowerGet(id) <= 0 || this->PowerGetTotal(id) <= 0)
	{
		return 0;
	}

	int64 power = this->PowerGet(id);
	int64 power_total = this->PowerGetTotal(id);

	int64 percent = power * 100 / power_total;

	if (percent > 100)
	{
		percent = 100;
	}

	if (percent < 0)
	{
		percent = 0;
	}

	return percent;
}
		
void Unit::PowerReduce(uint8 id, int32 value, bool send)
{
	if ( value <= 0 )
		return;

	this->power[id].op_dec(value);
	this->power[id].limit_min(0);

	if ( send )
	{
		this->UpdatePowers(id);
	}
}

void Unit::PowerIncrease(uint8 id, int32 value, bool send)
{
	if ( value <= 0 )
		return;

	this->power[id].op_add(value);
	this->power[id].limit_max(this->PowerGetTotal(id));

	if ( send )
	{
		this->UpdatePowers(id);
	}
}

void Unit::PowerAlter(uint8 id, int32 value, bool increase)
{
	if ( value <= 0 )
		return;

	if ( increase )
	{
		this->PowerIncrease(id, value);
	}
	else
	{
		this->PowerReduce(id, value);
	}
}

bool Unit::PowerHas(uint8 id, int32 value)
{
	return this->PowerGet(id) >= value;
}

void Unit::PowerReduceAdd(uint8 id, int32 value, bool compare)
{
	if ( value <= 0 )
		return;

	this->power_add[id].op_dec(value);

	if ( compare && (this->PowerGet(id) > this->PowerGetTotal(id)) )
	{
		this->PowerSet(id, this->PowerGetTotal(id), true);
	}
}

void Unit::PowerIncreseAdd(uint8 id, int32 value, bool compare)
{
	if ( value <= 0 )
		return;

	this->power_add[id].op_add(value);

	if ( compare && (this->PowerGet(id) > this->PowerGetTotal(id)) )
	{
		this->PowerSet(id, this->PowerGetTotal(id), true);
	}
}

int32 FrustrumX[MAX_ARRAY_FRUSTRUM];
int32 FrustrumY[MAX_ARRAY_FRUSTRUM];

void InitFrustrum()
{
	float CameraViewFar    = 2400.0f;	// À§ÂÊ ³¡
	float CameraViewNear   = CameraViewFar * 0.19f;// ¾Æ·¡ ³¡
	float CameraViewTarget = CameraViewFar * 0.53f;// Ä³¸¯ÅÍÀÇ Áß¾Ó À§Ä¡
	float WidthFar  = 1190.0f;// À§ÂÊÀÇ °¡·Î Æø
	float WidthNear = 550.0f;// ¾Æ·¡ÂÊÀÇ °¡·Î Æø
		
	vec3_t p[4];

	Vector(-WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[0]);
	Vector( WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[1]);
	Vector( WidthNear,CameraViewNear-CameraViewTarget,0.f,p[2]);
	Vector(-WidthNear,CameraViewNear-CameraViewTarget,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f, 0.f, 45.f, Angle);
	AngleMatrix(Angle, Matrix);
    vec3_t Frustrum[4];

	for ( int32 i = 0; i < MAX_ARRAY_FRUSTRUM; ++i )
	{
		VectorRotate(p[i], Matrix, Frustrum[i]);
		FrustrumX[i] = (int)(Frustrum[i][0]*0.01f);
		FrustrumY[i] = (int)(Frustrum[i][1]*0.01f);
	}
}

void Unit::ClearPathData(bool update)
{
	this->GetPathData()->SetCount(0);
	this->GetPathData()->SetCurrent(0);
	this->GetPathData()->SetStartEnd(false);
	this->GetMovePathTime()->Start();

	for ( int32 i = 0; i < MAX_WALK_PATH; ++i )
	{
		this->GetPathData()->GetPosition(i)->Reset();
	}

	this->SetTX(this->GetX());
	this->SetTY(this->GetY());

	if ( update )
	{
		this->PositionSend(this->GetX(), this->GetY());
	}
}

void Unit::Die()
{
	this->SetLive(false);
	this->SetDeadStatus(DEAD_STANDBY);
	this->SetState(OBJECT_STATE_DEAD);
	this->SetRespawnTick(GetTickCount());
	this->PowerSet(POWER_LIFE, 0, true);

	Player* pPlayer = this->ToPlayer();
	Monster* pMonster = this->ToCreature();

	if ( this->GetTeleportStatus() == TELEPORT_STANDBY )
	{
		this->SetTeleportStatus(TELEPORT_NONE);
	}

	this->CancelDelayedTeleport();

	if ( pPlayer )
	{
		pPlayer->SetAccumulatedTime(0);

		pPlayer->CancelAllActivities();
	}
	else if ( pMonster )
	{
		pMonster->CalculateMaxAttacker();

		MONSTER_AI(pMonster)->OnDie();
	}
}

bool Unit::AttackElemental(Unit* pTarget, Skill* pSkill, bool magic_send, int32 damage, int32 count, bool combo)
{
	if ( this->GetElementalAttribute() == ELEMENTAL_ATTRIBUTE_NONE )
	{
		return false;
	}

	/*if ( AW_MAP_RANGE(this->GetWorldId()) )
	{
		if ( sArkaWar->GetState() == ARKA_WAR_STATE_STANDBY ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_RESULT ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_CLOSE ||
			 sArkaWar->GetState() == ARKA_WAR_STATE_END )
		{
			return false;
		}
	}*/

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if (pPlayer->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelPentagramDisabled())
		{
			return false;
		}

		if ( !pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsValid() || !pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsPentagramItem() )
		{
			return false;
		}
	}

	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(pSkill ? pSkill->GetSkill(): 0);
	uint8 interrupt = 0;
	bool miss = false;

	uint16 effect = this->GetElementalAttribute();

	if( !this->MissCheckElemental(pTarget, skill_base, miss, count) )
	{
		return true;
	}

	int32 defense = this->GetTargetElementalDefense(pTarget, effect);

	damage = this->GetAttackDamageElemental(pTarget, pSkill, effect, damage, defense);
	int32 punish_damage = 0;

	if( miss )
	{
		damage = (damage * 30) / 100;
	}

	Player* pPlayer = this->ToPlayer();
	Player* pVictim = pTarget->ToPlayer();

	if (!sGameServer->IsGLElementalDamageFix())
	{
		if (pPlayer)
		{
			if (pPlayer->GetClass() == Character::GROW_LANCER)
			{
				PLAYER_POINTER(this)->ApplyWingDamage(damage, skill_base);

				damage = (damage * (this->GetAgility() / 10 + 97 + this->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE))) / 100;
			}
		}
	}

	if (pVictim)
	{
		damage -= (damage * pVictim->GetPentagramJewelOption()->GetAddElementalDamageReduction()) / 100;
	}

	if ( pPlayer && pVictim )
	{
		damage -= (damage * pVictim->GetPentagramJewelOption()->GetAddElementalDamageReductionPvP()) / 100;
	}

	if ( !pPlayer && pVictim )
	{
		damage -= (damage * pVictim->GetPentagramJewelOption()->GetAddElementalDamageReductionPvM()) / 100;
	}

	int32 MinDamage = (this->GetTotalLevel()) / 10;

	MinDamage = ((MinDamage < 1) ? 1 : MinDamage);

	damage = ((damage < MinDamage) ? MinDamage : damage);

	if( pPlayer && pVictim )
	{
		pPlayer->PVPDamageReduction(damage, pVictim);
	}

	if (pTarget->HasBuff(BUFF_BASTION) && (pTarget->PowerGetPercent(POWER_SHIELD) < sGameServer->GetSkillBastionSD()))
	{
		damage -= damage * sGameServer->GetSkillBastionReduction() / 100;
	}

	if (pPlayer)
	{
		pPlayer->ApplyRadianceDebuff(pTarget, effect, punish_damage, count);
	}

	this->ApplySkillCountEffect(pTarget, skill_base, count, effect);

	if (pPlayer && pVictim && count <= 1)
	{
		if (pVictim->GetPentagramJewelOption()->GetElementalBastionDuration() > 0 && !pVictim->HasBuff(BUFF_BASTION) && (pVictim->PowerGetPercent(POWER_SHIELD) <= sGameServer->GetSkillBastionSD()) && roll_chance_i(sGameServer->GetSkillBastionRate()))
			pVictim->AddBuff(BUFF_BASTION, pVictim->GetPentagramJewelOption()->GetElementalBastionDuration(), 0, this);
	}

	if ( pPlayer )
	{
		pPlayer->ApplyMajesticDamage(pTarget->ToCreature(), damage, false);
	}

	if ( pVictim )
	{
		pVictim->ApplyMajesticDamage(this->ToCreature(), damage, true);
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetClass() == 709 ||
			 pMonster->GetClass() == 710 ||
			 pMonster->GetClass() == 711 ||
			 pMonster->GetClass() == 712 )
		{
			damage = 1;
		}
	}

	int32 shield_damge = this->DealDamage(pTarget, damage, 0, pSkill ? pSkill->GetSkill(): 0, true, MONSTER_THREAT_DAMAGE_ELEMENTAL);

	if (pVictim)
	{
		if (pVictim->GetPentagramJewelOption()->GetAddElementalShieldAbsorbRate() > 0 && roll_chance_i(30))
		{
			pVictim->PowerIncrease(POWER_SHIELD, damage * pVictim->GetPentagramJewelOption()->GetAddElementalShieldAbsorbRate() / 100, true);
		}

		if (pVictim->GetPentagramJewelOption()->GetAddElementalLifeAbsorbRate() > 0 && roll_chance_i(30))
		{
			pVictim->PowerIncrease(POWER_LIFE, damage * pVictim->GetPentagramJewelOption()->GetAddElementalLifeAbsorbRate() / 100, true);
		}
	}

	if (pTarget->IsCreature())
	{
		sHuntingRecord->Update(pPlayer, 0, punish_damage, 0, 0, 0);
	}

	if ( damage < 0 )
	{
		damage = 0;
	}

	if( damage > 0 || punish_damage > 0 )
	{
		this->SendDamageElemental(pTarget, effect, damage);

		pTarget->Unit::AfterHitCheck(this, damage, shield_damge, effect);
	}
	else
	{
		this->SendDamageElemental(pTarget, effect, 0);
	}

	if ( punish_damage > 0 )
	{
		this->SendDamageElemental(pTarget, ELEMENTAL_DAMAGE_TYPE_PUNISH, punish_damage);
	}

	return true;
}

bool Unit::MissCheckElemental(Unit* pTarget, uint16 skill_base, bool& miss, int32 count)
{
	///- Elemental Attack Success Rate
	int32 ElementalAttackSuccessRate = this->GetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE);

	ElementalAttackSuccessRate += (ElementalAttackSuccessRate * this->GetFloatData(UNIT_FLOAT_ELEMENTAL_ATTACK_SUCCESS_RATE_PERCENT)) / 100;

	if (Player* pPlayer = this->ToPlayer())
	{
		if (pTarget->IsPlayer())
		{
			ElementalAttackSuccessRate += pPlayer->GetData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVP);
		}
		else
		{
			ElementalAttackSuccessRate += pPlayer->GetData(PLAYER_DATA_ELEMENTAL_ATTACK_SUCCESS_RATE_PVM);
		}
	}

	ElementalAttackSuccessRate -= (ElementalAttackSuccessRate * this->GetBuffEffect(BUFF_BLINDNESS)) / 100;

	if (this->HasBuff(BUFF_ERRTEL_BERSERKER_I))
	{
		ElementalAttackSuccessRate += ElementalAttackSuccessRate * sGameServer->GetSkillElementalBerserkerIDamage() / 100;
	}

	ElementalAttackSuccessRate = ((ElementalAttackSuccessRate < 0) ? 0 : ElementalAttackSuccessRate);

	///- Elemental Defense Success Rate
	int32 ElementalDefenseSuccessRate = pTarget->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE);

	ElementalDefenseSuccessRate += (ElementalDefenseSuccessRate * pTarget->GetFloatData(UNIT_FLOAT_ELEMENTAL_DEFENSE_SUCCESS_RATE_PERCENT)) / 100;

	if (Player* pPlayer = pTarget->ToPlayer())
	{
		if (this->IsPlayer())
		{
			ElementalDefenseSuccessRate += pPlayer->GetData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVP);
		}
		else
		{
			ElementalDefenseSuccessRate += pPlayer->GetData(PLAYER_DATA_ELEMENTAL_DEFENSE_SUCCESS_RATE_PVM);
		}
	}

	ElementalDefenseSuccessRate = ((ElementalDefenseSuccessRate < 0) ? 0 : ElementalDefenseSuccessRate);

	///- Check
	uint16 effect = this->GetElementalAttribute();

	this->ApplySkillCountEffect(pTarget, skill_base, count, effect);

	if( ElementalAttackSuccessRate < ElementalDefenseSuccessRate )
	{
		miss = true;

		if( Random(100) >= 5 )
		{
			this->SendDamageElemental(pTarget, effect, 0);
			return false;
		}
	}
	else
	{
		miss = false;

		if( Random((ElementalAttackSuccessRate <= 0) ? 1 : ElementalAttackSuccessRate) < ElementalDefenseSuccessRate )
		{
			this->SendDamageElemental(pTarget, effect, 0);
			return false;
		}
	}

	if ( pTarget->HasBuff(BUFF_ERRTEL_IMMUNE_I) )
	{
		this->SendDamageElemental(pTarget, effect, 0);
		return false;
	}

	if( Player* pVictim = pTarget->ToPlayer() )
	{
		if (roll_chance_i(pVictim->GetPentagramOption()->GetAddElementalAvoidanceRate()))
		{
			this->SendDamageElemental(pTarget, 0, 0);
			return false;
		}
	}

	return true;
}

int32 Unit::GetTargetElementalDefense(Unit* pTarget, uint16 & effect)
{
	int32 defense = pTarget->GetIntData(UNIT_INT_ELEMENTAL_DEFENSE);

	if (Player* pPlayer = pTarget->ToPlayer())
	{
		defense += (((!pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT)->IsValid()) ? 0 : pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT)->GetDefense()) * pPlayer->GetPentagramOption()->GetMulPentagramDefense()) / 100;

		defense += (pTarget->GetIntData(UNIT_INT_DEFENSE) * pPlayer->GetPentagramOption()->GetAddElementalDefenseTransferRate()) / 100;

		defense += pPlayer->GetPentagramJewelOption()->GetAddElementalDefense();

		if (Player* pAttacker = this->ToPlayer())
		{
			defense += pPlayer->GetPentagramJewelOption()->GetAddElementalDefensePvP();
		}
		else
		{
			defense += pPlayer->GetPentagramJewelOption()->GetAddElementalDefensePvM();
		}
	}

	if (pTarget->HasBuff(BUFF_ERRTEL_BERSERKER_I))
	{
		defense -= defense * sGameServer->GetSkillElementalBerserkerIDefense() / 100;
	}

	float innovation_effect_percent = pTarget->GetEffect(BUFF_OPTION_INNOVATION_ELEMENTAL_PERCENT);
	if (innovation_effect_percent > 0)
	{
		defense -= defense * innovation_effect_percent / 100;
	}

	defense -= pTarget->GetEffect(BUFF_OPTION_INNOVATION_ELEMENTAL);

	sPentagramSystem->GetPentagramRelationshipDefense(this, pTarget, defense);

	defense = ((defense < 0) ? 0 : defense);

	return defense;
}
	
int32 Unit::GetAttackDamageElemental(Unit* pTarget, Skill* pSkill, uint16 & effect, int32 attack_damage, int32 target_defense)
{
	int32 damage_min = this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN);
	int32 damage_max = this->GetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX);

	Player* pPlayer = this->ToPlayer();
	Player* pVictim = pTarget->ToPlayer();

	if( pPlayer )
	{
		Item const* pPentagram = pPlayer->GetInventory()->GetItem(PENTAGRAM_SLOT);

		damage_min += (((!pPentagram->IsValid()) ? 0 : pPentagram->GetMinDamage()) * pPlayer->GetPentagramOption()->GetMulPentagramDamage()) / 100;
		damage_max += (((!pPentagram->IsValid()) ? 0 : pPentagram->GetMaxDamage()) * pPlayer->GetPentagramOption()->GetMulPentagramDamage()) / 100;

		damage_min += (attack_damage * pPlayer->GetPentagramOption()->GetAddElementalAttackTransferRate()) / 100;
		damage_max += (attack_damage * pPlayer->GetPentagramOption()->GetAddElementalAttackTransferRate()) / 100;

		damage_min += pPlayer->GetPentagramJewelOption()->GetAddElementalDamage();
		damage_max += pPlayer->GetPentagramJewelOption()->GetAddElementalDamage();

		if ( pPentagram->IsValid() && pSkill )
		{
			uint8 skill_elemental_attribute = sSkillMgr->GetSkillElementalAttribute(pSkill->GetSkill());
			uint16 skill_elemental_bonus = sSkillMgr->GetSkillElementalBonus(pSkill->GetSkill());

			if ( skill_elemental_attribute == this->GetElementalAttribute() )
			{
				damage_min += (pPlayer->GetData(PLAYER_DATA_ELEMENTAL_IMPRINT) * skill_elemental_bonus) / 100;
				damage_max += (pPlayer->GetData(PLAYER_DATA_ELEMENTAL_IMPRINT) * skill_elemental_bonus) / 100;
			}
		}

		if( pVictim )
		{
			damage_min += pPlayer->GetPentagramJewelOption()->GetAddElementalDamagePvP();
			damage_max += pPlayer->GetPentagramJewelOption()->GetAddElementalDamagePvP();

			damage_min += (damage_min * pPlayer->GetPentagramJewelOption()->GetMulElementalDamagePvP()) / 100;
			damage_max += (damage_max * pPlayer->GetPentagramJewelOption()->GetMulElementalDamagePvP()) / 100;
		}
		else
		{
			damage_min += pPlayer->GetPentagramJewelOption()->GetAddElementalDamagePvM();
			damage_max += pPlayer->GetPentagramJewelOption()->GetAddElementalDamagePvM();

			damage_min += (damage_min * pPlayer->GetPentagramJewelOption()->GetMulElementalDamagePvM()) / 100;
			damage_max += (damage_max * pPlayer->GetPentagramJewelOption()->GetMulElementalDamagePvM()) / 100;
		}
	}

	int32 range = (damage_max - damage_min);

	range = ((range < 1) ? 1 : range);

	int32 damage = damage_min + Random(range);

	if (this->HasBuff(BUFF_ERRTEL_BERSERKER_I))
	{
		damage += damage * sGameServer->GetSkillElementalBerserkerIDamage() / 100;
	}

	float weakness_effect_percent = this->GetEffect(BUFF_OPTION_WEAKNESS_ELEMENTAL_PERCENT);
	if (weakness_effect_percent > 0)
	{
		damage -= damage * weakness_effect_percent / 100;
	}
	
	damage -= this->GetEffect(BUFF_OPTION_WEAKNESS_ELEMENTAL);

	if ( this->CriticalDamageChance(pTarget, true, damage, damage_max) )
	{
		effect = ELEMENTAL_DAMAGE_TYPE_CRITICAL;
	}

	sPentagramSystem->GetPentagramRelationshipDamage(this, pTarget, damage);

	damage -= target_defense;

	damage = ((damage < 0) ? 0 : damage);

	return damage;
}

void Unit::SendDamageElemental(Unit *pTarget, uint8 elemental_attribute, int32 damage)
{
	ELEMENTAL_DAMAGE_SEND pMsg(ENTRY(pTarget), elemental_attribute, damage);

	pMsg.ViewHP = pTarget ? pTarget->PowerGet(POWER_LIFE) : 0;
	pMsg.ViewSD = pTarget ? pTarget->PowerGet(POWER_SHIELD) : 0;

	PLAYER_POINTER(this)->SEND_PCT(pMsg);
	PLAYER_POINTER(pTarget)->SEND_PCT(pMsg);
}

bool Unit::CommonMovementCheck()
{
	if ( !this->Object::IsPlaying() )
	{
		return false;
	}

	if ( !this->IsLive() )
	{
		return false;
	}

	if ( this->GetRegenStatus() == REGEN_STANDBY /*||
		 this->GetRegenStatus() == REGEN_READY */)
	{
		return false;
	}

	/*if ( this->IsDelayedTeleport() )
		this->CancelDelayedTeleport();*/

	if ( this->IsTeleporting() )
		return false;

	if (this->HasRestrictionBuff())
	{
		if (sGameServer->IsDebuffStopCheck())
		{
			if (Player* pPlayer = this->ToPlayer())
			{
				pPlayer->CancelMovement();
			}
		}

		return false;
	}

	return this->GetMoveLastTime()->Elapsed(100);
}

bool Unit::SendViewportData() const
{
	if ( this->HasBuff(BUFF_INVISIBILITY) )
		return false;

	if ( Player const* pPlayer = this->ToPlayer() )
	{
		if ( pPlayer->IsSpectator() )
		{
			return false;
		}
	}

	return true;
}

void Unit::UpdateOldPower()
{
	if ( !sGameServer->IsFixPowersOnBuff() )
	{
		return;
	}

	POWER_LOOP(i)
	{
		this->SetOldPower(i, this->PowerGet(i));
	}
}
	
void Unit::ApplyOldPower()
{
	if ( !sGameServer->IsFixPowersOnBuff() )
	{
		return;
	}

	POWER_LOOP(i)
	{
		if ( this->GetOldPower(i) == this->PowerGet(i) )
		{
			continue;
		}

		if ( this->GetOldPower(i) < this->PowerGet(i) )
		{
			this->SetOldPower(i, this->PowerGet(i));
		}

		if ( this->GetOldPower(i) > this->PowerGetTotal(i) )
		{
			this->SetOldPower(i, this->PowerGetTotal(i));
		}

		this->PowerSet(i, this->GetOldPower(i), true);
		this->UpdatePowers(i, true);
	}
}

void Unit::ClearDebuff(int32 count)
{
	if ( count <= 0 )
	{
		return;
	}

	BuffData * pBuffData = nullptr;
	buff_template const* buff_info = nullptr;

	BUFF_LOOP(i)
	{
		pBuffData = this->GetBuffData(i);

		if ( !pBuffData )
		{
			continue;
		}

		if ( !pBuffData->GetID() )
		{
			continue;
		}

		buff_info = sSkillMgr->GetBuff(pBuffData->GetID());

		if ( !buff_info )
		{
			continue;
		}

		if ( !buff_info->IsDebuff() )
		{
			continue;
		}

		this->RemoveBuff(buff_info);
		count--;

		if ( count <= 0 )
		{
			return;
		}
	}
}

BuffData const* Unit::GetBuff(uint16 id) const
{
	BUFF_LOOP(i)
	{
		BuffData const* pBuffData = this->GetBuffData(i);

		if ( !pBuffData )
		{
			continue;
		}

		if ( !pBuffData->GetID() )
		{
			continue;
		}

		if ( pBuffData->GetID() != id )
		{
			continue;
		}

		return pBuffData;
	}

	return nullptr;
}

void Unit::MissSend(Unit* pTarget, Skill* pSkill, bool send, int32 count, uint16 effect)
{
	uint16 base_skill = pSkill ? sSkillMgr->GetSkillBaseSkill(pSkill->GetSkill()) : 0;

	this->ApplySkillCountEffect(pTarget, base_skill, count, effect);
	
	this->SendDamage(pTarget, 0, 0, 0, effect);

	if( send && pSkill )
	{
		this->MagicAttackSend(ENTRY(pTarget), pSkill->GetSkill(), false);
	}
}

void Unit::ApplySkillCountEffect(Unit* pTarget, uint16 skill, int32 count, uint16 & effect)
{
	if (count <= 0)
	{
		return;
	}

	switch (skill)
	{
	case SKILL_UPPER_BEAST:
	case SKILL_DARK_SIDE:
	case SKILL_HARSH_STRIKE:
	case SKILL_SHINING_PEAK:
	case SKILL_MAGIC_PIN:
	case SKILL_FIRE_BLOW:
	case SKILL_SWORD_BLOW:
	case SKILL_METEOR_STRIKE:
	case SKILL_METEOR_STRIKE_2:
	case SKILL_METEOR_STORM:
	case SKILL_SOUL_SEEKER:
	case SKILL_FOCUS_SHOT:
	case SKILL_FIRE_BEAST:
	case SKILL_AQUA_BEAST:
	case SKILL_ICE_BLOOD:
	case SKILL_FIRE_BLOOD:
	case SKILL_DARK_BLAST:
	case SKILL_EVIL_WIND:
	case SKILL_DARK_PHOENIX_SHOT:
	case SKILL_SPIRIT_HOOK:
	case SKILL_MAGIC_ARROW:
	case SKILL_BAT_FLOCK:
	case SKILL_ICE_BLAST:
	case SKILL_BURSTING_FLARE:
		{
			effect |= (((count % 2) == 0) ? 0x20 : 0x10);
		} break;

	case SKILL_PIERCE_ATTACK:
	{
								if (pTarget && pTarget->HasBuff(BUFF_BAT_FLOCK))
								{
									effect |= (((count % 4) == 0) ? 0x20 : 0x10);
								}
								else
								{
									effect |= (((count % 2) == 0) ? 0x20 : 0x10);
								}
	} break;

	case SKILL_LARGE_RING_BLOWER:
	case SKILL_CHAIN_DRIVE:
	case SKILL_DRAGON_LORE:
	case SKILL_PHOENIX_SHOT:
	case SKILL_LIGHTNING_STORM:
	case SKILL_DEATHSIDE:
		{
			effect |= (((count % 4) == 0) ? 0x20 : 0x10);
		} break;

	default:
		{
			effect |= (((count % 2) == 0) ? 0x20 : 0x10);
		} break;
	}
}

bool Unit::CriticalDamageChance(Unit* pTarget, bool elemental, int32 & damage, int32 damage_max)
{
	float rate = 0.0f;

	if ( elemental )
	{
		if ( Player* pPlayer = this->ToPlayer() )
		{
			rate = pPlayer->GetPentagramOption()->GetAddElementalCriticalDamageRate();
		}
	}
	else
	{
		rate = this->GetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE);

		if ( Player* pPlayer = this->ToPlayer() )
		{
			if ( pTarget->IsPlayer() )
			{
				rate += pPlayer->GetDataFloat(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_RATE_PVP);
			}
			else
			{
				rate += pPlayer->GetDataFloat(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_RATE_PVM);
			}
		}

		rate -= pTarget->GetFloatData(UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE);
	}

	if ( rate < 0.0f )
	{
		rate = 0.0f;
	}

	if (roll_chance_f(rate))
	{
		damage = damage_max;
		damage += this->GetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD);

		if ( Player* pPlayer = this->ToPlayer() )
		{
			if ( pTarget->IsPlayer() )
			{
				damage += pPlayer->GetData(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_PVP);
			}
			else
			{
				damage += pPlayer->GetData(PLAYER_DATA_INCREASE_CRITICAL_DAMAGE_PVM);
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}
		
bool Unit::ExcellentDamageChance(Unit* pTarget, bool elemental, int32 & damage, int32 damage_max)
{
	float rate = this->GetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE);

	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( pTarget->IsPlayer() )
		{
			rate += pPlayer->GetDataFloat(PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_RATE_PVP);
		}
		else
		{
			rate += pPlayer->GetDataFloat(PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_RATE_PVM);
		}
	}

	rate -= pTarget->GetFloatData(UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE);

	if ( rate < 0.0f )
	{
		rate = 0.0f;
	}

	if (roll_chance_f(rate))
	{
		damage = (damage_max * 120 / 100);
		damage += this->GetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD);

		if ( Player* pPlayer = this->ToPlayer() )
		{
			if ( pTarget->IsPlayer() )
			{
				damage += pPlayer->GetData(PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_PVP);
			}
			else
			{
				damage += pPlayer->GetData(PLAYER_DATA_INCREASE_EXCELLENT_DAMAGE_PVM);
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}
	
bool Unit::IgnoreDefenseChance(Unit* pTarget, bool elemental)
{
	return false;
}

bool Unit::DoubleDamageChance(Unit* pTarget, bool elemental, int32 & damage)
{
	float rate = this->GetFloatData(UNIT_FLOAT_DOUBLE_DAMAGE_RATE);

	rate -= pTarget->GetFloatData(UNIT_FLOAT_RESIST_DOUBLE_DAMAGE_RATE);

	if ( rate < 0.0f )
	{
		rate = 0.0f;
	}

	if (roll_chance_f(rate))
	{
		damage += damage;
		damage += this->GetIntData(UNIT_INT_DOUBLE_DAMAGE_ADD);

		return true;
	}
	else
	{
		return false;
	}
}

bool Unit::TripleDamageChance(Unit* pTarget, bool elemental, int32 & damage)
{
	float rate = this->GetFloatData(UNIT_FLOAT_TRIPLE_DAMAGE_RATE);

	if (rate < 0.0f)
	{
		rate = 0.0f;
	}

	if (roll_chance_f(rate))
	{
		damage += (damage * 2);
		return true;
	}
	else
	{
		return false;
	}
}

void Unit::ProcessReflect(Unit* pTarget, int32 damage)
{
	if ( damage <= 0 )
	{
		return;
	}

	if ( !pTarget )
	{
		return;
	}

	if ( !this->IsLive() || pTarget == this )
	{
		return;
	}

	if ( !this->AttackAllowed(pTarget, true, true) )
	{
		return;
	}

	/*if ( !this->ViewportAttack(pTarget) )
	{
		return;
	}*/

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetClass() == 709 ||
			 pMonster->GetClass() == 710 ||
			 pMonster->GetClass() == 711 ||
			 pMonster->GetClass() == 712 )
		{
			damage = 1;
		}
	}

	int32 shield_damage = this->DealDamage(pTarget, damage, 0, 0, false, MONSTER_THREAT_DAMAGE_REFLECT);

	int32 send_damage = damage - shield_damage;

	if ( send_damage < 0 )
	{
		send_damage = 0;
	}

	uint8 interrupt = 0;

	if ( damage > 0 )
	{
		if (roll_chance_f(this->GetFloatData(UNIT_FLOAT_STUN_RATE)))
		{
			if ( !pTarget->IsDebuffInmune(false) && !pTarget->HasBuff(BUFF_STUN) )
			{
				pTarget->AddBuff(BUFF_STUN, 2, 0, this);
			}
		}

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
	}
	
	this->SendDamage(pTarget, send_damage, shield_damage, interrupt, DAMAGE_TYPE_REFLECT);

	pTarget->Unit::AfterHitCheck(this, send_damage, shield_damage, DAMAGE_TYPE_REFLECT);
}

void Unit::MoveSend(coord_type x, coord_type y, uint8 dir)
{
	MOVE_RESULT pMsg(this->GetEntry(), x, y, dir);
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Unit::GetManaShieldValue(float & value, int32 & duration)
{
	value = 10 + (this->GetAgility() / 50) + (this->GetEnergy() / 200);
	duration = 40 + (this->GetEnergy() / 60);

	if ( this->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		if ( value > sGameServer->GetSkillManaShieldCastleSiegeLimit() )
			value = sGameServer->GetSkillManaShieldCastleSiegeLimit();
	}
	else
	{
		if ( value > sGameServer->GetSkillManaShieldLimit() )
			value = sGameServer->GetSkillManaShieldLimit();
	}
	
	value += this->MagicGetValue(403);
	duration += this->MagicGetValue(404);

	//value += this->MagicGetValue(1080, true);

	if (value >= 100)
	{
		value = 99;
	}
}

void Unit::AdditionalDamage(Unit* pTarget, uint16 skill, int32 damage, uint16 damage_type)
{
	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(skill);

	float rate = 0.0f;
	
	if( skill_base == SKILL_FIRE_BLOW || 
		skill_base == SKILL_METEOR_STRIKE || 
		skill_base == SKILL_METEOR_STRIKE_2 || 
		skill_base == SKILL_METEOR_STORM ||
		skill_base == SKILL_SOUL_SEEKER || 
		skill_base == SKILL_FOCUS_SHOT || 
		skill_base == SKILL_FIRE_BEAST || 
		skill_base == SKILL_AQUA_BEAST || 
		skill_base == SKILL_ICE_BLOOD || 
		skill_base == SKILL_FIRE_BLOOD || 
		skill_base == SKILL_DARK_BLAST || 
		skill_base == SKILL_EVIL_WIND ||
		skill_base == SKILL_DARK_PHOENIX_SHOT ||
		skill_base == SKILL_SPIRIT_HOOK ||
		skill_base == SKILL_DEATHSIDE || 
		skill_base == SKILL_SWORD_BLOW ||
		skill_base == SKILL_ICE_BLAST ||
		skill_base == SKILL_BURSTING_FLARE)
	{
		rate = 5.0f;
	}

	if ( Player* pPlayer = this->ToPlayer() )
	{
		rate += pPlayer->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT_RATE);
	}

	if ( roll_chance_f(rate) )
	{
		int32 shield_damage = this->DealDamage(pTarget, damage, 0, skill_base, false, MONSTER_THREAT_DAMAGE_NORMAL);

		int32 send_damage = damage - shield_damage;

		if ( send_damage < 0 )
		{
			send_damage = 0;
		}

		sGameServer->LogDamagePVP(this->ToPlayer(), pTarget->ToPlayer(), send_damage, shield_damage, skill);

		this->SendDamage(pTarget, send_damage, shield_damage, 0, damage_type, 1);

		pTarget->Unit::AfterHitCheck(this, send_damage, shield_damage, damage_type);
	}
}

void Unit::DebuffBlowAttack(Unit* pTarget, Skill * pSkill)
{
	if ( !pTarget )
	{
		return;
	}

	if ( !pSkill )
	{
		return;
	}

	if ( !this->AttackAllowed(pTarget, true) )
	{
		return;
	}

	if ( !this->ViewportAttack(pTarget) )
	{
		return;
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		MonsterAI const* pAI = pMonster->GetAI();

		if ( pAI && pAI->InmuneToDebuffBlow(true) )
		{
			return;
		}
	}

	this->RemoveBuff(BUFF_TRANSPARENCY);

	if ( pTarget->HasBuff(pSkill ? BUFF_INMUNE_SKILL: BUFF_INMUNE_ATTACK) )
	{
		this->MissSend(pTarget, pSkill, false, 0);
		return;
	}

	if ( pTarget->HasBuff(BUFF_ORDER_OF_PROTECTION) )
	{
		this->MissSend(pTarget, pSkill, false, 0);
		return;
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetAI() && !pMonster->GetAI()->EnableAttack(this, pSkill, 0) )
		{
			return;
		}
	}

	uint16 skill = pSkill->GetSkill();
	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(skill);
	int8 skill_type = sSkillMgr->GetSkillType(skill);
	uint16 damage_type = 0;

	int32 target_defense = this->GetTargetDefense(pTarget, damage_type, 0);
	int32 damage = this->GetMajesticSpecialDamage(target_defense, damage_type, pSkill, pTarget);

	if ( damage < 0 )
	{
		damage = 0;
	}

	switch ( skill_base )
	{
	case SKILL_POISON_STORM:
		{
			SkillHandler(this, pTarget, pSkill, sSkillMgr->GetSkill(skill)).SkillPoisonStormSplash(damage);
		} break;

	case SKILL_FROZEN_SLAUGHTER:
		{
			SkillHandler(this, pTarget, pSkill, sSkillMgr->GetSkill(skill)).SkillFrozenSlaughterSplash(damage);
		} break;
	}

	if ( Monster* pMonster = pTarget->ToCreature() )
	{
		if ( pMonster->GetClass() == 709 ||
			 pMonster->GetClass() == 710 ||
			 pMonster->GetClass() == 711 ||
			 pMonster->GetClass() == 712 )
		{
			damage = 1;
		}
	}

	int32 shield_damage = this->DealDamage(pTarget, damage, 0, skill_base, false, MONSTER_THREAT_DAMAGE_NORMAL);

	this->SetRest(0);

	pTarget->ReduceBuffTime(BUFF_ICE_ARROW, 1);
	pTarget->RemoveBuff(BUFF_SLEEP);

	int32 send_damage = damage - shield_damage;

	if ( send_damage < 0 )
	{
		send_damage = 0;
	}

	this->SendDamage(pTarget, send_damage, shield_damage, 0, damage_type);

	pTarget->Unit::AfterHitCheck(this, send_damage, shield_damage, damage_type);

	if ( this->PowerGet(POWER_LIFE) <= 0 )
	{
		this->Unit::AfterHitCheck(pTarget, 0, 0, 0);
	}
}

std::string Unit::BuildPath() const
{
	World const* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return "";
	}

	std::ostringstream stream(std::to_string(pWorld->GetEntry()) + std::string("/") + std::string(pWorld->GetCurrentStatusName()) + std::string(" - "));

	for ( int32 n = 0; n < this->GetPathData()->GetCount(); ++n )
	{
		WorldGrid const& grid = pWorld->GetGrid(this->GetPathData()->GetPosition(n)->GetX(), this->GetPathData()->GetPosition(n)->GetY());

		stream << "[" << this->GetPathData()->GetPosition(n)->GetX() << "/" << this->GetPathData()->GetPosition(n)->GetY() << " - Att: " << uint32(grid.attribute) << "] ";
	}

	return stream.str();
}

float Unit::GetAbsorbDamageRate() const
{
	float value = 100 * (1 - (this->GetEffect(BUFF_OPTION_MANA_SHIELD) / 100.0f));

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

float Unit::GetIncreaseDamageRate() const
{
	return 0;
}

BuffData const* Unit::GetSolidProtectionBuff() const
{
	BUFF_LOOP(i)
	{
		auto pBuff = this->GetBuffData(i);

		if (pBuff->GetID() == 326 ||
			pBuff->GetID() == 329 ||
			pBuff->GetID() == 330 ||
			pBuff->GetID() == 331 ||
			pBuff->GetID() == 332 ||
			pBuff->GetID() == 333 ||
			pBuff->GetID() == 335)
		{
			return pBuff;
		}
	}

	return nullptr;
}