#ifndef SOGATE_H
#define SOGATE_H
//����������������������������������������������������������������������������
#include "SOCommonHeader.h"
#include "SOUtility.h"
#include "SOServer.h"
#include "SODataBase.h"
#include "SOPlayer.h"

class SOServer;

class SOGate : virtual SOUtility
{
//����������������������������������������������������������������������������
// ���� ����
//����������������������������������������������������������������������������
public:
	// SOGate�� ������
	SOGate(SOWorld *, SODatabase *, SOServer *);

	// SOGate�� �Ҹ���
	~SOGate();

	// Socket ó���� Polling �Լ�
	VOID		Poll(VOID);			
private:
	// Master Socket Set�� �ʱ�ȭ�ϰ� MainSocket Descripter���� �����Ѵ�.
	VOID		ResetSocketSets(VOID);
public:
	SOWorld		*clWorld;
	SOServer	*clServer;
	SODatabase	*clDatabase;

	// Server�� ù��° IP����
	//char		cServerIP[16];
	// Server IP�� �� �κк� ���
	UI08		h1, h2, h3, h4;
public:
	// Main Socket ������ �̿�� Socket Discripter
	SI32		siMainSocketA;

	// binding�� ���ο� �����ڵ��� ���� IP�� �����µ� ���Ǵ� ����ü
//	struct		sockaddr_in stSocketStructA;
	struct		hostent *pHostEnt;

	// �α��� ������ UDP�����ϱ� ���� 
	struct		sockaddr_in LogInServerAddr;
	SOCKET		LogInSocket;

	//	DB cache ������ �����ϱ� ����	 
	struct		sockaddr_in DBAddr;
	SOCKET		DBSocket;

	// ���ο� ������ �˻��ϴµ� ���Ǵ� Set
	fd_set		Master;

	// �б� ���⿡ ���Ǵ� Set
	fd_set		AllSockets;

	// ���� ���⿡ ���Ǵ� Set
	fd_set		ErrSockets;

	// select()���� ��ٸ� time out �ð�
	timeval		timeout;

	// �˴ٿ� ����
	SI32		siErrCode;
//����������������������������������������������������������������������������
// ���� / ���� ����
//����������������������������������������������������������������������������
public:
	// �������ִ� �� ����� ��
	SI32		siTotalConnectPlayer;

	// �ִ� ���� �� ����� ��
	SI32		siMaxConnectedPlayer;

	// �ϷĹ�ȣ �Ҵ�� ����.
	UI16		uiAccount;

	// ���� �����ϴ� �÷��̾� Ŭ����
	SOPlayer	*clDisconnectPlayer[ON_MAX_CONNECTION];
	SOPlayer	*clKillPlayer[ON_MAX_CONNECTION];

public:
	void		SendLogInfo(UI32 Info);
	void		InitUDP();
	// ���� ������ ó���Ѵ�.
	VOID		Disconnect();
	// �α� �ƿ��Ѵ�.
	VOID		LogOut(SOPlayer *clTempPlayer);

	// DB Cache���� ����
	SI32		ConnectDBCache();

	// Connect ������ ����...
	CRITICAL_SECTION	m_csGate;
	BOOL				m_bShotdown;
	VOID				ConnectPoll();
	HANDLE				m_hConnect;
	inline static unsigned __stdcall ConnectThread(void* pArg)
	{
		printf("Connection Thread Start!!!\n");

		SOGate* pGate = NULL;
		pGate = static_cast<SOGate *>(pArg);
		pGate->ConnectPoll();

		return 0;
	}
};
//����������������������������������������������������������������������������
#endif