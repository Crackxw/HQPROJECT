// CharListUpdateToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CharListUpdateTool.h"
#include "CharListUpdateToolDlg.h"
#include "JOCharListUpdateConfig.h"
#include "AdoExecutor.h"
#include <assert.h>

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
// CCharListUpdateToolDlg dialog

CCharListUpdateToolDlg::CCharListUpdateToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharListUpdateToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharListUpdateToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCharListUpdateToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharListUpdateToolDlg)
	DDX_Control(pDX, IDC_SERVERSELECTLIST, m_SelectListBox);
	DDX_Control(pDX, IDC_MSGDISPLAYLIST, m_MsgDisplayList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCharListUpdateToolDlg, CDialog)
	//{{AFX_MSG_MAP(CCharListUpdateToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILEOPN, OnFileopn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DBCONNECTBTN, OnDbconnectbtn)
	ON_BN_CLICKED(IDC_EXECUTEBTN, OnExecutebtn)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ALLCANCELBTN, OnAllcancelbtn)
	ON_BN_CLICKED(IDC_ALLSELECTBTN, OnAllselectbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharListUpdateToolDlg message handlers

BOOL CCharListUpdateToolDlg::OnInitDialog()
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

	Initialize_Parser();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCharListUpdateToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCharListUpdateToolDlg::OnPaint() 
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
HCURSOR CCharListUpdateToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void	CCharListUpdateToolDlg::Initialize_Parser()
{
	CoInitialize(NULL);
	m_bFileOpen = FALSE;
	m_dwSetServerCount	= 0;

	CJOCharListUpdateConfig Config;

	Config.SetConfigFileName(GERSANG_CHAR_LIST_UPDATE_INFO_FILE);
	m_dwSetServerCount = Config.DBConnectCountInt();

	m_pDBInfo = new DBINFO[m_dwSetServerCount];
	assert(m_pDBInfo != NULL);

	m_pAdoConn = new CAdoExecutor[m_dwSetServerCount];
	assert(m_pAdoConn != NULL);

	for(DWORD n = 0 ; n < m_dwSetServerCount ; n++)
	{
		strcpy(m_pDBInfo[n].szDBName, Config.DBServerNameString(n));
		strcpy(m_pDBInfo[n].szDBConnString, Config.DBConnectionString(n));

		m_SelectListBox.AddString(m_pDBInfo[n].szDBName);
	}
}

void	CCharListUpdateToolDlg::Finialize_Parser()
{

	Free();

	if(m_pDBInfo != NULL)
	{
		delete[] m_pDBInfo;
		m_pDBInfo = NULL;
	}

	if(m_pAdoConn != NULL)
	{
		delete[] m_pAdoConn;
		m_pAdoConn = NULL;
	}

	CoUninitialize();
}

void CCharListUpdateToolDlg::DBConnect()
{
	int nCheck = BST_CHECKED;

	for(int n = 0 ; n < m_dwSetServerCount ; n++)
	{
		nCheck = m_SelectListBox.GetCheck( n );
		if( nCheck != BST_CHECKED )
			continue;

		if(m_pAdoConn[n].IsOpen() == TRUE)
			continue;

		if(m_pAdoConn[n].Connect(m_pDBInfo[n].szDBConnString) == FALSE)
		{
			MsgPrint("%s DB connect failed", m_pDBInfo[n].szDBName);
		}
		else
		{
			MsgPrint("%s DB connect succeed", m_pDBInfo[n].szDBName);
		}
	}
}


void CCharListUpdateToolDlg::OnFileopn() 
{
	// TODO: Add your control notification handler code here

	TCHAR dir[256];
	GetCurrentDirectory(256,dir);
	
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,_T("All Files (*.*)|*.*||"));
	int ret = dlg.DoModal();

	if (ret != IDOK)
	{
		return;
	}	
	
	CString filename = dlg.GetPathName();
	CString filenamenopath = dlg.GetFileName();

	/////////////////////////////////////////////////////////////////
	// Temp
//	Load84YearBirthUser(filename.GetBuffer(0));

//	return;

	/////////////////////////////////////////////////////////////////



	FileLoad(filename.GetBuffer(0));

	SetCurrentDirectory(dir);
}

BOOL CCharListUpdateToolDlg::FileLoad(char* pCharFile)
{
	FILE		*fp		=	NULL;
	CHAR		*pToken	=	NULL;
	CHAR		szBuffer[1024];
	

	Free();

	fp = fopen(pCharFile, "r");
//	fp = BindJXFile::GetInstance()->GetFile( pCharFile );
	if( fp == NULL )
	{
		m_bFileOpen = FALSE;
		return FALSE;
	}


	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );


	CharHead*	pTempCharHead = NULL;

	while( fgets( szBuffer, 1024, fp ) )
	{

		pTempCharHead = new CharHead;
		assert(pTempCharHead != NULL);

//		ZeroMemory( &pTempCharHead, sizeof( CharHead ) );
		
		// ID
		pToken						=	strtok( szBuffer, "\n\t\r" );

		if( pToken == NULL ) break;

		pTempCharHead->uiID			=	GetID( pToken );

		// 케릭터 속성을 받는다.
		pTempCharHead->siAttri		=	GetAttri( pToken );

		// 몬스터 수를 알아 본다.
//		if( pTempCharHead->siAttri == ON_ATTRI_MONSTER )	
//			siMonsterNum++;

		// 케릭터 이름
		pToken							=	strtok( NULL, "\n\t\r" );
		strncpy(pTempCharHead->szCharName, pToken, 40);
	//	pTempCharHead->szCharName[11] = '\n';

		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->uiCharName	=	atoi( pToken );


		// 경험치 다음
		pToken								=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siCurExp = atoi( pToken );

		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->uiMyExp		=	atoi( pToken );

		// 국가
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siNation		=	atoi( pToken );

		// 힘
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siStr = atoi( pToken );

		// 민첩
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siDex = atoi( pToken );

		// 생명		
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siVit = atoi( pToken );

		// 지능
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siInt = atoi( pToken );

		
		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siAC = atoi( pToken );


		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siLv			=	atoi( pToken );


		pToken							=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siCon		=	atoi( pToken );

		
		pToken									 =	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.DamageResist =	atoi( pToken );

		
		pToken									 =	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.MagicResist  =	atoi( pToken );

		
		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.EquipMinDamage	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.EquipMaxDamage	=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siMoveSpeed				=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->Parameta.siLife = atoi( pToken );



		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siManaMax				=	atoi( pToken );
		pTempCharHead->Parameta.siMana = pTempCharHead->siManaMax;

		
		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siCost					=	atoi( pToken );		


		pToken										=	strtok( NULL, "\n\t\r" );
		pTempCharHead->siNeedLv					=	atoi( pToken );


		pToken										=	strtok( NULL, "\n\t\r" );

		if( strcmp( pToken, "Arrow" ) == 0 )			pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_ARROW;
		else if( strcmp( pToken, "Bullet" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BULLET;
		else if( strcmp( pToken, "Dart" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DART;
		else if( strcmp( pToken, "sword" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SWORD;
		else if( strcmp( pToken, "spear" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SEPEAR;
		else if( strcmp( pToken, "axe" ) == 0 )			pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_AXE;
		else if( strcmp( pToken, "fan" ) == 0 )			pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FAN;
		else if( strcmp( pToken, "craw" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_CRAW;
		else if( strcmp( pToken, "buddist" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BUDDIST;
		else if( strcmp( pToken, "beads") == 0)			pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_BEADS;
		else if( strcmp( pToken, "DoubleStick") == 0)	pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DOUBLESTICK;
		else if( strcmp( pToken, "Scythe") == 0 )       pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SCYTHE;
		else if( strcmp( pToken, "SpinStone") == 0)		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_SPINSTONE;
		else if( strcmp( pToken, "firegun") == 0)		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FIREGUN;
		else if( strcmp( pToken, "empty") == 0)			pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_EMPTY;
		else if( strcmp( pToken, "dsword" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_DSWORD;
		else if( strcmp( pToken, "stick" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_STICK;
		else if( strcmp( pToken, "pearl" ) == 0 )		pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_PEARL;
		
		else if( atoi( pToken ) == 1 )					pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_FREE;
		else											pTempCharHead->uiWeaponType	=	ON_ITEM_MISSLE_TYPE_NONE;

		pToken					  =	strtok( NULL, "\n\t\r" );
		pTempCharHead->siNeedCredit =	atoi( pToken );


		for( SI16 i=0 ; i<4; i++ )
		{
			pToken = strtok( NULL, "\n\t\r" );
			pTempCharHead->siItemDrop[i] = atoi(pToken);

			pToken = strtok( NULL, "\n\t\r" );
			pTempCharHead->siDropPercent[i] = atoi(pToken);
		}

		pToken = strtok( NULL, "\n\t\r" );
		pTempCharHead->siDescText = atoi(pToken);

		pToken = strtok( NULL, "\n\t\r" );
		pTempCharHead->siGeneral = atoi(pToken);

		pToken = strtok( NULL, "\n\t\r" );
		pToken = strtok( NULL, "\n\t\r" );
		pTempCharHead->siMonsterType = atoi(pToken);

		pTempCharHead->Parameta.siBonus = 0;		

		m_CharHeader.push_back( pTempCharHead );
	}	

	sort( m_CharHeader.begin(), m_CharHeader.end() );
	fclose( fp );

	m_bFileOpen = TRUE;
	return TRUE;
}


UI16	CCharListUpdateToolDlg::GetID(CHAR *pToken)
{
	UI16	uiVal;

	uiVal		=	MAKEWORD(pToken[0], atoi(&pToken[1]));	

	return uiVal;
}

SI16	CCharListUpdateToolDlg::GetAttri( char *pToken )
{
	switch( pToken[0] )
	{
	case 'C':	return ON_ATTRI_PLAYER;
	case 'M':	return ON_ATTRI_MONSTER;
	case 'U':	return ON_ATTRI_MERCENARY;
	}

	return -1;
}

void CCharListUpdateToolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default	
	
	CDialog::OnClose();
}

void CCharListUpdateToolDlg::MsgPrint(const TCHAR* szFormat,...)
{
	va_list	vl;
	TCHAR	szLog[1024];

	memset(szLog, 0, sizeof(szLog));
	va_start(vl, szFormat);
	_vstprintf(szLog, szFormat, vl);
	va_end(vl);

	int nLast = strlen(szLog) - 1;
	if(szLog[nLast] == '\n')
		szLog[nLast] = '\0';

	m_MsgDisplayList.AddString(szLog);

	m_MsgDisplayList.SetCaretIndex(m_MsgDisplayList.GetCount()-1);
}

void CCharListUpdateToolDlg::OnDbconnectbtn() 
{
	// TODO: Add your control notification handler code here

	DBConnect();
}

void CCharListUpdateToolDlg::OnExecutebtn() 
{
	// TODO: Add your control notification handler code here

	if(m_bFileOpen == FALSE)
	{
		MessageBox("Yet file open not!!", MB_OK);
		return;
	}

	int		nCheck = BST_CHECKED;
	DWORD	dwUpdate = 0, dwInsert = 0;
	DWORD	dwNumMaxData = (DWORD)m_CharHeader.size();

	vector<CharHead*>::iterator it;
	BYTE byRetVal = 0;

	for(DWORD n = 0 ; n < m_dwSetServerCount ; n++)
	{
		nCheck = m_SelectListBox.GetCheck( n );
		if( nCheck != BST_CHECKED )
			continue;

		if(m_pAdoConn[n].IsOpen() == FALSE)
		{
			MsgPrint("[%s] server connect failed\n", m_pDBInfo[n].szDBName);
			continue;
		}

		for(it = m_CharHeader.begin() ; it != m_CharHeader.end() ; ++it)
		{
			if(strlen((*it)->szCharName) > 10)
			{
				MsgPrint("[%s] [%s] invalid string length!\n", m_pDBInfo[n].szDBName, (*it)->szCharName );
				continue;
			}

			if((byRetVal = m_pAdoConn[n].Execution(1000, *it)) == 0)
			{
				MsgPrint("[%s] [%s] query execute failed!\n", m_pDBInfo[n].szDBName, (*it)->szCharName );
			}
			else
			{
				if(byRetVal == 1)
				{
					dwUpdate++;
					MsgPrint("[%s] [%s] update query execute succeed!\n", m_pDBInfo[n].szDBName, (*it)->szCharName);					
				}
				else if(byRetVal == 2)
				{
					dwInsert++;
					MsgPrint("[%s] [%s] insert query execute succeed!\n", m_pDBInfo[n].szDBName, (*it)->szCharName);					
				}				
			}
		}

		MsgPrint("[%s] Total/Update/Insert : %d/%d/%d\n", m_pDBInfo[n].szDBName, dwNumMaxData, dwUpdate, dwInsert);
		dwUpdate = 0;
		dwInsert = 0;
	}
}

void CCharListUpdateToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	Finialize_Parser();
	
	// TODO: Add your message handler code here
	
}

void CCharListUpdateToolDlg::OnAllcancelbtn() 
{
	// TODO: Add your control notification handler code here
	
	for(DWORD n = 0 ; n < m_dwSetServerCount ; n++)
	{
		m_SelectListBox.SetCheck(n, 0);
	}
}

void CCharListUpdateToolDlg::OnAllselectbtn() 
{
	// TODO: Add your control notification handler code here
	for(DWORD n = 0 ; n < m_dwSetServerCount ; n++)
	{
		m_SelectListBox.SetCheck(n, BST_CHECKED);
	}
}


BOOL CCharListUpdateToolDlg::Load84YearBirthUser(char* pCharFile)
{
	FILE		*fp		=	NULL;
	CHAR		*pToken	=	NULL;
	CHAR		szBuffer[1024];

	fp = fopen(pCharFile, "r");
	if( fp == NULL )
	{
		m_bFileOpen = FALSE;
		return FALSE;
	}

	SHORT snResult = 0;

	CharHead*	pTempCharHead = NULL;

	while( fgets( szBuffer, 1024, fp ) )
	{
		pToken= strtok( szBuffer, "\n\t\r" );

		snResult = m_pAdoConn[m_dwSetServerCount-1].Execution(1001, pToken);

		if(snResult == 2)
		{
			MsgPrint("[%s] not existence\n", pToken);

			m_LogWriter.PrintF("Not existence.Log", "[%s] not existence\n", pToken);
		}
		else if(snResult == 1)
		{
			MsgPrint("[%s] insert failed\n", pToken);

			m_LogWriter.PrintF("insert failed.Log", "[%s] insert failed\n", pToken);
		}
		else
		{		
			MsgPrint("[%s] insert succeed!\n", pToken);
		}
	}

	fclose(fp);
	m_bFileOpen = TRUE;
	return TRUE;
}
