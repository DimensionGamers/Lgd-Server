class ProtectorOfAcheronBossScript: public MonsterScriptAI
{
public:
	explicit ProtectorOfAcheronBossScript(): ScriptAI("protector_of_acheron_boss") { }
	virtual ~ProtectorOfAcheronBossScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { }
		virtual ~AI() { }

		bool Update()
		{
			if ( sProtectorOfAcheron->GetState() != PROTECTOR_OF_ACHERON_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		void OnDie()
		{
			sProtectorOfAcheron->ReduceBossCount(1);
		}
	};
};

class ProtectorOfAcheronObeliskScript: public MonsterScriptAI
{
public:
	explicit ProtectorOfAcheronObeliskScript(): ScriptAI("protector_of_acheron_obelisk") { }
	virtual ~ProtectorOfAcheronObeliskScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster) { }
		virtual ~AI() { }

		bool Update()
		{
			if ( sProtectorOfAcheron->GetState() != PROTECTOR_OF_ACHERON_STATE_PLAYING )
			{
				me()->Remove();
				return true;
			}

			return false;
		}

		bool PushBackAllowed()
		{
			return false;
		}

		void OnDie()
		{
			sProtectorOfAcheron->SendSuccess();
			sProtectorOfAcheron->SetState_End();
		}
	};
};

void AddSC_ProtectorOfAcheron()
{
	sScriptAI->AddScriptAI(new ProtectorOfAcheronBossScript());
	sScriptAI->AddScriptAI(new ProtectorOfAcheronObeliskScript());
}