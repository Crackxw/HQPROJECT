#ifndef	_ONLINE_ONILNEVILLAGESTRUCTDATA_HEADER
#define _ONLINE_ONILNEVILLAGESTRUCTDATA_HEADER

// ���ϸ� : OnlineVillageStructManager.h
// ����� : ������

#include "OnlineVillageBaseStructure.h"
#include "OnlineClient.h"

#define	ON_MAX_VIL_STRUCTNUM			10							// �� ���� ������ ���� �� �ִ� �ִ� �ǹ��� ��. 


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
	
	SI32							siKinds[ON_MAX_VIL_STRUCTNUM];										// ������ ������ �ִ� �ǹ����� ����
	UI16							uiVillageCode;														// ������ Code
	SI32							siKind;																// ������ ����
	BOOL							siSuccess;

	OnlineVillageBaseStructure		*pStructure;														// �� ���� ������ �� 10���� ������ ���� �� �ִ�.		
	
	cltOnlineWorld					*pMyOnlineWorld;													// ���� Ŭ����.

	// ������ ���� ���� ����Ÿ���� �����صδ� ����.
	ReceivedItems					RIItems;
	ReceivedINNChatPlayerInfo		RICPI;	

public:
	VillageStructManager();																				// ������.
	~VillageStructManager();																			// �Ҹ���.

	VOID	Init(UI16 VillageCode, SI32 psiKind[ON_MAX_VIL_STRUCTNUM], cltOnlineWorld *pOnlineWorld);	// �ʱ�ȭ.
	VOID	Free();																						// ����.
	
	inline	BOOL	Poll(LPDIRECTDRAWSURFACE7 pSurface);
	inline  BOOL	Action();

	VOID	EnterStructure(SI32 siStructureIndex, BOOL bField = FALSE);									// �ش� �ǹ� ������ ����.
	BOOL	IsEnterStructure();																			// � �ǹ������� ������ �˾ƿ´�.

	SI32	GetEnterStructureKind(void);																// ���� �� �ִ� ������ Kind�� ��´�.

	// ������ ���� ���� �޽�����.	
	VOID	ReceiveItemList(SI32 siItemNum, OnlineReceivedItemInfo *pOnlineReceivedItemInfo);			// ������ �ִ� �������� ����Ʈ�� �޾Ҵ�.


	VOID	FreeReceiveItemList();																		// ������ �ִ� �������� ����Ʈ�� �ʱ�ȭ �Ѵ�.

	SI32	GetKind()			{ return siKind; };														// ���� ������ �����Ѵ�.
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
