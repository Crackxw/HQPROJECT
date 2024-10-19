#pragma once

#include <DDraw.h>
#include "Online.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlinePortrait.h"
//#include "OnlineMercenary.h"

class OnlineVillageBaseBarrack : public OnlineVillageBaseStructure
{
protected:
	OnlineVillageBaseBarrack() : OnlineVillageBaseStructure()	{}
	virtual	~OnlineVillageBaseBarrack()							{}

	virtual	VOID			Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)	{}
	virtual	VOID			Free()								{}
	virtual	BOOL			Poll(LPDIRECTDRAWSURFACE7 pSurface)	{ return OnlineVillageBaseStructure::Poll(pSurface); }
	virtual	BOOL			Action()							{ return FALSE; }

//	void	RenderCharacter( LPDIRECTDRAWSURFACE7 pSurface, UI16 Kind, SI16 X, SI16 Y, BOOL fGray, SI16 siLifeP, SI16 siMana )
	// actdoll (2004/01/10 13:31) : 랜더 캐릭터 대폭 수정. 이 함수는 초상을 아이템 형식으로 사용할 때 쓸 것이다.
	void	RenderCharacter( LPDIRECTDRAWSURFACE7 pSurface, SI32 X, SI32 Y, SI32 siDBID, SI32 siDrawFlag, BOOL bEnableStatusBar = false, R32 fHPRate = 0.0f, R32 fMPRate = 0.0f )
	{
//		pMyOnlineWorld->m_pMercenary->DrawPortait(X, Y, pMyOnlineWorld->m_pMercenary->TransKindToFont(Kind));
//		pMyOnlineWorld->m_pMercenary->DrawHpMp(X, Y, siLifeP, siMana);
		// actdoll (2004/01/02 19:00) : 초상 출력 수정
		if( !pSurface )		return;
		// 일단 초상 그려주고
		pMyOnlineWorld->m_pPortrait->DrawAsNormalEx	( pSurface, X, Y, siDBID, true, 60, 60 );
		// 스테이터스바 출력시
		if( bEnableStatusBar )
		{
			// HP가 없거나 있는데도 죽은 상태로 찍게끔 했다면
			if( fHPRate <= 0.0f || siDrawFlag & DF_FLAG_PORTRAIT_ICON_DIEOVER )	pMyOnlineWorld->m_pPortrait->Draw_Icons	( X, Y, DF_FLAG_PORTRAIT_ICON_DIEOVER );
			if( fHPRate > 0.0f )		// HP가 존재할 경우만 스테이터스바를 찍는다.
			{
				pMyOnlineWorld->m_pPortrait->Draw_StatusBar	( X + 1, Y + 52, 60 - 2, 4, fHPRate, true );
				pMyOnlineWorld->m_pPortrait->Draw_StatusBar	( X + 1, Y + 57, 60 - 2, 2, fMPRate, false );
			}
		}
		else
		{
			// 죽은 상태로 찍게끔 했다면
			if( siDrawFlag & DF_FLAG_PORTRAIT_ICON_DIEOVER )	pMyOnlineWorld->m_pPortrait->Draw_Icons	( X, Y, DF_FLAG_PORTRAIT_ICON_DIEOVER );
		}
		// 하이라이트 출력 시
		if( siDrawFlag & DF_FLAG_PORTRAIT_HIGHLIGHT )	pMyOnlineWorld->m_pPortrait->Draw_HighLight	( X - 3, Y - 3, 60 + 6, 60 + 6 );
		else											pMyOnlineWorld->m_pPortrait->Draw_HighLight	( X - 3, Y - 3, 60 + 6, 60 + 6, 0xF7 );
	}

	VOID	MakeName(UI32 Name, UI32 AddOption, BYTE* lpName, UI32 uiBuffSize )
	{
		char		TempBuffer[256];
		ZeroMemory( lpName, uiBuffSize );

		int	i = sizeof(lpName);
		sprintf((char*)TempBuffer, "%s", pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->pIOnlineCharKI->GetName(pMyOnlineWorld->pIOnlineCharKI->GetIndex(Name))));
		strncpy((char*)lpName, (char*)TempBuffer, ON_ID_LENGTH);
		lpName[strlen((char*)lpName)] = NULL;
	}
};
