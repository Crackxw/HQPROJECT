// ���ϸ� : OnlineWorld-LogInOut.cpp
// ����� : ���ؼ�

#include <GSL.h>

#include <Mouse.h>
#include <clGame.h>

#include "..\Gersang\directx.h"

#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineWorld.h"
#include "OnlineGameMsg.h"
#include "OnlineConfig.h"
#include "OnlineMegaText.h"
#include "OnlineHelp.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineClient.h"
#include "OnlineResource.h"
#include "OnlineText.h"
#include "OnlineFieldChat.h"
#include "OnlineMsgBox.h"
#include "OnlineNotice.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include <Common.h>

// actdoll (2004/05/04 16:33) : [OECEX] �� ����Ʈ ��Ʈ�ѷ� ����
#include <dimm.h>		// actdoll (2004/10/25 15:16) : �̳��� Microsoft Flatform SDK�� Internet Explorer SDK�� ��� �ȴ�.
#include "GS_IMEBase.h"
#include "OnlineEditControlEx.h"
#include "Handler_OnlineEditControlEx.h"


enum
{   
	ON_ACCOUNT_LOGIN_INIT				= 0,
	ON_ACCOUNT_LOGIN_POLL,
	ON_ACCOUNT_LOGIN_WAIT,
	ON_ACCOUNT_LOGIN_CHATSERVER_CONNECT,
	ON_ACCOUNT_LOGIN_CHARINFO_WAIT
};


extern _clGame* pGame;
extern _InputDevice			IpD;
extern	int					bActive;


//----------------------------------------------------------------------------------------------------------------
//	����	:	�α��� �ϱ����� ȣ���ϴ� �Լ�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitAccountLogIn()
{
	// �ٽ� �α��� �� ��� ���� ����Ÿ �ʱ�ȭ
	pIOnlineChar->AllFree();	

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_login_box.Spr", pOnlineLogInMgr->m_LogInData.LogInBoxImage);

	// ��ư�� �����Ѵ�.
	pOnlineLogInMgr->m_LogInData.OkButton.Create(561, 367, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_LOGIN), BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_LogInData.JoyOnButton.Create(561, 412, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, g_stConnectServerInfo.m_ServiceName, BUTTON_PUT_LEFT, TRUE);
	pOnlineLogInMgr->m_LogInData.RegistrationButton.Create(561, 457, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_NEWACCOUNT), BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_LogInData.ExitButton.Create(561, 502, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_BACK), BUTTON_PUT_LEFT, TRUE );

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	HOEC	hOec;
	RECT	rcRect;
	OnlineEditControlEx		*pOecEx;

	// ���� IDâ
	SetRect( &rcRect, 550, 271, 700, 286 );
	DWORD	dwOecType = 0;
	hOec	= Handler_OnlineEditControlEx::CreateControl( dwOecType, &rcRect, LOGIN::ID_MAX_LENGTH - 1 );
	
	if( !hOec )		clGrp.Error("", "OecEx Creation Failed1 !!");
	pOnlineLogInMgr->m_LogInData.m_hOecID	= hOec;

	// �н����� â
	SetRect( &rcRect, 550, 316, 700, 331 );
	hOec	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_PASSWORD, &rcRect, LOGIN::PW_MAX_LENGTH - 1 );
	if( !hOec )		clGrp.Error("", "OecEx Creation Failed2 !!");
	pOnlineLogInMgr->m_LogInData.m_hOecPW	= hOec;

	// ��Ÿ ���õ�(ID)
	pOecEx	= (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_LogInData.m_hOecID );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetExceptionChar( ' ' );
	pOecEx->SetExceptionChar( '\'' );

	// ��Ÿ ���õ�(PW)
	pOecEx	= (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_LogInData.m_hOecPW );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	Handler_OnlineEditControlEx::SetNextHandle( pOnlineLogInMgr->m_LogInData.m_hOecID, pOnlineLogInMgr->m_LogInData.m_hOecPW );
	Handler_OnlineEditControlEx::SetNextHandle( pOnlineLogInMgr->m_LogInData.m_hOecPW, pOnlineLogInMgr->m_LogInData.m_hOecID );
	
	pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
	pOnlineLogInMgr->m_LogInData.siCount		=	0;
	ZeroMemory(pOnlineLogInMgr->m_LogInData.szID,   sizeof(pOnlineLogInMgr->m_LogInData.szID));
	ZeroMemory(pOnlineLogInMgr->m_LogInData.szPW,   sizeof(pOnlineLogInMgr->m_LogInData.szPW));

	// Ű���� �ʱ�ȭ 
	pOnlineKeyboard->Init();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�α��� �Ҷ� ȣ���ϴ��Լ�.
//----------------------------------------------------------------------------------------------------------------
SI08 cltOnlineWorld::AccountLogIn()
{
	static	CHAR	szLastInputID[ ON_ACCOUNT_LENGTH + 1 ];
	UI16	uiNextMenu = 0;	
	
	// ������ �ʱ�ȭ �Ѵ�.
	pOnlineHelp->Initialize();

	// ��ư ��Ʈ���� �޾Ƴ��´�.
	HOEC					hOecID, hOecPW;
	OnlineEditControlEx		*pOecID, *pOecPW;

	hOecID	= pOnlineLogInMgr->m_LogInData.m_hOecID;
	hOecPW	= pOnlineLogInMgr->m_LogInData.m_hOecPW;
	pOecID	= Handler_OnlineEditControlEx::GetControl( hOecID );
	pOecPW	= Handler_OnlineEditControlEx::GetControl( hOecPW );

	// Exit ��ư�� ���������� �� �޴��� ����.
	if( pOnlineLoginMsgBox->IsActive() == FALSE )
	{
		if(pOnlineLogInMgr->m_LogInData.ExitButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
		{
			pOnlineClient->Close();
			pOnlineLogInMgr->bStatusBack = TRUE;
			pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);
			return ON_RETURN_EXIT;
		}
	}
	
	switch(pOnlineLogInMgr->m_LogInData.siLogInStatus)
	{	
	case ON_ACCOUNT_LOGIN_INIT:
		{
			pOnlineLogInMgr->m_LogInData.OkButton.SetAction(TRUE);
			pOnlineLogInMgr->m_LogInData.JoyOnButton.SetAction(TRUE);
			pOnlineLogInMgr->m_LogInData.RegistrationButton.SetAction(TRUE);
			pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);

			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
			pOecID->Clear( true );
			pOecPW->Clear( true );
			Handler_OnlineEditControlEx::ClearCurrentFocus();
			
			// Config ���Ͽ��� ID�� & �н����带 �޾� ����Ʈ �ڽ��� �־� �ָ� Ŀ���� �������� ��ġ ���� �ش�.
			pConfig->OpenConfig();

			if( szLastInputID[ 0 ] == 0 )
			{
				strncpy( szLastInputID, pConfig->GetID(), ON_ACCOUNT_LENGTH );
			}
			

			if( szLastInputID[0] == 0)
			{
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				Handler_OnlineEditControlEx::SetFocus( hOecID );

			}
			else
			{
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				switch(pGame->LanguageCode)
				{
				case ELANGAGECODE_KOREAN:
					{
						pOecID->SetString( szLastInputID );
						Handler_OnlineEditControlEx::SetFocus( hOecPW );
					}
					break;
				default:
					Handler_OnlineEditControlEx::SetFocus( hOecID );
					break;
				}
			}
			
			pOnlineLogInMgr->m_LogInData.siLogInStatus	=  ON_ACCOUNT_LOGIN_POLL;
		}
		break;	
	case ON_ACCOUNT_LOGIN_POLL:
		{
/*			if(pOnlineLogInMgr->m_LogInData.siCount > 3)
			{
				m_bForcedLogout = TRUE;
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_CONNECT_3ERROR));
			}*/

			// �α��� �ڽ��� ������ ��� �۾� �ߴ�.
			if(pOnlineLoginMsgBox->IsActive())
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				Handler_OnlineEditControlEx::ClearCurrentFocus();
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(FALSE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(FALSE);
			}
			
			// �α��� �ڽ����� OK ��ȣ�� �������ٸ� ������� ������.
			if(m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
			{ 
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				if( pOecID->GetStringLen() > 0 )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
				else								Handler_OnlineEditControlEx::SetFocus( hOecID );
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(TRUE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);
			}
			
			// ���� �α׾ƿ� ó���� �ƴϸ�, ESCŰ�� �����ٸ� �α��� �ڽ� �� �ִ� ���� ��������.
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)) && !m_bForcedLogout)
			{
				pOnlineLoginMsgBox->SetAction(FALSE);
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(TRUE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);

				if( pOecID->GetStringLen() > 0 )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
				else								Handler_OnlineEditControlEx::SetFocus( hOecID );
			}
			
			// �α��� �ڽ��� Ȱ��ȭ ���̶�� ���� �۾� ����.
			if(pOnlineLoginMsgBox->IsActive())		break;
			
			if( (pOnlineLogInMgr->m_LogInData.OkButton.IsInMouse() && pOnlineLogInMgr->bMouseDown) ||	// �α��� ��ư ���콺�� ���Ȱų�
				( pOecPW->IsPushEnter() && Handler_OnlineEditControlEx::IsFocus( hOecPW ) ))			// �н����� â���� ���Ͱ� ���Ȱų�
			{			
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				strcpy( pOnlineLogInMgr->m_LogInData.szID, pOecID->GetString() );
				strcpy( pOnlineLogInMgr->m_LogInData.szPW, pOecPW->GetString() );

				// ���̵� �н����尡 �����̸� �α��� �޽����� ������ �ʴ´�.
				if( ( strlen( pOnlineLogInMgr->m_LogInData.szID ) == 0 ) || ( strlen( pOnlineLogInMgr->m_LogInData.szPW ) == 0 ) || ( pOnlineLogInMgr->m_LogInData.szID[0] == '!') )
				{				
					if( strlen( pOnlineLogInMgr->m_LogInData.szID ) == 0 )		pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_ID_ERROR ) );
					else if( strlen( pOnlineLogInMgr->m_LogInData.szPW ) == 0 )	pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_PASSWORD_ERROR ) );
					else														pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_IDCREATE_NOTID));
					
					pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				}
				// ���̵� Ȥ�� �н����� ���ڿ��� ������ �ִٸ� ����
				else if(!CheckLeadByte(pOnlineLogInMgr->m_LogInData.szID))
					pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_ERROR));
				else if(!CheckLeadByte(pOnlineLogInMgr->m_LogInData.szPW))
					pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_PASSWORD_ERROR));
				// �ƹ� ������ ���ٸ� ó�� ����!
				else
				{			
					// ������ �Է��ߴ� ID�� �����Ѵ�.
					ZeroMemory( szLastInputID, sizeof( szLastInputID ) );
					strncpy( szLastInputID, pOnlineLogInMgr->m_LogInData.szID, ON_ACCOUNT_LENGTH );

					pOnlineClient->SendAccountLogIn(pOnlineLogInMgr->m_LogInData.szID, pOnlineLogInMgr->m_LogInData.szPW);
					
					pOnlineLogInMgr->m_LogInData.siLogInStatus	=  ON_ACCOUNT_LOGIN_WAIT;
					
					// ID�� �н����带 �Է��ϴ� ����Ʈ ��Ʈ���� Disable ��Ų��.
					// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
					Handler_OnlineEditControlEx::ClearCurrentFocus();
					pOnlineLogInMgr->m_LogInData.OkButton.SetAction(FALSE);
					pOnlineLogInMgr->m_LogInData.JoyOnButton.SetAction(FALSE);
					pOnlineLogInMgr->m_LogInData.RegistrationButton.SetAction(FALSE);

					pOnlineLogInMgr->m_StartTickCount = timeGetTime();
				}
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				pOecPW->Clear( true );
			}
			
			// ID���� ���Ͱ� ������ ��Ŀ���� IDâ�� ������ �ִٸ� �н����� â���� ��Ŀ�� �̵�
			if( pOecID->IsPushEnter() && Handler_OnlineEditControlEx::IsFocus( hOecID ) )
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				Handler_OnlineEditControlEx::SetFocus( hOecPW );
			}
			// ===============================================================================
			
			// ���콺�� Ŭ�� �Ǿ��� �� ������ ���� ��Ŀ�� ��ġ�� �̵�
			if( pOnlineLogInMgr->bMouseDown )
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
				if( pOecID->IsPointInRect( pOnlineLogInMgr->ptMouse ) )			Handler_OnlineEditControlEx::SetFocus( hOecID );
				else if( pOecPW->IsPointInRect( pOnlineLogInMgr->ptMouse ) )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
			}
			
			// ���̿� Ȩ������
			if(pOnlineLogInMgr->m_LogInData.JoyOnButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
			{
				ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceURL, NULL, NULL, SW_SHOWNORMAL );		
			}
			
			// �� ���� ��� Ȩ������
			if(pOnlineLogInMgr->m_LogInData.RegistrationButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
			{
				ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceCreateAccountURL, NULL, NULL, SW_SHOWNORMAL );
			}
		}
		break;
	case ON_ACCOUNT_LOGIN_WAIT:	
		{
			// ���� �α��� ������ ��ٸ���.
			pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", pOnlineText->Get(ON_TEXT_WAIT));

			LOGIN::sResponseLogin *pOnLogInMsg = (LOGIN::sResponseLogin*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_LOGIN);			
			
			if( pOnLogInMsg == NULL )		break;
			
			if( pOnLogInMsg->responseKind == LOGIN::OK )									// �α��� ����
			{
				pConfig->SetConfig( pOnlineLogInMgr->m_LogInData.szID );
				pConfig->SaveConfig();

				pOnlineLogInMgr->m_LogInData.siLogInStatus	= ON_ACCOUNT_LOGIN_CHATSERVER_CONNECT;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::NOTHAS_AUTH )						// ������ ������ ���� ���� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_NOTHAS_AUTH) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::INVALID_ID )						// ���̵� �ùٸ��� ���� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::INVALID_PASSWORD )					// �н����尡 �ùٸ��� ���� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_PASSWORD_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY )				// ��� �Ⱓ(�ð�)�� ����� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY_TIME_PERDAY )	// ���� ��� �ð��� ����� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY_TIME_PERDAY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY_TERM_PERDAY )	// ���� ��� �Ⱓ�� ����� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY_TERM_PERDAY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::ALREADY_LOGIN )					// �̹� �α��� �Ǿ� �ִ� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_USE) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::USED_ALL_IP )						// ������ IP������ ��� ������� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_USED_ALL_IP) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::NOT_ENOUGHT_POINT )				// ���� ����Ʈ�� ������ ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_NOT_ENOUGHT_POINT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::FAIL_SHORT_TIME )					// 5�� ���� 3�� �̻� �����Ͽ� ��� �ִ� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_FAIL_SHORT_TIME) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::LOCK_MAIN_ACCOUNT )				// ���� ������ ��� �ִ� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_LOCK_MAIN_ACCOUNT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::LOCK_SERVICE_ACCOUNT )				// ����/���� ������ ��� �ִ� ���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_LOCK_SERVICE_ACCOUNT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if(  pOnLogInMsg->responseKind == -85 )									// ����� �ʰ�
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(4000295) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else																			// �˼����� �α��ο���
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_UNKNOWN_LOGIN_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;	
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
		}
		break;
	case ON_ACCOUNT_LOGIN_CHATSERVER_CONNECT:
		{
			// Chatting ������ �����Ѵ�.
			if(!pOnlineClientChatting->Connect( pOnlineClientChatting->IPAddress, g_stConnectServerInfo.m_ChatPort))		// 4011
				pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DISCONNECT_CHATTING_SERVER));

			// ĳ���� ������ ��û�Ѵ�.
			pOnlineClient->SendRequestCharInfo();
			pOnlineLogInMgr->m_LogInData.siLogInStatus	= ON_ACCOUNT_LOGIN_CHARINFO_WAIT;
		}
		break;
	case ON_ACCOUNT_LOGIN_CHARINFO_WAIT:
		{
			LOGIN::sResponseCharsInfo *pResCharsInfo = (LOGIN::sResponseCharsInfo*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_CHARS_INFO);			
			
			if( pResCharsInfo == NULL )	break;
			
			// MyData�� ĳ���� ������ �����Ѵ�.
			pMyData->SetMyCharacters(pResCharsInfo->numChar, pResCharsInfo->charsInfo);

			return ON_RETURN_OK;
		}
		break;
	}

	return ON_RETURN_NONE;
}

VOID	cltOnlineWorld::DrawAccountLogIn()
{
	HDC				hdc;
	CHAR			szTemp[128];
	ZeroMemory(szTemp, sizeof(szTemp));

	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		clGrp.PutSpriteT(523, 233, pOnlineLogInMgr->m_LogInData.LogInBoxImage.Header.Xsize, pOnlineLogInMgr->m_LogInData.LogInBoxImage.Header.Ysize, pOnlineLogInMgr->m_LogInData.LogInBoxImage.Image);

		pOnlineLogInMgr->m_LogInData.OkButton.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_LogInData.JoyOnButton.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_LogInData.ExitButton.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_LogInData.RegistrationButton.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}	
	
	// ��ư ��Ʈ���� �޾Ƴ��´�.
	HOEC					hOecID, hOecPW;
	OnlineEditControlEx		*pOecID, *pOecPW;

	hOecID	= pOnlineLogInMgr->m_LogInData.m_hOecID;
	hOecPW	= pOnlineLogInMgr->m_LogInData.m_hOecPW;
	pOecID	= Handler_OnlineEditControlEx::GetControl( hOecID );
	pOecPW	= Handler_OnlineEditControlEx::GetControl( hOecPW );

	// ���ڸ� ��´�.
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// ���� ��� ���� & ��Ʈ�� �����Ѵ�.
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject( hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		pOnlineMegaText->DrawTextCenter( hdc, 598-1, 252-1, 56, 14, pOnlineText->Get(ON_TEXT_ID), RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter( hdc, 598-1, 297-1, 56, 14, pOnlineText->Get(ON_TEXT_PASSWORD), RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter( hdc, 598, 252, 56, 14, pOnlineText->Get(ON_TEXT_ID), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc, 598, 297, 56, 14, pOnlineText->Get(ON_TEXT_PASSWORD), RGB(250, 250, 250));

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
		if( Handler_OnlineEditControlEx::IsFocus( hOecID ) )		
		{
			pOecID->SetCursorShow( true );
			pOecPW->SetCursorShow( false );
		}
		else if( Handler_OnlineEditControlEx::IsFocus( hOecPW ) )
		{
			pOecID->SetCursorShow( false );
			pOecPW->SetCursorShow( true );
		}
		pOecID->Draw( hdc );
		pOecPW->Draw( hdc );
		
		pOnlineLogInMgr->m_LogInData.OkButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_LogInData.JoyOnButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_LogInData.ExitButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_LogInData.RegistrationButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		
		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}	
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� -->  4) �׸��� �����Ѵ�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeAccountLogIn()
{
	bFirstLogIn = FALSE;

	clGrp.FreeXspr(pOnlineLogInMgr->m_LogInData.LogInBoxImage);

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW�� ����Ʈ ��Ʈ�� ����
	Handler_OnlineEditControlEx::KillControl( pOnlineLogInMgr->m_LogInData.m_hOecID );
	Handler_OnlineEditControlEx::KillControl( pOnlineLogInMgr->m_LogInData.m_hOecPW );
}

