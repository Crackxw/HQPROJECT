#ifndef		SOMARKETPLACE_H
#define		SOMARKETPLACE_H

#include "SOCommonHeader.h"
#include "SOUtility.h"
#include "OnlineItem.h"
#include "SOPlayer.h"
#include "OnlineVillage-Parser.h"
#include "OnlineTicket-Parser.h"
#include "OnlineCommonStruct.h"
#include "Stack.h"
#include "OnlineGameMsg.h"
#include "GSCDefine.h"
#include "OnlineMapStore-Parser.h"


struct	stBOOTHITEM
{
	MyItemData	*pItem;
	MyItemData	ItemInfo;
	MONEY		siPrice;	
	UI08		FollowerID;
	CHAR		Note[ON_MAX_BOOTH_NOTE+1];

	VOID	Init()
	{
		pItem			=	NULL;
		siPrice			=	0;
		FollowerID		=	0;
		memset(Note, 0, ON_MAX_BOOTH_NOTE);
	}
};

struct	stBOOTH
{
	SOPlayer				*pSeller;
	MapStoreHeader			*pHeader;
	stBOOTHITEM				BoothItem[ON_MAX_BOOTH_ITEM];
	BOOL					bIsActive;								// ���� ���� ������ Ȱ��ȭ �Ǿ��°�?

	//������
	stBOOTH()
	{
		pSeller		=	NULL;
		memset(BoothItem, 0, sizeof(stBOOTHITEM)*ON_MAX_BOOTH_ITEM);
	}
	//�Ҹ���
	~stBOOTH()
	{
		pSeller		=	NULL;
		memset(BoothItem, 0, sizeof(stBOOTHITEM)*ON_MAX_BOOTH_ITEM);
	}

	VOID	DelItemAll()
	{
		for(int i =0; i<ON_MAX_BOOTH_ITEM; i++ )
		{
			BoothItem[i].Init();
		}
	}

	BOOL	EnterBooth(SOPlayer *pPlayer)
	{
		if(pSeller == NULL)
		{
			if(pPlayer->siStatus & ON_PLAYER_STATUS_INFIELD)	//2���� ���Ǵ븦 �������� ���ϰ� �ϱ�����
			{
				pSeller	=	pPlayer;
				pPlayer->siStatus	=	ON_PLAYER_STATUS_BOOTH;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL	LeaveBooth(SOPlayer *pPlayer)
	{
		if(pSeller == pPlayer)
		{
			pSeller = NULL;
			DelItemAll();
			pPlayer->siStatus = ON_PLAYER_STATUS_MARKETPLACE;
			//pPlayer->SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH,SO_SFM_MEANDNEARCHAR,LPVOID(ON_RET_MARKETPLACE_EMPTY),LPVOID(pHeader->siMapStoreCode));	// �ֺ� ������� �������� ���ٰ� ������.
			//pPlayer->SendFieldMsg(ON_RESPONSE_DISPLAYBOOTH,SO_SFM_ONLYNEARCHAR,LPVOID(ON_RET_MARKETPLACE_EMPTY),LPVOID(pHeader->siMapStoreCode));
			return TRUE;
		}
		return FALSE;
	}

	BOOL	SetItem(UI08 uiNum, UI08 uiFollowerID, MyItemData *pItemData, MONEY Price)
	{
		if(Price>0)
		{
			BoothItem[uiNum].pItem			=	pItemData;
			BoothItem[uiNum].siPrice		=	Price;
			BoothItem[uiNum].FollowerID		=	uiFollowerID;
			return TRUE;
		}
		return FALSE;
	}

	CHAR*	GetNote(UI08 uiNum)
	{
		if(uiNum < ON_MAX_BOOTH_ITEM)
		{
			if( BoothItem[uiNum].Note[0] != 0 )
				return BoothItem[uiNum].Note;
		}
		return NULL;
	}

	BOOL	SetNote(UI08 uiNum, char *szNote)
	{
		if(uiNum < ON_MAX_BOOTH_ITEM)
		{
			memset(BoothItem[uiNum].Note, 0, ON_MAX_BOOTH_NOTE+1);
			memcpy( BoothItem[uiNum].Note, szNote, ON_MAX_BOOTH_NOTE );
			BoothItem[uiNum].Note[ON_MAX_BOOTH_NOTE] = '\0';
			return TRUE;
		}
		return FALSE;
	}
	BOOL	DelItem(UI08 uiNum)
	{
		if(uiNum < ON_MAX_BOOTH_ITEM)
		{
			BoothItem[uiNum].Init();
			return TRUE;
		}
		return FALSE;
	}
	stBOOTHITEM	*GetBoothItem(UI08 uiNum)
	{
		if(uiNum < ON_MAX_BOOTH_ITEM)
		{
			return &BoothItem[uiNum];
		}
		return NULL;
	}

	stBOOTHITEM	*SearchBoothItem(UI16 ItemID)
	{
		for(int i=0; i< ON_MAX_BOOTH_ITEM; i++)
		{
			if( BoothItem[i].pItem != NULL)
			{
				if( BoothItem[i].pItem->uiID	== ItemID )
					return &BoothItem[i];
			}
		}
		return NULL;
	}
	
};

class SOMarketPlace
{
public:
	SOWorld*				pWorld;
	SOVillage*				pVillage;
	stBOOTH*				pBooth;			// ���Ǵ�
	OnlineMapStoreParser	clBoothInfo;	// ���Ǵ� ������ �ִ� Text File


public:
	// ������
	SOMarketPlace()
	{
		pWorld		=	NULL;
		pBooth		=	NULL;
	}
	
	SOMarketPlace(SOWorld *clWorld, char *FileName)			// "Online\\Map\\MapStore.txt"
	{
		pWorld		=	clWorld;
		pBooth		=	NULL;
		if( clBoothInfo.ReadText(FileName) == TRUE )
		{
			if(clBoothInfo.siMapStoreNum > 0)
			{
				pBooth		=	new stBOOTH[clBoothInfo.siMapStoreNum];
				for(int i=0; i<clBoothInfo.siMapStoreNum; i++)
				{
					pBooth[i].pHeader	=	&clBoothInfo.pMapStoreHeader[i];
				}
				printf("����[Online\\Map\\MapStore.txt]�б� ����[���Ǵ� ����:%d]\n",clBoothInfo.siMapStoreNum);
			}
		}
		else
		{
			printf("����[Online\\Map\\MapStore.txt]�б� ����\n");
			getchar();
		}
	}

	// �Ҹ���
	~SOMarketPlace()
	{
		clBoothInfo.Free();
		pWorld		=	NULL;
		if( pBooth != NULL )
		{
			delete [] pBooth;
			pBooth	=	NULL;
		}
	}

	stBOOTH*	GetBooth(UI16 uiBoothID);
	BOOL		LeaveBooth(SOPlayer *pPlayer);
};

#endif		//SOMARKETPLACE_H