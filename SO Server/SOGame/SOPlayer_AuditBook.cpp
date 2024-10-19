#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¾ÆÀÌÅÛÀÇ ÀÚ»êÀ» °è»ê ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
MONEY	SOPlayer::CalculateItemValue(MONEY &siItemMoney)
{
	SI32			i, j;
	MyItemData		stItem;
	MONEY			siItemValue = 0;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i´Â uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			for( j=0; j< ON_MYDATA_MAX_ITEM_NUM; j++)		//	j´Â ¾ÆÀÌÅÛ ¹è¿­ÀÇ Index
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ÃÑÀç»ê Á¤º¸¸¦ ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditTotalInfo()
{
	if( GetTotalProperty( stPropertyData ) == ON_RET_AUDIT_OK )	SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK), LPVOID(&stPropertyData));	
	else														SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));	
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀåºÎ¿¡¼­ ÃÑÀç»ê Á¤º¸¸¦ ¾ò´Â´Ù. 
//	¼öÁ¤ ÀÏÀÚ	:	2002-07-20 ¿ÀÈÄ 5:22:01 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOPlayer::GetTotalProperty(PROPERTY_INFO &PropertyInfo)
{
	DBIKPopertyRanking				PropertyRanking;
	PropertyInfo.siMoneyProperty	=	siMoney;

	// 1´Ü°è: ÀüÀåÀÜ°í ¹× ÀüÀå´ëÃâ °ªÀ» ¾ò¾î¿Â´Ù.
	if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_1, &stAccountInfo, LPVOID(&PropertyInfo)) == FALSE )
		return ON_RET_AUDIT_NO;
	
	CalculateItemValue(PropertyInfo.siItemProperty);

	PropertyInfo.siTotalProperty	=	PropertyInfo.siItemProperty + PropertyInfo.siLoanProperty + PropertyInfo.siMoneyProperty + PropertyInfo.siSaveProperty;
	PropertyRanking.pPlayer			=	this;
	PropertyRanking.siProperty		=	PropertyInfo.siTotalProperty;
	
	// 2´Ü°è: ÃÑÀç»êÀ» ±¸ÇÏ¿©, µðºñ¿¡ ÀúÀåÇÏ°í, µî¼ö¸¦ ¾ò¾î¿Â´Ù.
	//if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), LPVOID(&PropertyInfo)) == FALSE )
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), sizeof(DBIKPopertyRanking));
		

	return ON_RET_AUDIT_OK;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ °èÁÂ¸ñ·ÏÀ» ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ °èÁÂ¸ñ·ÏÀ» ¾ò´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOPlayer::GetAuditAccountList()
{
	return ON_RET_AUDIT_NO;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ºÎµ¿»ê º¸À¯ÇöÈ²À» ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ºÎµ¿»ê º¸À¯ÇöÈ²À» ¾ò´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOPlayer::GetAuditOwnList()
{
	// ºÎµ¿»ê Á¤º¸
	return TRUE;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ³»°¡ ÅõÀÚÇÑ ¸¶À»µéÀÇ ¸®½ºÆ®¸¦ ´Þ¶ó´Â ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditVillageList()
{
	SI32					i;
	SI32					siVillageCount	=	0;
	OnAuditVillageInfo		stVillageInfo[ON_MAX_AUDIT_VILLAGE_NUM];
	stVillageInvestorData	*pstInvestmentData;

	// ÀÚ½ÅÀÌ ÅõÀÚÇÑ ¸¶À»ÀÌ¸é stVillageList¿¡ ÅõÀÚ Á¤º¸¸¦ Ãß°¡ÇÑ´Ù.
	for( i = 0; i < pMyWorld->siTotalVillageNum; i++ )
	{		
		if( (pstInvestmentData = pMyWorld->pclVillage[i].m_Government->GetInvestmentDataByID(szID)) != NULL )	// ÀÌ ¸¶À»¿¡¼­ ³ªÀÇ ÅõÀÚ Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
		{
			stVillageInfo[siVillageCount].uiVillageCode			=	pMyWorld->pclVillage[i].uiVillageCode;		// ¸¶À» ÄÚµå
			stVillageInfo[siVillageCount].siLastInvestedTime	=	pstInvestmentData->m_UpdateTime;			// ÅõÀÚ ½Ã°£
			stVillageInfo[siVillageCount].fPercent				=	pstInvestmentData->m_Percent;				// Á¡À¯À²
			stVillageInfo[siVillageCount].mnInvestedMoney		=	pstInvestmentData->m_Money;					// ÅõÀÚ ±Ý¾×
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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ¸¶À» Á¤º¸¸¦ ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditVillageInfo()
{
	// ¿Ö ¾È¾²ÀÌÁö? -_-;
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ¸¶À» Á¤º¸¸¦ ¾ò´Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOPlayer::GetAuditVillageInfo()
{
	// °¡Àå Èûµç ºÎºÐ...Àý¶ó ºý½ê..
	return TRUE;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀåºÎ¿¡¼­ Ä£±¸ ¸®½ºÆ®¸¦ ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
// ¼­¹ö°¡ Ä£±¸ µ¥ÀÌÅÍ¸¦ ½Ç½Ã°£À¸·Î ¾÷µ¥ÀÌÆ® ÇÏ¹Ç·Î DB¿¡ ¿äÃ»ÇÏÁö ¾Ê°í ÇöÀç Á¤º¸¸¦ º¸³»ÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditFriendList()
{
	SI32					i, numFriends;
	OnAuditFriendInfo		*pstTemp, stFriendInfo[MAX_FRIEND_NUM];

	numFriends	=	m_clFriendList.GetNumFriends();
	// µî·ÏµÈ Ä£±¸¼ö¸¸Å­ OnAuditFriendInfo ±¸Á¶Ã¼¿¡ Á¤º¸¸¦ Ã¤¿î´Ù.
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀåºÎ¿¡¼­ Ä£±¸¸¦ ¼ö½Å°ÅºÎ Ã³¸® ÇØ´Þ¶ó´Â ¿äÃ»À» ¹Þ¾Ò´Ù.
// ¼­¹öÀÇ Ä£±¸ µ¥ÀÌÅÍ¸¦ ¾÷µ¥ÀÌÆ®ÇÏ°í DB¿¡ ¾Ë·ÁÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditFriendSetDenial()
{
	OnReqAuditFriendSetDenial	*pstReqMsg;
	strFriendInfo				*pstFriendInfo;

	pstReqMsg		=	(OnReqAuditFriendSetDenial*)cMsg;
	pstFriendInfo	=	m_clFriendList.GetFriend( pstReqMsg->szCharID );

	if( pstFriendInfo != NULL )			// Ä£±¸ ¸®½ºÆ®¿¡ Á¸ÀçÇÏ¸é
	{
		if( m_clFriendList.SetDenial(pstReqMsg->szCharID, pstReqMsg->bDenial) == TRUE )
		{
			// DB¿¡ ¼³Á¤ÇÑ´Ù.
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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀåºÎ¿¡¼­ Ä£±¸ ÇÑ¸íÀ» Ãß°¡ÇØ´Þ¶ó´Â ¿äÃ»À» ¹Þ¾Ò´Ù.
// Ä£±¸ Ãß°¡ ¿äÃ»ÀÌ ¿ÔÀ» °æ¿ì Á¢¼ÓÁßÀÎ Ä£±¸¿¡ ´ëÇØ¼­¸¸ Ã³¸®ÇØÁØ´Ù. (¼­¹ö´Â Ä³¸¯ÅÍ Á¸Àç ¿©ºÎ¸¦ ¾Ë ¼ö ¾øÀ½)
// ¼­¹öÀÇ Ä£±¸ µ¥ÀÌÅÍ¸¦ ¾÷µ¥ÀÌÆ®ÇÏ°í DB¿¡ ¾Ë·ÁÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditFriendAddOne()
{
	OnReqAuditFriendAddOne		*pstReqMsg;
	BOOL						bLogin;
	FRESULT						siResult;
	CHAR						szID[ON_ID_LENGTH+1];

	pstReqMsg		=	(OnReqAuditFriendAddOne*)cMsg;
	ZeroMemory( szID, sizeof(szID) );
	strncpy( szID, pstReqMsg->szCharID, ON_ID_LENGTH );

	// ¸ÕÀú À¯È¿ÇÑ Ä³¸¯ÅÍ ÀÌ¸§ÀÎÁö °Ë»çÇÑ´Ù.
	if( CheckForbiddenName(szID) == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(FRIEND_INVALID_NAME), FALSE );
		return;
	}

	bLogin	=	( pMyWorld->GetPlayer(szID) != NULL ) ? TRUE : FALSE;		// ÇöÀç Á¢¼Ó ¿©ºÎ È®ÀÎ

	if( (siResult = m_clFriendList.AddFriend(this->szID, szID, bLogin)) == FRIEND_OK )
	{
		// DB¿¡ ¼³Á¤ÇÑ´Ù.
		DBIKAuditFriendAddOne		DBParam;

		DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
		DBParam.uiSlot				=	stAccountInfo.uiSlot;
		ZeroMemory( DBParam.szFriendCharID, sizeof(DBParam.szFriendCharID) );
		strcpy( DBParam.szFriendCharID, szID );
				
		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE, &DBParam, sizeof(DBParam) );
	}

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(siResult), LPVOID(bLogin) );
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀåºÎ¿¡¼­ Ä£±¸ ÇÑ¸íÀ» »èÁ¦ÇØ´Þ¶ó´Â ¿äÃ»À» ¹Þ¾Ò´Ù.
// ¼­¹öÀÇ Ä£±¸ µ¥ÀÌÅÍ¸¦ ¾÷µ¥ÀÌÆ®ÇÏ°í DB¿¡ ¾Ë·ÁÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditFriendRemoveSome()
{
	OnReqAuditFriendRemoveSome		*pstReqMsg;
	CHAR							szID[ON_ID_LENGTH+1];
	BYTE							*pszCurID;

	pstReqMsg		=	(OnReqAuditFriendRemoveSome*)cMsg;

	if( pstReqMsg->uiCount >= 1 && pstReqMsg->uiCount <= MAX_FRIEND_NUM )
	{
		pszCurID	=	(BYTE*)( (BYTE*)pstReqMsg + sizeof(OnReqAuditFriendRemoveSome) );		// Ã¹¹øÂ° IDÀÇ Æ÷ÀÎÅÍ

		// pszCurID Æ÷ÀÎÅÍ¸¦ ON_ID_LENGTH¸¸Å­¾¿ ¿Å°Ü°¡¸é¼­ ¾ÆÀÌµð ¸®½ºÆ®¸¦ Ã³¸®ÇÑ´Ù.
		for( SI32 i = 0; i < pstReqMsg->uiCount; i++ )
		{
			ZeroMemory( szID, sizeof(szID) );
			memcpy( szID, pszCurID, ON_ID_LENGTH );

			if( m_clFriendList.DeleteFriend(szID) == TRUE )
			{
				// DB¿¡ ¼³Á¤ÇÑ´Ù.
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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
// ÀåºÎ¿¡¼­ Ä£±¸ ÀüÃ¼¸¦ »èÁ¦ÇØ´Þ¶ó´Â ¿äÃ»À» ¹Þ¾Ò´Ù.
// ¼­¹öÀÇ Ä£±¸ µ¥ÀÌÅÍ¸¦ ¾÷µ¥ÀÌÆ®ÇÏ°í DB¿¡ ¾Ë·ÁÁØ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditFriendRemoveAll()
{
	m_clFriendList.DeleteAllFriend();

	// DB¿¡ ¼³Á¤ÇÑ´Ù.
	DBIKAuditFriendRemoveAll	DBParam;

	DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
	DBParam.uiSlot				=	stAccountInfo.uiSlot;
			
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL, &DBParam, sizeof(DBParam) );

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK) );
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ¹«Àå Á¤º¸¸¦ ´Þ¶ó°í ¿äÃ»À» ¹Þ¾Ò´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	SOPlayer::RecvAuditStatus()
{
	SI32			i;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i´Â uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			IFollower.GetFollowerCharID(i);
		//	IFollower.;
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ÀÚ½ÅÀÇ ¹«ÀåÁ¤º¸¸¦ ¾ò¾î ¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	SOPlayer::GetAuditStatus()
{
	// ÆÀ¿ø ¸®½ºÆ®(ÀÌ¸§, ·¹º§, °âÄ¡, ±¹°¡, Èû, ¹ÎÃ¸, °Ç°­, Áö¼º, »ý¸í, ¸¶¹ý
	// ¹æ¾îÀ², ¸¶¹ýÀúÇ×, ÃÖ¼Ò°ø°Ý, ÃÖ´ë°ø°Ý, °ø°ÝÀ², È¸ÇÇÀ², ¹«°Ô) ÂÁ~~
	return TRUE;
}

VOID	SOPlayer::RecvAuditFollowerStatus()
{
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ÀÚ½ÅÀÇ ¿ëº´¿¡°Ô ÀÌ¸§À» Á¤ÇÑ´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	ÀåºÎ¿¡¼­ ¿äÃ»ÇÑ ¸¶À»ÀÇ ¸î³â°£ÀÇ ¼öÀÍ Åë°è º¸±â.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
