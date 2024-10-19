/*****************************************************************************************************************
	파일명	:	OnlineDungeonParser.h
	작성자  :	이 준 석
	담당자	:	이 준 석
	작성일	:	2001.04.09
*****************************************************************************************************************/

#ifndef _ONLINE_DUNGEON_PARSER_H_
#define _ONLINE_DUNGEON_PARSER_H_

#include "OnlineText.h"

struct	DungeonHeader
{
	SI16	siCode;																	// Code
	
	SI16    siSelect;																// 지름길 or 던전.

	char	szName[128];															// Name

	SI16	siMapIndex;																// Map Index
	SI16	siMapXPos;																// Map X Pos
	SI16	siMapYPos;																// Map Y Pos
	SI16	siBattlePoint;															// 필요전투력
};


class OnlineDungeonParser
{
private:
	OnlineText				*pOnlineText;
	DungeonHeader			*pDungeonHeader;										// Dungeon의 모든 정보를 가진 구조체
	SI16					siDungeonNum;											// 총 Dungeon
	
public:
	OnlineDungeonParser();
	~OnlineDungeonParser();

	BOOL					Init(OnlineText *pOnlineText);													// Dungeon 로드
	VOID					Free();													// 지운다

	DungeonHeader*			GetDungeonHeader(SI16 siIndex);							// Get Dungeon Header
	UI16					GetDungeonNum();										// Dungeon 총 개수를 얻는다.
};

#endif