#include <GSL.h>
#include "IOnlineShipKI.h"
#include "ShipInfoParser.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IOnlineShipKI::IOnlineShipKI()
{
	pOnlineShipKI	=	NULL;
	siTotalShipNum	=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
IOnlineShipKI::~IOnlineShipKI()
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦�飛� и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineShipKI::Init()
{
	cltShipInfoParser	clShipInfoParser;
	SI32				i;
	strShipInfo			*pstShipInfo;

	if(clShipInfoParser.Init("Online\\ShipInfo\\ShipInfo.txt") == TRUE)
	{
		siTotalShipNum	=	clShipInfoParser.GetTotalShipNum();

		pOnlineShipKI	=	new OnlineShipKI[siTotalShipNum];

		for(i = 0; i < siTotalShipNum; i++)
		{
			pstShipInfo		=	clShipInfoParser.GetShipInfo(i);
			pOnlineShipKI[i].Init(pstShipInfo->siID, pstShipInfo->siName, pstShipInfo->uiMaxCustom, pstShipInfo->siMoveSpeed);
		}

		return TRUE;
	}
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭賅葬蒂 п薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	IOnlineShipKI::Free()
{
	if(pOnlineShipKI)
	{
		delete [] pOnlineShipKI;
		pOnlineShipKI	=	NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Name Code蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineShipKI::GetName(SI32 siID)
{
	return pOnlineShipKI[FindIndex(siID)].GetName();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	譆渠 驍 熱 氈朝 檣錳擊 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineShipKI::GetMaxCustomer(SI32 siID)
{
	return pOnlineShipKI[FindIndex(siID)].GetMaxCustomer();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	檜翕 樓紫蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineShipKI::GetMoveSpeed(SI32 siID)
{
	return pOnlineShipKI[FindIndex(siID)].GetMoveSpeed();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ID蒂 餌辨ж罹 Index蒂 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineShipKI::FindIndex(SI32 siID)
{
	SI32	i;

	for(i = 0; i < siTotalShipNum; i++)
	{
		if(siID == pOnlineShipKI[i].GetID())
			return i;
	}

	return -1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦�飛� ц朝雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	IOnlineShipKI::IsInit()
{
	if(pOnlineShipKI)
		return TRUE;
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	識 寡曖 熱蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	IOnlineShipKI::GetTotalShipNum()
{
	return siTotalShipNum;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Index蒂 餌辨ж罹 OnlineShipKI ん檣攪蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineShipKI*	IOnlineShipKI::GetInfoByIndex(SI32 siIndex)
{
	return &pOnlineShipKI[siIndex];
}
