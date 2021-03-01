#ifndef NPC_SELL_SCRIPT_H
#define NPC_SELL_SCRIPT_H

class NpcSellScript
{
	public:
		explicit NpcSellScript(Player* pPlayer);
		~NpcSellScript();

		DECLARE_PTR(Player, Player);
		DECLARE_ARRAY_STRUCT(Item, Item, MAX_ITEM_SOLD);
		DECLARE_PROPERTY(int32, Count);

		void Clear();
		void AssignItem(Item const&, uint8 slot, uint32 price);
		void SaveDB(SQLTransaction & trans, uint8 rules);

		void LoadDBItem(Field* fields, uint32 owner, ItemBox box);

		void DeleteItem(uint8 slot);
		uint8 AddItem(Item const& item, uint32 price);

		uint8 GetEmptySlot();
};

#endif