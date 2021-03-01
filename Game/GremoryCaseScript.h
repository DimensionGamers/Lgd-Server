#ifndef GREMORY_CASE_SCRIPT_H
#define GREMORY_CASE_SCRIPT_H

class GremoryCaseScript
{
	public:
		explicit GremoryCaseScript(Player* pPlayer, ItemBox box, int32 size);
		~GremoryCaseScript();

		DECLARE_PTR(Player, Player);
		DECLARE_ENUM(ItemBox, Box);
		DECLARE_STRUCT_PTR(Item, Item);

		void Clear();
		void AssignItem(Item const&, uint8 slot);
		void SaveDB(SQLTransaction & trans, uint8 rules);

		void LoadDBItem(Field* fields, uint32 owner, ItemBox box);

		void DeleteItem(uint8 slot);
		uint8 AddItem(Item const& item, uint8 slot);
		uint8 AddItem(Item const& item);

		bool IsSlotInRange(uint8 slot) const;
		uint8 GetEmptySlot() const;
		uint32 GetTotalFreeSlots() const;
};

#endif