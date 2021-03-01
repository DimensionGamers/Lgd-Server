class QuestCentScript: public MonsterScriptAI
{
public:
	explicit QuestCentScript(): ScriptAI("cent_quest_ai") { }
	virtual ~QuestCentScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_PTR(Player, Player);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			
		}

		virtual ~AI() {}

		bool Update()
		{
			Quest4thInstanceData const* pData = sQuestMgr->GetQuest4thInstanceData(me()->GetInstance());
			
			if ( !pData )
			{
				me()->Remove();
				return true;
			}

			if ( pData->GetState() != 1 )
			{
				me()->Remove();
				return true;
			}

			return true;
		}
	};
};

class QuestMonsterCentScript: public MonsterScriptAI
{
public:
	explicit QuestMonsterCentScript(): ScriptAI("cent_monster_quest_ai") { }
	virtual ~QuestMonsterCentScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		DECLARE_ENUM(uint8, NovaState);

		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->SetNovaState(0);
			this->CreateTimer(0);
		}

		virtual ~AI() {}

		bool Update()
		{
			Quest4thInstanceData const* pData = sQuestMgr->GetQuest4thInstanceData(me()->GetInstance());

			if ( !pData )
			{
				me()->Remove();
				return true;
			}

			if ( pData->GetState() != 3 )
			{
				me()->Remove();
				return true;
			}

			switch ( this->GetNovaState() )
			{
			case 1:
				{
					if ( this->IsTimerElapsed(0, 500) )
					{
						this->NovaCast();
					}

					return true;
				} break;

			case 2:
				{
					if ( this->IsTimerElapsed(0, 2000) )
					{
						this->NovaExplode();
					}

					return true;
				} break;
			}

			return false;
		}

		void OnDie()
		{
			Quest4thInstanceData * pData = sQuestMgr->GetQuest4thInstanceData(me()->GetInstance());

			if ( !pData )
			{
				return;
			}

			if ( pData->GetState() != 3 )
			{
				return;
			}

			sQuestMgr->Change4thQuestInstanceState(pData->GetInstance(), 0, 0);
		}

		bool Drop()
		{
			return true;
		}

		bool OnAttack(Unit*, bool common)
		{
			uint8 skill_id = 0;

			Quest4thInstanceData * pData = sQuestMgr->GetQuest4thInstanceData(me()->GetInstance());

			if ( pData )
			{
				if ( pData->GetQuestID() == 9 )
				{
					skill_id = Random(6);
				}
				else
				{
					skill_id = Random(3);
				}
			}

			switch ( skill_id )
			{
			case 0:
				{
					this->NormalHit();
				} break;

			case 1:
				{
					this->FireSlash();
				} break;

			case 2:
				{
					this->MultiTwister();
				} break;

			case 3:
				{
					this->NovaStart();
				} break;

			case 4:
				{
					this->FireBlood();
				} break;

			case 5:
				{
					this->IceBlood();
				} break;
			}
		
			
			/*
			124 = Pega
			125 = Fire Slash
			126 = Twister Multiple
			127 = Nova Start
			128 = Nova Cast
			129 = Nova Explode
			130 = Fire Blood
			131 = Ice Blood
			*/

			return true;
		}

		void NormalHit()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(2000);

			me()->SpecialMagicSend(me()->GetTarget(), 124);

			me()->AddDelayed(DELAYED_ATTACK, 500, me()->GetTarget());
		}

		void FireSlash()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 125);

			float angle = me()->GetAngle(me()->GetTarget()->GetX(), me()->GetTarget()->GetY());
			me()->SkillAngleCalculate(angle, 2.0f, 4.0f, 5.0f, 0.0f, false);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !me()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
				{
					continue;
				}

				if ( !mTargetSec->ToUnit() )
				{
					continue;
				}

				if ( Monster* pMonster = mTargetSec->ToCreature() )
				{
					if ( !pMonster->GetSummoner() )
					{
						continue;
					}

					if ( pMonster->GetSummoner() == pMonster )
					{
						continue;
					}

					if ( !pMonster->GetSummoner()->IsPlayer() )
					{
						continue;
					}
				}

				me()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit());

			VIEWPORT_CLOSE
		}

		void MultiTwister()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 126);

			int32 angle = me()->GetAngle(me()->GetTarget()->GetX(), me()->GetTarget()->GetY());

			me()->SkillAngleCalculate(angle * 255 / 360, 6.0f, 6.0f, 1.0f, 0.0f);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !mTargetSec->ToUnit() )
				{
					continue;
				}

				if ( Monster* pMonster = mTargetSec->ToCreature() )
				{
					if ( !pMonster->GetSummoner() )
					{
						continue;
					}

					if ( pMonster->GetSummoner() == pMonster )
					{
						continue;
					}

					if ( !pMonster->GetSummoner()->IsPlayer() )
					{
						continue;
					}
				}

				me()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit());

			VIEWPORT_CLOSE
		}

		void NovaStart()
		{
			if ( this->GetNovaState() != 0 )
			{
				return;
			}

			this->SetNovaState(1);
			this->StartTimer(0);

			me()->SetNextActionTime(1000);
			me()->SpecialMagicSend(me()->GetTarget(), 127);
		}

		void NovaCast()
		{
			if ( this->GetNovaState() != 1 )
			{
				return;
			}

			this->SetNovaState(2);
			this->StartTimer(0);

			me()->SpecialMagicSend(me()->GetTarget(), 128);
		}

		void NovaExplode()
		{
			if ( this->GetNovaState() != 2 )
			{
				return;
			}

			this->SetNovaState(0);

			me()->SpecialMagicSend(me()->GetTarget(), 129);

			me()->SetNextActionTime(2000);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !IN_RANGE(me(), mTargetSec, 6) )
				{
					continue;
				}
				
				if ( !mTargetSec->ToUnit() )
				{
					continue;
				}

				if ( Monster* pMonster = mTargetSec->ToCreature() )
				{
					if ( !pMonster->GetSummoner() )
					{
						continue;
					}

					if ( pMonster->GetSummoner() == pMonster )
					{
						continue;
					}

					if ( !pMonster->GetSummoner()->IsPlayer() )
					{
						continue;
					}
				}

				me()->AttackProc(mTargetSec->ToUnit(), nullptr);

			VIEWPORT_CLOSE
		}

		void FireBlood()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 130);

			float angle = me()->GetAngle(me()->GetTarget()->GetX(), me()->GetTarget()->GetY());
			me()->SkillAngleCalculate(angle, 2.0f, 4.0f, 5.0f, 0.0f, false);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !me()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
				{
					continue;
				}

				if ( !mTargetSec->ToUnit() )
				{
					continue;
				}

				if ( Monster* pMonster = mTargetSec->ToCreature() )
				{
					if ( !pMonster->GetSummoner() )
					{
						continue;
					}

					if ( pMonster->GetSummoner() == pMonster )
					{
						continue;
					}

					if ( !pMonster->GetSummoner()->IsPlayer() )
					{
						continue;
					}
				}

				me()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit());

			VIEWPORT_CLOSE
		}

		void IceBlood()
		{
			if ( !me()->GetTarget() )
			{
				return;
			}

			me()->SetNextActionTime(3000);

			me()->SpecialMagicSend(me()->GetTarget(), 131);

			float angle = me()->GetAngle(me()->GetTarget()->GetX(), me()->GetTarget()->GetY());
			me()->SkillAngleCalculate(angle, 2.0f, 4.0f, 5.0f, 0.0f, false);

			Object * mTargetSec;

			VIEWPORT_LOOP_OBJECT(me(), mTargetSec)

				if ( !me()->SkillInAngle(mTargetSec->GetX(), mTargetSec->GetY()) )
				{
					continue;
				}

				if ( !mTargetSec->ToUnit() )
				{
					continue;
				}

				if ( Monster* pMonster = mTargetSec->ToCreature() )
				{
					if ( !pMonster->GetSummoner() )
					{
						continue;
					}

					if ( pMonster->GetSummoner() == pMonster )
					{
						continue;
					}

					if ( !pMonster->GetSummoner()->IsPlayer() )
					{
						continue;
					}
				}

				me()->AddDelayed(DELAYED_ATTACK, 1000, mTargetSec->ToUnit());

			VIEWPORT_CLOSE
		}
	};
};

class QuestMonster4thScript: public MonsterScriptAI
{
public:
	explicit QuestMonster4thScript(): ScriptAI("4th_monster_quest_ai") { }
	virtual ~QuestMonster4thScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster)
		{
			
		}

		virtual ~AI() {}

		bool Update()
		{
			Quest4thInstanceData const* pData = sQuestMgr->GetQuest4thInstanceData(me()->GetInstance());

			if ( !pData )
			{
				me()->Remove();
				return true;
			}

			if ( pData->GetState() != 3 )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		bool Drop()
		{
			return true;
		}
	};
};

void AddSC_QuestCentScript()
{
	sScriptAI->AddScriptAI(new QuestCentScript());
	sScriptAI->AddScriptAI(new QuestMonsterCentScript());
	sScriptAI->AddScriptAI(new QuestMonster4thScript());
}