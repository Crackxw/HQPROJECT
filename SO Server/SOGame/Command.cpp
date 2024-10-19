#include "SOMain.h"
#include "SOWorld.h"
#include "SOCommonHeader.h"
#include "Command.h"

strCommand	cltCommand::m_stCommand[SO_TOTAL_COMMAND_NUM]	=	
{
	{"W",					ON_USERGRADE_ADMIN1,	""},				// SO_COMMAND_WARP
	{"CM",					ON_USERGRADE_ADMIN3,	""},				// SO_COMMAND_CREATEMONSTER
	{"GIVEM",				ON_USERGRADE_ADMIN4,	""},				// SO_COMMAND_GIVEMONEY
	{"GIVEI",				ON_USERGRADE_ADMIN5,	""},				// SO_COMMAND_GIVEITEM
	{"LOGOUT",				ON_USERGRADE_ADMIN3,	""},				// SO_COMMAND_LOGOUT
	{"N",					ON_USERGRADE_ADMIN1,	""},				// SO_COMMAND_NOTICE
	{"G",					ON_USERGRADE_ADMIN1,	""},				// SO_COMMAND_GETINFO
	{"BONUSUP",				ON_USERGRADE_ADMIN5,	""},				// SO_COMMAND_BONUSUP
	{"GUILD",				ON_USERGRADE_NORMAL,	SO_GUILD_SC_SHN},	// SO_COMMAND_GNOTICE
	{"TO",					ON_USERGRADE_NORMAL,	""},				// SO_COMMAND_WHISPER	
	{"CREDITUP",			ON_USERGRADE_ADMIN5,	""},				// SO_COMMAND_CREDITUP
	{"EXPUP",				ON_USERGRADE_ADMIN5,	""},				// SO_COMMAND_EXPUP
	{"KILLNEARMONSTER",		ON_USERGRADE_ADMIN3,	""},				// SO_COMMAND_KILL_NEAR_MONSTER
	{"CLEARNEARITEM",		ON_USERGRADE_ADMIN3,	""},				// SO_COMMAND_CLEAR_NEAR_ITEM
	{"COMPLETEQUEST",		ON_USERGRADE_ADMIN5,	""},				// SO_COMMAND_COMPLETE_QUEST
	{"PARTYCHAT",			ON_USERGRADE_NORMAL,	""},				// SO_COMMAND_PARTYCHAT
};

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltCommand::cltCommand()
{	
	pMyWorld	=	NULL;		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
cltCommand::~cltCommand()
{
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltCommand::Init(SOWorld *pmyworld)
{
	pMyWorld	=	pmyworld;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� ���� ��Ų��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY)
{
	SOPlayer	*pPlayer;
	CHAR		szGMString[1024];

	ZeroMemory(szGMString, sizeof(szGMString));
	

	// ����� ���Ϳ� ����� ������ ��ų �� �ִ�.
	if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// �����̴�.
		sprintf(szGMString, "[%s] [%d] [%d] [%d]", pMyWorld->clSOCharKI.GetNameByString(pMyWorld->clMonster.GetKind(uiPlayerID)), 
													uiMapID, siX, siY);
		strcat(m_szGMString, szGMString);
		WriteStringToFile();		

		return pMyWorld->clMonster.Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(ISPLAYER(uiPlayerID) == TRUE)
	{
		// ����̴�.
		// ����� ��쿡�� ���ο� ������ ���� �Ͽ��� ��� ������ ȯ���� �ٽ� �޾ƾ� �ϱ� ������
		// ������ �ټ� �����ϴ�.
		pPlayer	=	pMyWorld->pPlayerOnAccount[uiPlayerID];

		if(pPlayer == NULL)
			return FALSE;
		else
		{
			sprintf(szGMString, "[%s] [%d] [%d] [%d]", pPlayer->szID, uiMapID, siX, siY);
			strcat(m_szGMString, szGMString);
			WriteStringToFile();		

			return pPlayer->Warp(uiMapID, siX, siY);
		}
	}
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���͸� �����Ѵ�.
//	���� ����	:	2002-12-10 - ������
//	>cm M01 100
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::CreateMonster(UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterLevel, UI16 uiMonsterGroupNum)
{
	UI16					uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER];
	SI32					uiMonsterLevels[ON_MAX_CHAR_PER_PLAYER];
	SI32					i;	
	UI16					uiMonsterNum;
	UI16					uiMapID;
	SOPlayer				*pPlayer;
	POINT					ptMonsterPos;	
	strOnlineBaseCharKI		*pOnlineBaseCharKI;
	CHAR					szGMString[1024];

	ZeroMemory(szGMString, sizeof(szGMString));
/*	
	if ( BYTE(pszMonsterName[0]) > 127 ) // �ѱ�
		pOnlineBaseCharKI		=	pMyWorld->clSOCharKI.GetBaseCharKI(pszMonsterName);
	else
		pOnlineBaseCharKI		=	pMyWorld->clSOCharKI.GetBaseCharKI( pMyWorld->clCharParser.GetID(pszMonsterName) );
*/
	pOnlineBaseCharKI		=	pMyWorld->clSOCharKI.GetBaseCharKI(pszMonsterName);	
	if(pOnlineBaseCharKI == NULL) 
		pOnlineBaseCharKI		=	pMyWorld->clSOCharKI.GetBaseCharKI( pMyWorld->clCharParser.GetID(pszMonsterName) );

	if(pOnlineBaseCharKI == NULL) return FALSE;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;


	sprintf(szGMString, "[%s] [%d] [%d]", pszMonsterName, uiMonsterLevel, uiMonsterGroupNum);
	strcat(m_szGMString, szGMString);
	WriteStringToFile();		

	uiMapID		=	pPlayer->uiMapID;
	
	while(uiMonsterGroupNum)	
	{
		ZeroMemory(uiMonsterKinds, sizeof(uiMonsterKinds));
		ZeroMemory(uiMonsterLevels, sizeof(uiMonsterLevels));

		uiMonsterNum	=	rand() % ON_MAX_CHAR_PER_PLAYER + 1;

		for(i = 0; i < uiMonsterNum; i++)
		{
			uiMonsterKinds[i]	=	pOnlineBaseCharKI->uiID;
			uiMonsterLevels[i]	=	uiMonsterLevel;
		}
		
		ptMonsterPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, pPlayer->siX, pPlayer->siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		if(ptMonsterPos.x == -1 && ptMonsterPos.y == -1) break;

		if(pMyWorld->clMonster.CreateMonster(uiMapID, ptMonsterPos.x, ptMonsterPos.y, pOnlineBaseCharKI->uiID, uiMonsterKinds, uiMonsterLevels) == TRUE)
			uiMonsterGroupNum--;
	}

	return TRUE;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ ���̵�� ���͸� �����Ѵ�
//	���� ����	:	2004-12-2 ���� 2:01 - �ֺ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::CreateMonsterID(UI16 uiGMPlayerID, UI16 uiMonId, UI16 uiMonsterLevel, UI16 uiMonsterGroupNum)
{
	UI16					uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER];
	SI32					uiMonsterLevels[ON_MAX_CHAR_PER_PLAYER];
	SI32					i;	
	UI16					uiMonsterNum;
	UI16					uiMapID;
	SOPlayer				*pPlayer;
	POINT					ptMonsterPos;	
	strOnlineBaseCharKI		*pOnlineBaseCharKI;
	CHAR					szGMString[1024];

	ZeroMemory(szGMString, sizeof(szGMString));
	pOnlineBaseCharKI		=	pMyWorld->clSOCharKI.GetBaseCharKI(uiMonId);	
	if(pOnlineBaseCharKI == NULL) return FALSE;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;


	sprintf(szGMString, "[%d] [%d] [%d]", pOnlineBaseCharKI->uiName, uiMonsterLevel, uiMonsterGroupNum);
	strcat(m_szGMString, szGMString);
	WriteStringToFile();		

	uiMapID		=	pPlayer->uiMapID;
	
	while(uiMonsterGroupNum)	
	{
		ZeroMemory(uiMonsterKinds, sizeof(uiMonsterKinds));
		ZeroMemory(uiMonsterLevels, sizeof(uiMonsterLevels));

		uiMonsterNum	=	rand() % ON_MAX_CHAR_PER_PLAYER + 1;

		for(i = 0; i < uiMonsterNum; i++)
		{
			uiMonsterKinds[i]	=	pOnlineBaseCharKI->uiID;
			uiMonsterLevels[i]	=	uiMonsterLevel;
		}
		
		ptMonsterPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, pPlayer->siX, pPlayer->siY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);

		if(ptMonsterPos.x == -1 && ptMonsterPos.y == -1) break;

		if(pMyWorld->clMonster.CreateMonster(uiMapID, ptMonsterPos.x, ptMonsterPos.y, pOnlineBaseCharKI->uiID, uiMonsterKinds, uiMonsterLevels) == TRUE)
			uiMonsterGroupNum--;
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ ���� �����Ѵ�.
//	���� ����	:	2002-10-24 ���� 2:27:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GiveMoney(UI16 uiPlayerID, MONEY Money)
{
	SOPlayer		*pPlayer;	
	CHAR			szGMString[1024];

	// ����ڰ� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];	
	if(pPlayer == NULL)	return FALSE;

	MONEY tempMoney = pPlayer->siMoney;

	pPlayer->GiveBonusMoney( Money, ON_RET_CHNGEMONEY_FROMADMIN ,FALSE);

	DBIKGiveMoneyByAdmin DBInfo;

	DBInfo.siDBAccount = pPlayer->stAccountInfo.siDBAccount;
	DBInfo.uiDBSlot = pPlayer->stAccountInfo.uiSlot;
	// ��ȭ���� ���.
	DBInfo.moMoney = pPlayer->siMoney - tempMoney;

	pPlayer->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GIVEMONEYBYADMIN,&DBInfo,sizeof(DBInfo));
	
	sprintf(szGMString, "[%s] [%I64d]", pPlayer->szID, Money );
	strcat(m_szGMString, szGMString);
	WriteStringToFile();		

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�������� ���� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GiveItem(UI16 uiGMPlayerID, CHAR *pszItemName, UI16 uiItemQuantity)
{	
	BOOL					bSuccess = TRUE;
	POINT					ptThrowXY;	
	SI32					siX, siY;
	SOPlayer				*pPlayer;
	UI16					uiMapID;
	UI32					uiMoveableTileInfo;
	OnReqThrowItemMsg		ReqThrowItemMsg;
	CItemHeader				*pItemHeader;
	CHAR					szDBArg[ 1024 ];
	CHAR					szGMString[ 1024 ];

	// ������ ������ 0�� ��� FALSE�� �����Ѵ�.
	if(uiItemQuantity == 0) return FALSE;

	// �����ϴ� �������� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if((pItemHeader = pMyWorld->clItem.GetItem(pszItemName)) == NULL) return FALSE;
	
	// GM�� �������� �������� ������.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// �������� ���� �� �ִ� �ڸ��� �ִ��� �Ǵ��Ѵ�.
	// ---- ������ - 20050304 ����
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// �������� ���� �� �ִ� ��ǥ�� ã�� ���ߴ�.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// �������� ����߸� ��ġ�� ã�Ҵ�.
	// �� �������� ����߸��� ��ġ�� �������� ������ ĳ���Ϳ��� �������� ����߷ȴٰ� �����ش�.
	// ���⼭ �ӽ÷� �޽����� ������ش�.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// �ʿ� �� �������� �����Ѵ�.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;


	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;
	//Msg.uiItemID = pItemHeader->m_uiID;
	//Msg.uiItemQuantity = uiItemQuantity;

	// ����: ���� �����۰� ������ �������� ���� �����Ƿ� ���� ������ �ٽ� ���´�.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;

	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);



	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	/*pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
							LPVOID( ReqThrowItemMsg.uiItemID ), LPVOID( ReqThrowItemMsg.uiQuantity ) );*/

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %u, %u", pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
													pPlayer->uiMapID, ptThrowXY.x, ptThrowXY.y, 
													pItemHeader->m_uiID, uiItemQuantity );
	pPlayer->pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_CREATEITEM_BYADMIN, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	sprintf( szGMString, "[%d] [%d] [%u, %s]", ptThrowXY.x, ptThrowXY.y, pItemHeader->m_uiID, pszItemName );
	strcat( m_szGMString, szGMString );
	WriteStringToFile();		

	return TRUE;
}

BOOL	cltCommand::GiveItemID(UI16 uiGMPlayerID, UI16 uiItemID, UI16 uiItemQuantity)
{
	BOOL					bSuccess = TRUE;
	POINT					ptThrowXY;	
	SI32					siX, siY;
	SOPlayer				*pPlayer;
	UI16					uiMapID;
	UI32					uiMoveableTileInfo;
	OnReqThrowItemMsg		ReqThrowItemMsg;
	CItemHeader				*pItemHeader;
	CHAR					szDBArg[ 1024 ];
	CHAR					szGMString[ 1024 ];

	// ������ ������ 0�� ��� FALSE�� �����Ѵ�.
	if(uiItemQuantity == 0) return FALSE;

	// �����ϴ� �������� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if((pItemHeader = pMyWorld->clItem.GetItem(uiItemID)) == NULL) return FALSE;
	
	// GM�� �������� �������� ������.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// �������� ���� �� �ִ� �ڸ��� �ִ��� �Ǵ��Ѵ�.
	// ---- ������ - 20050304 ����
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// �������� ���� �� �ִ� ��ǥ�� ã�� ���ߴ�.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// �������� ����߸� ��ġ�� ã�Ҵ�.
	// �� �������� ����߸��� ��ġ�� �������� ������ ĳ���Ϳ��� �������� ����߷ȴٰ� �����ش�.
	// ���⼭ �ӽ÷� �޽����� ������ش�.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// �ʿ� �� �������� �����Ѵ�.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;
	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;

	// ����: ���� �����۰� ������ �������� ���� �����Ƿ� ���� ������ �ٽ� ���´�.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;
	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	// DB�� �����Ѵ�.
	ZeroMemory( szDBArg, sizeof( szDBArg ) );
	sprintf( szDBArg, "%d, %d, %d, %d, %d, %u, %u", pPlayer->stAccountInfo.siDBAccount, pPlayer->stAccountInfo.uiSlot,
													pPlayer->uiMapID, ptThrowXY.x, ptThrowXY.y, 
													pItemHeader->m_uiID, uiItemQuantity );

	pPlayer->pMyWorld->clDatabase->writeDBQuery( SO_DB_INFOKIND_CREATEITEM_BYADMIN, LPVOID( szDBArg ), strlen( szDBArg ) + 1 );

	sprintf( szGMString, "[%d] [%d] [%u, %u]", ptThrowXY.x, ptThrowXY.y, pItemHeader->m_uiID, pItemHeader->m_siItemName);
	strcat( m_szGMString, szGMString );
	WriteStringToFile();		

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� �߹��Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::LogOut(UI16 uiPlayerID)
{
	SOPlayer				*pPlayer;

	// ������ ��쿡�� '�α׾ƿ�'�� �ȴ�.
	// ������ ��쿡�� �״´�.
	if(ISPLAYER(uiPlayerID) == TRUE)
	{
		pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
		if(pPlayer == NULL) return FALSE;

		pPlayer->LogOut();

		return TRUE;
	}
	else if(ISMONSTER(uiPlayerID) == TRUE)
	{
		pMyWorld->clMonster.DeadChar(uiPlayerID, FALSE);
		return TRUE;
	}
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��������.
//	���� ����	:	2002-03-26 ���� 3:31:45 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::Notice(CHAR *pszNotice)
{
	if(pszNotice == NULL) return FALSE;

	// ��� ����ڿ��� ���� �޽����� ������.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_TEXT, LPARAM(pszNotice));	
}

BOOL cltCommand::SendSMSMsg(SMS_REQ_MSG* pSMSMsg)
{
	if(pSMSMsg == NULL) return FALSE;

	// ��� ����ڿ��� ���� �޽����� ������.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_SMS_MSG, 
		LPARAM(pSMSMsg->dwCallCount), 
		LPARAM(pSMSMsg->szUserCode), 
		LPARAM(pSMSMsg->szOrgPhone), 
		LPARAM(pSMSMsg->szDstPhone), 
		LPARAM(pSMSMsg->szCallMessage), 
		LPARAM(pSMSMsg->szRecvTime), 
		LPARAM(pSMSMsg->szReserved));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ڿ��� �ӼӸ��� ������.
//	���� ����	:	2002-03-22 ���� 10:01:56 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::Whisper(UI16 uiPlayerID, CHAR *pszID, CHAR *pText)
{
	SOPlayer	*pPlayer;	
	SOPlayer	*pOpponentPlayer;
	CHAR		szWhipser[1024];

	if(ISPLAYER(uiPlayerID) != TRUE)	return	FALSE;	
	if((pPlayer	= pMyWorld->pPlayerOnAccount[uiPlayerID]) == NULL)	return	FALSE;
		
	if((pOpponentPlayer	=	pMyWorld->GetPlayer(pszID)) != NULL)
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "�ӼӸ��� ���������� �����߽��ϴ�.");

		sprintf(szWhipser, "[%s] : %s", pPlayer->szID, pText);
		pOpponentPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), szWhipser);
	}
	else
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "�ӼӸ��� �����ϴµ� �����߽��ϴ�.");
	}

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	��� ��������.
//	���� ����	:	2002-03-26 ���� 3:33:09 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GuildNotice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	if(uiGuildID == 0)		return	FALSE;
	if(pszNotice == NULL)	return	FALSE;	

	// �������� �޽����� ������.
	return pMyWorld->clIGuild.Notice(uiGuildID, pszName, pszNotice);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������� ������ ���´�.
//				������� ����(��, ��ǥ)���� ���ͼ� ��ڿ��� �����ش�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetPlayerInfo(UI16 uiGMPlayerID, UI16 uiPlayerID)
{
	SOPlayer	*pGMPlayer;
	SOPlayer	*pPlayer;
	CHAR		szPlayerInfo[1024] = "";
	CHAR		szID[ON_ID_LENGTH + 1];
	CHAR		*pszVillageName;
	POINT		ptMonsterPos;
	UI16		uiMonsterKind;
	CHAR		*pszMonsterName;
	UI16		uiMapIndex;
	SI32		siFieldID;
	
	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	ZeroMemory(szPlayerInfo, sizeof(szPlayerInfo));	

	// ������϶��� �����϶��� �ٸ��� ó���Ѵ�.
	if(ISPLAYER(uiPlayerID) == TRUE)
	{
		pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
		if(pPlayer == NULL) return FALSE;

		// �� ID�� ��� ��ȿ�ϴ�.

		// �÷��̾��� ID�� ���´�.
		ZeroMemory(szID, sizeof(szID));
		strncpy(szID, pPlayer->szID, ON_ID_LENGTH);		

		switch(pPlayer->siStatus)
		{
		case ON_PLAYER_STATUS_INFIELD:
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INVILLAGE:		
			if(pPlayer->stAccountInfo.pVillage != NULL)		pszVillageName	=	pMyWorld->clOnlineText.Get(pPlayer->stAccountInfo.pVillage->uiVillageName);
			else											pszVillageName	=	NULL;		
			
			if(pszVillageName != NULL)
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], ���� :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], ���� :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "�� �� ���� ����", pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Pos :[%d : %d, %d], ������..., State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INFIELD | ON_PLAYER_STATUS_INVILLAGE:
			if(pPlayer->stAccountInfo.pVillage != NULL)		pszVillageName	=	pMyWorld->clOnlineText.Get(pPlayer->stAccountInfo.pVillage->uiVillageName);
			else											pszVillageName	=	NULL;		

			if(pszVillageName != NULL)
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], ���� :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], ���� :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "�� �� ���� ����", 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		default:
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Unknown Status... [%d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->siStatus, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		}
	}
	else if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// ��ȿ�� �������� �˾ƿ´�.
		if(pMyWorld->clMonster.IsValidID(uiPlayerID) == FALSE)	return	FALSE;
			
		// ������ ������ ���´�.
		uiMonsterKind	=	pMyWorld->clMonster.GetKind(uiPlayerID);

		// ������ �̸��� ���´�.
		ZeroMemory(szID, sizeof(szID));
		pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind);
		if(pszMonsterName != NULL)
		{
			strncpy(szID, pszMonsterName, ON_ID_LENGTH);
		}
		else
		{
			strncpy(szID, "����", ON_ID_LENGTH);
		}
		
		// ������ ��ġ�� ���´�.
		pMyWorld->clMonster.GetPos(uiPlayerID, &uiMapIndex, &ptMonsterPos);

		siFieldID	=	pMyWorld->clMonster.GetFieldID(uiPlayerID);

		switch(pMyWorld->clMonster.GetStatus(uiPlayerID))
		{
		case ON_PLAYER_STATUS_INFIELD:
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d]", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d], ������...", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		default:
			sprintf(szPlayerInfo, "ID : [%s], Unknown Status...", szID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;
		}
	}
	else
	{
		sprintf(szPlayerInfo, "�� �� ���� ĳ���� �����Դϴ�");
	}

	// ��ڿ��� �����ش�.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szPlayerInfo));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������� ���ڿ� ID�� ����Ͽ� ���� Account�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::IDToAccount(UI16 uiGMPlayerID, CHAR *pszID)
{
	SOPlayer	*pGMPlayer;
	SI32		i;
	CHAR		szPlayerID[ON_ID_LENGTH + 1];
	CHAR		szAccount[1024];

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	for(i = 0; i < ON_MAX_CONNECTION; i++)
	{
		if(pMyWorld->clPlayer[i] != NULL)
		{
			ZeroMemory(szPlayerID, sizeof(szPlayerID));
			strncpy(szPlayerID, pMyWorld->clPlayer[i]->szID, ON_ID_LENGTH);
			if(strcmp(szPlayerID, pszID) == 0)
			{
				ZeroMemory(szAccount, sizeof(szAccount));
				wsprintf(szAccount, "ID : [%s], Account : %u", pszID, pMyWorld->clPlayer[i]->uiAccount);

				return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szAccount));
			}			
		}
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���ӵǾ� �ִ� ����ڵ��� ������ ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetConnectUsersInfo(UI16 uiGMPlayerID)
{
	CHAR		szInfo[1024] = "";
	SOPlayer	*pGMPlayer;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	wsprintf(szInfo, "�ִ� ���� ������ �� : [%d], ���� ������ �� : [%d]", pMyWorld->clGate->siMaxConnectedPlayer, pMyWorld->clGate->siTotalConnectPlayer);
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szInfo));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���� ������ �ʴ� ������ ���� ���Ѵ�.
//	���� ����	:	2002-07-08 ���� 3:19:41 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetFPS(UI16 uiGMPlayerID)
{
	CHAR		szInfo[1024] = "";
	SOPlayer	*pGMPlayer;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	wsprintf( szInfo, "Max FPS : [%u], Min FPS : [%u], Cur FPS : [%u]", pMyWorld->clServer->dwMaxCurrentFPS, pMyWorld->clServer->dwMinCurrentFPS, pMyWorld->clServer->dwCurrentFPS );
	pGMPlayer->SendFieldMsg( ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID( ON_RET_TEXTMSG_WHISPER ), LPVOID( szInfo ) );

	wsprintf( szInfo, "Max WFPS : [%u], Min WFPS : [%u], Cur WFPS : [%u]", pMyWorld->clServer->dwWorldMaxCurrentFPS, pMyWorld->clServer->dwWorldMinCurrentFPS, pMyWorld->clServer->dwWorldCurrentFPS );
	pGMPlayer->SendFieldMsg( ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID( ON_RET_TEXTMSG_WHISPER ), LPVOID( szInfo ) );

	return	TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	���ʽ��� ���� �����ش�.
//	���� ����	:	2002-08-12 ���� 2:27:43 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::BonusUp(UI16 uiPlayerID, UI08 uiFollowerID)
{
	// ����ڰ� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// ���ʽ��� Up ���ش�.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseBonus(uiFollowerID, 5);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	�ſ뵵�� ���� ��Ų��.
//	���� ����	:	2002-08-12 ���� 2:21:20 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::CreditUp(UI16 uiPlayerID, SI32 siCredit)
{
	// ����ڰ� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// ���ʽ��� Up ���ش�.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseCredit(siCredit);	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����ġ�� ���� ��Ų��.
//	���� ����	:	2002-09-26 ���� 5:28:14 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::ExpUp( UI16 uiPlayerID, UI08 uiFollowerID, UI32 uiExp )
{
	// ����ڰ� �ƴ� ��쿡�� FALSE�� �����Ѵ�.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// ����ġ�� ���������ش�.
	SOPlayer	*pPlayer;	

	if( ( pPlayer =	pMyWorld->GetPlayer( uiPlayerID ) ) == NULL)		return	FALSE;
	
	return pPlayer->expUp( uiFollowerID, uiExp );	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������� E-Mail�� ���´�.
//	���� ����	:	2002-02-20 ���� 3:35:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetUserMailAddress(UI16 uiGMPlayerID, UI16 uiPlayerID)
{
	SOPlayer	*pGMPlayer;
	SOPlayer	*pPlayer;
	CHAR		szCharName[ON_ID_LENGTH + 1];
	CHAR		szUserMailAddress[1024];
	CHAR		szBuffer[1024];

	// ����ڰ� �ƴϸ� FALSE�� �����Ѵ�.
	if(ISPLAYER(uiPlayerID)	!= TRUE)	return FALSE;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL)	return FALSE;

	pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL)		return FALSE;
	
	ZeroMemory(szCharName, sizeof(szCharName));
	strncpy(szCharName, pPlayer->szID, ON_ID_LENGTH);

	// ������� E-Mail�� ���´�.
	ZeroMemory(szUserMailAddress, sizeof(szUserMailAddress));
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_USERMAILADDRESS, LPVOID(szCharName), LPVOID(szUserMailAddress));
	
	wsprintf(szBuffer, "N:[%s], M:[%s]", szCharName, szUserMailAddress);

	// ��ڿ��� �����ش�.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ó���Ѵ�.	
//	���� ����	:	2002-03-26 ���� 3:33:34 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::ProcessCMD(UI16 uiGMPlayerID, CHAR *pszCMD)
{
	CHAR			cSep[] = " ,";
	CHAR            cSep2[] = "''";
	CHAR			*pToken;
	UI16			uiPlayerID;
	UI08			uiFollowerID;
	SI32			siX, siY;	
	UI16			uiMapID;
	UI16			uiItemQuantity;
	UI16			uiMonsterNum;
UI16			uiMonsterQuantity;
CHAR			uiMonsterName[128];
	SI32			siWorkAmount;
	MONEY			Money;
	SOPlayer		*pPlayer;
	CHAR			szID[ON_ID_LENGTH + 1];
	CHAR			szItemName[1024];
	strCommand		*pstCommand	=	NULL;
	SI32			siFieldMonsterID;	
	DWORD			dwAccount;
	UI32			uiUpCredit, uiUpExp;
	strWarpInfo		*pstWarpInfo;


	// GM�� ����� �ƴ϶�� FALSE
	if(ISPLAYER(uiGMPlayerID) != TRUE) return FALSE;

	// ��ȿ�� �÷��̾����� �ٽ� �ѹ� �˻��Ѵ�.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL)																return FALSE;

	// ��ɾ ���´�.
	if((pToken = strtok(pszCMD, cSep)) == NULL) return FALSE;		

	// ����� �����͸� ���´�.
	// �ش� ����� ���ų� �� ����� ����� �� �ִ� ������ �ƴϸ� FALSE�� �����Ѵ�.
	if((pstCommand = GetCommand(pToken, pPlayer->USGrade, pPlayer->siClassInGuild)) == NULL)	return	FALSE;		

	// m_szGMString�� �ʱ�ȭ �Ѵ�.
	ZeroMemory(m_szGMString, sizeof(m_szGMString));

	// m_szGMString�� ����� �̸��� �ִ´�.
	sprintf(m_szGMString, "[%s] : [%s] : ", pPlayer->szID, pToken);

	if(stricmp(pToken, m_stCommand[SO_COMMAND_WARP].szCommand) == 0)					// command 'w'
	{
		// uiPlayerID�� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// ������ �ϴ� ����� �ΰ����� �ִ�.
		// ���� �̸��� �Է��ϴ� �����
		// �� �ε���, ��ġ�� �Է��ϴ� ���.		

		// Token�� �ѹ� ���Ѵ�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		// �켱 ���� �̸��� ������ �ִ��� �˾ƺ���.
		pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pToken );

		if( pstWarpInfo != NULL )
		{
			uiMapID	=	pstWarpInfo->uiMapIndex;
			siX		=	pstWarpInfo->uiX;
			siY		=	pstWarpInfo->uiY;
		}
		else
		{
			// ���� �̸��� �Է��� ���� �ƴϴ�.

			uiMapID		=	atoi(pToken);

			// siX�� ���´�.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siX			=	atoi(pToken);

			// siY�� ���´�.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siY			=	atoi(pToken);
		}

		return Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_CREATEMONSTER].szCommand) == 0)		// command 'cm'
	{
/*		// ������ �̸��� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
		strcpy(uiMonsterName, pToken);

		if((pToken	= strtok(NULL, cSep)) == NULL) 
			uiMonsterQuantity = 10;
		else
			uiMonsterQuantity = atoi(pToken);
		if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

		return CreateMonster(uiGMPlayerID, uiMonsterName, 1, uiMonsterQuantity);*/
		//2004-12-09���� - �ֺ���
		int nRet = FindChar(pszCMD, 39); //���ڿ��� '�� �ִ��� �˻��Ѵ�.
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0: //�ڵ���
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				
				DWORD dwCode = 0;
				dwCode = ConvertCode(pToken, strlen(pToken));
				
				if(dwCode < 0) return FALSE;
				else if(dwCode == 0)
					dwCode = atoi(pToken); //������ �ڵ�
				
				//����
				if((pToken	= strtok(NULL, cSep)) == NULL) 
					uiMonsterQuantity = 10;
				else
					uiMonsterQuantity = atoi(pToken);
			
				if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

				return CreateMonsterID(uiGMPlayerID, dwCode, 1, uiMonsterQuantity);
			}
			break;

		case 1: //�̸� ���
			{
				if((pToken	= strtok(NULL, cSep2)) == NULL) return FALSE;
				ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
				strcpy(uiMonsterName, pToken); //������ �̸�
            
				//����
				if((pToken	= strtok(NULL, cSep)) == NULL) 
					uiMonsterQuantity = 10;
				else
					uiMonsterQuantity = atoi(pToken);
			
				if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

				return CreateMonster(uiGMPlayerID, uiMonsterName, 1, uiMonsterQuantity);
			}

		default:
			return FALSE;
		}
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GIVEMONEY].szCommand) == 0)			// command 'givem'
	{
		// uiPlayerID�� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// Money�� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		Money		=	atoi(pToken);

		return GiveMoney(uiPlayerID, Money);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GIVEITEM].szCommand) == 0)			// command 'givei'
	{
/*		// ������ �̸��� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		ZeroMemory(szItemName, sizeof(szItemName));
		strcpy(szItemName, pToken);

		// uiItemQuantity�� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL)	return FALSE;
		if( atoi(pToken) < 0 )						return FALSE;
		uiItemQuantity	=	atoi(pToken);

		return GiveItem(uiGMPlayerID, szItemName, uiItemQuantity); */
		//2004-12-09���� - �ֺ���
		int nRet = FindChar(pszCMD, 39);
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0:
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				UI16 uiItemId = atoi(pToken);
				if(uiItemId < 0) return FALSE;

				// uiItemQuantity�� ���´�.
				if((pToken	= strtok(NULL, cSep)) == NULL)	return FALSE;
				if( atoi(pToken) < 0 )						return FALSE;
				uiItemQuantity	=	atoi(pToken);

				return GiveItemID(uiGMPlayerID, uiItemId, uiItemQuantity);
			}
			break;

		case 1:
			{
				if((pToken	= strtok(NULL, cSep2)) == NULL) return FALSE;
				ZeroMemory(szItemName, sizeof(szItemName));
				strcpy(szItemName, pToken);

				// uiItemQuantity�� ���´�.
				if((pToken	= strtok(NULL, cSep)) == NULL)	return FALSE;
				if( atoi(pToken) < 0 )						return FALSE;
				uiItemQuantity	=	atoi(pToken);

				return GiveItem(uiGMPlayerID, szItemName, uiItemQuantity);
			}
			break;

		default:
			return FALSE;
		}
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_LOGOUT].szCommand) == 0)				// command 'logout'
	{
		// uiPlayerID�� ���´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		return LogOut(uiPlayerID);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_NOTICE].szCommand) == 0)
	{
		if((pToken	= strtok(NULL, "")) == NULL) return FALSE;

//		pMyWorld->clServer->WriteInfo( "..\\NoticeLog.txt", "[%s]\t%s", pPlayer->szID, pToken );
		writeInfoToFile( "NoticeLog.txt", "[%s]\t%s", pPlayer->szID, pToken );
		return Notice(pToken);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GETINFO].szCommand) == 0)
	{
		// ����� ������ ����� �˾ƿ´�.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		if(stricmp(pToken, "C") == 0)
		{
			// ������� ������ �������� �Ѵ�.

			// uiPlayerID�� ���´�.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID	=	atoi(pToken);

			return GetPlayerInfo(uiGMPlayerID, uiPlayerID);
		}		
		else if(stricmp(pToken, "I") == 0)
		{
			// Account�� ��ȯ�� ID�� ���´�.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			ZeroMemory(szID, sizeof(szID));
			strncpy(szID, pToken, ON_ID_LENGTH);

			return IDToAccount(uiGMPlayerID, szID);
		}
		else if(stricmp(pToken, "U") == 0)
		{
			return GetConnectUsersInfo(uiGMPlayerID);
		}
		else if(stricmp(pToken, "FPS") == 0)
		{
			return GetFPS(uiGMPlayerID);
		}
		else if(stricmp(pToken, "USERMA") == 0)
		{
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID	=	atoi(pToken);

			return GetUserMailAddress(uiGMPlayerID, uiPlayerID);
		}
		else if(stricmp(pToken, "FMINFO") == 0)
		{
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siFieldMonsterID	=	atoi(pToken);

			return GetFieldMonsterInfo(uiGMPlayerID, siFieldMonsterID);
		}		
		else if(stricmp(pToken, "WEIGHTINFO") == 0)
		{
			// ����� ������ ����� �˾ƿ´�.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID		=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiFollowerID	=	atoi(pToken);

			return GetFollowerWeightInfo(uiGMPlayerID, uiPlayerID, uiFollowerID);
		}
		else if(stricmp(pToken, "TI") == 0)
		{
			// ����� ������ ����� �˾ƿ´�.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siX				=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siY				=	atoi(pToken);

			return GetTileInfo(uiGMPlayerID, siX, siY);
		}
		else if(stricmp(pToken, "DBACTOAC") == 0)
		{
			// DB Account�� �̿��Ͽ� ĳ������ Account�� �˾ƿ´�.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			dwAccount		=	DWORD(atoi(pToken));

			return	GetPlayerAccountByDBAccount(uiGMPlayerID, dwAccount);
		}
		else if (stricmp ( pToken, "TOTALMONSTERNUM") == 0)
		{
			GetTotalMonsterNum( uiGMPlayerID );

			return	TRUE;
		}
		else
			return FALSE;	
	}	
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_BONUSUP].szCommand) == 0)
	{
		// ����� ������ ����� �˾ƿ´�.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiFollowerID	=	atoi(pToken);

		return BonusUp(uiPlayerID, uiFollowerID);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GNOTICE].szCommand) == 0)
	{
		// �������� ������ ������.
		if((pToken	= strtok(NULL, "")) == NULL) return FALSE;
		return GuildNotice(pPlayer->uiGuildID, pPlayer->szID, pToken);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_WHISPER].szCommand) == 0)
	{
		/*
		// Ư�� �÷��̾�� �ӼӸ��� ������.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		ZeroMemory(szID, sizeof(szID));
		strncpy(szID, pToken, ON_ID_LENGTH);

		if((pToken	= strtok(NULL, "")) == NULL) return FALSE;
		return	Whisper(uiGMPlayerID, szID, pToken);
		*/
		return FALSE;
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_CREDITUP].szCommand) == 0)
	{
		// ����� ������ ����� �˾ƿ´�.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiUpCredit		=	atoi(pToken);

		return CreditUp(uiPlayerID, uiUpCredit);
	}
	else if( stricmp(pToken, m_stCommand[ SO_COMMAND_EXPUP ].szCommand) == 0 )
	{
		// ����� ������ ����� �˾ƿ´�.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiFollowerID	=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiUpExp			=	atoi(pToken);

		return ExpUp( uiPlayerID, uiFollowerID, uiUpExp );
	}
	else if( stricmp(pToken, m_stCommand[ SO_COMMAND_KILL_NEAR_MONSTER ].szCommand) == 0 )
	{
		return	KillNearMonster( pPlayer->uiMapID, pPlayer->siX, pPlayer->siY );
	}
	else if( stricmp(pToken, m_stCommand[ SO_COMMAND_CLEAR_NEAR_ITEM ].szCommand) == 0 )
	{
		return	ClearNearItem( pPlayer );
	}
	else if( stricmp( pToken, m_stCommand[ SO_COMMAND_COMPLETE_QUEST ].szCommand) == 0 )
	{
		// ����� ����Ʈ�� �Ϸ��ϴ��� �˾ƿ´�.
		if( ( pToken = strtok( NULL, cSep ) ) == NULL )				return FALSE;		

		if( stricmp( pToken, "MONSTER" ) == 0 )
		{			
			if( ( pToken = strtok( NULL, cSep ) ) == NULL )			return FALSE;

			// ������ �̸�
			ZeroMemory( szID, sizeof( szID ) );
			strncpy( szID, pToken, ON_ID_LENGTH );

			// ������ ��
			if( ( pToken = strtok( NULL, cSep ) ) == NULL )			return FALSE;
			uiMonsterNum	=	atoi( pToken );			

			return	CompleteQuest_KillMonster( uiGMPlayerID, szID, uiMonsterNum );

		}
		else if( stricmp( pToken, "WORK" ) == 0 )
		{
			if( ( pToken = strtok( NULL, cSep ) ) == NULL )			return FALSE;
			siWorkAmount	=	atoi( pToken );

			return	CompleteQuest_DoWork( uiGMPlayerID, siWorkAmount );
		}
	}
	else if( stricmp( pToken, m_stCommand[ SO_COMMAND_PARTYCHAT ].szCommand) == 0 )
	{
		// ��Ƽ���鿡�� ������ ������.
		if((pToken	= strtok(NULL, "")) == NULL)					return FALSE;

		return PartyChat( pPlayer, pToken);		
	}


	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	ȭ�Ͽ� �����Ѵ�.
//	���� ����	:	2002-02-20 ���� 10:12:18 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltCommand::WriteStringToFile()
{

	writeInfoToFile("CommandLog.txt", "%s",  m_szGMString);		
	// ȭ���� �����Ѵ�.
/*
	FILE	*fp;
	CHAR	szDateBuf[128];
	CHAR	szTimeBuf[128];	

	ZeroMemory(szDateBuf, sizeof(szDateBuf));
	ZeroMemory(szTimeBuf, sizeof(szTimeBuf));

	_strtime(szTimeBuf);
	_strdate(szDateBuf);

	if((fp = fopen("CommandLog.txt", "at")) != NULL)
	{
		fprintf(fp,"[%s %s] %s\n", szDateBuf, szTimeBuf, m_szGMString);		
		fclose(fp);
	}
*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	����� ���´�.
//	���� ����	:	2002-03-27 ���� 9:19:16 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
strCommand*	cltCommand::GetCommand(CHAR *pszCommand, UserGrade USGrade, SI32 siClassInGuild)
{
	SI32	i;	
	SI32	siReqClass;

	for(i = 0; i < SO_TOTAL_COMMAND_NUM; i++)
	{
		if(stricmp(m_stCommand[i].szCommand, pszCommand) == 0)
		{		
			if(m_stCommand[i].siReqGrade > USGrade)	return	NULL;
			//if(ON_USERGRADE_NORMAL ==  USGrade)	return	NULL;

			if(stricmp(m_stCommand[i].szReqClassInGuild, "") != 0)				
			{
				siReqClass	=	pMyWorld->clIGuildClass.GetNumericClassCode(m_stCommand[i].szReqClassInGuild);

				if(pMyWorld->clIGuildClass.GetHighClass(siReqClass, siClassInGuild) != siClassInGuild)	return	NULL;
			}	

			return	&m_stCommand[i];
		}
	}
	
	return	NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	FieldMonster�� ������ ���´�.
//	���� ����	:	2002-06-17 ���� 10:57:58 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetFieldMonsterInfo(UI16 uiGMPlayerID, SI32 siFieldMonsterID)
{
	BOOL		bSuccess;
	CHAR		szBuffer[1024];
	SOPlayer	*pGMPlayer;

	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	// FieldMonster�� ������ ���´�.
	bSuccess	=	pMyWorld->clFieldMonster.GetInfo(siFieldMonsterID, szBuffer);

	if(bSuccess == TRUE)
	{
		return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
	}
	else
		return	FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Follower�� ���� ������ ���´�.
//	���� ����	:	2002-07-12 ���� 3:37:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetFollowerWeightInfo(UI16 uiGMPlayerID, UI16 uiPlayerID, UI08 uiFollowerID)
{
	SOPlayer	*pGMPlayer;	
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];

	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	if(pPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;
	
	wsprintf(szBuffer, "Follower Weight Info : [%d], [%d], [%d]", uiPlayerID, uiFollowerID, pPlayer->IFollower.GetTotalItemWeight(uiFollowerID));

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	Ÿ���� ������ ���´�.
//	���� ����	:	2002-07-18 ���� 3:25:24 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetTileInfo(UI16 uiGMPlayerID, SI32 siX, SI32 siY)
{
	SOPlayer	*pGMPlayer;		
	CHAR		szBuffer[1024];	

	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	ZeroMemory(szBuffer, sizeof(szBuffer));
	if(pMyWorld->clISOMap.GetTileInfo(pGMPlayer->uiMapID, siX, siY, szBuffer) == FALSE)	return	FALSE;

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	DB Account�� �̿��Ͽ� �÷��̾��� Account�� �̿��Ѵ�.
//	���� ����	:	2002-08-12 ���� 1:26:10 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::GetPlayerAccountByDBAccount(UI16 uiGMPlayerID, DWORD dwDBAccount)
{	
	SOPlayer	*pGMPlayer;		
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];	

	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	// ��� �÷��̾��� Account�� ���´�.
	pPlayer			=	pMyWorld->GetPlayerByDBAccount(dwDBAccount);
	if(pPlayer == NULL)	return	FALSE;
	
	wsprintf(szBuffer, "DBAccount : [%u], Account : [%u]", pPlayer->stAccountInfo.siDBAccount, pPlayer->uiAccount);

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));		
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����		:	������ ���͵��� ���δ�.
//	���� ����	:	2002-12-17 ���� 10:49:07 - ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltCommand::KillNearMonster( UI16 uiMapID, SI32 siX, SI32 siY )
{	
//	UI16	uiCharUniIDs[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	UI16	uiCharUniIDs[ 5000 ];
	SI32	siMonsterNum;
	SI32	i;

	siMonsterNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList( uiMapID, siX, siY, uiCharUniIDs, ON_MAP_CHARID_MONSTER );

	for( i = 0; i < siMonsterNum; i++ )
	{
		pMyWorld->clMonster.DeadChar( uiCharUniIDs[ i ], FALSE );
	}

	return	TRUE;
}

BOOL	cltCommand::GetTotalMonsterNum( UI16 uiGMPlayerID )
{
	SOPlayer	*pGMPlayer;		
	CHAR		szBuffer[1024];	

	// �ش� �÷��̾��� ������ ���´�.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;		
	
	wsprintf(szBuffer, "Total Monster Num : [%u]", pMyWorld->clMonster.GetTotalMonsterNum() );

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));			
}

BOOL	cltCommand::ClearNearItem( SOPlayer *pGMPlayer )
{
	OnlineReceiveFieldItemInfo	FieldItemInfo[ ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	OnlineReceiveFieldItemInfo	*pFieldItem;
	UI16						uiCharUniIDs [ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE ];
	SI32						siFieldItemNum, siSendCharNum;
	SI32						i;
	OnChangeFieldItemMsg		ChangeFieldItemMsg;

	// �޽����� �����Ѵ�.
	ChangeFieldItemMsg.dwMsgType		=	ON_CHANGE_FIELDITEM;

	siFieldItemNum	=	pMyWorld->clISOMap.GetNearSectorItemList( pGMPlayer->uiMapID, pGMPlayer->siX, pGMPlayer->siY, FieldItemInfo );

	for( i = 0; i < siFieldItemNum; i++ )
	{	
		pFieldItem		=	&FieldItemInfo[ i ];		

		// �ʵ忡�� �������� �����Ѵ�.
		if( pMyWorld->clISOMap.DelItem( pGMPlayer, pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY ) == TRUE )
		{
			
			ChangeFieldItemMsg.siX				=	pFieldItem->siX;
			ChangeFieldItemMsg.siY				=	pFieldItem->siY;
			ChangeFieldItemMsg.uiItemID			=	0;
			ChangeFieldItemMsg.uiItemQuantity	=	0;			

			siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList( pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY, 
													uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
			
			// �ٸ� �����鿡�� �������� ���ŵǾ��ٰ� �˷��ش�.
			pGMPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), LPVOID( &ChangeFieldItemMsg), NULL );
		}
	}

	return	TRUE;
}


BOOL	cltCommand::CompleteQuest_KillMonster( UI16 uiGMPlayerID, CHAR *pszMonsterName, UI16 uiMonsterNum )
{
	strOnlineBaseCharKI		*pOnlineBaseCharKI;
	SOPlayer				*pGMPlayer;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[ uiGMPlayerID ];
	if( pGMPlayer == NULL ) return FALSE;

	if( ( pOnlineBaseCharKI = pMyWorld->clSOCharKI.GetBaseCharKI( pszMonsterName ) ) == NULL )		return	FALSE;
	if( uiMonsterNum > 10000 )																		return	FALSE;

	pGMPlayer->Quest_AddQuestObject_KillMonster( pOnlineBaseCharKI->uiID, uiMonsterNum );

	return	TRUE;
}

BOOL	cltCommand::CompleteQuest_DoWork( UI16 uiGMPlayerID, SI32 siWorkAmount )
{
	SOPlayer				*pGMPlayer;		

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[ uiGMPlayerID ];
	if( pGMPlayer == NULL ) return FALSE;

	if( siWorkAmount > 10000 )																		return	FALSE;

	pGMPlayer->Quest_AddQuestObject_Work( siWorkAmount );	

	return	TRUE;
}

BOOL	cltCommand::PartyChat( SOPlayer *pPlayer, CHAR *pszChatMsg )
{
	// ��Ƽ���鿡�� 
	return pPlayer->PartyChat( pszChatMsg );	
}

int     cltCommand::FindChar(CHAR *pszStr, CHAR find)
{
	if(pszStr == NULL) return -1;

	int i = 3;
	for(; i < 9; i++)
	{
		if(pszStr[i] == find) return 1;
	}

	return 0;
}

//2004-12-13 �Է¹��� ���ڿ��� DWORD������ ��ȯ �Ѵ�. - �ֺ���
DWORD   cltCommand::ConvertCode(char *szStr, int nSize)
{
	if(szStr == NULL || nSize <= 0) return -1;
	if((szStr[0] < 65 || szStr[0] > 90) && (szStr[0] < 97 || szStr[0] > 122)) return 0;

	DWORD dwCode = 0;
	char Temp;
	char Temp1[8] = {0,};
	
	memcpy(&Temp, szStr, 1);
	memcpy(Temp1, &szStr[1], nSize - 1);

	int ncode = atoi(Temp1);

	if(szStr[0] >= 97 && szStr[0] <= 122) 
		dwCode = CONVERT_CHARCODE_TO_DBCHARNO(Temp - 32, ncode);
	else 
		dwCode = CONVERT_CHARCODE_TO_DBCHARNO(Temp, ncode);

	return dwCode;
}