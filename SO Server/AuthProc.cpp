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


			/* ��Ʈ ��Ʈ�� ������ */

			if( timeGetTime() - dwLastSendTime >= 30000 )
			{									
				AuthServer.ReportHeartbeat();
				dwLastSendTime = timeGetTime();							
			}



			/* ���� ó���� ���� �Ѵ� */
			
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


// ���� ������ ����
void cAuthProc::Start()
{
	m_siActionCode = AUTH_ACT_START;
	SetEvent( m_hEvent );
}


// ���� ������ ����
void cAuthProc::Stop()
{
	m_siActionCode = AUTH_ACT_STOP;
	SetEvent( m_hEvent );
}


// ���� ó��
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
			// �α׾ƿ� ��û �����̸�,

			pWorld->clPlayer[ i ]->m_mainState.set( USER_WAIT_FOR_LOGOUT );

			pWorld->clPlayer[ i ]->LogOut();

		} else if( pWorld->clPlayer[ i ]->m_mainState.match( USER_IN_AUTH ) == TRUE ) {

			// �޽��� ó��..

			count++;

			pXPlayer = pWorld->clPlayer[ i ]->m_pXPlayer;

			// ��Ŷ�� ������ ����
			pMsg = pXPlayer->GetSocket()->GetRecvBuffer()->GetFirstPacket();
			
			/////////////////////////////
			// ��Ŷ ó��.. 
			// pMsg�� ������ ���� �峭ġ�� �ȵ� 
			// ó���ϴ� �߿��� ���ο� ��Ŷ�� ���ŵȴ�

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

			// ��Ŷ ���� 
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
//	Desc : �α��� ��û�� ���� ó���� ���� �Ѵ� 
//--------------------------------------------------------------------------------------------
int cAuthProc::recvRequestLogin(SOPlayer* pPlayer, LOGIN::sRequestLogin* pData)
{

	//===============================================
	// AUTHENTICATION: ���������� �α����� ��û �Ѵ� 
	//===============================================


	// ����� �α����� ��û�� ������ �ٷ� �ޱ� ������ ����ũ���̵� �����ʴ´� 	
	AUTH::sUniqueID		userUniqID;		
	userUniqID.set(0,0);

	char				responseKind;
	DWORD				dbAccount;

	// �ִ� ��� ���� �����ڼ� üũ
	SOWorld		*pWorld			=	m_pServer->clWorld;

	// �ش� ������ �����ǰ� ��� �������̸� �����ڼ��� ������ ���� �ʴ´�.	
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

	// password ���ڿ��� (')�� ('')�� �ٲ۴�.
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
						
			// ���̵�, �н�����, DB��ī��Ʈ�� ���� �Ѵ�
			
			memcpy( pPlayer->stAccountInfo.id,			pData->id,			LOGIN::ID_MAX_LENGTH );
			memcpy( pPlayer->stAccountInfo.password,	pData->password,	LOGIN::PW_MAX_LENGTH );

			
			pPlayer->stAccountInfo.siDBAccount = dbAccount;		
			


			// �α��� �ϷḦ ��û �Ѵ� 
			 
			m_pAuthServer->RequestLoginEnd(userUniqID, dbAccount, &responseKind);
										
		}
	}


	// �α��ο�û�� ���� �Ѵ� 
	sendResponseLogin( pPlayer , responseKind );


	return responseKind;
	
}




//--------------------------------------------------------------------------------------------
//	Name : 
//	Desc : �α��� ��û�� �����ϴ� ��Ŷ�� ������ 
//--------------------------------------------------------------------------------------------
void cAuthProc::sendResponseLogin(SOPlayer* pPlayer, int responseKind)
{
	LOGIN::sResponseLogin resPacket;

	resPacket.command		=	LOGIN::RESPONSE_LOGIN;
	resPacket.responseKind	=	responseKind;

	pPlayer->Write(	&resPacket, sizeof(resPacket) );

}

