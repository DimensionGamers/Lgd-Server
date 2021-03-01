#ifndef PERSONAL_STORE_SCRIPT_H
#define PERSONAL_STORE_SCRIPT_H

typedef std::map<uint32, Item*> PersonalStoreItemMap;

class PersonalStoreScript
{
	public:
		explicit PersonalStoreScript(Player* pPlayer, ItemBox box, int32 size);
		~PersonalStoreScript();

		DECLARE_PTR(Player, Player);
		DECLARE_ENUM(ItemBox, Box);
		DECLARE_STRUCT_PTR(Item, Item);

		DECLARE_BOOL(Open);
		DECLARE_STRING_FIXED(Name, PERSONAL_STORE_NAME_LENGTH + 1);
		DECLARE_BOOL(Off);
		DECLARE_ENUM(TCType, OffTime);
		DECLARE_BOOL(Ending);

		DECLARE_PTR(Player, ViewStore);
		DECLARE_ENUM(uint32, ViewSlot);
		DECLARE_ENUM(uint8, ViewType);

		void Reset()
		{
			this->SetOpen(false);
			this->ResetName();
			this->SetOff(false);
			this->SetOffTime(0);
			this->SetEnding(false);
		}

		void Clear()
		{
			this->SetOpen(false);
			this->SetOff(false);
			this->SetOffTime(0);
			this->SetEnding(false);
		}

		void End()
		{
			this->SetOpen(false);
			this->SetEnding(true);
		}

		bool IsBusy()
		{
			return this->IsOpen();
		}

		void ResetView()
		{
			this->SetViewStore(nullptr);
			this->SetViewSlot(-1);
			this->SetViewType(0);
		}

		void Init();
		void AssignItem(Item const&, uint32 slot);
		void SaveDB(SQLTransaction & trans, uint8 rules);

		void LoadDBItem(Field* fields, uint32 owner, ItemBox box);

		void DeleteItem(uint32 slot);
		uint8 AddItem(Item const& item, uint8 slot);

		bool IsSlotInRange(uint32 slot) const;
		bool IsSellRange(uint32 slot) const;

		bool IsBundle(uint32 slot);
		void GetItemList(uint32 slot, PersonalStoreItemMap & map);

		bool IsEmpty() const;
};

#endif