/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MixHandler.h"
*
*/

#ifndef MIX_HANDLER_H
#define MIX_HANDLER_H

struct ChaosMixHandler
{
	uint16 id;
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 data);
	bool mass_combination;
	int32 count;
};

struct PentagramMixHandler
{
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 * slot, uint8 data);
};

struct PentagramJewelMixHandler
{
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 * slot, uint8 * data);
};

#define MAX_SOCKET_MIX				8

struct SocketMixHandler
{
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 * slot, uint8 data, int32 slot_count);
};

#define MAX_WING_SOCKET_MIX				2

struct WingSocketMixHandler
{
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 * slot, int32 slot_count);
};

#define MAX_EARRING_MIX				1

struct EarringMixHandler
{
	const char* name;
	std::string type;
	void (MixHandler::*handler)(uint8 * slot, int32 slot_count);
};

extern ChaosMixHandler ChaosMixTable[];
extern PentagramMixHandler PentagramMixTable[CHAOS_MIX_PENTAGRAM_MAX];
extern PentagramJewelMixHandler PentagramUpgradeMixTable[CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_MAX];
extern SocketMixHandler SocketMixTable[MAX_SOCKET_MIX];
extern WingSocketMixHandler WingSocketMixTable[MAX_WING_SOCKET_MIX];
extern EarringMixHandler EarringMixTable[MAX_EARRING_MIX];

enum ChaosMixResult
{
	CHAOS_MIX_RESULT_FAIL = 0,
	CHAOS_MIX_RESULT_SUCCESS = 1,
	CHAOS_MIX_RESULT_NOT_ENOUGH_ZEN = 2,
	CHAOS_MIX_RESULT_TOO_MANY_ITEMS = 3,
	CHAOS_MIX_RESULT_LOW_LEVEL_USER = 4,
	CHAOS_MIX_RESULT_UNKNOWN_1 = 5,
	CHAOS_MIX_RESULT_LACKING_MIX_ITEMS = 6,
	CHAOS_MIX_RESULT_INCORRECT_MIX_ITEMS = 7,
	CHAOS_MIX_RESULT_INVALID_ITEM_LEVEL = 8,
	CHAOS_MIX_RESULT_USER_CLASS_LOW_LEVEL = 9,
	CHAOS_MIX_RESULT_INCORRECT_BC_ITEMS = 10,
	CHAOS_MIX_RESULT_NOT_ENOUGH_ZEN_BC = 11,
	CHAOS_MIX_RESULT_MASS_COMBINATION = 16,
	CHAOS_MIX_RESULT_UNKNOWN_2 = 32,
	CHAOS_MIX_RESULT_RESURRECTION_SUCCESS = 100,
	CHAOS_MIX_RESULT_TALISMAN_OF_LUCK = 240,
};

enum SpiritStoneFlags
{
	SPIRIT_STONE_FLAG_LESSER		= 1 << 0,
	SPIRIT_STONE_FLAG_MEDIUM		= 1 << 1,
	SPIRIT_STONE_FLAG_GREATER		= 1 << 2,
};

enum
{
	SPIRIT_STONE_LESSER = 0,
	SPIRIT_STONE_MEDIUM,
	SPIRIT_STONE_GREATER,
	MAX_SPIRIT_STONE,
};

#define MAX_SPELL_STONE_COUNT			15

#define MAX_JEWEL_ITEM_COUNT			5
#define MAX_GUARDIAN_MIX_MATERIAL		5

class MixHandler
{
	public:
		static bool DuplicatedMixCheck();
		static ChaosMixHandler* GetMixHandler(uint16 mix_id);

		MixHandler(Player* pPlayer, uint8 type);
		bool tributeTaxPrice(bool tax = true, uint8 mix_kind = 0);

		void ExecuteMix(uint8 * packet);
		void ExecuteMassCombination(uint8 * packet);
		void ChaosMixNone(uint8 data);
		void ChaosMixWeapon(uint8 data);
		void ChaosMixDevilSquare(uint8 data);
		void ChaosMixUpgrade10(uint8 data);
		void ChaosMixUpgrade11(uint8 data);
		void ChaosMixDinorant(uint8 data);
		void ChaosMixStatFruit(uint8 data);
		void ChaosMixWingSecond(uint8 data);
		void ChaosMixBloodCastle(uint8 data);
		void ChaosMixWingFirst(uint8 data);
		void ChaosMixDarkHorse(uint8 data);
		void ChaosMixDarkSpirit(uint8 data);
		void ChaosMixBlessPotion(uint8 data);
		void ChaosMixSoulPotion(uint8 data);
		void ChaosMixLifeStone(uint8 data);
		void ChaosMixSiegeLord(uint8 data);
		void ChaosMixUpgrade12(uint8 data);
		void ChaosMixUpgrade13(uint8 data);
		void ChaosMixCloak(uint8 data);
		void ChaosMixFenrirFragment(uint8 data);
		void ChaosMixFenrirBrokenHorn(uint8 data);
		void ChaosMixFenrir(uint8 data);
		void ChaosMixFenrirUpgrade(uint8 data);
		void ChaosMixSDLvl1(uint8 data);
		void ChaosMixSDLvl2(uint8 data);
		void ChaosMixSDLvl3(uint8 data);
		void ChaosMixSD(uint32 potion_count, uint32 price, int32 rate, uint16 item);
		void ChaosMixHarmonyRefine(uint8 data);
		void ChaosMixHarmonyRefineItem(uint8 data);
		void ChaosMixHarmonyRestoreItem(uint8 data);
		void ChaosMix380(uint8 data);
		void ChaosMixIllusionTemple(uint8 data);
		void ChaosMixCondorFeather(uint8 data);
		void ChaosMixWingThird(uint8 data);
		void ChaosMixChaosCard(uint8 data);
		void ChaosMixCherryBlossom(uint8 data);

		void ChaosMixSecromicon(uint8 data);
		void ChaosMixSealedBox(uint8 data);
		void ChaosMixUpgrade14(uint8 data);
		void ChaosMixUpgrade15(uint8 data);
		void ChaosMixLuckyTicket(uint8 data);
		void ChaosMixExtensionJewel(uint8 data);
		
		void ChaosMixUpgradeLevel(uint8 data, uint8 level);

		void ChaosMixArkaWarSign(uint8 data);
		void ChaosMixArkaWarBooty(uint8 data);
		void ChaosMixSummonScroll(uint8 data);
		void ChaosMixMonsterWing(uint8 data);
		void ChaosMixConquerorWings(uint8 data);
		void ChaosMixDarkTransformationRing(uint8 data);

		void ChaosMixAncientHeroSoul(uint8 data);
		void ChaosMixSealedMasteryFinal(uint8 data);

		void ChaosMixBloodangelToDarkangelSet(uint8 data);

		void ChaosMixArchangelsHammer(uint8 data);
		void ChaosMixUpgradeDivineArchangelWeapon(uint8 data);

		void ChaosMixUpgradeDarkangelToHolyangelSet(uint8 data);
		void ChaosMixUpgradeHolyangelToAwakeningSet(uint8 data);
		void ChaosMixUpgradeAwakeningToBlueEyeSet(uint8 data);
		void ChaosMixUpgradeBlueEyeToSilverHeartSet(uint8 data);

		void ChaosMixUpgradeBloodangelToDarkangelWeapon(uint8 data);
		void ChaosMixUpgradeDarkangelToHolyangelWeapon(uint8 data);
		void ChaosMixUpgradeHolyangelToSoulWeapon(uint8 data);
		void ChaosMixUpgradeSoulToBlueEyeWeapon(uint8 data);

		void ChaosMixSealedBloodangelHero(uint8 data);
		void ChaosMixElfConversionAttackToSupport(uint8 data);
		void ChaosMixElfConversionSupportToAttack(uint8 data);
		void ChaosMixMGConversionAttackToMagic(uint8 data);
		void ChaosMixMGConversionMagicToAttack(uint8 data);
		void ChaosMixKnightConversionEnergyToStrength(uint8 data);
		void ChaosMixKnightConversionStrengthToEnergy(uint8 data);

		void ChaosMixCreateSpellStone(uint8 data);
		void ChaosMixBindWingsCreateLvl2(uint8 data);
		void ChaosMixBindWingsCreateLvl3(uint8 data);
		void ChaosMixBindWingsCreateLvl4(uint8 data);
		void ChaosMixGarudaFeather(uint8 data);
		void ChaosMixFourthWings(uint8 data);
		void ChaosMixHandcraftStones(uint8 data);

		void ChaosMixUpgradeEarringRageToAncestors(uint8 data);
		void ChaosMixUpgradeEarringAncestorsToHonor(uint8 data);

		void ChaosMixSealArmorPiece(uint8 data);
		void ChaosMixScratchedSealedBloodangel(uint8 data);

		void PentagramMix(uint8 id, uint8 * slot, uint8 data);
		void PentagramMixErrtel(uint8 id, uint8 * data, uint8 * slot);
		void PentagramMixNone(uint8 * slot, uint8 data);
		void PentagramJewelMix(uint8 * slot, uint8 data);
		void PentagramSlotOfRadiance(uint8 * slot, uint8 data);
		void ErrtelElementChange(uint8 * slot, uint8 data);

		void PentagramJewelMixNone(uint8 * slot, uint8 * data);
		void PentagramJewelUpgradeLevelMix(uint8 * slot, uint8 * data);
		void PentagramJewelUpgradeRankMix(uint8 * slot, uint8 * data);

		void SocketMix(uint8 id, uint8 * slot, uint8 data, uint8 count);
		void SocketMixResult(uint8 result);

		void SocketMixNone(uint8 * slot, uint8 data, int32 slot_count);

		void SocketMixSetUpgrade(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSeedCreate(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSeedAssemble(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSeedUpgrade(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSeedInsert(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSeedRemove(uint8 * slot, uint8 data, int32 slot_count);
		void SocketMixSlotReduction(uint8 * slot, uint8 data, int32 slot_count);

		void SocketMixSphereStrengthenerRuneFind(int32 & count, uint8 * slot, uint8 slot_count);
		void SocketMixSphereStrengthenerRuneConsume(int32 & count, uint8 * slot, uint8 slot_count);

		bool HaveJewels();
		void NormalizeJewels();
		bool IsJewel(uint8 slot);
		void JewelFind();
		void JewelConsume(uint8 * slot, uint8 slot_count);

		void WingSocketMix(uint8 id, uint8 * slot, uint8 count);
		void WingSocketMixAdd(uint8 * slot, int32 slot_count);
		void WingSocketMixUpgrade(uint8 * slot, int32 slot_count);

		void EarringMix(uint8 id, uint8 * slot, uint8 count);
		void EarringUpgrade(uint8 * slot, int32 slot_count);
		
		void PetTrainerMix(uint8 type, uint8 id, uint8 main_slot, uint8 * slot, uint8 count);
		void PetTrainerGuardianResurrection(uint8 type, uint8 id, uint8 main_slot, uint8 * slot, int32 slot_count);

		void ChaosMixLogItem(Item * item, uint8 slot);
		void ChaosMixLogFinalItem(Item * item, uint8 slot);
		void ChaosMixLogError(const char * log, ...);
		void ChaosMixLogInvalidMixItems(bool pentagram = false);
		void ChaosMixLogLackingMixItems(bool pentagram = false);
		void ChaosMixLogTooManyMixItems(bool pentagram = false);
		void ChaosMixLogInvalidMixItemLevel(bool pentagram = false);
		void ChaosMixLogNotEnoughMixZen();
		void ChaosMixLogTooManyTalismanOfLuck(bool pentagram = false);
		void ChaosMixLogInvalidMix(uint8 result);
		void ChaosMixLogSuccess();
		void ChaosMixLogFail();
		void ChaosMixDebugMessage();

		void ChaosMixDBSave();

		void ChaosMixItemDown();
		void ChaosMixItemDown(uint8 slot);
		void ChaosMixCondorFeatherItemDown();
		void ChaosMixGarudaFeatherItemDown();
		void ChaosMix380ItemDelete();
		void ChaosMixArchangelItemDown();
		void ChaosMixEarringItemDown();

		void ChaosMixGetUpgradeItemSuccessRate(uint8 slot);

		void ChaosMixFailInit(uint8 result, bool commit = true, uint8 type = 0, uint8 mix_kind = 0);

		bool ChaosMixTalismanOfLuck(bool elemental = false);

		void GenerateMassCombinationResult(uint8 count, int32 success_rate, int32 max_success_rate);
		void CalculateSpiritStone();
		int32 GetSpiritStoneRate(uint32 flags) const;
	private:
		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE(int32, success_rate);
		DECLARE_ENUM(int32, MaxSuccessRate);
		uint32 mix_price;
		uint32 tax_price;
		DECLARE_PROPERTY_STRING(MixName);
		DECLARE_ENUM(uint16, MixID);
		DECLARE_BOOL(Success);
		DECLARE_PROPERTY_ARRAY(int64, Price, ITEM_PRICE_TYPE_MAX);
		DECLARE_PROPERTY(int32, LuckyCharm);
		DECLARE_ENUM(int32, Count);
		DECLARE_ENUM(int32, MaxCount);
		DECLARE_BOOL(Completed);
		DECLARE_ENUM(uint8, Type);
		DECLARE_PROPERTY_ARRAY(uint8, MassCombinationList, chaos_box_size);
		DECLARE_FLAG(uint32, DBFlag);

		DECLARE_PROPERTY_STRING(ItemDataBefore);
		DECLARE_PROPERTY_STRING(ItemDataAfter);

		DECLARE_PROPERTY_ARRAY(int32, SpiritStone, MAX_SPIRIT_STONE);

		DECLARE_PROPERTY_ARRAY(uint16, JewelItem, MAX_JEWEL_ITEM_COUNT);
		DECLARE_PROPERTY_ARRAY(int32, JewelCount, MAX_JEWEL_ITEM_COUNT);
		DECLARE_PROPERTY_ARRAY(int32, MaxJewelCount, MAX_JEWEL_ITEM_COUNT);

		bool roll_success();
};

#endif