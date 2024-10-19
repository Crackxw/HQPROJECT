// 파일명 : OnlineWorld-LogInOut.cpp
// 담당자 : 이준석

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
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS 용 데이터 추가
#include "OnlinePannel.h"
#include <Common.h>

// actdoll (2004/05/04 16:33) : [OECEX] 새 에디트 컨트롤러 세팅
#include <dimm.h>		// actdoll (2004/10/25 15:16) : 이놈은 Microsoft Flatform SDK의 Internet Explorer SDK를 깔면 된다.
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
//	설명	:	로그인 하기전에 호출하는 함수.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitAccountLogIn()
{
	// 다시 로그인 할 경우 이전 데이타 초기화
	pIOnlineChar->AllFree();	

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_login_box.Spr", pOnlineLogInMgr->m_LogInData.LogInBoxImage);

	// 버튼을 생성한다.
	pOnlineLogInMgr->m_LogInData.OkButton.Create(561, 367, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_LOGIN), BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_LogInData.JoyOnButton.Create(561, 412, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, g_stConnectServerInfo.m_ServiceName, BUTTON_PUT_LEFT, TRUE);
	pOnlineLogInMgr->m_LogInData.RegistrationButton.Create(561, 457, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_NEWACCOUNT), BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_LogInData.ExitButton.Create(561, 502, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_BACK), BUTTON_PUT_LEFT, TRUE );

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
	HOEC	hOec;
	RECT	rcRect;
	OnlineEditControlEx		*pOecEx;

	// 먼저 ID창
	SetRect( &rcRect, 550, 271, 700, 286 );
	DWORD	dwOecType = 0;
	hOec	= Handler_OnlineEditControlEx::CreateControl( dwOecType, &rcRect, LOGIN::ID_MAX_LENGTH - 1 );
	
	if( !hOec )		clGrp.Error("", "OecEx Creation Failed1 !!");
	pOnlineLogInMgr->m_LogInData.m_hOecID	= hOec;

	// 패스워드 창
	SetRect( &rcRect, 550, 316, 700, 331 );
	hOec	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_PASSWORD, &rcRect, LOGIN::PW_MAX_LENGTH - 1 );
	if( !hOec )		clGrp.Error("", "OecEx Creation Failed2 !!");
	pOnlineLogInMgr->m_LogInData.m_hOecPW	= hOec;

	// 기타 세팅들(ID)
	pOecEx	= (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_LogInData.m_hOecID );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->SetExceptionChar( ' ' );
	pOecEx->SetExceptionChar( '\'' );

	// 기타 세팅들(PW)
	pOecEx	= (OnlineEditControlEx*)Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_LogInData.m_hOecPW );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_CHAT ) );

	Handler_OnlineEditControlEx::SetNextHandle( pOnlineLogInMgr->m_LogInData.m_hOecID, pOnlineLogInMgr->m_LogInData.m_hOecPW );
	Handler_OnlineEditControlEx::SetNextHandle( pOnlineLogInMgr->m_LogInData.m_hOecPW, pOnlineLogInMgr->m_LogInData.m_hOecID );
	
	pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
	pOnlineLogInMgr->m_LogInData.siCount		=	0;
	ZeroMemory(pOnlineLogInMgr->m_LogInData.szID,   sizeof(pOnlineLogInMgr->m_LogInData.szID));
	ZeroMemory(pOnlineLogInMgr->m_LogInData.szPW,   sizeof(pOnlineLogInMgr->m_LogInData.szPW));

	// 키보드 초기화 
	pOnlineKeyboard->Init();
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	로그인 할때 호출하는함수.
//----------------------------------------------------------------------------------------------------------------
SI08 cltOnlineWorld::AccountLogIn()
{
	static	CHAR	szLastInputID[ ON_ACCOUNT_LENGTH + 1 ];
	UI16	uiNextMenu = 0;	
	
	// 헬프를 초기화 한다.
	pOnlineHelp->Initialize();

	// 버튼 컨트롤을 받아놓는다.
	HOEC					hOecID, hOecPW;
	OnlineEditControlEx		*pOecID, *pOecPW;

	hOecID	= pOnlineLogInMgr->m_LogInData.m_hOecID;
	hOecPW	= pOnlineLogInMgr->m_LogInData.m_hOecPW;
	pOecID	= Handler_OnlineEditControlEx::GetControl( hOecID );
	pOecPW	= Handler_OnlineEditControlEx::GetControl( hOecPW );

	// Exit 버튼을 눌렀을때는 전 메뉴로 들어간다.
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

			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
			pOecID->Clear( true );
			pOecPW->Clear( true );
			Handler_OnlineEditControlEx::ClearCurrentFocus();
			
			// Config 파일에서 ID를 & 패스워드를 받아 에드트 박스에 넣어 주며 커서는 마지막에 위치 시켜 준다.
			pConfig->OpenConfig();

			if( szLastInputID[ 0 ] == 0 )
			{
				strncpy( szLastInputID, pConfig->GetID(), ON_ACCOUNT_LENGTH );
			}
			

			if( szLastInputID[0] == 0)
			{
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				Handler_OnlineEditControlEx::SetFocus( hOecID );

			}
			else
			{
			// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
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

			// 로그인 박스가 떴더면 모든 작업 중단.
			if(pOnlineLoginMsgBox->IsActive())
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				Handler_OnlineEditControlEx::ClearCurrentFocus();
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(FALSE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(FALSE);
			}
			
			// 로그인 박스에서 OK 신호가 떨어졌다면 원래대로 돌린다.
			if(m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
			{ 
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				if( pOecID->GetStringLen() > 0 )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
				else								Handler_OnlineEditControlEx::SetFocus( hOecID );
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(TRUE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);
			}
			
			// 강제 로그아웃 처리가 아니며, ESC키를 눌렀다면 로그인 박스 떠 있는 것을 꺼버린다.
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)) && !m_bForcedLogout)
			{
				pOnlineLoginMsgBox->SetAction(FALSE);
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				pOnlineLogInMgr->m_LogInData.OkButton.SetAction(TRUE);
				pOnlineLogInMgr->m_LogInData.ExitButton.SetAction(TRUE);

				if( pOecID->GetStringLen() > 0 )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
				else								Handler_OnlineEditControlEx::SetFocus( hOecID );
			}
			
			// 로그인 박스가 활성화 중이라면 이후 작업 없다.
			if(pOnlineLoginMsgBox->IsActive())		break;
			
			if( (pOnlineLogInMgr->m_LogInData.OkButton.IsInMouse() && pOnlineLogInMgr->bMouseDown) ||	// 로그인 버튼 마우스가 눌렸거나
				( pOecPW->IsPushEnter() && Handler_OnlineEditControlEx::IsFocus( hOecPW ) ))			// 패스워드 창에서 엔터가 눌렸거나
			{			
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				strcpy( pOnlineLogInMgr->m_LogInData.szID, pOecID->GetString() );
				strcpy( pOnlineLogInMgr->m_LogInData.szPW, pOecPW->GetString() );

				// 아이디나 패스워드가 공백이면 로그인 메시지를 보내지 않는다.
				if( ( strlen( pOnlineLogInMgr->m_LogInData.szID ) == 0 ) || ( strlen( pOnlineLogInMgr->m_LogInData.szPW ) == 0 ) || ( pOnlineLogInMgr->m_LogInData.szID[0] == '!') )
				{				
					if( strlen( pOnlineLogInMgr->m_LogInData.szID ) == 0 )		pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_ID_ERROR ) );
					else if( strlen( pOnlineLogInMgr->m_LogInData.szPW ) == 0 )	pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_PASSWORD_ERROR ) );
					else														pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get( ON_TEXT_IDCREATE_NOTID));
					
					pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				}
				// 아이디 혹은 패스워드 문자열에 문제가 있다면 에러
				else if(!CheckLeadByte(pOnlineLogInMgr->m_LogInData.szID))
					pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_ERROR));
				else if(!CheckLeadByte(pOnlineLogInMgr->m_LogInData.szPW))
					pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_PASSWORD_ERROR));
				// 아무 문제가 없다면 처리 개시!
				else
				{			
					// 기존에 입력했던 ID를 저장한다.
					ZeroMemory( szLastInputID, sizeof( szLastInputID ) );
					strncpy( szLastInputID, pOnlineLogInMgr->m_LogInData.szID, ON_ACCOUNT_LENGTH );

					pOnlineClient->SendAccountLogIn(pOnlineLogInMgr->m_LogInData.szID, pOnlineLogInMgr->m_LogInData.szPW);
					
					pOnlineLogInMgr->m_LogInData.siLogInStatus	=  ON_ACCOUNT_LOGIN_WAIT;
					
					// ID와 패스워드를 입력하는 에디트 컨트롤을 Disable 시킨다.
					// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
					Handler_OnlineEditControlEx::ClearCurrentFocus();
					pOnlineLogInMgr->m_LogInData.OkButton.SetAction(FALSE);
					pOnlineLogInMgr->m_LogInData.JoyOnButton.SetAction(FALSE);
					pOnlineLogInMgr->m_LogInData.RegistrationButton.SetAction(FALSE);

					pOnlineLogInMgr->m_StartTickCount = timeGetTime();
				}
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				pOecPW->Clear( true );
			}
			
			// ID에서 엔터가 눌리고 포커스가 ID창에 맞춰져 있다면 패스워드 창으로 포커스 이동
			if( pOecID->IsPushEnter() && Handler_OnlineEditControlEx::IsFocus( hOecID ) )
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				Handler_OnlineEditControlEx::SetFocus( hOecPW );
			}
			// ===============================================================================
			
			// 마우스가 클릭 되었을 때 영역에 따라 포커스 위치를 이동
			if( pOnlineLogInMgr->bMouseDown )
			{
				// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
				if( pOecID->IsPointInRect( pOnlineLogInMgr->ptMouse ) )			Handler_OnlineEditControlEx::SetFocus( hOecID );
				else if( pOecPW->IsPointInRect( pOnlineLogInMgr->ptMouse ) )	Handler_OnlineEditControlEx::SetFocus( hOecPW );
			}
			
			// 조이온 홈페이지
			if(pOnlineLogInMgr->m_LogInData.JoyOnButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
			{
				ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceURL, NULL, NULL, SW_SHOWNORMAL );		
			}
			
			// 새 계정 등록 홈페이지
			if(pOnlineLogInMgr->m_LogInData.RegistrationButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
			{
				ShellExecute (  NULL, "open", g_stConnectServerInfo.m_ServiceCreateAccountURL, NULL, NULL, SW_SHOWNORMAL );
			}
		}
		break;
	case ON_ACCOUNT_LOGIN_WAIT:	
		{
			// 계정 로그인 응답을 기다린다.
			pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_NONE, "", pOnlineText->Get(ON_TEXT_WAIT));

			LOGIN::sResponseLogin *pOnLogInMsg = (LOGIN::sResponseLogin*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_LOGIN);			
			
			if( pOnLogInMsg == NULL )		break;
			
			if( pOnLogInMsg->responseKind == LOGIN::OK )									// 로그인 성공
			{
				pConfig->SetConfig( pOnlineLogInMgr->m_LogInData.szID );
				pConfig->SaveConfig();

				pOnlineLogInMgr->m_LogInData.siLogInStatus	= ON_ACCOUNT_LOGIN_CHATSERVER_CONNECT;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::NOTHAS_AUTH )						// 권한을 가지고 있지 않은 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_NOTHAS_AUTH) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::INVALID_ID )						// 아이디가 올바르지 않은 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::INVALID_PASSWORD )					// 패스워드가 올바르지 않은 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_PASSWORD_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY )				// 사용 기간(시간)이 만료된 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY_TIME_PERDAY )	// 일일 사용 시간이 만료된 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY_TIME_PERDAY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::OVER_PLAY_VALIDITY_TERM_PERDAY )	// 일일 사용 기간이 만료된 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_OVER_PLAY_VALIDITY_TERM_PERDAY) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::ALREADY_LOGIN )					// 이미 로그인 되어 있는 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_ID_USE) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::USED_ALL_IP )						// 결제한 IP수량을 모두 사용중인 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_USED_ALL_IP) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::NOT_ENOUGHT_POINT )				// 결제 포인트가 부족한 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_NOT_ENOUGHT_POINT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::FAIL_SHORT_TIME )					// 5분 동안 3번 이상 실패하여 잠겨 있는 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_FAIL_SHORT_TIME) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::LOCK_MAIN_ACCOUNT )				// 메인 계정이 잠겨 있는 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_LOCK_MAIN_ACCOUNT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if( pOnLogInMsg->responseKind == LOGIN::LOCK_SERVICE_ACCOUNT )				// 서비스/게임 계정이 잠겨 있는 경우
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_LOGIN_LOCK_SERVICE_ACCOUNT) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else if(  pOnLogInMsg->responseKind == -85 )									// 사용자 초가
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(4000295) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;		
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
			else																			// 알수없는 로그인에러
			{
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_FAILCONNECT), pOnlineText->Get(ON_TEXT_UNKNOWN_LOGIN_ERROR) );
				pOnlineLogInMgr->m_LogInData.siLogInStatus	=	ON_ACCOUNT_LOGIN_INIT;	
				pOnlineLogInMgr->m_LogInData.siCount++;
			}
		}
		break;
	case ON_ACCOUNT_LOGIN_CHATSERVER_CONNECT:
		{
			// Chatting 서버에 접속한다.
			if(!pOnlineClientChatting->Connect( pOnlineClientChatting->IPAddress, g_stConnectServerInfo.m_ChatPort))		// 4011
				pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pOnlineText->Get(ON_TEXT_DISCONNECT_CHATTING_SERVER));

			// 캐릭터 정보를 요청한다.
			pOnlineClient->SendRequestCharInfo();
			pOnlineLogInMgr->m_LogInData.siLogInStatus	= ON_ACCOUNT_LOGIN_CHARINFO_WAIT;
		}
		break;
	case ON_ACCOUNT_LOGIN_CHARINFO_WAIT:
		{
			LOGIN::sResponseCharsInfo *pResCharsInfo = (LOGIN::sResponseCharsInfo*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_CHARS_INFO);			
			
			if( pResCharsInfo == NULL )	break;
			
			// MyData에 캐릭터 정보를 설정한다.
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
	
	// 버튼 컨트롤을 받아놓는다.
	HOEC					hOecID, hOecPW;
	OnlineEditControlEx		*pOecID, *pOecPW;

	hOecID	= pOnlineLogInMgr->m_LogInData.m_hOecID;
	hOecPW	= pOnlineLogInMgr->m_LogInData.m_hOecPW;
	pOecID	= Handler_OnlineEditControlEx::GetControl( hOecID );
	pOecPW	= Handler_OnlineEditControlEx::GetControl( hOecPW );

	// 글자를 찍는다.
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// 투명 모드 설정 & 폰트를 설정한다.
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject( hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
		
		pOnlineMegaText->DrawTextCenter( hdc, 598-1, 252-1, 56, 14, pOnlineText->Get(ON_TEXT_ID), RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter( hdc, 598-1, 297-1, 56, 14, pOnlineText->Get(ON_TEXT_PASSWORD), RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter( hdc, 598, 252, 56, 14, pOnlineText->Get(ON_TEXT_ID), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc, 598, 297, 56, 14, pOnlineText->Get(ON_TEXT_PASSWORD), RGB(250, 250, 250));

		// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
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
//	설명	:	해제 -->  4) 그림을 해제한다.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeAccountLogIn()
{
	bFirstLogIn = FALSE;

	clGrp.FreeXspr(pOnlineLogInMgr->m_LogInData.LogInBoxImage);

	// actdoll (2004/05/04 16:33) : [OECEX] ID/PW용 에디트 컨트롤 세팅
	Handler_OnlineEditControlEx::KillControl( pOnlineLogInMgr->m_LogInData.m_hOecID );
	Handler_OnlineEditControlEx::KillControl( pOnlineLogInMgr->m_LogInData.m_hOecPW );
}

