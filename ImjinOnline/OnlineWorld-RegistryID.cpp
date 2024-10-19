#include <Main.h>
#include <Mouse.h>

#include "..\Gersang\directx.h"

#include <OnlineWorld.h>
#include <Debugtool.h>

#define	ON_REGISTRATION_INITREGISTRATION	1
#define	ON_REGISTRATION_REGISTRATION		2
#define	ON_REGISTRATION_SENDREGISTRATION	3
#define	ON_REGISTRATION_VERIFYREGISTRATION	4

extern	HWND					Hwnd;
extern	HINSTANCE				hInst;
extern	unsigned char			OriginalGamePalette[];
extern _InputDevice				IpD;

extern	LPDIRECTDRAWSURFACE7	lpBriefingBuffer;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이디를 등록할때 호출하는 초기화 함수.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitRegistryIDPoll()
{
//	UI08	uiPalette[768];	

	// 로그인에서 필요한 데이타를 할당 받는다.
	pOnlineRegistryIDData	=	new OnlineRegistryIDData;

	// 버튼을 생성한다.
	pOnlineRegistryIDData->BIDRegistration.Create(200, 400, 100, 30, "ID 등록", BUTTON_PUT_CENTER, TRUE);
	pOnlineRegistryIDData->BExit.Create(350, 400, 100, 30, " 취 소 ", BUTTON_PUT_CENTER, TRUE);


	// 팔레트를 불러온다.
//	clGrp.LoadOnlyPaletteFile("Online\\Palette\\BattleNet.pal", uiPalette);
//	SetGamePalette(uiPalette);	 

	// 그림을 불러온다.(LoadXspr)
//	if(clGrp.LoadXspr("Online\\Login\\Back.spr", pOnlineRegistryIDData->BackImage) == FALSE) clGrp.Error("등록용 그림 로드 실패", "");

	// ID / PW 등록용 에디트 컨트롤 생성.
	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl			=	new OnlineEditControl;
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl	=	new OnlineEditControl;

	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->Create(200, 200, 200, 20, Hwnd, hInst, TRUE, FALSE);
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->Create(200, 300, 200, 20, Hwnd, hInst, TRUE, TRUE);

	// TAB을 눌렀을때 다음 칸으로 이동
	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetNextWindow(pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->GethWnd()); 
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->SetNextWindow(pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->GethWnd());

//	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetString("확인ID");
//	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->SetString("PW");

	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetFocus();	

	pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이디를 등록할때 호출하는 함수.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	cltOnlineWorld::RegistryIDPoll()
{
	HDC		hdc;
	RECT	rcRect;
	UI16	uiNextMenu	=	0;	
	CHAR	szIDRegistration[1024];
	CHAR	szPWRegistration[1024];
	UI16	uiDataProcRet;
//	UI08	*pcBuffer;
//	UI08	*pIP;

	// 수신 버퍼에 있는 데이타를 처리한다.
	uiDataProcRet	=	pOnlineClient->DataProc();

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	switch(pOnlineRegistryIDData->siRegistryIDStatus)
	{
	case ON_REGISTRATION_INITREGISTRATION:
		// ID와 패스워드를 등록하는 에디트 컨트롤을 Disable 시킨다.
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->EnableWindow(TRUE);
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->EnableWindow(TRUE);

		pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_REGISTRATION;
		break;
	case ON_REGISTRATION_REGISTRATION:
		// 등록하기 버튼을 누르면 ID & P.W를 등록한다.
		if(pOnlineRegistryIDData->BIDRegistration.Process(pOnlineRegistryIDData->bMousePush) == TRUE)
		{
			pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_SENDREGISTRATION;

			// ID와 패스워드를 등록하는 에디트 컨트롤을 Disable 시킨다.
			pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->EnableWindow(FALSE);
			pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->EnableWindow(FALSE);			
		}

		// 등록 취소 버튼을 누르면 전 메뉴로 돌아간다.
		if(pOnlineRegistryIDData->BExit.Process(pOnlineRegistryIDData->bMousePush) == TRUE)
		{
			uiNextMenu		= ON_MAINPOLL_INITLOGIN;
		}
		break;
	case ON_REGISTRATION_SENDREGISTRATION:		
		// ID 와 P.W를 에디트 박스에서 얻어온다.
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->GetString(szIDRegistration, 1023);
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->GetString(szPWRegistration, 1023);
			
		pOnlineClient->SendEntry(szIDRegistration, szPWRegistration);
		pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_VERIFYREGISTRATION;		
		break;
	case ON_REGISTRATION_VERIFYREGISTRATION:
		if(uiDataProcRet != ON_CMDPROC_NONE)
		{
			switch(uiDataProcRet)
			{
			case ON_CMDPROC_ENTRYOK:
				clGrp.Error("", "성공 적으로 ID 가입 완료"); 											
				uiNextMenu	=	ON_MAINPOLL_INITLOGIN;
				break;
			case ON_CMDPROC_EXISTID:
				clGrp.Error("존재하는 ID", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			case ON_CMDPROC_INVALIDID:
				clGrp.Error("유효하지 않은 ID", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			case ON_CMDPROC_CANTENTRY:
				clGrp.Error("가입 실패", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			}
		}		
		break;
	}
	

	// 그림을 찍는다.
	if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
	{
		//clGrp.PutImage32(0, 0, pOnlineRegistryIDData->BackImage.Header.Xsize, pOnlineRegistryIDData->BackImage.Header.Ysize,
		//					&pOnlineRegistryIDData->BackImage.Image[pOnlineRegistryIDData->BackImage.Header.Start[0]]);

		// 여기서 에디트 박스의 영역을 살려준다.
		rcRect	=	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->GetRect();
		clGrp.FillBox(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 254);

		rcRect	=	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->GetRect();
		clGrp.FillBox(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 254);

		clGrp.UnlockSurface(lpBriefingBuffer);
	}


	// 버튼을 찍는다.
	pOnlineRegistryIDData->BIDRegistration.Put(lpBriefingBuffer);
	pOnlineRegistryIDData->BExit.Put(lpBriefingBuffer);

	// 글자를 찍는다.
	if(lpBriefingBuffer->GetDC(&hdc) == DD_OK)
	{
		// 투명 모드 설정.
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		lpBriefingBuffer->ReleaseDC(hdc);
	}

	UpdateScreen(lpBriefingBuffer, TRUE);

	//  마우스의 버튼 확인 
	if(IpD.LeftPressSwitch)	pOnlineRegistryIDData->bMousePush	=	TRUE;	
	else pOnlineRegistryIDData->bMousePush	=	FALSE;

	if(uiNextMenu != 0)
	{
		FreeRegistryIDPoll();
	}

	return	uiNextMenu;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeRegistryIDPoll()
{
	// 프라이머리 화면을 검정색으로 칠해준다.
	if(clGrp.LockSurface(SURFACE_PRIMARY) == TRUE)
	{
		clGrp.FillScreen(0);
		clGrp.UnlockSurface(SURFACE_PRIMARY);
	}

	// 원래 팔레트로 되돌려준다.
	SetGamePalette(OriginalGamePalette);

	// 에디트 컨트롤을 없앤다.(ID 등록용)
	if(pOnlineRegistryIDData->pOnlineIDRegistrationEditControl)
	{
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->Free();
		
		delete pOnlineRegistryIDData->pOnlineIDRegistrationEditControl;
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl = NULL;
	}
	   
	// 에디트 컨트롤을 없앤다.(PW 등록용)
	if(pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl)
	{
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->Free();

		delete pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl;
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl = NULL;
	}

	// ID등록시 필요한 것을을 없앤다.
	if(pOnlineRegistryIDData)
	{
		delete pOnlineRegistryIDData;
		pOnlineRegistryIDData = NULL;
	}
}
