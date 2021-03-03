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

struct PMSG_MINI_MAP_INFO_SEND : C1_HEADER_SUB
{
	PMSG_MINI_MAP_INFO_SEND()
	{
		Set(0xE7, 0x03, sizeof(PMSG_MINI_MAP_INFO_SEND));
	}

	uint8 index;
	uint8 group;
	uint8 type;
	uint8 flag;
	uint8 x;
	uint8 y;
	char text[33];
};
#pragma pack()
//**********************************************//
//**********************************************//
//**********************************************//

struct MiniMapInfo
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, Group);
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(int16, X);
	DECLARE_ENUM(int16, Y);
	DECLARE_STRING_FIXED(Text, 32);
};

class MiniMap
{
	SingletonInstance(MiniMap);

	public:
		MiniMap();
		virtual ~MiniMap();
		void Load();

		void CGMiniMapStartPartyInfoRecv(Player* player);
		void CGMiniMapClosePartyInfoRecv(Player* player);
		void CGMiniMapInfoRecv(Player* player, uint8 * packet);
		void GCMiniMapPartyInfoSend(Player* player);
		void GCMiniMapInfoSend(Player* pPlayer);
	private:
		std::map<uint16, std::vector<MiniMapInfo*>> _miniMapInfo;
};

#endif