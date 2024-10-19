/************************************************************************************************
	파일명 : IOnlineCharKI.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef _ONLINE_CHAR_IKI_H
#define _ONLINE_CHAR_IKI_H

#include <Online.h>
#include <OnlineCharKI.h>		
#include "OnlineChar-Parser.h"

class	IOnlineCharKI
{
private:
	SI32			siTotalCharKINum;
	OnlineCharKI	*pOnlineCharKI;
public:
	IOnlineCharKI();
	~IOnlineCharKI();

	BOOL	Init(cltOnlineWorld *pOnlineWorld);				// 생성자.
	VOID	Free();											// 해제.	

	SI32		GetIndex(UI16 uiCharID);						// Char ID가 설정되어있는 Index를 얻어온다.
	UI16		GetCharID(SI32 siKindIndex);					// 애니메이션의 ID를 얻어온다.
	SI32		GetPlayerCharIDList(UI16 *pIndexArray);			// 주인공 캐릭터의 ID 인덱스를 얻어온다.
	UI32		GetName(SI32 siKindIndex);						// 몬스터의 경우 몬스터의 이름을 얻어온다.
	SI32		GetDescText(SI32 siKindIndex);					// 캐릭터 설명 Text
	SI32		GetClass(SI32 siKindIndex);						// 캐릭터의 클래스를 얻어온다.
	SI32		GetNation(SI32 siKindIndex);					// 캐릭터의 Nation를 얻어온다.
	SI16		GetMoveSpeed(SI32 siKindIndex);					// Move Speed 를 얻어온다.
	BOOL		IsPlayer(UI16 uiCharID);						// 해당 아디가 주인공인지 알아 본다.
	BOOL		IsSoldier(UI16 uiCharID);
	UI08		GetWeaponType(UI16 uiCharID);					// 무기 타입를 얻어온다.
	SI16		GetGeneral(UI16 uiCharID);						// 장수 Flag
	CharHead*	GetCharHeader(UI16 uiCharID);					// 캐릭터 정보(배같은경우 데이터가 없음)
};


#endif
