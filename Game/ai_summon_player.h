#ifndef AI_SUMMON_PLAYER
#define AI_SUMMON_PLAYER

struct SummonPlayerAI: public MonsterAI
{
	explicit SummonPlayerAI(Monster* monster): MonsterAI(monster) { this->SetFlags(MONSTER_AI_FLAG_VIEWPORT); }
	virtual ~SummonPlayerAI() {}
	bool ViewportListAddConditions(Unit* pAdd);
	virtual bool MoveAttempt();
	virtual bool MoveAllowed(int16 x, int16 y);
	virtual bool SearchEnemy();
	virtual void OnDie();
	bool ManageThreat() const { return false; }

	bool IsViewportControl() const { return true; }
};

#endif