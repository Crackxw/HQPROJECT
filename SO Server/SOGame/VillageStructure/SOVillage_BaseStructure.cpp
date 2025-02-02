#include "SOMain.h"
#include "SOPlayer.h"
#include "SOVillage_BaseStructure.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	儅撩濠.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:03:14 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_BaseStructure::SOVillage_BaseStructure()
{

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	模資濠.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:03:32 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SOVillage_BaseStructure::~SOVillage_BaseStructure()
{
	Free();
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 勒僭縑 譆渠 菟橫螢 熱 氈朝 餌塋曖 熱蒂 撲薑и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:03:52 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_BaseStructure::Init( UI16 uiVillageCode, DWORD dwMaxPlayer )
{
	m_uiVillageCode	=	uiVillageCode;

	m_PlayerMgr.Init( dwMaxPlayer );	
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	詭賅葬蒂 п薯и棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:04:28 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	SOVillage_BaseStructure::Free()
{
	m_PlayerMgr.Free();		
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑 菟橫諮棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:04:50 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_BaseStructure::EnterPlayer( SOPlayer *pPlayer )
{
	// 檜嘐 Ы溯檜橫陛 檜 勒僭縑 菟橫諮朝雖 匐餌и棻.
	if( IsEntered( pPlayer ) == TRUE )						return	FALSE;

	// Ы溯檜橫蒂 蹺陛衛鑑棻.
	if( m_PlayerMgr.AddPlayer( pPlayer ) == -1 )			return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑憮 釭鬲棻.
//	熱薑 橾濠	:	2002-09-30 螃�� 9:08:21 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_BaseStructure::LeavePlayer( SOPlayer *pPlayer )
{
	// Ы溯檜橫蒂 餉薯и棻.
	if( m_PlayerMgr.RemovePlayer( pPlayer ) == -1 )			return	FALSE;

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	檜 勒僭縑 菟橫諦 氈朝 Ы溯檜橫曖 葬蝶お蒂 橢橫螞棻.
//	熱薑 橾濠	:	2002-10-01 螃瞪 11:46:43 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	SOVillage_BaseStructure::GetEnteredPlayerList( UI16 *puiPlayerList )
{
	return	m_PlayerMgr.GetAddedPlayerList( puiPlayerList );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	⑷營 檜 勒僭縑 菟橫諦氈朝 賅萇 Ы溯檜橫啪 翕橾и 詭衛雖蒂 爾魚棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 1:50:49 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_BaseStructure::SendMsgToAllPlayer( SI32 siMsg, LPVOID pParam3, LPVOID pParam4, LPVOID pParam5, LPVOID pParam6 )
{
	SOPlayer		*pPlayer;	
	UI16			uiPlayersNum;
	UI16			uiPlayersAccount[ 1000 ];		// 橫替 勒僭檜萇 譆渠 1000貲虜檜 菟橫陞 熱氈棻.

	// 衛瞪縑 菟橫氈朝 Ы溯檜橫蒂 掘и棻.
	uiPlayersNum	=	GetEnteredPlayerList( uiPlayersAccount );

	if( uiPlayersNum >= 1 )
	{		
		pPlayer		=	m_PlayerMgr.GetFirstPlayer();		

		pPlayer->SendFieldMsg( siMsg, SO_SFM_CUSTOM, LPVOID( uiPlayersNum ), LPVOID( uiPlayersAccount ), pParam3, pParam4, pParam5, pParam6 );
	}		

	return	TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲		:	Ы溯檜橫陛 檜 勒僭縑 菟橫諦氈朝雖 匐餌и棻.
//	熱薑 橾濠	:	2002-10-01 螃�� 4:00:11 - 曄薑賅
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	SOVillage_BaseStructure::IsEntered( SOPlayer *pPlayer )
{
	return	m_PlayerMgr.IsAddedPlayer( pPlayer );
}


