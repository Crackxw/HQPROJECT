/************************************************************************************************
	���ϸ� : IOnlineFollower.cpp
	����� : ������
	��  �� : �̼��� (������ ���û��� ó���ϸ鼭 �ٲ��.)
	���ϵ��� ��� ������ �����Ѵ�.
************************************************************************************************/
#include <GSL.h>
#include <limits.h>
#include "OnlineItem.h"
#include "IOnlineFollower.h"
#include "Mmsystem.h"
#include "OnlineChar-Parser.h"




CItem*				OnlineFollower::pCItem;								// Item Class.

//***********************************************************************************************
//	OnlineFollower ����ü
//***********************************************************************************************
OnlineFollower::OnlineFollower()
{
	Init();
}

OnlineFollower::~OnlineFollower()
{
	Init();
}

VOID	OnlineFollower::Init()
{
	// ID�� �ʱ�ȭ �Ѵ�.
	uiCharID	=	0;	
	// �������� �ʱ�ȭ �Ѵ�.	
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		Items[i].siPosInInventory	=	i;
		Items[i].uiID				=	0;
		Items[i].uiQuantity			=	0;
	}
	ZeroMemory(Name, sizeof(Name));
	// ���⸦ �ʱ�ȭ �Ѵ�.
	Weapon.Init();

	dwSaveTime = timeGetTime();

	dwTotalItemWeight	=	0;
}

MyItemData*		OnlineFollower::GetItem(UI16 uiItemID)	// �������� ���´�.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return &Items[i];
	}
	return NULL;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID)	// �������� ���´�.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			return i;
	}
	return -1;
}

SI32		OnlineFollower::GetItemIndex(UI16 uiItemID,UI16 uiQuantity)	// �������� ���´�.
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == uiItemID)
			if (Items[i].uiQuantity >= uiQuantity)
			return i;
	}
	return -1;
}



MyItemData*		OnlineFollower::GetEmptyItem()
{
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == 0)
			return &Items[i];
	}
	return NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�־��� �縸ŭ�� ���԰� �߰� �ɼ� �ִ��� �˻�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::CheckWeight(UI32 Weight)
{
	return ParametaBox.GetMaxWeight() >= (ParametaBox.GetCurWeight() + (SI32)Weight) ? TRUE:FALSE ;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ �߰�, (DB����ȵ�, �ٸ� �Լ����� ȣ���Ϸ��� Player �� �ִ� AddItem() �����.)
//	���� ����	:	2002-07-12 ���� 3:15:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::AddItem(MyItemData* pMyItem, BOOL bTakeOff)
{
	MyItemData		*pItem;		
	DWORD			dwRet  = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;	
	UI16			uiAddedItemID, uiAddedItemQuantity;		

	uiAddedItemID		=	pMyItem->uiID;
	uiAddedItemQuantity	=	pMyItem->uiQuantity;
		
	// 1. ���� �������� �ִٸ�, �װ��� �߰��Ѵ�.
	pItem	=	GetItem(pMyItem->uiID);
	if(pItem)
	{
		pItem->uiQuantity			+=	pMyItem->uiQuantity;	
		
		// ���� ������ �ִ� �������� ������ �˷��ش�.
		pMyItem->uiQuantity			=   pItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.

		// siPosInInventory���� �߸��� ��� DB�� �������� �ʵ��� FALSE ó���Ѵ�.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;
		goto	END;		
	}

	// 2. ���� �������� ���ٸ�, ������ ���Կ� ����� �õ�.
	if(pMyItem->siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
	{
		pItem	=	&Items[pMyItem->siPosInInventory];
		if( pItem->uiID	==	0 )
		{
			pItem->uiID				=	pMyItem->uiID;
			pItem->uiQuantity		=	pMyItem->uiQuantity;			

			dwRet					=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;
			goto	END;
		}
	}

	// 3. ���� �������� ����, ������ ���Կ��� ������ ���ٸ�, �󽽷��� ã�� �ִ´�..
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	pMyItem->uiID;
		pItem->uiQuantity			=	pMyItem->uiQuantity;
		pMyItem->siPosInInventory	=	pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.

		// siPosInInventory���� �߸��� ��� DB�� �������� �ʵ��� FALSE ó���Ѵ�.
		if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

		dwRet						=	ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;
		goto	END;
	}

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// �������� ���Ը� �����ش�.
		// �������� ���°Ŷ�� �������� �ʴ´�.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰��Ѵ�.
//	���� ����	:	2002-10-14 ���� 11:47:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::AddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff )
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// ��ȿ�� ���������� üũ�Ѵ�.
	if( uiItemID == 0)			goto	END;

	uiAddedItemID		=	uiItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. ���� �������� �ִٸ�, �װ��� �߰��Ѵ�.
	pItem	=	GetItem( uiItemID );
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;	

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. ��� �ִ� ���� ã�� �߰��Ѵ�.
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	uiItemID;
		pItem->uiQuantity			=	uiQuantity;

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;		
		goto	END;
	}	

END:

	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// �������� ���Ը� �����ش�.
		// �������� ���°Ŷ�� �������� �ʴ´�.
		if(bTakeOff == FALSE)				AddItemWeight( uiAddedItemID, uiAddedItemQuantity );	
	}

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰��Ѵ�.
//	���� ����	:	2002-07-12 ���� 3:17:35 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::AddItem(UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItem;
	DWORD			dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	UI16			uiAddedItemID, uiAddedItemQuantity;	
	

	// ��ȿ�� ���������� üũ�Ѵ�.
	if(ItemID == 0)		goto	END;

	uiAddedItemID		=	ItemID;
	uiAddedItemQuantity	=	uiQuantity;	

	// 1. ���� �������� �ִٸ�, �װ��� �߰��Ѵ�.
	pItem	=	GetItem(ItemID);
	if(pItem)
	{
		pItem->uiQuantity			+=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_ADD;		
		goto	END;
	}

	// 2. ��� �ִ� ���� ã�� �߰��Ѵ�.
	pItem	=	GetEmptyItem();
	if(pItem)
	{
		pItem->uiID					=	ItemID;
		pItem->uiQuantity			=	uiQuantity;
		*psiPosInInventory			=	pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.

		dwRet = ONLINEFOLLOWER_ADDITEMRETURNTYPE_CREATE;		
		goto	END;
	}	

END:
	if(dwRet != ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE)
	{				
		// �������� ���Ը� �����ش�.
		// �������� ���°Ŷ�� �������� �ʴ´�.
		if(bTakeOff == FALSE)				AddItemWeight(uiAddedItemID, uiAddedItemQuantity);	
	}

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-11-19 ���� 10:28:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::DelItem(MyItemData* pMyItem, BOOL bPutOn)
{
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	pMyItem->uiID;
	uiDeletedItemQuantity	=	pMyItem->uiQuantity;

	if( ( pItem = GetItem( uiDeletedItemID ) ) == NULL )				goto	END;
	if( pItem->uiQuantity < uiDeletedItemQuantity )						goto	END;
	if( IsValidItemPosInInventory(pItem->siPosInInventory) == FALSE )	goto	END;

	pItem->uiQuantity			-=	pMyItem->uiQuantity;
	pMyItem->uiQuantity			=   pItem->uiQuantity;
	pMyItem->siPosInInventory	=   pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.


	if( pItem->uiQuantity == 0 )
	{
		pItem->uiID				=	0;
		pMyItem->uiID			=   0;		
		
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;		
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;	
	}

END:

	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// �������� �������ش�.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-11-19 ���� 10:26:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::DelItem( UI16 ItemID, UI16 uiQuantity, SI16 *psiPosInInventory, BOOL bPutOn )
{
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	ItemID;
	uiDeletedItemQuantity	=	uiQuantity;

	if( ( pItem = GetItem( ItemID ) ) == NULL )		goto	END;
	if( pItem->uiQuantity < uiQuantity )			goto	END;	

	pItem->uiQuantity		-=	uiQuantity;			
	*psiPosInInventory		=   pItem->siPosInInventory;	// �κ��丮 ��ġ�� �˷���� DB�� ����.

	if(pItem->uiQuantity == 0)
	{
		pItem->uiID				=	0;				
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;	
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;
	}	

END:

	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// �������� �������ش�.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ش� �������� ������ ����.
//	���� ����	:	2002-07-05 ���� 4:16:06 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::DelItem( UI16 uiItemID, UI16 uiQuantity, BOOL bPutOn )
{	
	MyItemData		*pItem;
	DWORD			dwRet	=	ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
	UI16			uiDeletedItemID, uiDeletedItemQuantity;

	uiDeletedItemID			=	uiItemID;
	uiDeletedItemQuantity	=	uiQuantity;

	if( ( pItem = GetItem( uiItemID ) ) == NULL )			goto	END;
	if( pItem->uiQuantity < uiQuantity )					goto	END;	
	
	pItem->uiQuantity			-=	uiQuantity;			

	if(pItem->uiQuantity == 0)
	{
		pItem->uiID				=	0;				

		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_ALLDEL;	
	}
	else
	{
		dwRet					=	ONLINEFOLLOWER_DELITEMRETURNTYPE_DEL;	
	}	

END:
	if( dwRet != ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )
	{
		// �������� �������ش�.
		if( bPutOn == FALSE )	DelItemWeight( uiDeletedItemID, uiDeletedItemQuantity );
	}

	return dwRet;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ �߿� �������� �����Ѵ�.
//	���� ����	:	2002-07-12 ���� 4:05:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::SetWearItem(UI08 uiLoc, UI16 uiItemID,SI16 siItemLimitTime)
{
	// ���� ���� ������ ��
	UI16	uiOldItemID	= Weapon.GetWearItem( uiLoc );

	// ���Ը� ���ְ�
	DelItemWeight( uiOldItemID, 1 );

	// �ű� ���������� �����ϰ�
	Weapon.SetWearItem(uiLoc, uiItemID,siItemLimitTime);
	
	// ���� ��� �ִ� �������� ���Կ� �߰����ش�.
	AddItemWeight(uiItemID, 1);

	// ĳ������ �ɷ�ġ���� �������Ѵ�.
	ParametaBox.Action(pCItem, Weapon);		

	return	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	����	:	������ ����
//----------------------------------------------------------------------------------------------------------------
/*MyItemData*	OnlineFollower::WearItem(CItemHeader* pItemHeader, SI08 uiLoc, SI32 siTradeGrade)
{	
	MyItemData		*pItem;	

	if( Weapon.GetWearItem(uiLoc) != 0 )		return NULL;

	if( pItemHeader->m_Attribute & ON_ITEM_ATTACH_BODY_POSSIBLE )
	{
		if( pItemHeader->m_siGroupCode & ON_ITEM_DRESS )
		{			
			if( pItemHeader->m_siReqLv > siTradeGrade)
				return NULL;
		}
		else
		{
			if( pItemHeader->m_siReqLv > ParametaBox.IP.GetLevel() )
				return NULL;
		}

		pItem	=	GetItem( pItemHeader->m_uiID );
		if(pItem)
		{
			if( pItem->uiQuantity >= 1 )
			{
				pItem->uiQuantity--;
				if(pItem->uiQuantity == 0)			pItem->uiID	=	0;
				Weapon.SetWearItem(uiLoc,pItemHeader->m_uiID);
				return pItem;
			}
		}
	}
	return NULL;
}*/

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �ִ� �����۵� ����, ������ �����۵� ����� �Ѵ�.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineFollower::IsEmpty()
{
	// 1. ������ �ִ� �������� ����� �Ѵ�.
	for(int i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID != 0)
			return FALSE;
	}

	// 2. �����ϰ� �ִ� �����۵� ����� �Ѵ�.
	return Weapon.IsEmpty();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-05 ���� 7:23:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// �������� ���� ���� �ƴҶ����� ���Ը� ����Ѵ�.
	if(bTakeOff == FALSE)
	{		
		// �߰� ��Ű���� �������� ���Ը� ���´�.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// �������� �� ���Ը� �ʰ��ϴ��� �˾ƿ´�.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// ���� �������� �ִ��� ã�ƺ���.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// �κ��丮�� �̹� ���� �������� �ִ�.
		
		// ������ ���� �����÷ο� ���� �ʴ��� �˻��Ѵ�.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// �κ��丮�� ���� �������� ����.		
	
		// �ش� ��ġ�� �������� ������ �ִ��� �˾ƺ���.
		if( GetItemByPos(siPosInInventory, &pItemInfo) == FALSE )			return FALSE;		// �κ��丮 ��ġ�� Ʋ���� ���� ������ GetItemByPos����
		if( pItemInfo != NULL)												return	FALSE;		// �������� �̹� ������
	}

	return	TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-12 ���� 7:56:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanAddItem(UI16 uiItemID, UI16 uiQuantity, BOOL bTakeOff)
{
	MyItemData		*pItemInfo;	
	CItemHeader		*pItemHeader;		
	DWORD			dwAddItemWeight = 0;

	// �������� ���� ���� �ƴҶ����� ���Ը� ����Ѵ�.
	if(bTakeOff == FALSE)
	{		
		// �߰� ��Ű���� �������� ���Ը� ���´�.
		if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)							return	FALSE;
		dwAddItemWeight		+=	(pItemHeader->m_uiWeight * uiQuantity);

		// �������� �� ���Ը� �ʰ��ϴ��� �˾ƿ´�.
		if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)								return	FALSE;
	}

	// ���� �������� �ִ��� ã�ƺ���.
	if((pItemInfo = GetItem(uiItemID)) != NULL)
	{
		// �κ��丮�� �̹� ���� �������� �ִ�.
		
		// ������ ���� �����÷ο� ���� �ʴ��� �˻��Ѵ�.
		if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity ) < uiQuantity )		return	FALSE;
	}
	else
	{
		// �κ��丮�� ���� �������� ����.		
	
		// �� ������ �ִ��� �˻��Ѵ�. �� ������ ������ �������� �߰� ��ų �� ����.
		if(GetEmptyItem() == NULL)											return	FALSE;
		
	}

	return	TRUE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���´�.
//	���� ����	:	2002-07-05 ���� 7:25:53 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::GetItemByPos( SI16 siPosInInventory, MyItemData **ppItemData )
{	
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )				return FALSE;

	if( Items[siPosInInventory].uiID != 0 )			*ppItemData	=	&Items[siPosInInventory];
	else											*ppItemData	=	NULL;

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-07 ���� 4:39:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanAddItem(UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum, BOOL bTakeOff)
{
	SI32			siNeedSlot;
	MyItemData		*pItemInfo;
	SI32			i;
	DWORD			dwAddItemWeight;
	CItemHeader		*pItemHeader;
	
	siNeedSlot			=	0;
	dwAddItemWeight		=	0;
	for(i = 0; i < 	uiItemNum; i++)
	{
		// �������� ���� ���� �ƴҶ����� ���Ը� ����Ѵ�.
		if(bTakeOff == FALSE)
		{			
			// �߰� ��Ű���� �������� ���Ը� ���´�.
			if((pItemHeader = pCItem->GetItem(puiID[i])) == NULL)								return	FALSE;		
			dwAddItemWeight		+=	(pItemHeader->m_uiWeight * puiQuantity[i]);

			// �������� �� ���Ը� �ʰ��ϴ��� �˾ƿ´�.
			if(IsOverflowItemWeight(dwAddItemWeight) == TRUE)									return	FALSE;
		}

		if((pItemInfo = GetItem(puiID[i])) == NULL)	
		{
			// ���ο� �ڸ��� �ʿ��ϴ�.
			siNeedSlot++;
		}
		else
		{
			// ������ �̹� �������� �ִ�.

			// ������ �����ۿ� �� �������� ���� �� �ִ��� �˾ƿ´�.
			// ���� ���� �� ���ٸ� �������� �߰� ��ų �� ����.
			if( ( MAX_FOLLOWER_HAVE_ITEM_NUM - pItemInfo->uiQuantity) < puiQuantity[ i ] )		return	FALSE;					
		}		
	}
	
	// �ʿ��� �ڸ��� ���ٸ� TRUE�� �����Ѵ�.
	if(siNeedSlot == 0)	return	TRUE;

	// ���� ���� �� �ִ� �������� ���� ���´�.
	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Items[i].uiID == 0)
		{
			siNeedSlot--;

			// �ʿ��� ������ ��� ã�Ҵ�.
			if(siNeedSlot == 0)	return	TRUE;
		}
	}

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰������� ��� ���Ը� �ʰ��ϴ��� �˾ƿ´�.
//	���� ����	:	2002-07-12 ���� 2:52:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::IsOverflowItemWeight(DWORD dwAddItemWeight)
{

	if( ( GetTotalItemWeight() + dwAddItemWeight ) > GetMaxWeight() )		return	TRUE;
	else																	return	FALSE;

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �ʰ����� �˻��Ͽ� �ȱ� ���� �����̸� TRUE�� �����Ѵ�.
//	���� ����	:	2003-03-19 ���� 10:57:23 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::IsOverWeight()
{
	if( GetTotalItemWeight() > GetMaxWeight() )		return	TRUE;
	else											return	FALSE;

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���Ը� �߰������ش�.
//	���� ����	:	2002-07-12 ���� 2:58:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::AddItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// �߰� ��Ű���� �������� ���Ը� ���´�.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// �������� ���Ը� ���Ѵ�.
	dwTotalItemWeight	+=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���Ը� ���ش�.
//	���� ����	:	2002-07-12 ���� 2:58:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::DelItemWeight(UI16 uiItemID, UI16 uiItemQuantity)
{
	CItemHeader		*pItemHeader;

	// �߰� ��Ű���� �������� ���Ը� ���´�.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)					return	FALSE;		
	
	// �������� ���Ը� ���Ѵ�.
	dwTotalItemWeight	-=	(pItemHeader->m_uiWeight * uiItemQuantity);

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �ʱ�ȭ.
//	���� ����	:	2002-07-12 ���� 3:27:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFollower::InitStaticVariable(CItem* pItem)
{
	pCItem			=	pItem;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� ĳ������ ��ü �������� �𹫰Ը� ���´�.
//	���� ����	:	2002-07-12 ���� 2:45:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::GetTotalItemWeight()
{
	return dwTotalItemWeight;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ĳ���Ͱ� �ְ� �� �� �ִ� ����.
//	���� ����	:	2002-07-12 ���� 4:17:11 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	OnlineFollower::GetMaxWeight()
{
	return ParametaBox.GetMaxWeight();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ĳ���Ͱ� �ְ� ���� �� �ִ� �������� ������ ���´�.
//	���� ����	:	2002-07-12 ���� 5:52:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	OnlineFollower::GetItemQuantityToTake(UI16 uiItemID)
{
	MyItemData		*pItem;			
	UI16			uiItemQuantity;
	CItemHeader		*pItemHeader;
	
	// �������� ������ ���´�.
	if((pItemHeader = pCItem->GetItem(uiItemID)) == NULL)	return	0;		

	// ���� ���԰� ���� �÷ο� �Ǿ����� �˾ƿ´�.
	if(IsItemWeightOverflowed() == TRUE)					return	0;
	
	if((pItem = GetItem(uiItemID)) != NULL)
	{	
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM - pItem->uiQuantity;
	}
	else
	{
		// ���� ����ִ� ������ ã�´�.
		if((pItem = GetEmptyItem()) == NULL)	return 0;
		
		uiItemQuantity	=	MAX_FOLLOWER_HAVE_ITEM_NUM;
	}
			

	// ���Ը� ����ؼ� �� �� �ִ� �������� ���Ը� üũ�Ѵ�.
	return UI16(min((GetMaxWeight() - GetTotalItemWeight()) / pItemHeader->m_uiWeight, uiItemQuantity));	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ĳ���Ͱ� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.
//	���� ����	:	2002-07-12 ���� 8:19:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::IsItemWeightOverflowed()
{
	if(GetMaxWeight() < GetTotalItemWeight())	return	TRUE;
	else										return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� �ִ� �������� ���´�.
//	���� ����	:	2002-07-12 ���� 9:17:00 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::TakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// �κ��丮�� �������� �־��ش�.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	1;
	ItemData.siPosInInventory	=	siPosInInventory;
	if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// �������� ���´�.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// �κ��丮�� ���� ������ ���´�.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// ĳ������ �ɷ�ġ���� �������Ѵ�.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

BOOL	OnlineFollower::DisAppearItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	MyItemData	ItemData;
	
	// �κ��丮�� �������� �־��ش�.
	ItemData.uiID				=	0;
	ItemData.uiQuantity			=	0;
	ItemData.siPosInInventory	=	0;
	//if( AddItem(&ItemData, TRUE) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )	return FALSE;

	// �������� ���´�.
	Weapon.SetWearItem(uiWearLoc, 0,0);

	// �κ��丮�� ���� ������ ���´�.
	if(pMyItemData != NULL)
		*pMyItemData			=	ItemData;

	// ĳ������ �ɷ�ġ���� �������Ѵ�.
	ParametaBox.Action(pCItem, Weapon);
	
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 2:17:37 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::PutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{	
	MyItemData			DeleteItemInfo;	

	// �κ��丮���� �������� ����.
	DeleteItemInfo.uiID					=	uiItemID;
	DeleteItemInfo.uiQuantity			=	1;
	DeleteItemInfo.siPosInInventory		=	siPosInInventory;
	if( DelItem(&DeleteItemInfo, TRUE) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )		return FALSE;

	// �������� ���� ��Ų��.
	SI32	siMaxLimitDate	= pCItem->GetItemUsingDay(uiItemID);
	( siMaxLimitDate > 0 )	?	siMaxLimitDate++ : siMaxLimitDate = 0;
	Weapon.SetWearItem( uiWearLoc, uiItemID, siMaxLimitDate );
	
	// �κ��丮�� ���� ������ ���´�.
	if(pMyItemData != NULL)
		*pMyItemData			=	DeleteItemInfo;

	// ĳ������ �ɷ�ġ���� �������Ѵ�.
	ParametaBox.Action(pCItem, Weapon);

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: GetWearItem()
// Code: actdoll (2004-07-19)
// Desc: ���� �� ĳ���Ͱ� Ư���� ��ġ�� �����ϰ� �ִ� �������� ������ ��´�.
//	uiWearLoc			- ���� ��ġ �ε���
//	bIsTimerItem		- ���� �������� �ð��� �������̸� true ����
//	siRemainDate		- �ð��� �������� �����Ǿ��� ��� ���� ������ ����
//	return				- ������ �������� ID. ���ٸ� 0�� ����
//-----------------------------------------------------------------------------
UI16	OnlineFollower::GetWearItem( UI08 uiWearLoc, BOOL &bIsTimerItem, SI16 &siRemainDate )
{
	bIsTimerItem	= false;
	siRemainDate	= 0;
	UI16 uiItemID = Weapon.GetWearItem( uiWearLoc );
	if( uiItemID == 0 )		return uiItemID;

	// �� �������� �ð��� ���������� �Ǵ�
	SI16	siTemp;
	CItemHeader *pItemHeader = pCItem->GetItemInfo( uiItemID, siTemp );
	if( pItemHeader->m_bTimeLimit )		// �ð��� �������� ���
	{
		bIsTimerItem	= (BOOL)pItemHeader->m_bTimeLimit;
		siRemainDate	= Weapon.GetWearItemLimitTime( uiWearLoc );
	}

	return	uiItemID;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �����۵��� ����Ʈ�� ���´�.
//	���� ����	:	2002-07-13 ���� 2:56:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::GetWearItems(UI16 puiWearItems[WEAR_ITEM_NUM])
{
	Weapon.GetWearItems(puiWearItems);
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 4:00:36 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanPutOnItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	MyItemData			*pItemDataInInventory;
	CItemHeader			*pItem;	

	// ���� �������� ������ �ִ��� Ȯ���Ѵ�.
	if( GetItemByPos(siPosInInventory, &pItemDataInInventory) )	
	{
		if( pItemDataInInventory == NULL )
		{
			ErrorCode = NOFIND_ERROR;
			return	FALSE;
		}
	}
	// ���� �������� ������ �ִ��� Ȯ���Ѵ�.
	if(pItemDataInInventory->uiID != uiItemID)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// �����Ϸ��� ���� �������� ����ִ��� Ȯ���Ѵ�.
	if(Weapon.GetWearItem(uiWearLoc) != 0)							
	{
		ErrorCode = NOFIND_ERROR;
		return	FALSE;
	}
	// ������ ������ �ִ��� Ȯ��
	if((pItem = pCItem->GetItem(uiItemID)) == NULL)						
	{
		ErrorCode = NOFIND_ERROR;
		return  FALSE;	
	}
	// ������ ������ ��ȣ������ Ȯ�� - ���ΰ��� ���밡��
	if(pItem->m_siGroupCode & ON_ITEM_GUARDIAN)
	{
		if((uiCharID  & 255) != 'C')
		{
			ErrorCode = CANTWEARGAURDIAN_ERROR;
			return FALSE;
		}
	}
	// ������ ������ �ð��� ���������� Ȯ�� - ���ΰ��� ���� ����
	// actdoll (2005/02/01 17:00) : 
	//	���� �Ȱ��� ������ �ִµ� �̰� �� ���� �־�����? �ϴ� �� ������ ���� ������ ����
	if(pCItem->IsTimeLimitItem(uiItemID))
	{
		if((uiCharID & 255) != 'C')
		{
			ErrorCode = CANTWEARTIMELIMITITEM_ERROR;
			return FALSE;
		}
	}
	
	// ������ ������ �巹������ Ȯ�� - ���ΰ��� ���밡���ϸ� �ſ뵵 �ʿ�
	if(pItem->m_siGroupCode & ON_ITEM_DRESS)
	{	
		if ((uiCharID & 255) == 'C')
		{			
			if(pItem->m_siReqLv > siTradeGrade)
			{
				ErrorCode = CREDIT_ERROR;
				return	FALSE;
			}
		}
		else
		{
			ErrorCode = CANTPUTONDRESS_ERROR;
			return FALSE;
		}
	}
	else
	{
		// �������� ������ �� �ִ� ������ �Ǵ��� �˻��Ѵ�.
		if(pItem->m_siReqLv > ParametaBox.IP.GetLevel())
		{
			ErrorCode = LEVEL_ERROR;
			return	FALSE;
		}
	}

	// ������ ���� (�������� ������ �տ��� ��üũ �ǹǷ� ���ΰ��� ��츸 �˻��Ѵ�.)
	if(pItem->pWeaponHeader && (uiCharID & 255) == 'C')
	{
		switch(pItem->pWeaponHeader->siWeaponType)
		{
		case ON_ITEM_MISSLE_TYPE_FAN:			// �Ϻ� ���ڸ� ��ä�� ��� �� �� �̵�.			
			// �Ϻ� ���ڸ� TRUE
			if (uiCharID == 9027) return TRUE;
			else
			{
				ErrorCode = FAN_ERROR;
				return FALSE;
			}			
			break;			
		case ON_ITEM_MISSLE_TYPE_DOUBLESTICK:	// ���� ���ڸ� �����︦ ��� �� �� �̵�.								
			if( uiCharID == 323 ) return TRUE;
			else {			
				ErrorCode = DOUBLESTICK_ERROR;
				return FALSE;
			}
			break;
									
		case ON_ITEM_MISSLE_TYPE_SPINSTONE:		// ���� ���ڸ� ��ɵ��� ��� �� �� �̵�.								
			if( uiCharID == 67 ) return TRUE;
			else {			
				ErrorCode = SPINSTONE_ERROR;
				return FALSE;
			}		
			break;									
		case ON_ITEM_MISSLE_TYPE_SCYTHE:		// �Ϻ� ���ڸ� ���� ��� �� �� �̵�.							
			if( uiCharID == 8771 )	return TRUE;
			else {				
				ErrorCode = SCYTHE_ERROR;
				return FALSE;
			}
			break;	
		case ON_ITEM_MISSLE_TYPE_BEADS:         // �븸 �� �븸 ��� ���ָ� ��������.
			/*if( uiCharID == 10307 || uiCharID == 10563) return FALSE;

			break;*/
		case ON_ITEM_MISSLE_TYPE_BUDDIST:		// ���ΰ��� ��Ź ���ִ� �� �� ����.
			return FALSE;
			break;
		case ON_ITEM_MISSLE_TYPE_FIREGUN:		// ȭ���� ���ΰ��� ���� ����.
		case ON_ITEM_MISSLE_TYPE_STICK:
		case ON_ITEM_MISSLE_TYPE_DSWORD:
		case ON_ITEM_MISSLE_TYPE_PEARL:
			return FALSE;
			break;

									
		case ON_ITEM_MISSLE_TYPE_FREE:			// ��� �ɸ��Ͱ� �� �� �̵�.
			break;
		}
	}	
	return	TRUE;
}
	

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 4:01:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanTakeOffItem(UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	// �ش� �뺴�� ���� �������� �ϴ� �������� �����ϰ� �ִ� �����۰� ������ ���Ѵ�.
	if(Weapon.GetWearItem(uiWearLoc) != uiItemID)						return	FALSE;

	// �������� �ϴ� �������� �κ��丮�� ���� �� �ִ��� �˻��Ѵ�.
	if(CanAddItem(uiItemID, 1, siPosInInventory, TRUE) == FALSE)		return	FALSE;

	if(pCItem->IsTimeLimitItem(uiItemID) == TRUE)						return  FALSE;

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-14 ���� 10:27:28 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::HasItem( UI16 uiItemID, UI16 uiItemQuantity )
{
	MyItemData	*pMyItemData;

	if( ( pMyItemData = GetItem( uiItemID ) ) == NULL )			return	FALSE;

	if( pMyItemData->uiQuantity < uiItemQuantity )				return	FALSE;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ű� �� �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-11-12 ���� 10:48:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanMoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 siPosInInventory )
{
	// �������� �ִ��� �˻��Ѵ�.
	if( HasItem( uiItemID, uiItemQuantity ) == FALSE )			return	FALSE;
		
	// �������� �ű���� �ڸ��� ��� �ִ��� �˻��Ѵ�.
	if( IsEmptyItemInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �ñ��.
//	���� ����	:	2002-11-12 ���� 10:57:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::MoveItem( UI16 uiItemID, UI16 uiItemQuantity, SI16 *psiPosInInventory )
{
	MyItemData			ItemData;

	if( CanMoveItem( uiItemID, uiItemQuantity, *psiPosInInventory ) == FALSE )			return	FALSE;

	// �������� ����.
	if( DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )	return	FALSE;
	
	// �������� ���Ѵ�.
	ItemData.uiID				=	uiItemID;
	ItemData.uiQuantity			=	uiItemQuantity;
	ItemData.siPosInInventory	=	*psiPosInInventory;
	if( AddItem(&ItemData) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )					return	FALSE;

	*psiPosInInventory			=	ItemData.siPosInInventory;

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮���� �������� �ִ� ��ġ�� ������� �˾ƿ´�.
//	���� ����	:	2002-11-12 ���� 10:52:19 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::IsEmptyItemInInventory( SI16 siPosInInventory )
{
	if( IsValidItemPosInInventory( siPosInInventory ) == FALSE )	return	FALSE;

	if( Items[ siPosInInventory ].uiID == 0 )						return	TRUE;
	else															return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮������ �������� �ִ� ��ġ�� ��ȿ���� �˻��Ѵ�.
//	���� ����	:	2002-11-12 ���� 10:56:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::IsValidItemPosInInventory( SI16 siPosInInventory )
{
	if( ( siPosInInventory >= 0 ) && ( siPosInInventory < ON_MYDATA_MAX_ITEM_NUM ) )	return	TRUE;
	else																				return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���Ⱑ ���׷��̵� �������� �˾ƿ´�(��޹����ΰ�?)
//	���� ����	:	2003-3-31 ���� 17:33 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFollower::CanWeaponBeUpgraded()
{
	return pCItem->IsUpgradable( Weapon.GetWearItem(ON_WEAR_WEAPON_ITEM) );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���⸦ Ư�� ����� ���׷��̵����ش�.
//	���� ����	:	2003-3-31 ���� 17:35 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFollower::UpgradeWeapon( UI16 uiItemID )
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, uiItemID ,0);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���⸦ ���߸���. (����)
//	���� ����	:	2003-3-31 ���� 17:38 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFollower::BrakeWeapon()
{
	Weapon.SetWearItem( ON_WEAR_WEAPON_ITEM, 0 ,0);
}



//***********************************************************************************************
//	IOnlineFollower ����ü
//***********************************************************************************************

//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::IOnlineFollower()
{	
	Init();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//----------------------------------------------------------------------------------------------------------------
IOnlineFollower::~IOnlineFollower()
{	
	Free();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �ʱ�ȭ�� �����ϰ� �ʿ��� �޸𸮸� �Ҵ� �޴´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Init()
{	
	SI32	i;

	// ���ϵ��� ����Ÿ�� �ʱ�ȭ �Ѵ�.
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		Followers[i].Init();
	} 
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�� Ŭ�������� ����ߴ� �޸𸮸� ���� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	IOnlineFollower::Free()
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ���� �߿� ����ִ� ���۸� ã�´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineFollower*		IOnlineFollower::GetFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// ���� �� ���Կ� ���ϰ� �ִ��� ����.
		if(Followers[uiFollowerSlot].uiCharID == 0)
			return NULL;
		else
			return &Followers[uiFollowerSlot];
	}
	else
		return NULL;
}

OnlineFollower*		IOnlineFollower::GetHero()
{
	return Followers;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���� �߿� ����ִ� ���۸� ã�´�.
//	���� ����	:	2002-07-03 ���� 3:34:22 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	IOnlineFollower::FindEmptyItemBuffer(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;
	SI32				i;
//	MyItemData			*pItemData;

	pFollower	=	GetFollower(uiFollowerSlot);
	if(pFollower)
	{
		for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		{
			if(pFollower->Items[i].uiID == 0)
				return i;
		}
	}
	return -1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���� �߿� ���� �������� ����ִ� ���۸� ã�´�.
//	���� ����	:	2002-07-03 ���� 3:34:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	IOnlineFollower::FindSameItemBuffer(UI08 uiFollowerSlot, UI16 ItemID)
{
	SI32	i;
	
	if(IsValidFollower(uiFollowerSlot) == FALSE)
		return 0;

	for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
	{
		if(Followers[uiFollowerSlot].Items[i].uiID == ItemID)
			return i;
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���԰� �� �� �ִ��� Ȯ���Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::CheckWeight(UI08 uiFollowerSlot, UI32 Weight)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);
	if(pFollower)
	{
		return pFollower->CheckWeight(Weight);
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�����ʰ��� �뺴�� �Ѹ��̶� �����Ͽ� �ȱ� ���� �����̸� TRUE�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::IsOverWeight()
{
	SI32		i;
	BOOL		bOverWeight		=	FALSE;

	for( i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			if( Followers[i].IsOverWeight() )
			{
				bOverWeight	=	TRUE;
				break;
			}
		}
	}
	return bOverWeight;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �ִ��� �˻�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::CheckItem(UI16 uiItemID, SI32 siCount)
{
	SI32	TempCount;
	SI32	TempCount1;
	
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				if(Followers[TempCount].Items[TempCount1].uiID == uiItemID)
				{
					siCount -= Followers[TempCount].Items[TempCount1].uiQuantity;
					if(siCount <= 0) return TRUE;
				}
			}

			/*
			// ����� Item
			siCount -= Followers[TempCount].Weapon.CheckItemCount(uiItemID);
			if(siCount <= 0) return TRUE;
			*/
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ �ִ� Item�� ���� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	IOnlineFollower::GetItemCount(UI16 uiItemID)
{
	SI32	TempCount;
	SI32	TempCount1;
	SI32	TempItemCount;
	
	TempItemCount = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			// Invetory Item
			for(TempCount1 = 0; TempCount1 < ON_MYDATA_MAX_ITEM_NUM; TempCount1++)
			{
				if(Followers[TempCount].Items[TempCount1].uiID == uiItemID)
				{
					TempItemCount += Followers[TempCount].Items[TempCount1].uiQuantity;
				}
			}

			// ����� Item
			TempItemCount += Followers[TempCount].Weapon.CheckItemCount(uiItemID);
		}
	}

	return TempItemCount;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ������ ����Ʈ�� �߰��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;

	// �켱 ������ ���� �������� �ִ��� �˾ƿ´�.
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->AddItem(pItem);
	}
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ����Ʈ�� �߰��Ѵ�.
//	���� ����	:	2002-10-14 ���� 11:44:29 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower(uiFollowerSlot) ) == NULL )	return	ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
	
	return lpFollower->AddItem( uiItemID, uiQuantity );	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ����Ʈ�� �߰��Ѵ�.
//	���� ����	:	2002-07-03 ���� 10:29:51 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory)
{
	MyItemData			ItemData;

	ItemData.uiID				=	uiItemID;
	ItemData.siPosInInventory	=	siPosInInventory;
	ItemData.uiQuantity			=	uiQuantity;

	return	AddItem(uiFollowerSlot, &ItemData);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ����Ʈ�� �߰��Ѵ�.
//	���� ����	:	2002-07-07 ���� 11:28:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::AddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory)
{
	OnlineFollower*		lpFollower;

	// �켱 ������ ���� �������� �ִ��� �˾ƿ´�.
	lpFollower = GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->AddItem(uiItemID, uiQuantity, psiPosInInventory);
	
	return ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ������ ����Ʈ���� �����Ѵ�.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(pItem);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ����Ʈ���� �����Ѵ�.				
//	���� ����	:	2002-07-05 ���� 4:15:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(uiItemID, uiQuantity);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ ����Ʈ���� �����Ѵ�.
//	���� ����	:	2002-07-07 ���� 11:36:23 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::DelItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 *psiPosInInventory)
{
	OnlineFollower*		lpFollower;
	
	lpFollower = GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->DelItem(uiItemID, uiQuantity, psiPosInInventory);
	}
	return ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ������ ����Ʈ���� ���´�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	IOnlineFollower::GetItem(UI08 uiFollowerSlot, UI16 uiIndex, MyItemData *pItem)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(lpFollower->Items[uiIndex].uiID != 0)
		{
			memcpy(pItem, &lpFollower->Items[uiIndex], sizeof(MyItemData));
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� ������ ����Ʈ���� ���´�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData*	IOnlineFollower::GetItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory<ON_MYDATA_MAX_ITEM_NUM)
		{
			// 1. �켱 �־��� ���� �������� �ִٸ� �װ� �����Ѵ�.
			if(lpFollower->Items[siPosInInventory].uiID	==	uiItemID )
			{
				return &lpFollower->Items[siPosInInventory];
			}
		}
		// 2. �� ��ġ�� �� �������� ���ٸ�, ���� �������� ã�ƺ���.
		return lpFollower->GetItem(uiItemID);
	}
	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ID�� ����Ͽ� �������� ���´�.
//	���� ����	:	2002-07-03 ���� 10:15:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
MyItemData*	IOnlineFollower::GetItemByID(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	
	if(lpFollower)	
		return lpFollower->GetItem(uiItemID);		
	else
		return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ID�� ����Ͽ� �������� ���´�.
//	���� ����	:	2002-07-03 ���� 10:15:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::GetItemByPos( UI08 uiFollowerSlot, SI16 siItemPos, MyItemData **ppItemData )
{
	OnlineFollower*		lpFollower;

	lpFollower	=	GetFollower( uiFollowerSlot );
	
	if( !lpFollower )												return FALSE;

	if( lpFollower->GetItemByPos(siItemPos, ppItemData) == FALSE )	return FALSE;

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-14 ���� 10:25:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::HasItem( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity )
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	FALSE;

	return	lpFollower->HasItem( uiItemID, uiItemQuantity );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ������ �ִ��� �˻��Ѵ�.
//	���� ����	:	2002-10-14 ���� 10:25:27 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineFollower::GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiItemQuantity )
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	-1;

	return	lpFollower->GetItemIndex( uiItemID, uiItemQuantity );
}

SI32	IOnlineFollower::GetItemIndex( UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower*		lpFollower;

	if( ( lpFollower = GetFollower( uiFollowerSlot) ) == NULL )			return	-1;

	return	lpFollower->GetItemIndex( uiItemID);
}



//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �ִ��� �˻�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL IOnlineFollower::IsItem(UI08 uiFollowerSlot, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
		{
			if(lpFollower->Items[siPosInInventory].uiID != 0 )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ���������� �˻��Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL IOnlineFollower::IsSameItem(UI08 uiFollowerSlot, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		if(siPosInInventory < ON_MYDATA_MAX_ITEM_NUM)
		{
			if((lpFollower->Items[siPosInInventory].uiID == uiItemID))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�����۸���Ʈ�� �����ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
MyItemData* IOnlineFollower::GetItemList(UI08 uiFollowerSlot)
{
	return (MyItemData*)Followers[uiFollowerSlot].Items;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȿ�� ���� �������� �˻��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::IsValidFollower(UI08 uiFollowerSlot)
{
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		// ���� �� ���Կ� ���ϰ� �ִ��� ����.
		if(Followers[uiFollowerSlot].uiCharID != 0)
			return TRUE;
	}
	return FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ĳ������ ID�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	IOnlineFollower::GetFollowerCharID(UI08 uiFollowerSlot)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		return lpFollower->uiCharID;
	}
	return 0;
}

VOID	IOnlineFollower::SetFollowerCharID(UI08 uiFollowerSlot, UI16 uiCharID)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE)	return;

	Followers[uiFollowerSlot].SetCharID(uiCharID);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ϸ� �ʱ�ȭ �Ѵ�.
//	���� ����	:	2002-10-29 ���� 9:31:44 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::InitFollower(UI08 uiFollowerSlot, OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	OnlineFollower*		lpFollower;
	SI32				i;

	
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		lpFollower	=	&Followers[uiFollowerSlot];
		if(lpFollower->uiCharID	==	0)
		{
			lpFollower->Init();
			lpFollower->ParametaBox.Init(lpFollowerInfo->siExp, lpFollowerInfo->siLevel, lpFollowerInfo->siStr, lpFollowerInfo->siDex, lpFollowerInfo->siVit, lpFollowerInfo->siInt, lpFollowerInfo->siLife, lpFollowerInfo->siMana, lpFollowerInfo->siAC, lpFollowerInfo->siBonus, lpFollowerInfo->siDamageRegist, lpFollowerInfo->siMagicRegist, lpFollowerInfo->siEquipMinDamage, lpFollowerInfo->siEquipMaxDamage, lpFollowerInfo->siLiftConstant);

			// �������� ���� ��Ų��.
			for( i = 0; i < WEAR_ITEM_NUM; i++ )
			{
				lpFollower->SetWearItem( i, lpFollowerInfo->stWearItemInfo[i].uiItemID,  lpFollowerInfo->stWearItemInfo[i].siLimitTime);
			}

			lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);
			lpFollower->uiCharID		=	lpFollowerInfo->uiKind;	
			lpFollower->Nation			=   Nation;
			strncpy( lpFollower->Name, lpFollowerInfo->Name, ON_ID_LENGTH );

			lpFollower->dwSaveTime = timeGetTime();
			lpFollower->ParametaBox.IP.setCycleSaveToDB(FALSE);
			lpFollower->ParametaBox.IP.setSaveToDB(FALSE);

			return TRUE;
		}
	}
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ϸ� �ʱ�ȭ �Ѵ�.	
//	���� ����	:	2002-07-08 ���� 12:30:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::InitFollower(OnFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	SI32						i;
	OnlineFollower*				lpFollower;
	SI16						siEmptyFollowerSlot;
	OnlineReceivedMyItemInfo	*pItemInfo;

	if((siEmptyFollowerSlot = FindEmptyFollowerSlot()) != -1)
	{
		// lpFollowerInfo ��ü�� �����Ѵ�.
		lpFollowerInfo->uiSlot	=	UI08(siEmptyFollowerSlot);

		if(InitFollower(lpFollowerInfo->uiSlot, lpFollowerInfo, Nation, pItem) == TRUE)
		{
			// ��ü�� ���´�.
			lpFollower	=	&Followers[lpFollowerInfo->uiSlot];

			pItemInfo	=	(OnlineReceivedMyItemInfo*)((CHAR*)lpFollowerInfo + sizeof(OnFollowerInfo));

			// �������� �ִٸ� �������� �߰������ش�.
			for(i = 0; i < lpFollowerInfo->uiItemNum; i++)			
				lpFollower->AddItem((MyItemData*)&pItemInfo[i]);							
			
			return	TRUE;
		}
	}

	return	FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���ϸ� Update �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::UpdataFollower(UI08 uiFollowerSlot, OnChangeJobFollowerInfo* lpFollowerInfo, SI32 Nation, CItem* pItem)
{
	OnlineFollower*		lpFollower;
	
	if(uiFollowerSlot < ON_MAX_FOLLOWER_NUM)
	{
		lpFollower	=	&Followers[uiFollowerSlot];
		if(lpFollower->uiCharID	!= 0)
		{
			lpFollower->ParametaBox.Init(lpFollowerInfo->siExp, lpFollowerInfo->siLevel, lpFollowerInfo->siStr, lpFollowerInfo->siDex, lpFollowerInfo->siVit, lpFollowerInfo->siInt, lpFollowerInfo->siLife, lpFollowerInfo->siMana, lpFollowerInfo->siAC, lpFollowerInfo->siBonus, lpFollowerInfo->siDamageRegist, lpFollowerInfo->siMagicRegist, lpFollowerInfo->siEquipMinDamage, lpFollowerInfo->siEquipMaxDamage, lpFollowerInfo->siLiftConstant);
			lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);
			lpFollower->uiCharID		=	lpFollowerInfo->uiKind;	
			lpFollower->Nation			=   Nation;
			strncpy( lpFollower->Name, lpFollowerInfo->Name, ON_ID_LENGTH );

			lpFollower->dwSaveTime = timeGetTime();
			lpFollower->ParametaBox.IP.setCycleSaveToDB(FALSE);
			lpFollower->ParametaBox.IP.setSaveToDB(FALSE);

			return TRUE;
		}
	}
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::DeleteFollower(UI08 uiFollowerSlot)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{
		lpFollower->Init();		
		return TRUE;
	}
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get Follower Base Parameter Data
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineFollower::GetFollowerBaseParameterCurExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetCurExp();
}

SI32	IOnlineFollower::GetFollowerBaseParameterSTR(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetStr();
}

SI32	IOnlineFollower::GetFollowerBaseParameterDEX(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetDex();
}

SI32	IOnlineFollower::GetFollowerBaseParameterVIT(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetVit();
}

SI32	IOnlineFollower::GetFollowerBaseParameterINT(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetInt();
}

SI32	IOnlineFollower::GetFollowerBaseParameterAC(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetAC();
}

SI32	IOnlineFollower::GetFollowerBaseParameterBonus(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
}

BOOL	IOnlineFollower::SetFollowerBaseParameterBonus(UI08 uiFollowerSlot, SI32 siBonus)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(siBonus);
	return TRUE;
}

BOOL	IOnlineFollower::SetFollowerParameterBonusSubTract1(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot)                  == FALSE) return FALSE;
	if(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() <  1)     return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - 1);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerBaseParameterDamageResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.DamageResist;
}

SI32	IOnlineFollower::GetFollowerBaseParameterMagicResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.MagicResist;
}

SI32	IOnlineFollower::GetFollowerBaseParameterEquipMinDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.EquipMinDamage;
}

SI32	IOnlineFollower::GetFollowerBaseParameterEquipMaxDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.EquipMaxDamage;
}

SI32	IOnlineFollower::GetFollowerBaseParameterLiftConstance(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.LiftConstant;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get Follower Parameter Data
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	IOnlineFollower::GetFollowerParameterLevel(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetLevel();	
}

SI32	IOnlineFollower::GetFollowerParameterCurExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCurExp();
}

SI32	IOnlineFollower::GetFollowerParameterPrevExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return GetRequiredExpForLevel(GetFollowerParameterLevel(uiFollowerSlot));
}

SI32	IOnlineFollower::GetFollowerParameterNextExp(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetNextExp();
}

SI32	IOnlineFollower::GetFollowerParameterNation(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].Nation;
}

VOID	IOnlineFollower::SetFollowerParameterNation(UI08 uiFollowerSlot, SI32 Nation)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return;

	Followers[uiFollowerSlot].SetFollowerParameterNation(Nation);
}

SI32	IOnlineFollower::GetFollowerParameterTotalStr(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalStr();
}

SI32	IOnlineFollower::GetFollowerParameterTotalDex(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalDex();
}

SI32	IOnlineFollower::GetFollowerParameterTotalVit(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalVit();
}

SI32	IOnlineFollower::GetFollowerParameterTotalInt(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalInt();
}

SI32	IOnlineFollower::GetFollowerParameterTotalAC(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetDpower();
}


SI32	IOnlineFollower::GetFollowerParameterTotalDamageResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalDamageResist();
}

SI32	IOnlineFollower::GetFollowerParameterTotalMagicResist(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetTotalMagicResist();
}

SI32	IOnlineFollower::GetFollowerParameterTotalEquipMinDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMinApower();
}

SI32	IOnlineFollower::GetFollowerParameterTotalEquipMaxDamage(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxApower();
}

SI32	IOnlineFollower::GetFollowerParameterLife(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetLife();
}

BOOL	IOnlineFollower::SetFollowerParameterLife(UI08 uiFollowerSlot, SI32 Life)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetLife(Life);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerParameterMana(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetMana();
}

BOOL	IOnlineFollower::SetFollowerParameterMana(UI08 uiFollowerSlot, SI32 Mana)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return FALSE;

	Followers[uiFollowerSlot].ParametaBox.IP.SetMana(Mana);
	return TRUE;
}

SI32	IOnlineFollower::GetFollowerParameterMoveSpeed(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMoveSpeed();
}

SI32	IOnlineFollower::GetFollowerParameterMaxWeight(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxWeight();
}

SI32	IOnlineFollower::GetFollowerParameterCurWeight(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCurWeight();
}

SI32	IOnlineFollower::GetFollowerParameterCredit(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetCredit();
}

SI32	IOnlineFollower::GetFollowerParameterLiftPower(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetLiftPower();
}

SI16	IOnlineFollower::GetFollowerParameterBonus(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
}


SI32	IOnlineFollower::GetFollowerParameterMaxLife(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxLife();
}
SI32	IOnlineFollower::GetFollowerParameterMaxMana(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetMaxMana();
}


SI32	IOnlineFollower::GetFollowerParameterBattlePower(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].ParametaBox.GetBattlePower();
}

SI32	IOnlineFollower::GetFollowerParameterBattlePower(void)
{
	SI32		TempCount;
	SI32		TempBattlePower;

	TempBattlePower = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		TempBattlePower += GetFollowerParameterBattlePower(TempCount);
	}

	return TempBattlePower;
}

SI32	IOnlineFollower::GetFollowerParameterKind(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return 0;

	return Followers[uiFollowerSlot].uiCharID;
}

char*	IOnlineFollower::GetFollowerName(UI08 uiFollowerSlot)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE) return NULL;

	return (char*)Followers[uiFollowerSlot].Name;
}

BOOL	IOnlineFollower::SetFollowerName(UI08 uiFollowerSlot, char* lpName)
{
	OnlineFollower*		lpFollower;

	lpFollower		=	GetFollower(uiFollowerSlot);
	if(lpFollower)
	{		
		ZeroMemory(lpFollower->Name, sizeof(Followers[uiFollowerSlot].Name));
		if(lpName)
		{
			memcpy(lpFollower->Name, lpName,ON_ID_LENGTH);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL	IOnlineFollower::Bonus_FollowerParameterUp(UI08 uiFollowerSlot, AbilityKindType Type, SI16 Bonus, CItem* pItem)
{
	if(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() < Bonus) return FALSE;

	switch(Type)
	{
		case ON_ABILITY_KIND_STR:
			Followers[uiFollowerSlot].ParametaBox.IP.SetStr(Followers[uiFollowerSlot].ParametaBox.IP.GetStr() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_DEX:
			Followers[uiFollowerSlot].ParametaBox.IP.SetDex(Followers[uiFollowerSlot].ParametaBox.IP.GetDex() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_VIT:
			Followers[uiFollowerSlot].ParametaBox.IP.SetVit(Followers[uiFollowerSlot].ParametaBox.IP.GetVit() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;

		case ON_ABILITY_KIND_INT:
			Followers[uiFollowerSlot].ParametaBox.IP.SetInt(Followers[uiFollowerSlot].ParametaBox.IP.GetInt() + Bonus);
			Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(Followers[uiFollowerSlot].ParametaBox.IP.GetBonus() - Bonus);
			Followers[uiFollowerSlot].ParametaBox.Action(pItem, Followers[uiFollowerSlot].Weapon);
			return TRUE;
	}

	return FALSE;
}

BOOL	IOnlineFollower::AllCharacterDeleteLife(SI32 DeleteLife)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - ((Followers[TempCount].ParametaBox.GetMaxLife() * DeleteLife) / 100);
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}
BOOL	IOnlineFollower::AllCharacterDeleteLifePoint(SI32 DeleteLife)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - DeleteLife;
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}

BOOL	IOnlineFollower::AllCharacterDeleteLifePercent(SI32 DeleteLifeP)
{
	SI32		TempCount;
	SI32		TempLife;

	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			if(GetFollowerParameterLife(TempCount) >= 1)
			{
				TempLife = GetFollowerParameterLife(TempCount) - ((GetFollowerParameterMaxLife(TempCount) * DeleteLifeP) / 100);
				if(TempLife < 1) TempLife = 1;

				Followers[TempCount].ParametaBox.IP.SetLife(TempLife);
			}
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� ���� ���´�.
//	���� ����	:	2002-03-25 ���� 3:00:40 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	IOnlineFollower::GetFollowerNum()
{	
	UI08	i;
	UI08	siTotalFollowerNum; 

	// ���ϵ��� ����Ÿ�� �ʱ�ȭ �Ѵ�.
	siTotalFollowerNum	=	0;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsValidFollower(i) == TRUE)
			siTotalFollowerNum++;
	} 

	return	siTotalFollowerNum ;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ���´�
//	���� ����	:	2002-05-02 ���� 3:02:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::GetBattleInfo(UI08 uiFollowerSlot, OnImjinBattleFollowerInfo *pFollowerInfo)
{
	OnlineFollower		*pOnlineFollower;
	MyItemData			*pMyItemData;	
	SI32				i;
	DWORD				dwAllItemStructSize;
	

	pOnlineFollower	=	&Followers[uiFollowerSlot];	

	if(IsValidFollower(uiFollowerSlot) == TRUE)
	{
		// ���� ����.
		pFollowerInfo->uiSlotID		=	uiFollowerSlot;

		// �̸� ����.
		strncpy(pFollowerInfo->szPlayerName, pOnlineFollower->Name, ON_ID_LENGTH);
	
		pFollowerInfo->uiKind	=	pOnlineFollower->uiCharID;			

		// Parameta Box ����.
		memcpy(&pFollowerInfo->Iparameta, &pOnlineFollower->ParametaBox.IP, sizeof(_ImportantParameta));

		// My Weapon ����.
		memcpy(&pFollowerInfo->MW, &pOnlineFollower->Weapon, sizeof(MyWeapons));

		pMyItemData				=	(MyItemData*)((CHAR*)(pFollowerInfo) + sizeof(OnImjinBattleFollowerInfo));

		dwAllItemStructSize				=	0;
		pFollowerInfo->uiItemNum		=	0;
		for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
		{
			if(pOnlineFollower->Items[i].uiID != 0)
			{
				*pMyItemData	=	pOnlineFollower->Items[i];
				
				// �����͸� ������Ų��.
				// MyItemData���̹Ƿ� sizeof(MyItemData) ��ŭ �����ȴ�.
				pMyItemData++;

				pFollowerInfo->uiItemNum++;
			}
		}

		return	DWORD(sizeof(OnImjinBattleFollowerInfo) +  (pFollowerInfo->uiItemNum * sizeof(MyItemData)));
	}
	else
		return	0;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ����� ������ ��ġ �˻�
//	���� ����	:	2002-06-19 ���� 9 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32 IOnlineFollower::GetUseSatietyWarAfter(void)
{
	SI32		TempCount;
	SI32		TempUseSatiety;

	TempUseSatiety = 0;
	for(TempCount = 0; TempCount < ON_MAX_FOLLOWER_NUM; TempCount++)
	{
		if(IsValidFollower(TempCount))
		{
			TempUseSatiety += 1 + ( GetFollowerParameterTotalStr( TempCount ) / 20 ) ;
		}
	}

	return TempUseSatiety;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-05 ���� 7:20:32 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity, SI16 siPosInInventory)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		
	{
		return pFollower->CanAddItem(uiItemID, uiQuantity, siPosInInventory);
	}
	else
	{
		return FALSE;	
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-07 ���� 4:38:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 *puiID, UI16 *puiQuantity, UI16 uiItemNum)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(puiID, puiQuantity, uiItemNum);
	else				return FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-07 ���� 5:01:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanAddFollower(UI08 uiAddFollowerNum)
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(Followers[i].uiCharID == 0)
		{
			uiAddFollowerNum--;

			if(uiAddFollowerNum == 0)	return	TRUE;
		}
	}

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ִ� �뺴 ���۸� ã�´�.
//	���� ����	:	2002-07-08 ���� 5:01:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	IOnlineFollower::FindEmptyFollowerSlot()
{
	SI32				i;	

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
		if(Followers[i].uiCharID == 0)	return	i;

	return	-1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� ���� ������ ���´�.
//	���� ����	:	2002-07-09 ���� 1:59:13 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::GetFollowerTradeInfo(UI08 uiFollowerSlot, OnTradeFollowerInfo *pTradeFollowerInfo)
{
	OnlineFollower	*pFollower;
	
	if((pFollower = GetFollower(uiFollowerSlot)) == FALSE)	return	FALSE;

	pTradeFollowerInfo->uiKind				=		pFollower->uiCharID;
	pTradeFollowerInfo->siLevel				=		pFollower->ParametaBox.IP.GetLevel();
	pTradeFollowerInfo->siExp				=		pFollower->ParametaBox.IP.GetCurExp();
	pTradeFollowerInfo->siSTR				=		pFollower->ParametaBox.IP.GetStr();
	pTradeFollowerInfo->siDEX				=		pFollower->ParametaBox.IP.GetDex();
	pTradeFollowerInfo->siVIT				=		pFollower->ParametaBox.IP.GetVit();
	pTradeFollowerInfo->siINT				=		pFollower->ParametaBox.IP.GetInt();
	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �ʱ�ȭ.
//	���� ����	:	2002-07-12 ���� 3:28:25 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineFollower::InitStaticVariable(CItem* pItem)
{
	OnlineFollower::InitStaticVariable(pItem);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �� ĳ������ ��ü �������� ���Ը� ���´�.	
//	���� ����	:	2002-07-12 ���� 3:41:59 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::GetTotalItemWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetTotalItemWeight();
	else				return 0;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ĳ���Ͱ� �ְ� �� �� �ִ� ����.
//	���� ����	:	2002-07-12 ���� 5:34:50 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	IOnlineFollower::GetMaxWeight(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetMaxWeight();
	else				return 0;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�� ĳ���Ͱ� �ְ� ���� �� �ִ� �������� ������ ���´�.
//	���� ����	:	2002-07-12 ���� 5:51:38 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
UI16	IOnlineFollower::GetItemQuantityToTake(UI08 uiFollowerSlot, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetItemQuantityToTake(uiItemID);
	else				return 0;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �߰� ��ų �� �ִ���?
//	���� ����	:	2002-07-12 ���� 7:58:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanAddItem(UI08 uiFollowerSlot, UI16 uiItemID, UI16 uiQuantity)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanAddItem(uiItemID, uiQuantity);
	else				return FALSE;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴�� �߿� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.
//	���� ����	:	2002-07-12 ���� 8:21:02 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::IsItemWeightOverflowed()
{
	SI32	i;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		if(IsValidFollower(i) == TRUE)
		{
			if(Followers[i].IsItemWeightOverflowed() == TRUE)	return	TRUE;
		}
	}

	return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Ư�� �뺴�� �ڱⰡ ������ �� �ִ°� ���� �������� �ʰ� �ؼ� ����� ���´�.
//	���� ����	:	2002-07-12 ���� 8:22:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::IsItemWeightOverflowed(UI08 uiFollowerSlot)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->IsItemWeightOverflowed();
	else				return 0;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�����ϰ� �ִ� �������� ���´�.
//	���� ����	:	2002-07-12 ���� 9:16:04 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::TakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return	pFollower->TakeOffItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);	
	}

	return FALSE;
}

BOOL	IOnlineFollower::DisAppearItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return	pFollower->DisAppearItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);	
	}

	return FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� �����Ѵ�.
//	���� ����	:	2002-07-13 ���� 2:15:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::PutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, MyItemData *pMyItemData)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if( pFollower )
	{
		return pFollower->PutOnItem(uiWearLoc, uiItemID, siPosInInventory, pMyItemData);
	}
	
	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �����۵��� ����Ʈ�� ���´�.
//	���� ����	:	2002-07-13 ���� 2:54:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::GetWearItems(UI08 uiFollowerSlot, UI16 puiWearItems[WEAR_ITEM_NUM])
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->GetWearItems(puiWearItems);
	else				return FALSE;			
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �����Ѵ�.
//	���� ����	:	2002-07-15 ���� 12:43:31 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::SetWearItem2(UI08 uiFollowerSlot, UI08 uiLoc, UI16 uiItemID)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->SetWearItem(uiLoc, uiItemID,OnlineFollower::pCItem->GetItemUsingDay(uiItemID));
	else				return FALSE;				
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 3:58:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanPutOnItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory, SI32 siTradeGrade, SI08 &ErrorCode)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanPutOnItem(uiWearLoc, uiItemID, siPosInInventory, siTradeGrade, ErrorCode);
	else				return FALSE;				
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�κ��丮�� �ִ� �������� ������ �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-07-15 ���� 3:59:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanTakeOffItem(UI08 uiFollowerSlot, UI08 uiWearLoc, UI16 uiItemID, SI16 siPosInInventory)
{
	OnlineFollower		*pFollower;

	pFollower	=	GetFollower(uiFollowerSlot);

	if(pFollower)		return pFollower->CanTakeOffItem(uiWearLoc, uiItemID, siPosInInventory);
	else				return FALSE;					
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �̵� ��ų �� �ִ��� �˾ƿ´�.
//	���� ����	:	2002-11-12 ���� 10:30:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanMoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 siPosInInventory )
{
	OnlineFollower		*pFollower1;
	OnlineFollower		*pFollower2;
	

	if( ( pFollower1 = GetFollower( uiFollowerSlot1 ) ) == NULL )										return	FALSE;
	if( ( pFollower2 = GetFollower( uiFollowerSlot2 ) ) == NULL )										return	FALSE;

	if( pFollower1 != pFollower2 )
	{
		// �������� ������ �ִ��� �˻��Ѵ�.
		if( pFollower1->HasItem( uiItemID, uiItemQuantity ) == FALSE )									return	FALSE;

		// �������� �޴� ĳ���Ͱ� �������� ���� �� �ִ��� �˻��Ѵ�.
		if( pFollower2->CanAddItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )				return	FALSE;
	}
	else
	{
		if( pFollower1->CanMoveItem( uiItemID, uiItemQuantity, siPosInInventory ) == FALSE )			return	FALSE;
	}


	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� �̵� ��Ų��.
//	���� ����	:	2002-11-12 ���� 10:24:46 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::MoveItem( UI08 uiFollowerSlot1, UI16 uiItemID, UI16 uiItemQuantity, UI08 uiFollowerSlot2, SI16 *psiPosInInventory )
{
	OnlineFollower		*pFollower1;
	OnlineFollower		*pFollower2;
	MyItemData			ItemData;

	if( CanMoveItem( uiFollowerSlot1, uiItemID, uiItemQuantity, uiFollowerSlot2, *psiPosInInventory ) == FALSE )		return	FALSE;

	pFollower1 = GetFollower( uiFollowerSlot1 );
	pFollower2 = GetFollower( uiFollowerSlot2 );

	if( pFollower1 != pFollower2 )
	{
		// �������� �����Ѵ�.
		if( pFollower1->DelItem(uiItemID, uiItemQuantity) == ONLINEFOLLOWER_DELITEMRETURNTYPE_FALSE )					return	FALSE;

		// �������� �߰��Ѵ�.		
		ItemData.uiID				=	uiItemID;
		ItemData.uiQuantity			=	uiItemQuantity;
		ItemData.siPosInInventory	=	*psiPosInInventory;
		if( pFollower2->AddItem(&ItemData) == ONLINEFOLLOWER_ADDITEMRETURNTYPE_FALSE )									return	FALSE;
	
		*psiPosInInventory			=	ItemData.siPosInInventory;
	}
	else
	{
		pFollower1->MoveItem( uiItemID, uiItemQuantity, psiPosInInventory );
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�뺴 ������ ���ʽ� ��ġ�� �ٲ۴�.
//	���� ����	:	2002-12-22 - ���ؼ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::ChangeFollowerParameterToBonus(UI08 uiFollowerSlot, OnlineCharParser *pCharParser, UI16 uiFollowerID , SI32 *pBonus, CItem* pItem)
{
	if(IsValidFollower(uiFollowerSlot) == FALSE)  return FALSE;

	SI32 BaseStr, BaseInt, BaseDex, BaseVit, BaseHP, BaseMP, BaseTot;
	SI32 CurStr, CurInt, CurDex, CurVit, CurBonus, CurTot;
	SI32 TotBonus;

	// �뺴�� �⺻��ġ
	BaseStr = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetStr();
	BaseInt = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetInt();
	BaseDex = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetDex();
	BaseVit = pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetVit();
	BaseHP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetLife();
	BaseMP	= pCharParser->GetCharHeaderID(uiFollowerID)->Parameta.GetMana();
	BaseTot = BaseStr + BaseInt + BaseDex + BaseVit;

	// �뺴�� �����ġ
	CurStr	= Followers[uiFollowerSlot].ParametaBox.IP.GetStr();
	CurInt	= Followers[uiFollowerSlot].ParametaBox.IP.GetInt();
	CurDex	= Followers[uiFollowerSlot].ParametaBox.IP.GetDex();
	CurVit	= Followers[uiFollowerSlot].ParametaBox.IP.GetVit();
	CurBonus= Followers[uiFollowerSlot].ParametaBox.IP.GetBonus();
	CurTot	= CurStr + CurInt + CurDex + CurVit + CurBonus;

	TotBonus = CurTot - BaseTot;

	if (pBonus) {
		*pBonus = TotBonus;
	}

	// �뺴 �⺻��ġ�� ���� �ʱ�ȭ
	Followers[uiFollowerSlot].ParametaBox.IP.SetStr(BaseStr);
	Followers[uiFollowerSlot].ParametaBox.IP.SetInt(BaseInt);
	Followers[uiFollowerSlot].ParametaBox.IP.SetDex(BaseDex);
	Followers[uiFollowerSlot].ParametaBox.IP.SetVit(BaseVit);
	Followers[uiFollowerSlot].ParametaBox.IP.SetLife(BaseHP);
	Followers[uiFollowerSlot].ParametaBox.IP.SetMana(BaseMP);
	Followers[uiFollowerSlot].ParametaBox.IP.SetBonus(TotBonus);

	OnlineFollower *lpFollower = &Followers[uiFollowerSlot];
	lpFollower->ParametaBox.Action(pItem, lpFollower->Weapon);

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���׷��̵� ������ �������� �˾ƿ´�. (��޹���)
//	���� ����	:	2003-3-31 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	IOnlineFollower::CanWeaponBeUpgraded( UI16 uiFollowerSlot )
{
	return Followers[uiFollowerSlot].CanWeaponBeUpgraded();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ���� �������� Ư�� ���������� ���׷��̵� �����ش�.
//	���� ����	:	2003-3-31 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineFollower::UpgradeWeapon( UI16 uiFollowerSlot, UI16 uiItemID )
{
	Followers[uiFollowerSlot].UpgradeWeapon( uiItemID );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�������� ���� �������� �ı���Ų��.
//	���� ����	:	2003-3-31 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineFollower::BrakeWeapon( UI16 uiFollowerSlot )
{
	Followers[uiFollowerSlot].BrakeWeapon();
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �뺴���� ü���� 100% ȸ�������ش�.
//	���� ����	:	2003-09-18 16:28 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineFollower::SetFullLifeOfAllChar()
{
	OnlineFollower				*pOnlineFollower;

	for( SI32 i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			pOnlineFollower		=	GetFollower(i);
			pOnlineFollower->ParametaBox.IP.SetLife( pOnlineFollower->ParametaBox.GetMaxLife() );
		}
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� �뺴���� ������ 100% ȸ�������ش�.
//	���� ����	:	2003-09-18 16:33 - �����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	IOnlineFollower::SetFullManaOfAllChar()
{
	OnlineFollower				*pOnlineFollower;

	for( SI32 i = 0; i < ON_MAX_FOLLOWER_NUM; i++ )
	{
		if( IsValidFollower(i) )
		{
			pOnlineFollower		=	GetFollower(i);
			pOnlineFollower->ParametaBox.IP.SetMana( pOnlineFollower->ParametaBox.GetMaxMana() );
		}
	}
}
