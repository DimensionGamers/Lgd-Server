/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BloodCastle.cpp"
*
*/

BloodCastle::BloodCastle(): EventGroundCommon("BloodCastle", LOG_BLOOD_CASTLE, BC_MAP_RANGE, EVENT_BLOOD_CASTLE)
{
}

BloodCastle::~BloodCastle()
{
}

void BloodCastle::Update()
{
	switch(this->GetState())
	{
	case BLOOD_CASTLE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case BLOOD_CASTLE_STATE_OPEN:
		{
			this->ProcState_Open();
		} break;

	case BLOOD_CASTLE_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case BLOOD_CASTLE_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case BLOOD_CASTLE_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

void BloodCastle::SetState_None()
{
	this->SetState(BLOOD_CASTLE_STATE_NONE);
	this->SetSubState(BLOOD_CASTLE_SUBSTATE_NONE);
	this->MovePlayersOut(BLOOD_CASTLE_GATE);
	this->InitMessageSent();
	this->SetDoorKilled(false);
	this->SetStatueKilled(false);
	this->SetDuration(0);
	this->RunTime(0);
	this->SetMonsterMax(-1);
	this->SetMonsterCurrent(-1);
	this->SetWeaponOwner(nullptr);
	this->ResetWeaponOwnerName();
	this->SetWeaponOwnerX(-1);
	this->SetWeaponOwnerY(-1);
	this->SetWeaponLevel(0);
	this->GetDoorOpenTime()->Reset();
	this->ClearMonster();
	
	this->ReleaseBlockEntrance(true);
	this->ReleaseBlockBridge(true);
	this->ReleaseBlockDoor(true);
	this->ReleaseBlockBossZone(true);
	this->ReleaseBlockAltar(true);
	
	LOG_EVENT_STATE();
}

void BloodCastle::SetState_Open()
{
	this->InitPlayers<BloodCastlePlayer>(this->GetSettings()->GetMaxPlayers());

	this->SetState(BLOOD_CASTLE_STATE_OPEN);
	this->RunTime(0);
	this->AddMonster(BLOOD_CASTLE_MONSTER_ARCHANGEL);

	LOG_EVENT_STATE();
}

void BloodCastle::SetState_StandBy()
{
	this->SetState(BLOOD_CASTLE_STATE_STANDBY);
	this->RunTime(BLOOD_CASTLE_STANDBY_TIME * MINUTE * IN_MILLISECONDS);
	this->SendStandByStart();

	LOG_EVENT_STATE();
}

void BloodCastle::SetState_Playing()
{
	if ( this->GetPlayersCount(true) < 1 )
	{
		this->SetState_None();
		return;
	}

	this->SetState(BLOOD_CASTLE_STATE_PLAYING);
	this->SetSubState(BLOOD_CASTLE_SUBSTATE_MONSTER_KILL);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);
	this->SendStart();
	this->AddMonster(BLOOD_CASTLE_MONSTER_BRIDGE);
	this->AddMonster(BLOOD_CASTLE_MONSTER_DOOR);
	this->ReleaseBlockEntrance(false);
	this->UpdateStatus(BLOOD_CASTLE_STATUS_GO);
	this->CalculateMonsterKill();

	LOG_EVENT_STATE();
}

void BloodCastle::SetState_Playend()
{
	this->SetState(BLOOD_CASTLE_STATE_PLAYEND);
	this->RunTime(BLOOD_CASTLE_END_TIME * MINUTE * IN_MILLISECONDS);
	this->ClearMonster();
	this->UpdateStatus(BLOOD_CASTLE_STATUS_END);

	LOG_EVENT_STATE();
}

void BloodCastle::ProcState_None()
{

}

void BloodCastle::ProcState_Open()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer(false);
}

void BloodCastle::ProcState_StandBy()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer();

	this->SendMessageID(EVENT_MESSAGE_START_BLOOD_CASTLE);

	if ( this->TimePassed() )
		this->SetState_Playing();
}

void BloodCastle::ProcState_Playing()
{
	if ( this->SecondPassed() )
	{
		this->UpdatePlayerStatus();

		int32 current_players = this->GetPlayersCount(true);

		if ( current_players < 1 )
		{
			this->SetState_None();
			return;
		}

		switch ( this->GetSubState() )
		{
		case BLOOD_CASTLE_SUBSTATE_MONSTER_KILL:
			{
				this->ProcSubState_MonsterKill();
			} break;

		case BLOOD_CASTLE_SUBSTATE_DOOR_STANDBY:
			{
				this->ProcSubState_DoorStandBy();
			} break;

		case BLOOD_CASTLE_SUBSTATE_DOOR_KILL:
			{
				this->ProcSubState_DoorKill();
			} break;

		case BLOOD_CASTLE_SUBSTATE_BOSS_KILL:
			{
				this->ProcSubState_BossKill();
			} break;

		case BLOOD_CASTLE_SUBSTATE_STATUE_KILL:
			{
				this->ProcSubState_StatueKill();
			} break;

		case BLOOD_CASTLE_SUBSTATE_DELIVER_WEAPON:
			{
				this->ProcSubState_DeliverWeapon();
			} break;
		}

		if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_MONSTER_KILL ||
			 this->GetSubState() == BLOOD_CASTLE_SUBSTATE_DOOR_STANDBY ||
			 this->GetSubState() == BLOOD_CASTLE_SUBSTATE_DOOR_KILL )
		{
			this->UpdateStatus(BLOOD_CASTLE_STATUS_MONSTER_KILL);
		}
		else
		{
			this->UpdateStatus(BLOOD_CASTLE_STATUS_MAGIC_SKELETON);
		}

		if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_WEAPON_DELIVERED )
		{
			this->UpdateStatus(BLOOD_CASTLE_STATUS_MAGIC_SKELETON);

			this->SendMessageCompleted();
			this->SendReward();
			this->SetSubState(BLOOD_CASTLE_SUBSTATE_COMPLETE);
			this->SetState_Playend();
		}
	}
	
	this->SendMessageID(EVENT_MESSAGE_END_BLOOD_CASTLE);

	if ( this->TimePassed() )
	{
		this->SendFail();
		this->SetState_Playend();
	}
}

void BloodCastle::ProcState_Playend()
{
	if ( this->SecondPassed() )
		this->UpdatePlayer();

	this->SendMessageID(EVENT_MESSAGE_CLEAR_BLOOD_CASTLE);

	if ( this->TimePassed() )
		this->SetState_None();
}

void BloodCastle::ProcSubState_MonsterKill()
{
	if ( this->GetMonsterCurrent() < this->GetMonsterMax() )
	{
		return;
	}

	this->SetMonsterMax(-1);
	this->UpdateStatus(BLOOD_CASTLE_STATUS_UNK2);
	this->SetSubState(BLOOD_CASTLE_SUBSTATE_DOOR_STANDBY);
	this->GetDoorOpenTime()->Start();

	this->SendMonsterTerminated();

	this->Log("Killed Bridge Monsters");
}

void BloodCastle::ProcSubState_DoorStandBy()
{
	if ( !this->GetDoorOpenTime()->Elapsed(3000) )
	{
		return;
	}

	this->SetSubState(BLOOD_CASTLE_SUBSTATE_DOOR_KILL);

	this->ReleaseBlockBridge(false);
}
	
void BloodCastle::ProcSubState_DoorKill()
{
	if ( !this->IsDoorKilled() )
	{
		return;
	}

	this->SetSubState(BLOOD_CASTLE_SUBSTATE_BOSS_KILL);
	this->CalculateMonsterKill();
	this->AddMonster(BLOOD_CASTLE_MONSTER_MISC);
	this->AddMonster(BLOOD_CASTLE_MONSTER_BOSS);
	this->ReleaseBlockDoor(false);
	this->ReleaseBlockAltar(false);
	this->ReleaseBlockBossZone(false);
}
	
void BloodCastle::ProcSubState_BossKill()
{
	if ( this->GetMonsterCurrent() < this->GetMonsterMax() )
	{
		return;
	}

	this->SetMonsterMax(-1);
	this->SetSubState(BLOOD_CASTLE_SUBSTATE_STATUE_KILL);
	this->AddMonster(BLOOD_CASTLE_MONSTER_STATUE);
	this->SendBossTerminated();

	this->Log("Killed Boss");
}
	
void BloodCastle::ProcSubState_StatueKill()
{
	if ( !this->IsStatueKilled() )
	{
		return;
	}

	this->SetSubState(BLOOD_CASTLE_SUBSTATE_DELIVER_WEAPON);	
}
	
void BloodCastle::ProcSubState_DeliverWeapon()
{

}

void BloodCastle::UpdatePlayerStatus()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		{
			this->Log("Removed Player %s : Disconnected.", pPlayer->GetName());

			this->GetPlayer(i)->SetPlayer(nullptr);
			continue;
		}

		if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			continue;

		if ( !pPlayer->IsLive() )
		{
			this->Log("Removed Player %s : Died.", pPlayer->GetName());

			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, sMessage->Get(MSG_BLOOD_CASTLE_INFILTRATION_FAILED).c_str());
			this->SendFail(pPlayer, true);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( !BC_MAP_RANGE(pPlayer->GetWorldId())  )
		{
			this->Log("Removed Player %s : Not in event.", pPlayer->GetName());

			pPlayer->SendNotice(NOTICE_NORMAL_BLUE, sMessage->Get(MSG_BLOOD_CASTLE_INFILTRATION_FAILED).c_str());
			this->SendFail(pPlayer);
			pPlayer->EventResetData();
			this->GetPlayer(i)->Reset();
			continue;
		}

		if ( this->GetPlayer(i)->IsReconnected() )
		{
			if ( this->GetPlayer(i)->GetReconnectedTime()->Elapsed(5000) )
			{
				this->GetPlayer(i)->SetReconnected(false);
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "BloodCastle zone unlocked.");
				this->SendWorldStatus(pPlayer);
			}
		}
	}

	this->UpdateWeaponStatus();
}

void BloodCastle::UpdateStatus(uint8 status)
{
	BLOOD_CASTLE_STATE pMsg(status);

	if ( status != BLOOD_CASTLE_STATUS_UNK2 )
	{
		pMsg.remain_seconds = this->GetRemain() / IN_MILLISECONDS;
		pMsg.monsters_max = this->GetMonsterMax();
		pMsg.monsters_current = this->GetMonsterCurrent();
		pMsg.weapon_user = ENTRY(this->GetWeaponOwner());
		pMsg.weapon_type = this->GetWeaponLevel() + 1;
	}
	else
	{
		pMsg.remain_seconds = 0;
		pMsg.monsters_max = 0;
		pMsg.monsters_current = 0;
		pMsg.weapon_user = 0;
		pMsg.weapon_type = -1;
	}

	this->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void BloodCastle::SendStandByStart()
{
	int32 remain = this->GetRemain() / IN_MILLISECONDS;

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, sMessage->Get(MSG_BLOOD_CASTLE_STAND_BY_START).c_str(), this->GetGround() + 1, remain);
		pPlayer->CommandSend(1, 45, 0);
	}
}

void BloodCastle::SendStart()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_STARTED).c_str(), this->GetGround()+1);
	}
}

void BloodCastle::SendMonsterTerminated()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_MONSTER_ELIMINATED).c_str());
	}
}

void BloodCastle::SendBossTerminated()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_BOSS_ELIMINATED).c_str());
	}
}

void BloodCastle::ProcessMonsterKilled(Player* pKiller, bool boss)
{
	if ( boss )
	{
		if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_BOSS_KILL )
		{
			this->MonsterAddCount();
		}
	}
	else
	{
		if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_MONSTER_KILL )
		{
			this->MonsterAddCount();
		}
	}

	if (!pKiller)
	{
		return;
	}

	for (uint32 i = 0; i < this->GetPlayerSize(); ++i)
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if (!pPlayer)
		{
			continue;
		}

		if (!IN_RANGE(pPlayer, pKiller, MAX_PARTY_MONSTER_KILL_DISTANCE))
		{
			continue;
		}

		if (pKiller == pPlayer)
		{
			this->GetPlayer(i)->ToBC()->IncreaseKillScore(1);
		}
		else if (pKiller && pKiller->GetPartyID() != PARTY_NULL && pKiller->GetPartyID() == pPlayer->GetPartyID())
		{
			this->GetPlayer(i)->ToBC()->IncreaseKillScore(1);
		}
	}
}

void BloodCastle::ProcessDoorDestroyed(Player* pKiller, Monster* pGate)
{
	if ( this->IsDoorKilled() )
	{
		return;
	}

	this->SetDoorKilled(true);

	pKiller ? this->Log("Door has been destroyed by %s", pKiller->GetName()) : this->Log("Door has been destroyed");
	
	!pKiller ? this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_GATE_DESTROYED_1).c_str()):
			   this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_GATE_DESTROYED_2).c_str(), pKiller->GetName());

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		int32 bonus = this->GetSettings()->GetBonusDoorCleared();

		if ( pKiller == pPlayer )
		{
			bonus = this->GetSettings()->GetBonusDoorCleared();
			this->GetPlayer(i)->ToBC()->SetDoorDestroyed(true);
		}
		else if ( pKiller && pKiller->GetPartyID() != PARTY_NULL && pKiller->GetPartyID() == pPlayer->GetPartyID() )
		{
			bonus = this->GetSettings()->GetBonusDoorCleared();
		}
		else
		{
			bonus /= 2;
		}

		this->GetPlayer(i)->ToBC()->IncreaseExperience(bonus);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[ Blood Castle ] Door Destroy Bonus: %d", bonus);
		pPlayer->SetEventScore(this->GetSettings()->GetScoreSurvive());
	}

	if (pKiller)
	{
		pKiller->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_BLOOD_CASTLE_GATE_DESTROY);
	}
}

void BloodCastle::ProcessStatueDestroyed(Player* pKiller, Monster* pStatue)
{
	if ( this->IsStatueKilled() )
	{
		return;
	}

	this->SetStatueKilled(true);

	pKiller ? this->Log("Statue has been destroyed by %s", pKiller->GetName()) : this->Log("Statue has been destroyed");

	this->SetWeaponLevel(pStatue->GetClass() - 132);

	Item item(ITEMGET(13, 19), this->GetWeaponLevel());
	item.bc_weapon.event_id = EVENT_BLOOD_CASTLE;
	item.bc_weapon.event_ground = this->GetGround();

	sItemMgr->ItemSerialCreate(pStatue->GetMaxAttacker(), pStatue->GetWorldId(), pStatue->GetX(), pStatue->GetY(), item, false, true, true);
	
	!pKiller ? this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_STATUE_DESTROYED_1).c_str()):
			   this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_STATUE_DESTROYED_2).c_str(), pKiller->GetName());

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		int32 bonus = this->GetSettings()->GetBonusStatueCleared();

		if ( pKiller == pPlayer )
		{
			bonus = this->GetSettings()->GetBonusStatueCleared();
			this->GetPlayer(i)->ToBC()->SetReward(1);
			this->GetPlayer(i)->ToBC()->SetStatueDestroyed(true);
		}
		else if ( pKiller && pKiller->GetPartyID() != PARTY_NULL && pKiller->GetPartyID() == pPlayer->GetPartyID() )
		{
			bonus = this->GetSettings()->GetBonusStatueCleared();
			this->GetPlayer(i)->ToBC()->SetReward(2);
		}
		else
		{
			bonus /= 2;
		}

		this->GetPlayer(i)->ToBC()->IncreaseExperience(bonus);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[ Blood Castle ] Statue Destroy Bonus: %d", bonus);
		pPlayer->SetEventScore(this->GetSettings()->GetScoreSurvive());
	}
}

void BloodCastle::AddMonster(BloodCastleMonsterType type)
{
	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_BLOOD_CASTLE);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (event_monster->blood_castle.ground != GetGround())
			continue;

		if (event_monster->blood_castle.type != type)
			continue;

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
			monster->SetRespawn(IN_MILLISECONDS);

			switch (type)
			{
			case BLOOD_CASTLE_MONSTER_DOOR:
				monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				break;

			case BLOOD_CASTLE_MONSTER_STATUE:
				monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				monster->SetClass(132 + Random(3));
				break;
			}

			monster->SetEventGround(GetGround());
			monster->AddToWorld();

			this->Log("Added Monster [%u][%u].", monster->GetEntry(), monster->GetClass());
		}
	}
}

void BloodCastle::ClearMonster()
{
	HASH_SUMMON(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if ( !pMonster || !pMonster->IsPlaying() )
			continue;

		if ( pMonster->GetEventId() != EVENT_BLOOD_CASTLE )
			continue;

		if ( pMonster->GetEventGround() != this->GetGround() )
			continue;

		pMonster->Remove();
	}

	this->Log("Clearing Monsters.");
}

void BloodCastle::ReleaseBlockEntrance(bool block)
{
	World * pWorld = sWorldMgr->GetWorld(m_BloodCastleData[this->GetGround()].world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(m_BloodCastleZoneEntrance, 4, block);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 4, 1, block, m_BloodCastleZoneEntrance);
	}
}
	
void BloodCastle::ReleaseBlockBridge(bool block)
{
	World * pWorld = sWorldMgr->GetWorld(m_BloodCastleData[this->GetGround()].world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(m_BloodCastleZoneBridge, 8, block);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 8, 1, block, m_BloodCastleZoneBridge);
	}
}
	
void BloodCastle::ReleaseBlockDoor(bool block)
{
	World * pWorld = sWorldMgr->GetWorld(m_BloodCastleData[this->GetGround()].world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(m_BloodCastleZoneDoor, 4, block);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 4, 1, block, m_BloodCastleZoneDoor);
	}
}
	
void BloodCastle::ReleaseBlockBossZone(bool block)
{
	World * pWorld = sWorldMgr->GetWorld(m_BloodCastleData[this->GetGround()].world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(m_BloodCastleZoneBossZone, 4, block);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 4, 1, block, m_BloodCastleZoneBossZone);
	}
}
	
void BloodCastle::ReleaseBlockAltar(bool block)
{
	World * pWorld = sWorldMgr->GetWorld(m_BloodCastleData[this->GetGround()].world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->ApplyAttribute(m_BloodCastleZoneAltar, 4, block);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendWorldAttribute(0, 4, 1, block, m_BloodCastleZoneAltar);
	}
}

void BloodCastle::CalculateMonsterKill()
{
	this->SetMonsterMax(0);
	this->SetMonsterCurrent(0);

	if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_MONSTER_KILL )
	{
		this->SetMonsterMax(this->GetPlayersCount(false) * this->GetSettings()->GetMonsterPerUser());

		if ( this->GetSettings()->GetMonsterLimit() != -1 )
			this->LimitMaxMonsterMax(this->GetSettings()->GetMonsterLimit());
	}
	else if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_BOSS_KILL )
	{
		this->SetMonsterMax(this->GetPlayersCount(false) * this->GetSettings()->GetBossPerUser());

		if ( this->GetSettings()->GetBossLimit() != -1 )
			this->LimitMaxMonsterMax(this->GetSettings()->GetBossLimit());
	}
}

void BloodCastle::AddDeliverWeaponExperience()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		pPlayer->SetEventScore(this->GetSettings()->GetScoreSurvive());

		if ( this->GetPlayer(i)->ToBC()->IsComplete() )
		{
			this->GetPlayer(i)->ToBC()->IncreaseExperience(this->GetSettings()->GetBonusWeaponDelivered());

			if ( this->GetPlayer(i)->ToBC()->GetReward() == 1 )
			{
				pPlayer->SetEventScore(this->GetSettings()->GetScoreQuestFinished());
			}
			else if ( this->GetPlayer(i)->ToBC()->GetReward() == 2 )
			{
				pPlayer->SetEventScore(this->GetSettings()->GetScoreQuestFinishedParty());
			}
		}
	}
}

void BloodCastle::AddRemainTimeExperience()
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		int32 bonus = (this->GetRemain() / IN_MILLISECONDS) * this->GetSettings()->GetBonusRemainTime();

		this->GetPlayer(i)->ToBC()->IncreaseExperience(bonus);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[ Blood Castle ] Remaining Time Bonus: %d", bonus);
	}
}

bool BloodCastle::CheckPlayerComplete(Player* mPlayer)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( !this->GetPlayer(i)->GetPlayer() || this->GetPlayer(i)->GetPlayer() != mPlayer )
			continue;

		return this->GetPlayer(i)->ToBC()->IsComplete();
	}

	return false;
}

bool BloodCastle::CanGetReward(Player* mPlayer)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( !this->GetPlayer(i)->GetPlayer() || this->GetPlayer(i)->GetPlayer() != mPlayer )
			continue;

		return this->GetPlayer(i)->ToBC()->GetReward() != 0;
	}

	return false;
}

void BloodCastle::SetPlayerComplete(Player* mPlayer)
{
	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		if ( !this->GetPlayer(i)->GetPlayer() || this->GetPlayer(i)->GetPlayer() != mPlayer )
			continue;

		this->GetPlayer(i)->ToBC()->SetComplete(true);
	}
}

void BloodCastle::SendFail(Player* mPlayer, bool die)
{
	BLOOD_CASTLE_SCORE pMsg(0);
	memcpy(pMsg.data.name, mPlayer->GetName(), MAX_CHARACTER_LENGTH);
	pMsg.data.score = (die)?this->GetSettings()->GetScoreDie():this->GetSettings()->GetScoreQuestFail();
	pMsg.data.exp = 0;
	pMsg.data.zen = (die) ? this->GetSettings()->GetZenRewardDie(): 0;

	if ( die )
	{
		for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
		{
			Player* pPlayer = this->GetPlayer(i)->GetPlayer();

			if ( !pPlayer )
				continue;

			if ( pPlayer != mPlayer )
				continue;

			pMsg.data.exp = this->GetPlayer(i)->ToBC()->GetExperience();

			int64 experience = this->GetPlayer(i)->ToBC()->GetExperience();

			pPlayer->GiveExperience(experience, true, true);
			break;
		}
	}

	mPlayer->MoneyAdd(pMsg.data.zen);
	mPlayer->sendPacket(MAKE_PCT(pMsg));

	BLOOD_CASTLE_STATE pMsg2(2);
	pMsg2.remain_seconds = 0;
	pMsg2.monsters_max = 0;
	pMsg2.monsters_current = 0;
	pMsg2.weapon_user = 0;
	pMsg2.weapon_type = -1;

	mPlayer->SEND_PCT(pMsg2);
}
	
void BloodCastle::SendFail()
{
	BLOOD_CASTLE_SCORE pMsg(0);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		int64 experience = static_cast<int64>(this->GetPlayer(i)->ToBC()->GetExperience());

		pPlayer->GiveExperience(experience, true, true);

		memcpy(pMsg.data.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		pMsg.data.score = this->GetSettings()->GetScoreQuestFail();
		pMsg.data.exp = this->GetPlayer(i)->ToBC()->GetExperience();
		pMsg.data.zen = 0;

		pPlayer->sendPacket(MAKE_PCT(pMsg));

		BLOOD_CASTLE_STATE pMsg2(2);
		pMsg2.remain_seconds = 0;
		pMsg2.monsters_max = 0;
		pMsg2.monsters_current = 0;
		pMsg2.weapon_user = 0;
		pMsg2.weapon_type = -1;

		pPlayer->SEND_PCT(pMsg2);

		if ( !this->GetPlayer(i)->ToBC()->IsComplete() )
		{
			if ( this->GetPlayer(i)->ToBC()->IsDoorDestroyed() || this->GetPlayer(i)->ToBC()->IsStatueDestroyed() )
			{
				sEventMgr->GiveContributionReward(pPlayer, EVENT_BLOOD_CASTLE, this->GetGround() + 1, 3, GremoryCaseReward::BLOOD_CASTLE);
			}
			else if (sGameServer->GetBloodCastleRewardScore() > 0 && this->GetPlayer(i)->ToBC()->GetKillScore() >= sGameServer->GetBloodCastleRewardScore())
			{
				sEventMgr->GiveContributionReward(pPlayer, EVENT_BLOOD_CASTLE, this->GetGround() + 1, 3, GremoryCaseReward::BLOOD_CASTLE);
			}
		}
	}
}

void BloodCastle::SendReward()
{
	if ( this->GetSubState() == BLOOD_CASTLE_SUBSTATE_COMPLETE )
	{
		return;
	}

	this->AddDeliverWeaponExperience();
	this->AddRemainTimeExperience();

	BLOOD_CASTLE_SCORE pMsg(1);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
		{
			continue;
		}

		int64 experience = static_cast<int64>(this->GetPlayer(i)->ToBC()->GetExperience());

		pPlayer->GiveExperience(experience, true, true);

		pPlayer->MoneyAdd(this->GetSettings()->GetZenRewardSurvive());

		memcpy(pMsg.data.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		pMsg.data.score = pPlayer->GetEventScore();
		pMsg.data.exp = this->GetPlayer(i)->ToBC()->GetExperience();
		pMsg.data.zen = this->GetSettings()->GetZenRewardSurvive();

		pPlayer->UpdateStatistic(STATISTIC_BLOOD_CASTLE_POINTS, pPlayer->GetEventScore());
		pPlayer->sendPacket(MAKE_PCT(pMsg));

		sEventMgr->AddEventScore(pPlayer->GetGUID(), pPlayer->GetName(), EVENT_BLOOD_CASTLE, this->GetGround(), pPlayer->GetEventScore());

		if ( !this->GetPlayer(i)->ToBC()->IsComplete() )
		{
			if ( this->GetPlayer(i)->ToBC()->IsDoorDestroyed() || this->GetPlayer(i)->ToBC()->IsStatueDestroyed() )
			{
				sEventMgr->GiveContributionReward(pPlayer, EVENT_BLOOD_CASTLE, this->GetGround() + 1, 2, GremoryCaseReward::BLOOD_CASTLE);
			}
			else
			{
				sEventMgr->GiveContributionReward(pPlayer, EVENT_BLOOD_CASTLE, this->GetGround() + 1, 3, GremoryCaseReward::BLOOD_CASTLE);
			}
		}
	}
}

void BloodCastle::SendMessageCompleted()
{
	this->GetWeaponOwner() ? 
		this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_COMPLETED_1).c_str(), this->GetWeaponOwner()->GetName()):
		this->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_COMPLETED_2).c_str());
}

void BloodCastle::SendNotice(NoticeType id, const char* msg, ...)
{
	ARG(buffer, msg);

	for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
	{
		Player* pPlayer = this->GetPlayer(i)->GetPlayer();

		if ( !pPlayer )
			continue;

		pPlayer->SendNoticeNormal(id, buffer);
	}
}

void BloodCastle::SendWorldStatus(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( this->GetState() == BLOOD_CASTLE_STATE_PLAYEND || this->GetState() == BLOOD_CASTLE_STATE_PLAYING )
	{
		pPlayer->SendWorldAttribute(0, 4, 1, false, m_BloodCastleZoneEntrance);

		if ( this->GetSubState() > BLOOD_CASTLE_SUBSTATE_MONSTER_KILL )
		{
			pPlayer->SendWorldAttribute(0, 8, 1, false, m_BloodCastleZoneBridge);
		}

		if ( this->GetSubState() > BLOOD_CASTLE_SUBSTATE_DOOR_KILL )
		{
			pPlayer->SendWorldAttribute(0, 4, 1, false, m_BloodCastleZoneDoor);
			pPlayer->SendWorldAttribute(0, 4, 1, false, m_BloodCastleZoneBossZone);
			pPlayer->SendWorldAttribute(0, 4, 1, false, m_BloodCastleZoneAltar);
		}
	}
}

void BloodCastle::UpdateWeaponStatus()
{
	if ( this->GetSubState() != BLOOD_CASTLE_SUBSTATE_WEAPON_DELIVERED && this->GetSubState() != BLOOD_CASTLE_SUBSTATE_COMPLETE )
	{
		if ( Player* pPlayer = this->GetWeaponOwner() )
		{
			if ( !Player::IsPlayerBasicState(pPlayer, false) || !BC_MAP_RANGE(pPlayer->GetWorldId()) )
			{
				this->SetWeaponStatus(nullptr);
			}
			else
			{
				if ( pPlayer->GetRegenStatus() == REGEN_NONE )
				{
					this->SetWeaponOwnerX(pPlayer->GetX());
					this->SetWeaponOwnerY(pPlayer->GetY());
				}
			}
		}
	}
}

void BloodCastle::SetWeaponStatus(Player* pPlayer)
{
	if ( this->GetSubState() != BLOOD_CASTLE_SUBSTATE_WEAPON_DELIVERED && this->GetSubState() != BLOOD_CASTLE_SUBSTATE_COMPLETE )
	{
		if ( !pPlayer )
		{
			Item item(ITEMGET(13, 19), this->GetWeaponLevel());
			item.bc_weapon.event_id = EVENT_BLOOD_CASTLE;
			item.bc_weapon.event_ground = this->GetGround();

			sItemMgr->ItemSerialCreate(nullptr, m_BloodCastleData[this->GetGround()].world, this->GetWeaponOwnerX(), this->GetWeaponOwnerY(), item, false, true, true);

			this->Log("%s dropped Archangel Weapon.", this->GetWeaponOwnerName());

			for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
			{
				PLAYER_POINTER(this->GetPlayer(i)->GetPlayer())->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_ARCHANGEL_WEAPON_DROPPED).c_str());
			}

			this->SetWeaponOwner(nullptr);
			this->ResetWeaponOwnerName();
		}
		else
		{
			this->SetWeaponOwner(pPlayer);
			this->SetWeaponOwnerName(pPlayer->GetName());
			this->SetWeaponOwnerX(pPlayer->GetX());
			this->SetWeaponOwnerY(pPlayer->GetY());

			/*
			if ( this->GetPlayer(i)->ToBC()->GetReward() == 0 )
			{
				this->GetPlayer(i)->ToBC()->SetReward(1);
			}
			*/

			this->Log("%s obtained Archangel Weapon.", pPlayer->GetName());

			for ( uint32 i = 0; i < this->GetPlayerSize(); ++i )
			{
				Player* pPlayerBC = this->GetPlayer(i)->GetPlayer();

				if ( !pPlayerBC )
				{
					continue;
				}

				switch ( this->GetWeaponLevel() )
				{
				case 0:
					{
						pPlayerBC->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_ARCHANGEL_WEAPON_1).c_str(), this->GetWeaponOwnerName());
					} break;

				case 1:
					{
						pPlayerBC->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_ARCHANGEL_WEAPON_2).c_str(), this->GetWeaponOwnerName());
					} break;

				case 2:
					{
						pPlayerBC->SendNotice(NOTICE_GLOBAL, sMessage->Get(MSG_BLOOD_CASTLE_ARCHANGEL_WEAPON_3).c_str(), this->GetWeaponOwnerName());
					} break;
				}
			}
		}
	}
}

CBloodCastleMgr::CBloodCastleMgr(): EventCommonMgr(MAX_BLOOD_CASTLE_GROUND)
{
	FOR_EACH_GROUND(BLOOD_CASTLE)->SetGround(i);
	FOR_EACH_GROUND(BLOOD_CASTLE)->SetState_None();
	this->SetState_None();
}

CBloodCastleMgr::~CBloodCastleMgr()
{
	FOR_EACH_GROUND(BLOOD_CASTLE)->SetState_None();
}

void CBloodCastleMgr::LoadBloodCastleSetting()
{
	if ( !sGameServer->IsBloodCastleEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Blood Castle Setting...");

	ReadScript file("EventData/BloodCastle.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 max_players = file.GetInt32();
					file.GetToken();	bool pk_check = file.GetBool();
					file.GetToken();	uint8 pk_level_max = file.GetUInt8();
					file.GetToken();	bool remove_elf_soldier_buff = file.GetBool();

					if ( id < MAX_BLOOD_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetMaxPlayers(max_players);
						this->GetGround(id)->GetSettings()->SetPKCheck(pk_check);
						this->GetGround(id)->GetSettings()->SetPKLevelMax(pk_level_max);
						this->GetGround(id)->GetSettings()->SetRemoveElfSoldierBuff(remove_elf_soldier_buff);
					}
				} break;

			case 1:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 monster_user = file.GetInt32();
					file.GetToken();	int32 monster_limit = file.GetInt32();
					file.GetToken();	int32 boss_user = file.GetInt32();
					file.GetToken();	int32 boss_limit = file.GetInt32();
					file.GetToken();	int32 door_life = file.GetInt32();
					file.GetToken();	int32 statue_life = file.GetInt32();

					if ( id < MAX_BLOOD_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetMonsterPerUser(monster_user);
						this->GetGround(id)->GetSettings()->SetMonsterLimit(monster_limit);
						this->GetGround(id)->GetSettings()->SetBossPerUser(boss_user);
						this->GetGround(id)->GetSettings()->SetBossLimit(boss_limit);
						this->GetGround(id)->GetSettings()->SetDoorLife(door_life);
						this->GetGround(id)->GetSettings()->SetStatueLife(statue_life);
					}
				} break;

			case 2:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 bonus_door_cleared = file.GetInt32();
					file.GetToken();	int32 bonus_statue_cleared = file.GetInt32();
					file.GetToken();	int32 bonus_weapon_delivered = file.GetInt32();
					file.GetToken();	int32 bonus_remain_time = file.GetInt32();

					if ( id < MAX_BLOOD_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetBonusDoorCleared(bonus_door_cleared);
						this->GetGround(id)->GetSettings()->SetBonusStatueCleared(bonus_statue_cleared);
						this->GetGround(id)->GetSettings()->SetBonusWeaponDelivered(bonus_weapon_delivered);
						this->GetGround(id)->GetSettings()->SetBonusRemainTime(bonus_remain_time);
					}
				} break;

			case 3:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	int32 score_quest_finished = file.GetInt32();
					file.GetToken();	int32 score_quest_finished_party = file.GetInt32();
					file.GetToken();	int32 score_survive = file.GetInt32();
					file.GetToken();	int32 score_die = file.GetInt32();
					file.GetToken();	int32 score_quest_fail = file.GetInt32();

					if ( id < MAX_BLOOD_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetScoreQuestFinished(score_quest_finished);
						this->GetGround(id)->GetSettings()->SetScoreQuestFinishedParty(score_quest_finished_party);
						this->GetGround(id)->GetSettings()->SetScoreSurvive(score_survive);
						this->GetGround(id)->GetSettings()->SetScoreDie(score_die);
						this->GetGround(id)->GetSettings()->SetScoreQuestFail(score_quest_fail);
					}
				} break;

			case 4:
				{
					uint8 id = file.GetUInt8() - 1;

					file.GetToken();	uint32 zen_reward_die = file.GetUInt32();
					file.GetToken();	uint32 zen_reward_survive = file.GetUInt32();

					if ( id < MAX_BLOOD_CASTLE_GROUND )
					{
						this->GetGround(id)->GetSettings()->SetZenRewardDie(zen_reward_die);
						this->GetGround(id)->GetSettings()->SetZenRewardSurvive(zen_reward_survive);
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Blood Castle Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CBloodCastleMgr::StartGround(uint32 duration)
{
	if ( !sGameServer->IsBloodCastleEnabled() )
		return;

	FOR_EACH_GROUND(BLOOD_CASTLE)->SetDuration(duration);
}

void CBloodCastleMgr::Update()
{
	FOR_EACH_GROUND(BLOOD_CASTLE)->UpdateTime();
}

void CBloodCastleMgr::SetState_None()
{
	this->SetState(EVENT_MGR_STATE_NONE);
	this->SetMessageSent(EVENT_MESSAGE_OPEN_BLOOD_CASTLE, false);
	this->RunTime(0);

	sLog->outInfo(LOG_BLOOD_CASTLE,"[BloodCastleMgr] SetState_None()");
}
	
void CBloodCastleMgr::SetState_Notify()
{
	this->SetState(EVENT_MGR_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_BLOOD_CASTLE,"[BloodCastleMgr] SetState_Notify()");
}
	
void CBloodCastleMgr::SetState_Open()
{
	FOR_EACH_GROUND(BLOOD_CASTLE)->SetState_Open();

	this->SetState(EVENT_MGR_STATE_OPEN);
	this->RunTime(EVENT_MGR_OPEN_TIME * MINUTE * IN_MILLISECONDS);

	sServerLink->EventNotification(EVENT_NOTIFICATION_BLOOD_CASTLE, 1);

	sLog->outInfo(LOG_BLOOD_CASTLE,"[BloodCastleMgr] SetState_Open()");
}
	
void CBloodCastleMgr::SetState_Playing()
{
	this->SetState(EVENT_MGR_STATE_PLAYING);
	this->RunTime(0);

	sServerLink->EventNotification(EVENT_NOTIFICATION_BLOOD_CASTLE, 0);

	FOR_EACH_GROUND(BLOOD_CASTLE)->SetState_StandBy();

	sLog->outInfo(LOG_BLOOD_CASTLE, "[BloodCastleMgr] SetState_Playing()");
}

void CBloodCastleMgr::ProcState_None()
{

}
	
void CBloodCastleMgr::ProcState_Notify()
{
	if ( this->TimePassed() )
		this->SetState_Open();
}
	
void CBloodCastleMgr::ProcState_Open()
{
	if ( this->GetNotificationTime()->Elapsed(EVENT_NOTIFICATION_TIME) )
	{
		sServerLink->EventNotification(EVENT_NOTIFICATION_BLOOD_CASTLE, 1);
	}

	this->SendRemainMinutes("%d minute(s) left before Blood Castle Start", 1, 5);

	this->SendMessageID(EVENT_MESSAGE_OPEN_BLOOD_CASTLE);

	if ( this->TimePassed() )
		this->SetState_Playing();
}
	
void CBloodCastleMgr::ProcState_Playing()
{
	for ( uint8 i = 0; i < MAX_BLOOD_CASTLE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() != BLOOD_CASTLE_STATE_NONE )
			return;
	}

	this->SetState_None();
}

void CBloodCastleMgr::EnterRequest(Player * pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None && pPlayer->GetInterfaceState()->GetID() != InterfaceData::BloodCastleEntrance )
		return;

	POINTER_PCT_LOG(BLOOD_CASTLE_ENTER_REQUEST, lpMsg, Packet, 0);

	uint8 ground = lpMsg->ground;
	uint8 item_slot = lpMsg->item_slot;

	if ( !use_inventory_range(item_slot) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( ground >= MAX_BLOOD_CASTLE_GROUND )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( this->GetGround(ground)->GetSettings()->IsPKCheck() && (pPlayer->GetPKLevel() >= this->GetGround(ground)->GetSettings()->GetPKLevelMax()) )
	{
		this->EnterResult(pPlayer, 0x06);
		return;
	}

	if ( this->GetState() != EVENT_MGR_STATE_OPEN )
	{
		this->EnterResult(pPlayer, 0x02);
		return;
	}

	int32 current_players = this->GetGround(ground)->GetPlayersCount(true);

	if ( current_players >= this->GetGround(ground)->GetPlayerSize() )
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}

	if ( !this->EventCommonMgr::HaveTicket(pPlayer->GetInventory()->GetItem(item_slot), ITEMGET(13, 18), ITEMGET(13, 47), ground) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( !sEventMgr->CanEnterEvent(pPlayer, EVENT_BLOOD_CASTLE) )
	{
		this->EnterResult(pPlayer, 0x0B);
		return;
	}

	uint8 real_ground = sEventMgr->GetEventLevel(EVENT_BLOOD_CASTLE, pPlayer);

	if ( real_ground >= MAX_BLOOD_CASTLE_GROUND )
	{
		this->EnterResult(pPlayer, 0x04);
		return;
	}

	if ( real_ground != ground )
	{
		if ( ground < real_ground )
		{
			this->EnterResult(pPlayer, 0x03);
			return;
		}

		if ( ground > real_ground )
		{
			this->EnterResult(pPlayer, 0x04);
			return;
		}
	}

	pPlayer->GetInterfaceState()->Reset();

	if ( !this->GetGround(ground)->AddPlayer(pPlayer) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	if ( !pPlayer->MoveToGate(m_BloodCastleData[ground].gate) )
	{
		this->EnterResult(pPlayer, 0x01);
		return;
	}

	this->ConsumeTicket(pPlayer, ITEMGET(13, 18), ITEMGET(13, 47), item_slot);

	if ( this->GetGround(ground)->GetSettings()->IsRemoveElfSoldierBuff() )
	{
		pPlayer->RemoveBuff(BUFF_ELF_SOLDIER);
	}

	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You are entering Blood Castle %d.", ground + 1);

	sEventMgr->EventEnter(pPlayer, EVENT_BLOOD_CASTLE);
}

void CBloodCastleMgr::EnterResult(Player * pPlayer, uint8 result)
{
	BLOOD_CASTLE_ENTER_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

bool CBloodCastleMgr::IsIn(Player* pPlayer)
{
	if ( this->GetState() != EVENT_MGR_STATE_PLAYING &&
		 this->GetState() != EVENT_MGR_STATE_OPEN )
		return false;

	for ( uint8 i = 0; i < MAX_BLOOD_CASTLE_GROUND; ++i )
	{
		if ( this->GetGround(i)->GetState() == BLOOD_CASTLE_STATE_NONE ||
			 this->GetGround(i)->GetState() == BLOOD_CASTLE_STATE_PLAYEND )
			continue;

		if ( !this->GetGround(i)->IsIn(pPlayer) )
			continue;

		pPlayer->SetEventId(EVENT_BLOOD_CASTLE);
		pPlayer->SetEventGround(i);
		return true;
	}

	return false;
}