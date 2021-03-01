/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "TeleportMgr.h"
*
*/

#ifndef TELEPORT_TEMPLATE_H
#define TELEPORT_TEMPLATE_H

struct GateData
{
	uint16 id;
	uint8 flag;
	world_type world;
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
	uint16 target_id;
	uint8 direction;
	int16 min_level;
	DECLARE_STRING_FIXED(Description, 32);

	coord_type GetMinX() const { return this->x1 == this->x2 ? this->x1: std::min(this->x1, this->x2); }
	coord_type GetMaxX() const { return this->x1 == this->x2 ? this->x2: std::max(this->x1, this->x2); }
	coord_type GetMinY() const { return this->y1 == this->y2 ? this->y1: std::min(this->y1, this->y2); }
	coord_type GetMaxY() const { return this->y1 == this->y2 ? this->y2: std::max(this->y1, this->y2); }
};

struct TeleportData
{
	uint8 id;
	DECLARE_PROPERTY_STRING(Name);
	uint32 zen;
	int16 min_level;
	uint16 gate;
	DECLARE_ENUM(uint16, Monster);
};

struct MoveLevel
{
	int16 level;
	world_type world;
	coord_type x1;
	coord_type y1;
	coord_type x2;
	coord_type y2;
	DECLARE_BOOL(Disabled);
};

typedef std::map<uint16, GateData*> GateDataMap;
typedef std::map<uint8, TeleportData*> TeleportDataMap;
typedef std::list<MoveLevel*> MoveLevelList;

class TeleportMgr
{
	SingletonInstance(TeleportMgr);

	protected:
		GateDataMap gate_template_map;
		TeleportDataMap teleport_template_map;
		MoveLevelList move_level_list;
	public:
		TeleportMgr() {}
		virtual ~TeleportMgr();

		void LoadGateTemplate();
		void LoadTeleportTemplate();
		void LoadMoveLevel();

		GateData const* GetGate(uint16 id, bool target = false);
		TeleportData const* GetTeleport(uint8 id) const;
		TeleportData const* GetTeleport(std::string map) const;

		int16 GetMoveLevel(world_type world, coord_type x, coord_type y, bool special);

		void Update();
		void SendBattleZone(Player* pPlayer, world_type world);
		void SendMonsterInfo(Player* pPlayer);
};

#endif