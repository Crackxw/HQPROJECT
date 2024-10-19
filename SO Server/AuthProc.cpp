#include "SOMain.h"
#include "AuthProc.h"

#include "SOWorld.h"
#include "SOServer.h"
#include "SOPlayer.h"
#include "XPlayer.h"
#include "blackpig.h"

#include "..\\JWCommon\\JWSocket.h"
#include "..\\JWCommon\\JWRecvBuffer.h"
#include "..\\AuthServer\\_AuthDefine.h"


// CAuthServerProc
#include "../kojimodo/utility/Winsock.h"
#include "AuthServerProc.h"

#include <stdio.h>

unsigned __stdcall auth_worker( LPVOID param )
{

	cAuthProc *pClass = (cAuthProc *)param;

	CAuthServerProc		AuthServer;
	pClass->setAuthServerProc(&AuthServer);


	DWORD	dwRet;
	bool	bConnect = FALSE;
	DWORD	dwLastSendTime = 0;


	while( 1 ) {

		dwRet = WaitForSingleObject( pClass->m_hEvent, 20 );

		if( dwRet == WAIT_OBJECT_0 ) {

			ResetEvent( pClass->m_hEvent );

			if( pClass->m_siActionCode == AUTH_ACT_START ) {

				pClass->m_siState = AUTH_STAT_RUN;
			
			} else if( pClass->m_siActionCode == AUTH_ACT_STOP ) {

				break;
			}


		} else if( dwRet == WAIT_TIMEOUT && pClass->m_siState == AUTH_STAT_RUN ) {


			/* 허트 비트를 보낸다 */

			if( timeGetTime() - dwLastSendTime >= 30000 )
			{									
				AuthServer.ReportHeartbeat();
				dwLastSendTime = timeGetTime();							
			}



			/* 메인 처리를 수행 한다 */
			
			pClass->Run();						
		}
	}

	return 0;
}


cAuthProc::cAuthProc()
{
	m_siActionCode = 0;

	m_siState = AUTH_STAT_STOP;

	m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	UI32 threadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, auth_worker, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

}


cAuthProc::~cAuthProc()
{
	Stop();

	CloseHandle( m_hEvent );
}


// 인증 쓰레드 시작
void cAuthProc::Start()
{
	m_siActionCode = AUTH_ACT_START;
	SetEvent( m_hEvent );
}


// 인증 쓰레드 종료
void cAuthProc::Stop()
{
	m_siActionCode = AUTH_ACT_STOP;
	SetEvent( m_hEvent );
}


// 인증 처리
void cAuthProc::Run()
{
	int i;
	
	SOWorld *pWorld = m_pServer->clWorld;
	XPlayer *pXPlayer;

	char *pMsg;

	int	 count = 0;
	int  responseKind = LOGIN::UNKNOWN_FAIL;

	for( i = 0; i < ON_MAX_CONNECTION; ++i ) {

		if( pWorld->clPlayer[ i ]->m_mainState.match( USER_MUST_LOGOUT_IN_AUTH) == TRUE ) {
			// 로그아웃 요청 상태이면,

			pWorld->clPlayer[ i ]->m_mainState.set( USER_WAIT_FOR_LOGOUT );

			pWorld->clPlayer[ i ]->LogOut();

		} else if( pWorld->clPlayer[ i ]->m_mainState.match( USER_IN_AUTH ) == TRUE ) {

			// 메시지 처리..

			count++;

			pXPlayer = pWorld->clPlayer[ i ]->m_pXPlayer;

			// 패킷의 포인터 얻어옴
			pMsg = pXPlayer->GetSocket()->GetRecvBuffer()->GetFirstPacket();
			
			/////////////////////////////
			// 패킷 처리.. 
			// pMsg를 가지고 쓰기 장난치면 안됨 
			// 처리하는 중에도 새로운 패킷은 수신된다

			if( pMsg == NULL ) continue;

			CBlackPig encr;

			encr.SetKey( pXPlayer->m_cKey1, pXPlayer->m_cKey2 );

			if( encr.Decode( IN (unsigned char *)pMsg, OUT (unsigned char *)m_pmsg, pXPlayer->m_dwRoundKey ) == FALSE ) {
				pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();
				continue;
			}

			pMsg    =   m_pmsg;
			
			OnMsg		*pOnMsg;		
			pOnMsg	=	(OnMsg*)(pMsg + 3);

			switch(pOnMsg->dwMsgType)
			{				
				//============================================================================
				case LOGIN::REQUEST_LOGIN:
					responseKind = recvRequestLogin( pWorld->clPlayer[i], (LOGIN::sRequestLogin*)(pOnMsg) );
					break;					
				//============================================================================

			}

			//
			/////////////////////////////

			// 패킷 지움 
			pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

			if( responseKind == LOGIN::OK )
			{
				pWorld->clPlayer[i]->m_mainState.set( USER_IN_LOGIN );
				InterlockedIncrement( (long *)&pWorld->clGate->siTotalConnectPlayer );
				pWorld->clGate->siMaxConnectedPlayer	=	max( pWorld->clGate->siMaxConnectedPlayer, pWorld->clGate->siTotalConnectPlayer );

			}
		}

	}

}





//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 로그인 요청의 수신 처리를 수행 한다 
//--------------------------------------------------------------------------------------------
int cAuthProc::recvRequestLogin(SOPlayer* pPlayer, LOGIN::sRequestLogin* pData)
{

	//===============================================
	// AUTHENTICATION: 인증서버에 로그인을 요청 한다 
	//===============================================


	// 현재는 로그인을 요청후 응답을 바로 받기 때문에 유니크아이디를 쓰지않는다 	
	AUTH::sUniqueID		userUniqID;		
	userUniqID.set(0,0);

	char				responseKind;
	DWORD				dbAccount;

	// 최대 허용 동시 접속자수 체크
	SOWorld		*pWorld			=	m_pServer->clWorld;

	// 해당 유저의 아이피가 운영자 아이피이면 접속자수의 제한을 받지 않는다.	
	if( pWorld->clGate->siTotalConnectPlayer >= g_GameServerInfoParser.GetNumAllowedConcurrentUser() )
	{
		if( pWorld->m_clGMManager.IsGM( pPlayer->stAccountInfo.IP ) == FALSE )
		{
			responseKind	=	LOGIN::TOOMANYUSERS;
			sendResponseLogin( pPlayer , responseKind );

			return responseKind;
		}
	}

	if( !CheckLeadByte(pData->id) )
	{
		writeInfoToFile( "LeadByte_ID.txt", "ID(%s)  password(%s)", pData->id, pData->password );
		return LOGIN::UNKNOWN_FAIL;
	}

	if( !CheckLeadByte(pData->password) )
	{
		writeInfoToFile( "LeadByte_pass.txt", "ID(%s)  password(%s)", pData->id, pData->password );
		return LOGIN::UNKNOWN_FAIL;
	}

	// password 문자열의 (')를 ('')로 바꾼다.
	char szPassword[128];
	MakeDblQuote( pData->password, szPassword );
		
	if( m_pAuthServer->RequestLogin ( userUniqID, 
									  pData->id, 
									  szPassword, /*pData->password, */
									  pPlayer->stAccountInfo.IP, 
									  &responseKind, 
									  &dbAccount) )

	{

		if( responseKind == LOGIN::OK )
		{
						
			// 아이디, 패스워드, DB어카운트를 설정 한다
			
			memcpy( pPlayer->stAccountInfo.id,			pData->id,			LOGIN::ID_MAX_LENGTH );
			memcpy( pPlayer->stAccountInfo.password,	pData->password,	LOGIN::PW_MAX_LENGTH );

			
			pPlayer->stAccountInfo.siDBAccount = dbAccount;		
			


			// 로그인 완료를 요청 한다 
			 
			m_pAuthServer->RequestLoginEnd(userUniqID, dbAccount, &responseKind);
										
		}
	}


	// 로그인요청에 응답 한다 
	sendResponseLogin( pPlayer , responseKind );


	return responseKind;
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 로그인 요청에 응답하는 패킷을 보낸다 
//--------------------------------------------------------------------------------------------
void cAuthProc::sendResponseLogin(SOPlayer* pPlayer, int responseKind)
{
	LOGIN::sResponseLogin resPacket;

	resPacket.command		=	LOGIN::RESPONSE_LOGIN;
	resPacket.responseKind	=	responseKind;

	pPlayer->Write(	&resPacket, sizeof(resPacket) );

}

