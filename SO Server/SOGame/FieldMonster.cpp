#include "SOMain.h"
#include "FieldMonster.h"
#include "OnlineCGSHeader.h"
#include "ISOMonster.h"
#include "IOnlineMap.h"

IMonster	*cltFieldMonster::pIMonster		=	NULL;
ISOMap		*cltFieldMonster::pIMap			=	NULL;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltFieldMonster::cltFieldMonster()
{
	siID						=	0;
	uiMapID						=	0;
	siBaseX						=	0;
	siBaseY						=	0;
	siRadius					=	0;
	siMaxMonsterKindPerGroup	=	0;
	siMaxMonsterNum				=	0;
	siMinMonsterNum				=	0;
	dwPrevResurrectTime			=	0;
	
	ZeroMemory(FieldMonsterInfo, sizeof(FieldMonsterInfo));
	ZeroMemory(&rcActivityArea, sizeof(rcActivityArea));
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//	熱薑 橾濠	:	2002-02-27 螃�� 1:07:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::Init(SI32 siid, UI16 uimapid, SI32 six, SI32 siy, SI32 siradius, SI32 simaxmonsterkindpergroup, SI32 sigroupminquantity, SI32 sigroupmaxquantity, SI32 simaxmonsternum, SI32 siminmonsternum, stFieldMonsterInfo pFieldMonsterInfo[MAX_FIELD_MONSTER_KIND_PER_GROUP])
{	
	SI32						i;
	SI32						siMonsterCounter = 0;		
	SIZE						szMapSize;

	siID						=	siid;
	uiMapID						=	uimapid;
	siBaseX						=	six;
	siBaseY						=	siy;
	siRadius					=	siradius;
	siMaxMonsterKindPerGroup	=	simaxmonsterkindpergroup;
	siMaxMonsterNum				=	simaxmonsternum;
	siMinMonsterNum				=	siminmonsternum;	
	siMaxMonsterNumPerGroup		=	sigroupmaxquantity;
	siMinMonsterNumPerGroup		=	sigroupminquantity;

	szMapSize					=	pIMap->GetMapSize(uiMapID);

	rcActivityArea.left			=	max(siBaseX - siRadius, 0);
	rcActivityArea.top			=	max(siBaseY - siRadius, 0);
	rcActivityArea.right		=	min(siBaseX + siRadius, szMapSize.cx - 1);
	rcActivityArea.bottom		=	min(siBaseY + siRadius, szMapSize.cy - 1);
	
	memcpy(FieldMonsterInfo, pFieldMonsterInfo, sizeof(FieldMonsterInfo));

	// Total Rate蒂 掘и棻.
	siMaxMonsterKindNum		=	0;	
	for(i = 0; i < MAX_FIELD_MONSTER_KIND_PER_GROUP; i++)
	{
		if(FieldMonsterInfo[i].uiKind != 0)
		{
			uiSourceMonsterKindIndexTable[siMaxMonsterKindNum]	=	i;
			siMaxMonsterKindNum++;
		}
	}

	// 跨蝶攪 siMaxMonsterNum虜躑 雖紫縑 褕朝棻.	
	siCurrentFieldMonsterNum	=	0;
	for(i = 0; i < siMaxMonsterNum; i++)
	{	
		// 跨蝶攪蒂 儅撩и棻.
		CreateMonster();			
	}
	
	// 檜瞪縑 跨蝶攪蒂 睡�偷藷� 衛除擊 晦煙и棻.
	dwPrevResurrectTime	=	timeGetTime();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	虜菟 跨蝶攪曖 Kind蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-12-15 螃�� 12:51:13 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	cltFieldMonster::GetWillCreateMonsters( UI16 uiMonsterKinds[ ON_MAX_CHAR_PER_PLAYER ], SI32 siMonsterLevel[ ON_MAX_CHAR_PER_PLAYER ] )
{	
	stFieldMonsterInfo		*pstMonsterInfo;
	UI16					uiRandomlyIndex;
	SI32					siMonsterKindPerGroup, siMonsterKindNum;
	UI08					uiMonsterKindIndexTable[ MAX_FIELD_MONSTER_KIND_PER_GROUP ];
	UI08					uiSelectedMonsterKindIndex[ MAX_FIELD_MONSTER_KIND_PER_GROUP ];
	SI32					i, j;
	SI32					siMonsterCounter, siTempMonsterNum;
	UI16					uiRepresentKind, siRepresentKindNum;
	SI32					siMonsterNumPerGroup;
	SI32					siTotalRate;     

	//pIMonster->CheckInvalidChar( "Start cltFieldMonster::GetWillCreateMonsters()" );

	ZeroMemory( uiMonsterKinds, sizeof( UI16 ) * ON_MAX_CHAR_PER_PLAYER );
	ZeroMemory( siMonsterLevel, sizeof( SI32 ) * ON_MAX_CHAR_PER_PLAYER );	

	if( siMaxMonsterKindPerGroup == 0 || siMaxMonsterKindNum == 0 )
	{
		printf("siMaxMonsterKindPerGroup or(and) siMaxMonsterKindNum Equal Zero [%d, [%d\n", siMaxMonsterKindPerGroup, siMaxMonsterKindNum);
		return 0;
	}

	if( siMaxMonsterKindPerGroup > siMaxMonsterKindNum )
	{
		printf("siMaxMonsterKindPerGroup greater than siMaxMonsterKindNum, [%d] > [%d]\n", siMaxMonsterKindPerGroup, siMaxMonsterKindNum);
		return 0;
	}
	
	// siMaxMonsterKindPerGroup曖 跨蝶攪虜 蹺轎п 頂賊 脹棻.
	siMonsterKindPerGroup		=	siMaxMonsterKindPerGroup;

	// MonsterKind 陛 菟橫氈朝 Table擊 犒餌и棻.	
	siMonsterKindNum			=	siMaxMonsterKindNum;
	memcpy( uiMonsterKindIndexTable, uiSourceMonsterKindIndexTable, siMaxMonsterKindNum );	

	siTotalRate					=	0;		
	siMonsterNumPerGroup		=	rand() % ( siMaxMonsterNumPerGroup - siMinMonsterNumPerGroup + 1 ) + siMinMonsterNumPerGroup;		// и 跨蝶攪 斜瑜檜 陛雖朝 跨蝶攪曖 熱蒂 掘и棻.			
	siMonsterNumPerGroup		= siMonsterNumPerGroup < 1 ? siMinMonsterNumPerGroup : siMonsterNumPerGroup;

	// 辦摹 siMonsterKindPerGroup 虜躑 楠渾ж啪 跨蝶攪曖 Kind蒂 蹺轎и棻.
	for( i = 0; i < siMonsterKindPerGroup; i++ )
	{
		// 楠渾ж啪 ж釭曖 檣策蝶蒂 骯轎п魚棻.
		uiRandomlyIndex		=	rand() % siMonsterKindNum;

		pstMonsterInfo					=	&FieldMonsterInfo[ uiMonsterKindIndexTable[ uiRandomlyIndex ] ];		
		uiSelectedMonsterKindIndex[ i ]	=	uiMonsterKindIndexTable[ uiRandomlyIndex ];

		siTotalRate						+=	pstMonsterInfo->uiRate;		

		// 跨蝶攪 Table擊 棻衛 薑葬и棻.		
		memmove( &uiMonsterKindIndexTable[ uiRandomlyIndex ], &uiMonsterKindIndexTable[ uiRandomlyIndex + 1 ], ( siMonsterKindNum - ( uiRandomlyIndex + 1 ) ) );		

		// siMonsterKindNum擊 ж釭 馬模衛鑑棻.
		siMonsterKindNum--;
	}	
	
	uiRepresentKind				=	0;
	siRepresentKindNum			=	0;
	siMonsterCounter			=	0;	
	for(i = 0; i < siMonsterKindPerGroup; i++)
	{			
		pstMonsterInfo		=	&FieldMonsterInfo[uiSelectedMonsterKindIndex[i]];

		// 譆模 и葆葬 檜鼻擎 釭諦 輿橫撿 и棻.		
		siTempMonsterNum	=	max((siMonsterNumPerGroup * pstMonsterInfo->uiRate) / siTotalRate, 1);
		
		for(j = 0; j < siTempMonsterNum; j++)
		{		
			uiMonsterKinds[j + siMonsterCounter]	=	pstMonsterInfo->uiKind;
			siMonsterLevel[j + siMonsterCounter]	=	pstMonsterInfo->siLevel;
		}
	
		siMonsterCounter	+=	siTempMonsterNum;
	
		if(siRepresentKindNum < siTempMonsterNum)
		{
			uiRepresentKind		=	pstMonsterInfo->uiKind;
			siRepresentKindNum	=	siTempMonsterNum;
		}	
	}	
	
	//pIMonster->CheckInvalidChar( "End cltFieldMonster::GetWillCreateMonsters()" );

	return uiRepresentKind;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	擋暮.
//	熱薑 橾濠	:	2002-02-27 螃�� 1:07:17 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::Action()
{

	if(siCurrentFieldMonsterNum < siMinMonsterNum)
	{
		// ⑷營 跨蝶攪 熱陛 譆模 跨蝶攪 熱爾棻 濛擎 鼠褻勒 棻衛 營儅衛鑑棻.		
		CreateMonster();

		// 跨蝶攪蒂 營儅и 衛除擊 晦橘и棻.
		dwPrevResurrectTime	=	timeGetTime();
	}
	else
	{
		if(siCurrentFieldMonsterNum < siMaxMonsterNum)
		{
			// ⑷營 譆渠 跨蝶攪 爾棻 濛堅 橾薑 衛除檜 唳婁 ц棻賊 跨蝶攪蒂 и葆葬 營儅衛鑑棻.
			if((timeGetTime() - dwPrevResurrectTime) > 1000)
			{
				// 跨蝶攪蒂 и葆葬 儅撩и棻.				
				CreateMonster();

				// 跨蝶攪蒂 營儅и 衛除擊 晦橘и棻.
				dwPrevResurrectTime	=	timeGetTime();
			}
		}
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪蒂 儅撩и棻.(и葆葬)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltFieldMonster::CreateMonster()
{
	UI16		uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER];	
	SI32		siMonsterLevel[ON_MAX_CHAR_PER_PLAYER];	
	UI16		uiRepresentKind;

	// 儅撩п撿 ж朝 跨蝶攪曖 謙盟蒂 掘и棻.
	uiRepresentKind	=	GetWillCreateMonsters(uiMonsterKinds, siMonsterLevel);

	if(uiRepresentKind != 0)
	{
		// 跨蝶攪蒂 儅撩и棻.
		// 跨蝶攪蒂 儅撩ж朝 雖羲擊 憲溥遽棻.
		if(pIMonster->CreateMonster(siID, uiMapID, &rcActivityArea, uiRepresentKind, uiMonsterKinds, siMonsterLevel) == FALSE)
		{
			// 葬欐高檜 FALSE塭賊 渦 檜鼻檜 跨蝶攪蒂 儅撩й 熱 橈棻朝 匙檜棻.
			return FALSE;
		}
		else
		{
			siCurrentFieldMonsterNum++;				// ⑷營 в萄縑 氈朝 跨蝶攪曖 熱蒂 隸陛 衛鑑棻.
			return TRUE;
		}	
	}
	else
	{
		return FALSE;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪陛 避歷棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::DeadMonster()
{	
	siCurrentFieldMonsterNum	=	max(siCurrentFieldMonsterNum - 1, 0);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltFieldMonster::GetID()
{
	return siID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑瞳 滲熱蒂 蟾晦�� и棻.
//	熱薑 橾濠	:	2002-06-03 螃�� 4:10:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::InitStaticVariable(IMonster *pimonster, ISOMap *pimap)
{	
	pIMonster	=	pimonster;
	pIMap		=	pimap;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 衛濛и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 1:07:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::StartBattle()
{
	// ⑷營 в萄縑 氈朝 跨蝶攪曖 熱蒂 馬模 衛鑑棻.
	siCurrentFieldMonsterNum--;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	瞪癱蒂 謙猿и棻.
//	熱薑 橾濠	:	2002-02-27 螃�� 1:29:58 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::EndBattle()
{
	// ⑷營 в萄縑 氈朝 跨蝶攪曖 熱蒂 隸陛 衛鑑棻.
	siCurrentFieldMonsterNum++;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-06-17 螃瞪 11:08:40 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltFieldMonster::GetInfo(CHAR *pszBuffer)
{
	SI32	i;
	SI32	siRealMonsterNum;
	UI16	uiAccount;
	UI16	uiFirstMonsterAccount = 0;	

	siRealMonsterNum	=	0;
	for(i = 0; i < MAX_MONSTER_NUM; i++)
	{
		uiAccount	=	MONSTER_START_ACCOUNT + i;

		if(pIMonster->IsValidID(uiAccount) == TRUE)
		{
			if(pIMonster->GetFieldID(uiAccount) == siID)
			{
				if(uiFirstMonsterAccount == 0)
				{
					//if(pIMonster->GetStatus(uiAccount) == ON_PLAYER_STATUS_INFIELD)
					if( CHECKSTATUS( pIMonster->GetStatus(uiAccount), ON_PLAYER_STATUS_INFIELD ) == TRUE )
						uiFirstMonsterAccount	=	uiAccount;
				}

				siRealMonsterNum++;
			}
		}
	}
		
	sprintf(pszBuffer, "FID : [%d] MAX : [%u], MIN : [%u], CUR : [%u], REAL  : [%u], Account : [%u]", 
						siID, siMaxMonsterNum, siMinMonsterNum, siCurrentFieldMonsterNum, siRealMonsterNum, uiFirstMonsterAccount);
}

