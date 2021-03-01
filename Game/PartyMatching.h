#ifndef PARTY_MATCHING_H
#define PARTY_MATCHING_H

#pragma once

#pragma pack(1)
struct PARTY_MATCHING_REGISTER : C1_HEADER_SUB
{
	char text[PARTY_MATCHING_TEXT_LENGHT + 1];
	char password[PARTY_MATCHING_PASSWORD_LENGTH + 1];
	uint16 min_level;
	uint16 max_level;
	uint8 use_password;
	uint8 auto_accept;
	uint8 required_elf;
	uint8 unk;
};

struct PARTY_MATCHING_REGISTER_RESULT : C1_HEADER_SUB
{
	PARTY_MATCHING_REGISTER_RESULT()
	{
		this->Set(0xEF, 0x00, sizeof(PARTY_MATCHING_REGISTER_RESULT));
		this->result = 0;
	}

	uint32 result;
};

struct PARTY_MATCHING_SEARCH : C1_HEADER_SUB
{
	uint32 page;
	char search_word[PARTY_MATCHING_SEARCH_LENGTH + 1];
	uint8 search_flags;
};

struct PARTY_MATCHING_SEARCH_CHARACTER
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint16 level;
	uint16 race;
};

struct PARTY_MATCHING_SEARCH_BODY
{
	char text[PARTY_MATCHING_TEXT_LENGHT + 1]; //0
	uint8 gens; //41
	uint8 use_password; //42
	uint8 members_count; //43
	uint16 min_level; //44
	uint16 max_level; //46
	PARTY_MATCHING_SEARCH_CHARACTER member[MAX_PARTY_MEMBERS]; //48
};

struct PARTY_MATCHING_SEARCH_HEAD : C2_HEADER_SUB
{
	uint32 count; //5
	uint32 page; //9
	uint32 page_max; //13
	uint32 result; //17
};

struct PARTY_MATCHING_JOIN : C1_HEADER_SUB
{
	char leader_name[MAX_CHARACTER_LENGTH + 1];
	char password[PARTY_MATCHING_PASSWORD_LENGTH + 1];
	uint8 use_password;
	uint8 random;
	uint8 automatic_helper_assist;
};

struct PARTY_MATCHING_JOIN_RESULT : C1_HEADER_SUB
{
	PARTY_MATCHING_JOIN_RESULT()
	{
		this->Set(0xEF, 0x02, sizeof(PARTY_MATCHING_JOIN_RESULT));
		this->result = 0;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->text, 0, PARTY_MATCHING_TEXT_LENGHT + 1);
	}

	uint32 result;
	uint8 use_password;
	uint8 gens;
	char name[MAX_CHARACTER_LENGTH + 1];
	char text[PARTY_MATCHING_TEXT_LENGHT + 1];
};

struct PARTY_MATCHING_JOIN_DATA : C1_HEADER_SUB
{
	
};

struct PARTY_MATCHING_JOIN_DATA_RESULT : C1_HEADER_SUB
{
	PARTY_MATCHING_JOIN_DATA_RESULT()
	{
		this->Set(0xEF, 0x03, sizeof(PARTY_MATCHING_JOIN_DATA_RESULT));
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
		memset(this->text, 0, PARTY_MATCHING_TEXT_LENGHT + 1);
	}

	uint32 result;
	uint32 server;
	uint8 use_password;
	uint8 gens;
	char name[MAX_CHARACTER_LENGTH + 1];
	char text[PARTY_MATCHING_TEXT_LENGHT + 1];
	uint8 data;
};

struct PARTY_MATCHING_JOIN_LIST : C1_HEADER_SUB
{
	PARTY_MATCHING_JOIN_LIST()
	{
		this->Set(0xEF, 0x04, sizeof(PARTY_MATCHING_JOIN_LIST));
	}
};

struct PARTY_MATCHING_JOIN_LIST_BODY
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 race;
	uint32 level;
	uint8 data;
	uint8 junk[3];
};

struct PARTY_MATCHING_JOIN_LIST_HEAD : C2_HEADER_SUB
{
	uint8 junk[3];
	uint32 count;
	uint32 result;
};

struct PARTY_MATCHING_ACCEPT : C1_HEADER_SUB
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 type;
};

struct PARTY_MATCHING_ACCEPT_RESULT : C1_HEADER_SUB
{
	PARTY_MATCHING_ACCEPT_RESULT()
	{
		this->Set(0xEF, 0x05, sizeof(PARTY_MATCHING_ACCEPT_RESULT));
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);
	}

	char name[MAX_CHARACTER_LENGTH + 1]; //4
	uint8 type; //15
	uint8 flag; //16
	uint8 junk[3]; //17
	uint32 result; //20
};

struct PARTY_MATCHING_CANCEL : C1_HEADER_SUB
{
	uint8 type;
};

struct PARTY_MATCHING_CANCEL_RESULT : C1_HEADER_SUB
{
	PARTY_MATCHING_CANCEL_RESULT()
	{
		this->Set(0xEF, 0x06, sizeof(PARTY_MATCHING_CANCEL_RESULT));
	}

	uint8 type;
	uint8 junk[3];
	uint32 result;
};

struct PARTY_MATCHING_JOIN_NOTIFY : C1_HEADER_SUB
{
	PARTY_MATCHING_JOIN_NOTIFY()
	{
		this->Set(0xEF, 0x08, sizeof(PARTY_MATCHING_JOIN_NOTIFY));
	}
};

struct PARTY_MATCHING_NOTIFY : C1_HEADER_SUB
{
	PARTY_MATCHING_NOTIFY()
	{
		this->Set(0xEF, 0x09, sizeof(PARTY_MATCHING_NOTIFY));
	}

	uint32 result;
};
#pragma pack()
class PartyMatching
{
	SingletonInstance(PartyMatching);

	public:
		PartyMatching();
		virtual ~PartyMatching();
		void HandleRegister(Player* pPlayer, uint8 * Packet);
		void HandleSearch(Player* pPlayer, uint8 * Packet);
		void HandleJoin(Player* pPlayer, uint8 * Packet);
		void HandleJoinData(Player* pPlayer, uint8 * Packet);
		void HandleJoinList(Player* pPlayer, uint8 * Packet);
		void HandleAccept(Player* pPlayer, uint8 * Packet);
		void HandleCancelRegister(Player* pPlayer, uint8 * Packet);

		void InfoRemove(uint16 id);

		Party * FindPartyMatchingInfo(const char * name);
		Party * FindPartyMatchingInfo(Player* pPlayer);
		
		bool IsPartyMatchingJoin(const char * name) const;

		void Notify(Party* pParty, Player* pPlayer, uint8 result);
		void CancelRegister(Player* pPlayer, uint8 type, uint8 flag);
};

#endif