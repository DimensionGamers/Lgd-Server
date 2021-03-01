#ifndef INVENTORY_SCRIPT_H
#define INVENTORY_SCRIPT_H

class InventoryScript: public StoreScript
{
	public:
		explicit InventoryScript(Player* pPlayer): StoreScript(pPlayer, inventory_size, inventory_map_size) {}

		void Clear();

		bool CanUseExpandedSlot(uint8 slot) const;
		uint8 RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const;

		bool IsEmptySpace(uint8 W, uint8 H);
		bool IsEmptySpace(uint16 item);
		void ItemSet(uint8 slot, uint8 set);
		void DeleteItem(uint8 slot);
		int32 GetItemCount(uint16 item, uint8 level, bool weared = false, bool weared_only = false);
		Item* IsWearingRing(uint16 item, uint8 level);
		bool IsExpirableItem(uint8 slot, uint16 item, uint8 level) const;

		void AssignItem(Item const&, uint8 slot) override;

		uint8 AddItem(Item const& item, uint8 slot) override;
		uint8 AddItem(Item const& item) override;

		int32 GetInventoryMaxValue() const;

		DECLARE_PROPERTY(uint8, Expanded);

		/* TEMPORARY ITEM INSERT */
		DECLARE_BOOL(Temporary);

		void TemporaryStart();
		void TemporaryEnd();

		bool TemporaryItemAdd(Item const& item);
};

#endif