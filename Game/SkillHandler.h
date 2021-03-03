#ifndef SKILL_HANDER_H
#define SKILL_HANDER_H

class SkillHandler
{
	public:
		explicit SkillHandler(Unit* pCaster, Unit* pTarget, Skill * pSkill, skill_template const* skill_info = nullptr, int16 x = 0, int16 y = 0, int32 count = 0, int32 angle = 0);
		virtual ~SkillHandler() {}

		bool SkillUseProc(bool combo = false, int16 x = 0, int16 y = 0, uint8 dir = 0, uint8 dis = 0, uint8 pos = 0, bool duration = false);

		void SkillShieldDefense();
		void SkillBuffManaShield();
		void SkillBuffHeal();
		void SkillBuffGreaterDefense();
		void SkillBuffGreaterAttack();
		void SkillBuffGreaterFortitude();
		void SkillSummon();
		void SkillBuffCriticalDamage();

		void SkillBuffStun();
		void SkillBuffCancelStun();
		void SkillBuffSwellMana();
		void SkillBuffInvisibility();
		void SkillBuffCancelInvisibility();
		void SkillBuffCancelMagic();
		
		void SkillBuffInfinityArrow();
		void SkillBuffDamageReflection();
		void SkillBuffBerserker();
		void SkillBuffDarkness();
		void SkillBuffSleep();
		void SkillBuffWeakness();
		void SkillBuffInnovation();
		void SkillBuffExpansionOfWizardy();
		void SkillBuffRecovery();
		void SkillBuffRedArmorIgnore();
		void SkillBuffFitness();
		void SkillBuffDefenseSuccessRate();

		void SkillSummonMonster(uint16 monster);

		void SkillAttackToLocation();

		void SkillAttackPowerSlash();
		void SkillAttackTwistingSlash();
		void SkillAttackRagefulBlow();
		void SkillAttackDeathStab();
		void SkillAttackFireBurst();
		void SkillAttackDarkHorse();
		void SkillAttackChaoticDiseier();
		void SkillAttackFlameStrike();
		void SkillAttackStrikeOfDestruction();
		void SkillAttackEvilSpirit();
		void SkillAttackHellFire();
		void SkillAttackInferno();
		void SkillAttackFlame();
		void SkillAttackTripleShot();
		void SkillAttackMultiShot();
		void SkillAttackLightningShock();
		void SkillAttackChainLightning();
		void SkillAttackDragonLore();
		void SkillAttackDrainLife();
		void SkillAttackNova();
		void SkillAttackFenrir();
		void SkillAttackFlameOfEvil();
		void SkillAttackDopplegangerSuicide();
		void SkillAttackExplosion();
		void SkillAttackRequiem();
		void SkillAttackPollution();
		void SkillAttackGiganticStorm();
		void SkillAttackForce();
		void SkillAttackMultiSingle();
		void SkillAttackDarkSide();
		void SkillAttackCharge();
		void SkillAttackPhoenixShot();
		void SkillBlast();
		void SkillAquaBeam();
		void SkillPenetration();
		void SkillFireSlash();
		void SkillAttackFireScream();
		void SkillAttackFireScreamExplosion(int32 damage);
		void SkillCrescentMoonSlash();
		void SkillManaGlaive();
		void SkillStarfall();
		void SkillSpiralSlash();
		void SkillManaRays();
		void SkillFireBlast();

		void SkillBloodStorm();
		void SkillCure();
		void SkillPartyHeal();
		void SkillBless();
		void SkillBlind();
		void SkillEarthPrison();
		void SkillIronDefense();
		void SkillBloodHowling();

		void SkillSpinStep();
		void SkillSpinStepExplosion(int32 damage);
		void SkillCircleShield();
		void SkillObsidian();
		void SkillMagicPin();
		void SkillMagicPinExplosion(int32 damage);
		void SkillClash();
		void SkillHarshStrike();
		void SkillShiningPeak();
		void SkillWrath();
		void SkillBreche();
		void SkillBurst();
		void SkillArchangelsWill();
		void SkillIllusion();
		void SkillShadowStep();
		void SkillEvasion();
		void SkillRush();
		void SkillFireBlow();
		void SkillSwordBlow();
		void SkillMeteorStrike();
		void SkillMeteorStorm();
		void SkillSoulSeeker();
		void SkillFocusShot();
		void SkillFireBeast();
		void SkillAquaBeast();
		void SkillIceBlood();
		void SkillFireBlood();
		void SkillDarkBlast();
		void SkillEvilWind();
		void SkillDarkPhoenixShot();
		void SkillSpiritHook();
		void SkillDeathside();
		void SkillDeathsideInitial();

		void EliteMonsterSkill();

		void SkillRadianceInmuneI();
		void SkillRadianceInmuneII();
		void SkillRadianceWidenedI();

		void SkillPoisonStorm();
		void SkillPoisonStormSplash(int32 damage);
		void SkillFrozenSlaughter();
		void SkillFrozenSlaughterSplash(int32 damage);
		void SkillBloodyingHit();
		void SkillBloodyingHitSplash(int32 damage);
		void SkillShockBoom();

		void SkillMagicArrow();
		void SkillMagicArrowSplash(int32 damage);
		void SkillPlasmaBall();
		void SkillLightningStorm();
		void SkillLightningStormSplash(bool delay = false, Unit* pTarget = nullptr);
		void SkillBurstRW();
		void SkillHasteRW();

		void SkillSwordInertia();
		void SkillBatFlock();
		void SkillPierceAttack();
		void SkillDetection();
		void SkillDemolish();

		void SkillTwister();
		
		void SkillSwordWrath();
		void SkillStrongBelief();
		void SkillSolidProtection();

		void SkillDarkPlasma();
		void SkillIceBreak();
		void SkillIceBlast();
		void SkillFireDeath();
		void SkillBurstingFlare();

		void SendMultiTarget(std::set<uint16> const& list);

		void SkillDelayed();

		void SkillMonsterSummon();
		void SkillMonsterInmuneToMagic();
		void SkillMonsterInmuneToHarm();
		void SkillAttackElectricSpark();
		void SkillAttackElectricSparkReduceHP();

		bool SkillBuffFree();
		void GetPartyMembers(uint8 distance = 0);

		bool CheckSkillRange(int16 x, int16 y, Unit* pTarget);
		bool CheckSkillRadio(int16 x, int16 y, Unit* pTarget);
		static bool CheckSkillRange(Unit* pUnit, uint16 skill, int16 x, int16 y, Unit* pTarget);
		static bool CheckSkillRadio(Unit* pUnit, uint16 skill, int16 x, int16 y, Unit* pTarget);
		static int32 GetSkillRange(Unit* pUnit, uint16 skill);
		static int32 GetSkillRadio(Unit* pUnit, uint16 skill);

		DECLARE_PROPERTY_PTR(Unit, Caster);
		DECLARE_PROPERTY_PTR(Skill, Skill);
		DECLARE_PROPERTY_PTR(skill_template const, SkillInfo);
		DECLARE_PROPERTY_PTR(Unit, Target);
		DECLARE_ENUM(int16, AttackX);
		DECLARE_ENUM(int16, AttackY);
		DECLARE_ENUM(uint8, Angle);
		DECLARE_ENUM(uint8, SpecialValue);
		DECLARE_BOOL(Combo);
		DECLARE_ENUM(int32, Count);

		Unit *PartyMember[MAX_PARTY_MEMBERS];
		uint8 party_member_count;
};

class SpecialSkillHandler
{
	public:
		explicit SpecialSkillHandler(Monster* pMonster, uint16 skill, skill_special const* skill_info, Unit* pTarget);
		void Run();
		void RunTarget();
		void RunSelf();
		void RunEnemyGroup();
		void RunSelfGroup();
		void SpecialMagicHit(Unit* pTarget);

		void SpecialSkillElementStun(uint8 element);
		void SpecialSkillElementMove(uint8 element);
		void SpecialSkillElementHp(uint8 element);
		void SpecialSkillElementMp(uint8 element);
		void SpecialSkillElementAg(uint8 element);
		void SpecialSkillElementSd(uint8 element);
		void SpecialSkillElementDefense(uint8 element);
		void SpecialSkillElementAttack(uint8 element);
		void SpecialSkillElementDurability(uint8 element);
		void SpecialSkillElementSummon(uint8 element);
		void SpecialSkillElementPush(uint8 element);
		void SpecialSkillElementStatEnergy(uint8 element);
		void SpecialSkillElementStatStrength(uint8 element);
		void SpecialSkillElementStatAgility(uint8 element);
		void SpecialSkillElementStatVitality(uint8 element);
		void SpecialSkillElementStatLeadership(uint8 element);
		void SpecialSkillElementRemoveSkill(uint8 element);
		void SpecialSkillElementResistSkill(uint8 element);
		void SpecialSkillElementInmuneSkill(uint8 element);
		void SpecialSkillElementTeleport(uint8 element);
		void SpecialSkillElementDoubleHP(uint8 element);
		void SpecialSkillElementPoison(uint8 element);
		void SpecialSkillElementNormalAttack(uint8 element);
		void SpecialSkillElementInvincible(uint8 element);

		DECLARE_PROPERTY_PTR(Monster, Caster);
		DECLARE_PROPERTY_PTR(Unit, Target);
		DECLARE_ENUM(uint16, Skill);
		DECLARE_PROPERTY_PTR(skill_special const, SkillInfo);

};

#endif