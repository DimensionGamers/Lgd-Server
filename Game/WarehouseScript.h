#ifndef WAREHOUSE_SCRIPT_H
#define WAREHOUSE_SCRIPT_H

class WarehouseScript: public StoreScript
{
	public:
		explicit WarehouseScript(Player* pPlayer): StoreScript(pPlayer, warehouse_size, warehouse_size) {}

		DECLARE_PROPERTY(uint32, Zen);
		DECLARE_ENUM(uint16, Password);
		DECLARE_BOOL(Locked);
		DECLARE_PROPERTY(uint8, Expanded);
		DECLARE_PROPERTY(time_t, ExpandedTime);

		void LoadDBData(PreparedQueryResult result);
		void LoadDBItemList(PreparedQueryResult result);
		void Open();

		void SaveDBData(SQLTransaction & trans);

		void Clear();

		uint8 RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const;
		void ItemSet(uint8 slot, uint8 set);

		void DeleteItem(uint8 slot);

		bool MoneyReachMaximum(uint32 ammount) const;
		void MoneyAdd(uint32 ammount);
		void MoneyReduce(uint32 ammount);
		bool MoneyHave(uint32 ammount);

		void AssignItem(Item const& add_item, uint8 slot);

		uint8 AddItem(Item const& item, uint8 slot) override;
};

#endif