#ifndef	_ONLINE_ONILNEVILLAGESTRUCTDATA_HEADER
#define _ONLINE_ONILNEVILLAGESTRUCTDATA_HEADER

// 파일명 : OnlineVillageStructManager.h
// 담당자 : 정진욱

#include "OnlineVillageBaseStructure.h"
#include "OnlineClient.h"

#define	ON_MAX_VIL_STRUCTNUM			10							// 한 마을 내에서 있을 수 있는 최대 건물의 수. 


struct	ReceivedItems
{
	SI32						siItemNum;
	OnlineReceivedItemInfo		*pOnlineReceivedItemInfo;
};

struct	ReceivedINNChatPlayerInfo
{
	SI32						siPlayerNum;
	OnVilINNChatPlayerInfo		*pPlayerInfo;
};




class	VillageStructManager
{
private:
	
	SI32							siKinds[ON_MAX_VIL_STRUCTNUM];										// 마을이 가지고 있는 건물들의 종류
	UI16							uiVillageCode;														// 마을의 Code
	SI32							siKind;																// 마을의 종류
	BOOL							siSuccess;

	OnlineVillageBaseStructure		*pStructure;														// 한 마을 내에서 총 10개의 마을을 가질 수 있다.		
	
	cltOnlineWorld					*pMyOnlineWorld;													// 월드 클래스.

	// 서버로 부터 받은 데이타들을 저장해두는 변수.
	ReceivedItems					RIItems;
	ReceivedINNChatPlayerInfo		RICPI;	

public:
	VillageStructManager();																				// 생성자.
	~VillageStructManager();																			// 소멸자.

	VOID	Init(UI16 VillageCode, SI32 psiKind[ON_MAX_VIL_STRUCTNUM], cltOnlineWorld *pOnlineWorld);	// 초기화.
	VOID	Free();																						// 해제.
	
	inline	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);
	inline  BOOL	Action();

	VOID	EnterStructure(SI32 siStructureIndex, BOOL bField = FALSE);									// 해당 건물 안으로 들어간다.
	BOOL	IsEnterStructure();																			// 어떤 건물안으로 들어갔는지 알아온다.

	SI32	GetEnterStructureKind(void);																// 현재 들어가 있는 선물의 Kind를 얻는다.

	// 서버로 부터 받은 메시지들.	
	VOID	ReceiveItemList(SI32 siItemNum, OnlineReceivedItemInfo *pOnlineReceivedItemInfo);			// 서버에 있는 아이템의 리스트를 받았다.


	VOID	FreeReceiveItemList();																		// 서버에 있는 아이템의 리스트를 초기화 한다.

	SI32	GetKind()			{ return siKind; };														// 마을 종류를 리턴한다.
	VOID	*GetStructure()		{ return pStructure; };
};

inline BOOL VillageStructManager::Action()
{
	if( !pStructure )	return TRUE;
	if( pStructure->Action() )
	{
		FreeReceiveItemList();
		if(pStructure != NULL)
		{
			delete pStructure;
			pStructure	=	NULL;
			return TRUE;
		}
	}

	return FALSE;
}

inline BOOL VillageStructManager::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( !pStructure )	return TRUE;
	if( pStructure->Poll(pSurface) )
	{
		FreeReceiveItemList();
		if(pStructure != NULL)
		{
			delete pStructure;
			pStructure	=	NULL;
			return TRUE;
		}
	}

	return FALSE;
}

#endif
