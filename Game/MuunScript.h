#ifndef MUUN_SCRIPT_H
#define MUUN_SCRIPT_H

class MuunScript
{
	public:
		explicit MuunScript(Player* pPlayer);
		~MuunScript();

		DECLARE_PTR(Player, Player);
		DECLARE_ARRAY_STRUCT(Item, Item, MUUN_INVENTORY_SIZE);

		void Clear();
		void AssignItem(Item const&, uint8 slot);
		void SaveDB(SQLTransaction & trans, uint8 rules);

		void LoadDBItem(Field* fields, uint32 owner, ItemBox box);

		void DeleteItem(uint8 slot);
		uint8 AddItem(Item const& item, uint8 slot);
		uint8 AddItem(Item const& item);

		uint8 GetEmptySlot();
};

#endif