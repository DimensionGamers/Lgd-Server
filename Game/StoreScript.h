#ifndef PLAYER_STORE_SCRIPT_H
#define PLAYER_STORE_SCRIPT_H

class StoreScript
{
	public:
		explicit StoreScript(Player*, int32 item_count, int32 map_count, ItemBox box = ITEM_BOX_NONE);
		virtual ~StoreScript();

		DECLARE_PROPERTY_PTR(Player, Player);
		DECLARE_ENUM(int32, ItemCount);
		DECLARE_ENUM(int32, MapCount);
		DECLARE_ENUM(ItemBox, Box);
		Item* item;
		uint8* item_map;
		uint8* item_map_tmp;

		virtual void Clear();
		virtual void ClearTmp();
		virtual void StartTmp();
		virtual void EndTmp();
		virtual void Assign(StoreScript*);
		virtual void AssignItem(Item const&, uint8 slot);

		virtual void SaveDB(SQLTransaction & trans, uint8 rules);

		void LoadDBItem(Field* fields, uint32 owner, ItemBox box);

		virtual uint8 RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const;
		virtual void ItemSet(uint8 slot, uint8 set);
		virtual void DeleteItem(uint8 slot);

		Item* GetItem(uint8 slot);
		Item const* GetItem(uint8 slot) const;

		virtual uint8 AddItem(Item const& item, uint8 slot);
		virtual uint8 AddItem(Item const& item);

		bool IsEmptySpace(uint8 x, uint8 y) const;
};

#endif