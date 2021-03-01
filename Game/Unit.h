/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Unit.h"
*
*/
#ifndef COMMON_OBJECT_H
#define COMMON_OBJECT_H

struct PathPosition
{
	DECLARE_ENUM(coord_type, Ori);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_ENUM(coord_type, Dir);

	void Reset()
	{
		this->SetOri(0);
		this->SetX(0);
		this->SetY(0);
		this->SetDir(0);
	}
};

struct PathData
{
	DECLARE_PROPERTY(int32, Count);
	DECLARE_PROPERTY(int32, Current);
	DECLARE_BOOL(StartEnd);
	DECLARE_ARRAY_STRUCT(PathPosition, Position, MAX_WALK_PATH);
};

struct NovaData
{
	NovaData()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetRunning(false);
		this->SetTick(0);
		this->SetCount(0);
		this->SetSkill(0);
	}

	void Start()
	{
		this->SetRunning(true);
		this->SetTick(GetTickCount());
		this->SetCount(0);
		this->SetSkill(0);
	}

	void UpdateCount()
	{
		this->SetCount((GetTickCount() - this->GetTick()) / 500);
		this->LimitMaxCount(Nova::Max - 1);
	}

	DECLARE_PROPERTY(uint8, Count);
	DECLARE_BOOL(Running);
	DECLARE_ENUM(uint32, Tick);
	DECLARE_ENUM(uint16, Skill);
};

struct BuffEffect
{
	BuffEffect()
	{
		this->Reset();
	}

	BuffEffect(uint8 effect, float value)
	{
		this->Set(effect, value);
	}
	void Set(uint8 effect, float value)
	{
		this->SetEffect(effect);
		this->SetValue(value);
	}
	void Reset()
	{
		this->Set(BUFF_OPTION_NONE, 0);
	}

	DECLARE_ENUM(uint8, Effect);
	DECLARE_FLOAT(Value);
};

struct BuffData
{
	void Reset()
	{
		this->SetDuration(0);
		
		BUFF_EFFECT_LOOP(i)
		{
			this->GetEffect(i)->Reset();
		}

		this->SetID(0);
		this->SetCaster(nullptr);
		this->SetFlag(0);
		this->SetDisabled(true);
	}
	BuffData()
	{
		this->Reset();
	}

	bool IsExpired()
	{
		if ( !this->IsFlag(BUFF_FLAG_EXPIRE) )
			return false;

		return time(nullptr) >= this->GetDuration();
	}

	time_t GetRemainTime()
	{
		if ( this->IsFlag(BUFF_FLAG_EXPIRE) )
		{
			if ( !this->IsExpired() )
			{
				return this->GetDuration() - time(nullptr);
			}

			return 0;
		}

		return this->GetDuration();
	}

	void Update(time_t diff)
	{
		if ( !this->IsFlag(BUFF_FLAG_EXPIRE) )
		{
			if ( this->GetDuration() > diff )
			{
				this->SetDuration(this->GetDuration() - diff);
			}
			else
			{
				this->SetDuration(0);
			}
		}
	}

	float FindEffect(uint8 effect) const
	{
		for (int32 i = 0; i < BUFF_EFFECT_MAX; ++i)
		{
			if (this->GetEffect(i)->GetEffect() == effect)
			{
				return this->GetEffect(i)->GetValue();
			}
		}

		return 0;
	}
	
	DECLARE_ENUM(uint16, ID);
	DECLARE_ENUM(time_t, Duration);
	DECLARE_ARRAY_STRUCT(BuffEffect, Effect, BUFF_EFFECT_MAX);
	DECLARE_PROPERTY_PTR(Unit, Caster);
	DECLARE_FLAG(uint8, Flag);
	DECLARE_BOOL(Disabled);
};

struct UnitDelayedData
{
	UnitDelayedData()
	{
		this->Reset();
	}

	virtual ~UnitDelayedData()
	{
		
	}

	void Reset()
	{
		this->SetID(DELAYED_NONE);
		this->SetTime(0);
		this->SetUnit(nullptr);
		this->raw.data[0] = 0;
		this->raw.data[1] = 0;
		this->raw.data[2] = 0;
		this->raw.data[3] = 0;
		this->raw.data[4] = 0;
	}

	DECLARE_ENUM(DelayedID, ID);
	DECLARE_ENUM(uint32, Time);
	DECLARE_PROPERTY_PTR(class Unit, Unit);
	DECLARE_ENUM(uint16, World);

	union
	{
		struct
		{
			int32 count;
		} push_back;

		struct
		{
			int32 skill;
			int32 damage;
			int32 damage_type;
			int32 combo;
			int32 count;
		} damage;

		struct
		{
			int32 id;
			int32 duration;
			int32 effect;
			int32 value;
			int32 flag;
		} buff;

		struct
		{
			int32 action;
		} dark_spirit;

		struct
		{
			int32 value;
		} life_recover;

		struct
		{
			int32 range;
			int32 x;
			int32 y;
			int32 skill;
			int32 skill_monster;
		} damage_range;

		struct
		{
			int32 x;
			int32 y;
			int32 range;
			int32 count;
		} ferea_mine;

		struct
		{
			int32 x;
			int32 y;
			int32 range;
		} swamp_of_darkness_skill;

		struct
		{
			int32 x;
			int32 y;
			int32 range;
			int32 count;
		} swamp_of_darkness_pet;

		struct
		{
			int32 skill;
			int32 count;
			int32 x;
			int32 y;
		} multi_skill;

		struct
		{
			int32 skill;
			int32 count;
			int32 x;
			int32 y;
			int32 angle;
		} multi_skill_angle;

		struct
		{
			int32 data[5];
		} raw;
	};
};

typedef std::list<UnitDelayedData*> DelayedDataList;

extern int32 mercenary_count;

typedef std::map<uint32, uint32> UnitPunishMap;

class Unit: public Object
{
	non_copyable(Unit);

	public:
		virtual ~Unit();

		virtual void Init();

		virtual void sendPacket(uint8 * packet, uint16 len) { }
		virtual void sendPacket(uint8 * packet) { }
		virtual void SendPacket(C1_HEADER * packet) { }
		virtual void SendPacket(C2_HEADER * packet) { }
		virtual void SendPacket(GamePacket * packet) { }

		bool IsTeleporting() const { return this->GetTeleportStatus() != TELEPORT_NONE; }
		bool IsInTeleportInmuneState() const;
		bool IsInTeleportRemoveViewportState() const;

		virtual bool IsUnitAttackable() const { return true; }

		virtual int16 GetTotalLevel() const { return 1; }

		bool isCastleSiegeAttacker() const { return this->GetCastleSiegeJoinSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_1 ||
													this->GetCastleSiegeJoinSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_2 ||
													this->GetCastleSiegeJoinSide() == CASTLE_SIEGE_JOIN_SIDE_ATTACK_3; }

		bool isCastleSiegeDefender() const { return this->GetCastleSiegeJoinSide() == CASTLE_SIEGE_JOIN_SIDE_DEFENSE; }
		
		DECLARE(int32, power[POWER_MAX]);
		DECLARE(int32, power_max[POWER_MAX]);
		DECLARE(int32, power_add[POWER_MAX]);

		DECLARE_ARRAY_STRUCT(Skill, Skill, MAX_SKILL);
		DECLARE_ARRAY_STRUCT(BuffData, BuffData, MAX_BUFF);

		DECLARE_STRUCT(NovaData, Nova);
		DECLARE_STRUCT(GenData, Gen);
		
		void SendCustomObjectData();

		void ActionSend(uint16 target, uint8 action, uint8 direction, bool me = false);
		void PositionSend(coord_type x, coord_type y);
		void EffectSend(uint8 effect);
		void StateInfoSend(uint16 buff, uint8 state);
		void StateInfoSendSingle(Player* pPlayer, uint16 buff, uint8 state);

		void SetLocation(coord_type x, coord_type y, uint8 direction);
		void UpdateLastLocation();
		void UpdateLastLocation(world_type world_id, coord_type x, coord_type y, int32 instance, uint8 direction);
		virtual void UpdateDelayedTeleport();
		void CancelDelayedTeleport();
		
		virtual int32 GetStrength() const { return 1; }
		virtual int32 GetAgility() const { return 1; }
		virtual int32 GetVitality() const { return 1; }
		virtual int32 GetEnergy() const { return 1; }
		virtual int32 GetLeadership() const { return 1; }

		virtual int16 GetLevel() const = 0;

		virtual void UpdatePowers(uint8 type, bool max = false) = 0;

		virtual bool MoveToGate(uint16 gate);
		virtual void GetValidCoordinates(uint16 gate, world_type & world, coord_type & x, coord_type & y);

		int32 MagicAdd(uint16 skill, uint8 level, bool weapon = false);
		int32 MagicRemove(uint16 skill);
		bool MagicFind(uint16 skill, uint8 level = 0) const;
		Skill * MagicGet(uint16 skill);
		Skill const* MagicGet(uint16 skill) const;
		Skill * MagicGet(uint16 skill, int32 & position);
		virtual uint8 GetMagicTreeLevel(uint16 skill) const { return 0; }
		virtual float MagicGetValue(uint16 skill, bool majestic = false) { return 0.0f; };
		void MagicCancel(uint16 skill);
		
		virtual void KillSummoned(bool reset = true);
		virtual void SendSummonedHP(int32 hp, int32 max_hp) {}
		void SetSummonedTarget(Unit* mTarget);
		bool IsSummoned() { return this->GetSummoner() != this; }

		bool IsUsingShield()
		{ 
			return (GetTickCount() - this->GetShieldTick()) < (4 * IN_MILLISECONDS);
		}

		virtual void TeleportToLocation(world_type world, coord_type x, coord_type y, uint8 direction, int32 instance); // Lo mueve a una zona definida
		virtual void TeleportToLocation(world_type world);
		virtual void TeleportToLocation(); // Lo mueve al ultimo lugar donde estuvo
		virtual void TeleportToObject(Unit* pUnit);
		virtual bool TeleportAreaCheck(coord_type x, coord_type y);
		virtual void StartRegen(world_type world, coord_type x, coord_type y, uint8 direction, int32 instance, uint16 gate_id = 0) { }

		static uint8 GetPathPacketDirPos(coord_type px, coord_type py);

		bool IsRest() const { return this->GetRest() != 0; }

		bool IsInDragonTower() const;
		static bool IsInDragonTower(coord_type x1, coord_type y1, coord_type x2, coord_type y2);

		virtual int32 GetBookHitCount() const { return 5; }

		void FireworksSend(uint8 count = 1);

		void ApplyDamageDecrease(int32 & damage);

		void ReflectDamage(int32 damage, Unit* pVictim);
		void TotalPowerRecovery();

		virtual void AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag);
		void Kill();
		void Hit(Unit* pAttacker, int32 damage, int32 shield_damage, uint16 damage_type, bool damage_send = true);
		
		void KillCountSend();
		void KillCountAdd(uint8 count);
		void KillCountDecrease(uint8 count);
		void KillCountReset();

		void GenerateRandomLocation(World* pWorld, coord_type &x, coord_type &y, int32 length, uint8 exclude, int32 instance);

		int32 PowerGetTotal(uint8 id) const;
		int32 PowerGet(uint8 id) const;
		int32 PowerGetMax(uint8 id) const;
		int32 PowerGetAdd(uint8 id) const;
		void PowerSet(uint8 id, int32 value, bool send = false);
		void PowerSetMax(uint8 id, int32 value);
		void PowerSetAdd(uint8 id, int32 value);
		uint8 PowerGetPercent(uint8 id) const;
		void PowerReduce(uint8 id, int32 value, bool send = false);
		void PowerIncrease(uint8 id, int32 value, bool send = false);
		void PowerAlter(uint8 id, int32 value, bool increase);
		bool PowerHas(uint8 id, int32 value);
		void PowerReduceAdd(uint8 id, int32 value, bool compare = true);
		void PowerIncreseAdd(uint8 id, int32 value, bool compare = true);

		

		bool CommonMovementCheck();
		// BUFF PROCEDURES
		void UpdateBuffStatus();

		void ApplyDebuffDamage(BuffData * pBuffData);

		bool AddItemBuff(uint16 item, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true, bool from_use = false);
		void AddBuff(uint16 id, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		void AddBuff(uint16 id, BuffEffect effect_1, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		void AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		void AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		void AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, BuffEffect effect_4, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		void AddBuff(uint16 id, BuffEffect effect_1, BuffEffect effect_2, BuffEffect effect_3, BuffEffect effect_4, BuffEffect effect_5, time_t duration, uint8 flags, Unit * Caster = nullptr, bool group_remove = true);
		virtual void ApplyBuffValues(uint8 opt, float value);
		virtual void ClearBuffValues(uint8 opt, float value);
		bool HasBuff(uint16 id) const;
		time_t GetBuffTime(uint16 id);
		time_t GetBuffGroupTime(uint16 id);
		bool HasBuffGroup(uint16 id) const;
		BuffData const* GetBuffByGroup(uint16 id) const;
		void BuffDataRemove(BuffData * data);
		void buffClearGroup(uint16 group);
		void RemoveBuff(uint16 id, bool group_remove = true, bool from_add = false);
		void RemoveBuff(buff_template const * buff_info, bool group_remove = true, bool from_add = false);
		void RemoveNonSelfBuff();
		float GetBuffEffect(uint16 id) const;
		float GetEffect(uint8 effect) const;
		bool ReplaceBuff(uint16 id, float value_1 = 0.0f, float value_2 = 0.0f, float value_3 = 0.0f, float value_4 = 0.0f, float value_5 = 0.0f) const;
		void ReduceBuffEffect(uint16 id, uint8 effect, float value);
		void IncreaseBuffEffect(uint16 id, uint8 effect, float value);
		void ReduceBuffTime(uint16 id, time_t time);
		void SetBuffTime(uint16 id, time_t time);
		virtual void ClearAllBuff(uint8 flag);
		void ApplyBuffInstantEffect(uint16 id);
		bool HasRestrictionBuff() const;
		bool HasSlowdownBuff() const;
		bool HasTeleportRestrictionBuff() const;
		bool HasTeleportCoordinateBuff() const;
		uint8 GetActiveBuffCount() const;
		void ClearDebuff(int32 count);
		BuffData const* GetBuff(uint16 id) const;

		
		void ApplyBerserkerDamage(int32 & damage_min, int32 & damage_max);
		
		bool IsDebuffInmune(bool elemental) const;
	// ATTACK PROCEDURES
		void AttackProc(Unit * mTarget, Skill * mSkill, bool magic_send = false, int32 damage = 0, uint8 combo = 0, uint16 damage_type = 0, int32 count = 0);
		int32 DealDamage(Unit* pVictim, int32 damage, int32 shield_damage, uint16 skill = 0, bool elemental = false, MonsterThreatDamageType type = MONSTER_THREAT_DAMAGE_NORMAL, uint8 extra = 0);
		int32 GetShieldDamage(Unit* mTarget, int32 damage, uint16 skill = 0, bool elemental = false);
		int32 GetShieldDamageMonster(Unit* mTarget, int32 damage);
		bool AttackAllowed(Unit * mTarget, bool targeted = false, bool reflect = false);
		void PushBackSimple(Unit* mTarget);
		void PushBackCount(Unit* mTarget, uint8 count);
		bool PushBackAllowed();
		bool PushBackCheck(coord_type & x, coord_type & y, int32 & direction);

		bool MissCheck(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all);
		bool MissCheckNormal(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all);
		bool MissCheckPVP(Unit * pTarget, Skill* pSkill, bool send, int32 count, bool & miss_all);
		virtual bool ConsumeSkillItem() { return true; }
		bool TargetResistance(Unit * mTarget, uint16 skill);

		bool ResistElement(uint8 type);

		int32 GetTargetDefense(Unit * mTarget, uint16 & ignore_defense, uint8 attack_type);
		int32 GetAttackDamage(int32 t_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget);
		int32 GetMagicDamage(int32 t_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget);
		int32 GetFenrirAttackDamage(int32 t_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget);
		int32 GetMajesticSpecialDamage(int32 t_defense, uint16 & damage_type, Skill const* mSkill, Unit* pTarget);
		
		void SkillAngleCalculate(float angle, float tx, float ty, float sx, float sy, bool convert = true, int32 add_x = -1, int32 add_y = -1);
		bool SkillInAngle(coord_type x, coord_type y);
		int32 GetAngle(coord_type x, coord_type y);

		virtual void GetPartyMembers(Unit* mMember[MAX_PARTY_MEMBERS], uint8 & count, uint8 distance);

		void MagicAttackSend(uint16 target, uint16 skill, bool success = true);
		void NewMagicAttackSend(uint16 target, uint16 skill, bool success = true);
		void SendDamage(Unit *mTarget, int32 life_damage, int32 shield_damage, uint8 interrupt, uint16 damage_type, uint8 special = 0);

		virtual void SkillGetDamage(uint16 skill, int32 & damage_min, int32 & damage_max);
		void RunNova();
		void SkillTeleportUse(coord_type x, coord_type y);
		void UpdateTeleport();
		
		void MissSend(Unit* pTarget, Skill* pSkill, bool send, int32 count, uint16 effect = 0);

		void ApplySkillCountEffect(Unit* pTarget, uint16 skill, int32 count, uint16 & effect);

		bool CriticalDamageChance(Unit* pTarget, bool elemental, int32 & damage, int32 damage_max);
		bool ExcellentDamageChance(Unit* pTarget, bool elemental, int32 & damage, int32 damage_max);
		bool IgnoreDefenseChance(Unit* pTarget, bool elemental);
		bool DoubleDamageChance(Unit* pTarget, bool elemental, int32 & damage);
		bool TripleDamageChance(Unit* pTarget, bool elemental, int32 & damage);

		void ProcessReflect(Unit* Target, int32 damage);

		bool AttackElemental(Unit* pTarget, Skill* pSkill, bool magic_send, int32 damage, int32 count, bool combo);
		bool MissCheckElemental(Unit* pTarget, uint16 skill_base, bool& miss, int32 count);
		int32 GetTargetElementalDefense(Unit* pTarget, uint16 & effect);
		int32 GetAttackDamageElemental(Unit* pTarget, Skill* pSkill, uint16 & effect, int32 attack_damage, int32 target_defense);
		void SendDamageElemental(Unit *pTarget, uint8 elemental_attribute, int32 damage);

		void GetManaShieldValue(float & value, int32 & duration);

		void AdditionalDamage(Unit* pTarget, uint16 skill, int32 damage, uint16 damage_type);

		void DebuffBlowAttack(Unit* pTarget, Skill * pSkill);
	// DELAYED
		void UpdateDelayed();
		void AddDelayed(DelayedID id, uint32 delay, Unit * mUnit = nullptr, int32 data_1 = 0, int32 data_2 = 0, int32 data_3 = 0, int32 data_4 = 0, int32 data_5 = 0);

		virtual bool IsOnDuel() const { return false; }
		virtual bool IsOnDuel(Player* pPlayer) { return false; }

		DECLARE_STRUCT(ObjectLocation, RegenLocation);
		DECLARE_STRUCT(ObjectLocation, DelayedLocation);

		virtual void UpdateMovement() = 0;
		virtual void UpdateStatus() = 0;

		void Die();

		bool SendViewportData() const;

		virtual void Respawn() = 0;

		void UpdateOldPower();
		void ApplyOldPower();

		void MoveSend(coord_type x, coord_type y, uint8 dir);

		virtual bool InmuneToRadiance() const = 0;
		virtual bool InmuneToPunish(Player* pPlayer) = 0;

		std::string BuildPath() const;

		virtual float GetAbsorbDamageRate() const;
		virtual float GetIncreaseDamageRate() const;

		virtual int32 GetBookCurseDamage() const { return 0; }

		BuffData const* GetSolidProtectionBuff() const;
	protected:
		explicit Unit(uint16 entry);

		DECLARE_ENUM(RegenStatus, RegenStatus);

		DECLARE_PROPERTY(coord_type, TX);
		DECLARE_PROPERTY(coord_type, TY);

		DECLARE_BOOL(DelayedTeleport);
		DECLARE_PROPERTY(int16, DelayedTeleportTime);

		DECLARE_PROPERTY_ARRAY(uint8, Resistance, Element::MAX);
		DECLARE_PROPERTY_ARRAY(uint8, ResistanceAdd, Element::MAX);
		DECLARE_ENUM(TeleportStatus, TeleportStatus);
		DECLARE_ENUM(uint32, TeleportTime);
		DECLARE_ENUM(uint8, ViewState);
		DECLARE_PROPERTY_PTR(Unit, Summoner);
		DECLARE_PROPERTY_PTR(Unit, Summoned);
		DECLARE_ENUM(uint8, Rest);
		DECLARE_ENUM(uint8, Action);
		DECLARE_PROPERTY(uint8, CastleSiegeJoinSide);
		DECLARE_PROPERTY(uint8, KillCount);
		DECLARE_BOOL(ForceUpgrade);
		DECLARE_PROPERTY_ARRAY(coord_type, SkillAngleX, MAX_ARRAY_FRUSTRUM);
		DECLARE_PROPERTY_ARRAY(coord_type, SkillAngleY, MAX_ARRAY_FRUSTRUM);
		DECLARE_STRUCT(PathData, PathData);
		DECLARE_PROPERTY_PTR(Unit, Killer);

		DECLARE_PROPERTY_ARRAY_ENUM(int32, IntData, UNIT_INT_MAX, UnitIntFieldData);
		DECLARE_PROPERTY_ARRAY_ENUM(float, FloatData, UNIT_FLOAT_MAX, UnitFloatFieldData);

		/*NUEVO*/
		DelayedDataList delayed_data;
		DECLARE_STRUCT(TickTimer, MovePathTime);
		DECLARE_STRUCT(TickTimer, MoveLastTime);
		DECLARE_ENUM(uint32, ShieldTick);
		DECLARE_PROPERTY_ARRAY(int32, OldPower, POWER_MAX);		
		
		void ClearPathData(bool update = false);

		DECLARE_ENUM(uint8, ElementalAttribute);
		DECLARE_PROPERTY(int32, ElementalPattern);
		DECLARE_PTR(Unit, MiningIndex);
		DECLARE_PROPERTY(uint8, MiningStage);

		DECLARE_PTR(Unit, LastAttacker);

		// Almacena las coordenadas hacia donde tiene intencion de moverse
		DECLARE_ENUM(coord_type, TempX);
		DECLARE_ENUM(coord_type, TempY);

		UnitPunishMap punish_map;
};

#define NAME_S(p) if((p)!=nullptr){delete (p);(p)=nullptr;}

#define NAME_SAFE(u) ( (u)? u->GetName(): "")

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec34_t[3][4];

void InitFrustrum();
//void VectorRotate (const float in1[3*4],const float in2[3*4],float out[3*4]);
//void AngleMatrix (const float *angles, float (*matrix)[4] );

void VectorRotate (const vec3_t in1, const float in2[3][4], vec3_t out);
void AngleMatrix (const vec3_t angles, float matrix[3][4] );

extern int32 FrustrumX[MAX_ARRAY_FRUSTRUM];
extern int32 FrustrumY[MAX_ARRAY_FRUSTRUM];

#endif