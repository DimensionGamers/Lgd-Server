/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChatHandler.h"
*
*/

#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H

class ChatCommand
{
	public:
		const char* name;
		void (ChatHandler::*handler)(const char* msg);
		uint8 Authority;
		AdminFlag flag;
		std::string sintax;
		ChatCommand * SubTable;
};

class ChatHandler
{
	public:
		explicit ChatHandler(Player* pPlayer, const char * whisper_name = "");
		virtual ~ChatHandler();
	
		bool IsAvailable(ChatCommand const& table);
		void ExecuteCommand(const char * text);
		void ExecuteCommandAdmin(const char * text);
		bool ExecuteCommandInTable(ChatCommand * table, const char * text, std::string & fullcmd);
		bool ShowHelpForCommand(ChatCommand* table, const char* cmd);
		bool ShowHelpForSubCommands(ChatCommand* table, char const* cmd, char const* subcmd);

		// Normal
		bool CommandAddValid();
		void CommandAddStrength(const char * msg);
		void CommandAddAgility(const char * msg);
		void CommandAddVitality(const char * msg);
		void CommandAddEnergy(const char * msg);
		void CommandAddLeadership(const char * msg);

		void CommandHostilCancel(const char * msg);
		void CommandRequest(const char * msg);
		void CommandPostRequest(const char * msg);
		void CommandRefresh(const char * msg);

		bool CommandFriendKickVerify(const char * name);
		void CommandFriendAddKick(const char * msg);
		void CommandFriendRemoveKick(const char * msg);
		void CommandFriendKick(const char * msg);
		void CommandGuildWar(const char * msg);
		void CommandBattleSoccer(const char * msg);
		void CommandNormalPost(const char * msg);

		void CommandSiegeRefresh(const char * msg);

		//void CommandMasterUP(const char * msg);

		void CommandOffTrade(const char * msg);

		void CommandAuthorize(const char * msg);
		void CommandOffAttack(const char * msg);
		void CommandPKPoints(const char * msg);

		void CommandErrtel(const char * msg);

		void CommandGold(const char * msg);

		void CommandTestPacket(const char * msg);

		// Administrator
		void CommandKick(const char * msg);
		void CommandEvents(const char * msg);
		void CommandStopEvents(const char * msg);
		void CommandMute(const char * msg);
		void CommandItemNormal(const char * msg);
		void CommandItemAdd(const char * msg);
		void CommandItemShow(const char * msg);
		void CommandItemHide(const char * msg);
		void CommandItemRemove(const char * msg);
		void CommandAdminItemPentagram(const char * msg);
		void CommandItemGremory(const char * msg);
		void CommandItemWing(const char * msg);
		void CommandBanChar(const char * msg);
		void CommandBanAcc(const char * msg);
		void CommandMonsterAdd(const char * msg);
		void CommandZenAdd(const char * msg);
		void CommandZenSet(const char * msg);
		void CommandGuildTalk(const char * msg);
		void CommandAllianceTalk(const char * msg);
		void CommandFirework(const char * msg);
		void CommandMonsterAddTemp(const char * msg);
		void CommandGMBuff(const char * msg);
		void CommandGMBuffAll(const char * msg);
		void CommandGMBuffTo(const char * msg);
		void CommandGMBuffApply(Unit* pUnit, int32 id, int32 duration, int32 value);
		void CommandRestriction(const char * msg);
		void CommandOnline(const char * msg);
		void CommandShutdown(const char * msg);
		void CommandAction(const char * msg);
		void CommandMonsterRemove(const char * msg);
		void CommandMonsterAlter(const char * msg);
		void CommandMiscSiege(const char * msg);
		void CommandMiscKill(const char * msg);

		void CommandChangeGuild(const char * msg);
		void CommandChangeAlliance(const char * msg);
		void CommandChangeHostil(const char * msg);
		void CommandChangeCSOwner(const char * msg);

		void CommandSkillAdd(const char * msg);
		void CommandSkillRemove(const char * msg);

		void CommandCastleSiegeRegister(const char * msg);
		void CommandCastleSiegeUnRegister(const char * msg);
		void CommandCastleSiegeIncreaseMarks(const char * msg);
		void CommandCastleSiegeReduceMarks(const char * msg);
		void CommandCastleSiegeNPCLoad(const char * msg);
		void CommandCastleSiegeFix(const char * msg);
		void CommandCastleSiegeFixGate(const char * msg);

		void CommandArcaWarRegisterMaster(const char * msg);
		void CommandArcaWarRegisterMember(const char * msg);
		void CommandArcaWarIncreaseMarks(const char * msg);
		void CommandArcaWarReload(const char * msg);

		void CommandAdminStatModify(Player* pPlayer, int32 stat_id, int32 points, uint8 type);
		void CommandAdminStatAdd(const char * msg);
		void CommandAdminStatSet(const char * msg);
		void CommandAdminStatRemove(const char * msg);

		void CommandAdminChallenge(const char * msg);
		void CommandAdminSetPK(const char * msg);
		void CommandAdminCrywolf(const char * msg);
		void CommandAdminSummon(const char * msg);
		void CommandAdminSeason(const char * msg);
		void CommandAdminResetDuel(const char * msg);
		void CommandAdminTransfer(const char * msg);
		void CommandAdminAccountKick(const char * msg);
		void CommandAdminIP(const char * msg);
		void CommandAdminKickIPMAC(const char * msg);
		void CommandAdminPersonalStore(const char * msg);
		void CommandAdminPostReset(const char * msg);
		void CommandAdminGlobalMute(const char * msg);

		void CommandAdminMuteMAC(const char * msg);
		void CommandAdminMACRestrict(const char * msg);

		void CommandAdminItemInfo(const char * msg);

		void AdminCommandTrace(const char * msg);
		void AdminCommandTrans(const char * msg);
		void AdminCommandGate(const char * msg);
		void AdminCommandMove(const char * msg);
		void AdminCommandFollow(const char * msg);
		void AdminCommandUpdateStat(const char * msg);

		void AdminCommandAcitve(const char * msg);
		void CommandAdminArcaWar(const char * msg);

		void CommandAdminInvi(const char * msg);
		void CommandAdminTrack(const char * msg);

		void CommandAdminChangeQuestState(const char * msg);
		void CommandAdminChangeQuestKillCount(const char * msg);

		void CommandAdminSpotCreate(const char * msg);

		void CommandAdminTrackCharacter(const char * msg);
		void CommandAdminAttrChange(const char * msg);

		// Reload
		void ReloadShop(const char * msg);
		void ReloadQuest(const char * msg);
		void ReloadEvent(const char * msg);
		void ReloadMonster(const char * msg);
		void ReloadConfig(const char * msg);
		void ReloadItem(const char * msg);
		void ReloadCharacter(const char * msg);
		void ReloadSkill(const char * msg);
		void ReloadTeleport(const char * msg);
		void ReloadCashShop(const char * msg);
		void ReloadMix(const char * msg);
		void ReloadWorld(const char * msg);
		void ReloadPentagram(const char * msg);

		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE_STRING_FIXED(WhisperName, MAX_CHARACTER_LENGTH + 1);
};

#endif
