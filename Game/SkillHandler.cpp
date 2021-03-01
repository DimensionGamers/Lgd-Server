SkillHandler::SkillHandler(Unit* pCaster, Unit* pTarget, Skill * pSkill, skill_template const* skill_info, coord_type x, coord_type y, int32 count, int32 angle)
{
	this->SetCaster(pCaster);
	this->SetTarget(pTarget);
	this->SetSkill(pSkill);
	this->SetSkillInfo(skill_info);

	if ( !this->GetSkillInfo() )
	{
		this->SetSkillInfo(this->GetSkill() ? sSkillMgr->GetSkill(this->GetSkill()->GetSkill()): nullptr);
	}

	this->SetAttackX(x);
	this->SetAttackY(y);

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
		this->PartyMember[i] = nullptr;

	this->party_member_count = 0;

	this->SetCount(count);
	this->SetAngle(angle);
}

bool SkillHandler::SkillUseProc(bool combo, coord_type x, coord_type y, uint8 dir, uint8 dis, uint8 pos, bool duration)
{
	SafeRAssert(this->GetCaster(), "this->GetCaster() == nullptr", false);
	SafeRAssert(this->GetSkill(), "this->GetSkill() == nullptr", false);
	SafeRAssert(this->GetSkillInfo(), "this->GetSkillInfo() == nullptr", false);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		if ((this->GetSkill()->GetSkill() == SKILL_FIRE_BREATH) && pPlayer->GetVisibleMount() != PET_DINORANT)
		{
			if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
			{
				pPlayer->GetHelper()->SetSkillID(0);
			}

			return false;
		}

		if ((this->GetSkill()->GetSkill() == SKILL_PLASMA_STORM) && pPlayer->GetVisibleMount() != PET_FENRIR)
		{
			if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
			{
				pPlayer->GetHelper()->SetSkillID(0);
			}

			return false;
		}

		if ((this->GetSkill()->GetSkill() == SKILL_EARTHSHAKE) && pPlayer->GetVisibleMount() != PET_DARK_HORSE)
		{
			if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
			{
				pPlayer->GetHelper()->SetSkillID(0);
			}

			return false;
		}

		if (this->GetSkillInfo()->GetMountCheck() == 2 && pPlayer->HasMount())
		{
			if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
			{
				pPlayer->GetHelper()->SetSkillID(0);
			}

			return false;
		}

		if (sGameServer->IsElfSkillWeaponCheck())
		{
			if (this->GetSkillInfo()->GetBaseSkill() == SKILL_TRIPLE_SHOT ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_ICE_ARROW ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_PENETRATION ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_STARFALL ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_MULTI_SHOT ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_POISON_ARROW ||
				this->GetSkillInfo()->GetBaseSkill() == SKILL_FOCUS_SHOT)
			{
				Item const* pItem = pPlayer->GetInventory()->GetItem(WEAPON_01);

				if (!pItem || !pItem->IsItem())
				{
					if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
					{
						pPlayer->GetHelper()->SetSkillID(0);
					}

					return false;
				}

				if (pItem->GetKind2() != ItemKind::BOW && pItem->GetKind2() != ItemKind::CROSSBOW)
				{
					if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
					{
						pPlayer->GetHelper()->SetSkillID(0);
					}

					return false;
				}
			}
		}
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		if (pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_REMOVE_COOLDOWN) <= 0.0f)
		{
			if (!pPlayer->IsSkillCooldown(this->GetSkill()->GetSkill()))
			{
				if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
				{
					pPlayer->GetHelper()->SetSkillID(0);
				}

				return false;
			}
		}
	}

	if (this->GetCaster()->IsCreature() && this->GetTarget())
	{
		switch (this->GetSkillInfo()->GetBaseSkill())
		{
		case SKILL_TRIPLE_SHOT:
		case SKILL_DECAY:
		case SKILL_ICE_STORM:
		case SKILL_DEATH_STAB:
		case SKILL_RAGEFUL_BLOW:
		case SKILL_PENETRATION:
		case SKILL_POWER_SLASH:
		case SKILL_ELECTRIC_SPARK:
		case SKILL_FIRE_SCREAM:
		case SKILL_EXPLOSION:
		case SKILL_REQUIEM:
		case SKILL_POLLUTION:
		case SKILL_LIGHTNING_SHOCK:
		case SKILL_STRIKE_OF_DESTRUCTION:
		case SKILL_MULTI_SHOT:
		case SKILL_CHAOTIC_DESEIER:
		{
									  x = this->GetTarget()->GetX();
									  y = this->GetTarget()->GetY();
		} break;
		}
	}

	this->SetAngle(pos);
	this->SetAttackX(x);
	this->SetAttackY(y);
	this->SetCombo(combo);
	this->SetSpecialValue(dir);

	if (this->GetCaster()->GetNova()->IsRunning() && this->GetSkillInfo()->GetBaseSkill() != SKILL_NOVA)
	{
		this->GetCaster()->RunNova();
		return true;
	}

	bool mana_consume = true;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		if ((this->GetSkillInfo()->GetBaseSkill() == SKILL_UPPER_BEAST ||
			this->GetSkillInfo()->GetBaseSkill() == SKILL_CHAIN_DRIVE ||
			this->GetSkillInfo()->GetBaseSkill() == SKILL_DARK_SIDE ||
			this->GetSkillInfo()->GetBaseSkill() == SKILL_DRAGON_SLAYER) &&
			pPlayer->GetRageFighterSkill() == this->GetSkill()->GetSkill())
		{
			if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
			{
				if (pPlayer->GetRageFighterSkillCount() > 0)
				{
					mana_consume = false;
				}

				if (pPlayer->GetRageFighterSkillCount() >= pPlayer->GetRageFighterSkillMaxCount())
				{
					return false;
				}

				pPlayer->IncreaseRageFighterSkillCount(1);
			}
		}
	}

	if (mana_consume)
	{
		if (Player* pPlayer = this->GetCaster()->ToPlayer())
		{
			int32 manause = pPlayer->SkillManaUse(this->GetSkill(), this->GetSkillInfo());
			int32 staminause = pPlayer->SkillStaminaUse(this->GetSkill(), this->GetSkillInfo());

			if (!pPlayer->PowerHas(POWER_MANA, manause) || !pPlayer->PowerHas(POWER_STAMINA, staminause))
			{
				if (pPlayer->GetHelper()->GetSkillID() == this->GetSkill()->GetSkill())
				{
					pPlayer->GetHelper()->SetSkillID(0);
				}

				return false;
			}

			pPlayer->PowerReduce(POWER_MANA, manause, true);
			pPlayer->PowerReduce(POWER_STAMINA, staminause, true);
		}
	}

	if (duration && this->GetSkillInfo()->GetBaseSkill() != SKILL_DARK_SIDE)
	{
		DURATION_MAGIC_ATTACK_RESULT pMsg(this->GetSkill()->GetSkill(), this->GetCaster()->GetEntry(), x, y, dir);

		this->GetCaster()->sendPacket(MAKE_PCT(pMsg));
		this->GetCaster()->sendPacket_viewport(MAKE_PCT(pMsg));
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		pPlayer->ApplySkillTime(this->GetSkillInfo()->GetBaseSkill(), this->GetAttackX(), this->GetAttackY());

		pPlayer->SetLastDurationSkill(this->GetSkill()->GetSkill());
		pPlayer->SetLastDurationSkillTime(GetTickCount());
		pPlayer->SetLastDurationSkillCount(0);
		pPlayer->SetLastDurationSkillX(this->GetAttackX());
		pPlayer->SetLastDurationSkillY(this->GetAttackY());

		if (sGameServer->IsComboCheckEnabled() && !pPlayer->GetHelper()->IsOffline() && combo)
		{
			uint32 combo_check = pPlayer->GetTimer(PLAYER_TIMER_COMBO_CHECK)->GetElapsed();
			pPlayer->GetTimer(PLAYER_TIMER_COMBO_CHECK)->Start();

			if (combo_check <= sGameServer->GetComboCheckTimeDiff())
			{
				pPlayer->IncreaseCounter(PLAYER_COUNTER_COMBO_CHECK, 1);

				if (pPlayer->GetCounter(PLAYER_COUNTER_COMBO_CHECK) >= sGameServer->GetComboCheckCount())
				{
					if (sGameServer->IsComboCheckDatabaseLog())
					{
						sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_COMBO_CHECK, "Combo Check: %d", pPlayer->GetCounter(PLAYER_COUNTER_COMBO_CHECK));
					}

					if (sGameServer->IsComboCheckKick())
					{
						KICK_PLAYER(pPlayer, "Combo Check");

						sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_KICK, "Combo Check");
						return false;
					}

					if (sGameServer->IsComboCheckStop())
					{
						return false;
					}
				}
			}
			else
			{
				pPlayer->SetCounter(PLAYER_COUNTER_COMBO_CHECK, 0);
			}
		}
	}

	this->GetCaster()->SetShieldTick(0);

	switch (this->GetSkillInfo()->GetBaseSkill())
	{
	case SKILL_MANA_SHIELD: { this->SkillBuffManaShield(); } break;
	case SKILL_DEFENSE: { this->SkillShieldDefense(); } break;
	case SKILL_HEAL: { this->SkillBuffHeal(); } break;
	case SKILL_GREATER_DEFENSE: { this->SkillBuffGreaterDefense(); } break;
	case SKILL_GREATER_DAMAGE: { this->SkillBuffGreaterAttack(); } break;
	case SKILL_GREATER_FORTITUDE: { this->SkillBuffGreaterFortitude(); } break;
	case SKILL_SUMMON: { this->SkillSummon(); } break;
	case SKILL_CRITICAL_DAMAGE: { this->SkillBuffCriticalDamage(); } break;
	case SKILL_SUMMON_MONSTER_1: { this->SkillSummonMonster(26); } break;
	case SKILL_SUMMON_MONSTER_2: { this->SkillSummonMonster(32); } break;
	case SKILL_SUMMON_MONSTER_3: { this->SkillSummonMonster(21); } break;
	case SKILL_SUMMON_MONSTER_4: { this->SkillSummonMonster(20); } break;
	case SKILL_SUMMON_MONSTER_5: { this->SkillSummonMonster(10); } break;
	case SKILL_SUMMON_MONSTER_6: { this->SkillSummonMonster(150); } break;
	case SKILL_SUMMON_MONSTER_7: { this->SkillSummonMonster(151); } break;
	case SKILL_SUMMON_SATYROS:	 { this->SkillSummonMonster(597); } break;

	case SKILL_STERN: { this->SkillBuffStun(); } break;
	case SKILL_CANCEL_STERN: { this->SkillBuffCancelStun(); } break;
	case SKILL_SWELL_MANA: { this->SkillBuffSwellMana(); } break;
	case SKILL_INVISIBILITY: { this->SkillBuffInvisibility(); } break;
	case SKILL_CANCEL_INVISIBILITY: { this->SkillBuffCancelInvisibility(); } break;
	case SKILL_CANCEL_MAGIC: { this->SkillBuffCancelMagic(); } break;
	case SKILL_CRESCENT_MOON_SLASH: { this->SkillCrescentMoonSlash(); } break;
	case SKILL_MANA_GLAIVE: { this->SkillManaGlaive(); } break;
	case SKILL_STARFALL: { this->SkillStarfall(); } break;
	case SKILL_SPIRAL_SLASH: { this->SkillSpiralSlash(); } break;
	case SKILL_MANA_RAYS: { this->SkillManaRays(); } break;
	case SKILL_FIRE_BLAST: { this->SkillFireBlast(); } break;

	case SKILL_INFINITY_ARROW: { this->SkillBuffInfinityArrow(); } break;

	case SKILL_DAMAGE_REFLECTION: { this->SkillBuffDamageReflection(); } break;
	case SKILL_BERSERKER: { this->SkillBuffBerserker(); } break;
	case SKILL_DARKNESS: { this->SkillBuffDarkness(); } break;
	case SKILL_SLEEP: { this->SkillBuffSleep(); } break;
	case SKILL_WEAKNESS: { this->SkillBuffWeakness(); } break;
	case SKILL_INNOVATION: { this->SkillBuffInnovation(); } break;
	case SKILL_EXPANSION_OF_WIZARDY: { this->SkillBuffExpansionOfWizardy(); } break;
	case SKILL_RECOVERY: { this->SkillBuffRecovery(); } break;
	case SKILL_RED_ARMOR_IGNORE: { this->SkillBuffRedArmorIgnore(); } break;
	case SKILL_FITNESS: { this->SkillBuffFitness(); } break;
	case SKILL_DEFENSE_SUCCESS_RATE: { this->SkillBuffDefenseSuccessRate(); } break;

	case SKILL_POISON:
	case SKILL_METEORITE:
	case SKILL_LIGHTNING:
	case SKILL_FIRE_BALL:
	case SKILL_ICE:
	case SKILL_ENERGY_BALL:
	case SKILL_FALLING_SLASH:
	case SKILL_LUNGE:
	case SKILL_UPPERCUT:
	case SKILL_CYCLONE:
	case SKILL_SLASH:
	case SKILL_POWER_WAVE:
	case SKILL_FIRE_BREATH:
	case SKILL_ICE_ARROW:
	case SKILL_DRAGON_SLAYER:
	case SKILL_POISON_ARROW:
	{
							   if (this->GetTarget())
							   {
								   this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
							   }
	} break;

	case SKILL_TRIPLE_SHOT:
	{
							  this->SkillAttackTripleShot();
	} break;

	case SKILL_EVIL_SPIRIT:
	{
							  this->SkillAttackEvilSpirit();
	} break;

	case SKILL_HELL_FIRE:
	{
							this->SkillAttackHellFire();
	} break;

	case SKILL_INFERNO:
	{
						  this->SkillAttackInferno();
	} break;

	case SKILL_FLAME:
	{
						this->SkillAttackFlame();
	} break;

	case SKILL_FIRE_SCREAM:
	{
							  this->SkillAttackFireScream();
	} break;

	case SKILL_BLAST:
	{
						this->SkillBlast();
	} break;

	case SKILL_AQUA_BEAM:
	{
							this->SkillAquaBeam();
	} break;

	case SKILL_PENETRATION:
	{
							  this->SkillPenetration();
	} break;

	case SKILL_FIRE_SLASH:
	{
							 this->SkillFireSlash();
	} break;

	case SKILL_TWISTER:
	{
						  this->SkillTwister();
	} break;

	case SKILL_DECAY:					{ this->SkillAttackToLocation(); } break;
	case SKILL_ICE_STORM:				{ this->SkillAttackToLocation(); } break;
	case SKILL_TWISTING_SLASH:			{ this->SkillAttackTwistingSlash(); } break;
	case SKILL_RAGEFUL_BLOW:			{ this->SkillAttackRagefulBlow(); } break;
	case SKILL_DEATH_STAB:				{ this->SkillAttackDeathStab(); } break;
	case SKILL_POWER_SLASH:				{ this->SkillAttackPowerSlash(); } break;
	case SKILL_FIRE_BURST:				{ this->SkillAttackFireBurst(); } break;
	case SKILL_EARTHSHAKE:				{ this->SkillAttackDarkHorse(); } break;
	case SKILL_CHAIN_LIGHTNING:			{ this->SkillAttackChainLightning(); } break;
	case SKILL_LIGHTNING_SHOCK:			{ this->SkillAttackLightningShock(); } break;
	case SKILL_STRIKE_OF_DESTRUCTION:	{ this->SkillAttackStrikeOfDestruction(); } break;
	case SKILL_MULTI_SHOT:				{ this->SkillAttackMultiShot(); } break;
	case SKILL_FLAME_STRIKE:			{ this->SkillAttackFlameStrike(); } break;
	case SKILL_CHAOTIC_DESEIER:			{ this->SkillAttackChaoticDiseier(); } break;
	case SKILL_DRAGON_LORE:				{ this->SkillAttackDragonLore(); } break;
	case SKILL_DRAIN_LIFE:				{ this->SkillAttackDrainLife(); } break;
	case SKILL_NOVA:					{ this->SkillAttackNova(); } break;
	case SKILL_PLASMA_STORM:			{ this->SkillAttackFenrir(); } break;
	case SKILL_FLAME_OF_EVIL:			{ this->SkillAttackFlameOfEvil(); } break;
	case SKILL_DOPPLEGANGER_SUICIDE:	{ this->SkillAttackDopplegangerSuicide(); } break;
	case SKILL_EXPLOSION:				{ this->SkillAttackExplosion(); } break;
	case SKILL_REQUIEM:					{ this->SkillAttackRequiem(); } break;
	case SKILL_POLLUTION:				{ this->SkillAttackPollution(); } break;
	case SKILL_GIGANTIC_STORM:			{ this->SkillAttackGiganticStorm(); } break;
	case SKILL_FORCE:					{ this->SkillAttackForce(); } break;
	case SKILL_LARGE_RING_BLOWER:
	case SKILL_UPPER_BEAST:
	case SKILL_CHAIN_DRIVE:				{ this->SkillAttackMultiSingle(); } break;
	case SKILL_DARK_SIDE:				{ this->SkillAttackDarkSide(); } break;
	case SKILL_CHARGE:					{ this->SkillAttackCharge(); } break;
	case SKILL_PHOENIX_SHOT:			{ this->SkillAttackPhoenixShot(); } break;

	case SKILL_BLOOD_STORM:				{ this->SkillBloodStorm(); } break;
	case SKILL_CURE:					{ this->SkillCure(); } break;
	case SKILL_PARTY_HEAL:				{ this->SkillPartyHeal(); } break;
	case SKILL_BLESS:					{ this->SkillBless(); } break;
	case SKILL_BLIND:					{ this->SkillBlind(); } break;
	case SKILL_EARTH_PRISON:			{ this->SkillEarthPrison(); } break;
	case SKILL_IRON_DEFENSE_1:			{ this->SkillIronDefense(); } break;
	case SKILL_IRON_DEFENSE_2:			{ this->SkillIronDefense(); } break;
	case SKILL_BLOOD_HOWLING:			{ this->SkillBloodHowling(); } break;

	case SKILL_SPIN_STEP: { this->SkillSpinStep(); } break;
	case SKILL_CIRCLE_SHIELD: { this->SkillCircleShield(); } break;
	case SKILL_OBSIDIAN: { this->SkillObsidian(); } break;
	case SKILL_MAGIC_PIN: { this->SkillMagicPin(); } break;
	case SKILL_CLASH: { this->SkillClash(); } break;
	case SKILL_HARSH_STRIKE: { this->SkillHarshStrike(); } break;
	case SKILL_SHINING_PEAK: { this->SkillShiningPeak(); } break;
	case SKILL_WRATH: { this->SkillWrath(); } break;
	case SKILL_BRECHE: { this->SkillBreche(); } break;
	case SKILL_BURST: { this->SkillBurst(); } break;
	case SKILL_ARCHANGELS_WILL: { this->SkillArchangelsWill(); }break;
	case SKILL_ILLUSION: { this->SkillIllusion(); } break;
	case SKILL_SHADOW_STEP: { this->SkillShadowStep(); } break;
	case SKILL_EVASION: { this->SkillEvasion(); } break;
	case SKILL_RUSH: { this->SkillRush(); } break;
	case SKILL_RUSH_KNIGHT: { this->SkillRush(); } break;
	case SKILL_FIRE_BLOW: { this->SkillFireBlow(); } break;
	case SKILL_SWORD_BLOW: { this->SkillSwordBlow(); } break;
	case SKILL_METEOR_STRIKE: { this->SkillMeteorStrike(); } break;
	case SKILL_METEOR_STORM: { this->SkillMeteorStorm(); } break;
	case SKILL_SOUL_SEEKER: { this->SkillSoulSeeker(); } break;
	case SKILL_FOCUS_SHOT: { this->SkillFocusShot(); } break;
	case SKILL_FIRE_BEAST: { this->SkillFireBeast(); } break;
	case SKILL_AQUA_BEAST: { this->SkillAquaBeast(); } break;
	case SKILL_ICE_BLOOD: { this->SkillIceBlood(); } break;
	case SKILL_FIRE_BLOOD: { this->SkillFireBlood(); } break;
	case SKILL_DARK_BLAST: { this->SkillDarkBlast(); } break;
	case SKILL_METEOR_STRIKE_2: { this->SkillMeteorStrike(); } break;
	case SKILL_EVIL_WIND: { this->SkillEvilWind(); } break;
	case SKILL_DARK_PHOENIX_SHOT: { this->SkillDarkPhoenixShot(); } break;
	case SKILL_SPIRIT_HOOK: { this->SkillSpiritHook();  } break;
	case SKILL_DEATHSIDE: { this->SkillDeathside(); } break;

	case SKILL_SUMMON_MONSTER: { this->SkillMonsterSummon(); } break;
	case SKILL_MAGIC_ATTACK_INMUNITY: { this->SkillMonsterInmuneToMagic(); } break;
	case SKILL_PHYSICAL_ATTACK_INMUNITY: { this->SkillMonsterInmuneToHarm(); } break;

	case SKILL_ELECTRIC_SPARK: { this->SkillAttackElectricSpark(); } break;

	case SKILL_RADIANCE_INMUNE_I: { this->SkillRadianceInmuneI(); } break;
	case SKILL_RADIANCE_INMUNE_II: { this->SkillRadianceInmuneII(); } break;
	case SKILL_RADIANCE_WIDENED_I: { this->SkillRadianceWidenedI(); } break;

	case SKILL_POISON_STORM: { this->SkillPoisonStorm(); } break;
	case SKILL_FROZEN_SLAUGHTER: { this->SkillFrozenSlaughter(); } break;
	case SKILL_BLEEDING_HEAT: { this->SkillBloodyingHit(); } break;
	case SKILL_SHOCK_BOOM: { this->SkillShockBoom(); } break;

	case SKILL_MAGIC_ARROW:
		this->SkillMagicArrow();
		break;

	case SKILL_PLASMA_BALL:
		this->SkillPlasmaBall();
		break;

	case SKILL_LIGHTNING_STORM:
		this->SkillLightningStorm();
		break;

	case SKILL_BURST_RW:
		this->SkillBurstRW();
		break;

	case SKILL_HASTE_RW:
		this->SkillHasteRW();
		break;

	case SKILL_SWORD_INERTIA:
		this->SkillSwordInertia();
		break;

	case SKILL_BAT_FLOCK:
		this->SkillBatFlock();
		break;

	case SKILL_PIERCE_ATTACK:
		this->SkillPierceAttack();
		break;

	case SKILL_DETECTION:
		this->SkillDetection();
		break;

	case SKILL_DEMOLISH:
		this->SkillDemolish();
		break;

	case SKILL_SWORD_WRATH:
		this->SkillSwordWrath();
		break;

	case SKILL_STRONG_BELIEF:
		this->SkillStrongBelief();
		break;

	case SKILL_SOLID_PROTECTION:
		this->SkillSolidProtection();
		break;

	case SKILL_ELITE_MONSTER:
		this->EliteMonsterSkill();
		break;

	case SKILL_DARK_PLASMA:
		this->SkillDarkPlasma();
		break;

	case SKILL_ICE_BREAK:
		this->SkillIceBreak();
		break;

	case SKILL_ICE_BLAST:
		this->SkillIceBlast();
		break;

	case SKILL_FIRE_DEATH:
		this->SkillFireDeath();
		break;

	case SKILL_BURSTING_FLARE:
		this->SkillBurstingFlare();
		break;
	}

	return true;
}

void SkillHandler::SkillSummonMonster(uint16 monster)
{
	if ( this->GetCaster()->GetSummoned() )
	{
		Player* pPlayer = this->GetCaster()->ToPlayer();

		if ( pPlayer )
		{
			if ( pPlayer->GetHelper()->IsStarted() )
			{
				this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
				return;
			}
		}

		this->GetCaster()->SendSummonedHP(0, 60);
		this->GetCaster()->KillSummoned();

		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
		return;
	}

	if ( !this->GetCaster()->IsWorldFlag(WORLD_FLAG_ALLOW_SUMMON_MONSTER) )
	{
		PLAYER_POINTER(this->GetCaster())->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to summon on this map.");
		return;
	}

	Monster *mSummoned = sObjectMgr->MonsterTryAdd(monster, this->GetCaster()->GetWorldId());

	if ( mSummoned )
	{
		mSummoned->SetSendType(MAIN_OBJECT_PLAYER);
		mSummoned->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		mSummoned->SetMoveDistance(15);
		mSummoned->SetX(this->GetCaster()->GetX() + 1);
		mSummoned->SetY(this->GetCaster()->GetY() - 1);
		mSummoned->SetDirection(2);
		mSummoned->SetWorldId(this->GetCaster()->GetWorldId());
		mSummoned->SetInstance(this->GetCaster()->GetInstance());
		mSummoned->SetSummoner(this->GetCaster());
		mSummoned->SetScriptName("summon_ai");
		mSummoned->SetNameData("$owner's %s", mSummoned->GetName());
		this->GetCaster()->SetSummoned(mSummoned);

		mSummoned->IncreaseScriptMaxPower(POWER_LIFE, mSummoned->GetScriptMaxPower(POWER_LIFE) * this->GetCaster()->MagicGetValue(SKILL_TREE_SUMMONED_MONSTER_INCREASE_HP) / 100);
		mSummoned->PowerSetMax(POWER_LIFE, mSummoned->GetScriptMaxPower(POWER_LIFE));
		mSummoned->PowerSet(POWER_LIFE, mSummoned->GetScriptMaxPower(POWER_LIFE));
		mSummoned->PercentIntData(UNIT_INT_DEFENSE, this->GetCaster()->MagicGetValue(SKILL_TREE_SUMMONED_MONSTER_INCREASE_DEFENSE));
		mSummoned->PercentIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_SUMMON_DAMAGE));
		mSummoned->PercentIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_SUMMON_DAMAGE));
		
		mSummoned->AddToWorld();

		this->GetCaster()->SendSummonedHP(mSummoned->PowerGet(POWER_LIFE), mSummoned->PowerGetTotal(POWER_LIFE));
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillShieldDefense()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		auto pItem = this->GetCaster()->ToPlayer()->GetInventory()->GetItem(WEAPON_02);

		if (!pItem->IsItem())
		{
			return;
		}

		if (pItem->GetKind2() != ItemKind::SHIELD)
		{
			return;
		}

		if (!pItem->GetSkill())
		{
			return;
		}
	}

	this->GetCaster()->SetShieldTick(GetTickCount());
	this->GetCaster()->ActionSend(this->GetCaster()->GetEntry(), 18, this->GetCaster()->GetDirection());
}

void SkillHandler::SkillBuffManaShield()
{
	if (!this->GetTarget() || !this->GetTarget()->IsPlaying() || !this->GetTarget()->IsLive())
	{
		this->SetTarget(this->GetCaster());
	}

	if (this->GetTarget() != this->GetCaster())
	{
		if (this->GetCaster()->IsPlayer() && !this->GetTarget()->IsPlayer())
		{
			this->SetTarget(this->GetCaster());
		}

		if (this->GetCaster()->IsCreature())
		{
			this->SetTarget(this->GetCaster());
		}

		if (this->GetCaster()->IsPlayer())
		{
			if (this->GetCaster()->ToPlayer()->GetPartyID() == PARTY_NULL)
			{
				this->SetTarget(this->GetCaster());
			}

			if (this->GetTarget()->IsPlayer() && this->GetTarget()->ToPlayer()->GetPartyID() != this->GetCaster()->ToPlayer()->GetPartyID())
			{
				this->SetTarget(this->GetCaster());
			}
		}
	}

	if (!this->GetTarget())
	{
		return;
	}

	float value = 0;
	int32 duration = 0;

	this->GetCaster()->GetManaShieldValue(value, duration);

	if (this->GetTarget()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), value))
	{
		this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
			BuffEffect(BUFF_OPTION_MANA_SHIELD, value),
			BuffEffect(BUFF_OPTION_MP_MUL, this->GetCaster()->MagicGetValue(406)), duration, 0, this->GetCaster(), true);
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	}
	else
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), false);
	}
	
	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(value);
}

void SkillHandler::SkillBuffHeal()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
	{
		return;
	}

	if ( this->GetCaster()->IsPlayer() && this->GetTarget()->IsPlayer() )
	{
		if ( this->GetCaster()->ToPlayer()->GetHelper()->IsStarted() )
		{
			if ( this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID() )
			{
				return;
			}
		}
	}

	int32 add_life = 5 + (this->GetCaster()->GetEnergy() / 5);
	add_life += (add_life * this->GetCaster()->MagicGetValue(413) / 100);

	int32 heal_value = (this->GetTarget()->PowerGet(POWER_LIFE) + add_life) > this->GetTarget()->PowerGetTotal(POWER_LIFE) ? this->GetTarget()->PowerGetTotal(POWER_LIFE) - this->GetTarget()->PowerGet(POWER_LIFE): add_life;

	if ( heal_value < 0 )
	{
		heal_value = 0;
	}

	sHuntingRecord->Update(this->GetTarget()->ToPlayer(), 0, 0, 0, heal_value, 0);

	this->GetTarget()->PowerIncrease(POWER_LIFE, add_life, true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_life);
}

void SkillHandler::SkillBuffGreaterDefense()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
	{
		return;
	}

	if ( this->GetCaster()->IsPlayer() && this->GetTarget()->IsPlayer() )
	{
		if ( this->GetCaster()->ToPlayer()->GetHelper()->IsStarted() )
		{
			if ( this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID() )
				return;
		}
	}

	int32 add_defense = 2 + (this->GetCaster()->GetEnergy() / 8);
	add_defense += (add_defense * this->GetCaster()->MagicGetValue(417) / 100);
	add_defense += (add_defense * this->GetCaster()->MagicGetValue(423) / 100);
	
	if ( Player* pPlayer = this->GetCaster()->ToPlayer() )
	{
		add_defense += (add_defense * pPlayer->GetAncientEffect(ANCIENT_EFFECT_ENHANCED_DEFENSE_BUFF) / 100);
	}

	int32 duration = 60 + this->GetCaster()->GetMagicTreeLevel(423);

	if ( this->GetTarget()->IsPlayer() )
	{
		if ( this->GetTarget()->ToPlayer()->GetClass() == Character::DARK_KNIGHT ||
				this->GetTarget()->ToPlayer()->GetClass() == Character::RAGE_FIGHTER ||
				this->GetTarget()->ToPlayer()->GetClass() == Character::SLAYER)
		{
			add_defense += (add_defense / 10);
		}
	}
	
	if ( this->GetTarget()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_defense) )
	{
		this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, add_defense), duration, 0, this->GetCaster(), true);
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	}
	else
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), false);
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_defense);
}
	
void SkillHandler::SkillBuffGreaterAttack()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
	{
		return;
	}

	if ( this->GetCaster()->IsPlayer() && this->GetTarget()->IsPlayer() )
	{
		if ( this->GetCaster()->ToPlayer()->GetHelper()->IsStarted() )
		{
			if ( this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID() )
			{
				return;
			}
		}
	}

	int32 add_attack = 3 + (this->GetCaster()->GetEnergy() / 7);
	add_attack += (add_attack * this->GetCaster()->MagicGetValue(420) / 100);
	add_attack += (add_attack * this->GetCaster()->MagicGetValue(422) / 100);

	if ( Player* pPlayer = this->GetCaster()->ToPlayer() )
	{
		add_attack += (add_attack * pPlayer->GetAncientEffect(ANCIENT_EFFECT_ENHANCED_ATTACK_BUFF) / 100);
	}

	int32 duration = 60 + this->GetCaster()->GetMagicTreeLevel(422);

	if ( Player* pPlayer = this->GetTarget()->ToPlayer() )
	{
		if (pPlayer->GetClass() == Character::DARK_KNIGHT || pPlayer->GetClass() == Character::SLAYER)
		{
			add_attack += (add_attack / 10);
		}
	}
	
	if ( this->GetTarget()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_attack) )
	{
		this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_GREATER_DAMAGE, add_attack), duration, 0, this->GetCaster(), true);
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	}
	else
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), false);
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_attack);
}

void SkillHandler::SkillBuffGreaterFortitude()
{
	GetPartyMembers();

	int32 add_life = 12 + (this->GetCaster()->GetVitality() / 100) + (this->GetCaster()->GetEnergy() / 20) + (this->party_member_count - 1) + this->GetCaster()->MagicGetValue(356)/* + this->GetCaster()->MagicGetValue(1073, true)*/;
	int32 add_mana = this->GetCaster()->MagicGetValue(360);
	int32 add_stamina = this->GetCaster()->MagicGetValue(363);
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 10);

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			if ( this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_life, add_mana, add_stamina) )
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
					BuffEffect(BUFF_OPTION_HP_MUL, add_life), 
					BuffEffect(BUFF_OPTION_MP_MUL, add_mana), 
					BuffEffect(BUFF_OPTION_AG_MUL, add_stamina), duration, 0, this->GetCaster(), true);
				this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill(), true);
			}
		}
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_life + add_mana);
}

void SkillHandler::SkillSummon()
{
	if ( !this->GetCaster()->IsPlayer() )
	{
		return;
	}

	Player* pPlayer = this->GetCaster()->ToPlayer();
	Party* pParty = pPlayer->GetParty();

	if ( !pParty )
		return;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; i++ )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			continue;

		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if ( !pMember )
			continue;

		if ( pMember == pPlayer )
			continue;

		if ( pMember->IsDelayedTeleport() )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Skill is being used by a party member.");
			return;
		}
	}

	if ( !pPlayer->SummonCheckWorld() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot use summon skill on this map.");
		return;
	}

	if ( pPlayer->IsBusy() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Please close the window or Personal Shop that's currently open.");
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
	uint8 count = 0;

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; i++ )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			continue;

		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if ( !pMember )
			continue;

		if ( pMember == pPlayer )
			continue;

		if ( this->GetCaster()->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			if ( this->GetCaster()->GetCastleSiegeJoinSide() != pParty->GetMember(i)->GetPlayer()->GetCastleSiegeJoinSide() )
				continue;
		}

		if ( sCrywolf->IsInAltar(pMember) )
		{
			continue;
		}

		int16 move_level = sTeleport->GetMoveLevel(this->GetCaster()->GetWorldId(), this->GetCaster()->GetX(), this->GetCaster()->GetY(), pMember->IsSpecialCharacter());
		bool summon_world = pMember->SummonCheckWorld();
		bool busy = pMember->IsBusy();
		bool equipment = pMember->CanEnterWorld(this->GetCaster()->GetWorldId(), this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetCaster()->GetX(), this->GetCaster()->GetY());
		
		if ( pMember->GetTotalLevel() >= move_level &&
			 move_level != -1 && summon_world && !busy && equipment )
		{
			coord_type x = this->GetCaster()->GetX();
			coord_type y = this->GetCaster()->GetY();
			int32 loop = 50;

			while ( loop-- > 0 )
			{
				coord_type tmp_x = this->GetCaster()->GetX() + RANDOM(9) - 4;
				coord_type tmp_y = this->GetCaster()->GetY() + RANDOM(9) - 4;

				if ( this->GetCaster()->TeleportAreaCheck(tmp_x, tmp_y) )
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
			pMember->GetDelayedLocation()->SetInstance(this->GetCaster()->GetInstance());
			pMember->GetDelayedLocation()->SetWorldId(this->GetCaster()->GetWorldId());
			pMember->GetDelayedLocation()->SetDirection(pMember->GetDirection());
			pMember->SetDelayedTeleportGate(pPlayer->GetCurrentGate());
			pMember->SetDelayedTeleportHelper(pMember->GetHelper()->IsStarted() || pParty->GetMember(i)->IsAssisted());

			pMember->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Summoning in process. It may be cancelled if you move.");
			++count;
		}
		else
		{
			pMember->SendNotice(CUSTOM_MESSAGE_ID_RED, "You cannot warp.");
		}
	}

	if ( count > 0 )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%d players will be summoned.", count);
	}
}

void SkillHandler::SkillBuffCriticalDamage()
{
	GetPartyMembers();

	int32 critical_damage_add = (this->GetCaster()->GetLeadership() / 25) + (this->GetCaster()->GetEnergy() / 30) + this->GetCaster()->MagicGetValue(511);
	int32 critical_rate_add = this->GetCaster()->MagicGetValue(517);
	int32 excellent_rate_add = this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_GREATER_CRITICAL_DAMAGE_EXTENDED);
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 10) + this->GetCaster()->MagicGetValue(515);

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			if ( this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), critical_damage_add, critical_rate_add, excellent_rate_add) )
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
					BuffEffect(BUFF_OPTION_INCREASE_CRITICAL_DAMAGE, critical_damage_add), 
					BuffEffect(BUFF_OPTION_INCREASE_CRITICAL_DAMAGE_RATE, critical_rate_add), 
					BuffEffect(BUFF_OPTION_INCREASE_EXE_DAMAGE_RATE, excellent_rate_add), duration, 0, this->GetCaster(), true);
				this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill(), true);

				PLAYER_POINTER(this->PartyMember[i])->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Magic skill cast will remain for %d minute(s).", duration / 60);
			}
		}
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(critical_damage_add + critical_rate_add);
}

void SkillHandler::SkillBuffStun()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	coord_type pos_x = this->GetCaster()->GetX() - (8 - (this->GetAngle() & 15));
	coord_type pos_y = this->GetCaster()->GetY() - (8 - ((this->GetAngle() & 240) >> 4));

	int32 fangle = this->GetCaster()->GetAngle(pos_x, pos_y);

	this->GetCaster()->SkillAngleCalculate(fangle, 1.5f, 6.0f, 1.5f, 0.0f, false);

	this->GetCaster()->KillCountReset();

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !mTargetSec->IsPlayer() )
			continue;

		if ( Util::Distance(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > 4 )
			continue;

		if ( !this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
			continue;

		mTargetSec->ToPlayer()->AddBuff(BUFF_STUN, 10, 0, this->GetCaster());
		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBuffCancelStun()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	this->GetCaster()->KillCountReset();

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !mTargetSec->IsPlayer() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		mTargetSec->ToPlayer()->RemoveBuff(this->GetSkillInfo()->GetBuffIcon());

		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBuffSwellMana()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetCaster()->IsPlayer() )
		return;

	this->GetCaster()->KillCountReset();

	if ( this->GetCaster()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), 50) )
	{
		this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
			BuffEffect(BUFF_OPTION_MP_MUL, 50), 60, 0, this->GetCaster());
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !mTargetSec->IsPlayer() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		if ( !mTargetSec->ToPlayer()->GetGuildID() || mTargetSec->ToPlayer()->GetGuildID() != this->GetCaster()->ToPlayer()->GetGuildID() )
			continue;

		if ( mTargetSec->ToPlayer()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), 50) )
		{
			mTargetSec->ToPlayer()->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
				BuffEffect(BUFF_OPTION_MP_MUL, 50), 60, 0, this->GetCaster());
		}

		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBuffInvisibility()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() || !this->GetTarget()->IsPlayer() )
		return;

	this->GetCaster()->KillCountReset();

	this->GetTarget()->AddBuff(BUFF_TRANSPARENCY, 60, 0, this->GetCaster());

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillBuffCancelInvisibility()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	this->GetCaster()->KillCountReset();
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	Object* mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !mTargetSec->IsPlayer() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		mTargetSec->ToPlayer()->RemoveBuff(BUFF_TRANSPARENCY);
		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBuffCancelMagic()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	this->GetCaster()->KillCountReset();
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	Object* mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !mTargetSec->IsPlayer() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		mTargetSec->ToPlayer()->CancelAleEffect(ALE_DAMAGE);
		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBuffInfinityArrow()
{
	if ( this->GetCaster()->HasBuffGroup(this->GetSkillInfo()->GetBuffIcon()) )
	{
		return;
	}
		
	int32 add_damage_percent = this->GetCaster()->MagicGetValue(441);
	int32 duration = 1200;

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INFINITY_ARROW_DAMAGE, add_damage_percent), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillBuffDamageReflection()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
	{
		return;
	}

	if ( this->GetCaster()->IsPlayer() && this->GetTarget()->IsPlayer() )
	{
		if ( this->GetCaster()->ToPlayer()->GetHelper()->IsStarted() )
		{
			if ( this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID() )
			{
				return;
			}
		}
	}

	int32 add_reflect = 30 + (this->GetCaster()->GetEnergy() / 42);
	int32 duration = 30 + (this->GetCaster()->GetEnergy() / 25);

	if (add_reflect > sGameServer->GetSkillReflectLimit())
	{
		add_reflect = sGameServer->GetSkillReflectLimit();
	}

	if ( this->GetTarget()->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_reflect) )
	{
		this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_DAMAGE_REFLECTION_RATE, add_reflect), duration, 0, this->GetCaster(), true);
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	}
	else
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), false);
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_reflect);
}

void SkillHandler::SkillBuffBerserker()
{
	int32 value_damage = (this->GetCaster()->GetEnergy() / 30);
	int32 value_defense = (this->GetCaster()->GetEnergy() / 60);
	int32 value_speed = (this->GetCaster()->GetEnergy() / 200) + this->GetCaster()->MagicGetValue(470);
	int32 duration = WEEK;

	if (this->GetSkill()->GetSkill() == 470)
	{
		value_defense = 0;
	}

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_BERSERKER_DAMAGE, value_damage), BuffEffect(BUFF_OPTION_BERSERKER_DEFENSE, value_defense), BuffEffect(BUFF_OPTION_INCREASE_ATTACK_SPEED, value_speed), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillBuffDarkness()
{
	int32 value_decrease = (this->GetCaster()->GetEnergy() / 60);
	int32 duration = WEEK;

	if (this->GetSkill()->GetSkill() == 771)
	{
		value_decrease = 0;
	}

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_DARKNESS_DECREASE, value_decrease), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillBuffSleep()
{
	if (!this->GetTarget())
	{
		return;
	}

	if (!this->GetTarget()->IsPlaying() || !this->GetTarget()->IsLive())
	{
		return;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (this->GetTarget()->ResistElement(sSkillMgr->GetSkillAttribute(this->GetSkillInfo()->GetBaseSkill())))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (!this->GetCaster()->AttackAllowed(this->GetTarget()))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		if (sGameServer->GetSkillSleepReuseTime() > 0 && !pPlayer->GetTimer(PLAYER_TIMER_SLEEP_REUSE)->Elapsed(sGameServer->GetSkillSleepReuseTime()))
		{
			this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
			return;
		}
	}

	int32 success_rate = 0;
	int32 duration = 0;

	if (!this->GetTarget()->IsPlayer())
	{
		success_rate = 20 + (this->GetCaster()->GetEnergy() / 30) + (this->GetCaster()->GetBookCurseDamage() / 6);
		duration = 5 + (this->GetCaster()->GetEnergy() / 100) + (this->GetTarget()->GetTotalLevel() / 20);
	}
	else
	{
		success_rate = 15 + (this->GetCaster()->GetEnergy() / 37) + (this->GetCaster()->GetBookCurseDamage() / 6);
		duration = 4 + (this->GetCaster()->GetEnergy() / 250) + ((this->GetCaster()->GetTotalLevel() - this->GetTarget()->GetTotalLevel()) / 100);
	}

	if (success_rate > sGameServer->GetSkillSleepLimit())
	{
		success_rate = sGameServer->GetSkillSleepLimit();
	}

	if (duration > sGameServer->GetSkillSleepLimitTime())
	{
		duration = sGameServer->GetSkillSleepLimitTime();
	}

	success_rate += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_SLEEP_IMPROVED);
	
	if (duration <= 0 || success_rate <= 0)
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (success_rate > 100)
	{
		success_rate = 100;
	}

	if (!roll_chance_i(success_rate))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (this->GetTarget()->IsDebuffInmune(false))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (Monster* pMonster = this->GetTarget()->ToCreature())
	{
		if (roll_chance_f(pMonster->GetFloatData(UNIT_FLOAT_RESIST_STUN)))
		{
			this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0, MISS_TYPE_RESIST);
			return;
		}
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetTarget()->AddBuff(BUFF_SLEEP, BuffEffect(BUFF_OPTION_SLEEP, duration), duration, 0, this->GetCaster());
}
	
void SkillHandler::SkillBuffWeakness()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	int32 duration = this->GetSkillInfo()->GetCooldown() + (this->GetCaster()->GetMagicTreeLevel(459) * IN_MILLISECONDS);

	Monster* pMonster = sObjectMgr->MonsterTryAdd(821, this->GetCaster()->GetWorldId());

	if (!pMonster)
	{
		return;
	}

	pMonster->SetWorldId(this->GetCaster()->GetWorldId());
	pMonster->SetBasicLocation(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetCaster()->GetX(), this->GetCaster()->GetY());
	pMonster->SetInstance(this->GetCaster()->GetInstance());
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
	pMonster->SetDespawnTime(duration);
	pMonster->SetDespawnTick(GetTickCount());
	pMonster->SetMoveDistance(0);
	pMonster->SetDirection(Random(8));

	pMonster->AddAdditionalDataPtr(0, pPlayer);
	pMonster->AddAdditionalDataInt(0, 0);
	pMonster->AddAdditionalDataInt(1, this->GetSkill()->GetSkill());

	pMonster->AddToWorld();

	if (pPlayer->GetSummonerDebuff())
	{
		pPlayer->GetSummonerDebuff()->Remove();
	}

	pPlayer->SetSummonerDebuff(pMonster);
}
	
void SkillHandler::SkillBuffInnovation()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	int32 duration = this->GetSkillInfo()->GetCooldown() + (this->GetCaster()->GetMagicTreeLevel(460) * IN_MILLISECONDS);

	Monster* pMonster = sObjectMgr->MonsterTryAdd(822, this->GetCaster()->GetWorldId());

	if (!pMonster)
	{
		return;
	}

	pMonster->SetWorldId(this->GetCaster()->GetWorldId());
	pMonster->SetBasicLocation(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetCaster()->GetX(), this->GetCaster()->GetY());
	pMonster->SetInstance(this->GetCaster()->GetInstance());
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
	pMonster->SetDespawnTime(duration);
	pMonster->SetDespawnTick(GetTickCount());
	pMonster->SetMoveDistance(0);
	pMonster->SetDirection(Random(8));

	pMonster->AddAdditionalDataPtr(0, pPlayer);
	pMonster->AddAdditionalDataInt(0, 1);
	pMonster->AddAdditionalDataInt(1, this->GetSkill()->GetSkill());

	pMonster->AddToWorld();

	if (pPlayer->GetSummonerDebuff())
	{
		pPlayer->GetSummonerDebuff()->Remove();
	}

	pPlayer->SetSummonerDebuff(pMonster);
}

void SkillHandler::SkillBuffExpansionOfWizardy()
{
	auto pPlayer = this->GetCaster()->ToPlayer();
	if (!pPlayer)
	{
		return;
	}
		
	int32 min_wizard = (pPlayer->GetOnlyEquipmentStat(ENERGY) / 9) * 2 / 10;
	int32 max_wizard = (pPlayer->GetOnlyEquipmentStat(ENERGY) >> 2) * this->GetCaster()->MagicGetValue(380) / 100;
	float critical_damage_rate = this->GetCaster()->MagicGetValue(383);
	int32 duration = 1800;
	
	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
		BuffEffect(BUFF_OPTION_MIN_WIZARDY, min_wizard), 
		BuffEffect(BUFF_OPTION_MAX_WIZARDY, max_wizard), 
		BuffEffect(BUFF_OPTION_INCREASE_CRITICAL_DAMAGE_RATE, critical_damage_rate), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillBuffRecovery()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
		return;

	if ( this->GetCaster()->IsPlayer() && this->GetTarget()->IsPlayer() )
	{
		if ( this->GetCaster()->ToPlayer()->GetHelper()->IsStarted() )
		{
			if ( this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID() )
				return;
		}
	}

	if ( this->GetTarget()->PowerGet(POWER_SHIELD) >= this->GetTarget()->PowerGetTotal(POWER_SHIELD) )
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), false);
		return;
	}
	
	int32 value = (this->GetCaster()->GetEnergy() / 4) + this->GetCaster()->GetTotalLevel();

	this->GetTarget()->PowerIncrease(POWER_SHIELD, value, true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(value);
}

void SkillHandler::SkillBuffRedArmorIgnore()
{
	int32 add_ignore = 3 + ((this->GetCaster()->GetEnergy() - this->GetSkillInfo()->GetRequiredStat(ENERGY)) / 100);
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 5);

	if (add_ignore > 10)
	{
		add_ignore = 10;
	}
	
	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_IGNORE_RATE, add_ignore), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(add_ignore);
}

void SkillHandler::SkillBuffFitness()
{
	GetPartyMembers();

	int32 effect = 30 + ((this->GetCaster()->GetEnergy() - this->GetSkillInfo()->GetRequiredStat(ENERGY)) / 10);
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 5);

	effect += this->GetCaster()->MagicGetValue(SKILL_TREE_FITNRESS_STRENGTHENER);
	//effect += this->GetCaster()->MagicGetValue(1114, true);

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			if ( this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), effect) )
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_VITALITY, effect), duration, 0, this->GetCaster(), true);
				this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill(), true);
			}
		}
	}
}

void SkillHandler::SkillBuffDefenseSuccessRate()
{
	GetPartyMembers();

	int32 add_defense_success_rate = 10 + (this->GetCaster()->GetEnergy() / 10);
	int32 add_defense = this->GetCaster()->MagicGetValue(572);
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 5);

	if (add_defense_success_rate > 100)
	{
		add_defense_success_rate = 100;
	}

	add_defense_success_rate += this->GetCaster()->MagicGetValue(569);

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			if ( this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_defense_success_rate, add_defense) )
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
					BuffEffect(BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE, add_defense_success_rate), 
					BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, add_defense), duration, 0, this->GetCaster(), true);
				this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill(), true);
			}
		}
	}
}

void SkillHandler::SkillAttackToLocation()
{
	Object * mTargetSec;

	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			this->GetCaster()->AddDelayed(DELAYED_ATTACK, Random<uint32>(500), mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, (i - 1));
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackTwistingSlash()
{
	Object * mTargetSec;
	int32 target_count = 0;

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (target_count < 10 || RANDOM(2))
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, this->IsCombo() ? 1 : 0);

			target_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackRagefulBlow()
{
	Object * mTargetSec;
	int32 target_count = 0;

	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (target_count < 10 || RANDOM(2))
		{
			for (int32 i = 1; i < (hit_count + 1); ++i)
			{
				this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, this->IsCombo() ? 1 : 0, (i - 1));
			}

			target_count++;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillAttackDeathStab()
{
	if (!this->GetTarget())
	{
		return;
	}

	int32 distance = Util::Distance(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()->GetX(), this->GetTarget()->GetY());

	Object * mTargetSec;

	float angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
	this->GetCaster()->SkillAngleCalculate(angle, 1.5f, SkillHandler::GetSkillRadio(this->GetCaster(), this->GetSkill()->GetSkill()) + distance, 1.0f, 0.0f + distance, false);

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	int32 hit_count = 3;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	for (int32 i = 0; i < hit_count; ++i)
	{
		VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

			if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
			{
				continue;
			}

			if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
			{
				continue;
			}

			if (i == 0 || RANDOM(3) == 0 || (hit_count > 3 && i >= 3))
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, this->IsCombo() ? 1 : 0);
			}

		VIEWPORT_CLOSE
	}
}

void SkillHandler::SkillAttackPowerSlash()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);
	}

	Object * mTargetSec;
	int32 hit_count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (hit_count < 5 || RANDOM(2))
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
			hit_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFireBurst()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;

	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (mTargetSec != this->GetTarget())
		{
			if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
			{
				continue;
			}

			if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
			{
				continue;
			}
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			this->GetCaster()->AddDelayed(DELAYED_ATTACK, RANDOM(300) + 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, i - 1);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackDarkHorse()
{
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackChaoticDiseier()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetAngle(), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}

	Object * mTargetSec;
	int32 target_count = 0;

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (target_count < 8 || RANDOM(2))
		{
			for (int32 i = 1; i < (hit_count + 1); ++i)
			{
				this->GetCaster()->AddDelayed(DELAYED_ATTACK, 200, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, i - 1);
			}
			target_count++;
		}

		if (this->GetTarget() == mTargetSec)
		{
			this->GetCaster()->AddDelayed(DELAYED_ATTACK, 300, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFlameStrike()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 2.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 5.0f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetAngle(), 2.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 5.0f, 0.0f);
	}

	Object * mTargetSec;
	int32 hit_count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (hit_count < 8 || RANDOM(2))
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
			hit_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackStrikeOfDestruction()
{
	Object * mTargetSec;
	int32 hit_count = 0;
	uint8 allow_attack = 0;
	int32 damage_number = 1;
	Player* pPlayer = this->GetCaster()->ToPlayer();
	if (pPlayer)
	{
		if (pPlayer->GetSkillTreeSkill(807))
		{
			damage_number += 1;
		}

		damage_number += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		allow_attack = (hit_count <= 4) ? 1 : hit_count > 8 ? 0 : RANDOM(2);

		if (allow_attack)
		{
			if (damage_number == 1)
			{
				this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
			}
			else
			{
				for (int32 i = 1; i < (damage_number + 1); ++i)
				{
					this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), i);
				}
			}
			hit_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackEvilSpirit()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	Object * mTargetSec;
	int32 count = 0;

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i - 1);
		}
		++count;

		if (count >= 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackHellFire()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	Object * mTargetSec;
	int32 count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
		++count;

		if (count > 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillAttackInferno()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	Object * mTargetSec;
	int32 count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
		++count;

		if (count > 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFlame()
{
	if (this->GetTarget())
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
	}

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 0; i < 3; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);
		}
		++count;

		if (count > 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackTripleShot()
{
	if (this->GetTarget())
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
	}

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);

	vec3_t Angle; //Good
	Vector(0.f, 6.0f, 0.f, Angle); //3,4,5 Good

	vec3_t p[4]; //Good size OK

	Vector(0.f, 0.f, 40.0f, p[0]); //Good
	Vector(0.f, 0.f, 20.0f, p[1]); //Good
	Vector(0.f, 0.f, 340.0f, p[2]); //Good
	Vector(0.f, 0.f, 320.0f, p[3]); //Good

	float Matrix[3][4]; //Good

	vec3_t vFrustrum[5]; //Good

	AngleMatrix(p[0], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

	AngleMatrix(p[1], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

	Vector(0.f, 6.0f, 0.f, vFrustrum[2]);//36,37,38

	AngleMatrix(p[2], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

	AngleMatrix(p[3], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

	//Vector(0.f, 0.f, pPlayer->GetHelper()->GetAttackAngle(), p[0]); //15, 16, 17 Good
	Vector(0.f, 0.f, (pPlayer->GetHelper()->GetAttackAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good

	AngleMatrix((float*)p, Matrix); //Good

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int Value1[3];
	int Value2[3];
	int Value3[3];

	vec3_t vFrustrum2[3];

	for (int i = 0; i < 3; i++) //loc70 Good
	{
		VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

		Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
		Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
	}

	Object * mTargetSec;
	int32 count = 0;

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		for (int32 j = 0; j < 3; ++j)
		{
			Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

			if (Value3[j] > -3 && Value3[j] < 3)
			{
				for (int32 i = 1; i < (hit_count + 1); ++i)
				{
					this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i - 1);
				}
				++count;
			}
		}

		if (count >= 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackMultiShot()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);
	}

	vec3_t Angle; //Good
	Vector(0.f, 6.0f, 0.f, Angle); //3,4,5 Good

	vec3_t p[4]; //Good size OK

	Vector(0.f, 0.f, 40.0f, p[0]); //Good
	Vector(0.f, 0.f, 20.0f, p[1]); //Good
	Vector(0.f, 0.f, 340.0f, p[2]); //Good
	Vector(0.f, 0.f, 320.0f, p[3]); //Good

	float Matrix[3][4]; //Good

	vec3_t vFrustrum[5]; //Good

	AngleMatrix(p[0], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

	AngleMatrix(p[1], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

	Vector(0.f, 6.0f, 0.f, vFrustrum[2]);//36,37,38

	AngleMatrix(p[2], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

	AngleMatrix(p[3], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		Vector(0.f, 0.f, (pPlayer->GetHelper()->GetAttackAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good
	}
	else
	{
		Vector(0.f, 0.f, (this->GetAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good
	}

	AngleMatrix((float*)p, Matrix); //Good

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int Value1[5];
	int Value2[5];
	int Value3[5];

	vec3_t vFrustrum2[5];

	for (int i = 0; i < 5; i++) //loc70 Good
	{
		VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

		Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
		Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
	}

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		for (int32 j = 0; j < 5; ++j)
		{
			Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

			if (Value3[j] > -5 && Value3[j] < 5)
			{
				for (int32 i = 1; i < (hit_count + 1); ++i)
				{
					this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i - 1);
				}
			}
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackLightningShock()
{
	Object * mTargetSec;

	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, i - 1);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackChainLightning()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		return;
	}

	Unit* pTargets[3] = { this->GetTarget(), nullptr, nullptr };
	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (mTargetSec->GetX() >= this->GetTarget()->GetX() - 1 && mTargetSec->GetX() <= this->GetTarget()->GetX() + 1 &&
			mTargetSec->GetY() >= this->GetTarget()->GetY() - 1 && mTargetSec->GetY() <= this->GetTarget()->GetY() + 1 &&
			mTargetSec->IsCreature())
		{
			pTargets[1] = mTargetSec->ToUnit();

			if (pTargets[2])
			{
				break;
			}
		}
		else if (mTargetSec->GetX() >= this->GetTarget()->GetX() - 2 && mTargetSec->GetX() <= this->GetTarget()->GetX() + 2 &&
			mTargetSec->GetY() >= this->GetTarget()->GetY() - 2 && mTargetSec->GetY() <= this->GetTarget()->GetY() + 2 &&
			mTargetSec->IsCreature())
		{
			pTargets[2] = mTargetSec->ToUnit();

			if (pTargets[1])
			{
				break;
			}
		}

	VIEWPORT_CLOSE

	if (!pTargets[2])
	{
		pTargets[2] = pTargets[1];
	}

	if (!pTargets[1])
	{
		pTargets[1] = pTargets[0];
		pTargets[2] = pTargets[1];
	}

	uint8 buffer[255];
	POINTER_PCT(CHAIN_SKILL_HEAD, head, buffer, 0);
	POINTER_PCT(CHAIN_SKILL_BODY, body, buffer, sizeof(CHAIN_SKILL_HEAD));
	uint16 size = sizeof(CHAIN_SKILL_HEAD) + sizeof(CHAIN_SKILL_BODY) * 3;
	head->h.set(HEADCODE_EVENT_MISC, SUBCODE_CHAIN_LIGHTNING, size);
	head->index = cast(uint16, this->GetCaster()->GetEntry());
	head->skillH = SET_NUMBERH(this->GetSkill()->GetSkill());
	head->skillL = SET_NUMBERL(this->GetSkill()->GetSkill());
	head->count = 3;

	for (uint8 i = 0; i < 3; i++)
	{
		body[i].target = pTargets[i] ? pTargets[i]->GetEntry() : -1;
	}

	this->GetCaster()->sendPacket(buffer, size);
	this->GetCaster()->sendPacket_viewport(buffer, size);

	for (uint8 i = 0; i < 3; i++)
	{
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 200 + (200 * i), pTargets[i], this->GetSkill()->GetSkill(), 0, 0, i + 1);
	}
}

void SkillHandler::SkillAttackDragonLore()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 5;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; i++)
		{
			if ((i % (hit_count - 1)) == 1)
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true, 0, 0, 0, i);
			}
			else
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
			}
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackDrainLife()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		return;
	}

	this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 700, this->GetTarget(), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillAttackNova()
{
	if (!this->IsCombo())
	{
		if (!this->GetCaster()->GetNova()->IsRunning())
		{
			return;
		}
	}
	else
	{
		if (this->GetCaster()->GetNova()->IsRunning())
		{
			return;
		}

		this->GetCaster()->GetNova()->Start();
		this->GetCaster()->GetNova()->SetSkill(this->GetSkill()->GetSkill());
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_NOVA_CHARGE);
		return;
	}

	this->GetCaster()->GetNova()->SetRunning(false);
	this->GetCaster()->GetNova()->SetTick(0);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 600, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE

		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillAttackFenrir()
{
	if (this->GetCaster()->IsPlayer() && this->GetCaster()->GetLevel() < 300)
		return;

	Object * mTargetSec;
	int32 hit_count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (hit_count < 5 || mTargetSec == this->GetTarget())
		{
			this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 300, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
			hit_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFlameOfEvil()
{
	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackDopplegangerSuicide()
{
	Object * mTargetSec;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

	VIEWPORT_CLOSE

	this->GetCaster()->Kill();
}

void SkillHandler::SkillAttackExplosion()
{
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if (!this->CheckSkillRange(this->GetAttackX(), this->GetAttackY(), this->GetCaster()))
	{
		return;
	}

	int32 hit_count = 4;

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

		--hit_count;

		if (hit_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillAttackRequiem()
{
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if ( !this->CheckSkillRange(this->GetAttackX(), this->GetAttackY(), this->GetCaster()) )
	{
		return;
	}

	int32 hit_count = 4;

	Object * mTargetSec;
	
	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
			continue;

		if ( !this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

		--hit_count;

		if (hit_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackPollution()
{
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if (!this->CheckSkillRange(this->GetAttackX(), this->GetAttackY(), this->GetCaster()))
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4;
	uint8 allow_attack = 0;

	if (this->GetSkill()->GetSkill() == 778)
	{
		hit_count += 1;
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		continue;

	if (!this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false);

	--hit_count;

	if (hit_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackGiganticStorm()
{
	Object * mTargetSec;
	int32 target_count = 0;
	uint8 allow_attack = 0;

	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		allow_attack = (target_count <= 8) ? 1 : target_count > 12 ? 0 : RANDOM(2);

		if (allow_attack)
		{
			for (int32 i = 1; i < (hit_count + 1); ++i)
			{
				this->GetCaster()->AddDelayed(DELAYED_ATTACK, 400, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, i - 1);
			}
			target_count++;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackForce()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	if (!this->GetCaster()->IsForceUpgrade())
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
		return;
	}

	Object * mTargetSec;
	int32 loop_count = 0;
	int32 angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
	this->GetCaster()->SkillAngleCalculate(angle, 1.5f, this->GetSkillInfo()->GetEffectRange(), 1.0f, 0.0f, false);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (loop_count == 0 || RANDOM(3) == 0)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());
		}

		loop_count++;

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackMultiSingle()
{
	if (!this->GetTarget())
	{
		return;
	}

	if (!this->GetCaster()->AttackAllowed(this->GetTarget(), true))
	{
		return;
	}

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		if (this->GetSkillInfo()->GetBaseSkill() == SKILL_UPPER_BEAST ||
			this->GetSkillInfo()->GetBaseSkill() == SKILL_CHAIN_DRIVE)
		{
			this->GetCaster()->NewMagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
		}
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		return;
	}

	int32 hit_count = 0;

	if (this->GetSkillInfo()->GetBaseSkill() == SKILL_LARGE_RING_BLOWER)
	{
		hit_count = 5;
	}
	else if (this->GetSkillInfo()->GetBaseSkill() == SKILL_UPPER_BEAST)
	{
		hit_count = 3;
	}
	else if (this->GetSkillInfo()->GetBaseSkill() == SKILL_CHAIN_DRIVE)
	{
		hit_count = 9;
	}

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		if ((i % (hit_count - 1)) == 1)
		{
			this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true, 0, 0, 0, i);
		}
		else
		{
			this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, 0, 0, i);
		}
	}
}

void SkillHandler::SkillAttackDarkSide()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		pPlayer->NewMagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	}
	else
	{
		//pPlayer->NewMagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	}

	int32 hit_count = 3;
	int32 target_count = 5;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		target_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	if (target_count > 10)
	{
		target_count = 10;
	}

	DARK_SIDE_RESULT pMsg(SKILL_DARK_SIDE);

	pPlayer->GetSkillDarkSideTargets(this->GetTarget() && this->GetTarget()->GetEntry() == 0 ? nullptr : this->GetTarget(), this->GetSkillInfo(), pMsg.target, target_count);

	if (pMsg.target[0] == 10000)
	{
		return;
	}

	pPlayer->SEND_PCT(pMsg);

	for (int32 n = 0; n < target_count; ++n)
	{
		if (pMsg.target[n] != uint16(-1))
		{
			if (Unit* pTarget = sObjectMgr->GetUnit(pMsg.target[n]))
			{
				for (int32 i = 1; i < hit_count; ++i)
				{
					if ((i % (hit_count - 1)) == 1)
					{
						this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 150, pTarget, this->GetSkill()->GetSkill(), 0, 0, 0, 1);
					}
					else
					{
						this->GetCaster()->AddDelayed(DELAYED_ATTACK, 150, pTarget, this->GetSkill()->GetSkill(), 0, 0, 0, 2);
					}
				}
				//this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, (150+(n*150)), pTarget, this->GetSkill()->GetSkill(), 0, 0, 0, 1);
			}
		}
	}
}

void SkillHandler::SkillAttackCharge()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
}
	
void SkillHandler::SkillAttackPhoenixShot()
{
	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	int32 hit_count = 5;

	if ( Player* pPlayer = this->GetCaster()->ToPlayer() )
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	if ( this->GetCaster()->AttackAllowed(this->GetTarget(), true) )
	{
		for( int32 i = 1; i < hit_count; i++ )
		{
			if( (i%(hit_count-1)) == 1  )
			{
				this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true, 0, 0, 0, i);
			}
			else
			{
				this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, 0, 0, i);
			}
		}
	}

	Object* mTargetSec = nullptr;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(),mTargetSec)

		if ( mTargetSec == this->GetTarget() )
			continue;

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) )
			continue;

		if ( !this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}

		for(int32 i = 1; i < hit_count; i++ )
		{
			if( (i%(hit_count-1)) == 1  )
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true, 0, 0, 0, i);
			}
			else
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
			}
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBloodStorm()
{
	int32 count = 0;

	Object* mTargetSec = nullptr;

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 1000, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, this->IsCombo() ? 1 : 0);
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 1500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0);

		if (++count >= 10)
		{
			break;
		}

	VIEWPORT_CLOSE
}
		
void SkillHandler::SkillCure()
{
	this->SkillBuffFree();

	if ( !this->GetTarget() )
		return;

	if (roll_chance_i(sGameServer->GetSkillCureBuffRate()))
	{
		this->GetTarget()->ClearDebuff(1);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill(), true);
}
	
void SkillHandler::SkillPartyHeal()
{
	if (!this->GetCaster()->IsPlayer())
	{
		return;
	}

	Party* pParty = this->GetCaster()->ToPlayer()->GetParty();

	if (!pParty)
	{
		return;
	}

	if (!this->GetTarget())
	{
		this->SetTarget(this->GetCaster());
	}

	if (!this->GetTarget()->IsPlayer())
	{
		return;
	}

	if (this->GetCaster()->ToPlayer()->GetPartyID() != this->GetTarget()->ToPlayer()->GetPartyID())
	{
		return;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		return;
	}

	this->GetPartyMembers();

	if (this->party_member_count > 3)
		this->party_member_count = 3;

	int32 value = 6 + (this->GetCaster()->GetEnergy() / 6);

	value += (value * this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_PARTY_HEAL_IMPROVED)) / 100;
	
	uint8 buffer[255];
	POINTER_PCT(CHAIN_SKILL_HEAD, head, buffer, 0);
	POINTER_PCT(CHAIN_SKILL_BODY, body, buffer, sizeof(CHAIN_SKILL_HEAD));
	head->index = this->GetCaster()->GetEntry();
	head->skillH = SET_NUMBERH(this->GetSkill()->GetSkill());
	head->skillL = SET_NUMBERL(this->GetSkill()->GetSkill());
	head->count = 0;

	for (uint8 i = 0; i < this->party_member_count; i++)
	{
		if (this->PartyMember[i])
		{
			this->GetCaster()->AddDelayed(DELAYED_LIFE_RECOVER, 200 + (200 * head->count), this->PartyMember[i], ((value * (100 - (head->count * 20))) / 100), 0, 0, 0);

			body[head->count].target = this->PartyMember[i]->GetEntry();
			++head->count;
		}
	}

	head->h.set(HEADCODE_EVENT_MISC, SUBCODE_CHAIN_SKILL, sizeof(CHAIN_SKILL_HEAD)+(head->count * sizeof(CHAIN_SKILL_BODY)));

	this->GetCaster()->sendPacket(buffer, head->h.get_size());
	this->GetCaster()->sendPacket_viewport(buffer, head->h.get_size());
}

void SkillHandler::SkillBless()
{
	if ( !this->GetCaster()->IsPlayer() )
	{
		return;
	}

	if ( this->GetTarget() )
	{
		if ( !this->GetTarget()->IsPlayer() )
		{
			this->SetTarget(nullptr);
		}
		else
		{
			if ( this->GetTarget()->ToPlayer()->GetPartyID() != this->GetCaster()->ToPlayer()->GetPartyID() )
			{
				this->SetTarget(nullptr);
			}
		}
	}

	if (this->GetCaster()->GetWorldId() == WORLD_LAST_MAN_STANDING || CC_MAP_RANGE(this->GetCaster()->GetWorldId()))
	{
		this->SetTarget(nullptr);
	}

	if ( !this->GetTarget() )
	{
		this->SetTarget(this->GetCaster());
	}

	if( !this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()) )
	{
		return;
	}

	int32 value = this->GetCaster()->GetEnergy() / 100;

	value += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_BLESS_IMPROVED);

	if ( Player* pPlayer = this->GetCaster()->ToPlayer() )
	{
		value += pPlayer->GetAncientEffect(ANCIENT_EFFECT_ENHANCED_BLESS_BUFF);
	}

	int32 duration = 300;

	if ( sGameServer->IsSkillBlessStrenghtenerFix() )
	{
		this->GetTarget()->AddBuff(142, BuffEffect(BUFF_OPTION_INCREASE_ALL_STATS, value), duration, 0, this->GetCaster());
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), SKILL_BLESS);
	}
	else
	{
		this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_ALL_STATS, value), duration, 0, this->GetCaster());
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	}
}
	
void SkillHandler::SkillBlind()
{
	if (!this->GetTarget())
	{
		return;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), this->GetTarget()))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (!this->GetCaster()->AttackAllowed(this->GetTarget()))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	int32 rate = sGameServer->GetSkillBlindAdd() + (this->GetCaster()->GetEnergy() / sGameServer->GetSkillBlindDiv());

	if (!roll_chance_i(rate))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	if (this->GetTarget()->IsDebuffInmune(false))
	{
		this->GetCaster()->MissSend(this->GetTarget(), this->GetSkill(), true, 0);
		return;
	}

	this->GetTarget()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_DECREASE_ATTACK_SUCCESS_RATE, sGameServer->GetSkillBlindValue()), sGameServer->GetSkillBlindTime(), 0, this->GetCaster());

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
}
	
void SkillHandler::SkillEarthPrison()
{
	int32 count = 0;

	Object* mTargetSec = nullptr;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 1200, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 1500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());

		if (++count >= 10)
		{
			break;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillIronDefense()
{
	if ( !this->GetCaster()->IsPlayer() || this->GetCaster() != this->GetTarget() )
	{
		return;
	}

	int32 value = 0;
	
	value += this->GetCaster()->MagicGetValue(SKILL_IRON_DEFENSE_1);
	value += this->GetCaster()->MagicGetValue(SKILL_IRON_DEFENSE_2);

	value += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_IRON_DEFENSE_IMPROVED_1);
	value += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_IRON_DEFENSE_IMPROVED_2);

	int32 duration = sGameServer->GetSkillIronDefenseDuration();

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
		BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, value), 
		BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_LIFE, value), duration, 0, this->GetCaster());

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}
	
void SkillHandler::SkillBloodHowling()
{
	if ( !this->GetCaster()->IsPlayer() || this->GetCaster() != this->GetTarget() )
	{
		return;
	}

	int32 duration = sGameServer->GetSkillBloodHowlingDuration();

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), duration, 0, this->GetCaster());
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillSpinStep()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
}

void SkillHandler::SkillSpinStepExplosion(int32 damage)
{
	if (!this->GetTarget())
	{
		return;
	}

	int32 explosion_damage = (damage * 0.7f) * (this->GetCaster()->GetAgility() / 10 + 97 + this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_RAGE_DAMAGE)) / 100;

	explosion_damage += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_SPIN_STEP_MASTERY);

	if (explosion_damage < 1)
	{
		return;
	}

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (this->GetTarget() == mTargetSec)
		{
			continue;
		}

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), nullptr, false, explosion_damage);
		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), SKILL_SPIN_STEP_EXPLOSION);

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillCircleShield()
{
	int32 duration = sGameServer->GetSkillCircleShieldDuration();
	int32 reduce_effect = sGameServer->GetSkillCircleShieldReduceAG() + this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_CIRCLE_SHIELD_IMPROVED);

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_CIRCLE_SHIELD, reduce_effect), duration, 0, this->GetCaster());
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}
	
void SkillHandler::SkillObsidian()
{
	if ( !this->GetCaster()->IsPlayer() )
	{
		return;
	}

	this->GetPartyMembers();

	int32 value = this->GetCaster()->GetStrength() / 20;
	int32 duration = 120;

	value += (this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_OBSIDIAN_IMPROVED) * value) / 100;

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE_SKILL, value), duration, 0, this->GetCaster());
			this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill());
		}
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(value);
}
		
void SkillHandler::SkillMagicPin()
{
	if (!this->GetTarget())
	{
		return;
	}

	int32 hit_count = 4;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		if ((i % (hit_count - 1)) == 1)
		{
			this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, (i * 100), this->GetTarget(), this->GetSkill()->GetSkill(), 0, 0, 0, i);
		}
		else
		{
			this->GetCaster()->AddDelayed(DELAYED_ATTACK, (i * 100), this->GetTarget(), this->GetSkill()->GetSkill(), 0, 0, 0, i);
		}
	}
}

void SkillHandler::SkillMagicPinExplosion(int32 damage)
{
	if ( !this->GetTarget() )
	{
		return;
	}

	if (!roll_chance_i(sGameServer->GetSkillMagicPinExplosionRate()))
	{
		return;
	}

	/*int32 explosion_damage = (damage * sGameServer->GetSkillMagicPinExplosionDamage()) / 100;

	if ( explosion_damage < 1 )
	{
		return;
	}

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
		{
			continue;
		}

		if ( !this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}
			
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 100, mTargetSec->ToUnit(), 0, explosion_damage);*/
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), SKILL_MAGIC_PIN_EXPLOSION);

	//VIEWPORT_CLOSE
}
		
void SkillHandler::SkillClash()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
}
		
void SkillHandler::SkillHarshStrike()
{
	if ( !this->GetTarget() )
	{
		return;
	}

	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true, 0, 0, 0, 1);
	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, 0, 0, 2);

	if (this->GetCaster()->GetMagicTreeLevel(691) > 0)
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, 0, 0, 3);
	}
}
		
void SkillHandler::SkillShiningPeak()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!this->GetTarget())
	{
		return;
	}

	int32 hit_count = 3 + pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 n = 1; n < (hit_count + 1); ++n)
		{
			this->GetCaster()->AddDelayed(DELAYED_MAGIC_ATTACK, 100, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, n);
		}

	VIEWPORT_CLOSE
}
		
void SkillHandler::SkillWrath()
{
	if ( !this->GetCaster()->IsPlayer() )
	{
		return;
	}

	int32 duration = 15 + this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_WRATH_IMPROVED);
	
	if ( this->GetSkill()->GetSkill() == SKILL_TREE_ADD_WRATH_MASTERY )
	{
		this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_WRATH_DAMAGE, 50), duration, 0, this->GetCaster());
	}
	else
	{
		this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_WRATH_DAMAGE, 50), BuffEffect(BUFF_OPTION_WRATH_DEFENSE, 30), duration, 0, this->GetCaster());
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}
		
void SkillHandler::SkillBreche()
{
	Object * mTargetSec;
	int32 target_count = 0;
	int32 hit_count = 1;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i - 1);
		}

		target_count++;

		if (target_count >= 4)
		{
			break;
		}

	VIEWPORT_CLOSE
}
		
void SkillHandler::SkillBurst()
{
	if ( !this->GetCaster()->IsPlayer() )
	{
		return;
	}

	float value = this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_BURST);

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_IGNORE_RATE, value), 15, 0, this->GetCaster());
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}
		
void SkillHandler::SkillArchangelsWill()
{
	this->GetPartyMembers();

	int32 add_attack_power = sGameServer->GetSkillArchangelWillDamage();
	int32 add_skill_damage = sGameServer->GetSkillArchangelWillSkillDamage();
	int32 duration = sGameServer->GetSkillArchangelWillDuration();

	for ( int32 i = 0; i < this->party_member_count; ++i )
	{
		if ( this->PartyMember[i] )
		{
			if ( this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), add_attack_power, add_skill_damage) )
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), 
					BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, add_attack_power), 
					BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE_SKILL, add_skill_damage), 
					duration, 0, this->GetCaster());

				this->GetCaster()->MagicAttackSend(ENTRY(this->PartyMember[i]), this->GetSkill()->GetSkill());
			}
		}
	}
}

void SkillHandler::SkillIllusion()
{
	if (this->GetCaster()->GetSummoned())
	{
		this->GetCaster()->SendSummonedHP(0, 60);
		this->GetCaster()->KillSummoned();
		return;
	}

	if (!this->GetCaster()->IsWorldFlag(WORLD_FLAG_ALLOW_SUMMON_MONSTER))
	{
		PLAYER_POINTER(this->GetCaster())->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to summon on this map.");
		return;
	}

	Monster *mSummoned = sObjectMgr->MonsterTryAdd(707, this->GetCaster()->GetWorldId());

	if (mSummoned)
	{
		mSummoned->SetSendType(MAIN_OBJECT_PLAYER);
		mSummoned->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		//mSummoned->SetDespawnTime(sGameServer->GetSkillIllusionDuration() * IN_MILLISECONDS);
		//mSummoned->SetDespawnTick(GetTickCount());
		//mSummoned->SetDespawnType(MONSTER_DESPAWN_TIME);
		mSummoned->SetMoveDistance(15);
		mSummoned->SetX(this->GetCaster()->GetX() + 1);
		mSummoned->SetY(this->GetCaster()->GetY() - 1);
		mSummoned->SetDirection(2);
		mSummoned->SetWorldId(this->GetCaster()->GetWorldId());
		mSummoned->SetInstance(this->GetCaster()->GetInstance());
		mSummoned->SetSummoner(this->GetCaster());
		mSummoned->SetScriptName("summon_illusion_ai");
		mSummoned->SetNameData("$owner's %s", mSummoned->GetName());
		this->GetCaster()->SetSummoned(mSummoned);

		mSummoned->SetScriptMaxPower(POWER_LIFE, this->GetCaster()->PowerGetTotal(POWER_LIFE));
		mSummoned->PowerSetMax(POWER_LIFE, this->GetCaster()->PowerGetTotal(POWER_LIFE));
		mSummoned->PowerSet(POWER_LIFE, this->GetCaster()->PowerGetTotal(POWER_LIFE));
		mSummoned->SetIntData(UNIT_INT_DEFENSE, this->GetCaster()->GetIntData(UNIT_INT_DEFENSE));
		mSummoned->SetIntData(UNIT_INT_DEFENSE_RATE, this->GetCaster()->GetIntData(UNIT_INT_DEFENSE_RATE));
		mSummoned->SetIntData(UNIT_INT_ATTACK_RATE, this->GetCaster()->GetIntData(UNIT_INT_ATTACK_RATE));

		mSummoned->AddToWorld();

		this->GetCaster()->SendSummonedHP(mSummoned->PowerGet(POWER_LIFE), mSummoned->PowerGetTotal(POWER_LIFE));
	}
}

void SkillHandler::SkillShadowStep()
{
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillEvasion()
{
	int32 duration = 7;
	int32 value = (this->GetCaster()->GetIntData(UNIT_INT_DEFENSE_RATE) * 50) / 100;

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE, value), duration, 0, this->GetCaster());
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillRush()
{
	if ( !this->GetTarget() )
	{
		return;
	}
	
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
	/*this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, this->IsCombo());

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}*/
}

void SkillHandler::SkillFireBlow()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	int32 hit_count = 4 + Random(2);

	if (this->IsCombo())
	{
		hit_count = 2;
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, this->IsCombo() ? 1 : 0, 0, i);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillSwordBlow()
{
	if (!this->GetTarget())
	{
		return;
	}

	std::set<uint16> targets;

	Object * mTargetSec;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	int32 hit_count = 4 + Random(2);

	if (this->IsCombo())
	{
		hit_count = 2;
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	if (this->IsCombo())
	{
		this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), SKILL_COMBO);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, this->IsCombo() ? 1 : 0, 0, i);
	}

	targets.insert(mTargetSec->GetEntry());

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SkillMeteorStrike()
{
	if ( !this->GetTarget() )
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4 + Random(2);

	if ( Player* pPlayer = this->GetCaster()->ToPlayer() )
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
		{
			continue;
		}

		if ( !this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()) ) 
		{
			continue;
		}

		for( int32 i = 1; i < hit_count; ++i )
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillMeteorStorm()
{
	Object * mTargetSec;

	int32 hit_count = 3 + Random(2);
	int32 total_count = 8;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

		--total_count;

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillDeathside()
{
	Object * mTargetSec;

	int32 hit_count = 5;
	int32 total_count = 6;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	if (!this->GetCaster()->HasBuff(BUFF_DEATHSIDE))
	{
		//this->SkillDeathsideInitial();

		this->GetCaster()->AddBuff(BUFF_DEATHSIDE, 10, 0, this->GetCaster());
	}
	else
	{
		this->GetCaster()->SetBuffTime(BUFF_DEATHSIDE, 10);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
	}

	--total_count;

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillDeathsideInitial()
{
	std::set<uint16> targets;

	Object * mTargetSec;

	int32 hit_count = 5;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
	}

	targets.insert(mTargetSec->GetEntry());

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}
	
void SkillHandler::SkillSoulSeeker()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4 + Random(2);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillFocusShot()
{
	if (!this->GetTarget())
	{
		return;
	}

	int32 hit_count = 4 + Random(2);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), false, 0, 0, 0, i);
	}
}

void SkillHandler::SkillFireBeast()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4 + Random(2);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillAquaBeast()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4 + Random(2);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillIceBlood()
{
	if (!this->GetTarget())
	{
		return;
	}

	float angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
	this->GetCaster()->SkillAngleCalculate(angle, 2.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 5.0f, 0.0f, false);

	Object * mTargetSec;

	int32 hit_count = 4 + Random(2);
	int32 total_count = 8;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (this->GetTarget() != mTargetSec)
		{
			--total_count;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillFireBlood()
{
	if (!this->GetTarget())
	{
		return;
	}

	float angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
	this->GetCaster()->SkillAngleCalculate(angle, 2.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 5.0f, 0.0f, false);

	Object * mTargetSec;

	int32 hit_count = 4 + Random(2);
	int32 total_count = 8;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		if (this->GetTarget() != mTargetSec)
		{
			--total_count;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillDarkBlast()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec;
	int32 hit_count = 4 + Random(2);

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	//this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillEvilWind()
{
	if (!this->GetTarget())
	{
		return;
	}

	Object * mTargetSec = nullptr;
	int32 hit_count = 4 + Random(2);
	int32 total_count = 8;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (this->GetTarget() != mTargetSec)
		{
			--total_count;
		}

		for (int32 i = 1; i < hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillDarkPhoenixShot()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	int32 hit_count = 4 + Random(2);
	int32 total_count = 3;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	Object* mTargetSec = nullptr;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (this->GetTarget() != mTargetSec)
		{
			--total_count;
		}

		for (int32 i = 1; i < hit_count; i++)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillSpiritHook()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	int32 hit_count = 4;
	int32 total_count = 3;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	Object* mTargetSec = nullptr;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		if (this->GetTarget() != mTargetSec)
		{
			--total_count;
		}

		for (int32 i = 1; i < hit_count; i++)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), this->GetTarget() != mTargetSec ? true : false, 0, 0, 0, i);
		}

		if (total_count <= 0)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillRadianceInmuneI()
{
	//this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
	//this->GetCaster()->AddBuff(BUFF_ERRTEL_IMMUNE_I, 60, 0, this->GetCaster());
}
	
void SkillHandler::SkillRadianceInmuneII()
{
	//this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
	//this->GetCaster()->AddBuff(BUFF_ERRTEL_IMMUNE_II, 60, 0, this->GetCaster());
}
	
void SkillHandler::SkillRadianceWidenedI()
{
	//this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());
	//this->GetCaster()->AddBuff(BUFF_ERRTEL_BERSERKER_I, 60, 0, this->GetCaster());
}

void SkillHandler::SkillPoisonStorm()
{
	if ( !this->GetTarget() )
	{
		return;
	}

	if ( !this->GetTarget()->IsCreature() )
	{
		return;
	}

	if ( !this->GetTarget()->HasBuff(BUFF_DEADLY_POISONING) )
	{
		return;
	}

	this->GetTarget()->RemoveBuff(BUFF_DEADLY_POISONING);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetCaster()->DebuffBlowAttack(this->GetTarget(), this->GetSkill());
}

void SkillHandler::SkillPoisonStormSplash(int32 damage)
{
	if (!this->GetTarget() || !this->GetSkill())
	{
		return;
	}

	if (damage <= 0)
	{
		return;
	}

	Object* mTargetSec = nullptr;
	int32 range = this->GetCaster()->MagicGetValue(1199, true);

	if (range <= 0)
	{
		return;
	}

	int32 infect_targets = this->GetCaster()->MagicGetValue(1133, true);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
		{
			continue;
		}

		if (mTargetSec == this->GetTarget())
		{
			continue;
		}

		if (this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) > range)
		{
			continue;
		}

		Monster* pMonster = mTargetSec->ToCreature();

		if (!pMonster)
		{
			continue;
		}

		this->GetCaster()->AttackProc(pMonster, nullptr, false, damage, 0, DAMAGE_TYPE_IGNORE);

		if (infect_targets > 0 && this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) <= 3)
		{
			if (pMonster->HasBuff(BUFF_DEADLY_POISONING))
			{
				continue;
			}

			if (pMonster->HasBuff(BUFF_POISONING))
			{
				pMonster->AddBuff(BUFF_DEADLY_POISONING, BuffEffect(BUFF_OPTION_NONE, 2000), 5, 0, this->GetCaster());
			}
			else
			{
				pMonster->AddBuff(BUFF_POISONING, BuffEffect(BUFF_OPTION_NONE, 2000), 5, 0, this->GetCaster());
			}

			--infect_targets;
		}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillFrozenSlaughter()
{
	if ( !this->GetTarget() )
	{
		return;
	}

	if ( !this->GetTarget()->IsCreature() )
	{
		return;
	}

	if ( !this->GetTarget()->HasBuff(BUFF_FREEZING) )
	{
		return;
	}

	this->GetTarget()->RemoveBuff(BUFF_FREEZING);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetCaster()->DebuffBlowAttack(this->GetTarget(), this->GetSkill());
}

void SkillHandler::SkillFrozenSlaughterSplash(int32 damage)
{
	if (!this->GetTarget() || !this->GetSkill())
	{
		return;
	}

	if (damage <= 0)
	{
		return;
	}

	Object* mTargetSec = nullptr;
	int32 range = this->GetCaster()->MagicGetValue(1200, true);

	if (range <= 0)
	{
		return;
	}

	int32 infect_targets = this->GetCaster()->MagicGetValue(1134, true);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
	{
		continue;
	}

	if (mTargetSec == this->GetTarget())
	{
		continue;
	}

	if (this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) > range)
	{
		continue;
	}

	Monster* pMonster = mTargetSec->ToCreature();

	if (!pMonster)
	{
		continue;
	}

	this->GetCaster()->AttackProc(pMonster, nullptr, false, damage, 0, DAMAGE_TYPE_IGNORE);

	if (infect_targets > 0 && this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) <= 3)
	{
		if (pMonster->HasBuff(BUFF_FREEZING))
		{
			continue;
		}

		if (pMonster->HasBuff(BUFF_CHILLING))
		{
			pMonster->AddBuff(BUFF_FREEZING, BuffEffect(BUFF_OPTION_DEFENSE_REDUCE, 20), 5, 0, this->GetCaster());
		}
		else
		{
			pMonster->AddBuff(BUFF_CHILLING, BuffEffect(BUFF_OPTION_DEFENSE_REDUCE, 20), 5, 0, this->GetCaster());
		}

		--infect_targets;
	}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillBloodyingHit()
{
	if ( !this->GetTarget() )
	{
		return;
	}

	if ( !this->GetTarget()->IsCreature() )
	{
		return;
	}

	if ( !this->GetTarget()->HasBuff(BUFF_HEMORRHAGE) )
	{
		return;
	}

	this->GetTarget()->RemoveBuff(BUFF_HEMORRHAGE);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetCaster()->DebuffBlowAttack(this->GetTarget(), this->GetSkill());
}

void SkillHandler::SkillBloodyingHitSplash(int32 damage)
{
	if (!this->GetTarget() || !this->GetSkill())
	{
		return;
	}

	if (damage <= 0)
	{
		return;
	}

	Object* mTargetSec = nullptr;
	int32 range = this->GetCaster()->MagicGetValue(1201, true);

	if (range <= 0)
	{
		return;
	}

	int32 infect_targets = this->GetCaster()->MagicGetValue(1135, true);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
	{
		continue;
	}

	if (mTargetSec == this->GetTarget())
	{
		continue;
	}

	if (this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) > range)
	{
		continue;
	}

	Monster* pMonster = mTargetSec->ToCreature();

	if (!pMonster)
	{
		continue;
	}

	this->GetCaster()->AttackProc(pMonster, nullptr, false, damage, 0, DAMAGE_TYPE_IGNORE);

	if (infect_targets > 0 && this->GetTarget()->Distance(mTargetSec->GetX(), mTargetSec->GetY()) <= 3)
	{
		if (pMonster->HasBuff(BUFF_HEMORRHAGE))
		{
			continue;
		}

		if (pMonster->HasBuff(BUFF_BLEEDING))
		{
			pMonster->AddBuff(BUFF_HEMORRHAGE, BuffEffect(BUFF_OPTION_NONE, 2000), 5, 0, this->GetCaster());
		}
		else
		{
			pMonster->AddBuff(BUFF_BLEEDING, BuffEffect(BUFF_OPTION_NONE, 2000), 5, 0, this->GetCaster());
		}

		--infect_targets;
	}

	VIEWPORT_CLOSE
}
	
void SkillHandler::SkillShockBoom()
{
	/*if ( !this->GetTarget() )
	{
		return;
	}

	if ( !this->GetTarget()->IsCreature() )
	{
		return;
	}

	if ( !this->GetTarget()->HasBuff(BUFF_SHOCK_2) )
	{
		return;
	}

	this->GetTarget()->RemoveBuff(BUFF_SHOCK_2);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	this->GetCaster()->DebuffBlowAttack(this->GetTarget(), this->GetSkill());

	this->GetTarget()->AddBuff(BUFF_STUN, 2 + this->GetCaster()->MagicGetValue(1136, true), 0, this->GetCaster());*/
}

void SkillHandler::SkillMagicArrow()
{
	NEW_SKILL_INFO pMsg;
	pMsg.SetSkill(this->GetSkill()->GetSkill());
	pMsg.SetSource(this->GetCaster()->GetEntry());
	pMsg.projectile_amount = sGameServer->GetSkillMagicArrowProjectiles();

	if (this->GetCaster()->GetMagicTreeLevel(749) > 0)
	{
		pMsg.projectile_amount += 1;
	}

	pMsg.start_direction = this->GetAngle() & 1;

	this->GetCaster()->SendPacket(&pMsg);
	this->GetCaster()->SendPacketViewport(&pMsg);

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer)
	{
		if (pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
		{
			this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);

			vec3_t Angle; //Good
			Vector(0.f, 6.0f, 0.f, Angle); //3,4,5 Good

			vec3_t p[4]; //Good size OK

			Vector(0.f, 0.f, 40.0f, p[0]); //Good
			Vector(0.f, 0.f, 20.0f, p[1]); //Good
			Vector(0.f, 0.f, 340.0f, p[2]); //Good
			Vector(0.f, 0.f, 320.0f, p[3]); //Good

			float Matrix[3][4]; //Good

			vec3_t vFrustrum[5]; //Good

			AngleMatrix(p[0], Matrix); //Good
			VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

			AngleMatrix(p[1], Matrix); //Good
			VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

			Vector(0.f, 6.0f, 0.f, vFrustrum[2]);//36,37,38

			AngleMatrix(p[2], Matrix); //Good
			VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

			AngleMatrix(p[3], Matrix); //Good
			VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

			//Vector(0.f, 0.f, pPlayer->GetHelper()->GetAttackAngle(), p[0]); //15, 16, 17 Good
			Vector(0.f, 0.f, (pPlayer->GetHelper()->GetAttackAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good

			AngleMatrix((float*)p, Matrix); //Good

			///////////////////////////////////////////////////////////////////////////////////////////////////
			int Value1[3];
			int Value2[3];
			int Value3[3];

			vec3_t vFrustrum2[3];

			for (int i = 0; i < 3; i++) //loc70 Good
			{
				VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

				Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
				Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
			}

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

				if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
				{
					continue;
				}

				if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
				{
					continue;
				}

				for (int32 j = 0; j < 3; ++j)
				{
					Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

					if (Value3[j] > -3 && Value3[j] < 3)
					{
						this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
					}
				}

			VIEWPORT_CLOSE
		}
	}
}

void SkillHandler::SkillMagicArrowSplash(int32 damage)
{
	if (!this->GetTarget())
	{
		return;
	}

	std::set<uint16> targets;

	Object* mTargetSec = nullptr;
	int32 damage_percent = sGameServer->GetSkillMagicArrowSplashDamage();

	int32 target_count = sGameServer->GetSkillMagicArrowSplashTargets() + this->GetCaster()->MagicGetValue(1154, true);

	int32 distance = sGameServer->GetSkillMagicArrowSplashDistance() + this->GetCaster()->MagicGetValue(1153, true);

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()))
		{
			continue;
		}

		if (mTargetSec == this->GetTarget())
		{
			continue;
		}

		if (Util::Distance(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) > distance)
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), nullptr, false, damage * damage_percent / 100, 0, 0);

		targets.insert(mTargetSec->GetEntry());

		--target_count;

		if (target_count <= 0)
		{
			break;
		}

		VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SkillPlasmaBall()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	NEW_SKILL_INFO pMsg;
	pMsg.SetSkill(this->GetSkill()->GetSkill());
	pMsg.SetSource(this->GetCaster()->GetEntry());

	pMsg.lightning_x = this->GetAttackX();
	pMsg.lightning_y = this->GetAttackY();

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		pMsg.ball_direction = this->GetSpecialValue();
	}
	else
	{
		if (this->GetCount() <= 0)
		{
			this->SetAngle(pPlayer->GetHelper()->GetAttackAngle());
		}

		pMsg.ball_direction = this->GetAngle() + (180 * 180 / 255);
	}

	pMsg.ball_move_distance = sGameServer->GetSkillPlasmaBallMoveDistance();

	if (this->GetSkill()->GetSkill() == 751)
	{
		pMsg.ball_move_distance += 1;
	}

	pMsg.ball_attack_distance = sGameServer->GetSkillPlasmaBallAttackDistance();
	pMsg.ball_move_speed = sGameServer->GetSkillPlasmaBallMoveSpeed();

	float reduce_speed = this->GetCaster()->MagicGetValue(1151, true);

	if (reduce_speed < 0)
	{
		reduce_speed = 0;
	}

	pMsg.ball_move_speed -= reduce_speed;

	if (pMsg.ball_move_speed <= 0)
	{
		pMsg.ball_move_speed = 1;
	}

	pMsg.ball_move_distance -= static_cast<int32>(reduce_speed);

	if (pMsg.ball_move_distance <= 0)
	{
		pMsg.ball_move_distance = 1;
	}

	pMsg.ball_target_count = sGameServer->GetSkillPlasmaBallTargetCount();
	pMsg.ball_target_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);

	pMsg.ball_attack_speed = sGameServer->GetSkillPlasmaBallAttackSpeed();

	if (this->GetCount() <= 0)
	{
		this->GetCaster()->SendPacket(&pMsg);
		this->GetCaster()->SendPacketViewport(&pMsg);
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	if (this->GetCount() <= 0)
	{
		this->SetAttackX(this->GetCaster()->GetX());
		this->SetAttackY(this->GetCaster()->GetY());
	}

	this->GetCaster()->SkillAngleCalculate(this->GetAngle(), pMsg.ball_attack_distance, pMsg.ball_attack_distance + this->GetCount(), 0, 0 + this->GetCount(), true, this->GetAttackX(), this->GetAttackY());

	int32 count = 0;
	Object* mTargetSec = nullptr;
	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());

	++count;

	if (count >= pMsg.ball_target_count)
	{
		break;
	}

	VIEWPORT_CLOSE

	if (this->GetCount() <= pMsg.ball_move_distance)
	{
		this->GetCaster()->AddDelayed(DELAYED_MULTI_SKILL_ANGLE, (pMsg.ball_move_speed * 1000) / pMsg.ball_move_distance, this->GetTarget(), this->GetSkill()->GetSkill(), this->GetCount() + 1, this->GetAttackX(), this->GetAttackY(), this->GetAngle());
	}
}

void SkillHandler::SkillLightningStorm()
{
	if (!this->CheckSkillRange(this->GetAttackX(), this->GetAttackY(), this->GetCaster()))
	{
		return;
	}

	NEW_SKILL_INFO pMsg;
	pMsg.SetSkill(this->GetSkill()->GetSkill());
	pMsg.SetSource(this->GetCaster()->GetEntry());

	pMsg.lightning_x = this->GetAttackX();
	pMsg.lightning_y = this->GetAttackY();
	pMsg.ball_attack_distance = GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill());

	this->GetCaster()->SendPacket(&pMsg);
	this->GetCaster()->SendPacketViewport(&pMsg);

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer)
	{
		if (pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
		{
			this->SkillLightningStormSplash(true);
		}
	}
}

void SkillHandler::SkillLightningStormSplash(bool delay, Unit* pTarget)
{
	Object * mTargetSec;
	int32 count = 0;
	int32 hit_count = 4;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != pTarget)
		{
			continue;
		}

		if (!SkillHandler::CheckSkillRadio(this->GetCaster(), this->GetSkill()->GetSkill(), this->GetAttackX(), this->GetAttackY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 i = 1; i < (hit_count + 1); ++i)
		{
			if (delay)
			{
				this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill(), 0, 0, 0, i);
			}
			else
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
			}
		}

		++count;

		if (count >= 8)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBurstRW()
{
	int32 effect1 = this->GetCaster()->GetEnergy() / sGameServer->GetSkillBurstRWEffect1Div();
	int32 effect2 = this->GetCaster()->GetEnergy() / sGameServer->GetSkillBurstRWEffect2Div();
	int32 duration = sGameServer->GetSkillBurstRWDurationAdd() + (this->GetCaster()->GetEnergy() / sGameServer->GetSkillBurstRWDurationDiv()) + this->GetCaster()->MagicGetValue(765);
	
	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, effect1), BuffEffect(BUFF_OPTION_INCREASES_MP_CONSUMPTION, effect2), duration, 0, this->GetCaster());

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(effect1 + effect2);
}

void SkillHandler::SkillHasteRW()
{
	this->GetPartyMembers();

	int32 effect1 = this->GetCaster()->GetEnergy() / sGameServer->GetSkillHasteRWEffect1Div();
	int32 effect2 = this->GetCaster()->GetEnergy() / sGameServer->GetSkillHasteRWEffect2Div();
	int32 duration = sGameServer->GetSkillHasteRWDurationAdd() + (this->GetCaster()->GetEnergy() / sGameServer->GetSkillHasteRWDurationDiv()) + this->GetCaster()->MagicGetValue(768);

	for (int32 i = 0; i < this->party_member_count; ++i)
	{
		if (this->PartyMember[i])
		{
			if (this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), effect1, effect2))
			{
				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_INCREASE_ATTACK_SPEED, effect1), BuffEffect(BUFF_OPTION_INCREASES_AG_CONSUMPTION, effect2), duration, 0, this->GetCaster());
			}
		}
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill());

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(effect1 + effect2);
}

void SkillHandler::SkillSwordInertia()
{
	NEW_SKILL_INFO pMsg;
	pMsg.SetSkill(this->GetSkill()->GetSkill());
	pMsg.SetSource(this->GetCaster()->GetEntry());
	pMsg.projectile_amount = 3;

	this->GetCaster()->SendPacket(&pMsg);
	this->GetCaster()->SendPacketViewport(&pMsg);

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);

	vec3_t Angle; //Good
	Vector(0.f, 6.0f, 0.f, Angle); //3,4,5 Good

	vec3_t p[4]; //Good size OK

	Vector(0.f, 0.f, 40.0f, p[0]); //Good
	Vector(0.f, 0.f, 20.0f, p[1]); //Good
	Vector(0.f, 0.f, 340.0f, p[2]); //Good
	Vector(0.f, 0.f, 320.0f, p[3]); //Good

	float Matrix[3][4]; //Good

	vec3_t vFrustrum[5]; //Good

	AngleMatrix(p[0], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

	AngleMatrix(p[1], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

	Vector(0.f, 6.0f, 0.f, vFrustrum[2]);//36,37,38

	AngleMatrix(p[2], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

	AngleMatrix(p[3], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

	//Vector(0.f, 0.f, pPlayer->GetHelper()->GetAttackAngle(), p[0]); //15, 16, 17 Good
	Vector(0.f, 0.f, (pPlayer->GetHelper()->GetAttackAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good

	AngleMatrix((float*)p, Matrix); //Good

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int Value1[3];
	int Value2[3];
	int Value3[3];

	vec3_t vFrustrum2[3];

	for (int i = 0; i < 3; i++) //loc70 Good
	{
		VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

		Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
		Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
	}

	Object * mTargetSec;
	int32 count = 0;

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	for (int32 j = 0; j < 3; ++j)
	{
		Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

		if (Value3[j] > -3 && Value3[j] < 3)
		{
			for (int32 i = 1; i < (hit_count + 1); ++i)
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i - 1);
			}
			++count;
		}
	}

	if (count >= 5)
	{
		break;
	}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillBatFlock()
{
	std::set<uint16> targets;

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}
	else
	{
		uint8 angle = this->GetSpecialValue() - 0x80;

		this->GetCaster()->SkillAngleCalculate(angle, 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}

	Object* mTargetSec;
	int32 hit_count = 2;
	int32 total_count = 5;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	if (this->GetTarget() != mTargetSec)
	{
		--total_count;
	}

	for (int32 i = 0; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i + 1);
	}

	targets.insert(mTargetSec->GetEntry());

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SkillPierceAttack()
{
	if (!this->GetTarget())
	{
		return;
	}

	std::set<uint16> targets;

	Object * mTargetSec = nullptr;
	int32 hit_count = 2;
	int32 total_count = 8;

	if (this->GetTarget()->HasBuff(BUFF_BAT_FLOCK))
	{
		hit_count += 2;
	}

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	if (this->GetTarget() != mTargetSec)
	{
		--total_count;
	}

	for (int32 i = 0; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i + 1);
	}

	targets.insert(mTargetSec->GetEntry());

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SkillDetection()
{
	World* pWorld = this->GetCaster()->GetWorld();

	if (!pWorld)
	{
		return;
	}

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), MINUTE, 0, this->GetCaster());

	int32 detection_range = this->GetCaster()->MagicGetValue(794);

	if (detection_range <= 0)
	{
		detection_range = sGameServer->GetSkillDetectionRange();
	}

	detection_range /= 2;

	uint8 buffer[8192];
	POINTER_PCT(UNIT_DETECTION_HEAD, head, buffer, 0);
	POINTER_PCT(UNIT_DETECTION_BODY, body, buffer, sizeof(UNIT_DETECTION_HEAD));
	head->junk = 0;
	head->count = 0;

	WorldObjectMap const& list = pWorld->GetAllObjects();
	for (WorldObjectMap::const_iterator itr = list.begin(); itr != list.end(); ++itr)
	{
		Object* pObject = itr->second;

		if (!pObject)
		{
			continue;
		}

		Monster* pMonster = pObject->ToCreature();

		if (!pMonster)
		{
			continue;
		}

		if (!this->GetCaster()->SameDimension(pMonster))
		{
			continue;
		}

		if (!IN_RANGE(this->GetCaster(), pMonster, detection_range))
		{
			continue;
		}

		body[head->count].id = 2;
		body[head->count].x = pMonster->GetX();
		body[head->count].y = pMonster->GetY();

		++head->count;
	}

	head->Set(0xE7, 0x05, sizeof(UNIT_DETECTION_HEAD)+(head->count * sizeof(UNIT_DETECTION_BODY)));
	this->GetCaster()->sendPacket(buffer);
}

void SkillHandler::SkillDemolish()
{
	GetPartyMembers();

	int32 damage_reduction = 3;
	float defense_ignore = (((this->GetCaster()->GetStrength() / 8) + (this->GetCaster()->GetAgility() / 28) + 120) * 0.03f) + this->GetCaster()->MagicGetValue(788);
	int32 duration = 60;

	for (int32 i = 0; i < this->party_member_count; ++i)
	{
		if (this->PartyMember[i])
		{
			this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
				BuffEffect(BUFF_OPTION_DEMOLISH_DAMAGE_DECREASE, damage_reduction),
				BuffEffect(BUFF_OPTION_IGNORE_BASE_DEFENSE, defense_ignore), duration, 0, this->GetCaster(), true);
		}
	}

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(damage_reduction + defense_ignore);
}

void SkillHandler::SkillTwister()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	if (this->GetCount() <= 0)
	{
		this->SetAttackX(this->GetCaster()->GetX());
		this->SetAttackY(this->GetCaster()->GetY());

		this->SetAngle(pPlayer->GetHelper()->GetAttackAngle());
	}

	this->GetCaster()->SkillAngleCalculate(this->GetAngle(), 1, 1 + this->GetCount(), 1, 0 + this->GetCount(), true, this->GetAttackX(), this->GetAttackY());

	Object* mTargetSec = nullptr;
	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());

	VIEWPORT_CLOSE

	if (this->GetCount() <= 6)
	{
		this->GetCaster()->AddDelayed(DELAYED_MULTI_SKILL_ANGLE, 250, this->GetTarget(), this->GetSkill()->GetSkill(), this->GetCount() + 1, this->GetAttackX(), this->GetAttackY(), this->GetAngle());
	}
}

void SkillHandler::SkillSwordWrath()
{
	this->GetCaster()->RemoveBuff(325);

	int32 duration = WEEK;

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillStrongBelief()
{
	this->GetCaster()->RemoveBuff(324);

	int32 duration = WEEK;
	int32 basic_defense = this->GetCaster()->GetEnergy() / 70;

	basic_defense += this->GetCaster()->MagicGetValue(810);

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(), BuffEffect(BUFF_OPTION_BASIC_DEFENSE, basic_defense), duration, 0, this->GetCaster(), true);
	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillSolidProtection()
{
	this->GetPartyMembers();
	int32 duration = 60 + (this->GetCaster()->GetEnergy() / 10);

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);

	int32 party_damage = this->GetCaster()->GetEnergy() / 70;
	int32 party_defense = 0;
	float hp_transfer = this->GetCaster()->GetAgility() / 150;
	float damage_transfer = this->GetCaster()->GetEnergy() / 250;

	int32 shield_defense = 0;
	float damage_decrease = 0;
	int32 base_defense = 0;
	int32 maximum_hp = 0;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		party_damage += pPlayer->GetAncientEffect(ANCIENT_EFFECT_SOLID_PROTECTION_PARTY_DAMAGE);
		party_damage += pPlayer->MagicGetValue(806);
		party_damage += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_MEMBER_DAMAGE);

		party_defense += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_MEMBER_DEFENSE);

		hp_transfer += pPlayer->MagicGetValue(803);
		hp_transfer += pPlayer->GetAncientEffect(ANCIENT_EFFECT_SOLID_PROTECTION_HP_TRANSFER);
		hp_transfer += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_HP_TRANSFER);

		damage_transfer += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_DAMAGE_TRANSFER);

		shield_defense += pPlayer->GetAncientEffect(ANCIENT_EFFECT_SOLID_PROTECTION_SHIELD_DEFENSE);
		shield_defense += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_SHIELD_DEFENSE);

		damage_decrease += pPlayer->MagicGetValue(804);

		base_defense += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_BASE_DEFENSE);
		maximum_hp += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_SP_MAX_HP);
	}

	if (damage_transfer > 50)
	{
		damage_transfer = 50;
	}

	if (hp_transfer > 50)
	{
		hp_transfer = 50;
	}

	for (int32 i = 0; i < this->party_member_count; ++i)
	{
		if (this->PartyMember[i] && this->PartyMember[i] != this->GetCaster())
		{
			if (this->PartyMember[i]->ReplaceBuff(this->GetSkillInfo()->GetBuffIcon(), party_damage, hp_transfer, party_defense))
			{
				int32 decrease_hp = this->PartyMember[i]->PowerGetMax(POWER_LIFE) * hp_transfer / 100;
				maximum_hp += decrease_hp;

				this->PartyMember[i]->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
					BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, party_damage),
					BuffEffect(BUFF_OPTION_SOLID_PROTECTION_DAMAGE_TRANSFER, damage_transfer),
					BuffEffect(BUFF_OPTION_DECREASE_MAXIMUM_HP, decrease_hp),
					BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, party_defense), 
					duration, 0, this->GetCaster(), true);
			}
		}
	}

	this->GetCaster()->AddBuff(this->GetSkillInfo()->GetBuffIcon(),
		BuffEffect(BUFF_OPTION_SOLID_PROTECTION_SHIELD_DEFENSE, shield_defense),
		BuffEffect(BUFF_OPTION_INCREASE_DAMAGE_DECREASE, damage_decrease),
		BuffEffect(BUFF_OPTION_BASIC_DEFENSE, base_defense),
		BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_LIFE, maximum_hp),
		duration, 0, this->GetCaster(), true);

	PLAYER_POINTER(this->GetCaster())->IncreaseAggroAround(party_damage + hp_transfer + party_defense);
}

void SkillHandler::SkillDarkPlasma()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	NEW_SKILL_INFO pMsg;
	pMsg.SetSkill(this->GetSkill()->GetSkill());
	pMsg.SetSource(this->GetCaster()->GetEntry());

	pMsg.dark_plasma_x = 0;
	pMsg.dark_plasma_y = 0;

	pMsg.lightning_x = this->GetAttackX();
	pMsg.lightning_y = this->GetAttackY();

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		pMsg.ball_direction = this->GetSpecialValue();
	}
	else
	{
		if (this->GetCount() <= 0)
		{
			this->SetAngle(pPlayer->GetHelper()->GetAttackAngle());
		}

		pMsg.ball_direction = this->GetAngle() + (180 * 180 / 255);
	}

	pMsg.ball_attack_distance = sGameServer->GetSkillDarkPlasmaAttackDistance();

	pMsg.ball_target_count = sGameServer->GetSkillDarkPlasmaTargetCount();
	pMsg.ball_target_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	pMsg.ball_target_count += pPlayer->MagicGetValue(820);

	pMsg.ball_attack_speed = sGameServer->GetSkillDarkPlasmaAttackSpeed();

	if (this->GetCount() <= 0)
	{
		this->GetCaster()->SendPacket(&pMsg);
		this->GetCaster()->SendPacketViewport(&pMsg);
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	if (this->GetCount() <= 0)
	{
		this->SetAttackX(this->GetCaster()->GetX());
		this->SetAttackY(this->GetCaster()->GetY());
	}

	this->GetCaster()->SkillAngleCalculate(this->GetAngle(), pMsg.ball_attack_distance, pMsg.ball_attack_distance + this->GetCount(), 0, 0 + this->GetCount(), true, this->GetAttackX(), this->GetAttackY());

	int32 count = 0;
	Object* mTargetSec = nullptr;
	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());

	++count;

	if (count >= pMsg.ball_target_count)
	{
		break;
	}

	VIEWPORT_CLOSE

	if (this->GetCount() <= 5)
	{
		this->GetCaster()->AddDelayed(DELAYED_MULTI_SKILL, 1000, this->GetTarget(), this->GetSkill()->GetSkill(), this->GetCount() + 1, this->GetAttackX(), this->GetAttackY());
	}
}

void SkillHandler::SkillIceBreak()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY()), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f, false);
	}

	int32 hit_count = 1;
	int32 total_count = 8;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		continue;

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		continue;

	if (hit_count == 1)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());
	}
	else
	{
		for (int32 i = 1; i <= hit_count; ++i)
		{
			this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
		}
	}

	--total_count;

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillIceBlast()
{
	Object * mTargetSec;

	int32 hit_count = 5;
	int32 total_count = 6;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->CheckSkillRange(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
	{
		continue;
	}

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
	}

	--total_count;

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillFireDeath()
{
	if (!this->GetTarget())
	{
		return;
	}

	Player* pPlayer = this->GetCaster()->ToPlayer();

	float angle = 0;
	bool convert = true;

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		angle = pPlayer->GetHelper()->GetAttackAngle();
	}
	else
	{
		angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
		convert = false;
	}

	this->GetCaster()->SkillAngleCalculate(angle, 3.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f, convert);

	vec3_t Angle; //Good
	Vector(0.f, 3.0f, 0.f, Angle); //3,4,5 Good

	vec3_t p[4]; //Good size OK

	Vector(0.f, 0.f, 40.0f, p[0]); //Good
	Vector(0.f, 0.f, 20.0f, p[1]); //Good
	Vector(0.f, 0.f, 340.0f, p[2]); //Good
	Vector(0.f, 0.f, 320.0f, p[3]); //Good

	float Matrix[3][4]; //Good

	vec3_t vFrustrum[5]; //Good

	AngleMatrix(p[0], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

	AngleMatrix(p[1], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

	Vector(0.f, 3.0f, 0.f, vFrustrum[2]);//36,37,38

	AngleMatrix(p[2], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

	AngleMatrix(p[3], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

	Vector(0.f, 0.f, convert ? (angle * 360 / (BYTE)255) : angle, p[0]); //15, 16, 17 Good

	AngleMatrix((float*)p, Matrix); //Good

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int Value1[5];
	int Value2[5];
	int Value3[5];

	vec3_t vFrustrum2[5];

	for (int i = 0; i < 5; i++) //loc70 Good
	{
		VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

		Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
		Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
	}

	int32 hit_count = 1;

	if (pPlayer)
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
	}

	std::set<uint16> targets;

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
	{
		continue;
	}

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
	{
		continue;
	}

	for (int32 j = 0; j < 5; ++j)
	{
		Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

		if (Value3[j] > -5 && Value3[j] < 5)
		{
			if (hit_count == 1)
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill());
			}
			else
			{
				for (int32 i = 1; i <= hit_count; ++i)
				{
					this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
				}
			}

			targets.insert(mTargetSec->GetEntry());
		}
	}

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SkillBurstingFlare()
{
	if (!this->GetTarget())
	{
		return;
	}

	std::set<uint16> targets;

	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (pPlayer && pPlayer->GetHelper()->IsStarted() && pPlayer->GetHelper()->IsOffline())
	{
		this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f);
	}
	else
	{
		this->GetCaster()->SkillAngleCalculate(this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY()), 1.5f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.5f, 0.0f, false);
	}

	int32 hit_count = 5;
	int32 total_count = 6;

	if (Player* pPlayer = this->GetCaster()->ToPlayer())
	{
		hit_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);
		total_count += pPlayer->GetMajesticDataValueBySkill(this->GetSkill()->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_TARGET);
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

	if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		continue;

	if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) && !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()))
		continue;

	for (int32 i = 1; i < hit_count; ++i)
	{
		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), false, 0, 0, 0, i);
	}

	targets.insert(mTargetSec->GetEntry());

	--total_count;

	if (total_count <= 0)
	{
		break;
	}

	VIEWPORT_CLOSE

	this->SendMultiTarget(targets);
}

void SkillHandler::SendMultiTarget(std::set<uint16> const& list)
{
	uint8 buffer[8192];
	POINTER_PCT(NEW_SKILL_VIEW_HEAD, head, buffer, 0);
	POINTER_PCT(NEW_SKILL_VIEW_BODY, body, buffer, sizeof(NEW_SKILL_VIEW_HEAD));
	head->count = 0;
	head->skill = this->GetSkill()->GetSkill();
	head->source = this->GetCaster()->GetEntry();

	for (auto & itr : list)
	{
		auto & data = body[head->count++];
		data.target = itr;
	}

	head->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_NEW_SKILL_VIEW, sizeof(NEW_SKILL_VIEW_HEAD)+(head->count * sizeof(NEW_SKILL_VIEW_BODY)));

	this->GetCaster()->sendPacket(buffer);
	this->GetCaster()->SendPacketViewport(buffer);
}

void SkillHandler::SkillDelayed()
{
	if (!this->GetSkillInfo())
	{
		return;
	}

	switch (this->GetSkillInfo()->GetBaseSkill())
	{
	case SKILL_FLAME:
		//this->SkillAttackFlame();
		break;

	case SKILL_EVIL_SPIRIT:
		//this->SkillAttackEvilSpirit();
		break;

	case SKILL_HELL_FIRE:
		//this->SkillAttackHellFire();
		break;

	case SKILL_INFERNO:
		//this->SkillAttackInferno();
		break;

	case SKILL_TWISTER:
		this->SkillTwister();
		break;

	case SKILL_PLASMA_BALL:
		this->SkillPlasmaBall();
		break;

	case SKILL_DARK_PLASMA:
		this->SkillDarkPlasma();
		break;
	}
}

void SkillHandler::EliteMonsterSkill()
{
	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());

	switch (this->GetCaster()->GetElementalAttribute())
	{
	case ELEMENTAL_ATTRIBUTE_FIRE:
	{
									 int32 value1 = 1200 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_BLEEDING_RESISTANCE);
									 this->GetTarget()->AddBuff(309, BuffEffect(BUFF_OPTION_NONE, value1), 10, 0, this->GetCaster());

									 int32 value2 = 1000 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_DAMAGE_REDUCTION_RESISTANCE);
									 this->GetTarget()->AddBuff(311, BuffEffect(BUFF_OPTION_ELITE_DAMAGE_REDUCTION, value2), 10, 0, this->GetCaster());
	} break;

	case ELEMENTAL_ATTRIBUTE_WATER:
	{
									  int32 value1 = 1700 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_POISON_RESISTANCE);
									  this->GetTarget()->AddBuff(310, BuffEffect(BUFF_OPTION_NONE, value1), 10, 0, this->GetCaster());

									  int32 value2 = 1000 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_DAMAGE_REDUCTION_RESISTANCE);
									  this->GetTarget()->AddBuff(311, BuffEffect(BUFF_OPTION_ELITE_DAMAGE_REDUCTION, value2), 10, 0, this->GetCaster());
	} break;

	case ELEMENTAL_ATTRIBUTE_EARTH:
	{
									  int32 value1 = 1200 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_BLEEDING_RESISTANCE);
									  this->GetTarget()->AddBuff(309, BuffEffect(BUFF_OPTION_NONE, value1), 10, 0, this->GetCaster());

									  int32 value2 = 20 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_ATTACK_SPEED_REDUCTION_RESISTANCE);
									  this->GetTarget()->AddBuff(312, BuffEffect(BUFF_OPTION_ELITE_ATTACK_SPEED_REDUCTION, value2), 10, 0, this->GetCaster());
	} break;

	case ELEMENTAL_ATTRIBUTE_WIND:
	{
									 int32 value1 = 1700 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_POISON_RESISTANCE);
									 this->GetTarget()->AddBuff(310, BuffEffect(BUFF_OPTION_NONE, value1), 10, 0, this->GetCaster());

									 int32 value2 = 800 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_DEFENSE_REDUCTION_RESISTANCE);
									 this->GetTarget()->AddBuff(313, BuffEffect(BUFF_OPTION_ELITE_DEFENSE_REDUCTION, value2), 10, 0, this->GetCaster());
	} break;

	case ELEMENTAL_ATTRIBUTE_DARK:
	{
									 int32 value1 = 1200 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_BLEEDING_RESISTANCE);
									 this->GetTarget()->AddBuff(309, BuffEffect(BUFF_OPTION_NONE, value1), 10, 0, this->GetCaster());

									 int32 value2 = 800 - this->GetTarget()->GetIntData(UNIT_INT_DEBUFF_DEFENSE_REDUCTION_RESISTANCE);
									 this->GetTarget()->AddBuff(313, BuffEffect(BUFF_OPTION_ELITE_DEFENSE_REDUCTION, value2), 10, 0, this->GetCaster());
	} break;
	}

	this->GetCaster()->ActionSend(this->GetTarget()->GetEntry(), 120, this->GetCaster()->GetDirection());
	this->GetCaster()->AttackProc(this->GetCaster(), this->GetSkill());
}

bool SkillHandler::SkillBuffFree()
{
	if ( !this->GetTarget() || !this->GetTarget()->IsPlaying() || !this->GetTarget()->IsLive() )
	{
		this->SetTarget(this->GetCaster());
		return false;
	}

	if ( this->GetTarget() != this->GetCaster() )
	{
		if ( this->GetTarget()->IsCreature() && !this->GetTarget()->GetSummoner()->IsPlayer() )
		{
			this->SetTarget(this->GetCaster());
			return false;
		}
		
		if ( CC_MAP_RANGE(this->GetCaster()->GetWorldId()) || 
			 this->GetCaster()->GetWorldId() == WORLD_VULCANUS_ROOM )
		{
			this->SetTarget(this->GetCaster());
			return false;
		}

		if ( this->GetCaster()->GetWorldId() == WORLD_LAST_MAN_STANDING )
		{
			this->SetTarget(this->GetCaster());
			return false;
		}
	}

	return true;
}

void SkillHandler::GetPartyMembers(uint8 distance)
{
	if ( distance == 0 )
	{
		distance = SkillHandler::GetSkillRadio(this->GetCaster(), this->GetSkillInfo()->GetID());
	}

	this->GetCaster()->GetPartyMembers(PartyMember, party_member_count, distance);

	if ( party_member_count > MAX_PARTY_MEMBERS )
		party_member_count = 1;
}

bool SkillHandler::CheckSkillRange(coord_type x, coord_type y, Unit* pTarget)
{
	return SkillHandler::CheckSkillRange(this->GetCaster(), this->GetSkillInfo() ? this->GetSkillInfo()->GetID() : 0, x, y, pTarget);
}
	
bool SkillHandler::CheckSkillRadio(coord_type x, coord_type y, Unit* pTarget)
{
	return SkillHandler::CheckSkillRadio(this->GetCaster(), this->GetSkillInfo() ? this->GetSkillInfo()->GetID() : 0, x, y, pTarget);
}

bool SkillHandler::CheckSkillRange(Unit* pUnit, uint16 skill, coord_type x, coord_type y, Unit* pTarget)
{
	if (!pUnit || !pTarget)
	{
		return false;
	}

	int32 distance = SkillHandler::GetSkillRange(pUnit, skill);

	if (distance <= -1)
	{
		return false;
	}

	if (Util::Distance(x, y, pTarget->GetX(), pTarget->GetY()) > distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool SkillHandler::CheckSkillRadio(Unit* pUnit, uint16 skill, coord_type x, coord_type y, Unit* pTarget)
{
	if (!pUnit || !pTarget)
	{
		return false;
	}

	int32 distance = SkillHandler::GetSkillRadio(pUnit, skill);

	if (distance <= -1)
	{
		return false;
	}

	if (Util::Distance(x, y, pTarget->GetX(), pTarget->GetY()) > distance)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int32 SkillHandler::GetSkillRange(Unit* pUnit, uint16 skill)
{
	if (!pUnit)
	{
		return -1;
	}

	skill_template const* pSkillInfo = sSkillMgr->GetSkill(skill);

	if (!pSkillInfo)
	{
		return -1;
	}

	int32 distance = pSkillInfo->GetEffectRange();

	if (Player* pPlayer = pUnit->ToPlayer())
	{
		distance += pPlayer->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_ATTACK_RANGE);

		if (pPlayer->IsActiveInventoryItem(PET_DARK_HORSE))
		{
			distance += 2;
		}

		if (pSkillInfo->GetBaseSkill() == SKILL_DEATH_STAB || pSkillInfo->GetBaseSkill() == SKILL_FIRE_BLOW || pSkillInfo->GetBaseSkill() == SKILL_SWORD_BLOW)
		{
			if (pPlayer->HasBuff(BUFF_SWORD_WRATH))
			{
				distance += 1;
			}

			if (pPlayer->HasBuff(BUFF_SWORD_WRATH_MASTERY))
			{
				distance += 2;
			}
		}
	}

	return distance;
}

int32 SkillHandler::GetSkillRadio(Unit* pUnit, uint16 skill)
{
	if (!pUnit)
	{
		return -1;
	}

	skill_template const* pSkillInfo = sSkillMgr->GetSkill(skill);

	if (!pSkillInfo)
	{
		return -1;
	}

	int32 distance = pSkillInfo->GetEffectRadio();

	if (Player* pPlayer = pUnit->ToPlayer())
	{
		distance += pPlayer->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_DISTANCE);

		if (pSkillInfo->GetBaseSkill() == SKILL_DEATH_STAB || pSkillInfo->GetBaseSkill() == SKILL_FIRE_BLOW || pSkillInfo->GetBaseSkill() == SKILL_SWORD_BLOW)
		{
			if (pPlayer->HasBuff(BUFF_SWORD_WRATH))
			{
				distance += 1;
			}

			if (pPlayer->HasBuff(BUFF_SWORD_WRATH_MASTERY))
			{
				distance += 2;
			}
		}
	}

	return distance;
}

void SkillHandler::SkillBlast()
{
	if (!this->GetTarget())
	{
		return;
	}

	Monster* pMonster = this->GetCaster()->ToCreature();

	if (pMonster)
	{
		this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill(), true);
		return;
	}


	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	Object * mTargetSec;
	int32 count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->CheckSkillRadio(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->ToUnit()))
		{
			continue;
		}

		for (int32 j = 0; j < 3; ++j)
		{
			if (Random(2) == 0)
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);
			}
		}

		if (count >= 3)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAquaBeam()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 1.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 1.0f, 0.0f);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);

	VIEWPORT_CLOSE
}

void SkillHandler::SkillPenetration()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline() )
	{
		return;
	}

	this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 2.0f, 8.0f, 2.0f, 0.0f);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
			continue;

		if ( !this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
			continue;

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);

	VIEWPORT_CLOSE
}

void SkillHandler::SkillFireSlash()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	if (!this->GetTarget())
	{
		return;
	}

	float angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());
	this->GetCaster()->SkillAngleCalculate(angle, 2.0f, SkillHandler::GetSkillRange(this->GetCaster(), this->GetSkill()->GetSkill()) + 1, 5.0f, 0.0f, false);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);

	mTargetSec->ToUnit()->AddBuff(BUFF_DEFENSIBILITY_REDUCTION, BuffEffect(BUFF_OPTION_DECREASE_DEFENSE_PERCENT, 10 + this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_FIRE_SLASH_ENHANCED)), 10, 0, this->GetCaster());

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFireScream()
{
	Player* pPlayer = this->GetCaster()->ToPlayer();

	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline())
	{
		return;
	}

	this->GetCaster()->SkillAngleCalculate(pPlayer->GetHelper()->GetAttackAngle(), 6.0f, 9.0f, 1.0f, 0.0f);

	vec3_t Angle; //Good
	Vector(0.f, 6.0f, 0.f, Angle); //3,4,5 Good

	vec3_t p[4]; //Good size OK

	Vector(0.f, 0.f, 40.0f, p[0]); //Good
	Vector(0.f, 0.f, 20.0f, p[1]); //Good
	Vector(0.f, 0.f, 340.0f, p[2]); //Good
	Vector(0.f, 0.f, 320.0f, p[3]); //Good

	float Matrix[3][4]; //Good

	vec3_t vFrustrum[5]; //Good

	AngleMatrix(p[0], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[0]); //loc44 Good

	AngleMatrix(p[1], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[1]); //loc41 Good

	Vector(0.f, 6.0f, 0.f, vFrustrum[2]);//36,37,38

	AngleMatrix(p[2], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[3]); //loc35 Good

	AngleMatrix(p[3], Matrix); //Good
	VectorRotate(Angle, Matrix, vFrustrum[4]); //loc32 Good

	//Vector(0.f, 0.f, pPlayer->GetHelper()->GetAttackAngle(), p[0]); //15, 16, 17 Good
	Vector(0.f, 0.f, (pPlayer->GetHelper()->GetAttackAngle() * 360 / (BYTE)255), p[0]); //15, 16, 17 Good

	AngleMatrix((float*)p, Matrix); //Good

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int Value1[3];
	int Value2[3];
	int Value3[3];

	vec3_t vFrustrum2[3];

	for (int i = 0; i < 3; i++) //loc70 Good
	{
		VectorRotate(vFrustrum[i], Matrix, vFrustrum2[i]);

		Value1[i] = (int)vFrustrum2[i][0] + this->GetCaster()->GetX();
		Value2[i] = (int)vFrustrum2[i][1] + this->GetCaster()->GetY();
	}

	Object * mTargetSec;
	int32 count = 0;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		for (int32 j = 0; j < 3; ++j)
		{
			Value3[j] = (((int)Value1[j] - this->GetCaster()->GetX()) * (mTargetSec->GetY() - this->GetCaster()->GetY())) - (((int)Value2[j] - this->GetCaster()->GetY()) * (mTargetSec->GetX() - this->GetCaster()->GetX()));

			if (Value3[j] > -3 && Value3[j] < 3)
			{
				this->GetCaster()->AttackProc(mTargetSec->ToUnit(), this->GetSkill(), true);
				++count;
			}
		}

		if (count >= 5)
		{
			break;
		}

	VIEWPORT_CLOSE
}

void SkillHandler::SkillAttackFireScreamExplosion(int32 damage)
{
	if (!roll_chance_i(sGameServer->GetSkillFireScreamExplosionRate(), 10000))
	{
		return;
	}

	int32 explosion_damage = damage / 10;

	if ( explosion_damage < 1 )
		return;

	explosion_damage += this->GetCaster()->MagicGetValue(SKILL_TREE_ADD_FIRE_SCREAM_ENHANCED);

	Object * mTargetSec;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if ( !this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget() )
			continue;

		if ( !this->CheckSkillRadio(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->ToUnit()) )
		{
			continue;
		}
			
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 100, mTargetSec->ToUnit(), 0, explosion_damage);
		this->GetCaster()->MagicAttackSend(ENTRY(mTargetSec), SKILL_FIRE_SCREAM_EXPLOSION);

	VIEWPORT_CLOSE
}

void SkillHandler::SkillCrescentMoonSlash()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
}

void SkillHandler::SkillManaGlaive()
{
	if (this->GetCaster()->IsPlayer())
	{
		if (this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START)
		{
			return;
		}
	}

	if (!this->GetTarget())
	{
		return;
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, this->GetTarget(), this->GetSkill()->GetSkill());
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 700, this->GetTarget(), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillStarfall()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 800, this->GetTarget(), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillSpiralSlash()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 100, this->GetTarget(), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillManaRays()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AttackProc(this->GetTarget(), this->GetSkill());
}

void SkillHandler::SkillFireBlast()
{
	if ( this->GetCaster()->IsPlayer() )
	{
		if ( this->GetCaster()->GetWorldId() != WORLD_CASTLE_SIEGE || sCastleSiege->GetState() != CASTLE_SIEGE_STATE_START )
		{
			return;
		}
	}

	if ( !this->GetTarget() )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), this->GetSkill()->GetSkill());
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, this->GetTarget(), this->GetSkill()->GetSkill());
}

void SkillHandler::SkillMonsterSummon()
{
	if ( !this->GetCaster()->IsCreature() )
		return;

	World* pWorld = this->GetCaster()->GetWorld();

	if ( !pWorld )
		return;
	
	int32 summon_class = 0;
	coord_type summoned_x = 0;
	coord_type summoned_y = 0;
	bool success = false;

	switch ( this->GetCaster()->ToCreature()->GetClass() )
	{
	case 161: { summon_class = Random(2) + 147; } break;
	case 181: { summon_class = Random(2) + 177; } break;
	case 189: { summon_class = Random(2) + 185; } break;
	case 197: { summon_class = Random(2) + 193; } break;
	case 267: { summon_class = Random(2) + 263; } break;
	case 275: { summon_class = Random(2) + 271; } break;
	case 338: { summon_class = Random(2) + 331; } break;
	}

	if ( Random(15) )
		return;

	if ( pWorld->GetFreeLocation(summoned_x, summoned_y, 2, 2, 30) )
	{
		Monster *mSummoned = sObjectMgr->MonsterTryAdd(summon_class, this->GetCaster()->GetWorldId());

		if ( mSummoned )
		{
			mSummoned->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			mSummoned->SetMoveDistance(15);
			mSummoned->SetX(summoned_x);
			mSummoned->SetY(summoned_y);
			mSummoned->SetDirection(2);
			mSummoned->SetWorldId(this->GetCaster()->GetWorldId());
			mSummoned->SetInstance(this->GetCaster()->GetInstance());
			mSummoned->AddToWorld();
			success = true;
		}
	}

	this->GetCaster()->MagicAttackSend(MAKE_NUMBERW(summoned_x, summoned_y), this->GetSkill()->GetSkill(), success);
}

void SkillHandler::SkillMonsterInmuneToMagic()
{
	if ( !this->GetCaster()->HasBuff(BUFF_INMUNE_SKILL) && !this->GetCaster()->HasBuff(BUFF_INMUNE_ATTACK) )
	{
		this->GetCaster()->AddBuff(BUFF_INMUNE_SKILL, 10, 0, this->GetCaster());
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}
	
void SkillHandler::SkillMonsterInmuneToHarm()
{
	if ( !this->GetCaster()->HasBuff(BUFF_INMUNE_SKILL) && !this->GetCaster()->HasBuff(BUFF_INMUNE_ATTACK) )
	{
		this->GetCaster()->AddBuff(BUFF_INMUNE_ATTACK, 10, 0, this->GetCaster());
	}

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetCaster()), this->GetSkill()->GetSkill(), true);
}

void SkillHandler::SkillAttackElectricSpark()
{
	coord_type TargetX = this->GetCaster()->GetX() - (8 - (this->GetAngle() & 15));
	coord_type TargetY = this->GetCaster()->GetY() - (8 - ((this->GetAngle() & 240) >> 4));

	int32 angle = this->GetCaster()->GetAngle(TargetX, TargetY);

	this->GetCaster()->SkillAngleCalculate(angle, 1.5f, 6.0f, 1.5f, 0.0f, false);

	Object* mTargetSec;
	bool hit = false;

	VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

		if (!this->GetCaster()->AttackAllowed(mTargetSec->ToUnit()) && mTargetSec != this->GetTarget())
		{
			continue;
		}

		if (!this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()))
		{
			continue;
		}

		this->GetCaster()->AddDelayed(DELAYED_ATTACK, 500, mTargetSec->ToUnit(), this->GetSkill()->GetSkill());
		hit = true;

	VIEWPORT_CLOSE

	if (hit)
	{
		this->SkillAttackElectricSparkReduceHP();
	}
}

void SkillHandler::SkillAttackElectricSparkReduceHP()
{
	if ( !this->GetCaster()->IsPlayer() )
		return;

	Party * pParty = this->GetCaster()->ToPlayer()->GetParty();

	if ( !pParty )
		return;

	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
			continue;

		Player* pPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( !this->GetCaster()->SameDimension(pPlayer) )
			continue;

		if ( !IN_RANGE(this->GetCaster(), pPlayer, 9) )
			continue;

		int32 dec_life = pPlayer->PowerGet(POWER_LIFE) * 20 / 100;
		int32 dec_mana = pPlayer->PowerGet(POWER_MANA) * 5 / 100;

		pPlayer->PowerReduce(POWER_LIFE, dec_life, true);
		pPlayer->PowerReduce(POWER_MANA, dec_mana, true);
	}
}

SpecialSkillHandler::SpecialSkillHandler(Monster* pMonster, uint16 skill, skill_special const* skill_info, Unit* pTarget)
{
	this->SetCaster(pMonster);
	this->SetSkill(skill);
	this->SetSkillInfo(skill_info);
	this->SetTarget(pTarget ? pTarget: this->GetCaster()->GetTarget());
}

void SpecialSkillHandler::Run()
{
	if ( !this->GetTarget() )
		return;

	if ( this->GetSkillInfo()->target == SKILL_TARGET_SELF ||
		 this->GetSkillInfo()->target == SKILL_TARGET_SELF_2 )
	{
		this->SetTarget(this->GetCaster());
	}
	else if ( this->GetSkillInfo()->target == SKILL_TARGET_SELF_GROUP )
	{
		this->SetTarget(this->GetCaster()->FindGroupMemberToHeal());

		if ( !this->GetTarget() )
			this->SetTarget(this->GetCaster());
	}
	
	this->GetCaster()->SpecialMagicSend(this->GetTarget(), this->GetSkill());

	switch ( this->GetSkillInfo()->target.get() )
	{
	case SKILL_TARGET_ENEMY:
		{
			this->RunTarget();
		} break;

	case SKILL_TARGET_SELF:
	case SKILL_TARGET_SELF_2:
		{
			this->RunSelf();
		} break;

	case SKILL_TARGET_ENEMY_GROUP:
		{
			this->RunEnemyGroup();
		} break;

	case SKILL_TARGET_SELF_GROUP:
		{
			this->RunSelfGroup();
		} break;
	}
}

void SpecialSkillHandler::RunTarget()
{
	Object * mTargetSec;

	if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET )
	{
		this->SpecialMagicHit(this->GetTarget());
	}
	else
	{
		int32 angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());

		this->GetCaster()->SkillAngleCalculate(angle, 1.0f, 6.0f, 1.0f, 0.0f, false);

		VIEWPORT_LOOP_OBJECT(this->GetCaster(), mTargetSec)

			if ( !mTargetSec->IsLive() || !mTargetSec->IsPlaying() || !mTargetSec->IsPlayer() )
				continue;

			bool enable_attack = false;

			if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_RANGE )
			{
				if ( Util::Distance(this->GetCaster()->GetX(), this->GetCaster()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) < this->GetSkillInfo()->scope_value.get() )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_AREA )
			{
				if ( this->GetCaster()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET_RANGE )
			{
				if ( Util::Distance(this->GetTarget()->GetX(), this->GetTarget()->GetY(), mTargetSec->GetX(), mTargetSec->GetY()) < this->GetSkillInfo()->scope_value.get() )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET_CHAIN )
			{

			}

			if ( enable_attack )
			{
				this->SpecialMagicHit(mTargetSec->ToUnit());
			}

		VIEWPORT_CLOSE
	}
}
		
void SpecialSkillHandler::RunSelf()
{
	this->SpecialMagicHit(this->GetTarget());
}
	
void SpecialSkillHandler::RunEnemyGroup()
{
	Object * pObject;

	if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET )
	{
		this->SpecialMagicHit(this->GetTarget());
	}
	else
	{
		int32 angle = this->GetCaster()->GetAngle(this->GetTarget()->GetX(), this->GetTarget()->GetY());

		this->GetCaster()->SkillAngleCalculate(angle, 1.0f, 6.0f, 1.0f, 0.0f, false);

		VIEWPORT_LOOP_OBJECT(this->GetCaster(), pObject)

			if ( !pObject->IsLive() || !pObject->IsPlaying() || !pObject->IsPlayer() )
				continue;

			bool enable_attack = false;

			if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_RANGE )
			{
				if ( Util::Distance(this->GetCaster()->GetX(), this->GetCaster()->GetY(), pObject->GetX(), pObject->GetY()) < this->GetSkillInfo()->scope_value.get() )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_AREA )
			{
				if ( this->GetCaster()->SkillInAngle(pObject->GetX(), pObject->GetY()) )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET_RANGE )
			{
				if ( Util::Distance(this->GetTarget()->GetX(), this->GetTarget()->GetY(), pObject->GetX(), pObject->GetY()) < this->GetSkillInfo()->scope_value.get() )
				{
					enable_attack = true;
				}
			}
			else if ( this->GetSkillInfo()->scope_type == SKILL_SCOPE_TARGET_CHAIN )
			{

			}

			if ( enable_attack )
			{
				this->SpecialMagicHit(pObject->ToUnit());
			}

		VIEWPORT_CLOSE
	}
}
	
void SpecialSkillHandler::RunSelfGroup()
{
	this->SpecialMagicHit(this->GetTarget());
}

void SpecialSkillHandler::SpecialMagicHit(Unit* pTarget)
{
	SafeAssert(pTarget, "pTarget == nullptr");

	this->SetTarget(pTarget);

	switch ( this->GetSkill() )
	{
	case SKILL_SPECIAL_GAION_FLAME_STRIKE:
		{
			pTarget->AddBuff(BUFF_FLAME_STRIKE, 1, 0, this->GetCaster());
		} break;

	case SKILL_SPECIAL_GAION_GIGANTIC_STORM:
		{
			pTarget->AddBuff(BUFF_GIGANTIC_STORM, 1, 0, this->GetCaster());
		} break;
	}

	for ( uint8 i = 0; i < MONSTER_SKILL_ELEMENT_MAX; i++ )
	{
		if ( !this->GetSkillInfo()->GetElement(i) )
			continue;

		if (!roll_chance_i(this->GetSkillInfo()->GetElement(i)->rate.get()))
			continue;

		switch ( this->GetSkillInfo()->GetElement(i)->type.get() )
		{
		case SKILL_ELEMENT_STUN:
			{
				this->SpecialSkillElementStun(i);
			} break;

		case SKILL_ELEMENT_MOVE:
			{
				this->SpecialSkillElementMove(i);
			} break;

		case SKILL_ELEMENT_HP:
			{
				this->SpecialSkillElementHp(i);
			} break;

		case SKILL_ELEMENT_MP:
			{
				this->SpecialSkillElementMp(i);
			} break;

		case SKILL_ELEMENT_AG:
			{
				this->SpecialSkillElementAg(i);
			} break;

		/*case SKILL_ELEMENT_SD:
			{
				this->SpecialSkillElementSd(i);
			} break;*/

		case SKILL_ELEMENT_DEFENSE:
			{
				this->SpecialSkillElementDefense(i);
			} break;

		case SKILL_ELEMENT_ATTACK:
			{
				this->SpecialSkillElementAttack(i);
			} break;

		case SKILL_ELEMENT_DURABILITY:
			{
				this->SpecialSkillElementDurability(i);
			} break;

		case SKILL_ELEMENT_SUMMON:
			{
				this->SpecialSkillElementSummon(i);
			} break;

		case SKILL_ELEMENT_PUSH:
			{
				this->SpecialSkillElementPush(i);
			} break;

		case SKILL_ELEMENT_STAT_ENERGY:
			{
				this->SpecialSkillElementStatEnergy(i);
			} break;

		case SKILL_ELEMENT_STAT_STRENGTH:
			{
				this->SpecialSkillElementStatStrength(i);
			} break;

		case SKILL_ELEMENT_STAT_AGILITY:
			{
				this->SpecialSkillElementStatAgility(i);
			} break;

		case SKILL_ELEMENT_STAT_VITALITY:
			{
				this->SpecialSkillElementStatVitality(i);
			} break;

		/*case SKILL_ELEMENT_STAT_LEADERSHIP:
			{
				this->SpecialSkillElementStatLeadership(i);
			} break;*/

		case SKILL_ELEMENT_REMOVE_SKILL:
			{
				this->SpecialSkillElementRemoveSkill(i);
			} break;

		case SKILL_ELEMENT_RESIST_SKILL:
			{
				this->SpecialSkillElementResistSkill(i);
			} break;

		case SKILL_ELEMENT_IMMUNE_SKILL:
			{
				this->SpecialSkillElementInmuneSkill(i);
			} break;

		case SKILL_ELEMENT_TELEPORT_SKILL:
			{
				this->SpecialSkillElementTeleport(i);
			} break;

		case SKILL_ELEMENT_DOUBLE_HP:
			{
				this->SpecialSkillElementDoubleHP(i);
			} break;

		case SKILL_ELEMENT_POISON:
			{
				this->SpecialSkillElementPoison(i);
			} break;

		case SKILL_ELEMENT_NORMALATTACK:
			{
				this->SpecialSkillElementNormalAttack(i);
			} break;

		case SKILL_ELEMENT_INVINCIBLE:
			{
				this->SpecialSkillElementInvincible(i);
			} break;
		}
	}
}

void SpecialSkillHandler::SpecialSkillElementStun(uint8 element)
{
	if ( this->GetSkillInfo()->GetElement(element)->duration == -1 )
	{
		this->GetTarget()->AddBuff(BUFF_STUN, 0, BUFF_FLAG_CONSTANT, this->GetCaster());
	}
	else
	{
		this->GetTarget()->AddBuff(BUFF_STUN, this->GetSkillInfo()->GetElement(element)->duration.get(), 0, this->GetCaster());
	}
}

void SpecialSkillHandler::SpecialSkillElementMove(uint8 element)
{

}

void SpecialSkillHandler::SpecialSkillElementHp(uint8 element)
{
	uint32 value = 0;
	bool increase = true;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type != SKILL_INCDEC_TYPE_NONE )
	{
		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			value = this->GetTarget()->PowerGet(POWER_LIFE) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100.0f;

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC || 
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
				increase = false;
		}
		else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC || 
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC || 
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC || 
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
				increase = false;
		}

		/*if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo )
			{
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoHPTime = this->m_iContinuanceTime;
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoHP = iIncDecValue;

				if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
				else
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
			}
		}*/
	}

	this->GetTarget()->PowerAlter(POWER_LIFE, value, increase);
	this->GetTarget()->UpdatePowers(POWER_LIFE);
}

void SpecialSkillHandler::SpecialSkillElementMp(uint8 element)
{
	uint32 value = 0;
	bool increase = true;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type != SKILL_INCDEC_TYPE_NONE )
	{
		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			value = this->GetTarget()->PowerGet(POWER_MANA) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100.0f;

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
			{
				increase = false;
			}
		}
		else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
			{
				increase = false;
			}
		}

		/*if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo )
			{
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoMPTime = this->m_iContinuanceTime;
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoMP = iIncDecValue;

				if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoMPCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
				else
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoMPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
			}
		}*/
	}

	this->GetTarget()->PowerAlter(POWER_MANA, value, increase);
	this->GetTarget()->UpdatePowers(POWER_MANA);
}
	
void SpecialSkillHandler::SpecialSkillElementAg(uint8 element)
{
	uint32 value = 0;
	bool increase = true;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type != SKILL_INCDEC_TYPE_NONE )
	{
		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			value = this->GetTarget()->PowerGet(POWER_STAMINA) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100.0f;

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
			{
				increase = false;
			}
		}
		else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
			{
				increase = false;
			}
		}

		/*if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo )
			{
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoAGTime = this->m_iContinuanceTime;
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoAG = iIncDecValue;

				if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoAGCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
				else
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoAGCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
			}
		}*/
	}

	this->GetTarget()->PowerAlter(POWER_STAMINA, value, increase);
	this->GetTarget()->UpdatePowers(POWER_STAMINA);
}

void SpecialSkillHandler::SpecialSkillElementSd(uint8 element)
{
	int32 value = 0;
	bool increase = true;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type != SKILL_INCDEC_TYPE_NONE )
	{
		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC ||
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			value = this->GetTarget()->PowerGet(POWER_SHIELD) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100.0f;

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
			{
				increase = false;
			}
		}
		else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC ||
				  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

			if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
				 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
			{
				increase = false;
			}
		}

		/*if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo )
			{
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoSDTime = this->m_iContinuanceTime;
				lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoSD = iIncDecValue;

				if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoSDCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
				else
					lpTargetObj->m_MonsterSkillElementInfo[0].m_iSkillElementAutoSDCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
			}
		}*/
	}
	
	this->GetTarget()->PowerAlter(POWER_SHIELD, value, increase);
	this->GetTarget()->UpdatePowers(POWER_SHIELD);
}

void SpecialSkillHandler::SpecialSkillElementDefense(uint8 element)
{
	if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_NONE )
		return;

	int32 value = 0;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC || 
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC || 
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
	{
		value = this->GetTarget()->GetIntData(UNIT_INT_DEFENSE) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100;

		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
			value = -value;
	}
	else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC || 
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC || 
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
	{
		value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
			value = -value;
	}

	if ( this->GetTarget()->IsCreature() )
	{
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_DEFENSE)->SetValue(value);
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_DEFENSE)->SetTime(this->GetSkillInfo()->GetElement(element)->duration.get());
	}
}
	
void SpecialSkillHandler::SpecialSkillElementAttack(uint8 element)
{
	if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_NONE )
		return;

	int32 value = 0;

	if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTINC || 
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC ||
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTINC || 
		 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
	{
		value = this->GetTarget()->GetIntData(UNIT_INT_DEFENSE) * this->GetSkillInfo()->GetElement(element)->calc_value.get() / 100;

		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_PERCENTDEC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_PERCENTDEC )
			value = -value;
	}
	else if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTINC || 
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC ||
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTINC || 
			  this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
	{
		value = this->GetSkillInfo()->GetElement(element)->calc_value.get();

		if ( this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CONSTANTDEC || 
			 this->GetSkillInfo()->GetElement(element)->calc_type == SKILL_INCDEC_TYPE_CYCLE_CONSTANTDEC )
			value = -value;
	}

	if ( this->GetTarget()->IsCreature() )
	{
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_ATTACK)->SetValue(value);
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_ATTACK)->SetTime(this->GetSkillInfo()->GetElement(element)->duration.get());
	}
}
	
void SpecialSkillHandler::SpecialSkillElementDurability(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementSummon(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementPush(uint8 element)
{
	if ( this->GetCaster() == this->GetTarget() || !this->GetTarget() )
		return;

	if ( this->GetSkillInfo()->GetElement(element)->magic == -1 || this->GetSkillInfo()->GetElement(element)->calc_value == -1 )
	{
		this->GetCaster()->PushBackCount(this->GetTarget(), 3);
	}
	else
	{
		this->GetCaster()->AddDelayed(DELAYED_PUSH_BACK_COUNT, this->GetSkillInfo()->GetElement(element)->magic.get(), this->GetTarget(), this->GetSkillInfo()->GetElement(element)->calc_value.get());
	}
}

void SpecialSkillHandler::SpecialSkillElementStatEnergy(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementStatStrength(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementStatAgility(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementStatVitality(uint8 element)
{

}

void SpecialSkillHandler::SpecialSkillElementStatLeadership(uint8 element)
{

}
	
void SpecialSkillHandler::SpecialSkillElementRemoveSkill(uint8 element)
{
	skill_template const* skill_info = sSkillMgr->GetSkill(this->GetSkillInfo()->GetElement(element)->magic.get());

	if ( !skill_info )
		return;

	this->GetTarget()->RemoveBuff(skill_info->GetBuffIcon());
}
	
void SpecialSkillHandler::SpecialSkillElementResistSkill(uint8 element)
{
	if ( this->GetTarget()->IsCreature() )
	{
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_RESIST_SKILL)->SetValue(this->GetSkillInfo()->GetElement(element)->magic.get());
		this->GetTarget()->ToCreature()->GetSkillElement(SKILL_ELEMENT_RESIST_SKILL)->SetTime(this->GetSkillInfo()->GetElement(element)->duration.get());
	}
}
	
void SpecialSkillHandler::SpecialSkillElementInmuneSkill(uint8 element)
{
	if ( this->GetTarget()->IsCreature() )
	{
		this->GetTarget()->AddBuff(BUFF_INMUNE_SKILL, this->GetSkillInfo()->GetElement(element)->duration.get(), 0, this->GetCaster());
	}
}
	
void SpecialSkillHandler::SpecialSkillElementTeleport(uint8 element)
{
	if ( this->GetCaster()->HasRestrictionBuff() )
		return;

	coord_type depth = RANDOM(4) + 3;
	coord_type x = 0;
	coord_type y = 0;

	if ( RANDOM(2) == 0 )
		x = this->GetCaster()->GetX() + depth;
	else
		x = this->GetCaster()->GetX() - depth;

	if ( RANDOM(2) == 0 )
		y = this->GetCaster()->GetY() + depth;
	else
		y = this->GetCaster()->GetY() - depth;

	FIX_COORD(x);
	FIX_COORD(y);

	if ( !this->GetCaster()->TeleportAreaCheck(x, y) )
		return;

	this->GetCaster()->MagicAttackSend(ENTRY(this->GetTarget()), SKILL_TELEPORT);
	this->GetTarget()->SkillTeleportUse(x, y);
}
	
void SpecialSkillHandler::SpecialSkillElementDoubleHP(uint8 element)
{
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 400, this->GetTarget(), -1);
	this->GetCaster()->AddDelayed(DELAYED_ATTACK, 600, this->GetTarget(), -1);
}
	
void SpecialSkillHandler::SpecialSkillElementPoison(uint8 element)
{
	if ( this->GetTarget()->HasBuff(BUFF_POISON) )
		return;

	if ( this->GetTarget()->ResistElement(Element::POISON) )
		return;

	if ( this->GetSkillInfo()->GetElement(element)->magic == -1 )
	{
		this->GetTarget()->AddBuff(BUFF_POISON, BuffEffect(BUFF_OPTION_NONE, 3), this->GetSkillInfo()->GetElement(element)->duration.get(), 0, this->GetCaster());
	}
	else
	{
		this->GetCaster()->AddDelayed(DELAYED_BUFF_ADD, this->GetSkillInfo()->GetElement(element)->magic.get(), this->GetTarget(), BUFF_POISON, this->GetSkillInfo()->GetElement(element)->duration.get(), 0, 3);
	}
}
	
void SpecialSkillHandler::SpecialSkillElementNormalAttack(uint8 element)
{
	if ( this->GetSkillInfo()->GetElement(element)->magic == -1 )
	{
		this->GetCaster()->AttackProc(this->GetTarget(), nullptr, false);
	}
	else
	{
		this->GetCaster()->AddDelayed(DELAYED_ATTACK, this->GetSkillInfo()->GetElement(element)->magic.get(), this->GetTarget(), -1);
	}
}
	
void SpecialSkillHandler::SpecialSkillElementInvincible(uint8 element)
{
	if ( this->GetSkillInfo()->GetElement(element)->magic == -1 )
	{
		this->GetTarget()->AddBuff(BUFF_INMUNE_SKILL, 0, BUFF_FLAG_CONSTANT, this->GetCaster());
		this->GetTarget()->AddBuff(BUFF_INMUNE_ATTACK, 0, BUFF_FLAG_CONSTANT, this->GetCaster());
	}
	else
	{
		this->GetTarget()->AddBuff(BUFF_INMUNE_SKILL, this->GetSkillInfo()->GetElement(element)->magic.get(), 0, this->GetCaster());
		this->GetTarget()->AddBuff(BUFF_INMUNE_ATTACK, this->GetSkillInfo()->GetElement(element)->magic.get(), 0, this->GetCaster());
	}
}