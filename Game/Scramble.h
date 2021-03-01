#ifndef SCRAMBLE_H
#define SCRAMBLE_H

typedef std::vector<std::string> ScrambleWordList;

class Scramble: public EventCommon
{
	SingletonInstance(Scramble);

	public:
		Scramble();
		virtual ~Scramble();

		void LoadWordList();
		void Start(int32 duration, int32 notify);

		void Update();

		void SetState_None();
		void SetState_Notify();
		void SetState_Playing();

		void ProcState_None();
		void ProcState_Notify();
		void ProcState_Playing();

		void MakeRandomWord();
		bool CompareWord(Player* pPlayer, std::string const& word);
		void MakeReward(Player* pPlayer);

		DECLARE_ENUM(int32, Duration);
		DECLARE_ENUM(int32, Notify);

		DECLARE_PROPERTY(int32, TotalWordCount);
		DECLARE_PROPERTY(int32, CurrentWordCount);
		DECLARE_STRUCT(TickTimer, WordTime);
		DECLARE_PROPERTY_STRING(Word);
		DECLARE_PROPERTY_STRING(OriginalWord);
		DECLARE_BOOL(WordChange);

		ScrambleWordList word_list;
};

#endif