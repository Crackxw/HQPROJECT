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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltCommand::cltCommand()
{	
	pMyWorld	=	NULL;		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltCommand::~cltCommand()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltCommand::Init(SOWorld *pmyworld)
{
	pMyWorld	=	pmyworld;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 錶Щ 衛鑑棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::Warp(UI16 uiPlayerID, UI16 uiMapID, SI32 siX, SI32 siY)
{
	SOPlayer	*pPlayer;
	CHAR		szGMString[1024];

	ZeroMemory(szGMString, sizeof(szGMString));
	

	// ⑷營朝 跨蝶攪諦 餌塋虜 錶Щ蒂 衛鑒 熱 氈棻.
	if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// 跨蝶攪檜棻.
		sprintf(szGMString, "[%s] [%d] [%d] [%d]", pMyWorld->clSOCharKI.GetNameByString(pMyWorld->clMonster.GetKind(uiPlayerID)), 
													uiMapID, siX, siY);
		strcat(m_szGMString, szGMString);
		WriteStringToFile();		

		return pMyWorld->clMonster.Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(ISPLAYER(uiPlayerID) == TRUE)
	{
		// 餌塋檜棻.
		// 餌塋曖 唳辦縑朝 億煎遴 夠戲煎 錶Щ ж艘擊 唳辦 輿嬪曖 �秣磍� 棻衛 嫡嬴撿 ж晦 陽僥縑
		// 瞰離陛 棻模 犒濩ж棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪蒂 儅撩и棻.
//	熱薑 橾濠	:	2002-12-10 - 嶸營��
//	>cm M01 100
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	if ( BYTE(pszMonsterName[0]) > 127 ) // и旋
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
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曖 嬴檜蛤煎 跨蝶攪蒂 儅撩и棻
//	熱薑 橾濠	:	2004-12-2 螃�� 2:01 - 譆煽輿
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 絲擊 褻薑и棻.
//	熱薑 橾濠	:	2002-10-24 螃�� 2:27:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GiveMoney(UI16 uiPlayerID, MONEY Money)
{
	SOPlayer		*pPlayer;	
	CHAR			szGMString[1024];

	// 餌辨濠陛 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];	
	if(pPlayer == NULL)	return FALSE;

	MONEY tempMoney = pPlayer->siMoney;

	pPlayer->GiveBonusMoney( Money, ON_RET_CHNGEMONEY_FROMADMIN ,FALSE);

	DBIKGiveMoneyByAdmin DBInfo;

	DBInfo.siDBAccount = pPlayer->stAccountInfo.siDBAccount;
	DBInfo.uiDBSlot = pPlayer->stAccountInfo.uiSlot;
	// 滲�風挫� 晦煙.
	DBInfo.moMoney = pPlayer->siMoney - tempMoney;

	pPlayer->pMyWorld->clDatabase->writeDBQuery(SO_DB_INFOKIND_GIVEMONEYBYADMIN,&DBInfo,sizeof(DBInfo));
	
	sprintf(szGMString, "[%s] [%I64d]", pPlayer->szID, Money );
	strcat(m_szGMString, szGMString);
	WriteStringToFile();		

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嬴檜蠱擊 階縑 幗溥遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// 嬴檜蠱 熱榆檜 0橾 唳辦 FALSE蒂 葬欐и棻.
	if(uiItemQuantity == 0) return FALSE;

	// 襄營ж朝 嬴檜蠱檜 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if((pItemHeader = pMyWorld->clItem.GetItem(pszItemName)) == NULL) return FALSE;
	
	// GM擊 晦遽戲煎 嬴檜蠱擊 幗萼棻.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// 嬴檜蠱擊 幗萵 熱 氈朝 濠葬陛 氈朝雖 っ欽и棻.
	// ---- 彌艙渠 - 20050304 熱薑
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// 嬴檜蠱擊 褕擊 熱 氈朝 謝ル蒂 瓊雖 跤ц棻.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// 嬴檜蠱擊 雲橫嗑萵 嬪纂蒂 瓊懊棻.
	// 斜 嬴檜蠱擊 雲橫嗑葬朝 嬪纂蒂 晦遽戲煎 輿嬪曖 議葛攪縑啪 嬴檜蠱擊 雲橫嗑溜棻堅 爾頂遽棻.
	// 罹晦憮 歜衛煎 詭衛雖蒂 虜菟橫遽棻.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// 裘縑 斜 嬴檜蠱擊 撲薑и棻.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;


	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;
	//Msg.uiItemID = pItemHeader->m_uiID;
	//Msg.uiItemQuantity = uiItemQuantity;

	// 輿曖: 晦襄 嬴檜蠱婁 熱榆檜 м藥螺擊 熱紫 氈戲嘎煎 譆謙 熱榆擊 棻衛 橢橫螞棻.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;

	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);



	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	/*pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_MEANDNEARCHAR, LPVOID( ptThrowXY.x ), LPVOID( ptThrowXY.y ), 
							LPVOID( ReqThrowItemMsg.uiItemID ), LPVOID( ReqThrowItemMsg.uiQuantity ) );*/

	// DB縑 盪濰и棻.
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

	// 嬴檜蠱 熱榆檜 0橾 唳辦 FALSE蒂 葬欐и棻.
	if(uiItemQuantity == 0) return FALSE;

	// 襄營ж朝 嬴檜蠱檜 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if((pItemHeader = pMyWorld->clItem.GetItem(uiItemID)) == NULL) return FALSE;
	
	// GM擊 晦遽戲煎 嬴檜蠱擊 幗萼棻.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL) return FALSE;

	uiMapID				=	pPlayer->uiMapID;
	siX					=	pPlayer->siX;
	siY					=	pPlayer->siY;
	uiMoveableTileInfo	=	pPlayer->uiMoveableTileInfo;

	// 嬴檜蠱擊 幗萵 熱 氈朝 濠葬陛 氈朝雖 っ欽и棻.
	// ---- 彌艙渠 - 20050304 熱薑
	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID, uiItemQuantity );
	// ----
//	ptThrowXY	=	pMyWorld->clISOMap.SearchMap( uiMapID, siX, siY, ON_MAP_SI_ITEM, pItemHeader->m_uiID );

	// 嬴檜蠱擊 褕擊 熱 氈朝 謝ル蒂 瓊雖 跤ц棻.
	if(ptThrowXY.x == -1 && ptThrowXY.y == -1)	return FALSE;		

	// 嬴檜蠱擊 雲橫嗑萵 嬪纂蒂 瓊懊棻.
	// 斜 嬴檜蠱擊 雲橫嗑葬朝 嬪纂蒂 晦遽戲煎 輿嬪曖 議葛攪縑啪 嬴檜蠱擊 雲橫嗑溜棻堅 爾頂遽棻.
	// 罹晦憮 歜衛煎 詭衛雖蒂 虜菟橫遽棻.
	ReqThrowItemMsg.uiItemID	=	pItemHeader->m_uiID;
	ReqThrowItemMsg.uiQuantity	=	uiItemQuantity;
	
	// 裘縑 斜 嬴檜蠱擊 撲薑и棻.	
	pMyWorld->clISOMap.SetItem( uiMapID, ptThrowXY.x, ptThrowXY.y, ReqThrowItemMsg.uiItemID, uiItemQuantity, FALSE );	

	OnChangeFieldItemMsg Msg;
	Msg.dwMsgType = ON_CHANGE_FIELDITEM;
	Msg.siX = ptThrowXY.x;
	Msg.siY = ptThrowXY.y;

	// 輿曖: 晦襄 嬴檜蠱婁 熱榆檜 м藥螺擊 熱紫 氈戲嘎煎 譆謙 熱榆擊 棻衛 橢橫螞棻.
	pMyWorld->clISOMap.GetItemID( uiMapID, ptThrowXY.x, ptThrowXY.y, &Msg.uiItemID, &Msg.uiItemQuantity );

	UI16 uiCharUniIDs[ON_STATUS_AREA_XSIZE * ON_STATUS_AREA_YSIZE];


	SI32 siSendCharNum;
	siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList(uiMapID, Msg.siX, Msg.siY, uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
	pPlayer->SendFieldMsg( ON_CHANGE_FIELDITEM, SO_SFM_CUSTOM, LPVOID( siSendCharNum ), LPVOID( uiCharUniIDs ), 
							LPVOID( &Msg), NULL );

	// DB縑 盪濰и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 蹺寞и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::LogOut(UI16 uiPlayerID)
{
	SOPlayer				*pPlayer;

	// 跨蝶攪曖 唳辦縑紫 '煎斜嬴醒'檜 脹棻.
	// 跨蝶攪曖 唳辦縑朝 避朝棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	奢雖餌о.
//	熱薑 橾濠	:	2002-03-26 螃�� 3:31:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::Notice(CHAR *pszNotice)
{
	if(pszNotice == NULL) return FALSE;

	// 賅萇 餌辨濠縑啪 奢雖 詭衛雖蒂 爾魚棻.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_TEXT, LPARAM(pszNotice));	
}

BOOL cltCommand::SendSMSMsg(SMS_REQ_MSG* pSMSMsg)
{
	if(pSMSMsg == NULL) return FALSE;

	// 賅萇 餌辨濠縑啪 奢雖 詭衛雖蒂 爾魚棻.
	return pMyWorld->OfficialAnnouncement(ON_RET_OA_SMS_MSG, 
		LPARAM(pSMSMsg->dwCallCount), 
		LPARAM(pSMSMsg->szUserCode), 
		LPARAM(pSMSMsg->szOrgPhone), 
		LPARAM(pSMSMsg->szDstPhone), 
		LPARAM(pSMSMsg->szCallMessage), 
		LPARAM(pSMSMsg->szRecvTime), 
		LPARAM(pSMSMsg->szReserved));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨濠縑啪 敏樓蜓擊 爾魚棻.
//	熱薑 橾濠	:	2002-03-22 螃瞪 10:01:56 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::Whisper(UI16 uiPlayerID, CHAR *pszID, CHAR *pText)
{
	SOPlayer	*pPlayer;	
	SOPlayer	*pOpponentPlayer;
	CHAR		szWhipser[1024];

	if(ISPLAYER(uiPlayerID) != TRUE)	return	FALSE;	
	if((pPlayer	= pMyWorld->pPlayerOnAccount[uiPlayerID]) == NULL)	return	FALSE;
		
	if((pOpponentPlayer	=	pMyWorld->GetPlayer(pszID)) != NULL)
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "敏樓蜓擊 撩奢瞳戲煎 瞪歎ц蝗棲棻.");

		sprintf(szWhipser, "[%s] : %s", pPlayer->szID, pText);
		pOpponentPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), szWhipser);
	}
	else
	{
		pPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), "敏樓蜓擊 瞪歎ж朝等 褒ぬц蝗棲棻.");
	}

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	望萄 奢雖餌о.
//	熱薑 橾濠	:	2002-03-26 螃�� 3:33:09 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GuildNotice(UI32 uiGuildID, CHAR *pszName, CHAR *pszNotice)
{
	if(uiGuildID == 0)		return	FALSE;
	if(pszNotice == NULL)	return	FALSE;	

	// 望萄錳縑啪 詭衛雖蒂 爾魚棻.
	return pMyWorld->clIGuild.Notice(uiGuildID, pszName, pszNotice);	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餌辨濠曖 薑爾蒂 橢橫螞棻.
//				餌辨濠曖 薑爾(絲, 謝ル)蛔擊 橢橫諦憮 遴艙濠縑啪 爾頂遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	
	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	ZeroMemory(szPlayerInfo, sizeof(szPlayerInfo));	

	// 餌辨濠橾陽諦 跨蝶攪橾陽諦 棻腦啪 籀葬и棻.
	if(ISPLAYER(uiPlayerID) == TRUE)
	{
		pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
		if(pPlayer == NULL) return FALSE;

		// 舒 ID陛 賅舒 嶸�褲炴�.

		// Ы溯檜橫曖 ID蒂 橢橫螞棻.
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
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 葆擊 :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 葆擊 :[%s], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "憲 熱 橈朝 葆擊", pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Pos :[%d : %d, %d], 瞪癱醞..., State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		case ON_PLAYER_STATUS_INFIELD | ON_PLAYER_STATUS_INVILLAGE:
			if(pPlayer->stAccountInfo.pVillage != NULL)		pszVillageName	=	pMyWorld->clOnlineText.Get(pPlayer->stAccountInfo.pVillage->uiVillageName);
			else											pszVillageName	=	NULL;		

			if(pszVillageName != NULL)
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 葆擊 :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pszVillageName, 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			else
				sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], 葆擊 :[%s], Pos :[%d : %d, %d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, "憲 熱 橈朝 葆擊", 
									pPlayer->uiMapID, pPlayer->siX, pPlayer->siY, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		default:
			sprintf(szPlayerInfo, "ID : [%s], Money : [%I64u], Unknown Status... [%d], State : [%d], UserGrade : [ %d ], MaxGeneral : [ %d ]", szID, pPlayer->siMoney, pPlayer->siStatus, pPlayer->uiState, pPlayer->USGrade, pPlayer->m_siMaxGeneral );
			break;
		}
	}
	else if(ISMONSTER(uiPlayerID) == TRUE)
	{
		// 嶸�褲� 跨蝶攪檣雖 憲嬴螞棻.
		if(pMyWorld->clMonster.IsValidID(uiPlayerID) == FALSE)	return	FALSE;
			
		// 跨蝶攪曖 謙盟蒂 橢橫螞棻.
		uiMonsterKind	=	pMyWorld->clMonster.GetKind(uiPlayerID);

		// 跨蝶攪曖 檜葷擊 橢橫螞棻.
		ZeroMemory(szID, sizeof(szID));
		pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind);
		if(pszMonsterName != NULL)
		{
			strncpy(szID, pszMonsterName, ON_ID_LENGTH);
		}
		else
		{
			strncpy(szID, "跨蝶攪", ON_ID_LENGTH);
		}
		
		// 跨蝶攪曖 嬪纂蒂 橢橫螞棻.
		pMyWorld->clMonster.GetPos(uiPlayerID, &uiMapIndex, &ptMonsterPos);

		siFieldID	=	pMyWorld->clMonster.GetFieldID(uiPlayerID);

		switch(pMyWorld->clMonster.GetStatus(uiPlayerID))
		{
		case ON_PLAYER_STATUS_INFIELD:
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d]", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		case ON_PLAYER_STATUS_INBATTLE:		
			sprintf(szPlayerInfo, "ID : [%s], FID : [%d], Pos :[%d : %d, %d], 瞪癱醞...", szID, siFieldID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;		
		default:
			sprintf(szPlayerInfo, "ID : [%s], Unknown Status...", szID, uiMapIndex, ptMonsterPos.x, ptMonsterPos.y);
			break;
		}
	}
	else
	{
		sprintf(szPlayerInfo, "憲 熱 橈朝 議葛攪 謙盟殮棲棻");
	}

	// 遴艙濠縑啪 爾頂遽棻.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szPlayerInfo));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餌辨濠曖 僥濠翮 ID蒂 餌辨ж罹 璋濠 Account蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 蕾樓腎橫 氈朝 餌辨濠菟曖 薑爾蒂 掘и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetConnectUsersInfo(UI16 uiGMPlayerID)
{
	CHAR		szInfo[1024] = "";
	SOPlayer	*pGMPlayer;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	wsprintf(szInfo, "譆渠 翕衛 蕾樓濠 熱 : [%d], ⑷營 蕾樓濠 熱 : [%d]", pMyWorld->clGate->siMaxConnectedPlayer, pMyWorld->clGate->siTotalConnectPlayer);
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szInfo));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 憮幗曖 蟾渡 Щ溯歜 熱蒂 掘и棻.
//	熱薑 橾濠	:	2002-07-08 螃瞪 3:19:41 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	爾傘蝶蒂 隸陛 衛麵遽棻.
//	熱薑 橾濠	:	2002-08-12 螃�� 2:27:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::BonusUp(UI16 uiPlayerID, UI08 uiFollowerID)
{
	// 餌辨濠陛 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 爾傘蝶蒂 Up п遽棻.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseBonus(uiFollowerID, 5);	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	褐辨紫蒂 隸陛 衛鑑棻.
//	熱薑 橾濠	:	2002-08-12 螃�� 2:21:20 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::CreditUp(UI16 uiPlayerID, SI32 siCredit)
{
	// 餌辨濠陛 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 爾傘蝶蒂 Up п遽棻.
	SOPlayer	*pPlayer;	

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	return pPlayer->IncreaseCredit(siCredit);	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	唳я纂蒂 隸陛 衛鑑棻.
//	熱薑 橾濠	:	2002-09-26 螃�� 5:28:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::ExpUp( UI16 uiPlayerID, UI08 uiFollowerID, UI32 uiExp )
{
	// 餌辨濠陛 嬴棍 唳辦縑朝 FALSE蒂 葬欐и棻.
	if(ISPLAYER(uiPlayerID) != TRUE) return FALSE;

	// 唳я纂蒂 隸陛衛麵遽棻.
	SOPlayer	*pPlayer;	

	if( ( pPlayer =	pMyWorld->GetPlayer( uiPlayerID ) ) == NULL)		return	FALSE;
	
	return pPlayer->expUp( uiFollowerID, uiExp );	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	餌辨濠曖 E-Mail擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-02-20 螃�� 3:35:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetUserMailAddress(UI16 uiGMPlayerID, UI16 uiPlayerID)
{
	SOPlayer	*pGMPlayer;
	SOPlayer	*pPlayer;
	CHAR		szCharName[ON_ID_LENGTH + 1];
	CHAR		szUserMailAddress[1024];
	CHAR		szBuffer[1024];

	// 餌辨濠陛 嬴棲賊 FALSE蒂 葬欐и棻.
	if(ISPLAYER(uiPlayerID)	!= TRUE)	return FALSE;

	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL)	return FALSE;

	pPlayer			=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL)		return FALSE;
	
	ZeroMemory(szCharName, sizeof(szCharName));
	strncpy(szCharName, pPlayer->szID, ON_ID_LENGTH);

	// 餌辨濠曖 E-Mail擊 橢橫螞棻.
	ZeroMemory(szUserMailAddress, sizeof(szUserMailAddress));
	pMyWorld->clDatabase->GetDBInfo(SO_DB_INFOKIND_USERMAILADDRESS, LPVOID(szCharName), LPVOID(szUserMailAddress));
	
	wsprintf(szBuffer, "N:[%s], M:[%s]", szCharName, szUserMailAddress);

	// 遴艙濠縑啪 爾頂遽棻.
	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	貲滄擊 籀葬и棻.	
//	熱薑 橾濠	:	2002-03-26 螃�� 3:33:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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


	// GM檜 餌塋檜 嬴棲塭賊 FALSE
	if(ISPLAYER(uiGMPlayerID) != TRUE) return FALSE;

	// 嶸�褲� Ы溯檜橫檣雖 棻衛 и廓 匐餌и棻.
	pPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pPlayer == NULL)																return FALSE;

	// 貲滄橫蒂 橢橫螞棻.
	if((pToken = strtok(pszCMD, cSep)) == NULL) return FALSE;		

	// 貲滄曖 ん檣攪蒂 橢橫螞棻.
	// п渡 貲滄檜 橈剪釭 斜 貲滄擊 餌辨й 熱 氈朝 掏и檜 嬴棲賊 FALSE蒂 葬欐и棻.
	if((pstCommand = GetCommand(pToken, pPlayer->USGrade, pPlayer->siClassInGuild)) == NULL)	return	FALSE;		

	// m_szGMString蒂 蟾晦�� и棻.
	ZeroMemory(m_szGMString, sizeof(m_szGMString));

	// m_szGMString縑 遴艙濠曖 檜葷擊 厥朝棻.
	sprintf(m_szGMString, "[%s] : [%s] : ", pPlayer->szID, pToken);

	if(stricmp(pToken, m_stCommand[SO_COMMAND_WARP].szCommand) == 0)					// command 'w'
	{
		// uiPlayerID蒂 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// 錶Щ蒂 ж朝 寞徹擎 舒陛雖陛 氈棻.
		// 葆擊 檜葷擊 殮溘ж朝 寞徹婁
		// 裘 檣策蝶, 嬪纂蒂 殮溘ж朝 寞徹.		

		// Token擊 и廓 渦и棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		// 辦摹 偽擎 檜葷曖 葆擊檜 氈朝雖 憲嬴獄棻.
		pstWarpInfo	=	pMyWorld->m_clWarpParser.GetWarpInfo( pToken );

		if( pstWarpInfo != NULL )
		{
			uiMapID	=	pstWarpInfo->uiMapIndex;
			siX		=	pstWarpInfo->uiX;
			siY		=	pstWarpInfo->uiY;
		}
		else
		{
			// 葆擊 檜葷擊 殮溘и 匙檜 嬴棲棻.

			uiMapID		=	atoi(pToken);

			// siX蒂 橢橫螞棻.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siX			=	atoi(pToken);

			// siY蒂 橢橫螞棻.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			siY			=	atoi(pToken);
		}

		return Warp(uiPlayerID, uiMapID, siX, siY);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_CREATEMONSTER].szCommand) == 0)		// command 'cm'
	{
/*		// 跨蝶攪曖 檜葷擊 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
		strcpy(uiMonsterName, pToken);

		if((pToken	= strtok(NULL, cSep)) == NULL) 
			uiMonsterQuantity = 10;
		else
			uiMonsterQuantity = atoi(pToken);
		if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

		return CreateMonster(uiGMPlayerID, uiMonsterName, 1, uiMonsterQuantity);*/
		//2004-12-09熱薑 - 譆煽輿
		int nRet = FindChar(pszCMD, 39); //僥濠翮縑 '陛 氈朝雖 匐餌и棻.
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0: //囀萄寞衝
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				
				DWORD dwCode = 0;
				dwCode = ConvertCode(pToken, strlen(pToken));
				
				if(dwCode < 0) return FALSE;
				else if(dwCode == 0)
					dwCode = atoi(pToken); //跨蝶攪曖 囀萄
				
				//偃熱
				if((pToken	= strtok(NULL, cSep)) == NULL) 
					uiMonsterQuantity = 10;
				else
					uiMonsterQuantity = atoi(pToken);
			
				if ( uiMonsterQuantity > 100 ) uiMonsterQuantity=100;

				return CreateMonsterID(uiGMPlayerID, dwCode, 1, uiMonsterQuantity);
			}
			break;

		case 1: //檜葷 寞衝
			{
				if((pToken	= strtok(NULL, cSep2)) == NULL) return FALSE;
				ZeroMemory(uiMonsterName, sizeof(uiMonsterName));
				strcpy(uiMonsterName, pToken); //跨蝶攪曖 檜葷
            
				//偃熱
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
		// uiPlayerID蒂 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID	=	atoi(pToken);

		// Money蒂 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		Money		=	atoi(pToken);

		return GiveMoney(uiPlayerID, Money);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GIVEITEM].szCommand) == 0)			// command 'givei'
	{
/*		// 嬴檜蠱 檜葷擊 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
		ZeroMemory(szItemName, sizeof(szItemName));
		strcpy(szItemName, pToken);

		// uiItemQuantity蒂 橢橫螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL)	return FALSE;
		if( atoi(pToken) < 0 )						return FALSE;
		uiItemQuantity	=	atoi(pToken);

		return GiveItem(uiGMPlayerID, szItemName, uiItemQuantity); */
		//2004-12-09熱薑 - 譆煽輿
		int nRet = FindChar(pszCMD, 39);
		if(nRet < 0) return FALSE;

		switch(nRet)
		{
		case 0:
			{
				if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
				UI16 uiItemId = atoi(pToken);
				if(uiItemId < 0) return FALSE;

				// uiItemQuantity蒂 橢橫螞棻.
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

				// uiItemQuantity蒂 橢橫螞棻.
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
		// uiPlayerID蒂 橢橫螞棻.
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
		// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
		if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;

		if(stricmp(pToken, "C") == 0)
		{
			// 餌辨濠曖 薑爾蒂 橢戲溥堅 и棻.

			// uiPlayerID蒂 橢橫螞棻.
			if((pToken	= strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID	=	atoi(pToken);

			return GetPlayerInfo(uiGMPlayerID, uiPlayerID);
		}		
		else if(stricmp(pToken, "I") == 0)
		{
			// Account煎 滲�納� ID蒂 橢橫螞棻.
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
			// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiPlayerID		=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			uiFollowerID	=	atoi(pToken);

			return GetFollowerWeightInfo(uiGMPlayerID, uiPlayerID, uiFollowerID);
		}
		else if(stricmp(pToken, "TI") == 0)
		{
			// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siX				=	atoi(pToken);

			if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
			siY				=	atoi(pToken);

			return GetTileInfo(uiGMPlayerID, siX, siY);
		}
		else if(stricmp(pToken, "DBACTOAC") == 0)
		{
			// DB Account蒂 檜辨ж罹 議葛攪曖 Account蒂 憲嬴螞棻.
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
		// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiFollowerID	=	atoi(pToken);

		return BonusUp(uiPlayerID, uiFollowerID);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_GNOTICE].szCommand) == 0)
	{
		// 望萄錳縑啪 奢雖蒂 爾魚棻.
		if((pToken	= strtok(NULL, "")) == NULL) return FALSE;
		return GuildNotice(pPlayer->uiGuildID, pPlayer->szID, pToken);
	}
	else if(stricmp(pToken, m_stCommand[SO_COMMAND_WHISPER].szCommand) == 0)
	{
		/*
		// か薑 Ы溯檜橫縑啪 敏樓蜓擊 爾魚棻.
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
		// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiPlayerID		=	atoi(pToken);

		if((pToken		=	strtok(NULL, cSep)) == NULL) return FALSE;
		uiUpCredit		=	atoi(pToken);

		return CreditUp(uiPlayerID, uiUpCredit);
	}
	else if( stricmp(pToken, m_stCommand[ SO_COMMAND_EXPUP ].szCommand) == 0 )
	{
		// 橫雯匙曖 薑爾蒂 橢朝雖 憲嬴螞棻.
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
		// 橫雯匙曖 蠡蝶お蒂 諫猿ж朝雖 憲嬴螞棻.
		if( ( pToken = strtok( NULL, cSep ) ) == NULL )				return FALSE;		

		if( stricmp( pToken, "MONSTER" ) == 0 )
		{			
			if( ( pToken = strtok( NULL, cSep ) ) == NULL )			return FALSE;

			// 跨蝶攪曖 檜葷
			ZeroMemory( szID, sizeof( szID ) );
			strncpy( szID, pToken, ON_ID_LENGTH );

			// 跨蝶攪曖 熱
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
		// だじ錳菟縑啪 奢雖蒂 爾魚棻.
		if((pToken	= strtok(NULL, "")) == NULL)					return FALSE;

		return PartyChat( pPlayer, pToken);		
	}


	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	�倣狤� 盪濰и棻.
//	熱薑 橾濠	:	2002-02-20 螃瞪 10:12:18 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltCommand::WriteStringToFile()
{

	writeInfoToFile("CommandLog.txt", "%s",  m_szGMString);		
	// �倣狨� 儅撩и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	貲滄擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-27 螃瞪 9:19:16 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	FieldMonster曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-17 螃瞪 10:57:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetFieldMonsterInfo(UI16 uiGMPlayerID, SI32 siFieldMonsterID)
{
	BOOL		bSuccess;
	CHAR		szBuffer[1024];
	SOPlayer	*pGMPlayer;

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	// FieldMonster曖 薑爾蒂 橢橫螞棻.
	bSuccess	=	pMyWorld->clFieldMonster.GetInfo(siFieldMonsterID, szBuffer);

	if(bSuccess == TRUE)
	{
		return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
	}
	else
		return	FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Follower曖 鼠啪 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-12 螃�� 3:37:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetFollowerWeightInfo(UI16 uiGMPlayerID, UI16 uiPlayerID, UI08 uiFollowerID)
{
	SOPlayer	*pGMPlayer;	
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;

	pPlayer		=	pMyWorld->pPlayerOnAccount[uiPlayerID];
	if(pPlayer == NULL) return FALSE;

	if(pPlayer->IFollower.IsValidFollower(uiFollowerID) == FALSE)	return	FALSE;
	
	wsprintf(szBuffer, "Follower Weight Info : [%d], [%d], [%d]", uiPlayerID, uiFollowerID, pPlayer->IFollower.GetTotalItemWeight(uiFollowerID));

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	顫橾曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-18 螃�� 3:25:24 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetTileInfo(UI16 uiGMPlayerID, SI32 siX, SI32 siY)
{
	SOPlayer	*pGMPlayer;		
	CHAR		szBuffer[1024];	

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	ZeroMemory(szBuffer, sizeof(szBuffer));
	if(pMyWorld->clISOMap.GetTileInfo(pGMPlayer->uiMapID, siX, siY, szBuffer) == FALSE)	return	FALSE;

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	DB Account蒂 檜辨ж罹 Ы溯檜橫曖 Account蒂 檜辨и棻.
//	熱薑 橾濠	:	2002-08-12 螃�� 1:26:10 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltCommand::GetPlayerAccountByDBAccount(UI16 uiGMPlayerID, DWORD dwDBAccount)
{	
	SOPlayer	*pGMPlayer;		
	SOPlayer	*pPlayer;		
	CHAR		szBuffer[1024];	

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
	pGMPlayer		=	pMyWorld->pPlayerOnAccount[uiGMPlayerID];
	if(pGMPlayer == NULL) return FALSE;	

	// 渠鼻 Ы溯檜橫曖 Account蒂 橢橫螞棻.
	pPlayer			=	pMyWorld->GetPlayerByDBAccount(dwDBAccount);
	if(pPlayer == NULL)	return	FALSE;
	
	wsprintf(szBuffer, "DBAccount : [%u], Account : [%u]", pPlayer->stAccountInfo.siDBAccount, pPlayer->uiAccount);

	return pGMPlayer->SendFieldMsg(ON_TEXT_MSG, SO_SFM_ONLYME, LPVOID(ON_RET_TEXTMSG_WHISPER), LPVOID(szBuffer));		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	輿嬪縑 跨蝶攪菟擊 避檣棻.
//	熱薑 橾濠	:	2002-12-17 螃瞪 10:49:07 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	// п渡 Ы溯檜橫曖 薑爾蒂 橢橫螞棻.
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

	// 詭衛雖蒂 撮たи棻.
	ChangeFieldItemMsg.dwMsgType		=	ON_CHANGE_FIELDITEM;

	siFieldItemNum	=	pMyWorld->clISOMap.GetNearSectorItemList( pGMPlayer->uiMapID, pGMPlayer->siX, pGMPlayer->siY, FieldItemInfo );

	for( i = 0; i < siFieldItemNum; i++ )
	{	
		pFieldItem		=	&FieldItemInfo[ i ];		

		// в萄縑憮 嬴檜蠱擊 薯剪и棻.
		if( pMyWorld->clISOMap.DelItem( pGMPlayer, pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY ) == TRUE )
		{
			
			ChangeFieldItemMsg.siX				=	pFieldItem->siX;
			ChangeFieldItemMsg.siY				=	pFieldItem->siY;
			ChangeFieldItemMsg.uiItemID			=	0;
			ChangeFieldItemMsg.uiItemQuantity	=	0;			

			siSendCharNum	=	pMyWorld->clISOMap.GetNearSectorPlayerList( pGMPlayer->uiMapID, pFieldItem->siX, pFieldItem->siY, 
													uiCharUniIDs, ON_MAP_CHARID_ALLPLAYER);
			
			// 棻艇 嶸盪菟縑啪 嬴檜蠱檜 薯剪腎歷棻堅 憲溥遽棻.
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
	// だじ錳菟縑啪 
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

//2004-12-13 殮溘嫡擎 僥濠翮擊 DWORD⑽戲煎 滲�� и棻. - 譆煽輿
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