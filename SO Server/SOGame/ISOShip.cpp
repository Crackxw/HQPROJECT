#include "SOMain.h"
#include "ISOShip.h"

#define	SHIPACCOUNTTOINDEX(x)		(x - SHIP_START_ACCOUNT)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
ISOShip::ISOShip()
{
	uiLastShipAccount		=	SHIP_START_ACCOUNT;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
ISOShip::~ISOShip()
{
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOShip::Init()
{		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	й渡 嫡懊湍 詭賅葬蒂 п薯и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOShip::Free()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.		
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOShip::Init(UI16 uimapid, UI16 uishipid, UI16 uideparturevillagecode, UI16 uidestinationvillagecode, SI32 simovepathid, UI16 uiCustomerNum, UI16 *puiCustomerArray)
{
	UI16	uiAccount;

	uiAccount	=	FindAccount();	

	if(uiAccount != 0)
	{
		// 蟾晦�飛� и棻.
		if(clSOShip[SHIPACCOUNTTOINDEX(uiAccount)].Init(uiAccount, uimapid, uishipid, uideparturevillagecode, uidestinationvillagecode, simovepathid, uiCustomerNum, puiCustomerArray) == TRUE)
		{			
			return TRUE;
		}	
		else
			return FALSE;		
	}
	else
		return FALSE;	
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Poll л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOShip::Poll()
{
	SI32	i;

	for(i = 0; i < MAX_SHIP_NUM; i++)
	{
		if(clSOShip[i].uiAccount != 0)
		{
			if(clSOShip[i].Poll() == TRUE)
			{
				// 寡陛 部梱雖 薯 羲й擊 棻 ц棻賊 TRUE蒂 葬欐и棻.
				clSOShip[i].Free();
			}
		}
	}
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	綠橫 氈朝 蝸煜擊 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	ISOShip::FindAccount()
{
	do
	{
		uiLastShipAccount++;
		if(uiLastShipAccount > (SHIP_START_ACCOUNT + MAX_SHIP_NUM))
			uiLastShipAccount	=	SHIP_START_ACCOUNT;		

		if(clSOShip[SHIPACCOUNTTOINDEX(uiLastShipAccount)].uiAccount == 0)
			return uiLastShipAccount;

	}while(TRUE);

	return 0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Ы溯檜橫陛 寡蒂 顫堅 氈朝 鼻鷓縑憮 煎斜 嬴醒擊 ц棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOShip::LogOutPlayer(UI16 uiShipAccount, UI16 uiPlayerAccount, SI32 siDBAccount)
{
	clSOShip[SHIPACCOUNTTOINDEX(uiShipAccount)].LogOutPlayer(uiPlayerAccount, siDBAccount);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-08-15 螃�� 1:09:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	ISOShip::GetInfo(UI16 uiShipAccount, OnCharShipInfo *pShipInfo)
{
	clSOShip[SHIPACCOUNTTOINDEX(uiShipAccount)].GetInfo(pShipInfo);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� ID 檣陛?
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	ISOShip::IsValidID(UI16 uiAccount)
{
	if(ISSHIP(uiAccount) == TRUE)
	{
		if(clSOShip[SHIPACCOUNTTOINDEX(uiAccount)].uiAccount != 0)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	寡蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-26 螃�� 6:08:12 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
cltSOShip*	ISOShip::GetShip(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == FALSE)	return	NULL;
	else								return	&clSOShip[SHIPACCOUNTTOINDEX(uiAccount)];
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜翕 ц擊衛縑 檜翕 詭衛雖蒂 輿嬪縑 Ы溯檜橫菟縑啪 爾麻朝陛 憲嬴螞棻.
//	熱薑 橾濠	:	2002-07-26 螃�� 6:15:00 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI08		ISOShip::DidSendMoveInfo(UI16 uiAccount)
{
	if(IsValidID(uiAccount) == FALSE)	return	0;
	else								return	clSOShip[SHIPACCOUNTTOINDEX(uiAccount)].DidSendMoveInfo();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	蝓摹и 餌塋菟曖 薑爾蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-07-27 螃�� 7:00:53 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
UI16	ISOShip::GetCustomers(UI16 uiAccount, UI16 *puiAccounts)
{
	if(IsValidID(uiAccount) == FALSE)	return	0;
	else								return	clSOShip[SHIPACCOUNTTOINDEX(uiAccount)].GetCustomers(puiAccounts);	
}
