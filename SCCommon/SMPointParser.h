#ifndef	ON_SMPOINT_PARSER_HEADER
#define ON_SMPOINT_PARSER_HEADER

#include <windows.h>
#include <directives.h>

class	cltShipMovePointParser
{
private:
	SI32				siShipMovePointNum;
	POINT				*ptShipMovePoint;
public:
	cltShipMovePointParser();				// 생성자.
	~cltShipMovePointParser();				// 소멸자.

	BOOL				Load(CHAR *pszFileName);		// 화일을 로드한다.
	VOID				Free();							// 메모리를 해제한다.
	SI32				GetPointNum();					// 전체 포인트 개수를 얻어온다.
	POINT*				GetPoint();						// 포인트 배열의 포인터를 얻어온다.
};

#endif
