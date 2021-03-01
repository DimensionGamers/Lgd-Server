#ifndef GUILD_MATCHING_H
#define GUILD_MATCHING_H

// GuildMatching.h: interface for the CGuildMatching class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct GuildMatchingJoinInfo
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint32, GuildID);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint32, Level);
};

typedef std::map<uint32, GuildMatchingJoinInfo*> GuildMatchingJoinInfoMap;

class GuildMatching
{
	SingletonInstance(GuildMatching);

	public:
		GuildMatching();
		virtual ~GuildMatching();

		void LoadGuildMatching();

		GuildMatchingJoinInfo* GetGuildMatchingJoinInfo(uint32 id);
		GuildMatchingJoinInfo* GetGuildMatchingJoinInfo(const char* name);
		void RemoveGuildMatchingJoinInfo(uint32 id);
		void RemoveGuildMatchingJoinInfo(const char* name);
		void RemoveGuildMatchingJoinInfoNotifyAll(uint32 id);

		void GuildMatchingListRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingListSearchRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingInsertRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingCancelRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingJoinInsertRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingJoinCancelRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingJoinAcceptRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingJoinListRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingJoinInfoRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void GuildMatchingInsertSaveRecv(uint8 * Packet, std::shared_ptr<ServerSocket> socket);

		void DGGuildMatchingNotifySend(const char* name, uint32 id, uint32 result);
		void DGGuildMatchingNotifyMasterSend(const  char* name, uint32 result);

	private:
		GuildMatchingJoinInfoMap m_guild_matching_join_info_map;
};

#endif