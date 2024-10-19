/********************************************************************************************
	파일명	:	OnlineNumberControl.cpp
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2001.07.23
	수정일	:	2001.12.10
	내  용  :	숫자 입력
********************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "OnlineWorld.h"
#include "OnlineNumberControl.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"



#define ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE		1

extern _clGame* pGame;

extern _InputDevice				IpD;


OnlineNumberControl::OnlineNumberControl()
{
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox		=	NULL;
//	m_YetEditHWND         = NULL;
	m_hOec				= NULL;
	m_hPrevOec			= NULL;
	m_bPrevOecEnable   = 0;
	m_bPrevOecShortKey = 0;

	MaxNumber       =   -1;
	MinNumber       =   -1;

	// Market Item Data
	NumberControlInfoType = 0;
	ItemMoney			  = 0;
	ItemName              = 0;
	OKButtonName          = 0;

	NowInstmentMoney = 0;
	MyInstmentMoney  = 0;

	m_siUnit        =	NUMBERCONTROL_UNIT_THING;
}

OnlineNumberControl::~OnlineNumberControl()
{
	Free();
}

/********************************************************************************************
	총 입력 받은 수를 리턴을 받는다.
********************************************************************************************/
__int64	OnlineNumberControl::GetNumber()
{
	return _atoi64(m_pszBuffer);
}

VOID	OnlineNumberControl::SetNumber(__int64 sinumber)
{
	// actdoll (2004/05/04 16:33) : [OECEX]
//	char buffer[10];
// 	_i64toa(sinumber, buffer, 10 );
//	pEditBox->SetString(buffer);
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->SetValue( sinumber );
}

/********************************************************************************************
	출력을 한다.
********************************************************************************************/
VOID	OnlineNumberControl::Draw( LPDIRECTDRAWSURFACE7	lpSurface )
{
	if( bAction == FALSE )			return;	

	SI16	i;
	SI16	siTextSize;
	__int64	TempInt;
	SI16	TableAdd[11] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0};
	SI16	TableMul[11] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 100};
	CHAR	*pTable[11]	 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "00"};
	CHAR	TempBB[512];
	CHAR	TempBB1[512];
	HDC		hdc;
	XSPR*	lpSpr;
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->GetString();

	// actdoll (2004/05/04 16:33) : [OECEX]
//	if(pEditBox)
//	{
//		if(MaxNumber != -1)
//		{
//			for(i = 0; i < 11; i++)
//			{
//				TempInt = GetNumber() * TableMul[i];
//
//				BNumPad[i].SetAction(TRUE);
//				if(i != 10)
//				{
//					pEditBox->SetKeyLock(pTable[i][0], FALSE);
//				}
//
//				if((TempInt + TableAdd[i]) > MaxNumber)
//				{
//					BNumPad[i].SetAction(FALSE);
//					if(i != 10)
//					{
//						pEditBox->SetKeyLock(pTable[i][0], TRUE);
//					}
//				}
//			}
//		}
//	}

	if( m_hOec )
	{
		if(MaxNumber != -1)
		{
			for(i = 0; i < 11; i++)
			{
				TempInt = GetNumber() * TableMul[i];

				BNumPad[i].SetAction(TRUE);
				if(i != 10)
				{
//					pEditBox->SetKeyLock(pTable[i][0], FALSE);
					pOecEx->SetExceptionChar( pTable[i][0], false );
				}

				if((TempInt + TableAdd[i]) > MaxNumber)
				{
					BNumPad[i].SetAction(FALSE);
					if(i != 10)
					{
//						pEditBox->SetKeyLock(pTable[i][0], TRUE);
						pOecEx->SetExceptionChar( pTable[i][0], true );
					}
				}
			}
		}
	}
	if(LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_MULTIPLY)))
	{
		// actdoll (2004/05/04 16:33) : [OECEX]
		pOecEx->SetValue( MaxNumber );
//		CHAR	buffer[10];
//		_i64toa(MaxNumber, buffer, 10);
//		pEditBox->SetString(buffer);
	}


	KeyMessage();

	// Max Count 를 검사
	__int64	TempNumber = GetNumber();
	UI08	TempB11[512];

	// actdoll (2004/05/04 16:33) : [OECEX]
//	if(pEditBox)
//	{
//		if(MaxNumber != -1)
//		{
//			if(TempNumber > MaxNumber)
//			{
//				TempNumber = MaxNumber;
//				sprintf((char*)TempB11, "%I64d", MaxNumber);
//				pEditBox->SetString((char*)TempB11);
//				pEditBox->GetString(m_pszBuffer);
//			}
//		}
//	}
	if( m_hOec )
	{
		if(MaxNumber != -1)
		{
			if(TempNumber > MaxNumber)
			{
				TempNumber = MaxNumber;
				pOecEx->SetValue( MaxNumber );
				strcpy( m_pszBuffer, pOecEx->GetString() );
			}
		}
	}

	TempB11[0] = NULL;
	if(TempNumber) sprintf((char*)TempB11, "%I64d", GetNumber());
	if(strcmp(m_pszBuffer, (char*)TempB11) != 0)
	{
		// actdoll (2004/05/04 16:33) : [OECEX]
//		pEditBox->SetString((char*)TempB11);
//		pEditBox->GetString(m_pszBuffer);
		pOecEx->SetString( (char*)TempB11 );
		strcpy( m_pszBuffer, pOecEx->GetString() );
	}

	pMyOnlineWorld->pIOnlineMouse->ShowMouse(TRUE);	

	//==================================================================================================
	// 출력
	if(clGrp.LockSurface(lpSurface) == TRUE)
	{
		clGrp.PutSpriteT(siX, siY, pBorderSpr->Header.Xsize, pBorderSpr->Header.Ysize, pBorderSpr->Image);

		// 버튼 출력
		for(i = 0; i < 10; i++) BNumPad[i].Put(pButton1, 0, 2, 1, BUTTON_PUT_NOMOVE);
		BNumPad[10].Put(pButton2, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		BOk.Put(pButton4,     0, 2, 1, BUTTON_PUT_NOMOVE);
		BCancel.Put(pButton3, 0, 2, 1, BUTTON_PUT_NOMOVE);
		BAll.Put(pButton3,    0, 2, 1, BUTTON_PUT_NOMOVE);

		// 스크롤바 출력
		lpSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_GAGEBAR);
		clGrp.SetClipArea(siX + 13, siY, siX + 151, siY + pBorderSpr->Header.Ysize);
		clGrp.PutSpriteT(siX + 13 - lpSpr->Header.Xsize + (SI32)((long double)GetNumber() / (long double)MaxNumber * lpSpr->Header.Xsize), siY + 28, lpSpr->Header.Xsize, lpSpr->Header.Ysize, lpSpr->Image);
		clGrp.SetClipArea(0, 0, 799, 599);

		clGrp.UnlockSurface(lpSurface);
	}
	
	if(lpSurface->GetDC(&hdc) == DD_OK)
	{
		SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_BUTTON));
		SetBkMode(hdc, TRANSPARENT );
		
		SetTextColor(hdc, RGB(210, 210, 210));

		// 버튼 출력
		BOk.Put(hdc);
		BCancel.Put(hdc);
		BAll.Put(hdc);
		for(i = 0; i < 11; i++) BNumPad[i].Put(hdc);


		switch(m_siUnit)
		{
		case NUMBERCONTROL_UNIT_MONEY:
		// 숫자를 출력
			if(strlen(m_pszBuffer) == 0)
			{
				sprintf((char*)TempBB, "0 %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
			}
			else
			{
				MakeMoneyText(m_pszBuffer, (char*)TempBB1);
				sprintf((char*)TempBB, "%s %s", TempBB1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_WON));
			}
			
			break;
		// robypark 2004/11/18 16:47
		// 공성전 부대수 (예 : 부대) 단위 표시하기
		case NUMBERCONTROL_UNIT_ARMY:	// 2
		// 숫자를 출력
			if(strlen(m_pszBuffer) == 0)
			{
				sprintf((char*)TempBB, "0 %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT_ARMY));
			}
			else
			{
				MakeMoneyText(m_pszBuffer, (char*)TempBB1);
				sprintf((char*)TempBB, "%s %s", TempBB1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT_ARMY));
			}
			break;

		case NUMBERCONTROL_UNIT_THING:
		default:

			// 숫자를 출력
			if(strlen(m_pszBuffer) == 0)
			{
				sprintf((char*)TempBB, "0 %s", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT));
			}
			else
			{
				MakeMoneyText(m_pszBuffer, (char*)TempBB1);
				sprintf((char*)TempBB, "%s %s", TempBB1, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNIT));
			}
			break;
		}

		siTextSize = strlen(TempBB);
		TextOut(hdc, siX + 150 - (siTextSize * 6), siY + 46, TempBB, siTextSize);

		// 스크롤바 수치 출력
		sprintf((char*)TempBB, "%3d %%", (SI32)((long double)GetNumber() / (long double)MaxNumber * 100.0));
		siTextSize = strlen(TempBB);
		TextOut(hdc, siX + 150 - (siTextSize * 6), siY + 15, TempBB, siTextSize);

		lpSurface->ReleaseDC(hdc);
	}
}

/********************************************************************************************
	키 입력을 받는다.
********************************************************************************************/
VOID	OnlineNumberControl::KeyMessage()
{
	SI16	i;
	POINT	ptMouse;
	SI32	TempXPos;
	__int64	TempMoney;
	OnlineEditControlEx	*pOecEx;

//	pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->EnableWindow(FALSE);
	pMyOnlineWorld->pIOnlineMouse->GetPos(&ptMouse);

	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox->SetFocus();
//	pEditBox->GetString(m_pszBuffer);
//	Handler_OnlineEditControlEx::SetFocus( pGame->Hwnd, m_hOec );
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	strcpy( m_pszBuffer, pOecEx->GetString() );

	if((BAll.Process(bMouseDown) == TRUE))
	{
		sprintf((char*)m_pszBuffer, "%I64d", MaxNumber);
//		pEditBox->SetString((char*)m_pszBuffer);
		pOecEx->SetValue( MaxNumber );
	}
	if((BOk.Process(bMouseDown) == TRUE) || pOecEx->IsPushEnter()) //|| LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
	{
		bAction		=	FALSE;
		bSuccess	=	TRUE;
		*pNumber	=	GetNumber();

//		pEditBox->EnableWindow(TRUE);
//		pEditBox->SetString("");
		pOecEx->Clear();

		// Focus 지정
		pMyOnlineWorld->SetShortKey(m_bPrevOecShortKey);
//		if( m_YetEditHWND )	SetFocus(m_YetEditHWND);
//		if( m_YetEditHWND )	EnableWindow(m_YetEditHWND, m_bPrevOecEnable);
		Handler_OnlineEditControlEx::ClearCurrentFocus();
		if( m_hPrevOec )	
		{
			Handler_OnlineEditControlEx::SetFocus( m_hPrevOec );
		}

		ZeroMemory(m_pszBuffer, 512);
	}
	if((BCancel.Process(bMouseDown) == TRUE))
	{
		bAction		=	FALSE;
		bSuccess	=	TRUE;
		*pNumber	=	0;

//		pEditBox->EnableWindow(TRUE);
//		pEditBox->SetString("");
		pOecEx->Clear();

		// Focus 지정
		pMyOnlineWorld->SetShortKey(m_bPrevOecShortKey);
//		SetFocus(m_YetEditHWND);
//		EnableWindow(m_YetEditHWND, m_bPrevOecEnable);
		Handler_OnlineEditControlEx::ClearCurrentFocus();
		if( m_hPrevOec )
		{
			Handler_OnlineEditControlEx::SetFocus( m_hPrevOec );
		}
		ZeroMemory(m_pszBuffer, 512);
	}

	for(i = 0; i < 11; i++)
	{
		// actdoll (2004/05/04 16:33) : [OECEX]
		if(BNumPad[i].Process(bMouseDown) == TRUE)
		{
//			if(strcmp(BNumPad[i].GetMenuName(), "0") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '0', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "1") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '1', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "2") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '2', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "3") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '3', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "4") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '4', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "5") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '5', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "6") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '6', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "7") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '7', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "8") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '8', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "9") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '9', 0);
//			}
//			else if(strcmp(BNumPad[i].GetMenuName(), "00") == 0)
//			{
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '0', 0);
//				SendMessage(pEditBox->GethWnd(), WM_CHAR, '0', 0);
//			}
			if(strcmp(BNumPad[i].GetMenuName(), "0") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '0', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "1") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '1', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "2") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '2', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "3") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '3', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "4") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '4', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "5") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '5', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "6") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '6', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "7") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '7', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "8") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '8', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "9") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '9', 0);
			}
			else if(strcmp(BNumPad[i].GetMenuName(), "00") == 0)
			{
				SendMessage( pGame->Hwnd, WM_CHAR, '0', 0);
				SendMessage( pGame->Hwnd, WM_CHAR, '0', 0);
			}

		}
	}

	// 스크롤 바 수정시
	if(IpD.LeftPressSwitch)
	{
		if((ptMouse.x >= (siX + 6)) && (ptMouse.x <= (siX + 176)) && (ptMouse.y >= (siY + 28)) && (ptMouse.y <= (siY + 40)))
		{
			TempXPos  = ptMouse.x - (siX + 16);
			if(TempXPos < 0)   TempXPos = 0;
			if(TempXPos > 150) TempXPos = 150;
			TempMoney = (__int64)((long double)MaxNumber * (long double)((float)TempXPos / 150.0f));

			sprintf((char*)m_pszBuffer, "%I64d", TempMoney);
			// actdoll (2004/05/04 16:33) : [OECEX]
//			pEditBox->SetString((char*)m_pszBuffer);
			pOecEx->SetValue( TempMoney );
		}
	}

	if(IpD.LeftPressSwitch) bMouseDown = TRUE;
	else				    bMouseDown = FALSE;
}
/********************************************************************************************
	키 입력을 받는다.
********************************************************************************************/
VOID	OnlineNumberControl::Init( __int64 *pCount, cltOnlineWorld *pOnlineWorld, __int64 MaxNumberCount)
{
	SI16	ix, iy;
	SI16	siCnt;
	CHAR	*pTable[11]	= {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "00"};
	OnlineEditControlEx	*pOecEx;

	pMyOnlineWorld	=	pOnlineWorld;
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	pNumber			=	pCount;

	pBorderSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER);
	pButton1   = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBERBUTTON);
	pButton2   = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_BUTTON1);
	pButton3   = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_BUTTON2);
	pButton4   = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_NUMBER_BUTTON3);

	siX				=	0;
	siY				=	0;
	siCnt			=	0;

	//===========================================================================================
	// 에디트 박스 생성
	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox		=	new OnlineEditControl;
//	pEditBox->CreateEditBox(1000, 1000, 150, 20, pGame->Hwnd, pGame->hInst, FALSE, FALSE, 19, TRUE );
//	pEditBox->EnableWindow(FALSE);
//	pEditBox->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
//	m_YetEditHWND         = NULL;

	RECT	rcRect = { 0, 0, 150, 20 };
	m_hOec	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYNUMERIC, &rcRect, 19, 100000000 );
	pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	m_hPrevOec			= NULL;
	m_bPrevOecEnable   = 0;
	m_bPrevOecShortKey = 0;

	int siButtonTopX = 13;
	int siButtonTopY = 67;

	int siButtonIntervalX = 32;
	int siButtonIntervalY = 32;


	MaxNumber       =   MaxNumberCount;
	//===========================================================================================

	//===========================================================================================
	// 이미지 버튼 생성
	BOk.Create(siX + siButtonTopX + siButtonIntervalX * 3,     siY + siButtonTopY + siButtonIntervalY * 2, pButton4->Header.Xsize, pButton4->Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NUMBER_OK), BUTTON_PUT_LEFT, TRUE);
	BCancel.Create(siX + siButtonTopX + siButtonIntervalX * 3, siY + siButtonTopY + siButtonIntervalY * 1, pButton3->Header.Xsize, pButton3->Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_QUESTINFO_CANCEL), BUTTON_PUT_LEFT, TRUE);
	BAll.Create(siX + siButtonTopX + siButtonIntervalX * 3,    siY + siButtonTopY,  pButton3->Header.Xsize, pButton3->Header.Ysize, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_NUMBER_ALL), BUTTON_PUT_LEFT, TRUE);

	// 나머지 숫자 버튼 9개를 생성을 한다.
	siCnt = 0;
	for(iy = 0; iy < 3; iy++)
	{
		for(ix = 0; ix < 3; ix++)
		{	
			BNumPad[siCnt].Create(siX + siButtonTopX + (ix * siButtonIntervalX), siY + siButtonTopY + (iy * siButtonIntervalY), pButton1->Header.Xsize, pButton1->Header.Ysize, pTable[siCnt], BUTTON_PUT_LEFT, TRUE);
			siCnt++;			
		}
	}

	BNumPad[9].Create(siX + siButtonTopX,  siY + siButtonTopY + siButtonIntervalY * 3, pButton1->Header.Xsize, pButton1->Header.Ysize, pTable[9],  BUTTON_PUT_LEFT, TRUE);
	BNumPad[10].Create(siX + siButtonTopX + siButtonIntervalX, siY + siButtonTopY + siButtonIntervalY * 3, pButton2->Header.Xsize, pButton2->Header.Ysize, pTable[10], BUTTON_PUT_LEFT, TRUE);
	//===========================================================================================

	ZeroMemory(m_pszBuffer, 512);
}

/********************************************************************************************
	보이는가?
********************************************************************************************/
BOOL	OnlineNumberControl::IsShow()
{
	return bAction;
}
/********************************************************************************************
	성공여부
********************************************************************************************/
BOOL	OnlineNumberControl::IsSuccess()
{
	return bSuccess;
}

/********************************************************************************************
	성공여부
********************************************************************************************/
VOID	OnlineNumberControl::SetShow(SI16 x, SI16 y)
{
	SI16	ix, iy;
	SI16	siCnt;

	if(bAction == TRUE)	return;

	if(x < 0)			x = 0;
	if(y < 0)			y = 0;

	if((y + pBorderSpr->Header.Ysize) > ON_SCREEN_YSIZE) y = ON_SCREEN_YSIZE - pBorderSpr->Header.Ysize - 1;
	if((x + pBorderSpr->Header.Xsize) > ON_SCREEN_XSIZE) x = ON_SCREEN_XSIZE - pBorderSpr->Header.Xsize - 1;	
	

	siX		=	x;
	siY		=	y;
	siCnt	=	0;

	bAction	=	TRUE;


	// actdoll (2004/05/04 16:33) : [OECEX]
	// Focus 지정
//	m_bPrevOecShortKey = pMyOnlineWorld->GetShortKey();
//	m_YetEditHWND         = GetFocus();
//	if( m_YetEditHWND ) m_bPrevOecEnable = IsWindowEnabled(m_YetEditHWND);
//	pEditBox->EnableWindow(TRUE);
//	pEditBox->SetFocus();
	m_bPrevOecShortKey	= pMyOnlineWorld->GetShortKey();
	m_hPrevOec			= Handler_OnlineEditControlEx::GetFocus();
	Handler_OnlineEditControlEx::ClearCurrentFocus();
	Handler_OnlineEditControlEx::SetFocus( m_hOec );
	pMyOnlineWorld->SetShortKey(TRUE);

	int siButtonTopX = 13;
	int siButtonTopY = 67;

	int siButtonIntervalX = 32;
	int siButtonIntervalY = 32;

	BOk.SetX(siX + siButtonTopX + siButtonIntervalX * 3);
	BOk.SetY(siY + siButtonTopY + siButtonIntervalY * 2);
	BCancel.SetX(siX + siButtonTopX + siButtonIntervalX * 3);
	BCancel.SetY(siY + siButtonTopY + siButtonIntervalY * 1);
	BAll.SetX(siX + siButtonTopX + siButtonIntervalX * 3);
	BAll.SetY(siY + siButtonTopY);

	siCnt = 0;
	for(iy = 0; iy < 3; iy++)
	{
		for(ix = 0; ix < 3; ix++)
		{	
			BNumPad[siCnt].SetX(siX + siButtonTopX + (ix * siButtonIntervalX));
			BNumPad[siCnt].SetY(siY + siButtonTopY + (iy * siButtonIntervalY));
			siCnt++;			
		}
	}

	BNumPad[9].SetX(siX + siButtonTopX);
	BNumPad[9].SetY(siY + siButtonTopY + siButtonIntervalY * 3);
	BNumPad[10].SetX(siX + siButtonTopX + siButtonIntervalX);
	BNumPad[10].SetY(siY + siButtonTopY + siButtonIntervalY * 3);
}

VOID	OnlineNumberControl::SetShowCenter(void)
{
	SetShow((ON_SCREEN_XSIZE - pBorderSpr->Header.Xsize) / 2, ((ON_SCREEN_YSIZE - pBorderSpr->Header.Ysize) / 2) - 50);
}

/********************************************************************************************
	초기화
********************************************************************************************/
VOID	OnlineNumberControl::Initialize(__int64 MaxNumberCount)
{
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	*pNumber		=	0;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox->SetString("");
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->Clear();
	pOecEx->SetMaxValue( MaxNumberCount );

	MaxNumber       =   MaxNumberCount;
	MinNumber       =   0;

	NumberControlInfoType = NUMBERCONTROL_INFOTYPE_NORMAL;
}

VOID	OnlineNumberControl::InitializeMarket(SI32 ItemMoney, SI32 ItemName, SI32 OKButtonName, __int64 MaxNumberCount)
{
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	*pNumber		=	0;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox->SetString("");
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->Clear();
	pOecEx->SetMaxValue( MaxNumberCount );

	MaxNumber       =   MaxNumberCount;
	MinNumber       =   0;

	NumberControlInfoType = NUMBERCONTROL_INFOTYPE_MARKET;
	ItemMoney             = ItemMoney;
	ItemName              = ItemName;
	OKButtonName          = OKButtonName;
}

VOID	OnlineNumberControl::InitializeBank(__int64 MaxNumberCount)
{
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	*pNumber		=	0;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox->SetString("");
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->Clear();
	pOecEx->SetMaxValue( MaxNumberCount );


	MaxNumber       =   MaxNumberCount;
	MinNumber       =   0;

	NumberControlInfoType = NUMBERCONTROL_INFOTYPE_BANK;
}

VOID	OnlineNumberControl::InitializeGovernmentInvestment(__int64 MaxNumberCount, __int64 MinNumberCount, __int64 NowInvestmentMoney, __int64 MyInvestmentMoney)
{
	bAction			=	FALSE;
	bSuccess		=	FALSE;
	*pNumber		=	0;

	// actdoll (2004/05/04 16:33) : [OECEX]
//	pEditBox->SetString("");
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );
	pOecEx->Clear();
	pOecEx->SetMaxValue( MaxNumberCount );

	MaxNumber       =   MaxNumberCount;
	MinNumber       =   MinNumberCount;

	NowInstmentMoney = NowInvestmentMoney;
	MyInstmentMoney  = MyInvestmentMoney;

	NumberControlInfoType = NUMBERCONTROL_INFOTYPE_GOVERNMENTINVEST;
}

/********************************************************************************************
	날림
********************************************************************************************/
VOID	OnlineNumberControl::Free()
{
	// actdoll (2004/05/04 16:33) : [OECEX]
//	if(pEditBox)
//	{
//		pEditBox->Free();
//		delete pEditBox;
//		pEditBox		=	NULL;	
//	}
	Handler_OnlineEditControlEx::KillControl( m_hOec );

//	m_YetEditHWND         = NULL;
	m_hPrevOec			= NULL;
	m_bPrevOecEnable   = 0;
	m_bPrevOecShortKey = 0;

	MaxNumber       =   -1;
	MinNumber       =   -1;

	// Market Item Data
	NumberControlInfoType = 0;
	ItemMoney			  = 0;
	ItemName              = 0;
	OKButtonName          = 0;
}

VOID	OnlineNumberControl::SetUnit(SI32 siUnit)
{
	m_siUnit = siUnit;

}

// robypark 2004/12/31 15:25
// 숫자판 컨트롤을 닫는다. (취소버튼 클릭과 동일한 처리)
VOID	OnlineNumberControl::CloseNumberControl(VOID)
{
	OnlineEditControlEx	*pOecEx;
	pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOec );

	bAction		=	FALSE;
	bSuccess	=	TRUE;
	*pNumber	=	0;

//		pEditBox->EnableWindow(TRUE);
//		pEditBox->SetString("");
	if (pOecEx)
		pOecEx->Clear();

	// Focus 지정
	pMyOnlineWorld->SetShortKey(m_bPrevOecShortKey);
//		SetFocus(m_YetEditHWND);
//		EnableWindow(m_YetEditHWND, m_bPrevOecEnable);
	Handler_OnlineEditControlEx::ClearCurrentFocus();
	if( m_hPrevOec )
	{
		Handler_OnlineEditControlEx::SetFocus( m_hPrevOec );
	}
	ZeroMemory(m_pszBuffer, 512);
}
