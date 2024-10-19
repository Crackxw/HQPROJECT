#include <GSL.h>

#include <OnlineText.h>
#include <Mouse.h>

#include "OnlineWorld.h"
#include "OnlineParty.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineResource.h"
#include "OnlineFieldArea.h"
#include "OnlineTrade.h"
#include "OnlineMsgBox.h"
#include "OnlineTradeBook.h"
#include "OnlineText.h"

extern _InputDevice				IpD;

#define PARTY_CHECK_MSG		1
#define PARTY_MSG			2
#define PARTY_ACTION		3
#define PARTY_WAIT			4
#define PARTY_NO_MSG		5
#define PARTY_DISS			6

/*POINT m_ptCharNamePos[6] = 
{
	{54,101},{54,132},{54,163},
	{54,194},{54,225},{54,256}
};
*/
OnlineParty::OnlineParty()
{
	m_pMyOnlineWorld  = NULL;
	m_pMsgBox		  =	NULL;
	m_stPartyInfo	  = NULL;
}

OnlineParty::~OnlineParty()
{
	Free();
}

void OnlineParty::Init(cltOnlineWorld* pOnlineWorld)
{
	m_pMyOnlineWorld = pOnlineWorld;

	Initialize();

	m_pMsgBox			=	new OnlineMsgBox;
	m_pMsgBox->Init( m_pMyOnlineWorld );

	clGrp.LoadXspr( "Online\\GameSpr\\BuySell_base.Spr", m_BtnBaseSpr);
	clGrp.LoadXspr( "Online\\GameSpr\\BuySell_Updown.Spr",m_BtnSpr);

//	clGrp.LoadXspr( "Online\\GameSpr\\Party_w_base.Spr",m_Party_w_Base);

//	clGrp.LoadXspr( "Online\\GameSpr\\Book\\Book_Button_small.Spr",m_BtnPartyBaseSpr);

//	clGrp.LoadXspr("Online\\GameSpr\\Book\\BOOK_3_3.Spr",m_BookPartySpr);

	//m_siNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siPartyBaseX, m_siPartyBaseY, 
	//	m_siPartyBaseX + m_Party_w_Base.Header.Xsize, m_siPartyBaseY + m_Party_w_Base.Header.Ysize );

	m_BTradebttn.Create(m_siX, m_siY, m_BtnSpr.Header.Xsize, m_BtnSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(2278), BUTTON_PUT_LEFT, TRUE);

	m_BPartybttn.Create( m_siX, m_siY, m_BtnSpr.Header.Xsize, m_BtnSpr.Header.Ysize, 
		m_pMyOnlineWorld->pOnlineText->Get(2279), BUTTON_PUT_LEFT, TRUE);

//	m_BPartyOutBasebttn.Create(m_siPartyBaseX + 90,m_siPartyBaseY + 304,m_BtnPartyBaseSpr.Header.Xsize,
//		m_BtnPartyBaseSpr.Header.Ysize,m_pMyOnlineWorld->pOnlineText->Get(2280),BUTTON_PUT_LEFT, TRUE);

/*	m_BPartyBasebttn.Create(m_siPartyBaseX + 189,m_siPartyBaseY + 79,m_BtnPartyBaseSpr.Header.Xsize,
		m_BtnPartyBaseSpr.Header.Ysize,m_pMyOnlineWorld->pOnlineText->Get(2281),BUTTON_PUT_LEFT, TRUE);
*/

}

void OnlineParty::Initialize()
{
	m_bMouseDown    = FALSE;
	m_bFlag			= FALSE;
	m_bTFlag		= FALSE;

	m_uiPartyMsg	= PARTY_NO_MSG;

	m_siX		=	0;
	m_siY		=	0;
	m_siMyChar	=   0;
	m_siPartyNum =  0;

	m_siPartyBaseX	= 95;
	m_siPartyBaseY	= 45;

	m_siNoField		=	-1;

	m_bCharInfo     = FALSE;
//	m_bBaseCharInfo = FALSE;
}

void OnlineParty::Free()
{
	if( m_pMsgBox )
	{
		delete m_pMsgBox;
		m_pMsgBox = NULL;
	}

	clGrp.FreeXspr(m_BtnBaseSpr );
	clGrp.FreeXspr(m_BtnSpr );
//	clGrp.FreeXspr(m_Party_w_Base);
//	clGrp.FreeXspr(m_BtnPartyBaseSpr);
//	clGrp.FreeXspr(m_BookPartySpr);

}

void OnlineParty::Draw(LPDIRECTDRAWSURFACE7	pSurface)
{
	HDC		hDC;
	SI16	j	=	0;

	if(!m_bFlag)
		return;
	//캐릭터 리스트는 계속 뿌려줘야 한다.

	
	switch(m_uiPartyMsg)
	{
		case PARTY_CHECK_MSG:
		{
			if( clGrp.LockSurface( pSurface ) == TRUE )
			{
				clGrp.PutSpriteT( m_siX, m_siY, m_BtnBaseSpr.Header.Xsize, m_BtnBaseSpr.Header.Ysize, m_BtnBaseSpr.Image );

				m_BTradebttn.Put( &m_BtnSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );
				m_BPartybttn.Put( &m_BtnSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );				

				clGrp.UnlockSurface( pSurface );
			}
			if( pSurface->GetDC( &hDC ) == DD_OK )
			{
				::SetBkMode(hDC, TRANSPARENT);
				SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON2));
				SetTextColor(hDC, m_pMyOnlineWorld->PaletteWhiteColor);

				m_BTradebttn.Put( hDC );
				m_BPartybttn.Put( hDC );
				pSurface->ReleaseDC( hDC );
			}
		}
		break;
		case PARTY_MSG:
			break;
		case PARTY_ACTION:
			break;
		case PARTY_WAIT:
			break;
		case PARTY_NO_MSG:
			break;
		case PARTY_DISS:
			break;
	}
	
	//flag를 둔다.
//	if(m_bCharInfo && m_bBaseCharInfo)
//	{
		if( clGrp.LockSurface( pSurface ) == TRUE )
		{
			//clGrp.PutSpriteT( m_siPartyBaseX, m_siPartyBaseY, m_Party_w_Base.Header.Xsize, m_Party_w_Base.Header.Ysize, m_Party_w_Base.Image );
	
//			clGrp.PutSpriteT(m_siPartyBaseX, m_siPartyBaseY, m_BookPartySpr.Header.Xsize , m_BookPartySpr.Header.Ysize, m_BookPartySpr.Image);
			
//			m_BPartyBasebttn.Put( &m_BtnPartyBaseSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );
//			m_BPartyOutBasebttn.Put( &m_BtnPartyBaseSpr, 0, 1, 1, BUTTON_PUT_NOMOVE );

//			m_pMyOnlineWorld->pOnlineTradeBook->DrawTab(pSurface);

			clGrp.UnlockSurface( pSurface );
		}

		//아이디를 뿌려준다.
		if( pSurface->GetDC( &hDC ) == DD_OK )
		{
			SetBkMode(hDC, TRANSPARENT);
			SelectObject(hDC, m_pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT2));
			/*
			for(int i = 0 ; i < m_siPartyNum;i++)
			{
				if(i !=m_siMyChar)
				{
					if(m_stPartyInfo[i].bIsPartyLeader == TRUE)
					{
						 m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siPartyBaseX + m_ptCharNamePos[j].x ,m_siPartyBaseY + m_ptCharNamePos[j].y + 1,
							130, 15,m_stPartyInfo[i].Name,RGB(255,0, 0));
					}
					else
					{
						 m_pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hDC,m_siPartyBaseX + m_ptCharNamePos[j].x ,m_siPartyBaseY + m_ptCharNamePos[j].y + 1,
							130, 15,m_stPartyInfo[i].Name,RGB(216,201, 183));				
					}
					 j++;
				}
			}
			*/
			
//			m_BPartyBasebttn.Put(hDC);
//			m_BPartyOutBasebttn.Put(hDC);

			pSurface->ReleaseDC( hDC );
		}
//	}
	m_pMsgBox->Draw(pSurface);

	if( IpD.LeftPressSwitch )		m_bMouseDown	=	TRUE;
	else							m_bMouseDown	=	FALSE;
}

void OnlineParty::Action()
{
	SI16 siRet;

	if(!m_bFlag)
		return;

	switch(m_uiPartyMsg)
	{
		case PARTY_CHECK_MSG:
		{	
			if(m_pMyOnlineWorld->pMyData->IsGotoPortal())
			{
				SetPartyMsg(5);
				break;
			}
			if(m_BTradebttn.Process(m_bMouseDown))
			{
				if(m_pMyOnlineWorld->pMyData->GetTradeGrade() <= 5)
				{
					ZeroMemory(&m_pMyOnlineWorld->pOnlineTrade->FriendInfo, sizeof(m_pMyOnlineWorld->pOnlineTrade->FriendInfo));
					m_pMyOnlineWorld->pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", m_pMyOnlineWorld->pOnlineText->Get(8000470));
					SetPartyMsg(5);
					break;
				}
				m_pMyOnlineWorld->pOnlineClient->SendRequestAskTrade(m_uiCharmsg);
				//??에게 거래를 요청중입니다. 메세지를 띄운다.
				m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_CANCEL, m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_WAITTING), m_pMyOnlineWorld->pOnlineText->Get(ON_TEXT_TRADE_WAITTINGDATA), m_szCharName );
				m_pMyOnlineWorld->pOnlineTrade->SetRequest();
				m_uiPartyMsg = PARTY_NO_MSG;
				break;
			}
			if(m_BPartybttn.Process(m_bMouseDown))
			{
				if((strcmp(m_szPartyLeader,m_pMyOnlineWorld->pMyData->GetMyCharName()) == 0) && m_siPartyNum > 6)
				{
					m_pMyOnlineWorld->pOnlineParty->m_pMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "",m_pMyOnlineWorld->pOnlineText->Get(2282));
					m_uiPartyMsg = PARTY_WAIT;
				}
				else
				{
					m_pMyOnlineWorld->pOnlineClient->SendRequestAskParty(m_uiCharmsg);
					m_uiPartyMsg = PARTY_WAIT;
					m_pMyOnlineWorld->bCharMove = FALSE;
				}
				break;
			}
			
		}
		break;
		case PARTY_WAIT:
		{	
			siRet	= m_pMsgBox->Process();
			if(siRet == ON_MSGBOX_RETURN_CANCEL)
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequestPartyCancel();
				m_uiPartyMsg = PARTY_NO_MSG;
				m_pMyOnlineWorld->bCharMove = TRUE;
			}
			if(siRet == ON_MSGBOX_RETURN_OK)
			{
				m_uiPartyMsg = PARTY_NO_MSG;
				m_pMyOnlineWorld->bCharMove = TRUE;
			}
		}
		break;
		case PARTY_MSG:
		{
			siRet	= m_pMsgBox->Process();
	
			//파티 수락 요청을 찬성했을때.
			if(siRet == ON_MSGBOX_RETURN_OK) 
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequestParty(ON_RET_OK);
				m_uiPartyMsg = PARTY_NO_MSG;
			}
			//파티 수락 요청을 거절했을때.
			else if(siRet == ON_MSGBOX_RETURN_CANCEL)
			{
				m_pMyOnlineWorld->pOnlineClient->SendRequestParty(ON_RET_NO);
				m_uiPartyMsg = PARTY_NO_MSG;
			}
		}
		break;

		case PARTY_ACTION:
		{
			siRet  = m_pMsgBox->Process();
			if(siRet == ON_MSGBOX_RETURN_OK) 
			{
				m_uiPartyMsg = PARTY_NO_MSG;
				m_pMyOnlineWorld->bCharMove = TRUE;

				if(m_bTFlag == TRUE)
				{
					m_pMyOnlineWorld->pOnlineTrade->m_pMsgBox->Initialize();
					SetAction(FALSE,PARTY_NO_MSG);
					m_bTFlag = FALSE;
				}
			}				
		}
		break;
		case PARTY_NO_MSG:
			m_pMyOnlineWorld->bCharMove = TRUE;
			break;
		case PARTY_DISS:
		{
			siRet = m_pMsgBox->Process();
			if(siRet == ON_MSGBOX_RETURN_OK) 
			{
				m_bCharInfo = FALSE;
//				m_bBaseCharInfo = FALSE;
				m_pMyOnlineWorld->bCharMove = TRUE;
				SetAction(FALSE,PARTY_NO_MSG);
			}
		}
		break;
	}

	if(m_pMyOnlineWorld->pOnlineTradeBook->GetStatus() == ON_BOOK_PARTY)
	{
		if(m_bCharInfo) // && m_bBaseCharInfo)
		{
			if(m_BPartyBasebttn.Process(m_bMouseDown))   //파티 창 닫기.
			{
	//			m_bBaseCharInfo = FALSE;
	//			if(m_siNoField != -1)
	//				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
	//			m_siNoField = -1;
			}

			if(m_BPartyOutBasebttn.Process(m_bMouseDown)) //파티 나가기.
			{ 
	//			m_bBaseCharInfo = FALSE;
				m_pMyOnlineWorld->pOnlineClient->SendRequestQuitParty();
	//			if(m_siNoField != -1)
	//				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
	//			m_siNoField = -1;
			}
			if(m_pMyOnlineWorld->pOnlineTradeBook->m_BPartyOutBasebttn.Process(m_bMouseDown))
			{
				m_pMyOnlineWorld->pOnlineTradeBook->SetAction(FALSE);
				m_pMyOnlineWorld->pOnlineClient->SendRequestQuitParty();
			}
		}
	}


	if( IpD.LeftPressSwitch )		m_bMouseDown	=	TRUE;
	else							m_bMouseDown	=	FALSE;
}

void OnlineParty::SetAction(BOOL flag,UI16 uiMsg)
{
	m_bFlag			= flag;
	m_uiPartyMsg	= uiMsg;
}

void OnlineParty::SetRePosition()
{
	m_siX = m_siCharX +5;
	m_siY = m_siCharY +5;

	m_BTradebttn.SetX(m_siX + 4);
	m_BTradebttn.SetY(m_siY + 4);

	m_BPartybttn.SetX(m_siX + 4);
	m_BPartybttn.SetY(m_siY + 23);
}


void OnlineParty::SetPartyMode(SI16 x,SI16 y,UI16 msg)
{
	m_siCharX   = x;
	m_siCharY	= y;
	m_uiCharmsg = msg;	
	SetRePosition();
	SetAction(TRUE,PARTY_CHECK_MSG);
}


BOOL OnlineParty::GetPartyAction()
{
	return m_bFlag;
}

BOOL OnlineParty::GetPartyMsg()
{
	return m_uiPartyMsg;
}

VOID OnlineParty::SetCharID(CHAR *pName)
{
	ZeroMemory(m_szCharName, sizeof(m_szCharName));

	if(pName)
	{
		strncpy((char*)m_szCharName, pName, ON_ID_LENGTH);
	}
}

void OnlineParty::SetPartyInfo(OnPartyInfo* pOnPartyInfo,SI16 siNum)
{
	m_siMyChar   = -1;
	m_siPartyNum = siNum;
	if(m_stPartyInfo)
	{
		delete m_stPartyInfo;
		m_stPartyInfo = NULL;
	}

	//ZeroMemory(m_szPartyLeader,sizeof());

	if(pOnPartyInfo != NULL)
	{
		m_stPartyInfo = new OnPartyInfo[siNum];
		memcpy(m_stPartyInfo,pOnPartyInfo,sizeof(OnPartyInfo)*siNum);

		for(int i = 0 ; i < siNum ; i++)
		{
			if(strcmp(m_stPartyInfo[i].Name,m_pMyOnlineWorld->pMyData->GetMyCharName()) == 0)
			{
				m_siMyChar = i;
			}

			if(m_stPartyInfo[i].bIsPartyLeader == TRUE)
			{
				sprintf(m_szPartyLeader,m_stPartyInfo[i].Name);
			}
		}

		if(m_siMyChar == -1)
		{
			m_bTFlag = TRUE;
			m_bCharInfo = FALSE;
	//		m_bBaseCharInfo = FALSE;		
		}
		else
		{
			m_bCharInfo = TRUE;
	//		m_bBaseCharInfo = TRUE;


	//		if(m_siNoField != -1)
	//			m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

	//		m_siNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siPartyBaseX, m_siPartyBaseY, 
	//			m_siPartyBaseX + m_BookPartySpr.Header.Xsize, m_siPartyBaseY + m_BookPartySpr.Header.Ysize );
		}
	}
}

/*void OnlineParty::SetPartyCharInfo(BOOL bInfo)
{
//	m_bBaseCharInfo = bInfo;

	if(m_bCharInfo)
	{
		if(m_bBaseCharInfo)m_bBaseCharInfo
		{
//			if(m_siNoField != -1)
//				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );

//			m_siNoField = m_pMyOnlineWorld->pOnlineFieldArea->SetNoFieldArea( m_siPartyBaseX, m_siPartyBaseY, 
//				m_siPartyBaseX + m_BookPartySpr.Header.Xsize, m_siPartyBaseY + m_BookPartySpr.Header.Ysize );
		}
		else
		{
//			if(m_siNoField != -1)
//				m_pMyOnlineWorld->pOnlineFieldArea->SetFieldArea( m_siNoField );
//			m_siNoField = -1;
		}
	}
}	*/







