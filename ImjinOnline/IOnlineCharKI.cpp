/************************************************************************************************
	だ橾貲 : IOnlineCharKI.cpp
	氬渡濠 : 檜彌戮

	Character曖 薑爾蒂 婦葬и棻.
************************************************************************************************/
#include <GSL.h>
#include "OnlineWorld.h"
#include "OnlineChar-Parser.h"

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠.
//----------------------------------------------------------------------------------------------------------------	
IOnlineCharKI::IOnlineCharKI()
{
	pOnlineCharKI	=	NULL;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	п薯.
//----------------------------------------------------------------------------------------------------------------
IOnlineCharKI::~IOnlineCharKI()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.(寡紫 議葛攪煎 除輿ж罹 偽檜 蟾晦�飛� и棻.)
//----------------------------------------------------------------------------------------------------------------
BOOL	IOnlineCharKI::Init(cltOnlineWorld *pOnlineWorld)
{	
	SI32				i;	
	CharHead			*pCharHead;
	OnlineShipKI		*pOnlineShipKI;
	SI32				siFieldCharNum;


	// 辦摹 寡 檣攪む檜蝶 贗楚蝶陛 撩奢瞳戲煎 蟾晦�� 腎歷朝雖 憲嬴螞棻.
	if(pOnlineWorld->pIOnlineShipKI == NULL)			return	FALSE;
	if(pOnlineWorld->pIOnlineShipKI->IsInit() == FALSE)	return	FALSE;


	pOnlineWorld->pOnlineCharParser = new OnlineCharParser;
	if(pOnlineWorld->pOnlineCharParser->Init("Online\\CharInfo\\Character List.txt") == FALSE)	
		return FALSE;

	// 臢蝶お �倣狤� 氈朝 в萄 議葛攪曖 熱蒂 橢橫螞棻.
	siTotalCharKINum	=	siFieldCharNum	=	pOnlineWorld->pOnlineCharParser->GetCharTotalNum();

	// 寡曖 熱蒂 橢橫螞棻.
	siTotalCharKINum	+=	pOnlineWorld->pIOnlineShipKI->GetTotalShipNum();

	pOnlineCharKI	=	new	OnlineCharKI[siTotalCharKINum + 1];

	for(i = 1; i <= siTotalCharKINum; i++)
	{
		if(i <= siFieldCharNum)
		{
			// в萄縑 氈朝 議葛攪菟檜棻.
			pCharHead	=	pOnlineWorld->pOnlineCharParser->GetCharHeader((i - 1));
			
			// 議葛攪蒂 蟾晦�� и棻.
			if(pOnlineCharKI[i].Init(pCharHead->uiID, pCharHead->siNation, pCharHead->uiCharName, pCharHead->siDescText, pCharHead->siMoveSpeed, pCharHead->siNeedLv, pCharHead->uiWeaponType, pCharHead->siGeneral, pCharHead, pOnlineWorld) == FALSE)
			{
				clGrp.Error("Character Initialize Error", "[%d]", pCharHead->uiID);				
			}
		}
		else
		{
			// 寡檜棻.
			pOnlineShipKI	=	pOnlineWorld->pIOnlineShipKI->GetInfoByIndex(i - siFieldCharNum - 1);		

			// 議葛攪蒂 蟾晦�� и棻.
			if(pOnlineCharKI[i].Init(pOnlineShipKI->GetID(), 0, pOnlineShipKI->GetName(), 0, pOnlineShipKI->GetMoveSpeed(), 0, 0, 0, NULL, pOnlineWorld) == FALSE)
			{
				clGrp.Error("Character Initialize Error", "[%d]", pCharHead->uiID);				
			}
			
		}	
	}


	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	п薯.
//----------------------------------------------------------------------------------------------------------------
VOID	IOnlineCharKI::Free()
{
	if(pOnlineCharKI)
	{
		delete [] pOnlineCharKI;
		pOnlineCharKI = NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擁棲詭檜暮曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	IOnlineCharKI::GetCharID(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetCharID();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Char ID陛 撲薑腎橫氈朝 Index蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineCharKI::GetIndex(UI16 uiCharID)
{
	SI32	i;

	for(i = 1; i <= siTotalCharKINum; i++)
	{
		if(pOnlineCharKI[i].GetCharID() == uiCharID)
			return i;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	輿檣奢 議葛攪曖 ID 檣策蝶蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineCharKI::GetPlayerCharIDList(UI16 *pIndexArray)
{
	SI32	i;
	SI32	siIndexArrayCounter;

	siIndexArrayCounter	=	0;
	for(i = 1; i <= siTotalCharKINum; i++)
	{		
		if(pOnlineCharKI[i].GetClass() == ON_CHAR_KI_CLASS_PLAYER)
		{
			pIndexArray[siIndexArrayCounter]	=	pOnlineCharKI[i].GetCharID();			
			siIndexArrayCounter++;			
		}						
	}

	return siIndexArrayCounter;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曖 唳辦 跨蝶攪曖 檜葷擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	IOnlineCharKI::GetName(SI32 siKindIndex)
{
	if(siKindIndex <= 0)	return 0;
	return pOnlineCharKI[siKindIndex].GetName();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪 撲貲 Text
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineCharKI::GetDescText(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetDescText();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 贗楚蝶蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineCharKI::GetClass(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetClass();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 Nation蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineCharKI::GetNation(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetNation();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Move Speed 蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	IOnlineCharKI::GetMoveSpeed(SI32 siKindIndex)
{
	return pOnlineCharKI[siKindIndex].GetMoveSpeed();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	鼠晦 顫殮蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	IOnlineCharKI::GetWeaponType(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetWeaponType();
	else				return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п渡 嬴蛤陛 輿檣奢檣雖 憲嬴 獄棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineCharKI::IsPlayer(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if(siIndex == -1)	return	FALSE;
	else
		if(pOnlineCharKI[siIndex].GetClass() == ON_CHAR_KI_CLASS_PLAYER)	return	TRUE;	

	return	FALSE;
}

BOOL	IOnlineCharKI::IsSoldier(UI16 uiCharID)
{
	SI32	siIndex =	GetIndex(uiCharID);

	if(siIndex == -1)	return	FALSE;
	else
		if(pOnlineCharKI[siIndex].GetClass() == ON_CHAR_KI_CLASS_SOLDER)	return	TRUE;	

	return	FALSE;
	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱 Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	IOnlineCharKI::GetGeneral(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetGeneral();
	else				return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪 薑爾(寡偽擎唳辦 等檜攪陛 橈擠)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CharHead*	IOnlineCharKI::GetCharHeader(UI16 uiCharID)
{
	SI32	siIndex	=	GetIndex( uiCharID );

	if( siIndex != -1 )	return pOnlineCharKI[siIndex].GetCharHeader();
	else				return NULL;
}
