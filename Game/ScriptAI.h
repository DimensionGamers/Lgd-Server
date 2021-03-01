#ifndef SCRIPT_AI_H
#define SCRIPT_AI_H

template <class TAI, class T>
class ScriptAI
{
	friend class ScriptAIMgr;

	public:
		explicit ScriptAI(std::string const& name)
		{
			this->SetName(name);
		}
		virtual ~ScriptAI() { }
		virtual TAI* GetAI(T*) const { return nullptr; }

	private:
		DECLARE_PROPERTY_STRING(Name);
};

#define MonsterScriptAI ScriptAI<MonsterAI, Monster>
typedef std::list<MonsterScriptAI*> ScriptAIList;

class ScriptAIMgr
{
	SingletonInstance(ScriptAIMgr);

	public:
		ScriptAIMgr();
		virtual ~ScriptAIMgr();

		void LoadScriptAI();

		void AddScriptAI(MonsterScriptAI*);
		void GetScriptAI(std::string const&, Monster*) const;
		bool IsScriptAI(std::string const&) const;
	private:
		ScriptAIList script_ai_list;
};

#endif