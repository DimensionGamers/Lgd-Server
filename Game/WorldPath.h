#ifndef ZZZPATH_H
#define ZZZPATH_H

#define MAX_PATH_FIND	MAX_WALK_PATH

class PathFinder
{
	private:
		int32 NumPath;
		coord_type PathX[MAX_PATH_FIND];
		coord_type PathY[MAX_PATH_FIND];
		int32 LastDir;
		int32 NumFails;
		WorldGrid* Map;
		int32 stx;
		int32 sty;
		int32 edx;
		int32 edy;

		DECLARE_PTR(World, World);

		uint8 HitMap[WORLD_MAX_SIZE];
	private:
		void SetHitMap(coord_type x, coord_type y, bool hit);
		int VerifyThatOnPath(coord_type x, coord_type y);
		bool CanWeMoveForward(coord_type x, coord_type y, Unit* pUnit);
		bool IsThisSpotOK(coord_type x, coord_type y);
		int FindNextDir(coord_type sx, coord_type sy, coord_type dx, coord_type dy, int dirstart, bool first, bool error_check, Unit* pUnit);
		bool AssignPath(coord_type x, coord_type y);

	public:
		PathFinder();
		virtual ~PathFinder();

		void Create(World*);
		bool FindPath(coord_type startx, coord_type starty, coord_type endx, coord_type endy, bool error_check, Unit* pUnit);
		bool FindPath(Unit* pUnit);
};



/********************************************
			NEW PATH FINDING
********************************************/


#endif