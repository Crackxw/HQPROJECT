#include "LoginProc.h"

#include "SOWorld.h"
#include "SOServer.h"
#include "SOPlayer.h"
#include "XPlayer.h"

#include "blackpig.h"

#include "..\\JWCommon\\JWSocket.h"
#include "..\\JWCommon\\JWRecvBuffer.h"


unsigned __stdcall login_worker( LPVOID param )
{

	cLoginProc *pClass = (cLoginProc *)param;

	DWORD dwRet;

	while( 1 ) {

		dwRet = WaitForSingleObject( pClass->m_hEvent, 20 );


		if( dwRet == WAIT_OBJECT_0 ) {

			ResetEvent( pClass->m_hEvent );

			if( pClass->m_siActionCode == LOGIN_ACT_START ) {

				pClass->m_siState = LOGIN_STAT_RUN;
			
			} else if( pClass->m_siActionCode == LOGIN_ACT_STOP ) {

				break;
			}


		} else if( dwRet == WAIT_TIMEOUT && pClass->m_siState == LOGIN_STAT_RUN ) {

			pClass->Run();
		}
	}

	return 0;
}



cLoginProc::cLoginProc()
{
	m_siActionCode = 0;

	m_siState = LOGIN_STAT_STOP;

	m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	UI32 threadID;

	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, login_worker, (LPVOID)this, 0, &threadID );
	CloseHandle( hThread );	

}


cLoginProc::~cLoginProc()
{
	Stop();

	CloseHandle( m_hEvent );
}


// 로그인 쓰레드 시작
void cLoginProc::Start()
{
	m_siActionCode = LOGIN_ACT_START;
	SetEvent( m_hEvent );
}

// 로그인 쓰레드 종료
void cLoginProc::Stop()
{
	m_siActionCode = LOGIN_ACT_STOP;
	SetEvent( m_hEvent );
}


// 로그인 처리
void cLoginProc::Run()
{

	BOOL bChangeState = FALSE;

	int i;

	SOWorld *pWorld = m_pServer->clWorld;
	XPlayer *pXPlayer;

	char *pMsg;

	for( i = 0; i < ON_MAX_CONNECTION; ++i ) {

		if( pWorld->clPlayer[ i ]->m_mainState.match( USER_MUST_LOGOUT_IN_LOGIN ) == TRUE ) {
			// 로그아웃 요청 상태이면,

			pWorld->clPlayer[ i ]->m_mainState.set( USER_WAIT_FOR_LOGOUT );

			InterlockedDecrement( (long *)&pWorld->clGate->siTotalConnectPlayer );

			pWorld->clPlayer[ i ]->LogOut();

		} else if( pWorld->clPlayer[ i ]->m_mainState.match( USER_IN_LOGIN ) == TRUE ) {

			// 메시지 처리..

			pXPlayer = pWorld->clPlayer[ i ]->m_pXPlayer;

			// 패킷의 포인터 얻어옴
			pMsg = pXPlayer->GetSocket()->GetRecvBuffer()->GetFirstPacket();

			if( pMsg == NULL ) continue;

			/////////////////////////////
			// 패킷 처리.. 
			// pMsg를 가지고 쓰기 장난치면 안됨 
			// 처리하는 중에도 새로운 패킷은 수신된다


			CBlackPig encr;

			encr.SetKey( pXPlayer->m_cKey1, pXPlayer->m_cKey2 );

			if( encr.Decode( IN (unsigned char *)pMsg, OUT (unsigned char *)m_pmsg, pXPlayer->m_dwRoundKey ) == FALSE ) {
				pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();
				continue;
			}

			pMsg    =   m_pmsg;


			OnMsg		*pOnMsg;		
			pOnMsg	=	(OnMsg*)(pMsg+3);

			switch(pOnMsg->dwMsgType)
			{
				case LOGIN::REQUEST_NEW_CHAR:
					{


						sDB_CharsInfo	charsinfo;

						charsinfo.dbAccount = pWorld->clPlayer[ i ]->stAccountInfo.siDBAccount;

						pWorld->clLoginDatabase->GetDBInfo3( SO_LOGIN_DB_CHARSINFO, &charsinfo, pWorld->clPlayer[ i ] );


						LOGIN::sRequestNewChar* pLogin = (LOGIN::sRequestNewChar*)(pMsg + 3);
						LOGIN::sResponseNewChar response;

						sDB_NewChar newchar;
						int			ret;
						int			slot;

						ret = 0;
						slot = pWorld->clPlayer[ i ]->getEmptySlot();
						
						if( slot > 0 ) {
							// 슬롯이 있으면, 

							newchar.dbAccount = pWorld->clPlayer[ i ]->stAccountInfo.siDBAccount;
							newchar.slot = slot;

							memcpy( newchar.name, pLogin->name, LOGIN::NAME_MAX_LENGTH );

							newchar.kind = pLogin->kind;
							newchar.Str = pLogin->Str;
							newchar.Dex = pLogin->Dex;
							newchar.Vit = pLogin->Vit;
							newchar.Int = pLogin->Int;

							// 능력치 이상 
							if( newchar.Str + newchar.Dex + newchar.Vit + newchar.Int > 10 ) ret = 0;
							else 
							if( pWorld->clLoginDatabase->GetDBInfo3( SO_LOGIN_DB_NEWCHAR, &newchar, &ret ) == FALSE ) {
								ret = 0;
							}
						}
						
						// 응답 
						response.command = LOGIN::RESPONSE_NEW_CHAR;
						
						if( ret == 1 ) {
							//성공이면, 
							pWorld->clPlayer[ i ]->setSlot( slot );

							response.responseKind = 1;
							response.newSlot = slot;
						
							pWorld->clPlayer[ i ]->Write( &response, sizeof( response ) );

						
						} else {
							//실패이면, -97 ( 특수문자 ) , -98 (중복된 이름)
							// 12월 30일 유재휘
							response.responseKind = ret; 
							response.newSlot = slot;

							pWorld->clPlayer[ i ]->Write( &response, sizeof( response ) );

						}
						
					}
					break;

				case LOGIN::REQUEST_DEL_CAHR:
					{

						sDB_CharsInfo	charsinfo;

						charsinfo.dbAccount = pWorld->clPlayer[ i ]->stAccountInfo.siDBAccount;

						pWorld->clLoginDatabase->GetDBInfo3( SO_LOGIN_DB_CHARSINFO, &charsinfo, pWorld->clPlayer[ i ] );
						
						
						LOGIN::sRequestDelChar* pLogin = (LOGIN::sRequestDelChar*)(pMsg + 3);
						LOGIN::sResponseDelChar response;

						sDB_DelChar delchar;
						int			ret;

						delchar.dbAccount =		pWorld->clPlayer[ i ]->stAccountInfo.siDBAccount;
						delchar.slot	  =		pLogin->delSlot;

						if( pWorld->clLoginDatabase->GetDBInfo3( SO_LOGIN_DB_DELCHAR, &delchar, &ret ) == FALSE ) {
							ret = 0;
						}

						response.command = LOGIN::RESPONSE_DEL_CHAR;

						// 응답
						if( ret > 0 && ret <= LOGIN::CHAR_MAX_NUM ) {
							// 성공이면,
							pWorld->clPlayer[ i ]->delSlot( ret );

							response.responseKind = ret;
							pWorld->clPlayer[ i ]->Write( &response, sizeof( response ) );

						} else {
							// 실패이면,

							response.responseKind = 0;
							pWorld->clPlayer[ i ]->Write( &response, sizeof( response ) );
						}


					}
					break;

				case LOGIN::REQUEST_CHARS_INFO:
					{
						LOGIN::sRequestCharsInfo* pLogin = (LOGIN::sRequestCharsInfo*)(pMsg + 3);

						sDB_CharsInfo	charsinfo;

						charsinfo.dbAccount = pWorld->clPlayer[ i ]->stAccountInfo.siDBAccount;

						pWorld->clLoginDatabase->GetDBInfo3( SO_LOGIN_DB_CHARSINFO, &charsinfo, pWorld->clPlayer[ i ] );

						pWorld->clPlayer[ i ]->sendCharsInfo();

					}
					break;

				case ON_REQUEST_LOGIN:
					{
						pWorld->clPlayer[ i ]->RecvLogIn( pMsg + 3 );

						bChangeState = TRUE;
					}
					break;
			}


			//
			/////////////////////////////

			// 패킷 지움 
			pXPlayer->GetSocket()->GetRecvBuffer()->RemoveFirstPacket();

			if( bChangeState ) {
				pWorld->clPlayer[ i ]->m_mainState.set( USER_IN_GAME );

			}

		}

	}

}
