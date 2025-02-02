#include "SOMain.h"
#include "SOWorld.h"

#include "SOMonster.h"
#include "SORand.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
Monster::Monster()
{
	siX					=	0;
	siY					=	0;

	uiKind				=	0;
	uiAccount			=	0;	
	uiGuildID			=	0;
	siClassInGuild		=	0;


	// 鼻鷓 蟾晦�� 
	//----------------------------------------------------------------------------------------
	
	// 鼻鷓蒂 蛔煙 и棻 
	CState					state;
	state.setState( NORMAL_MOVING );
	state.addTransitionState( SEEN_WEAK_ENEMY, FOLLOW_MOVING );
	state.addTransitionState( SEEN_STRONG_ENEMY, AVOID_MOVING );

	addState(state);
	//----------------------------------------------------------------------------------------
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
Monster::~Monster()
{	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��
//	熱薑 橾濠	:	2002-06-29 螃瞪 10:23:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::Init()
{
	// 蟾晦�飛� и棻.
	BaseChar::Init();

	siFieldMonsterID	=	0;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蟾晦��.
//	熱薑 橾濠	:	2002-06-20 螃�� 1:34:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::Init(SI32 sifieldmonsterid, UI16 uiaccount, UI16 uimapid, SI16 six, SI16 siy, LPRECT prcactivityarea, UI16 uiRepresentKind, UI16 uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER], SI32 siMonsterLevel[ON_MAX_CHAR_PER_PLAYER])
{	
	if( ( ISMONSTER( uiaccount ) == FALSE ) || ( uiRepresentKind == 0 ) ) 
	{

//		pMyWorld->clServer->WriteInfo( "..\\MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );
		writeInfoToFile( "MonsterInitError.txt", "Account : [ %u ], Kind : [ %u ] ",  uiaccount, uiRepresentKind );		
	}


	// 蟾晦�� 
	normalState();

	SI32					i, j;
	UI16					uiMoveSpeed;
	OnFollowerInfo			FollowerInfo;
	UI16					uiMonsterKind;	
	_ImportantParameta		Parameter;		
	CharHead*				lpCharHead;	
	OnlineFollower			*pFollower;
	SI16					siDropItem[4];
	SI32					siDropPercent[4];

	// 晦獄 瞳檣 
	CHAR	*pszMonsterName	=	pMyWorld->clSOCharKI.GetNameByString( uiRepresentKind );

	BaseChar::Init( uiaccount, uimapid, six, siy, uiRepresentKind, pszMonsterName );	
	

	// 跨蝶攪曖 晦獄 滲熱蒂 蟾晦�� п遽棻.
	SETSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );
	//siStatus			=	ON_PLAYER_STATUS_INFIELD;
	siFieldMonsterID	=	sifieldmonsterid;			
	
	// �做� 艙羲擊 盪濰и棻.	
	prcActivityArea		=	prcactivityarea;				
	
	// 贗塭檜樹お曖 檜翕 樓紫蒂 憮幗縑憮曖 檜翕 雖翱 衛除戲煎 滲�納挬�.
	uiMoveSpeed			=	pMyWorld->clSOCharKI.GetMoveSpeed(uiRepresentKind);
	m_actionDelay		=	SO_CONVERSION_CHARMOVEPSEED(uiMoveSpeed);			
	m_prevActionTime	=   0;	

	// 跨蝶攪曖 棟溘纂蒂 撲薑и棻.
	IFollower.Init();
 
	for(i = 0; i < ON_MAX_CHAR_PER_PLAYER; i++)
	{
		uiMonsterKind	=	uiMonsterKinds[i];

		if(uiMonsterKind != 0)
		{			
			// 嶸�褲� 跨蝶攪檜棻.
			ZeroMemory(&FollowerInfo, sizeof(FollowerInfo));
			
			FollowerInfo.uiKind		=	uiMonsterKind;														// 跨蝶攪曖 ID.			

			strncpy(FollowerInfo.Name, pMyWorld->clSOCharKI.GetNameByString(uiMonsterKind), ON_ID_LENGTH);	// 跨蝶攪曖 檜葷.
		
			memcpy(&Parameter, pMyWorld->clSOCharKI.GetParameta(uiMonsterKind), sizeof(_ImportantParameta));
			lpCharHead = pMyWorld->GetCharHeader(FollowerInfo.uiKind);	
			
			Parameter.SetCurExp(GetRequiredExpForLevel(siMonsterLevel[i]));
			Parameter.SetLevel(siMonsterLevel[i]);

			Parameter.SetStr(Parameter.GetStr() + (siMonsterLevel[i] -1 ) );
			Parameter.SetDex(Parameter.GetDex() + (siMonsterLevel[i] -1 ) );
			Parameter.SetVit(Parameter.GetVit() + (siMonsterLevel[i] -1 ) );
			Parameter.SetInt(Parameter.GetInt() + (siMonsterLevel[i] -1 ) );		

			FollowerInfo.siExp					=	Parameter.GetCurExp();
			FollowerInfo.siLevel				=	Parameter.GetLevel();
			FollowerInfo.siStr					=	Parameter.GetStr();
			FollowerInfo.siDex					=	Parameter.GetDex();
			FollowerInfo.siVit					=	Parameter.GetVit();
			FollowerInfo.siInt					=	Parameter.GetInt();							
			FollowerInfo.siLife					=	Parameter.GetLife();
			FollowerInfo.siMana					=	Parameter.GetMana();
			FollowerInfo.siBonus				=	Parameter.GetBonus();
			FollowerInfo.siAC					=	Parameter.GetAC();
			FollowerInfo.siDamageRegist			=	Parameter.DamageResist;
			FollowerInfo.siMagicRegist			=	Parameter.MagicResist;
			
			FollowerInfo.siLiftConstant			=	lpCharHead->siCon;			

			FollowerInfo.siEquipMinDamage		=	Parameter.EquipMinDamage;
			FollowerInfo.siEquipMaxDamage		=	Parameter.EquipMaxDamage;

			// 睡ж菟擊 蟾晦�� и棻.
			IFollower.InitFollower(i, &FollowerInfo, lpCharHead->siNation, &pMyWorld->clItem);

			// 羹溘婁 葆徹溘擊 譆渠高戲煎 撲薑и棻.
			pFollower							=	IFollower.GetFollower(i);
			pFollower->ParametaBox.IP.SetLife(pFollower->ParametaBox.GetMaxLife());
			pFollower->ParametaBox.IP.SetMana(pFollower->ParametaBox.GetMaxMana(), 8);			

			// 跨蝶攪陛 避擊陽 雲橫嗑葬朝 嬴檜蠱擊 撲薑и棻.
			memcpy(siDropItem, pMyWorld->clSOCharKI.GetDropItem(FollowerInfo.uiKind), sizeof(UI16) * 4);
			memcpy( siDropPercent, pMyWorld->clSOCharKI.GetDropPercent( FollowerInfo.uiKind ), sizeof( SI32 ) * 4 );

			// 0. 嬴檜蠱擊 陛韓 �捕�煎 嬴檜蠱擊 檣漸縑 蹺陛и棻 
			//-----------------------------------------------------------------------------------------------
			for(j = 0; j < 3; j++)
			{
				if( ( lrand() % 100000000L) <= siDropPercent[j] )
				{		
					AddItem(i, siDropItem[j] , 1);
				}
			}		
		}
	}
	
	// 跨蝶攪蒂 裘縑 撲薑и棻.
	if(SetIDOnMap( 13 ) == FALSE)
		writeInfoToFile("SetIDOnMap Failed.txt", "3");	
//		pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "3");	

	if( ( uiAccount != uiaccount ) || ( uiKind != uiRepresentKind ) || ( uiKind == 0 ) ) 
	{
//		pMyWorld->clServer->WriteInfo( "..\\MonsterInitError.txt", "Account : [ %u, %u ], Kind : [ %u, %u ], Name : [ %d, %s ]",  
		writeInfoToFile( "MonsterInitError.txt", "Account : [ %u, %u ], Kind : [ %u, %u ], Name : [ %d, %s ]",  
						uiAccount, uiaccount, uiKind, uiRepresentKind, 
						( pszMonsterName != NULL ) ? strlen( pszMonsterName ) : 0, 
						( pszMonsterName != NULL ) ? pszMonsterName : "Null Point");	
	}

}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	п薯.
//	熱薑 橾濠	:	2002-05-11 螃�� 3:08:51 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::Free(BOOL IsDeadInBattle)
{
	// 跨蝶攪陛 避歷棻賊 ⑷營 濠晦 в萄縑啪 濠褐檜 避歷棻朝匙擊 憲溥遽棻.	
	if((siFieldMonsterID != 0) && (IsDeadInBattle == FALSE))
		pMyWorld->clFieldMonster.DeadMonster(siFieldMonsterID);

	// ⑷營 в萄縑 氈堅,
	// ⑷營 裘縑 濠晦陛 褕橫螳 氈棻賊 裘縑憮 ID蒂 餉薯 и棻.
	//if(siStatus == ON_PLAYER_STATUS_INFIELD)
	if( CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD ) == TRUE )
	{			
		// 裘縑憮 釭蒂 餉薯и棻.
		DelIDOnMap( 14 );
	}	

	Init();	
}	



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 ID陛 嶸�褲捘� 獄棻.(彰嬪蒂 慇橫釭雖朝 彊朝雖)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	Monster::IsValidCharID(UI08 uicharID)
{
	if(uicharID >= 0 && uicharID < ON_MAX_CHAR_PER_PLAYER)
		return TRUE;
	else
		return FALSE;
}


/*
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	⑷營 瞪癱醞檣雖 匐餌
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	Monster::GetfFight(void)
{
	if(siStatus == ON_PLAYER_STATUS_INBATTLE) return TRUE;
	return FALSE;
}
*/


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	跨蝶攪 議葛攪曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 12:58:03 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::GetInfo(OnCharMonsterInfo *pMonsterInfo)
{	
	BaseChar::GetInfo((OnCharBaseInfo*)pMonsterInfo);	
	
	// 跨蝶攪曖 蹺陛 薑爾蒂 橢橫螞棻.
	pMonsterInfo->siFightingPower = GetFightingPower();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	錶Щ蒂 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	Monster::Warp(UI16 uimapid, SI32 six, SI32 siy)
{
	UI16	uiPrevMapID;
	SI32	siPrevX, siPrevY;

	// 檜翕ж溥朝 裘檜 嶸�褲狫� 彊戲賊 FALSE蒂 葬欐и棻.
	if(pMyWorld->clISOMap.IsValidMapID(uimapid) == FALSE)	return FALSE;				

	// 錶Щ陛 褒ぬ й 唳辦蒂 渠綠п憮 ⑷營曖 裘 ID蒂 盪濰и棻.
	uiPrevMapID		=	uiMapID;	
	siPrevX			=	siX;
	siPrevY			=	siY;

	if(BaseChar::Warp(uimapid, six, siy) == TRUE)
	{		
		return TRUE;
	}
	else
	{
		// 億煎遴 雖薄戲煎 錶Щж朝等 褒ぬц棻賊
		// 錳楚 濠葬煎 棻衛 錶Щи棻.
		BaseChar::Warp(uiPrevMapID, siPrevX, siPrevY);
		return FALSE;
	}
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 遽綠и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 1:22:25 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::PrepareBattle()
{
	// 奩萄衛 瞪癱醞縑 氈橫撿 и棻.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "跨蝶攪陛 瞪癱縑 氈雖紫 彊擠 PrepareBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}

	// clFieldMonster縑啪 瞪癱 醞檜塭朝 匙擊 憲溥遽棻.
	pMyWorld->clFieldMonster.StartBattle(siFieldMonsterID);
		
	// 瞪癱蒂 遽綠и棻.
	BaseChar::PrepareBattle();
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 謙猿ц棻.
//	熱薑 橾濠	:	2002-05-10 螃�� 3:18:15 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	Monster::EndBattle()
{
	POINT		ptPos;

	// 奩萄衛 瞪癱醞縑 氈橫撿 и棻.
	if(GetBattle() == NULL)
	{		
		char	szbuffer[1024];
				
		sprintf(szbuffer, "跨蝶攪陛 瞪癱縑 氈雖紫 彊擠 EndBattle [%u]\n", uiAccount);		
		OutputDebugString(szbuffer);		
		return;
	}
	

	if( IFollower.GetFollowerNum() == 0)
	{
		Free(TRUE);
	}
	else
	{
		// clFieldMonster縑啪 瞪癱 謙猿 ц棻朝 匙擊 憲溥遽棻.
		pMyWorld->clFieldMonster.EndBattle(siFieldMonsterID);
		
		// 棻衛 в萄縑 撲薑ж晦 嬪п ⑷營 嬪纂蒂 晦遽戲煎 濠褐曖 ID蒂 撲薑й 嬪纂蒂 瓊朝棻
		//----------------------------------------------------------------------------------------------
		ptPos	=	pMyWorld->clISOMap.SearchMap(uiMapID, siX, siY, ON_MAP_SI_CHAR, uiMoveableTileInfo);
		
		if(ptPos.x != -1 && ptPos.y != -1)
		{
			siX	=	SI16(ptPos.x);
			siY	=	SI16(ptPos.y);
			
			// 裘縑棻陛 濠晦曖 嬴檜蛤蒂 撲薑и棻.		
			if(SetIDOnMap( 15 ) == FALSE)
				writeInfoToFile("SetIDOnMap Failed.txt", "4");
//				pMyWorld->clServer->WriteInfo("..\\SetIDOnMap Failed.txt", "4");
		}
		else
			writeInfoToFile("SearchMap Failed.txt", "1");
//			pMyWorld->clServer->WriteInfo("..\\SearchMap Failed.txt", "1");
		
		// 瞪癱蒂 謙猿 и棻 
		BaseChar::EndBattle();	


		// ゎ鼻衛 鼻鷓煎 給萼棻 
		normalState();
	}	
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	跨蝶攪陛 模樓п 氈朝 в萄曖 ID蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-17 螃瞪 9:47:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	Monster::GetFieldID()
{
	return	siFieldMonsterID;
}






//-----------------------------------------------------------------------------------------------------------------------
//
//										跨蝶攪 鼻鷓 籀葬 婦溼 睡碟 
//
//-----------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 跨蝶攪陛 ⑷營 в萄嬪縑 嬪纂ж堅 氈朝陛 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isMonsterInField()
{
	return CHECKSTATUS( siStatus, ON_PLAYER_STATUS_INFIELD );	
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 輿橫霞 嬪纂陛 濠褐曖 �做� 艙羲 寰縑 氈朝雖 匐餌 и棻 
//-----------------------------------------------------------------------------------------------------------------------
bool	Monster::isPosInMyArea(int x, int y)
{		
	POINT pos;	
	pos.x = x;	pos.y = y;
				
	// �做� 艙羲檜 NULL檣 唳辦 薯и脹 艙羲檜 橈擠 
	if( prcActivityArea == NULL )			return	TRUE;
	if( PtInRect(prcActivityArea, pos) )	return	TRUE;

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ⑷營 跡ル煎 鳴擎 瞳擊 覲嬴 棻擠 嬪纂蒂 橢朝棻
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getTargetNextPos()
{
	POINT nextPos;
	
	if( m_targetEnemy )
	{				
		nextPos.x = siX;
		nextPos.y = siY;

		if( m_targetEnemy->siX > siX )			nextPos.x  = siX + 1;
		else									nextPos.x  = siX - 1;

		if( m_targetEnemy->siY > siY )			nextPos.y  = siY + 1;
		else									nextPos.y  = siY - 1;		

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 虞�� 夠橾 唳辦 綠橫 氈朝 夠擊 瓊嬴 檜翕и棻 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// 渠鼻檜 橈擊 唳辦縑朝 楠渾и 檜翕擊 評艇棻 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ⑷營 跡ル煎 鳴擎 瞳擊 Яж罹 棻擠 嬪纂蒂 橢朝棻 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getAvoidNextPos()
{
	POINT nextPos;
	
	if( m_targetEnemy )
	{				
		nextPos.x = siX;
		nextPos.y = siY;

		if( m_targetEnemy->siX > siX )			nextPos.x  = siX - 1;
		else									nextPos.x  = siX + 1;

		if( m_targetEnemy->siY > siY )			nextPos.y  = siY - 1;
		else									nextPos.y  = siY + 1;		

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 虞�� 夠橾 唳辦 綠橫 氈朝 夠擊 瓊嬴 檜翕и棻 
		if( pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) 
			nextPos = getEmptyNextPos();
	}
	// 渠鼻檜 橈擊 唳辦縑朝 楠渾и 檜翕擊 評艇棻 
	else 
		nextPos = getRandomNextPos();

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 斬籀縑 綠橫 氈朝 嬪纂蒂 橢朝棻 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getEmptyNextPos()
{		
	// 50% �捕�煎 衛濛 雖薄擊 夥羞棻 
	int 	 entryPoint = rand() % 100 < 50 ? 7 : 0;
	int		 i = entryPoint;
	
	while( TRUE )
	{		
		// 寞щ擊 摹鷗и棻 
		int	direction;
		switch(i)				
		{							
			case 0: direction		=	ON_EAST;				break;	
			case 1:	direction		=	ON_EAST | ON_SOUTH;		break;			
			case 2:	direction		=	ON_SOUTH;				break;
			case 3:	direction		=	ON_WEST | ON_SOUTH;		break;
			case 4:	direction		=	ON_WEST;				break;
			case 5:	direction		=	ON_WEST | ON_NORTH;		break;
			case 6:	direction		=	ON_NORTH;				break;								
			case 7:	direction		=	ON_EAST | ON_NORTH;		break;
		}
								

		// 檜翕й 謝ル蒂 掘и棻 
		POINT  nextPos;		
		switch(direction)
		{
			case ON_WEST:		
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY;
				break;
			case ON_EAST:				
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY;
				break;
			case ON_NORTH:					
				nextPos.x	=	siX;			
				nextPos.y	=	siY - 1;
				break;
			case ON_SOUTH:
				nextPos.x	=	siX;
				nextPos.y	=	siY + 1;
				break;
			case ON_WEST | ON_NORTH:
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY - 1;
				break;
			case ON_WEST | ON_SOUTH:					
				nextPos.x	=	siX - 1;			
				nextPos.y	=	siY + 1;
				break;
			case ON_EAST | ON_NORTH:					
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY + 1;
				break;
			case ON_EAST | ON_SOUTH:					
				nextPos.x	=	siX + 1;			
				nextPos.y	=	siY - 1;
				break;
		}	

		if( nextPos.x < 0 ) nextPos.x = 0;
		if( nextPos.y < 0 ) nextPos.y = 0;

		// 虞�鷙� 彊擎 夠檜塭賊 醞雖 и棻 
		if( !pMyWorld->clISOMap.IsColl(uiMapID, nextPos.x, nextPos.y, uiMoveableTileInfo) ) return nextPos;		
		
		if( entryPoint == 0 ) if( ++i > 7 ) break;
		if( entryPoint == 7 ) if( --i < 0 ) break;
	}

	// 綠橫 氈朝 夠擊 瓊雖 跤ц棻賊 ⑷營 嬪纂蒂 葬欐и棻 
	POINT curPos;
	curPos.x = siX;
	curPos.y = siY;
	
	return curPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 楠渾ж啪 棻擠縑 檜翕й 嬪纂蒂 橢朝棻 
//-----------------------------------------------------------------------------------------------------------------------
POINT	Monster::getRandomNextPos()
{
	// 楠渾ж啪 檜翕й 寞щ擊 薑и棻 	
	int	direction;
	switch(rand() % 8)				
	{					
		case 0: direction		=	ON_EAST;				break;	
		case 1:	direction		=	ON_EAST | ON_SOUTH;		break;			
		case 2:	direction		=	ON_SOUTH;				break;
		case 3:	direction		=	ON_WEST | ON_SOUTH;		break;
		case 4:	direction		=	ON_WEST;				break;
		case 5:	direction		=	ON_WEST | ON_NORTH;		break;
		case 6:	direction		=	ON_NORTH;				break;								
		case 7:	direction		=	ON_EAST | ON_NORTH;		break;
	}
							
	// 檜翕й 謝ル蒂 掘и棻 
	POINT nextPos;	
	switch(direction)
	{
		case ON_WEST:		
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY;
			break;
		case ON_EAST:				
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY;
			break;
		case ON_NORTH:					
			nextPos.x	=	siX;			
			nextPos.y	=	siY - 1;
			break;
		case ON_SOUTH:
			nextPos.x	=	siX;
			nextPos.y	=	siY + 1;
			break;
		case ON_WEST | ON_NORTH:
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY - 1;
			break;
		case ON_WEST | ON_SOUTH:					
			nextPos.x	=	siX - 1;			
			nextPos.y	=	siY + 1;
			break;
		case ON_EAST | ON_NORTH:					
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY + 1;
			break;
		case ON_EAST | ON_SOUTH:					
			nextPos.x	=	siX + 1;			
			nextPos.y	=	siY - 1;
			break;
	}
		
	if( nextPos.x < 0 ) nextPos.x = 0;	
	if( nextPos.y < 0 ) nextPos.y = 0;

	return nextPos;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 輿嬪縑 瞳檜 氈朝雖 馬雖 и棻 
//-----------------------------------------------------------------------------------------------------------------------
SOPlayer* Monster::checkEnemyToArround()
{	
	WORD		 nearPlayerID;

	// 輿嬪 3蘊擊 匐儀 
	if( pMyWorld->clISOMap.GetCharUniID(uiMapID, 
									    siX - 3, siY - 3, siX + 3, siY + 3, 
									    &nearPlayerID, 1, ON_MAP_CHARID_PLAYER) >= 1 )
	{		
		SOPlayer *enemy = pMyWorld->pPlayerOnAccount[nearPlayerID];
			
		if( enemy )
		{
			// 瞳檜 瞪癱渠晦醞橾陽虜 奢問и棻 
			// 濠褐曖 艙羲寰縑 氈朝 瞳檣 唳辦縑虜 嶸�褲炴� 
			if( enemy->GetBattle() && isPosInMyArea( enemy->siX, enemy->siY ) ) return enemy;
		}
	}

	return NULL;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ч翕 雖翱 衛除擊 匐餌 и棻 
//-----------------------------------------------------------------------------------------------------------------------
bool Monster::checkActionDelay()
{
	if( (pMyWorld->dwtimeGetTime - m_prevActionTime) > m_actionDelay )
	{
		// ч翕 п撿 й 衛除檜棻 
		m_prevActionTime = pMyWorld->dwtimeGetTime;
		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ゎ鼻衛 鼻鷓煎 給萼棻 
//-----------------------------------------------------------------------------------------------------------------------
void	Monster::normalState()
{	
	m_targetEnemy = NULL;	
	setCurState( NORMAL_MOVING );
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 渠鼻戲煎 и 瞳擊 奢問и棻 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Attack()
{							
	if(m_targetEnemy)
	{
		// 斬蕾п 氈擊 唳辦 
		if(max( abs(siX - m_targetEnemy->siX), abs(siY - m_targetEnemy->siY)) <= 1)
		{	
			// 20%曖 �捕�煎 
			if( rand() % 100 <= 20 )
			{
				if( PickBattleWithEnemy(m_targetEnemy->uiAccount) )
				{					
					m_targetEnemy->SendFieldMsg(ON_RELEASEME, SO_SFM_ONLYME, LPVOID(uiAccount));
				}
			}
		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 褒薯煎 跨蝶攪曖 嬪纂蒂 檜翕и棻 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Move(POINT movePos)
{
	// 億煎遴 謝ル陛 檜翕 陛棟и 艙羲 橾唳辦 
	if( isPosInMyArea( movePos.x , movePos.y ) )
	{
		pMyWorld->clISOMap.MoveChar(this, movePos.x, movePos.y,FALSE, uiMoveableTileInfo);
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 瞳擊 場蟻擊 唳辦 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::lostEnemy()
{				
	if(m_targetEnemy)
	{
		// �飛橦� 替釵ル ル衛蒂 п薯 и棻 
		m_targetEnemy->SendFieldMsg(ON_RELEASEME, SO_SFM_ONLYME, LPVOID(uiAccount));			
		
		// ゎ鼻衛 鼻鷓煎 給萼棻 
		normalState();		
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 瞳擊 嫦唯 ц擊 唳辦 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::seenEnemy(SOPlayer* enemy)
{	
	if( enemy )
	{
		m_targetEnemy = enemy;		

		// 嫦唯и 鼻渠曖 瞪癱溘擊 髦Я堅 檜縑 蜃蹺橫 渠擬и棻 
		if( enemy->GetFightingPower() < GetFightingPower() * 2 )
		{			
			m_targetEnemy->SendFieldMsg(ON_TARGETME, SO_SFM_ONLYME, LPVOID(uiAccount));																		
			transitionState( SEEN_WEAK_ENEMY );		
		}
		else 
		{
			transitionState( SEEN_WEAK_ENEMY );		
		}
	}
}



//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 跨蝶攪 詭檣 瑞Щ 
//-----------------------------------------------------------------------------------------------------------------------
void Monster::Action()
{		
	if( GetBattle()			!= NULL  )		return;
	if( isMonsterInField()  == FALSE )		return;
	if( checkActionDelay()  == FALSE )		return;


	switch( getCurState() )
	{
		// 橾奩 檜翕 
		case NORMAL_MOVING:
			{
				// 檜翕 
				POINT nextPos = getRandomNextPos();								
				Move( nextPos );
				
				// 輿嬪蒂 匐餌и棻 
				SOPlayer* enemy;
				if( enemy = checkEnemyToArround() ) 
				{														
					seenEnemy(enemy);																			
				}						
			}
			break;

		// 蹺瞳 檜翕  
		case FOLLOW_MOVING:
			{	
				SOPlayer* enemy = checkEnemyToArround();
				
				// 輿嬪縑 瞳檜 橈剪釭 檜瞪縑 蹺瞳ж湍 瞳檜 嬴棍 唳辦 
				if( !enemy || enemy != m_targetEnemy )
				{					
					lostEnemy();
				}				
				else					
				{						
					POINT nextPos = getTargetNextPos();
					Move( nextPos );					

					Attack();
				}
			}
			break;		

		// �裔� 檜翕 
		case AVOID_MOVING:
			{
				// 橾欽 紫蜂纂堅 
				POINT nextPos = getAvoidNextPos();
				Move( nextPos );					

				// 楠渾 ж啪 輿嬪蒂 髦б棻 
				if( rand() % 100 < 30 )
				{
					SOPlayer* enemy = checkEnemyToArround();

					// 輿嬪縑 瞳檜 橈剪釭 檜瞪曖 瞳檜 嬴棍 唳辦 
					if( !enemy || enemy != m_targetEnemy )
					{					
						lostEnemy();
					}				
				}
			}
			break;
					
		default:
			break;
	}

}