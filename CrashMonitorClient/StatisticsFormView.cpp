// StatisticsFormView.cpp : implementation file
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "StatisticsFormView.h"
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"
#include "..\\XCommon\\JXConnectSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsFormView

IMPLEMENT_DYNCREATE(CStatisticsFormView, CFormView)

CStatisticsFormView::CStatisticsFormView()
	: CFormView(CStatisticsFormView::IDD), m_pConnection(NULL)
{
	//{{AFX_DATA_INIT(CStatisticsFormView)
	
	//}}AFX_DATA_INIT
}

CStatisticsFormView::~CStatisticsFormView()
{
}

void CStatisticsFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsFormView)
	DDX_Control(pDX, IDC_STARTMINUTESPIN, m_StartMinuteSpin);
	DDX_Control(pDX, IDC_STARTHOURSPIN, m_StartHourSpin);
	DDX_Control(pDX, IDC_ENDMINUTESPIN, m_EndMinuteSpin);
	DDX_Control(pDX, IDC_ENDHOURSPIN, m_EndHourSpin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsFormView, CFormView)
	//{{AFX_MSG_MAP(CStatisticsFormView)
	ON_BN_CLICKED(IDC_ALLSTATISTICSRADIO, OnAllstatisticsradio)
	ON_BN_CLICKED(IDC_DATESTATISTICSRADIO, OnDatestatisticsradio)
	ON_BN_CLICKED(IDC_CRASHSTATISTICSBTN, OnCrashstatisticsbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsFormView diagnostics

#ifdef _DEBUG
void CStatisticsFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStatisticsFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStatisticsFormView message handlers

void CStatisticsFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	m_StartHourSpin.SetRange(0, 23);
	m_StartHourSpin.SetPos(0);

	m_StartMinuteSpin.SetRange(0, 60);
	m_StartMinuteSpin.SetPos(0);

	m_EndHourSpin.SetRange(0, 23);
	m_EndHourSpin.SetPos(0);

	m_EndMinuteSpin.SetRange(0, 60);
	m_EndMinuteSpin.SetPos(0);

	GetDlgItem(IDC_STARTHOUREDIT)->SetWindowText("0" );
	GetDlgItem(IDC_STARTMINUTEEDIT)->SetWindowText("0");
	GetDlgItem(IDC_ENDHOUREDIT)->SetWindowText("0");
	GetDlgItem(IDC_ENDMINUTEEDIT)->SetWindowText("0");

	((CButton*)(GetDlgItem(IDC_ALLSTATISTICSRADIO)))->SetCheck(TRUE);
	m_bAllStatistics = TRUE;

	m_dwCtrlID[0][0] = IDC_STARTDATETIMEPICKER;
	m_dwCtrlID[0][1] = IDC_STARTHOUREDIT;
	m_dwCtrlID[0][2] = IDC_STARTHOURSPIN;
	m_dwCtrlID[0][3] = IDC_STARTMINUTEEDIT;
	m_dwCtrlID[0][4] = IDC_STARTMINUTESPIN;

	m_dwCtrlID[1][0] = IDC_ENDDATETIMEPICKER;
	m_dwCtrlID[1][1] = IDC_ENDHOUREDIT;
	m_dwCtrlID[1][2] = IDC_ENDHOURSPIN;
	m_dwCtrlID[1][3] = IDC_ENDMINUTEEDIT;
	m_dwCtrlID[1][4] = IDC_ENDMINUTESPIN;

	EnableCtrlWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

void CStatisticsFormView::OnAllstatisticsradio() 
{
	m_bAllStatistics = TRUE;
	EnableCtrlWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

void CStatisticsFormView::OnDatestatisticsradio() 
{
	m_bAllStatistics = FALSE;
	EnableCtrlWindow(TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void CStatisticsFormView::EnableCtrlWindow(BOOL bEnable)
{
	for(int n = 0 ; n < 2 ; n++)
	{
		for(int i = 0 ; i < 5 ; i++)
		{
			GetDlgItem(m_dwCtrlID[n][i])->EnableWindow(bEnable);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

BOOL CStatisticsFormView::SendCrashStatisticsInfo()
{
	if(m_pConnection == NULL)
		return FALSE;

	int		nOperCursel	[5]		= {CB_ERR,};
	int		nWhereCursel[5]		= {CB_ERR,};
	int		nValueCursel[5]		= {CB_ERR,};
	char	szQuery[256]		= {0,};
	int		nQueryLen			= 0;
	CComboBox* pComboBox		= NULL;
	SYSTEMTIME sTime;
	CDateTimeCtrl *pTimeWnd		= NULL;
	CString strHour;
	CString strMinute;

	if(m_bAllStatistics == FALSE)	
	{
		pTimeWnd = (CDateTimeCtrl*)GetDlgItem(m_dwCtrlID[0][0]);
		pTimeWnd->GetTime(&sTime);		
		nQueryLen += sprintf(szQuery+nQueryLen, " AND (EndTime BETWEEN '%d-%d-%d ", 
			sTime.wYear, sTime.wMonth, sTime.wDay);

		GetDlgItem(m_dwCtrlID[0][1])->GetWindowText(strHour);
		if(strHour.GetLength() == 0)
		{
			MessageBox("Hour input.", MB_OK);
			return FALSE;
		}

		GetDlgItem(m_dwCtrlID[0][3])->GetWindowText(strMinute);
		if(strMinute.GetLength() == 0)
		{
			MessageBox("Minute input.", MB_OK);
			return FALSE;
		}

		nQueryLen += sprintf(szQuery+nQueryLen, "%d:%d:00' ", 
			atoi(strHour), atoi(strMinute));

		pTimeWnd = (CDateTimeCtrl*)GetDlgItem(m_dwCtrlID[1][0]);
		pTimeWnd->GetTime(&sTime);
		nQueryLen += sprintf(szQuery+nQueryLen, "AND '%d-%d-%d ", 
			sTime.wYear, sTime.wMonth, sTime.wDay);

		GetDlgItem(m_dwCtrlID[1][1])->GetWindowText(strHour);
		if(strHour.GetLength() == 0)
		{
			MessageBox("Hour input.", MB_OK);
			return FALSE;
		}

		GetDlgItem(m_dwCtrlID[1][3])->GetWindowText(strMinute);
		if(strMinute.GetLength() == 0)
		{
			MessageBox("Minute input.", MB_OK);
			return FALSE;
		}

		nQueryLen += sprintf(szQuery+nQueryLen, "%d:%d:00')", 
			atoi(strHour), atoi(strMinute));
	}

	DWORD dwMsgType		= 0;
	SHORT snPos			= 0;
	SHORT snPacketSize	= 0;
	char  szPacket[2048];
	ON_REQ_CRASHSTATISTICSINFO sMsg;

	memset(&sMsg, 0, sizeof(sMsg));
	memset(szPacket, 0, sizeof(szPacket));

	snPacketSize = CRASH_SERVER_HEADER_SIZE+(sizeof(DWORD)*2)+sizeof(ON_REQ_CRASHSTATISTICSINFO);
	memcpy(szPacket, &snPacketSize, sizeof(SHORT));
	snPos += sizeof(SHORT);

	dwMsgType = ON_CRASH_MAIL_TOOL;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	dwMsgType = ON_REQ_CRASH_STATISTICSINFO;
	memcpy(szPacket+snPos, &dwMsgType, sizeof(DWORD));
	snPos += sizeof(DWORD);

	sMsg.byAllField = m_bAllStatistics == TRUE ? 1 : 0;
	strncpy(sMsg.szQuery, szQuery, 256);
	memcpy(szPacket+snPos, &sMsg, sizeof(ON_REQ_CRASHSTATISTICSINFO));

	m_pConnection->SendPacket(szPacket, snPacketSize);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CStatisticsFormView::OnCrashstatisticsbtn() 
{
	SendCrashStatisticsInfo();
}
/////////////////////////////////////////////////////////////////////////////