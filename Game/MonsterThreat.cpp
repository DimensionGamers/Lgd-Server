/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterThreat.cpp"
*
*/

void MonsterThreat::Create(Monster* pOwner)
{
	this->SetOwner(pOwner);
	this->SetMostHatedID(0);
	this->SetHighestThreat(0);
}

void MonsterThreat::ClearThreat()
{
	this->SetMostHatedID(0);
	this->SetHighestThreat(0);
	CLEAR_MAP(this->m_ThreatMap);
}
	
void MonsterThreat::ClearDamage()
{
	LIST_CLEAR(DamageDataList::iterator, this->damage_list);
}

void MonsterThreat::AddThreat(Player* pPlayer, int64 threat)
{
	if ( !this->IsUsingThreat() )
		return;

	if ( !pPlayer )
		return;

    if ( pPlayer->IsAdministrator() && !pPlayer->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) )
        return;

	if ( !pPlayer->IsLive() || !GetOwner()->IsLive() )
		return;

	if ( GetOwner()->GetAI() && !GetOwner()->GetAI()->AddAggro(pPlayer, threat) )
		return;

	///- Busco el jugador en la lista
	ThreatData* pData = this->GetThreatData(pPlayer->GetGUID());

	if ( pData ) ///- Si el jugador existe le sumo la amenaza
	{
		pData->AddThreat(threat);
		return;
	}

	///- Agrego el jugador a la lista
	this->m_ThreatMap[pPlayer->GetGUID()] = new ThreatData(pPlayer->GetGUID(), threat, pPlayer);
}

void MonsterThreat::AddDamage(Player* pPlayer, MonsterThreatDamageType type, int64 damage)
{
	if ( GetOwner()->IsSummoned() && GetOwner()->GetSummoner()->IsPlayer() )
	{
		return;
	}

	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->IsLive() || !GetOwner()->IsLive() )
	{
		return;
	}

	///- Busco el jugador en la lista
	DamageData* pData = this->GetDamageData(pPlayer);

	if ( pData ) ///- Si el jugador existe le sumo el daño
	{
		pData->AddDamage(pPlayer->GetName(), type, damage);
		return;
	}

	///- Agrego el jugador a la lista
	this->damage_list.push_back(new DamageData(pPlayer->GetGUID(), pPlayer->GetName(), type, damage));
}

bool ThreatSort(ThreatData a, ThreatData b)
{
	return a.GetThreat() < b.GetThreat();
}

void MonsterThreat::Update()
{
	if ( !GetOwner()->IsLive() )
		return;

	if ( !this->IsUsingThreat() )
		return;

	if ( this->GetOwner()->PowerGet(POWER_LIFE) == this->GetOwner()->PowerGetTotal(POWER_LIFE) )
	{
		this->ClearDamage();
	}

	Object* pObject = nullptr;
	Player* pPlayer = nullptr;

	VIEWPORT_LOOP_OBJECT(GetOwner(), pObject)

		pPlayer = pObject->ToPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->IsAdministrator() && !pPlayer->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) )
			continue;

		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
			continue;

		if ( pPlayer->HasBuff(BUFF_INVISIBILITY) )
			continue;

		if ( !IN_RANGE(GetOwner(), pPlayer, (GetOwner()->GetMonsterTemplate()->ViewRange + 2)) )
			continue;

		this->AddThreat(pPlayer, 0);

	VIEWPORT_CLOSE

	this->UpdateThreat();
}

void MonsterThreat::UpdateThreat()
{
	World* pWorld = GetOwner()->GetWorld();

	if (!pWorld)
		return;

	int64 MaxThreat = -1;
	uint32 MaxGUID = 0;
	Player* pPlayer = nullptr;

	for (ThreatDataMap::iterator it = this->m_ThreatMap.begin(); it != this->m_ThreatMap.end();)
	{
		ThreatData * data = it->second;

		if (!data)
		{
			this->m_ThreatMap.erase(it++);
			continue;
		}

		pPlayer = data->GetPlayer();

		if (!pPlayer || !pPlayer->IsPlaying() || !GetOwner()->SameDimension(pPlayer) || !pPlayer->IsLive())
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (pPlayer->GetGUID() != data->GetGuid())
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if ((pPlayer->IsAdministrator() && !pPlayer->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED)) || pPlayer->IsInSafeZone())
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled())
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (!pWorld->CheckWall(GetOwner()->GetX(), GetOwner()->GetY(), pPlayer->GetX(), pPlayer->GetY()))
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (!IN_RANGE(GetOwner(), pPlayer, (GetOwner()->GetMonsterTemplate()->ViewRange + 2)))
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (pPlayer->HasBuff(BUFF_INVISIBILITY))
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (pPlayer->IsInGuildWar())
		{
			delete it->second;
			this->m_ThreatMap.erase(it++);
			continue;
		}

		if (data->GetThreat() >= MaxThreat)
		{
			MaxThreat = data->GetThreat();
			MaxGUID = data->GetGuid();
		}

		++it;
	}

	this->SetMostHatedID(MaxGUID);
	this->SetHighestThreat(MaxThreat);
}

bool MonsterThreat::IsInThreat(Unit* pTarget)
{
	if ( !this->IsUsingThreat() )
		return true;

	if ( !pTarget ) ///- Si el target es inválido, entonces no pertenece
		return false;

	Player* pPlayer = nullptr;

	if ( pTarget->IsSummoned() ) ///- Si es un objeto sumoneado, tengo que verificar que su dueño sea un jugador
	{
		Unit* pSummoner = pTarget->GetSummoner();

		if ( !pSummoner ) ///- Si el objeto no tiene ningún dueño, entonces no lo proceso
			return false;

		pPlayer = pSummoner->ToPlayer();
	}
	else ///- En caso de que no sea un objeto sumoneado, tengo que verificar que sea un jugador
	{
		pPlayer = pTarget->ToPlayer();
	}

	if ( !pPlayer ) ///- Si no es un jugador, entonces no pertenece
		return false;

	if ( !pPlayer->IsPlaying() ) ///- Solamente proceso cuando está jugando
		return false;

	ThreatData * pData = this->GetThreatData(pPlayer->GetGUID());

	return pData != nullptr;
}

bool MonsterThreat::IsUsingThreat() const
{
	if ( GetOwner()->IsSummoned() || !GetOwner()->IsUnitAttackable() )
		return false;

	if ( GetOwner()->GetAI() && !GetOwner()->GetAI()->ManageThreat() )
		return false;

	return true;
}

DamageData* MonsterThreat::GetDamageData(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	for ( DamageDataList::iterator it = this->damage_list.begin(); it != this->damage_list.end(); ++it )
	{
		DamageData* pData = (*it);

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetGuid() != pPlayer->GetGUID() )
		{
			continue;
		}

		return pData;
	}

	return nullptr;
}
	
DamageData* MonsterThreat::GetDamageData(uint32 char_id)
{
	for ( DamageDataList::iterator it = this->damage_list.begin(); it != this->damage_list.end(); ++it )
	{
		DamageData* pData = (*it);

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetGuid() != char_id )
		{
			continue;
		}

		return pData;
	}

	return nullptr;
}

ThreatData* MonsterThreat::GetThreatData(uint32 id)
{
	auto itr = this->m_ThreatMap.find(id);
	if (itr != this->m_ThreatMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}