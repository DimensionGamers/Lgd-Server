class NPCScript : public MonsterScriptAI
{
public:
	explicit NPCScript() : ScriptAI("special_event_map_ai") { }
	virtual ~NPCScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		explicit AI(Monster* monster) : MonsterAI(monster)
		{

		}
		virtual ~AI()
		{

		}

		bool OnTalk(Player* pPlayer)
		{
			if (!pPlayer)
			{
				return true;
			}

			World* pWorld = sWorldMgr->GetWorld(WORLD_SPECIAL_EVENT_MAP);
			if (!pWorld)
			{
				return true;
			}

			int32 count = pWorld->GetPlayersCountByPC(pPlayer);

			if (count == -1 || count >= sGameServer->GetSpecialMapCharacterCount())
			{
				pPlayer->SendMessageBox(1, "Special Event Map", "You can't move more characters to the special map.");
				return true;
			}

			pPlayer->MoveToGate(10001);
			return true;
		}
	};
};

void AddSC_SpecialMapNPC()
{
	sScriptAI->AddScriptAI(new NPCScript());
}