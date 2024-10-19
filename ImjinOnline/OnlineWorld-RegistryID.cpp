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
//	����	:	���̵� ����Ҷ� ȣ���ϴ� �ʱ�ȭ �Լ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitRegistryIDPoll()
{
//	UI08	uiPalette[768];	

	// �α��ο��� �ʿ��� ����Ÿ�� �Ҵ� �޴´�.
	pOnlineRegistryIDData	=	new OnlineRegistryIDData;

	// ��ư�� �����Ѵ�.
	pOnlineRegistryIDData->BIDRegistration.Create(200, 400, 100, 30, "ID ���", BUTTON_PUT_CENTER, TRUE);
	pOnlineRegistryIDData->BExit.Create(350, 400, 100, 30, " �� �� ", BUTTON_PUT_CENTER, TRUE);


	// �ȷ�Ʈ�� �ҷ��´�.
//	clGrp.LoadOnlyPaletteFile("Online\\Palette\\BattleNet.pal", uiPalette);
//	SetGamePalette(uiPalette);	 

	// �׸��� �ҷ��´�.(LoadXspr)
//	if(clGrp.LoadXspr("Online\\Login\\Back.spr", pOnlineRegistryIDData->BackImage) == FALSE) clGrp.Error("��Ͽ� �׸� �ε� ����", "");

	// ID / PW ��Ͽ� ����Ʈ ��Ʈ�� ����.
	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl			=	new OnlineEditControl;
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl	=	new OnlineEditControl;

	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->Create(200, 200, 200, 20, Hwnd, hInst, TRUE, FALSE);
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->Create(200, 300, 200, 20, Hwnd, hInst, TRUE, TRUE);

	// TAB�� �������� ���� ĭ���� �̵�
	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetNextWindow(pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->GethWnd()); 
	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->SetNextWindow(pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->GethWnd());

//	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetString("Ȯ��ID");
//	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->SetString("PW");

	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->SetFocus();	

	pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���̵� ����Ҷ� ȣ���ϴ� �Լ�.
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

	// ���� ���ۿ� �ִ� ����Ÿ�� ó���Ѵ�.
	uiDataProcRet	=	pOnlineClient->DataProc();

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	switch(pOnlineRegistryIDData->siRegistryIDStatus)
	{
	case ON_REGISTRATION_INITREGISTRATION:
		// ID�� �н����带 ����ϴ� ����Ʈ ��Ʈ���� Disable ��Ų��.
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->EnableWindow(TRUE);
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->EnableWindow(TRUE);

		pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_REGISTRATION;
		break;
	case ON_REGISTRATION_REGISTRATION:
		// ����ϱ� ��ư�� ������ ID & P.W�� ����Ѵ�.
		if(pOnlineRegistryIDData->BIDRegistration.Process(pOnlineRegistryIDData->bMousePush) == TRUE)
		{
			pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_SENDREGISTRATION;

			// ID�� �н����带 ����ϴ� ����Ʈ ��Ʈ���� Disable ��Ų��.
			pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->EnableWindow(FALSE);
			pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->EnableWindow(FALSE);			
		}

		// ��� ��� ��ư�� ������ �� �޴��� ���ư���.
		if(pOnlineRegistryIDData->BExit.Process(pOnlineRegistryIDData->bMousePush) == TRUE)
		{
			uiNextMenu		= ON_MAINPOLL_INITLOGIN;
		}
		break;
	case ON_REGISTRATION_SENDREGISTRATION:		
		// ID �� P.W�� ����Ʈ �ڽ����� ���´�.
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
				clGrp.Error("", "���� ������ ID ���� �Ϸ�"); 											
				uiNextMenu	=	ON_MAINPOLL_INITLOGIN;
				break;
			case ON_CMDPROC_EXISTID:
				clGrp.Error("�����ϴ� ID", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			case ON_CMDPROC_INVALIDID:
				clGrp.Error("��ȿ���� ���� ID", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			case ON_CMDPROC_CANTENTRY:
				clGrp.Error("���� ����", ""); 
				pOnlineRegistryIDData->siRegistryIDStatus	=	ON_REGISTRATION_INITREGISTRATION;
				break;
			}
		}		
		break;
	}
	

	// �׸��� ��´�.
	if(clGrp.LockSurface(lpBriefingBuffer) == TRUE)
	{
		//clGrp.PutImage32(0, 0, pOnlineRegistryIDData->BackImage.Header.Xsize, pOnlineRegistryIDData->BackImage.Header.Ysize,
		//					&pOnlineRegistryIDData->BackImage.Image[pOnlineRegistryIDData->BackImage.Header.Start[0]]);

		// ���⼭ ����Ʈ �ڽ��� ������ ����ش�.
		rcRect	=	pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->GetRect();
		clGrp.FillBox(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 254);

		rcRect	=	pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->GetRect();
		clGrp.FillBox(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, 254);

		clGrp.UnlockSurface(lpBriefingBuffer);
	}


	// ��ư�� ��´�.
	pOnlineRegistryIDData->BIDRegistration.Put(lpBriefingBuffer);
	pOnlineRegistryIDData->BExit.Put(lpBriefingBuffer);

	// ���ڸ� ��´�.
	if(lpBriefingBuffer->GetDC(&hdc) == DD_OK)
	{
		// ���� ��� ����.
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		lpBriefingBuffer->ReleaseDC(hdc);
	}

	UpdateScreen(lpBriefingBuffer, TRUE);

	//  ���콺�� ��ư Ȯ�� 
	if(IpD.LeftPressSwitch)	pOnlineRegistryIDData->bMousePush	=	TRUE;	
	else pOnlineRegistryIDData->bMousePush	=	FALSE;

	if(uiNextMenu != 0)
	{
		FreeRegistryIDPoll();
	}

	return	uiNextMenu;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	����
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeRegistryIDPoll()
{
	// �����̸Ӹ� ȭ���� ���������� ĥ���ش�.
	if(clGrp.LockSurface(SURFACE_PRIMARY) == TRUE)
	{
		clGrp.FillScreen(0);
		clGrp.UnlockSurface(SURFACE_PRIMARY);
	}

	// ���� �ȷ�Ʈ�� �ǵ����ش�.
	SetGamePalette(OriginalGamePalette);

	// ����Ʈ ��Ʈ���� ���ش�.(ID ��Ͽ�)
	if(pOnlineRegistryIDData->pOnlineIDRegistrationEditControl)
	{
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl->Free();
		
		delete pOnlineRegistryIDData->pOnlineIDRegistrationEditControl;
		pOnlineRegistryIDData->pOnlineIDRegistrationEditControl = NULL;
	}
	   
	// ����Ʈ ��Ʈ���� ���ش�.(PW ��Ͽ�)
	if(pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl)
	{
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl->Free();

		delete pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl;
		pOnlineRegistryIDData->pOnlinePassWordRegistrationEditControl = NULL;
	}

	// ID��Ͻ� �ʿ��� ������ ���ش�.
	if(pOnlineRegistryIDData)
	{
		delete pOnlineRegistryIDData;
		pOnlineRegistryIDData = NULL;
	}
}
