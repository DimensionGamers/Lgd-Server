/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "WorldClass.cpp"
*
*/

WorldStatusData::WorldStatusData()
{
	this->SetStatus(0);
	this->ResetName();
}

WorldStatusData::~WorldStatusData()
{
}

uint8 attribute[WORLD_MAX_SIZE];

#define ATT_FILE_129KB_SIZE 65536*2+4
#define ATT_FILE_65KB_SIZE 65536+4

inline void decrypt2(char* buffer, size_t size)
{
	const BYTE xorKeys[] = {0xFC, 0xCF, 0xAB};
	for (size_t i=0; i<size; ++i)
	{
		*buffer ^= xorKeys[i%3];
		buffer++;
	}
}

inline void decrypt(char* buffer, size_t size)
{
	const BYTE xorKeys[] = {
		0xd1, 0x73, 0x52, 0xf6,
		0xd2, 0x9a, 0xcb, 0x27,
		0x3e, 0xaf, 0x59, 0x31,
		0x37, 0xb3, 0xe7, 0xa2
	};
	char key = 0x5E;
	for (size_t i=0; i<size; ++i)
	{
		char encode = *buffer;
		*buffer ^= xorKeys[i%16];
		*buffer -= key;
		key = encode+0x3D;
		buffer++;
	}
}

bool WorldStatusData::LoadWorld(const char * filename)
{
	FILE * fp = fopen(filename, "rb");
	
	if ( !fp )
	{
		sLog->outError("root", "%s -- Wrong World Attribute File. [%s - %s]", __FUNCTION__, this->GetName(), filename);
		return false;
	}

	uint8 head;
	fread(&head, sizeof(head), 1, fp);
	fread(&this->width, sizeof(char), 1, fp);
	fread(&this->height, sizeof(char), 1, fp);

	if ( this->width > 255 || this->height > 255 )
	{
		fclose(fp);
		return false;
	}

	fread(attribute, 256, 256, fp);
	fclose(fp);

	for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
	{
		this->GetGrid(i)->attribute = attribute[i];
	}

	return true;
}

World::World()
{
	this->SetEntry(-1);
	this->SetDisplay(0);
	this->SetStatus(0);

	for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
		this->ItemData[i] = new WorldItem(i);

	this->m_world_item_loop = 0;
	this->SetWeather(0);
	this->SetWeatherVariation(0);
	this->weather_tick.Reset();
	this->SetPKBossEnabled(false);
	this->GetPKBossTime()->Reset();

	this->GetPathFinderManager()->Create(this);
}

World::~World() 
{ 
	LIST_CLEAR(WorldAIPathList::iterator, this->m_path_list);
	MAP_CLEAR(WorldBuffMap::iterator, this->m_buff);
	LIST_CLEAR(WorldAreaList::iterator, this->m_area);
	LIST_CLEAR(WorldItemQueuedList::iterator, this->world_item_queue);
	LIST_CLEAR(WorldAreaAttributeList::iterator, this->m_attribute);
	
	for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
		delete this->ItemData[i];

	this->world_object_map.clear();
	this->world_player_map.clear();
}

const char* World::GetCurrentStatusName()  const
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return "";
	else
		return WorldStatus[GetStatus()].GetName();
}

bool World::IsCurrentStatusFlag(uint32 flags) const
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return false;
	else
		return (WorldStatus[GetStatus()].GetFlags() & flags) != 0;
}
	
uint32 World::GetCurrentStatusExperienceRate() const
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return 0;
	else
		return WorldStatus[GetStatus()].GetExperienceRate();
}
	
uint32 World::GetCurrentStatusZenRate() const
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return 0;
	else
		return WorldStatus[GetStatus()].GetZenRate();
}

uint16 World::GetCurrentGate() const
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return 0;
	else
		return WorldStatus[GetStatus()].GetSpawnGate();
}

void World::GetRespawn(uint16 & spawn_world, int16 & x1, int16 & y1, int16 & x2, int16 & y2)
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return;

	auto const& status_data = WorldStatus[GetStatus()];
	if (!status_data.IsActive())
		return;

	auto gate_data = sTeleport->GetGate(status_data.GetSpawnGate());
	if (gate_data)
	{
		spawn_world = gate_data->MapId;
		x1 = gate_data->X1;
		y1 = gate_data->Y1;
		x2 = gate_data->X2;
		y2 = gate_data->Y2;
	}
}

void World::GetRespawn(uint16 & spawn_world, int16 & x, int16 & y)
{
	if (GetStatus() >= MAX_WORLD_STATUS)
		return;

	auto const& status_data = WorldStatus[GetStatus()];
	if (!status_data.IsActive())
		return;

	auto gate_data = sTeleport->GetGate(status_data.GetSpawnGate());
	if (gate_data)
	{
		if (World* pWorld = sWorldMgr->GetWorld(gate_data->MapId))
		{
			spawn_world = gate_data->MapId;
			pWorld->GetRandomLocation(x, y, gate_data->X1, gate_data->Y1, gate_data->X2, gate_data->Y2);
		}
	}
}

void World::GetRandomLocation(int16 & x, int16 & y, int16 x1, int16 y1, int16 x2, int16 y2)
{
	int32 loopcount = 50;
	int16 tx = x;
	int16 ty = y;

	x = x1;
	y = y1;

	while ( loopcount-- > 0 )
	{
		if ( (x2 - x1) > 0 )
		{
			tx = x1 + ( Random(x2 - x1) );
		}
		else
		{
			tx = x1;
		}
		if ( (y2 - y1) > 0 )
		{
			ty = y1 + ( Random(y2 - y1) );
		}
		else
		{
			ty = y1;
		}

		WorldGrid const& attr = this->GetGrid(tx, ty);

		if ( !attr.IsLocked_1() && !attr.IsLocked_2() )
		{
			x = tx;
			y = ty;
			break;
		}
	}
}

bool World::GetFreeLocation(int16 & x, int16 & y, int16 range_x, int16 range_y, int32 count)
{
	int16 tx = x;
	int16 ty = y;

	while ( count-- > 0 )
	{
		tx = Random(range_x + 1) * (((Random(2) == 0) ? -1: 1)) + x;
		ty = Random(range_y + 1) * (((Random(2) == 0) ? -1: 1)) + y;

		FIX_COORD(tx);
		FIX_COORD(ty);

		WorldGrid const& attr = this->GetGrid(tx, ty);

		if ( attr.attribute == 0 )
		{
			x = tx;
			y = ty;
			return true;
		}
	}

	return false;
}

bool World::GetRandomLocation(int16 & x, int16 & y, int32 length) const
{
	int32 count = 10;
	
	if ( length < 1 )
		length = 1;

	int32 max_length = length * 2;
		
	while ( count-- > 0 )
	{
		int16 px = (x - length) + Random(max_length);
		int16 py = (y - length) + Random(max_length);

		WorldGrid const& grid = this->GetGrid(px, py);

		if ( !grid.IsLocked_1() && !grid.IsLocked_2() )
		{
			x = px;
			y = py;
			return true;
		}
	}

	return false;
}

bool World::IsObject(Object * pObject)
{
	return true;
}

void World::AddObject(Object * pObject)
{
	/// - No debería pasar pero por las dudas reviso que no sea nulo
	if ( !pObject )
		return;

	/// - Solamente agregar Players, Monsters, Npc
	if ( pObject->GetType() == OBJECT_TYPE_WORLD_ITEM )
		return;

	/// - Buscar si ya existe en la lista
	WorldObjectMap::const_iterator it = this->world_object_map.find(pObject->GetEntry());

	if ( it != this->world_object_map.end() )
		return;

	this->world_object_map[pObject->GetEntry()] = pObject;
	
	if ( Unit* pUnit = pObject->ToUnit() )
		this->RemoveInvalidBuff(pUnit);
}

void World::RemoveInvalidBuff(Unit* pUnit)
{
	if ( !pUnit || !pUnit->IsPlayer() )
		return;

	bool update = false;
	WorldBuff* buff_data = nullptr;

	BUFF_LOOP(i)
	{
		BuffData * data = pUnit->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		buff_data = this->GetBuffData(data->GetID());

		if ( !buff_data )
		{
			if ( data->IsDisabled() )
			{
				data->SetDisabled(false);
				update = true;
			}
			continue;
		}

		if ( buff_data->IsFlag(WORLD_BUFF_FLAG_DISABLE) )
		{
			data->SetDisabled(true);
			update = true;
		}

		if ( buff_data->IsFlag(WORLD_BUFF_FLAG_REMOVE) )
		{
			pUnit->RemoveBuff(data->GetID(), false);
		}
	}

	if ( update )
	{
		PLAYER_POINTER(pUnit)->CalculateCharacter();
	}
}

WorldBuff* World::GetBuffData(uint16 buff) const
{
	WorldBuffMap::const_iterator it = this->m_buff.find(buff);

	return it != this->m_buff.end() ? it->second: nullptr;
}

void World::Update()
{
	if ( this->GetPKBossTime()->Elapsed(sGameServer->GetPKBossTime()) )
	{
		this->SetPKBossEnabled(false);
	}

	this->world_player_map.clear();

	for ( WorldObjectMap::iterator it = this->world_object_map.begin(); it != this->world_object_map.end(); )
	{
		Object* pObject = it->second;

		if ( !pObject )
		{
			this->world_object_map.erase(it++);
			continue;
		}

		if ( pObject->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			this->world_object_map.erase(it++);
			continue;
		}

		if ( pObject->IsUnit() && pObject->ToUnit()->GetRegenStatus() != REGEN_NONE )
		{
			++it;
			continue;
		}

		if ( pObject->GetWorldId() != this->GetEntry() )
		{
			this->world_object_map.erase(it++);
			continue;
		}

		if ( Player* pPlayer = pObject->ToPlayer() )
		{
			this->world_player_map[pPlayer->GetEntry()] = pPlayer;
		}

		++it;
	}

	for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
	{
		this->GetItemData(i)->Update(0);
	}
}

void World::UpdateWeather()
{
	if ( !this->weather_tick.Elapsed() )
		return;

	this->weather_tick.SetDelay(10000 + Random(10000));
	this->SetWeatherVariation(Random(10));
	this->SetWeather(Random(3));

	if ( this->world_player_map.empty() )
		return;

	Player* pPlayer = nullptr;

	WorldPlayerMap const& wplayers = this->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = wplayers.begin(); it != wplayers.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() )
			continue;

		pPlayer->SendWeather(this->GetWeatherConverted());
	}
}

void World::UpdateItemQueue()
{
	for ( WorldItemQueuedList::const_iterator it = this->world_item_queue.begin(); it != this->world_item_queue.end(); )
	{
		if ( this->add_item((*it)->item, (*it)->GetOwner(), (*it)->GetX(), (*it)->GetY(), (*it)->IsOnlyOwner(), (*it)->IsVisible(), false) )
		{
			uint16 owner_entry = -1;
			std::string owner_name = "";

			if ( Unit* pUnit = (*it)->GetOwner() )
			{
				owner_entry = pUnit->GetEntry();
				owner_name = pUnit->GetName();
			}

			sLog->outInfo(LOG_ITEMS, "%s -- [%u / %s -- %d / %d] Owner: [%u / %s] -- %s", __FUNCTION__, this->GetEntry(), this->GetCurrentStatusName(), (*it)->GetX(), (*it)->GetY(), 
				owner_entry, owner_name.c_str(), (*it)->item.BuildLog(0).c_str());

			delete *it;
			it = this->world_item_queue.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool World::add_item(Item item, Unit* owner, int16 x, int16 y, bool only_owner, bool visible, bool to_queue)
{
	if ( this->GetGrid(x, y).IsLocked_1() || this->GetGrid(x, y).IsLocked_2() )
	{
		return false;
	}

	if ( to_queue )
	{
		WorldItemQueued * pItemQueued = new WorldItemQueued;
		pItemQueued->SetOwner(owner);
		pItemQueued->item = item;
		pItemQueued->SetX(x);
		pItemQueued->SetY(y);
		pItemQueued->SetOnlyOwner(only_owner);
		pItemQueued->SetVisible(visible);

		this->world_item_queue.push_back(pItemQueued);
		return true;
	}

	int32 totalcount = 0;

	while ( true )
	{
		if ( this->GetItemData(this->m_world_item_loop)->GetState() == WORLD_ITEM_STATE_NONE )
		{
			this->GetItemData(this->m_world_item_loop)->Clear();
			this->GetItemData(this->m_world_item_loop)->SetState(WORLD_ITEM_STATE_STANDBY);
			this->GetItemData(this->m_world_item_loop)->item = item;
			this->GetItemData(this->m_world_item_loop)->item.Convert();
			this->GetItemData(this->m_world_item_loop)->SetOwner(item.GetItem() == ITEMGET(14, 15) ? nullptr: owner);
			this->GetItemData(this->m_world_item_loop)->GetLocation()->SetWorldId(this->GetEntry());
			this->GetItemData(this->m_world_item_loop)->GetLocation()->SetInstance(owner ? owner->GetInstance(): -1);
			this->GetItemData(this->m_world_item_loop)->GetLocation()->SetX(x);
			this->GetItemData(this->m_world_item_loop)->GetLocation()->SetY(y);
			this->GetItemData(this->m_world_item_loop)->SetLive(true);

			uint32 time = item.GetDurationTime() == 0 ? sGameServer->item_duration_time.get(): item.GetDurationTime();
			uint32 looting = item.GetLootingTime() == 0 ? sGameServer->looting_time.get(): item.GetLootingTime();

			this->GetItemData(this->m_world_item_loop)->GetLiveTime()->Start(time);
			this->GetItemData(this->m_world_item_loop)->GetLootingTime()->Start(looting);
			this->GetItemData(this->m_world_item_loop)->SetOnlyOwner(only_owner);
			this->GetItemData(this->m_world_item_loop)->SetEventId(owner ? owner->GetEventId(): EVENT_NONE);
			this->GetItemData(this->m_world_item_loop)->SetEventGround(owner ? owner->GetEventGround(): 0);
			this->GetItemData(this->m_world_item_loop)->SetVisible(visible);

			if ( item.IsEventArchangelWeapon() )
			{
				this->GetItemData(this->m_world_item_loop)->SetEventId(item.bc_weapon.event_id);
				this->GetItemData(this->m_world_item_loop)->SetEventGround(item.bc_weapon.event_ground);
			}

			/*sLog->outInfo(LOG_ITEMS, "WorldItem Owner:[%d-%s] - World: %d(%s) - Instance:%d - Location: [%d/%d] - ID:%d - ItemData: %s",
				owner ? owner->GetEntry(): -1, NAME_SAFE(owner), this->GetEntry(), this->GetCurrentStatusName(), 
				this->GetItemData(this->m_world_item_loop)->GetInstance(), x, y, this->m_world_item_loop, item.BuildLog(uint8(-1)).c_str());*/

			++this->m_world_item_loop;
		
			if ( this->m_world_item_loop >= MAX_WORLD_ITEM )
			{
				this->m_world_item_loop = 0;
			}

			return true;
		}

		++this->m_world_item_loop;
		
		if ( this->m_world_item_loop >= MAX_WORLD_ITEM )
		{
			this->m_world_item_loop = 0;
		}

		++totalcount;

		if ( totalcount >= MAX_WORLD_ITEM )
		{
			return false;
		}
	}

	return false;
}

WorldItem * World::GetItem(uint16 id)
{
	if ( id >= MAX_WORLD_ITEM )
		return nullptr;

	return this->GetItemData(id);
}

void World::ClearItem()
{
	for ( uint16 i = 0; i < MAX_WORLD_ITEM; i++ )
	{
		this->GetItemData(i)->Clear();
	}

	this->m_world_item_loop = 0;
}

WorldGrid World::GetGrid(int16 x, int16 y) const
{
	if ( !IS_COORDINATE_RANGE(x) || !IS_COORDINATE_RANGE(y) )
	{
		return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
	}

	if ( this->GetStatus() >= MAX_WORLD_STATUS )
	{
		return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
	}

	WorldStatusData const& StatusData = this->WorldStatus[this->GetStatus()];

	if ( !StatusData.IsActive() )
	{
		return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
	}

	int32 grid = WORLD_MAKE_GRID(x, y);

	if ( grid < 0 || grid >= WORLD_MAX_SIZE )
	{
		return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
	}

	WorldGrid const* Grid = StatusData.GetGrid(grid);

	if ( !Grid )
	{
		return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
	}

	return *Grid;
}

bool World::GetRandomDropLocation(int16 stx, int16 sty, int16 & x, int16 & y, int16 range_x, int16 range_y, int32 loop) const
{
	limitmin(range_x, 1);
	limitmin(range_y, 1);
	limitmin(loop, 1);

	int16 tmp_x = stx;
	int16 tmp_y = sty;

	while ( loop-- > 0 )
	{
		tmp_x = Random(range_x + 1) * (((Random(2) == 0) ? -1: 1)) + stx;
		tmp_y = Random(range_y + 1) * (((Random(2) == 0) ? -1: 1)) + sty;

		 if ( this->IsAreaRestriction(tmp_x, tmp_y, WORLD_AREA_FLAG_ITEM_DROP) )
			 continue;

		if ( this->GetGrid(tmp_x, tmp_y).IsDropAllowed() )
		{
			x = tmp_x;
			y = tmp_y;
			return true;
		}
	}

	return false;
}

bool World::AddZen(Unit* pUnit, int16 x, int16 y, uint32 zen)
{
	WorldGrid const& attr = this->GetGrid(x, y);

	if ( !attr.IsZenAllowed() || !zen )
		return false;

	Item item(ITEMGET(14, 15));
	item.zen.ammount = zen;

	return this->add_item(item, pUnit, x, y, false);
}

bool World::CheckWall(int16 sx1, int16 sy1, int16 sx2, int16 sy2) const
{
	if ( this->GetStatus() >= MAX_WORLD_STATUS )
	{
		return false;
	}

	WorldStatusData const& StatusData = this->WorldStatus[this->GetStatus()];

	if ( !StatusData.IsActive() )
	{
		return false;
	}

	int32 Index = WORLD_MAKE_GRID(sx1, sy1);
	int16 nx1;
	int16 ny1;
	int16 d1;
	int16 d2;
	int16 len1;
	int16 len2;
	int16 px1 = sx2 - sx1;
	int16 py1 = sy2 - sy1;

	if ( px1 < 0 )
	{
		px1 = -px1;
		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if ( py1 < 0 )
	{
		py1 = -py1;
		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if ( px1 > py1 )
	{
		len1 = px1;
		len2= py1;
		d1 = ny1;
		d2 = nx1;
	}
	else
	{
		len1 = py1;
		len2 = px1;
		d1 = nx1;
		d2 = ny1;
	}

	int32 error = 0;
	int32 count = 0;

	do
	{
		if ( Index < 0 || Index >= WORLD_MAX_SIZE )
		{
			return false;
		}

		WorldGrid const* Grid = StatusData.GetGrid(Index);

		if ( !Grid || Grid->IsLocked_1() )
			return false;

		error += len2;

		if ( error > len1/2 )
		{
			Index += d1;
			error -= len1;
		}
		
		Index += d2;
		count++;	
	}
	while ( count <= len1 );

	return true;
}

void World::ApplyAttribute(int16 x, int16 y, uint8 attr, bool apply)
{
	if ( !IS_COORDINATE_RANGE(x) || !IS_COORDINATE_RANGE(y) )
	{
		return;
	}

	if ( this->GetStatus() >= MAX_WORLD_STATUS )
	{
		return;
	}

	WorldStatusData & StatusData = this->WorldStatus[this->GetStatus()];

	WorldGrid * Grid = StatusData.GetGrid(WORLD_MAKE_GRID(x, y));

	if ( !Grid )
		return;

	if ( apply )
	{
		Grid->attribute |= attr;
	}
	else
	{
		Grid->attribute &= ~attr;
	}
}

void World::ApplyAttribute(int16 const* data, uint8 attr, bool apply)
{
	for ( int16 x = data[0]; x <= data[2]; ++x )
	{
		for ( int16 y = data[1]; y <= data[3]; ++y )
		{
			this->ApplyAttribute(x, y, attr, apply);
		}
	}
}

void World::statusChange(uint8 status)
{
	if ( status >= MAX_WORLD_STATUS )
		return;

	if ( !this->WorldStatus[status].IsActive() )
		return;

	this->SetStatus(status);
}

void World::MakeItemVisible(Player* pPlayer)
{
	for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
	{
		if ( this->GetItemData(i)->GetOwner() != pPlayer )
			continue;

		this->GetItemData(i)->MakeVisible();
	}
}

bool World::IsAreaRestriction(int16 x, int16 y, uint8 flag) const
{
	for ( WorldAreaList::const_iterator it = this->m_area.begin(); it != this->m_area.end(); ++it )
	{
		if ( !(*it)->IsFlags(flag) )
			continue;

		int16 x1 = (*it)->GetX() - (*it)->GetRange();
		int16 y1 = (*it)->GetY() - (*it)->GetRange();
		int16 x2 = (*it)->GetX() + (*it)->GetRange();
		int16 y2 = (*it)->GetY() + (*it)->GetRange();

		FIX_COORD(x1);
		FIX_COORD(y1);
		FIX_COORD(x2);
		FIX_COORD(y2);

		if ( x >= x1 && x <= x2 &&
			 y >= y1 && y <= y2 )
		{
			return true;
		}
	}

	return false;
}

int32 World::GetPlayersCountByPC(Player* pPlayer)
{
	if (!pPlayer)
	{
		return -1;
	}

	int32 count = 0;

	WorldPlayerMap const& players = this->GetAllPlayers();
	for (WorldPlayerMap::const_iterator itr = players.begin(); itr != players.end(); ++itr)
	{
		Player* pWPlayer = itr->second;

		if (!pWPlayer || !pWPlayer->IsPlaying())
		{
			continue;
		}

		if (memcmp(pWPlayer->GetAccountData()->GetIP(), pPlayer->GetAccountData()->GetIP(), MAX_ACCOUNT_IP_LENGTH))
		{
			continue;
		}

		++count;
	}

	return count;
}