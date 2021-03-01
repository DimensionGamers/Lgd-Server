ImperialFortressInstance::ImperialFortressInstance()
{
	this->SetReadyToRemove(false);
	this->SetReadyToMove(false);

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		this->GetPlayer(i)->Reset();
	}

	this->SetZone(0);
	this->SetState(IMPERIAL_FORTRESS_STATE_NONE);
	this->GetTime()->Start();
	this->GetSecondTime()->Start();
	this->SetMonsterCount(0);
}

ImperialFortressInstance::~ImperialFortressInstance()
{
	LIST_CLEAR(ImperialFortressReleaseZoneList::iterator, this->m_ImperialFortressReleaseZoneList);
}

void ImperialFortressInstance::Update()
{
	switch (this->GetState())
	{
	case IMPERIAL_FORTRESS_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case IMPERIAL_FORTRESS_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case IMPERIAL_FORTRESS_STATE_DOOR_1:
	case IMPERIAL_FORTRESS_STATE_FIGHT:
	case IMPERIAL_FORTRESS_STATE_DOOR_2:
	case IMPERIAL_FORTRESS_STATE_DOOR_3:
	case IMPERIAL_FORTRESS_STATE_MOVE:
		{
			this->ProcState_Playing();
		} break;

	case IMPERIAL_FORTRESS_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void ImperialFortressInstance::SetState_None()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_NONE);
	this->SetDay(0);
	this->SetWeather(0);
	this->SetPartyID(-1);
	this->SetMonsterCount(0);
	this->GetTime()->Start();
	this->SetZoneLevel(0);

	this->MoveOut();
	this->ClearMonster();
}

void ImperialFortressInstance::SetState_StandBy()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_STANDBY);
	this->GetTime()->Start(sGameServer->GetImperialFortressStandbyTime());

	/*for (uint8 h = 0; h < IMPERIAL_FORTRESS_ZONE_MAX; ++h)
	{
		this->BlockReleaseZone(h, true);
	}*/

	this->AddMonster(IMPERIAL_FORTRESS_TYPE_GATE);
	this->AddMonster(IMPERIAL_FORTRESS_TYPE_STATUE);
}

void ImperialFortressInstance::SetState_Door1()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_DOOR_1);
	if (this->GetZone() < IMPERIAL_FORTRESS_MAX_ZONES)
	{
		this->GetTime()->Start(sGameServer->GetImperialFortressTimeZone(this->GetZone()));
	}
	else
	{
		this->GetTime()->Start(sGameServer->GetImperialFortressPlayTime());
	}
}

void ImperialFortressInstance::SetState_Fight()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_FIGHT);
}

void ImperialFortressInstance::SetState_Door2()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_DOOR_2);
}

void ImperialFortressInstance::SetState_Door3()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_DOOR_3);
}

void ImperialFortressInstance::SetState_Move()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetReadyToMove(true);

	this->SetState(IMPERIAL_FORTRESS_STATE_MOVE);
}

void ImperialFortressInstance::SetState_End()
{
	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());

	this->SetState(IMPERIAL_FORTRESS_STATE_END);
	this->GetTime()->Start(sGameServer->GetImperialFortressEndTime());
}

void ImperialFortressInstance::ProcState_None()
{

}

void ImperialFortressInstance::ProcState_StandBy()
{
	if (this->GetSecondTime()->Elapsed(IN_MILLISECONDS))
	{
		this->SendTime();
		this->TrapDamage();
		this->UpdatePlayers();
	}

	if (this->GetTime()->Elapsed())
	{
		this->SetReadyToMove(false);

		this->SetState_Door1();
	}
}

void ImperialFortressInstance::ProcState_Playing()
{
	if (this->GetSecondTime()->Elapsed(IN_MILLISECONDS))
	{
		this->SendTime();
		this->TrapDamage();
		this->UpdatePlayers();
	}

	switch (this->GetState())
	{
	case IMPERIAL_FORTRESS_STATE_FIGHT:
	{
		if (this->GetMonsterCount() <= 0)
		{
			if (this->GetZone() == 2 && this->GetDay() != 6)
			{
				this->SendSuccess();
				this->SetState_End();
			}
			else if (this->GetZone() == 3 && this->GetDay() == 6)
			{
				this->SendSuccess();
				this->SetState_End();
			}
			else
			{
				this->SetState_Door2();
			}
		}
	} break;
	}

	this->UpdateTime();
}

void ImperialFortressInstance::ProcState_End()
{
	if (this->GetSecondTime()->Elapsed(IN_MILLISECONDS))
	{
		for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			Player* pPlayer = this->GetPlayer(i)->GetPlayer();

			if (!pPlayer)
				continue;

			if (!pPlayer->IsPlaying() || !pPlayer->IsLive())
			{
				pPlayer->EventResetData();
				this->GetPlayer(i)->Reset();
				continue;
			}

			if (pPlayer->GetWorldId() != g_ImperialFortressWorld[this->GetDay()])
			{
				pPlayer->EventResetData();
				this->GetPlayer(i)->Reset();
				continue;
			}
		}
	}

	if (this->GetTime()->Elapsed())
	{
		this->SetState_None();
		this->SetReadyToRemove(true);
	}
}

void ImperialFortressInstance::UpdatePlayers()
{
	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if (!pPlayer)
		{
			continue;
		}

		if (!pPlayer->IsPlaying())
		{
			sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

			this->GetPlayer(i)->Reset();

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s not playing", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
			continue;
		}

		if (pPlayer->GetPartyID() != this->GetPartyID())
		{
			sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

			this->SendFail(pPlayer);

			if (sGameServer->IsImperialFortressMoveToPrevious())
			{
				pPlayer->TeleportToLocation(this->GetPlayer(i)->GetWorld(), this->GetPlayer(i)->GetX(), this->GetPlayer(i)->GetY(), -1, -1);
			}
			else
			{
				pPlayer->TeleportToGate(508);
			}
			
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s wrong party id [%u / %u]", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str(), pPlayer->GetPartyID(), this->GetPartyID());
			continue;
		}

		if (!pPlayer->IsLive())
		{
			sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

			this->SendFail(pPlayer);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s not live", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
			continue;
		}

		if (pPlayer->GetWorldId() != g_ImperialFortressWorld[this->GetDay()])
		{
			sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

			this->SendFail(pPlayer);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s not in world", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
			continue;
		}

		if (pPlayer->GetInstance() != this->GetID())
		{
			sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

			this->SendFail(pPlayer);
			
			if (sGameServer->IsImperialFortressMoveToPrevious())
			{
				pPlayer->TeleportToLocation(this->GetPlayer(i)->GetWorld(), this->GetPlayer(i)->GetX(), this->GetPlayer(i)->GetY(), -1, -1);
			}
			else
			{
				pPlayer->TeleportToGate(508);
			}

			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s not in instance", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
			continue;
		}

		if (!this->IsReadyToMove())
		{
			if (this->GetPlayer(i)->GetZone() != this->GetZone())
			{
				sImperialFortressMgr->RemoveInstanceByCharacter(this->GetPlayer(i)->GetGUID());

				this->SendFail(pPlayer);
				
				if (sGameServer->IsImperialFortressMoveToPrevious())
				{
					pPlayer->TeleportToLocation(this->GetPlayer(i)->GetWorld(), this->GetPlayer(i)->GetX(), this->GetPlayer(i)->GetY(), -1, -1);
				}
				else
				{
					pPlayer->TeleportToGate(508);
				}

				pPlayer->EventResetData();
				this->GetPlayer(i)->Reset();

				sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Removed player %s not in zone", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
				continue;
			}
		}
	}
}

void ImperialFortressInstance::TrapDamage()
{
	ImperialFortressTrapList const& traps = sImperialFortressMgr->GetTrapList(this->GetDay());
	for (ImperialFortressTrapList::const_iterator itr = traps.begin(); itr != traps.end(); ++itr)
	{
		ImperialFortressTrap const* pTrap = *itr;

		if (!pTrap)
		{
			continue;
		}

		for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			Player* pPlayer = this->GetPlayer(i)->GetPlayer();

			if (!pPlayer)
			{
				continue;
			}

			if (pTrap->IsInTrap(pPlayer->GetX(), pPlayer->GetY()))
			{
				int32 damage = pTrap->GetDamage() * pPlayer->PowerGetTotal(POWER_LIFE) / 100.0f;
				uint8 damage_type = pTrap->GetElement() == Element::ICE ? 3 :
					pTrap->GetElement() == Element::POISON ? 5 :
					pTrap->GetElement() == Element::FIRE ? 6 :
					0;

				pPlayer->Hit(pPlayer, damage, 0, damage_type);
			}
		}
	}
}

void ImperialFortressInstance::UpdateTime()
{
	if (this->IsZoneEmpty() || this->GetTime()->Elapsed())
	{
		this->SendFail();
		this->SetState_None();
		this->SetReadyToRemove(true);
	}
}

void ImperialFortressInstance::AddMonster(ImperialFortressMonsterType type)
{
	ImperialFortressLevel const* pLevelData = sImperialFortressMgr->GetLevel(this->GetZoneLevel());

	for (MonsterEventList::const_iterator it = sMonsterMgr->monster_event_list.begin(); it != sMonsterMgr->monster_event_list.end(); ++it)
	{
		if ((*it)->GetEventID() != EVENT_IMPERIAL_FORTRESS)
		{
			continue;
		}

		if ((*it)->imperial_fortress.day != (this->GetDay() + 1) || (*it)->imperial_fortress.stage != this->GetZone() || (*it)->imperial_fortress.type != type)
		{
			continue;
		}

		Monster* pMonster = sObjectMgr->MonsterTryAdd((*it)->GetID(), (*it)->GetWorld());

		if (pMonster)
		{
			pMonster->SetEventDBData(*it);
			pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);

			if (pLevelData)
			{
				if (ImperialFortressMonsterLevel * pMonsterData = pLevelData->GetMonster((*it)->GetID()))
				{
					pMonster->SetLevel(pMonsterData->level.get());

					for (uint8 i = 0; i < POWER_MAX; i++)
					{
						pMonster->PowerSet(i, pMonsterData->power[i].get());
						pMonster->PowerSetMax(i, pMonster->PowerGet(i));
						pMonster->SetScriptMaxPower(i, pMonster->PowerGet(i));
					}

					pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pMonsterData->attack_min.get());
					pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pMonsterData->attack_max.get());
					pMonster->SetIntData(UNIT_INT_ATTACK_RATE, pMonsterData->attack_rate.get());
					pMonster->SetIntData(UNIT_INT_DEFENSE, pMonsterData->defense.get());
					pMonster->SetIntData(UNIT_INT_DEFENSE_RATE, pMonsterData->defense_rate.get());
					pMonster->SetItemBag(pMonsterData->GetItemBag());
				}
			}

			pMonster->AddAdditionalDataInt(0, this->GetID());
			pMonster->AddAdditionalDataInt(1, (*it)->imperial_fortress.id);
			pMonster->AddAdditionalDataInt(2, this->GetZone());

			pMonster->SetEventGround(this->GetZone());
			pMonster->SetInstance(this->GetID());

			if (pMonster->GetScriptName() == "imperial_fortress_monster_ai" || pMonster->GetScriptName() == "imperial_fortress_boss_ai")
			{
				this->IncreaseMonsterCount(1);
			}

			pMonster->AddToWorld();

			pMonster->SetEventScore(0);

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Added Monster [%u][%u].", __FUNCTION__, this->GetDay(), this->GetZone(), pMonster->GetEntry(), pMonster->GetClass());
		}
	}
}

void ImperialFortressInstance::ClearMonster()
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if (!pMonster || !pMonster->IsPlaying())
		{
			continue;
		}

		if (pMonster->GetEventId() != EVENT_IMPERIAL_FORTRESS)
		{
			continue;
		}

		if (pMonster->GetInstance() != this->GetID())
		{
			continue;
		}

		pMonster->Remove();
	}

	sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u)", __FUNCTION__, this->GetDay(), this->GetZone());
}

void ImperialFortressInstance::BlockReleaseZone(uint8 zone, bool block)
{
	/*World * pWorld = sWorldMgr->GetWorld(g_ImperialFortressWorld[this->GetDay()]);

	if (!pWorld)
	{
		return;
	}

	ImperialFortressZoneDef const& data = g_ImperialFortressZone[this->GetDay()].zone[this->GetZone()][zone];

	coord_type buffer_data[4] = { data.x1, data.y1, data.x2, data.y2 };

	pWorld->ApplyAttribute(buffer_data, 16, block);
	pWorld->ApplyAttribute(buffer_data, 8, block);
	pWorld->ApplyAttribute(buffer_data, 4, block);

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 16, 1, block, buffer_data);
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 8, 1, block, buffer_data);
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 4, 1, block, buffer_data);
	}*/
}

void ImperialFortressInstance::BlockReleaseZone(Monster* pMonster)
{
	coord_type coords[4];
	coord_type x = pMonster->GetX();
	coord_type y = pMonster->GetY();

	switch (pMonster->GetDirection())
	{
	case 1:
		{
			coords[0] = x - 2;
			coords[1] = y;
			coords[2] = x + 2;
			coords[3] = y + 3;
		} break;
	
	case 3:
		{
			coords[0] = x - 3;
			coords[1] = y - 2;
			coords[2] = x;
			coords[3] = y + 2;
		} break;

	case 5:
		{
			coords[0] = x - 2;
			coords[1] = y - 3;
			coords[2] = x + 2;
			coords[3] = y;
		} break;

	case 7:
		{
			coords[0] = x;
			coords[1] = y - 2;
			coords[2] = x + 3;
			coords[3] = y + 2;
		} break;
	}

	ImperialFortressReleaseZone* add_coord = new ImperialFortressReleaseZone;
	add_coord->SetX1(coords[0]);
	add_coord->SetY1(coords[1]);
	add_coord->SetX2(coords[2]);
	add_coord->SetY2(coords[3]);

	this->m_ImperialFortressReleaseZoneList.push_back(add_coord);

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 16, 1, false, coords);
	}
}

void ImperialFortressInstance::MoveOut()
{
	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if (pPlayer)
		{
			if (sGameServer->IsImperialFortressMoveToPrevious())
			{
				pPlayer->TeleportToLocation(this->GetPlayer(i)->GetWorld(), this->GetPlayer(i)->GetX(), this->GetPlayer(i)->GetY(), -1, -1);
			}
			else
			{
				pPlayer->TeleportToGate(508);
			}

			sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Moved player out %s", __FUNCTION__, this->GetDay(), this->GetZone(), pPlayer->BuildLog().c_str());
		}

		this->GetPlayer(i)->Reset();
	}
}

void ImperialFortressInstance::SendTime()
{
	uint8 state = this->GetState() == IMPERIAL_FORTRESS_STATE_END ? 0 :
		this->GetState() == IMPERIAL_FORTRESS_STATE_STANDBY ? 0 :
		this->GetState() == IMPERIAL_FORTRESS_STATE_NONE ? 1 : 2;

	IMPERIAL_FORTRESS_TIMER pMsg(state, this->GetTime()->GetRemain(), this->GetMonsterCount());
	pMsg.day = this->GetDay() + 1;
	pMsg.zone = this->GetZone() + 1;

	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if (pPlayer)
		{
			pPlayer->sendPacket(MAKE_PCT(pMsg));
		}
	}
}

void ImperialFortressInstance::SendFail()
{
	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		this->SendFail(this->GetPlayer(i)->GetPlayer());
	}
}

void ImperialFortressInstance::SendFail(Player* pPlayer)
{
	if (!pPlayer)
		return;

	IMPERIAL_FORTRESS_END pMsg(0, 0);
	pPlayer->SEND_PCT(pMsg);
}

void ImperialFortressInstance::SendSuccess()
{
	ImperialFortressLevel const* pLevelData = sImperialFortressMgr->GetLevel(this->GetZoneLevel());

	if (!pLevelData)
	{
		return;
	}

	IMPERIAL_FORTRESS_END pMsg(2, pLevelData->experience.get());

	for (uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if (!pPlayer)
			continue;

		int64 experience = pLevelData->experience.get();

		pPlayer->GiveExperience(experience, true, true);
		pPlayer->UpdateStatistic(STATISTIC_IMPERIAL_FORTRESS_POINTS, experience / 100);

		pPlayer->sendPacket(MAKE_PCT(pMsg));

		sEventMgr->GiveContributionReward(pPlayer, EVENT_IMPERIAL_FORTRESS, pLevelData->guid.get() + 1, 1, GremoryCaseReward::IMPERIAL_FORTRESS);

		sLog->outInfo("imperial_fortress", "%s(Day:%u / Zone:%u) -- Gain experience %I64d / %s", __FUNCTION__, this->GetDay(), this->GetZone(), experience, pPlayer->BuildLog().c_str());
	}
}

ImperialFortressPlayer* ImperialFortressInstance::FindPlayer(Player* pPlayer)
{
	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetPlayer(i)->GetGUID() == pPlayer->GetGUID())
		{
			return this->GetPlayer(i);
		}
	}

	return nullptr;
}

bool ImperialFortressInstance::IsZoneEmpty() const
{
	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (this->GetPlayer(i)->GetGUID())
		{
			return false;
		}
	}

	return true;
}

bool ImperialFortressInstance::IsZoneReleased(coord_type x, coord_type y) const
{
	for (ImperialFortressReleaseZoneList::const_iterator itr = this->m_ImperialFortressReleaseZoneList.begin(); itr != this->m_ImperialFortressReleaseZoneList.end(); ++itr)
	{
		ImperialFortressReleaseZone const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if ((x >= pData->GetX1() && x <= pData->GetX2()) &&
			(y >= pData->GetY1() && y <= pData->GetY2()))
		{
			return true;
		}
	}

	return false;
}