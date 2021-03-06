/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Monster.cpp"
*
*/

Monster::Monster(uint16 entry): Unit(entry)
{
	this->GetThreatManager()->Create(this);
	this->SetAI(nullptr);
	this->SetType(OBJECT_TYPE_MONSTER);

	this->reset();
}

Monster::~Monster()
{
	this->reset();
}

void Monster::reset()
{
	this->SetConnectStatus(CONNECT_STATUS_NONE);
	this->SetSendType(MAIN_OBJECT_MONSTER);
	this->ResetName();
	this->SetClass(uint16(-1));
	this->ResetSpawnX(0);
	this->ResetSpawnY(0);
	this->SetRespawnDistance(0);
	this->SetRespawnType(GAME_OBJECT_RESPAWN_NONE);
	this->SetRespawnLocation(MONSTER_RESPAWN_NORMAL);
	this->SetMoveDistance(0);
	this->ResetNpcFunction();
	this->GetAction()->Reset();
	
	this->SetTarget(nullptr);
	this->SetTargetX(0);
	this->SetTargetY(0);
	this->SetTargetHitCount(0);
	this->SetMaxAttacker(nullptr);
	this->SetMaxAttackerGUID(0);
	this->ResetMaxAttackerName();
	this->SetLevel(0);

	this->SetZen(0);
	this->SetBattleDelay(0);
	this->SetDespawnType(MONSTER_DESPAWN_NONE);

	for ( int32 i = 0; i < MAX_MONSTER_SPECIAL_SKILL; ++i )
	{
		this->SetSkillSpecial(i, 0);
	}

	for ( uint8 i = 0; i < SKILL_ELEMENT_MAX; ++i )
	{
		this->GetSkillElement(i)->Reset();
	}

	this->ResetRefillTick(0);

	this->talk_reference_list.clear();

	this->GetThreatManager()->ClearThreat();
	this->GetThreatManager()->ClearDamage();

	this->EraseAI();

	this->ResetItemBag();
	this->ResetScriptName();

	this->m_AdditionalDataPtr.clear();
	this->m_AdditionalDataInt.clear();
	this->m_AdditionalDataFloat.clear();
	this->SetSpawning(false);
	this->SetSpawnTick(0);
	this->SetSpawnTime(0);
	this->SetDespawnTick(0);
	this->SetDespawnTime(0);
	this->SetCurrentAI(0);
	this->SetBasicAI(0);
	this->SetOrderAI(0);
	this->SetLastAIRunTime(0);
	this->SetLastAIAutomataRunTime(0);
	this->SetLastAIAutomataDelay(0);
	this->SetCurrentAIState(0);
	this->SetGroupAI(0);
	this->SetGroupMemberAI(0);
	this->SetAITick(GetTickCount());
	this->SetLastUpdate(0);

	this->SetRespawnID(0);
	this->SetOriginalDirection(-1);

	this->ResetScriptMaxPower(0);

	this->SetCurActionTime(0);
	this->SetNextActionTime(0);
	this->SetDelayActionTime(0);

	this->SetSummonTick(0);

	this->SetSummonType(1);
	this->SetSummonPlayer(nullptr);

	this->SetScriptElementalAttribute(ELEMENTAL_ATTRIBUTE_NONE);
	this->SetRespawnTimeRangeMin(0);
	this->SetRespawnTimeRangeMax(0);

	this->SetLastMoveTime(0);
	
	this->SetRegenPowerTime(MyGetTickCount());

	this->SetLastActionTime(MyGetTickCount());
	this->SetLastHitTime(MyGetTickCount());

	this->SetKillID(0);
	this->GetTeleportOutOfRangeTime()->Reset();

	this->Unit::Init();
}

void Monster::SetBasicLocation(int16 x1, int16 y1, int16 x2, int16 y2)
{
	this->SetX(x1);
	this->SetY(y1);
	this->SetTX(x1);
	this->SetTY(y1);
	this->SetSpawnX(0, x1);
	this->SetSpawnY(0, y1);
	this->SetSpawnX(1, x2);
	this->SetSpawnY(1, y2);

	this->GetRegenLocation()->SetX(this->GetX());
	this->GetRegenLocation()->SetY(this->GetY());
}

bool Monster::HasTalkReference() const
{
	return !talk_reference_list.empty();
}

void Monster::AddTalkReference(Player* pPlayer)
{
	talk_reference_list.insert(pPlayer);
}

void Monster::UpdateTalkReference()
{
	for ( TalkReferenceList::iterator it = talk_reference_list.begin(); it != talk_reference_list.end(); )
	{
		Player* pPlayer = *it;

		if ( !pPlayer )
		{
			it = talk_reference_list.erase(it);
			continue;
		}

		if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING || !pPlayer->SameDimension(this) || pPlayer->GetInterfaceState()->GetID() == InterfaceData::None || pPlayer->GetInterfaceState()->GetTarget() != this )
		{
			sLog->outInfo(LOG_MONSTER, ">>> [%u][%s] Removing Talk Reference [%s]", this->GetEntry(), this->GetName(), pPlayer->GetName());
			it = talk_reference_list.erase(it);
			continue;
		}

		++it;
	}
}

void Monster::PowerRefill()
{
	if (!IsViewportEmpty())
		return;

	if (HasBuff(BUFF_NARS_CLONE))
		return;

	if (MyGetTickCount() < GetRegenPowerTime())
		return;

	POWER_LOOP(i)
	{
		if (this->PowerGet(i) >= this->PowerGetTotal(i) || !_monsterTemplate->StatRecoveryTime[i] || _monsterTemplate->StatRecovery[i] <= 0.0f)
			continue;

		if ((MyGetTickCount() - this->GetRefillTick(i)) > _monsterTemplate->StatRecoveryTime[i])
		{
			this->SetRefillTick(i, MyGetTickCount());

			this->PowerIncrease(i, this->PowerGetMax(i) * _monsterTemplate->StatRecovery[i] / 100.0f, true);
		}
	}
}

void Monster::UpdateLive()
{
	if ( !this->IsLive() )
		return;

	if ( this->GetBattleDelay() > 0 )
		this->ReduceBattleDelay(1);

	if ( (GetTickCount() - this->GetCurActionTime()) < (this->GetNextActionTime() + this->GetDelayActionTime()) )
		return;

	this->SetCurActionTime(GetTickCount());

	sMiningSystem->UpdateMining(this);

	AI_CONTROL(Update());

	if ( this->IsStopAction() )
	{
		return;
	}
	
	this->ReactionUpdate();

	if ( this->GetAction()->Move )
	{
		this->PathFindToMove();
		this->GetAction()->Move = 0;
		return;
	}

	this->Attack();
}

void Monster::Respawn()
{
	AI_CONTROL(StopRespawn());
		
	if ( this->GetLastUpdate() != sMonsterManager->GetLastUpdate() && this->GetEventId() == EVENT_NONE )
	{
		if ( !this->SetTemplateInfo() )
		{
			this->Remove();
			return;
		}

		this->SetDBData();
	}

	this->ViewportClose();

	this->MakeRespawnLocation();
	this->GenerateDirection();
	this->SetRespawnTime(this->GetRandomRespawnTimeRange());

	POWER_LOOP(i)
	{
		this->PowerSetMax(i, this->GetScriptMaxPower(i));
	}

	if ( this->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE || sCrywolf->GetState() < 2 || sCrywolf->GetState() > 5  )
	{
		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_SUCCESS && sGameServer->IsCrywolfBenefitApply() )
		{
			POWER_LOOP(i)
			{
				this->PowerSetMax(i, int64(int64(this->GetScriptMaxPower(i)) * int64(sGameServer->GetCrywolfBenefitMonsterHP()) / 100));
			}
		}
	}

	POWER_LOOP(i)
	{
		this->PowerSet(i, this->PowerGetTotal(i));
	}

	if ( this->GetScriptElementalAttribute() >= ELEMENTAL_ATTRIBUTE_MAX )
	{
		this->SetElementalAttribute(Random(5) + 1);
	}
	
	this->GetAction()->Attack = 0;
	this->GetAction()->EmotionCount = 0;
	this->GetAction()->Escape = 0;
	this->GetAction()->Move = 0;
	this->GetAction()->Rest = 0;
	this->GetAction()->Emotion = 0;
	this->SetNextActionTime(2000);
	this->SetTarget(nullptr);
	this->SetViewState(VIEW_STATE_NONE);
	this->SetTeleportStatus(TELEPORT_NONE);
	this->SetTX(this->GetX());
	this->SetTY(this->GetY());
	this->GetRegenLocation()->SetX(this->GetX());
	this->GetRegenLocation()->SetY(this->GetY());
	this->talk_reference_list.clear();
	this->SetKiller(nullptr);
	this->SetMiningIndex(nullptr);
	this->SetMiningStage(0);
	this->IncreaseKillID(1);

	this->SetDeadStatus(DEAD_NONE);
	this->SetState(OBJECT_STATE_STAND_BY);
	this->SetLive(true);

	MONSTER_AI(this)->OnRespawn();
	this->SendCustomObjectData();

	if ( this->GetGroupAI() && this->GetGroupMemberAI() )
	{
		this->ChangeAIOrder(0);
	}
}

void Monster::UpdateStatus()
{
	if ( this->GetTeleportStatus() == TELEPORT_NONE && this->GetViewState() == VIEW_STATE_TELEPORT )
	{
		this->SetViewState(VIEW_STATE_NONE);
	}

	if ( this->GetTeleportStatus() == TELEPORT_STANDBY )
	{
		if ( this->GetDeadStatus() != DEAD_NONE )
		{
			this->SetTeleportStatus(TELEPORT_NONE);
		}
		else if( (GetTickCount() - this->GetTeleportTime()) > 100)
		{
			this->SetTeleportStatus(TELEPORT_READY);
			this->ViewportDeleteSend();
		}
	}
	else if( this->GetTeleportStatus() == TELEPORT_READY )
	{
		this->SetTeleportStatus(TELEPORT_UNK);
		this->SetTX(this->GetX());
		this->SetTY(this->GetY());
		this->CreateFrustrum();
		this->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
		this->ClearPathData();
	}

	if ( this->GetState() == OBJECT_STATE_STAND_BY )
	{
		this->SetState(OBJECT_STATE_READY);
	}

	if ( this->GetRegenStatus() == REGEN_STARTED )
	{
		this->SetRegenStatus(REGEN_NONE);
	}

	if ( this->GetTeleportStatus() == TELEPORT_UNK )
	{
		this->SetTeleportStatus(TELEPORT_NONE);
	}
}

void Monster::MakeRespawnLocation(bool random)
{
	if ( random )
	{
		sMonsterManager->GenerateRespawnLocation(this);
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	//if ( this->GetRespawnLocation() == MONSTER_RESPAWN_NORMAL && !this->GetRespawnDistance() )
	//	return;

	int32 count = 100;
	int16 x1 = this->GetSpawnX(0);
	int16 y1 = this->GetSpawnY(0);
	int16 x2 = this->GetSpawnX(1);
	int16 y2 = this->GetSpawnY(1);

	if ( this->GetRespawnLocation() == MONSTER_RESPAWN_NORMAL && this->GetRespawnDistance() )
	{
		x1 -= this->GetRespawnDistance();
		y1 -= this->GetRespawnDistance();
		x2 += this->GetRespawnDistance();
		y2 += this->GetRespawnDistance();
	}

	FIX_COORD(x1);
	FIX_COORD(y1);
	FIX_COORD(x2);
	FIX_COORD(y2);

	if ( x2 < x1 )
	{
		x2 = x1;
	}

	if ( y2 < y1 )
	{
		y2 = y1;
	}

	//limitmin(x2, x1);
	//limitmin(y2, y1);

	while ( count-- > 0 )
	{
		int16 tmp_x = x1 + Random(x2 - x1);
		int16 tmp_y = y1 + Random(y2 - y1);

		WorldGrid const& attr = pWorld->GetGrid(tmp_x, tmp_y);

		if ( !attr.IsSafe() && !attr.IsLocked_1() && !attr.IsLocked_2() )
		{
			this->SetX(tmp_x);
			this->SetY(tmp_y);
			this->SetTX(tmp_x);
			this->SetTY(tmp_y);
			break;
		}
	}
}

bool Monster::SetTemplateInfo()
{
	_monsterTemplate = sMonsterManager->GetMonsterTemplate(GetClass());
	if (!_monsterTemplate)
		return false;

	SetName(_monsterTemplate->Name.c_str());
	SetLevel(_monsterTemplate->Level);
	SetSendType((MainObjectType)_monsterTemplate->Type);
	SetRespawnTimeRangeMin(_monsterTemplate->RespawnTimeMin * IN_MILLISECONDS);
	SetRespawnTimeRangeMax(_monsterTemplate->RespawnTimeMax * IN_MILLISECONDS);
	SetRespawnTime(GetRandomRespawnTimeRange());
	SetSize(_monsterTemplate->Size);

	for (int32 i = 0; i < POWER_MAX; ++i)
	{
		power[i].set(_monsterTemplate->Stat[i]);
		power_max[i].set(_monsterTemplate->Stat[i]);
		SetScriptMaxPower(i, _monsterTemplate->Stat[i]);
	}

	SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, _monsterTemplate->DamageMin);
	SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, _monsterTemplate->DamageMax);
	SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, _monsterTemplate->DamageMin);
	SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, _monsterTemplate->DamageMax);
	SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, _monsterTemplate->DamageMin);
	SetIntData(UNIT_INT_CURSE_DAMAGE_MAX, _monsterTemplate->DamageMax);
	SetIntData(UNIT_INT_ATTACK_SPEED, _monsterTemplate->AttackSpeed);
	SetIntData(UNIT_INT_MAGIC_SPEED, _monsterTemplate->AttackSpeed);
	SetIntData(UNIT_INT_ATTACK_RATE, _monsterTemplate->AttackSuccessRate);
	SetIntData(UNIT_INT_DEFENSE, _monsterTemplate->Defense);
	SetIntData(UNIT_INT_DEFENSE_MAGIC, _monsterTemplate->Defense);
	SetIntData(UNIT_INT_DEFENSE_RATE, _monsterTemplate->DefenseSuccessRate);
	SetFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, _monsterTemplate->CriticalDamageRate);
	SetIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, _monsterTemplate->CriticalDamageAdd);
	SetFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, _monsterTemplate->ExcellentDamageRate);
	SetIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, _monsterTemplate->ExcellentDamageAdd);
	SetIntData(UNIT_INT_MOVE_SPEED, _monsterTemplate->MovementSpeed);

	SetScriptName(_monsterTemplate->ScriptName);

	for (int32 i = 0; i < Element::MAX; ++i)
		SetResistance(i, _monsterTemplate->Resistance[i]);

	SetScriptElementalAttribute(_monsterTemplate->ElementalAttribute);
	SetElementalAttribute(_monsterTemplate->ElementalAttribute);
	SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, _monsterTemplate->ElementalDefense);
	SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, _monsterTemplate->ElementalDamageMin);
	SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, _monsterTemplate->ElementalDamageMax);
	SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, _monsterTemplate->ElementalAttackSuccessRate);
	SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, _monsterTemplate->ElementalDefenseSuccessRate);
	SetIntData(UNIT_INT_DEBUFF_RESISTANCE, _monsterTemplate->DebuffResistance);
	SetIntData(UNIT_INT_DEBUFF_DEFENSE, _monsterTemplate->DebuffDefense);

	SetFloatData(UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE, _monsterTemplate->CriticalDamageResistance);
	SetFloatData(UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE, _monsterTemplate->ExcellentDamageResistance);

	auto skills = sMonsterManager->GetMonsterSkillMapBounds(GetClass());
	for (auto skill : skills)
		MagicAdd(skill, 0);

	auto special_skills = sMonsterManager->GetMonsterSkillSpecial(GetClass());
	for (auto & skill : special_skills)
		AddSpecialSkill(skill->GetSkill());

	SetLastUpdate(sMonsterManager->GetLastUpdate());
	SetSpawning(true);
	SetLive(false);
	SetDeadStatus(DEAD_STANDBY);
	SetState(OBJECT_STATE_DEAD);
	SetRespawnTick(GetTickCount());

	return true;
}

void Monster::SetDBData()
{
	monster const* data = sMonsterManager->GetMonsterData(this->GetEntry());

	if ( !data )
	{
		return;
	}

	this->SetDBData(data);
}

void Monster::SetDBData(monster const* data)
{
	SetWorldId(data->GetWorld());
	SetInstance(-1);
	SetBasicLocation(data->GetX1(), data->GetY1(), data->GetX2(), data->GetY2());
	SetDirection(data->GetDirection());
	SetRespawnID(data->GetRespawnID());
	SetOriginalDirection(data->GetDirection());
	SetSpawnTick(GetTickCount());
	SetSpawnTime(data->GetSpawnDelay() * IN_MILLISECONDS);

	SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);

	if (data->GetRespawnTimeMin() != 0 || data->GetRespawnTimeMax() != 0)
	{
		SetRespawnTimeRangeMin(data->GetRespawnTimeMin() * IN_MILLISECONDS);
		SetRespawnTimeRangeMax(data->GetRespawnTimeMax() * IN_MILLISECONDS);
		SetRespawnTime(GetRandomRespawnTimeRange());
	}

	SetRespawnDistance(data->GetSpawnDistance());
	SetRespawnLocation((data->GetX1() == data->GetX2() && data->GetY1() == data->GetY2()) ? MONSTER_RESPAWN_NORMAL : MONSTER_RESPAWN_ZONE);

	SetMoveDistance(data->GetMoveDistance());
	SetNpcFunction(data->GetNpcFunction());

	if (data->GetElementalAttribute() != ELEMENTAL_ATTRIBUTE_NONE)
	{
		SetElementalAttribute(data->GetElementalAttribute());
		SetScriptElementalAttribute(data->GetElementalAttribute());
	}

	SetItemBag(data->GetItemBag());

	if (!data->GetScriptName().empty())
		SetScriptName(data->GetScriptName());
}

void Monster::SetEventDBData(MonsterEvent const* monsterEvent)
{
	SetWorldId(monsterEvent->MapId);
	SetInstance(-1);
	SetBasicLocation(monsterEvent->X1, monsterEvent->Y1, monsterEvent->X2, monsterEvent->Y2);
	SetDirection(monsterEvent->Direction);
	SetEventId(monsterEvent->EventId);
	SetRespawnID(monsterEvent->RespawnId);
	SetOriginalDirection(monsterEvent->Direction);
	SetSpawnTick(GetTickCount());
	SetSpawnTime(monsterEvent->SpawnDelay * IN_MILLISECONDS);

	SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);

	if (monsterEvent->RespawnTime != 0)
		SetRespawn(monsterEvent->RespawnTime * IN_MILLISECONDS);

	SetRespawnDistance(monsterEvent->SpawnDistance);
	SetRespawnLocation((monsterEvent->X1 == monsterEvent->X2 && monsterEvent->Y1 == monsterEvent->Y2) ? MONSTER_RESPAWN_NORMAL : MONSTER_RESPAWN_ZONE);

	SetMoveDistance(monsterEvent->MovementDistance);
	SetNpcFunction(monsterEvent->NpcFunction);
	SetItemBag(monsterEvent->ItemBag);

	if (!monsterEvent->ScriptName.empty())
		SetScriptName(monsterEvent->ScriptName);

	SetGroupAI(monsterEvent->AIGroup);
	SetGroupMemberAI(monsterEvent->AIGroupMember);
}

void Monster::ReactionUpdate()
{
	if ( this->HasBuff(BUFF_SLEEP) )
	{
		return;
	}

	if ( this->GetAI() && this->GetAI()->IgnoreTarget() )
	{
		this->SetTarget(nullptr);
	}

	if ( !this->ViewportExist(this->GetTarget()) )
	{
		this->SetTarget(nullptr);
		this->GetAction()->Emotion = EMOTION_REST;
	}

	/*Unit* pTarget = this->GetTarget();
	
	if ( pTarget )
	{
		if ( !pTarget->IsLive() || 
			  pTarget->IsTeleporting() || 
			  pTarget->IsInSafeZone() ||
			  !this->ViewportExist(pTarget) )
		{
			pTarget = nullptr;
		}
		else if ( !this->CheckWall(pTarget) )
		{
			pTarget = nullptr;
		}
	}

	if ( !pTarget || Random(5) == 0 )
	{
		this->GetAction()->Emotion = EMOTION_REST;
		this->SetTarget(nullptr);
		this->FindEnemy();
	}*/
		
	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return;
	}

	/*if ( (MyGetTickCount() - this->GetLastActionTime()) > 10000 )
	{
		if ( !this->IsSummoned() )
		{
			this->GetAction()->Emotion = EMOTION_REST;
			this->SetTarget(nullptr);
			pTarget = nullptr;
			this->MoveAttempt();
		}
	}*/

	switch( this->GetAction()->Emotion )
	{
	case EMOTION_REST:
		{
			if ( this->GetAction()->Attack )
			{
				this->GetAction()->Attack = 0;
				this->SetTarget(nullptr);
				this->SetNextActionTime(500);
			}

			int32 action_code = this->IsSummoned() ? 1: Random(2);

			if ( action_code == 0 )
			{
				this->GetAction()->Rest = 1;
				this->SetNextActionTime(500);
			}
			else if ( _monsterTemplate->MovementRange > 0 && !this->HasRestrictionBuff() )
			{
				if ( !this->IsSummoned() )
				{
					this->MoveAttempt();
				}
				else
				{
					Unit* pSummoner = this->GetSummoner();

					if ( pSummoner )
					{
						if ( pSummoner->GetConnectStatus() == CONNECT_STATUS_PLAYING )
						{
							if ( !pSummoner->IsRest() )
							{
								int16 tx = this->GetX();
								int16 ty = this->GetY();

								if ( this->GetTargetPosition(pSummoner->GetX(), pSummoner->GetY(), tx, ty) )
								{
									this->SetTempX(tx);
									this->SetTempY(ty);
									this->GetAction()->Move = 1;
									this->SetNextActionTime(1000);
								}
							}
						}
					}
				}
			}

			this->FindEnemy();

			if ( this->GetTarget() )
			{
				this->GetAction()->EmotionCount = 30;
				this->GetAction()->Emotion = EMOTION_ATTACK;
				//this->ClearPathData();
			}
		} break;

	case EMOTION_ATTACK:
		{
			this->GetAction()->Update();

			if ( this->GetTarget() && !this->GetPathData()->IsStartEnd() )
			{
				int32 dis = Util::Distance(this->GetX(), this->GetY(), this->GetTarget()->GetX(), this->GetTarget()->GetY());
				int32 attackrange = _monsterTemplate->AttackRange;

				if ( dis <= attackrange )
				{
					if ( this->CheckWall(this->GetTarget()) )
					{
						if ( !this->GetTarget()->IsInSafeZone() )
						{
							this->GetAction()->Attack = 1;

							this->SetDirection(GetPathPacketDirPos(this->GetTarget()->GetX() - this->GetX(), this->GetTarget()->GetY() - this->GetY()));
						}
						else
						{
							this->SetTarget(nullptr);
							this->GetAction()->EmotionCount = 30;
							this->GetAction()->Emotion = EMOTION_ATTACK;
						}

						this->SetNextActionTime(this->GetIntData(UNIT_INT_ATTACK_SPEED));
					}
					else
					{
						this->SetTarget(nullptr);
						this->GetAction()->Attack = 0;
						this->SetNextActionTime(500);
						this->GetAction()->Emotion = EMOTION_REST;
						this->GetAction()->Move = 1;
					}

					//this->GetAction()->Attack = 1;
					//this->SetDirection(GetPathPacketDirPos(this->GetTarget()->GetX() - this->GetX(), this->GetTarget()->GetY() - this->GetY()));
					//this->SetNextActionTime(this->GetIntData(UNIT_INT_ATTACK_SPEED));
				}
				else
				{
					if ( this->GetTargetPosition() )
					{
						if ( this->CheckWall(this->GetTempX(), this->GetTempY()) )
						{
							this->GetAction()->Move = 1;
							this->SetNextActionTime(400);
							this->SetDirection(GetPathPacketDirPos(this->GetTarget()->GetX() - this->GetX(), this->GetTarget()->GetY() - this->GetY()));
						}
						else
						{
							this->MoveAttempt();
							this->GetAction()->Emotion = EMOTION_ESCAPE;
							this->GetAction()->EmotionCount = 10;
						}
					}
					else
					{
						this->MoveAttempt();
					}
				}
			}
		} break;

	case EMOTION_MOVE:
		{
			this->GetAction()->Update();
			this->GetAction()->Move = 1;
			this->SetNextActionTime(800);

			if ( this->GetTarget() )
			{
				int tdir = GetPathPacketDirPos(this->GetTarget()->GetX() - this->GetX(), this->GetTarget()->GetY() - this->GetY()) * 2;
				this->SetTempX(this->GetTempX() + Path::Table[tdir] * (-3));
				this->SetTempY(this->GetTempY() + Path::Table[tdir + 1] * (-3));
			}
		} break;

	case EMOTION_ESCAPE:
		{
			this->GetAction()->Update();
			this->GetAction()->Move = 0;
			this->GetAction()->Attack = 0;
			this->SetNextActionTime(400);
		} break;
	}
}

void Monster::UpdateSpawn()
{
	if ( (MyGetTickCount() - this->GetSpawnTick()) > this->GetSpawnTime() )
	{
		this->SetDespawnTick(MyGetTickCount());
		sScriptAI->GetScriptAI(this->GetScriptName(), this);
		MONSTER_AI(this)->OnCreate();
		this->BuildName();
		this->SetDeadStatus(DEAD_READY);
		this->SetSpawning(false);
	}
}

void Monster::UpdateDespawn()
{
	if ( this->IsSpawning() )
	{
		return;
	}

	if ( this->GetDespawnType() == MONSTER_DESPAWN_TIME )
	{
		if ( (MyGetTickCount() - this->GetDespawnTick()) > this->GetDespawnTime() )
		{
			MONSTER_AI(this)->OnDespawn();
			this->Remove();
		}
	}
}

void Monster::GenerateDirection()
{
	if ( this->GetDirection() == uint8(-1) )
		this->SetDirection(Random<uint8>(8));
}

void Monster::Say(const char * msg, ...)
{
	ARG(buffer, msg);

	{
		TEXT_SAY pMsg(this->GetEntry(), buffer);
		this->sendPacket_viewport(MAKE_PCT(pMsg));
	}

	{
		PACKET_CHAT_DATA pMsg("", "");
		strcpy(pMsg.message, this->GetName());
		strcpy(&pMsg.message[strlen(this->GetName())], " Say: ");
		strcpy(&pMsg.message[strlen(pMsg.message)], buffer);
		this->sendPacket_viewport(MAKE_PCT(pMsg));
	}
}

void Monster::SayTo(Unit* pUnit, const char * msg, ...)
{
	ARG(buffer, msg);

	{
		TEXT_SAY pMsg(this->GetEntry(), buffer);
		pUnit->sendPacket(MAKE_PCT(pMsg));
	}

	{
		PACKET_CHAT_DATA pMsg("", "");
		strcpy(pMsg.message, this->GetName());
		strcpy(&pMsg.message[strlen(this->GetName())], " Say: ");
		strcpy(&pMsg.message[strlen(pMsg.message)], buffer);
		pUnit->sendPacket(MAKE_PCT(pMsg));
	}
}

void Monster::AlterData(float size, const char * name)
{
	this->SetSize(size);
	this->SetName(name);

	this->SendCustomObjectData();
}

void Monster::AfterHitCheck(Unit * pAttacker, int32 damage, int32 shield_damage, uint16 damage_flag)
{
	Unit * pAttackerMaster = pAttacker->GetSummoner();

	if ( this->GetClass() == 709 ||
		 this->GetClass() == 710 ||
		 this->GetClass() == 711 ||
		 this->GetClass() == 712 )
	{
		if ( this->GetLastAttacker() )
		{
			if ( Player* pPlayer = this->GetLastAttacker()->ToPlayer() )
			{
				this->SetMaxAttacker(pPlayer);
				this->SetMaxAttackerGUID(pPlayer->GetGUID());
				this->SetMaxAttackerName(pPlayer->GetName());
			}
		}
	}

	this->ClearAllBuff(BUFF_CLEAR_FLAG_ALL);
	this->KillExperience();
	
	this->DropItem();

	if ( pAttackerMaster && pAttackerMaster->IsPlayer() )
	{
		pAttackerMaster->ToPlayer()->PartySendDamage(this, damage, shield_damage, damage_flag);
	}

	if ( this->GetMaxAttacker() )
	{
		sHuntingRecord->Update(this->GetMaxAttacker(), 0, 0, 1, 0, 0);

		this->GetMaxAttacker()->AddDelayed(DELAYED_MONSTER_DIE_PLUS, 100, this);

		if ( this->IsWorldFlag(WORLD_FLAG_ALLOW_PK_MONSTER_KILL) )
		{
			this->GetMaxAttacker()->PKUpdate(this->GetLevel());
		}

		if ( Party* pParty = this->GetMaxAttacker()->GetParty() )
		{
			pParty->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_MONSTER_KILL, this);
			pParty->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_MONSTER_KILL_MAP, this);

			pParty->QuestGuidedMonsterKill(this);
		}
		else
		{
			this->GetMaxAttacker()->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_MONSTER_KILL, this);
			this->GetMaxAttacker()->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_MONSTER_KILL_MAP, this);

			this->GetMaxAttacker()->QuestGuidedMonsterKill(this);
		}

		this->GetMaxAttacker()->UpdateStatistic(STATISTIC_KILLED_MONSTERS, 1);
		this->GetMaxAttacker()->QuestEvolutionMonsterKill(this);
		sMonsterSoul->MonsterKill(this->GetMaxAttacker(), this);
	}

	//sLog->outInfo(LOG_MONSTER, "%s [%u] %s Killed by %s", __FUNCTION__, this->GetEntry(), this->GetName(), NAME_SAFE(this->GetMaxAttacker()));
}

void Monster::BuildName()
{
	this->SetName(sObjectMgr->MakeText(this->GetName(), this, nullptr).c_str());
}

bool Monster::IsMoveAllowed(int16 x, int16 y, bool target)
{
	if ( this->GetAI() )
	{
		return this->GetAI()->MoveAllowed(x, y);
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	WorldGrid const& grid = pWorld->GetGrid(x, y);

	if ( grid.IsSafe() || grid.IsLocked_1() || grid.IsLocked_2() )
		return false;

	return true;
}

MonsterAI * Monster::AIGroup(std::string const& ss)
{
	std::string name;
	std::istringstream stream(ss.c_str());

    while (stream)
    {
        stream >> name;

		if ( this->GetAI() && this->GetScriptName() == name && !name.empty() )
			return this->GetAI();

        name = "";
    }

	return nullptr;
}

bool Monster::IsAIGroup(std::string const& ss)
{
	std::string name;
	std::istringstream stream(ss.c_str());

    while (stream)
    {
        stream >> name;

		if ( this->GetScriptName() == name && !name.empty() )
			return true;

        name = "";
    }

	return false;
}

MonsterAI const* Monster::AIGroup(std::string const& ss) const
{
	std::string name;
	std::istringstream stream(ss.c_str());

    while (stream)
    {
        stream >> name;

		if ( this->GetAI() && this->GetScriptName() == name && !name.empty() )
			return this->GetAI();

        name = "";
    }

	return nullptr;
}

void Monster::AISet(MonsterAI* pAI)
{
	this->EraseAI();
	this->SetAI(pAI);
}

void Monster::SetNameData(const char* name, ...)
{
	ARG(buffer, name);
	this->SetName(sObjectMgr->MakeText(buffer, this, nullptr).c_str());
}

float Monster::GetElementValue(uint8 element)
{
	if ( element >= SKILL_ELEMENT_MAX )
		return 0.0f;

	if ( !this->GetSkillElement(element)->GetTime() )
		return 0.0f;

	return this->GetSkillElement(element)->GetValue();
}

void Monster::FindEnemy()
{
	if ( this->GetAI() && this->GetAI()->IgnoreTarget() )
	{
		this->SetTarget(nullptr);
		return;
	}

	if ( !this->GetAI() || !this->GetAI()->SearchEnemy() )
	{
		/*if ( Random(5) == 0 || !this->GetTarget() )
			this->SetTarget(this->GetMaxAggro());

		if ( this->GetTarget() && 
			 this->GetTarget()->GetSummoned() && 
			 this->GetTarget()->GetSummoned()->IsPlaying() && 
			 this->GetTarget()->GetSummoned()->IsLive() )
		{
			this->SetTarget(this->GetTarget()->GetSummoned());
		}*/

		int64 max_threat = -1;
		const auto& threat_data = this->GetThreatManager()->GetThreatData();
		for (auto & itr : threat_data)
		{
			ThreatData const* pData = itr.second;

			if ( !pData )
			{
				continue;
			}

			Player* pPlayer = sObjectMgr->FindPlayerByGUID(pData->GetGuid());

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->IsInTeleportInmuneState() )
			{
				continue;
			}

			if ( pPlayer->GetRegenStatus() != REGEN_NONE )
			{
				continue;
			}

			if ( max_threat < pData->GetThreat() )
			{
				max_threat = pData->GetThreat();
				this->SetTarget(pPlayer);
			}
		}

		if ( this->GetTarget() && 
			 this->GetTarget()->GetSummoned() && 
			 this->GetTarget()->GetSummoned()->IsPlaying() && 
			 this->GetTarget()->GetSummoned()->IsLive() )
		{
			this->SetTarget(this->GetTarget()->GetSummoned());
		}
	}

	/*if ( this->GetTarget() )
	{
		this->GetAction()->EmotionCount = 30;
		this->GetAction()->Emotion = EMOTION_ATTACK;

		if ( this->GetTarget()->GetX() != this->GetTargetX() || this->GetTarget()->GetY() != this->GetTargetY() )
		{
			this->ClearPathData();
		}

		this->SetTargetX(this->GetTarget()->GetX());
		this->SetTargetY(this->GetTarget()->GetY());

		int32 dis = Util::Distance(this->GetX(), this->GetY(), this->GetTarget()->GetX(), this->GetTarget()->GetY());
		int32 attackrange = this->GetAttackRange();

		if ( dis <= attackrange )
		{
			this->ClearPathData();
		}
		else
		{

		}
	}
	else
	{
		this->GetAction()->Emotion = EMOTION_REST;
		this->GetAction()->Attack = 0;
		this->GetAction()->Move = 0;
		this->SetNextActionTime(1000);
	}*/
}

bool Monster::HasSpecialSkill(uint16 skill)
{
	for ( int32 i = 0; i < MAX_MONSTER_SPECIAL_SKILL; ++i )
	{
		if ( this->GetSkillSpecial(i) == skill )
			return true;
	}

	return false;
}
	
void Monster::AddSpecialSkill(uint16 skill)
{
	for ( int32 i = 0; i < MAX_MONSTER_SPECIAL_SKILL; ++i )
	{
		if ( this->GetSkillSpecial(i) == 0 )
		{
			this->SetSkillSpecial(i, skill);
			break;
		}
	}
}
	
uint16 Monster::GetRandomSpecialSkill()
{
	RandomValue<uint16> m_randomizer(0);

	for ( int32 i = 0; i < MAX_MONSTER_SPECIAL_SKILL; ++i )
	{
		if ( this->GetSkillSpecial(i) )
		{
			m_randomizer.AddValue(this->GetSkillSpecial(i), 0);
		}
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool Monster::RunAI()
{
	if ( !this->GetCurrentAI() )
		return false;

	if ( this->GetBasicAI() )
	{
		this->SetCurrentAI(this->GetBasicAI());
	}

	MonsterAIHandler(this).Run();
	return true;
}

Monster* Monster::FindGroupMemberToSummon()
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return nullptr;

	Object* pObject = nullptr;
	Monster* pMonster = nullptr;

	HASH_SUMMON(i)
	{
		pObject = ObjectContainer[i];

		if ( !pObject )
			continue;

		pMonster = pObject->ToCreature();

		if ( !pMonster )
			continue;

		if ( pMonster == this )
			continue;

		if ( !pMonster->IsLive() || !pMonster->IsPlaying() || pMonster->GetWorldId() != this->GetWorldId() || pMonster->GetInstance() != this->GetInstance() )
			continue;

		if ( pMonster->GetGroupAI() != this->GetGroupAI() )
			continue;

		return pMonster;
	}

	return nullptr;
}
	
Monster* Monster::FindGroupMemberToHeal()
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return nullptr;

	Object* pObject = nullptr;
	Monster* pMinHP = nullptr;
	int32 iMinHP = 100000000;
	Monster* pMonster = nullptr;

	HASH_SUMMON(i)
	{
		pObject = ObjectContainer[i];

		if ( !pObject )
			continue;

		pMonster = pObject->ToCreature();

		if ( !pMonster )
			continue;

		if ( !pMonster->IsLive() || !pMonster->IsPlaying() || pMonster->GetWorldId() != this->GetWorldId() || pMonster->GetInstance() != this->GetInstance() )
			continue;

		if ( pMonster->GetGroupAI() != this->GetGroupAI() )
			continue;

		if ( !IN_RANGE(this, pMonster, 6) )
			continue;

		int32 iPercentHP = pMonster->PowerGetPercent(POWER_LIFE);

		if ( iPercentHP < 60 )
		{
			if ( pMonster == this )
				iPercentHP = 1;

			if ( iPercentHP < iMinHP )
			{
				iMinHP = iPercentHP;
				pMinHP = pMonster;
			}
		}
	}

	return pMinHP;
}

void Monster::ChangeAIOrder(uint8 order)
{
	if ( !this->GetGroupAI() || !this->GetGroupMemberAI() )
		return;

	if ( order == uint8(-1) )
	{
		order = this->GetOrderAI() + 1;
	}

	if ( order >= MONSTER_MAX_AI_ORDER )
		return;

	uint8 old_order = order == 0 ? 0: order - 1;

	AIData const* pAIData = sMonsterManager->GetMonsterAIData(this->GetGroupAI(), this->GetGroupMemberAI());

	if ( pAIData )
	{

		if ( order == 0 || (GetTickCount() - this->GetAITick()) > pAIData->GetAITime(old_order) )
		{
			this->SetAITick(GetTickCount());
			this->SetCurrentAI(pAIData->GetAIOrder(order));
			this->SetBasicAI(pAIData->GetAIOrder(order));
			this->SetOrderAI(order);
		}
	}
	else
	{
		this->SetGroupAI(0);
		this->SetGroupMemberAI(0);
	}
}

bool Monster::SendHPBar()
{
	if ( this->GetAI() )
		return this->GetAI()->SendHPBar();

	return true;
}

void Monster::SendDirection()
{
	this->ActionSend(this->GetEntry(), 122, this->GetDirection());
}

bool Monster::ViewportIsCharacter(Unit* pTarget)
{
	if ( !pTarget )
		return false;

	return this->viewport_data.find(pTarget->GetHighGUID()) != this->viewport_data.end();
}

void Monster::UpdatePowers(uint8 type, bool max)
{
	if ( type == POWER_LIFE )
	{
		MONSTER_LIFE_UPDATE_SEND pMsg(this->GetEntry(), this->PowerGet(POWER_LIFE), this->PowerGetTotal(POWER_LIFE));
		this->sendPacket_viewport(MAKE_PCT(pMsg));
	}
}

void Monster::ProcessHit(Unit* pAttacker)
{
	if ( !pAttacker || !pAttacker->IsLive() || pAttacker->GetState() != OBJECT_STATE_READY )
		return;

	if ( this->GetTarget() == pAttacker )
		return;

	if ( !this->CheckWall(pAttacker) )
		return;

	if ( pAttacker->IsInSafeZone() )
		return;

	if ( this->GetAction()->Emotion == EMOTION_REST )
	{
		this->GetAction()->Emotion = EMOTION_ATTACK;
		this->GetAction()->EmotionCount = 10;
	}
	else if ( this->GetAction()->Emotion == EMOTION_ATTACK )
	{
		this->GetAction()->EmotionCount = 10;
	}

	int32 range = _monsterTemplate->AttackRange;
	int32 dis = Util::Distance(this->GetX(), this->GetY(), pAttacker->GetX(), pAttacker->GetY());

	if ( this->GetAction()->Attack == 0 )
	{
		if ( dis <= range )
		{
			if (pAttacker->IsSummoned() || roll_chance_i(90))
			{
				this->SetTarget(pAttacker);
			}
		}
		else
		{
			if ( !this->GetTarget() )
			{
				this->SetTarget(pAttacker);
			}
		}

		/*if ( lpObj->m_bIsInMonsterHerd != false && lpObj->TargetNumber == aIndex )
		{
			if ( lpObj->m_lpMonsterHerd )
			{
				lpObj->m_lpMonsterHerd->BeenAttacked (lpObj, &gObj[aIndex]);
			}
		}*/
	}
	else
	{
		if ( Random(2) == 1 )
		{
			this->GetAction()->Attack = 1;
			this->SetTarget(pAttacker);
		}
	}
}

bool Monster::Update(uint32 diff)
{
	this->UpdateTimer();

	if ( this->IsHalfSecondPassed() )
		this->UpdateDelayed();

	this->UpdateWorld();

	if ( this->IsLive() )
	{
		this->UpdateMovement();

		if ( this->IsHalfSecondPassed() )
			this->CreateFrustrum();
		
		if ( this->IsSecondPassed() )
		{
			this->UpdateDelayedTeleport();
			this->UpdateBuffStatus();

			this->UpdateTalkReference();
			this->PowerRefill();
			this->UpdateDespawn();
			this->GetThreatManager()->Update();
		}

		if ( !this->GetCurrentAI() )
		{
			this->UpdateLive();
		}
		else
		{
			if ( !this->GetAI() || !this->GetAI()->Update() )
				this->RunAI();
		}
	}
	else
	{
		if ( this->IsSpawning() )
		{
			this->UpdateSpawn();
		}
		else
		{
			if( this->GetDeadStatus() == DEAD_STANDBY )
			{
				if( (MyGetTickCount() - this->GetRespawnTick()) > (this->GetRespawnTime() + 1000) )
				{
					this->SetRespawnTick(MyGetTickCount());
					this->ClearAllBuff(0);
					this->SetDeadStatus(DEAD_READY);
					this->SetState(OBJECT_STATE_RESPAWN);
					this->GetThreatManager()->ClearThreat();
					this->GetThreatManager()->ClearDamage();
					this->SetMaxAttacker(nullptr);
					this->SetMaxAttackerGUID(0);
					this->ResetMaxAttackerName();
					this->SetTarget(nullptr);
					this->SetZen(0);
					this->ClearPathData();
					this->GetAction()->Reset();
								
					if ( this->GetRespawnType() == GAME_OBJECT_RESPAWN_DELETE )
					{
						this->Remove();
					}
					else if ( this->GetDespawnType() == MONSTER_DESPAWN_DIE )
					{
						if ((MyGetTickCount() - this->GetDespawnTick()) > this->GetDespawnTime())
						{
							MONSTER_AI(this)->OnDespawn();
							this->Remove();
						}
					}
				}
			}
			else if( this->GetDeadStatus() == DEAD_READY )
			{
				if( (GetTickCount() - this->GetRespawnTick()) > 1000 )
				{
					this->Respawn();
				}
			}
		}
	}

	return true;
}

bool Monster::IsViewportControl() const
{
	if ( !sGameServer->IsMonsterViewportControl() )
		return true;

	if ( !this->GetAI() )
		return false;

	return this->GetAI()->IsViewportControl();
}

bool Monster::IsStopAction() const
{
	if ( !sGameServer->IsMonsterStopAction() )
		return false;

	if ( !this->IsViewportEmpty() )
		return false;

	if ( !this->GetAI() )
		return true;

	return this->GetAI()->IsStopAction();
}

void Monster::SetRespawn(uint32 time)
{
	this->SetRespawnTimeRangeMin(time);
	this->SetRespawnTimeRangeMax(time);
	this->SetRespawnTime(this->GetRandomRespawnTimeRange());
}

void Monster::LogKillData(bool all, bool save_db)
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	DamageDataList const& damage_data = this->GetThreatManager()->GetDamageData();
	for (DamageDataList::const_iterator it = damage_data.begin(); it != damage_data.end(); ++it)
	{
		DamageData const* pData = *it;

		if (!pData)
		{
			continue;
		}

		PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_BOSS_KILL);
		stmt->setUInt16(this->GetEntry());
		stmt->setString(this->GetName());
		stmt->setUInt8(this->GetKillID());
		stmt->setUInt32(pData->GetGuid());
		stmt->setString(pData->GetName());
		stmt->setInt64(pData->GetDamage(MONSTER_THREAT_DAMAGE_NORMAL));
		stmt->setInt64(pData->GetDamage(MONSTER_THREAT_DAMAGE_REFLECT));
		stmt->setInt64(pData->GetDamage(MONSTER_THREAT_DAMAGE_DEBUFF));
		stmt->setInt64(pData->GetDamage(MONSTER_THREAT_DAMAGE_ELEMENTAL));
		stmt->setUInt16(sGameServer->GetServerCode());
		stmt->setString(ConvertTimeToString(pData->GetFirstHit()));
		trans->Append(stmt);
	}

	MuLogDatabase.CommitTransaction(trans);

	if (save_db && this->GetRespawnTime() > 0)
	{
		SQLTransaction trans2 = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_DELETE);
		stmt->setUInt16(sGameServer->GetServerCode());
		stmt->setUInt16(this->GetEntry());
		trans2->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_INSERT);
		stmt->setUInt16(sGameServer->GetServerCode());
		stmt->setUInt16(this->GetEntry());
		stmt->setInt64(time(nullptr) + (this->GetRespawnTime() / IN_MILLISECONDS));
		trans2->Append(stmt);

		MuDatabase.CommitTransaction(trans2);
	}

	if (all)
	{
		sObjectMgr->SendNoticeToAll(NOTICE_GLOBAL, "%s has been killed by %s", this->GetName(), this->GetMaxAttackerName());
	}
	else
	{
		sObjectMgr->SendNoticeToMap(this->GetWorldId(), NOTICE_GLOBAL, "%s has been killed by %s", this->GetName(), this->GetMaxAttackerName());
	}
}

void Monster::TeleportToRegen()
{
	if ( !sGameServer->IsMonsterTeleportOutOfRange() )
	{
		return;
	}

	if ( !this->IsViewportEmpty() )
	{
		this->GetTeleportOutOfRangeTime()->Start();
	}
	else
	{
		if ( this->GetTeleportOutOfRangeTime()->Elapsed(10000) )
		{
			if ( this->IsOutOfMoveRange() )
			{
				this->TeleportToLocation(this->GetWorldId(), this->GetRegenLocation()->GetX(), this->GetRegenLocation()->GetY(), this->GetDirection(), this->GetInstance());
			}
		}
	}
}

bool Monster::InmuneToRadiance() const
{
	if (IsDebuffInmune(true))
		return true;

	auto const pAI = this->GetAI();
	if (pAI)
		return pAI->InmuneToRadiance();

	if (_monsterTemplate->RadianceImmune != 1)
		return false;

	return true;
}

bool Monster::InmuneToPunish(Player* pPlayer)
{
	if (!pPlayer)
		return false;

	if (_monsterTemplate->RadianceImmune != 2)
		return false;

	return true;
}

int32 Monster::GetPlayerViewportCount() const
{
	int32 count = 0;
	Object* pObject;
	VIEWPORT_LOOP(pObject)
		if ( pObject && pObject->IsPlayer() )
		{
			++count;	
		}
	VIEWPORT_CLOSE

	return count;
}