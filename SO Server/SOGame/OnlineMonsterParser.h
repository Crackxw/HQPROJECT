#ifndef	__ONLINE_MONSTER_PARSER_H
#define	__ONLINE_MONSTER_PARSER_H


/*******************************************************************************************************
	파일명	:	OnlineMonsterParser.h
	작성자	:	정 진 욱
	작성일	:	2001.08.03
	수정일	:	2001.11.19
*******************************************************************************************************/


#define _ON_MAX_GROUP			7


struct	MonsterPatternHeader
{
	UI32					uiMonsterID[_ON_MAX_GROUP];										// 아이디
	SI16					siRate[_ON_MAX_GROUP];											// 비율
	UI16					uiMonsterLv[_ON_MAX_GROUP];										// 몬스터 레벨
	SI16					siX, siY;														// 좌표
	SI16					siMapIndex;
	SI16					siRadius;														// 범위
	SI16					siMaxKind;														// 몬스터 종류
	SI16					siGroupMaxQuantity, siGroupMinQuantity;
	SI16					siMaxQuantity;													// 몬스터 최대
	SI16					siMinQuantity;													// 몬스터 최소
	
};


class OnlineMonsterParser
{
private:
	SI32					siTotalPattern;													// 총 패턴수
	MonsterPatternHeader	*pMonsterPatternHeader;											// 패턴 헤더

public:
	OnlineMonsterParser();
	~OnlineMonsterParser();

	BOOL					Init( CHAR *pMonsterText );										// 몬스터 텍스를 읽는다.	
	VOID					Free();															// 날린다.

	SI32					GetTotalPattern();												// 총 몬스터 패턴을 리턴 한다.
	MonsterPatternHeader	*GetPatternHeader( UI32 uiIndex );								// 패턴 해더를 리턴한다.

private:
	UI32					GetID( CHAR *pStr );											// 문자열을 아디로 변환
};

#endif