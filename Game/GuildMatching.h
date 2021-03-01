#ifndef GUILD_MATCHING_H
#define GUILD_MATCHING_H

// GuildMatching.h: interface for the CGuildMatching class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//
#pragma pack(1)
struct GUILD_MATCHING_LIST : C1_HEADER_SUB
{
	uint32 page;
};

struct GUILD_MATCHING_LIST_HEAD : C2_HEADER_SUB
{
	uint8 junk[3];
	uint32 current_page;
	uint32 max_page;
	uint32 count;
	uint32 result;
};

struct GUILD_MATCHING_LIST_BODY
{
	char text[GUILD_MATCHING_TEXT_LENGTH + 1]; //0
	char name[MAX_CHARACTER_LENGTH + 1]; //41
	char guild_name[MAX_GUILD_NAME_LENGTH + 1]; //52
	uint8 members_count; //61
	uint8 master_class; //62
	uint8 interest_type; //63
	uint8 level_range; //64
	uint8 junk; //65
	uint16 class_type; //66
	uint32 master_level; //68
	uint32 board_number; //72
	uint32 guild_id; //76
	uint32 gens; //80
};

struct GUILD_MATCHING_LIST_SEARCH : C1_HEADER_SUB
{
	uint32 page;
	char search_word[11];
};

struct GUILD_MATCHING_REGISTER : C1_HEADER_SUB
{
	char text[GUILD_MATCHING_TEXT_LENGTH + 1];
	uint8 interest_type;
	uint8 level_range;
	INDEX_DATA(Class);
};

struct GUILD_MATCHING_REGISTER_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_REGISTER_RESULT(uint32 result)
	{
		this->Set(0xED, 0x02, sizeof(GUILD_MATCHING_REGISTER_RESULT));
		this->result = result;
	}

	uint32 result;
};

struct GUILD_MATCHING_CANCEL_REGISTER : C1_HEADER_SUB
{
	
};

struct GUILD_MATCHING_CANCEL_REGISTER_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_CANCEL_REGISTER_RESULT(uint32 result)
	{
		this->Set(0xED, 0x03, sizeof(GUILD_MATCHING_CANCEL_REGISTER_RESULT));
		this->result = result;
	}

	uint32 result;
};

struct GUILD_MATCHING_JOIN : C1_HEADER_SUB
{
	uint32 guild_id;
};

struct GUILD_MATCHING_JOIN_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_JOIN_RESULT(uint32 result)
	{
		this->Set(0xED, 0x04, sizeof(GUILD_MATCHING_JOIN_RESULT));
		this->result = result;
	}

	uint32 result;
};

struct GUILD_MATCHING_JOIN_CANCEL : C1_HEADER_SUB
{

};

struct GUILD_MATCHING_JOIN_CANCEL_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_JOIN_CANCEL_RESULT()
	{
		this->Set(0xED, 0x05, sizeof(GUILD_MATCHING_JOIN_CANCEL_RESULT));
	}

	uint32 result;
};

struct GUILD_MATCHING_JOIN_ACCEPT : C1_HEADER_SUB
{
	uint32 type;
	char name[MAX_CHARACTER_LENGTH + 1];
};

struct GUILD_MATCHING_JOIN_ACCEPT_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_JOIN_ACCEPT_RESULT(uint32 type, uint32 result, const char* name)
	{
		this->Set(0xED, 0x06, sizeof(GUILD_MATCHING_JOIN_ACCEPT_RESULT));
		this->type = type;
		this->result = result;
		memset(this->name, 0, MAX_CHARACTER_LENGTH + 1);

		if (name)
		{
			memcpy(this->name, name, MAX_CHARACTER_LENGTH);
		}
	}

	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 junk;
	uint32 type;
	uint32 result;
};

struct GUILD_MATCHING_JOIN_LIST : C1_HEADER_SUB
{
	
};

struct GUILD_MATCHING_JOIN_LIST_HEAD : C2_HEADER_SUB
{
	GUILD_MATCHING_JOIN_LIST_HEAD()
	{
		this->Set(0xED, 0x07, sizeof(GUILD_MATCHING_JOIN_LIST_HEAD));
	}

	uint8 junk[3];
	uint32 count;
	uint32 result;
};

struct GUILD_MATCHING_JOIN_LIST_BODY
{
	char name[MAX_CHARACTER_LENGTH + 1];
	uint8 race;
	uint32 level;
	uint32 junk;
};

struct GUILD_MATCHING_JOIN_INFO : C1_HEADER_SUB
{

};

struct GUILD_MATCHING_JOIN_INFO_RESULT : C1_HEADER_SUB
{
	GUILD_MATCHING_JOIN_INFO_RESULT()
	{
		this->Set(0xED, 0x08, sizeof(GUILD_MATCHING_JOIN_INFO_RESULT));
	}

	char master_name[MAX_CHARACTER_LENGTH + 1];
	char guild_name[MAX_GUILD_NAME_LENGTH + 1];
	uint32 result;
};

struct GUILD_MATCHING_NOTIFY : C1_HEADER_SUB
{
	GUILD_MATCHING_NOTIFY()
	{
		this->Set(0xED, 0x09, sizeof(GUILD_MATCHING_NOTIFY));
	}

	uint32 result;
};

struct GUILD_MATCHING_NOTIFY_MASTER : C1_HEADER_SUB
{
	GUILD_MATCHING_NOTIFY_MASTER()
	{
		this->Set(0xED, 0x10, sizeof(GUILD_MATCHING_NOTIFY_MASTER));
	}

	uint32 result;
};
#pragma pack()

class GuildMatching
{
	SingletonInstance(GuildMatching);

	public:
		GuildMatching();
		virtual ~GuildMatching();
		void CGGuildMatchingListRecv(Player* pPlayer, uint8 * Packet);
		void CGGuildMatchingListSearchRecv(Player* pPlayer, uint8 * Packet);
		void CGGuildMatchingInsertRecv(Player* pPlayer, uint8 * Packet);
		void InsertResult(Player* pPlayer, uint32 result);

		void CGGuildMatchingCancelRecv(Player* pPlayer, uint8 * Packet);
		void CancelResult(Player* pPlayer, uint32 result);

		void CGGuildMatchingJoinInsertRecv(Player* pPlayer, uint8 * Packet);
		void JoinInsertResult(Player* pPlayer, uint32 result);

		void CGGuildMatchingJoinCancelRecv(Player* pPlayer, uint8 * Packet);
		void CGGuildMatchingJoinAcceptRecv(Player* pPlayer, uint8 * Packet);
		void JoinAcceptResult(Player* pPlayer, uint32 type, uint32 result, const char * name);

		void CGGuildMatchingJoinListRecv(Player* pPlayer, uint8 * Packet);
		void CGGuildMatchingJoinInfoRecv(Player* pPlayer, uint8 * Packet);

		void DGGuildMatchingNotifyRecv(uint8 * Packet);
		void DGGuildMatchingNotifyMasterRecv(uint8 * Packet);
		void DGGuildMatchingListRecv(uint8 * Packet);
		void DGGuildMatchingInsertRecv(uint8 * Packet);
		void DGGuildMatchingCancelRecv(uint8 * Packet);
		void DGGuildMatchingJoinInsertRecv(uint8 * Packet);
		void DGGuildMatchingJoinCancelRecv(uint8 * Packet);
		void DGGuildMatchingJoinAcceptRecv(uint8 * Packet);
		void DGGuildMatchingJoinListRecv(uint8 * Packet);
		void DGGuildMatchingJoinInfoRecv(uint8 * Packet);

		void GDGuildMatchingListSend(Player* pPlayer, uint32 page);
		void GDGuildMatchingListSearchSend(Player* pPlayer, uint32 page, const char* SearchWord);
		void GDGuildMatchingJoinCancelSend(Player* pPlayer, uint8 flag);
		void GDGuildMatchingJoinListSend(Player* pPlayer);
		void GDGuildMatchingJoinInfoSend(Player* pPlayer);
		void Update(Player* pPlayer);

	private:
};

#endif