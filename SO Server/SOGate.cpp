#include "SOMain.h"
#include "SOWorld.h"
#include "SOGate.h"
#include "ReadIP.h"
#include "XPlayer.h"
#include "XPlayerMgr.h"

//����������������������������������������������������������������������������
// SOGate()�� ������
//����������������������������������������������������������������������������
SOGate::SOGate(SOWorld *clTempWorld, SODatabase *clTempDatabase, SOServer *clTempServer)
{	
	printf("[%d]\n", FD_SETSIZE);

//	char	cHostName[40];
	SI32	iSocketStructSize;	

	// ���� �������� �� ����� ��
	siTotalConnectPlayer	=	0;
	siMaxConnectedPlayer	=	0;
	// �ϷĹ�ȣ �Ҵ�� ����.
	uiAccount				=	0;

	memset(clKillPlayer, 0, ON_MAX_CONNECTION * 4 );
	memset(clDisconnectPlayer, 0, ON_MAX_CONNECTION * 4 );

	// SOServer Ŭ������ �����͸� �����Ѵ�.
	clServer			= clTempServer;		
	
	// SOWorld Ŭ������ �����͸� �����Ѵ�.
	clWorld				= clTempWorld;			
	
	// SODatabase Ŭ������ �����͸� �����Ѵ�.
	clDatabase				= clTempDatabase;
	
	iSocketStructSize	= 0;
	// socket ����ü�� �ʱ�ȭ�Ѵ�.
//	memset(&stSocketStructA, 0, sizeof(stSocketStructA));

	// Internet Family�� �ʱ�ȭ�Ѵ�.
//	stSocketStructA.sin_family = AF_INET;

	// IP �ּҸ� ���´�.
//	gethostname(cHostName, 40);

	// Host�̸����κ��� IP�� ���´�.
//	pHostEnt = gethostbyname(cHostName);
		
	// ������ IP�� �����Ѵ�.
//	h1 = (*pHostEnt).h_addr_list[0][0];
//	h2 = (*pHostEnt).h_addr_list[0][1];
//	h3 = (*pHostEnt).h_addr_list[0][2];
//	h4 = (*pHostEnt).h_addr_list[0][3];

	// IP�� �ϳ��� ���ڿ��� �ٲ۴�.
//	sprintf(cServerIP, "%i.%i.%i.%i", h1, h2, h3, h4);

	// Socket ����ü�� IP�� �����Ѵ�.
//	stSocketStructA.sin_addr.s_addr = inet_addr(cServerIP);

	// Socket ����ü�� Port�� �����Ѵ�.
//	stSocketStructA.sin_port = htons(CONNECT_PORT);

	// Socket ũ�⸦ �����Ѵ�.
	iSocketStructSize	= sizeof(struct sockaddr_in);

}

//����������������������������������������������������������������������������
// SOGate�� �Ҹ���
//����������������������������������������������������������������������������
SOGate::~SOGate()
{
	// ������ ����....
	m_bShotdown = TRUE;

}
//����������������������������������������������������������������������������
// Master Socket Set�� �ʱ�ȭ�ϰ� MainSocket Descripter���� �����Ѵ�.
//����������������������������������������������������������������������������
VOID SOGate::ResetSocketSets()
{
	return;
}
//����������������������������������������������������������������������������
// ���Ӱ� ������ ���� ��� ���� ó��
//����������������������������������������������������������������������������
VOID SOGate::Poll(VOID)
{
	return;
}

//����������������������������������������������������������������������������
// �α� �ƿ� ����Ʈ�� ���
//����������������������������������������������������������������������������
VOID SOGate::LogOut(SOPlayer *clTempPlayer)
{
	return;
}
//����������������������������������������������������������������������������
// �α� �ƿ� ó��
//����������������������������������������������������������������������������
VOID SOGate::Disconnect(VOID)
{
	return;
}
 
void SOGate::InitUDP()
{
	char	LogInServerIP[20];
	SI32	TempPort = 6666;	

	// �α��� ������ IP�� ���´�.	
	
	LogInServerAddr.sin_family = AF_INET;
	// Socket ����ü�� IP�� �����Ѵ�.
	LogInServerAddr.sin_addr.s_addr = inet_addr(LogInServerIP);	
	// Socket ����ü�� Port�� �����Ѵ�.
	LogInServerAddr.sin_port = htons(TempPort);
	
	// MainSocketA�� socket�� �����Ѵ�.
	if((LogInSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		// MainSocket�� ������ �� ����.
		// Server Shutdown
		siErrCode = 0;
	}
	
}



void SOGate::SendLogInfo(UI32 Info)
{
	BYTE	SendBuf[4];
	UI16	uTemp1, uTemp2;
	
	uTemp1 = LOWORD(Info);
	uTemp2 = HIWORD(Info);
	SendBuf[0] = LOBYTE(uTemp1);
	SendBuf[1] = HIBYTE(uTemp1);
	SendBuf[2] = LOBYTE(uTemp2);
	SendBuf[3] = HIBYTE(uTemp2);

	::sendto(LogInSocket, (char*)SendBuf, 4, 0 , (sockaddr *)&LogInServerAddr, sizeof(sockaddr_in));
}

//����������������������������������������������������������������������������
// DB ĳ�� ������ Ŭ���̾�Ʈ�μ� �����Ѵ�. - DB ĳ�� ������ �̸� �� �־����.
// �ۼ��� : �̼���
//����������������������������������������������������������������������������
SI32 SOGate::ConnectDBCache()
{
	return 2;
}

//����������������������������������������������������������������������������
// Connect�� �޴� ������.
// �ۼ��� : �ѵ���
//����������������������������������������������������������������������������
void SOGate::ConnectPoll()
{

}