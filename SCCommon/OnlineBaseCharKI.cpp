#include <GSL.h>
#include "OnlineBaseCharKI.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltOnlineBaseCharKI::cltOnlineBaseCharKI()
{
	uiTotalCharNum		=	0;
	pstOnlineBaseCharKI	=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltOnlineBaseCharKI::~cltOnlineBaseCharKI()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 й渡 嫡朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineBaseCharKI::Init(OnlineText* ponlinetext, UI32 uitotalcharnum)
{
	pOnlineText			=	ponlinetext;

	uiTotalCharNum		=	uitotalcharnum + 1;							// 0廓 檣策蝶蒂 縑楝煎 噙晦 嬪п憮 蕨擒п菩棻.
	pstOnlineBaseCharKI	=	new	strOnlineBaseCharKI[uiTotalCharNum];
	ZeroMemory(pstOnlineBaseCharKI, sizeof(strOnlineBaseCharKI) * uiTotalCharNum);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 п薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineBaseCharKI::Free()
{
	if(pstOnlineBaseCharKI)
	{
		delete [] pstOnlineBaseCharKI;
		pstOnlineBaseCharKI	=	NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	и貲曖 議葛攪蒂 蟾晦�飛� и棻.
//	熱薑 橾濠	:	2002-07-08 螃瞪 11:27:34 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	cltOnlineBaseCharKI::Init(UI32 uiIndex, UI16 uiID, UI32 uiName, SI16 siNation, SI16 siMoveSpeed, _ImportantParameta *pstParameta, SI16 siItemDrop[4], SI32 siDropPercent[4], SI32 siGeneral)
{
	// 0 廓 檣策蝶蒂 餌辨ж雖 彊擊 匙檜棻.
	uiIndex		+=	1;

	pstOnlineBaseCharKI[uiIndex].uiID			=	uiID;
	pstOnlineBaseCharKI[uiIndex].siNation		=	siNation;
	pstOnlineBaseCharKI[uiIndex].siMoveSpeed	=	siMoveSpeed;
	pstOnlineBaseCharKI[uiIndex].uiName			=	uiName;
	pstOnlineBaseCharKI[uiIndex].siGeneral		=	siGeneral;

	memcpy(pstOnlineBaseCharKI[uiIndex].siItemDrop, siItemDrop, sizeof(UI16) * 4);
	memcpy(pstOnlineBaseCharKI[uiIndex].siDropPercent, siDropPercent, sizeof( SI32 ) * 4);

	memcpy(&pstOnlineBaseCharKI[uiIndex].stImportantParameta, pstParameta, sizeof(_ImportantParameta));	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и貲曖 措陛蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	cltOnlineBaseCharKI::GetNation(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);
	return pstOnlineBaseCharKI[uiIndex].siNation;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	и貲曖 檜翕 樓紫蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	cltOnlineBaseCharKI::GetMoveSpeed(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);
	return pstOnlineBaseCharKI[uiIndex].siMoveSpeed;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ID蒂 餌辨ж罹 檣策蝶蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltOnlineBaseCharKI::FindIndex(UI16 uiID)
{
	UI32	i;

	for(i = 1; i < uiTotalCharNum; i++)
	{
		if(pstOnlineBaseCharKI[i].uiID	==	uiID)
			return i;
	}

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Name擊 餌辨ж罹 檣策蝶蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltOnlineBaseCharKI::FindIndex(CHAR *pszName)
{
	UI32	i;
	CHAR	*pszMonsterName;

	for(i = 1; i < uiTotalCharNum; i++)
	{
		pszMonsterName	=	pOnlineText->Get(pstOnlineBaseCharKI[i].uiName);

		if(pszMonsterName != NULL)
		{
			if(strcmp(pszMonsterName, pszName) == 0)
				return i;
		}
	}

	return 0;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	だ塭詭顫蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
_ImportantParameta*	cltOnlineBaseCharKI::GetParameta(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	return &pstOnlineBaseCharKI[uiIndex].stImportantParameta;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	晦獄 議葛攪 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strOnlineBaseCharKI*	cltOnlineBaseCharKI::GetBaseCharKI(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex == 0)	return NULL;
	else				return &pstOnlineBaseCharKI[uiIndex];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	晦獄 議葛攪 薑爾蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
strOnlineBaseCharKI*	cltOnlineBaseCharKI::GetBaseCharKI(CHAR *pszName)
{
	UI32	uiIndex	=	FindIndex(pszName);
	
	if(uiIndex == 0)	return NULL;
	else				return &pstOnlineBaseCharKI[uiIndex];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� ID檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltOnlineBaseCharKI::IsValidID(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return TRUE;
	else				return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 檜葷檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	cltOnlineBaseCharKI::IsValidName(CHAR *pszName)
{
	UI32	uiIndex	=	FindIndex(pszName);

	if(uiIndex != 0)	return TRUE;
	else				return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 檜葷(僥濠翮)擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-03-05 螃�� 5:10:26 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CHAR*	cltOnlineBaseCharKI::GetNameByString(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return	pOnlineText->Get(pstOnlineBaseCharKI[uiIndex].uiName);
	else				return	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪曖 檜葷(璋濠)擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	cltOnlineBaseCharKI::GetNameByNumeric(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return	pstOnlineBaseCharKI[uiIndex].uiName;
	else				return	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪陛 避歷擊衛 雲橫嗑葬朝 嬴檜蠱擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-08 螃瞪 11:15:45 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16*	cltOnlineBaseCharKI::GetDropItem(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return	pstOnlineBaseCharKI[uiIndex].siItemDrop;
	else				return	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	議葛攪陛 避歷擊衛 嬴檜蠱擊 雲橫嗑葬朝 �捕�擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-08 螃瞪 11:16:19 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32*	cltOnlineBaseCharKI::GetDropPercent(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return	pstOnlineBaseCharKI[uiIndex].siDropPercent;
	else				return	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	濰熱高擊 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-15 螃�� 9:51:54 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	cltOnlineBaseCharKI::GetGeneral(UI16 uiID)
{
	UI32	uiIndex	=	FindIndex(uiID);

	if(uiIndex != 0)	return	pstOnlineBaseCharKI[uiIndex].siGeneral;
	else				return	0;	
}

BOOL	cltOnlineBaseCharKI::IsFieldAttackUnit(UI16 uiID)
{
	UI32	uiIndex =	FindIndex(uiID);

	if(uiIndex != 0)	
	{	
		//4 == 奢撩嶸棉.
		if(pstOnlineBaseCharKI[uiIndex].siGeneral == 4)
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}
