#ifndef	__ONLINECHARPARSER_H
#define	__ONLINECHARPARSER_H

/********************************************************************************************
	파일명	:	OnlineChar-Parser.h
	작성자	:	정 진 욱
	담당자  :   이 상 원
	작성일	:	2001.06.30
	수정일	:	2001.12.24
********************************************************************************************/


#define ON_ATTRI_PLAYER				1
#define ON_ATTRI_MONSTER			2
#define ON_ATTRI_MERCENARY 			4

#define ON_MONSTER_TYPE1			1
#define ON_MONSTER_TYPE2			2
#define ON_MONSTER_TYPE3			3
#define ON_MONSTER_HALT				4



#pragma warning ( disable : 4786 )

#include	"ParametaBox.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct	CharHead
{
	UI16				uiID;						// 케릭터 아이디
	UI32				uiCharName;					// 케릭터 이름
	SI16				siAttri;
	UI16				uiMyExp;					// 사용 되지 않음
	SI16				siNation;					// 국가
	SI16				siMoveSpeed;				// 이동 속도
	SI32				siManaMax;	
	SI32				siCost;
	SI16				siLv;
	SI16				siCon;
	SI16				siNeedLv;					// 용병을 살수 있는 렙
	UI08				uiWeaponType;				// 사용 할수 있는 무기
	SI16				siNeedCredit;				// 용병을 구입할수 있는 신용도
	SI16				siItemDrop[4];				// 떨어 지는 아이템
	SI32				siDropPercent[4];			// 떨어 지는 확률
	SI32				siDescText;
	SI16				siGeneral;					// 일반용병(0), 장수(1), 몬스터(2)
	SI08				siMonsterType;				// 일반몬스터(1), 보스급(2)

	_ImportantParameta	Parameta;	

	const bool CharHead::operator < (const CharHead &lhs) const { return lhs.uiID < this->uiID ? true : false; };
	const bool CharHead::operator ==(const CharHead &lhs) const { return lhs.uiID == this->uiID ? true : false; };
};

class	OnlineCharParser
{
public:
	vector<CharHead>	m_CharHeader;
	typedef vector<CharHead>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;	

	SI16				siMonsterNum;																// 몬스터의 총 수
	SI16				siTotalCharNum;	
public:
	OnlineCharParser()								   { siMonsterNum = 0; };
	~OnlineCharParser()								   { Free(); };

	VOID				Free()						   { m_CharHeader.clear(); };					// 케릭터 구조체를 날린다.
	virtual BOOL		Init( CHAR *pCharFile );													// 케릭터 구조체에 데이타를 삽입
	CharHead			*GetCharHeader( SI16 siIndex ) { return &m_CharHeader[siIndex]; };			// 케릭터 헤더를 리턴 받는다.
	CharHead			*GetCharHeaderID( UI16 uiID );												// 케릭터 헤더를 리턴 받는다.
	SI16				GetCharTotalNum()			   { return m_CharHeader.size(); };				// 총 케릭터 수를 리턴 한다.
	SI16				GetMonsterTotalNum()		   { return siMonsterNum; };					// 총 몬스터 수를 리턴 한다.
	UI16				GetID( char *pToken );														// 케릭터의 아이디를 리턴 받는다.

protected:
	SI16				GetAttri( char *pToken );													// 케릭터가 몬스터 인가 플레이어 인가 알아 온다.
};
#endif