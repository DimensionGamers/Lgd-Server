/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Monster.h"
*
*/

#ifndef MONSTER_H
#define MONSTER_H

enum EmotionType
{
	EMOTION_REST,
	EMOTION_ATTACK,
	EMOTION_MOVE,
	EMOTION_ESCAPE
};

struct ActionData
{
	public:
		uint8 Rest;
		uint8 Attack;
		uint8 Move;
		uint8 Escape;

		uint8 Emotion;
		int8 EmotionCount;

		void Reset()
		{
			this->Rest = 0;
			this->Attack = 0;
			this->Move = 0;
			this->Escape = 0;
			this->Emotion = EMOTION_REST;
			this->EmotionCount = 0;
		}

		void Update()
		{
			if ( this->EmotionCount > 0 )
			{
				this->EmotionCount--;
			}
			else
			{
				this->Emotion = EMOTION_REST;
			}
		}
};

struct monster_template;
struct skill_special;
struct skill_special_element;
struct monster;
struct MonsterAI;

class Monster: public Unit
{
	non_copyable(Monster);

	friend struct MonsterAI;

	public:
		explicit Monster(uint16 entry);
		virtual ~Monster();

		void reset();

		bool IsMonster() const { return this->GetSendType() == MAIN_OBJECT_MONSTER; }
		bool IsNpc() const { return this->GetSendType() == MAIN_OBJECT_NPC; }
		bool IsTrap() const {  return this->GetSendType() == MAIN_OBJECT_TRAP; }
		bool IsFurniture() const {  return this->GetSendType() == MAIN_OBJECT_FURNITURE; }
		bool IsNeutral() const {  return this->GetSendType() == MAIN_OBJECT_FRIENDLY_TARGET; }
		
		bool IsUnitAttackable() const { return !(this->IsNpc() || this->IsTrap() || this->IsFurniture());  }

		void SetNameData(const char* name, ...);

		int16 GetTotalLevel() const { return this->GetLevel(); }

		void SetBasicLocation(coord_type x1, coord_type y1, coord_type x2, coord_type y2);

		bool IsDelayed() { return this->GetBattleDelay() > 0; }

		bool IsGuardianStatue() const { return this->GetClass() == 283; }
		bool IsCastleSiegeGate() const { return this->GetClass() == 277; }
		bool IsWolfStatue() const { return this->GetClass() == 204; }
		bool IsWolfAltar() const { return (this->GetClass() >= 205 && this->GetClass() <= 209); }
		bool IsBalgass() const { return this->GetClass() == 349; }
		bool IsDarkElf() const { return this->GetClass() == 340; }

		bool IsPriestDevin() const { return this->GetClass() == 406; }
		bool IsWerewolfQuarrel() const { return this->GetClass() == 407; }

		bool IsKundun() const { return (this->GetClass() == 275); }
		bool IsBloodCastleDoor() const { return (this->GetClass() == 131); }

		bool HasTalkReference() const;
		void AddTalkReference(Player* pPlayer);
		void UpdateTalkReference();
		void UpdateLive();
		void Respawn();
		void UpdateStatus();

		void MakeRespawnLocation(bool random = true);

		bool SetTemplateInfo();
		void SetDBData();
		void SetDBData(monster const* data);
		void SetEventDBData(monster_event const* data);
		
		void UpdateMovement() override;
		void MoveAttempt();
		bool MoveCheck(coord_type x, coord_type y);
		bool IsOutOfMoveRange();
		bool PathFindToMove();
		void PathProcess(uint8 * path);
		void MoveToLocation(coord_type x, coord_type y, bool target = false);
		void SetMoveCoordinates(coord_type x, coord_type y);
		bool SetMoveCoordinatesNormal(coord_type x, coord_type y);
		bool IsMoveCoordinates(coord_type x, coord_type y);
				
		bool IsMoveAllowed(coord_type x, coord_type y, bool target);
		void TeleportBack();
		bool MoveBack();

		/* Spawn Data */
		void UpdateSpawn();
		void UpdateDespawn();

		void PowerRefill();
		void ReactionUpdate();

		bool FollowOwner();

		void Attack();
		bool MagicAttack();
		void NormalAttack();
		bool SpecialMagicAttack();
		bool SpecialMagicAttack(uint16 skill, int16 type = -1, Unit* pTarget = nullptr);
		void SpecialMagicSend(Unit* mTarget, uint16 skill);
		
		Skill * GetRandomSkill();

		void AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag) override;

		void ProcessHit(Unit* pAttacker);
	// AGGRO
		void CalculateMaxAttacker();
		
		Player * GetMaxAggro();

		void KillExperience();

		void DropItem();
		void DropZen();

		int32 GetTargetDistance() { return Util::Distance(this->GetX(), this->GetY(), this->GetTarget()->GetX(), this->GetTarget()->GetY()); }

		DECLARE_STRUCT(ActionData, Action);
		
		bool IsAIGroup(std::string const& ss);
		MonsterAI * AIGroup(std::string const& ss);
		MonsterAI const* AIGroup(std::string const& ss) const;
		void AISet(MonsterAI* pAI);
		void GenerateDirection();

		void Say(const char * msg, ...);
		void SayTo(Unit* pUnit, const char * msg, ...);

		void AlterData(float size, const char * name);
		void BuildName();

		float GetElementValue(uint8 element);

		bool GetOwnerPosition(coord_type &tx, coord_type &ty);
		bool GetTargetPosition(coord_type sx, coord_type sy, coord_type &tx, coord_type &ty);
		bool GetTargetPosition();
		void FindEnemy();

		void AddAdditionalDataPtr(uint32 id, void* Data)
		{
			m_AdditionalDataPtr[id] = Data;
		}

		void AddAdditionalDataInt(uint32 id, int64 Data)
		{
			m_AdditionalDataInt[id] = Data;
		}

		void AddAdditionalDataFloat(uint32 id, float Data)
		{
			m_AdditionalDataFloat[id] = Data;
		}

		bool SendHPBar();

		void SendDirection();

		bool ViewportIsCharacter(Unit* pTarget);
		void UpdatePowers(uint8 type, bool max = false) override;

		bool Update(uint32 diff) override;
		bool IsViewportControl() const;
		bool IsStopAction() const;

		void SetRespawn(uint32 time);

		void LogKillData(bool all, bool save_db = true);

		void TeleportToRegen();

		bool InmuneToRadiance() const;
		bool InmuneToPunish(Player* pPlayer);

		int32 GetPlayerViewportCount() const;
	protected:
		DECLARE_PROPERTY(uint16, Class);
		DECLARE_ENUM(uint16, Model);
		DECLARE_STRING_FIXED(Name, MAX_MONSTER_NAME_LENGTH);
		DECLARE_PROPERTY(int32, MoveDistance);
		DECLARE_ENUM(RespawnType, RespawnType);
		DECLARE_ENUM(MonsterDespawn, DespawnType);
		DECLARE_PROPERTY(uint8, RespawnDistance);
		DECLARE_ENUM(uint8, MoveRange);
		DECLARE_PROPERTY_STRING(NpcFunction);
		DECLARE_PROPERTY_STRING(ItemBag);
		DECLARE_PROPERTY_STRING(ScriptName);
		DECLARE_PROPERTY(int32, AttackRange);
		DECLARE_PROPERTY(int32, ViewRange);
		DECLARE_BOOL(Custom);
		DECLARE_PROPERTY_PTR(Player, MaxAttacker);
		DECLARE_ENUM(uint32, MaxAttackerGUID);
		DECLARE_STRING_FIXED(MaxAttackerName, MAX_CHARACTER_LENGTH + 1);
		DECLARE_ENUM(int16, Level);
		DECLARE_ENUM(int32, ZenRate);
		DECLARE_ENUM(int32, ItemRate);
		DECLARE_ENUM(RespawnLocation, RespawnLocation);
		DECLARE_PROPERTY_PTR(Unit, Target);
		DECLARE_PROPERTY(coord_type, TargetX);
		DECLARE_PROPERTY(coord_type, TargetY);
		DECLARE_PROPERTY(int32, TargetHitCount);
		DECLARE_PROPERTY_ARRAY(coord_type, SpawnX, 2);
		DECLARE_PROPERTY_ARRAY(coord_type, SpawnY, 2);
		DECLARE_BOOL(Spawning);
		DECLARE_ENUM(TCType, SpawnTick);
		DECLARE_ENUM(uint32, SpawnTime);
		DECLARE_ENUM(TCType, DespawnTick);
		DECLARE_ENUM(uint32, DespawnTime);
		DECLARE_PROPERTY(uint32, CurrentAI);
		DECLARE_PROPERTY(uint32, BasicAI);
		DECLARE_PROPERTY(uint8, OrderAI);
		DECLARE_PROPERTY(uint32, LastAIRunTime);
		DECLARE_PROPERTY(uint32, LastAIAutomataRunTime);
		DECLARE_PROPERTY(int32, LastAIAutomataDelay);
		DECLARE_ENUM(uint8, CurrentAIState);
		DECLARE_PROPERTY(uint32, GroupAI);
		DECLARE_PROPERTY(uint32, GroupMemberAI);
		DECLARE_ENUM(time_t, LastUpdate);
		
		DECLARE_PROPERTY(int32, Zen);
		
		DECLARE_PROPERTY_ARRAY(uint16, SkillSpecial, MAX_MONSTER_SPECIAL_SKILL);
		DECLARE_ARRAY_STRUCT(SkillElement, SkillElement, SKILL_ELEMENT_MAX);

		DECLARE_PROPERTY_ARRAY(float, RegenPower, POWER_MAX);
		DECLARE_PROPERTY_ARRAY(uint32, RefillTime, POWER_MAX);
		DECLARE_PROPERTY_ARRAY(TCType, RefillTick, POWER_MAX);
		DECLARE_ENUM(TCType, RegenPowerTime);

		typedef std::set<Player*> TalkReferenceList;
		TalkReferenceList talk_reference_list;

		DECLARE_PTR(MonsterAI, AI);

		typedef std::map<uint32, void*> AdditionalDataMapPtr;
		typedef std::map<uint32, int64> AdditionalDataMapInt;
		typedef std::map<uint32, float> AdditionalDataMapFloat;
		
		AdditionalDataMapPtr m_AdditionalDataPtr;
		AdditionalDataMapInt m_AdditionalDataInt;
		AdditionalDataMapFloat m_AdditionalDataFloat;

		DECLARE_STRUCT(MonsterThreat, ThreatManager);
		
		bool HasSpecialSkill(uint16 skill);
		void AddSpecialSkill(uint16 skill);
		uint16 GetRandomSpecialSkill();

		bool RunAI();
	
		Monster* FindGroupMemberToSummon();
		Monster* FindGroupMemberToHeal();

		bool GetXYToPatrol();
		bool GetXYToEscape();
		bool GetXYToChase();

		void ChangeAIOrder(uint8 order);

		// Almacena las coordenadas justo en el momento donde es interrumpido
		DECLARE_ENUM(uint32, RespawnID);
		DECLARE_ENUM(uint8, OriginalDirection);

		DECLARE_PROPERTY_ARRAY(int32, ScriptMaxPower, POWER_MAX);
		DECLARE_PROPERTY(int32, BattleDelay);

		DECLARE_ENUM(uint32, CurActionTime);
		DECLARE_ENUM(uint32, NextActionTime);
		DECLARE_ENUM(uint32, DelayActionTime);

		DECLARE_ENUM(TCType, SummonTick);
		DECLARE_ENUM(uint32, AITick);

		DECLARE_ENUM(uint8, SummonType);
		DECLARE_PTR(Player, SummonPlayer);

		DECLARE_ENUM(uint8, ScriptElementalAttribute);

		DECLARE_RANGE(uint32, RespawnTimeRange);

		DECLARE_ENUM(TCType, LastMoveTime);

		DECLARE_ENUM(TCType, LastActionTime);
		DECLARE_ENUM(TCType, LastHitTime);

		DECLARE_PROPERTY(uint8, KillID);

		DECLARE_STRUCT(TickTimer, TeleportOutOfRangeTime);

		DECLARE_ENUM(uint8, RadianceImmune);

		DECLARE_BOOL(Elite);
		DECLARE_ENUM(uint8, DamageAbsorb);
};

#endif