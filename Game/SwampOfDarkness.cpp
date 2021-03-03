SwampOfDarkness::SwampOfDarkness()
{
	this->ChangeState(SWAMP_OF_DARKNESS_STATE_NONE);
}

SwampOfDarkness::~SwampOfDarkness()
{

}

void SwampOfDarkness::ChangeState(SwampOfDarknessState state)
{
	this->SetState(state);

	switch (this->GetState())
	{
	case SWAMP_OF_DARKNESS_STATE_NONE:
		{
			this->SetState_None();
		} break;

	case SWAMP_OF_DARKNESS_STATE_OPEN:
		{
			this->SetState_Open();
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_1:
		{
			this->SetState_Battle_1();
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_2:
		{
			this->SetState_Battle_2();
		} break;

	case SWAMP_OF_DARKNESS_STATE_END:
		{
			this->SetState_End();
		} break;
	}

	this->SendState();
}

void SwampOfDarkness::Update()
{
	if (!sGameServer->IsSwampOfDarknessEnabled())
	{
		return;
	}

	switch (this->GetState())
	{
	case SWAMP_OF_DARKNESS_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case SWAMP_OF_DARKNESS_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_1:
		{
			this->ProcState_Battle_1();
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_2:
		{
			this->ProcState_Battle_2();
		} break;

	case SWAMP_OF_DARKNESS_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void SwampOfDarkness::SetState_None()
{
	this->GetTime()->Start(sGameServer->GetRandomSwampOfDarknessClosedTime());
	this->GetSecondTimer()->Start();
	this->SetWaterMonsterX(0);
	this->SetWaterMonsterY(0);

	sLog->outInfo("swamp_of_darkness", "%s", __FUNCTION__);
}

void SwampOfDarkness::SetState_Open()
{
	sLog->outInfo("swamp_of_darkness", "%s", __FUNCTION__);
}

void SwampOfDarkness::SetState_Battle_1()
{
	this->GetTime()->Start(sGameServer->GetSwampOfDarknessBossAppearTime());
	this->GetSecondTimer()->Start();

	this->CreateBattle1();

	sLog->outInfo("swamp_of_darkness", "%s", __FUNCTION__);
}

void SwampOfDarkness::SetState_Battle_2()
{
	this->GetTime()->Start(sGameServer->GetSwampOfDarknessBattleTime());
	this->GetSecondTimer()->Start();

	this->CreateBattle2();

	sLog->outInfo("swamp_of_darkness", "%s", __FUNCTION__);
}

void SwampOfDarkness::SetState_End()
{
	this->GetTime()->Start(5 * MINUTE * IN_MILLISECONDS);
	this->GetSecondTimer()->Start();

	sLog->outInfo("swamp_of_darkness", "%s", __FUNCTION__);
}

void SwampOfDarkness::ProcState_None()
{
	if (this->GetTime()->Elapsed())
	{
		this->ChangeState(SWAMP_OF_DARKNESS_STATE_OPEN);
	}
}

void SwampOfDarkness::ProcState_Open()
{
	if (this->GetSecondTimer()->Elapsed(IN_MILLISECONDS))
	{
		if (this->GetPlayerCount() > 0)
		{
			this->ChangeState(SWAMP_OF_DARKNESS_STATE_BATTLE_1);
		}
	}
}

void SwampOfDarkness::ProcState_Battle_1()
{
	if (this->GetTime()->Elapsed())
	{
		this->ChangeState(SWAMP_OF_DARKNESS_STATE_BATTLE_2);
	}
}

void SwampOfDarkness::ProcState_Battle_2()
{
	if (this->GetSecondTimer()->Elapsed(IN_MILLISECONDS))
	{
		this->SendTime();
	}

	if (this->GetTime()->Elapsed())
	{
		this->MovePlayersOut();

		this->ChangeState(SWAMP_OF_DARKNESS_STATE_END);
	}
}

void SwampOfDarkness::ProcState_End()
{
	if (this->GetTime()->Elapsed())
	{
		this->MovePlayersOut();

		this->ChangeState(SWAMP_OF_DARKNESS_STATE_NONE);
	}
}

void SwampOfDarkness::CreateBattle1()
{
	Monster* pMonster = sObjectMgr->MonsterTryAdd(790, WORLD_SWAMP_OF_DARKNESS);

	if (pMonster)
	{
		pMonster->SetWorldId(WORLD_SWAMP_OF_DARKNESS);
		pMonster->SetBasicLocation(183, 16, 185, 20);
		pMonster->SetDirection(1);
		pMonster->SetMoveDistance(50);
		pMonster->SetSpawnTime(1 * IN_MILLISECONDS);
		pMonster->SetSpawnTick(GetTickCount());
		pMonster->SetRespawn(IN_MILLISECONDS);
		pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pMonster->SetDespawnTick(GetTickCount());
		pMonster->AddToWorld();
	}

	Monster* pWaterMonster = sObjectMgr->MonsterTryAdd(793, WORLD_SWAMP_OF_DARKNESS);

	if (pWaterMonster)
	{
		pWaterMonster->SetWorldId(WORLD_SWAMP_OF_DARKNESS);
		pWaterMonster->SetBasicLocation(183, 16, 185, 20);
		pWaterMonster->SetDirection(1);
		pWaterMonster->SetMoveDistance(50);
		pWaterMonster->SetSpawnTime(1 * IN_MILLISECONDS);
		pWaterMonster->SetSpawnTick(GetTickCount());
		pWaterMonster->SetRespawn(IN_MILLISECONDS);
		pWaterMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		pWaterMonster->SetDespawnTick(GetTickCount());
		pWaterMonster->AddToWorld();

		sSwampOfDarkness->SendWaterMonster(793);
	}
}

void SwampOfDarkness::CreateBattle2()
{
	Monster* pMonster = sObjectMgr->MonsterTryAdd(794, WORLD_SWAMP_OF_DARKNESS);

	if (!pMonster)
	{
		this->ChangeState(SWAMP_OF_DARKNESS_STATE_END);
		return;
	}

	pMonster->SetWorldId(WORLD_SWAMP_OF_DARKNESS);
	pMonster->SetBasicLocation(this->GetWaterMonsterX(), this->GetWaterMonsterY(), this->GetWaterMonsterX(), this->GetWaterMonsterY());
	pMonster->SetDirection(1);
	pMonster->SetMoveDistance(50);
	pMonster->SetSpawnTime(1 * IN_MILLISECONDS);
	pMonster->SetSpawnTick(GetTickCount());
	pMonster->SetRespawn(IN_MILLISECONDS);
	pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	pMonster->SetDespawnTick(GetTickCount());
	pMonster->AddToWorld();

	this->SendWaterMonster(794);
}

void SwampOfDarkness::MovePlayersOut()
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (!this->IsBossZone(pPlayer))
		{
			continue;
		}

		pPlayer->TeleportToLocation(WORLD_SWAMP_OF_DARKNESS);
	}
}

bool SwampOfDarkness::IsBossZone(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return false;
	}

	if (pPlayer->GetWorldId() != WORLD_SWAMP_OF_DARKNESS)
	{
		return false;
	}

	if ((pPlayer->GetX() >= g_SwampOfDarkness[0].x && pPlayer->GetX() <= g_SwampOfDarkness[1].x) &&
		(pPlayer->GetY() >= g_SwampOfDarkness[0].y && pPlayer->GetY() <= g_SwampOfDarkness[1].y))
	{
		return true;
	}

	return false;
}

bool SwampOfDarkness::IsBossZone(uint16 world, int16 x1, int16 y1, int16 x2, int16 y2) const
{
	if (world != WORLD_SWAMP_OF_DARKNESS)
	{
		return false;
	}

	if ((x1 >= g_SwampOfDarkness[0].x && x1 <= g_SwampOfDarkness[1].x) &&
		(y1 >= g_SwampOfDarkness[0].y && y1 <= g_SwampOfDarkness[1].y))
	{
		return true;
	}

	if ((x2 >= g_SwampOfDarkness[0].x && x2 <= g_SwampOfDarkness[1].x) &&
		(y2 >= g_SwampOfDarkness[0].y && y2 <= g_SwampOfDarkness[1].y))
	{
		return true;
	}

	return false;
}

int32 SwampOfDarkness::GetPlayerCount() const
{
	int32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (!this->IsBossZone(pPlayer))
		{
			continue;
		}

		++count;
	}

	return count;
}

bool SwampOfDarkness::IsAllowedToEnter(Player* pPlayer) const
{
	return this->GetPlayerCount() < sGameServer->GetSwampOfDarknessMaxPlayers();
}

void SwampOfDarkness::SendPacket(uint8 * Packet, uint16 size)
{
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr)
	{
		Player* pPlayer = itr->second;

		if (!pPlayer)
		{
			continue;
		}

		if (!this->IsBossZone(pPlayer))
		{
			continue;
		}

		pPlayer->sendPacket(Packet, size);
	}
}

void SwampOfDarkness::SendTime()
{
	SWAMP_OF_DARKNESS_BOSS_TIMER pMsg;
	pMsg.time = this->GetTime()->GetRemain();

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void SwampOfDarkness::SendState(Player* pPlayer)
{
	if (pPlayer && pPlayer->GetWorldId() != WORLD_SWAMP_OF_DARKNESS)
	{
		return;
	}

	SWAMP_OF_DARKNESS_BOSS_AREA pMsg;

	switch (this->GetState())
	{
	case SWAMP_OF_DARKNESS_STATE_NONE:
	case SWAMP_OF_DARKNESS_STATE_OPEN:
	case SWAMP_OF_DARKNESS_STATE_END:
		{
			pMsg.state = 3;
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_1:
		{
			pMsg.state = 4;
		} break;

	case SWAMP_OF_DARKNESS_STATE_BATTLE_2:
		{
			pMsg.state = 0;
		} break;
	}

	if (!pPlayer)
	{
		sObjectMgr->sendPacketWorld(WORLD_SWAMP_OF_DARKNESS, (uint8*)&pMsg, pMsg.h.get_size());
	}
	else
	{
		pPlayer->SEND_PCT(pMsg);
	}
}

void SwampOfDarkness::SendAttack(Monster* pMonster, uint8 id)
{
	if (!pMonster)
	{
		return;
	}

	SWAMP_OF_DARKNESS_BOSS_ATTACK pMsg;
	pMsg.monster = pMonster->GetEntry();
	pMsg.id = id;

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void SwampOfDarkness::SendWaterMonster(uint16 id)
{
	SWAMP_OF_DARKNESS_BOSS_WATER_MONSTER pMsg;
	pMsg.id = id;

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}