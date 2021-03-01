/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ai_trap.cpp"
*
*/

class SummonerDebuffScript : public MonsterScriptAI
{
public:
	explicit SummonerDebuffScript() : ScriptAI("summoner_debuff_ai") { }
	virtual ~SummonerDebuffScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new TrapAI(monster); }

	struct TrapAI : public MonsterAI
	{
		explicit TrapAI(Monster* monster) : MonsterAI(monster) { }
		virtual ~TrapAI() {}

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

			if (!this->GetPlayer()->GetSummonerDebuff())
			{
				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetSummonerDebuff() != me())
			{
				me()->Remove();
				return true;
			}

			if (!this->GetPlayer()->IsPlaying() || !this->GetPlayer()->IsLive())
			{
				this->GetPlayer()->SetSummonerDebuff(nullptr);

				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetWorldId() != me()->GetWorldId())
			{
				this->GetPlayer()->SetSummonerDebuff(nullptr);

				me()->Remove();
				return true;
			}

			if (this->GetPlayer()->GetInstance() != me()->GetInstance())
			{
				this->GetPlayer()->SetSummonerDebuff(nullptr);

				me()->Remove();
				return true;
			}

			skill_template const* pSkillInfo = sSkillMgr->GetSkill(this->GetSkill());

			if (!pSkillInfo)
			{
				this->GetPlayer()->SetSummonerDebuff(nullptr);

				me()->Remove();
				return true;
			}

			Object * mTargetSec;

			int32 curse = this->GetPlayer()->GetBookCurseDamage();

			VIEWPORT_LOOP_OBJECT(this->GetPlayer(), mTargetSec)

				Unit* pUnit = mTargetSec->ToUnit();

				if (!pUnit)
				{
					continue;
				}

				if (!this->GetPlayer()->AttackAllowed(pUnit))
				{
					continue;
				}

				if (!SkillHandler::CheckSkillRadio(this->GetPlayer(), this->GetSkill(), me()->GetX(), me()->GetY(), pUnit))
				{
					continue;
				}

				if (pUnit->IsDebuffInmune(false))
				{
					this->GetPlayer()->MissSend(pUnit, nullptr, false, 0);
					continue;
				}

				if (pUnit->ResistElement(sSkillMgr->GetSkillAttribute(pSkillInfo->GetBaseSkill())))
				{
					this->GetPlayer()->MissSend(pUnit, nullptr, false, 0);
					continue;
				}

				if (pUnit->HasBuffGroup(pSkillInfo->GetBuffIcon()))
				{
					continue;
				}

				int32 success_rate = 0;
				int32 duration = 0;

				if (pUnit->IsPlayer())
				{
					success_rate = (this->GetPlayer()->GetEnergy() / 50) + (curse / 6) + 17;
					duration = this->GetPlayer()->GetEnergy() / 300 + (this->GetPlayer()->GetTotalLevel() - pUnit->GetTotalLevel()) / 150 + 5;
				}
				else
				{
					success_rate = (this->GetPlayer()->GetEnergy() / 50) + (curse / 6) + 32;
					duration = this->GetPlayer()->GetEnergy() / 100 + 4 - (pUnit->GetTotalLevel() / 40);
				}

				if (success_rate <= 0 || duration <= 0)
				{
					this->GetPlayer()->MissSend(pUnit, nullptr, false, 0);
					continue;
				}

				if (roll_chance_i(success_rate))
				{
					if (this->GetType() == 0)
					{
						float value_percent = 0;

						if (pUnit->IsPlayer())
						{
							value_percent = (this->GetPlayer()->GetEnergy() / 93) + 3;
						}
						else
						{
							value_percent = (this->GetPlayer()->GetEnergy() / 58) + 4;
						}

						float value = this->GetPlayer()->MagicGetValue(459);
						value_percent += this->GetPlayer()->MagicGetValue(459) / 2;
						float elemental_value = this->GetPlayer()->MagicGetValue(772);
						float elemental_value_percent = this->GetPlayer()->GetMagicTreeLevel(772);

						pUnit->AddBuff(pSkillInfo->GetBuffIcon(),
							BuffEffect(BUFF_OPTION_WEAKNESS_PERCENT, value_percent),
							BuffEffect(BUFF_OPTION_WEAKNESS, value),
							BuffEffect(BUFF_OPTION_WEAKNESS_ELEMENTAL, elemental_value),
							BuffEffect(BUFF_OPTION_WEAKNESS_ELEMENTAL_PERCENT, elemental_value_percent), duration, 0, this->GetPlayer());
					}
					else
					{
						float value_percent = 0;

						if (pUnit->IsPlayer())
						{
							value_percent = (this->GetPlayer()->GetEnergy() / 110) + 12;
						}
						else
						{
							value_percent = (this->GetPlayer()->GetEnergy() / 90) + 20;
						}

						float value = this->GetPlayer()->MagicGetValue(460);
						value_percent += this->GetPlayer()->MagicGetValue(460) / 2;
						float elemental_value = this->GetPlayer()->MagicGetValue(773);
						float elemental_value_percent = this->GetPlayer()->GetMagicTreeLevel(773);

						pUnit->AddBuff(pSkillInfo->GetBuffIcon(),
							BuffEffect(BUFF_OPTION_INNOVATION_PERCENT, value_percent), 
							BuffEffect(BUFF_OPTION_INNOVATION, value), 
							BuffEffect(BUFF_OPTION_INNOVATION_ELEMENTAL, elemental_value),
							BuffEffect(BUFF_OPTION_INNOVATION_ELEMENTAL_PERCENT, elemental_value_percent), duration, 0, this->GetPlayer());
					}
				}
				else
				{
					this->GetPlayer()->MissSend(pUnit, nullptr, false, 0);
				}

			VIEWPORT_CLOSE

			return true;
		}

		void OnDespawn()
		{ 
			if (this->GetPlayer() && this->GetPlayer()->GetSummonerDebuff() == me())
			{
				this->GetPlayer()->SetSummonerDebuff(nullptr);
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

void AddSC_SummonerDebuff()
{
	sScriptAI->AddScriptAI(new SummonerDebuffScript());
}