#ifndef _JWCONNECT_H
#define _JWCONNECT_H

#define JWCONNECT_ACT_ATTEMPT			1			// attempt to connect
#define JWCONNECT_ACT_STOP				2			// stop to attempt
#define JWCONNECT_ACT_CLOSE				3
#define JWCONNECT_ACT_EXIT				4
#define JWCONNECT_ACT_SENDIM			5

#define JWCONNECT_STATE_ATTEMPTING		100
#define JWCONNECT_STATE_ESTABLISHED		101
#define JWCONNECT_STATE_CLOSED			102

#include "JWSocket.h"

class JWConnect : public JWSocket
{
	friend unsigned __stdcall connect_send( LPVOID param );
	friend unsigned __stdcall connect_run( LPVOID param );

public:
	JWConnect();
	virtual ~JWConnect();

	void				InitConnect( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize );
	void				Connect();
	void				Stop();
	void				Close();


private:
	BOOL				process_state_attempting();
	BOOL				process_state_established();
	BOOL				process_state_closed();

private:

	HANDLE				m_hEvents[ 3 ];			// 0¹ø ÀÎµ¦½º´Â Action Event

	UI32				m_uiActionCode;

	HANDLE				m_hEventSend;
	UI32				m_uiActionCodeSend;

	UI32				m_uiState;

	BOOL				m_bInitOk;

	char				m_szIP[ 16 ];
	UI16				m_usPort;

	UI32				m_uiSendInterval;
	UI32				m_uiAttemptInterval;
};

#endif
