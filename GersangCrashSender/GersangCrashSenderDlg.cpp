// GersangCrashSenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GersangCrashSender.h"
#include "GersangCrashSenderDlg.h"
//#include ".\EmailLib\smtp.h"
//#include ".\EmailLib\MIMEMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderDlg dialog

CGersangCrashSenderDlg::CGersangCrashSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGersangCrashSenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGersangCrashSenderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGersangCrashSenderDlg::~CGersangCrashSenderDlg()
{
	 WSACleanup();
}

void CGersangCrashSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGersangCrashSenderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGersangCrashSenderDlg, CDialog)
	//{{AFX_MSG_MAP(CGersangCrashSenderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnSendOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGersangCrashSenderDlg message handlers

BOOL CGersangCrashSenderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	WORD wVersion;
	WSADATA wsaData;

	wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGersangCrashSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGersangCrashSenderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGersangCrashSenderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGersangCrashSenderDlg::OnSendOK() 
{
	// TODO: Add your control notification handler code here

	//*** Retreive all information
	char	szIP[20] = {0,};
	char	szBody[1024] = {0,};
	char	szSystemType[64] = {0,};
	char	szOSType[64] = {0,};
	char	szPacket[512] = {0,};
	SHORT	snPacketSize = 0;
	SHORT	snPos = 0;
	DWORD	dwMsgType = 0;
	BOOL	bResult = FALSE;

	////////////////////////////////////////////////////////
	MEMORYSTATUS MemStatus;
	MemStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStatus);
	////////////////////////////////////////////////////////
	OSVERSIONINFO	OSVersion;
	OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersion);
	strncpy(szOSType, GetOSType(OSVersion.dwPlatformId, OSVersion.dwMinorVersion), 64);
	////////////////////////////////////////////////////////
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	strncpy(szSystemType, GetProcessType(SystemInfo.dwProcessorType), 64);
	////////////////////////////////////////////////////////
	
	snPacketSize	= CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+sizeof(ON_REQ_CRASHINFO);
	memcpy(szPacket+snPos, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);
	dwMsgType		= ON_CRASH_MAIL_CLIENT;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);
	dwMsgType		= ON_REQ_CRASH_USER_INFO_SAVE;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);
	
	strncpy(m_CrashInfo.szProcessType, szSystemType, 64);
	strncpy(m_CrashInfo.szOSType, szOSType, 64);
	m_CrashInfo.nProcessLevel	= (SHORT)SystemInfo.wProcessorLevel;
	m_CrashInfo.nProcessNumber	= (SHORT)SystemInfo.dwNumberOfProcessors;
	m_CrashInfo.nPageSize		= (LONG)MemStatus.dwAvailPageFile;	

	SOCKET Socket = INVALID_SOCKET;

	if((Socket = Connect()) != INVALID_SOCKET)
	{
		GetHostIP(m_CrashInfo.szIP, INVALID_SOCKET);
		memcpy(szPacket+snPos, &m_CrashInfo, sizeof(ON_REQ_CRASHINFO));

//		if(IsRecvAble(Socket) == TRUE)
		{
			if(SendPacket(Socket, szPacket, snPacketSize) == TRUE)
				bResult = TRUE;
		}
	}

	if(bResult == FALSE)
		GetDlgItem(IDC_STATIC)->SetWindowText("전송실패");
	else
		GetDlgItem(IDC_STATIC)->SetWindowText("감사합니다. 에러메시지를 보냈습니다.");
	
	Sleep(1000);

	exit(1);
//	OnOK();
}


int CGersangCrashSenderDlg::Connect()
{
	SOCKET Socket = INVALID_SOCKET;
	Socket = socket(AF_INET, SOCK_STREAM, 0);

	if(Socket == INVALID_SOCKET) return INVALID_SOCKET;

	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = inet_addr(CRASH_MAIL_SERVER_IP);
	SocketAddr.sin_port = htons(CRASH_MAIL_USER_SERVER_PORT);

	int nErr = connect(Socket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr));

	if(nErr == SOCKET_ERROR)
	{
		closesocket(Socket);
		return INVALID_SOCKET;
	}
	return Socket;
}

BOOL CGersangCrashSenderDlg::SendPacket(const SOCKET Socket, 
										const char* pPacket, 
										int nSize)
{
	int nSendBytes	= 0;
	int nSendPos	= 0;
	int nRemainSize = nSize;
	int nSendTickCount = GetTickCount();

	while(1)
	{
		if(GetTickCount() - nSendTickCount > 2000)
			return FALSE;

		nSendBytes = ::send(Socket, (const char*)(pPacket+nSendPos), nRemainSize, 0);

		if(nSendBytes == SOCKET_ERROR)
		{
			if(GetLastError() != WSAEWOULDBLOCK)
			{
				return FALSE;
			}
		}

		if(nSendBytes < nRemainSize)
		{
			nSendPos += nSendBytes;
			nRemainSize -= nSendBytes;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGersangCrashSenderDlg::IsRecvAble(SOCKET Socket) const
{
	timeval	timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	

	DWORD nRecvTickCount = GetTickCount();

	while(1)
	{
		fd_set setfds, errfds;
		FD_ZERO(&setfds);
		FD_SET(Socket, &setfds);
		FD_ZERO(&errfds);
		FD_SET(Socket, &errfds);

		int nRetVal = select(Socket, &setfds, 0, &errfds, &timeout);		
	
		if(GetTickCount() - nRecvTickCount > 5000)
			return FALSE;

		if(nRetVal != SOCKET_ERROR)
		{
			if(FD_ISSET(Socket, &setfds) && !FD_ISSET(Socket, &errfds))
				return TRUE;
			else
				continue;
		}
		else
			return FALSE;
	}
	return FALSE;
}

void CGersangCrashSenderDlg::GetHostIP(char* IP, SOCKET Socket)
{
	if(Socket != INVALID_SOCKET)
	{
		sockaddr_in addr;
		memset(&addr,0,sizeof(sockaddr));
		//back ip address
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	

		memcpy( IP, inet_ntoa( addr.sin_addr ), 16 );
	}
	else
	{
		char				hostName[100];
		memset(hostName, 0, sizeof(hostName));

		struct hostent *	pHostEnt;						
		
		BYTE				h1, h2, h3, h4;				    
		
		if( gethostname(hostName,sizeof(hostName)) == 0 )
		{
			pHostEnt = gethostbyname(hostName);         
			
			h1=(*pHostEnt).h_addr_list[0][0];         
			h2=(*pHostEnt).h_addr_list[0][1];
			h3=(*pHostEnt).h_addr_list[0][2];
			h4=(*pHostEnt).h_addr_list[0][3];
			
			sprintf(IP,"%i.%i.%i.%i",h1,h2,h3,h4);
		}
	}
}

void CGersangCrashSenderDlg::SetCrashInfo( char* szStartTime, char* szLineAddress, char* szCrashInfo)
{
	m_strStartTime		= szStartTime;
	m_strCrashAddress	= szCrashInfo;
	m_strLineAddress	= szLineAddress;

	memset(&m_CrashInfo, 0, sizeof(ON_REQ_CRASHINFO));

	char szTemp[MAX_PATH*2] = {0,};

	char*	pBuf;
	int		nLen = 0;

	/////////////////////////////////////////////////////////////
	// 시작 시간
	pBuf = strtok(szCrashInfo, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nYear	= atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nMonth = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nDay = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nHour = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nMinute = atoi(pBuf);
	}
	
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.sTime.nSecond = atoi(pBuf);
	}

	/////////////////////////////////////////////////////////////
	// 종료 시간
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nYear	= atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nMonth = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nDay = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nHour = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nMinute = atoi(pBuf);
	}

	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.eTime.nSecond = atoi(pBuf);
	}

	/////////////////////////////////////////////////////////////
	// Crash Type
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.nCrashType = atoi(pBuf);
	}

	/////////////////////////////////////////////////////////////
	// Crash Address
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.nCrashAddress = atoi(pBuf);
	}

	/////////////////////////////////////////////////////////////
	// Line Address
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		m_CrashInfo.nLineAddress = atoi(pBuf);
	}

	/////////////////////////////////////////////////////////////
	// Nation Code
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		strncpy(m_CrashInfo.szNationCode, pBuf, 64);
	}

	/////////////////////////////////////////////////////////////
	// Supposition
	nLen += strlen(pBuf)+1;
	pBuf = strtok(szCrashInfo+nLen, " ");
	if(pBuf != NULL)
	{
		strncpy(m_CrashInfo.szSupposition, pBuf, 256);			
	}
	/////////////////////////////////////////////////////////////

	m_bGetParam			= true;
}

const char* CGersangCrashSenderDlg::GetProcessType(DWORD dwType)
{
	switch(dwType)
	{	
		case ON_PROCESSOR_INTEL_386 :
			return "ON_PROCESSOR_INTEL_386";
		case ON_PROCESSOR_INTEL_486 :
			return "ON_PROCESSOR_INTEL_486";
		case ON_PROCESSOR_INTEL_PENTIUM :
			return "ON_PROCESSOR_INTEL_PENTIUM";
		case ON_PROCESSOR_MIPS_R4000 :
			return "ON_PROCESSOR_MIPS_R4000";
		case ON_PROCESSOR_ALPHA_21064 :
			return "ON_PROCESSOR_ALPHA_21064";
		case ON_PROCESSOR_PPC_601 :
			return "ON_PROCESSOR_PPC_601";
		case ON_PROON_CESSOR_PPC_603 :
			return "ON_PROON_CESSOR_PPC_603";
		case ON_PROCESSOR_PPC_604 :
			return "ON_PROCESSOR_PPC_604";
		case ON_PROCESSOR_PPC_620 :
			return "ON_PROCESSOR_PPC_620";
		case ON_PROCESSOR_HITACHI_SH3 :
			return "ON_PROCESSOR_HITACHI_SH3";
		case ON_PROCESSOR_HITACHI_SH3E :
			return "ON_PROCESSOR_HITACHI_SH3E";
		case ON_PROCESSOR_HITACHI_SH4 :
			return "ON_PROCESSOR_HITACHI_SH4";
		case ON_PROCESSOR_MOTOROLA_821 :
			return "ON_PROCESSOR_MOTOROLA_821";
		case ON_PROCESSOR_SHx_SH3 :
			return "ON_PROCESSOR_SHx_SH3";
		case ON_PROCESSOR_SHx_SH4 :
			return "ON_PROCESSOR_SHx_SH4";
		case ON_PROCESSOR_STRONGARM :
			return "ON_PROCESSOR_STRONGARM";
		case ON_PROCESSOR_ARM720 :
			return "ON_PROCESSOR_ARM720";
		case ON_PROCESSOR_ARM820 :
			return "ON_PROCESSOR_ARM820";
		case ON_PROCESSOR_ARM920 :
			return "ON_PROCESSOR_ARM920";
		case ON_PROCESSOR_ARM_7TDMI :
			return "ON_PROCESSOR_ARM_7TDMI";
		default : 
			return "UNKNOWN TYPE";
	}
}


const char* CGersangCrashSenderDlg::GetOSType(DWORD dwPlatformId, DWORD dwMinorVersion)
{
	switch(dwPlatformId)
	{	
		case VER_PLATFORM_WIN32s :
			return "Windows 3.1 OS";
		case VER_PLATFORM_WIN32_WINDOWS :
			{
				if(dwMinorVersion == 0)
				{
					return "Windows 95 OS";
				}
				else
				{
					return "Windows 98 OS";
				}				
			}
			break;
		case VER_PLATFORM_WIN32_NT :
			return "Windows NT or 2000 or XP OS";
		default : 
			return "UNKNOWN";
	}
} 



























/*
	CString serverAdr;
	CString from;
	CString to;
	CString subject;
	CString body;
	char	szIP[20] = {0,};
	char	szBody[1024] = {0,};
	char	szSystemType[128] = {0,};

	serverAdr = "mail.joyon.com";
	from = "gersanguser@joyon.com";
	to = "gersangcrash@joyon.com";


	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);	

	strcpy(szSystemType, GetProcessType(SystemInfo.dwProcessorType));
	GetHostIP(szIP);

	subject.Format( "[GersangCrash] [%s]", m_strVersion);
	sprintf(szBody, "%s\n1. IP : %s\n\n2. Process Type : %s\n3. Number of processors : %d\n4. Process Level : %d\n5. Page Size : %d", 
		m_strInfo.GetBuffer(0), szIP, szSystemType, SystemInfo.dwNumberOfProcessors, 
		SystemInfo.wProcessorLevel,	SystemInfo.dwPageSize);
	body = szBody;


	//*** Send
	CMIMEMessage msg;
	CSMTP smtp( serverAdr );

	msg.m_sFrom    = from;
	msg.m_sSubject = subject;
	msg.m_sBody    = body;
	msg.AddMultipleRecipients( to );

	smtp.Connect("gersangcrash.joyon.com");

	if (!smtp.SendMessage( &msg ))
	{
		CString strError;
		strError.Format( "전송실패[%s]", smtp.GetLastError() );
		GetDlgItem(IDC_STATIC)->SetWindowText(strError);
	}
	else
	{
		GetDlgItem(IDC_STATIC)->SetWindowText("감사합니다. 에러메시지를 보냈습니다.");
	}

	smtp.Disconnect();

	Sleep( 1000 );

	OnOK();
*/
