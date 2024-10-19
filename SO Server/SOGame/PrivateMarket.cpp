#include "SOMain.h"
#include "SOPlayer.h"
#include "PrivateMarket.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-07-02 ���� 8:10:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltPrivateMarket::Init(SOPlayer *pOwnerPlayer)
{
	m_pOwnerPlayer	=	pOwnerPlayer;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ʱ�ȭ.
//	���� ����	:	2002-07-03 ���� 7:29:05 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltPrivateMarket::Init()
{
	SI32	i;

	m_bOpen				=	FALSE;
	
	// Ȥ�ó� ������ �𸣴� ��� �������� �����.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ ����.
//	���� ����	:	2002-07-03 ���� 7:27:42 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltPrivateMarket::Open()
{
	m_bOpen	=	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ݴ´�.
//	���� ����	:	2002-07-03 ���� 7:27:48 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltPrivateMarket::Close()
{
	SI32	i;	

	// ��� �������� �����Ѵ�.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		DelItem(i);

	m_bOpen	=	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ȿ�� �������� �˾ƿ´�.
//	���� ����	:	2002-07-02 ���� 7:48:57 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::IsValidSlot(UI08 uiSlot)
{
	if(uiSlot >= 0 && uiSlot < ON_MAX_BOOTH_ITEM)	return	TRUE;
	else											return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ Ȱ��ȭ �Ǿ��°�?
//	���� ����	:	2002-07-03 ���� 7:28:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::IsOpen()
{
	return m_bOpen;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �������� �߰��Ѵ�. ( �߰� : ���� ������ �뺴�� �߰��Ѵ�. �Ŀ� �ǹ� �ŷ��� ���� �غ� �뷫�� �� ���Ҵ�.)
//	���� ����	:	2002-07-02 ���� 7:50:31 - ������ ( �̻�� : 2003-03-14 ���� 11 : 45 )
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//BOOL	cltPrivateMarket::AddItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote)
BOOL	cltPrivateMarket::AddItem( OnBoothItemForSell	*pBoothItem )
{	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� ���� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if ( ! IsValidSlot (pBoothItem->uiBoothSlot ) )
	{
		return FALSE;
	}

	// ���� ��� ���� �����͸� �����Ѵ�.
	strPMItem *pstPMItem;

	pstPMItem	=	&m_stPMItem[pBoothItem->uiBoothSlot];

	// �ش� ������ �ݵ�� ��� �־�� �Ѵ�.
	//�ش� ���ǿ� item �̳� �뺴�� �ִ� �� üũ �Ѵ�.
	if( pstPMItem->uiItemID || pstPMItem->uiTradeFollowerSlotID )
	{
		return	FALSE;
	}

	ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));

	if ( pBoothItem->uiTradeFollowerID )
	{
		pstPMItem->uiTradeFollowerSlotID = pBoothItem->uiTradeFollowerID;
		pstPMItem->Price			=	pBoothItem->Price;
		strncpy(pstPMItem->szNote, pBoothItem->Desc, ON_MAX_BOOTH_NOTE);
		pstPMItem->siType = LIST_FOLLOWER;
	}
	else if ( pBoothItem->uiItemID )
	{
		pstPMItem->uiFollowerID		=	pBoothItem->uiFollowerID;		
		pstPMItem->Price = pBoothItem->Price;
		strncpy(pstPMItem->szNote, pBoothItem->Desc, ON_MAX_BOOTH_NOTE);
		pstPMItem->uiItemID = pBoothItem->uiItemID;
		pstPMItem->siType = LIST_ITEM;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �� �� �� ���� ( 2003. 3. 14 )
// �뺴 �ŷ� �κ� �߰� �� �κ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	/*
	strPMItem *pstPMItem;

	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// ���� ��� ���� �����͸� �����Ѵ�.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// �ش� ������ �ݵ�� ��� �־�� �Ѵ�.
	if(pstPMItem->uiItemID != 0)
	{
		return	FALSE;
	}

	// ������ ä�� �ִ´�.
	pstPMItem->uiFollowerID		=	uiFollowerID;
	pstPMItem->Price			=	Price;

	// ������ �����Ѵ�.
	ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
	trncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

	// ������ �������� ä�� �ִ´�.	
	pstPMItem->uiItemID	=	uiItemID;
	*/

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �������� �����Ѵ�.
//	���� ����	:	2002-07-02 ���� 7:58:26 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::DelItem(UI08 uiSlotID)
{
	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if( IsValidSlot(uiSlotID) == FALSE )
	{
		return	FALSE;
	}

	ZeroMemory(&m_stPMItem[uiSlotID], sizeof(strPMItem));	
	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �������� ���. ( �뺴�� �����ϴ� ���� �� �Լ��� ���� �ʴ´�.)
//	���� ����	:	2002-07-02 ���� 8:00:03 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::BuyItem(UI08 uiSlotID, UI16 uiQuantity, UI16 *puiRemainQuantity, MONEY *pTotalItemPrice,SI16 *psiFromMercenarySlot)
{
	strPMItem		*pstPMItem;		
	MyItemData		*pMyItem;

	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if(IsValidSlot(uiSlotID) == FALSE)
	{
		return	FALSE;
	}
	
	// ���� ��� ���� �����͸� �����Ѵ�.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	// �Ǹ��ϴ� ���������� �˻��Ѵ�.
	if(pstPMItem->uiItemID == 0)
	{
		return	FALSE;
	}

	// �������� ������ ���´�.	
	if((pMyItem	= m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) == NULL)
	{
		return	FALSE;
	}

	// ����� �ϴ� ������ �ȷ��� �ϴ� ������ �´��� �˻��Ѵ�.
	if(pMyItem->uiQuantity < uiQuantity)
	{
		return	FALSE;
	}

	// �������� ���ش�.
	if(m_pOwnerPlayer->DelItemNoQuery(pstPMItem->uiFollowerID, pstPMItem->uiItemID, uiQuantity) == FALSE)
	{
		return	FALSE;
	}
	
	// ���� ���� ������ ������ �˷��ش�.
	*puiRemainQuantity	=	pMyItem->uiQuantity;
	*pTotalItemPrice	=	pstPMItem->Price * uiQuantity;
	*psiFromMercenarySlot = pstPMItem->uiFollowerID;
	

	// ���� ���� ������ 0 �̶�� ����ϴ� ������ ��Ͽ��� �����.
	if(*puiRemainQuantity == 0)	
	{
		DelItem(uiSlotID);
	}

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �Ǹ��ϴ� �������� �ٲ۴�.
//	���� ����	:	2002-07-02 ���� 8:29:08 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::ChangeItem(UI08 uiSlotID, UI08 uiFollowerID, UI16 uiItemID, MONEY Price, CHAR *pszNote, UI08	uiTradeFollowerID)
{
	strPMItem	*pstPMItem;

	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if( ! IsValidSlot(uiSlotID) )
	{
		return	FALSE;
	}
	
	// ���� ��� ���� �����͸� �����Ѵ�.
	pstPMItem	=	&m_stPMItem[uiSlotID];

	if ( uiItemID )
	{
		pstPMItem->siType = LIST_ITEM;
		// ������ ä�� �ִ´�.
		pstPMItem->uiFollowerID		=	uiFollowerID;
		pstPMItem->Price			=	Price;

		// ������ �����Ѵ�.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);						

		// ������ �������� ä�� �ִ´�.	
		pstPMItem->uiItemID	=	uiItemID;
	}
	else if ( uiTradeFollowerID )
	{
		pstPMItem->siType = LIST_FOLLOWER;
		// ������ �뺴�� ä�� �ִ´�.	
		pstPMItem->uiTradeFollowerSlotID = uiTradeFollowerID;
		pstPMItem->Price = Price;

		// ������ �����Ѵ�.
		ZeroMemory(pstPMItem->szNote, sizeof(pstPMItem->szNote));
		strncpy(pstPMItem->szNote, pszNote, ON_MAX_BOOTH_NOTE);
	}
	else
	{
		return FALSE;
	}

	return	TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �Ǹ��ϴ� �������� ������ ���´�.
//	���� ����	:	2002-07-02 ���� 8:57:49 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
strPMItem*	cltPrivateMarket::GetItem(UI08 uiSlotID)
{
	// ������ ID�� ��ȿ���� �˻��Ѵ�.
	if( ! IsValidSlot(uiSlotID) )
	{
		return	NULL;
	}
	else
	{
		if( ( m_stPMItem[uiSlotID].uiItemID != 0 ) || ( m_stPMItem[uiSlotID].uiTradeFollowerSlotID != 0 ) )
		{
			return	&m_stPMItem[uiSlotID];
		}		
		else
		{
			return	NULL;
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �Ǹ��ϴ� �������� ������ ���´�.
//	���� ����	:	2002-07-11 ���� 2:47:33 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
strPMItem*	cltPrivateMarket::GetItem(UI08 uiSlotID, UI16 uiItemID)
{	
	strPMItem	*pstPMItem;

	if((pstPMItem = GetItem(uiSlotID)) != NULL)
	{
		if(pstPMItem->uiItemID == uiItemID)
			return pstPMItem;
	}
	
	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� ���� �Ǹ��ϴ� �����۵��� ���´�.
//	���� ����	:	2002-07-03 ���� 9:06:08 - ������
//  2�� ����    :   2003-03-17 ���� 3:00:00 - �̻�� (�뺴�� ���� ������ ��������)
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	cltPrivateMarket::GetBaseItems(OnBaseBoothItem *pBaseBoothItem, SI08 *psiItemNum)
{
	DWORD		dwUsedSize;
	SI32		i;
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	
	// �� ���� �ʱ�ȭ�Ѵ�.
	siItemNum	=	-1;
	dwUsedSize	=	0;

	if(IsOpen() == FALSE)
	{
		goto END;
	}

	for(siItemNum = i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		if((pstPMItem = GetItem(i)) != NULL)
		{
			switch( pstPMItem->siType )
			{
			case LIST_ITEM:
				pBaseBoothItem[siItemNum].siType = pstPMItem->siType;
				pBaseBoothItem[siItemNum].uiBoothSlot	=	i;
				pBaseBoothItem[siItemNum].uiProductID		=	pstPMItem->uiItemID;
				break;
			case LIST_FOLLOWER:
				pBaseBoothItem[siItemNum].siType = pstPMItem->siType;
				pBaseBoothItem[siItemNum].uiBoothSlot	=	i;
				pBaseBoothItem[siItemNum].uiProductID	=	m_pOwnerPlayer->IFollower.GetFollowerParameterKind(pstPMItem->uiTradeFollowerSlotID);
				break;
			}
			

			// ��ȿ�� �����̴�.
			siItemNum++;			// �������� �ϳ� ������Ų��.			
		}
	}

	dwUsedSize	=	sizeof(OnBaseBoothItem) * siItemNum;

END:	
	*psiItemNum	=	siItemNum;

	return	dwUsedSize;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� ���� �Ǹ��ϴ� �����۵��� ���´�.	
//	���� ����	:	2002-07-05 ���� 11:08:55 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	cltPrivateMarket::GetItems(OnBoothItemInfo *pBoothItem, SI08 *psiItemNum, SOPlayer* pPlayer, MyItemData *pFollowerItems, UI16 &numFollowerItems)
{
	DWORD		dwUsedSize;
	SI32		i;	
	SI08		siItemNum;
	strPMItem	*pstPMItem;
	MyItemData	*pItemData;
	MyItemData	*pCurFollowerItem	=	pFollowerItems;
	
	// �� ���� �ʱ�ȭ�Ѵ�.
	siItemNum	=	-1;
	dwUsedSize	=	0;

	numFollowerItems	=	0;	// ������ ������ ��, Booth�� �ö�� ��� �뺴�� MyItemData���� ������ ��(������ ������)

	if( IsOpen() == FALSE )
	{
		goto END;
	}

	for( siItemNum = i = 0; i < ON_MAX_BOOTH_ITEM; i++ )
	{
		if( ( pstPMItem = GetItem(i) ) != NULL )
		{
			switch( pstPMItem->siType ) 
			{
			case LIST_ITEM:
				{
					if( ( pItemData = m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) != NULL )
					{
						pBoothItem[siItemNum].uiBoothSlot	=	i;
						pBoothItem[siItemNum].Item.uiItemID	=	pItemData->uiID;
						pBoothItem[siItemNum].Item.uiItemCount =	pItemData->uiQuantity;
						pBoothItem[siItemNum].siPrice	=	pstPMItem->Price;
						pBoothItem[siItemNum].siType	= LIST_ITEM;

						ZeroMemory( &pBoothItem[siItemNum].Follower, sizeof(OnFollowerInfo) );		// �뺴 ���� ����

						ZeroMemory( pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note) );
						strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);

						// ��ȿ�� �����̴�.
						siItemNum++;			// �������� �ϳ� ������Ų��.			
					}
					break;
				}
			case LIST_FOLLOWER:
				{
					pBoothItem[siItemNum].uiBoothSlot	=	i;
					pBoothItem[siItemNum].Item.uiItemID	=	0;
					pBoothItem[siItemNum].Item.uiItemCount = 1;
					pBoothItem[siItemNum].siPrice		=	pstPMItem->Price;
					pBoothItem[siItemNum].siType		= LIST_FOLLOWER;

					CHAR	cFollowerInfoBuf[8192];
					pPlayer->GetFollowerInfo( pstPMItem->uiTradeFollowerSlotID, (OnFollowerInfo*)cFollowerInfoBuf );	// �뺴 ���� �޾ƿ���
					memcpy( &pBoothItem[siItemNum].Follower, cFollowerInfoBuf, sizeof(OnFollowerInfo) );	// �뺴 ���� ä���

					SI32	numItem		=	pBoothItem[siItemNum].Follower.uiItemNum;			// �뺴�� ���� �ִ� ������ ����(���� ������ ��)
					memcpy( pCurFollowerItem, cFollowerInfoBuf + sizeof(OnFollowerInfo), sizeof(MyItemData) * numItem );	// �뺴�� ������ ���� ä���
					pCurFollowerItem	+=	numItem;
					numFollowerItems	+=	numItem;

					ZeroMemory(pBoothItem[siItemNum].Note, sizeof(pBoothItem[siItemNum].Note));
					strncpy(pBoothItem[siItemNum].Note, pstPMItem->szNote, ON_MAX_BOOTH_NOTE);
					
					siItemNum++;			// �������� �ϳ� ������Ų��.			

					break;
				}
			}	// end of switch
		}	// end of if
	}	// end of for

	dwUsedSize	=	sizeof(OnBoothItemInfo) * siItemNum;

END:	
	*psiItemNum	=	siItemNum;

	return	dwUsedSize;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �Ǹ��ϰ� �ִ� �������� ������ ���´�.
//	���� ����	:	2002-07-05 ���� 8:44:30 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL cltPrivateMarket::GetItemQuantity(UI08 uiSlotID, UI16 *puiQuantity)
{
	BOOL			bSuccess;
	strPMItem		*pstPMItem;
	MyItemData		*pItemData;

	if( IsOpen() == FALSE )
	{
		goto	END;
	}
	if( ( pstPMItem = GetItem(uiSlotID) ) == NULL )
	{
		goto	END;
	}
	if( ( pItemData = m_pOwnerPlayer->IFollower.GetItemByID( pstPMItem->uiFollowerID, pstPMItem->uiItemID ) ) == NULL )
	{
		goto	END;
	}

	// ������ �����Ѵ�.
	*puiQuantity	=	pItemData->uiQuantity;

	bSuccess	=	TRUE;
END:
	return	bSuccess;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� �������� �������� �� �� �ִ°�?	
//	���� ����	:	2002-07-05 ���� 9:24:47 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltPrivateMarket::CanBuyItem(UI08 uiSlotID, UI16 uiQuantity, MONEY Money)
{
	strPMItem		*pstPMItem;
	MyItemData		*pItemData;

	if( IsOpen() == FALSE)
	{
		return	FALSE;
	}
	if((pstPMItem = GetItem(uiSlotID)) == NULL)
	{
		return	FALSE;
	}
	if((pItemData = m_pOwnerPlayer->IFollower.GetItemByID(pstPMItem->uiFollowerID, pstPMItem->uiItemID)) == NULL)
	{
		return	FALSE;
	}
	if(pItemData->uiQuantity < uiQuantity)
	{
		return	FALSE;
	}
	if((uiQuantity * pstPMItem->Price) > Money)
	{
		return	FALSE;
	}

	return	TRUE;
}

// ���� �������� �Ǹ��ϰ� �ִ� �������� ������ �˾ƿ´�.
// �뺴�� ��쿡�� uiQuantity�� ���õȴ�.
BOOL	cltPrivateMarket::GetTotalPrice( UI08 uiSlotID, UI16 uiQuantity, MONEY *pPrice )
{
	strPMItem		*pstPMItem;
	
	if( IsOpen() == FALSE )									return	FALSE;
	if( ( pstPMItem = GetItem( uiSlotID ) ) == NULL )		return	FALSE;

	switch( pstPMItem->siType )
	{
	case LIST_ITEM:
		*pPrice		=	( pstPMItem->Price * uiQuantity );
		break;

	case LIST_FOLLOWER:	
		// �뺴�� ��쿡�� uiQuantity�� ���õȴ�.
		*pPrice		=	pstPMItem->Price;
		break;


	default:
		return	FALSE;

	}

	return	TRUE;
}
