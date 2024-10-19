/************************************************************************************************
	파일명 : OnlineCharKI.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef _ONLINE_CHAR_KI_H
#define _ONLINE_CHAR_KI_H

#define	ON_CHAR_KI_CLASS_UNKNOWN	0		// 알 수 없는 캐릭터 클래스이다.
#define	ON_CHAR_KI_CLASS_PLAYER		1		// 주인공 캐릭터이다.
#define	ON_CHAR_KI_CLASS_MONSTER	2		// 몬스터 캐릭터이다.
#define	ON_CHAR_KI_CLASS_SOLDER		3		// 용병 캐릭터이다.
#define	ON_CHAR_KI_CLASS_SHIP		4		// 배이다.


#include <online.h>
#include "OnlineChar-Parser.h"

class	OnlineCharKI
{
private:
	UI16			uiCharID;	

	SI16			siMoveSpeed;	
	SI32			siHeightIndent;
	SI32			siFrameDelay;					// 프레임의 딜레이

	SI32			siClass;						// 캐릭터 클래스.
	SI32			siNation;						// 국가
	SI16			siNeedLevel;					// 용병 구입시 필요 레벨

	UI32			uiName;							// 캐릭터의 이름 변수이다.(플레이어가 아닌 몬스터의 경우에만 유효하다.)
	SI32			siDescText;						// 캐릭터 설명 Text
	UI08			uiWeaponType;					// 캐릭터가 사용 할 수 있는 무기 종류

	SI16			siGeneralFlag;					// 장수 Flag
	CharHead		sCharHeader;					// 캐릭터 정보(배같은경우 데이터가 없음)

	cltOnlineWorld	*pMyOnlineWorld;				// 온라인 월드의 클래스.

private:
	VOID			AnalysisCharID();				// 캐릭터의 ID를 분석하여 정보를 설정해준다.
public:
	OnlineCharKI();
	~OnlineCharKI();
	
	BOOL		Init(UI16 uicharID, SI32 Nation, UI32 uiname, SI32 siDescText, SI16 simovespeed, SI16 siReqLv, UI08 uiWeaponType, SI16 siGeneralFlag, CharHead* lpCharHeader, cltOnlineWorld *pOnlineWorld);// 초기화.	
	VOID		Free();																										// 메모리를 해제한다.
	UI16		GetCharID();																								// 애니메이션의 ID를 얻어온다.
	SI32		GetClass();																									// 클래스를 얻어온다.
	SI32		GetNation();																								// 국가를 얻어온다.
	UI32		GetName();																									// 몬스터의 이름을 얻어온다.
	SI32		GetDescText();																								// 캐릭터 설명 Text
	SI16		GetMoveSpeed();																								// Move Speed 를 얻어온다.
	SI16		GetNeedLevel();																								// 용병 구입시 필요 레벨
	UI08		GetWeaponType();																							// 캐릭터가 사용 할 수 있는 무기 타입
	SI16		GetGeneral();																								// 장수 Flag
	CharHead*	GetCharHeader();																							// 캐릭터 정보(배같은경우 데이터가 없음)
};	

#endif
