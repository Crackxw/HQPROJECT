/************************************************************************************************
	だ橾貲 : OnlineCharKI.cpp
	氬渡濠 : 檜彌戮

	Character曖 薑爾蒂 婦葬и棻.
************************************************************************************************/
#include <GSL.h>
#include <OnlineWorld.h>
#include <DebugTool.h>

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	儅撩濠.
//----------------------------------------------------------------------------------------------------------------
OnlineCharKI::OnlineCharKI()
{
	uiCharID			= 0;
	siClass				= ON_CHAR_KI_CLASS_UNKNOWN;
	siNation            = 0;

	siMoveSpeed			= 0;
	uiWeaponType		= 0;
	siNeedLevel			= 0;	
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	模資濠.
//----------------------------------------------------------------------------------------------------------------
OnlineCharKI::~OnlineCharKI()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	蟾晦��.
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineCharKI::Init(UI16 uicharID, SI32 Nation, UI32 uiname, SI32 siDescText, SI16 simovespeed, SI16 siReqLv, UI08 uiWeaponType, SI16 siGeneralFlag, CharHead* lpCharHeader, cltOnlineWorld *pOnlineWorld)
{	
	// 議葛攪曖 薑爾蒂 撲薑п 遽棻.
	OnlineCharKI::uiCharID		=	uicharID;
	OnlineCharKI::uiName		=	uiname;
	OnlineCharKI::siDescText    =   siDescText;

	// 議葛攪曖 ID蒂 碟戮ж罹 賃賃 薑爾蒂 撲薑п遽棻.(議葛攪 贗楚蝶)
	AnalysisCharID();	
	siNation = Nation;

	OnlineCharKI::siMoveSpeed	=	simovespeed;
	OnlineCharKI::siNeedLevel	=	siReqLv;
	OnlineCharKI::uiWeaponType	=	uiWeaponType;
	OnlineCharKI::siGeneralFlag =   siGeneralFlag;

	if(lpCharHeader)
	{
		CopyMemory(&sCharHeader, lpCharHeader, sizeof(sCharHeader));
	}
	else
	{
		ZeroMemory(&sCharHeader, sizeof(sCharHeader));
	}

	pMyOnlineWorld				=	pOnlineWorld;	

	return	TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//	撲貲	:	
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineCharKI::Free()
{
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擁棲詭檜暮曖 ID蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	OnlineCharKI::GetCharID()
{
	return uiCharID;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	贗楚蝶蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineCharKI::GetClass()
{
	return siClass;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	措陛蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineCharKI::GetNation()
{
	return siNation;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪曖 ID蒂 碟戮ж罹 薑爾蒂 撲薑п遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineCharKI::AnalysisCharID()
{
	// 議葛攪曖 贗楚蝶蒂 撲薑и棻.	
	switch(LOBYTE(uiCharID))
	{
	case 'C':
		// Ы溯檜橫檜棻.
		siClass	=	ON_CHAR_KI_CLASS_PLAYER;
		break;
	case 'M':
		siClass	=	ON_CHAR_KI_CLASS_MONSTER;
		break;
	case 'U':
		siClass	=	ON_CHAR_KI_CLASS_SOLDER;
		break;
	case 'S':
		siClass	=	ON_CHAR_KI_CLASS_SHIP;
		break;
	default:
		clGrp.Error(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNKNOWN_CHAR_CLASS), "[%d]", LOBYTE(uiCharID));
		siClass	=	ON_CHAR_KI_CLASS_UNKNOWN;
		break;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	跨蝶攪曖 檜葷擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI32	OnlineCharKI::GetName()
{
	return uiName;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪 撲貲 Text
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineCharKI::GetDescText()
{
	return siDescText;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Move Speed 蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineCharKI::GetMoveSpeed()
{
	return siMoveSpeed;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Move Speed 蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineCharKI::GetNeedLevel()
{
	return siNeedLevel;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Move Speed 蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08	OnlineCharKI::GetWeaponType()
{
	return uiWeaponType;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	濰熱 Flag
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI16	OnlineCharKI::GetGeneral()
{
	return siGeneralFlag;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪 薑爾(寡偽擎唳辦 等檜攪陛 橈擠)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
CharHead*	OnlineCharKI::GetCharHeader()
{
	if(uiCharID == sCharHeader.uiID)
	{
		return &sCharHeader;
	}

	return NULL;
}
