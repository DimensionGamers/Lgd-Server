#ifndef EVENT_INVENTORY_SCRIPT_H
#define EVENT_INVENTORY_SCRIPT_H

class EventInventoryScript: public StoreScript
{
	public:
		explicit EventInventoryScript(Player* pPlayer): StoreScript(pPlayer, EVENT_INVENTORY_SIZE, EVENT_INVENTORY_SIZE) { }

		void AssignItem(Item const&, uint8 slot) override;

		bool IsEmptySpace(uint8 W, uint8 H);
		bool IsEmptySpace(uint16 item);
};

#endif