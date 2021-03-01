PathFinder::PathFinder()
{
	
}

PathFinder::~PathFinder()
{

}

void PathFinder::Create(World* pWorld)
{
	this->SetWorld(pWorld);
}

void PathFinder::SetHitMap(coord_type x, coord_type y, bool hit)
{
	if ( !IS_COORDINATE_RANGE(x) || !IS_COORDINATE_RANGE(y) )
	{
		return;
	}

	this->HitMap[WORLD_MAKE_GRID(x, y)] = hit;
}

int PathFinder::VerifyThatOnPath(coord_type x, coord_type y)
{
	for ( int32 i = 0; i < this->NumPath; ++i )
	{
		if ( x == this->PathX[i] && y == this->PathY[i] )
		{
			return i;
		}
	}
	return -1;
}

bool PathFinder::CanWeMoveForward(coord_type x, coord_type y, Unit* pUnit)
{
	if ( !IS_COORDINATE_RANGE(x) || !IS_COORDINATE_RANGE(y) )
	{
		return false;
	}

	if ( Monster* pMonster = pUnit->ToCreature() )
	{
		if ( !pMonster->IsMoveCoordinates(x, y) )
		{
			this->HitMap[WORLD_MAKE_GRID(x, y)] = 1;
			return false;
		}
	}
	else if ( Player* pPlayer = pUnit->ToPlayer() )
	{
		WorldGrid const& grid = this->GetWorld()->GetGrid(x, y);

		if ( grid.attribute )
		{
			this->HitMap[WORLD_MAKE_GRID(x, y)] = 1;
			return false;
		}
	}

	if ( this->HitMap[WORLD_MAKE_GRID(x, y)] != 0 )
	{
		this->HitMap[WORLD_MAKE_GRID(x, y)] = 1;
		return false;
	}
	
	return true;
};

bool PathFinder::IsThisSpotOK(coord_type x, coord_type y)
{
	if ( !IS_COORDINATE_RANGE(x) || !IS_COORDINATE_RANGE(y) )
	{
		return false;
	}

	if ( this->HitMap[WORLD_MAKE_GRID(x, y)] != 0 )
	{
		return false;
	}

	return true;
};

int PathFinder::FindNextDir(coord_type sx, coord_type sy, coord_type dx, coord_type dy, int dirstart, bool first, bool error_check, Unit* pUnit)
{
	int32 MinDist = 10000000;
	int32 ldir;
	int32 WhichDir = 0;

	if ( first )
	{
		for (int32 i = 0; i < 8; ++i )
		{
			ldir = i % 8;
			coord_type endx = sx + Path::Table[ldir * 2];
			coord_type endy = sy + Path::Table[ldir * 2 + 1];
			int32 dist = Util::Distance(endx, endy, dx, dy);

			if ( MinDist > dist )
			{
				if ( this->IsThisSpotOK(endx, endy) )
				{
					MinDist = dist;
					WhichDir = ldir;
				}
			}
		}
	}
	else
	{
		for (int32 i = dirstart + 7; i <= dirstart + 9; ++i )
		{
			ldir = i % 8;
			coord_type endx = sx + Path::Table[ldir * 2];
			coord_type endy = sy + Path::Table[ldir * 2 + 1];
			int32 dist = Util::Distance(endx, endy, dx, dy);

			if ( MinDist > dist )
			{
				if ( this->IsThisSpotOK(endx, endy) )
				{
					MinDist = dist;
					WhichDir = ldir;
				}
			}
		}

		if ( MinDist == 10000000 )
		{
			for (int32 i = dirstart + 2; i <= dirstart + 6; ++i )
			{
				ldir = i % 8;
				coord_type endx = sx + Path::Table[ldir * 2];
				coord_type endy = sy + Path::Table[ldir * 2 + 1];
				int32 dist = Util::Distance(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
	}

	this->LastDir = dirstart;
	int32 LastDist = Util::Distance(sx, sy, sx + Path::Table[this->LastDir * 2], sy + Path::Table[this->LastDir * 2 + 1]);

	if ( ( LastDist > MinDist - 6 ) && ( LastDist < MinDist + 6 ) && ( MinDist >= 25 ) )
	{
		WhichDir = this->LastDir;
	}

	if ( MinDist == 10000000 )
	{
		return -1;
	}
		
	if ( this->CanWeMoveForward(sx + Path::Table[WhichDir * 2], sy + Path::Table[WhichDir * 2 + 1], pUnit) )
	{
		int32 path = this->VerifyThatOnPath(sx, sy);

		if ( path != -1 )
		{
			this->SetHitMap(sx, sy, true);
			this->NumPath = path;
		}

		return WhichDir;
	}

	if ( error_check )
	{
		int32 dir = (WhichDir + 7) % 8;

		if ( this->CanWeMoveForward(sx + Path::Table[dir * 2], sy + Path::Table[dir * 2 + 1], pUnit) )
		{
			int32 path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->SetHitMap(sx, sy, true);
				this->NumPath = path;
			}

			return dir;
		}

		int32 dir2 = (WhichDir + 9) % 8;

		if ( this->CanWeMoveForward(sx + Path::Table[dir2 * 2], sy + Path::Table[dir2 * 2 + 1], pUnit) )
		{
			int32 path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->SetHitMap(sx, sy, true);
				this->NumPath = path;
			}

			return dir2;
		}
	}
		
	this->SetHitMap(sx, sy, true);
	return -1;
};

bool PathFinder::AssignPath(coord_type x, coord_type y)
{
	this->PathX[this->NumPath] = x;
	this->PathY[this->NumPath] = y;
	this->NumPath++;

	if ( this->NumPath >= MAX_WALK_PATH )
	{
		return false;
	}

	return true;
}

bool PathFinder::FindPath(coord_type startx, coord_type starty, coord_type endx,  coord_type endy, bool error_check, Unit* pUnit)
{
	this->NumPath = 0;
	int32 WhichDir = 0;
	bool first = true;
	int32 NumFails = 0;

	//LastDir = -1;

	/*for ( int32 i = 0; i < MAX_PATH_FIND; ++i )
	{
		PathX[i] = 0;
		PathY[i] = 0;
	}*/

	memset(this->HitMap, 0, WORLD_MAX_SIZE);

	while ( (startx != endx) || (starty != endy) )
	{
		WhichDir = this->FindNextDir(startx, starty, endx, endy, WhichDir, first, error_check, pUnit);
		first = false;
			
		if ( WhichDir >= 0 )
		{
			if ( !AssignPath(startx, starty) )
				return false;

			startx += Path::Table[WhichDir * 2];
			starty += Path::Table[WhichDir * 2 + 1];
		}
		else if ( error_check )
		{
			this->NumPath--;

			if ( this->NumPath < 0 )
			{
				return false;
			}

			startx = this->PathX[this->NumPath];
			starty = this->PathY[this->NumPath];

			NumFails++;

			if ( NumFails >= 10 )
			{
				return false;
			}
		}
		else
		{
			break;
		}
	}

	if ( !AssignPath(startx, starty) )
		return false;

	return true;
};

bool PathFinder::FindPath(Unit* pUnit)
{
	if ( pUnit->HasRestrictionBuff() )
		return false;

	uint8 bPath[8];

	bool Success = FindPath(pUnit->GetX(), pUnit->GetY(), pUnit->GetTempX(), pUnit->GetTempY(), true, pUnit);

	if ( !Success )
	{
		Success = FindPath(pUnit->GetX(), pUnit->GetY(), pUnit->GetTempX(), pUnit->GetTempY(), false, pUnit);
	}

	if ( Success && NumPath )
	{
		coord_type tx;
		coord_type ty;
		coord_type sx = pUnit->GetX();
		coord_type sy = pUnit->GetY();
		uint8 pos = 0;

		memset(bPath, 0, sizeof(bPath));
		bPath[0] = (pUnit->GetDirection() * 16) + ((NumPath - 1) & 0x0F);
		
		for ( int32 n = 1; n < NumPath; ++n )
		{
			tx = PathX[n];
			ty = PathY[n];
			pos = Unit::GetPathPacketDirPos(tx - sx, ty - sy);
			pUnit->GetPathData()->GetPosition(n)->SetX(tx);
			pUnit->GetPathData()->GetPosition(n)->SetY(ty);
			pUnit->GetPathData()->GetPosition(n)->SetDir(pos);
			sx = tx;
			sy = ty;

			if ( (n % 2) == 1 )
			{
				bPath[(n + 1) / 2] = (pos * 16);
			}
			else
			{
				bPath[(n + 1) / 2] |= pos;
			}
		}

		pUnit->GetPathData()->SetCount(NumPath);
		pUnit->GetMovePathTime()->Start();
		bPath[0] &= 0x0F;
		bPath[0] |= pos << 4;

		if ( Monster* pMonster = pUnit->ToCreature() )
		{
			pMonster->PathProcess(bPath);
		}
		else if ( Player* pPlayer = pUnit->ToPlayer() )
		{
			MOVE_REQUEST pMsg;
			pMsg.x = pPlayer->GetX();
			pMsg.y = pPlayer->GetY();
			memcpy(pMsg.path, bPath, 8);
			pPlayer->CharacterMove((uint8*)&pMsg);
		}
		return true;
	}

	return false;
}