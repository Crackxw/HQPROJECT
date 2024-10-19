// PatchToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchTool.h"
#include "PatchToolDlg.h"

#include "SelectInfo.h"
#include "ListSelectData.h"
#include "DirDialog.h"
#include "DBJobRecord.h"
#include "MyAboutDlg.h"
#include "AttachPathDlg.h"
#include "LoginDlg.h"
#include "GersangServerPatch.h"
#include "DestinationPathDailog.h"
#include "..\ServerMonitor\monitorprotocol.h"

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
// CPatchToolDlg dialog

CPatchToolDlg::CPatchToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSelectInfo = NULL;
	m_pData = NULL;
	m_pDesPatch = NULL;
	m_nPatchCount = 0;
	m_strCurTime = _T("");
	memset(&m_SrcPatch, 0, sizeof(_S_PATCH_INFO));
	m_strAttachPath = _T("");

	try{
		m_pSelectInfo = new CSelectInfo;
	}
	catch(std::bad_alloc){
		_ASSERTE(0);
	}
	try{
		m_pData = new CListSelectData(&m_pSelectInfo->m_asNames);
	}
	catch(std::bad_alloc){
		_ASSERTE(0);
	}
}

void CPatchToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchToolDlg)
	DDX_Control(pDX, IDC_FILETRANSFRERNAMESTATIC, m_FileTransferName);	
	DDX_Control(pDX, IDC_MSGDISPLAYLIST, m_MsgDisplayList);
	DDX_Control(pDX, IDC_SOURCEDIRECTORYPATHEDIT, m_SourceDirectoryPathEditCtrl);
	DDX_Control(pDX, IDC_SERVERSELECTLIST, m_SelectListBox);
	DDX_Control(pDX, IDC_COPYFILEPROGRESS, m_CopyFileProgressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPatchToolDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SOURCEDIRECTORYPATHBTN, OnSourcedirectorypathbtn)
	ON_BN_CLICKED(IDC_ALL_FILE_ADD_BTN, OnAllFileAddBtn)
	ON_BN_CLICKED(IDC_ALL_FILE_REMOVE_BTN, OnAllFileRemoveBtn)
	ON_BN_CLICKED(IDC_ALLPATCHCHOICEBTN, OnAllpatchchoicebtn)
	ON_BN_CLICKED(IDC_CHOICEPATCHCANCELBTN, OnChoicepatchcancelbtn)
	ON_BN_CLICKED(IDC_DELETEPATCHBTN, OnDeletepatchbtn)
	ON_BN_CLICKED(IDC_STARTPATCHBTN, OnStartpatchbtn)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_ATTACHRADIO, OnAttachradio)
	ON_BN_CLICKED(IDC_NEWVISIONRADIO, OnNewvisionradio)
	ON_BN_CLICKED(IDC_MASTER_SERVER_CONNECT, OnMasterServerConnect)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchToolDlg message handlers

BOOL CPatchToolDlg::OnInitDialog()
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

	m_byNewVersion = 1;
	CButton* pNewButton = static_cast<CButton*>(GetDlgItem(IDC_NEWVISIONRADIO));
	pNewButton->SetCheck(TRUE);

	Initialize();
	Init_DestinationPath();
	Init_PatchDestinationPath();

	// Progress control initialize.
	m_CopyFileProgressCtrl.SetRange(0, 100);
	m_CopyFileProgressCtrl.SetStep(1);
	m_CopyFileProgressCtrl.SetBkColour(RGB(125, 125, 125));
	m_CopyFileProgressCtrl.SetTextForeColour(RGB(255, 255, 255));
	m_CopyFileProgressCtrl.SetShowText(TRUE);
	m_CopyFileProgressCtrl.SetWindowText("0/0 Bytes");

	SetTimer(SERVER_PATCH_TIMER_ID, 128, NULL);
	SetTimer(SERVER_REFRESH_TIMER_ID,1000,NULL);	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPatchToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPatchToolDlg::OnPaint() 
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
HCURSOR CPatchToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Initialize.
//
void CPatchToolDlg::Initialize()
{
	////////////////////////////////////////////////////////////////////////////////
	// Subclass the list controls///////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	m_SourceFileList.SubclassDlgItem( IDC_SOURCEFILELIST, this );
	m_TargetFileList.SubclassDlgItem( IDC_TARGETFILELIST, this );	

	////////////////////////////////////////////////////////////////////////////////
	// Create one column for th "List fo var" list view contor./////////////////////
	////////////////////////////////////////////////////////////////////////////////
	CString s(TCHAR('M'),29);
	int len = m_SourceFileList.GetStringWidth(s)+15;
	LV_COLUMN lvC;
	lvC.mask = LVCF_FMT | LVCF_WIDTH;
	lvC.fmt  = LVCFMT_LEFT;  // Left-align column
	lvC.cx   = len;          // Width of column in pixels
	if (m_SourceFileList.InsertColumn(0,&lvC) == -1)
		return;

	m_SourceFileList.DeleteAllItems();

	m_SourceFileList.SetLocalDD(FALSE);   // Disable local Drag&Drop
	m_SourceFileList.SetScrolling(FALSE); // and auto scrolling

    if (m_TargetFileList.InsertColumn(0,&lvC) == -1)
		return;
	m_TargetFileList.DeleteAllItems();

	////////////////////////////////////////////////////////////////////////////////
	// Set the content of the list view controls.///////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	CWordArray aVars;
	int size = (int)m_pSelectInfo->m_asNames.GetSize();
	aVars.SetSize(size);
	// Initially the list of available variables contains all variables
	for (int i=0; i<size; i++)
		aVars[i] = i;
   
	////////////////////////////////////////////////////////////////////////////////
	// Fill the list of variables.//////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	m_pData->FillListCtrl(m_SourceFileList, aVars);
	m_pData->FillListCtrl(m_TargetFileList, m_pSelectInfo->m_aRowsVar);
	
	////////////////////////////////////////////////////////////////////////////////
	// Initialize the list controls for Drag&Drop.//////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////	
//	m_SourceFileList.Initialize();
	m_TargetFileList.Initialize();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 원본 파일 리스트 출력.(Local 용)
//
void CPatchToolDlg::FileListDraw()
{
	CFileFind	finder;
	CString		strSourceDir(m_SourceDirectoryPath+_T("\\"));
	CString		strSearchParam = strSourceDir+_T("*.*");
	CString		strFileName;
	int			nRow = 0;
	CWordArray	aVars;

	m_SourceFileList.DeleteAllItems();
	m_TargetFileList.DeleteAllItems();
	m_pSelectInfo->m_asNames.RemoveAll();

	BOOL bWorking = finder.FindFile(strSearchParam);

	while(bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		strFileName = finder.GetFileName();
		m_pSelectInfo->m_asNames.SetAtGrow(nRow, strFileName);
		nRow++;
	}
	
	int nSize = (int)m_pSelectInfo->m_asNames.GetSize();
	aVars.SetSize(nSize);

	for(int n = 0 ; n < nSize ; n++)
		aVars[n] = n;

	m_pData->FillListCtrl(m_SourceFileList, aVars);

	finder.Close();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Config 파일을 읽어 경로를 초기화 한다.(Local 용)
//
void CPatchToolDlg::Init_DestinationPath()
{
	SetConfigFileName(DEFAULT_CONFIG_FILE);
//	m_nPatchCount = GetPatchCount();
//
//	// Destination directory setting.
//	if(m_nPatchCount > 0){
//		m_pDesPatch = new _S_PATCH_INFO[m_nPatchCount];
//		for(int n = 0 ; n < m_nPatchCount ; n++){
//			strncpy(m_pDesPatch[n].szDir, GetPatchDir(n+1), _MAX_DIR);
//			strncpy(m_pDesPatch[n].szSettingDir, GetSettingDir(n+1), _MAX_DIR);
//			strncpy(m_pDesPatch[n].szServerName, GetServerName(n+1), SERVER_NAME_MAX_SIZE);
//
//			m_SelectListBox.AddString(m_pDesPatch[n].szServerName);
//		}
//	}

	// Source directory setting.
	strncpy(m_SrcPatch.szDir, GetSourcePatchDir(), _MAX_DIR);
	m_SourceDirectoryPath = m_SrcPatch.szDir;

	if(m_SourceDirectoryPath.GetLength() != 0){
		m_SourceDirectoryPathEditCtrl.SetWindowText(m_SourceDirectoryPath);
		FileListDraw();
	}

	// m_DBRecord.Open();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 소스 파일 경로를 지정한다.
//
void CPatchToolDlg::OnSourcedirectorypathbtn() 
{	
	CDirDialog dlgDir;

	dlgDir.m_strTitle = _T("Select directory for file search");
	UpdateData(TRUE);

	if(dlgDir.DoBrowse(this) == IDOK){
		m_SourceDirectoryPath = dlgDir.m_strPath;
		m_SourceDirectoryPathEditCtrl.SetWindowText(m_SourceDirectoryPath);

		FileListDraw();
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일을 삭제한다.(Local 용)
//
BOOL CPatchToolDlg::OnDeleteDirectorys()
{
	CListCtrl* pTargetList = static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));
	int nCopyFileCount = pTargetList->GetItemCount();
	int	nRealPatchCount = GetRealPatchCount();
	int nCheck = 0;
	char szMemssage[256] = {0, };
	BOOL bRetVal = TRUE;

	m_CopyFileProgressCtrl.SetPos(0);

	for(int n = 0 ; n < m_nPatchCount ; n++){
		nCheck = m_SelectListBox.GetCheck(n);
		if(nCheck != BST_CHECKED) continue;

		CString strDestDir = m_pDesPatch[n].szDir+(_T("\\")+m_strCurTime);
		m_stackDirectoryPath.push(strDestDir);
		
		bRetVal = DeleteFiles(strDestDir+_T("\\"));

		if(bRetVal == FALSE) return bRetVal;
	}

	// Directorys delete.
	RemoveDirectorys();

	m_CopyFileProgressCtrl.SetPos(0);
	MessageBox("Delete completion.", "Message", MB_OK);

	return bRetVal;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 폴더를 삭제한다.
//
BOOL CPatchToolDlg::RemoveDirectorys()
{
	CString strDirectory;
	BOOL bRetVal = TRUE;

	while(!m_stackDirectoryPath.empty()){
		strDirectory = m_stackDirectoryPath.top();
		m_stackDirectoryPath.pop();
		bRetVal = RemoveDirectory(strDirectory);
		if(!bRetVal) break;
		m_CopyFileProgressCtrl.StepIt();
	}

	return bRetVal;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일을 삭제한다.(Local 용)
//
BOOL CPatchToolDlg::DeleteFiles(CString strDestinationDirectory)
{
	int nPercent = 0;
	CString strFileFinderName;
	CString ddir(strDestinationDirectory);
	CString source_subfile;
	CString destination_subfile;
	CString filename;
	char szMessage[256] = {0,};
	DWORD dwAttrs;
	CFileFind finder;
	BOOL bRetVal = TRUE;

	CString search_param = ddir+_T("*.*");
	BOOL bWorking = finder.FindFile(search_param);
	
	while(bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		filename = finder.GetFileName();
		destination_subfile = ddir+filename;

		dwAttrs = GetFileAttributes(ddir);
		if(!(dwAttrs & FILE_ATTRIBUTE_NORMAL)){ // if read only.
			SetFileAttributes(destination_subfile, // file normal attributes.
				dwAttrs | FILE_ATTRIBUTE_NORMAL);
		}

		if(finder.IsDirectory()){ // if directory.
			m_stackDirectoryPath.push(destination_subfile);
			DeleteFiles(destination_subfile+_T("\\"));
		}
		else{
			bRetVal = DeleteFile(destination_subfile);
			if(!bRetVal){ // if file delete failed.
				bWorking = FALSE;
				sprintf(szMessage, "Cannot delete from the %s file. Do you want delete stop?", filename);
				if(AfxMessageBox(szMessage, MB_YESNO) == IDYES){
					m_CopyFileProgressCtrl.SetPos(0);
					break;
				}
			}
		}		
		m_CopyFileProgressCtrl.StepIt();
	}

	finder.Close();

	return bRetVal;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 세팅 파일을 복사한다.(Local 용)
//
BOOL CPatchToolDlg::CopySettingFile(int n, CString strDestinationDirectory)
{
	CFileFind finder;
	CString strTemp(m_pDesPatch[n].szSettingDir);
	CString strFileName;
	BOOL bRetVal = FALSE;
	DWORD dwAttrs;
	
	CString strSearchFile(strTemp+_T("\\*.*"));
	BOOL bWorking = finder.FindFile(strSearchFile);

	while(bWorking){
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;
		strFileName = finder.GetFileName();
		CString strSrcFilePath = 
			m_pDesPatch[n].szSettingDir+(_T("\\")+strFileName);
		CString strDesFilePath = 
			strDestinationDirectory+(_T("\\")+strFileName);
		
		bRetVal = CopyFile(strSrcFilePath, strDesFilePath, FALSE);

		if(!bRetVal){
			char szMessage[256]={0,};
			sprintf( szMessage, "Cannot patch from the %s setting file. Do you want to patch stop?", 
				strDesFilePath.GetBuffer(0));

			if( AfxMessageBox( szMessage, MB_YESNO ) == IDYES ){
				m_CopyFileProgressCtrl.SetPos( 0 );
				finder.Close();
				return FALSE;
			}
			else continue;
		}
		dwAttrs = GetFileAttributes(strTemp+_T("\\"));
		if (!(dwAttrs & FILE_ATTRIBUTE_NORMAL)){ // if read only.
			SetFileAttributes(strDesFilePath, // file normal attributes.
				dwAttrs | FILE_ATTRIBUTE_NORMAL);
		}
	}

	return TRUE;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 복사를 시작한다.(Local 용)
//
BOOL CPatchToolDlg::OnCopyDirectorys()
{
//	if( IsPatchEnable() == FALSE )
//		return FALSE;

	CListCtrl* pTargetList = static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));
	int nCopyFileCount = pTargetList->GetItemCount();
	int nRealPatchCount = GetRealPatchCount();
	int nCheck = 0;
	int nPercent = 1;
	int nOldPercent = 0;
	int nCount = 1;
	int nPatchCount = 0;
	char szMessage[256] = {0,};
	char szServer[JOB_APPLYSERVER_MAX_SIZE]={0,};
	int nLen = 0;
	m_CopyFileProgressCtrl.SetPos(0);
	BOOL bPatch = FALSE;
	int  nOneLen = 0;

	// New version.
	if(m_byNewVersion == 1){	
		// Get current time.
		CTime t = CTime::GetCurrentTime();
		m_strCurTime = t.Format("%Y%m%d%H%M");
	}
	// Attach version.
	else{
		m_strCurTime = _T("");
		CAttachPathDlg dlgAttach;
		dlgAttach.SetAttach(GetAttachDir());
		if(dlgAttach.DoModal() == IDOK)
		{
			if(dlgAttach.GetAttach(m_strAttachPath) == FALSE)
			{
				MessageBox("Path input.", "Message", MB_OK);
				return FALSE;
			}
			// Modify file save.
			SetAttachPath(m_strAttachPath);
		}
		else
			return FALSE;
	}

	for( int i = 0 ; i < m_nPatchCount ; i++ ){
		nCheck = m_SelectListBox.GetCheck(i);
		if( nCheck != BST_CHECKED ) continue;

		CString strDescDir;
		// New version.
		if(m_byNewVersion == 1)
			strDescDir = m_pDesPatch[i].szDir+(_T("\\")+m_strCurTime);
		// Attach version.
		else
			strDescDir = m_pDesPatch[i].szDir+(_T("\\")+m_strAttachPath);

		// New version.
		if(m_byNewVersion == 1)
			CreateDirectory(strDescDir, NULL);
		
		BOOL b = CopyDirectorys(m_SourceDirectoryPath+_T("\\"), 
			strDescDir+_T("\\"), nCount, i );

		if( b == FALSE ){
			sprintf( szMessage, "Error happening. Do you want to patch stop?" );
			if( AfxMessageBox( szMessage, MB_YESNO ) == IDYES ){
				m_CopyFileProgressCtrl.SetPos(0);
				return FALSE;
			}
			else continue;
		}

		// Setting file copy.
		if( CopySettingFile( i, strDescDir ) == FALSE )
			return FALSE;
		nOneLen = (int)strlen( m_pDesPatch[i].szServerName );
		if( (nLen+nOneLen) < (JOB_APPLYSERVER_MAX_SIZE-1))
			nLen += sprintf( szServer+nLen, "%s;", m_pDesPatch[i].szServerName );

		bPatch = TRUE;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Job log record.	
//	if( bPatch == TRUE ){
//		CString strMaker;
//		CString strSubstance;
//		DWORD dwJobID = (DWORD)time(NULL);
//		m_JobMakerEditCtrl.GetWindowText(strMaker);
//		m_JobSubstanceEditCtrl.GetWindowText(strSubstance);
//		bool bRetVal = m_DBRecord.InsertJobLog(dwJobID, strMaker, strSubstance, szServer);
//		if( bRetVal == false ){
//			memset( szMessage, 0, sizeof( szMessage ) );
//			sprintf( szMessage, "Patch success and each DB record failed.", dwJobID );
//			MessageBox( szMessage, "Message", MB_OK );
//		}
//	}
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	m_CopyFileProgressCtrl.SetPos( 100 );
	MessageBox( "Patch completion.", "Message", MB_OK );

	return TRUE;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Job progress.
//
void CPatchToolDlg::JobProgressBar( int& nCount, int nPatchCount )
{
	m_CopyFileProgressCtrl.StepIt();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일 복사한다. 재귀호출 부분(Local 용)
//
BOOL CPatchToolDlg::CopyDirectorys( LPCTSTR strSourceDirectory, 
								    LPCTSTR strDestinationDirectory,
									int& nCount, 
									int nPatchCount)
{
	BOOL b = TRUE;
	int nPercent = 0;
	CString strFileFinderName;
	CString sdir(strSourceDirectory);
	CString ddir(strDestinationDirectory);
	CString source_subfile;
	CString destination_subfile;
	CString filename;
	char szMessage[256]={0,};
	DWORD dwAttrs;	
	CFileFind finder;	

	CString search_param = sdir + _T("*.*");
	BOOL bWorking = finder.FindFile(search_param);

	while( bWorking ){
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		filename = finder.GetFileName();
		if( !IsCopyFile( filename ) ){
			if( (m_SourceDirectoryPath+_T("\\")) == sdir )
				continue;
		}

		source_subfile = sdir + filename;
		destination_subfile = ddir + filename;

		JobProgressBar( nCount, nPatchCount ); // job progress.

		if( finder.IsDirectory()){
			CreateDirectory( destination_subfile,NULL );
			CopyDirectorys( source_subfile+_T("\\"), \
				destination_subfile+_T("\\"), nCount, nPatchCount );
		}
		else{
			b = CopyFile(source_subfile, destination_subfile, FALSE);
			if( b == FALSE ){ // if patch failed.
				bWorking = FALSE;
			}
			else{ // patch succeed.
				dwAttrs = GetFileAttributes(strSourceDirectory);
				if (!(dwAttrs & FILE_ATTRIBUTE_NORMAL)){ // if read only.
					SetFileAttributes(destination_subfile, // file normal attributes.
						dwAttrs | FILE_ATTRIBUTE_NORMAL);
				}
			}
		}
	}

	finder.Close();

	return b;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일을 복사 가능한지 체크한다.
//
BOOL CPatchToolDlg::IsCopyFile( CString strFileName )
{
	CListCtrl* pTargetList = 
		static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));
	
	int nCopyFileCount = pTargetList->GetItemCount();

	CString filename;
	for( int n = 0 ; n < nCopyFileCount ; n++ ){
		filename = pTargetList->GetItemText( n, 0 );

		if( filename == strFileName )
			return TRUE;
	}
	return FALSE;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 
//
BOOL CPatchToolDlg::IsPatchEnable()
{
	return TRUE;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 원본 파일을 모두 선택한다.
//
void CPatchToolDlg::OnAllFileAddBtn() 
{	
	CListCtrl* pSourceList = 
		static_cast<CListCtrl*>(GetDlgItem(IDC_SOURCEFILELIST));
	CListCtrl* pTargetList = 
		static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));\
		
	int nCount = m_SourceFileList.GetItemCount();
	CString strFile;
	CWordArray aVars;

	m_TargetFileList.DeleteAllItems();
	for( int n = 0 ; n < nCount ; n++ ){
		strFile = pSourceList->GetItemText( n, 0 );
		m_pSelectInfo->m_asNames.SetAtGrow( n, strFile );
	}

	int size = (int)m_pSelectInfo->m_asNames.GetSize();
	aVars.SetSize(size);
	for (int i=0; i<size; i++)
		aVars[i] = i;
	m_pData->FillListCtrl(m_TargetFileList, aVars);

	// All delete Source list.
	m_SourceFileList.DeleteAllItems();
//	m_pSelectInfo->m_asNames.RemoveAll();
	
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 선택한 모든 파일을 취소한다.
//
void CPatchToolDlg::OnAllFileRemoveBtn() 
{	
	CListCtrl* pSourceList = static_cast<CListCtrl*>(GetDlgItem(IDC_SOURCEFILELIST));
	CListCtrl* pTargetList = static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));
	int nCount = m_TargetFileList.GetItemCount();
	CString strFile;
	CWordArray aVars;

	m_SourceFileList.DeleteAllItems();
	for( int n = 0 ; n < nCount ; n++ ){
		strFile = pTargetList->GetItemText( n, 0 );
		m_pSelectInfo->m_asNames.SetAtGrow( n, strFile );
	}

	int size = (int)m_pSelectInfo->m_asNames.GetSize();
	aVars.SetSize(size);
	for (int i=0; i<size; i++)
		aVars[i] = i;
	m_pData->FillListCtrl(m_SourceFileList, aVars);

	// All delete Source list.
	m_TargetFileList.DeleteAllItems();
//	m_pSelectInfo->m_asNames.RemoveAll();	
}

void CPatchToolDlg::OnAllpatchchoicebtn() 
{	
	for(int n = 0 ; n < m_nPatchCount ; n++){
		m_SelectListBox.SetCheck(n, BST_CHECKED);
	}
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 모든 서버를 선택한다.
//
void CPatchToolDlg::OnChoicepatchcancelbtn() 
{	
	for(int n = 0 ; n < m_nPatchCount ; n++){
		m_SelectListBox.SetCheck(n, 0);
	}	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일을 삭제를 시작한다.
//
void CPatchToolDlg::OnDeletepatchbtn() 
{
	EnableWindowCtrl(FALSE);
	OnDeleteDirectorys();
	EnableWindowCtrl(TRUE);
	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Enable window control.
//
void CPatchToolDlg::EnableWindowCtrl(BOOL bEnable)
{
	CButton* pButton = static_cast<CButton*>(GetDlgItem(IDC_SOURCEDIRECTORYPATHBTN));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_STARTPATCHBTN));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_DELETEPATCHBTN));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_ALLPATCHCHOICEBTN));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_CHOICEPATCHCANCELBTN));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_ABOUT));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDCANCEL));
	pButton->EnableWindow( bEnable );
	pButton = static_cast<CButton*>(GetDlgItem(IDC_PATCHSUBSTANCECONFIIRMBTN));
	pButton->EnableWindow( bEnable );
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 도움말 다이아로그 출력.
//
void CPatchToolDlg::OnAbout() 
{	
	CMyAboutDlg dlg;
	dlg.DoModal();
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 새 버전
//
void CPatchToolDlg::OnAttachradio() 
{
	m_byNewVersion = 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 덮어쓰기 버전
//
void CPatchToolDlg::OnNewvisionradio() 
{
	m_byNewVersion = 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 프로그램을 종료한다.
//
void CPatchToolDlg::OnCancel() 
{	
	SetSourcePath(m_SourceDirectoryPath);

	CDialog::OnCancel();
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 마스터 서버에 연결한다.
//
void CPatchToolDlg::OnMasterServerConnect() 
{
	g_GersangServerPatch.SetAddress(GetMasterServerIP(), 
		GetMasterServerPort());
	
	CLoginDlg dlgLogin;

	if(dlgLogin.DoModal() == IDOK)
	{
		ZeroMemory(g_GersangServerPatch.m_szID, 256);
		ZeroMemory(g_GersangServerPatch.m_szPassword, 256);

		dlgLogin.SetID(g_GersangServerPatch.m_szID);
		dlgLogin.SetPW(g_GersangServerPatch.m_szPassword);

		g_GersangServerPatch.ConnectToServer();
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 대상 파일 경로를 Config에 저장한다.
//
void CPatchToolDlg::SetDestinationPathSave(BYTE byRetVal, CString strDestPath)
{
	// 경로 추가
	if(byRetVal == 1 && (m_wDestPathCount+1) < 32)
	{
		m_pDestPath[m_wDestPathCount++] = strDestPath;		
		m_DestPathConfig.SetPathCount(m_wDestPathCount);
		m_DestPathConfig.SetDestinationPath(m_wDestPathCount, strDestPath);
	}
	// 첫번째 경로 업데이트
	else if(byRetVal == 2 && m_wDestPathCount > 0)
	{
		m_pDestPath[0] = strDestPath;
		m_DestPathConfig.SetDestinationPath(1, strDestPath);
	}
	// 첫번째 경로와 마지막 경로를 바꾼다.
	else if(byRetVal == 0 && m_wDestPathCount > 0)
	{
		if(m_wDestPathCount == 1)
		{
			m_pDestPath[0] = strDestPath;
			m_DestPathConfig.SetDestinationPath(1, strDestPath);
		}
		else
		{
			CString strTemp;
			strTemp = m_pDestPath[0];
			m_pDestPath[0] = m_pDestPath[m_wDestPathCount-1];
			m_pDestPath[m_wDestPathCount-1] = strTemp;

			m_DestPathConfig.SetDestinationPath
				(
					m_wDestPathCount, 
					m_pDestPath[m_wDestPathCount-1]
				);

			m_DestPathConfig.SetDestinationPath
				(
					1, 
					m_pDestPath[0]
				);
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 패치를 시작한다.
//
void CPatchToolDlg::OnStartpatchbtn()
{
	m_CopyFileProgressCtrl.SetPos(0);

	CDestinationPathDailog dlgDestDir;

	dlgDestDir.SetDestinationPath(m_wDestPathCount, m_pDestPath);
	int nRetVal = dlgDestDir.DoModal();

	if(nRetVal != IDOK)
		return;

	CString		strDestPath;
	BYTE		byRetVal		= dlgDestDir.SetModifyPath(strDestPath);

	SetDestinationPathSave(byRetVal, strDestPath);
	// New version.
	if(m_byNewVersion == 1){
		CTime t = CTime::GetCurrentTime();
		CString strCurTime = t.Format("%Y%m%d%H%M");

		strDestPath = strDestPath+(_T("\\")+strCurTime);

		char szMessageBox[512]={0,};
		_stprintf(szMessageBox, "새 버전 %s에 패치 하겠습니까?", strDestPath );
		if(AfxMessageBox(szMessageBox, MB_YESNO) == IDNO)
			return;

		MsgPrint("New version.");
	}
	else
		MsgPrint("Attach version.");

	CListCtrl* pTargetList = 
		static_cast<CListCtrl*>(GetDlgItem(IDC_TARGETFILELIST));
	int nCount = m_TargetFileList.GetItemCount();

	if(nCount == 0){
		MsgPrint("Select no file.");
		return;
	}

	WORD wServerList[512]	= {0,};
	WORD wServerCount		= 0;

	CString strFile;
	
	m_dwTickCount = GetTickCount();
	for( int n = 0 ; n < nCount ; n++ )
	{
		strFile = pTargetList->GetItemText( n, 0 );

		wServerCount = GetSelectedServer(wServerList);
		// Transfer start.
		g_GersangServerPatch.SendFile
			(
				wServerCount, 
				wServerList,  
				m_byNewVersion, 
				strDestPath, 
				m_SourceDirectoryPath+(_T("\\")+strFile), 
				strFile
			);
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 선택한 서버 코드를 얻어온다.
//
WORD CPatchToolDlg::GetSelectedServer(WORD* pServerList)
{
	int  nChecked		= 0;
	WORD wServerCount	= 0;
	int nCount			= m_SelectListBox.GetCount();
	int nRealCount		= GetRealPatchCount();

	for (int n = 0; n < nCount; n++)
	{
		nChecked = m_SelectListBox.GetCheck(n);
		if(nChecked != BST_CHECKED) continue;
		
		int groupnum = n / nRealCount;
		int kind = n % nRealCount;

		pServerList[wServerCount] = MAKESERVERCODE(groupnum,kind);
		
		wServerCount++;
	}

	return wServerCount;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 서버 패치 완료를 알려준다.
//
void CPatchToolDlg::FileTransferState(
										int nGropNum,  
										const char* pFileName
									 )
{
	if(nGropNum < 0 || nGropNum >= m_nPatchCount)
		return;

	MsgPrint("Send lead Time:%d", GetTickCount() - m_dwTickCount);
	MsgPrint(_T("%s transferred to %s server successfully."),
		pFileName, g_GersangServerPatch.m_szServerName[nGropNum] );
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 패치할 서버를 추가한다.
//
void CPatchToolDlg::AddServer()
{
	m_nPatchCount = g_GersangServerPatch.GetServerGroupNum();
	for(int n = 0 ; n <	g_GersangServerPatch.GetServerGroupNum() ; n++)
	{
		m_SelectListBox.AddString(g_GersangServerPatch.m_szServerName[n]);
	}

	m_SelectListBox.SetCaretIndex(g_GersangServerPatch.GetServerGroupNum()-1);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 선택한 서버 개수를 얻는다.
//
int CPatchToolDlg::GetRealPatchCount()
{
	int nCheck			= 0;
	int nCheckCount		= 0;

	for( int n = 0 ; n < m_nPatchCount ; n++ ){
		nCheck = m_SelectListBox.GetCheck( n );
		if( nCheck == BST_CHECKED )
			nCheckCount++;
	}

	return nCheckCount;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 지금까지 선택된 파일경로를 구한다.
//
void CPatchToolDlg::Init_PatchDestinationPath()
{
	m_DestPathConfig.SetConfigFileName(DESTINATION_PATH_CONFIG_FILE);

	m_wDestPathCount = m_DestPathConfig.GetPathCount();

	if		(m_wDestPathCount < 0 )		return;
	else if	(m_wDestPathCount > 32)		m_wDestPathCount = 32;

	for(int n = 0 ; n < m_wDestPathCount ; n++)
	{
		m_pDestPath[n] = m_DestPathConfig.GetDestinationPath(n+1);
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 타이머 함수.
//
void CPatchToolDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == SERVER_PATCH_TIMER_ID)
	{
		g_GersangServerPatch.Process();
	}
	else if(nIDEvent == SERVER_REFRESH_TIMER_ID)
	{
		if(g_GersangServerPatch.IsConnected() == TRUE)
			TransmissionPercentFile();
	}
	
	CDialog::OnTimer(nIDEvent);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 메세지를 리스트 박스에 출력한다.
//
void CPatchToolDlg::MsgPrint(const TCHAR* szFormat,...)
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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 파일 전송 진행 사항을 보여준다.
//
void CPatchToolDlg::TransmissionPercentFile()
{
	TCHAR szFileState[1024] = {0,};
	int nPercent = 0;

	int	nSendByte		= g_GersangServerPatch.m_siSendFileByte;
	int nTolSendByte	= g_GersangServerPatch.m_siSendFileByteTotal;

	_stprintf
		(
			szFileState,
			_T("File Name : %s"),
			g_GersangServerPatch.m_szTransferFileNameNoPath
		);
		
	m_FileTransferName.SetWindowText(szFileState);
	
	_stprintf
		(
			szFileState,
			_T("File Sended Byte : %ld / %ld"),
			nSendByte,
			nTolSendByte
		);
	
	m_CopyFileProgressCtrl.SetWindowText(szFileState);

	if(!(nTolSendByte == 0 || nSendByte == 0))
	{
		nPercent = (nSendByte*100) / nTolSendByte;
		m_CopyFileProgressCtrl.SetPos(nPercent);
	}	
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
BOOL CPatchToolDlg::DestroyWindow() 
{	
	KillTimer(SERVER_PATCH_TIMER_ID);
	KillTimer(SERVER_REFRESH_TIMER_ID);

	if( m_pData != NULL ){ delete m_pData; m_pData = NULL; }
	if( m_pSelectInfo != NULL ){ delete m_pSelectInfo; m_pSelectInfo = NULL; }
	if( m_pDesPatch != NULL ){ delete[] m_pDesPatch; m_pDesPatch = NULL; }	

	return CDialog::DestroyWindow();
}