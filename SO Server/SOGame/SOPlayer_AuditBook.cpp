#include "SOMain.h"
#include "SOWorld.h"
#include "SOBaseChar.h"
#include "OnlineProtocol.h"
#include "OnlineCommonStruct.h"
#include "OnlineGameMsg.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	嬴檜蠱曖 濠骯擊 啗骯 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
MONEY	SOPlayer::CalculateItemValue(MONEY &siItemMoney)
{
	SI32			i, j;
	MyItemData		stItem;
	MONEY			siItemValue = 0;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i朝 uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			for( j=0; j< ON_MYDATA_MAX_ITEM_NUM; j++)		//	j朝 嬴檜蠱 寡翮曖 Index
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 識營骯 薑爾蒂 殖塭堅 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditTotalInfo()
{
	if( GetTotalProperty( stPropertyData ) == ON_RET_AUDIT_OK )	SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK), LPVOID(&stPropertyData));	
	else														SendFieldMsg(ON_RESPONSE_AUDIT_TOTAL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_NO));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰睡縑憮 識營骯 薑爾蒂 橢朝棻. 
//	熱薑 橾濠	:	2002-07-20 螃�� 5:22:01 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GetTotalProperty(PROPERTY_INFO &PropertyInfo)
{
	DBIKPopertyRanking				PropertyRanking;
	PropertyInfo.siMoneyProperty	=	siMoney;

	// 1欽啗: 瞪濰濤堅 塽 瞪濰渠轎 高擊 橢橫螞棻.
	if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_1, &stAccountInfo, LPVOID(&PropertyInfo)) == FALSE )
		return ON_RET_AUDIT_NO;
	
	CalculateItemValue(PropertyInfo.siItemProperty);

	PropertyInfo.siTotalProperty	=	PropertyInfo.siItemProperty + PropertyInfo.siLoanProperty + PropertyInfo.siMoneyProperty + PropertyInfo.siSaveProperty;
	PropertyRanking.pPlayer			=	this;
	PropertyRanking.siProperty		=	PropertyInfo.siTotalProperty;
	
	// 2欽啗: 識營骯擊 掘ж罹, 蛤綠縑 盪濰ж堅, 蛔熱蒂 橢橫螞棻.
	//if( pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), LPVOID(&PropertyInfo)) == FALSE )
	pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_AUDIT_TOTAL_2, LPVOID(&PropertyRanking), sizeof(DBIKPopertyRanking));
		

	return ON_RET_AUDIT_OK;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 啗謝跡煙擊 殖塭堅 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 啗謝跡煙擊 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GetAuditAccountList()
{
	return ON_RET_AUDIT_NO;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 睡翕骯 爾嶸⑷�窕� 殖塭堅 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 睡翕骯 爾嶸⑷�窕� 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GetAuditOwnList()
{
	// 睡翕骯 薑爾
	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 頂陛 癱濠и 葆擊菟曖 葬蝶お蒂 殖塭朝 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditVillageList()
{
	SI32					i;
	SI32					siVillageCount	=	0;
	OnAuditVillageInfo		stVillageInfo[ON_MAX_AUDIT_VILLAGE_NUM];
	stVillageInvestorData	*pstInvestmentData;

	// 濠褐檜 癱濠и 葆擊檜賊 stVillageList縑 癱濠 薑爾蒂 蹺陛и棻.
	for( i = 0; i < pMyWorld->siTotalVillageNum; i++ )
	{		
		if( (pstInvestmentData = pMyWorld->pclVillage[i].m_Government->GetInvestmentDataByID(szID)) != NULL )	// 檜 葆擊縑憮 釭曖 癱濠 薑爾蒂 橢橫螞棻.
		{
			stVillageInfo[siVillageCount].uiVillageCode			=	pMyWorld->pclVillage[i].uiVillageCode;		// 葆擊 囀萄
			stVillageInfo[siVillageCount].siLastInvestedTime	=	pstInvestmentData->m_UpdateTime;			// 癱濠 衛除
			stVillageInfo[siVillageCount].fPercent				=	pstInvestmentData->m_Percent;				// 薄嶸徽
			stVillageInfo[siVillageCount].mnInvestedMoney		=	pstInvestmentData->m_Money;					// 癱濠 旎擋
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 葆擊 薑爾蒂 殖塭堅 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditVillageInfo()
{
	// 謁 寰噙檜雖? -_-;
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 葆擊 薑爾蒂 橢朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GetAuditVillageInfo()
{
	// 陛濰 ��萇 睡碟...瞰塭 緇赭..
	return TRUE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濰睡縑憮 耀掘 葬蝶お蒂 殖塭堅 蹂羶擊 嫡懊棻.
// 憮幗陛 耀掘 等檜攪蒂 褒衛除戲煎 機等檜お ж嘎煎 DB縑 蹂羶ж雖 彊堅 ⑷營 薑爾蒂 爾頂遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditFriendList()
{
	SI32					i, numFriends;
	OnAuditFriendInfo		*pstTemp, stFriendInfo[MAX_FRIEND_NUM];

	numFriends	=	m_clFriendList.GetNumFriends();
	// 蛔煙脹 耀掘熱虜躑 OnAuditFriendInfo 掘褻羹縑 薑爾蒂 瓣遴棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濰睡縑憮 耀掘蒂 熱褐剪睡 籀葬 п殖塭朝 蹂羶擊 嫡懊棻.
// 憮幗曖 耀掘 等檜攪蒂 機等檜おж堅 DB縑 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditFriendSetDenial()
{
	OnReqAuditFriendSetDenial	*pstReqMsg;
	strFriendInfo				*pstFriendInfo;

	pstReqMsg		=	(OnReqAuditFriendSetDenial*)cMsg;
	pstFriendInfo	=	m_clFriendList.GetFriend( pstReqMsg->szCharID );

	if( pstFriendInfo != NULL )			// 耀掘 葬蝶お縑 襄營ж賊
	{
		if( m_clFriendList.SetDenial(pstReqMsg->szCharID, pstReqMsg->bDenial) == TRUE )
		{
			// DB縑 撲薑и棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濰睡縑憮 耀掘 и貲擊 蹺陛п殖塭朝 蹂羶擊 嫡懊棻.
// 耀掘 蹺陛 蹂羶檜 諮擊 唳辦 蕾樓醞檣 耀掘縑 渠п憮虜 籀葬п遽棻. (憮幗朝 議葛攪 襄營 罹睡蒂 憲 熱 橈擠)
// 憮幗曖 耀掘 等檜攪蒂 機等檜おж堅 DB縑 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditFriendAddOne()
{
	OnReqAuditFriendAddOne		*pstReqMsg;
	BOOL						bLogin;
	FRESULT						siResult;
	CHAR						szID[ON_ID_LENGTH+1];

	pstReqMsg		=	(OnReqAuditFriendAddOne*)cMsg;
	ZeroMemory( szID, sizeof(szID) );
	strncpy( szID, pstReqMsg->szCharID, ON_ID_LENGTH );

	// 試盪 嶸�褲� 議葛攪 檜葷檣雖 匐餌и棻.
	if( CheckForbiddenName(szID) == FALSE )
	{
		SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(FRIEND_INVALID_NAME), FALSE );
		return;
	}

	bLogin	=	( pMyWorld->GetPlayer(szID) != NULL ) ? TRUE : FALSE;		// ⑷營 蕾樓 罹睡 �挫�

	if( (siResult = m_clFriendList.AddFriend(this->szID, szID, bLogin)) == FRIEND_OK )
	{
		// DB縑 撲薑и棻.
		DBIKAuditFriendAddOne		DBParam;

		DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
		DBParam.uiSlot				=	stAccountInfo.uiSlot;
		ZeroMemory( DBParam.szFriendCharID, sizeof(DBParam.szFriendCharID) );
		strcpy( DBParam.szFriendCharID, szID );
				
		pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_ADD_ONE, &DBParam, sizeof(DBParam) );
	}

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_ADD_ONE, SO_SFM_ONLYME, LPVOID(siResult), LPVOID(bLogin) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濰睡縑憮 耀掘 и貲擊 餉薯п殖塭朝 蹂羶擊 嫡懊棻.
// 憮幗曖 耀掘 等檜攪蒂 機等檜おж堅 DB縑 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditFriendRemoveSome()
{
	OnReqAuditFriendRemoveSome		*pstReqMsg;
	CHAR							szID[ON_ID_LENGTH+1];
	BYTE							*pszCurID;

	pstReqMsg		=	(OnReqAuditFriendRemoveSome*)cMsg;

	if( pstReqMsg->uiCount >= 1 && pstReqMsg->uiCount <= MAX_FRIEND_NUM )
	{
		pszCurID	=	(BYTE*)( (BYTE*)pstReqMsg + sizeof(OnReqAuditFriendRemoveSome) );		// 羅廓簞 ID曖 ん檣攪

		// pszCurID ん檣攪蒂 ON_ID_LENGTH虜躑噶 衡啖陛賊憮 嬴檜蛤 葬蝶お蒂 籀葬и棻.
		for( SI32 i = 0; i < pstReqMsg->uiCount; i++ )
		{
			ZeroMemory( szID, sizeof(szID) );
			memcpy( szID, pszCurID, ON_ID_LENGTH );

			if( m_clFriendList.DeleteFriend(szID) == TRUE )
			{
				// DB縑 撲薑и棻.
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 濰睡縑憮 耀掘 瞪羹蒂 餉薯п殖塭朝 蹂羶擊 嫡懊棻.
// 憮幗曖 耀掘 等檜攪蒂 機等檜おж堅 DB縑 憲溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditFriendRemoveAll()
{
	m_clFriendList.DeleteAllFriend();

	// DB縑 撲薑и棻.
	DBIKAuditFriendRemoveAll	DBParam;

	DBParam.siDBAccount			=	stAccountInfo.siDBAccount;
	DBParam.uiSlot				=	stAccountInfo.uiSlot;
			
	pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_AUDIT_FRIEND_REMOVE_ALL, &DBParam, sizeof(DBParam) );

	SendFieldMsg( ON_RESPONSE_AUDIT_FRIEND_REMOVE_ALL, SO_SFM_ONLYME, LPVOID(ON_RET_AUDIT_OK) );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 鼠濰 薑爾蒂 殖塭堅 蹂羶擊 嫡懊棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOPlayer::RecvAuditStatus()
{
	SI32			i;

	for( i=0; i< ON_MAX_FOLLOWER_NUM; i++)					//	i朝 uiFollowerSlot
	{
		if( IFollower.IsValidFollower(i) == TRUE )
		{
			IFollower.GetFollowerCharID(i);
		//	IFollower.;
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 濠褐曖 鼠濰薑爾蒂 橢橫 螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOPlayer::GetAuditStatus()
{
	// ぜ錳 葬蝶お(檜葷, 溯漣, 售纂, 措陛, ��, 團繪, 勒鬼, 雖撩, 儅貲, 葆徹
	// 寞橫徽, 葆徹盪о, 譆模奢問, 譆渠奢問, 奢問徽, �裔Ш�, 鼠啪) 臏~~
	return TRUE;
}

VOID	SOPlayer::RecvAuditFollowerStatus()
{
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 濠褐曖 辨煽縑啪 檜葷擊 薑и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	濰睡縑憮 蹂羶и 葆擊曖 賃喇除曖 熱櫛 鱔啗 爾晦.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
