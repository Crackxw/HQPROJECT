#include <GSL.h>

#include	<Mouse.h>
#include	<clGame.h>

#include	"OnlineWorld.h"
#include	"OnlineMapStore-Parser.h"
#include	"OnlineMap.h"
#include	"OnlineTrade.h"
#include	"OnlineMegaText.h"
#include	"OnlineFont.h"
#include	"OnlineClient.h"
#include	"GSCDefine.h"
#include	"OnlineResource.h"
#include	"OnlineText.h"
#include	"OnlineMsgBox.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineFieldArea.h"
#include	"OnlineNumberControl.h"
#include	"OnlineItem.h"
#include	"OnlineCharUI.h"
#include	"OnlineHelp.h"
#include	"OnlineFieldChat.h"
#include	"OnlineKeyboard.h"
#include	"OnlineMercenary-Parser.h"
#include	"Onlineetc.h"
#include	"OnlineBooth.h"
#include	<Common.h>
#include	"OnlineSoldierTrade.h"
#include	"OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include	"OnlinePannel.h"
#include	"OnlineMercenary.h"
#include	"OnlinePortrait.h"


extern _InputDevice				IpD;
extern _clGame* pGame;

#define BUY_ERROR_MESSAGE_MONEY		0;
#define BUY_ERROR_MESSAGE_WEIGHT	1;
#define BUY_ERROR_MESSAGE_POSITION	2;

OnlineBooth::OnlineBooth()
{
	m_bActionIn				=	FALSE;
	m_bActionOut			=	FALSE;
	m_bMouseDown			=	FALSE;
	m_bField				=	FALSE;
	m_bOpen					=	FALSE;
	m_bInventory			=	FALSE;
	m_bFollowerMode			=	FALSE;

	m_pMyOnlineWorld		=	NULL;

//	m_pEditBox				=	NULL;
//	m_pPriceEditBox			=	NULL;
//	m_pDescEditBox			=	NULL;

	m_pSellStoreHeader		=	NULL;
	m_pBuyStoreHeader		=	NULL;
	pOnlineMsgBoxBench		=	NULL;
	m_pBoothList			=	NULL;
//	m_pPopUp				=	NULL;
	m_pSoldierTrade			=	NULL;

	m_StatusIn				=	ON_BENCH_IN_INIT;
	m_StatusOut				=	ON_BENCH_OUT_INIT;

	m_siNoField = -1;
	frame		= 0;
	m_siRet		= -1;
	m_uiAccount = 0;
	m_siSelect  = -1;
}

OnlineBooth::~OnlineBooth()
{
	Free();
}

VOID OnlineBooth::Initialize()
{
	m_bActionIn				=	FALSE;
	m_bActionOut			=	FALSE;
	m_bMouseDown			=	FALSE;
	m_bField				=	FALSE;
	m_bOpen					=	FALSE;
	m_bInventory			=	FALSE;
	m_bFollowerMode			=	FALSE;

	m_StatusIn				=	ON_BENCH_IN_INIT;
	m_StatusOut				=	ON_BENCH_OUT_INIT;

	m_siNoField = -1;
	frame		= 0;
	m_siRet		= -1;
	m_uiAccount = 0;
	m_siSelect  = -1; 

	for(SI16 i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bFollowerSlot[i] = FALSE;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bItemSlot[i] = FALSE;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bEnableSoldier[i] = FALSE;
//	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bCheckSoldier[i] = FALSE;

//	m_pEditBox->Initialize();
//	m_pPriceEditBox->Initialize();
//	m_pDescEditBox->Initialize();
	OnlineEditControlEx	*pOecEx;
	pOecEx	=	Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
	pOecEx->Clear();
	pOecEx	=	Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
	pOecEx->Clear();

}

// �ʱ�ȭ
VOID OnlineBooth::Init( cltOnlineWorld	*pOnlineWorld )
{
	m_pMyOnlineWorld = pOnlineWorld;

	m_pSlotSpr		 = m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_CHARACTERSLOT );

	clGrp.LoadXspr( "Online\\GameSpr\\inventory\\Sub_Button.Spr",		m_pExitButtonSpr);
	clGrp.LoadXspr( "Online\\GameSpr\\Trade\\SHOP_MAKE.Spr",			m_MainSellSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Trade\\SHOP.Spr",					m_MainBuySpr );
	clGrp.LoadXspr( "Online\\GameSpr\\village\\SMALL_WINDOW.Spr",		m_InfoSellSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\village\\SMALL_WINDOW.Spr",		m_InfoBuySpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Small_Window_Line.Spr",	m_InfoLineSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button1.Spr",		m_OkButtonSpr);
	clGrp.LoadXspr("Online\\GameSpr\\Inventory\\main_button2.Spr",		m_MercenarySpr);


	m_siX = 95;
	m_siY = 45;
	m_siInfoX = m_siX+30;
	m_siInfoY = m_siY + m_MainSellSpr.Header.Ysize;

	// ��ư�� �����.
	m_BFollower.Create	( m_siX + 128, m_siY + 105, m_MercenarySpr.Header.Xsize, m_MercenarySpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_SELECT), BUTTON_PUT_LEFT, TRUE );
	m_BInvenOk.Create	( m_siX + 64, m_siY + 132, m_OkButtonSpr.Header.Xsize, m_OkButtonSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_OK), BUTTON_PUT_LEFT, TRUE );
	m_BInvenExit.Create	( m_siX + 204, m_siY + 132, m_OkButtonSpr.Header.Xsize, m_OkButtonSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE );
	m_BOk.Create		( m_siInfoX + 22, m_siInfoY + 147, m_pExitButtonSpr.Header.Xsize, m_pExitButtonSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_OK ), BUTTON_PUT_LEFT, TRUE );
	m_BCancel.Create	( m_siInfoX + 210, m_siInfoY + 147,
		m_pExitButtonSpr.Header.Xsize, m_pExitButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE );
	m_BSoldierInfo.Create ( m_siInfoX +118, m_siInfoY + 147, 
		m_pExitButtonSpr.Header.Xsize, m_pExitButtonSpr.Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(8000493),BUTTON_PUT_LEFT, TRUE );
	m_BInvenBuyInfo.Create(m_siX + 138,m_siY + 102,m_OkButtonSpr.Header.Xsize, m_OkButtonSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(8000492), BUTTON_PUT_LEFT, TRUE );

	// ����Ʈ �ڽ��� ����� �ش�.
//	m_pEditBox			=	new OnlineEditControl;
//	m_pEditBox->CreateEditBox(1000, 1000, m_InfoSellSpr.Header.Xsize - 70, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_MAX_BOOTH_NOTE, TRUE, TRUE );
//	SendMessage( m_pEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)(ON_MAX_BOOTH_NOTE), 0 );

//	m_pPriceEditBox		= new OnlineEditControl;
//	m_pDescEditBox		= new OnlineEditControl;

//	m_pPriceEditBox->CreateEditBox( 1000, 1000, m_InfoSellSpr.Header.Xsize - 70, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 9, TRUE, FALSE );
//	m_pDescEditBox->CreateEditBox( 1000, 1000, m_InfoSellSpr.Header.Xsize, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, ON_MAX_BOOTH_NOTE );
//	SendMessage( m_pPriceEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)9, 0 );
//	SendMessage( m_pDescEditBox->GethWnd(), EM_LIMITTEXT, (WPARAM)(ON_MAX_BOOTH_NOTE), 0 );
//	m_pPriceEditBox->SetNextWindow( m_pDescEditBox->GethWnd() );
//	m_pDescEditBox->SetNextWindow( m_pPriceEditBox->GethWnd() );
	

	OnlineEditControlEx	*pOecEx;

	if( m_hOecPrice )	Handler_OnlineEditControlEx::KillControl( m_hOecPrice );
	RECT	rcRect  = { m_siInfoX + 40, m_siInfoY + 60, m_siInfoX + 200, m_siInfoY + 80};
	m_hOecPrice		= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 9,999999999);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(220, 176, 106) );

	if( m_hOecDesc )	Handler_OnlineEditControlEx::KillControl( m_hOecDesc );
	RECT	rcRect2  = { m_siInfoX + 40, m_siInfoY + 80, m_siInfoX + 300, m_siInfoY + 100};
	m_hOecDesc	= Handler_OnlineEditControlEx::CreateControl( 0, &rcRect2, ON_MAX_BOOTH_NOTE);
	pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
	pOecEx->SetFont( pOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetColor( RGB(220, 176, 106) );

	Handler_OnlineEditControlEx::SetNextHandle( m_hOecPrice, m_hOecDesc);
	Handler_OnlineEditControlEx::SetNextHandle( m_hOecDesc, m_hOecPrice);

	pOnlineMsgBoxBench = new OnlineMsgBox;
	pOnlineMsgBoxBench->Init(m_pMyOnlineWorld);

	// �����κ��丮 ����Ʈ ����
	m_pBoothList = new OnlineListControl(pOnlineWorld);
	m_pBoothList->Init(60, 60, 5, 1, m_siX + 10, m_siY + 33, 0);

	// �κ��丮 ���� ����
	SetRect(&m_rtBoothRect, m_siX + 10, m_siY + 33, m_siX + 320, m_siY + 93);
	SetRect(&m_rtInvenRect, 467, 280, 767, 400);

	for(SI16 i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bFollowerSlot[i] = FALSE;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bItemSlot[i] = FALSE;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bEnableSoldier[i] = FALSE;
//	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bCheckSoldier[i] = FALSE;


	m_pSoldierTrade = new OnlineSoldierTrade(pOnlineWorld, "Online\\GameSpr\\Inventory\\main.Spr");
}


// ������.
VOID OnlineBooth::Free()
{
	if( m_pSoldierTrade )
	{
		delete m_pSoldierTrade;
		m_pSoldierTrade = NULL;
	}

	if( m_pBoothList )
	{
		delete m_pBoothList;
		m_pBoothList = NULL;
	}

//	if( m_pEditBox )
//	{
//		m_pEditBox->Free();
//		delete m_pEditBox;
//		m_pEditBox = NULL;
//	}
	/*
	if( m_pPriceEditBox )
	{
		m_pPriceEditBox->Free();
		delete m_pPriceEditBox;
		m_pPriceEditBox = NULL;
	}
	if( m_pDescEditBox )
	{
		m_pDescEditBox->Free();
		delete m_pDescEditBox;
		m_pDescEditBox = NULL;
	}*/

	Handler_OnlineEditControlEx::KillControl( m_hOecPrice);
	Handler_OnlineEditControlEx::KillControl( m_hOecDesc);

	if(pOnlineMsgBoxBench)
	{		
		pOnlineMsgBoxBench->Initialize();
		delete pOnlineMsgBoxBench;
		pOnlineMsgBoxBench = NULL;
	}

/*	if(m_pPopUp)
	{
		clGrp.FreeXspr(m_pPopUp->PopSpr);
		delete m_pPopUp;
		m_pPopUp = NULL;
	}
*/
	if(m_pExitButtonSpr.Image)  {clGrp.FreeXspr(m_pExitButtonSpr);  m_pExitButtonSpr.Image = NULL;}
	if(m_MainSellSpr.Image)  {clGrp.FreeXspr(m_MainSellSpr);  m_MainSellSpr.Image = NULL;}
	if(m_MainBuySpr.Image)	 {clGrp.FreeXspr(m_MainBuySpr);   m_MainBuySpr.Image  = NULL;}
	if(m_InfoSellSpr.Image)  {clGrp.FreeXspr(m_InfoSellSpr);  m_InfoSellSpr.Image = NULL;}
	if(m_InfoBuySpr.Image)	 {clGrp.FreeXspr(m_InfoBuySpr);   m_InfoBuySpr.Image  = NULL;}
	if(m_InfoLineSpr.Image)  {clGrp.FreeXspr(m_InfoLineSpr);  m_InfoLineSpr.Image = NULL;}
	if(m_OkButtonSpr.Image)  {clGrp.FreeXspr(m_OkButtonSpr);  m_OkButtonSpr.Image = NULL;}
	if(m_MercenarySpr.Image)  {clGrp.FreeXspr(m_MercenarySpr);  m_MercenarySpr.Image = NULL;}
	
}

// ���
VOID OnlineBooth::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(m_bActionIn)
		DrawSellerInfo(pSurface);
	if(m_bActionOut)
	{
		DrawBuyerInfo(pSurface);
		m_pSoldierTrade->Draw(pSurface);
	}
}

VOID OnlineBooth::DrawSellerInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC	hDC;
	CItemHeader *pItem;
	char szTemp[256];
	ZeroMemory(szTemp, sizeof(szTemp));

	// ���� �κ��丮�� �׸���.
	if((m_StatusIn >= ON_BENCH_IN_BENCH_CLICK) && (m_StatusIn <= ON_BENCH_IN_ITEM_ADD))
	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_MainSellSpr.Header.Xsize, m_MainSellSpr.Header.Ysize, m_MainSellSpr.Image );
			
			if( m_bFollowerMode )	m_BFollower.Put( &m_MercenarySpr, 2, 2, 2, BUTTON_PUT_NOMOVE );
			else					m_BFollower.Put( &m_MercenarySpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BInvenOk.Put( &m_OkButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BInvenExit.Put( &m_OkButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );

			clGrp.UnlockSurface( pSurface );
		}
		
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode(hDC, TRANSPARENT);			
			SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
			
			// H2-B-01.20.05 //
			memset(szTemp, 0x0, 256);
			// ���� Ÿ��Ʋ�� '�ŷ� ����'���� 'xxx�� ����'���� ������ ĳ������ �������� ����
			sprintf(szTemp, "%s%s", m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000494));
			
			//m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 7,118,13, m_pMyOnlineWorld->pOnlineText->Get(8000494),RGB(200, 200, 200));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 8,118,13, szTemp,RGB(200, 200, 200));			
			// H2-E-01.20.05 //

			m_BFollower.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BInvenOk.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BInvenExit.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			pSurface->ReleaseDC( hDC );
		}
		
		// ���� �κ��丮 ������ �׸���
		m_pBoothList->Draw( pSurface );
	}

	// ������ ���� �Է�â�� �׸���.
	if(m_StatusIn == ON_BENCH_IN_INPUT_ITEM_INFO)
	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteJin( m_siInfoX, m_siInfoY, m_InfoSellSpr.Header.Xsize, m_InfoSellSpr.Header.Ysize, m_InfoSellSpr.Image );
			m_BOk.Put( &m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			//m_BSoldierInfo.Put(&m_pExitButtonSpr,0,2,1,BUTTON_PUT_NOMOVE );
			m_BCancel.Put( &m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			clGrp.UnlockSurface( pSurface );
		}

		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode(hDC, TRANSPARENT);			
			SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
			
			m_BOk.Put( hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BCancel.Put( hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			//m_BSoldierInfo.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));

			// ���� ���õ� ������ ���ϱ�
			ItemInfo *pTempItemInfo = NULL;
			
			if(m_siInvenSlot >= 0)		// �ڱ� �κ��丮���� �������� �������� ���� ���
				pTempItemInfo = const_cast<ItemInfo*>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
			else						// �������� ������, �뺴 ���� ������ ���
				pTempItemInfo = const_cast<ItemInfo*>(m_pBoothList->GetSlotInfo(m_siBoothSlot));

			// ������, �뺴 �̸�
			if(pTempItemInfo)
			{
				if(pTempItemInfo->siType == LIST_ITEM)								// �������� �ø��ų�, ����
				{
					pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(pTempItemInfo->uiItemID);
					if(pItem)				sprintf((char*)szTemp, "%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
				}
				else if(pTempItemInfo->siType == LIST_FOLLOWER)						// �뺴������ ����
				{
					char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)pTempItemInfo->Follower.uiSlot);
					if(pTempFollowerName)	sprintf((char*)szTemp, "%s", pTempFollowerName);
				}
			}
			else if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_FOLLOWER)		// �뺴�� �ø����
			{
				char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_TempSellItemInfo[m_siBoothSlot].Follower.uiSlot);
				if(pTempFollowerName)	sprintf((char*)szTemp, "%s", pTempFollowerName);
			}
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX+40, m_siInfoY+40, szTemp, RGB(220, 176, 106));

			// ����
			if(m_siSellItemPrice >= 0)
			{
				MakeMoneyText(m_siSellItemPrice, m_szSellItemPrice);
				sprintf((char*)szTemp, "%s%s", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_INFO_MONEYN), m_szSellItemPrice);
				//if( m_pPriceEditBox->IsFocus() )		
				if(Handler_OnlineEditControlEx::IsFocus(m_hOecPrice))
					if( rand()%100 > 30 )	strcat( szTemp, "_" );
			}
			else
			{
				sprintf((char*)szTemp, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_INFO_MONEYN));
				//if( m_pPriceEditBox->IsFocus() )
				if(Handler_OnlineEditControlEx::IsFocus(m_hOecPrice))
					if( rand()%100 > 30 )	strcat( szTemp, "_" );
			}
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX+40, m_siInfoY+60, szTemp, RGB(220, 176, 106));

			// ����
			//m_pDescEditBox->DrawText(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON), m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_BUTTON)
			//	, RGB(220, 176, 106), m_siInfoX + 40, m_siInfoY + 80, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_INFO_DESC), m_pDescEditBox->IsFocus());
			//m_pDescEditBox->DrawText(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON), m_pMyOnlineWorld->pOnlineFonts->GetFontUnderline(ON_FONT_BUTTON)
			//	, RGB(220, 176, 106), m_siInfoX + 40, m_siInfoY + 80, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_INFO_DESC), Handler_OnlineEditControlEx::IsFocus(m_hOecDesc));

			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX+40, m_siInfoY+80, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_INFO_DESC), RGB(220, 176, 106));
/*
			if(Handler_OnlineEditControlEx::IsFocus(m_hOecDesc))
			{
				OnlineEditControlEx	*pOecEx;
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
				// H2-B-01.20.05 //
				// ������ ������ ó�� 3����Ʈ�� ���ڸ� �Է��� �� ������ ���� 
				char sztemp[4];
				memcpy(sztemp, pOecEx->GetString(), 4);
				if((sztemp[0] > '0' && sztemp[0] <'9') || (sztemp[1] > '0' && sztemp[1] <'9') || (sztemp[2] > '0' && sztemp[2] <'9'))
				{				
					pOecEx->SetString("");
				}

				// H2-E-01.20.05 //
				pOecEx->SetRectOfDraw(m_siInfoX+80,m_siInfoY+80,m_siInfoX+300,m_siInfoY+95); 				
				pOecEx->Draw( hDC );
			}
*/
			OnlineEditControlEx	*pOecEx;
			pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
			// H2-B-01.20.05 //
			// ������ ������ ó�� 3����Ʈ�� ���ڸ� �Է��� �� ������ ���� 
			
			char szTemp[128], sztmp[32];			
			int	 nMoney[2] = { 100000000, 10000 };

			int	 iTempPrice2, iTempPrice= m_siSellItemPrice;
			
			memset(szTemp, 0x0, 128);
			memset(sztmp, 0x0, 32);

			if(iTempPrice < 10000)
				sprintf(szTemp, "%i%s", iTempPrice, m_pMyOnlineWorld->pOnlineText->Get(8000859));
			else
			{
				for(int i=0; i<3; i++)
				{
					if(i<2) iTempPrice2 = iTempPrice/nMoney[i];

					if(iTempPrice2 > 0)
					{
						sprintf(sztmp, "%i%s ", iTempPrice2, m_pMyOnlineWorld->pOnlineText->Get(8000857+i));
						if(i<2) iTempPrice = iTempPrice - (iTempPrice2*nMoney[i]);
						else 	sprintf(sztmp, "%i%s ", iTempPrice, m_pMyOnlineWorld->pOnlineText->Get(8000857+i));
						strcat(szTemp, sztmp);
						memset(sztmp, 0x0, 32);
					}
				}
			}
			pOecEx->SetString(szTemp);

			// H2-E-01.20.05 //
			pOecEx->SetRectOfDraw(m_siInfoX+80,m_siInfoY+80,m_siInfoX+300,m_siInfoY+95); 				
			pOecEx->Draw( hDC );



			pSurface->ReleaseDC( hDC );
		}
	}

	// �뺴���� �˾�â ��ο�
//	DrawPopup(pSurface);
}

VOID OnlineBooth::DrawBuyerInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;
	CHAR	TempBuffer[256];
	CHAR	szTemp[256];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));

	// ���� �κ��丮�� �׸���.
	if(m_StatusOut >= ON_BENCH_OUT_INVENTORY_POLL)
	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			clGrp.PutSpriteT( m_siX, m_siY, m_MainBuySpr.Header.Xsize, m_MainBuySpr.Header.Ysize, m_MainBuySpr.Image );
			//m_BInvenExit.Put( &m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE );
			m_BInvenBuyInfo.Put(&m_OkButtonSpr,0,2,1,BUTTON_PUT_NOMOVE);

			clGrp.UnlockSurface( pSurface );
		}
		
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode(hDC, TRANSPARENT);			
			SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
			
			// H2-B-01.20.05 //
			memset(szTemp, 0x0, 256);
			// ���� Ÿ��Ʋ�� '�ŷ� ����'���� 'xxx�� ����'���� ������ ĳ������ �������� ����
			sprintf(szTemp, "%s%s", m_pMyOnlineWorld->OnlineCharName(m_uiAccount), m_pMyOnlineWorld->pOnlineText->Get(8000494));
			
			//m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 7,118,13, m_pMyOnlineWorld->pOnlineText->Get(8000494),RGB(200, 200, 200));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 8,118,13, szTemp,RGB(200, 200, 200));			
			// H2-E-01.20.05 //
			
			//m_BInvenExit.Put( hDC );
			m_BInvenBuyInfo.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			pSurface->ReleaseDC( hDC );
		}

		// ���� �κ��丮 ������ �׸���
		m_pBoothList->Draw( pSurface );

		// �뺴����â�� �׸���.
		if(m_StatusOut == ON_BENCH_OUT_BUY_FOLLOWER)
		{
			UI16 uiFollowerKind;
			const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
			if(pTempItemInfo)
				uiFollowerKind = pTempItemInfo->Follower.uiKind;
			
			if(clGrp.LockSurface(pSurface) == TRUE)
			{
				// ���� ���
				clGrp.PutSpriteT(m_siInfoX, m_siInfoY, m_InfoLineSpr.Header.Xsize, m_InfoLineSpr.Header.Ysize, m_InfoLineSpr.Image);
				clGrp.PutSpriteJin(m_siInfoX, m_siInfoY, m_InfoBuySpr.Header.Xsize, m_InfoBuySpr.Header.Ysize, m_InfoBuySpr.Image);
				m_BOk.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_BCancel.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_BSoldierInfo.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				
				// ����ҷ��� �뺴 �� ���
				if(pTempItemInfo)
				{					
					//clGrp.PutSpriteT(m_siInfoX + 40, m_siInfoY + 45, m_pSlotSpr->Header.Xsize, m_pSlotSpr->Header.Ysize, m_pSlotSpr->Image);					
					// actdoll (2004/01/02 19:00) : �ʻ� ��� ����
//					m_pMyOnlineWorld->m_pMercenary->DrawPortait(m_siInfoX + 44, m_siInfoY + 50, m_pMyOnlineWorld->m_pMercenary->TransKindToFont(uiFollowerKind));
					m_pMyOnlineWorld->m_pPortrait->DrawAsNormalEx(pSurface, m_siInfoX + 44, m_siInfoY + 50,
						uiFollowerKind, TRUE, 60, 60);
				}
				clGrp.UnlockSurface(pSurface);
			}
			
			if(pSurface->GetDC(&hDC) == DD_OK)
			{
				SetBkMode(hDC, TRANSPARENT);
				SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));

				
				if(pTempItemInfo)
				{
					// Name
					UI32 TextID = m_pMyOnlineWorld->pIOnlineCharKI->GetName(m_pMyOnlineWorld->pIOnlineCharKI->GetIndex(uiFollowerKind));
					sprintf((char*)TempBuffer, "%s", m_pMyOnlineWorld->pOnlineText->Get(TextID));
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 20, TempBuffer, RGB(200, 200, 200));
					
					// Level
					sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_LV), pTempItemInfo->Follower.siLevel);
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 40, TempBuffer, RGB(200, 200, 200));
					
					// STR
					sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_BUY_STR), pTempItemInfo->Follower.siStr);
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 60, TempBuffer, RGB(200, 200, 200));
					
					// DEX
					sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_DEX), pTempItemInfo->Follower.siDex);
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 80, TempBuffer, RGB(200, 200, 200));
					
					// VIT
					sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_VIT), pTempItemInfo->Follower.siVit);
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 100, TempBuffer, RGB(200, 200, 200));
					
					// INT
					sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_TEXT_DISMISSAL_INT), pTempItemInfo->Follower.siInt);
					m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX + 145, m_siInfoY + 120, TempBuffer, RGB(200, 200, 200));
				}
				
				SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				m_BOk.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BCancel.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				m_BSoldierInfo.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
				
				pSurface->ReleaseDC(hDC);
			}
		}
		
		// ���콺�� �����.
		if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow() == FALSE)
		{
			if(m_pBoothList->GetDragSlot() != -1)	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( FALSE );
			// �̵����� �������� ���ٸ�
			else									m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
		}
		else										m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( TRUE );
	}
}

VOID OnlineBooth::ActionBench()
{
	m_siRet = pOnlineMsgBoxBench->Process();

	ActionInBench();
	ActionOutOfBench();	
	m_pSoldierTrade->Actoin();
	
	// ������ ���ǹ��� �����ߴ��� üũ�ϰ� ó��

	if( IpD.LeftPressSwitch )	m_bMouseDown = TRUE;
	else						m_bMouseDown = FALSE;	
}

VOID OnlineBooth::ActionOutOfBench()
{
	POINT	pt;
	
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	if(!m_bActionOut)	return;

	if(m_StatusOut != ON_BENCH_OUT_BUY_FOLLOWER && m_StatusOut != ON_BENCH_OUT_BUY_FOLLOWER_OK && 
		m_StatusOut != ON_BENCH_OUT_SOLDIER_INFO)
		m_siBoothSlot = m_pBoothList->GetDragSlot();
	
	switch(m_StatusOut)
	{
	case ON_BENCH_OUT_INIT:
		{
			// ������ ��� OnlineWorld���� �ٸ� ĳ�������ϴ��� ������.
		}
		break;
	case ON_BENCH_OUT_BENCH_CLICK:
		{
			frame++;

			if(frame > 100)
			{
				frame = 0;
				m_StatusOut = ON_BENCH_OUT_INIT;
			}
			// ĳ�� ����
			m_pMyOnlineWorld->bCharMove = FALSE;
		}
		break;
	case ON_BENCH_OUT_INVENTORY_POLL:
		{
			// �ڱ� �κ��丮 â�� ����. (���� �κ��丮�� �ڵ����� ����)
			if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);
			
			// ���콺�� ������ ���� �÷����ҳ�?
			if(m_pSoldierTrade->IsAction())		break;
			SI16 siSlot;
			if((siSlot = m_pBoothList->GetSelectSlot()) != -1)
			{				
				const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(siSlot);
				if(TempItemInfo)
				{
					if(!m_pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_SHIFT))
					{
						SellItemInfo *pItemInfo = new SellItemInfo;
						ZeroMemory(pItemInfo, sizeof(pItemInfo));
						
						if(TempItemInfo->siType == LIST_ITEM)
						{
							pItemInfo->siType			= LIST_ITEM;
							pItemInfo->Item.uiItemID	= TempItemInfo->uiItemID;
							pItemInfo->Item.uiItemCount	= TempItemInfo->ItemCount;
							ZeroMemory(&pItemInfo->Follower, sizeof(OnFollowerInfo));
						}
						else if(TempItemInfo->siType == LIST_FOLLOWER)
						{
							pItemInfo->siType			= LIST_FOLLOWER;
							CopyMemory(&pItemInfo->Follower, &TempItemInfo->Follower, sizeof(OnFollowerInfo));
						}
						pItemInfo->siItemPrice			= TempItemInfo->uiBuyPrice;
						strncpy(pItemInfo->szItemDesc, m_szBuyItemDesc[siSlot], ON_MAX_BOOTH_NOTE);
						
						m_pMyOnlineWorld->pOnlineHelp->SetTextBooth(pItemInfo, (SI16)pt.x, (SI16)pt.y);							

						delete pItemInfo;
					}
					else if(TempItemInfo->siType == LIST_ITEM)
					{
						CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem( TempItemInfo->uiItemID );
						if(pItem)	m_pMyOnlineWorld->pOnlineHelp->SetTextItem( pItem, const_cast<ItemInfo*>(TempItemInfo), (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN, FALSE, FALSE);
					}
					/*
					else if(TempItemInfo->siType == LIST_FOLLOWER)
					{
						const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(siSlot);
						m_pSoldierTrade->SetSoldier(const_cast<ItemInfo*>(TempItemInfo)->Follower, m_FollowerItemData[TempItemInfo->Follower.uiSlot]);
					}
					*/
				}
			}

/*			if(m_BSoldierInfo.Process(m_bMouseDown))
			{
				const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(siSlot);
				m_pSoldierTrade->SetSoldier(const_cast<ItemInfo*>(TempItemInfo)->Follower, m_FollowerItemData[TempItemInfo->Follower.uiSlot]);
			}
*/
			// ���� �κ��丮�� Ŭ���߳�?
			if(m_pMyOnlineWorld->fLeftOneSwitch == 1)
			{
				if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() == -1)
				{
					const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(siSlot);
					if(TempItemInfo)
					{
						if(TempItemInfo->siType == LIST_FOLLOWER)
						{
							m_siBoothSlot = siSlot;
							m_StatusOut = ON_BENCH_OUT_BUY_FOLLOWER;
						}
						else if(m_pBoothList->DragUp())
						{
							m_siBoothSlot = m_pBoothList->GetDragSlot();
							m_StatusOut = ON_BENCH_OUT_INVENTORY_CLICK;
						}
					}
				}
				else if(PtInRect(&m_rtBoothRect, pt) == TRUE)
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
			}
			
			// ������ ��ư�� ������?
			if(m_BInvenBuyInfo.Process(m_bMouseDown))
			{
				// ĳ�� ���� ����
				m_pMyOnlineWorld->bCharMove = TRUE;

				// ���ʵ� ����
				if( m_siNoField != -1 )
					m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField  );
				m_siNoField	=	-1;

				ClearBench();

				m_pSoldierTrade->SetHide();
				m_StatusOut = ON_BENCH_OUT_INIT;
			}
		}
		break;
	case ON_BENCH_OUT_INVENTORY_CLICK:
		{
			// �� �κ��丮�� �̵�
			if(m_pMyOnlineWorld->fLeftOneSwitch == 1)
			{
				if(PtInRect( &m_rtInvenRect, pt ) == TRUE)
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)
						m_pMyOnlineWorld->pOnlineInventory->DragFail();

					
					// ���Կ� ���� ����ִ� ���� ���ϱ�
					const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);

					if(pTempItemInfo)
					{
						if(pTempItemInfo->siType == LIST_ITEM)
						{
							m_uiBuyFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
							SI32 siResult = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(m_uiBuyFollowerID, pTempItemInfo->uiItemID);
							SI16 siItemCount = min((SI16)siResult, (SI16)pTempItemInfo->ItemCount);
							
							if(siItemCount > 0)			// ����ִ� ������ �ִٸ�
							{
								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(siItemCount);
								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
								m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(IpD.Mouse_X, IpD.Mouse_Y);
								m_StatusOut = ON_BENCH_OUT_BUY_ITEM;
							}
							else
							{
								m_pBoothList->StopDrag();
								m_pMyOnlineWorld->pOnlineInventory->DragFail();
								m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
								pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_WEIGHTOVER));
							}
						}
					}
					else
					{
						m_pBoothList->StopDrag();
						m_pMyOnlineWorld->pOnlineInventory->DragFail();
						m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
					}
				}
				else
				{
					m_pBoothList->StopDrag();
					m_pMyOnlineWorld->pOnlineInventory->DragFail();
					m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
				}
			}
		}
		break;
	case ON_BENCH_OUT_BUY_ITEM:
		{
			if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)
				m_pMyOnlineWorld->pOnlineInventory->DragFail();
			
			// ���� �Է��� �޴´�.
			if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE)
			{
				if( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0)
				{
					m_siBuyItemQuantity	= (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
					// �����˻�(������, �����ʰ�)
					if(ErrorMsg())
					{
						m_pBoothList->StopDrag();
						m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
					}
					else
					{
						// ������ �����ۿ� ���� ������ ��´�.
						m_siBuyItemQuantity				= (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
						const ItemInfo *TempItemInfo	= m_pBoothList->GetSlotInfo(m_siBoothSlot);
						// actdoll (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
						//	TempItemInfo�� NULL�� ������ ���� �ִ� ��Ȳ���� �ش� �����Ϳ� ����
						//	Ȯ���� ���� �����Ƿ� �＼�� ���̿÷��̼��� �߻��ߴ�. 
						//	���� ����Ʈ���� �߻��ߴ� �󵵼��� ���� �࿡ ���Ѵ�.
						//	���� �ϴ� TempItemInfo������ NULL�� �ƴҶ��� ���� ����� �����ϵ��� �۾��Ѵ�.
						//	���� ���� �����ÿ� �� �κп��� ���װ� �ȳ����ų� ��ġ�� ���� Ư���� ������ ���� ��� �� �ּ��� �����ص� ����.
						if( TempItemInfo )
						{
							m_siBuyItemID					= TempItemInfo->uiItemID;
							m_siInvenSlot					= m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(TempItemInfo->uiItemID);
							
							// ���� ����� ������ �˷��ش�.
							m_pMyOnlineWorld->pOnlineClient->SendBuyBoothItem(m_uiAccount, m_siBuyItemQuantity,
								(UI08)m_siBoothSlot, (UI08)m_siInvenSlot, (UI16)TempItemInfo->uiItemID, (UI08)m_uiBuyFollowerID);
							m_StatusOut = ON_BENCH_OUT_BUY_ITEM_OK;
						}
						else
						{
							// ������ ���� ��ġ�� �̵�
							m_pBoothList->StopDrag();
							
							m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
						}
					}
				}
				else
				{
					// ������ ���� ��ġ�� �̵�
					m_pBoothList->StopDrag();
					
					m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
				}
			}
		}
		break;
	case ON_BENCH_OUT_SOLDIER_INFO:

		break;

	case ON_BENCH_OUT_BUY_FOLLOWER:
		{
			if(m_BOk.Process(m_bMouseDown) == TRUE)
			{
				const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
				MONEY Money = m_pMyOnlineWorld->pMyData->GetMoney();
				if(Money < pTempItemInfo->uiBuyPrice)	
				{
					m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
					pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
				}
				else
				{
					// ���� ������ �ڸ��� ������ ��� ������ Error Message
					SI08 TempUnitCount = 0;
					for(SI08 TempCount = 1; TempCount < 12; TempCount++)
					{
						if(m_pMyOnlineWorld->pMyData->IsValidFollower(TempCount) == FALSE)	TempUnitCount++;
					}
					
					if(TempUnitCount <= 0)
					{
						m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_BUY_ERROR1));
						m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
					}
					else
					{
						char TempBuffer[256];
						SI16 MaxFollower = m_pMyOnlineWorld->pOnlineMercenaryParser->GetMaxFollowerNum((SI32)m_pMyOnlineWorld->pMyData->GetTradeGrade(), (SI32)m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(0));
						if((11 - TempUnitCount + 1) > MaxFollower)
						{
							sprintf((char*)TempBuffer, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BARRACK_NOWSOLIDER), MaxFollower);
							m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", (char*)TempBuffer);
							m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
						}
						else
						{
							// ���� ����� ������ �˷��ش�.
							m_pMyOnlineWorld->pOnlineClient->SendBuyBoothItem(m_uiAccount, 0, (UI08)m_siBoothSlot, 0, 0, 0);
							m_StatusOut = ON_BENCH_OUT_BUY_FOLLOWER_OK;
						}
					}
				}
			}
			else if(m_BSoldierInfo.Process(m_bMouseDown) == TRUE)
			{
				const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
				if(TempItemInfo)
				{
					m_pSoldierTrade->SetSoldier(const_cast<ItemInfo*>(TempItemInfo)->Follower, m_FollowerItemData[TempItemInfo->Follower.uiSlot]);
					m_StatusOut = ON_BENCH_OUT_SOLDIER_INFO;
				}
			}
			else if(m_BCancel.Process(m_bMouseDown) == TRUE)
			{
				m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
			}

		}
		break;
	case ON_BENCH_OUT_BUY_ITEM_OK:
		{
			frame++;

			if(frame > 100)
			{
				frame = 0;
				m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
			}
		}
		break;
	case ON_BENCH_OUT_BUY_FOLLOWER_OK:
		{
		}
		break;
	}
}

VOID OnlineBooth::ActionInBench()
{
	POINT		pt;
	UI08		i;

	pt.x	= IpD.Mouse_X;
	pt.y	= IpD.Mouse_Y;

	if(!m_bActionIn)	return;

	if(m_pMyOnlineWorld->pOnlineMap->m_MapIndex == 0 && m_StatusIn != ON_BENCH_IN_EXIT_WAIT)
		m_StatusIn = ON_BENCH_IN_EXIT;
	
	switch(m_StatusIn)
	{
	case ON_BENCH_IN_INIT:
		{
			//�κ��丮 â�� ����.
			m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
			
			for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
			{
				if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(i)) //�������� �ִٸ�
				{
					m_pBoothList->Clear();
					m_bInventory = TRUE;
					if(m_siNoField)
						m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
										m_MainSellSpr.Header.Xsize, m_siY + m_MainSellSpr.Header.Ysize);
					m_StatusIn	=	ON_BENCH_IN_BENCH_CLICK;
					break;
				}
			}
			
			if(m_StatusIn != ON_BENCH_IN_BENCH_CLICK)
			{
				if(m_bField)
					pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATENOTFINDITEM));
				else
					pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_NOTFINDITEM));
				m_StatusIn	=	ON_BENCH_IN_POLL;
			}
		}
		break;
	case ON_BENCH_IN_POLL:
		{
			m_pMyOnlineWorld->bCharMove = FALSE;

			if(m_bField)
			{
				if(m_pMyOnlineWorld->pOnlineMap->m_fHeroSelect && m_bMouseDown)
				{
					// �ڱ� �κ��丮 â�� ����.
					if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())
						m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);

					m_bInventory = TRUE;
					if(m_siNoField)
						m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
										m_MainSellSpr.Header.Xsize, m_siY + m_MainSellSpr.Header.Ysize);

					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				}
			}
			else
			{
				// ���ǿ� �������� �������� ������ Ŭ���ϴ��� �˻�
				if(m_bMouseDown && ClickBenchIn())
				{
					// �ڱ� �κ��丮 â�� ����.
					if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())
						m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);
					
					m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize();
					
					m_bInventory = TRUE;
					if(m_siNoField)
						m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
						m_MainSellSpr.Header.Xsize, m_siY + m_MainSellSpr.Header.Ysize);
					
					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				}
			}
		}
		break;
		
	case ON_BENCH_IN_BENCH_CLICK:
		{
			m_pMyOnlineWorld->bCharMove = FALSE;

			// �뺴���� �˾�â ����
			PopupAction();

			// ���콺�� ������ ���� �÷����ҳ�?
			SI16 siSlot;
			if((siSlot = m_pBoothList->GetSelectSlot()) != -1)
			{
				const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(siSlot);

				if(TempItemInfo)
				{
					SellItemInfo *pItemInfo = new SellItemInfo;
					ZeroMemory(pItemInfo, sizeof(pItemInfo));

					if(TempItemInfo->siType == LIST_ITEM)
					{
						pItemInfo->siType			= LIST_ITEM;
						pItemInfo->Item.uiItemCount	= TempItemInfo->ItemCount;
						pItemInfo->Item.uiItemID	= TempItemInfo->uiItemID;
						ZeroMemory(&pItemInfo->Follower, sizeof(OnFollowerInfo));
					}
					else if(TempItemInfo->siType == LIST_FOLLOWER)
					{
						pItemInfo->siType			= LIST_FOLLOWER;
						CopyMemory(&pItemInfo->Follower, &TempItemInfo->Follower, sizeof(OnFollowerInfo));
						//m_pSoldierTrade->SetSoldier(pItemInfo->Follower, NULL);
					}
					pItemInfo->siItemPrice			= TempItemInfo->uiSellPrice;
					sprintf(pItemInfo->szItemDesc, m_SellItemInfo[siSlot].szItemDesc);
					
					m_pMyOnlineWorld->pOnlineHelp->SetTextBooth(pItemInfo, (SI16)pt.x, (SI16)pt.y);					
					delete pItemInfo;
				}
			}

			// �뺴���� ��ư�� ������
			if(m_BFollower.Process(m_bMouseDown))
			{
				if(m_bFollowerMode)
				{					
					//if(m_pPopUp)
					//{
						//clGrp.FreeXspr(m_pPopUp->PopSpr);
						//delete m_pPopUp;
					//}

					//m_pPopUp    = NULL;
					m_bFollowerMode = FALSE;
					m_BInvenOk.SetAction(TRUE);
					m_BInvenExit.SetAction(TRUE);
				}
				else
				{
					m_BInvenOk.SetAction(FALSE);
					m_BInvenExit.SetAction(FALSE);
					m_BFollower.SetAction(FALSE);
					
					SetPopup();
				}
			}

			// ���� â�� Ȯ�� ��ư�� ������?
			if(m_BInvenOk.Process(m_bMouseDown))
			{
				UI08 uiNum = 0;
				for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
				{
					if(m_TempSellItemInfo[i].siType >= 0)	uiNum++;
				}

				// �κ��丮�� �÷��� �������� �ִٸ�
				if(uiNum)
				{
					// �κ��丮�� �ִ� ��� ������ ������ ������ ������.
					OnBoothItemForSell *pOnBoothItemForSell = new OnBoothItemForSell[uiNum];
					ZeroMemory( pOnBoothItemForSell, sizeof( OnBoothItemForSell ) * (uiNum) );
					uiNum = 0;
					for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
					{
						switch(m_TempSellItemInfo[i].siType)
						{
						case LIST_ITEM:
							{
								pOnBoothItemForSell[uiNum].uiTradeFollowerID = 0;
								pOnBoothItemForSell[uiNum].uiItemID		= m_TempSellItemInfo[i].Item.uiItemID;
								pOnBoothItemForSell[uiNum].uiFollowerID	= m_TempSellItemInfo[i].Item.uiFollowerID;
								pOnBoothItemForSell[uiNum].uiBoothSlot	= i;
								pOnBoothItemForSell[uiNum].Price		= m_TempSellItemInfo[i].siItemPrice;
								strncpy(pOnBoothItemForSell[uiNum].Desc, m_TempSellItemInfo[i].szItemDesc, ON_MAX_BOOTH_NOTE);
								uiNum++;
							}
							break;
						case LIST_FOLLOWER:
							{
								pOnBoothItemForSell[uiNum].uiTradeFollowerID = m_TempSellItemInfo[i].Follower.uiSlot;
								pOnBoothItemForSell[uiNum].uiItemID		= 0;
								pOnBoothItemForSell[uiNum].uiFollowerID	= 0;
								pOnBoothItemForSell[uiNum].uiBoothSlot	= i;
								pOnBoothItemForSell[uiNum].Price		= m_TempSellItemInfo[i].siItemPrice;
								strncpy(pOnBoothItemForSell[uiNum].Desc, m_TempSellItemInfo[i].szItemDesc, ON_MAX_BOOTH_NOTE);
								uiNum++;
							}
							break;
						default:	break;
						}
					}
					
					// ���� �ӽ�
					if(!m_bOpen)
						m_pMyOnlineWorld->pOnlineClient->SendOpenBoothMsg(uiNum, pOnBoothItemForSell);
					else
						m_pMyOnlineWorld->pOnlineClient->SendChangeBoothItemMsg(uiNum, pOnBoothItemForSell);

					delete [] pOnBoothItemForSell;
					
					m_StatusIn = ON_BENCH_IN_ITEM_ADD;
				}
				else	m_StatusIn = ON_BENCH_IN_POLL;

				m_bInventory = FALSE;
				if( m_siNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField  );
				m_siNoField	=	-1;

				break;
			}
			
			// ���� â�� ������ ��ư�� ������?
			if(m_BInvenExit.Process(m_bMouseDown))
			{
				m_pSoldierTrade->SetHide();
				for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
				{
					// BoothList->ItemInfo���� �ӽ÷� ���Ե� �������� �����Ѵ�.
					if(m_TempSellItemInfo[i].siType == LIST_ITEM)
					{
						m_bItemSlot[m_TempSellItemInfo[i].Item.uiFollowerID] = FALSE;
						m_pBoothList->DeleteSlot(i);
					}
					else if(m_TempSellItemInfo[i].siType == LIST_FOLLOWER)
					{
						m_bFollowerSlot[m_TempSellItemInfo[i].Follower.uiSlot] = FALSE;
						m_pBoothList->DeleteSlot(i);
					}
				}

				// H2-B-01.18.05 //
				// ���ǿ� �÷����� ������ �����Ϸ��� ����ϴ� ��� 
				// ���� ���� ������ ����(m_SellItemInfo[])�� �ʱ�ȭ ���ش�.
				for(i=0; i<ON_MAX_BOOTH_ITEM; i++)
				{					
					// �÷����� ������ �����ҷ��ٰ� ����ϴ� ��� ������ pBoothList�� �ٽ� ����.
					if(m_SellItemInfo[i].siType == 0 || m_SellItemInfo[i].siType == 1)
					{					
						switch(m_SellItemInfo[i].siType)
						{
						case LIST_ITEM:
							{
								ItemInfo TempItemInfo;
								ZeroMemory(&TempItemInfo, sizeof(ItemInfo));
								TempItemInfo.siType			= LIST_ITEM;
								TempItemInfo.uiItemID		= m_SellItemInfo[i].Item.uiItemID;
								TempItemInfo.ItemCount		= m_SellItemInfo[i].Item.uiItemCount;
								TempItemInfo.uiSellPrice	= m_SellItemInfo[i].siItemPrice;
								m_pBoothList->AddItem(TempItemInfo, i);
							}
							break;
						case LIST_FOLLOWER:
							{
								ItemInfo TempItemInfo;
								TempItemInfo.siType				= LIST_FOLLOWER;
								TempItemInfo.uiItemID			= 1;
								TempItemInfo.ItemCount			= 1;
								TempItemInfo.uiSellPrice		= m_SellItemInfo[i].siItemPrice;
								CopyMemory(&TempItemInfo.Follower, &m_SellItemInfo[i].Follower, sizeof(OnFollowerInfo));
								m_pBoothList->AddItem(TempItemInfo, i);
							}
							break;
						default:	break;
						}
					}
				}
				// H2-E-01.18.05 //

				// m_TempItemInfo �ʱ�ȭ
				InitTempItemInfo();

				m_bInventory = FALSE;
				if( m_siNoField != -1 )	m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField  );
				m_siNoField	=	-1;

				m_StatusIn = ON_BENCH_IN_POLL;
				break;
			}

			if(IpD.LeftPressSwitch)
			{
				if(m_pMyOnlineWorld->pOnlineInventory)
				{
					m_siInvenSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot();

					// ���� �κ��丮�� Ŭ���߳�?
					if(m_pBoothList)
					{
						if(m_pBoothList->GetSelectSlot() != -1)
						{
							// ���� //////////////////////////////////////////////
							// robypark 2004/12/2 16:53
							// ������ ������ �������� ���ǿ� �ø� �� ����.
							if (-1 != m_siInvenSlot)
							{
								const ItemInfo *pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));

								if(pTempItemInfo)
								{
									if(pTempItemInfo->siType == LIST_ITEM)
									{
										CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pTempItemInfo->uiItemID);

										// ������ ������ �̶��
										if (pItemHeader->m_pImperium)
										{
											// ���ǿ� �ø� �� ����.
											m_siInvenSlot = -1;
											m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
											break;
										}
									}
								}
							}
							// �� //////////////////////////////////////////////

							m_siBoothSlot = m_pBoothList->GetSelectSlot();
							m_pMyOnlineWorld->pOnlineInventory->DragFail();
							m_StatusIn = ON_BENCH_IN_INVENTORY_CLICK;
						}
					}
				}
			}			
		}
		break;
	case ON_BENCH_IN_INVENTORY_CLICK:
		{			
			// �κ��丮�� �������� ���ǿ� �÷�������
			if(m_siInvenSlot != -1)
			{
				// �Ĵ� �����ۿ� ���� �������� �ӽ÷� �����Ѵ�.
				m_uiSellFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
				
				// Ŭ���� �ڸ��� �������� �����Ѵٸ�
				if( m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1 )
					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				else
				{
					m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
					
					// �̹� �÷����� �������� �ٽ� �����Ѱ� �ƴ��� �˻�
					ItemInfo const *pTempItemInfo = NULL;
					pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
					if(CheckSameItem(m_uiSellFollowerID, pTempItemInfo->uiItemID))
					{
						if(m_bField)
							pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ISITEM));
						else
							pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH),m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ISITEM));
						m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
					}
					else
					{
						// ������ ����, ���� �Է�â�� ����.
						SetActiveSellInfo(TRUE);
						
						m_StatusIn = ON_BENCH_IN_INPUT_ITEM_INFO;
					}
				}
			}
			else
			{
				// ������ Ŭ���� �ڸ��� �������� �����Ѵٸ�
				if( m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1 )
				{
					// ������ ������ ������ �����ְ� �����ϰ� �Ѵ�.
					SetActiveSellInfo(TRUE);
					
					m_StatusIn = ON_BENCH_IN_INPUT_ITEM_INFO;
				}
				else
				{
					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				}
			}
		}
		break;
	case ON_BENCH_IN_INPUT_ITEM_INFO: // ������ �������� Ŭ������ ���
		{
			OnlineEditControlEx	*pOecEx;
			// �Ĵ� �����ۿ� ���� �������� �ӽ÷� �����Ѵ�.
			ZeroMemory(m_szSellItemDesc, sizeof(m_szSellItemDesc));
			if((m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1) && !frame)
			{
				// Ŭ���� �ڸ��� ǰ��(������, �뺴)�� �����Ѵٸ�
				if(m_SellItemInfo[m_siBoothSlot].siType >= 0)
				{
					sprintf(m_szSellItemPrice, "%d", m_SellItemInfo[m_siBoothSlot].siItemPrice);
					strncpy(m_szSellItemDesc, m_SellItemInfo[m_siBoothSlot].szItemDesc, ON_MAX_BOOTH_NOTE);
				}
				else
				{
					sprintf(m_szSellItemPrice, "%d", m_TempSellItemInfo[m_siBoothSlot].siItemPrice);
					strncpy(m_szSellItemDesc, m_TempSellItemInfo[m_siBoothSlot].szItemDesc, ON_MAX_BOOTH_NOTE);
				}
				
				pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
				pOecEx->SetString(m_szSellItemPrice);
				pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
				pOecEx->SetString(m_szSellItemDesc);

				//m_pPriceEditBox->SetString(m_szSellItemPrice);
				//m_pDescEditBox->SetString(m_szSellItemDesc);

				frame++;
			}
			pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
			strcpy(m_szSellItemPrice,pOecEx->GetString());
			//m_pPriceEditBox->GetString(m_szSellItemPrice);
			m_siSellItemPrice = atoi(m_szSellItemPrice);
			//m_pDescEditBox->GetString(m_szSellItemDesc);
			pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
			strcpy(m_szSellItemDesc,pOecEx->GetString());

			//if(m_pPriceEditBox->IsDoneMessage() == TRUE)
			pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
			if(pOecEx->IsPushEnter())
			{
				Handler_OnlineEditControlEx::SetFocus( m_hOecDesc );
				//m_pDescEditBox->SetFocus();
				//m_pPriceEditBox->InitDoneMessage();
			}

			BOOL bOk = FALSE;
			pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
			if(pOecEx->IsPushEnter() == TRUE)
			//if(m_pDescEditBox->IsDoneMessage() == TRUE)
			{
				bOk = TRUE;
				//m_pDescEditBox->InitDoneMessage();
			}
			// ���� & ���� �Է�â�� Ȯ�ι�ư
			if(m_BOk.Process(m_bMouseDown) || bOk)
			{
				if(m_siSellItemPrice <= 0 || m_siSellItemPrice > ON_MAX_MONEY)
				{
					if(m_bField)	pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH),m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_REDEFINEPRICE));
					else			pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH),m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_REDEFINEPRICE));

/*					if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_FOLLOWER)
					{
						ZeroMemory(&m_TempSellItemInfo[m_siBoothSlot], sizeof(SellItemInfo));
						m_TempSellItemInfo[m_siBoothSlot].siType = LIST_NOT;
					}
*/
					SetActiveSellInfo(FALSE);

					m_StatusIn = ON_BENCH_IN_ITEM_WAIT;
					break;
				}

				if(!CheckLeadByte(m_szSellItemDesc))
				{
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8000479));
					break;
				}

				// ���������� �Է�â�� ����.
				SetActiveSellInfo(FALSE);

				ItemInfo const *pTempItemInfo = NULL;
				pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
				if(!pTempItemInfo)	pTempItemInfo = const_cast<const ItemInfo *>(m_pBoothList->GetSlotInfo(m_siBoothSlot));

				// m_TempSellItemInfo�� ������ ���� ����
				if(m_TempSellItemInfo[m_siBoothSlot].siType != LIST_FOLLOWER)
				{
					if(pTempItemInfo)
					{
						// ���κ��丮 �������� �巡���ؼ� ���ǿ� �÷��� ���
						m_TempSellItemInfo[m_siBoothSlot].Item.uiItemID		= pTempItemInfo->uiItemID;
						m_TempSellItemInfo[m_siBoothSlot].Item.uiItemCount	= pTempItemInfo->ItemCount;
						m_TempSellItemInfo[m_siBoothSlot].siType			= pTempItemInfo->siType;
						m_TempSellItemInfo[m_siBoothSlot].Item.uiFollowerID	= m_uiSellFollowerID;
						if(pTempItemInfo->siType == LIST_FOLLOWER)
							CopyMemory(&m_TempSellItemInfo[m_siBoothSlot].Follower, &pTempItemInfo->Follower, sizeof(OnFollowerInfo));
					}
				}
				m_TempSellItemInfo[m_siBoothSlot].siItemPrice = m_siSellItemPrice;
				ZeroMemory(m_TempSellItemInfo[m_siBoothSlot].szItemDesc, sizeof(m_TempSellItemInfo[m_siBoothSlot].szItemDesc));
				strncpy(m_TempSellItemInfo[m_siBoothSlot].szItemDesc, m_szSellItemDesc, ON_MAX_BOOTH_NOTE);

				// ���� �κ��丮�� �������� �����Ѵ�.
				ItemInfo TempItemInfo;
				ZeroMemory(&TempItemInfo, sizeof(ItemInfo));
				TempItemInfo.siType			= m_TempSellItemInfo[m_siBoothSlot].siType;
				TempItemInfo.uiItemID		= m_TempSellItemInfo[m_siBoothSlot].Item.uiItemID;
				TempItemInfo.ItemCount		= m_TempSellItemInfo[m_siBoothSlot].Item.uiItemCount;
				TempItemInfo.uiSellPrice	= m_TempSellItemInfo[m_siBoothSlot].siItemPrice;
				CopyMemory(&TempItemInfo.Follower, &m_TempSellItemInfo[m_siBoothSlot].Follower, sizeof(OnFollowerInfo));

				m_pBoothList->AddItem(TempItemInfo, m_siBoothSlot);

				if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_ITEM)		
				{
					m_bItemSlot[m_TempSellItemInfo[m_siBoothSlot].Item.uiFollowerID] = TRUE;
					m_bEnableSoldier[m_TempSellItemInfo[m_siBoothSlot].Item.uiFollowerID] = FALSE;
				}
				if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_FOLLOWER)	
				{
					m_bFollowerSlot[m_TempSellItemInfo[m_siBoothSlot].Follower.uiSlot] = TRUE;
					m_bEnableSoldier[m_siSelect] = FALSE;
				}
				m_siInvenSlot = -1;
				frame = 0;
//				m_bCheckSoldier[m_siSelect] = TRUE;
				m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
			}
			// ���� �Է�â�� ��� ��ư
			else if(m_BCancel.Process(m_bMouseDown))
			{
				if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_FOLLOWER)
				{
					ZeroMemory(&m_TempSellItemInfo[m_siBoothSlot], sizeof(SellItemInfo));
					m_TempSellItemInfo[m_siBoothSlot].siType = LIST_NOT;
				}

				SetActiveSellInfo(FALSE);
				frame = 0;
				m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
			}
		}
		break;
	case ON_BENCH_IN_ITEM_WAIT:
		if(m_siRet == ON_MSGBOX_RETURN_OK) 
		{
			m_StatusIn = ON_BENCH_IN_INPUT_ITEM_INFO;
			SetActiveSellInfo(TRUE);
		}
		break;
	case ON_BENCH_IN_ITEM_ADD:			
			m_StatusIn	=	ON_BENCH_IN_POLL;
		break;

	case ON_BENCH_IN_EXIT_INIT:
		{
			m_pMyOnlineWorld->bCharMove = FALSE;
			if(m_siRet == ON_MSGBOX_RETURN_OK)		m_StatusIn = ON_BENCH_IN_EXIT;
			else if(m_siRet == ON_MSGBOX_RETURN_CANCEL)
			{
				if(m_bInventory)
				{
					if(m_siNoField)
						m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + m_MainSellSpr.Header.Xsize, m_siY + m_MainSellSpr.Header.Ysize);
					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				}
				else	m_StatusIn = ON_BENCH_IN_POLL;
			}
		}
		break;
	case ON_BENCH_IN_EXIT:
		if(m_bOpen)
		{
			// �������� �����ٰ� �����ش�.
			m_pMyOnlineWorld->pOnlineClient->SendLeaveBoothMsg();
		}

		m_StatusIn = ON_BENCH_IN_EXIT_WAIT;
		break;
	case ON_BENCH_IN_EXIT_WAIT:
		{
			if(!m_bOpen)
			{
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), 0);

				if(!m_bField)
					m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(m_pSellStoreHeader->siMapStoreCode);

				m_bOpen = FALSE;
				m_pMyOnlineWorld->bCharMove = TRUE;
				SetActionIn(FALSE);
				SetActionOut(TRUE);
				::SetFocus(pGame->Hwnd);
			}
		}
		break;
	}	
}

// TRUE�� ���ϵǸ� ���ǿ��� ������.~
BOOL OnlineBooth::MoveOutBench(SI16 siHeroX, SI16 siHeroY)
{
	if( m_pSellStoreHeader == NULL )	return TRUE;
	
	if( m_pSellStoreHeader->siMapStoreMapXStartPosition > siHeroX )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapXEndPosition < siHeroX )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapYStartPosition > siHeroY )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapYEndPosition < siHeroY )		return TRUE;

	return FALSE;
}

// ���ǿ� �ִ� �������� �ʱ�ȭ �Ѵ�.
VOID OnlineBooth::ClearBench()
{
	if(m_pBoothList)		m_pBoothList->Clear();

	InitTempItemInfo();

	m_siInvenSlot		= -1;
	m_siBoothSlot		= -1;
	m_pSellStoreHeader	= NULL;
	m_bField			= FALSE;
	m_uiAccount			= 0;

	ZeroMemory(m_szBoothName, sizeof(m_szBoothName));
	ZeroMemory(m_szSellItemDesc, sizeof(m_szSellItemDesc));
	ZeroMemory(m_szBuyItemDesc, sizeof(m_szBuyItemDesc));

	if( m_pMyOnlineWorld )		m_pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
}

VOID OnlineBooth::InitTempItemInfo()
{
	for(int i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		ZeroMemory(&m_TempSellItemInfo[i], sizeof(SellItemInfo));
		m_TempSellItemInfo[i].siType = -1;
	}
}

// ���� Ŭ���� �������� �ֵ� TRUE�� ������...
BOOL OnlineBooth::ClickBenchIn()
{
	SI32 siX, siY;
	SI16 siTemp;
	
	MapStoreHeader *pTempHeader;

	m_pMyOnlineWorld->pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);
	pTempHeader	=	m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser(siX, siY, &siTemp);

	// ���� ���ǿ� ��� ���� pTempHeader�� ���ǿ� ���� ������ ������ �ȴ�.
	if(pTempHeader)
	{
		if(pTempHeader->siMapStoreCode == m_pSellStoreHeader->siMapStoreCode)
		{
			// �������� �巡�� ������ �˾� ����.
			if( m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() == -1 )		return	TRUE;
		}
	}

	return FALSE;
}

BOOL OnlineBooth::WhererU(SI16 siHeroX, SI16 siHeroY)
{
	// ���� ����� ������ �޾Ƽ� ���ǿ� ��� ���� �˻縦 �ϸ� ����,,,
	SI16 TempSelect;
	m_pSellStoreHeader = m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );

	// ���ǿ� ��� ���� ����
	if( m_pSellStoreHeader )
	{
		m_pMyOnlineWorld->pIOnlineChar->SetMove(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(),siHeroX, siHeroY);
		SetActionIn(TRUE);
		SetActionOut(FALSE);
		return TRUE;
	}
	else
	{
		SetActionOut(TRUE);
	}
	return FALSE;
}

VOID OnlineBooth::SetActionIn(BOOL bFlag)
{
	SI08 i;
	m_bActionIn = bFlag;

	if(m_bActionIn)
	{
		//m_BOk.SetY(m_siInfoY + 86);
		//m_BCancel.SetY(m_siInfoY + 86);
		InitTempItemInfo();
		
		for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		{
			ZeroMemory(&m_SellItemInfo[i], sizeof(SellItemInfo));
			m_SellItemInfo[i].siType = -1;
		}
		
		// ĳ�� ����, ���ʵ� ����

		if(! m_pMyOnlineWorld->m_pMercenary->IsAction())
			m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
		m_pMyOnlineWorld->bCharMove = FALSE;
		//m_pPriceEditBox->EnableWindow(TRUE);
		//m_pDescEditBox->EnableWindow(TRUE);

		m_StatusIn = ON_BENCH_IN_INIT;
	}
	else
	{
		ClearBench();

		// ĳ�� ������, ���ʵ� ���� ����
		m_pMyOnlineWorld->bCharMove = TRUE;
		m_bField = FALSE;
		//m_pPriceEditBox->EnableWindow(FALSE);
		//m_pDescEditBox->EnableWindow(FALSE);

		if( m_siNoField != -1 )
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField  );
		m_siNoField	= -1;

		// �뺴 �˾� â
/*		if(m_pPopUp)
		{
			clGrp.FreeXspr(m_pPopUp->PopSpr);
			delete m_pPopUp;
			m_pPopUp = NULL;
		}
*/
		m_bFollowerMode	= FALSE;
		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bFollowerSlot[i] = FALSE;
		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bItemSlot[i] = FALSE;

		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bEnableSoldier[i] = FALSE;
//		for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bCheckSoldier[i] = FALSE;



		m_StatusIn = ON_BENCH_IN_EXIT_WAIT;
	}
}

VOID OnlineBooth::SetActionOut(BOOL bFlag)
{
	if(m_pMyOnlineWorld->pOnlineTrade->IsActive())	return;

	m_bActionOut = bFlag;
	if(bFlag)
	{
		//m_BOk.SetY(m_siInfoY + 147);
		//m_BCancel.SetY(m_siInfoY + 147);
	}
}

BOOL OnlineBooth::ClickBenchOut(SI32 siItemX, SI32 siItemY)
{
	SI16 siTemp;
	m_pBuyStoreHeader =	m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser(siItemX, siItemY, &siTemp);

	if(m_pBuyStoreHeader)	return TRUE;
	else					return FALSE;
}

VOID OnlineBooth::SetActiveSellInfo(BOOL active)
{
	if(active)
	{
		// �Ĵ� ������ ����, ���� �Է�â�� ����.
		//m_pPriceEditBox->EnableWindow( TRUE );
		//m_pDescEditBox->EnableWindow( TRUE );
		//m_pPriceEditBox->SetFocus();
		Handler_OnlineEditControlEx::SetFocus( m_hOecPrice );
		m_pMyOnlineWorld->SetShortKey( TRUE );
	}
	else
	{
		OnlineEditControlEx	*pOecEx;
		pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
		pOecEx->Clear();
		pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecDesc );
		pOecEx->Clear();

		//m_pPriceEditBox->SetString("");
		//m_pDescEditBox->SetString("");
		//m_pPriceEditBox->EnableWindow( FALSE );
		//m_pDescEditBox->EnableWindow( FALSE );
		m_pMyOnlineWorld->SetShortKey( FALSE );
	}
}

VOID OnlineBooth::SetActiveBuyInfo(BOOL active)
{
	if(active)
	{
		// ��� ������ ���� �Է�â�� ����.
//		m_pEditBox->EnableWindow( TRUE );
//		m_pEditBox->SetFocus();
		m_pMyOnlineWorld->SetShortKey( TRUE );
	}
	else
	{
//		m_pEditBox->SetString("");
//		m_pEditBox->EnableWindow( FALSE );
		m_pMyOnlineWorld->SetShortKey( FALSE );
	}
}

BOOL OnlineBooth::ErrorMsg()
{
	// ���� ���Ը� �˻��Ͽ� �� ��ŭ ��� �ִ��� �˻�
	const ItemInfo *TempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
	if(TempItemInfo == NULL)		return FALSE;

	CItemHeader *pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(TempItemInfo->uiItemID);	
	if(pItem == NULL)				return FALSE;

	if(!m_pMyOnlineWorld->pMyData->GetFollowersInterface()->CanAddItem(m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot(),
			(UI16)TempItemInfo->uiItemID, (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount))
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_WEIGHTOVER));
		
		return	TRUE;
	}

	// �κ��丮�� ������ �ִ��� �˻�
	SI16 siSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(TempItemInfo->uiItemID);
	if(siSlot == -1)
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTITEMSLOT));
		return TRUE;
	}

	// ������ �˻�
	MONEY Money = m_pMyOnlineWorld->pMyData->GetMoney();
	if(Money < m_siBuyItemQuantity * TempItemInfo->uiBuyPrice)
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTMONEY));
		
		return	TRUE;
	}

	return FALSE;
}

BOOL OnlineBooth::CheckSameItem(UI08 uiFollowerID, UI16 uiItemID)
{
	// �÷����� �뺴�� �������� �ƴѰ� üũ
	if(m_bFollowerSlot[uiFollowerID])	return TRUE;

	// �̹� �÷����� �������� �ƴѰ� üũ
	for(int i=0; i<ON_MAX_BOOTH_ITEM; i++)
	{
		if((m_SellItemInfo[i].Item.uiFollowerID == uiFollowerID) && (m_SellItemInfo[i].Item.uiItemID == uiItemID))
			return TRUE;
		if((m_TempSellItemInfo[i].Item.uiFollowerID == uiFollowerID) && (m_TempSellItemInfo[i].Item.uiItemID == uiItemID))
			return TRUE;
	}
	return FALSE;
}

VOID	OnlineBooth::SetActionFieldBooth(BOOL bFlag)
{
	m_bField = bFlag;
}

VOID	OnlineBooth::SetBoothSpeech(UI16 uiAccount, OnBaseBoothItem* pOnBaseBoothItem, UI08 uiBoothItemNum)
{
	CHAR buf1[256], buf2[256];
	ZeroMemory(buf1, sizeof(buf1));
	ZeroMemory(buf2, sizeof(buf2));
	int i;
	
	SI16 TempSelect, siHeroX, siHeroY;
	m_pMyOnlineWorld->pIOnlineChar->GetPos(uiAccount, &siHeroX, &siHeroY);
	MapStoreHeader *pStoreHeader = m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );
	if(pStoreHeader)
	{
		// �����̶�� ���ǿ� ������ �̹��� �÷�����
		m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.SetBoothData(pStoreHeader->siMapStoreCode, pStoreHeader->siMapStoreMapXPosition, 
			pStoreHeader->siMapStoreMapYPosition, pStoreHeader->siMapStoreMapIndex, pOnBaseBoothItem, uiBoothItemNum);
	}

	SI08 siFirst = 0;
	for(i = 0; i < uiBoothItemNum; i++)
	{
		if(pOnBaseBoothItem[i].siType == LIST_ITEM)
		{
			CItemHeader	*pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(pOnBaseBoothItem[i].uiProductID);
			if(pItem)
			{
				if(!siFirst)	sprintf(buf2, "%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
				else			sprintf(buf2, "	%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
				strcat(buf1, buf2);
				siFirst++;
			}
		}
		else if(pOnBaseBoothItem[i].siType == LIST_FOLLOWER)
		{
			UI32 TextID = m_pMyOnlineWorld->pIOnlineCharKI->GetName(m_pMyOnlineWorld->pIOnlineCharKI->GetIndex(pOnBaseBoothItem[i].uiProductID));
			if(TextID <= 0)	continue;

			if(!siFirst)	sprintf(buf2, "%s", m_pMyOnlineWorld->pOnlineText->Get(TextID));
			else			sprintf(buf2, "	%s", m_pMyOnlineWorld->pOnlineText->Get(TextID));
			strcat(buf1, buf2);
			siFirst++;
		}
	}

	m_pMyOnlineWorld->pIOnlineChar->SetSpeech(uiAccount, buf1, TRUE);	
}

BOOL	OnlineBooth::IsOpen()
{
	return m_bOpen;
}

VOID	OnlineBooth::SetFocusEditBox()
{
	//if(!m_pPriceEditBox->IsFocus() && !m_pDescEditBox->IsFocus())
	//	m_pPriceEditBox->SetFocus();
	if(!Handler_OnlineEditControlEx::IsFocus(m_hOecPrice) && !Handler_OnlineEditControlEx::IsFocus(m_hOecDesc))
		Handler_OnlineEditControlEx::SetFocus( m_hOecPrice );

}

// �뺴 �ø��� �˾�â
VOID	OnlineBooth::SetPopup()
{
	//m_pPopUp = new PopUp;

	//clGrp.LoadXspr("Online\\GameSpr\\TradePopup.Spr", m_pPopUp->PopSpr);
	//m_pPopUp->siSelect = -1;
	//m_pPopUp->bType = TRUE;

	// �뺴â
	//if(!m_pPopUp->bType)	return;

	//XSPR *pScroll = m_pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NOTICE_SCROLL_BAR);
	//m_pPopUp->siX = m_BFollower.GetX();// + m_BFollower.GetWidth() - m_pPopUp->PopSpr.Header.Xsize;
	//m_pPopUp->siY = m_BFollower.GetY() + m_BFollower.GetHeight(); //- m_pPopUp->PopSpr.Header.Ysize;
	
	//m_pPopUp->Slot.Create(m_pPopUp->siX, m_pPopUp->siY, m_pPopUp->PopSpr.Header.Xsize, m_pPopUp->PopSpr.Header.Ysize, m_pPopUp->siX + 5, 
	//	m_pPopUp->siY + 5, m_pPopUp->PopSpr.Header.Xsize, m_pPopUp->PopSpr.Header.Ysize, m_pPopUp->PopSpr.Header.Xsize-30, 20, NULL, 
	//	pScroll, m_pPopUp->siX + 84, m_pPopUp->siY+5, m_pPopUp->siY + m_pPopUp->PopSpr.Header.Ysize );
	
	SI16 siAdd = 0;
	// ��ȯ ������ �뺴�� �־� �ش�.
	for( SI16 i = 1; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		//m_pPopUp->Follower[siAdd] = (UI08)i;
		
		if(!m_bFollowerSlot[i] && !m_bItemSlot[i])
		{
			SI32 siKind = m_pMyOnlineWorld->pMyData->GetFollowerParameterKind((UI08)i);
			SI16 siGeneral = m_pMyOnlineWorld->pIOnlineCharKI->GetGeneral(siKind);

			if(siGeneral == 1)	continue;
			if(siGeneral == 3)  continue;
/*			if(m_bCheckSoldier[i] == TRUE) 
			{
				m_bEnableSoldier[i] = FALSE;
				continue;
			}
*/
			//char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)i);
			
			//if(pTempFollowerName)
			//{
				//m_pPopUp->Slot.AddSell((LPVOID)siAdd, pTempFollowerName);
			m_bEnableSoldier[i] = TRUE;	
			siAdd++;
			//}
		}
	}
	m_bFollowerMode = TRUE;
	
	// �������� �ƹ��͵� ����..
	if(siAdd == 0 )
	{
		//clGrp.FreeXspr(m_pPopUp->PopSpr);
		//delete m_pPopUp;
		//m_pPopUp = NULL;
		m_bFollowerMode = FALSE;
		
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADEINFO_SOILDER_NOTFINDSOILDER), "" );
		m_BInvenOk.SetAction(TRUE);
		m_BInvenExit.SetAction(TRUE);
		m_BFollower.SetAction(FALSE);
	}
	
	if(!m_pMyOnlineWorld->m_pMercenary->IsAction())
		m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
	m_pMyOnlineWorld->m_pMercenary->SetHighLight(TRUE);
}

// �˾�â ����
VOID	OnlineBooth::PopupAction()
{
	if(!m_bFollowerMode)	return;	

//	SI32 siSelect;

	if(!IpD.LeftPressSwitch)		return;

	SI16 siSelect = (SI16)m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE);
//	m_pPopUp->Slot.Action(IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &siSelect);

	// robypark 2005/1/8 16:31
	// �������� ������ �ִ� �뺴�� �ŷ��� �� ����.
	if (m_pMyOnlineWorld->IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(siSelect))
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100114));				
		return;
	}

	if(siSelect == -1)		return;

	// �뺴�� ���������Ƿ� ���̶���Ʈ�� �����Ѵ�.
	m_pMyOnlineWorld->m_pMercenary->SetHighLight(FALSE);

	if(m_bEnableSoldier[siSelect] == FALSE) 
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_INSERT_ERROR2));
		m_bFollowerMode = FALSE;
		IpD.LeftPressSwitch = FALSE;
		m_BInvenExit.SetAction(TRUE);
		m_BInvenOk.SetAction(TRUE);
		m_BFollower.SetAction(TRUE);
		return;
	}

	// siSelect�� -1�� �ƴϹǷ� Ư���뺴�� ������ �����̴�. 
	//if(m_pPopUp->bType)
	//{	
	{
		// ����ִ� �������ֳ� �˻�
		m_siBoothSlot = m_pBoothList->GetEmptySlot();
		if(m_siBoothSlot != -1)
		{
			IOnlineFollower *pFollower = m_pMyOnlineWorld->pMyData->GetFollowersInterface();
			ZeroMemory(&m_TempSellItemInfo[m_siBoothSlot], sizeof(SellItemInfo));
			m_TempSellItemInfo[m_siBoothSlot].siType			= LIST_FOLLOWER;
			m_TempSellItemInfo[m_siBoothSlot].Item.uiItemID		= 1;
			m_TempSellItemInfo[m_siBoothSlot].Item.uiItemCount	= 1;
			m_TempSellItemInfo[m_siBoothSlot].Follower.uiKind	= m_pMyOnlineWorld->pMyData->GetFollowerParameterKind(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.siLevel = m_pMyOnlineWorld->pMyData->GetFollowerParameterLevel(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.siDex	= pFollower->GetFollowerBaseParameterDEX(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.siInt	= pFollower->GetFollowerBaseParameterINT(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.siStr	= pFollower->GetFollowerBaseParameterSTR(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.siVit	= pFollower->GetFollowerBaseParameterVIT(siSelect);
			m_TempSellItemInfo[m_siBoothSlot].Follower.uiSlot	= siSelect;

			m_siSelect = siSelect;
			
			SetActiveSellInfo(TRUE);
			m_StatusIn = ON_BENCH_IN_INPUT_ITEM_INFO;
			
		}
	//	}
	//	else
	//		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_INSERT_ERROR2));

//		clGrp.FreeXspr(m_pPopUp->PopSpr);
//		delete m_pPopUp;
//		m_pPopUp = NULL;
		m_bFollowerMode = FALSE;
		IpD.LeftPressSwitch = FALSE;
		m_BInvenExit.SetAction(TRUE);
		m_BInvenOk.SetAction(TRUE);
		m_BFollower.SetAction(TRUE);
	}
}

// ��ο�
/*VOID	OnlineBooth::DrawPopup(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(!m_bFollowerMode)	return;

	if(clGrp.LockSurface(pSurface))
	{
		clGrp.PutSpriteJin(m_pPopUp->siX, m_pPopUp->siY, m_pPopUp->PopSpr.Header.Xsize, m_pPopUp->PopSpr.Header.Ysize, m_pPopUp->PopSpr.Image);
		clGrp.UnlockSurface(pSurface);
	}

	m_pPopUp->Slot.Draw(pSurface, m_pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ), RGB(200, 200, 200), RGB(96, 96, 96), TRUE, RGB(0, 255, 0), RGB(255, 255, 255));
}*/

void	OnlineBooth::OnResOpen(BYTE *pMsg)
{
	// ������ ������ �����ϸ� Char�� �����ϱ�
	OnResPMOpenMsg *pOnResPMOpenMsg;
	pOnResPMOpenMsg = (OnResPMOpenMsg *)pMsg;


	if(pOnResPMOpenMsg->dwResponse == ON_RET_OK)
	{
		if( pOnResPMOpenMsg->uiAccount == m_pMyOnlineWorld->pMyData->GetMyCharUniqueID() )
		{
			ItemAddBooth(pMsg);	
		}
		else
		{
			m_pMyOnlineWorld->pIOnlineChar->SetBoothOpen(pOnResPMOpenMsg->uiAccount, TRUE);

		}
	}
}

void	OnlineBooth::OnResBuyItem(BYTE *pMsg)
{
	OnResPMBuyItemMsg *pOnResPMBuyItemMsg = (OnResPMBuyItemMsg*)pMsg;

	if((pOnResPMBuyItemMsg != NULL) && (pOnResPMBuyItemMsg->dwResponse == ON_RET_OK))
	{
		if( m_bActionIn )
		{
			CHAR buf[256];
			ZeroMemory(buf, sizeof(buf));
			UI16 ItemID;
			UI08 FollowerID;
			SI16 InvenPos;
			MyItemData *MyItemData, TempItemData;
			if(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType == LIST_ITEM)
			{
				ItemID		= m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Item.uiItemID;
				FollowerID	= m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Item.uiFollowerID;
				InvenPos	= m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(ItemID);
				MyItemData	= m_pMyOnlineWorld->pMyData->GetItem(FollowerID, ItemID, InvenPos);
				
				TempItemData.siPosInInventory	= InvenPos;
				TempItemData.uiID				= ItemID;
				TempItemData.uiQuantity			= MyItemData->uiQuantity - pOnResPMBuyItemMsg->uiQuantity;
				if(TempItemData.uiQuantity < 0)	TempItemData.uiQuantity = 0;
				
				// ä��â�� �ȸ� ��� ǥ��
				CItemHeader *pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(ItemID);	
				sprintf(buf, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMSELL), m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName), TempItemData.uiQuantity);
				m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(TRUE);
				m_pMyOnlineWorld->pOnlinePannel->AddChat(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), buf, CHATTINGMODE_NORMAL);
				
				// ���� �κ��丮���� ������ ����
				ItemInfo *TempItemInfo = (ItemInfo *)(m_pBoothList->GetSlotInfo(pOnResPMBuyItemMsg->uiBoothSlot));
				if(TempItemInfo)
				{
					if(TempItemInfo->ItemCount <= TempItemData.uiQuantity)
						m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, 0);
					else
						m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, TempItemData.uiQuantity);
				}
				
				// ���� �κ��丮���� �ȸ� ������ ����
				m_pMyOnlineWorld->pMyData->DelItem(FollowerID, &TempItemData);

				// �ȸ� �������� �뺴 �κ��丮 ����
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( FollowerID );		//.actdoll (021224)

				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(FollowerID);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
			}
			else if(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType == LIST_FOLLOWER)
			{
				// ä��â�� �ȸ� ��� ǥ��
				char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Follower.uiSlot);
				sprintf(buf, "%s %s", pTempFollowerName, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_SOLD));
				m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(TRUE);
				m_pMyOnlineWorld->pOnlinePannel->AddChat(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), buf, CHATTINGMODE_NORMAL);

				// ���� �κ��丮���� ������ ����
				m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, 0);
				
				// ��ĳ������ ����
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(0);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();

				// ���� �뺴â���� �뺴 ����
				m_pMyOnlineWorld->pMyData->DeleteFollower(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Follower.uiSlot);
			}

			// ������ �ٽ� ���
			m_pMyOnlineWorld->pMyData->SetMoney(pOnResPMBuyItemMsg->NewMoney);
			
			// m_SellItemInfo ���� ����
			if(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType == LIST_ITEM)
			{
				if(TempItemData.uiQuantity)		m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Item.uiItemCount = pOnResPMBuyItemMsg->uiQuantity;
				else							
				{
					ZeroMemory(&m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot], sizeof(SellItemInfo));
					m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType = LIST_NOT;
				}
			}
			else if(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType == LIST_FOLLOWER)
			{
				ZeroMemory(&m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot], sizeof(SellItemInfo));
				m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType = LIST_NOT;
			}

			// ��ǳ�� ����
			if(m_pBoothList->GetItemNum())
			{
				UI08 uiNum = 0;
				OnBaseBoothItem* pOnBaseBoothItem = new OnBaseBoothItem[m_pBoothList->GetItemNum()];
				for(UI08 Slot = 0; Slot < ON_MAX_BOOTH_ITEM; Slot++)
				{
					const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(Slot);
					if(pTempItemInfo)
					{
						pOnBaseBoothItem[uiNum].siType = pTempItemInfo->siType;
						pOnBaseBoothItem[uiNum].uiBoothSlot = Slot;
						if(pTempItemInfo->siType == LIST_ITEM)		pOnBaseBoothItem[uiNum].uiProductID = pTempItemInfo->uiItemID;
						if(pTempItemInfo->siType == LIST_FOLLOWER)	pOnBaseBoothItem[uiNum].uiProductID = pTempItemInfo->Follower.uiKind;
						uiNum++;
					}
				}
				SetBoothSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), pOnBaseBoothItem, uiNum);
				delete [] pOnBaseBoothItem;
			}
			else
			{
				SetBoothSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), NULL, 0);
			}
		}

		else
		{
			MyItemData MyItemData;		

			
			m_pBoothList->StopDrag();
			
			if(pOnResPMBuyItemMsg->dwResponse == ON_RET_OK)
			{
				// ������ ���� �κ��丮�� �߰�
				MyItemData.siPosInInventory	=	m_siInvenSlot;
				MyItemData.uiID				=	m_siBuyItemID;
				MyItemData.uiQuantity		=	m_siBuyItemQuantity;				
				m_pMyOnlineWorld->pMyData->AddItem(m_uiBuyFollowerID, &MyItemData);
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( m_uiBuyFollowerID );		//.actdoll (021224)
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiBuyFollowerID);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
				
				// �����ݿ��� ���Դ�� ����
				m_pMyOnlineWorld->pMyData->SetMoney(pOnResPMBuyItemMsg->NewMoney);
				
				// ���� �κ��丮���� ������ ������ ����
				m_pBoothList->DeleteSlot(m_siBoothSlot, m_siBuyItemQuantity);
				
				m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
			}
			else if(pOnResPMBuyItemMsg->dwResponse == ON_RET_NO)
			{
				m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
			}
		}

	}
}

void	OnlineBooth::OnResClose(BYTE *pMsg)
{
	// ���� ���� ����ߴ��� ����
	OnResPMCloseMsg *pOnResPMCloseMsg = (OnResPMCloseMsg*)pMsg;
	if(pOnResPMCloseMsg != NULL)
	{
		if(pOnResPMCloseMsg->dwResponse == ON_RET_OK)
		{
			if( pOnResPMCloseMsg->uiAccount == m_pMyOnlineWorld->pMyData->GetMyCharUniqueID() )
			{
				m_StatusIn = ON_BENCH_IN_POLL;

				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), 0);

				if(!m_bField)
					m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(m_pSellStoreHeader->siMapStoreCode);

				m_bOpen = FALSE;
				m_pMyOnlineWorld->bCharMove = TRUE;
				SetActionIn(FALSE);
				SetActionOut(TRUE);	
				
				return ;
			}

			SI16 TempSelect, siHeroX, siHeroY;
			m_pMyOnlineWorld->pIOnlineChar->GetPos(pOnResPMCloseMsg->uiAccount, &siHeroX, &siHeroY);
			MapStoreHeader *pStoreHeader = m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );
			if(pStoreHeader)
			{
				// ���ǰ����̶��
				m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(pStoreHeader->siMapStoreCode);
				// ��ǳ�� ���ֱ�
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(pOnResPMCloseMsg->uiAccount, 0);
			}
			else// ���λ����̶��
			{	
				// ��ǳ�� ���ֱ�
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(pOnResPMCloseMsg->uiAccount, 0);
			}

			// ���� ��� ����
			m_pMyOnlineWorld->pIOnlineChar->SetBoothOpen(pOnResPMCloseMsg->uiAccount, FALSE);
		}
		else
		{
			if( pOnResPMCloseMsg->uiAccount == m_pMyOnlineWorld->pMyData->GetMyCharUniqueID() )
				pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_EXITERROR));
		}
	}

}

void	OnlineBooth::OnResBuyFollower(BYTE *pMsg)
{
	OnCompleteTradeMsg	*pOnCompleteTradeMsg = (OnCompleteTradeMsg*)pMsg;
	if(pOnCompleteTradeMsg == NULL)	return;
	
	OnFollowerInfo	 *pFollowerInfo		= NULL;
	OnlineReceivedMyItemInfo *pItemInfo = NULL;
	
	pFollowerInfo = (OnFollowerInfo*)(pOnCompleteTradeMsg+1);
	pItemInfo	  =	(OnlineReceivedMyItemInfo*)((CHAR*)pFollowerInfo + sizeof(OnFollowerInfo));

	if(!pOnCompleteTradeMsg->uiTradeFollowerNum)	
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_BUY_ERROR2));
		m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
		return;
	}
	
	for(SI08 i = 0; i < pOnCompleteTradeMsg->uiTradeFollowerNum; i++)
	{
		// ���� ������ ����
		m_pMyOnlineWorld->pMyData->InitFollower(pFollowerInfo->uiSlot, pFollowerInfo);						
		
		// �κ��丮 ������ ����
		for(SI16 y=0; y<pFollowerInfo->uiItemNum; y++)
		{
			m_pMyOnlineWorld->pMyData->AddItem(pFollowerInfo->uiSlot, &pItemInfo[y]);
		}
		
		m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( pFollowerInfo->uiSlot );		//.actdoll (021224)

		m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(pFollowerInfo->uiSlot);
		m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
		
		pFollowerInfo = (OnFollowerInfo*)((CHAR*)pItemInfo + (sizeof(OnlineReceivedMyItemInfo)*pFollowerInfo->uiItemNum));
		pItemInfo     = (OnlineReceivedMyItemInfo*)((CHAR*)pFollowerInfo + sizeof(OnFollowerInfo));
	}
	m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
	m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(0);
	m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
	
	// �����ݿ��� ���Դ�� ����
	const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
	MONEY Money = m_pMyOnlineWorld->pMyData->GetMoney();
	m_pMyOnlineWorld->pMyData->SetMoney(Money - pTempItemInfo->uiBuyPrice);
	
	// ���� �κ��丮���� ������ ������ ����
	m_pBoothList->DeleteSlot(m_siBoothSlot, 0);
	
	m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;

}

void	OnlineBooth::OnResChangeItem(BYTE *pMsg)
{
	// �����κ��� ��� OK�� ������
	OnResChangeBoothItemMsg *pOnResChangeBoothItemMsg;
	pOnResChangeBoothItemMsg = (OnResChangeBoothItemMsg *)pMsg;
	

	if(pOnResChangeBoothItemMsg->dwResponse == ON_RET_OK)
	{
		ItemAddBooth(pMsg);	
	}
/*
		if(!m_bField)
		{
			// �����̶�� ���ǿ� ������ �̹��� �����ϱ�
			SMapBoothData* pTempSMapBoothData = m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.GetBoothData(m_pSellStoreHeader->siMapStoreCode);
			pTempSMapBoothData->uiItemID[m_siBoothSlot]	= 0;
		}

		m_pBoothList->DeleteSlot(m_siBoothSlot);

		ZeroMemory(&m_SellItemInfo[m_siBoothSlot], sizeof(SellItemInfo));
		ZeroMemory(&m_TempSellItemInfo[m_siBoothSlot], sizeof(SellItemInfo));
	
		SI08 siFirst = 0;
		//if(m_bField)
		{
			CHAR buf[256];
			ZeroMemory(buf, sizeof(buf));
			ZeroMemory(m_szBoothName, sizeof(m_szBoothName));
			for(SI32 i = 0; i < ON_MAX_BOOTH_ITEM; i++)
			{
				switch(m_SellItemInfo[i].siType)
				{
				case LIST_ITEM:
					{
						CItemHeader *pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SellItemInfo[i].Item.uiItemID);
						if(!pItem)	break;
						
						if(!siFirst)	sprintf(buf, "%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
						else			sprintf(buf, "	%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
						strcat(m_szBoothName, buf);
						siFirst++;
					}
					break;
				case LIST_FOLLOWER:
					{
						char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_SellItemInfo[i].Follower.uiSlot);

						if(!siFirst)	sprintf(buf, "%s", pTempFollowerName);
						else			sprintf(buf, "	%s", pTempFollowerName);
						strcat(m_szBoothName, buf);
						siFirst++;
					}
					break;
				default:	break;
				}
			}
			
			// ��ǳ�� ����
			m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_szBoothName, TRUE);
		}
	}
	else
	{
		// ���� ������ ��� ����
		if(m_bField)	pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMINSERTERROR));
		else			pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMINSERTERROR));
	}

  */
}

void OnlineBooth::ItemAddBooth(BYTE *pMsg)
{
	// �������� ����
	m_bOpen		= TRUE;
	SI08 siFirst = 0;

	// �߰��� �������� m_SellItemInfo�� ����.
	for(SI32 i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		if(m_TempSellItemInfo[i].siType >= 0)
			CopyMemory(&m_SellItemInfo[i], &m_TempSellItemInfo[i], sizeof(SellItemInfo));
	}

	// �߰��� �������� OnlineChar�� ����.
	for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		if(m_TempSellItemInfo[i].siType >= 0)
			m_pMyOnlineWorld->pIOnlineChar->SetBoothItem(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), i, m_TempSellItemInfo[i]);
	}

	//if(m_bField)
	{
		CHAR buf[256];
		ZeroMemory(buf, sizeof(buf));
		ZeroMemory(m_szBoothName, sizeof(m_szBoothName));
		for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
		{
			switch(m_SellItemInfo[i].siType)
			{
			case LIST_ITEM:
				{
					CItemHeader *pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(m_SellItemInfo[i].Item.uiItemID);
					if(!pItem)	break;
					
					if(!siFirst)	sprintf(buf, "%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
					else			sprintf(buf, "	%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
					strcat(m_szBoothName, buf);
					siFirst++;
				}
				break;
			case LIST_FOLLOWER:
				{
					char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_SellItemInfo[i].Follower.uiSlot);

					if(!siFirst)	sprintf(buf, "%s", pTempFollowerName);
					else			sprintf(buf, "	%s", pTempFollowerName);
					strcat(m_szBoothName, buf);
					siFirst++;
				}
				break;
			default:	break;
			}
		}
		
		// ��ǳ�� ����
		m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_szBoothName, TRUE);
	}
	if(!m_bField)
	{
		if(m_pSellStoreHeader)
		{
			// ���� ����
			OnResPMOpenMsg *pOnResItemAddMsg1 = (OnResPMOpenMsg*)pMsg;
			if(pOnResItemAddMsg1 != NULL)
			{
				m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.AddBoothData(m_pSellStoreHeader->siMapStoreCode, m_pSellStoreHeader->siMapStoreMapXPosition
					, m_pSellStoreHeader->siMapStoreMapYPosition, m_pSellStoreHeader->siMapStoreMapIndex);
			}

			// �����̶�� ���ǿ� ������ �̹��� �÷�����
			for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
			{
				if(m_TempSellItemInfo[i].siType == LIST_ITEM)
				{
					SMapBoothData* pTempSMapBoothData = m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.GetBoothData(m_pSellStoreHeader->siMapStoreCode);
					pTempSMapBoothData->uiBoothID	= m_pSellStoreHeader->siMapStoreCode;
					pTempSMapBoothData->siTileX		= m_pSellStoreHeader->siMapStoreMapXPosition;
					pTempSMapBoothData->siTileY		= m_pSellStoreHeader->siMapStoreMapYPosition;
					pTempSMapBoothData->siMapIndex	= m_pSellStoreHeader->siMapStoreMapIndex;
					pTempSMapBoothData->uiItemID[i]	= m_TempSellItemInfo[i].Item.uiItemID;
					pTempSMapBoothData->sifData		= TRUE;

				}
			}
		}
	}
	
	InitTempItemInfo();
}

void OnlineBooth::ClearBooth()
{
	// m_pBoothList�� �ӽ÷� �÷����� �������� ����.
	ItemInfo TempItemInfo;
	BOOL bIsFollower = FALSE;
	for(SI32 i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		switch(m_TempSellItemInfo[i].siType)
		{
		case LIST_ITEM:
			{
				ItemInfo const *pTempItemInfo = m_pBoothList->GetSlotInfo(i);
				if(!pTempItemInfo)		break;

				if(pTempItemInfo->uiItemID == m_TempSellItemInfo[i].Item.uiItemID)
					m_pBoothList->DeleteSlot(i);
			}
		case LIST_FOLLOWER:
			{
				bIsFollower = TRUE;
				ItemInfo const *pTempItemInfo = m_pBoothList->GetSlotInfo(i);
				if(!pTempItemInfo)		break;

				if(pTempItemInfo->Follower.uiSlot == m_TempSellItemInfo[i].Follower.uiSlot)
					m_pBoothList->DeleteSlot(i);
			}
			break;
		default:		break;
		}
	}

	if(!bIsFollower)
	{
		if(m_bField)	pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH),m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_ITEMINSERTFAILE));
		else			pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH),m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_ITEMINSERTFAILE));
	}
	else
	{
		if(m_bField)	pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_INSERT_ERROR1));
		else			pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_INSERT_ERROR1));
	}

	// m_TempItemInfo �ʱ�ȭ
	InitTempItemInfo();
}