#include <windows.h>
#include "Directives.h"

#ifndef _ONLINE_WARP_PARSER_H
#define _ONLINE_WARP_PARSER_H

#define	MAX_WARP_DEST_LENGTH		32			// 워프 목적지의 최대 텍스트 길이


// Code		Destination		Destination_Type	Location X	Location Y	Map index
struct strWarpInfo
{
	SI16		siCode;								// 번호
	CHAR		szDest[MAX_WARP_DEST_LENGTH+1];		// 지역 대상(설명글)
	SI32		siOnlineTextNo;						// 온라인 텍스트에서의 번호	// actdoll (2004/06/08 13:12) : 이번에 추가됨
	CHAR		cType;								// 국가 타입
	UI16		uiX;								// 좌표 x
	UI16		uiY;								// 좌표 y
	UI16		uiMapIndex;							// 맵 번호
};


class cltOnlineWarpParser
{
private:
	SI32			m_siInfoCount;				// 개수
	strWarpInfo		*m_pstWarpInfoList;			// 워프 정보 리스트의 포인터

public:
	cltOnlineWarpParser();
	~cltOnlineWarpParser();
	BOOL			Init( CHAR *pszFileName, OnlineText *pOnlineText );
	SI32			GetWarpCode( CHAR *szDest );		// 워프 목적지로 워프 코드를 얻어낸다.
	strWarpInfo*	GetWarpInfo( CHAR *szDest );		// 워프 목적지로 워프 정보를 얻어낸다.
	strWarpInfo*	GetWarpInfo( SI32 siCode );			// 워프 코드로 워프 정보를 얻어낸다.
	VOID			Free();
};

#endif