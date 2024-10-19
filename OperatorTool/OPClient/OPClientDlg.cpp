// OPClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPClient.h"

#include "Graphics16.h"
#include "OPClientDlg.h"
#include "TextDlg.h"

#include "..\\common\\defines.h"

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
// COPClientDlg dialog

COPClientDlg::COPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COPClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COPClientDlg)
	m_strID = _T("");
	m_strPW = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COPClientDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPW);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COPClientDlg, CDialog)
	//{{AFX_MSG_MAP(COPClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, OnButtonText)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CHAR, OnButtonChar)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_ITEM, OnButtonItem)
	ON_MESSAGE(WM_CONNECT, OnConnect)
	ON_MESSAGE(WM_LOGIN, OnLogin)	
	ON_MESSAGE(ON_RES_ITEM_EDIT, OnItemEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPClientDlg message handlers

BOOL COPClientDlg::OnInitDialog()
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



	// 모든 버튼을 비활성화 시킨다.
	GetDlgItem(IDC_BUTTON_ITEM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CHAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_QUIT)->ShowWindow(SW_HIDE);


	//서버로 접속을 한다.
	m_ClientSocket.Create();
	m_ClientSocket.Connect(IP, PORT);
	m_ClientSocket.SetHwnd(this->m_hWnd);

	m_bConnect = FALSE;

	// 메시지 변경
	GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText("서버에 접속중입니다.");


	// 텍스트 편집용 다알로그
	m_TextDlg.Create(IDD_DIALOG_TEXT, this);

	// 아이템 편집용
	m_ItemDlg.Create(IDD_DIALOG_ITEM, this);

	// 캐릭터 편집용
	m_CharDlg.Create(IDD_DIALOG_CHAR, this);

//	m_ModifyDlg.Create(IDD_DIALOG_MODIFY);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COPClientDlg::OnPaint() 
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
HCURSOR COPClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COPClientDlg::disableLogin(int disable)
{
	GetDlgItem(IDC_STATIC_BOX)->ShowWindow(disable);
	GetDlgItem(IDC_STATIC_LOGIN_LINE)->ShowWindow(disable);
	GetDlgItem(IDC_STATIC_PW_LINE)->ShowWindow(disable);
	GetDlgItem(IDC_STATIC_LOGIN_ID)->ShowWindow(disable);
	GetDlgItem(IDC_STATIC_LOGIN_PW)->ShowWindow(disable);
	GetDlgItem(IDC_EDIT_ID)->ShowWindow(disable);
	GetDlgItem(IDC_EDIT_PW)->ShowWindow(disable);
	GetDlgItem(IDC_BUTTON_LOGIN)->ShowWindow(disable);
	GetDlgItem(IDOK)->ShowWindow(disable);	
}

void COPClientDlg::OnConnect(WPARAM wParam, LPARAM lParam)
{
	m_bConnect = (BOOL)wParam;

	m_bConnect == TRUE ? GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText("아이디와 패스워드를 입력해 주세요.")
		: GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText("서버와 연결을 실패 하였습니다.");

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(m_bConnect);
}

void COPClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();	

	m_TextDlg.DestroyWindow();
	m_CharDlg.DestroyWindow();
	m_ItemDlg.DestroyWindow();
}

BOOL COPClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN)	return 0;
	if(pMsg->wParam == VK_ESCAPE)	return 0;

	return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 버튼 
void COPClientDlg::OnButtonLogin() 
{
	UpdateData();

	ReqLogin	login;

	if(m_strID.IsEmpty())
	{
		AfxMessageBox("아이디를 입력해 주세요");
		return;
	}

	if(m_strPW.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력해 주세요");
		return;
	}

	strcpy(login.strID, (LPSTR)(LPCTSTR)m_strID);
	strcpy(login.strPW, (LPSTR)(LPCTSTR)m_strPW);
	
	m_strPW = "";

	m_ClientSocket.Send((char*)&login, sizeof(ReqLogin));

	GetDlgItem(IDC_SYSTEM_MSG)->SetWindowText("아이디와 패스워드를 체크 중입니다.");
}

void COPClientDlg::OnButtonText() 
{
	RECT rt;
	GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(&rt);
	

	::MoveWindow(m_TextDlg.m_hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
	m_TextDlg.ShowWindow(SW_SHOW);
	

	this->GetWindowRect(&rt);

	// 언두 /////////////////////////////////////////////////////////////////////////////////////////////////////
//	RECT moveRt;
//	m_ModifyDlg.GetWindowRect(&moveRt);

//	::MoveWindow(m_ModifyDlg.m_hWnd, rt.left, rt.bottom+1, rt.right-rt.left-1, moveRt.bottom-moveRt.top, TRUE);
//	m_ModifyDlg.ShowWindow(SW_SHOW);	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_CharDlg.ShowWindow(SW_HIDE);
	m_ItemDlg.ShowWindow(SW_HIDE);
}

void COPClientDlg::OnButtonChar() 
{
	RECT rt;
	GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(&rt);

	::MoveWindow(m_CharDlg.m_hWnd, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, TRUE);
	m_CharDlg.ShowWindow(SW_SHOW);

	m_ItemDlg.ShowWindow(SW_HIDE);
	m_TextDlg.ShowWindow(SW_HIDE);
//	m_ModifyDlg.ShowWindow(SW_HIDE);
}

void COPClientDlg::OnButtonItem() 
{
	ReqItemEdit	ItemEdit;

	m_ClientSocket.Send((void*)&ItemEdit, sizeof(ItemEdit));
}

void COPClientDlg::OnButtonQuit() 
{
	CDialog::OnOK();	
}
