/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterAI.cpp"
*
*/

MonsterAI::MonsterAI(Monster* mMonster): pMEMonster(mMonster)
{
	this->SetFlags(MONSTER_AI_FLAG_NONE);
	this->m_phase.set(0);
	this->m_phase_time.set(0);
}

MonsterAI::~MonsterAI()
{
	this->m_TimerMap.clear();
}

bool MonsterAI::Update()
{
	return false;
}

bool MonsterAI::UpdateMovement()
{
	return true;
}

bool MonsterAI::MoveAttempt()
{
	return false;
}

bool MonsterAI::MoveAllowed(coord_type x, coord_type y)
{
	World* pWorld = me()->GetWorld();
	
	if ( !pWorld )
		return false;

	WorldGrid const& grid = pWorld->GetGrid(x, y);

	if ( !grid.IsSafe() && !grid.IsLocked_1() && !grid.IsLocked_2() )
	{
		return true;
	}

	return false;
}

bool MonsterAI::OnAttack(Unit* pVictim, bool common)
{
	return false;
}

bool MonsterAI::EnableAttack(Unit* pAttacker, Skill*, int32 count)
{
	return true;
}

bool MonsterAI::SearchEnemy()
{
	return false;
}

bool MonsterAI::Drop()
{
	return false;
}

bool MonsterAI::OnTalk(Player* pPlayer)
{
	return false;
}

bool MonsterAI::OnItemBuy(Player*, uint8)
{
	return false;
}

bool MonsterAI::PushBackAllowed()
{
	return true;
}

bool MonsterAI::UpdateLocation()
{
	if ( !me()->SameDimension(me()->GetSummoner()) || !IN_RANGE(me(), me()->GetSummoner(), 14) || !me()->CheckWall(me()->GetSummoner()) )
	{
		me()->ClearPathData();

		coord_type x = me()->GetSummoner()->GetX() + 1;
		coord_type y = me()->GetSummoner()->GetY() - 1;

		me()->GetSummoner()->GenerateRandomLocation(me()->GetSummoner()->GetWorld(), x, y, 3, WORLD_ATTRIBUTE_LOCK_1 | WORLD_ATTRIBUTE_LOCK_2 | WORLD_ATTRIBUTE_STAND, me()->GetSummoner()->GetInstance());

		me()->TeleportToLocation(me()->GetSummoner()->GetWorldId(), x, y, me()->GetDirection(), me()->GetSummoner()->GetInstance());
		me()->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
		me()->SetTarget(nullptr);
		return true;
	}

	return false;
}

bool MonsterAI::NormalAttack()
{
	return false;
}

bool MonsterAI::MagicAttack()
{
	return false;
}
	
bool MonsterAI::SpecialMagicAttack()
{
	return false;
}

void MonsterAI::CreateTimer(uint32 id)
{
	TickTimer & timer = this->m_TimerMap[id];

	timer.Reset();
}

void MonsterAI::StartTimer(uint32 id, uint32 time)
{
	TickTimer & timer = this->m_TimerMap[id];

	timer.Start(time);
}

bool MonsterAI::IsTimerElapsed(uint32 id, uint32 time)
{
	TickTimer & timer = this->m_TimerMap[id];

	if ( time )
	{
		return timer.Elapsed(time);
	}
	else
	{
		return timer.Elapsed();
	}
}

void MonsterAI::PushBackPlayers(bool live = true)
{
	if (live && !sGameServer->IsBossStackCheck())
	{
		return;
	}
	
	Object* pObject = nullptr;

	VIEWPORT_LOOP_OBJECT(me(), pObject)

		if (!pObject->IsUnit())
			continue;

		if (pObject->IsInSafeZone())
			continue;

		if (pObject->GetX() != me()->GetX() || pObject->GetY() != me()->GetY())
		{
			continue;
		}

		if (pObject->IsCreature() && (!pObject->ToUnit()->GetSummoner() || !pObject->ToUnit()->GetSummoner()->IsPlayer()))
			continue;

		if (pObject->ToUnit()->IsTeleporting())
			continue;

		me()->PushBackCount(pObject->ToUnit(), 1);

	VIEWPORT_CLOSE
}