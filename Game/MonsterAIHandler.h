#ifndef MONSTER_AI_HANDLER_H
#define MONSTER_AI_HANDLER_H

class MonsterAIHandler
{
	public:
		explicit MonsterAIHandler(Monster*);
		void Run();
		bool RunAIUnit();
		bool RunAIAutomata();
		bool ForceAIElement(); 
		void ApplyElementCommon();
		void ApplyElementMove();
		void ApplyElementMoveTarget();
		void ApplyElementGroupMove();
		void ApplyElementAttack();
		void ApplyElementAttackArea();
		void ApplyElementAttackPenetration();
		void ApplyElementAvoid();
		void ApplyElementHealSelf();
		void ApplyElementHealGroup();
		void ApplyElementSpecialSommon();
		void ApplyElementSpecialImmune();
		void ApplyElementNightmareSummon();
		void ApplyElementNightmareWarp();
		void ApplyElementSkillAttack();
		void ApplyElementAIChange();

		DECLARE_PROPERTY_PTR(Monster, Monster);
		DECLARE_PROPERTY_PTR(Unit, Target);
		DECLARE_PROPERTY_PTR(monster_ai_unit const, AIUnit);
		DECLARE_PROPERTY_PTR(monster_ai_automata const, AIAutomata);
		DECLARE_PROPERTY_PTR(monster_ai_state const, AIState);
		DECLARE_PROPERTY_PTR(monster_ai_element const, AIElement);
};

#endif