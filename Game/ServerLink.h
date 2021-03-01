#ifndef SERVER_LINK_H
#define SERVER_LINK_H

class ServerLinkSocket: public TCPClient<ServerLinkSocket>
{
	public:
		explicit ServerLinkSocket(): TCPClient(_ioService, "ServerLink", TCPCLIENT_ON_ERROR_CLOSE) {}

		void OnStart() override;
		void OnStop() override;
		void ReadHandler() override;
};

class ServerLink: public TCPClientMgr<ServerLinkSocket>
{
	SingletonInstance(ServerLink);

	public:
		static std::unordered_map<uint8, LoginHandler<ServerLink>> InitHandlers();

		void HandleHeadcodeOnConnect(uint8 * Packet);

		void HandleHeadcodeGuildChat(uint8 * packet);
		void HandleHeadcodeAllianceChat(uint8 * packet);
		void HandleHeadcodeGuildNotice(uint8 * packet);
		void HandleHeadcodePost(uint8 * packet);
		
		void HandleHeadcodeCharacterOnOff(uint8 * packet);
		

		void HandleHeadcodeNotice(uint8 * packet);

		void ChatSend(uint8 headcode, Player * pPlayer, const char * msg);

		//void GuildListRequest();
		//void GuildListResult(uint8 * Packet);

		//void GuildMemberListRequest(uint32 guild);
		//void GuildMemberListResult(uint8 * Packet);

		void GuildCreateRequest(Player * pPlayer, const char * name, uint8 * emblem);
		void GuildCreateResult(uint8 * Packet);
		void GuildDeleteRequest(uint32 guild);
		void GuildDeleteResult(uint8 * Packet);
		void GuildMemberAddRequest(Player* pPlayer, Player* pToAdd, uint32 guild);
		void GuildMemberAddRequest(const char * member_name, uint32 guild);
		void GuildMemberAddResult(uint8 * packet);
		void GuildMemberDelRequest(uint32 guild, uint32 id);
		void GuildMemberDelResult(uint8 * Packet);
		void GuildMemberStatusRequest(uint32 guild, const char * name, uint8 status);
		void GuildMemberStatusResult(uint8 * packet);
		void GuildRelationshipRequest(uint32 guild01, uint32 guild02, uint8 type, bool operation);
		void GuildRelationshipResult(uint8 * packet);
		void GuildScore(uint32 guild, int32 score);
		void GuildScoreResult(uint8 * packet);

		void CharacterOnOff(Player* pPlayer, uint8 status);
	
		void NoticeSend(uint8 type, const char * notice, ...);
		void NoticeSendNormal(uint8 type, const char * notice);

		void CastleSiegeLoadDataRequest();
		void CastleSiegeLoadDataResult(uint8 * packet);
		void CastleSiegeLoadNpcRequest();
		void CastleSiegeLoadNpcResult(uint8 * packet);
		void CastleSiegeLoadRegisteredGuildRequest();
		void CastleSiegeLoadRegisteredGuildResult(uint8 * packet);
		void CastleSiegeClearGuild();
		void CastleSiegeRegisteredGuildRequest(Player* pPlayer, Guild* pGuild);
		void CastleSiegeRegisteredGuildResult(uint8 * packet);
		void CastleSiegeRegisteredGuildAllRequest(Player* pPlayer);
		void CastleSiegeRegisteredGuildAllResult(uint8 * packet);

		void HandleHeadcodeCastleSiegeRegisterGuild(uint8 * packet);
		void HandleHeadcodeCastleSiegeTaxRate(uint8 * packet);
		void HandleHeadcodeCastleSiegeMoney(uint8 * packet);
		void HandleHeadcodeCastleSiegeUpdateOwner(uint8 * packet);

		void CastleSiegeRegisterGuild(uint32 guild, bool status, uint32 mark);

		void CharacterCreateRequest(Player* pPlayer, uint8 race, const char * name, uint8 slot, CharacterBaseData const* info);
		void CharacterCreateResult(uint8 * Packet);
		void CharacterDeleteRequest(Player* pPlayer, const char * name);
		void CharacterDeleteResult(uint8 * Packet);

		void AdminCommandRequest(Player* pPlayer, uint8 type, uint16 sub_type, const char * target_name, const char * target_account, time_t time);
		void AdminCommandResult(uint8 * Packet);

		void WhisperRequest(Player* pPlayer, const char * target_name, const char * message);
		void WhisperResult(uint8 * Packet);

		void SimpleMessageRequest(const char * name, CustomMessageID type, const char * message, ...);
		void SimpleMessageResult(uint8 * Packet);

		void Signal(uint8 * Packet);

		void CrywolfDataRequest();
		void CrywolfDataResult(uint8 * Packet);
		void CrywolfDataSave();

		void GuildMatchingList(uint8 * Packet);
		void GuildMatchingInsert(uint8 * Packet);
		void GuildMatchingCancel(uint8 * Packet);
		void GuildMatchingJoinInsert(uint8 * Packet);
		void GuildMatchingJoinCancel(uint8 * Packet);
		void GuildMatchingJoinAccept(uint8 * Packet);
		void GuildMatchingJoinList(uint8 * Packet);
		void GuildMatchingJoinInfo(uint8 * Packet);
		void GuildMatchingNotify(uint8 * Packet);
		void GuildMatchingMasterNotify(uint8 * Packet);

		void EventNotification(uint8 event_id, uint8 open);
		void EventNotification(uint8 * Packet);

		void EventStateUpdate(uint8 event_id, uint8 state, uint8 occupation_state = 0);
		void EventStateUpdate(uint8 * Packet);

		void ArkaWarMasterRegister(Player* pPlayer, uint32 guild);
		void ArkaWarMasterRegister(uint8 * Packet);
		void ArkaWarMemberRegister(Player* pPlayer);
		void ArkaWarMemberRegister(uint8 * Packet);
		void ArkaWarSignRegister(Player* pPlayer, uint8 result, int32 count = 0);
		void ArkaWarSignRegister(uint8 * Packet);
		void ArkaWarEnter(Player* pPlayer);
		void ArkaWarEnter(uint8 * Packet);
		void ArkaWarList();
		void ArkaWarList(uint8 * Packet);
		void ArkaWarClear();
		void ArkaWarMemberCount(Player* pPlayer);
		void ArkaWarMemberCount(uint8 * Packet);
		void ArkaWarGuildRequest();
		void ArkaWarGuildRequest(uint8 * Packet);
		void ArkaWarGuildSave();

		void ArkaWarSignOfLordCheckRequest(Player* pPlayer);
		void ArkaWarSignOfLordCheckResult(uint8 * Packet);

		void CharacterSignal(uint32 id, uint8 type);
		void CharacterSignal(uint8 * Packet);
	public:
		ServerLink();

		void HandlePacketData(uint8 headcode, uint8 * packet);

};

#define sServerLink SingNull(ServerLink)
#endif