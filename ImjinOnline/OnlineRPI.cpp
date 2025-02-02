#include <GSL.h>
#include "OnlineClient.h"
#include "OnlineRPI.h"
#include <DebugTool.h>

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnRPI::OnRPI()
{	
	Init();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnRPI::Init(OnlineClient	*pOnlineClient)
{
	pMyOnlineClient	=	pOnlineClient;
	Init();
} 

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnRPI::Init()
{
	ZeroMemory(uiStorageSendID, sizeof(uiStorageSendID));
	ZeroMemory(uiRequestPlayerID, sizeof(uiRequestPlayerID));	
	uiRequestPlayerNum	=	0;	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蹺陛.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnRPI::Add(UI16 uiUniqueID)
{	
	SI32	i;	

	// 爾麻湍 詭衛雖釭 ⑷營 爾鳥溥朝 詭衛雖縑 檜嘐 蛔煙腎橫 氈戲賊 蹺陛ж雖 彊朝棻.
	if(GetSlot(uiUniqueID) != -1)
		return;


	for(i = 0; i < uiRequestPlayerNum; i++)
		if(uiRequestPlayerID[i] == uiUniqueID) return;		

	if(uiRequestPlayerNum < MAX_REQUEST_PLAYER_ID)
	{		
		uiRequestPlayerID[uiRequestPlayerNum]	=	uiUniqueID;
		uiRequestPlayerNum++;		
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	餉薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnRPI::Del(UI16 uiUniqueID)
{
	SI32	siSlot;

	siSlot	=	GetSlot(uiUniqueID);		

	if(siSlot != -1)	
		uiStorageSendID[siSlot]	=	0;
}	

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭衛雖蒂 爾魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnRPI::Send()
{
	SI32	i;
	SI32	siIndex;
	
	if(uiRequestPlayerNum != 0)	
	{		
		pMyOnlineClient->SendRequestPlayerInfo(uiRequestPlayerNum, uiRequestPlayerID);			

		for(i = 0; i < uiRequestPlayerNum; i++)
		{
			siIndex		=	FindEmptySlot();

			if(siIndex != -1)
			{
				uiStorageSendID[siIndex]	=	uiRequestPlayerID[i];
			}
		}

		// 棻衛 蟾晦�� 衛麵遽棻.
		uiRequestPlayerNum	=	0;		
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	綠橫 氈朝 蝸煜擊 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnRPI::FindEmptySlot()
{
	SI32	i;

	for(i = 0; i < MAX_STORAGE_SEND_ID; i++)
	{
		if(uiStorageSendID[i] == 0)
			return i;
	}

	return -1;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	ID陛 撲薑腎橫 氈朝 蝸煜擊 瓊朝棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnRPI::GetSlot(UI16 uiUniqueID)
{
	SI32	i;

	for(i = 0; i < MAX_STORAGE_SEND_ID; i++)
	{
		if(uiStorageSendID[i] == uiUniqueID)
			return i;
	}

	return -1;			
}