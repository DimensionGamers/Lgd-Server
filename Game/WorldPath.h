#ifndef ZZZPATH_H
#define ZZZPATH_H

#define MAX_PATH_FIND	MAX_WALK_PATH

class PathFinder
{
	private:
		int32 NumPath;
		int16 PathX[MAX_PATH_FIND];
		int16 PathY[MAX_PATH_FIND];
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
		void SetHitMap(int16 x, int16 y, bool hit);
		int VerifyThatOnPath(int16 x, int16 y);
		bool CanWeMoveForward(int16 x, int16 y, Unit* pUnit);
		bool IsThisSpotOK(int16 x, int16 y);
		int FindNextDir(int16 sx, int16 sy, int16 dx, int16 dy, int dirstart, bool first, bool error_check, Unit* pUnit);
		bool AssignPath(int16 x, int16 y);

	public:
		PathFinder();
		virtual ~PathFinder();

		void Create(World*);
		bool FindPath(int16 startx, int16 starty, int16 endx, int16 endy, bool error_check, Unit* pUnit);
		bool FindPath(Unit* pUnit);
};



/********************************************
			NEW PATH FINDING
********************************************/


#endif