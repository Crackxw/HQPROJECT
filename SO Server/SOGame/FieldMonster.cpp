#include "SOMain.h"
#include "FieldMonster.h"
#include "OnlineCGSHeader.h"
#include "ISOMonster.h"
#include "IOnlineMap.h"

IMonster	*cltFieldMonster::pIMonster		=	NULL;
ISOMap		*cltFieldMonster::pIMap			=	NULL;

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	»ý¼ºÀÚ.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ÃÊ±âÈ­.
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-27 ¿ÀÈÄ 1:07:12 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

	// Total Rate¸¦ ±¸ÇÑ´Ù.
	siMaxMonsterKindNum		=	0;	
	for(i = 0; i < MAX_FIELD_MONSTER_KIND_PER_GROUP; i++)
	{
		if(FieldMonsterInfo[i].uiKind != 0)
		{
			uiSourceMonsterKindIndexTable[siMaxMonsterKindNum]	=	i;
			siMaxMonsterKindNum++;
		}
	}

	// ¸ó½ºÅÍ siMaxMonsterNum¸¸Å­ Áöµµ¿¡ ½É´Â´Ù.	
	siCurrentFieldMonsterNum	=	0;
	for(i = 0; i < siMaxMonsterNum; i++)
	{	
		// ¸ó½ºÅÍ¸¦ »ý¼ºÇÑ´Ù.
		CreateMonster();			
	}
	
	// ÀÌÀü¿¡ ¸ó½ºÅÍ¸¦ ºÎÈ°½ÃÅ² ½Ã°£À» ±â·ÏÇÑ´Ù.
	dwPrevResurrectTime	=	timeGetTime();
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¸¸µé ¸ó½ºÅÍÀÇ Kind¸¦ ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-12-15 ¿ÀÈÄ 12:51:13 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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
	
	// siMaxMonsterKindPerGroupÀÇ ¸ó½ºÅÍ¸¸ ÃßÃâÇØ ³»¸é µÈ´Ù.
	siMonsterKindPerGroup		=	siMaxMonsterKindPerGroup;

	// MonsterKind °¡ µé¾îÀÖ´Â TableÀ» º¹»çÇÑ´Ù.	
	siMonsterKindNum			=	siMaxMonsterKindNum;
	memcpy( uiMonsterKindIndexTable, uiSourceMonsterKindIndexTable, siMaxMonsterKindNum );	

	siTotalRate					=	0;		
	siMonsterNumPerGroup		=	rand() % ( siMaxMonsterNumPerGroup - siMinMonsterNumPerGroup + 1 ) + siMinMonsterNumPerGroup;		// ÇÑ ¸ó½ºÅÍ ±×·ìÀÌ °¡Áö´Â ¸ó½ºÅÍÀÇ ¼ö¸¦ ±¸ÇÑ´Ù.			
	siMonsterNumPerGroup		= siMonsterNumPerGroup < 1 ? siMinMonsterNumPerGroup : siMonsterNumPerGroup;

	// ¿ì¼± siMonsterKindPerGroup ¸¸Å­ ·£´ýÇÏ°Ô ¸ó½ºÅÍÀÇ Kind¸¦ ÃßÃâÇÑ´Ù.
	for( i = 0; i < siMonsterKindPerGroup; i++ )
	{
		// ·£´ýÇÏ°Ô ÇÏ³ªÀÇ ÀÎµ¦½º¸¦ »êÃâÇØ³½´Ù.
		uiRandomlyIndex		=	rand() % siMonsterKindNum;

		pstMonsterInfo					=	&FieldMonsterInfo[ uiMonsterKindIndexTable[ uiRandomlyIndex ] ];		
		uiSelectedMonsterKindIndex[ i ]	=	uiMonsterKindIndexTable[ uiRandomlyIndex ];

		siTotalRate						+=	pstMonsterInfo->uiRate;		

		// ¸ó½ºÅÍ TableÀ» ´Ù½Ã Á¤¸®ÇÑ´Ù.		
		memmove( &uiMonsterKindIndexTable[ uiRandomlyIndex ], &uiMonsterKindIndexTable[ uiRandomlyIndex + 1 ], ( siMonsterKindNum - ( uiRandomlyIndex + 1 ) ) );		

		// siMonsterKindNumÀ» ÇÏ³ª °¨¼Ò½ÃÅ²´Ù.
		siMonsterKindNum--;
	}	
	
	uiRepresentKind				=	0;
	siRepresentKindNum			=	0;
	siMonsterCounter			=	0;	
	for(i = 0; i < siMonsterKindPerGroup; i++)
	{			
		pstMonsterInfo		=	&FieldMonsterInfo[uiSelectedMonsterKindIndex[i]];

		// ÃÖ¼Ò ÇÑ¸¶¸® ÀÌ»óÀº ³ª¿Í ÁÖ¾î¾ß ÇÑ´Ù.		
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


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	¾×¼Ç.
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-27 ¿ÀÈÄ 1:07:17 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltFieldMonster::Action()
{

	if(siCurrentFieldMonsterNum < siMinMonsterNum)
	{
		// ÇöÀç ¸ó½ºÅÍ ¼ö°¡ ÃÖ¼Ò ¸ó½ºÅÍ ¼öº¸´Ù ÀÛÀº ¹«Á¶°Ç ´Ù½Ã Àç»ý½ÃÅ²´Ù.		
		CreateMonster();

		// ¸ó½ºÅÍ¸¦ Àç»ýÇÑ ½Ã°£À» ±â¾ïÇÑ´Ù.
		dwPrevResurrectTime	=	timeGetTime();
	}
	else
	{
		if(siCurrentFieldMonsterNum < siMaxMonsterNum)
		{
			// ÇöÀç ÃÖ´ë ¸ó½ºÅÍ º¸´Ù ÀÛ°í ÀÏÁ¤ ½Ã°£ÀÌ °æ°ú Çß´Ù¸é ¸ó½ºÅÍ¸¦ ÇÑ¸¶¸® Àç»ý½ÃÅ²´Ù.
			if((timeGetTime() - dwPrevResurrectTime) > 1000)
			{
				// ¸ó½ºÅÍ¸¦ ÇÑ¸¶¸® »ý¼ºÇÑ´Ù.				
				CreateMonster();

				// ¸ó½ºÅÍ¸¦ Àç»ýÇÑ ½Ã°£À» ±â¾ïÇÑ´Ù.
				dwPrevResurrectTime	=	timeGetTime();
			}
		}
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸ó½ºÅÍ¸¦ »ý¼ºÇÑ´Ù.(ÇÑ¸¶¸®)
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
BOOL	cltFieldMonster::CreateMonster()
{
	UI16		uiMonsterKinds[ON_MAX_CHAR_PER_PLAYER];	
	SI32		siMonsterLevel[ON_MAX_CHAR_PER_PLAYER];	
	UI16		uiRepresentKind;

	// »ý¼ºÇØ¾ß ÇÏ´Â ¸ó½ºÅÍÀÇ Á¾·ù¸¦ ±¸ÇÑ´Ù.
	uiRepresentKind	=	GetWillCreateMonsters(uiMonsterKinds, siMonsterLevel);

	if(uiRepresentKind != 0)
	{
		// ¸ó½ºÅÍ¸¦ »ý¼ºÇÑ´Ù.
		// ¸ó½ºÅÍ¸¦ »ý¼ºÇÏ´Â Áö¿ªÀ» ¾Ë·ÁÁØ´Ù.
		if(pIMonster->CreateMonster(siID, uiMapID, &rcActivityArea, uiRepresentKind, uiMonsterKinds, siMonsterLevel) == FALSE)
		{
			// ¸®ÅÏ°ªÀÌ FALSE¶ó¸é ´õ ÀÌ»óÀÌ ¸ó½ºÅÍ¸¦ »ý¼ºÇÒ ¼ö ¾ø´Ù´Â °ÍÀÌ´Ù.
			return FALSE;
		}
		else
		{
			siCurrentFieldMonsterNum++;				// ÇöÀç ÇÊµå¿¡ ÀÖ´Â ¸ó½ºÅÍÀÇ ¼ö¸¦ Áõ°¡ ½ÃÅ²´Ù.
			return TRUE;
		}	
	}
	else
	{
		return FALSE;
	}
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	¸ó½ºÅÍ°¡ Á×¾ú´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltFieldMonster::DeadMonster()
{	
	siCurrentFieldMonsterNum	=	max(siCurrentFieldMonsterNum - 1, 0);
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í	:	ID¸¦ ¾ò¾î¿Â´Ù.
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
SI32	cltFieldMonster::GetID()
{
	return siID;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Á¤Àû º¯¼ö¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-06-03 ¿ÀÈÄ 4:10:53 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltFieldMonster::InitStaticVariable(IMonster *pimonster, ISOMap *pimap)
{	
	pIMonster	=	pimonster;
	pIMap		=	pimap;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀüÅõ¸¦ ½ÃÀÛÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-27 ¿ÀÈÄ 1:07:40 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltFieldMonster::StartBattle()
{
	// ÇöÀç ÇÊµå¿¡ ÀÖ´Â ¸ó½ºÅÍÀÇ ¼ö¸¦ °¨¼Ò ½ÃÅ²´Ù.
	siCurrentFieldMonsterNum--;
}

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	ÀüÅõ¸¦ Á¾·áÇÑ´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-02-27 ¿ÀÈÄ 1:29:58 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
VOID	cltFieldMonster::EndBattle()
{
	// ÇöÀç ÇÊµå¿¡ ÀÖ´Â ¸ó½ºÅÍÀÇ ¼ö¸¦ Áõ°¡ ½ÃÅ²´Ù.
	siCurrentFieldMonsterNum++;	
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	¼³¸í		:	Á¤º¸¸¦ ¾ò¾î¿Â´Ù.
//	¼öÁ¤ ÀÏÀÚ	:	2002-06-17 ¿ÀÀü 11:08:40 - ¾çÁ¤¸ð
//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
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

