//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - �ܼ� >> ���â
//	File Name		: OnlineFieldAuction.cpp
//	Birth Date		: 2002. 10. 19.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: ������� �ֿܼ��� ��� ���� â�� ����ϴ� �κ��Դϴ�.
//
//===================================================================================================

#include <GSL.h>

#include	<Mouse.h>
#include	<clGame.h>

#include	"OnlineFieldAuction.h"
#include	"OnlineListControl.h"
#include	"OnlineInventory.h"
#include	"OnlineFieldArea.h"
#include    "OnlineHelp.h"
#include	"OnlineMegaText.h"
#include	"OnlineVillage.h"
#include	"OnlineTrade.h"
#include	"OnlineTradeBook.h"
#include	"OnlineNumberControl.h"
#include	"ChattingServer\\HQChattingServerClientSocket.h"
#include    "OnlineCharUI.h"
#include    "OnlineMiniMap.h"
#include    "OnlineSetting.h"
#include	"OnlineTip.h"
#include	"OnlineBooth.h"
#include	"OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include	"OnlinePannel.h"
#include	"OnlineMercenary.h"

const int RECEPTION_POS_X						= 80;
const int RECEPTION_POS_Y						= 100;

const int RECEPTION_OK_BUTTON_X					= 5;
const int RECEPTION_OK_BUTTON_Y					= 258;
const int RECEPTION_CANCEL_BUTTON_X				= 185;
const int RECEPTION_CANCEL_BUTTON_Y				= 258;

const int RECEPTION_ITEM_SLOT_X					= 38;
const int RECEPTION_ITEM_SLOT_Y					= 150;

const int RECEPTION_EXPLANATION_X				= 28;
const int RECEPTION_EXPLANATION_Y				= 40;
const int RECEPTION_EXPLANATION_WIDTH			= 240;



const int RECEPTION_MINIMUM_PRICE_EDIT_X		= 178;
const int RECEPTION_MINIMUM_PRICE_EDIT_Y		= 154;
const int RECEPTION_MINIMUM_PRICE_EDIT_WIDTH	= 80;          
const int RECEPTION_MINIMUM_PRICE_EDIT_HEIGHT	= 15;

const int RECEPTION_MAXIMUM_PRICE_EDIT_X		= 178;
const int RECEPTION_MAXIMUM_PRICE_EDIT_Y		= 186;
const int RECEPTION_MAXIMUM_PRICE_EDIT_WIDTH	= 80;          
const int RECEPTION_MAXIMUM_PRICE_EDIT_HEIGHT	= 15;


const int FIELD_AUCTION_PANEL_X					= 80;
const int FIELD_AUCTION_PANEL_Y					= 30;
const int FIELD_AUCTION_PANEL_WIDTH				= 600;
const int FIELD_AUCTION_PANEL_HEIGHT			= 120;


const int FIELD_AUCTION_POS_X					= 600;
const int FIELD_AUCTION_POS_Y					= 320;

const int FIELD_OK_BUTTON_X						= 44;
const int FIELD_OK_BUTTON_Y						= 125;
const int FIELD_CANCEL_BUTTON_X					= 44;
const int FIELD_CANCEL_BUTTON_Y					= 151;

const int FIELD_BIDDING_PRICE_EDIT_X			= 80;
const int FIELD_BIDDING_PRICE_EDIT_Y			= 64;
const int FIELD_BIDDING_PRICE_EDIT_WIDTH		= 150;
const int FIELD_BIDDING_PRICE_EDIT_HEIGHT		= 50;

const int AUCTION_TEXT_UNIQUE_ID				= 77777;	// text���۸� ������� unique ID 
															// ĳ������ unique id �� 65535�� ���������Ƿ� �����ϴ�.


enum
{   
	RECEPTION_MAIN           = 0,
	RECEPTION_ENTER_ITEMNUMBER,
	RECEPTION_ENTER_FOLLOWER,
	RECEPTION_WAITING_REPLY
};


extern _InputDevice				IpD;
extern _clGame* pGame;


OnlineReception::OnlineReception()
{
	m_pMyOnlineWorld = NULL;
	m_pButtonSpr = NULL;

	m_siNoField = -1;

	m_siInvenSlot = -1;

	m_siItemCount = 0;

	m_siStatus = RECEPTION_MAIN;

}

OnlineReception::~OnlineReception()
{
	Free();
}

void OnlineReception::Init(cltOnlineWorld *pWorld)
{
	m_pMyOnlineWorld = pWorld;
	
	m_siX = RECEPTION_POS_X;
	m_siY = RECEPTION_POS_Y;

	m_pButtonSpr		=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_EXITBUTTON );
	
	// ===============================================================================
//	m_pMinimumPriceEdit	=	new OnlineEditControl;
//	m_pMinimumPriceEdit->CreateEditBox(1000, 1000, 80, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 8, TRUE, FALSE );
//
//	m_pMaximumPriceEdit	=	new OnlineEditControl;
//	m_pMaximumPriceEdit->CreateEditBox(1000, 1200, 80, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 8, TRUE, FALSE );
//
//	m_pMinimumPriceEdit->SetNextWindow( m_pMaximumPriceEdit->GethWnd() );
//	m_pMaximumPriceEdit->SetNextWindow( m_pMinimumPriceEdit->GethWnd() );
	// ===============================================================================

	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	RECT	rcRect;
	SetRect( &rcRect, m_siX + RECEPTION_MINIMUM_PRICE_EDIT_X, m_siY + RECEPTION_MINIMUM_PRICE_EDIT_Y, 
						m_siX + RECEPTION_MINIMUM_PRICE_EDIT_X + RECEPTION_MINIMUM_PRICE_EDIT_WIDTH, m_siY + RECEPTION_MINIMUM_PRICE_EDIT_Y + RECEPTION_MINIMUM_PRICE_EDIT_HEIGHT );
	m_hOecMinPrice	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 8, 99999999 );

	SetRect( &rcRect, m_siX + RECEPTION_MAXIMUM_PRICE_EDIT_X, m_siY + RECEPTION_MAXIMUM_PRICE_EDIT_Y, 
						m_siX + RECEPTION_MAXIMUM_PRICE_EDIT_X + RECEPTION_MAXIMUM_PRICE_EDIT_WIDTH, m_siY + RECEPTION_MAXIMUM_PRICE_EDIT_Y + RECEPTION_MAXIMUM_PRICE_EDIT_HEIGHT );
	m_hOecMaxPrice	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 8, 99999999 );

	Handler_OnlineEditControlEx::SetNextHandle( m_hOecMinPrice, m_hOecMaxPrice );
	Handler_OnlineEditControlEx::SetNextHandle( m_hOecMaxPrice, m_hOecMinPrice );
	// ===============================================================================

	// ��Ž�û �������ư!
	m_BOk.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_REGISTER), BUTTON_PUT_LEFT, TRUE );
	m_BCancel.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_EXIT), BUTTON_PUT_LEFT, TRUE );

	clGrp.LoadXspr( "Online\\GameSpr\\Reception_board.Spr", m_BoardSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Item_Slot.Spr", m_SlotSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Auction_Edit.Spr", m_ValueSpr );
		
	m_pAuctionList = new OnlineListControl(m_pMyOnlineWorld);
	m_pAuctionList->Init(60, 60, 1, 1, m_siX, m_siY, 0);

	SetPos(RECEPTION_POS_X,RECEPTION_POS_Y);
	SetAction(FALSE);
}

void OnlineReception::Free()
{
	// ===============================================================================
//	if( m_pMinimumPriceEdit )
//	{
//		m_pMinimumPriceEdit->Free();
//		delete m_pMinimumPriceEdit;
//		m_pMinimumPriceEdit = NULL;
//	}
//
//	if( m_pMaximumPriceEdit )
//	{
//		m_pMaximumPriceEdit->Free();
//		delete m_pMaximumPriceEdit;
//		m_pMaximumPriceEdit = NULL;
//	}
	// ===============================================================================
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	Handler_OnlineEditControlEx::KillControl( m_hOecMinPrice );
	Handler_OnlineEditControlEx::KillControl( m_hOecMaxPrice );
	// ===============================================================================

	if(m_pAuctionList)
	{
		delete m_pAuctionList;
		m_pAuctionList = NULL;
	}


	if (m_BoardSpr.Image) { clGrp.FreeXspr(m_BoardSpr); }
	if (m_SlotSpr.Image) { clGrp.FreeXspr(m_SlotSpr); }
	if (m_ValueSpr.Image) { clGrp.FreeXspr(m_ValueSpr); }

	m_pButtonSpr = NULL;
	

}

void OnlineReception::SetPos(SI32 siX,SI32 siY)
{
	m_siX = siX;
	m_siY = siY;

	m_BOk.SetX(m_siX + RECEPTION_OK_BUTTON_X);
	m_BOk.SetY(m_siY + RECEPTION_OK_BUTTON_Y);
	m_BCancel.SetX(m_siX + RECEPTION_CANCEL_BUTTON_X);
	m_BCancel.SetY(m_siY + RECEPTION_CANCEL_BUTTON_Y);

	m_pAuctionList->SetPos(m_siX + RECEPTION_ITEM_SLOT_X,m_siY + RECEPTION_ITEM_SLOT_Y);

	
	if (m_siNoField != -1) {
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
		m_BoardSpr.Header.Xsize, m_siY + m_BoardSpr.Header.Ysize);

	}



}


void OnlineReception::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{

	static int showcaret = 0;
	static int  caretcount = 0;

	caretcount++;
	if (caretcount >= 10)
	{
		caretcount = 0;
		showcaret ^= 1;

	}
	HDC hDC;

	if (m_bActive) {
		if (pSurface)
		{
			if( clGrp.LockSurface( pSurface ) == TRUE )
			{
				// �޹��
				clGrp.PutSpriteT( m_siX, m_siY, m_BoardSpr.Header.Xsize, m_BoardSpr.Header.Ysize, m_BoardSpr.Image );
				clGrp.PutSpriteT( m_siX + RECEPTION_MINIMUM_PRICE_EDIT_X - 2,m_siY + RECEPTION_MINIMUM_PRICE_EDIT_Y - 4 ,
					m_ValueSpr.Header.Xsize,m_ValueSpr.Header.Ysize,m_ValueSpr.Image);
				clGrp.PutSpriteT( m_siX + RECEPTION_MAXIMUM_PRICE_EDIT_X - 2,m_siY + RECEPTION_MAXIMUM_PRICE_EDIT_Y - 4 ,
					m_ValueSpr.Header.Xsize,m_ValueSpr.Header.Ysize,m_ValueSpr.Image);
				
				clGrp.PutSpriteT( m_siX + RECEPTION_ITEM_SLOT_X - 4,m_siY + RECEPTION_ITEM_SLOT_Y - 4,
					m_SlotSpr.Header.Xsize,m_SlotSpr.Header.Ysize,m_SlotSpr.Image);


				
				
				
				if (m_pButtonSpr) {
					m_BOk.Put		( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
					m_BCancel.Put	( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
				}
				
				clGrp.UnlockSurface( pSurface );
			}

			m_pAuctionList->Draw(pSurface);
			
			if( pSurface->GetDC( &hDC ) == DD_OK )
			{
				SetBkMode(hDC, TRANSPARENT);			
				SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				
				m_BOk.Put( hDC );
				m_BCancel.Put( hDC );

				COLORREF rgb = RGB(0,0,0);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_EXPLANATION_X, m_siY+RECEPTION_EXPLANATION_Y, RECEPTION_EXPLANATION_WIDTH,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CANNOT_RECEIVE), rgb);
			    m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_EXPLANATION_X, m_siY+RECEPTION_EXPLANATION_Y + 20, RECEPTION_EXPLANATION_WIDTH,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_FIVE_FAIL_WARNING), rgb);

				char strsusu[256];
				sprintf(strsusu,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_COMMISSION),AUCTION::FEE_RATE_INT);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_EXPLANATION_X, m_siY+RECEPTION_EXPLANATION_Y + 40, RECEPTION_EXPLANATION_WIDTH,strsusu, rgb);

				sprintf(strsusu,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_MIN_COMMISSION),AUCTION::MIN_FEE);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_EXPLANATION_X, m_siY+RECEPTION_EXPLANATION_Y + 60, RECEPTION_EXPLANATION_WIDTH,strsusu, rgb);

				sprintf(strsusu,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_SAVE_TO_BANK));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_EXPLANATION_X, m_siY+RECEPTION_EXPLANATION_Y + 80, RECEPTION_EXPLANATION_WIDTH,strsusu, rgb);

				// ���� ǥ��
				rgb = RGB(0,0,0);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_MINIMUM_PRICE_EDIT_X - 50, m_siY+RECEPTION_MINIMUM_PRICE_EDIT_Y, RECEPTION_MINIMUM_PRICE_EDIT_WIDTH, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_MINIMUM_PRICE ), rgb);

				rgb = RGB(0,0,0);

				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_MAXIMUM_PRICE_EDIT_X - 50, m_siY+RECEPTION_MAXIMUM_PRICE_EDIT_Y, RECEPTION_MAXIMUM_PRICE_EDIT_WIDTH, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_MAXIMUM_PRICE ), rgb);

				// �Է� ��ġ ���
//				char szTemp[256];
//				ZeroMemory(szTemp,256);
//				m_pMinimumPriceEdit->GetString(szTemp);
//				MONEY money = atol(szTemp);
//				MakeMoneyText(money,szTemp);
//
//				if (m_pMinimumPriceEdit->IsFocus() && showcaret)
//				{
//					strcat(szTemp,"_");
//
//				}
//
//				rgb = RGB(245, 245, 245);
//
//				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_MINIMUM_PRICE_EDIT_X, m_siY+RECEPTION_MINIMUM_PRICE_EDIT_Y, RECEPTION_MINIMUM_PRICE_EDIT_WIDTH, szTemp, rgb);
//
//				ZeroMemory(szTemp,256);
//				m_pMaximumPriceEdit->GetString(szTemp);
//				MONEY money2 = atol(szTemp);
//				MakeMoneyText(money2,szTemp);
//
//				if (m_pMaximumPriceEdit->IsFocus() && showcaret)
//				{
//					strcat(szTemp,"_");
//				}
//
//				rgb = RGB(245, 245, 245);
//				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+RECEPTION_MAXIMUM_PRICE_EDIT_X, m_siY+RECEPTION_MAXIMUM_PRICE_EDIT_Y, RECEPTION_MAXIMUM_PRICE_EDIT_WIDTH, szTemp, rgb);

				// ===============================================================================
				// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
				OnlineEditControlEx	*pOecEx;
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMinPrice );
				pOecEx->Draw( hDC, true );
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMaxPrice );
				pOecEx->Draw( hDC, true );
				// ===============================================================================

				pSurface->ReleaseDC( hDC );
			}
			
		}
		
	}
}

void OnlineReception::SetAction(BOOL bActive)
{
	OnlineEditControlEx *pOecEx = NULL;
	m_bActive = bActive;
	m_bMouseSwitch = FALSE;
	// Ȱ��ȭ�� �ɶ��� �������� ���ϰ� �ϰ� ��Ȱ��ȭ �ɶ��� �����̰� �Ѵ�.
	if (m_bActive)
	{
		// �κ��丮�� ���� ������ ���� �ش�.
		if(!m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(TRUE);
		// ��ΰ� ���� ������ �ݾ��ش�.
		if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);

		m_pAuctionList->Clear();

		m_siInvenSlot = -1;

		

		if (m_siNoField != -1) {
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		}
		m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
						m_BoardSpr.Header.Xsize, m_siY + m_BoardSpr.Header.Ysize);

		m_siStatus = RECEPTION_MAIN;
		m_uiSellFollowerID = -1;
		m_siItemCount = 0;

//		m_pMinimumPriceEdit->EnableWindow(TRUE);
//		m_pMaximumPriceEdit->EnableWindow(TRUE);
		
//		m_pMinimumPriceEdit->SetString("");
//		m_pMaximumPriceEdit->SetString("");
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
		pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMinPrice );
		pOecEx->SetValue( 0 );
		pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMaxPrice );
		pOecEx->SetValue( 0 );
		// ===============================================================================

		m_BOk.SetAction(FALSE);

		// �������̰� ��Ÿ Ŀ�ǵ� ���� ���Ұ�!!!
		m_pMyOnlineWorld->bCharMove = FALSE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);

	}
	else
	{
		if(m_pMyOnlineWorld->pOnlineInventory->IsActive()) m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);

		if (m_siNoField != -1) {
			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		}

		m_siNoField = -1;

//		m_pMinimumPriceEdit->EnableWindow(FALSE);
//		m_pMaximumPriceEdit->EnableWindow(FALSE);
		
//		m_pMinimumPriceEdit->Initialize();
//		m_pMaximumPriceEdit->Initialize();
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
		pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMinPrice );
		pOecEx->Clear();
		pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMaxPrice );
		pOecEx->Clear();
		// ===============================================================================

		// �̵� ���� �� ��ŸĿ�ǵ� ��밡��!!!
		m_pMyOnlineWorld->bCharMove = TRUE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);

		
	}
}

void OnlineReception::Process()
{
	OnlineEditControlEx	*pOecEx;
	if (m_bActive)
	{

		switch(m_siStatus) {
			
		case RECEPTION_MAIN:
			
			// Ok ��ư�� ��������
			if (m_BOk.Process(m_bMouseSwitch))
			{
				// ���� �÷����� �������� �ִٸ� ��� ��û �޽����� ������.
				if (m_pAuctionList->GetItemNum() == 1)
				{
					ItemInfo const *pTempItemInfo = NULL;
					pTempItemInfo = const_cast<const ItemInfo *>(m_pAuctionList->GetSlotInfo(0));

					char szPrice[256];

					ZeroMemory(szPrice,256);

					// ������ ��� ������ ��û�Ѵ�.
					AUCTION::sRequestSellItem packet;
					packet.command = AUCTION::REQUEST_SELL_ITEM;
					
//				    m_pMinimumPriceEdit->GetString(szPrice);
//					packet.minPrice = atol(szPrice);
//					m_pMaximumPriceEdit->GetString(szPrice);
//					packet.maxPrice = atol(szPrice);

					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
					pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecMinPrice );
					packet.minPrice	= pOecEx->GetValue();
					pOecEx			= Handler_OnlineEditControlEx::GetControl( m_hOecMaxPrice );
					packet.maxPrice	= pOecEx->GetValue();
				// ===============================================================================

					packet.article_itemID = pTempItemInfo->uiItemID;
					packet.article_quantity = pTempItemInfo->ItemCount;

					packet.atricle_hasFollowerSlot = m_uiSellFollowerID;

					DWORD fee = (packet.minPrice * AUCTION::FEE_RATE);
					fee = ((fee <= AUCTION::MIN_FEE) ?  AUCTION::MIN_FEE : fee);


					if (m_pMyOnlineWorld->pMyData->GetMoney() >= fee) {

						if (packet.minPrice >= 0 && packet.maxPrice >= 0 && (packet.maxPrice == 0 || packet.maxPrice >= packet.minPrice)) {

							m_moSendedMinimumMoney = packet.minPrice;

							



							

							m_pMyOnlineWorld->pOnlineClient->Write(&packet,sizeof(packet));
							// ���� ��� ���·�~
							m_siStatus = RECEPTION_WAITING_REPLY;

							m_BOk.SetAction(FALSE);

						}
					}
						

				}

			}
			else if (m_BCancel.Process(m_bMouseSwitch))
			{
				SetAction(FALSE);
			}

			RECT rect;
			rect.left = m_siX + RECEPTION_MINIMUM_PRICE_EDIT_X;
			rect.top = m_siY + RECEPTION_MINIMUM_PRICE_EDIT_Y - 4;
			rect.right = rect.left + RECEPTION_MINIMUM_PRICE_EDIT_WIDTH;
			rect.bottom = rect.top + RECEPTION_MINIMUM_PRICE_EDIT_HEIGHT;

			RECT rect2;
			rect2.left = m_siX + RECEPTION_MAXIMUM_PRICE_EDIT_X;
			rect2.top = m_siY + RECEPTION_MAXIMUM_PRICE_EDIT_Y - 4;
			rect2.right = rect2.left + RECEPTION_MAXIMUM_PRICE_EDIT_WIDTH;
			rect2.bottom = rect2.top + RECEPTION_MAXIMUM_PRICE_EDIT_HEIGHT;


			POINT pt;
			pt.x = IpD.Mouse_X;
			pt.y = IpD.Mouse_Y;
			if (IpD.LeftPressSwitch)
			{
				if (PtInRect(&rect,pt))
				{
//					if (!m_pMinimumPriceEdit->IsFocus())
//					{
//						m_pMinimumPriceEdit->SetFocus();
//					}
					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
					if( !Handler_OnlineEditControlEx::IsFocus( m_hOecMinPrice ) )
					{
						Handler_OnlineEditControlEx::SetFocus( m_hOecMinPrice );
					}
					// ===============================================================================

				}
				else if (PtInRect(&rect2,pt))
				{
//					if (!m_pMaximumPriceEdit->IsFocus())
//					{
//						m_pMaximumPriceEdit->SetFocus();
//					}
					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
					if( !Handler_OnlineEditControlEx::IsFocus( m_hOecMaxPrice ) )
					{
						Handler_OnlineEditControlEx::SetFocus( m_hOecMaxPrice );
					}
					// ===============================================================================
				}
				else
				{
//					if( m_pMinimumPriceEdit->IsFocus() || m_pMaximumPriceEdit->IsFocus()) 
//					{
//						::SetFocus(pGame->Hwnd);
//					}
					// ===============================================================================
					// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
					if( Handler_OnlineEditControlEx::IsFocus( m_hOecMaxPrice ) || Handler_OnlineEditControlEx::IsFocus( m_hOecMinPrice ) )
					{
						Handler_OnlineEditControlEx::ClearCurrentFocus();
					}
					// ===============================================================================
				};

				
			}
			
			
			
			// �κ��丮���� �巡�� �ϰ��ִ� ������ üũ
			// ����Ʈ�� ������� ���� üũ
			if (m_pAuctionList->GetItemNum() == 0)
			{
				
				if(IpD.LeftPressSwitch)
				{
					if(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->DragUp())
						m_siInvenSlot = m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetDragSlot();
				}
				
					

			}
			
			// �κ��丮 ĭ�� ����ִ´�.
			if (m_pAuctionList->GetSelectSlot() != -1 && IpD.LeftPressSwitch)
			{
//				if (m_pMinimumPriceEdit->IsFocus()) 
//				{
//					::SetFocus(pGame->Hwnd);
//				}
				// ===============================================================================
				// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecMaxPrice ) || Handler_OnlineEditControlEx::IsFocus( m_hOecMinPrice ) )
				{
					Handler_OnlineEditControlEx::ClearCurrentFocus();
				}
				// ===============================================================================
				
				// �巡�� ����!
				m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->StopDrag();

				// ���� //////////////////////////////////////////////
				// robypark 2004/12/2 17:18
				// ������ ������ �������� ��ſ� �ø� �� ����.
				if (m_siInvenSlot != -1)	// �巡�� ���� �������� ������
				{
					// ��ſ� �ø����� �������� ������ �����´�.
					ItemInfo const *pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));

					if (pTempItemInfo)
					{
						// ������ ���� ��� ���
						CItemHeader *pItemHeader =	m_pMyOnlineWorld->pItemsInfo->GetItem(pTempItemInfo->uiItemID);
						
						if (pItemHeader)
						{
							// ������ ������ �������̶��
							if (pItemHeader->m_pImperium)
							{
								// ����
								m_siStatus = RECEPTION_MAIN;
								break;
							}
						}
					}
				}
				// �� //////////////////////////////////////////////
				
				
				// ����Ʈ�� ����ְ�
				if (m_pAuctionList->GetItemNum() == 0)
				{
					
					// ����ִ� �������� ������
					if (m_siInvenSlot != -1)
					{

						// �뺴 ���� ����
						m_uiSellFollowerID = m_pMyOnlineWorld->pOnlineInventory->GetMercenarySlot();
			
						// �� �������� ������ �����´�.
						ItemInfo const *pTempItemInfo = NULL;

						pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));

						// �ִ밪�� ���� ���������� �����ϰ� ���⸦ ����!

						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->Initialize(pTempItemInfo->ItemCount);
						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetUnit(NUMBERCONTROL_UNIT_THING);
						m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->SetShow(IpD.Mouse_X, IpD.Mouse_Y);

						// ���ڸ� �Է¹޴´�
						m_siStatus = RECEPTION_ENTER_ITEMNUMBER;
									
						
					}
					
				}
				

				
			}
			break;
		case RECEPTION_ENTER_ITEMNUMBER:
			{

				// ���� �Է��� �޴´�.
				if(m_pMyOnlineWorld->pOnlineNumPad->pNumberControl->IsSuccess() == TRUE)
				{
					// 0���� �ƴϸ�
					if( m_pMyOnlineWorld->pOnlineNumPad->siCount != 0)
					{
						// ������ ���� ����
						m_siItemCount = m_pMyOnlineWorld->pOnlineNumPad->siCount;
						
						
						
						// Ȥ�ö� �뺴������ ��ü������ �𸣹Ƿ� ���� �뺴�������� ��ü
						m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiSellFollowerID);
						
						// �� �������� ������ �����´�.
						ItemInfo const *pTempItemInfo = NULL;	
						pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
						
						
						
						ItemInfo TempItemInfo;
						if(pTempItemInfo)
						{
							TempItemInfo.uiItemID		= pTempItemInfo->uiItemID;
							TempItemInfo.ItemCount		= m_siItemCount;
						}
											
						// ������ ������ �ϳ��ۿ�����.
						m_pAuctionList->AddItem(TempItemInfo,0);
						
						
						// ��� ��û��ư�� �������ִ�~
						m_BOk.SetAction(TRUE);
						
					}

					m_siStatus = RECEPTION_MAIN;
				}
				
				
			}
			break;
		case RECEPTION_WAITING_REPLY:
			break;

			
		}

		if (IpD.LeftPressSwitch) 
		{
			m_bMouseSwitch = TRUE;

		}
		else
		{
			m_bMouseSwitch = FALSE;
		}


	
	}

	

}



OnlineFieldAuction::OnlineFieldAuction()
{
	m_bEnter = FALSE;
	m_bSendEnterMessage = FALSE;
	m_pMyOnlineWorld = NULL;
	m_pButtonSpr = NULL;
	m_siNoField = -1;
	

}

OnlineFieldAuction::~OnlineFieldAuction()
{
	Free();
}

void OnlineFieldAuction::Init(cltOnlineWorld *pWorld)
{
	strcpy(m_szTalk," ");

	m_bKeepTalk = FALSE;

	m_siTalk = AUCTIONTALK_NO_AUCTION;
	m_siTalkDelay = 0;

	m_bUnderAuction = FALSE;
	m_bEnter = FALSE;

	m_pMyOnlineWorld = pWorld;

	m_pButtonSpr		=	m_pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_EXITBUTTON );

	// actdoll (2004/02/04 11:17) : ���ο� IME�� ���� �����ڽ� �ʱ�ȭ ����
//	m_pPriceEdit	=	new OnlineEditControl;
//	m_pPriceEdit->CreateEditBox(1000, 1000, 80, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 8, TRUE, FALSE );
//	m_pPriceEdit->SetNextWindow( m_pPriceEdit->GethWnd() );
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	
	// ===============================================================================

	m_BOk.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_BID), BUTTON_PUT_LEFT, TRUE );
	m_BCancel.Create(  0, 0, m_pButtonSpr[0].Header.Xsize, m_pButtonSpr[0].Header.Ysize, m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );
	SetPos(FIELD_AUCTION_POS_X,FIELD_AUCTION_POS_Y);

	RECT	rcRect;
	SetRect( &rcRect, m_siX + FIELD_BIDDING_PRICE_EDIT_X, m_siY + FIELD_BIDDING_PRICE_EDIT_Y, 
						m_siX + FIELD_BIDDING_PRICE_EDIT_X  + 75, m_siY + FIELD_BIDDING_PRICE_EDIT_Y + 15 );
	m_hOecPrice	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 8, 99999999 );
	Handler_OnlineEditControlEx::SetNextHandle( m_hOecPrice, m_hOecPrice );


	clGrp.LoadXspr( "Online\\GameSpr\\Auction_Board.Spr", m_BoardSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\Auction_Edit.Spr", m_ValueSpr );
	
	SetAction(FALSE);
	
}

void OnlineFieldAuction::OnGameStart()
{
	strcpy(m_szTalk," ");

	m_bKeepTalk = FALSE;

	m_siTalk = AUCTIONTALK_NO_AUCTION;
	m_siTalkDelay = 0;

	m_bUnderAuction = FALSE;
	m_bEnter = FALSE;

}


void OnlineFieldAuction::Free()
{
//	if( m_pPriceEdit )
//	{
//		m_pPriceEdit->Free();
//		delete m_pPriceEdit;
//		m_pPriceEdit = NULL;
//	}
	// ===============================================================================
	// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
	Handler_OnlineEditControlEx::KillControl( m_hOecPrice );
	// ===============================================================================

	if (m_BoardSpr.Image) { clGrp.FreeXspr(m_BoardSpr); }
	if (m_ValueSpr.Image) { clGrp.FreeXspr(m_ValueSpr); }

	m_pButtonSpr = NULL;

}

void OnlineFieldAuction::SetPos(SI32 siX,SI32 siY)
{
	m_siX = siX;
	m_siY = siY;

	m_BOk.SetX(m_siX + FIELD_OK_BUTTON_X);
	m_BOk.SetY(m_siY + FIELD_OK_BUTTON_Y);
	m_BCancel.SetX(m_siX + FIELD_CANCEL_BUTTON_X);
	m_BCancel.SetY(m_siY + FIELD_CANCEL_BUTTON_Y);

	if (m_siNoField != -1) {
		m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea(m_siNoField);	
		m_siNoField	= m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea(m_siX, m_siY, m_siX + 
		m_BoardSpr.Header.Xsize, m_siY + m_BoardSpr.Header.Ysize);

	}

	

}


void OnlineFieldAuction::Draw(LPDIRECTDRAWSURFACE7 pSurface)
{

	static int showcaret = 0;
	static int  caretcount = 0;

	char str[256];

	COLORREF rgb;
	OnlineEditControlEx		*pOecEx;
	
	
	

	// �������̽��� �׸��� �κ�
	caretcount++;
	if (caretcount >= 10)
	{
		caretcount = 0;
		showcaret ^= 1;

	}
	HDC hDC;
	if (m_bActive) {
		if (pSurface)
		{
			if( clGrp.LockSurface( pSurface ) == TRUE )
			{
				// �޹��
				clGrp.PutSpriteT( m_siX, m_siY, m_BoardSpr.Header.Xsize, m_BoardSpr.Header.Ysize, m_BoardSpr.Image );

				
				clGrp.PutSpriteT( m_siX + FIELD_BIDDING_PRICE_EDIT_X - 2 ,m_siY + FIELD_BIDDING_PRICE_EDIT_Y - 4,
					m_ValueSpr.Header.Xsize,m_ValueSpr.Header.Ysize,m_ValueSpr.Image);
				
				if (m_pButtonSpr) {
					m_BOk.Put		( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
					m_BCancel.Put	( m_pButtonSpr, 0, 1, 2, BUTTON_PUT_NOMOVE );
				}
				
				clGrp.UnlockSurface( pSurface );
			}
			
			if( pSurface->GetDC( &hDC ) == DD_OK )
			{
				SetBkMode(hDC, TRANSPARENT);			
				SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
				
				m_BOk.Put( hDC );
				m_BCancel.Put( hDC );

//				char szTemp[256];
//				ZeroMemory(szTemp,256);
//				m_pPriceEdit->GetString(szTemp);
//				MONEY money = atol(szTemp);
//				MakeMoneyText(money,szTemp);
//
//				if (m_pPriceEdit->IsFocus() && showcaret)
//				{
//					strcat(szTemp,"_");
//				}
//
//				COLORREF rgb = RGB(245, 245, 245);
//				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+FIELD_BIDDING_PRICE_EDIT_X, m_siY+FIELD_BIDDING_PRICE_EDIT_Y, FIELD_BIDDING_PRICE_EDIT_WIDTH, szTemp, rgb);

				// ===============================================================================
				// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
				pOecEx->Draw( hDC, true );
				// ===============================================================================

				char szTemp[256];
				rgb = RGB(245,245,245);
				sprintf(szTemp, "%s :", m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TENDER_PRICE));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, m_siX+FIELD_BIDDING_PRICE_EDIT_X - 50, m_siY+FIELD_BIDDING_PRICE_EDIT_Y, FIELD_BIDDING_PRICE_EDIT_WIDTH, szTemp, rgb);

				
	

				
				pSurface->ReleaseDC( hDC );
			}
			
		}
	}

	// ��ſ� �����Ͽ���
	
	if (m_bEnter) {

		

		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			// ��� �ǳ��� �׸���.
			clGrp.FillBoxTrans(FIELD_AUCTION_PANEL_X,FIELD_AUCTION_PANEL_Y,FIELD_AUCTION_PANEL_WIDTH,FIELD_AUCTION_PANEL_HEIGHT,0);
			clGrp.UnlockSurface( pSurface );
		}
		
		// ������϶�
		if (m_bUnderAuction)
		{
			switch (m_ArticleInfo.articleKind)
			{
				
				// �������� ������̴�.
			case AUCTION::ARTICLE_ITEM:
				{
					CItemHeader	*pItem;
					pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_ItemInfo.itemID );											
						
					if( clGrp.LockSurface( pSurface ) == TRUE )
					{
						
						if (pItem) {
							XSPR *pImage;
							pImage = m_pMyOnlineWorld->pOnlineResource->GetxsprItem(pItem->m_siSpriteFile);
							if (pImage) {
								clGrp.PutSpriteT(FIELD_AUCTION_PANEL_X + 10,FIELD_AUCTION_PANEL_Y + 20,pImage->Header.Xsize, pImage->Header.Ysize,&pImage->Image[pImage->Header.Start[pItem->m_siSpriteFont]]);
							}
							
							
						}
						clGrp.UnlockSurface( pSurface );
					}

					if (pSurface->GetDC(&hDC) == DD_OK)
					{
						SetBkMode(hDC, TRANSPARENT);			
						if (pItem) {							
							// ǰ�� : %s  ���� : %ld
							sprintf(str,"%s : %s %s : %ld",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_ARTICLE ),m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ),m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_NUMBER ),m_ItemInfo.quantity);
							COLORREF rgb = RGB(245, 245, 245);
							m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, FIELD_AUCTION_PANEL_X + 70, FIELD_AUCTION_PANEL_Y + 20, 200, str, rgb);
						}

		
						pSurface->ReleaseDC(hDC);
					}
					
				}
				break;
			case AUCTION::ARTICLE_FOLLOWER:
				{
					
				}
				break;
			case AUCTION::ARTICLE_PRODUCTION:
				{
					
				}
				break;
			}
			// �������� ����ϴ� ������ ���
			char MinimumStr[64];
			char MaximumStr[64];
	
			// �ּҰ��� �ִ밡�� 0�̸� ���� �̶�� ���ڿ��� ����Ѵ�.
			if ( m_ArticleInfo.minPrice) {
				MakeMoneyText(m_ArticleInfo.minPrice, (char*)MinimumStr);
				
			}
			else
			{
				// �����̶�� �ؽ�Ʈ ���
				sprintf(MinimumStr,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_NONE ));
			}
			
			if ( m_ArticleInfo.maxPrice) {
				MakeMoneyText(m_ArticleInfo.maxPrice, (char*)MaximumStr);
				
			}
			else
			{
				sprintf(MaximumStr,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_NONE ));
				
			}

			if (pSurface->GetDC(&hDC) == DD_OK)
			{
				SetBkMode(hDC, TRANSPARENT);			
				
				// ����� : %s �ּ������� : %s �ִ������� : %s
				sprintf(str,"%s : %s %s : %s %s : %s",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_AUCTIONEER ),m_ArticleInfo.strAuctioneer,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_MINIMUM_PRICE ),MinimumStr,m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_MAXIMUM_PRICE ),MaximumStr);
				rgb = RGB(245, 245, 245);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, FIELD_AUCTION_PANEL_X + 70, FIELD_AUCTION_PANEL_Y + 40, 400, str, rgb);				
				// m_strMaxBidder �� �������� �̸��̰� �̸��� ������ (���̰� 0)�̸� ���� ���� �������Ѵ�.)			
				// ������ : %s  ���������� : %ld 
				char szCurrentBidPrice[256];

				MakeMoneyText(m_siCurrentBidPrice, (char*)szCurrentBidPrice);
				
				sprintf(str,"%s : %s  %s : %s %s : %ld",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_BIDDER ),strlen(m_strMaxBidder) > 0 ? m_strMaxBidder : m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_NONE ),m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_CURRENT_BID_PRICE ),szCurrentBidPrice,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_FAIL_COUNT),m_ArticleInfo.numFail);
				rgb = RGB(245, 245, 245);
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, FIELD_AUCTION_PANEL_X + 70, FIELD_AUCTION_PANEL_Y + 60, 400, str, rgb);
				pSurface->ReleaseDC(hDC);
			}
		}
		else
		{
			if (pSurface->GetDC(&hDC) == DD_OK)
			{
				SetBkMode(hDC, TRANSPARENT);			
				rgb = RGB(245, 245, 245);
				
				sprintf(str,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_NO_PROGRESS));
				m_pMyOnlineWorld->pOnlineMegaText->DrawTextLine2(hDC, FIELD_AUCTION_PANEL_X + 30, FIELD_AUCTION_PANEL_Y + 10, 600, str, rgb);
				
				pSurface->ReleaseDC(hDC);
			}
			
			
		}
		
		// ��� �����ڸ� �׸��� �κ�
		int count = 0;
		NPCIndexHeader *pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(count);

		// ��� NPC�� ��縦 ����Ѵ�.
		while (pNPC)
		{
			SI32 siAuctioneerX,siAuctioneerY;
			m_pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(pNPC->siMapXPos,pNPC->siMapYPos,siAuctioneerX,siAuctioneerY);
			
			siAuctioneerX -= m_pMyOnlineWorld->pOnlineMap->GetRX();
			siAuctioneerY -= m_pMyOnlineWorld->pOnlineMap->GetRY();
			
			m_pMyOnlineWorld->pOnlineMegaText->DrawText(AUCTION_TEXT_UNIQUE_ID,m_szTalk,RGB(255,255,255),TRUE);
						
			// SurfaceWidth�� �ٲ�����ϱ⶧����!!
			if( clGrp.LockSurface( pSurface ) == TRUE )
			{
				clGrp.UnlockSurface( pSurface );
			}
			m_pMyOnlineWorld->pOnlineMegaText->Show(AUCTION_TEXT_UNIQUE_ID, pSurface, siAuctioneerX,siAuctioneerY - 60);			

			count++;
			pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(count);
		}
	}
 
}

void OnlineFieldAuction::SetAction(BOOL bActive)
{
	m_bActive = bActive;
	m_bMouseSwitch = FALSE;
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );;
	// Ȱ��ȭ�� �ɶ��� �������� ���ϰ� �ϰ� ��Ȱ��ȭ �ɶ��� �����̰� �Ѵ�.
	if (m_bActive)
	{
		// 0������ ����
//		m_pPriceEdit->SetString("");
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
		pOecEx->SetValue( 0 );
		// ===============================================================================
		// ������ִ� �������̽� ��� �ݱ�
		if(m_pMyOnlineWorld->pOnlineInventory->IsActive())			m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
		if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())			m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
		if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive())			m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);
		if(m_pMyOnlineWorld->m_pMercenary->IsAction())				m_pMyOnlineWorld->m_pMercenary->OnButtonPush();
		if(m_pMyOnlineWorld->pOnlineTip->IsAction())				m_pMyOnlineWorld->pOnlineTip->SetAction(FALSE);
		if(m_pMyOnlineWorld->pOnlineTrade->IsActive())				m_pMyOnlineWorld->pOnlineTrade->SetActive(FALSE);
		if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())			m_pMyOnlineWorld->pOnlineBooth->SetActionIn(FALSE);
		if(m_pMyOnlineWorld->pOnlineMsgBox->IsActive())				m_pMyOnlineWorld->pOnlineMsgBox->SetAction(FALSE);
		if(m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->IsActive())	m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->SetAction(FALSE);
		
		// �������̰� ��Ÿ Ŀ�ǵ� ���� ���Ұ�!!!
		m_pMyOnlineWorld->bCharMove = FALSE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(FALSE);

	}
	else
	{
		// ���� ȸ��~
		m_pMyOnlineWorld->bCharMove = TRUE;
		m_pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);

//		m_pPriceEdit->Initialize();
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
		pOecEx->SetValue( 0 );
		// ===============================================================================
	}
}

void OnlineFieldAuction::Process()
{
	static DWORD requesttime = 0;
	

	
	BOOL bNPC = FALSE;
	NPCIndexHeader *pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(0);
	OnlineEditControlEx		*pOecEx;

	

	// ��� NPC�� ������
	if (pNPC)
	{
		// ��� ���� ���� �̸�
		if (m_bEnter)
		{
			
			// ���ΰ��� ��ġ�� ���Ѵ�.
			SI16 siHeroX,siHeroY;
			m_pMyOnlineWorld->pIOnlineChar->GetPos(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);

			BOOL bOut = TRUE;
			

			int count = 0;
			while (pNPC) {
				// npc�ֺ� 12ĭ �̳��� �ִ��� üũ 
				if ( (abs(pNPC->siMapXPos - siHeroX) + abs(pNPC->siMapYPos - siHeroY)) <= 12)
				{
					bOut = FALSE;
				
				}
				count++;
				pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(count);
			}

			// npc�ֺ��� ������ ������ ��Ŷ������  return;
			if (bOut)
			{
				AUCTION::sRequestLeave packet;
				packet.command = AUCTION::REQUEST_LEAVE;
				
				m_pMyOnlineWorld->pOnlineClient->Write(&packet,sizeof(packet));
				
				
				// �������� ��ſ� �������� �ƴѻ��·� �ٲ۴�.
				m_bEnter = FALSE;
				SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineMegaText->Free(AUCTION_TEXT_UNIQUE_ID);
				
				return;
			}
			
			if(m_pMyOnlineWorld->pOnlineTrade->IsActive())		m_pMyOnlineWorld->pOnlineTrade->SetActive(FALSE);			
			if(m_pMyOnlineWorld->pOnlineBooth->IsActionIn())	m_pMyOnlineWorld->pOnlineBooth->SetActionIn(FALSE);			
			
			// ��Ű� �������̸�
			if (m_bUnderAuction)
			{
				switch (m_ArticleInfo.articleKind)
				{
					
					// �������� ������̴�.
				case AUCTION::ARTICLE_ITEM:
					{
						CItemHeader	*pItem;
						pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_ItemInfo.itemID );																	


						// ���콺Ŀ���� �����۳��������� ������ ǥ��						
						RECT rect;
						rect.left = FIELD_AUCTION_PANEL_X + 10;
						rect.top = FIELD_AUCTION_PANEL_Y + 20;
						rect.right = rect.left + 64;
						rect.bottom = rect.top + 64;						
						POINT pt;
						pt.x = IpD.Mouse_X;
						pt.y = IpD.Mouse_Y;
						
						if (PtInRect(&rect,pt))
						{
							m_pMyOnlineWorld->pOnlineHelp->SetTextAuctionItem( pItem, (SI16)pt.x, (SI16)pt.y, ON_HELP_DOWN);														
						}
																							
						
						
					}
					break;
				case AUCTION::ARTICLE_FOLLOWER:			
					break;

				case AUCTION::ARTICLE_PRODUCTION:			
					break;
				}				
				
			}		
			
		}
		// �ȵ� ����
		else
		{

			// �޽����� �������̾��� 
			if (!m_bSendEnterMessage) {

				// ���ΰ��� ��ġ�� ���Ѵ�.
				SI16 siHeroX,siHeroY;
				m_pMyOnlineWorld->pIOnlineChar->GetPos(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), &siHeroX, &siHeroY);


				BOOL bEnter = FALSE;

				int count = 0;
				NPCIndexHeader *pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(count);

				while (pNPC) {					
					if ( (abs(pNPC->siMapXPos - siHeroX) + abs(pNPC->siMapYPos - siHeroY)) <= 7)
					{
						bEnter = TRUE;						
					}
					count++;
					pNPC = m_pMyOnlineWorld->pOnlineMap->m_NPCParser.GetAuctionNPC(count);
				}

				

				if (bEnter)
				{
					// ������û�� 3�ʰ� ��������
					if (timeGetTime() - requesttime >= 3000) {
						
						AUCTION::sRequestEnter packet;
						packet.command = AUCTION::REQUEST_ENTER;
						
						m_pMyOnlineWorld->pOnlineClient->Write(&packet,sizeof(packet));
						m_bSendEnterMessage = TRUE;
						
						requesttime = timeGetTime();
					}
					
				}

			}				
		}

		TalkChange();

		
		
		
		
	}
	else
	{
		// ��ſ� �� ���� �̸�
		if (m_bEnter)
		{
			// ������ �޽��� ����
			AUCTION::sRequestLeave packet;
			packet.command = AUCTION::REQUEST_LEAVE;
			
			m_pMyOnlineWorld->pOnlineClient->Write(&packet,sizeof(packet));
			m_pMyOnlineWorld->pOnlineMegaText->Free(AUCTION_TEXT_UNIQUE_ID);
			SetAction(FALSE);
			m_bEnter = FALSE;
			
			
		}
	
	}

	

	
	if (m_bActive)
	{
		
		// �������̽� �Է¿� ���� ó��
		RECT rect;
		rect.left = m_siX + FIELD_BIDDING_PRICE_EDIT_X;
		rect.top = m_siY + FIELD_BIDDING_PRICE_EDIT_Y;
		rect.right = rect.left + FIELD_BIDDING_PRICE_EDIT_WIDTH;
		rect.bottom = rect.top + FIELD_BIDDING_PRICE_EDIT_HEIGHT;
		
		POINT pt;
		pt.x = IpD.Mouse_X;
		pt.y = IpD.Mouse_Y;

		// ����â�� ������ ������ �ٲܼ�������
		if (IpD.LeftPressSwitch)
		{
//			if (PtInRect(&rect,pt))
//			{
//				if (!m_pPriceEdit->IsFocus())
//				{
//					m_pPriceEdit->SetFocus();
//				}
//			}
//			else 
//			{
//				if (m_pPriceEdit->IsFocus()) 
//				{
//					::SetFocus(pGame->Hwnd);
//				}
//			};
			
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
			if (PtInRect(&rect,pt))
			{
				if( !Handler_OnlineEditControlEx::IsFocus( m_hOecPrice ) )
				{
					Handler_OnlineEditControlEx::SetFocus( m_hOecPrice );
				}
			}
			else 
			{
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecPrice ) ) 
				{
					Handler_OnlineEditControlEx::ClearCurrentFocus();
				}
			}
		// ===============================================================================
		}
		
		// Ok ��ư�� ��������
		if (m_BOk.Process(m_bMouseSwitch))
		{

			if (m_ArticleInfo.uniqID != 0) {

				BOOL bBidSuccess = FALSE;
				int errorcode = 0;
				
				char szPrice[256];
				char pTempTalk[256];
				ZeroMemory(szPrice,256);
				// ������ ������ ��û�Ѵ�..
				AUCTION::sRequestBidding packet;
				packet.command = AUCTION::REQUEST_BIDDING;
				packet.auctionUniqID = m_ArticleInfo.uniqID;
				
				
				
//				m_pPriceEdit->GetString(szPrice);
//				packet.biddingPrice = atol(szPrice);
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
				pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecPrice );
				packet.biddingPrice = pOecEx->GetValue();
		// ===============================================================================
				
				
				if (packet.biddingPrice > 0 ) {
					if (packet.biddingPrice <= m_pMyOnlineWorld->pMyData->GetMoney()) {
						if ( packet.biddingPrice >= ((m_siCurrentBidPrice * 11) / 10) || packet.biddingPrice == m_ArticleInfo.maxPrice) {
							if ( packet.biddingPrice >= m_ArticleInfo.minPrice && (packet.biddingPrice <= m_ArticleInfo.maxPrice || m_ArticleInfo.maxPrice == 0)) {
								
								m_pMyOnlineWorld->pOnlineClient->Write(&packet,sizeof(packet));
								sprintf(pTempTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_PLAYER_CHAT_BID),packet.biddingPrice);
								m_pMyOnlineWorld->pIOnlineChar->SetSpeech(m_pMyOnlineWorld->pMyData->GetMyCharUniqueID(), pTempTalk);
								
								// �ʵ� ä�ø���Ʈ�� �ֱ�
								m_pMyOnlineWorld->pOnlinePannel->AddChat(0, pTempTalk, CHATTINGMODE_NORMAL);
								
								// �������� ���� ���� �����ش�.
								m_pMyOnlineWorld->pOnlineClientChatting->MessageSend_SendChatting(pTempTalk);

								bBidSuccess = TRUE;
								
							}
							else
							{
								errorcode = 1;
							}
						}
						else
						{
							errorcode = 1;
						}
					}
					else
					{
						errorcode = 2;
					}

					
				}
				else
				{
					errorcode = 1;
				}
				if (!bBidSuccess)
				{
					MONEY minprice = max(((m_siCurrentBidPrice * 11) / 10),m_ArticleInfo.minPrice);
					MONEY maxprice = m_ArticleInfo.maxPrice;

					switch(errorcode) {
					case 1:
						if (maxprice > 0)
						{		
							
							sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_BIDDING_PRICE_RANGE), minprice ,maxprice);
						}
						else
						{
							sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_BIDDING_PRICE_RANGE2), minprice);							
						}
						break;
					case 2:
						sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_NOT_ENOUGH_MONEY));							
						break;
					}
					m_bKeepTalk = TRUE;
					m_siTalkDelay = 120;
						
				}
			}

			
		}
		// ��� ��ư�� ��������
		else if (m_BCancel.Process(m_bMouseSwitch))
		{
			// �ڽ��� �ְ�� �����ڸ�
			if (!strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(),m_strMaxBidder))
			{
				// �ְ�� �����ڴ� ���������ٴ� �޽����� ����ϰ� �Է��� ����

				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_YOU_CANNOT_GO_OUT));
			}
			else 
			{
				
//				if (m_pPriceEdit->IsFocus()) 
//				{				
//					::SetFocus(pGame->Hwnd);
//				};
		// ===============================================================================
		// actdoll (2004/05/04 16:33) : [OECEX] ���� ����Ʈ ��Ʈ�� ����
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecPrice ) )
				{
					Handler_OnlineEditControlEx::ClearCurrentFocus();
				}
		// ===============================================================================
				SetAction(FALSE);
			}
		}

		if (IpD.LeftPressSwitch) 
		{
			m_bMouseSwitch = TRUE;

		}
		else
		{
			m_bMouseSwitch = FALSE;
		}

	}
}

BOOL  OnlineFieldAuction::CanBid()
{
	
	// �󽽷��� ������ TRUE �ƴ� FALSE;
	for (int a = 0; a < ON_MAX_FOLLOWER_NUM; a++) {
		if (m_pMyOnlineWorld->pMyData->FindEmptyItemBuffer(a) != 65535) return TRUE;
	}
	return FALSE;
}

BOOL OnlineFieldAuction::CanWeight()
{
	SI16 siNowCharLiftWeight;
	CItemHeader	*pItem;
	SI16 siTotalItemWeight;
	pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_ItemInfo.itemID );

	siTotalItemWeight = (SI16)(pItem->m_uiWeight * m_ItemInfo.quantity);

	for(SI16 i = 0 ; i < ON_MAX_FOLLOWER_NUM; i++)
	{
		siNowCharLiftWeight = m_pMyOnlineWorld->pMyData->GetFollowerParameterMaxWeight(i) - m_pMyOnlineWorld->pMyData->GetFollowerParameterTotalItemWeight(i);
		
		if(siNowCharLiftWeight > siTotalItemWeight)
			return TRUE;
	}
	return FALSE;
}

void OnlineFieldAuction::Enter()
{
	m_bKeepTalk = FALSE;
	m_bEnter = TRUE;
	m_pMyOnlineWorld->pOnlineMegaText->CreateGDIPage(AUCTION_TEXT_UNIQUE_ID);

	// �κ��丮�� ���� ������ �ݾ��ش�.
	if(m_pMyOnlineWorld->pOnlineInventory->IsActive())		m_pMyOnlineWorld->pOnlineInventory->SetActive(FALSE);
	// ��ΰ� ���� ������ �ݾ��ش�.
	if(m_pMyOnlineWorld->pOnlineTradeBook->IsAction())		m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
	// �̴ϸʵ� �ݴ´�!!
	if(m_pMyOnlineWorld->pOnlineMiniMap->IsActive()) m_pMyOnlineWorld->pOnlineMiniMap->SetActive(FALSE);
	// �� �ݾ�!!
	if(m_pMyOnlineWorld->pOnlineSetting->IsAction()) m_pMyOnlineWorld->pOnlineSetting->SetAction(FALSE);



}

void OnlineFieldAuction::Exit()
{

}

void OnlineFieldAuction::TalkChange()
{

	if (m_siTalkDelay <= 0)
	{
		switch(m_siTalk)
		{		
		case AUCTIONTALK_NO_AUCTION:					
			m_siTalk++;
			break;
		case AUCTIONTALK_NO_AUCTION+1:					
			m_siTalk++;
			break;
		case AUCTIONTALK_NO_AUCTION+2:					
			m_siTalk = AUCTIONTALK_NO_AUCTION;
			break;		
		case AUCTIONTALK_ITEM:
			m_siTalk++;
			break;
		case AUCTIONTALK_ITEM + 1:
			m_siTalk++;
			break;		
		case AUCTIONTALK_ITEM + 2:
			m_siTalk = AUCTIONTALK_ITEM;
			break;				
		case AUCTIONTALK_NEW:
			switch(m_ArticleInfo.articleKind)
			{
			case AUCTION::ARTICLE_ITEM:				
				m_siTalk = AUCTIONTALK_ITEM;				
				break;
			case AUCTION::ARTICLE_FOLLOWER:				
				m_siTalk = AUCTIONTALK_FOLLOWER;				
				break;
			case AUCTION::ARTICLE_PRODUCTION:
				m_siTalk = AUCTIONTALK_PROPERTY;				
				break;						
			}
			break;
		default:			
			break;
		}

		m_siTalkDelay = DEFAULT_TALK_DELAY;
		m_bKeepTalk = FALSE;
	
		
	}
	else 
	{
		m_siTalkDelay--;

	}

	if (!m_bKeepTalk)
	{
		switch(m_siTalk)
		{		
		case AUCTIONTALK_NO_AUCTION:		
			strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_NO_ITEM1));	
			break;
		case AUCTIONTALK_NO_AUCTION+1:		
			strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_NO_ITEM2));				
			break;
		case AUCTIONTALK_NO_AUCTION+2:		
			strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_NO_ITEM3));				
			break;		
		case AUCTIONTALK_ITEM:
			{
				CItemHeader	*pItem;
				pItem	=	m_pMyOnlineWorld->pItemsInfo->GetItem( m_ItemInfo.itemID );																				
				sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_NAME),m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ),m_pMyOnlineWorld->pOnlineText->Get( pItem->m_siItemName ));	
			}
			break;
		case AUCTIONTALK_ITEM + 1:
			{
				strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_ON_ITEM1));	
			}
			break;		
		case AUCTIONTALK_ITEM + 2:
			{
				strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_ON_ITEM2));	
			}
			break;			
		default:	
			strcpy(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_NO_ITEM3));				
			break;
		}


	}

		

}

void OnlineReception::WaitingReLay(BYTE *pMsg)
{
	AUCTION::sResponseSellArticle *pSellMsg = (AUCTION::sResponseSellArticle *)pMsg;

	if (pSellMsg)
	{
		switch(pSellMsg->responseKind)
		{
		case AUCTION::OK:
			{
				m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_REGISTER_SUCCESS));
				
				// �÷����� �������� ���ش�
				// Ȥ�ö� �뺴������ ��ü������ �𸣹Ƿ� ���� �뺴�������� ��ü
				m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(m_uiSellFollowerID);
				
				
				// �÷����� �������� ��������.
				
				ItemInfo const *pTempItemInfo = NULL;	
				pTempItemInfo = const_cast<const ItemInfo *>(m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->GetSlotInfo(m_siInvenSlot));
				
				MyItemData TempItemData;
				TempItemData.siPosInInventory	= m_siInvenSlot;
				TempItemData.uiID				= pTempItemInfo->uiItemID;
				TempItemData.uiQuantity			= m_siItemCount;
				
				m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->DeleteSlot(m_siInvenSlot,m_siItemCount);
				m_pMyOnlineWorld->pMyData->DelItem(m_uiSellFollowerID, &TempItemData);
				m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
				
				DWORD fee = (this->m_moSendedMinimumMoney * AUCTION::FEE_RATE);
				fee = ((fee <= AUCTION::MIN_FEE) ? AUCTION::MIN_FEE : fee);
				
				m_pMyOnlineWorld->pMyData->SetMoney(m_pMyOnlineWorld->pMyData->GetMoney() - fee);
				
				break;
			}
		default:
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_TOO_MANY_RECEPTION));
			break;

		}
		

		SetAction(FALSE);
	}	
}

void OnlineFieldAuction::Bidding(BYTE *pMsg)
{
	// �ڽ��� ���� ��û�� ���� ����!
	AUCTION::sResponseBidding *pResBid = (AUCTION::sResponseBidding *)pMsg;
	
	if (pResBid)
	{
		
		switch(pResBid->responseKind)
		{
			// ����������~
		case AUCTION::OK:
			break;
			// ����������~
		default:
			sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_BE_LATE));
			m_bKeepTalk = TRUE;
			m_siTalkDelay = 120;
			break;
		}
		
	}
}

void OnlineFieldAuction::ReportReMainTime(BYTE *pMsg)
{
	AUCTION::sReportRemainTime *pRepRemainTime = (AUCTION::sReportRemainTime *)pMsg;

	if (pRepRemainTime)
	{
		m_dwRemainTime = pRepRemainTime->remainTime;
		sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_REMAIN_SECOND),m_dwRemainTime / 1000);
		m_bKeepTalk = TRUE;
		m_siTalkDelay = 120;

	}
}

void OnlineFieldAuction::ReportBidding(BYTE *pMsg)
{
	// �̺κп����� �޽����� ��Ű�°��� �������� �ϴ� �޽����� �޾ƺ��� ���� ������� �������� �������� Ȯ���ϵ����Ѵ�!!
	
	// ������ ������ �����ߴپ�~~~
	AUCTION::sReportBidding *pRepBid = (AUCTION::sReportBidding *)pMsg;
	
	if (pRepBid)
	{
		// ������!
		if (pRepBid->uniqID == m_ArticleInfo.uniqID)
		{
			// �ִ������� ����!!!

			if (strlen(pRepBid->strBidder) > 0) {
				
				sprintf(m_strMaxBidder,pRepBid->strBidder);				
				
				
				m_siCurrentBidPrice = pRepBid->biddingPrice;
				
				sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_CHAT_BID),m_strMaxBidder,m_siCurrentBidPrice);
				m_bKeepTalk = TRUE;
				m_siTalkDelay = 120;
			}
			else
			{
				strcpy(m_strMaxBidder,"");
				m_siCurrentBidPrice = 0;
				
			}	
		}		
	}
}

void OnlineFieldAuction::ReportSuccessful(BYTE *pMsg)
{
	// ���������� �������� �����ƴپ�~
	
	AUCTION::sReportSuccessfulBid *pRepSuc = (AUCTION::sReportSuccessfulBid *)pMsg;
	
	if (pRepSuc)
	{
		// ������!
		if (pRepSuc->uniqID == m_ArticleInfo.uniqID)
		{		
			// ���� ������̾ƴϴ�.
			m_bUnderAuction = FALSE;

			sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_SUCCESSFUL_BIDDER),m_strMaxBidder);
			m_bKeepTalk = TRUE;
			m_siTalkDelay = 120;
			m_siTalk = AUCTIONTALK_NO_AUCTION;


			// �ִ������ڰ� ����!!!!
			if (!strcmp(m_pMyOnlineWorld->pMyData->GetMyCharName(),m_strMaxBidder))
			{
				// ���� ������� ��������  ���ųִ´�,!!!
				switch(pRepSuc->articleKind)
				{
				case AUCTION::ARTICLE_ITEM:
					{
						AUCTION::sArticleItemInfo2 *pInfo = (AUCTION::sArticleItemInfo2 *)pRepSuc->articleData;

						
						MyItemData TempItemData;
						TempItemData.siPosInInventory	= pInfo->posInInventory;
						TempItemData.uiID				= m_ItemInfo.itemID;
						TempItemData.uiQuantity			= m_ItemInfo.quantity;

						
						// �÷����� �������� ���ش�.
						// Ȥ�ö� �뺴������ ��ü������ �𸣹Ƿ� ���� �뺴�������� ��ü
			/*			m_pMyOnlineWorld->pOnlineInventory->SetMercenarySlot(pInfo->hasFollowerSlot);

						ItemInfo AddInfo;
						AddInfo.uiItemID = m_ItemInfo.itemID;
						AddInfo.ItemCount = m_ItemInfo.quantity;

						m_pMyOnlineWorld->pOnlineInventory->GetLCInventory()->AddItem( AddInfo,pInfo->posInInventory);*/

						SI16 siTempPos=0;
			

					

		

						
						m_pMyOnlineWorld->pMyData->GetFollowersInterface()->AddItem(pInfo->hasFollowerSlot, m_ItemInfo.itemID,m_ItemInfo.quantity,&siTempPos);
						m_pMyOnlineWorld->pOnlineInventory->FillItemListBox();
						
						
					}
					break;
				case AUCTION::ARTICLE_FOLLOWER:
					break;
				case AUCTION::ARTICLE_PRODUCTION:
					break;
				
				
				}

				// ���� ���~
				m_pMyOnlineWorld->pMyData->SetMoney(m_pMyOnlineWorld->pMyData->GetMoney() - m_siCurrentBidPrice);


			}

			
			
			// ������� ������
			sprintf(m_strMaxBidder,"");

			// �ִ밡�� ������~
			m_siCurrentBidPrice = 0;

			SetAction(FALSE);
		}
		
		
	}	
}

void OnlineFieldAuction::ReportFail(BYTE *pMsg)
{
	// ��� �������� �ƹ��� �Ȼ��~ T.T

	AUCTION::sReportFailBid *pRepFail = (AUCTION::sReportFailBid *)pMsg;
	
	if (pRepFail)
	{
		// ������!
		if (pRepFail->uniqID == m_ArticleInfo.uniqID)
		{
			// ť���� ������.


			sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_FAILED));
			m_bKeepTalk = TRUE;
			m_siTalkDelay = 120;
			m_siTalk = AUCTIONTALK_NO_AUCTION;
			
			// ���� ������̾ƴϴ�.
			m_bUnderAuction = FALSE;
			// ������� ������
			sprintf(m_strMaxBidder,"");
			
			// �ִ밡�� ������~
			m_siCurrentBidPrice = 0;

			SetAction(FALSE);
		}
		
	}
}

void OnlineFieldAuction::ReportNew(BYTE *pMsg)
{
	// ���� ��Ű� �����پ�~.
	AUCTION::sReportNewInfo *pRepNew = (AUCTION::sReportNewInfo *)pMsg;
	
	if (pRepNew)
	{
		// ���ο� ������� �Է�!!
		m_ArticleInfo = pRepNew->_c;

		// ������� ������
		sprintf(m_strMaxBidder,"");

		// �ִ밡�� ������~
		m_siCurrentBidPrice = 0;
		

		switch(m_ArticleInfo.articleKind)
		{
		case AUCTION::ARTICLE_ITEM:
			m_ItemInfo = *(AUCTION::sArticleItemInfo *)(pRepNew->articleData);
			break;
		case AUCTION::ARTICLE_FOLLOWER:
			m_FollowerInfo = *(AUCTION::sArticleFollowerInfo *)(pRepNew->articleData);
			break;
		case AUCTION::ARTICLE_PRODUCTION:
			break;						
		}

		sprintf(m_szTalk,m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_AUCTION_AUCTION_START));
		m_bKeepTalk = TRUE;
		m_siTalk = AUCTIONTALK_NEW;
		m_siTalkDelay = 120;
									
		// ��Ŵ� �ٽ� ���۵ȴ�!!!
		m_bUnderAuction = TRUE;
	
		
	}

}

void OnlineFieldAuction::Enter(BYTE *pMsg)
{
	// ��� ������ ���� ������ ��ٸ���.
	
	AUCTION::sResponseEnter *pEnterMsg = (AUCTION::sResponseEnter *)pMsg;
	if (pEnterMsg)
	{

		m_ArticleInfo.uniqID = 0;
		m_siCurrentBidPrice = 0; 
		switch(pEnterMsg->responseKind)
		{
		// ����� ������ ����
		case AUCTION::NOT_HAS:
			
			m_bUnderAuction = FALSE; // �������� �ƴ�
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_ENTER_SUCCESS));
			Enter();
			m_siTalk = AUCTIONTALK_NO_AUCTION;
			m_siTalkDelay = 0;										
			break;
		// �������
		case AUCTION::OK:
			m_bUnderAuction = TRUE; 
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_ENTER_SUCCESS));
			Enter();
			m_ArticleInfo = pEnterMsg->_c;
								
			m_siCurrentBidPrice = pEnterMsg->biddingPrice;
			strcpy(m_strMaxBidder,pEnterMsg->strBidder);

			switch(m_ArticleInfo.articleKind)
			{
			case AUCTION::ARTICLE_ITEM:
				m_ItemInfo = *(AUCTION::sArticleItemInfo *)(pEnterMsg->articleData);
				m_siTalk = AUCTIONTALK_ITEM;
				m_siTalkDelay = 0;
				break;
			case AUCTION::ARTICLE_FOLLOWER:
				m_FollowerInfo = *(AUCTION::sArticleFollowerInfo *)(pEnterMsg->articleData);
				m_siTalk = AUCTIONTALK_FOLLOWER;
				m_siTalkDelay = 0;
				break;
			case AUCTION::ARTICLE_PRODUCTION:
				m_siTalk = AUCTIONTALK_PROPERTY;
				m_siTalkDelay = 0;							
				break;						
			}
			
			break;
		// ��� ������ �źδ��ߴ�!
		default:
			m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK,"",m_pMyOnlineWorld->pOnlineText->Get( ON_TEXT_AUCTION_ENTER_FAILED));
			break;
		}

		// ������ �޾����Ƿ�.
		m_bSendEnterMessage = FALSE;
	}

}

