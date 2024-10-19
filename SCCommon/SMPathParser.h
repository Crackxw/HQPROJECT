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
	SI32				FindIndex(SI32 siID);										// �ش� id�� �ε����� ã�´�.
	VOID				GetWarpPoint(POINT ptStart, POINT ptDest, POINT *pptWarp);	// �� �� ���̿��� ������ ���� ã�´�.
public:
	cltShipMovePathParser();				// ������.
	~cltShipMovePathParser();				// �Ҹ���.

	BOOL				Load(CHAR *pszFileName);														// ȭ���� �ε��Ѵ�.
	VOID				Free();																			// �޸𸮸� �����Ѵ�.	
	strShipMovePath*	GetPath(SI32 siID);																// �н� �迭�� �����͸� ���´�.
};

#define	GETPATHID(x)	(MAKEWORD((CHAR*)(x)[0], atoi(&((CHAR*)x)[1])))

#endif
