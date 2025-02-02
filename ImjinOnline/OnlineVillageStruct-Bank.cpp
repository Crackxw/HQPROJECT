/***********************************************************************************************************
	熱薑濠 : 薑霞遵
	熱薑橾 : 2002.03.31
***********************************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>
#include <etc.h>

#include "OnlineWorld.h"
#include "OnlineVillage.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineNumberControl.h"
#include "OnlineVillageStruct-Bank.h"
#include "OnlineTradeBook.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 辨 等檜攪 蹺陛
#include "OnlinePannel.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineMiniMap.h"
#include "OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineMercenary.h"


extern _InputDevice				IpD;


///////////////////////////////////////////////////////
// 擋滕~
#define ON_BANK_MAIN_INIT			0
#define ON_BANK_MAIN_POLL			1
#define ON_BANK_EXIT				2
#define ON_BANK_ACCOUNT_INIT		3						// 啗謝 虜菟晦
#define ON_BANK_ACCOUNT_POLL		4						
#define ON_BANK_ACCOUNT_EXIT		5
#define ON_BANK_DEPOSIT_INIT		6						// 殮旎
#define ON_BANK_DEPOSIT_POLL		7
#define ON_BANK_DEPOSIT_EXIT		8
#define ON_BANK_DRAWING_INIT		9						// 轎旎
#define ON_BANK_DRAWING_POLL		10
#define ON_BANK_DRAWING_EXIT		11
#define ON_BANK_LOAN_INIT			12						// 渠轎
#define ON_BANK_LOAN_POLL			13
#define ON_BANK_LOAN_EXIT			14


//#include <iostream>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////
#define ON_BANK_ACCOUNT_NUM						6

extern	_clGame* pGame;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	儅撩濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureBank::OnlineVillageStructureBank() : OnlineVillageBaseStructure()
{
	dwTime		=	0;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	模資濠.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
OnlineVillageStructureBank::~OnlineVillageStructureBank()
{	
	Free();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	蟾晦��
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureBank::Init(UI16 VillageCode, cltOnlineWorld *pOnlineWorld, VillageStructManager	*pVillageStructManager)
{	

	OnlineVillageBaseStructure::Init(VillageCode, pOnlineWorld, pVillageStructManager);

	pMyOnlineWorld		=	pOnlineWorld;
	siActionStatus		=	ON_BANK_MAIN_INIT;
	siSpriteNum			=	0;
	siTalk				=	0;
	dwTime				=	timeGetTime();

	// 檜嘐雖蒂 煎萄и棻.	
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\town_button_medium.spr", MainButtonSpr01 );
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Town_Bank.Spr", SubSpr );
	clGrp.LoadXspr( "Online\\Gamespr\\Book\\main_window_button.Spr", ExitSpr );							// 釭陛晦 幗が

	siX					=	234;									// 晦遽檜 腎朝 濠ル X
	siY					=	115;																			// 晦遽檜 腎朝 濠ル Y
	bMouseLDown			=	FALSE;
	pBorder				=	pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_NUMBER );
	m_siAccontNum				=	-1;


	BDeposit.Create(  siX + 78,siY + 202, MainButtonSpr01.Header.Xsize, MainButtonSpr01.Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK_ACCOUNTBUTTON_SAVING ), BUTTON_PUT_LEFT, TRUE );
	BDrawing.Create(  siX + 176, siY + 202, MainButtonSpr01.Header.Xsize, MainButtonSpr01.Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK_ACCOUNTBUTTON_DRAWAL ), BUTTON_PUT_LEFT, TRUE );
	BBack.Create(  siX + 132, siY + 254, ExitSpr.Header.Xsize, ExitSpr.Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

	// 蝶觼煤 夥 儅撩
	XSPR	*pScrollBar = pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_NOTICE_SCROLL_BAR );

}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureBank::Free()
{	
	clGrp.FreeXspr( MainButtonSpr01 );	
	clGrp.FreeXspr( SubSpr );
	clGrp.FreeXspr( ExitSpr );	

	pMyOnlineWorld->pOnlineMsgBox->Initialize();
}


BOOL	OnlineVillageStructureBank::Action()
{
	CheckButton();

	pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );

	switch( siActionStatus )
	{
	case ON_BANK_MAIN_INIT:												// 瞪濰 薑爾蒂 嫡朝棻.
		{
			siActionStatus	=	ON_BANK_ACCOUNT_INIT;
			return FALSE;
		}
		break;

	case ON_BANK_MAIN_POLL:
		break;

	case ON_BANK_EXIT:													// 瞪濰 釭陛晦
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
		Free();
		return TRUE;

	
	case ON_BANK_DEPOSIT_INIT:											// 殮旎		
		{
			siSpriteNum			=	1;				
			siActionStatus		=	ON_BANK_DEPOSIT_POLL;
			m_siSaveStatus		=	ON_BANK_SAVE_INIT;
		}
		break;

	case ON_BANK_DEPOSIT_POLL:
		SaveAction();
		break;

	case ON_BANK_DEPOSIT_EXIT:
		siActionStatus	    =	ON_BANK_ACCOUNT_INIT;
		break;

	case ON_BANK_DRAWING_INIT:
		{
			siSpriteNum					=	2;	
			siActionStatus		=	ON_BANK_DRAWING_POLL;
			m_siWithdrawStatus	=	ON_BANK_WITHDRAW_INIT;
		}
		break;

	case ON_BANK_DRAWING_POLL:
		WithdrawAction();
		break;

	case ON_BANK_DRAWING_EXIT:
		siActionStatus	=	ON_BANK_ACCOUNT_INIT;
		break;

	}


	DWORD	dwTempTime = timeGetTime();

	if( ( ( dwTempTime - dwTime ) / 1000 ) >= 10 )
	{
		siTalk	=	0;
		dwTime	=	dwTempTime;	
	}

	if( IpD.LeftPressSwitch )					bMouseLDown	=	TRUE;
	else										bMouseLDown =	FALSE;

	return FALSE;
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	擋暮.	
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL	OnlineVillageStructureBank::Poll(LPDIRECTDRAWSURFACE7 pSurface)
{
	pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TRADEBOOK | 
		ONLINEPANNEL_DISABLEFLAG_MINIMAP | ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_INVENTORY
		| ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON);

	pMyOnlineWorld->pOnlineVillage->DrawVillageInfo(pSurface, TRUE);

	Draw( pSurface );

	return FALSE;
}



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	萄煎辦 (�飛橦� 轎溘и棻.)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureBank::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;

	if( siActionStatus == ON_BANK_MAIN_INIT || siActionStatus == ON_BANK_EXIT)		return;

	if( clGrp.LockSurface( pSurface ) == TRUE )
	{

		clGrp.PutSpriteT( 234, 115, SubSpr.Header.Xsize, SubSpr.Header.Ysize, SubSpr.Image);		


		if( siActionStatus == ON_BANK_DEPOSIT_POLL )		BDeposit.Put( &MainButtonSpr01, 1, 1, 1, BUTTON_PUT_NOMOVE );
		else												BDeposit.Put( &MainButtonSpr01, 0, 2, 1, BUTTON_PUT_NOMOVE );

		if( siActionStatus == ON_BANK_DRAWING_POLL )		BDrawing.Put( &MainButtonSpr01, 1, 1, 1, BUTTON_PUT_NOMOVE );
		else												BDrawing.Put( &MainButtonSpr01, 0, 2, 1, BUTTON_PUT_NOMOVE );


		BBack.Put( &ExitSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );		

		clGrp.UnlockSurface( pSurface );
	}

	// actdoll (2004/01/06 17:08) : 棻艇 寞徹戲煎 NPC 蟾鼻斜葬晦
	pMyOnlineWorld->m_pPortrait->DrawAsNPC( siX + 25, siY + 45, NPC_KIND_BANK, pMyOnlineWorld->pOnlineVillage->GetNation() );


	if( pSurface->GetDC( &hDC ) == DD_OK )
	{
		SetBkMode( hDC, TRANSPARENT );
		SelectObject( hDC, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );

		// npc 渠餌 轎溘 ( 10蟾 )
		if( siTalk != 0)
		{			
			pMyOnlineWorld->pOnlineMegaText->DrawTextLine( hDC, siX + 118, siY + 50, 188, pMyOnlineWorld->pOnlineText->Get( siTalk ) );
		}

		char szTemp2[256];

		MakeMoneyText(pMyOnlineWorld->pMyData->GetSaving(), szTemp2);

		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, siX + 67, siY + 163, 72, 14, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_ACCOUNT_BALANCE), RGB( 210, 210, 210 ) );		// 殮旎擋
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter( hDC, siX + 144, siY + 161, 123, 18 ,szTemp2,RGB( 210, 210, 210 ));

		BDeposit.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		BDrawing.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		BBack.Put(hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

		pSurface->ReleaseDC( hDC );
	}

	// 辨煽
	pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// 濰睡
	if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
	{
		pMyOnlineWorld->pOnlineTradeBook->Poll();
		pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 嘐棲裘
	if(pMyOnlineWorld->pOnlineMiniMap->IsActive())
	{
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	// 啗骯晦蒂 轎溘ж晦 嬪ж罹 蟾晦�� ж朝 睡碟.
	if( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow() )
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->Draw( pSurface );
}
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	詭檣 �飛橦� 氈朝 幗が菟擊 羹觼 и棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
VOID	OnlineVillageStructureBank::CheckButton()
{	
	POINT	pt;
	
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	if( BDeposit.Process( bMouseLDown ) == TRUE && !pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
	{
		if( siActionStatus != ON_BANK_DEPOSIT_POLL )
		{
			siActionStatus		=	ON_BANK_DEPOSIT_INIT;
			bMouseLDown			=	FALSE;
			siSpriteNum			=	1;
			siTalk				=	ON_TEXT_BANK2INPUTDEPOSITMONEY;
			dwTime				=	timeGetTime();
			return;
		}
	}

	// 轎旎 幗が
	else if( BDrawing.Process( bMouseLDown ) == TRUE )
	{
		// 啗骯晦陛 爾檜朝 鼻鷓縑憮朝 詭景滲唳 碳陛.
		
		if( siActionStatus != ON_BANK_DRAWING_POLL && !pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow())
		{
			siActionStatus		=	ON_BANK_DRAWING_INIT;
			bMouseLDown			=	FALSE;
			siSpriteNum			=	1;
			siTalk				=	ON_TEXT_BANK2INPUTWITHDRAWMONEY;
			dwTime				=	timeGetTime();
			return;
		}
	}

	else if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)) || BBack.Process(bMouseLDown))
	{		
		if(!pMyOnlineWorld->pOnlineInventory->IsActive() )
		{
			siActionStatus		=	ON_BANK_EXIT;
			bMouseLDown			=	FALSE;
		}
	}
}

VOID OnlineVillageStructureBank::SetAllButtonAction(BOOL bActive)
{
	BDeposit.SetAction(bActive);
	BDrawing.SetAction(bActive);
	BBack.SetAction(bActive);
}

VOID	OnlineVillageStructureBank::WithdrawAction()
{
	switch( m_siWithdrawStatus )
	{
	case ON_BANK_WITHDRAW_INIT:	
		// 鱔濰縑 絲檜 0錳檜鼻氈戲賊!
		if (pMyOnlineWorld->pMyData->GetSaving() > 0) {
			
			MONEY maxdrawing = pMyOnlineWorld->pMyData->GetSaving();
			
			// 譆渠 模雖旎檜鼻檜 腎啪 絲擊 檣轎й熱 橈棻!
			if (maxdrawing > (ON_MAX_MONEY - pMyOnlineWorld->pMyData->GetMoney()) )
			{
				maxdrawing = (ON_MAX_MONEY - pMyOnlineWorld->pMyData->GetMoney());
			}
			// 檣轎й熱氈朝 絲檜 氈擊陽虜!!!
			
			if (maxdrawing > 0 ) {
				
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( maxdrawing );
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( (ON_SCREEN_XSIZE - pBorder->Header.Xsize )/2, (ON_SCREEN_YSIZE-pBorder->Header.Ysize)/2);
				
				m_siWithdrawStatus = ON_BANK_WITHDRAW_INPUT;
				SetAllButtonAction(FALSE);
				break;
			}
		}
		m_siWithdrawStatus	=	ON_BANK_WITHDRAW_EXIT;
		break;

	case ON_BANK_WITHDRAW_INPUT:
		if( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )	{

			if (pMyOnlineWorld->pOnlineNumPad->siCount > 0) {

				pMyOnlineWorld->pOnlineClient->SendRequestBankWithdraw(pMyOnlineWorld->pOnlineNumPad->siCount);
				m_siWithdrawStatus		=	ON_BANK_WITHDRAW_WAIT;
			}
			else
			{
				m_siWithdrawStatus		=	ON_BANK_WITHDRAW_EXIT;

			}
		}
		break;	
	case ON_BANK_WITHDRAW_WAIT:		
		{
		}
		break;

	case ON_BANK_WITHDRAW_EXIT:
		siActionStatus		=	ON_BANK_DRAWING_EXIT;
		SetAllButtonAction(TRUE);
		break;
	case ON_BANK_WITHDRAW_ERROR:
		siActionStatus		=	ON_BANK_DRAWING_EXIT;
		pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK_ACCOUNTRESULT_DRAWALACCOUNT_ERROR ) );					
		SetAllButtonAction(TRUE);
		break;
	}
}


VOID	OnlineVillageStructureBank::SaveAction()
{
	switch( m_siSaveStatus )
	{
	case ON_BANK_SAVE_INIT:	
		// 濠晦 絲檜 0錳檜鼻氈戲賊!
		if (pMyOnlineWorld->pMyData->GetMoney() > 0) {
			
			MONEY maxsave = pMyOnlineWorld->pMyData->GetMoney();
			
			// 譆渠 盪蹴旎 檜鼻 盪旎й熱橈棻!!!
			if (maxsave > (ON_MAX_SAVING - pMyOnlineWorld->pMyData->GetSaving()) )
			{
				maxsave = (ON_MAX_SAVING - pMyOnlineWorld->pMyData->GetSaving());
			}
			// 檣轎й熱氈朝 絲檜 氈擊陽虜!!!
			
			if (maxsave > 0 ) {
				
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize( maxsave );
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_MONEY);
				pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow( (ON_SCREEN_XSIZE - pBorder->Header.Xsize )/2, (ON_SCREEN_YSIZE-pBorder->Header.Ysize)/2);
				
				m_siSaveStatus = ON_BANK_SAVE_INPUT;
				SetAllButtonAction(FALSE);
				break;
			}
		}
		m_siSaveStatus	=	ON_BANK_SAVE_EXIT;
		break;

	case ON_BANK_SAVE_INPUT:
		if( pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() )	{

			if (pMyOnlineWorld->pOnlineNumPad->siCount > 0) {
				pMyOnlineWorld->pOnlineClient->SendRequestBankSave(pMyOnlineWorld->pOnlineNumPad->siCount);
				m_siSaveStatus		=	ON_BANK_SAVE_WAIT;								
			}
			else
			{
				m_siSaveStatus		=	ON_BANK_SAVE_EXIT;

			}
		}
		break;	
	case ON_BANK_SAVE_WAIT:		
		{
		}
		break;

	case ON_BANK_SAVE_EXIT:
		siActionStatus		=	ON_BANK_DEPOSIT_EXIT;
		SetAllButtonAction(TRUE);
		break;
	case ON_BANK_SAVE_ERROR:
		siActionStatus		=	ON_BANK_DEPOSIT_EXIT;
		pMyOnlineWorld->pOnlineMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK_ACCOUNTRESULT_DRAWALACCOUNT_ERROR ) );					
		SetAllButtonAction(TRUE);
		break;
	}
}

