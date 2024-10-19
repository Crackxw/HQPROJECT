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
#include	"OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
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

// 초기화
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

	// 버튼을 만든다.
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

	// 에디트 박스를 만들어 준다.
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

	// 좌판인벤토리 리스트 생성
	m_pBoothList = new OnlineListControl(pOnlineWorld);
	m_pBoothList->Init(60, 60, 5, 1, m_siX + 10, m_siY + 33, 0);

	// 인벤토리 영역 설정
	SetRect(&m_rtBoothRect, m_siX + 10, m_siY + 33, m_siX + 320, m_siY + 93);
	SetRect(&m_rtInvenRect, 467, 280, 767, 400);

	for(SI16 i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bFollowerSlot[i] = FALSE;
	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)	m_bItemSlot[i] = FALSE;

	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bEnableSoldier[i] = FALSE;
//	for(i = 0; i < ON_MAX_FOLLOWER_NUM; i++)    m_bCheckSoldier[i] = FALSE;


	m_pSoldierTrade = new OnlineSoldierTrade(pOnlineWorld, "Online\\GameSpr\\Inventory\\main.Spr");
}


// 날린다.
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

// 출력
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

	// 좌판 인벤토리를 그린다.
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
			// 좌판 타이틀을 '거래 좌판'에서 'xxx의 좌판'으로 개설자 캐릭명의 좌판으로 변경
			sprintf(szTemp, "%s%s", m_pMyOnlineWorld->pMyData->GetMyCharName(), m_pMyOnlineWorld->pOnlineText->Get(8000494));
			
			//m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 7,118,13, m_pMyOnlineWorld->pOnlineText->Get(8000494),RGB(200, 200, 200));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 8,118,13, szTemp,RGB(200, 200, 200));			
			// H2-E-01.20.05 //

			m_BFollower.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BInvenOk.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			m_BInvenExit.Put( hDC, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			pSurface->ReleaseDC( hDC );
		}
		
		// 좌판 인벤토리 아이템 그리기
		m_pBoothList->Draw( pSurface );
	}

	// 아이템 가격 입력창을 그린다.
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

			// 현재 선택된 아이템 구하기
			ItemInfo *pTempItemInfo = NULL;
			
			if(m_siInvenSlot >= 0)		// 자기 인벤토리에서 좌판으로 아이템을 놓을 경우
				pTempItemInfo = const_cast<ItemInfo*>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
			else						// 좌판위의 아이템, 용병 정보 수정할 경우
				pTempItemInfo = const_cast<ItemInfo*>(m_pBoothList->GetSlotInfo(m_siBoothSlot));

			// 아이템, 용병 이름
			if(pTempItemInfo)
			{
				if(pTempItemInfo->siType == LIST_ITEM)								// 아이템을 올리거나, 수정
				{
					pItem = m_pMyOnlineWorld->pItemsInfo->GetItem(pTempItemInfo->uiItemID);
					if(pItem)				sprintf((char*)szTemp, "%s", m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName));
				}
				else if(pTempItemInfo->siType == LIST_FOLLOWER)						// 용병정보를 수정
				{
					char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)pTempItemInfo->Follower.uiSlot);
					if(pTempFollowerName)	sprintf((char*)szTemp, "%s", pTempFollowerName);
				}
			}
			else if(m_TempSellItemInfo[m_siBoothSlot].siType == LIST_FOLLOWER)		// 용병을 올릴경우
			{
				char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_TempSellItemInfo[m_siBoothSlot].Follower.uiSlot);
				if(pTempFollowerName)	sprintf((char*)szTemp, "%s", pTempFollowerName);
			}
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(hDC, m_siInfoX+40, m_siInfoY+40, szTemp, RGB(220, 176, 106));

			// 가격
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

			// 설명
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
				// 아이템 설명의 처음 3바이트에 숫자를 입력할 수 없도록 막음 
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
			// 아이템 설명의 처음 3바이트에 숫자를 입력할 수 없도록 막음 
			
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

	// 용병선택 팝업창 드로우
//	DrawPopup(pSurface);
}

VOID OnlineBooth::DrawBuyerInfo(LPDIRECTDRAWSURFACE7 pSurface)
{
	HDC		hDC;
	CHAR	TempBuffer[256];
	CHAR	szTemp[256];
	ZeroMemory(TempBuffer, sizeof(TempBuffer));

	// 좌판 인벤토리를 그린다.
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
			// 좌판 타이틀을 '거래 좌판'에서 'xxx의 좌판'으로 개설자 캐릭명의 좌판으로 변경
			sprintf(szTemp, "%s%s", m_pMyOnlineWorld->OnlineCharName(m_uiAccount), m_pMyOnlineWorld->pOnlineText->Get(8000494));
			
			//m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 7,118,13, m_pMyOnlineWorld->pOnlineText->Get(8000494),RGB(200, 200, 200));
			m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siX + 114,m_siY + 8,118,13, szTemp,RGB(200, 200, 200));			
			// H2-E-01.20.05 //
			
			//m_BInvenExit.Put( hDC );
			m_BInvenBuyInfo.Put(hDC,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			pSurface->ReleaseDC( hDC );
		}

		// 좌판 인벤토리 아이템 그리기
		m_pBoothList->Draw( pSurface );

		// 용병정보창을 그린다.
		if(m_StatusOut == ON_BENCH_OUT_BUY_FOLLOWER)
		{
			UI16 uiFollowerKind;
			const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
			if(pTempItemInfo)
				uiFollowerKind = pTempItemInfo->Follower.uiKind;
			
			if(clGrp.LockSurface(pSurface) == TRUE)
			{
				// 바탕 출력
				clGrp.PutSpriteT(m_siInfoX, m_siInfoY, m_InfoLineSpr.Header.Xsize, m_InfoLineSpr.Header.Ysize, m_InfoLineSpr.Image);
				clGrp.PutSpriteJin(m_siInfoX, m_siInfoY, m_InfoBuySpr.Header.Xsize, m_InfoBuySpr.Header.Ysize, m_InfoBuySpr.Image);
				m_BOk.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_BCancel.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				m_BSoldierInfo.Put(&m_pExitButtonSpr, 0, 2, 1, BUTTON_PUT_NOMOVE);
				
				// 고용할려는 용병 얼굴 출력
				if(pTempItemInfo)
				{					
					//clGrp.PutSpriteT(m_siInfoX + 40, m_siInfoY + 45, m_pSlotSpr->Header.Xsize, m_pSlotSpr->Header.Ysize, m_pSlotSpr->Image);					
					// actdoll (2004/01/02 19:00) : 초상 출력 수정
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
		
		// 마우스를 숨긴다.
		if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsShow() == FALSE)
		{
			if(m_pBoothList->GetDragSlot() != -1)	m_pMyOnlineWorld->pIOnlineMouse->ShowMouse( FALSE );
			// 이동중인 아이템이 없다면
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
	
	// 누군가 좌판물건 구입했는지 체크하고 처리

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
			// 노점일 경우 OnlineWorld에서 다른 캐릭선택하는지 감지함.
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
			// 캐릭 고정
			m_pMyOnlineWorld->bCharMove = FALSE;
		}
		break;
	case ON_BENCH_OUT_INVENTORY_POLL:
		{
			// 자기 인벤토리 창을 연다. (좌판 인벤토리도 자동으로 열림)
			if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);
			
			// 마우스를 아이템 위로 올려놓았나?
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
			// 좌판 인벤토리를 클릭했나?
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
			
			// 나가기 버튼을 눌렀나?
			if(m_BInvenBuyInfo.Process(m_bMouseDown))
			{
				// 캐릭 고정 해제
				m_pMyOnlineWorld->bCharMove = TRUE;

				// 노필드 해제
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
			// 내 인벤토리로 이동
			if(m_pMyOnlineWorld->fLeftOneSwitch == 1)
			{
				if(PtInRect( &m_rtInvenRect, pt ) == TRUE)
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() != -1)
						m_pMyOnlineWorld->pOnlineInventory->DragFail();

					
					// 무게에 따라 들수있는 수량 구하기
					const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);

					if(pTempItemInfo)
					{
						if(pTempItemInfo->siType == LIST_ITEM)
						{
							m_uiBuyFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
							SI32 siResult = m_pMyOnlineWorld->pMyData->GetFollowersInterface()->GetItemQuantityToTake(m_uiBuyFollowerID, pTempItemInfo->uiItemID);
							SI16 siItemCount = min((SI16)siResult, (SI16)pTempItemInfo->ItemCount);
							
							if(siItemCount > 0)			// 들수있는 수량이 있다면
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
			
			// 계산기 입력을 받는다.
			if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE)
			{
				if( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0)
				{
					m_siBuyItemQuantity	= (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
					// 에러검사(돈부족, 무게초과)
					if(ErrorMsg())
					{
						m_pBoothList->StopDrag();
						m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
					}
					else
					{
						// 구입한 아이템에 관한 정보를 얻는다.
						m_siBuyItemQuantity				= (UI16)m_pMyOnlineWorld->pOnlineNumPad->siCount;
						const ItemInfo *TempItemInfo	= m_pBoothList->GetSlotInfo(m_siBoothSlot);
						// actdoll (2004/03/15 22:26) : Critical Error Recovering at Ver.10205
						//	TempItemInfo가 NULL로 떨어질 수도 있는 상황에서 해당 포인터에 대한
						//	확인을 하지 않으므로 억세스 바이올레이션이 발생했다. 
						//	버그 리포트에서 발생했던 빈도수가 높은 축에 속한다.
						//	따라서 일단 TempItemInfo정보가 NULL이 아닐때만 하위 명령을 진행하도록 작업한다.
						//	다음 버그 리포팅에 이 부분에서 버그가 안나오거나 조치에 따른 특별한 문제가 없을 경우 이 주석을 제거해도 좋다.
						if( TempItemInfo )
						{
							m_siBuyItemID					= TempItemInfo->uiItemID;
							m_siInvenSlot					= m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(TempItemInfo->uiItemID);
							
							// 구입 사실을 서버에 알려준다.
							m_pMyOnlineWorld->pOnlineClient->SendBuyBoothItem(m_uiAccount, m_siBuyItemQuantity,
								(UI08)m_siBoothSlot, (UI08)m_siInvenSlot, (UI16)TempItemInfo->uiItemID, (UI08)m_uiBuyFollowerID);
							m_StatusOut = ON_BENCH_OUT_BUY_ITEM_OK;
						}
						else
						{
							// 아이템 원래 위치로 이동
							m_pBoothList->StopDrag();
							
							m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;
						}
					}
				}
				else
				{
					// 아이템 원래 위치로 이동
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
					// 현재 저장할 자리가 있으면 통과 없으면 Error Message
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
							// 구입 사실을 서버에 알려준다.
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
			//인벤토리 창을 띄운다.
			m_pMyOnlineWorld->pOnlineInventory->SetActive( TRUE );
			m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
			
			for(i = 0; i < ON_MYDATA_MAX_ITEM_NUM; i++)
			{
				if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(i)) //아이템이 있다면
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
					// 자기 인벤토리 창을 연다.
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
				// 좌판에 아이템을 놓기위해 좌판을 클릭하는지 검사
				if(m_bMouseDown && ClickBenchIn())
				{
					// 자기 인벤토리 창을 연다.
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

			// 용병선택 팝업창 동작
			PopupAction();

			// 마우스를 아이템 위로 올려놓았나?
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

			// 용병선택 버튼을 눌렀나
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

			// 좌판 창의 확인 버튼을 눌렀나?
			if(m_BInvenOk.Process(m_bMouseDown))
			{
				UI08 uiNum = 0;
				for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
				{
					if(m_TempSellItemInfo[i].siType >= 0)	uiNum++;
				}

				// 인벤토리에 올려진 아이템이 있다면
				if(uiNum)
				{
					// 인벤토리에 있는 모든 아이템 정보를 서버로 보낸다.
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
					
					// 절대 임쉬
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
			
			// 좌판 창의 나가기 버튼을 눌렀나?
			if(m_BInvenExit.Process(m_bMouseDown))
			{
				m_pSoldierTrade->SetHide();
				for(i = 0; i < ON_MAX_BOOTH_ITEM; i++)
				{
					// BoothList->ItemInfo에서 임시로 삽입된 아이템을 삭제한다.
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
				// 좌판에 올려놓은 아이템 수정하려다 취소하는 경우 
				// 원래 좌판 아이템 정보(m_SellItemInfo[])로 초기화 해준다.
				for(i=0; i<ON_MAX_BOOTH_ITEM; i++)
				{					
					// 올려놓은 아이템 수정할려다가 취소하는 경우 삭제된 pBoothList에 다시 삽입.
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

				// m_TempItemInfo 초기화
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

					// 좌판 인벤토리를 클릭했나?
					if(m_pBoothList)
					{
						if(m_pBoothList->GetSelectSlot() != -1)
						{
							// 시작 //////////////////////////////////////////////
							// robypark 2004/12/2 16:53
							// 공성전 소집권 아이템은 좌판에 올릴 수 없다.
							if (-1 != m_siInvenSlot)
							{
								const ItemInfo *pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));

								if(pTempItemInfo)
								{
									if(pTempItemInfo->siType == LIST_ITEM)
									{
										CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pTempItemInfo->uiItemID);

										// 소집권 아이템 이라면
										if (pItemHeader->m_pImperium)
										{
											// 좌판에 올릴 수 없다.
											m_siInvenSlot = -1;
											m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
											break;
										}
									}
								}
							}
							// 끝 //////////////////////////////////////////////

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
			// 인벤토리의 아이템을 좌판에 올려놓을때
			if(m_siInvenSlot != -1)
			{
				// 파는 아이템에 관한 정보들을 임시로 저장한다.
				m_uiSellFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
				
				// 클릭한 자리에 아이템이 존재한다면
				if( m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1 )
					m_StatusIn = ON_BENCH_IN_BENCH_CLICK;
				else
				{
					m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();
					
					// 이미 올려놓은 아이템을 다시 선택한건 아닌지 검사
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
						// 아이템 가격, 수량 입력창을 띄운다.
						SetActiveSellInfo(TRUE);
						
						m_StatusIn = ON_BENCH_IN_INPUT_ITEM_INFO;
					}
				}
			}
			else
			{
				// 좌판의 클릭한 자리에 아이템이 존재한다면
				if( m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1 )
				{
					// 선택한 아이템 정보를 보여주고 수정하게 한다.
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
	case ON_BENCH_IN_INPUT_ITEM_INFO: // 좌판의 아이템을 클릭했을 경우
		{
			OnlineEditControlEx	*pOecEx;
			// 파는 아이템에 관한 정보들을 임시로 저장한다.
			ZeroMemory(m_szSellItemDesc, sizeof(m_szSellItemDesc));
			if((m_pBoothList->IsEmptySlot(m_siBoothSlot) != -1) && !frame)
			{
				// 클릭한 자리에 품목(아이템, 용병)이 존재한다면
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
			// 가격 & 설명 입력창의 확인버튼
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

				// 아이템정보 입력창을 끈다.
				SetActiveSellInfo(FALSE);

				ItemInfo const *pTempItemInfo = NULL;
				pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
				if(!pTempItemInfo)	pTempItemInfo = const_cast<const ItemInfo *>(m_pBoothList->GetSlotInfo(m_siBoothSlot));

				// m_TempSellItemInfo에 아이템 정보 저장
				if(m_TempSellItemInfo[m_siBoothSlot].siType != LIST_FOLLOWER)
				{
					if(pTempItemInfo)
					{
						// 내인벤토리 아이템을 드래그해서 좌판에 올렸을 경우
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

				// 좌판 인벤토리에 아이템을 삽입한다.
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
			// 가격 입력창의 취소 버튼
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
			// 서버에다 나간다고 보내준다.
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

// TRUE가 리턴되면 좌판에서 나간다.~
BOOL OnlineBooth::MoveOutBench(SI16 siHeroX, SI16 siHeroY)
{
	if( m_pSellStoreHeader == NULL )	return TRUE;
	
	if( m_pSellStoreHeader->siMapStoreMapXStartPosition > siHeroX )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapXEndPosition < siHeroX )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapYStartPosition > siHeroY )		return TRUE;
	if( m_pSellStoreHeader->siMapStoreMapYEndPosition < siHeroY )		return TRUE;

	return FALSE;
}

// 좌판에 있는 아이템을 초기화 한다.
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

// 좌판 클릭시 아이템이 있듬 TRUE가 리턴이...
BOOL OnlineBooth::ClickBenchIn()
{
	SI32 siX, siY;
	SI16 siTemp;
	
	MapStoreHeader *pTempHeader;

	m_pMyOnlineWorld->pOnlineMap->TransformMouseCoordToMapXY(IpD.Mouse_X, IpD.Mouse_Y, siX, siY);
	pTempHeader	=	m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser(siX, siY, &siTemp);

	// 만약 좌판에 들어 가면 pTempHeader에 좌판에 대한 정보가 리턴이 된다.
	if(pTempHeader)
	{
		if(pTempHeader->siMapStoreCode == m_pSellStoreHeader->siMapStoreCode)
		{
			// 아이템을 드래그 중인지 알아 본다.
			if( m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot() == -1 )		return	TRUE;
		}
	}

	return FALSE;
}

BOOL OnlineBooth::WhererU(SI16 siHeroX, SI16 siHeroY)
{
	// 좌판 헤더를 리턴을 받아서 좌판에 들어 갔는 검사를 하면 되지,,,
	SI16 TempSelect;
	m_pSellStoreHeader = m_pMyOnlineWorld->pOnlineMap->GetMapStoreParser( siHeroX, siHeroY, &TempSelect );

	// 좌판에 들어 가기 성공
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
		
		// 캐릭 고정, 노필드 설정

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

		// 캐릭 움직임, 노필드 설정 해제
		m_pMyOnlineWorld->bCharMove = TRUE;
		m_bField = FALSE;
		//m_pPriceEditBox->EnableWindow(FALSE);
		//m_pDescEditBox->EnableWindow(FALSE);

		if( m_siNoField != -1 )
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField  );
		m_siNoField	= -1;

		// 용병 팝업 창
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
		// 파는 아이템 가격, 수량 입력창을 띄운다.
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
		// 사는 아이템 수량 입력창을 띄운다.
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
	// 현재 무게를 검사하여 그 만큼 들수 있는지 검사
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

	// 인벤토리에 공간이 있는지 검사
	SI16 siSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetEmptySlot(TempItemInfo->uiItemID);
	if(siSlot == -1)
	{
		pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_NOTITEMSLOT));
		return TRUE;
	}

	// 소지금 검사
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
	// 올려놓은 용병의 아이템이 아닌가 체크
	if(m_bFollowerSlot[uiFollowerID])	return TRUE;

	// 이미 올려놓은 아이템이 아닌가 체크
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
		// 좌판이라면 좌판에 아이템 이미지 올려놓기
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

// 용병 올릴때 팝업창
VOID	OnlineBooth::SetPopup()
{
	//m_pPopUp = new PopUp;

	//clGrp.LoadXspr("Online\\GameSpr\\TradePopup.Spr", m_pPopUp->PopSpr);
	//m_pPopUp->siSelect = -1;
	//m_pPopUp->bType = TRUE;

	// 용병창
	//if(!m_pPopUp->bType)	return;

	//XSPR *pScroll = m_pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NOTICE_SCROLL_BAR);
	//m_pPopUp->siX = m_BFollower.GetX();// + m_BFollower.GetWidth() - m_pPopUp->PopSpr.Header.Xsize;
	//m_pPopUp->siY = m_BFollower.GetY() + m_BFollower.GetHeight(); //- m_pPopUp->PopSpr.Header.Ysize;
	
	//m_pPopUp->Slot.Create(m_pPopUp->siX, m_pPopUp->siY, m_pPopUp->PopSpr.Header.Xsize, m_pPopUp->PopSpr.Header.Ysize, m_pPopUp->siX + 5, 
	//	m_pPopUp->siY + 5, m_pPopUp->PopSpr.Header.Xsize, m_pPopUp->PopSpr.Header.Ysize, m_pPopUp->PopSpr.Header.Xsize-30, 20, NULL, 
	//	pScroll, m_pPopUp->siX + 84, m_pPopUp->siY+5, m_pPopUp->siY + m_pPopUp->PopSpr.Header.Ysize );
	
	SI16 siAdd = 0;
	// 교환 가능한 용병을 넣어 준다.
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
	
	// 넣은거이 아무것두 없뜸..
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

// 팝업창 엑숀
VOID	OnlineBooth::PopupAction()
{
	if(!m_bFollowerMode)	return;	

//	SI32 siSelect;

	if(!IpD.LeftPressSwitch)		return;

	SI16 siSelect = (SI16)m_pMyOnlineWorld->m_pMercenary->GetMouseOverSlotNo(TRUE);
//	m_pPopUp->Slot.Action(IpD.Mouse_X, IpD.Mouse_Y, m_bMouseDown, &siSelect);

	// robypark 2005/1/8 16:31
	// 소집권을 가지고 있는 용병은 거래할 수 없다.
	if (m_pMyOnlineWorld->IsExistedItemInInventoryAboutTransformationOfSiegeWarfareUnit(siSelect))
	{
		m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8100114));				
		return;
	}

	if(siSelect == -1)		return;

	// 용병을 선택했으므로 하이라이트를 해제한다.
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

	// siSelect가 -1이 아니므로 특정용병을 선택한 상태이다. 
	//if(m_pPopUp->bType)
	//{	
	{
		// 비어있는 슬롯이있나 검사
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

// 드로우
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
	// 누군가 좌판을 개설하면 Char에 저장하기
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
				
				// 채팅창에 팔린 결과 표시
				CItemHeader *pItem	= m_pMyOnlineWorld->pItemsInfo->GetItem(ItemID);	
				sprintf(buf, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMSELL), m_pMyOnlineWorld->pOnlineText->Get(pItem->m_siItemName), TempItemData.uiQuantity);
				m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(TRUE);
				m_pMyOnlineWorld->pOnlinePannel->AddChat(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), buf, CHATTINGMODE_NORMAL);
				
				// 좌판 인벤토리에서 아이템 삭제
				ItemInfo *TempItemInfo = (ItemInfo *)(m_pBoothList->GetSlotInfo(pOnResPMBuyItemMsg->uiBoothSlot));
				if(TempItemInfo)
				{
					if(TempItemInfo->ItemCount <= TempItemData.uiQuantity)
						m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, 0);
					else
						m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, TempItemData.uiQuantity);
				}
				
				// 본인 인벤토리에서 팔린 아이템 삭제
				m_pMyOnlineWorld->pMyData->DelItem(FollowerID, &TempItemData);

				// 팔린 아이템의 용병 인벤토리 갱신
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( FollowerID );		//.actdoll (021224)

				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(FollowerID);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
			}
			else if(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].siType == LIST_FOLLOWER)
			{
				// 채팅창에 팔린 결과 표시
				char *pTempFollowerName = m_pMyOnlineWorld->pMyData->GetFollowerCharName((UI08)m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Follower.uiSlot);
				sprintf(buf, "%s %s", pTempFollowerName, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_FOLLOWER_SOLD));
				m_pMyOnlineWorld->pOnlinePannel->SetChatOpen(TRUE);
				m_pMyOnlineWorld->pOnlinePannel->AddChat(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), buf, CHATTINGMODE_NORMAL);

				// 좌판 인벤토리에서 아이템 삭제
				m_pBoothList->DeleteSlot(pOnResPMBuyItemMsg->uiBoothSlot, 0);
				
				// 본캐릭으로 선택
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( 0 );		//.actdoll (021224)
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(0);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();

				// 본인 용병창에서 용병 삭제
				m_pMyOnlineWorld->pMyData->DeleteFollower(m_SellItemInfo[pOnResPMBuyItemMsg->uiBoothSlot].Follower.uiSlot);
			}

			// 소지금 다시 계산
			m_pMyOnlineWorld->pMyData->SetMoney(pOnResPMBuyItemMsg->NewMoney);
			
			// m_SellItemInfo 정보 갱신
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

			// 말풍선 갱신
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
				// 구입한 물건 인벤토리에 추가
				MyItemData.siPosInInventory	=	m_siInvenSlot;
				MyItemData.uiID				=	m_siBuyItemID;
				MyItemData.uiQuantity		=	m_siBuyItemQuantity;				
				m_pMyOnlineWorld->pMyData->AddItem(m_uiBuyFollowerID, &MyItemData);
				m_pMyOnlineWorld->m_pMercenary->Set_SelectedSlot( m_uiBuyFollowerID );		//.actdoll (021224)
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiBuyFollowerID);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
				
				// 소지금에서 구입대금 지불
				m_pMyOnlineWorld->pMyData->SetMoney(pOnResPMBuyItemMsg->NewMoney);
				
				// 좌판 인벤토리에서 구입한 아이템 삭제
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
	// 노점 개설 취소했는지 감지
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
				// 좌판개설이라면
				m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.DelBoothData(pStoreHeader->siMapStoreCode);
				// 말풍선 없애기
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(pOnResPMCloseMsg->uiAccount, 0);
			}
			else// 개인상점이라면
			{	
				// 말풍선 없애기
				m_pMyOnlineWorld->pIOnlineChar->SetSpeech(pOnResPMCloseMsg->uiAccount, 0);
			}

			// 개설 취소 지정
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
		// 장착 아이템 설정
		m_pMyOnlineWorld->pMyData->InitFollower(pFollowerInfo->uiSlot, pFollowerInfo);						
		
		// 인벤토리 아이템 설정
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
	
	// 소지금에서 구입대금 지불
	const ItemInfo *pTempItemInfo = m_pBoothList->GetSlotInfo(m_siBoothSlot);
	MONEY Money = m_pMyOnlineWorld->pMyData->GetMoney();
	m_pMyOnlineWorld->pMyData->SetMoney(Money - pTempItemInfo->uiBuyPrice);
	
	// 좌판 인벤토리에서 구입한 아이템 삭제
	m_pBoothList->DeleteSlot(m_siBoothSlot, 0);
	
	m_StatusOut = ON_BENCH_OUT_INVENTORY_POLL;

}

void	OnlineBooth::OnResChangeItem(BYTE *pMsg)
{
	// 서버로부터 취소 OK를 받으면
	OnResChangeBoothItemMsg *pOnResChangeBoothItemMsg;
	pOnResChangeBoothItemMsg = (OnResChangeBoothItemMsg *)pMsg;
	

	if(pOnResChangeBoothItemMsg->dwResponse == ON_RET_OK)
	{
		ItemAddBooth(pMsg);	
	}
/*
		if(!m_bField)
		{
			// 좌판이라면 좌판에 아이템 이미지 삭제하기
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
			
			// 말풍선 띄우기
			m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_szBoothName, TRUE);
		}
	}
	else
	{
		// 좌판 아이템 취소 실패
		if(m_bField)	pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_PRIVATEBOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMINSERTERROR));
		else			pOnlineMsgBoxBench->SetMsg(ON_MSGBOX_TYPE_OK, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTH_BOOTH), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_BOOTHINFO_ITEMINSERTERROR));
	}

  */
}

void OnlineBooth::ItemAddBooth(BYTE *pMsg)
{
	// 상점개설 성공
	m_bOpen		= TRUE;
	SI08 siFirst = 0;

	// 추가된 아이템을 m_SellItemInfo에 삽입.
	for(SI32 i = 0; i < ON_MAX_BOOTH_ITEM; i++)
	{
		if(m_TempSellItemInfo[i].siType >= 0)
			CopyMemory(&m_SellItemInfo[i], &m_TempSellItemInfo[i], sizeof(SellItemInfo));
	}

	// 추가된 아이템을 OnlineChar에 저장.
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
		
		// 말풍선 띄우기
		m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), m_szBoothName, TRUE);
	}
	if(!m_bField)
	{
		if(m_pSellStoreHeader)
		{
			// 좌판 개설
			OnResPMOpenMsg *pOnResItemAddMsg1 = (OnResPMOpenMsg*)pMsg;
			if(pOnResItemAddMsg1 != NULL)
			{
				m_pMyOnlineWorld->pOnlineMap->m_MapBoothInfo.AddBoothData(m_pSellStoreHeader->siMapStoreCode, m_pSellStoreHeader->siMapStoreMapXPosition
					, m_pSellStoreHeader->siMapStoreMapYPosition, m_pSellStoreHeader->siMapStoreMapIndex);
			}

			// 좌판이라면 좌판에 아이템 이미지 올려놓기
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
	// m_pBoothList에 임시로 올려놓은 아이템을 비운다.
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

	// m_TempItemInfo 초기화
	InitTempItemInfo();
}