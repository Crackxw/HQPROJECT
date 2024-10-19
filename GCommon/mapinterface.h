#ifndef _ROCKET_NEW_TILE_MAP

#ifndef _MAPINTERFACE_H
#define _MAPINTERFACE_H


class cltMapInterface {
public:

	cltMapInterface();

	SI16 GetIDMap(SI32 siX, SI32 siY);
	SI16 GetSkyMap(SI32 siX, SI32 siY);

	void TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny);
	BOOL GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* x, SHORT* y);

	SI16 GetMapTileXsize();
	SI16 GetMapTileYsize();
	SI16 GetMapTileXsizeHalf();
	SI16 GetMapTileYsizeHalf();
	SI16 GetMaxDrawMapScreenXsize() ;
	SI16 GetMaxDrawMapScreenYsize() ;

	SHORT GetSector(SHORT x, SHORT y);
	void SetVirtualPos(SI16 siX, SI16 siY);
	void SetDrawForceSwitch(BOOL bMode);

	BOOL IsInMapArea(SI16 siX, SI16 siY);
	BOOL IsInScreen(SI16 siX, SI16 siY);

	BOOL IsEmpty(SI16 siX, SI16 siY);

	SI16 GetCropTypeMap(SI16 siX, SI16 siY);
	VOID PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount);

	SI16 GetXsize();
	SI16 GetYsize();

	BOOL IsLand(SI16 siX, SI16 siY);

	UI08 GetNewBlkMode(SI16 siX, SI16 siY);
	SI16 GetLevelInfo(SI16 siX, SI16 siY);

	SI16 GetMapType();
	void InitTile();

	SHORT GetPlayerNumberOfMap(char* filename);
	BOOL IsColl(SI16 siX, SI16 siY, SI16 siBlock);

	BOOL GetRealDotCenterXY(SI16 x, SI16 y, int & centerx, int & centery);


	UI08 GetShallowMap(SI16 siX, SI16 siY);

	BOOL CanBuild(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT kind);
	BOOL CanBuildByKind(SHORT x, SHORT y, SHORT kind, _NewID builderid);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ ���� ���δ�.
	// Desc :
	//-----------------------------------------------------------------------------
	void PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer = -1);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� �÷��̾��� Ư�� ���� ĳ���Ͱ� ���� �� ���갡���Ѱ� ?
	// Desc :
	//-----------------------------------------------------------------------------
	BOOL IsCharInGame(SI32 playernumber, SI32 kind);



};
#endif



#else

#ifndef _MAPINTERFACE_H
#define _MAPINTERFACE_H


class cltMapInterface {
public:

	cltMapInterface();

	SI16 GetIDMap(SI32 siX, SI32 siY);
	SI16 GetSkyMap(SI32 siX, SI32 siY);

	void TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny);
	BOOL GetRealTileByRealDot(SHORT realx, SHORT realy, SHORT* x, SHORT* y);

	SI16 GetMapTileXsize();
	SI16 GetMapTileYsize();
	SI16 GetMapTileXsizeHalf();
	SI16 GetMapTileYsizeHalf();
	SI16 GetMaxDrawMapScreenXsize() ;
	SI16 GetMaxDrawMapScreenYsize() ;

	SHORT GetSector(SHORT x, SHORT y);
	void SetVirtualPos(SI16 siX, SI16 siY);
	void SetDrawForceSwitch(BOOL bMode);

	BOOL IsInMapArea(SI16 siX, SI16 siY);
	BOOL IsInScreen(SI16 siX, SI16 siY);

	BOOL IsEmpty(SI16 siX, SI16 siY);

	SI16 GetCropTypeMap(SI16 siX, SI16 siY);
	VOID PutCrop(SHORT x, SHORT y, SHORT type, SHORT vary, SHORT mount, SHORT TreeType);	

	SI16 GetXsize();
	SI16 GetYsize();

	BOOL IsLand(SI16 siX, SI16 siY);

	UI08 GetNewBlkMode(SI16 siX, SI16 siY);
	SI16 GetLevelInfo(SI16 siX, SI16 siY);

	SI16 GetMapType();
	void InitTile();

	SHORT GetPlayerNumberOfMap(char* filename);
	BOOL IsColl(SI16 siX, SI16 siY, SI16 siBlock);

	BOOL GetRealDotCenterXY(SI16 x, SI16 y, int & centerx, int & centery);


	UI08 GetShallowMap(SI16 siX, SI16 siY);

	BOOL CanBuild(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT kind);
	BOOL CanBuildByKind(SHORT x, SHORT y, SHORT kind, _NewID builderid);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� ������ ���� ���δ�.
	// Desc :
	//-----------------------------------------------------------------------------
	void PutFire(SI32 siX, SI32 siY, SI32 siFireType, SI32 siLifeTime,_NewID siChar,SI16 siPlayer = -1);

	//-----------------------------------------------------------------------------
	// ���� : Ư�� �÷��̾��� Ư�� ���� ĳ���Ͱ� ���� �� ���갡���Ѱ� ?
	// Desc :
	//-----------------------------------------------------------------------------
	BOOL IsCharInGame(SI32 playernumber, SI32 kind);



};
#endif
#endif