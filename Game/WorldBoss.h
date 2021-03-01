#ifndef WORLD_BOSS_H
#define WORLD_BOSS_H

enum WorldBossState
{
	WORLD_BOSS_STATE_NONE,
	WORLD_BOSS_STATE_PLAYING,
	WORLD_BOSS_STATE_END
};

class WorldBoss
{
	SingletonInstance(WorldBoss);

	public:
		WorldBoss();
		virtual ~WorldBoss();

		void Start();

		void Update();

		void SetState_None();
		void SetState_Playing();
		void SetState_End();

		void ProcState_None();
		void ProcState_Playing();
		void ProcState_End();

		void CreateBoss();
		std::string GenerateItemBag() const;

		void KillPlayer(Player* pPlayer01, Player* pPlayer02);
		bool IsKillPlayer(Player* pPlayer01, Player* pPlayer02);

		DECLARE_ENUM(WorldBossState, State);
		DECLARE_PTR(Monster, Boss);
		DECLARE_STRUCT(TickTimer, Time);
};

#endif