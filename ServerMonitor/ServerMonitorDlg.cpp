// ServerMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMonitor.h"
#include "ServerMonitorDlg.h"
#include "GersangServerMonitor.h"
#include "GersangServerStatus.h"
#include "MyPacket.h"
#include "MonitorProtocol.h"
#include "SelectDirectoryDialog.h"
#include "LoginDialog.h"
#include <mmsystem.h>
#include <windowsx.h>
#include <commctrl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char g_SavePath[256];


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
// CServerMonitorDlg dialog

CServerMonitorDlg::CServerMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerMonitorDlg)
	m_MasterIpString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerMonitorDlg)
	DDX_Control(pDX, IDC_KICKTIMER, m_KickTimerEdit);
	DDX_Control(pDX, IDC_FILETRANSFRERNAMESTATIC, m_FileTransferName);
	DDX_Control(pDX, IDC_FILETRANSFERBYTESTATIC, m_FileTransferBytes);
	DDX_Control(pDX, IDC_INTERVALEDIT, m_IntervalEdit);
	DDX_Control(pDX, IDC_AUTONOTICECONTENT, m_AutoNoticeContent);
	DDX_Control(pDX, IDC_AUTONOTICE, m_AutoNoticeButton);
	DDX_Control(pDX, IDC_SOUNDCHECK1, m_SoundButton);
	DDX_Control(pDX, IDC_IPCOMBO, m_MasterIpCombo);
	DDX_Control(pDX, IDC_MAXUSERNUM, m_MaxUserStatic);
	DDX_Control(pDX, IDC_SERVERLISTCONTROL2, m_ServerListControl2);
	DDX_Control(pDX, IDC_TAB1, m_ListTab);
	DDX_Control(pDX, IDC_RICHEDIT1, m_ConsoleResultEdit);
	DDX_Control(pDX, IDC_SERVERLISTCONTROL, m_ServerListControl);
	DDX_Control(pDX, IDC_SERVERINFO_EDIT, m_ServerInfoEdit);
	DDX_Control(pDX, IDC_CONSOLE_EDIT, m_ConsoleEdit);
	DDX_CBString(pDX, IDC_IPCOMBO, m_MasterIpString);
	DDV_MaxChars(pDX, m_MasterIpString, 256);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CServerMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_NOTIFY(NM_CLICK, IDC_SERVERLISTCONTROL, OnClickServerlistcontrol)
	ON_BN_CLICKED(IDC_CONNECTBUTTON, OnConnectbutton)
	ON_BN_CLICKED(IDC_EXITBUTTON, OnExitbutton)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_FILETRANSFERBUTTON, OnFiletransferbutton)
	ON_NOTIFY(NM_RCLICK, IDC_SERVERLISTCONTROL2, OnRclickServerlistcontrol2)
	ON_BN_CLICKED(IDC_SERVEROFFBUTTON, OnServeroffbutton)
	ON_BN_CLICKED(IDC_SERVERONBUTTON, OnServeronbutton)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_SERVERLISTCONTROL, OnGetdispinfoServerlistcontrol)
	ON_CBN_KILLFOCUS(IDC_IPCOMBO, OnKillfocusIpcombo)
	ON_EN_KILLFOCUS(IDC_AUTONOTICECONTENT, OnKillfocusAutonoticecontent)
	ON_EN_KILLFOCUS(IDC_INTERVALEDIT, OnKillfocusIntervaledit)
	ON_BN_CLICKED(IDC_NOTICESENDBUTTON2, OnNoticesendbutton2)
	ON_NOTIFY(NM_RCLICK, IDC_SERVERLISTCONTROL, OnRclickServerlistcontrol)
	ON_BN_CLICKED(IDC_KICKBUTTON, OnKickbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorDlg message handlers

// 서버의 상태를 판단 하여 알려야할 상태면 TRUE 아니면 FALSE를 리턴
static BOOL DoAlarm(int servergroup,int servernum)
{
	BOOL ret = FALSE;
	DWORD ServerState = g_GersangServerMonitor.GetServerState(servergroup,servernum);
	switch(ServerState)
	{
		
	case SERVER_STATE_DEATH:		
	case SERVER_STATE_TIMEOUT:
		ret = TRUE;
		break;
		break;
	}

	
	
	// 게임서버이면
	// 유저수가 0명이당!
	if (servernum == 0) {
		if (g_GersangServerMonitor.m_pServer[servergroup].GetUserNum() == 0)
		{
			ret = TRUE;
			
		}
	}
/*
	// 유저수가 오랫동안 변하지 않았다!!!
	if (g_GersangServerMonitor.m_pServer[servergroup].GetSameUserCount() >= 10)
	{
		ret = TRUE;

	}
*/
	if (g_GersangServerMonitor.m_pServer[servergroup].GetMute())
	{
		ret = FALSE;
	}


	return ret;
		

}


static COLORREF TranslateServerStateToColor(int ServerState)
{
	COLORREF ret;
	switch(ServerState)
	{
	case SERVER_STATE_NOTCONNECTED:
		ret = RGB(0,0,0);
		break;
	case SERVER_STATE_CONNECTING:
		ret = RGB(0,0,0);
		break;
	case SERVER_STATE_LIVE:
		ret = RGB(0,0,240);
		break;
	case SERVER_STATE_DEATH:
		ret = RGB(255,0,0);
		break;
	case SERVER_STATE_TIMEOUT:
		ret = RGB(255,0,0);
		break;
	default:
		ret = RGB(255,0,255);
		break;
	}
	return ret;
		

}

static TCHAR *TranslateServerStateToString(int ServerState)
{
	TCHAR *ret;

	static TCHAR hanbuf[256];
	static BOOL bInit = FALSE;
	if (!bInit) {
		LoadString(::AfxGetInstanceHandle(),IDS_STRING104,hanbuf,256);
		bInit = TRUE;
	}

	
	switch(ServerState)
	{
	case SERVER_STATE_NOTCONNECTED:
		ret = _T("--disconnected--");
		break;
	case SERVER_STATE_CONNECTING:
		ret = _T("[ connecting ]");
		break;
	case SERVER_STATE_LIVE:
		ret = _T("[ ok ][ good ]");
		break;
	case SERVER_STATE_DEATH:
		ret = _T("[ dead ]");
		break;
	case SERVER_STATE_TIMEOUT:
		ret = _T("[ ok ][ no heartbeat ]");
		break;
	default:
		ret = _T("--unknown--");
		break;
	}
	return ret;
		

}

BOOL CServerMonitorDlg::OnInitDialog()
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

	
	

/*	CFont *pFont = new CFont;
	pFont->CreateFont(0,10,0,0,20,FALSE,FALSE,FALSE,HANGUL_CHARSET,0,0,0,0,_T("궁서체"));

	SetFont(pFont,TRUE);

	m_ServerListControl.SetFont(pFont,TRUE);
	m_ServerListControl2.SetFont(pFont,TRUE);
	m_IntervalEdit.SetFont(pFont,TRUE);
	m_ConsoleEdit.SetFont(pFont,TRUE);
	m_AutoNoticeContent.SetFont(pFont,TRUE);*/

	m_ConsoleResultEdit.SetOptions(ECOOP_OR, ECO_AUTOHSCROLL);

	

	m_IntervalEdit.SetLimitText(6);
	m_IntervalEdit.SetWindowText(_T("0"));

	m_AutoNoticeContent.SetLimitText(100);
	

	m_ConsoleEdit.SetLimitText(80);
	

	m_SoundButton.SetCheck(TRUE);
	m_hPopupMenu = NULL;

	m_siSetSel = 0;
	m_bFlash = FALSE;

	m_ListTab.InsertItem(0,_T("groupview"));
	m_ListTab.InsertItem(1,_T("serverview"));
//	m_ListTab.InsertItem(2,_T("Sms"));

	m_siCurrentListMode = 0;
	m_ServerListControl.ShowWindow(TRUE);
	m_ServerListControl2.ShowWindow(FALSE);

		

	ListView_SetExtendedListViewStyle(m_ServerListControl.m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	ListView_SetExtendedListViewStyle(m_ServerListControl2.m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

//	InitializeServerList();

	SetTimer(2,128,NULL);
	SetTimer(1,1000,NULL);
	SetTimer(4,500,NULL);
//	SetTimer(5,5000,NULL);

	FILE *fp;
	TCHAR buffer[256];
	
	ZeroMemory(buffer,sizeof(buffer));
	fp = _tfopen(_T("rcip.txt"),_T("rt"));
	if (fp != NULL)
	{
		while (_fgetts(buffer,256,fp) != NULL)
		{
			buffer[_tcslen(buffer) - 1] = 0;
			m_MasterIpCombo.AddString(buffer);			
		}
		fclose(fp);
		m_MasterIpCombo.SetCurSel(0);
	}

	

	

	





	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == SC_CLOSE) 
	    return;
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerMonitorDlg::OnPaint() 
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
HCURSOR CServerMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CServerMonitorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CServerMonitorDlg::OnOK() 
{
	// TODO: Add extra validation here
	return;
	
	CDialog::OnOK();
}

int CServerMonitorDlg::InitializeServerList()
{

	LVITEM li;
	ZeroMemory(&li,sizeof(li));
	int ret;

	ClearServerList();

	


	m_ServerListControl.InsertColumn(0,_T("Name"),LVCFMT_LEFT,70);


	
	m_hPopupMenu = CreatePopupMenu();

	
	for (int c = 0; c < g_GersangServerMonitor.GetServerPerGroup(); c++)
	{
		m_ServerListControl.InsertColumn(1 + c,g_GersangServerMonitor.GetServerUsage(c),LVCFMT_LEFT,140);

		

		if (!InsertMenu(m_hPopupMenu,10000 + c,MF_STRING | MF_BYCOMMAND,10000 + c,g_GersangServerMonitor.GetServerUsage(c)))
		{
			
		}
		
		
	}
	m_ServerListControl.InsertColumn(1 + c,_T("User"),LVCFMT_LEFT,70);
	m_ServerListControl.InsertColumn(2 + c,_T("diff"),LVCFMT_LEFT,70);
	m_ServerListControl.InsertColumn(3 + c,_T("kickcount"),LVCFMT_LEFT,70);
	m_ServerListControl.InsertColumn(4 + c,_T("serverqueue"),LVCFMT_LEFT,140);


	m_ServerListControl2.InsertColumn(0,_T("Name : kind"),LVCFMT_LEFT,150);	
	m_ServerListControl2.InsertColumn(1,_T("status"),LVCFMT_LEFT,140);
	m_ServerListControl2.InsertColumn(2,_T("filetransfer"),LVCFMT_LEFT,254);

	
	for (int a = 0; a < g_GersangServerMonitor.GetServerGroupNum(); a++) {
		m_ServerListControl.InsertItem(a,g_GersangServerMonitor.GetServerName(a),0);

		li.iItem = a;
		li.lParam = a;
		li.mask = LVIF_TEXT;// | LVIF_PARAM;
		for (int b = 0; b < CGersangServerStatus::GetServerNum(); b++) {
			ret = g_GersangServerMonitor.GetServerState(a,b);
			li.pszText = TranslateServerStateToString(ret);
			li.cchTextMax = _tcslen(li.pszText);

			li.iSubItem = b + 1;

			//m_ServerListControl.SetItemText(a,b + 1,li.pszText);
			int value = m_ServerListControl.SetItem(&li);

			m_ServerListControl.SetItemData(a,a);

			

			value = value;
		}

		
		

		// 접속자수 (일단 0)
		m_ServerListControl.SetItemText(a,b + 1,_T("0"));

		// 접속자수 차이
		m_ServerListControl.SetItemText(a,b + 2,_T("0"));

		// 서버다운 카운트.
		m_ServerListControl.SetItemText(a,b + 3,_T("0"));

		// 큐
		m_ServerListControl.SetItemText(a,b + 4,_T("0"));
		
	}

	
	for ( a = 0; a < g_GersangServerMonitor.GetServerGroupNum(); a++) {
		

		for (int b = 0; b < CGersangServerStatus::GetServerNum(); b++) {
			TCHAR szTemp[256];
			_stprintf(szTemp,_T("%s : %s"),g_GersangServerMonitor.GetServerName(a),g_GersangServerMonitor.GetServerUsage(b));
			
			m_ServerListControl2.InsertItem(a * CGersangServerStatus::GetServerNum() + b,szTemp,0);
			ret = g_GersangServerMonitor.GetServerState(a,b);
			li.pszText = TranslateServerStateToString(ret);
			m_ServerListControl2.SetItemText(a * CGersangServerStatus::GetServerNum() + b,1,li.pszText);
		}
		
	}

	
	CHeaderCtrl *phCtrl = m_ServerListControl.GetHeaderCtrl();
	phCtrl->ModifyStyle(HDS_DRAGDROP | HDS_FULLDRAG,0,0);

	phCtrl = m_ServerListControl2.GetHeaderCtrl();
	
	phCtrl->ModifyStyle(HDS_DRAGDROP | HDS_FULLDRAG,0,0);


	TCHAR str[256];

	_stprintf(str,_T("Current connected user : %d"),0);

	m_MaxUserStatic.SetWindowText(str);

	
	

		
	


	return 1;

}

// 이함수는 1초당 한번씩 호출된다.

void CServerMonitorDlg::RefreshServerList()
{
	LVITEM li;
	int ret;
	int currentuser = 0;
	static maxuser = 0;
	int alarm = 0;
	TCHAR szFileState[1024];

	for (int a = 0; a < g_GersangServerMonitor.GetServerGroupNum(); a++) {
	    g_GersangServerMonitor.m_pServer[a].Process();

		li.iItem = a;
		li.mask = LVIF_TEXT | LVIF_PARAM;

		// 각 서버 상태표시.
		for (int b = 0; b < CGersangServerStatus::GetServerNum(); b++) {
			ret = g_GersangServerMonitor.GetServerState(a,b);
			if (b == 0 && DoAlarm(a,b))
			{
				alarm = 1;
			}
			li.pszText = TranslateServerStateToString(ret);
			li.cchTextMax = _tcslen(li.pszText);
			li.iSubItem = b;
			
			m_ServerListControl.SetItemText(a,b + 1,li.pszText);
		}
		//
		int usernum = g_GersangServerMonitor.m_pServer[a].GetUserNum();

		// 전체유저수에 더해줌
		currentuser += usernum;

		// 리스트 컨트롤에 출력
		_stprintf(szFileState,_T("%d"), usernum);
		m_ServerListControl.SetItemText(a,b + 1,szFileState);

		// 변화량을 얻어와서		
		int differance = g_GersangServerMonitor.m_pServer[a].GetDifferance();
		int samecount = g_GersangServerMonitor.m_pServer[a].GetSameUserCount();

		// 리스트 컨트롤에 출력.
		_stprintf(szFileState,_T("%d(%d)"), differance,samecount);
		m_ServerListControl.SetItemText(a,b + 2,szFileState);

		// 서버다운되기 몇초전인가를 얻어옴
		int kickcount = g_GersangServerMonitor.m_pServer[a].GetKickCount();

		// 리스트 컨트롤에 출력.
		if (kickcount < 0) {
			_stprintf(szFileState,_T("None"));
		}
		else
		{
			_stprintf(szFileState,_T("%d"),kickcount);

		}

		m_ServerListControl.SetItemText(a,b + 3,szFileState);

		_stprintf(szFileState,_T("%ld / %ld"),g_GersangServerMonitor.m_pServer[a].m_siGateQueue,g_GersangServerMonitor.m_pServer[a].m_siSubQueue);

		m_ServerListControl.SetItemText(a,b + 4,szFileState);




		
		

		
	}

	for ( a = 0; a < g_GersangServerMonitor.GetServerGroupNum(); a++) {
		

		for (int b = 0; b < CGersangServerStatus::GetServerNum(); b++) {
			
			
			ret = g_GersangServerMonitor.GetServerState(a,b);
			
			li.pszText = TranslateServerStateToString(ret);
			m_ServerListControl2.SetItemText(a * CGersangServerStatus::GetServerNum() + b,1,li.pszText);

/*			if (g_GersangServerMonitor.m_pServer[a].m_pServerSockets[b].GetFileTransferStatus(szFileState))
			{
				m_ServerListControl2.SetItemText(a * CGersangServerStatus::GetServerNum() + b,2,szFileState);

			}
			else
			{
				m_ServerListControl2.SetItemText(a * CGersangServerStatus::GetServerNum() + b,2,"");

			}*/
		}
		
	}


	if (m_siCurrentListMode == 0) {
		int iSelected = m_ServerListControl.GetNextItem(-1,LVNI_SELECTED);	
		if (iSelected != -1)  {
			TCHAR szBuf[1024];
			
			ZeroMemory(szBuf,sizeof(szBuf));
			
			g_GersangServerMonitor.GetServerInfo(iSelected,szBuf);
			
			m_ServerInfoEdit.SetWindowText(szBuf);
			
			
		}
	}

	if (maxuser < currentuser)
	{
		maxuser = currentuser;

	}
	_stprintf(szFileState,_T("Current connected user : %d/%ld"),currentuser,maxuser);

	m_MaxUserStatic.SetWindowText(szFileState);
	if (alarm && m_SoundButton.GetCheck())
	{
		PlaySound(_T("ManagerWarning.wav"),NULL,SND_ASYNC | SND_FILENAME);
	}

	

	if (g_GersangServerMonitor.m_bFileTransfer) {
		_stprintf(szFileState,_T("File Name : %s"),g_GersangServerMonitor.m_szTransferFileNameNoPath);
		
		m_FileTransferName.SetWindowText(szFileState);
		
		
		_stprintf(szFileState,_T("File Sended Byte : %ld / %ld"),g_GersangServerMonitor.m_siSendFileByte,g_GersangServerMonitor.m_siSendFileByteTotal);
		
		m_FileTransferBytes.SetWindowText(szFileState);
	}
	else
	{
		_stprintf(szFileState,_T("<No file transfer>"));
		
		m_FileTransferName.SetWindowText(szFileState);
		
		
		_stprintf(szFileState,_T("File Sended Byte : 0 / 0"));
		
		m_FileTransferBytes.SetWindowText(szFileState);
	
		
	
	}

	if (alarm)
	{
		m_bFlash = TRUE;
	}
	else	
	{
		m_bFlash = FALSE;
	}

	TCHAR szTitle[256];
	_stprintf(szTitle,_T("[%ld users] Gersang Monitor"),currentuser);

	SetWindowText(szTitle);
	


}

void CServerMonitorDlg::OnClickServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	TCHAR szBuf[1024];

	int iSelected = m_ServerListControl.GetNextItem(-1,LVNI_SELECTED);	
	if (iSelected != -1)  {
		
	    ZeroMemory(szBuf,sizeof(szBuf));

	    g_GersangServerMonitor.GetServerInfo(iSelected,szBuf);

		m_ServerInfoEdit.SetWindowText(szBuf);

		if (m_ServerListControl.GetCheck(iSelected))
		{
			m_ServerListControl.SetCheck(iSelected,FALSE);
		}
		else
		{
			m_ServerListControl.SetCheck(iSelected,TRUE);

		}
		Invalidate(FALSE);


		
	}
	
	*pResult = 0;
}


BOOL CServerMonitorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	FILE* fp;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			TCHAR szString[256];
			ZeroMemory(szString,sizeof(szString));
			m_ConsoleEdit.GetWindowText(szString,256);
			ProcessConsoleCommand(szString);

			m_ConsoleEdit.SetWindowText(_T(""));
			
		}
	}
	


	fp = fopen("c:\\ad.txt","at");				
	fprintf(fp,"%d\n",pMsg->wParam);
	fclose(fp);
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CServerMonitorDlg::ProcessConsoleCommand(TCHAR *szCommandString)
{

	static TCHAR szCommand[256];
	static TCHAR szParam1[256];
	static TCHAR szParam2[256];
	static TCHAR szParam3[256];
	static WORD serverlist[256];

	if (!g_GersangServerMonitor.IsConnected())
	{
		PutConsoleString(RGB(255,0,0),_T("Not connected.\n"));
		return 0;
	}

	// 맨앞이 /이 아니면 채팅
	if (szCommandString[0] == '/')
	{
		szCommandString++;
	}
	else
	{
		g_GersangServerMonitor.SendChat(szCommandString);
		return 0;
		

	}
	
	if (_stscanf(szCommandString,_T("%s"),szCommand) == 1)
	{
		if (!_tcscmp(szCommand,_T("exit")))
		{
			;
		}
		else if (!_tcscmp(szCommand,_T("adduser")))
		{
			if (_stscanf(szCommandString,_T("%s %s %s %s"),szCommand,szParam1,szParam2,szParam3) != 4)
			{
				PutConsoleString(RGB(255,0,0),_T("Usage : /%s ID Password Name \n "),szCommand);
				return 1;
			}
			else			
			{
				CMyPacket Packet;
				char buf[96];
				ZeroMemory(buf,96);
				_tcscpy((TCHAR *)buf,szParam1);
				_tcscpy((TCHAR *)&buf[32],szParam2);
				_tcscpy((TCHAR *)&buf[64],szParam3);
				Packet.Pack(buf,MMC_QUERY_ADDUSER,96);
		
				g_GersangServerMonitor.PutPacket(&Packet);
		
			}
		}
		else if (!_tcscmp(szCommand,_T("deluser")))
		{
			if (_stscanf(szCommandString,_T("%s %s"),szCommand,szParam1) != 2)
			{
				PutConsoleString(RGB(255,0,0),_T(" Usage : /%s ID\n "),szCommand);
				return 1;
			}
			else			
			{
				CMyPacket Packet;
				char buf[32];
				ZeroMemory(buf,32);
				_tcscpy((TCHAR *)buf,szParam1);
				Packet.Pack(buf,MMC_QUERY_DELUSER,32);
		
				g_GersangServerMonitor.PutPacket(&Packet);
			}
		}
		else if (!_tcscmp(szCommand,_T("reboot")))
		{
			if (_stscanf(szCommandString,_T("%s %s"),szCommand,szParam1) != 2)
			{
				PutConsoleString(RGB(255,0,0),_T(" Usage : /%s NewMonitorServerFilename\n "),szCommand);
				return 1;
			}
			else			
			{
				CMyPacket Packet;
				TCHAR buf[256];
				ZeroMemory(buf,256);
				_tcscpy(buf,szParam1);
				Packet.Pack(buf,MONITOR_COMMAND_REBOOT,256);
		
				WORD servercount = GetSelectedServer(serverlist);
				g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
			}
		}
		else if (!_tcscmp(szCommand,_T("sms")))
		{
			if(_stscanf(szCommandString,_T("%s %s"),szCommand,szParam1) != 2)
			{
				PutConsoleString(RGB(255,0,0),_T(" Usage : /%s Message \n "),szCommand);
				return 1;
			}
			else
			{
				/*if(szParam1 > 79)
				{
					PutConsoleString(RGB(255,0,0),_T(" 한글 40자 영문 80자 이내로 적어주시죠.ㅡ.ㅡ \n "),szCommand);
					return 1;
				}*/
				//else
				{
					CMyPacket Packet;
					char buf[80];
					ZeroMemory(buf,80);
					_tcscpy(buf,szParam1);
					Packet.Pack(buf,MMC_QUERY_SMS,80);

					//WORD servercount = GetSelectedServer(serverlist);
					g_GersangServerMonitor.PutPacket(&Packet);
				}
				
			}
		}
		else if (!_tcscmp(szCommand,_T("execfilename")))
		{
			CMyPacket Packet;
			char buf[256];
			Packet.Pack(buf,MONITOR_COMMAND_QUERY_EXECFILENAME,0);
			
			WORD servercount = GetSelectedServer(serverlist);
			g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
						
		}		
		else if (!_tcscmp(szCommand,_T("userlist")))
		{
			CMyPacket Packet;
			char buf[32];
			Packet.Pack(buf,MMC_QUERY_USERLIST,0);
			
			g_GersangServerMonitor.PutPacket(&Packet);			
		}			
		else if (!_tcscmp(szCommand,_T("connectlist")))
		{
			CMyPacket Packet;
			char buf[32];
			Packet.Pack(buf,MMC_QUERY_CONNECTUSERLIST,0);
			
			g_GersangServerMonitor.PutPacket(&Packet);			
		}	
		else
		{
			PutConsoleString(RGB(255,0,0),_T(" '%s' : Unknown command.\n "),szCommand);
		}
	}

	return 1;

}
/*
int CServerMonitorDlg::strlength(char* str)
{
	int num;

	return num;
}
*/

void CServerMonitorDlg::PutConsoleString(COLORREF color, const TCHAR *szString,...)
{

	CHARFORMAT2 cf;

	va_list args;
	TCHAR pszString[1024];
	ZeroMemory(pszString,sizeof(pszString));
	va_start(args,szString);
	_vstprintf(pszString,szString,args);
	va_end(args);	


#ifdef _MBCS
	int siTemp = _tcslen(pszString); // 입력된 내용의 길이를 얻음	//이부분 수정	
#else if _UNICODE
	int siTemp = _tcslen(pszString); // 입력된 내용의 길이를 얻음	
#endif
	if (siTemp <= 0) return;									// 0 이하면 넘김

	
	
	Edit_SetSel(m_ConsoleResultEdit,m_siSetSel,m_siSetSel);		    // 리치에디터컨트롤의 포인터를 맨끝으로 옮긴후
    Edit_ReplaceSel(m_ConsoleResultEdit, pszString);				            // 내용을 삽입
    Edit_SetSel(m_ConsoleResultEdit,m_siSetSel,m_siSetSel + siTemp);   // 입력된 내용의 폰트변경을 위해 블럭으로 잡음

	
	/*     블럭으로 잡아놓은 내용의 폰트크기 색상등을 설정     */
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
    cf.cbSize = sizeof(CHARFORMAT2);
    cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_ITALIC | CFM_UNDERLINE;
    cf.dwEffects = 0;    
    cf.yOffset = 0;
    cf.crTextColor = color;
    cf.bCharSet = ANSI_CHARSET;
    cf.bPitchAndFamily = DEFAULT_PITCH;
    
	
	m_ConsoleResultEdit.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf); // 폰트형식 바꿈 메시지를 보냄
	m_siSetSel += siTemp;												            // 포인터를 맨끝으로
	
	m_ConsoleResultEdit.SendMessage( EM_SCROLLCARET, 0, 0 );						// 언제나 대화창의 맨끝부분을 보도록해준다

	int fbl = m_ConsoleResultEdit.GetFirstVisibleLine();
	int linecount = m_ConsoleResultEdit.GetLineCount();

	if (fbl < linecount - 9)
	{
		m_ConsoleResultEdit.LineScroll( (linecount - 9) - fbl);
	}
	


}



void CServerMonitorDlg::OnConnectbutton() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if (m_MasterIpString.GetLength() > 0 ) {
#ifdef _UNICODE
		char mbsipstring[256];
		ZeroMemory(mbsipstring,sizeof(mbsipstring));
		wcstombs(mbsipstring,m_MasterIpString,256);
		g_GersangServerMonitor.SetAddress(mbsipstring,MASTER_SERVER_PORT);
#else
		g_GersangServerMonitor.SetAddress(m_MasterIpString,MASTER_SERVER_PORT);
#endif
	}


	CLoginDialog LoginDlg;
	int ret = LoginDlg.DoModal();
	if (ret == IDOK) {
		ZeroMemory(g_GersangServerMonitor.m_szID,256);
		ZeroMemory(g_GersangServerMonitor.m_szPassword,256);

		_tcscpy(g_GersangServerMonitor.m_szID,LoginDlg.m_szID);
		_tcscpy(g_GersangServerMonitor.m_szPassword,LoginDlg.m_szPassword);


		//LoginDlg.m_IDEdit.GetWindowText(g_GersangServerMonitor.m_szID,32);
		//LoginDlg.m_PasswordEdit.GetWindowText(g_GersangServerMonitor.m_szPassword,32);
		g_GersangServerMonitor.ConnectToServer();
	}

	
}

void CServerMonitorDlg::OnExitbutton() 
{
	// TODO: Add your control notification handler code here
	PostQuitMessage(0);
	
}

void CServerMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
	case 1:
		if (g_GersangServerMonitor.IsConnected()) 
		{
			RefreshServerList();
		}
		break;	
	case 2:
		g_GersangServerMonitor.Process();
		break;
	case 3:
		{
			if (m_AutoNoticeButton.GetCheck()) 
			{
				if (g_GersangServerMonitor.IsConnected()) 
				{
					
					TCHAR szNotice[256];

					
					
					ZeroMemory(szNotice,256);
					
					m_AutoNoticeContent.GetWindowText(szNotice,256);
#ifdef _UNICODE
					
					char mbNotice[256];

					ZeroMemory(mbNotice,sizeof(mbNotice));
					
					wcstombs(mbNotice,szNotice,256);
					
					char *notice = mbNotice;
					char *temp = mbNotice;
					
#else
					char *notice = szNotice;
					char *temp = szNotice;
									
#endif
					BOOL flag = FALSE;
					
					while (*notice != 0)
					{
						flag = FALSE;
						if (*notice == '|')
						{
							*notice = NULL;
							if (strlen(temp) > 0) 
							{
								g_GersangServerMonitor.SendNotice(temp);
							}
							flag = TRUE;
							
						}
						notice++;
						if (flag) temp = notice;
						
					}
					
					if (strlen(temp) > 0) 
					{
						g_GersangServerMonitor.SendNotice(temp);													
					}
				}
			}
			
		}
		break;

	case 4:
		{
			if (m_bFlash)
			{
				FlashWindow(TRUE);
			}
		}
		break;
/*	case 5:
		{
			MonitorSmsReq* pMonitorSms = g_GersangServerMonitor.PopSmsData();

			
		}
		break;*/
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CServerMonitorDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	
	return CDialog::DestroyWindow();
}



void CServerMonitorDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	int ret = TabCtrl_GetCurSel(pNMHDR->hwndFrom);
	switch (ret)
	{
	case 0:
		m_siCurrentListMode = 0;
		m_ServerListControl.ShowWindow(TRUE);
		m_ServerListControl2.ShowWindow(FALSE);
		break;
	case 1:
		m_siCurrentListMode = 1;
		m_ServerListControl.ShowWindow(FALSE);
		m_ServerListControl2.ShowWindow(TRUE);
		break;
	}
	
	*pResult = 0;
}

void CServerMonitorDlg::OnFiletransferbutton() 
{
	TCHAR dir[256];
	GetCurrentDirectory(256,dir);

	WORD serverlist[256];
	int servercount = 0;


	// TODO: Add your control notification handler code here
//	if (m_siCurrentListMode != 1) return;

	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,_T("All Files (*.*)|*.*||"));
	int ret = dlg.DoModal();

	if (ret != IDOK)
	{
		return;

	}

	
	
	CString filename = dlg.GetPathName();
	CString filenamenopath = dlg.GetFileName();

	SetCurrentDirectory(dir);

	CSelectDirectoryDialog dlg2;
	

	ret = dlg2.DoModal();

	if (ret != IDOK)
	{
		return;

	}

	servercount = GetSelectedServer(serverlist);

	

	
//	if (servercount > 0) {
		g_GersangServerMonitor.SendFile(servercount,serverlist,filename,filenamenopath);
//	}
	
}

void CServerMonitorDlg::OnRclickServerlistcontrol2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	POINT pt;

	GetCursorPos(&pt);

	if (m_hPopupMenu) {
		int ret = TrackPopupMenu(m_hPopupMenu,TPM_LEFTALIGN,pt.x,pt.y,0,m_hWnd,NULL);
	}

	

	
	*pResult = 0;
}

BOOL CServerMonitorDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);

	if (wNotifyCode == 0)
	{
		if (wID >= 10000)
		{
			int usage = wID - 10000;
			int num = m_ServerListControl2.GetItemCount();

			BOOL bIsAnyItemOn = FALSE;


			for (int a = 0; a < num; a++)
			{
				if ((a % g_GersangServerMonitor.GetServerPerGroup()) == usage) {
					if (ListView_GetCheckState(m_ServerListControl2.m_hWnd,a))
					{
						bIsAnyItemOn = TRUE;
					}
					
				}
								
			}
			
			for ( a = 0; a < num; a++)
			{
				if ((a % g_GersangServerMonitor.GetServerPerGroup()) == usage) {
					ListView_SetItemState(m_ServerListControl2.m_hWnd,a,bIsAnyItemOn ? 0x1000 : 0x2000,LVIS_STATEIMAGEMASK);
					/*DWORD dw = ListView_GetItemState(m_ServerListControl2.m_hWnd,a,LVIS_STATEIMAGEMASK);
					char str[256];
					sprintf(str,"%x",dw);
					AfxMessageBox(str);*/
				}
								
			}
			
			
		}

	}

	
	return CDialog::OnCommand(wParam, lParam);
}

void CServerMonitorDlg::OnServeroffbutton() 
{
	// TODO: Add your control notification handler code here
	char szTemp[256];
	WORD serverlist[256];
	int servercount = 0;

	if (!ConfirmServer())
	{
		return;
	}



	if (AfxMessageBox(_T("Are you sure?"),MB_YESNO,0) != IDYES)
	{
		return;
	}

	servercount = GetSelectedServer(serverlist);

	
	if (servercount > 0) {
		CMyPacket Packet;
		Packet.Pack(szTemp,MONITOR_COMMAND_SERVER_OFF,0);
		
		g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	}

	for (int c = 0; c < servercount; c++)
	{
		TCHAR szChat[256];
		ZeroMemory(szChat,sizeof(szChat));
		int servercode = serverlist[c];
		int group = servercode / 10;
		int servernum = servercode % 10;
		_stprintf(szChat,_T("turned off %s %s server."),g_GersangServerMonitor.m_pServer[group].GetServerName(),g_GersangServerMonitor.m_ppszServerUsage[servernum]);
		g_GersangServerMonitor.SendChat(szChat);
	}
	
	
}

void CServerMonitorDlg::OnServeronbutton() 
{
	// TODO: Add your control notification handler code here
	char szTemp[256];
	WORD serverlist[256];
	int servercount = 0;

	if (!ConfirmServer())
	{
		return;
	}



	if (AfxMessageBox(_T("This command will reset server even if server was already on. Continue?"),MB_YESNO,0) != IDYES)
	{
		return;
	}

	servercount = GetSelectedServer(serverlist);

	
	if (servercount > 0) {
		CMyPacket Packet;
		Packet.Pack(szTemp,MONITOR_COMMAND_SERVER_ON,0);
		
		g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	}

	for (int c = 0; c < servercount; c++)
	{
		TCHAR szChat[256];
		ZeroMemory(szChat,sizeof(szChat));
		int servercode = serverlist[c];
		int group = servercode / 10;
		int servernum = servercode % 10;
		_stprintf(szChat,_T("turned on %s %s server."),g_GersangServerMonitor.m_pServer[group].GetServerName(),g_GersangServerMonitor.m_ppszServerUsage[servernum]);
		g_GersangServerMonitor.SendChat(szChat);
	}
	
}

void CServerMonitorDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDCtl)
	{
	case IDC_SERVERLISTCONTROL:
		{
		
		}
		break;

	}
	
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CServerMonitorDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDCtl)
	{
	case IDC_SERVERLISTCONTROL:
		{
			TCHAR str[256];
			int leftcount = 0;
			DRAWITEMSTRUCT *ds;
			int columncount = g_GersangServerMonitor.GetServerPerGroup() + 5;
			int servercolumn = g_GersangServerMonitor.GetServerPerGroup();
			ds = lpDrawItemStruct;
			
			
			
			for (int a = 0; a < columncount; a++) {

				// 서버이름
				if (a == 0) {

					COLORREF c;
					if (g_GersangServerMonitor.m_pServer[ds->itemData].m_siMute)
					{
						c = RGB(128,128,128);

					}
					else  
					{
						c = RGB(0,0,0);

					}
					COLORREF cl = SetTextColor(lpDrawItemStruct->hDC,c);


					if (m_ServerListControl.GetCheck(lpDrawItemStruct->itemID))
					{
						TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,_T("X"),2);

					}
					else
					{
						
						TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,_T("O"),2);
					}
					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount + 20,lpDrawItemStruct->rcItem.top,str,_tcslen(str));
				}
				// 서버상태
				else if ( a >= 1 && a <= servercolumn)
				{
					
					COLORREF cl = SetTextColor(lpDrawItemStruct->hDC,TranslateServerStateToColor(g_GersangServerMonitor.GetServerState(ds->itemData,a - 1)));

					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,str,_tcslen(str));
					SetTextColor(lpDrawItemStruct->hDC,cl);
				

				}				
				// 동접자수
				else if ( a == servercolumn + 1)
				{
					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,str,_tcslen(str));
				

				}
				// 증감량
				else if ( a == servercolumn + 2)
				{

					COLORREF c;

					int dif = g_GersangServerMonitor.m_pServer[ds->itemData].GetDifferance();

					if (dif > 0)
					{
						c = RGB(0,0,255);

					}
					else  
					{
						c = RGB(255,0,0);

					}
					COLORREF cl = SetTextColor(lpDrawItemStruct->hDC,c);

					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,str,_tcslen(str));

					SetTextColor(lpDrawItemStruct->hDC,cl);
				

				}
				// 킥카운트
				else if ( a == servercolumn + 3)
				{
					COLORREF c;
					int kicktimer = g_GersangServerMonitor.m_pServer[ds->itemData].GetKickCount();
					if (kicktimer < 0)
					{
						c = RGB(0,0,0);

					}
					else  
					{
						c = RGB(255,0,0);

					}
					COLORREF cl = SetTextColor(lpDrawItemStruct->hDC,c);

					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,str,_tcslen(str));

					SetTextColor(lpDrawItemStruct->hDC,cl);
				

				}
				else if ( a == servercolumn + 4)
				{
					COLORREF c;
					c = RGB(0,0,0);

					COLORREF cl = SetTextColor(lpDrawItemStruct->hDC,c);

					m_ServerListControl.GetItemText(lpDrawItemStruct->itemID,a,str,256);			
					TextOut(lpDrawItemStruct->hDC,lpDrawItemStruct->rcItem.left + leftcount,lpDrawItemStruct->rcItem.top,str,_tcslen(str));

					SetTextColor(lpDrawItemStruct->hDC,cl);
				

				}
			
				
				leftcount += m_ServerListControl.GetColumnWidth(a);
			}
			
			return;

		}
		break;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CServerMonitorDlg::OnGetdispinfoServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM *pItem = &pDispInfo->item;
	// TODO: Add your control notification handler code here
    pItem->mask |= LVIF_STATE; 
	pItem->stateMask = LVIS_STATEIMAGEMASK; 
	pItem->state = INDEXTOSTATEIMAGEMASK(m_ServerListControl.GetCheck(pItem->iItem) ? 2 : 1); 

	
	*pResult = 0;
}

WORD CServerMonitorDlg::GetSelectedServer(WORD *pServerlist)
{
	WORD servercount = 0;

	// 서버 군으로 보기
	if (m_siCurrentListMode == 0) {
		int num = m_ServerListControl.GetItemCount();
		
		for (int a = 0; a < num; a++)
		{
			if (ListView_GetCheckState(m_ServerListControl.m_hWnd,a))
			{
				int groupnum = a;

				
				for (int kind = 0; kind < CGersangServerStatus::GetServerNum(); kind++) {
					pServerlist[servercount] = MAKESERVERCODE(a,kind);
					servercount++;
				
					
				}
				
			}
			
		}

	}
	// 서버 별로보기
	else if (m_siCurrentListMode == 1) {
		
		int num = m_ServerListControl2.GetItemCount();
		
		for (int a = 0; a < num; a++)
		{
			if (ListView_GetCheckState(m_ServerListControl2.m_hWnd,a))
			{
				int groupnum = a / CGersangServerStatus::GetServerNum();
				int kind = a % CGersangServerStatus::GetServerNum();

				pServerlist[servercount] = MAKESERVERCODE(groupnum,kind);
				servercount++;				
				
			}
			
		}
	}

	return servercount;

	

}


void CServerMonitorDlg::OnKillfocusIpcombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_MasterIpString.GetLength() > 0 ) {
		FILE *fp;
		TCHAR buffer[256];
		TCHAR buffer2[256];

#ifdef _UNICODE
		{
		char tempip[256];
		wcstombs(tempip,m_MasterIpString,256);
		g_GersangServerMonitor.SetAddress(tempip,MASTER_SERVER_PORT);
		}

#else
		g_GersangServerMonitor.SetAddress(m_MasterIpString,MASTER_SERVER_PORT);
#endif

		
		
		
		m_MasterIpCombo.GetWindowText(buffer,256);


		fp = _tfopen(_T("rcip.txt"),_T("r+t"));
		
		if (fp != NULL)
		{
			fseek(fp,0,SEEK_SET);
			int flag = 0;
			while (_fgetts(buffer2,256,fp) != NULL)
			{
				buffer2[_tcslen(buffer2) - 1] = 0;
				if ( !_tcscmp(buffer2,buffer) ) {
					
					
					flag = 1;
				}
			}
			if (!flag)
			{
				_ftprintf(fp,_T("%s\n"),buffer);

				
				
			}
		}
		else
		{
			fp = fopen("rcip.txt","wt");
			_ftprintf(fp,_T("%s\n"),buffer);

			
		}
		
		fclose(fp);
	}
	
}

void CServerMonitorDlg::ClearServerList()
{
	if (m_hPopupMenu) {
		m_hPopupMenu = NULL;
		DestroyMenu(m_hPopupMenu);
	}

	m_ServerListControl.DeleteAllItems();

	int count = m_ServerListControl.GetHeaderCtrl()->GetItemCount();
	for (int a = 0; a < count; a++)
	{
		m_ServerListControl.DeleteColumn(0);

	}
	count = m_ServerListControl2.GetHeaderCtrl()->GetItemCount();
	m_ServerListControl2.DeleteAllItems();
	for (int b = 0; b < count; b++)
	{
		m_ServerListControl2.DeleteColumn(0);

	}


}

void CServerMonitorDlg::OnKillfocusAutonoticecontent() 
{
	// TODO: Add your control notification handler code here

	
}

void CServerMonitorDlg::OnKillfocusIntervaledit() 
{
	// TODO: Add your control notification handler code here

	static BOOL TimerSet = FALSE;
	TCHAR szTemp[256];
	m_IntervalEdit.GetWindowText(szTemp,256);
	int time = _ttoi(szTemp);
	if (time > 0)
	{
		if (TimerSet)
		{
			KillTimer(3);
		}
		SetTimer(3,time * 1000,NULL);
		TimerSet = TRUE;
	}
	
}

void CServerMonitorDlg::OnNoticesendbutton2() 
{
	// TODO: Add your control notification handler code here

	if (!ConfirmServer())
	{
		return;
	}

	int ret = AfxMessageBox(_T("This command will send notice to selected server. Continue?"), MB_YESNO|MB_ICONSTOP);

	if (ret != IDYES) return;

	if (g_GersangServerMonitor.IsConnected()) {
		
		TCHAR szNotice[256];		
		ZeroMemory(szNotice,sizeof(szNotice));
		
		m_AutoNoticeContent.GetWindowText(szNotice,256);

#ifdef _UNICODE

		char mbNotice[256];

		wcstombs(mbNotice,szNotice,256);

		char *notice = mbNotice;
		char *temp = mbNotice;
		

#else
		char *notice = szNotice;
		char *temp = szNotice;
		

#endif

		
		BOOL flag = FALSE;
		
		while (*notice != 0)
		{
			flag = FALSE;
			if (*notice == '|')
			{
				*notice = NULL;
				if (strlen(temp) > 0) {
					g_GersangServerMonitor.SendNotice(temp);
				}
				flag = TRUE;
				
			}
			notice++;
			if (flag) temp = notice;
			
		}
		
		if (strlen(temp) > 0) {
			g_GersangServerMonitor.SendNotice(temp);													
		}

	}
		
		
}

void CServerMonitorDlg::OnRclickServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	

	int iSelected = m_ServerListControl.GetNextItem(-1,LVNI_SELECTED);	
	if (iSelected != -1)  {
			
		g_GersangServerMonitor.m_pServer[iSelected].m_siMute ^= 1;



		Invalidate(FALSE);


		
	}
	
	*pResult = 0;
}

int CServerMonitorDlg::ConfirmServer() 
{
	// TODO: Add your control notification handler code here



	TCHAR szMsg[512];

	TCHAR szTemp[256];

	_stprintf(szMsg,_T("This action will take effect in\n"));


	if (m_siCurrentListMode == 0) {
		int num = m_ServerListControl.GetItemCount();
		
		for (int a = 0; a < num; a++)
		{
			if (ListView_GetCheckState(m_ServerListControl.m_hWnd,a))
			{
				int groupnum = a;

				_stprintf(szTemp,_T("%s server\n"),g_GersangServerMonitor.m_pServer[groupnum].GetServerName());
				_tcscat(szMsg,szTemp);

				
						
			}
			
		}

	}
	// 서버 별로보기
	else if (m_siCurrentListMode == 1) {
		
		int num = m_ServerListControl2.GetItemCount();
		
		for (int a = 0; a < num; a++)
		{
			if (ListView_GetCheckState(m_ServerListControl2.m_hWnd,a))
			{

				
				int groupnum = a / CGersangServerStatus::GetServerNum();
				int kind = a % CGersangServerStatus::GetServerNum();

				_stprintf(szTemp,_T("%s %s server\n"),
					g_GersangServerMonitor.m_pServer[groupnum].GetServerName(),
					g_GersangServerMonitor.m_ppszServerUsage[kind]);
				_tcscat(szMsg,szTemp);

				
				
			}
			
		}
	}




	


	




	int ret = AfxMessageBox(szMsg, MB_YESNO|MB_ICONEXCLAMATION);

	if (ret == IDYES) return 1;
	else return 0;

			
		
}

void CServerMonitorDlg::OnKickbutton() 
{
	// TODO: Add your control notification handler code here

	if (m_siCurrentListMode != 0) {
		return;
	}

	if (!ConfirmServer())
	{
		return;
	}

	TCHAR szTemp[256];

	m_KickTimerEdit.GetWindowText(szTemp,256);
	int time = _ttoi(szTemp);

	

	if (time < 0)
	{
		return;
	}
	else if (time == 0)
	{
		if (AfxMessageBox(_T("All user in selected server will be kicked!!"),MB_YESNO,0) != IDYES)
		{
			return;
		}
		g_GersangServerMonitor.SendLogout();
		
	}
	else 
	{
		_stprintf(szTemp,_T("All user in selected server will be kicked in %ld seconds!!"),time);
		if (AfxMessageBox(szTemp,MB_YESNO,0) != IDYES)
		{
			return;
		}
		for (int a = 0; a < g_GersangServerMonitor.GetServerGroupNum(); a++)
		{
			if (ListView_GetCheckState(m_ServerListControl.m_hWnd,a))
			{
				g_GersangServerMonitor.m_pServer[a].SetKickCount(time);
								
			}
			
		}

	}
	
	
	

	
	
}
