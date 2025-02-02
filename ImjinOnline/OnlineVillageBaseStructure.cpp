// 氬渡濠 : 薑霞遵

#include <GSL.h>

#include <Main.h> 
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineVillageManager.h"
#include "OnlineVillageStructManager.h"
#include "OnlineVillageBaseStructure.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"


extern _InputDevice				IpD;


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageBaseStructure::OnlineVillageBaseStructure()
{
	siKind			=	ON_VIL_ST_KIND_UNKNOWN;
	pMyOnlineWorld	=	NULL;
	BackImage.Image	=	NULL;
	pVillageHeader	=	NULL;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageBaseStructure::~OnlineVillageBaseStructure()										
{
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageBaseStructure::Init( UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager )
{
	uiVillageCode		=	VillageCode;																		// 葆擊曖 嬴檜蛤
	pMyOnlineWorld		=	pOnlineWorld;	
	pButtonSpr			=	pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_SCROLLV );	// 葬模曖 幗が曖 檜嘐雖
	pVillageManager		=	pVillageStructManager;

	// 葆擊曖 薑爾蒂 嫡朝棻.
	pVillageHeader		=	pMyOnlineWorld->pOnlineVillageManager->GetVillageByCode(uiVillageCode);
	if( pVillageHeader == NULL )	clGrp.Error("OnlineVillageBaseStructure::Init", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_NO_VILLAGE_INFO ) );

	// 幗が擊 儅撩擊 и棻.
	BExit.Create( 620, 550, pButtonSpr->Header.Xsize, pButtonSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageBaseStructure::Free()
{
	siKind			=	ON_VIL_ST_KIND_UNKNOWN;
	pMyOnlineWorld	=	NULL;

	clGrp.FreeXspr( BackImage );
	BackImage.Image	=	NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageBaseStructure::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	if( siKind == ON_VIL_ST_KIND_UNKNOWN )	return TRUE;
	else									return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆擊曖 謙盟(�こ羹�, 衛濰, 煽錳...)蒂 橢橫螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI32	OnlineVillageBaseStructure::GetKind()
{
	return pVillageManager->GetKind();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	嶸�褲� 勒僭檣雖 憲嬴螞棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageBaseStructure::IsVaildStructure()
{
	if(pMyOnlineWorld != NULL)
		return TRUE;
	else
		return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Start Mouse Pos 雖薑
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageBaseStructure::SetStartMousePos(POINT ptStartMousePos)
{
	StartMousePos = ptStartMousePos;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	葆辦蝶 鼻鷓蒂 羹觼п憮 勒僭縑憮 釭陛朝 匙擊 憲嬴 魚棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageBaseStructure::CheckExit()
{
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

	if( ( BExit.Process( bMouseDown ) == TRUE ) || ( LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) ) )
	{
		pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
		return TRUE;
	}

	if( IpD.LeftPressSwitch )	bMouseDown	=	TRUE;
	else						bMouseDown	=	FALSE;

	return FALSE;
}