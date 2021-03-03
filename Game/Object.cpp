/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Object.cpp"
*
*/

ObjectLocation::ObjectLocation(uint16 world, int32 instance, int16 x, int16 y)
{
	this->Reset();
	this->SetWorldId(world);
	this->SetInstance(instance);
	this->SetX(x);
	this->SetY(y);
}

uint16 ObjectLocation::GetDisplayWorld() const
{
	World* pWorld = sWorldMgr->GetWorld(this->GetWorldId());

	if ( !pWorld )
	{
		return 0;
	}
	else
	{
		return pWorld->GetDisplay();
	}
}

bool ObjectLocation::IsWorldFlag(uint32 flag) const
{
	World* pWorld = sWorldMgr->GetWorld(this->GetWorldId());

	if ( !pWorld )
	{
		return false;
	}
	else
	{
		return pWorld->flagHas(flag);
	}
}

Object::Object(uint16 entry)
{
	this->SetEntry(entry);
	this->viewport_data.clear();

	this->Init();
}

Object::~Object()
{
	MAP_CLEAR(ViewportDataList::iterator, this->viewport_data);
}

void Object::Init()
{
	this->SetState(OBJECT_STATE_NONE);
	this->SetLive(false);

	this->GetLocation()->Reset();
	this->SetEventId(-1);
	this->SetEventGround(-1);
	this->SetEventStage(-1);
	this->SetEventScore(0);

	this->SetSize(1.0f);
	this->ResetFrustrumX(0);
	this->ResetFrustrumY(0);

	this->SetDeadStatus(DEAD_NONE);
	this->SetRespawnTick(MyGetTickCount());
	this->SetRespawnTime(0);

	this->SetSecondTick(GetTickCount());
	this->SetSecondPassed(false);

	this->SetHalfSecondTick(GetTickCount());
	this->SetHalfSecondPassed(false);

	this->SetViewportEmpty(true);

	this->ViewportClear();
}

bool Object::SameDimension(Object const* pObject) const
{
	if (!pObject)
		return false;

	if ( this->GetWorldId() != pObject->GetWorldId() )
		return false;

	if ( this->GetInstance() != pObject->GetInstance() )
		return false;

	return true;
}

bool Object::SameDimension(uint16 world, int32 instance) const
{
	if ( this->GetWorldId() != world )
		return false;

	if ( this->GetInstance() != instance )
		return false;

	return true;
}

WorldGrid Object::GetGrid() const
{
	if ( World * pWorld = this->GetWorld() )
	{
		return pWorld->GetGrid(this->GetX(), this->GetY());
	}

	return WorldGrid(WORLD_ATTRIBUTE_LOCK_1);
}

bool Object::IsInSafeZone() const
{
	if ( World * pWorld = this->GetWorld() )
	{
		return pWorld->GetGrid(this->GetX(), this->GetY()).IsSafe();
	}

	return false;
}

World * Object::GetWorld() const
{
	return sWorldMgr->GetWorld(this->GetWorldId());
}

const char* Object::GetWorldName() const
{
	World* pWorld = this->GetWorld();

	return pWorld ? pWorld->GetCurrentStatusName(): "";
}

void Object::UpdateWorld()
{
	if ( this->GetWorldId() != this->GetLastLocation()->GetWorldId() )
	{
		this->GetLastLocation()->SetWorldId(this->GetWorldId());

		if ( World * pWorld = this->GetWorld() )
		{
			pWorld->AddObject(this);

			if ( Player* pPlayer = this->ToPlayer() )
			{
				sTeleport->SendBattleZone(pPlayer, pWorld->GetEntry());

				pPlayer->CalculateCharacter();
			}
		}
	}
}

void Object::CreateFrustrum()
{
	for ( uint8 i = 0; i < MAX_ARRAY_FRUSTRUM; ++i )
	{
		this->SetFrustrumX(i, FrustrumX[i] + this->GetX());
		this->SetFrustrumY(i, FrustrumY[i] + this->GetY());
	}
}

int32 Object::Distance(int16 x, int16 y)
{
	return Util::Distance(this->GetX(), this->GetY(), x, y);
}
	
bool Object::InRange(int16 x, int16 y, int32 distance)
{
	return Distance(x, y) <= distance;
}

void Object::sendPacket_viewport(uint8 * packet, uint16 len)
{
	if ( Player* pPlayer = this->ToPlayer() )
	{
		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
		{
			return;
		}

		if ( pPlayer->IsAdministrator() && pPlayer->HasBuff(BUFF_INVISIBILITY) )
		{
			return;
		}
	}
	
	ViewportDataList const& viewport = this->GetAllViewport();
	for ( ViewportDataList::const_iterator it = viewport.begin(); it != viewport.end(); ++it )
	{
		ViewportData* data = it->second;

		if ( !data )
			continue;

		if ( data->GetStatus() != Viewport::STATUS_READY )
			continue;

		Object* pObject = data->GetObjectData();

		if ( !pObject )
			continue;

		Player* pPlayer = pObject->ToPlayer();

		if ( !pPlayer )
			continue;

		pPlayer->sendPacket(packet, len);
	}
}

void Object::SendPacketViewportChat(uint8 * packet, uint16 len)
{
	if (Player* pPlayer = this->ToPlayer())
	{
		if (pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled())
		{
			return;
		}

		if (pPlayer->IsAdministrator() && pPlayer->HasBuff(BUFF_INVISIBILITY))
		{
			return;
		}
	}

	ViewportDataList const& viewport = this->GetAllViewport();
	for (ViewportDataList::const_iterator it = viewport.begin(); it != viewport.end(); ++it)
	{
		ViewportData* data = it->second;

		if (!data)
			continue;

		if (data->GetStatus() != Viewport::STATUS_READY)
			continue;

		Object* pObject = data->GetObjectData();

		if (!pObject)
			continue;

		Player* pPlayer = pObject->ToPlayer();

		if (!pPlayer)
			continue;

		if (pPlayer->ChatBlockFind(this->GetName()))
		{
			continue;
		}

		pPlayer->sendPacket(packet, len);
	}
}

void Object::SendPacketViewport(C1_HEADER * packet)
{
	this->sendPacket_viewport((uint8*)packet, packet->GetSize());
}

void Object::SendPacketViewport(C2_HEADER * packet)
{
	this->sendPacket_viewport((uint8*)packet, packet->GetSize());
}

void Object::SendPacketViewport(uint8 * packet)
{
	uint8 header = packet[0];

	switch (header)
	{
	case 0xC1:
	case 0xC3:
	{
		C1_HEADER * pHeader = (C1_HEADER*)packet;

		this->sendPacket_viewport(packet, pHeader->GetSize());
	} break;

	case 0xC2:
	case 0xC4:
	{
		C2_HEADER * pHeader = (C2_HEADER*)packet;

		this->sendPacket_viewport(packet, pHeader->GetSize());
	} break;
	}
}

bool Object::CheckFrustrumViewport(Object * pObject)
{
	if ( !pObject )
		return false;

	if( pObject->GetX() < this->GetX() - Viewport::MAX_DISTANCE || pObject->GetX() > this->GetX() + Viewport::MAX_DISTANCE || 
		pObject->GetY() < this->GetY() - Viewport::MAX_DISTANCE || pObject->GetY() > this->GetY() + Viewport::MAX_DISTANCE)
	{
		return false;
	}

	uint8 fail = 0;

	if ( sGameServer->IsViewportFrustrum() )
	{
		if ( !this->IsWorldFlag(WORLD_FLAG_NO_FRUSTRUM) )
		{
			int32 j = 3;

			for( int32 i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++ )
			{
				int32 frustrum = (this->GetFrustrumX(i)- pObject->GetX()) * (this->GetFrustrumY(j) - pObject->GetY()) - (this->GetFrustrumX(j) - pObject->GetX()) * (this->GetFrustrumY(i) - pObject->GetY());
				if(frustrum < 0)
				{
					fail++;
					break;
				}
			}

			if ( sGameServer->IsDobleFrustrum() )
			{
				j = 3;

				for( int32 i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++ )
				{
					int32 frustrum = (pObject->GetFrustrumX(i)- this->GetX()) * (pObject->GetFrustrumY(j) - this->GetY()) - (pObject->GetFrustrumX(j) - this->GetX()) * (pObject->GetFrustrumY(i) - this->GetY());
					if(frustrum < 0)
					{
						fail++;
						break;
					}
				}
			}
			else
			{
				if ( fail != 0 )
					fail = 2;
			}
		}
	}

	return fail < 2;
}

void Object::Remove()
{
	this->ViewportClose();
	this->SetConnectStatus(CONNECT_STATUS_NONE);
}

void Object::DieSend()
{
	DIE_SEND pMsg(this->GetEntry(), 0, 0);

	this->sendPacket_viewport(MAKE_PCT(pMsg));
	this->sendPacket(MAKE_PCT(pMsg));
}

void Object::SendData()
{

}

void Object::AddToWorld()
{
	if ( this->GetType() == OBJECT_TYPE_WORLD_ITEM )
	{
		return;
	}

	this->SetConnectStatus(CONNECT_STATUS_PLAYING);

	// Solo agrego objetos que no sean jugadores, los jugadores los agrego al abrir socket
	if ( this->GetType() != OBJECT_TYPE_PLAYER )
	{
		sObjectMgr->AddSession(this);
	}
	else
	{
		sObjectMgr->AddSessionCharacter(this->ToPlayer());
	}
}

void Object::UpdateTimer()
{
	if ( (GetTickCount() - this->GetSecondTick()) > IN_MILLISECONDS )
	{
		this->SetSecondTick(GetTickCount());
		this->SetSecondPassed(true);
	}
	else
	{
		this->SetSecondPassed(false);
	}

	if ( (GetTickCount() - this->GetHalfSecondTick()) > 500 )
	{
		this->SetHalfSecondTick(GetTickCount());
		this->SetHalfSecondPassed(true);
	}
	else
	{
		this->SetHalfSecondPassed(false);
	}
}

uint32 Object::GetHighGUID() const
{
	if ( this->GetType() == OBJECT_TYPE_WORLD_ITEM )
	{
		return MAKE_NUMBERDW(1, this->GetEntry());
	}

	return MAKE_NUMBERDW(0, this->GetEntry());
}

bool Object::CheckWall(Object* pObject) const
{
	if ( !pObject )
		return false;

	return this->CheckWall(pObject->GetX(), pObject->GetY());
}

bool Object::CheckWall(int16 x, int16 y) const
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	return pWorld->CheckWall(this->GetX(), this->GetY(), x, y);
}