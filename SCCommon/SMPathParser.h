#ifndef	ON_SMPATH_PARSER_HEADER
#define ON_SMPATH_PARSER_HEADER

#include <windows.h>
#include <directives.h>

struct	strShipMovePath
{
	SI32	siID;
	SI32	siNum;
	UI16	*puiShipMovePointArray;			
	POINT	ptDeparture;
	POINT	ptDepartureWarp;
	POINT	ptDestinationWarp;
	POINT	ptDestination;
};

class	cltShipMovePathParser
{
private:
	SI32				siShipMovePathNum;
	strShipMovePath		*pstShipMovePath;

private:
	SI32				FindIndex(SI32 siID);										// 해당 id의 인덱스를 찾는다.
	VOID				GetWarpPoint(POINT ptStart, POINT ptDest, POINT *pptWarp);	// 두 점 사이에서 워프할 곳을 찾는다.
public:
	cltShipMovePathParser();				// 생성자.
	~cltShipMovePathParser();				// 소멸자.

	BOOL				Load(CHAR *pszFileName);														// 화일을 로드한다.
	VOID				Free();																			// 메모리를 해제한다.	
	strShipMovePath*	GetPath(SI32 siID);																// 패스 배열의 포인터를 얻어온다.
};

#define	GETPATHID(x)	(MAKEWORD((CHAR*)(x)[0], atoi(&((CHAR*)x)[1])))

#endif
