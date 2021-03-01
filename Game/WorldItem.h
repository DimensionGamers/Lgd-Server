#ifndef WORLD_ITEM_H
#define WORLD_ITEM_H

class WorldItem: public Object
{
	non_copyable(WorldItem);

	public:
		explicit WorldItem(uint16 entry);
		virtual ~WorldItem();

		void Clear();
		bool EventFinished() const;
		bool Update(uint32 diff);
		void MakeVisible();

		const char * GetName() const { return item.GetName(); }

		Item item;
		DECLARE_ENUM(WorldItemState, State);
		DECLARE_PROPERTY_PTR(Unit, Owner);

		DECLARE_STRUCT(TickTimer, LiveTime);
		DECLARE_STRUCT(TickTimer, LootingTime);
		
		DECLARE_BOOL(OnlyOwner);
		DECLARE_BOOL(Visible);

		bool GiveCheck(Player * pPlayer);
		void Give();
		
		void SendCustomObjectData( ) { }
		void UpdateStatus() { }
		void Respawn() { }
		bool IsViewportControl() const { return false; }
};

struct WorldItemQueued
{
	Item item;
	DECLARE_ENUM(coord_type, X);
	DECLARE_ENUM(coord_type, Y);
	DECLARE_PTR(Unit, Owner);
	DECLARE_BOOL(OnlyOwner);
	DECLARE_BOOL(Visible);
};

typedef std::list<WorldItemQueued*> WorldItemQueuedList;

#endif