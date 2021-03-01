void AddSC_Trap();
void AddSC_Summon();
void AddSC_Guard();
void AddSC_PersonalNPC();
void AddSC_MossMerchant();
void AddSC_SoccerBall();
void AddSC_Kundun();
void AddSC_KalimaGate();
void AddSC_CastleSiege();
void AddSC_Raklion();
void AddSC_ChaosCastle();
void AddSC_Invasion();
void AddSC_DevilSquare();
void AddSC_BloodCastle();
void AddSC_Kanturu();
void AddSC_Doppelganger();
void AddSC_ImperialFortress();
void AddSC_Crywolf();
void AddSC_Champion();
void AddSC_Medusa();
void AddSC_ProtectorOfAcheron();
void AddSC_UrukMountain();
void AddSC_Nars();
void AddSC_Ferea();
void AddSC_TormentedSquare();
void AddSC_Evomon();
void AddSC_ArkaWar();
void AddSC_GroundDarkness();
void AddSC_NixiesLake();
void AddSC_CastleDeep();
void AddSC_LabyrinthOfDimensions();
void AddSC_WorldBoss();
void AddSC_QuestCentScript();
void AddSC_MovingNPC();
void AddSC_SwampOfDarkness();
void AddSC_InstancedDungeonNPC();
void AddSC_SummonerDebuff();
void AddSC_MajesticDebuff();
void AddSC_CustomBoss();
void AddSC_SpecialMapNPC();

ScriptAIMgr::ScriptAIMgr()
{

}

ScriptAIMgr::~ScriptAIMgr()
{
	LIST_CLEAR(ScriptAIList::iterator, this->script_ai_list);
}

void ScriptAIMgr::LoadScriptAI()
{
	sLog->outInfo(LOG_DEFAULT, "Loading AI Scripts...");

	/*******************************************
				Monster/Npc SCRIPTS
	*******************************************/

	AddSC_Trap();
	AddSC_Summon();
	AddSC_Guard();
	AddSC_PersonalNPC();
	AddSC_MossMerchant();
	AddSC_SoccerBall();
	AddSC_Kundun();
	AddSC_KalimaGate();
	AddSC_CastleSiege();
	AddSC_Raklion();
	AddSC_ChaosCastle();
	AddSC_Invasion();
	AddSC_DevilSquare();
	AddSC_BloodCastle();
	AddSC_Kanturu();
	AddSC_Doppelganger();
	AddSC_ImperialFortress();
	AddSC_Crywolf();
	AddSC_Champion();
	AddSC_Medusa();
	AddSC_ProtectorOfAcheron();
	AddSC_UrukMountain();
	AddSC_Nars();
	AddSC_Ferea();
	AddSC_TormentedSquare();
	AddSC_Evomon();
	AddSC_ArkaWar();
	AddSC_GroundDarkness();
	AddSC_NixiesLake();
	AddSC_CastleDeep();
	AddSC_LabyrinthOfDimensions();
	AddSC_WorldBoss();
	AddSC_QuestCentScript();
	AddSC_MovingNPC();
	AddSC_SwampOfDarkness();
	AddSC_InstancedDungeonNPC();
	AddSC_SummonerDebuff();
	AddSC_MajesticDebuff();
	AddSC_CustomBoss();
	AddSC_SpecialMapNPC();

	sLog->outInfo(LOG_DEFAULT, "AI Scripts loaded");
	sLog->outInfo(LOG_DEFAULT, "");
}

void ScriptAIMgr::AddScriptAI(MonsterScriptAI* pScriptAI)
{
	for ( ScriptAIList::const_iterator it = this->script_ai_list.begin(); it != this->script_ai_list.end(); ++it )
	{
		if ( (*it)->GetName() == pScriptAI->GetName() )
		{
			sLog->outError(LOG_MONSTER, "ScriptAIMgr::AddScriptAI() Repeated AI Script: %s", (*it)->GetName().c_str());
			SAFE_DELETE(pScriptAI);
			return;
		}
	}

	this->script_ai_list.push_back(pScriptAI);
}

void ScriptAIMgr::GetScriptAI(std::string const& name, Monster* pMonster) const
{
	for ( ScriptAIList::const_iterator it = this->script_ai_list.begin(); it != this->script_ai_list.end(); ++it )
	{
		if ( (*it)->GetName() == name )
		{
			pMonster->AISet((*it)->GetAI(pMonster));
			return;
		}
	}

	pMonster->AISet(nullptr);
	pMonster->SetScriptName("");
}

bool ScriptAIMgr::IsScriptAI(std::string const& name) const
{
	if ( name.empty() )
		return true;

	for ( ScriptAIList::const_iterator it = this->script_ai_list.begin(); it != this->script_ai_list.end(); ++it )
	{
		if ( (*it)->GetName() == name )
		{
			return true;
		}
	}
	
	return false;
}