#ifndef MINI_MAP_H
#define MINI_MAP_H

#pragma once

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_MINI_MAP_INFO_RECV
{
	PBMSG_HEAD2 h; // C1:E7:03
	uint8 flag;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//
#pragma pack(1)
struct MINI_MAP_PARTY_HEAD : C1_HEADER_SUB
{
	uint8 count;
};

struct MINI_MAP_PARTY_BODY
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint16 map;
	uint8 x;
	uint8 y;
};

struct PMSG_MINI_MAP_INFO_SEND
{
	PMSG_MINI_MAP_INFO_SEND()
	{
		this->h.set(0xE7, 0x03, sizeof(PMSG_MINI_MAP_INFO_SEND));
	}

	PBMSG_HEAD2 h; // C1:E7:03
	uint8 index;
	uint8 group;
	uint8 type;
	uint8 flag;
	uint8 x;
	uint8 y;
	char text[33];
};

/*
C1,29,E7,03,
00,01,02,00,
AB,69,43,68,61,72,6F,6E,00,74,
68,65,20,50,72,69,65,73,74,00,
20,5A,79,72,6F,00,68,61,6E,74,
00,00,
9B,

C1,29,E7,03,
01,01,04,00,AD,7D,45,6C,66,20,4C,61,6C,61,00,65,20,50,72,69,65,73,74,00,20,5A,79,72,6F,00,68,61,6E,74,00,00,9B,

C1,29,E7,03,
02,01,03,00,C3,7C,45,6F,20,74,68,65,20,43,72,61,66,74,73,6D,61,6E,00,00,20,5A,79,72,6F,00,68,61,6E,74,00,00,9B,

C1,29,E7,03,
03,01,05,00,AC,60,53,61,66,65,74,79,20,47,75,61,72,64,69,61,6E,00,00,00,20,5A,79,72,6F,00,68,61,6E,74,00,00,9B,
*/
#pragma pack()
//**********************************************//
//**********************************************//
//**********************************************//

struct MiniMapInfo
{
	DECLARE_ENUM(world_type, World);
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_STRING_FIXED(Text, 32);
};

typedef std::vector<MiniMapInfo*> MiniMapInfoList;
typedef std::map<world_type, MiniMapInfoList> MiniMapInfoMap;

class MiniMap
{
	SingletonInstance(MiniMap);

	public:
		MiniMap();
		virtual ~MiniMap();
		void Load();

		void CGMiniMapStartPartyInfoRecv(Player* pPlayer);
		void CGMiniMapClosePartyInfoRecv(Player* pPlayer);
		void CGMiniMapInfoRecv(Player* pPlayer, uint8 * Packet);
		void GCMiniMapPartyInfoSend(Player* pPlayer);
		void GCMiniMapInfoSend(Player* pPlayer);
	private:
		MiniMapInfoMap m_MiniMapInfo;
};

#endif