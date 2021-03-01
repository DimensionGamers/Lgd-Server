class MossMerchantScript: public MonsterScriptAI
{
public:
	explicit MossMerchantScript(): ScriptAI("ai_moss_merchant") { }
	virtual ~MossMerchantScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new MossMerchantAI(monster); }

	struct MossMerchantAI: public MonsterAI
	{
		explicit MossMerchantAI(Monster* monster): MonsterAI(monster) { }
		virtual ~MossMerchantAI() {}

		bool OnTalk(Player*)
		{
			if ( !sGameServer->IsMossMerchantEnabled() )
			{
				return true;
			}

			return false;
		}

		/*bool OnItemBuy(Player* pPlayer, uint8 slot)
		{
			if ( !sGameServer->IsMossMerchantEnabled() )
			{
				return true;
			}

			Shop const* shop = sShopMgr->GetShop(me()->GetNpcFunction());

			if ( !shop )
			{
				return true;
			}

			item_template const* item_info = sItemMgr->GetItem(shop->GetItem(slot)->GetItem());

			if ( !item_info )
			{
				return true;
			}

			uint8 moss_type = MOSS_ITEM_TYPE_MAX;

			switch ( item_info->GetItem() )
			{
			case ITEMGET(13, 71):
			case ITEMGET(13, 72):
			case ITEMGET(13, 73):
			case ITEMGET(13, 74):
			case ITEMGET(13, 75):
				{
					moss_type = item_info->GetItem() - ITEMGET(13, 71);
				} break;
			}

			if ( moss_type == MOSS_ITEM_TYPE_MAX )
				return true;

			if ( !pPlayer->GetInventory()->IsEmptySpace(2, 4) )
			{
				me()->SayTo(pPlayer, "Your inventory is full. Need 2 x 4 empty slots.");
				return true;
			}

			if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MISC, pPlayer, sGameServer->MossMerchantItemBag[moss_type]) == ITEM_BAG_RESULT_SUCCESS )
			{
				uint32 tax_rate = 0;

				if ( shop->IsFlag(SHOP_FLAG_SIEGE_TAX) )
				{
					tax_rate = shop->GetItem(slot)->GetBuyPrice() * sCastleSiege->GetTaxRateStore(pPlayer) / 100;
				}

				uint32 total_zen = tax_rate + shop->GetItem(slot)->GetBuyPrice();

				pPlayer->MoneyReduce(total_zen, true);
			}

			return true;
		}*/
	};
};

void AddSC_MossMerchant()
{
	sScriptAI->AddScriptAI(new MossMerchantScript());
}