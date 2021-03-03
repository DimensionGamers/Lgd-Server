/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "TeleportManager.h"
*
*/

#ifndef TELEPORT_TEMPLATE_H
#define TELEPORT_TEMPLATE_H

struct GateData
{
	uint16 Id;
	uint8 Flag;
	uint16 MapId;
	int16 X1;
	int16 Y1;
	int16 X2;
	int16 Y2;
	uint16 TargetId;
	uint8 Direction;
	int16 MinLevel;
	std::string Description;

	int16 GetMinX() const { return X1 == X2 ? X1 : std::min(X1, X2); }
	int16 GetMaxX() const { return X1 == X2 ? X2 : std::max(X1, X2); }
	int16 GetMinY() const { return Y1 == Y2 ? Y1 : std::min(Y1, Y2); }
	int16 GetMaxY() const { return Y1 == Y2 ? Y2 : std::max(Y1, Y2); }
};

struct TeleportData
{
	uint8 Id;
	std::string Name;
	uint32 Zen;
	int16 MinLevel;
	uint16 Gate;
	uint16 MonsterId;
};

struct MoveLevel
{
	int16 Level;
	uint16 MapId;
	int16 X1;
	int16 Y1;
	int16 X2;
	int16 Y2;
	bool Disabled;
};

class TeleportManager
{
	SingletonInstance(TeleportManager);

	public:
		TeleportManager();
		~TeleportManager();

		void LoadGateTemplate();
		void LoadTeleportTemplate();
		void LoadMoveLevel();

		GateData const* GetGate(uint16 id, bool target = false) const;
		TeleportData const* GetTeleport(uint8 id) const;
		TeleportData const* GetTeleport(std::string const& name) const;

		int16 GetMoveLevel(uint16 mapId, int16 x, int16 y) const;

		void Update();
		void SendBattleZone(Player* player, uint16 mapId);
		void SendMonsterInfo(Player* player);

	private:
		std::map<uint16, GateData*> _gates;
		std::map<uint8, TeleportData*> _teleports;
		std::vector<MoveLevel*> _moveLevels;
};

#endif