/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "NpcHandler.h"
*
*/

#ifndef NPC_HANDLER_H
#define NPC_HANDLER_H

class NpcHandler;
typedef void (NpcHandler:: *NpcTalkFunction)(); 

struct NpcTalkData
{
	const char * name;
	NpcTalkFunction function;
};

class NpcHandler
{
	private:
		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE_PROPERTY_PTR(Monster, Npc);

	public:
		NpcHandler(Player * pPlayer, Monster * pNpc)
		{
			this->SetPlayer(pPlayer);
			this->SetNpc(pNpc);
		}

		bool ExecuteCommandInTable(NpcTalkData * table, const char * text);
		void OnTalk();
		void SendShopList(Shop const* shop);

		void Warehouse();
		void Guild();
		void QuestEvolutionNpc();
		void BloodCastleEntrance();
		void BloodCastleAngel();
		void DevilSquareEntrance();
		void DoubleGoerEntrance();
		void KanturuGateway();

		void LordMix();

		void Snowman();
		void SantaClaus();

		void LandOfTrialsGuard();

		void IllusionTempleEntrance();

		void NpcTalkTormentedSquare();

		void NpcTalkImperialFortress();
		void NpcTalkCastleSiegeGuard();
		void NpcTalkArcaBattleLesnar();
		void NpcTalkSloveCaptain();
		void NpcTalkPersonalShopBoard();
		void NpcTalkAdniel();
		void NpcTalkMuunExchange();
		void NpcTalkQuestSupport();
		void NpcTalkJewelMix();
		void NpcTalkDuel();
		void NpcTalkLorenMarketTraveler();
		void NpcTalkPetTrainer();
		void NpcTalkChaosCard();
		void NpcTalkCherryBlossom();
		void NpcTalkSeedMaster();
		void NpcTalkSeedResearch();
		void NpcTalkChaosMachine();
		void NpcTalkLucky();
		void NpcTalkHarmonyGemstone();
		void NpcTalkHarmonySmeltItem();
		void NpcTalkHarmonyRestoreItem();
		void NpcTalkBuffSantaHealing();
		void NpcTalkBuffSantaProtection();
		void NpcTalkBuffSantaStrengthener();
		void NpcTalkBuffSantaDefense();
		void NpcTalkBuffSantaQuickness();
		void NpcTalkBuffSantaFortune();
		void NpcTalkSantaRestoreLife();
		void NpcTalkSantaRestoreMana();
		
		void NpcTalkZen();

		void NpcTalkLastManStanding();

		void NpcTalkCentTown();
		void NpcTalkCentEntrance();

		void NpcTalkMossMerchant();

		void NpcTalkItemRecovery();

		void SendTalkToNpc(uint8 result, std::string const& interface_id = InterfaceData::None, bool tax = false, uint8 data1 = 0, uint8 data2 = 0);
};

#endif