// CrashTopFormView.cpp : implementation file
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "CrashTopFormView.h"
#include "MainFrm.h"
#include "CrashMonitorClientView.h"

#include "..\\CrashMailProtocol\\CrashMailProtocol.h"
#include "..\\XCommon\\JXConnectSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashTopFormView

IMPLEMENT_DYNCREATE(CCrashTopFormView, CFormView)

CCrashTopFormView::CCrashTopFormView()
	: CFormView(CCrashTopFormView::IDD), m_pConnection(NULL), m_bDlgOpen(FALSE)
{
	//{{AFX_DATA_INIT(CCrashTopFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCrashTopFormView::~CCrashTopFormView()
{
}

void CCrashTopFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrashTopFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	
	DDX_Control(pDX, IDC_FILEDSELECTLIST, m_FieldSelectList);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrashTopFormView, CFormView)
	//{{AFX_MSG_MAP(CCrashTopFormView)
	ON_BN_CLICKED(IDC_FIELDSELECTOKBTN, OnFieldselectokbtn)
	ON_BN_CLICKED(IDC_FIELDSELECTCANCELBTN, OnFieldselectcancelbtn)
	ON_BN_CLICKED(IDC_SELECTWHERECHECK1, OnSelectwherecheck1)
	ON_BN_CLICKED(IDC_SELECTWHERECHECK2, OnSelectwherecheck2)
	ON_BN_CLICKED(IDC_SELECTWHERECHECK3, OnSelectwherecheck3)
	ON_BN_CLICKED(IDC_SELECTWHERECHECK4, OnSelectwherecheck4)
	ON_BN_CLICKED(IDC_ALLFIELDOPT, OnAllfieldopt)
	ON_BN_CLICKED(IDC_ALLWHEREOPT, OnAllwhereopt)
	ON_BN_CLICKED(IDC_SEARCHBTN, OnSearchbtn)
	ON_LBN_SELCHANGE(IDC_FILEDSELECTLIST, OnSelchangeFiledselectlist)
	ON_CBN_SELCHANGE(IDC_CRASHPAGENOCOMBO, OnSelchangeCrashpagenocombo)	
	ON_BN_CLICKED(IDC_SELECTWHERECHECK5, OnSelectwherecheck5)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashTopFormView diagnostics

#ifdef _DEBUG
void CCrashTopFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCrashTopFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCrashTopFormView message handlers

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	InitFormView();

	((CButton*)GetDlgItem(IDC_ALLFIELDOPT))->SetCheck(TRUE);	
	InitCtrlData();
}

/////////////////////////////////////////////////////////////////////////////

CMainFrame* CCrashTopFormView::GetMainFrm()
{
	CCrashMonitorClientApp * pApp = ( CCrashMonitorClientApp * )AfxGetApp();
	if( pApp == NULL )	return NULL;
	CMainFrame * pFrm = NULL;
	pFrm = ( CMainFrame * )pApp->GetMainWnd();
	return pFrm;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::InitCtrlData()
{
	GetDlgItem(IDC_FIELDSELECTEDIT1)->SetWindowText("Crash Type");
	GetDlgItem(IDC_FIELDSELECTEDIT2)->SetWindowText("Crash Address");
	GetDlgItem(IDC_FIELDSELECTEDIT3)->SetWindowText("Line  Address");
	GetDlgItem(IDC_FIELDSELECTEDIT4)->SetWindowText("Crash Time");
	GetDlgItem(IDC_FIELDSELECTEDIT5)->SetWindowText("Play Time");
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::InitFormView()
{
	m_dwWhereCtrnID[0][0] = IDC_SELECTWHERECHECK1;
	m_dwWhereCtrnID[0][1] = 0;
	m_dwWhereCtrnID[0][2] = IDC_FIELDSELECTEDIT1;
	m_dwWhereCtrnID[0][3] = IDC_FIELDOPERATORCOMBO1;
	m_dwWhereCtrnID[0][4] = IDC_FIELDVALUECOMBO1;	

	m_dwWhereCtrnID[1][0] = IDC_SELECTWHERECHECK2;
	m_dwWhereCtrnID[1][1] = IDC_FIELDANDORCOMBO1;
	m_dwWhereCtrnID[1][2] = IDC_FIELDSELECTEDIT2;
	m_dwWhereCtrnID[1][3] = IDC_FIELDOPERATORCOMBO2;
	m_dwWhereCtrnID[1][4] = IDC_FIELDVALUECOMBO2;

	m_dwWhereCtrnID[2][0] = IDC_SELECTWHERECHECK3;
	m_dwWhereCtrnID[2][1] = IDC_FIELDANDORCOMBO2;
	m_dwWhereCtrnID[2][2] = IDC_FIELDSELECTEDIT3;
	m_dwWhereCtrnID[2][3] = IDC_FIELDOPERATORCOMBO3;
	m_dwWhereCtrnID[2][4] = IDC_FIELDVALUECOMBO3;

	m_dwWhereCtrnID[3][0] = IDC_SELECTWHERECHECK4;
	m_dwWhereCtrnID[3][1] = IDC_FIELDANDORCOMBO3;
	m_dwWhereCtrnID[3][2] = IDC_FIELDSELECTEDIT4;
	m_dwWhereCtrnID[3][3] = IDC_FIELDOPERATORCOMBO4;
	m_dwWhereCtrnID[3][4] = IDC_FIELDVALUECOMBO4;

	m_dwWhereCtrnID[4][0] = IDC_SELECTWHERECHECK5;
	m_dwWhereCtrnID[4][1] = IDC_FIELDANDORCOMBO4;
	m_dwWhereCtrnID[4][2] = IDC_FIELDSELECTEDIT5;
	m_dwWhereCtrnID[4][3] = IDC_FIELDOPERATORCOMBO5;
	m_dwWhereCtrnID[4][4] = IDC_FIELDVALUECOMBO5;

	m_bAllField = TRUE;
	FileldSelectList(ALL_SELECT_OK);
	AllEnalbeWhereCtrl(FALSE);
	CheckCtrlAble(FALSE);

	memset(m_nWhereSel, 0, sizeof(m_nWhereSel));

	for(int n = 0 ; n < MAX_CRASHINFO_NUMBER ; n++)
	{
		m_FieldSelectList.AddString(CRASH_FILED_INFO[n]);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelchangeFiledselectlist() 
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->m_pCrashInfoView->CrashInfoDraw(1);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::AddPageNumber(int nCurRow, int nMaxNum)
{
	CComboBox* pWnd = static_cast<CComboBox*>(GetDlgItem(IDC_CRASHPAGENOCOMBO));

	pWnd->ResetContent();
	int nPageNum = (nCurRow/nMaxNum);
	char szBuf[32] = {0,};

	if(nPageNum == 0 && nCurRow > 0)
	{
		sprintf(szBuf, "%d", 1);
		pWnd->AddString(szBuf);
	}

	for(int n = 0 ; n < nPageNum ; n++)
	{
		sprintf(szBuf, "%d", n+1);
		pWnd->AddString(szBuf);
	}

	if(nCurRow > 0)
		pWnd->SetCurSel(0);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::FileldSelectList(int nChecked)
{
	for(int n = 0 ; n < MAX_CRASHINFO_NUMBER ; n++)
	{
		m_FieldSelectList.SetCheck(n, nChecked);
	}

	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->m_pCrashInfoView->CrashInfoDraw(1);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnFieldselectokbtn() 
{
	FileldSelectList(ALL_SELECT_OK);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnFieldselectcancelbtn() 
{
	FileldSelectList(ALL_SELECT_CANCEL);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::SelectWhere(int nSel)
{
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelectwherecheck1() 
{
	m_nWhereSel[0] = ((CButton*)GetDlgItem(IDC_SELECTWHERECHECK1))->GetCheck();

	EnableWhereCtrl(0, m_nWhereSel[0]);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelectwherecheck2() 
{
	m_nWhereSel[1] = ((CButton*)GetDlgItem(IDC_SELECTWHERECHECK2))->GetCheck();
	EnableWhereCtrl(1, m_nWhereSel[1]);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelectwherecheck3() 
{
	m_nWhereSel[2] = ((CButton*)GetDlgItem(IDC_SELECTWHERECHECK3))->GetCheck();
	EnableWhereCtrl(2, m_nWhereSel[2]);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelectwherecheck4() 
{
	m_nWhereSel[3] = ((CButton*)GetDlgItem(IDC_SELECTWHERECHECK4))->GetCheck();
	EnableWhereCtrl(3, m_nWhereSel[3]);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelectwherecheck5() 
{
	// TODO: Add your control notification handler code here
	m_nWhereSel[4] = ((CButton*)GetDlgItem(IDC_SELECTWHERECHECK5))->GetCheck();
	EnableWhereCtrl(4, m_nWhereSel[4]);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::EnableWhereCtrl(int nRowWhere, BOOL bCheck)
{
	for(int n = 0 ; n < 5 ; n++)
	{
		if(n == 0 || m_dwWhereCtrnID[nRowWhere][n] == 0)
			continue;

		GetDlgItem(m_dwWhereCtrnID[nRowWhere][n])->EnableWindow(bCheck);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::AllEnalbeWhereCtrl(BOOL bCheck)
{
	for(int n = 0 ; n < MAX_SELECT_WHERE_NUMBER+1 ; n++)
	{
		EnableWhereCtrl(n, bCheck);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::CheckCtrlAble(BOOL bCheck)
{
	for(int n = 0 ; n < MAX_SELECT_WHERE_NUMBER+1 ; n++)
	{
		GetDlgItem(m_dwWhereCtrnID[n][0])->EnableWindow(bCheck);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::SetCheck(BOOL bCheck)
{
	for(int n = 1 ; n < MAX_SELECT_WHERE_NUMBER+1 ; n++)
	{
		((CButton*)GetDlgItem(m_dwWhereCtrnID[n][0]))->SetCheck(bCheck);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnAllfieldopt() 
{
	m_bAllField = TRUE;

	AllEnalbeWhereCtrl(FALSE);
	SetCheck(FALSE);
	CheckCtrlAble(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnAllwhereopt() 
{
	m_bAllField = FALSE;

//	EnableWhereCtrl(0, TRUE);
	CheckCtrlAble(TRUE);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashTopFormView::IsFieldChecked(int nCur)
{
	return m_FieldSelectList.GetCheck(nCur) == BST_CHECKED ? TRUE : FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashTopFormView::GetFieldCount()
{
	int nFieldCount = 0;

	for(int n = 0 ; n < MAX_CRASHINFO_NUMBER ; n++)
	{
		if(m_FieldSelectList.GetCheck(n) == BST_CHECKED)
			nFieldCount++;
	}

	return nFieldCount;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashTopFormView::SendSearchCrashInfo()
{
	if(m_pConnection == NULL)
		return FALSE;

	DWORD dwMsgType		= 0;
	SHORT snPos			= 0;
	SHORT snPacketSize	= 0;
	char  szPacket[2048];
	memset(szPacket, 0, sizeof(szPacket));

	ON_REQ_SEARCHCRASHINFO sMsg;
	memset(&sMsg, 0, sizeof(sMsg));

	snPacketSize = CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+sizeof(ON_REQ_SEARCHCRASHINFO);
	memcpy(szPacket, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);

	dwMsgType = ON_CRASH_MAIL_TOOL;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	dwMsgType = ON_REQ_CRASH_SEARCH_INFO;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	sMsg.byAllField = m_bAllField == FALSE ? 0 : 1;
	memset(sMsg.szQuery, 0, sizeof(sMsg.szQuery));
	memcpy(szPacket+snPos, &sMsg, sizeof(ON_REQ_SEARCHCRASHINFO));

	m_pConnection->SendPacket(szPacket, snPacketSize);	
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSearchbtn() 
{
	if(m_bAllField == TRUE)
		SendSearchCrashInfo();
	else
		SendWhereCrashInfo();


	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		pFrm->m_pCrashInfoView->RemoveCrashInfo();
	}

	m_nCurSel = 0;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::OnSelchangeCrashpagenocombo() 
{	
	CComboBox* pWnd = static_cast<CComboBox*>(GetDlgItem(IDC_CRASHPAGENOCOMBO));

	int nSelCur = pWnd->GetCurSel();

	if(nSelCur != m_nCurSel)
	{
		CMainFrame* pFrm = GetMainFrm();

		if(pFrm != NULL)
		{
			pFrm->m_pCrashInfoView->CrashInfoDraw(nSelCur+1);
			m_nCurSel = nSelCur;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashTopFormView::SendWhereCrashInfo()
{
	if(m_pConnection == NULL)
		return FALSE;

	int		nOperCursel	[5]		= {CB_ERR,};
	int		nWhereCursel[5]		= {CB_ERR,};
	int		nValueCursel[5]		= {CB_ERR,};
	char	szQuery[256]		= {0,};
	int		nQueryLen			= 0;
	CComboBox* pComboBox	= NULL;

	for(int n = 0 ; n < 5 ; n++)
	{
		if(m_nWhereSel[n] != 0 )
		{
			if(n != 0)
			{
				nOperCursel[n] = ((CComboBox*)GetDlgItem(m_dwWhereCtrnID[n][1]))->GetCurSel();
				if(nOperCursel[n] == CB_ERR)
				{
					MessageBox("연산자를 선택하세요", MB_OK);
					return FALSE;
				}

				nQueryLen += sprintf(szQuery+nQueryLen, " %s ", SEARCHOPERATOR[nOperCursel[n]]);
			}
			nWhereCursel[n] = ((CComboBox*)GetDlgItem(m_dwWhereCtrnID[n][3]))->GetCurSel();
			if(nWhereCursel[n] == CB_ERR)
			{
				MessageBox("연산자를 선택하세요", MB_OK);
				return FALSE;
			}
			nQueryLen += sprintf(szQuery+nQueryLen, "%s %s ", SEARCHFIELDNAME[n], SEARCHWHERE[nWhereCursel[n]]);

			pComboBox = (CComboBox*)GetDlgItem(m_dwWhereCtrnID[n][4]);
			nValueCursel[n] = pComboBox->GetCurSel();

			if(n == 0)
			{
				if(nValueCursel[n] == CB_ERR)
				{
					MessageBox("입력이 아니라 선택하세요", MB_OK);
					return FALSE;
				}
				nQueryLen += sprintf(szQuery+nQueryLen, "%d", nValueCursel[n]+1);
			}
			else
			{
				CString str;
				pComboBox->GetWindowText(str);
				if(str.GetLength() == 0)
				{
					MessageBox("값을 입력하세요", MB_OK);
					return FALSE;
				}
				nQueryLen += sprintf(szQuery+nQueryLen, "'%s'", str.GetBuffer(0));
			}			
		}
	}

	DWORD dwMsgType		= 0;
	SHORT snPos			= 0;
	SHORT snPacketSize	= 0;
	char  szPacket[2048];
	ON_REQ_SEARCHCRASHINFO sMsg;

	memset(&sMsg, 0, sizeof(sMsg));
	memset(szPacket, 0, sizeof(szPacket));

	snPacketSize = CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+sizeof(ON_REQ_SEARCHCRASHINFO);
	memcpy(szPacket, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);

	dwMsgType = ON_CRASH_MAIL_TOOL;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	dwMsgType = ON_REQ_CRASH_SEARCH_INFO;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	sMsg.byAllField = 0;
	strncpy(sMsg.szQuery, szQuery, 256);
	memcpy(szPacket+snPos, &sMsg, sizeof(ON_REQ_SEARCHCRASHINFO));

	m_pConnection->SendPacket(szPacket, snPacketSize);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashTopFormView::SetSearchNumber(int nCount)
{
	char szBuf[256] = {0,};
	sprintf(szBuf, "결과 : %d", nCount);
	GetDlgItem(IDC_SEARCHNUMBERSTATIC)->SetWindowText(szBuf);
}

/////////////////////////////////////////////////////////////////////////////