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
		coord_type tx = 0;
		coord_type ty = 0;

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

bool Monster::GetOwnerPosition(coord_type &tx, coord_type &ty)
{
	int32 dis = Viewport::MAX_DISTANCE;

	coord_type tpx = this->GetSummoner()->GetX();
	coord_type tpy = this->GetSummoner()->GetY();

	coord_type mtx = this->GetSummoner()->GetX();
	coord_type mty = this->GetSummoner()->GetY();

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

bool Monster::GetTargetPosition(coord_type sx, coord_type sy, coord_type &tx, coord_type &ty)
{
	int32 dis = this->GetAttackRange();

	coord_type tpx = sx;
	coord_type tpy = sy;

	coord_type mtx = tpx;
	coord_type mty = tpy;

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

	if ( this->GetMoveRange() <= 0 || this->HasRestrictionBuff() )
		return false;

	if ( pTarget->IsInTeleportInmuneState() )
		return false;

	if ( !this->ViewportIsCharacter(pTarget) )
		return false;

	coord_type tpx = pTarget->GetX();
	coord_type tpy = pTarget->GetY();

	coord_type mtx = tpx;
	coord_type mty = tpy;

	int32 dis = this->GetAttackRange();

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

	int32 maxmoverange = this->GetMoveRange() * 2 + 1;
	this->SetNextActionTime(1000);
	coord_type tpx;
	coord_type tpy;

	for ( int32 i = 0; i < 10; ++i )
	{
		tpx = (this->GetX() - this->GetMoveRange()) + Random(maxmoverange);
		tpy = (this->GetY() - this->GetMoveRange()) + Random(maxmoverange);

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

bool Monster::MoveCheck(coord_type x, coord_type y)
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
	coord_type x = this->GetX();
	coord_type y = this->GetY();

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
	
	coord_type ax = GetX();
	coord_type ay = GetY();
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
		coord_type nextX = this->GetPathData()->GetPosition(1)->GetX();
		coord_type nextY = this->GetPathData()->GetPosition(1)->GetY();

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

void Monster::MoveToLocation(coord_type x, coord_type y, bool target)
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
	int32 maxmoverange = this->GetMoveRange() * 2 + 1;
	this->SetNextActionTime(1000);
	coord_type tpx = this->GetX();
	coord_type tpy = this->GetY();

	for ( int32 i = 0; i < 10; ++i )
	{
		tpx = (this->GetX() - this->GetMoveRange()) + Random(maxmoverange);
		tpy = (this->GetY() - this->GetMoveRange()) + Random(maxmoverange);

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

	coord_type tpx = pTarget->GetX();
	coord_type tpy = pTarget->GetY();
	coord_type mtx = tpx;
	coord_type mty = tpy;
	coord_type tx = GetX();
	coord_type ty = GetY();
	int32 dis = this->GetAttackRange() / sqrt(2.0);

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

	coord_type tpx = pTarget->GetX();
	coord_type tpy = pTarget->GetY();
	coord_type mtx = tpx;
	coord_type mty = tpy;

	int32 dis = this->GetAttackRange() / sqrt(2.0);

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

void Monster::SetMoveCoordinates(coord_type x, coord_type y)
{
	this->GetAction()->Emotion = EMOTION_REST;
	this->SetTempX(x);
	this->SetTempY(y);
	this->GetAction()->Move = 1;
	this->SetNextActionTime(500);
}

bool Monster::SetMoveCoordinatesNormal(coord_type x, coord_type y)
{
	if ( this->IsMoveCoordinates(x, y) )
	{
		this->SetTempX(x);
		this->SetTempY(y);
		return true;
	}

	return false;
}

bool Monster::IsMoveCoordinates(coord_type x, coord_type y)
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