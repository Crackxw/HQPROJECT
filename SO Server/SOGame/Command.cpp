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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	생성자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltCommand::cltCommand()
{	
	pMyWorld	=	NULL;		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	소멸자.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cltCommand::~cltCommand()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	초기화.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltCommand::Init(SOWorld *pmyworld)
{
	pMyWorld	=	pmyworld;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 워프 시킨다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY)
{
	SOPlayer	*pPlayer;
	CHAR		szGMString[1024];

	ZeroMemory(szGMString, sizeof(szGMString));
	

	// 현재는 몬스터와 사람만 워프를 시킬 수 있다.
	if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// 몬스터이다.
		sprintf(szGMString, "[%s] [%d] [%d] [%d]", pMyWorld->clSOCharKI.GetNameByString(pMyWorld->clMonster.GetKind(uiPlayerID)), 
													uiMapID, siX, siY);
		strcat(m_szGMString, szGMString);
		WriteStringToFile();		

		return pMyWorld->clMonster.Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(ISPLAYER(uiPlayerID) == TRUE)
	{
		// 사람이다.
		// 사람의 경우에는 새로운 곳으로 워프 하였을 경우 주위의 환경을 다시 받아야 하기 때문에
		// 절차가 다소 복잡하다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터를 생성한다.
//	수정 일자	:	2002-12-10 - 유재휘
//	>cm M01 100
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	if ( BYTE(pszMonsterName[0]) > 127 ) // 한글
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	몬스터의 아이디로 몬스터를 생성한다
//	수정 일자	:	2004-12-2 오후 2:01 - 최병주
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 돈을 조정한다.
//	수정 일자	:	2002-10-24 오후 2:27:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GiveMoney(UI16 uiPlayerID, MONEY Money)
{
	SOPlayer		*pPlayer;	
	CHAR			szGMString[1024];

	// 사용자가 아닐 경우에는 FALSE를 리턴한다.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];	
	if(pPlayer == NULL)	return FALSE;

	MONEY tempMoney = pPlayer->siMoney;

	pPlayer->GiveBonusMoney( Money, ON_RET_CHNGEMONEY_FROMADMIN ,FALSE);

	DBIKGiveMoneyByAdmin DBInfo;

	DBInfo.siDBAccount = pPlayer->stAccountInfo.siDBAccount;
	DBInfo.uiDBSlot = pPlayer->stAccountInfo.uiSlot;
	// 변화량을 기록.
	DBInfo.moMoney = pPlayer->siMoney - tempMoney;

	pPlayer->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GIVEMONEYBYADMIN,&DBInfo,sizeof(DBInfo));
	
	sprintf(szGMString, "[%s] [%I64d]", pPlayer->szID, Money );
	strcat(m_szGMString, szGMString);
	WriteStringToFile();		

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	아이템을 땅에 버려준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 아이템 수량이 0일 경우 FALSE를 리턴한다.
	if(uiItemQuantity == 0) return FALSE;

	// 존재하는 아이템이 아닐 경우에는 FALSE를 리턴한다.
	if((pItemHeader = pMyWorld->clItem.GetItem(pszItemName)) == NULL) return FALSE;
	
	// GM을 기준으로 아이템을 버린다.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// 아이템을 버릴 수 있는 자리가 있는지 판단한다.
	// ---- 윤영대 - 20050304 수정
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// 아이템을 심을 수 있는 좌표를 찾지 못했다.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// 아이템을 떨어뜨릴 위치를 찾았다.
	// 그 아이템을 떨어뜨리는 위치를 기준으로 주위의 캐릭터에게 아이템을 떨어뜨렸다고 보내준다.
	// 여기서 임시로 메시지를 만들어준다.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// 맵에 그 아이템을 설정한다.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;


	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;
	//Msg.uiItemID = pItemHeader->m_uiID;
	//Msg.uiItemQuantity = uiItemQuantity;

	// 주의: 기존 아이템과 수량이 합쳐졌을 수도 있으므로 최종 수량을 다시 얻어온다.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;

	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);



	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	/*pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
							LPVOID( ReqThrowItemMsg.uiItemID ), LPVOID( ReqThrowItemMsg.uiQuantity ) );*/

	// DB에 저장한다.
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

	// 아이템 수량이 0일 경우 FALSE를 리턴한다.
	if(uiItemQuantity == 0) return FALSE;

	// 존재하는 아이템이 아닐 경우에는 FALSE를 리턴한다.
	if((pItemHeader = pMyWorld->clItem.GetItem(uiItemID)) == NULL) return FALSE;
	
	// GM을 기준으로 아이템을 버린다.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// 아이템을 버릴 수 있는 자리가 있는지 판단한다.
	// ---- 윤영대 - 20050304 수정
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// 아이템을 심을 수 있는 좌표를 찾지 못했다.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// 아이템을 떨어뜨릴 위치를 찾았다.
	// 그 아이템을 떨어뜨리는 위치를 기준으로 주위의 캐릭터에게 아이템을 떨어뜨렸다고 보내준다.
	// 여기서 임시로 메시지를 만들어준다.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// 맵에 그 아이템을 설정한다.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;
	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;

	// 주의: 기존 아이템과 수량이 합쳐졌을 수도 있으므로 최종 수량을 다시 얻어온다.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;
	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	// DB에 저장한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 추방한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::LogOut(UI16 uiPlayerID)
{
	SOPlayer				*pPlayer;

	// 몬스터의 경우에도 '로그아웃'이 된다.
	// 몬스터의 경우에는 죽는다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	공지사항.
//	수정 일자	:	2002-03-26 오후 3:31:45 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::Notice(CHAR *pszNotice)
{
	if(pszNotice == NULL) return FALSE;

	// 모든 사용자에게 공지 메시지를 보낸다.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_TEXT, LPARAM(pszNotice));	
}

BOOL cltCommand::SendSMSMsg(SMS_REQ_MSG* pSMSMsg)
{
	if(pSMSMsg == NULL) return FALSE;

	// 모든 사용자에게 공지 메시지를 보낸다.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_SMS_MSG, 
		LPARAM(pSMSMsg->dwCallCount), 
		LPARAM(pSMSMsg->szUserCode), 
		LPARAM(pSMSMsg->szOrgPhone), 
		LPARAM(pSMSMsg->szDstPhone), 
		LPARAM(pSMSMsg->szCallMessage), 
		LPARAM(pSMSMsg->szRecvTime), 
		LPARAM(pSMSMsg->szReserved));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용자에게 귓속말을 보낸다.
//	수정 일자	:	2002-03-22 오전 10:01:56 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::Whisper(UI16 uiPlayerID, CHAR *pszID, CHAR *pText)
{
	SOPlayer	*pPlayer;	
	SOPlayer	*pOpponentPlayer;
	CHAR		szWhipser[1024];

	if(ISPLAYER(uiPlayerID) != TRUE)	return	FALSE;	
	if((pPlayer	= pMyWorld->pPlayerOnAccount[uiPlayerID]) == NULL)	return	FALSE;
		
	if((pOpponentPlayer	=	pMyWorld->GetPlayer(pszID)) != NULL)
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "귓속말을 성공적으로 전송했습니다.");

		sprintf(szWhipser, "[%s] : %s", pPlayer->szID, pText);
		pOpponentPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), szWhipser);
	}
	else
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "귓속말을 전송하는데 실패했습니다.");
	}

	return	TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	길드 공지사항.
//	수정 일자	:	2002-03-26 오후 3:33:09 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GuildNotice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	if(uiGuildID == 0)		return	FALSE;
	if(pszNotice == NULL)	return	FALSE;	

	// 길드원에게 메시지를 보낸다.
	return pMyWorld->clIGuild.Notice(uiGuildID, pszName, pszNotice);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용자의 정보를 얻어온다.
//				사용자의 정보(돈, 좌표)등을 얻어와서 운영자에게 보내준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	
	// 해당 플레이어의 정보를 얻어온다.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	ZeroMemory(szPlayerInfo, sizeof(szPlayerInfo));	

	// 사용자일때와 몬스터일때와 다르게 처리한다.
	if(ISPLAYER(uiPlayerID) == TRUE)
	{
		pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
		if(pPlayer == NULL) return FALSE;

		// 두 ID가 모두 유효하다.

		// 플레이어의 ID를 얻어온다.
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
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 마을 :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 마을 :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "알 수 없는 마을", pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Pos :[%d : %d, %d], 전투중..., State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INFIELD | ON_PLAYER_STATUS_INVILLAGE:
			if(pPlayer->stAccountInfo.pVillage != NULL)		pszVillageName	=	pMyWorld->clOnlineText.Get(pPlayer->stAccountInfo.pVillage->uiVillageName);
			else											pszVillageName	=	NULL;		

			if(pszVillageName != NULL)
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 마을 :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 마을 :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "알 수 없는 마을", 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		default:
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Unknown Status... [%d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->siStatus, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		}
	}
	else if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// 유효한 몬스터인지 알아온다.
		if(pMyWorld->clMonster.IsValidID(uiPlayerID) == FALSE)	return	FALSE;
			
		// 몬스터의 종류를 얻어온다.
		uiMonsterKind	=	pMyWorld->clMonster.GetKind(uiPlayerID);

		// 몬스터의 이름을 얻어온다.
		ZeroMemory(szID, sizeof(szID));
		pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind);
		if(pszMonsterName != NULL)
		{
			strncpy(szID, pszMonsterName, ON_ID_LENGTH);
		}
		else
		{
			strncpy(szID, "몬스터", ON_ID_LENGTH);
		}
		
		// 몬스터의 위치를 얻어온다.
		pMyWorld->clMonster.GetPos(uiPlayerID, &uiMapIndex, &ptMonsterPos);

		siFieldID	=	pMyWorld->clMonster.GetFieldID(uiPlayerID);

		switch(pMyWorld->clMonster.GetStatus(uiPlayerID))
		{
		case ON_PLAYER_STATUS_INFIELD:
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d]", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d], 전투중...", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		default:
			sprintf(szPlayerInfo, "ID : [%s], Unknown Status...", szID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;
		}
	}
	else
	{
		sprintf(szPlayerInfo, "알 수 없는 캐릭터 종류입니다");
	}

	// 운영자에게 보내준다.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szPlayerInfo));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용자의 문자열 ID를 사용하여 숫자 Account를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 접속되어 있는 사용자들의 정보를 구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetConnectUsersInfo(UI16 uiGMPlayerID)
{
	CHAR		szInfo[1024] = "";
	SOPlayer	*pGMPlayer;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	wsprintf(szInfo, "최대 동시 접속자 수 : [%d], 현재 접속자 수 : [%d]", pMyWorld->clGate->siMaxConnectedPlayer, pMyWorld->clGate->siTotalConnectPlayer);
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szInfo));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	현재 서버의 초당 프레임 수를 구한다.
//	수정 일자	:	2002-07-08 오전 3:19:41 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	보너스를 증가 시켜준다.
//	수정 일자	:	2002-08-12 오후 2:27:43 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::BonusUp(UI16 uiPlayerID, UI08 uiFollowerID)
{
	// 사용자가 아닐 경우에는 FALSE를 리턴한다.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 보너스를 Up 해준다.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseBonus(uiFollowerID, 5);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	신용도를 증가 시킨다.
//	수정 일자	:	2002-08-12 오후 2:21:20 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::CreditUp(UI16 uiPlayerID, SI32 siCredit)
{
	// 사용자가 아닐 경우에는 FALSE를 리턴한다.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 보너스를 Up 해준다.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseCredit(siCredit);	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	경험치를 증가 시킨다.
//	수정 일자	:	2002-09-26 오후 5:28:14 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::ExpUp( UI16 uiPlayerID, UI08 uiFollowerID, UI32 uiExp )
{
	// 사용자가 아닐 경우에는 FALSE를 리턴한다.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 경험치를 증가시켜준다.
	SOPlayer	*pPlayer;	

	if( ( pPlayer =	pMyWorld->GetPlayer( uiPlayerID ) ) == NULL)		return	FALSE;
	
	return pPlayer->expUp( uiFollowerID, uiExp );	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	사용자의 E-Mail을 얻어온다.
//	수정 일자	:	2002-02-20 오후 3:35:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetUserMailAddress(UI16 uiGMPlayerID, UI16 uiPlayerID)
{
	SOPlayer	*pGMPlayer;
	SOPlayer	*pPlayer;
	CHAR		szCharName[ON_ID_LENGTH + 1];
	CHAR		szUserMailAddress[1024];
	CHAR		szBuffer[1024];

	// 사용자가 아니면 FALSE를 리턴한다.
	if(ISPLAYER(uiPlayerID)	!= TRUE)	return FALSE;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL)	return FALSE;

	pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL)		return FALSE;
	
	ZeroMemory(szCharName, sizeof(szCharName));
	strncpy(szCharName, pPlayer->szID, ON_ID_LENGTH);

	// 사용자의 E-Mail을 얻어온다.
	ZeroMemory(szUserMailAddress, sizeof(szUserMailAddress));
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_USERMAILADDRESS, LPVOID(szCharName), LPVOID(szUserMailAddress));
	
	wsprintf(szBuffer, "N:[%s], M:[%s]", szCharName, szUserMailAddress);

	// 운영자에게 보내준다.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	명령을 처리한다.	
//	수정 일자	:	2002-03-26 오후 3:33:34 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


	// GM이 사람이 아니라면 FALSE
	if(ISPLAYER(uiGMPlayerID) != TRUE) return FALSE;

	// 유효한 플레이어인지 다시 한번 검사한다.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL)																return FALSE;

	// 명령어를 얻어온다.
	if((pToken = strtok(pszCMD, cSep)) == NULL) return FALSE;		

	// 명령의 포인터를 얻어온다.
	// 해당 명령이 없거나 그 명령을 사용할 수 있는 권한이 아니면 FALSE를 리턴한다.
	if((pstCommand = GetCommand(pToken, pPlayer->USGrade, pPlayer->siClassInGuild)) == NULL)	return	FALSE;		

	// m_szGMString를 초기화 한다.
	ZeroMemory(m_szGMString, sizeof(m_szGMString));

	// m_szGMString에 운영자의 이름을 넣는다.
	sprintf(m_szGMString, "[%s] : [%s] : ", pPlayer->szID, pToken);

	if(stricmp(pToken, m_stCommand[SO_COMMAND_WARP].szCommand) == 0)					// command 'w'
	{
		// uiPlayerID를 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// 워프를 하는 방법은 두가지가 있다.
		// 마을 이름을 입력하는 방법과
		// 맵 인덱스, 위치를 입력하는 방법.		

		// Token을 한번 더한다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		// 우선 같은 이름의 마을이 있는지 알아본다.
		pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pToken );

		if( pstWarpInfo != NULL )
		{
			uiMapID	=	pstWarpInfo->uiMapIndex;
			siX		=	pstWarpInfo->uiX;
			siY		=	pstWarpInfo->uiY;
		}
		else
		{
			// 마을 이름을 입력한 것이 아니다.

			uiMapID		=	atoi(pToken);

			// siX를 얻어온다.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siX			=	atoi(pToken);

			// siY를 얻어온다.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siY			=	atoi(pToken);
		}

		return Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_CREATEMONSTER].szCommand) == 0)		// command 'cm'
	{
/*		// 몬스터의 이름을 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
		strcpy(uiMonsterName, pToken);

		if((pToken	= strtok(NULL, cSep)) == NULL) 
			uiMonsterQuantity = 10;
		else
			uiMonsterQuantity = atoi(pToken);
		if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

		return CreateMonster(uiGMPlayerID, uiMonsterName, 1, uiMonsterQuantity);*/
		//2004-12-09수정 - 최병주
		int nRet = FindChar(pszCMD, 39); //문자열에 '가 있는지 검사한다.
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0: //코드방식
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				
				DWORD dwCode = 0;
				dwCode = ConvertCode(pToken, strlen(pToken));
				
				if(dwCode < 0) return FALSE;
				else if(dwCode == 0)
					dwCode = atoi(pToken); //몬스터의 코드
				
				//개수
				if((pToken	= strtok(NULL, cSep)) == NULL) 
					uiMonsterQuantity = 10;
				else
					uiMonsterQuantity = atoi(pToken);
			
				if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

				return CreateMonsterID(uiGMPlayerID, dwCode, 1, uiMonsterQuantity);
			}
			break;

		case 1: //이름 방식
			{
				if((pToken	= strtok(NULL, cSep2)) == NULL) return FALSE;
				ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
				strcpy(uiMonsterName, pToken); //몬스터의 이름
            
				//개수
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
		// uiPlayerID를 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// Money를 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		Money		=	atoi(pToken);

		return GiveMoney(uiPlayerID, Money);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GIVEITEM].szCommand) == 0)			// command 'givei'
	{
/*		// 아이템 이름을 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		ZeroMemory(szItemName, sizeof(szItemName));
		strcpy(szItemName, pToken);

		// uiItemQuantity를 얻어온다.
		if((pToken	= strtok(NULL, cSep)) == NULL)	return FALSE;
		if( atoi(pToken) < 0 )						return FALSE;
		uiItemQuantity	=	atoi(pToken);

		return GiveItem(uiGMPlayerID, szItemName, uiItemQuantity); */
		//2004-12-09수정 - 최병주
		int nRet = FindChar(pszCMD, 39);
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0:
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				UI16 uiItemId = atoi(pToken);
				if(uiItemId < 0) return FALSE;

				// uiItemQuantity를 얻어온다.
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

				// uiItemQuantity를 얻어온다.
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
		// uiPlayerID를 얻어온다.
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
		// 어떤것의 정보를 얻는지 알아온다.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		if(stricmp(pToken, "C") == 0)
		{
			// 사용자의 정보를 얻으려고 한다.

			// uiPlayerID를 얻어온다.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID	=	atoi(pToken);

			return GetPlayerInfo(uiGMPlayerID, uiPlayerID);
		}		
		else if(stricmp(pToken, "I") == 0)
		{
			// Account로 변환할 ID를 얻어온다.
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
			// 어떤것의 정보를 얻는지 알아온다.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID		=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiFollowerID	=	atoi(pToken);

			return GetFollowerWeightInfo(uiGMPlayerID, uiPlayerID, uiFollowerID);
		}
		else if(stricmp(pToken, "TI") == 0)
		{
			// 어떤것의 정보를 얻는지 알아온다.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siX				=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siY				=	atoi(pToken);

			return GetTileInfo(uiGMPlayerID, siX, siY);
		}
		else if(stricmp(pToken, "DBACTOAC") == 0)
		{
			// DB Account를 이용하여 캐릭터의 Account를 알아온다.
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
		// 어떤것의 정보를 얻는지 알아온다.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiFollowerID	=	atoi(pToken);

		return BonusUp(uiPlayerID, uiFollowerID);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GNOTICE].szCommand) == 0)
	{
		// 길드원에게 공지를 보낸다.
		if((pToken	= strtok(NULL, "")) == NULL) return FALSE;
		return GuildNotice(pPlayer->uiGuildID, pPlayer->szID, pToken);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_WHISPER].szCommand) == 0)
	{
		/*
		// 특정 플레이어에게 귓속말을 보낸다.
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
		// 어떤것의 정보를 얻는지 알아온다.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiUpCredit		=	atoi(pToken);

		return CreditUp(uiPlayerID, uiUpCredit);
	}
	else if( stricmp(pToken, m_stCommand[ SO_COMMAND_EXPUP ].szCommand) == 0 )
	{
		// 어떤것의 정보를 얻는지 알아온다.
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
		// 어떤것의 퀘스트를 완료하는지 알아온다.
		if( ( pToken = strtok( NULL, cSep ) ) == NULL )				return FALSE;		

		if( stricmp( pToken, "MONSTER" ) == 0 )
		{			
			if( ( pToken = strtok( NULL, cSep ) ) == NULL )			return FALSE;

			// 몬스터의 이름
			ZeroMemory( szID, sizeof( szID ) );
			strncpy( szID, pToken, ON_ID_LENGTH );

			// 몬스터의 수
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
		// 파티원들에게 공지를 보낸다.
		if((pToken	= strtok(NULL, "")) == NULL)					return FALSE;

		return PartyChat( pPlayer, pToken);		
	}


	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	화일에 저장한다.
//	수정 일자	:	2002-02-20 오전 10:12:18 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltCommand::WriteStringToFile()
{

	writeInfoToFile("CommandLog.txt", "%s",  m_szGMString);		
	// 화일을 생성한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	명령을 얻어온다.
//	수정 일자	:	2002-03-27 오전 9:19:16 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	FieldMonster의 정보를 얻어온다.
//	수정 일자	:	2002-06-17 오전 10:57:58 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetFieldMonsterInfo(UI16 uiGMPlayerID, SI32 siFieldMonsterID)
{
	BOOL		bSuccess;
	CHAR		szBuffer[1024];
	SOPlayer	*pGMPlayer;

	// 해당 플레이어의 정보를 얻어온다.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	// FieldMonster의 정보를 얻어온다.
	bSuccess	=	pMyWorld->clFieldMonster.GetInfo(siFieldMonsterID, szBuffer);

	if(bSuccess == TRUE)
	{
		return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
	}
	else
		return	FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	Follower의 무게 정보를 얻어온다.
//	수정 일자	:	2002-07-12 오후 3:37:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetFollowerWeightInfo(UI16 uiGMPlayerID, UI16 uiPlayerID, UI08 uiFollowerID)
{
	SOPlayer	*pGMPlayer;	
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];

	// 해당 플레이어의 정보를 얻어온다.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	if(pPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;
	
	wsprintf(szBuffer, "Follower Weight Info : [%d], [%d], [%d]", uiPlayerID, uiFollowerID, pPlayer->IFollower.GetTotalItemWeight(uiFollowerID));

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	타일의 정보를 얻어온다.
//	수정 일자	:	2002-07-18 오후 3:25:24 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetTileInfo(UI16 uiGMPlayerID, SI32 siX, SI32 siY)
{
	SOPlayer	*pGMPlayer;		
	CHAR		szBuffer[1024];	

	// 해당 플레이어의 정보를 얻어온다.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	ZeroMemory(szBuffer, sizeof(szBuffer));
	if(pMyWorld->clISOMap.GetTileInfo(pGMPlayer->uiMapID, siX, siY, szBuffer) == FALSE)	return	FALSE;

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	DB Account를 이용하여 플레이어의 Account를 이용한다.
//	수정 일자	:	2002-08-12 오후 1:26:10 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltCommand::GetPlayerAccountByDBAccount(UI16 uiGMPlayerID, DWORD dwDBAccount)
{	
	SOPlayer	*pGMPlayer;		
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];	

	// 해당 플레이어의 정보를 얻어온다.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	// 대상 플레이어의 Account를 얻어온다.
	pPlayer			=	pMyWorld->GetPlayerByDBAccount(dwDBAccount);
	if(pPlayer == NULL)	return	FALSE;
	
	wsprintf(szBuffer, "DBAccount : [%u], Account : [%u]", pPlayer->stAccountInfo.siDBAccount, pPlayer->uiAccount);

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));		
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명		:	주위에 몬스터들을 죽인다.
//	수정 일자	:	2002-12-17 오전 10:49:07 - 양정모
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

	// 해당 플레이어의 정보를 얻어온다.
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

	// 메시지를 세팅한다.
	ChangeFieldItemMsg.dwMsgType		=	ON_CHANGE_FIELDITEM;

	siFieldItemNum	=	pMyWorld->clISOMap.GetNearSectorItemList( pGMPlayer->uiMapID, pGMPlayer->siX, pGMPlayer->siY, FieldItemInfo );

	for( i = 0; i < siFieldItemNum; i++ )
	{	
		pFieldItem		=	&FieldItemInfo[ i ];		

		// 필드에서 아이템을 제거한다.
		if( pMyWorld->clISOMap.DelItem( pGMPlayer, pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY ) == TRUE )
		{
			
			ChangeFieldItemMsg.siX				=	pFieldItem->siX;
			ChangeFieldItemMsg.siY				=	pFieldItem->siY;
			ChangeFieldItemMsg.uiItemID			=	0;
			ChangeFieldItemMsg.uiItemQuantity	=	0;			

			siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList( pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY, 
													uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
			
			// 다른 유저들에게 아이템이 제거되었다고 알려준다.
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
	// 파티원들에게 
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

//2004-12-13 입력받은 문자열을 DWORD형으로 변환 한다. - 최병주
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