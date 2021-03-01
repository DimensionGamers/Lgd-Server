/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_trap.cpp"
*
*/

class DebuffScript : public MonsterScriptAI
{
public:
	explicit DebuffScript() : ScriptAI("majestic_debuff_ai") { }
	virtual ~DebuffScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		explicit AI(Monster* monster) : MonsterAI(monster) { }
		virtual ~AI() {}

		DECLARE_PTR(Player, Player);
		DECLARE_ENUM(uint8, Type);
		DECLARE_ENUM(uint16, Skill);

		bool Update()
		{
			me()->SetNextActionTime(me()->GetIntData(UNIT_INT_ATTACK_SPEED));

			if (!this->GetPlayer())
			{
				me()->Remove();
				return true;
			}

			if (!this->GetPlayer()->GetMajesticDebuff())
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetMajesticDebuff() != me())
			{
				me()->Remove();
				return true;
			}

			if (!this->GetPlayer()->IsPlaying() || !this->GetPlayer()->IsLive())
			{
				this->GetPlayer()->SetMajesticDebuff(nullptr);

				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetWorldId() != me()->GetWorldId())
			{
				this->GetPlayer()->SetMajesticDebuff(nullptr);

				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetInstance() != me()->GetInstance())
			{
				this->GetPlayer()->SetMajesticDebuff(nullptr);

				me()->Remove();
				return true;
			}

			skill_tree_majestic const* pSkillInfo = sSkillMgr->GetSkillTreeMajestic(this->GetPlayer()->GetClass(), this->GetSkill());

			if (!pSkillInfo)
			{
				this->GetPlayer()->SetMajesticDebuff(nullptr);

				me()->Remove();
				return true;
			}

			int32 targets = 7 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_POISONING_CIRCLE_1_TARGETS);
			int32 targets_upgrade = 7 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_POISONING_CIRCLE_2_TARGETS);
			int32 range = 7 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_POISONING_CIRCLE_RANGE);
			int32 duration = 10;

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(this->GetPlayer(), mTargetSec)

			if (!this->GetPlayer()->AttackAllowed(mTargetSec->ToUnit()))
			{
				continue;
			}

			Monster* pMonster = mTargetSec->ToCreature();

			if (!pMonster)
			{
				continue;
			}

			MonsterAI const* pAI = pMonster->GetAI();

			if (pAI && pAI->InmuneToDebuffBlow(false))
			{
				continue;
			}

			if (me()->Distance(pMonster->GetX(), pMonster->GetY()) > range)
			{
				continue;
			}

			switch (this->GetType())
			{
			case 0:
			{
					  if (!pMonster->HasBuff(BUFF_POISONING) && !pMonster->HasBuff(BUFF_DEADLY_POISONING))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_POISONING_SUCCESS_RATE);
						  if (targets > 0 && debuff_rate <= 0)
						  {
							  int32 effect = 2000 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_POISONING_EFFECT);

							  pMonster->AddBuff(BUFF_POISONING, BuffEffect(BUFF_OPTION_NONE, effect), duration, 0, this->GetPlayer());
							  --targets;
						  }
					  }
					  else if (pMonster->HasBuff(BUFF_POISONING) && !pMonster->HasBuff(BUFF_DEADLY_POISONING))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_DEADLY_POISONING_SUCCESS_RATE);
						  if (targets_upgrade > 0 && debuff_rate <= 0)
						  {
							  float rate = this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_POISONING_CIRCLE_DEBUFF_UPGRADE_RATE);
							  if (rate > 0 && roll_chance_f(rate))
							  {
								  int32 effect = 2000 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_DEADLY_POISONING_EFFECT);

								  pMonster->RemoveBuff(BUFF_POISONING);
								  pMonster->AddBuff(BUFF_DEADLY_POISONING, BuffEffect(BUFF_OPTION_NONE, effect), duration, 0, this->GetPlayer());
								  --targets_upgrade;
							  }
						  }
					  }
			}
			break;

			case 1:
			{
					  if (!pMonster->HasBuff(BUFF_CHILLING) && !pMonster->HasBuff(BUFF_FREEZING))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_CHILLING_SUCCESS_RATE);
						  if (targets > 0 && debuff_rate <= 0)
						  {
							  int32 effect = 20 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_CHILLING_EFFECT);

							  pMonster->AddBuff(BUFF_CHILLING, BuffEffect(BUFF_OPTION_DEFENSE_REDUCE, effect), duration, 0, this->GetPlayer());
							  --targets;
						  }
					  }
					  else if (pMonster->HasBuff(BUFF_CHILLING) && !pMonster->HasBuff(BUFF_FREEZING))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_FREEZING_SUCCESS_RATE);
						  if (targets_upgrade > 0 && debuff_rate <= 0)
						  {
							  float rate = this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_CHILLING_CIRCLE_DEBUFF_UPGRADE_RATE);
							  if (rate > 0 && roll_chance_f(rate))
							  {
								  int32 effect = 20 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_FREEZING_EFFECT);

								  pMonster->RemoveBuff(BUFF_CHILLING);
								  pMonster->AddBuff(BUFF_FREEZING, BuffEffect(BUFF_OPTION_DEFENSE_REDUCE, effect), duration, 0, this->GetPlayer());
								  --targets_upgrade;
							  }
						  }
					  }
			}
			break;

			case 2:
			{
					  if (!pMonster->HasBuff(BUFF_BLEEDING) && !pMonster->HasBuff(BUFF_HEMORRHAGE))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_BLEEDING_SUCCESS_RATE);
						  if (targets > 0 && debuff_rate <= 0)
						  {
							  int32 effect = 2000 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_BLEEDING_EFFECT);

							  pMonster->AddBuff(BUFF_BLEEDING, BuffEffect(BUFF_OPTION_NONE, effect), duration, 0, this->GetPlayer());
							  --targets;
						  }
					  }
					  else if (pMonster->HasBuff(BUFF_BLEEDING) && !pMonster->HasBuff(BUFF_HEMORRHAGE))
					  {
						  int32 debuff_rate = pMonster->GetIntData(UNIT_INT_DEBUFF_DEFENSE) - this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_HEMORRHAGE_SUCCESS_RATE);
						  if (targets_upgrade > 0 && debuff_rate <= 0)
						  {
							  float rate = this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_BLEEDING_CIRCLE_DEBUFF_UPGRADE_RATE);
							  if (rate > 0 && roll_chance_f(rate))
							  {
								  int32 effect = 2000 + this->GetPlayer()->GetMajesticDataValue(pSkillInfo->GetSection(), MAJESTIC_DEBUFF_HEMORRHAGE_EFFECT);

								  pMonster->RemoveBuff(BUFF_BLEEDING);
								  pMonster->AddBuff(BUFF_HEMORRHAGE, BuffEffect(BUFF_OPTION_NONE, effect), duration, 0, this->GetPlayer());
								  --targets_upgrade;
							  }
						  }
					  }
			}
			break;
			}
			
			VIEWPORT_CLOSE

			return true;
		}

		void OnDespawn()
		{
			if (this->GetPlayer() && this->GetPlayer()->GetMajesticDebuff() == me())
			{
				this->GetPlayer()->SetMajesticDebuff(nullptr);
			}
		}

		void OnCreate()
		{
			this->SetPlayer((Player*)me()->m_AdditionalDataPtr[0]);
			this->SetType(me()->m_AdditionalDataInt[0]);
			this->SetSkill(me()->m_AdditionalDataInt[1]);
		}
	};
};

void AddSC_MajesticDebuff()
{
	sScriptAI->AddScriptAI(new DebuffScript());
}