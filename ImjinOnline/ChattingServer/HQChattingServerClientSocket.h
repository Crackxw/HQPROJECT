/************************************************************************************************
	���ϸ� : HQChattingServerClientSocket.h
	����� : ������
************************************************************************************************/
#ifndef _HQCLIENTSOCKET_H		
#define _HQCLIENTSOCKET_H		
					

#define WSAEVENT                HANDLE


#include "HQChattingServerPacketCMD.h"
#include "HQChattingServerPacket.h"


class HQClientSocket
{
public:
	HQClientSocket();
	virtual ~HQClientSocket();

public:
	void    Create( UI32 uiRecvBufSize, UI32 uiSendBufSize );
	BOOL    Connect( const char *pszIP, UI16 usPort );

	SOCKET  GetSocket(){ return m_socket; };	
	BOOL    IsConnect(){ return m_bConnect; };	

	// ���� �ʱ�ȭ
	void    ClearSendBuffer();
	void    ClearRecvBuffer();
	
	BOOL    Recv();	
	BOOL    Send();	

	BOOL    WritePacket( HQPacket *pPacket );
	BOOL    WritePacket( char *pPacket, int len );

	void    Close();			
	BOOL    Reconnect();			

public:
	SOCKET        m_socket;	
	SOCKADDR_IN   m_sockaddr;

	BOOL          m_bConnect;		

	char          *m_pSendBuf;
	char          *m_pRecvBuf;

	char		  IPAddress[30];
	UI16		  Port;
	UI16		  m_GMGrade;

private:
	UI32          m_uiRecvBufSize;
	UI32          m_uiSendBufSize;

	// Recv �Լ� ����..
	BOOL		  m_bRecvDataSize;
	UI32		  m_uiRecvBytes;
	UI16		  m_usRecvBytesToRead;

	// Send �Լ� ����..
	int			  m_startPtr;						// ������ ���� ����
	int			  m_dataLen;						// �������� ����

	WSAEVENT		m_Event;

public:
	char			m_LogInID[32];
	DWORD			m_LogInDBAccount;

	UI16			m_fIsMap;						// 0 : ó����, 1 : �ʿ� ����, 2 : �ʿ� �������� ����
	UI16			m_NowMapMapIndex;
	UI16			m_NowMapXPos;
	UI16			m_NowMapYPos;

	DWORD			m_LastTickCount;
	DWORD			m_ReconnectTickCount;			// ä�ü����� �׾ �翬��� �����ð� ��������
													// �������� �õ��Ѵ�.

	BOOL			m_fLogInSucess;
	BOOL			m_ReconnectFlag;

public:
	BOOL			MessageSend_LogIn(char* lpID, DWORD DBAccount, UI16 uiGrade);

	BOOL			MessageSend_SendChatting(char* lpChatting);
	BOOL			MessageSend_SendWhisper(char* lpID, char* lpChatting);
	BOOL			MessageSend_MoveSell(UI16 uiMapCode, UI16 uiXPos, UI16 uiYPos);
	BOOL			MessageSend_NotIsMap(void);

	BOOL			HeartBeat(void);
	BOOL			DataProc(void);
};


#endif


