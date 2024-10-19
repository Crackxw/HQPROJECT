#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"

//����������������������������������������������������������������������������
//	�������� �ڻ��� ��� �Ѵ�.
//����������������������������������������������������������������������������
MONEY	SOPlayer::CalculateItemValue(MONEY &siItemMoney)
{
	SI32			i, j;
	MyItemData		stItem;
	MONEY			siItemValue = 0;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i�� uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			for( j=0; j< ON_MYDATA_MAX_ITEM_NUM; j++)		//	j�� ������ �迭�� Index
			{
				if( IFollower.GetItem( i, j, &stItem) == TRUE )
				{
					CItemHeader	*pItemHeader = pMyWorld->clItem.GetItem(stItem.uiID);
					siItemValue += ( pItemHeader->m_siDBPrice ) * stItem.uiQuantity;
				}
			}
		}
	}

	siItemMoney		=	siItemValue;
	return siItemMoney;
}

//����������������������������������������������������������������������������
//	��ο��� ����� ������ �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditTotalInfo()
{
	if( GetTotalProperty( stPropertyData ) == ON_RET_AUDIT_OK )	SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK), LPVOID(&stPropertyData));	
	else														SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��ο��� ����� ������ ��´�. 
//	���� ����	:	2002-07-20 ���� 5:22:01 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	SOPlayer::GetTotalProperty(PROPERTY_INFO &PropertyInfo)
{
	DBIKPopertyRanking				PropertyRanking;
	PropertyInfo.siMoneyProperty	=	siMoney;

	// 1�ܰ�: �����ܰ� �� ������� ���� ���´�.
	if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_1, &stAccountInfo, LPVOID(&PropertyInfo)) == FALSE )
		return ON_RET_AUDIT_NO;
	
	CalculateItemValue(PropertyInfo.siItemProperty);

	PropertyInfo.siTotalProperty	=	PropertyInfo.siItemProperty + PropertyInfo.siLoanProperty + PropertyInfo.siMoneyProperty + PropertyInfo.siSaveProperty;
	PropertyRanking.pPlayer			=	this;
	PropertyRanking.siProperty		=	PropertyInfo.siTotalProperty;
	
	// 2�ܰ�: ������� ���Ͽ�, ��� �����ϰ�, ����� ���´�.
	//if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), LPVOID(&PropertyInfo)) == FALSE )
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), sizeof(DBIKPopertyRanking));
		

	return ON_RET_AUDIT_OK;
}

//����������������������������������������������������������������������������
//	��ο��� ���¸���� �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditAccountList()
{
	DBIKBankBookList			BankBookList;
	
	memset(&BankBookList,0,sizeof(DBIKBankBookList));
	if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_BANKACCOUNT, this, LPVOID(&BankBookList)) == TRUE)
	{
		if(BankBookList.siBankBookNum>0 && BankBookList.siBankBookNum<=ON_MAX_BANKBOOK_NUM)
		{
			SendFieldMsg(ON_RESPONSE_AUDIT_ACCOUNT,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK),LPVOID(&BankBookList));
			return;
		}
	}
	
	SendFieldMsg(ON_RESPONSE_AUDIT_ACCOUNT,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));	
}

//����������������������������������������������������������������������������
//	��ο��� ���¸���� ��´�.
//����������������������������������������������������������������������������
BOOL	SOPlayer::GetAuditAccountList()
{
	return ON_RET_AUDIT_NO;
}

//����������������������������������������������������������������������������
//	��ο��� �ε��� ������Ȳ�� �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditOwnList()
{
	DBIKAuditOwnList			AuditOwnList;

	memset(&AuditOwnList,0,sizeof(DBIKAuditOwnList));

	if(pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_OWN, this, LPVOID(&AuditOwnList)) == TRUE)
	{
		if(AuditOwnList.OwnNum>0 && AuditOwnList.OwnNum<=ON_MAX_AUDIT_OWN)
		{
			SendFieldMsg(ON_RESPONSE_AUDIT_OWN,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK),LPVOID(&AuditOwnList));
			return;
		}
	}

	SendFieldMsg(ON_RESPONSE_AUDIT_OWN,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));
}

//����������������������������������������������������������������������������
//	��ο��� �ε��� ������Ȳ�� ��´�.
//����������������������������������������������������������������������������
BOOL	SOPlayer::GetAuditOwnList()
{
	// �ε��� ����
	return TRUE;
}

//����������������������������������������������������������������������������
//	��ο��� ���� ������ �������� ����Ʈ�� �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditVillageList()
{
	SI32					i;
	SI32					siVillageCount	=	0;
	OnAuditVillageInfo		stVillageInfo[ON_MAX_AUDIT_VILLAGE_NUM];
	stVillageInvestorData	*pstInvestmentData;

	// �ڽ��� ������ �����̸� stVillageList�� ���� ������ �߰��Ѵ�.
	for( i = 0; i < pMyWorld->siTotalVillageNum; i++ )
	{		
		if( (pstInvestmentData = pMyWorld->pclVillage[i].m_Government->GetInvestmentDataByID(szID)) != NULL )	// �� �������� ���� ���� ������ ���´�.
		{
			stVillageInfo[siVillageCount].uiVillageCode			=	pMyWorld->pclVillage[i].uiVillageCode;		// ���� �ڵ�
			stVillageInfo[siVillageCount].siLastInvestedTime	=	pstInvestmentData->m_UpdateTime;			// ���� �ð�
			stVillageInfo[siVillageCount].fPercent				=	pstInvestmentData->m_Percent;				// ������
			stVillageInfo[siVillageCount].mnInvestedMoney		=	pstInvestmentData->m_Money;					// ���� �ݾ�
			siVillageCount++;
		}
	}
	if( siVillageCount > 0 )
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_VILLAGE_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK), LPVOID(siVillageCount), LPVOID(stVillageInfo) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_VILLAGE_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO) );
	}
}


//����������������������������������������������������������������������������
//	��ο��� ���� ������ �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditVillageInfo()
{
	// �� �Ⱦ�����? -_-;
	/*
	OnReqAuditVillageInfoMsg		*pAuditVillageInfoMsg;
	DBIKAuditVillageInfo			stAuditVillageInfo;

	pAuditVillageInfoMsg				=	(OnReqAuditVillageInfoMsg*)	cMsg;

	stAuditVillageInfo.pOwner			=	this;
	stAuditVillageInfo.uiVillageCode	=	pAuditVillageInfoMsg->uiVillageCode;
	
	if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_VILLAGEINFO, LPVOID(&stAuditVillageInfo), LPVOID(&stAuditVillageInfo)) == TRUE )
	{
		SendFieldMsg(ON_RESPONSE_AUDIT_VILLAGE_INFO,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK),LPVOID(&stAuditVillageInfo));
	}
	else
	{
		SendFieldMsg(ON_RESPONSE_AUDIT_VILLAGE_INFO,SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));
	}
	*/

}

//����������������������������������������������������������������������������
//	��ο��� ���� ������ ��´�.
//����������������������������������������������������������������������������
BOOL	SOPlayer::GetAuditVillageInfo()
{
	// ���� ���� �κ�...���� ����..
	return TRUE;
}


//����������������������������������������������������������������������������������������������
// ��ο��� ģ�� ����Ʈ�� �޶�� ��û�� �޾Ҵ�.
// ������ ģ�� �����͸� �ǽð����� ������Ʈ �ϹǷ� DB�� ��û���� �ʰ� ���� ������ �����ش�.
//����������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditFriendList()
{
	SI32					i, numFriends;
	OnAuditFriendInfo		*pstTemp, stFriendInfo[MAX_FRIEND_NUM];

	numFriends	=	m_clFriendList.GetNumFriends();
	// ��ϵ� ģ������ŭ OnAuditFriendInfo ����ü�� ������ ä���.
	for( i = 0; i < numFriends; i++ )
	{
		if( (pstTemp = m_clFriendList.GetFriend(i)) != NULL )
		{
			CopyMemory( &stFriendInfo[i], pstTemp, sizeof(OnAuditFriendInfo) );
		}
	}

	if( numFriends > 0 )
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK), LPVOID(numFriends), LPVOID(stFriendInfo) );
	}
	else
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_LIST, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO) );
	}
}

//����������������������������������������������������������������������������
// ��ο��� ģ���� ���Űź� ó�� �ش޶�� ��û�� �޾Ҵ�.
// ������ ģ�� �����͸� ������Ʈ�ϰ� DB�� �˷��ش�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditFriendSetDenial()
{
	OnReqAuditFriendSetDenial	*pstReqMsg;
	strFriendInfo				*pstFriendInfo;

	pstReqMsg		=	(OnReqAuditFriendSetDenial*)cMsg;
	pstFriendInfo	=	m_clFriendList.GetFriend( pstReqMsg->szCharID );

	if( pstFriendInfo != NULL )			// ģ�� ����Ʈ�� �����ϸ�
	{
		if( m_clFriendList.SetDenial(pstReqMsg->szCharID, pstReqMsg->bDenial) == TRUE )
		{
			// DB�� �����Ѵ�.
			DBIKAuditFriendSetDenial	DBParam;

			DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
			DBParam.uiSlot				=	stAccountInfo.uiSlot;
			strcpy( DBParam.szFriendCharID, pstFriendInfo->szID );
			DBParam.bDenial				=	pstReqMsg->bDenial;
					
			pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_SET_DENIAL, &DBParam, sizeof(DBParam) );

			SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK) );
			return;
		}
	}
	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_SET_DENIAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO) );
}


//��������������������������������������������������������������������������������������������������������
// ��ο��� ģ�� �Ѹ��� �߰��ش޶�� ��û�� �޾Ҵ�.
// ģ�� �߰� ��û�� ���� ��� �������� ģ���� ���ؼ��� ó�����ش�. (������ ĳ���� ���� ���θ� �� �� ����)
// ������ ģ�� �����͸� ������Ʈ�ϰ� DB�� �˷��ش�.
//��������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditFriendAddOne()
{
	OnReqAuditFriendAddOne		*pstReqMsg;
	BOOL						bLogin;
	FRESULT						siResult;
	CHAR						szID[ON_ID_LENGTH+1];

	pstReqMsg		=	(OnReqAuditFriendAddOne*)cMsg;
	ZeroMemory( szID, sizeof(szID) );
	strncpy( szID, pstReqMsg->szCharID, ON_ID_LENGTH );

	// ���� ��ȿ�� ĳ���� �̸����� �˻��Ѵ�.
	if( CheckForbiddenName(szID) == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(FRIEND_INVALID_NAME), FALSE );
		return;
	}

	bLogin	=	( pMyWorld->GetPlayer(szID) != NULL ) ? TRUE : FALSE;		// ���� ���� ���� Ȯ��

	if( (siResult = m_clFriendList.AddFriend(this->szID, szID, bLogin)) == FRIEND_OK )
	{
		// DB�� �����Ѵ�.
		DBIKAuditFriendAddOne		DBParam;

		DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
		DBParam.uiSlot				=	stAccountInfo.uiSlot;
		ZeroMemory( DBParam.szFriendCharID, sizeof(DBParam.szFriendCharID) );
		strcpy( DBParam.szFriendCharID, szID );
				
		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE, &DBParam, sizeof(DBParam) );
	}

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(siResult), LPVOID(bLogin) );
}


//����������������������������������������������������������������������������
// ��ο��� ģ�� �Ѹ��� �����ش޶�� ��û�� �޾Ҵ�.
// ������ ģ�� �����͸� ������Ʈ�ϰ� DB�� �˷��ش�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditFriendRemoveSome()
{
	OnReqAuditFriendRemoveSome		*pstReqMsg;
	CHAR							szID[ON_ID_LENGTH+1];
	BYTE							*pszCurID;

	pstReqMsg		=	(OnReqAuditFriendRemoveSome*)cMsg;

	if( pstReqMsg->uiCount >= 1 && pstReqMsg->uiCount <= MAX_FRIEND_NUM )
	{
		pszCurID	=	(BYTE*)( (BYTE*)pstReqMsg + sizeof(OnReqAuditFriendRemoveSome) );		// ù��° ID�� ������

		// pszCurID �����͸� ON_ID_LENGTH��ŭ�� �Űܰ��鼭 ���̵� ����Ʈ�� ó���Ѵ�.
		for( SI32 i = 0; i < pstReqMsg->uiCount; i++ )
		{
			ZeroMemory( szID, sizeof(szID) );
			memcpy( szID, pszCurID, ON_ID_LENGTH );

			if( m_clFriendList.DeleteFriend(szID) == TRUE )
			{
				// DB�� �����Ѵ�.
				DBIKAuditFriendRemoveSome		DBParam;

				DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
				DBParam.uiSlot				=	stAccountInfo.uiSlot;
				strcpy( DBParam.szFriendCharID, szID );
						
				pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_SOME, &DBParam, sizeof(DBParam) );
			}
			pszCurID	+=	ON_ID_LENGTH;
		}
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK) );
		return;
	}

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_REMOVE_SOME, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO) );
}


//����������������������������������������������������������������������������
// ��ο��� ģ�� ��ü�� �����ش޶�� ��û�� �޾Ҵ�.
// ������ ģ�� �����͸� ������Ʈ�ϰ� DB�� �˷��ش�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditFriendRemoveAll()
{
	m_clFriendList.DeleteAllFriend();

	// DB�� �����Ѵ�.
	DBIKAuditFriendRemoveAll	DBParam;

	DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
	DBParam.uiSlot				=	stAccountInfo.uiSlot;
			
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL, &DBParam, sizeof(DBParam) );

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK) );
}


//����������������������������������������������������������������������������
//	��ο��� ���� ������ �޶�� ��û�� �޾Ҵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditStatus()
{
	SI32			i;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i�� uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			IFollower.GetFollowerCharID(i);
		//	IFollower.;
		}
	}
}

//����������������������������������������������������������������������������
//	��ο��� �ڽ��� ���������� ��� �´�.
//����������������������������������������������������������������������������
BOOL	SOPlayer::GetAuditStatus()
{
	// ���� ����Ʈ(�̸�, ����, ��ġ, ����, ��, ��ø, �ǰ�, ����, ����, ����
	// �����, ��������, �ּҰ���, �ִ����, ������, ȸ����, ����) ��~~
	return TRUE;
}

VOID	SOPlayer::RecvAuditFollowerStatus()
{
}


//����������������������������������������������������������������������������
//	��ο��� �ڽ��� �뺴���� �̸��� ���Ѵ�.
//����������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditSetName()
{
	DBIKSetName					SetName;
	OnReqSetNameMsg				*pSetNameMsg;
	DWORD						dwResult= ON_RET_AUDIT_NO;
	
	memset( &SetName, 0, sizeof(DBIKSetName));
	pSetNameMsg				=	(OnReqSetNameMsg*)	cMsg;
	if( this->IFollower.SetFollowerName(pSetNameMsg->uiFollowerNum, pSetNameMsg->cName) == TRUE )
	{
		SetName.DBAccount			=	stAccountInfo.siDBAccount;
		SetName.uiSlot				=	stAccountInfo.uiSlot;
		SetName.uiFollowerID		=	pSetNameMsg->uiFollowerNum;
		memcpy(SetName.cName, pSetNameMsg->cName, ON_ID_LENGTH);
		if( CheckForbiddenName(SetName.cName) == TRUE )
		{
			/*
			if( pMyWorld->clDatabase->SetDBInfo(SO_DB_INFOKIND_AUDIT_SETNAME, LPVOID(&SetName), LPVOID(this)) == TRUE )
			{
				dwResult	=	ON_RET_AUDIT_OK;
			}
			*/

			//**
			dwResult	=	ON_RET_AUDIT_OK;
			pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_AUDIT_SETNAME, &SetName, sizeof(SetName));
		}
	}
	SendFieldMsg(ON_RESPONSE_AUDIT_SETNAME,SO_SFM_ONLYME, LPVOID(dwResult));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	��ο��� ��û�� ������ ��Ⱓ�� ���� ��� ����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	SOPlayer::RecvAuditProfitStatistics()
{
	SI32							i;
	OnReqAuditProfitStatistics		*pReqMsg;
	stCapital						*pstCapital;

	pReqMsg		=	(OnReqAuditProfitStatistics*)cMsg;

	for( i = 0; i < pMyWorld->siTotalVillageNum; i++ )
	{
		if( pMyWorld->pclVillage[i].uiVillageCode == pReqMsg->uiVillageCode )
		{
			pstCapital	=	pMyWorld->pclVillage[i].m_Government->GetVillageCapital();
			SendFieldMsg( ON_RESPONSE_AUDIT_PROFIT_STATISTICS, SO_SFM_ONLYME, LPVOID(pstCapital) );
			//pMyWorld->pclVillage[i].m_Government->SendVillageCapital(this);
		}
	}
}
