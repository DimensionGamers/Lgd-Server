/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Player.h"
*
*/

#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

struct CharacterListItem
{
	CharacterListItem()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetItem(-1);
		this->SetLevel(0);
		this->SetExcellent(0);
		this->SetAncient(0);
	}

	DECLARE_ENUM(uint16, Item);
	DECLARE_ENUM(uint8, Level);
	DECLARE_ENUM(uint8, Excellent);
	DECLARE_ENUM(uint8, Ancient);
};

struct CharacterListData
{
	CharacterListData()
	{
		this->Reset();
	}

	void Reset()
	{
		this->SetSlot(0);
		this->SetGUID(0);
		this->ResetName();
		this->SetLevelNormal(0);
		this->SetLevelMaster(0);
		this->SetLevelMajestic(0);
		this->SetAuthority(0);
		this->SetRace(0);
		this->SetGuildRanking(0);
		this->SetKickTime(0);
		this->SetPKLevel(0);
		this->SetCreateDate(0);

		for ( int32 i = 0; i < PLAYER_MAX_EQUIPMENT; ++i )
		{
			this->GetItem(i)->Reset();
		}
	}

	DECLARE_ENUM(uint8, Slot);
	DECLARE_ENUM(uint32, GUID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(int16, LevelNormal);
	DECLARE_ENUM(int16, LevelMaster);
	DECLARE_ENUM(int16, LevelMajestic);
	DECLARE_ENUM(uint8, Authority);
	DECLARE_ENUM(uint8, Race);
	DECLARE_ENUM(uint8, GuildRanking);
	DECLARE_ENUM(time_t, KickTime);
	DECLARE_ENUM(uint8, PKLevel);
	DECLARE_ENUM(int64, CreateDate);

	DECLARE_ARRAY_STRUCT(CharacterListItem, Item, PLAYER_MAX_EQUIPMENT);
};

class AccountData
{
	public:
		DECLARE_ENUM(uint32, GUID);
		char secure_code[MAX_SECURE_CODE_LENGTH + 1];
		DECLARE_STRING_FIXED(Account, (MAX_ACCOUNT_LENGTH + 1));
		DECLARE_PROPERTY(int8, CloseCount);
		DECLARE_ENUM(CloseType, CloseType);
		DECLARE_ENUM(uint8, CloseAuthorization);
		DECLARE_BOOL(CloseGame);
		DECLARE_PROPERTY(uint8, LoginAttempts);
		DECLARE_FLAG(uint16, SpecialCharacter);

		DECLARE_PROPERTY(int32, VIPStatus);
		DECLARE_PROPERTY(time_t, VIPDuration);

		DECLARE_ENUM(CharacterHandleAction, CharacterHandleAction);

		DECLARE_ENUM(uint8, CurrentCharacterIndex);

		DECLARE_STRING_FIXED(Mac, MAX_ACCOUNT_MAC_LENGTH + 1);
		DECLARE_STRING_FIXED(IP, MAX_ACCOUNT_IP_LENGTH + 1);

		DECLARE_ENUM(uint8, FacebookStatus);

		DECLARE_STRING_FIXED(GameServerSerial, 17);

		DECLARE_PROPERTY(time_t, GoldenChannel);
		DECLARE_ENUM(uint32, DiskSerial);
};

struct ComboData
{
public:

	void Init()
	{
		this->GetTime()->Reset();
		this->ResetSkill(0);
		this->SetProgressIndex(-1);
	};

	DECLARE_STRUCT(TickTimer, Time);
	DECLARE_PROPERTY_ARRAY(uint16, Skill, 3);
	DECLARE_PROPERTY(int32, ProgressIndex);
};

class PlayerHelperSetting
{
private:
	DECLARE_BOOL(Loaded);
	DECLARE_BOOL(Started);
	DECLARE_ENUM(int32, Stage);
	DECLARE_ENUM(uint32, StageTick);
	DECLARE_ENUM(uint32, CostTick);

	// Hunting
	DECLARE_PROPERTY_ARRAY(uint16, AttackSkill, 3);
	DECLARE_PROPERTY_ARRAY(uint16, BuffSkill, 3);
	DECLARE_PROPERTY_ARRAY(uint16, Delay, 2);
	DECLARE_PROPERTY(uint16, TimeSpaceCasting);
	DECLARE_PROPERTY(uint8, HuntingRange);
	DECLARE_PROPERTY(uint16, MovementTime);
	DECLARE_PROPERTY_ARRAY(uint16, ItemBuff, 3);
	
	// Obtaining
	DECLARE_PROPERTY(uint8, ItemPickFlags);
	DECLARE_PROPERTY(uint8, PickRange);
	DECLARE_PROPERTY_ARRAY(uint32, OptionFlags, 2);
	DECLARE_PROPERTY(uint8, AutoPotPercent);
	DECLARE_PROPERTY(uint8, AutoHealPercent);
	DECLARE_PROPERTY(uint8, PartyHealPercent);
	DECLARE_PROPERTY(uint8, DrainLifePercent);

	DECLARE_BOOL(Offline);
	DECLARE_ENUM(uint16, SkillID);
	DECLARE_PROPERTY(uint8, CurrentPartyMember);
	DECLARE_PROPERTY(uint8, CurrentPartySkill);
	DECLARE_ENUM(uint32, BuffCastingTime);

	DECLARE_PROPERTY_ARRAY(uint32, SkillAttackDelay, 2);
	DECLARE_ENUM(uint32, OriginalPositionTime);

	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);

	DECLARE_PTR(Unit, Target);
	DECLARE_ENUM(int32, TargetRange);

	DECLARE_ENUM(uint32, AttackTime);

	DECLARE_ENUM(TCType, StartedTime);

	DECLARE_BOOL(Disconencted);
	DECLARE_ENUM(uint32, DisconnectTime);

	DECLARE_ENUM(uint32, SkillTime);
	DECLARE_ENUM(TCType, LastMoveTime);

	DECLARE_ENUM(int32, AttackAngle);

	DECLARE_ENUM(uint32, DieTime);
public:
	bool IsItemPickFlag(uint8 flag) const { return this->GetItemPickFlags() & flag; }

	bool IsAutoPot() const { return this->GetOptionFlags(0) & OPT_AUTOPOT; }
	bool IsAutoHeal() const { return this->GetOptionFlags(0) & OPT_AUTOHEAL; }
	bool IsDrainLife() const { return this->GetOptionFlags(0) & OPT_DRAIN_LIFE; }
	bool IsLongDistanceCtAttack() const { return this->GetOptionFlags(0) & OPT_LONG_DISTANCE_COUNTER_ATTACK; }
	bool IsOriginalPosition() const { return this->GetOptionFlags(0) & OPT_ORIGINAL_POSITION; }
	bool IsOptCombo() const { return this->GetOptionFlags(0) & OPT_COMBO; }
	bool IsOptParty() const { return this->GetOptionFlags(0) & OPT_PARTY; }
	bool IsOptReferenceOfPartyHeal() const { return this->GetOptionFlags(0) & OPT_REFERENCE_OF_PARTY_HEAL; }
	bool IsBuffDurationForAllParty() const { return this->GetOptionFlags(0) & OPT_BUFF_DURATION_FOR_ALL_PARTY; }
	bool IsDarkSpirit() const { return this->GetOptionFlags(0) & OPT_USE_DARK_SPIRIT; }
	bool IsBuffDuration() const { return this->GetOptionFlags(0) & OPT_BUFF_DURATION; }
	bool IsOptRepairItems() const { return this->GetOptionFlags(0) & OPT_REPAIR_ITEMS; }
	bool IsOptPickAllNearItems() const { return this->GetOptionFlags(0) & OPT_PICK_ALL_NEAR_ITEMS; }
	bool IsOptPickSelectedItems() const { return this->GetOptionFlags(0) & OPT_PICK_SELECTED_ITEMS; }
	bool IsOptDarkSpiritAutoAttack() const { return this->GetOptionFlags(0) & OPT_DARK_SPIRIT_AUTO_ATTACK; }
	bool IsOptDarkSpiritAttackTogether() const { return this->GetOptionFlags(0) & OPT_DARK_SPIRIT_ATTACK_TOGETHER; }
	bool IsAutoAcceptParty() const { return this->GetOptionFlags(0) & OPT_AUTO_ACCEPT_PARTY; }
	bool IsAutoAcceptGuildMember() const { return this->GetOptionFlags(0) & OPT_AUTO_ACCEPT_GUILD_MEMBER; }
	bool IsUseEliteManaPotion() const { return this->GetOptionFlags(0) & OPT_USE_ELITE_MANA_POTION; }
	bool IsUseSkillClosely() const { return this->GetOptionFlags(0) & OPT_USE_SKILL_CLOSELY; }
	bool IsUseRegularAttack() const { return this->GetOptionFlags(0) & OPT_USE_REGULAR_ATTACK; }
	bool IsPickEventItem() const { return this->GetOptionFlags(0) & OPT_PICK_EVENT_ITEMS; }

	bool IsBuffItem() const { return this->GetOptionFlags(1) & HELPER_OPTION_BUFF_ITEM; }

	bool IsSkillDelay(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_DELAY: OPT_SKILL_3_DELAY); }
	bool IsSkillCondition(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_CONDITION: OPT_SKILL_3_CONDITION); }
	bool IsSkillConditionMobAttackingMe(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_MONSTER_ATTACKING_ME: OPT_SKILL_3_MONSTER_ATTACKING_ME); }
	bool IsSkillConditionMobMoreThan5(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_MORE_THAN_5_MONSTERS: OPT_SKILL_3_MORE_THAN_5_MONSTERS); }
	bool IsSkillConditionMobMoreThan4(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_MORE_THAN_4_MONSTERS: OPT_SKILL_3_MORE_THAN_4_MONSTERS); }
	bool IsSkillConditionMobMoreThan3(uint8 id) const { return this->GetOptionFlags(0) & (id == 0 ? OPT_SKILL_2_MORE_THAN_3_MONSTERS: OPT_SKILL_3_MORE_THAN_3_MONSTERS); }

	void SetFlag(uint8 type, uint32 flag) { this->SetOptionFlags(type, this->GetOptionFlags(type) | flag); }
	void ClearFlag(uint8 type, uint32 flag) { this->SetOptionFlags(type, this->GetOptionFlags(type) & ~flag); }

	bool IsFlag(uint8 type, uint32 flag) const { return (this->GetOptionFlags(type) & flag) != 0; }
	
	void Reset()
	{
		this->SetLoaded(false);
		this->SetStarted(false);
		this->SetStageTick(0);
		this->SetCostTick(0);
		this->ResetAttackSkill(0);
		this->ResetDelay(0);
		this->ResetBuffSkill(0);
		this->SetTimeSpaceCasting(0);
		this->SetHuntingRange(1);
		this->SetMovementTime(0);
		this->SetItemPickFlags(0);
		this->SetPickRange(1);
		this->ResetOptionFlags(0);
		this->SetAutoPotPercent(50);
		this->SetAutoHealPercent(50);
		this->SetPartyHealPercent(50);
		this->SetDrainLifePercent(50);
		this->ResetItemBuff(-1);

		for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; i++ )
		{
			memset(this->item_name[i], 0, HELPER_ITEM_LENGTH + 1);
		}

		this->SetStage(0);
		this->SetOffline(false);
		this->SetSkillID(0);
		this->SetCurrentPartyMember(0);
		this->SetCurrentPartySkill(0);
		this->SetBuffCastingTime(GetTickCount());
		this->ResetSkillAttackDelay(GetTickCount());
		this->SetOriginalPositionTime(GetTickCount());
		this->SetX(0);
		this->SetY(0);
		this->SetTarget(nullptr);
		this->SetTargetRange(0);
		this->SetAttackTime(GetTickCount());
		this->SetStartedTime(MyGetTickCount());
		this->SetDisconencted(false);
		this->SetDisconnectTime(GetTickCount());
		this->SetSkillTime(GetTickCount());
		this->SetLastMoveTime(MyGetTickCount());
		this->SetAttackAngle(0);
		this->SetDieTime(GetTickCount());
	}

	char item_name[PLAYER_HELPER_MAX_ITEMS][HELPER_ITEM_LENGTH + 1];
};

typedef UNORDERED_MAP<uint8, QuestEvolutionStatus> QuestEvolutionStatusMap;

class Player: public Unit
{
	non_copyable(Player);

	private:
		uint8 GetCharacterEmptySlot();
		uint8 GetCharacterListIndex(const char * name);
		
	public:
		explicit Player(uint16 entry);
		virtual ~Player();
		
		DECLARE_STRUCT(AccountData, AccountData);

		void CreateSocket(std::shared_ptr<WorldSocket> sock);
		std::shared_ptr<WorldSocket> m_Socket;

		class PlayerSaveTransaction: public TransactionCallBack
		{
			public:
				explicit PlayerSaveTransaction(Player* pPlayer, uint8 operation);
				virtual ~PlayerSaveTransaction();

				void Finished();
				void Cancelled();

			public:
				DECLARE_PTR(Player, Player);
				DECLARE_ENUM(uint8, Operation);
		};

	public: // Player Socket
		void sendPacket(uint8 * packet, uint16 len);
		void sendPacket(uint8 * packet);
		void SendPacket(C1_HEADER * packet);
		void SendPacket(C2_HEADER * packet);
		void SendPacket(GamePacket * packet);

		void CloseSocket();
		void kick(std::string const& reason, const char * function);
		void UpdatePacket();
		void AddPacket(WorldPacket* packet);
		bool Update(uint32 diff) override;
		bool CloseSocketPassed(CloseSocketTimes id, uint32 time, std::string const& name);
		void InitCloseSocketTime(CloseSocketTimes id);

		LockedQueue<WorldPacket*> _recvPacket;

		void ResetCharacterHandleData();
	public: /* Callbacks */
        void ProcessQueryCallbacks();

		PreparedQueryResultFuture CallBackCashShopGiftSend;
		//PreparedQueryResultFuture CallBackWhiteList;

		QueryResultHolderFuture _charListCallBack;
		PreparedQueryResultFuture _charSelectCallBack_1;
		QueryResultHolderFuture _charSelectCallBack_2;
		PreparedQueryResultFuture _friendAddCallBack;
		PreparedQueryResultFuture _mailAddCallBack;
		PreparedQueryResultFuture _mailUpdateCallBack;
		PreparedQueryResultFuture _genRankingCallBack;
		PreparedQueryResultFuture CallBackCashShopCoin;
		PreparedQueryResultFuture CallBackCashShopGift;

	private:
		DECLARE_STRING_FIXED(AccountSafe, (MAX_ACCOUNT_LENGTH + 1));
		DECLARE_STRING_FIXED(PasswordSafe, MAX_PASSWORD_LENGTH + 1);

		char requested_friend[MAX_CHARACTER_LENGTH + 1];
		FRIEND_MAIL mail_data;
	public:
		void ClearAccount();

		bool IsPlaying() const;

		void LoginBoxSend();
		void LoginRequest(uint8 * Packet, bool autologin = false);
		void LoginResult(uint8 result);
	
		void LogoutRequest(uint8 * Packet);
		void LogoutResult(uint8 flag);
		void LogoutTimeCheck();

		void SaveDBAccount(SQLTransaction& trans);

		void WarehouseClose();
		void WarehouseCloseResult();
		void WarehousePasswordRequest(uint8 * Packet);
		void WarehouseStateSend(uint8 state);
		void WarehouseMoneyInOut(uint8 * Packet);
		void WarehouseMoneyInOutResult(uint8 result, uint32 w_money, uint32 i_money);
		uint32 WarehouseGetUsedHowMuch();

		void GameClose(bool close = true);
		void Close(int8 count, CloseType type);

		void CharacterListRequest();
		void CharacterListRequestCallBack(QueryHolder holder);

		void CharacterCreate(uint8 * Packet);
		void CharacterCreateResult(uint8 result);

		void CharacterDelete(uint8 * Packet);
		void CharacterDeleteResult(uint8 result);

		void CharacterNameCheck(uint8 * Packet);
		void CharacterSelect(uint8 * Packet, bool server_move = false);
		void CharacterSelectCallBack(PreparedQueryResult result);
		void CharacterLoginHandle(QueryHolder holder);
		void CharacterSelectMapCheck();
		bool CharacterSelectWorldSuccess();

		void CharacterStatPointAdd(uint8 * Packet);

		void GameOptionUpdate(uint8 * Packet);

		void CharacterAction(uint8 * Packet);
		void CharacterMove(uint8 * Packet);

		void ItemMove(uint8 * Packet);
		void ItemMoveResult(uint8 result, uint8 position = 0, uint8 * iteminfo = nullptr);
		
		void ItemDrop(uint8 * Packet, bool die);
		void ItemDropResult(uint8 result, uint8 slot);

		bool ItemGet(uint8 * Packet, bool from_helper = false);
		void ItemGetResult(uint8 result = uint8(-1), uint8 * item_info = nullptr, uint16 index = -1);
		bool ItemGetStack(WorldItem * pWorldItem);

		void ItemUse(uint8 * Packet);
		void ItemUseResult();

		bool DuplicatedSerialFind(uint8 slot);

		void TalkToNpc(uint8 * Packet);
		void CloseInterface();

		void ItemBuy(uint8 * Packet);
		void ItemBuyResult(uint8 result = uint8(-1), uint8 * item_info = nullptr);

		void ItemSell(uint8 * Packet);
		void ItemSellResult(uint8 result, uint32 zen);

		void Ping(uint8 * Packet);
		void Ping();

		void protocol_core(uint8 * Packet, uint16 size, uint8 headcode);
		void UpdateMovement() override;

		bool PacketTimeCheck();

		bool CanCreateCharacter(uint8 race);

		void SendInventory(uint8 slot, uint8 flag = 0, bool clear = false);
		void SendEquipment(uint8 slot);

		//void SendEquipment();

		void SendItemDurability(uint8 slot, uint8 flag);

		void ClearCharacter();

		DECLARE(uint32, zen);

		DECLARE(uint8, preview[MAX_PREVIEW_DATA]);
		
		QuestEvolutionStatusMap quest_evolution;
		DECLARE_BOOL(QuestEvolutionSend);

		DECLARE_STRUCT(ComboData, Combo);

		DECLARE_ARRAY_STRUCT(FriendData, Friend, FRIEND_MAX);
		DECLARE_ARRAY_STRUCT(MailData, MailData, MAIL_MAX);

		DECLARE_PROPERTY_ARRAY(uint8, PowerFillCount, POWER_MAX);
		DECLARE_ARRAY_STRUCT(TickTimer, PowerAutorecuperationTime, POWER_MAX);

		DECLARE_ENUM(TransactionType, Transaction);

		DECLARE_ENUM(uint32, GuildID);

		DECLARE_ENUM(uint16, LastDurationSkill);
		DECLARE_ENUM(uint32, LastDurationSkillTime);
		DECLARE_PROPERTY(uint8, LastDurationSkillCount);
		DECLARE_ENUM(coord_type, LastDurationSkillX);
		DECLARE_ENUM(coord_type, LastDurationSkillY);

		DECLARE_STRUCT(PlayerHelperSetting, Helper);

		DECLARE_PROPERTY(int32, AccumulatedDamage);
		DECLARE_PROPERTY(int32, AccumulatedTime);

		int32 GetStrength() const { return (this->GetStat(STRENGTH) + this->GetAddStat(STRENGTH)); }
		int32 GetAgility() const { return (this->GetStat(AGILITY) + this->GetAddStat(AGILITY)); }
		int32 GetVitality() const { return (this->GetStat(VITALITY) + this->GetAddStat(VITALITY)); }
		int32 GetEnergy() const { return (this->GetStat(ENERGY) + this->GetAddStat(ENERGY)); }
		int32 GetLeadership() const { return (this->GetStat(LEADERSHIP) + this->GetAddStat(LEADERSHIP)); }
		int32 GetTotalStat(uint8 stat) const { return (this->GetStat(stat) + this->GetAddStat(stat)); }

		int32 GetOnlyEquipmentStat(uint8 stat)
		{
			int32 skill_tree_stat = 0;

			switch (stat)
			{
			case STRENGTH:
			{
				skill_tree_stat = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_STRENGTH1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_STRENGTH2));
			} break;

			case AGILITY:
			{
				skill_tree_stat = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_DEXTERITY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_DEXTERITY2));
			} break;

			case VITALITY:
			{
				skill_tree_stat = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_VITALITY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_VITALITY2));
			} break;

			case ENERGY:
			{
				skill_tree_stat = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_ENERGY1)) + static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_ENERGY2));
			} break;

			case LEADERSHIP:
			{
				skill_tree_stat = static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_LEADERSHIP));
			} break;
			}

			return this->GetStat(stat) + skill_tree_stat;
		}

		int16 GetTotalLevel() const
		{
			int16 level = this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();

			if ( this->IsMaster() )
			{
				level += this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel();
			}

			if ( this->IsMajestic() )
			{
				level += this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel();
			}

			return level;
		}

		int16 GetLevel() const { return this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel(); }

		void CalculateCharacter();

		void CalculateItemStat();
		bool ValidItem(Item const* item);
		void Calculate380Item();
		void CalculateHarmonyItem();
		void CalculateExeItem();
		void CalculateSocketItem(bool flag);
		void CalculateAncientItemStat();
		void CalculateAncientItem();
		//void GetAncientItemOption(uint8 * pSetOptionTable, uint8 * pSetOptionCountTable, int32 & pSetOptionCount);
		bool GetAncientOption(uint8 option_id, int32 OptTableNum, uint8 * option, int32 * value);
		void SetAncientOptionStat(uint8 option, int32 value);
		void SetAncientOption(uint8 option, int32 value);
		bool GetAncientFullOption(uint8 option_id, int32 count, uint8 * option, int32 * value);
		void SetAncientMastery(bool stat);
		void SetAncientMastery(bool stat, uint8 option_id, int32 value, int32 level);
		int32 GetSpecialItemExperience();
		void ApplyStats1();

		void ApplySocketEffect(bool flag, SocketDataType socket, uint8 * socket_count);
		void ApplySocketBonus(bool flag, uint8 socket_bonus);
		void ApplySocketPackage(bool flag, uint8 * socket_count);

		void ApplyMasteryBonus(bool stat);

		void ApplyWingGradedOption(bool stat);
		void ApplyWingErrtelOption(bool stat);

		void ApplyEarringOption(uint8 slot, bool stat);
		void ApplyEarringOption(uint8 level, uint8 group, uint8 option, uint8 value, bool stat);
		void ApplyEarringBonus(bool stat);

		void CalculateMaxHP();
		void CalculateMaxMana();
		void CalculateMaxShield();
		void CalculateMaxStamina();

		void SendStats(bool flag = true);

		bool IsMaster() const;
		bool IsMajestic() const;

		void CalculateNextExperience();
		int64 GetNextExperience(int16 add_level = 1) const;

		void AddNormalPoints(int32 points, uint8 stat);

		void AutoRefill();

		void UpdateInterface();
		
		void TeleportRequest(uint8 * Packet);
		void TeleportRequest(TeleportData const* teleport, uint8 event_id = 0);

		void TeleportRequestSecond(uint8 * Packet);
		void TeleportRequestTarget(uint8 * Packet);
		void TeleportResult(uint16 id, bool regen = true);
		bool MoveToGate(uint16 gate_id);
		void TeleportToLocation(world_type world, coord_type x, coord_type y, uint8 direction, int32 instance); // Lo mueve a una zona definida
		void TeleportToLocation(world_type world);
		void TeleportToLocation(); // Lo mueve al ultimo lugar donde estuvo
		void TeleportToDelayed();
		void TeleportToGate(uint16 gate);
		void StartRegen(world_type world, coord_type x, coord_type y, uint8 direction, int32 instance, uint16 gate_id = 0);
		void MoveFail();
		void TeleportWorld(TeleportData const*, bool pk_check = true);
		
		void DataLoadingConfirm();

		void UpdateStatus();

		bool IsSpecialCharacter();

		void LoadDefaultSkill();

		uint8 GetManaPotionSlot();
		void DecreaseItemDurabilityByUse(uint8 position, float _durability);
		void ItemDelete(uint16 item, uint8 level, int32 count);
		void ItemDeleteByUse(uint8 slot);

		bool UpgradeItemCommon(uint8 source_slot, uint8 target_slot);
		void UpgradeItemSuccess(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingBless(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingSoul(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingLife(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingHarmony(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingRefineStone(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingExtension(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingElevation(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingTradeableSeal(uint8 source_slot, uint8 target_slot);
		bool UpgradeItemUsingMysteriousStone(uint8 source_slot, uint8 target_slot);

		void ClearItem(uint8 slot, uint8 send = 1);

		void ItemRepairRequest(uint8 * Packet);
		void ItemRepairSingle(uint8 slot, uint8 repair_way);
		void ItemRepair(uint8 slot, uint8 repair_way, bool autorepair = false);
		void ItemRepairResult(uint32 zen = 0);

		bool BeginTransaction();
		bool CommitTransaction();
		bool RollBackTransaction();

		void CancelAllActivities();

		void MonsterDiePlus(Unit * mMonster);

		void SendSummonedHP(int32 hp, int32 max_hp);

		void IncreaseAggroAround(int64 value);

		void UpdateAutorecuperation();

		uint8 GetKalimaLevelEntrance();
		bool CreateKalimaGate(uint8 level, coord_type x, coord_type y);
		bool GetKalimaGateRandomLocation(coord_type & x, coord_type & y);

		void PetInfoRequest(uint8 * Packet);
		void PetInfoResult(uint8 pet, uint8 type, uint8 slot);
		void PetInfoLevelUp(uint8 slot, uint8 pet);

		bool CreateMercenary(uint8 level, coord_type x, coord_type y);
		bool CreateLifeStone();

		void CommandSend(uint8 type, uint8 cmd1, uint8 cmd2);

		void DamagePet(int32 damage);
		void DamageMount(int32 damage);
		void DamageMount(int32 damage, Item* pItem);
		void ApplyWingDamage(int32 & damage, uint16 skill);

		void ApplyFenrirEquipmentReduction();

		void ItemDurabilityDown(DurabilityState state, uint8 slot);
		void WeaponDurabilityDown(Unit* pTarget, bool melee);
		void DecreaseWeaponDurabilityInCastle(float value = 1.0f);
		
		void UpdateDurabilityTime();
		void RandomArmorDurabilityDown(Unit* mAttacker, int32 damage);
		void RandomArmorDurabilityDownPercent(Unit* mAttacker, int32 percent);

		bool IsInSelfDefense();
		bool IsInSelfDefense(Player* pPlayer);
		void SelfDefenseActive(Player* pPlayer);
		void SelfDefenseUpdate();
		void SelfDefenseClear();

		void Respawn();
		void GenerateRespawnLocation(bool from_select = false);

		void LocationChange();
		void UpdateDelayedTeleport();

		void InventoryItemUse(uint8 * Packet);
		bool InventoryItemUse(uint8 use_type, uint8 slot, bool from_packet = false);
		void InventoryItemClearGroup(uint16 item);
		void InventoryItemUseResult(uint8 slot, uint8 use_type);
		void InventoryItemSend();

		bool hasMoonstoneRing();

		void Summon(uint32 id, uint32 time);

		void EnterWorldSuccess(uint8 * Packet);

		void UpdateStatistic(uint32 statistic_id, uint64 count);
		uint64 GetStatistic(uint32 statistic_id);
		void ReduceStatistic(uint32 statistic_id, uint64 count);

		void NpcInteractionBarrackTeleport();
		void NpcInteractionRefugeTeleport();

		void NpcInteractionResult(uint8 npc, uint8 result);

		void AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag) override;

		void UpdateWrongWorld();

		bool SummonPersonalMerchant();
		bool SummonPersonalWarehouse();

		void FinalClose();

		bool AttackAllowedToPlayer(Player* pPlayer, bool targeted);
		void PKLevelSend();
		void PKUpdate(int32 level);
		void PKUpdateMurder();
		void PKUpdateHero();
		static void PKPlayerMurder(Player* pPlayer, Unit* pKiller, uint8 damage_flag);
		void PKModifyStatus(int32 count, uint8 type);
		uint8 PKMurderCalculate() const;
		uint8 PKHeroCalculate() const;
		bool UseIndulgence();
		uint8 GetPKLevelGroup() const;

		bool WearItem(Item const* mItem, uint8 slot);
		void WearedItemApplySkill();
		void WearedItemRemove(uint8 slot);

		void PreviewMake();
		void PreviewSend();

		void SantaClausGift();
		void SnowmanMove();

		bool SummonCheckWorld();
		bool IsBusy();

		void Autosave();

		void PositionSet(uint8 * Packet);

		int32 MagicAddWeapon(uint16 skill, uint8 level);
		void MagicDelWeapon(uint16 skill, uint8 level);
		bool MagicFindSkillTree(uint16 skill) const;
		float MagicGetValue(uint16 skill, bool majestic = false) override;
		Skill const* MagicGetByBase(uint16 skill) const;
		Skill * MagicGetByBase(uint16 skill);

		int32 GetHorseDamage() const;

		uint32 MoneyGet() const { return this->zen.get(); }
		void MoneySet(uint32 ammount, bool send = false);
		bool MoneyReachMaximum(uint32 ammount) const;
		void MoneyAdd(uint32 ammount, bool send = true);
		void MoneyReduce(uint32 ammount, bool send = true);
		bool MoneyHave(uint32 ammount) const;

		void TownPortalScrollUse(uint8 slot);

		void OnDie(Unit* pUnit);

		void LorenMarketTeleport();

		void SendEventAnimation(uint8 event_id, uint8 state);
		void RestoreEventAnimation();

		void SendWeather(uint8 weather);
		void SendWeather();

		//void AdminPanelUpdate(uint8 * Packet);

		void SendDataSingle(Object* pObject);

		void CheckAddInfinityArrow();
		void SendEventEnterCount();

		bool IsWearingPet(uint16 pet);

		void UpdateGoblinPoints(Monster* pMonster);
		void UpdateGoblinPoints();

		void EquipmentAutochange(uint8 slot, uint16 item, uint8 level);

		void AccumulateDamage(int32 & damage);
		void PVPDamageReduction(int32 & damage, Player* pPlayer);

		void SendWorldAttribute(uint8 type, uint8 attribute, uint8 count, bool apply, coord_type const* data);

		void UpdatePowers(uint8 type, bool max = false) override;
		void LifeSend(bool max = false);
		void ManaSend(bool max = false);

		void PlaySoundRequest(int32 id, uint8 data1 = 0, uint8 data2 = 0);

		void BuildCustomPacketData(CUSTOM_PACKET_PLAYER & data);

		ViewportData* GetViewportItemByID(uint16 id);

		bool CanEnterWorld(world_type world, coord_type x1, coord_type y1, coord_type x2, coord_type y2, bool send_message = false);

		void OpenSealedBoxAttempt(uint8 slot);

		void UpdateRestriction();

		bool CanInteract(Unit* pTarget = nullptr, bool target = false);

		void ProcessAttackSafe();

		void LogoutEventRemove();

		bool IsViewportControl() const { return true; }

		bool HasSkillRequiredStat(skill_template const* skill_info);

		void HackReport(uint8 * Packet);

		void ChangeSkinOption(uint8 * Packet);

		void MuunUpdate();

		uint8 GetFreeInventorySlots() const;

		bool CheckItemMoveToInventory(Item const* pItem, uint8 slot);
		bool CheckItemMoveToTrade(Item const* pItem);
		bool CheckItemMoveToVault(Item const* pItem);
		bool CheckItemMoveToChaos(Item const* pItem, uint8 target);

		uint8 MoveItem(Inventory, Inventory);

		uint8 MoveItem(Trade, Trade);
		uint8 MoveItem(Trade, Inventory);
		uint8 MoveItem(Inventory, Trade);

		uint8 MoveItem(ChaosBox, ChaosBox);
		uint8 MoveItem(ChaosBox, Inventory);
		uint8 MoveItem(Inventory, ChaosBox);

		uint8 MoveItem(Warehouse, Warehouse);
		uint8 MoveItem(Warehouse, Inventory);
		uint8 MoveItem(Inventory, Warehouse);

		uint8 MoveItem(Trade, EventInventory);
		uint8 MoveItem(EventInventory, Trade);
		uint8 MoveItem(EventInventory, EventInventory);

		uint8 MoveItem(ChaosBox, EventInventory);
		uint8 MoveItem(EventInventory, ChaosBox);

		uint8 MoveItem(EventInventory, Inventory);

		uint8 MoveItem(Warehouse, EventInventory);
		uint8 MoveItem(EventInventory, Warehouse);

		uint8 MoveItem(MuunInventory, MuunInventory);

		bool InventoryAddItemStack(uint8 source_slot, uint8 target_slot);
		bool InventoryAddItemStackOnBuy(Item & item);

		void OffensiveFullRestore();

		void GetPreviewElementalDamage(int32 & damage_min, int32 & damage_max, int32 & pvp_damage_min, int32 & pvp_damage_max);
		void GetPreviewElementalDefense(int32 & defense, int32 & pvp_defense);
		void GetPreviewElementalAttackSuccessRate(int32 & attack_success_rate, int32 & pvp_attack_success_rate);
		void GetPreviewElementalDefenseSuccessRate(int32 & defense_success_rate, int32 & pvp_defense_success_rate);

		bool PotionUse(uint16 item, uint8 level, uint16 value);
		bool ExperiencePotionUse(uint16 item, uint8 level);

		void OpenBoxRequest(uint8 * Packet);
		void OpenBoxResult(uint8 result, uint16 item = -1);

		void ChatBlockAdd(uint8 * Packet);
		bool ChatBlockAdd(const char * name);
		void ChatBlockDelete(uint8 * Packet);
		void ChatBlockList(uint8 type, uint8 result);
		bool ChatBlockFind(const char * name) const;

		void ItemDisassemble(uint8 * Packet);
		void ItemDisassembleResult(uint8 result, uint32 item = -1, uint32 count = 0);

		void SendEventNotification(uint8 event_id, uint8 open);

		static bool IsPlayerBasicState(Player* pPlayer, bool packet_time = true, bool live = true);
		static bool IsPlayerTransactionFree(Player* pPlayer);

		void AcheronWarp();

		void SendRuudMoney();
		void SendRuudMoney(uint32 add);

		bool HasMount() const;
		bool IsActiveInventoryItem(uint16 item) const;
		Item* GetActiveInventoryItem(uint16 item);
		Item const* GetActiveInventoryItem(uint16 item) const;
		Item const* GetActiveGuardianMount() const;
		void SendMount(Player* pPlayer = nullptr);
		void ResetMount(uint16 item);
		void AssignMount();

		uint8 AddItem(Item const& item, uint8 & inventory, uint8 slot, AddItemSend send);
		uint8 AddItem(Item const& item, uint8 slot, AddItemSend send);

		bool IsOffline() const;

		bool IsBuffAllowed(uint16 id) const;

		bool IsTimeout() const;

		void UpdateBuffID();
		void UpdateBuffID(uint16 normal_id, uint16 master_id);

		void UpdateAccountStatusType(uint8 type);

		void UpdateCoordinateAttribute();

		void AddItemToSoldList(Item const& item, uint32 price);
		void ItemSoldListView();
		void ItemSoldListClose();
		void ItemSoldRecover(uint8 * Packet);

		void MixSafeItemRecoverAttempt();

		void GoldenChannelUpdate();

		float GetWingAbsorb() const;
		float GetWingDamage() const;
		float GetMountAbsorb() const;
		float GetMountDamage() const;
		float GetPetAbsorb() const;

		void AddEventScore(uint8 type, int32 level, int32 score);

		void AttackSpeedSend();
		void AttackSpeedRecv(uint8 * Packet);
		void HackCheckVerify(uint8 * Packet, uint8 id);
		void HackCheckVerify();

		void CheatScanDetect(uint8 * Packet);

		//void WhiteListRequest();
		//void WhiteListCallBack(PreparedQueryResult result);

		bool IsSkillCooldown(uint16 skill);
		bool CheckSkillCooldown(uint16 skill);
		uint32 GetSkillCooldown(uint16 skill);

		void SendExperienceEvent();

		void SendMessageBox(uint8 type, const char * caption, const char * message, ...);

		bool InmuneToRadiance() const;
		bool InmuneToPunish(Player* pPlayer);
		void ApplyRadianceDebuff(Unit* pTarget, uint16 effect, int32 & punish_damage, int32 count);

		void CreateDarkAncientItem(uint8 * Packet);

		uint8 GetMatchingClass() const;

		uint8 ItemFind(uint16 item, uint8 level, bool consume = false);
		uint8 FindExpirableItem(uint16 item, uint8 level) const;

		uint8 GetEventEnterCount(uint8 event_id) const;
		void UpdateEventEnterCount();

		int32 GetDarkHorseAbsorb() const;

		void WarpFavoriteListSave(uint8 * Packet);
		void WarpFavoriteListSend();

		void CentQuestStartRequest(uint8 * Packet);
		void CentQuestSpawnMonster(uint8 * Packet);
		void CentQuestMoveRequest(uint8 * Packet);

		void ItemSplit(uint8 * Packet);
		bool FillMagicSolution(uint8 source_slot, uint8 target_slot);
		void PlayerRightClick(uint8 * Packet);

		bool MajesticTreeReset();
		void ClientMinimize(uint8 * Packet);

		void WingSocketOptionAdd(uint8 * Packet);
		void WingSocketOptionAddResult(uint8 result);

		void WingSocketOptionUpgrade(uint8 * Packet);
		void WingSocketOptionUpgradeResult(uint8 result, uint8 slot);
		
		void CheckSkill(int32 count = 0);

		bool IsItemEquipped() const;

		void SendPCRoom();

		void EarringUpgrade(uint8 * Packet);
		void EarringUpgradeResult(uint8 result);

		void SendServerList();
		void ServerWarp(uint8 * Packet);

		void SendAntiMacro();
		void AntiMacroDetect(uint8 * Packet);

		void KuberaMineCart(uint8 * Packet);

		void PetTrainerMix1(uint8 * Packet);
		void PetTrainerMix2(uint8 * Packet);

		void SendPetTrainerMix1Result(uint8 type, uint8 result);
		void SendPetTrainerMix2Result(uint8 result);
	// Load from DB
		void LoadDB(QueryHolder holder);
		void LoadDBInfo(PreparedQueryResult result);
		void LoadDBItem(PreparedQueryResult result);
		void LoadDBSkill(PreparedQueryResult result);
		void LoadDBGameOption(PreparedQueryResult result);
		void LoadDBQuestEvo(PreparedQueryResult result);
		void LoadDBFriendList(PreparedQueryResult result);
		void LoadDBMailList(PreparedQueryResult result);
		void LoadDBGuild(PreparedQueryResult result);
		void LoadDBGen(PreparedQueryResult result, bool update = true);
		void LoadDBGenKill(PreparedQueryResult result);
		void LoadDBHelper(PreparedQueryResult result);
		void LoadDBBuff(PreparedQueryResult result);
		void LoadDBRestriction(PreparedQueryResult result);
		void LoadDBStatistics(PreparedQueryResult result);
		void LoadDBItemDelay(PreparedQueryResult result);
		void LoadDBChatBlock(PreparedQueryResult result);
		void LoadDBCashShopCoin(PreparedQueryResult result);
		void LoadDBCashShopItemList(PreparedQueryResult result);
		void LoadDBItemRecovery(PreparedQueryResult result);
		void LoadDBItemSell(PreparedQueryResult result);
		void LoadDBHuntingRecord(PreparedQueryResult result);
		void LoadDBPentagramInfo(PreparedQueryResult result);
		void LoadDBMuun(PreparedQueryResult result);
		void LoadDBEventInventory(PreparedQueryResult result);
		void LoadDBGremoryCase(PreparedQueryResult result, ItemBox box);
		void LoadDBLabyrinth(PreparedQueryResult result);
		void LoadDBLabyrinthZone(PreparedQueryResult result);
		void LoadDBLabyrinthLevel(PreparedQueryResult result);

		void LoadDBEventCount(PreparedQueryResult result);
		void LoadDBMiniBomb(PreparedQueryResult result);
		void LoadDBMiniBombGrid(PreparedQueryResult result);

		void LoadDBMuRoomy(PreparedQueryResult result);
		void LoadDBMuRoomyDeck(PreparedQueryResult result);

		void LoadDBWarpFavoriteList(PreparedQueryResult result);
		void LoadDBQuestGuided(PreparedQueryResult result);

		void LoadDBJewelBingo(PreparedQueryResult result);
		void LoadDBJewelBingoGrid(PreparedQueryResult result);

		void LoadDBDate(PreparedQueryResult result);
		void LoadDBMajesticTree(PreparedQueryResult result);
		void LoadDBMajesticStats(PreparedQueryResult result);

		void LoadDBNumericBaseball(PreparedQueryResult result);
		void LoadDBMixRecovery(PreparedQueryResult result);

		void LoadDBQuestMU(PreparedQueryResult result);
		void LoadMonsterSoul(PreparedQueryResult result);

		void LoadDBItemPersonalStore(PreparedQueryResult result);

	// Save to DB
		void SaveCharacter(uint8 operation = 0);
		void SaveDBInfo(SQLTransaction& trans);
		void SaveDBItem(SQLTransaction& trans);
		void SaveDBSkill(SQLTransaction& trans);
		void SaveDBGameOption(SQLTransaction& trans);
		void SaveDBQuestEvo(SQLTransaction& trans);
		void SaveDBFriendList(SQLTransaction& trans);
		void SaveDBMailList(SQLTransaction& trans);
		void SaveDBGen(SQLTransaction& trans);
		void SaveDBGenKill(SQLTransaction& trans);
		void SaveDBHelper(SQLTransaction& trans);
		void SaveDBBuff(SQLTransaction& trans);
		void SaveDBRestriction(SQLTransaction& trans);
		void SaveDBStatistics(SQLTransaction& trans);
		void SaveDBItemDelay(SQLTransaction& trans);
		void SaveDBRegister(uint32 register_id, std::string register_data, Item const* item);
		void SaveDBItem(SQLTransaction & trans, uint8 rules);
		void SaveDBChatBlock(SQLTransaction & trans);
		void SaveDBCashShopItem(SQLTransaction& trans);
		
		void SaveDBHuntingRecord(SQLTransaction& trans);
		void SaveDBPentagramInfo(SQLTransaction & trans);
		void SaveDBLabyrinth(SQLTransaction & trans);
		void SaveDBLabyrinthZone(SQLTransaction & trans);
		void SaveDBLabyrinthLevel(SQLTransaction & trans);

		void SaveDBEventCount(SQLTransaction & trans);

		void SaveDBMiniBomb(SQLTransaction & trans);
		void SaveDBMuRoomy(SQLTransaction& trans);

		void SaveDBWarpFavoriteList(SQLTransaction& trans);
		void SaveDBQuestGuided(SQLTransaction& trans);

		void SaveDBJewelBingo(SQLTransaction& trans);

		void SaveDBDate(SQLTransaction& trans);
		void SaveDBMajesticTree(SQLTransaction& trans);
		void SaveDBMajesticStats(SQLTransaction& trans);

		void SaveDBNumericBaseball(SQLTransaction& trans);

		void SaveDBQuestMU(SQLTransaction& trans);
		void SaveDBMonsterSoul(SQLTransaction& trans);

	// Send To Client
		void SendJoinInfo();
		void SendJoinInfo(uint8 result);

		void SendInventory();
		void SendSkill();
		void SendSkill(uint8 slot, uint16 skill, uint8 level, bool add = true);
		void SendGameOption();
		void SendFriendList();

		void MoneySend();
		void MoneySend(uint32 _zen);

		void LevelUpAddPoints();
		void NormalSendLevelUp(bool effect = true);

		void SendNotice(NoticeType type, const char * notice, ...);
		void SendNotice(const char * name, const char * notice, ...);

		void SendNoticeNormal(NoticeType type, const char * notice);
		void SendNoticeNormal(const char * name, const char * notice);

		void SendNotice(CustomMessageID type, const char * notice, ...);
		void SendNoticeNormal(CustomMessageID type, const char * notice);

	// Skill Tree
		void MasterSendStatus();
		void MasterSendLevelUp();
		void MasterSkillPointAdd(uint8 * Packet);
		void MasterSkillSend();
		void MasterSkillSend(uint16 skill);
		
		Skill* GetSkillTreeSkill(uint16 skill);
		Skill const* GetSkillTreeSkill(uint16 skill) const;

		bool CheckMasterReplaceSkill(uint16 skill);
		bool CheckMasterRequireSkill(uint16 skill);
		bool CheckMasterRequireGroup(uint8 group, uint8 rank) const;
		bool ReplaceMasterSkill(uint16 replace, uint16 skill, uint8 level);

		Skill* AddSkillTreeSkill(uint16 skill, uint8 level);
		void MasterSkillSend(uint16 skill, uint16 id);

		void GetMasterSkillWeapon(uint16 & skill, uint8 & level);

		bool MasterSkillReset(uint8 group);

		bool GetMasterSkillLevel(uint16 skill, uint8 & level) const;

		void ApplySkillTree(bool stat);

	// Friend
		bool IsFriend(char const*) const;
		bool AddFriend(const char*, uint16);

		void FriendAddRequest(uint8 * Packet);
		void FriendAddCallBack(PreparedQueryResult result);
		void FriendAddResult(uint8 result, const char * name, uint8 state = 0xFF);

		void FriendDelRequest(uint8 * Packet);
		void FriendDelResult(uint8 result, char * name = nullptr);

		void FriendOnOff(const char * name, uint16 server, bool online);
		void FriendStateSend(const char * name, uint16 server);

		void FriendMailRequest(uint8 * Packet);
		void FriendMailRequestCallBack(PreparedQueryResult result);
		void FriendMailResult(uint8 result, uint32 window_guid);
		void FriendMailList();
		void FriendMailSend(uint16 id, const char * from, const char * date, const char * subject, uint8 status);
		void FriendMailReadRequest(uint8 * Packet);
		void FriendMailDeleteRequest(uint8 * Packet);
		void FriendMailDeleteResult(uint8 result, uint16 id);

		int32 FriendGetCount();

		MailData * GetMail(uint16 id);
		bool DeleteMail(uint16 id);
		void UpdateMailStatus(uint16 id, uint8 status);
		uint8 MailGetCount() const;
		bool MailAdd(MailData const& Mail);
		void MailNewUpdate();
		void MailNewUpdateCallBack(PreparedQueryResult result);
		void MailDBUpdate();

		void FriendChatRoomCreateRequest(uint8 * Packet);
		void FriendChatRoomCreateResult(uint8 result, const char * ip, uint16 room, uint32 ticket, uint8 type, const char * name);
		void FriendChatRoomInvitationRequest(uint8 * Packet);
		void FriendChatRoomInvitationResult(uint8 result, uint32 window_guid);

	// Personal Store
		void PersonalStoreClose(bool complete = true);
		void UpdatePersonalStore();
		//void PersonalStoreCreateCurrency(uint32 price, uint16 price_job, uint16 price_jos);
		//void PersonalStoreGiveJewel(uint16 item, uint32 price);

		static void PersonalStoreBuyLog(Player* pPlayer01, Player* pPlayer02, Item const& item, std::string const& log, bool main);

		void HandlePersonalStoreSearch(uint8 * Packet);
		void HandlePersonalStoreSearchItemList(uint8 * Packet);
		void HandlePersonalStoreView(uint8 * Packet);
		void HandlePersonalStoreSearchItem(uint8 * Packet);
		void HandlePersonalStoreSelectItem(uint8 * Packet);
		void HandlePersonalStorePurchase(uint8 * Packet);
		void HandlePersonalStoreSellList(uint8 * Packet);
		void HandlePersonalStoreSellItemView(uint8 * Packet);
		void HandlePersonalStoreSellItem(uint8 * Packet);
		void HandlePersonalStoreSellItemCancel(uint8 * Packet);
		void HandlePersonalStoreState(uint8 * Packet);

		void SendPersonalStoreList(uint32 page, uint32 data);
		void SendPersonalStoreItemList(uint32 page, uint32 data, uint16 item, std::string const& item_name);
		void SendPersonalStorePet();
	// Party
		void PartyRequest(uint8 * Packet);
		void PartyRequestResult(uint8 result);
		void PartyRequestAnswer(uint8 * Packet);
		void PartyDelete();
		void HandleRemovePartyMember(uint8 * Packet);
		void PartyRemove();
		void PartyItemGetNotify(WorldItem const* item);
		void PartySendDamage(Unit* pVictim, int32 damage, int32 shield_damage, uint8 damage_flag);
		void PartyLeaderChange(uint8 * Packet);
		Party* GetParty() const;
		void PartyMove(uint8 * Packet);
		void PartySummon(uint8 * Packet);
		void PartyHelper(uint8 * Packet);
		
	// Trade
		void TradeRequest(uint8 * Packet);
		void TradeRequestResult(uint8 result, const char * name, uint16 level = 0, uint32 guild_id = 0);
		void TradeRequestAnswer(uint8 * Packet);
		static void TradeBegin(Player* pPlayer01, Player* pPlayer02);
		void TradeZen(uint8 * Packet);
		void TradeZenResult(uint8 result);
		void TradeZenAnswer(uint32 zen);
		void TradeButtonOK(uint8 * Packet);
		void TradeButtonOKSend(uint8 result);

		void TradeItemDelete(uint8 slot);
		void TradeItemDeleteToTarget(uint8 slot);

		void TradeItemAdd(uint8 slot, uint8 * info);
		void TradeItemAddToTarget(uint8 slot, uint8 * info);

		void TradeCancelRequest();
		void TradeCancel();
		void TradeResult(uint8 result);

		static void TradeOK(Player* pPlayer01, Player* pPlayer02);

		bool TradeToInventory();

		static void TradeCancel(Player* pPlayer01, Player* pPlayer02, uint8 result);
		static void TradeItemLog(Player* pPlayer);

		static void TradeBuyLog(Player* pPlayer01, Player* pPlayer02, std::string const& log);

	// Guild
		void GuildCreateRequest(uint8 * Packet);
		void GuildCreateResult(uint8 result, uint8 type = 0);

		void GuildJoinRequest(uint8 * Packet);
		void GuildJoinAnswer(uint8 * Packet);
		void GuildJoinResult(uint8 result);
		
		void GuildMemberDelRequest(uint8 * Packet);
		void GuildMemberDelResult(uint8 result);

		void GuildNpcAnswer(uint8 * Packet);

		void GuildListAll();

		void GuildUpdateViewport();
		void GuildUpdateViewport(uint8 * Packet);
		void GuildUpdateViewportDel(bool master);
		void GuildUpdateViewportList1();
		void GuildUpdateViewportList2();

		void GuildAssignStatus(uint8 * Packet);
		void GuildAssignStatusResult(uint8 type, uint8 result, char * name);

		void GuildAssignType(uint8 * Packet);
		void GuildAssignTypeResult(uint8 type, uint8 result);

		void GuildRelationShipRequest(uint8 * Packet);
		void GuildRelationShipResult(uint8 relation, uint8 type, uint8 result, uint16 target);
		void GuildRelationShipAnswer(uint8 * Packet);
		void GuildRelationShipList();
		void GuildAllianceKick(uint8 * Packet);

		bool GuildIsAllianceMaster();

		uint8 GuildRelationShip(Player* pPlayer);

		Guild* GuildGet() const;
		uint8 GuildGetRanking() const;

		bool IsInGuildWar() const;
		bool IsInGuildWar(Player* pPlayer);

		int32 GuildGetMaxGuildMember() const;

	// Experience
		void GiveSingleExperience(Monster * mMonster, int32 damage);
		void GivePartyExperience(Monster * mMonster, int32 damage);

		void GiveExperience(int64 & experience, bool apply_buff = false, bool send = false, uint8 type = 0);
		void GivePetExperience(int64 experience);

		void KillExperienceSend(uint16 target, int64 experience, uint16 damage = 0, uint8 flag = 0);

		void ApplyBuffExperience(int64 & experience, uint8 count);

		void LogLevelUp();

	// Skill Use
		void NormalAttack(uint8 * Packet);
		void NormalMagicAttack(uint8 * Packet);
		void DurationMagicAttack(uint8 * Packet);
		void MultiTargetMagicAttack(uint8 * Packet);
		void NewMagicAttack(uint8 * Packet);
		void DarkSideAttack(uint8 * Packet);
		void PositionAttack(uint8 * Packet);
		bool IsSkillUseAllowed(skill_template const* skill_info);

		void MagicCancel(uint8 * Packet);

		void GetPartyMembers(Unit* pMember[MAX_PARTY_MEMBERS], uint8 & count, uint8 distance);

		bool SkillComboCheck(uint16 skill);
		int16 SkillComboGetPos(uint16 skill);

		void SkillGetDamage(uint16 skill, int32 & damage_min, int32 & damage_max);
		void SkillCastNovaUpdate();
		bool SkillCastNovaConsumePotion(int32 consume_mana);

		int32 SkillManaUse(Skill * mSkill, skill_template const* skill_info);
		int32 SkillStaminaUse(Skill * mSkill, skill_template const* skill_info);

		void ApplyStaffDamage(uint16 skill, int32 & damage_min, int32 & damage_max);
		void ApplyCurseDamage(uint16 skill, int32 & damage_min, int32 & damage_max);
		void ApplyWeaponDamage(uint16 skill, int32 & damage_min, int32 & damage_max);
		void ApplyDarkSpiritDamage(int32 & damage_min, int32 & damage_max);
		bool ConsumeSkillItem();

		void ApplyBuffValues(uint8 opt, float value);
		void ApplyBuffStatEffect();
		void ApplyBuffNonStatEffect();

		void SendClearBuff();
		void SendBuffEffect(BuffData * data, uint8 remove = 0);
		void ClearAllBuff(uint8 flag);

		void GetSkillDarkSideTargets(Unit* pTarget, skill_template const* skill_info, uint16 * target, int32 target_count);

		int32 GetBookCurseDamage() const override;
	// Helper
		void HelperSendSettings();
		void HelperSaveSettings(uint8 * Packet);
		void HelperOpenRequest(uint8 * Packet);
		void HelperOpenResult(uint8 result, uint32 zen = 0, uint16 spent_time = 0);
		bool HelperStart();
		void HelperStop(uint8 reason);
		void HelperUpdate();
		bool HelperSettingsUpdate();
		bool HelperCanLoot(WorldItem const* pWorldItem);
		void HelperAutoloot();
		void HelperPetZenLoot();
		void HelperOffline();
		void HelperOfflineHP();
		uint8 HelperOfflineHPSlot() const;
		bool HelperOfflineMP(Skill * pSkill, skill_template const* skill_info);
		uint8 HelperOfflineMP() const;
		void HelperOfflineSupport();
		void HelperOfflineAttack();
		void HelperOfflineRepair();
		void HelperOfflineRepair(uint8 slot);
		bool HelperOfflineSkillUse(Skill* pSkill, Unit* pTarget);
		int32 HelperOfflineGetMonsterWithinHuntingRange();
		Monster* HelperOfflineGetRandomTarget(uint16 skill, bool attacking_me);
		Player* HelperOfflineGetRandomPartyMember(bool heal, uint16 skill);
		bool HelperOfflineSpeed();
		void HelperOfflineMoveToTarget(Unit* pUnit, int32 range);
		void HelperOfflineReloadArrow();

		uint8 HelperGetAngle(Unit* pTarget);
		bool IsHelperSkill(uint16 skill) const;

		void HandleHelperPlusData(uint8 * Packet);
		void HandleHelperPlusRun(uint8 * Packet);
	// Duel
		void DuelRequest(uint8 * Packet);
		void DuelAnswer(uint8 * Packet);
		void DuelEndRequest(uint8 * Packet);
		void DuelResult(uint8 result, uint16 target, const char * name);
		void DuelSpectatorRequest(uint8 * Packet);
		void DuelSpectatorRemove(uint8 * Packet);

		bool IsOnDuel() const;
		bool IsOnDuel(Player* pPlayer);
		void UpdateDuelScore();
		bool IsSpectator() const;

	// Quest Support
		void ElfSoldierBuffUP(bool levelup = false);

		QuestMUData const* GetQuestMU(uint16 id) const;
		QuestMUData* GetQuestMU(uint16 id);

		void AddQuestMU(uint16 id, uint8 state, uint16 objective);
		void SetQuestMUState(uint16 id, uint8 state);
		void RemoveQuestMU(uint16 id);
		void SendQuestMU();

		void QuestMUObjectiveUpdate(QuestMUObjectiveType type, Unit* pUnit = nullptr, int32 data = 0);
		void QuestMUObjectiveItem(Item const& item);
		void QuestMUObjectiveMonsterKill(Monster* pMonster);
		void QuestMUObjectiveLevel();
		void QuestMUObjectiveLocation();
		void QuestMUObjectiveFindNPC(Monster* pMonster);
		void QuestMUObjectiveMonsterKillMap(Monster* pMonster);
		void QuestMUObjectiveChaosCastleClear();
		void QuestMUObjectiveChaosCastlePlayerKill();
		void QuestMUObjectiveBloodCastleClear();
		void QuestMUObjectiveBloodCastleGateDestroy();
		void QuestMUObjectiveDevilSquareClear();
		void QuestMUObjectiveDevilSquarePoints(int32 points);
		void QuestMUObjectiveChaosCastleMonsterKill();

		bool QuestMUObjectiveItemDrop(Monster* pMonster);
		void QuestMUObjectiveItemGet(Item const& item);
		bool CanLootQuestMUItem(Item const& item) const;
		void QuestMURemoveGens();

		void SendQuestMUState(uint16 id);
		void UpdateQuestMULevel();
		bool IsQuestMUComplete(uint16 id) const;
		void UpdateQuestMU();
		void SendQuestMUAutoMove();
		bool IsQuestMURequirements(uint16 id) const;
		
		void QuestMUNpcTalk(uint8 * Packet);
		void QuestMUAccept(uint8 * Packet);
		void QuestMUGiveUp(uint8 * Packet);
		void QuestMUComplete(uint8 * Packet);
		void QuestMUReachTarget(uint8 * Packet);
		void QuestMUInfo(uint8 * Packet);
		void QuestMUMove(uint8 * Packet);
		void QuestMUClickCompleted(uint8 * Packet);
	// Gen
		void GenJoinRequest(uint8 * Packet);
		void GenJoinResult(uint8 result, uint8 family = 0);
		void GenDeleteRequest();
		void GenDeleteResult(uint8 result);
		void GenRewardRequest(uint8 * Packet);
		void GenRewardResult(uint8 result);
		void GenInfoRequest();
		bool IsGenPVP(Unit* mTarget) const;
		bool GenSatisfyGetReward(GenRewardData const* reward);
		void GenAddContribution(int32 value);
		void GenDecContribution(int32 value);
		void GenUpdateLevel();
		void GenKillReward(Player* pVictim, int32 level, uint8 count = 1);
		void GenDeathPenalty(int32 level, uint8 count = 1);
		bool GenApplySanction(Player* pVictim);

		void GenUpdate();
		void GenUpdateVictim();
		void GenSendInfo(bool info, bool viewport);

	// Events
		void EventGetRemainTime(uint8 * Packet);
		void EventResetData();
		
	// Mix
		void ChaosMixButton(uint8 * Packet);
		void ChaosMixClose();
		void ChaosMixCloseResult();
		void ChaosMixSend(uint8 type, bool state = true);
		void ChaosMixResult(uint8 result, uint8 * item_info = nullptr, uint8 slot = 0);
		void ChaosMixPentagramResult(uint8 result, uint8 gremory);
		void ChaosMixSocketResult(uint8 result, uint8 gremory, uint8 slot = -1);
		void ChaosMixMassCombination(uint8 * Packet);
		void ChaosMixMassCombinationResult(uint8 result);

	// Quest Evolution
		bool SatisfyQuestEvolutionClass(QuestEvolutionData const* pData) const;
		void QuestEvolutionSetState(uint8 * Packet);
		uint8 QuestEvolutionUpdateState(uint8 quest_id);
		uint8 QuestEvolutionGetState(uint8 quest_id);
		uint8 QuestEvolutionGetStateByte(uint8 quest_id);
		uint8 QuestEvolutionGetStateByteGroup(uint8 group);
		void QuestEvolutionSetState(uint8 quest_id, uint8 state);
		uint8 QuestEvolutionConditionAccept(QuestEvolution const* pQuest);
		uint8 QuestEvolutionConditionClear(QuestEvolution const* pQuest);
		QuestEvolutionStatus * QuestEvolutionFind(uint8 quest_id);
		void QuestEvolutionNpcTalk(Monster* mNpc);
		void QuestEvolutionSendInfo();
		void QuestEvolutionTake(QuestEvolution const* pQuest);
		void QuestEvolutionGiveReward(QuestEvolution const* pQuest);
		void QuestEvolutionSendPrize(uint8 prize, uint8 count);
		void QuestEvolutionSend(uint8 quest_id);
		void QuestEvolutionSendMonsterKill(uint8 quest_id);
		void QuestEvolutionSendMonsterKill();

		void QuestEvolutionMonsterKill(Monster* pVictim);
		bool QuestEvolutionMonsterDrop(Monster* pVictim);
		
		void QuestEvolutionUpdate();
		int32 QuestEvolutionGetInstance() const;

	// Chat
		void ChatRequest(uint8 * Packet);
		void ChatProcess(ChatType type, const char * msg, const char * whisper_name, bool log = true);
		ChatType ChatGetType(const char * msg) const;
		void WhisperRequest(uint8 * Packet);
		void WhisperOffline();
		bool ChatCanSpeak()
		{ 
			return this->GetTimer(PLAYER_TIMER_CHAT)->Elapsed(300);
		}
		void ChatMute(time_t mute_time) { this->SetMuteTime(mute_time + time(nullptr)); }
		bool IsMuted();

	// Administrator
		bool IsAdministrator() const { return this->GetAuthority() == AUTHORITY_CODE_ADMINISTRATOR; }

		DECLARE(uint32,TalkingGuild);
		DECLARE(uint32,TalkingAlliance);

	// PK
		bool IsMurder() const { return this->GetPKLevel() > PK_STATUS_COMMONER; }
		bool IsHighMurder() const { return this->GetPKLevel() > PK_STATUS_MURDER_LVL_1; }
		bool IsHero() const { return this->GetPKLevel() < PK_STATUS_COMMONER; }

		SelfDefenseDataMap const& GetSelfDefenseData() const { return self_defense_map; }

	// Stat Fruit
		void UsePlusStatFruit(uint8 slot);
		void UseMinusStatFruit(uint8 slot);
		bool UseStatFruitAllowed(uint8 level);
		void UseStatFruitResult(uint8 result, uint8 fruit, uint32 stat = 0);

		bool CanUsePlusStatFruit();
		bool CanUseMinusStatFruit();

		void UseCashShopFruit(uint8 slot);
	// Dark Spirit
		bool DarkSpiritExist(bool run = false);
		void DarkSpiritCalculate();

		void DarkSpiritRun();
		void DarkSpiritModeNormal();
		void DarkSpiritModeRandom();
		void DarkSpiritModeWithMaster();
		void DarkSpiritModeTarget();
		void DarkSpiritRangeAttack(Unit* mTarget);
		void DarkSpiritSendAttack(Unit* mTarget, uint8 type, uint8 action);
		void DarkSpiritAttack(Unit* pTarget, uint8 action);
		int32 DarkSpiritGetDamage(int32 target_defense, uint16 & damage_type);
		void DarkSpiritDamage(int32 damage);

		void DarkSpiritSetTarget(Unit* mTarget);

		void DarkSpiritSetMode(DarkSpiritMode mode, Unit* mTarget);
		void DarkSpiritSetCommand(uint8 * Packet);

	// Cash Shop
		void CashShopSendPath();
		void CashShopSendDate();
		void CashShopSendDate(uint8 type, uint16 id, uint16 year, uint16 month);

		CashShopItemData const* CashShopGetItem(uint16 server, uint32 serial) const;
		void CashShopDeleteItem(uint16 server, uint32 serial);

		void CashShopCreditCoinUseRequest(uint8 level);

		int32 CashShopGetTotalPage(uint8 type) const;
		bool CashShopInsertItem(uint8 type, CashShopItemData * pItem, bool check);

		void CashShopSendExpireItem();
		void CashShopSendExpireItem(uint8 slot);
		
		void CashShopCoinUpdate();
		void CashShopCoinCallBack(PreparedQueryResult result);

		void CashShopGiftSendCallBack(PreparedQueryResult result);
		void CashShopGiftUpdate();
		void CashShopGiftCallBack(PreparedQueryResult result);
		
	// World Quest
		///- QUEST GUIDED
		void QuestGuidedSend();

		PlayerQuestGuided * GetQuestGuided();
		PlayerQuestGuided const* GetQuestGuided() const;

		PlayerQuestGuided * GetQuestGuided(uint16 quest);
		PlayerQuestGuided const* GetQuestGuided(uint16 quest) const;
		void QuestGuidedSetStarting();
		void QuestGuidedMonsterKill(Monster* pMonster);
		void QuestGuidedReachLevel();
		void QuestGuidedItemGet();
		void QuestGuidedQuestComplete();

		void QuestGuidedComplete(uint8 * Packet);
		void QuestGuidedCompleteResult(uint16 quest, uint16 result);
		void QuestGuidedGiveUp(uint8 * Packet);
		void QuestGuidedGiveUpChapter(uint8 * Packet);
		void QuestGuidedWarp(uint8 * Packet);
		void QuestGuidedReachZone(uint8 * Packet);
		void QuestGuidedWalk();

		void QuestGuidedGiveReward();

		///- MAJESTIC SKILL TREE
		void MajesticSkillTreeSend(bool remove = false);
		void MajesticSkillTreeStatSend(bool remove = false);
		void MajesticSkillTreeSendPoints();
		void MajesticSkillTreeAdd(uint8 * Packet);
		void MajesticSkillTreeAddStat(uint8 * Packet);
		void MajesticSkillTreeAddPassive(uint8 * Packet);
		MajesticSkill * GetMajesticSkillTreeSkill(uint16 skill);
		MajesticSkill const* GetMajesticSkillTreeSkill(uint16 skill) const;
		MajesticSkill * GetMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id);
		MajesticSkill const* GetMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id) const;

		bool CheckMajesticReplaceSkill(uint8 section, uint16 skill, uint8 level) const;
		bool CheckMajesticRequiredSkill(uint8 section, uint16 id, uint8 level) const;

		bool AddMajesticSkillTreeSkill(uint8 type, uint8 section, uint16 id, uint8 level);
		bool ReplaceMajesticSkill(uint16 replace, uint16 skill, uint8 level);
		bool CheckMajesticRequireGroup(uint8 section, uint8 rank) const;

		void ApplyMajesticSkillTree(bool stat);
		void ApplyMajesticSkillTree(uint8 section, uint16 skill, float value);
		void ApplyMajesticStats(bool stat);
		void ApplyMajesticStats(uint16 skill, float value);

		void SpawnMajesticDebuffCircle(uint16 skill);
		void RunSpawnMajesticDebuffCircle(uint16 skill, uint8 type, int32 add_duration);

		void ApplyMajesticLinkedSplashDamage(Unit* pTarget, uint16 skill, int32 damage);

		void AddMajesticData(uint8 section, uint8 id, float value);
		float GetMajesticDataValue(uint8 section, uint8 id) const;
		float GetMajesticDataValueBySkill(uint16 skill, uint8 id) const;

		void ApplyMajesticDamage(Monster* pMonster, int32 & damage, bool target);

		bool GetMajesticSkillLevel(uint16 skill, uint8 & level) const;

		void ResetMajesticTree();

		uint16 GetMajesticSectionAddedPoints(uint8 section) const;
		void CalculateMajesticStatPoints();
	// Interface
		static bool InterfaceSharedCheck(Player* pPlayer01, Player* pPlayer02, std::string const& interface_id, uint8 state);
		static bool InterfaceCheck(Player* pPlayer, std::string const& interface_id, uint8 state);
		static bool TransactionSharedSerialCheck(Player* pPlayer01, Player* pPlayer02, std::string const& transaction);
		static bool TransactionSerialCheck(Player* pPlayer, std::string const& transaction);
		static void TransactionDBSave(Player* pPlayer01, Player* pPlayer02);
		static bool TransactionSerialFind(Player* pPlayer, uint16 serial_server, uint32 serial, std::string const& transaction);
	private:
		DECLARE_FLOAT(StaminaReduction);

		DECLARE_ENUM(time_t, MuteTime);
		DECLARE_FLAG(uint32, AdministratorFlag);

		DECLARE_STRUCT(DarkSpirit, DarkSpirit);

		DECLARE_ENUM(int32, BookHitCount);

		SelfDefenseDataMap self_defense_map;

		DECLARE_STRUCT(ObjectLocation, TalismanResurrection);
		DECLARE_STRUCT(ObjectLocation, TalismanMobility);

		DECLARE_BOOL(FirstTime);
		
		DECLARE_ARRAY_STRUCT(RestrictionData, Restriction, PlayerAction::PLAYER_ACTION_MAX);

		DECLARE_PROPERTY_ARRAY(uint32, AleTime, ALE_MAX);
		DECLARE_PROPERTY_ARRAY(int32, AleEffect, ALE_MAX);
		DECLARE_PROPERTY_ARRAY(uint16, AleItem, ALE_MAX);
		DECLARE_PROPERTY_ARRAY(uint8, AleLevel, ALE_MAX);

		DECLARE_ARRAY_STRUCT(StatisticData, StatisticData, STATISTIC_MAX);
		ItemDelayDataMap m_item_delay;

		DECLARE_PROPERTY_ARRAY_ENUM(int32, Data, PLAYER_DATA_MAX, PlayerFieldData);
		DECLARE_ARRAY_ENUM(PlayerFieldFloat, float, DataFloat, PLAYER_FLOAT_MAX);

		DECLARE_ENUM(uint32, Credits);
		CashShopItemDataList m_CashShopItemList;
		CashShopItemDataList m_CashShopGiftList;

		DECLARE_STRING_FIXED(CashShopGiftName, MAX_CHARACTER_LENGTH + 1);
		DECLARE_STRING_FIXED(CashShopGiftMessage, 201);
		DECLARE_ENUM(uint32, CashShopGiftPackage);
		DECLARE_ENUM(uint32, CashShopGiftOption);

		DECLARE_ARRAY_BOOL(EventAnimation, EVENT_STATE_MAX);

		GenVictimDataMap m_gen_victim_map;
		DECLARE_FLAG(uint32, Flag);
		DECLARE_FLAG(uint32, AdminPanelFlag);

	public:
		bool SecureCodeCheck(const char * code) const;

		bool HasRestriction(uint32 id, bool notify = true)
		{
			if ( id >= PlayerAction::PLAYER_ACTION_MAX )
				return false;

			if ( this->GetRestriction(id)->IsActive() && this->GetRestriction(id)->GetTime() > time(nullptr) )
			{
				if ( notify )
				{
					this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You have %s restriction.", PlayerAction::Name[id].c_str());
					this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Remain restriction time %s.", secsToTimeString(this->GetRestriction(id)->GetTime() - time(nullptr)).c_str());
				}
				return true;
			}

			return false;
		}

		void CancelAleEffect(uint8 id)
		{
			this->SetAleTime(id, 0);
		}
		
		/*void SpecialItemUseSend(uint8 type, uint16 time)
		{
			SPECIAL_ITEM_USE pMsg(type,time);
			this->sendPacket(MAKE_PCT(pMsg));
		}*/

		bool IsActionAllowed(uint32 action, Unit* pTarget = nullptr, bool notify = true);
		void UpdateAleTime();

		DECLARE_ENUM(time_t, SantaClausGiftDate);
		DECLARE_ENUM(uint8, SantaClausGiftRequest);
		DECLARE_PROPERTY_ARRAY(float, ItemDurabilityReduction, 4);

		std::string BuildLog() const;
		std::string BuildLogDB() const;
		std::string BuildLogStat() const;
		std::string BuildLogSet() const;
		void BuildLogSet(uint8 slot, std::string & item_data) const;
		std::string BuildLocationLog(coord_type x = -1, coord_type y = -1, bool to_db = false) const;
		
		void ReduceCredits(uint32 ammount);
		void IncreaseCredits(uint32 ammount);

		DECLARE_ENUM(uint32, GUID);
		DECLARE_STRING_FIXED(Name, (MAX_CHARACTER_LENGTH + 1));
		DECLARE_ENUM(uint16, PartyID);
		DECLARE_ENUM(uint8, Slot);
		DECLARE_PROPERTY(uint8, DBClass);
		DECLARE_ENUM(uint8, Class);
		DECLARE_PROPERTY_ARRAY(uint8, ChangeUP, 3); // Quest Evo 1 - Quest Evo 2 ( Master ) - Quest Evo 4
		DECLARE_ENUM(uint8, Authority);
		DECLARE_ENUM(time_t, PostTime);
		DECLARE_STRUCT(GameOption, GameOption);
		DECLARE_STRUCT(InterfaceState, InterfaceState);
		DECLARE_PROPERTY_ARRAY(int32, Stat, MAX_STAT_TYPE);
		DECLARE_PROPERTY_ARRAY(int32, AddStat, MAX_STAT_TYPE);

		DECLARE_PROPERTY(int32, FruitPointsAdd);
		DECLARE_PROPERTY(int32, FruitPointsDec);

		DECLARE_ARRAY_STRUCT(LevelData, LevelData, LEVEL_DATA_MAX);

		DECLARE_BOOL(FullAncient);

		DECLARE_PROPERTY_PTR(InventoryScript, Inventory);
		DECLARE_PROPERTY_PTR(InventoryScript, TransactionInventory);
		DECLARE_PROPERTY_PTR(TradeScript, TradeInventory);
		DECLARE_PROPERTY_PTR(StoreScript, MixInventory);
		DECLARE_PROPERTY_PTR(StoreScript, MixRecoveryInventory);

		DECLARE_PROPERTY_PTR(WarehouseScript, Warehouse);

		DECLARE_PROPERTY(uint8, PKLevel);
		DECLARE_PROPERTY(int32, PKCount);
		DECLARE_PROPERTY(int32, PKPoints);
		
		DECLARE_ENUM(uint16, Skin);

		DECLARE_ARRAY_STRUCT(MultiAttackData, MultiAttack, MULTI_ATTACK_MAX_INFO);
		DECLARE_PROPERTY(int32, MultiAttackCount);

		bool MultiAttackCheckFireScreamSkill(uint16 target, uint8 serial);
		bool MultiAttackCheckPenetrationSkill(uint16 target, uint8 serial);

		DECLARE_STRUCT(DurationMagicKeyData, DurationMagicKey);
		DECLARE_PROPERTY(int32, AttackStandby);
		DECLARE_PROPERTY(int32, CrywolfScore);

		DECLARE_PROPERTY(time_t, GoblinPointCount);

		DECLARE_BOOL(KanturuAllowed);
		DECLARE_PROPERTY_PTR(Monster, CurrentTarget);

		DECLARE_PROPERTY(int32, WrongSerialCount);
		DECLARE_PROPERTY(int32, WrongDecryptCount);

		DECLARE_ENUM(uint32, MSTime);

		DECLARE_ARRAY_STRUCT(CharacterListData, CharacterList, MAX_CHARACTER_PER_ACCOUNT);

		DECLARE_PROPERTY(uint32, GoblinPoints);

		DECLARE_BOOL(CastleSiegeMoneyRemove);

		DECLARE_ENUM(time_t, LastItemUpdate);

		DECLARE_PROPERTY(int32, WarningCount);
		DECLARE_PROPERTY(uint16, RageFighterSkill);
		DECLARE_PROPERTY(uint16, RageFighterSkillTarget);
		DECLARE_PROPERTY(int32, RageFighterSkillCount);
		DECLARE_PROPERTY(int32, RageFighterSkillMaxCount);

		DECLARE_BOOL(InChallenge);

		DECLARE_ENUM(uint16, DestServer);
		DECLARE_ENUM(world_type, DestWorld);
		DECLARE_ENUM(coord_type, DestX);
		DECLARE_ENUM(coord_type, DestY);
		DECLARE_ENUM(uint16, DestPort);
		DECLARE_STRING_FIXED(DestIP, MAX_ACCOUNT_IP_LENGTH + 1);
		DECLARE_PROPERTY_ARRAY(int32, DestAuth, 4);
		DECLARE_BOOL(ServerQuit);
		DECLARE_BOOL(LoginSent);
		DECLARE_BOOL(ServerMove);
		DECLARE_BOOL(ServerMoveFromCharacterSelect);

		DECLARE_ENUM(time_t, KickTime);

		DECLARE_STRING_FIXED(CurrentCharacterSearch, MAX_CHARACTER_LENGTH + 1);

		DECLARE_PROPERTY(uint8, ShopTime);

		DECLARE_PROPERTY_ARRAY(uint32, CloseSocketTime, CLOSE_SOCKET_TIME_COUNT);

		DECLARE_PROPERTY(int32, PostCount);
		DECLARE_ENUM(uint8, PostDay);
		DECLARE_ENUM(uint8, PostMonth);

		DECLARE_PROPERTY_STRING(AntiSpamMessage);

		DECLARE_ENUM(uint32, ErrorNotification);
		DECLARE_STRUCT(TickTimer, ErrorNotificationTime);

		DECLARE_BOOL(ArmorSetBonus);
		PlayerSkillTreeMap skill_tree_map;
		PlayerMajesticSkillTreeList majestic_skill_tree_list;
		PlayerMajesticSkillTreeList majestic_skill_tree_stat_list;
		DECLARE_BOOL(MajesticAddStatPoint);
		DECLARE_PROPERTY(int32, MajesticStatPoints);
		DECLARE_PROPERTY_ARRAY(uint16, MajesticStat, MAX_SKILL_TREE_MAJESTIC_PASSIVE);
		DECLARE_ARRAY_STRUCT(Skill, TransactionSkill, MAX_SKILL);
		DECLARE_ENUM(uint32, TransactionRuud);
		DECLARE_ENUM(uint32, TransactionMoney);
		DECLARE_ENUM(uint32, TransactionCredits);

		DECLARE_BOOL(UseGuildMatching);
		DECLARE_BOOL(UseGuildMatchingJoin);

		DECLARE_PTR(EventInventoryScript, EventInventory);
		DECLARE_PTR(EventInventoryScript, TransactionEventInventory);

		DECLARE_ARRAY_BOOL(MuunItemStatus, MUUN_INVENTORY_WEAR_SIZE);
		DECLARE_PTR(MuunScript, MuunInventory);

		DECLARE_ENUM(uint8, DisplayMuunSlot);

		DECLARE_ARRAY_STRUCT(PentagramJewelInfo, PentagramJewelInfoInventory, MAX_PENTAGRAM_JEWEL_INFO);
		DECLARE_ARRAY_STRUCT(PentagramJewelInfo, PentagramJewelInfoWarehouse, MAX_PENTAGRAM_JEWEL_INFO);

		DECLARE_STRUCT(MuRoomyInfo, MuRoomyInfo);

		DECLARE_STRUCT(PentagramOption, PentagramOption);
		DECLARE_STRUCT(PentagramJewelOption, PentagramJewelOption);

		DECLARE_BOOL(MiniMapState);
		DECLARE_ENUM(world_type, MiniMapWorld);

		DECLARE_ENUM(uint8, PacketResult);
		DECLARE_ENUM(int32, PacketData1);
		DECLARE_ENUM(int32, PacketData2);
		DECLARE_ENUM(int32, PacketData3);

		PlayerChatBlockList chat_block_list;

		DECLARE_ARRAY_STRUCT_PTR(GremoryCaseScript, GremoryCase, GREMORY_CASE_TYPE_MAX);
		DECLARE_PROPERTY_PTR(PersonalStoreScript, PersonalStore);

		//DECLARE_BOOL(Evomon);
		DECLARE_PTR(Monster, EvomonPortal);
		DECLARE_ENUM(TCType, EvomonTime);

		DECLARE_PROPERTY(uint32, RuudMoney);

		DECLARE_PROPERTY_ARRAY(uint8, EventNotification, EVENT_NOTIFICATION_MAX);
		DECLARE_BOOL(AuthorizationEnabled);

		DECLARE_PROPERTY_ARRAY(int32, SpecializationStat, CHARACTER_SPECIALIZATION_MAX);
		DECLARE_PROPERTY_ARRAY(int32, BaseStat, MAX_CHARACTER_STAT_ID);

		DECLARE_PTR(Unit, MuunTarget);

		DECLARE_ENUM(uint32, Timeout);

		DECLARE_ENUM(uint16, StartServer);

		DECLARE_ENUM(uint16, CurrentGate);
		DECLARE_ENUM(uint16, DelayedTeleportGate);

		DECLARE_ENUM(uint8, CoordinateAttribute);

		DECLARE_PTR(NpcSellScript, NpcSell);

		DECLARE_BOOL(MixCompleted);

		DECLARE_PROPERTY(int32, WalkSpeedCount);

		//DECLARE_BOOL(WhiteList);

		SkillCooldownMap skill_cooldown;

		DECLARE_BOOL(CharacterListRequested);

		DECLARE_ENUM(uint8, MiningStatus);

		DECLARE_BOOL(HuntingRecordActive);
		HuntingRecordDataMap hunting_record_data;
		DECLARE_STRUCT(HuntingRecordData, CurrentHuntingData);
		DECLARE_ENUM(world_type, HuntingRecordWorld);
		DECLARE_ENUM(uint16, HuntingRecordTarget);
		DECLARE_BOOL(HuntingRecordVisible);

		DECLARE_PROPERTY_ARRAY(int32, AncientEffect, ANCIENT_EFFECT_MAX);

		DECLARE_ENUM(uint8, DarkLordPetSlot);
		DECLARE_ENUM(uint8, DarkLordPetType);
		DECLARE_ENUM(uint8, DarkLordPetFlag);

		DECLARE_BOOL(GameOptionSaved);

		DECLARE_BOOL(GiftQuery);
		DECLARE_BOOL(MailQuery);

		DECLARE_ENUM(uint16, PartyMatchingID);
		DECLARE_BOOL(PartyMatchingAssist);

		DECLARE_BOOL(Dueling);
		DECLARE_PROPERTY(int32, DuelScore);
		DECLARE_ENUM(uint8, DuelRoom);
		DECLARE_BOOL(DuelSpectatorActive);
		DECLARE_PTR(Player, DuelTarget);
		DECLARE_ENUM(world_type, DuelWorld);
		DECLARE_BOOL(DuelClassic);
		
		DECLARE_ENUM(uint32, GuildMatchingUpdateTime);

		DECLARE_ARRAY_STRUCT(TickTimer, Timer, PLAYER_TIMER_MAX);
		DECLARE_PROPERTY_ARRAY(int32, Counter, PLAYER_COUNTER_MAX);
		DECLARE_PROPERTY_ARRAY(time_t, Date, PLAYER_DATE_MAX);

		DECLARE_STRUCT(PlayerLabyrinthData, LabyrinthData);
		
		PlayerEventEnterCountMap event_enter_count_map;

		DECLARE_ENUM(uint8, MiniBombState);
		DECLARE_PROPERTY(uint16, MiniBombCurrentScore);
		DECLARE_PROPERTY(uint16, MiniBombFinalScore);
		DECLARE_PROPERTY(int32, MiniBombTotalScore);
		DECLARE_ENUM(time_t, MiniBombDate);
		DECLARE_ARRAY_STRUCT(PlayerMineBomb, MiniBombData, MINI_BOMB_SIZE);

		DECLARE_ARRAY_STRUCT(PlayerWarpFavoriteList, WarpFavoriteList, WARP_FAVORITE_COUNT);

		DECLARE_ENUM(uint16, QuestGuidedID);
		DECLARE_BOOL(QuestGuidedMove);

		PlayerQuestGuidedMap quest_guided_map;

		DECLARE_ENUM(uint8, WorldState);

		MajesticDataMap majestic_data_map;

		DECLARE_ENUM(uint8, JewelBingoState);
		DECLARE_ENUM(uint8, JewelBingoBox);
		DECLARE_ENUM(uint8, JewelBingoCount);
		DECLARE_ENUM(uint8, JewelBingoCurrentJewel);
		DECLARE_PROPERTY(int32, JewelBingoScore1);
		DECLARE_PROPERTY(int32, JewelBingoScore2);
		DECLARE_PROPERTY(int32, JewelBingoScore3);
		DECLARE_ENUM(time_t, JewelBingoDate);
		DECLARE_ARRAY_STRUCT(PlayerJewelBingo, JewelBingoGrid, JEWEL_BINGO_GRID_SIZE);
		DECLARE_ARRAY_STRUCT(PlayerJewelBingo, JewelBingoJewel, JEWEL_BINGO_JEWEL_COUNT);
		DECLARE_ARRAY_STRUCT(PlayerJewelBingo, JewelBingoMatching, JEWEL_BINGO_MATCHING_SIZE);

		DECLARE_ENUM(uint8, NumericBaseballState);
		DECLARE_PROPERTY(int32, NumericBaseballScore);
		DECLARE_PROPERTY_ARRAY(uint8, NumericBaseballNumber, NUMERIC_BASEBALL_NUMBERS_PER_ROW);
		DECLARE_PROPERTY_ARRAY(uint8, NumericBaseballUsedNumber, NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW);
		DECLARE_PROPERTY_ARRAY(uint8, NumericBaseballStrike, NUMERIC_BASEBALL_ROWS);
		DECLARE_PROPERTY_ARRAY(uint8, NumericBaseballBall, NUMERIC_BASEBALL_ROWS);
		DECLARE_PROPERTY(uint8, NumericBaseballCurrentNumber);

		///- AUTOLOGIN
		DECLARE_BOOL(AutoLogin);
		DECLARE_ENUM(uint32, AutoLoginAccount);
		DECLARE_ENUM(uint32, AutoLoginCharacter);
		DECLARE_ENUM(uint8, AutoLoginType);
		DECLARE_BOOL(AutoLoginSent);

		void AutoLoginProcess();
		void AutoLoginCharacterSelect();
		void SetAutoLoginInfo(uint8 type);

		///- TRACKING
		DECLARE_ENUM(uint32, TrackingID);

		void TrackingUpdate();

		DECLARE_BOOL(SentMapState);
		void UpdateMapState();

		void CancelMovement();

		DECLARE_ENUM(uint16, CurrentTargetPlayer);
		DECLARE_BOOL(DelayedTeleportHelper);
		DECLARE_ENUM(int64, CreateDate);

		DECLARE_BOOL(ClientMinimized);
		DECLARE_BOOL(SendBuff);

		float GetAbsorbDamageRate() const;
		float GetIncreaseDamageRate() const;
		void ReduceAttackHP(uint16 skill);

		SkillTimeMap m_SkillTimeMap;
		void ApplySkillTime(uint16 skill, coord_type x, coord_type y);
		SkillTime* GetSkillTime(uint16 skill);
		
		DECLARE_ENUM(uint8, CashShopDiscountWC);
		DECLARE_ENUM(uint8, CashShopDiscountGP);
		DECLARE_ENUM(time_t, CashShopDiscountDate);

		bool IsDiscountWC() const;
		bool IsDiscountGP() const;

		void AddRecoveryMixItem(Item const* pItem);
		void RemoveOldRecoveryMixItem();
		void AttemptRecoveryMixItem(uint8 slot);
		void LogRecoveryMixItem(Item const* pItem, uint8 type);
		void UpdateRecoveryMix();
		void SendRecoveryMixItemList(Monster* pMonster, bool renew = false);
		//void SendCancelRecoveryMixItemList();
		//void CancelRecoveryMixItem(uint32 slot);
		//bool IsRecoveryMixFull() const;

		SkillCPSMap m_SkillCPSMap;
		bool IsSkillCPS(uint16 skill, uint32 time);
		SkillCPS* GetSkillCPS(uint16 skill);
		SkillCPS const* GetSkillCPS(uint16 skill) const;
		int32 GetSkillCPSCount(uint16 skill) const;

		AncientSetDataMap m_AncientSetDataMap;

		DECLARE_ENUM(uint8, VehiculeType);

		QuestMUDataMap m_QuestMUDataMap;
		DECLARE_ENUM(uint16, QuestMUAutoMoving);

		MonsterSoulDataMap m_MonsterSoulDataMap;
		MonsterSoulData* GetMonsterSoulData(uint32 id);
		void RemoveMonsterSoulData(uint32 id);

		DECLARE_ENUM(uint16, VisibleMount);

		DECLARE_PTR(Monster, SummonerDebuff);

		uint8 GetMagicTreeLevel(uint16 skill) const;

		void SwitchModeTurnOn(uint8 * Packet);

		void ItemEnhancementRequest(uint8 * Packet);
		void ItemEnhancementLog(Item* pItem, int32 success_rate, int32 price, bool success, uint8 talisman);

		DECLARE_PTR(Monster, MajesticDebuff);
		DECLARE_ENUM(TCType, MajesticDebuffTick);

		DECLARE_BOOL(AccountDataLoaded);

		DECLARE_ENUM(uint64, LastSkillTime);
		DECLARE_ENUM(uint16, LastSkillChecked);

		void PrintItemData(Item const* pItem);
		
		DECLARE_STRUCT(ACPathData, ACPathData);
};

#endif