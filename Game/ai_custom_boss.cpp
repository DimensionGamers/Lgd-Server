class BossScript : public MonsterScriptAI
{
public:
	explicit BossScript() : ScriptAI("custom_boss_ai") { }
	virtual ~BossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->StartTimer(0);
			this->StartTimer(1, 15000);
			this->StartTimer(2, 2000);
		}

		virtual ~AI()
		{

		}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			this->PushBackPlayers(true);

			if (this->IsTimerElapsed(0, sGameServer->GetCustomBossSummonTime()) && !me()->IsViewportEmpty())
			{
				this->Summon();
			}

			if (this->IsTimerElapsed(1))
			{
				this->PushBackAll();
			}

			if (this->IsTimerElapsed(2))
			{
				this->AOEAll();
			}

			return false;
		}

		void PushBackAll()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

			if (!pObject->IsUnit())
				continue;

			if (pObject->IsInSafeZone())
				continue;

			if (pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator())
				continue;

			if (pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()))
				continue;

			if (!IN_RANGE(me(), pObject, 10))
				continue;

			me()->PushBackCount(pObject->ToUnit(), 2);
			pObject->ToUnit()->AddBuff(BUFF_STUN, 3, 0, me());

			VIEWPORT_CLOSE

				me()->MagicAttackSend(ENTRY(me()), SKILL_COMBO);
		}

		void AOEAll()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

			if (!pObject->IsUnit())
				continue;

			if (pObject->IsInSafeZone())
				continue;

			if (pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator())
				continue;

			if (pObject->IsCreature() && (!pObject->ToCreature()->GetSummoner() || !pObject->ToCreature()->GetSummoner()->IsPlayer()))
				continue;

			if (!IN_RANGE(me(), pObject, 10))
				continue;

			me()->AttackProc(pObject->ToUnit(), nullptr, false, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 8 / 100, 0, DAMAGE_TYPE_DEBUFF);
			pObject->ToUnit()->AddBuff(BUFF_LIGHTNING_SHOCK, 1, 0, me());

			VIEWPORT_CLOSE
		}

		bool PushBackAllowed()
		{
			return false;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool InmuneToDebuffBlow(bool blow) const
		{
			if (blow)
			{
				if (sGameServer->IsBossDebuffBlow(2))
				{
					return true;
				}
			}
			else
			{
				if (sGameServer->IsBossDebuffBlow(1))
				{
					return true;
				}
			}

			return false;
		}

		bool EnableAttack(Unit* pUnit, Skill*, int32 count)
		{
			if (!pUnit)
			{
				return false;
			}

			if (!me()->CheckWall(pUnit))
			{
				return false;
			}

			return true;
		}

		void OnDie()
		{
			me()->LogKillData(true);
		}

		bool Drop()
		{
			sItemBagMgr->ExecuteItemBag(ITEM_BAG_MONSTER_ALL, me(), "CustomBossDrop");

			return true;
		}

		void Summon()
		{
			for (int32 i = 0; i < sGameServer->GetCustomBossSummonCount(); ++i)
			{
				Monster* pMonster = sObjectMgr->MonsterTryAdd(me()->GetClass() + 1, me()->GetWorldId());

				if (pMonster)
				{
					pMonster->SetBasicLocation(me()->GetX() - 8, me()->GetY() - 8, me()->GetX() + 8, me()->GetY() + 8);
					pMonster->SetWorldId(me()->GetWorldId());
					pMonster->SetInstance(me()->GetInstance());
					pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
					pMonster->SetDespawnTime(sGameServer->GetCustomBossSummonDespawnTime());
					pMonster->SetDespawnTick(MyGetTickCount());
					pMonster->SetDespawnType(MONSTER_DESPAWN_DIE);
					pMonster->AddAdditionalDataPtr(0, me());
					pMonster->AddToWorld();
				}
			}
		}
	};
};

class BossSummonScript : public MonsterScriptAI
{
public:
	explicit BossSummonScript() : ScriptAI("custom_boss_minion_ai") { }
	virtual ~BossSummonScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_PTR(Monster, Boss);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			this->StartTimer(0, 2 * IN_MILLISECONDS);
		}

		virtual ~AI()
		{

		}

		bool Update()
		{
			if (!this->GetBoss())
			{
				me()->Remove();
				return true;
			}

			if (!this->GetBoss()->IsPlaying())
			{
				me()->Remove();
				return true;
			}

			if (this->IsTimerElapsed(0) && !me()->IsTeleporting())
			{
				World* pWorld = me()->GetWorld();
				int32 distance = Util::Distance(me()->GetX(), me()->GetY(), this->GetBoss()->GetX(), this->GetBoss()->GetY());

				if (distance > 10 && pWorld)
				{
					coord_type x = this->GetBoss()->GetX();
					coord_type y = this->GetBoss()->GetY();

					pWorld->GetFreeLocation(x, y, 3, 3, 10);

					me()->MagicAttackSend(ENTRY(me()), SKILL_TELEPORT);
					me()->SkillTeleportUse(x, y);
					return true;
				}
			}

			return false;
		}

		void OnCreate()
		{
			this->SetBoss((Monster*)me()->m_AdditionalDataPtr[0]);
		}

		bool MoveAllowed(coord_type x, coord_type y)
		{
			if (!MonsterAI::MoveAllowed(x, y))
				return false;

			if (!this->GetBoss())
				return false;

			int32 distance = Util::Distance(me()->GetX(), me()->GetY(), this->GetBoss()->GetX(), this->GetBoss()->GetY());

			return distance <= 10;
		}

		bool Drop()
		{
			sItemBagMgr->ExecuteItemBag(ITEM_BAG_MONSTER_ALL, me(), "CustomBossMinionDrop");

			return true;
		}
	};
};

void AddSC_CustomBoss()
{
	sScriptAI->AddScriptAI(new BossScript());
	sScriptAI->AddScriptAI(new BossSummonScript());
}