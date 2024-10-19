/********************************************************************************************************
	���ϸ�	:	OnlineWorld-LoginMgr.cpp
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2003.01.20
********************************************************************************************************/

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>
#include "OnlineWorld.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineText.h"
#include "OnlineMsgBox.h"
#include "OnlineNotice.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineConfig.h"
#include "..\Gersang\directx.h"
#include "OnlineVersion.h"
#include "MakePalette.h"


extern _clGame* pGame;
extern _InputDevice			IpD;

#define RESTART_CLIENT		88

// H2-S-05.01.27 //
// ĳ���� ������ (����+����Ű)�� Ư�� ���� �Է� �������� Ű���� ��ŷ �Լ� �� ����
//HHOOK	g_hHook = NULL;
//BOOL	g_fGameState = FALSE;
//LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam );
// H2-E-05.01.27

VOID	cltOnlineWorld::InitLogInMgr()
{
	FreeLogInMgr();

/*	UI08	uiPalette[768];	
	clGrp.LoadOnlyPaletteFile("pal\\newlogin.pal", uiPalette);
	pGame->LoadPalette(uiPalette);
	clGrp.SetColorTable(uiPalette);
	SetGamePalette(uiPalette);
*/
	// �׷��� ���콺�� ���̰� �����Ѵ�.
	ShowSystemCursor(FALSE);
	pIOnlineMouse->SetLoginMouse(FALSE);
	pIOnlineMouse->ShowMouse(TRUE);

	// Ű���� �ʱ�ȭ 
	pOnlineKeyboard->Init();

	g_PingPerUserCount[0] = 0;
	g_PingPerUserCount[1] = 0;
	
	// �ʱ⼱�� ���� ����
	pConfig->OpenConfig();
	
	pOnlineLogInMgr = new OnlineLogInMgr;
	
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_bg.Spr", pOnlineLogInMgr->BackImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_bg_over.Spr", pOnlineLogInMgr->BackOverImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\login_play_grade.Spr", pOnlineLogInMgr->PlayGradeImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\button_1.Spr", pOnlineLogInMgr->SButtonImage);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\button_2.Spr", pOnlineLogInMgr->LButtonImage);
	if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Gamania.Spr", pOnlineLogInMgr->m_Spr_CompanyLogo );	// �ѱ�/�߱�/�Ϻ�/�븸/ȫ���
	else														clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Logo_Bolehcom.Spr", pOnlineLogInMgr->m_Spr_CompanyLogo );	// �ε��׽þƿ�

	// actdoll (2004/12/20 19:13) : �ΰ� ����
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\Login_Gersang_Logo.Spr",		pOnlineLogInMgr->LogoImage );		// �ΰ� �Ƚ�
	
	pOnlineLogInMgr->siStatus			= ON_LOGINMGR_LICENSE;
	pOnlineLogInMgr->bMouseDown			= FALSE;
	pOnlineLogInMgr->m_StartTickCount	= GetTickCount();
	pOnlineLogInMgr->bStatusBack		= FALSE;
	pOnlineLogInMgr->LogInTime			= 0;

	// ������� �ʱ�ȭ
	InitAgreeNotice(TRUE);
}

UI16	cltOnlineWorld::LogInMgrPoll()
{
	UI16	uiNextMenu = 0, uiReturnStatus = 0;

	pOnlineClient->DataProc();

	if(pOnlineClientChatting)
	{
		pOnlineClientChatting->DataProc();
	}

	m_siMsgBoxStatus = pOnlineLoginMsgBox->Process();

	if(!pOnlineLogInMgr)
	{
		// �α��� �Ŵ��� �ʱ�ȭ
		InitLogInMgr();
		// H2-S-05.10.27 //
		// ĳ���� �����ÿ��� ����Ű �������� �÷��� ����
		//g_fGameState = TRUE;
		// H2-E-05.01.27 //
	}

	pOnlineKeyboard->Action();
	pIOnlineMouse->GetPos(&pOnlineLogInMgr->ptMouse);				// ���콺 ��ǥ�� ���Ѵ�.

	if(m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK && m_bForcedLogout)
		pOnlineLogInMgr->siStatus = ON_LOGINMGR_EXIT; 

	switch(pOnlineLogInMgr->siStatus)
	{
	case ON_LOGINMGR_LICENSE:
		{
			// �̿��� Ȯ��
			if(bFirstLogIn)
			{
				uiReturnStatus = AgreeNotice();
				if(uiReturnStatus == ON_RETURN_OK)
				{
					FreeAgreeNotice();
					InitSelectServer();
					pOnlineLogInMgr->siStatus = ON_LOGINMGR_FRONTSERVER_CONNECT;
				}
				else if(uiReturnStatus == ON_RETURN_EXIT)
				{
					FreeAgreeNotice();
					pOnlineLogInMgr->siStatus = ON_LOGINMGR_EXIT;
				}
			}
			else
			{
				FreeAgreeNotice();
				InitSelectServer();
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_FRONTSERVER_CONNECT;
			}
		}
		break;
	case ON_LOGINMGR_FRONTSERVER_CONNECT:		// FrontServer ����
		{
			CHAR TempIP[512];
			sprintf(TempIP, g_stConnectServerInfo.m_FrontServerIP);
			pOnlineClient->InitZero();	
			pOnlineClient->InitSocket();
			pOnlineClientChatting->Close();
			if(pOnlineClient->Init(this, TempIP, 20000, FALSE) == TRUE)
			{
				pOnlineLogInMgr->LogInTime = timeGetTime();
				if(pOnlineLogInMgr->bStatusBack)			// �����α��ο��� ��������ȭ������ �ǵ��ƿ� ����
				{
					pOnlineLogInMgr->bStatusBack = FALSE;

					// �������� ��û
					pOnlineClient->SendRequestWorldInfo();
					pOnlineLogInMgr->siStatus = ON_LOGINMGR_SERVER_INFO_REQUEST;
				}
				else
					pOnlineLogInMgr->siStatus = ON_LOGINMGR_CLIENT_VERSION_CHECK;
			}
			else		
			{				
				pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CANTCONNECTSERVER), pOnlineText->Get(ON_TEXT_ERROR_WAIT));
				m_bForcedLogout = TRUE;
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_WAIT;
			}
		}
		break;
	case ON_LOGINMGR_CLIENT_VERSION_CHECK:
		{
			// ����üũ ��û
			pOnlineClient->SendClientKind();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_CLIENT_VERSION_CHECK_WAIT;
		}
		break;
	case ON_LOGINMGR_CLIENT_VERSION_CHECK_WAIT:
		{
			FRONT_USER::sResponseCheckVersion *pOnClientVerifyMsg = (FRONT_USER::sResponseCheckVersion*)pOnlineClient->GetMsgBuffer((DWORD)(FRONT_USER::RESPONSE_CHECK_VERSION));
			if(!pOnClientVerifyMsg)		break;

//			if(pGame->LanguageCode == ELANGAGECODE_CHINA)
//			{
//				FILE* fp;
//				char  Reg_Path[256];
//				HKEY	key;
//				DWORD	dwDisp;
//				char	TempBuffer[512];
//				int		TempServiceType = 100;
//
//				if(( fp = fopen("damduk.dat","rt")) == NULL)
//				{
//					fp = fopen("damduk.dat","wt");
//
//					strcpy((char*)Reg_Path, "Software\\JOYON\\GERSANG\\");
//
//					RegCreateKeyEx(HKEY_LOCAL_MACHINE, Reg_Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &dwDisp);
//
//					sprintf((char*)TempBuffer, "%d", TempServiceType);
//					RegSetValueEx(key, "ServiceType",  0, REG_SZ, (BYTE*)TempBuffer, strlen((char*)TempBuffer));
//					RegCloseKey(key);
//
//					pOnClientVerifyMsg->responseKind = RESTART_CLIENT;							//Ŭ���̾�Ʈ�� �ٽ� �����Ű���� ��û�Ѵ�.
//				}
//				fclose(fp);
//				//break;
//			}

			
			
			if(pOnClientVerifyMsg->responseKind == FRONT::OK)							// �������� Ŭ���̾�Ʈ�̴�.
			{
				// �������� ��û
				pOnlineClient->SendAdminMsg();
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_NOTICE_REQUEST;
			}
			else if(pOnClientVerifyMsg->responseKind == FRONT_USER::INVALID_VERSION)	// ������ ���� �ʴ�.
			{
				pOnlineLoginMsgBox->Initialize();
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_ERROR_NOTCONNECT), pOnlineText->Get(ON_TEXT_ERROR_VERSION));
				m_bForcedLogout = TRUE;
			}
			else if(pOnClientVerifyMsg->responseKind == FRONT::UNKNOWN_FAIL)			// �˼����� ����
			{
				pOnlineLoginMsgBox->Initialize();
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_ERROR_NOTCONNECT), pOnlineText->Get(ON_TEXT_INVALID_CLIENT_VERSION));
				m_bForcedLogout = TRUE;
			}
			else if(pOnClientVerifyMsg->responseKind == RESTART_CLIENT)								// 88
			{
				pOnlineLoginMsgBox->Initialize();
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", "Please, Restart Gersang. ");
				m_bForcedLogout = TRUE;
			}
			else
				break;

			
		}
		break;
	case ON_LOGINMGR_NOTICE_REQUEST:
		{
			FRONT_USER::sResponseNotice* pAdminMsg = (FRONT_USER::sResponseNotice*)pOnlineClient->GetMsgBuffer((DWORD)FRONT_USER::RESPONSE_NOTICE);
			if(!pAdminMsg)		break;
			
			// ���������� �������� �޾Ƽ� �־��ش�.
			ZeroMemory(pOnlineLogInMgr->szNotice, sizeof(FRONT::NOTICE_MAX_LENGTH));
			strcpy(pOnlineLogInMgr->szNotice, (char*)pAdminMsg->notice);
			
			// �������� ��û
			pOnlineClient->SendRequestWorldInfo();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_SERVER_INFO_REQUEST;
		}
		break;
	case ON_LOGINMGR_SERVER_INFO_REQUEST:
		{
			// ��� ������ ������ �޾Ƽ� �����Ѵ�.(�̸�, ���� ���̵�, ���絿����, ������¥)
			FRONT_USER::sResponseWorldsInfo* pWorldInfoMsg = (FRONT_USER::sResponseWorldsInfo*)pOnlineClient->GetMsgBuffer((DWORD)FRONT_USER::RESPONSE_WORLDS_INFO);
			if(!pWorldInfoMsg)		break;

			if(pWorldInfoMsg->numWorld > 0)
			{
				pOnlineLogInMgr->m_SelectServerData.siWorldCount = pWorldInfoMsg->numWorld;
				if(!pOnlineLogInMgr->m_SelectServerData.pWorldInfo)
					pOnlineLogInMgr->m_SelectServerData.pWorldInfo = new FRONT_USER::sWorldInfo[pWorldInfoMsg->numWorld];
				memcpy(pOnlineLogInMgr->m_SelectServerData.pWorldInfo, pWorldInfoMsg->worldsInfo, sizeof(FRONT_USER::sWorldInfo) * pWorldInfoMsg->numWorld);

				pOnlineLogInMgr->m_SelectServerData.OkButton.SetAction(TRUE);
			}

			pOnlineLogInMgr->siStatus = ON_LOGINMGR_SERVER_SELECT;
		}
		break;
	case ON_LOGINMGR_SERVER_SELECT:
		uiReturnStatus = SelectServer();
		if(uiReturnStatus == ON_RETURN_OK)
		{
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_GAMESERVER_LOGIN;
		}
		else if(uiReturnStatus == ON_RETURN_EXIT)
		{
			FreeSelectServer();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_EXIT;
		}
		break;
	case ON_LOGINMGR_GAMESERVER_LOGIN:
		{
			// ���� ������ �����Ѵ�.
			pOnlineClient->InitZero();	
			pOnlineClient->InitSocket();
			if(pOnlineClient->Init(this, pOnlineClient->IPAddress, 8000, TRUE) == TRUE)		// g_stConnectServerInfo.m_GatePort, 8000
			{				
				FreeSelectServer();
				pOnlineLogInMgr->m_GameServerLogIn.siLogInTime = timeGetTime();
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_RECV_PWDATA;			
				
			}
			else
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DATA_GAMESERVERCONNECTERROR));
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_FRONTSERVER_CONNECT;
			}
		}
		break;
	case ON_LOGINMGR_RECV_PWDATA:
		{
			pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", pOnlineText->Get(ON_TEXT_WAIT));
			
			SI16 siTempCount = max(7 - ((timeGetTime() - pOnlineLogInMgr->m_GameServerLogIn.siLogInTime) / 1000), 0);
			if(siTempCount < 0)		// 7�ʰ� ���
			{
				FreeAccountLogIn();
				InitSelectServer();
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_FRONTSERVER_CONNECT;
			}
			
			if(RecvPWData())
			{
				InitAccountLogIn();
				pOnlineLogInMgr->siStatus = ON_LOGINMGR_ACCOUNT_LOGIN;
			}
		}
		break;
	case ON_LOGINMGR_ACCOUNT_LOGIN:
		uiReturnStatus = AccountLogIn();
		if(uiReturnStatus == ON_RETURN_OK)
		{
			FreeAccountLogIn();
			InitAgreeNotice(FALSE);
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_NOTICE;
		}
		else if(uiReturnStatus == ON_RETURN_EXIT)
		{
			FreeAccountLogIn();
			InitSelectServer();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_FRONTSERVER_CONNECT;
		}
		break;
	case ON_LOGINMGR_NOTICE:
		uiReturnStatus = AgreeNotice();
		if(uiReturnStatus == ON_RETURN_OK)
		{
			FreeAgreeNotice();
			InitSelectCharacter();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_CHAR_SELECT;
		}
		else if(uiReturnStatus == ON_RETURN_EXIT)
		{
			FreeAgreeNotice();
			InitAccountLogIn();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_GAMESERVER_LOGIN;
		}
		break;
	case ON_LOGINMGR_CHAR_SELECT:
		uiReturnStatus = SelectCharacter();
		if(uiReturnStatus == ON_RETURN_OK)
		{
			FreeSelectCharacter();
			InitGameServerLogIn();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_IN_GAMEPOLL;
		}
		else if(uiReturnStatus == ON_RETURN_CREATE)
		{
			FreeSelectCharacter();
			InitCreateCharacter();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_CHAR_CREATE;
		}
		else if(uiReturnStatus == ON_RETURN_EXIT)
		{
			FreeSelectCharacter();
			InitAccountLogIn();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_GAMESERVER_LOGIN;
		}
		break;
	case ON_LOGINMGR_CHAR_CREATE:
		// H2-S-05.01.27
		// ����Ű �������� ��ŷ �Լ� ���� 
		//g_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, ::GetCurrentThreadId());
		// H2-E-05.01.27 
		uiReturnStatus = CreateCharacter();
		if(uiReturnStatus == ON_RETURN_EXIT)
		{
			// H2-S-05.01.27 //
			// UnhookWindowsHookEx(g_hHook) �Լ��� ��ŷ �Լ��� ���� �Ǵ� �� �˾Ҵµ� �ȵǼ�  g_fGameState�� ���� ���¿�
			// ���� ����Ű ����
			//g_fGameState = FALSE;
			//UnhookWindowsHookEx(g_hHook);
			// H2-E-05.10.27
			FreeCreateCharacter();
			InitSelectCharacter();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_CHAR_SELECT;			
		}
		break;
	case ON_LOGINMGR_IN_GAMEPOLL:
		uiReturnStatus = GameServerLogIn();
		if(uiReturnStatus == ON_RETURN_OK)
		{
			FreeGameServerLogIn();
			FreeLogInMgr();
			clGrp.ClearSurface(clGrp.lpDDSPrimary);
			uiNextMenu = ON_MAINPOLL_INITGAME;
		}
		else if(uiReturnStatus == ON_RETURN_EXIT)
		{
			FreeGameServerLogIn();
			InitSelectCharacter();
			pOnlineLogInMgr->siStatus = ON_LOGINMGR_GAMESERVER_LOGIN;
		}
		break;
	case ON_LOGINMGR_EXIT:
		{
			uiNextMenu = ON_MAINPOLL_FORCED_LOGOUT;
		}
		break;
	case ON_LOGINMGR_WAIT:
		break;
	}

	if(uiNextMenu != ON_MAINPOLL_INITGAME)
	{
		DrawLogInMgr();

		pOnlineLogInMgr->bMouseDown = IpD.LeftPressSwitch ? TRUE : FALSE;
		if(uiNextMenu == ON_MAINPOLL_FORCED_LOGOUT)
			FreeLogInMgr();
	}

	return uiNextMenu;
}

BOOL cltOnlineWorld::RecvPWData()
{
	OnEncryptionKey*	pEncryptionKey = (OnEncryptionKey*)pOnlineClient->GetMsgBuffer(ON_NOTICE_ENCRYPTION_KEY);
	
	if(pEncryptionKey)
	{
		pOnlineClient->m_BlackPig.SetKey(pEncryptionKey->cKey1, pEncryptionKey->cKey2);
		pOnlineClient->SetSeedKey(pEncryptionKey->dwRoundKey);
		return TRUE;
	}
	return FALSE;
}

VOID cltOnlineWorld::DrawLogInMgr()
{
	HDC		hDC;

	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		clGrp.PutSpriteT(0, 0, pOnlineLogInMgr->BackImage.Header.Xsize, pOnlineLogInMgr->BackImage.Header.Ysize, pOnlineLogInMgr->BackImage.Image);
		clGrp.PutSpriteT(490, 23, pOnlineLogInMgr->LogoImage.Header.Xsize , pOnlineLogInMgr->LogoImage.Header.Ysize, pOnlineLogInMgr->LogoImage.Image);
//		clGrp.PutSpriteT(479, 18, pOnlineLogInMgr->LogoImage.Header.Xsize , pOnlineLogInMgr->LogoImage.Header.Ysize, pOnlineLogInMgr->LogoImage.Image);

		if(pOnlineLogInMgr->siStatus != ON_LOGINMGR_CHAR_SELECT && pOnlineLogInMgr->siStatus != ON_LOGINMGR_CHAR_CREATE)
		{
			clGrp.PutSpriteT(0, 22, pOnlineLogInMgr->BackOverImage.Header.Xsize, pOnlineLogInMgr->BackOverImage.Header.Ysize, pOnlineLogInMgr->BackOverImage.Image );
			clGrp.PutSpriteT( 186, 550, pOnlineLogInMgr->m_Spr_CompanyLogo.Header.Xsize, pOnlineLogInMgr->m_Spr_CompanyLogo.Header.Ysize, pOnlineLogInMgr->m_Spr_CompanyLogo.Image );
			// actdoll (2004/09/01 13:36) : �̿밡�� ���ɹ����� ���� �ΰ�
			if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_KOREA )	clGrp.PutSpriteT(10, 10, pOnlineLogInMgr->PlayGradeImage.Header.Xsize, pOnlineLogInMgr->PlayGradeImage.Header.Ysize, pOnlineLogInMgr->PlayGradeImage.Image);
		}

		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}

	switch(pOnlineLogInMgr->siStatus)
	{
	case ON_LOGINMGR_LICENSE:
		DrawAgreeNotice();
		break; 
	case ON_LOGINMGR_FRONTSERVER_CONNECT:
	case ON_LOGINMGR_CLIENT_VERSION_CHECK:
	case ON_LOGINMGR_CLIENT_VERSION_CHECK_WAIT:
	case ON_LOGINMGR_NOTICE_REQUEST:
	case ON_LOGINMGR_SERVER_INFO_REQUEST:
		break;
	case ON_LOGINMGR_SERVER_SELECT:
		DrawSelectServer();
		break;
	case ON_LOGINMGR_GAMESERVER_LOGIN:
	case ON_LOGINMGR_RECV_PWDATA:
		break;
	case ON_LOGINMGR_ACCOUNT_LOGIN:
		DrawAccountLogIn();
		break;
	case ON_LOGINMGR_NOTICE:
		DrawAgreeNotice();
		break; 
	case ON_LOGINMGR_CHAR_SELECT:
		DrawSelectCharacter();
		break;
	case ON_LOGINMGR_CHAR_CREATE:
		DrawCreateCharacter();
		break;
	case ON_LOGINMGR_IN_GAMEPOLL:
		DrawGameServerLogIn();
		break;
	case ON_LOGINMGR_EXIT:
	case ON_LOGINMGR_WAIT:
		break;
	default:
		break;
	}

	if(GETSCREENSURFACE(this)->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		char temp[128];
		
		// actdoll (2004/09/02 4:23) : ���̼��� �ؽ�Ʈ
    		// actdoll (2004/09/02 4:23) : ���̼��� �ؽ�Ʈ
		if( pOnlineLogInMgr->siStatus != ON_LOGINMGR_CHAR_CREATE && pOnlineLogInMgr->siStatus != ON_LOGINMGR_CHAR_SELECT )
		{
			char	*pszLicense;
			if( pGame->m_iNation != DF_CODE_NATION_IDENTITY_INDONESIA )	pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_GAMANIA;
			else														pszLicense	= (char*)DF_TEXT_COPYRIGHT_WITH_BOLEHCOM;
			SelectObject( hDC, pOnlineFonts->GetFont(ON_FONT_ITEM));
			pOnlineMegaText->DrawText( hDC, 286, 564, pszLicense, RGB(250, 250, 250) );

			SelectObject(hDC, pOnlineFonts->GetFont(ON_FONT_CHAT));
			pOnlineMegaText->DrawText(hDC, 290, 579, pOnlineText->Get(ON_TEXT_LOADING_NOTICE), RGB(250, 250, 250));
		}

		// actdoll (2004/03/03 19:02) : ����� ��忡���� ����� �������� ǥ���Ѵ�.
#ifdef	_DEBUG
		sprintf( temp,"Debuged Mode %d.%04d", ON_CLINETVERSION / 10000, ON_CLINETVERSION % 10000 );
		pOnlineMegaText->DrawText(hDC, 650, 579, temp , RGB(250, 250, 250));
#else//	no _DEBUG
		sprintf( temp,"Ver %d.%04d", ON_CLINETVERSION / 10000, ON_CLINETVERSION % 10000 );
		pOnlineMegaText->DrawText(hDC, 730, 579, temp , RGB(250, 250, 250));
#endif//_DEBUG

		GETSCREENSURFACE(this)->ReleaseDC(hDC);
	}

	pOnlineLoginMsgBox->Draw(GETSCREENSURFACE(this));
	pOnlineHelp->Show(GETSCREENSURFACE(this));
	
	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		// ���콺�� ��´�.
		pIOnlineMouse->Draw();
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}

	UpdateScreen(GETSCREENSURFACE(this), TRUE);
}

VOID	cltOnlineWorld::FreeLogInMgr()
{
	if(pOnlineLogInMgr)
	{
		clGrp.FreeXspr(pOnlineLogInMgr->BackImage);
		clGrp.FreeXspr(pOnlineLogInMgr->BackOverImage);
		clGrp.FreeXspr(pOnlineLogInMgr->AniLogoImage);
		clGrp.FreeXspr(pOnlineLogInMgr->LogoImage);
		clGrp.FreeXspr(pOnlineLogInMgr->PlayGradeImage);	
		clGrp.FreeXspr(pOnlineLogInMgr->SButtonImage);
		clGrp.FreeXspr(pOnlineLogInMgr->LButtonImage);
		clGrp.FreeXspr(pOnlineLogInMgr->m_Spr_CompanyLogo);

		// OnlineNoticeData ����
		FreeAgreeNotice();

		// OnlineSelectServerData ����
		FreeSelectServer();

		// OnlineLogInData ����
		FreeAccountLogIn();
		
		// OnlineSelectCharacterData ����
		FreeSelectCharacter();

		// OnlineCreateCharacter ����
		FreeCreateCharacter();

		// OnlineGameServerLogIn ����
		FreeGameServerLogIn();

		delete pOnlineLogInMgr;
		pOnlineLogInMgr = NULL; 
		
		// ���� �ȷ�Ʈ�� �ǵ����ش�.
/*		UI08	uiPalette[768];	
		clGrp.LoadOnlyPaletteFile("pal\\Imjin2.pal", uiPalette);
//		SetGamePalette(uiPalette);
		pGame->LoadPalette(uiPalette);
		clGrp.SetColorTable(uiPalette);
*/
		// Imjin2 �ȷ�Ʈ ���콺�� �����Ѵ�.
		ShowSystemCursor(FALSE);	
		pIOnlineMouse->SetLoginMouse(FALSE);
		pIOnlineMouse->ShowMouse(TRUE);
	}
}

// H2-S-05.01.27 //
// ĳ���� ������ (����+����Ű)�� Ư�� ���� �Է� �������� Ű���� ��ŷ �Լ�
/*
LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{	   
    if (code < 0)
        return CallNextHookEx(g_hHook, code, wParam, lParam);
    else
    {
        if (wParam == VK_HANJA && g_fGameState)
            return -1;
        else
            return CallNextHookEx(g_hHook, code, wParam, lParam);
    }
}
*/
// H2-E-05.10.27 //