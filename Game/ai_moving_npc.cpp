class MovingNPCScript: public MonsterScriptAI
{
public:
	explicit MovingNPCScript(): ScriptAI("ai_moving_npc") { }
	virtual ~MovingNPCScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI: public MonsterAI
	{
		explicit AI(Monster* monster): MonsterAI(monster)
		{
			this->CreateTimer(0);
		}
		virtual ~AI()
		{

		}

		bool Update()
		{
			if ( me()->GetNpcFunction().empty() )
			{
				me()->SetNpcFunction(this->GetCurrentShop());
			}

			if ( this->IsTimerElapsed(0) )
			{
				if ( sMonsterMgr->TeleportToRandomLocation(me(), 5) )
				{
					me()->SetNpcFunction(this->GetCurrentShop());
					this->StartTimer(0, sGameServer->GetSecretShopWarpTime());
					return true;
				}
				else
				{
					this->StartTimer(0);
				}
			}

			return false;
		}

		bool MoveAllowed(coord_type x, coord_type y)
		{
			return me()->GetWorld()->GetGrid(x, y).IsSummonedMoveAllowed();
		}

		std::string GetCurrentShop()
		{
			if ( sGameServer->GetSecretShopList().empty() )
			{
				return "";
			}

			std::string shop_name;
			std::istringstream stream(sGameServer->GetSecretShopList().c_str());

			RandomValue<std::string> m_RandomValue("");

			while (stream)
			{
				stream >> shop_name;

				if ( sShopMgr->GetShop(shop_name) )
				{
					m_RandomValue.AddValue(shop_name, 0);
				}
			}

			return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
		}
	};
};

void AddSC_MovingNPC()
{
	sScriptAI->AddScriptAI(new MovingNPCScript());
}