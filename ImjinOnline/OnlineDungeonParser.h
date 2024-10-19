/*****************************************************************************************************************
	���ϸ�	:	OnlineDungeonParser.h
	�ۼ���  :	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.04.09
*****************************************************************************************************************/

#ifndef _ONLINE_DUNGEON_PARSER_H_
#define _ONLINE_DUNGEON_PARSER_H_

#include "OnlineText.h"

struct	DungeonHeader
{
	SI16	siCode;																	// Code
	
	SI16    siSelect;																// ������ or ����.

	char	szName[128];															// Name

	SI16	siMapIndex;																// Map Index
	SI16	siMapXPos;																// Map X Pos
	SI16	siMapYPos;																// Map Y Pos
	SI16	siBattlePoint;															// �ʿ�������
};


class OnlineDungeonParser
{
private:
	OnlineText				*pOnlineText;
	DungeonHeader			*pDungeonHeader;										// Dungeon�� ��� ������ ���� ����ü
	SI16					siDungeonNum;											// �� Dungeon
	
public:
	OnlineDungeonParser();
	~OnlineDungeonParser();

	BOOL					Init(OnlineText *pOnlineText);													// Dungeon �ε�
	VOID					Free();													// �����

	DungeonHeader*			GetDungeonHeader(SI16 siIndex);							// Get Dungeon Header
	UI16					GetDungeonNum();										// Dungeon �� ������ ��´�.
};

#endif