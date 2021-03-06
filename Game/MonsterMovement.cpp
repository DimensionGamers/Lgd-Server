/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterMovement.cpp"
*
*/

void Monster::UpdateMovement()
{
	if ( this->IsStopAction() )
	{
		this->ClearPathData();
		return;
	}

	if ( this->HasTalkReference() )
	{
		if ( this->GetPathData()->GetCount() != 0 )
		{
			this->ClearPathData();
		}

		return;
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return;

	if ( this->GetAI() && !this->GetAI()->UpdateMovement() )
		return;

	uint32 MoveTime = 0;

	if ( this->GetPathData()->GetCount() != 0 && !this->HasRestrictionBuff() )
	{
		if ( this->GetPathData()->GetCurrent() >= 14 )
		{
			return;
		}

		int32 DelayTime = this->HasSlowdownBuff() ? 300: 0;

		if ( this->IsSummoned() )
		{
			this->SetIntData(UNIT_INT_MOVE_SPEED, 200);
		}
		else if ( this->GetCurrentAI() )
		{
			this->SetIntData(UNIT_INT_MOVE_SPEED, 300);
		}
		else if ( DG_MAP_RANGE(this->GetWorldId()) )
		{
			this->SetIntData(UNIT_INT_MOVE_SPEED, sGameServer->GetDoppelgangerMonsterSpeed());
		}
		else
		{
			this->SetIntData(UNIT_INT_MOVE_SPEED, 400);
		}
		
		PathPosition const* Position = this->GetPathData()->GetPosition(this->GetPathData()->GetCurrent());

		if( Position->GetDir() % 2 == 0 )
		{
			MoveTime = (uint32)((this->GetIntData(UNIT_INT_MOVE_SPEED) + DelayTime) * 1.3f);
		}
		else
		{
			MoveTime = this->GetIntData(UNIT_INT_MOVE_SPEED) + DelayTime;
		}

		if ( this->HasBuff(BUFF_PARALYZE) || this->HasBuff(BUFF_PARALYZE_IMPROVED) )
		{
			MoveTime += MoveTime * 50 / 100;
		}

		if( this->GetMovePathTime()->Elapsed(MoveTime) )
		{
			this->SetX(Position->GetX());
			this->SetY(Position->GetY());
			this->SetDirection(Position->GetDir());
			this->GetPathData()->IncreaseCurrent(1);
			this->SetLastMoveTime(MyGetTickCount());
			this->SetLastActionTime(MyGetTickCount());

			if ( !sGameServer->IsNormalMonsterMovement() )
			{
				this->MoveSend(this->GetX(), this->GetY(), this->GetDirection() << 4);
			}

			if ( this->GetPathData()->GetCurrent() >= this->GetPathData()->GetCount() )
			{
				this->GetPathData()->SetCurrent(0);
				this->GetPathData()->SetCount(0);
				this->GetPathData()->SetStartEnd(false);
			}
		}
	}
}

bool Monster::FollowOwner()
{
	if ( this->GetSummoner() && this->GetSummoner()->IsPlaying() && !this->GetSummoner()->IsRest() && this->GetSummoner() != this )
	{
		int16 tx = 0;
		int16 ty = 0;

		if ( this->GetOwnerPosition(tx, ty) )
		{
			this->GetAction()->Emotion = EMOTION_REST;
			this->SetTempX(tx);
			this->SetTempY(ty);
			this->GetAction()->Move = 1;
			this->SetNextActionTime(1000);
			return true;
		}
	}

	return false;
}

bool Monster::GetOwnerPosition(int16 &tx, int16 &ty)
{
	int32 dis = Viewport::MAX_DISTANCE;

	int16 tpx = this->GetSummoner()->GetX();
	int16 tpy = this->GetSummoner()->GetY();

	int16 mtx = this->GetSummoner()->GetX();
	int16 mty = this->GetSummoner()->GetY();

	if ( this->GetX() < mtx )
	{
		tpx -= dis;
	}

	if ( this->GetX() > mtx )
	{
		tpx += dis;
	}

	if ( this->GetY() < mty )
	{
		tpy -= dis;
	}

	if ( this->GetY() > mty )
	{
		tpy += dis;
	}
	
	int16 searchp = GetPathPacketDirPos(mtx - tpx, mty - tpy) * 2;

	if ( this->IsMoveCoordinates(tpx, tpy) )
	{
		PATH_LOOP
		{
			mtx = GetSummoner()->GetX() + Path::Table[searchp];
			mty = GetSummoner()->GetY() + Path::Table[searchp + 1];

			if ( this->IsMoveCoordinates(mtx, mty) )
			{
				tx = mtx;
				ty = mty;
				return true;
			}

			Path::FixSearch(searchp);
		}

		tx = tpx;
		ty = tpy;
		return true;
	}

	return false;
}

bool Monster::GetTargetPosition(int16 sx, int16 sy, int16 &tx, int16 &ty)
{
	int32 dis = _monsterTemplate->AttackRange;

	int16 tpx = sx;
	int16 tpy = sy;

	int16 mtx = tpx;
	int16 mty = tpy;

	if ( this->GetX() < mtx )
	{
		tpx -= dis;
	}

	if ( this->GetX() > mtx )
	{
		tpx += dis;
	}

	if ( this->GetY() < mty )
	{
		tpy -= dis;
	}

	if ( this->GetY() > mty )
	{
		tpy += dis;
	}

	int16 searchp = GetPathPacketDirPos(sx - tpx, sy - tpy) * 2;

	if ( this->IsMoveCoordinates(tpx, tpy) )
	{
		PATH_LOOP
		{
			mtx = sx + Path::Table[searchp];
			mty = sy + Path::Table[searchp + 1];

			if ( this->IsMoveCoordinates(mtx, mty) )
			{
				tx = mtx;
				ty = mty;
				return true;
			}

			Path::FixSearch(searchp);
		}

		tx = tpx;
		ty = tpy;
		return true;
	}

	return false;
}

bool Monster::GetTargetPosition()
{
	Unit* pTarget = this->GetTarget();

	if ( !pTarget )
		return false;

	if (_monsterTemplate->MovementRange <= 0 || this->HasRestrictionBuff())
		return false;

	if ( pTarget->IsInTeleportInmuneState() )
		return false;

	if ( !this->ViewportIsCharacter(pTarget) )
		return false;

	int16 tpx = pTarget->GetX();
	int16 tpy = pTarget->GetY();

	int16 mtx = tpx;
	int16 mty = tpy;

	int32 dis = _monsterTemplate->AttackRange;

	if ( this->GetX() < mtx )
	{
		tpx -= dis;
	}

	if ( this->GetX() > mtx )
	{
		tpx += dis;
	}

	if ( this->GetY() < mty )
	{
		tpy -= dis;
	}

	if ( this->GetY() > mty )
	{
		tpy += dis;
	}

	int16 searchp = GetPathPacketDirPos(pTarget->GetX() - tpx, pTarget->GetY() - tpy) * 2;

	if ( this->IsMoveCoordinates(tpx, tpy) )
	{
		PATH_LOOP
		{
			mtx = pTarget->GetX() + Path::Table[searchp];
			mty = pTarget->GetY() + Path::Table[searchp + 1];

			if ( this->SetMoveCoordinatesNormal(mtx, mty) )
				return true;

			Path::FixSearch(searchp);
		}

		this->SetTempX(tpx);
		this->SetTempY(tpy);
		return true;
	}

	return false;
}

void Monster::MoveAttempt()
{
	if ( this->GetState() != OBJECT_STATE_READY )
	{
		return;
	}

	if ( this->HasRestrictionBuff() )
	{
		return;
	}

	AI_CONTROL(MoveAttempt());

	int32 maxmoverange = _monsterTemplate->MovementRange * 2 + 1;
	this->SetNextActionTime(1000);
	int16 tpx;
	int16 tpy;

	for ( int32 i = 0; i < 10; ++i )
	{
		tpx = (this->GetX() - _monsterTemplate->MovementRange) + Random(maxmoverange);
		tpy = (this->GetY() - _monsterTemplate->MovementRange) + Random(maxmoverange);

		FIX_COORD(tpx);
		FIX_COORD(tpy);

		if ( this->IsMoveCoordinates(tpx, tpy) )
		{
			this->SetMoveCoordinates(tpx, tpy);

			this->SetTarget(nullptr);
			this->GetAction()->Attack = 0;
			break;
		}
	}
}

bool Monster::MoveCheck(int16 x, int16 y)
{
	if ( this->GetAction()->Emotion == EMOTION_ATTACK )
	{
		return true;
	}

	Unit* pUnit = this->GetSummoner();

	if ( pUnit && pUnit != this )
	{
		x -= pUnit->GetX();
		y -= pUnit->GetY();
	}
	else
	{
		x -= this->GetRegenLocation()->GetX();
		y -= this->GetRegenLocation()->GetY();
	}

	int32 dis = static_cast<int32>(sqrt(double(x * x + y * y)));

	return dis <= this->GetMoveDistance();
}

bool Monster::IsOutOfMoveRange()
{
	int16 x = this->GetX();
	int16 y = this->GetY();

	x -= this->GetRegenLocation()->GetX();
	y -= this->GetRegenLocation()->GetY();

	int32 dis = static_cast<int32>(sqrt(double(x * x + y * y)));

	return dis > this->GetMoveDistance();
}

bool Monster::PathFindToMove()
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
		return false;

	bool result = pWorld->GetPathFinderManager()->FindPath(this);

	if ( !result )
	{
		this->ClearPathData();
	}

	return result;
}

void Monster::PathProcess(uint8 * path)
{
	if ( !this->CommonMovementCheck() )
	{
		this->ClearPathData();
		return;
	}

	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		this->ClearPathData();
		return;
	}

	if (this->IsDelayedTeleport())
		this->CancelDelayedTeleport();

	this->SetRest(0);

	if ( this->GetPathData()->GetCount() > MAX_WALK_PATH )
	{
		this->ClearPathData();
		return;
	}

	for ( int32 i = 0; i < MAX_WALK_PATH; ++i )
	{
		this->GetPathData()->GetPosition(i)->Reset();
	}
	
	int16 ax = GetX();
	int16 ay = GetY();
	int16 pathtable = 0;
	this->GetPathData()->GetPosition(0)->SetX(this->GetX());
	this->GetPathData()->GetPosition(0)->SetY(this->GetY());
	this->GetPathData()->GetPosition(0)->SetDir(this->GetDirection());
	this->GetPathData()->SetCurrent(1);
	this->GetPathData()->SetStartEnd(true);

	for ( int32 n = 1; n < this->GetPathData()->GetCount(); ++n )
	{
		if ( (n % 2) == 1 )
		{
			pathtable = path[(n + 1) / 2] >> 4;
		}
		else
		{
			pathtable = path[(n + 1) / 2] & 0x0F;
		}

		limitmin(pathtable, 0);
		limitmax(pathtable, Path::Max - 1);

		ax += Path::Table[pathtable * 2];
		ay += Path::Table[pathtable * 2 + 1];
		
		this->GetPathData()->GetPosition(n)->SetX(ax);
		this->GetPathData()->GetPosition(n)->SetY(ay);
		this->GetPathData()->GetPosition(n)->SetDir(pathtable);
		this->GetPathData()->GetPosition(n - 1)->SetOri(pathtable);
	}

	if ( this->GetPathData()->GetCount() > 0 )
	{
		int16 nextX = this->GetPathData()->GetPosition(1)->GetX();
		int16 nextY = this->GetPathData()->GetPosition(1)->GetY();

		WorldGrid const& attr = pWorld->GetGrid(nextX, nextY);

		if ( attr.IsLocked_1() || attr.IsLocked_2() )
		{
			this->ClearPathData();
			return;
		}
	}

	this->SetTX(ax);
	this->SetTY(ay);

	if ( sGameServer->IsNormalMonsterMovement() )
		this->MoveSend(ax, ay, this->GetDirection() << 4);

	this->SetViewState(0);
}

void Monster::MoveToLocation(int16 x, int16 y, bool target)
{
	this->SetTempX(x);
	this->SetTempY(y);
	this->PathFindToMove();
}

void Monster::TeleportBack()
{

}
	
bool Monster::MoveBack()
{
	if ( this->GetPathData()->GetCount() != 0 )
		return false;

	this->SetTempX(this->GetRegenLocation()->GetX());
	this->SetTempY(this->GetRegenLocation()->GetY());
	return this->PathFindToMove();
}

bool Monster::GetXYToPatrol()
{
	int32 maxmoverange = _monsterTemplate->MovementRange * 2 + 1;
	this->SetNextActionTime(1000);
	int16 tpx = this->GetX();
	int16 tpy = this->GetY();

	for ( int32 i = 0; i < 10; ++i )
	{
		tpx = (this->GetX() - _monsterTemplate->MovementRange) + Random(maxmoverange);
		tpy = (this->GetY() - _monsterTemplate->MovementRange) + Random(maxmoverange);

		if ( this->SetMoveCoordinatesNormal(tpx, tpy) )
			return true;
	}

	return false;
}
	
bool Monster::GetXYToEscape()
{
	Unit* pTarget = this->GetTarget();

	if ( !pTarget )
		return false;

	int16 tpx = pTarget->GetX();
	int16 tpy = pTarget->GetY();
	int16 mtx = tpx;
	int16 mty = tpy;
	int16 tx = GetX();
	int16 ty = GetY();
	int32 dis = _monsterTemplate->AttackRange / sqrt(2.0);

	Path::FixCoordinate(tx, mtx, dis);
	Path::FixCoordinate(ty, mty, dis);

	int16 searchp = GetPathPacketDirPos(GetX() - tx, GetY() - tx) * 2;

	if ( this->IsMoveCoordinates(tx, ty) )
	{
		PATH_LOOP
		{
			mtx = GetX() + Path::Table[searchp];
			mty = GetY() + Path::Table[searchp + 1];

			if ( this->SetMoveCoordinatesNormal(mtx, mty) )
				return true;

			Path::FixSearch(searchp);
		}

		this->SetTempX(tx);
		this->SetTempY(ty);
		return true;
	}

	return false;
}
	
bool Monster::GetXYToChase()
{
	Unit* pTarget = this->GetTarget();

	if ( !pTarget )
		return false;

	int16 tpx = pTarget->GetX();
	int16 tpy = pTarget->GetY();
	int16 mtx = tpx;
	int16 mty = tpy;

	int32 dis = _monsterTemplate->AttackRange / sqrt(2.0);

	if ( GetX() < mtx )
	{
		tpx -= dis;
	}

	if ( GetX() > mtx )
	{
		tpx += dis;
	}

	if ( GetY() < mty )
	{
		tpy -= dis;
	}

	if ( GetY() > mty )
	{
		tpy += dis;
	}

	int16 searchp = GetPathPacketDirPos(pTarget->GetX() - tpx, pTarget->GetY() - tpy) * 2;

	if ( this->IsMoveCoordinates(tpx, tpy) )
	{
		PATH_LOOP
		{
			mtx = pTarget->GetX() + Path::Table[searchp];
			mty = pTarget->GetY() + Path::Table[searchp + 1];

			if ( this->SetMoveCoordinatesNormal(mtx, mty) )
				return true;

			Path::FixSearch(searchp);
		}

		this->SetTempX(tpx);
		this->SetTempY(tpy);
		return true;
	}

	return false;
}

void Monster::SetMoveCoordinates(int16 x, int16 y)
{
	this->GetAction()->Emotion = EMOTION_REST;
	this->SetTempX(x);
	this->SetTempY(y);
	this->GetAction()->Move = 1;
	this->SetNextActionTime(500);
}

bool Monster::SetMoveCoordinatesNormal(int16 x, int16 y)
{
	if ( this->IsMoveCoordinates(x, y) )
	{
		this->SetTempX(x);
		this->SetTempY(y);
		return true;
	}

	return false;
}

bool Monster::IsMoveCoordinates(int16 x, int16 y)
{
	World* pWorld = this->GetWorld();

	if ( !pWorld )
	{
		return false;
	}

	if ( this->MoveCheck(x, y) )
	{
		if ( this->GetAI() )
		{
			if ( this->GetAI()->MoveAllowed(x, y) )
			{
				return true;
			}
		}
		else
		{
			WorldGrid const& grid = pWorld->GetGrid(x, y);

			if ( !grid.IsSafe() && !grid.IsLocked_1() && !grid.IsLocked_2() )
			{
				return true;
			}
		}
	}

	return false;
}