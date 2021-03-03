/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Party.h"
*
*/
#ifndef PARTY_H
#define PARTY_H

struct PartyMemberData
{
	DECLARE_PROPERTY_PTR(Player, Player);
	DECLARE_ENUM(uint32, Account);
	DECLARE_ENUM(uint32, GUID);
	DECLARE_ENUM(PartyMemberStatus, Status);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_STRUCT(TickTimer, LeftTime);
	DECLARE_ENUM(uint8, GenFamily);
	DECLARE_BOOL(Assisted);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint16, Level);

	void Reset()
	{
		this->SetPlayer(nullptr);
		this->SetAccount(0);
		this->SetGUID(0);
		this->SetStatus(PARTY_USER_FLAG_NONE);
		this->ResetName();
		this->GetLeftTime()->Reset();
		this->SetGenFamily(0);
		this->SetAssisted(false);
		this->SetClass(0);
		this->SetLevel(0);
	}

	void Set(PartyMemberData const* Data)
	{
		this->SetPlayer(Data->GetPlayer());
		this->SetAccount(Data->GetAccount());
		this->SetGUID(Data->GetGUID());
		this->SetStatus(Data->GetStatus());
		this->SetName(Data->GetName());
		this->GetLeftTime()->SetDelay(Data->GetLeftTime()->GetDelay());
		this->GetLeftTime()->SetTimer(Data->GetLeftTime()->GetTimer());
		this->SetGenFamily(Data->GetGenFamily());
		this->SetAssisted(Data->IsAssisted());
		this->SetClass(Data->GetClass());
		this->SetLevel(Data->GetLevel());
	}

	uint8 GetMatchingClass() const
	{
		uint8 db_class = this->GetClass();
		uint8 matching_class = db_class >> 4;
		uint8 change_up_1 = db_class & 0x07 & 0x01;
		uint8 change_up_2 = (db_class & 0x07 & 0x02) >> 1;
		uint8 change_up_3 = (db_class & 0x07 & 0x04) >> 2;

		if (change_up_1)
		{
			matching_class |= 16;
		}

		if (change_up_2)
		{
			matching_class |= 32;
		}

		if (change_up_3)
		{
			matching_class |= 64;
		}

		return matching_class;
	}
};

struct PartyMatchingData
{
	DECLARE_BOOL(Enabled);
	DECLARE_STRING_FIXED(Text, PARTY_MATCHING_TEXT_LENGHT + 1);
	DECLARE_STRING_FIXED(Password, PARTY_MATCHING_PASSWORD_LENGTH + 1);
	DECLARE_ENUM(uint16, MinLevel);
	DECLARE_ENUM(uint16, MaxLevel);
	DECLARE_ENUM(uint8, AutoAccept);
	DECLARE_ENUM(uint8, UsePassword);
	DECLARE_ENUM(uint8, RequiredElf);

	void Reset()
	{
		this->SetEnabled(false);
		this->ResetText();
		this->ResetPassword();
		this->SetMinLevel(0);
		this->SetMaxLevel(0);
		this->SetAutoAccept(0);
		this->SetUsePassword(0);
		this->SetRequiredElf(0);
	}
};

class Party
{
	public:
		Party();

		DECLARE_ARRAY_STRUCT(PartyMemberData, Member, MAX_PARTY_MEMBERS);
		DECLARE_ENUM(uint16, ID);
		DECLARE_ENUM(PartyStatus, Status);

		void UpdateDB();
		void DeleteDB();

		uint8 GetMemberCount() const;
		bool IsMember(uint32 guid);
		uint8 GetMemberPosition(Player * pPlayer) const;
		bool AddMember(Player * pPlayer, bool update = false);
		void DeleteMember(uint8 position);
		void Destroy();
		void Update();
		void Reset();

		void MoveMembers();

		void ListAllMembers();
		void SendData();
		void SendStats();
		void SendBuff();
		void UpdateData();
		
		void QuestMUObjectiveUpdate(QuestMUObjectiveType type, Unit* pUnit = nullptr, int32 data = 0);
		void QuestGuidedMonsterKill(Monster* pMonster);
		void GenKillReward(Player* pVictim);
		uint8 GetPartyMembersWithinDistance(uint16 world, int32 instance, int16 x, int16 y, uint8 distance);
		int16 GetPartyMemberHighestLevel(uint16 world, int32 instance, int16 x, int16 y, uint8 distance);
		void IncreaseAggroAround(Player* pPlayer, Monster* pMonster, int64 value);
		uint8 GetPKLevel() const;

		void SendNotice(const char * name, const char * msg,...);
		void SendNotice(NoticeType type, const char * notice, ...);
		void SendNoticeNormal(const char * name, const char * msg);
		void SendNoticeNormal(NoticeType type, const char * notice);

		void SendNotice(CustomMessageID type, const char * notice, ...);
		void SendNoticeNormal(CustomMessageID type, const char * notice);

		Player * GetLeader() { return this->GetMember(0)->GetPlayer(); }

		bool OnLogin(Player* pPlayer);

		void SendKillDamage(Player* pKiller, Unit* pVictim, int32 damage, int32 shield_damage, uint16 damage_flag);

		bool IsEvomonSummoned() const;
		PartyMemberData const* FindMember(uint16 index) const;
		PartyMemberData * FindMember(uint16 index);

		DECLARE_STRUCT(PartyMatchingData, Matching);
};

#endif