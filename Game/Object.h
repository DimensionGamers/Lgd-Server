/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Object.h"
*
*/
#ifndef OBJECT_H
#define OBJECT_H

#define ObjectCastTo(type, name) \
	type * To##name() { return this->Is##name() ? reinterpret_cast<type*>(this) : nullptr; } \
	type const* To##name() const { return this->Is##name() ? (type const*)((type*)this) : nullptr; }

class ObjectLocation
{
	public:
		ObjectLocation()
		{
			this->Reset();
		}

		ObjectLocation(world_type world, int32 instance, coord_type x, coord_type y);

		void Reset()
		{
			this->SetWorldId(-1);
			this->SetX(0);
			this->SetY(0);
			this->SetDirection(0);
			this->SetInstance(-1);
		}

		void Set(ObjectLocation const* data)
		{
			this->SetWorldId(data->GetWorldId());
			this->SetX(data->GetX());
			this->SetY(data->GetY());
			this->SetDirection(data->GetDirection());
			this->SetInstance(data->GetInstance());
		}

		world_type GetDisplayWorld() const;

		bool IsWorldFlag(uint32 flag) const;
		
	public:
		DECLARE_PROPERTY(world_type, WorldId);
		DECLARE_PROPERTY(coord_type, X);
		DECLARE_PROPERTY(coord_type, Y);
		DECLARE_PROPERTY(uint8, Direction);
		DECLARE_PROPERTY(int32, Instance);
};

struct ViewportData
{
	explicit ViewportData()
	{
		this->Reset();
	}

	void Set(Object* pObject, uint8 status)
	{
		this->SetObjectData(pObject);
		this->SetStatus(status);
	}

	void Reset()
	{
		this->Set(nullptr, Viewport::STATUS_NONE);
	}

	DECLARE_ENUM(uint8, Status);
	DECLARE_PTR(Object, ObjectData);
};

typedef std::unordered_map<uint32, ViewportData*> ViewportDataList;

typedef std::map<uint32, uint8> ViewportRemovedMap;

class Object
{
	non_copyable(Object);

	public:
		explicit Object(uint16 entry);
		virtual ~Object();

		virtual void Init();

		virtual bool IsPlaying() const { return this->GetConnectStatus() == CONNECT_STATUS_PLAYING; }
		virtual const char * GetName() const = 0;

		bool IsPlayer() const { return this->GetType() == OBJECT_TYPE_PLAYER; }
		bool IsCreature() const { return this->GetType() == OBJECT_TYPE_MONSTER; }
		bool IsWorldItem() const { return this->GetType() == OBJECT_TYPE_WORLD_ITEM; }
		bool IsUnit() const { return this->IsPlayer() || this->IsCreature(); }
		
		ObjectCastTo(Player, Player);
		ObjectCastTo(Monster, Creature);
		ObjectCastTo(WorldItem, WorldItem);
		ObjectCastTo(Unit, Unit);
		
		void SetWorldId(world_type world) { this->GetLocation()->SetWorldId(world); }
		void SetX(coord_type x) { this->GetLocation()->SetX(x); }
		void SetY(coord_type y) { this->GetLocation()->SetY(y); }
		void SetDirection(uint8 direction) { this->GetLocation()->SetDirection(direction); }
		void SetInstance(int32 instance) { this->GetLocation()->SetInstance(instance); }
		
		world_type GetWorldId() const { return this->GetLocation()->GetWorldId(); }
		World * GetWorld() const;
		const char* GetWorldName() const;
		coord_type GetX() const { return this->GetLocation()->GetX(); }
		coord_type GetY() const { return this->GetLocation()->GetY(); }
		uint8 GetDirection() const { return this->GetLocation()->GetDirection(); }
		int32 GetInstance() const { return this->GetLocation()->GetInstance(); }
		bool IsWorldFlag(uint32 flag) const { return this->GetLocation()->IsWorldFlag(flag); }

		world_type GetDisplayWorld() const { return this->GetLocation()->GetDisplayWorld(); }

		DECLARE_STRUCT(ObjectLocation, Location);
		DECLARE_STRUCT(ObjectLocation, LastLocation);

		bool SameDimension(Object const* pObject) const;
		bool SameDimension(world_type world, int32 instance) const;

		WorldGrid GetGrid() const;
		bool IsInSafeZone() const;
		void UpdateWorld();
		void CreateFrustrum();

		int32 Distance(coord_type x, coord_type y);
		bool InRange(coord_type x, coord_type y, int32 distance);

		virtual void sendPacket(uint8 * packet, uint16 len) { }
		virtual void sendPacket(uint8 * packet) { }
		virtual void SendPacket(C1_HEADER * packet) { }
		virtual void SendPacket(C2_HEADER * packet) { }
		virtual void SendPacket(GamePacket * packet) { }

		void sendPacket_viewport(uint8 * packet, uint16 len);
		void SendPacketViewportChat(uint8 * packet, uint16 len);
		void SendPacketViewport(C1_HEADER * packet);
		void SendPacketViewport(C2_HEADER * packet);
		void SendPacketViewport(uint8 * packet);

		bool CheckFrustrumViewport(Object * pObject);

		void ViewportUpdate(uint32 diff);
		void ViewportListRemove();
		bool ViewportListRemoveConditions(Object* pRemove);
		void ViewportListAdd();
		bool ViewportListAddConditions(Object* pAdd);

		bool ViewportExist(Object* pUnit);
		bool ViewportAttack(Object* pTarget);
		bool ViewportAdd(Object* pObject);
		void ViewportRemove(Object* pObject);
		void ViewportCreate(uint32 flags);
		void ViewportSendObject(Object* pObject, bool clear);
		void ViewportDeleteSend();
		int32 GetViewportCount();
		void ViewportClear();
		void ViewportClose();

		bool IsViewportRemoved(uint32 id) const
		{
			ViewportRemovedMap::const_iterator itr = this->viewport_removed_map.find(id);

			if ( itr != this->viewport_removed_map.end() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		static void ViewportConstruct(Object* pUnit, Object* pConstruct, uint8 * buffer, uint16 & size);
		static void ViewportConstructMonster(Object* pUnit, Monster* pMonster, uint8 * buffer, uint16 & size);
		static void ViewportConstructMonsterSummon(Object* pUnit, Monster* pMonster, uint8 * buffer, uint16 & size);
		static void ViewportConstructPlayer(Object* pUnit, Player* pPlayer, uint8 * buffer, uint16 & size);
		static void ViewportConstructPlayerChange(Object* pUnit, Player* pPlayer, uint8 * buffer, uint16 & size);
		static void ViewportConstructCastleMachine(Object* pUnit, Unit* pConstruct, uint8 * buffer, uint16 & size);
		static void ViewportConstructWorldItem(Object* pUnit, WorldItem* pWorldItem, uint8 * buffer, uint16 & size);
		static void ViewportConstructBuff(Object* pUnit, uint8 * buffer, uint16 & size);

		ViewportDataList const& GetAllViewport() const { return viewport_data; }

		ViewportDataList viewport_data;
		ViewportRemovedMap viewport_removed_map;

		//virtual void sendPacket(uint8 * packet, uint16 len) { }

		virtual void SendCustomObjectData() = 0;
		virtual void UpdateStatus() = 0;
		virtual void Respawn() = 0;

		virtual bool SendViewportData() const { return true; }

		void Remove();

		void DieSend();
		void SendData();

		virtual bool Update(uint32 diff) = 0;
		void AddToWorld();
		void UpdateTimer();

		virtual bool IsViewportControl() const = 0; // Viewport Control

		void OnObjectAdd(Object* pObject);
		void OnObjectRemove(Object* pObject);

		uint32 GetHighGUID() const;

		bool CheckWall(Object* pObject) const;
		bool CheckWall(coord_type x, coord_type y) const;
	private:
		DECLARE_ENUM(uint16, Entry);
		DECLARE_ENUM(ObjectType, Type);
		DECLARE_ENUM(MainObjectType, SendType);
		DECLARE_ENUM(ObjectState, State);
		DECLARE_ENUM(ConnectStatus, ConnectStatus);
		DECLARE_ENUM(DeadStatus, DeadStatus);
		DECLARE_BOOL(Live);
		DECLARE_ENUM(TCType, RespawnTick);
		DECLARE_ENUM(uint32, RespawnTime);
		
		DECLARE_PROPERTY(uint8, EventId);
		DECLARE_PROPERTY(int32, EventGround);
		DECLARE_PROPERTY(int32, EventStage);
		DECLARE_PROPERTY(int32, EventScore);

		DECLARE_FLOAT(Size);
		DECLARE_PROPERTY_ARRAY(int32, FrustrumX, MAX_ARRAY_FRUSTRUM);
		DECLARE_PROPERTY_ARRAY(int32, FrustrumY, MAX_ARRAY_FRUSTRUM);

		DECLARE_ENUM(uint32, SecondTick);
		DECLARE_BOOL(SecondPassed);

		DECLARE_ENUM(uint32, HalfSecondTick);
		DECLARE_BOOL(HalfSecondPassed);

		DECLARE_BOOL(ViewportEmpty);
};

#endif