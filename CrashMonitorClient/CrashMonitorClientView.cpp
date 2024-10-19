// CrashMonitorClientView.cpp : implementation of the CCrashMonitorClientView class
//

#include "stdafx.h"
#include "CrashMonitorClient.h"
#include "MainFrm.h"
#include "CrashTopFormView.h"
#include "CrashClientFormView.h"
#include "StatisticsFormView.h"

#include "CrashMonitorClientDoc.h"
#include "CrashMonitorClientView.h"
#include "CrashInfo.h"


#include ".\\Ctrl\\GridCtrl.h"
#include "..\\XCommon\\JXConnectSocket.h"
#include "..\\XCommon\\JXPacketQueue.h"
#include "..\\CrashMailProtocol\\CrashMailProtocol.h"
#include <process.h>
#include <assert.h>
// #include "Defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView

IMPLEMENT_DYNCREATE(CCrashMonitorClientView, CScrollView)

BEGIN_MESSAGE_MAP(CCrashMonitorClientView, CScrollView)
	//{{AFX_MSG_MAP(CCrashMonitorClientView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView construction/destruction

CCrashMonitorClientView::CCrashMonitorClientView() :
m_pCrashInfo(NULL),
m_pConnection(NULL), 
m_bThreadRun(FALSE), 
m_hExitEvent(NULL),
m_hBreakEvent(NULL)
{
	// TODO: add construction code here

	WORD wVersionRequested;
	WSADATA wsaData;	
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	WSAStartup( wVersionRequested, &wsaData );
}

/////////////////////////////////////////////////////////////////////////////

CCrashMonitorClientView::~CCrashMonitorClientView()
{
	StopProcessThread();
	StopSendRecvThread();

	if(m_hExitEvent != NULL)
		CloseHandle(m_hExitEvent);
	if(m_hBreakEvent != NULL)
		CloseHandle(m_hBreakEvent);

	if(m_pCrashInfo != NULL)
	{
		delete m_pCrashInfo;
		m_pCrashInfo = NULL;
	}

	if(m_pConnection != NULL)
	{
		m_pConnection->Disconnect();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashMonitorClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView drawing

void CCrashMonitorClientView::OnDraw(CDC* pDC)
{
	CCrashMonitorClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view

	Initialize();

	NetWorkdInitialize();

	memset(&m_sCrashStatisticsInfo, 0, sizeof(ON_RESP_CRASHSTATISTICSINFO));

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView printing

BOOL CCrashMonitorClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCrashMonitorClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView diagnostics

#ifdef _DEBUG
void CCrashMonitorClientView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCrashMonitorClientView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCrashMonitorClientDoc* CCrashMonitorClientView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCrashMonitorClientDoc)));
	return (CCrashMonitorClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientView message handlers

CMainFrame* CCrashMonitorClientView::GetMainFrm()
{
	CCrashMonitorClientApp * pApp = ( CCrashMonitorClientApp * )AfxGetApp();
	if( pApp == NULL )	return NULL;
	CMainFrame * pFrm = NULL;
	pFrm = ( CMainFrame * )pApp->GetMainWnd();
	return pFrm;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::Initialize()
{
	if(m_pCrashInfo != NULL)
	{
		delete m_pCrashInfo;
		m_pCrashInfo = NULL;
	}
	try{
		m_pCrashInfo = new CCrashInfo;
	}
	catch(std::bad_alloc){
		assert(0);
	}

	
	CRect rtCltRect;
	GetClientRect(rtCltRect);

	rtCltRect.SetRect(rtCltRect.left, 
		rtCltRect.top, rtCltRect.right-10, rtCltRect.bottom-70);

	m_pGridCtrl = new CGridCtrl;
	m_pGridCtrl->Create(rtCltRect, this, EDITOR_LOG_GRID_ID);
	SetListMode(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::SetListMode(BOOL bRetVal)
{
	assert(m_pGridCtrl != NULL);
	if(m_pGridCtrl == NULL) return;

	m_pGridCtrl->SetListMode(bRetVal);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashMonitorClientView::NetWorkdInitialize()
{
	if(m_pConnection != NULL)
	{
		delete m_pConnection;
		m_pConnection = NULL;
	}

	try{
		m_pConnection = new JXConnectSocket;
	}
	catch(std::bad_alloc){
		assert(0);
		return FALSE;
	}

	Create(CRASH_MAIL_SERVER_IP, CRASH_MAIL_TOOL_SERVER_PORT, 
		CRASH_SERVER_RECV_BUF_SIZE, CRASH_SERVER_SEND_BUF_SIZE, 
		CRASH_SERVER_QUEUE_SIZE);

	m_hExitEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hBreakEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::SetConnection()
{
	CMainFrame* pFrm = GetMainFrm();
	if(pFrm != NULL)
	{
		pFrm->m_pCrashFormView->SetConnection(m_pConnection);
		pFrm->m_pCrashTopFormView->SetConnection(m_pConnection);
		pFrm->m_pCrashStatisticsFormView->SetConnection(m_pConnection);
	}
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashMonitorClientView::Connect(const char* pIP)
{
	BOOL bRetVal = FALSE;
	bRetVal = m_pConnection->Connect(pIP, CRASH_MAIL_TOOL_SERVER_PORT);

	if(bRetVal == TRUE)
	{
		SetConnection();
		StartProcessThread();
	}

	return bRetVal;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashMonitorClientView::Create(const char* pIP, 
									 WORD wPort, 
									 WORD snRecbBufSize, 
									 WORD snSendBufSize, 
									 WORD snPackettQueueSize)
{
	m_pConnection->Create((char*)pIP, wPort, 
		snRecbBufSize, snSendBufSize, snPackettQueueSize, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::StartProcessThread()
{
	HANDLE hThread = NULL;
	unsigned int uiThreadID;

	hThread = (HANDLE)_beginthreadex(NULL, 0, Run_Process, 
		reinterpret_cast<CCrashMonitorClientView*>(this), 0, &uiThreadID);

	if(hThread != NULL)
	{
		m_bThreadRun = TRUE;
		CloseHandle(hThread);		
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::StopProcessThread()
{
	if(m_bThreadRun == FALSE)
		return;

	m_bThreadRun = FALSE;
	SetEvent(m_hBreakEvent);

	WaitForSingleObject(m_hExitEvent, INFINITE);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::StartSendRecvThread()
{
	m_pConnection->StartThread();
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::StopSendRecvThread()
{
	m_pConnection->StopThread();
}

/////////////////////////////////////////////////////////////////////////////

unsigned int CCrashMonitorClientView::Run_Process(void* vpThis)
{
	unsigned int uiRetVal = 0;

	try{
		uiRetVal = reinterpret_cast<CCrashMonitorClientView*>(vpThis)->Process();
		return uiRetVal;
	}
	catch(...){
		assert(0);
		return uiRetVal;
	}
}

/////////////////////////////////////////////////////////////////////////////

unsigned int CCrashMonitorClientView::Process()
{
	DWORD dwRetVal;
	HANDLE hEvent[2];
	WSANETWORKEVENTS wsaNetworks;

	hEvent[0] = m_hBreakEvent;
	hEvent[1] = WSACreateEvent();

	WSAEventSelect(m_pConnection->GetSocket(), hEvent[1], FD_CONNECT | FD_CLOSE);

	while(TRUE)
	{
		dwRetVal = WSAWaitForMultipleEvents(2, hEvent, FALSE, 50, FALSE);
		
		if(dwRetVal == WSA_WAIT_EVENT_0)
		{
			break;
		}
		else if(dwRetVal == WSA_WAIT_EVENT_0+1)
		{
			WSAEnumNetworkEvents(m_pConnection->GetSocket(), hEvent[1], 
				&wsaNetworks);
			if(wsaNetworks.lNetworkEvents & FD_CONNECT)
			{
				if(wsaNetworks.iErrorCode[FD_CONNECT_BIT] == 0)
				{
					StartSendRecvThread();					
				}
			}
			else if(wsaNetworks.lNetworkEvents & FD_CLOSE)
			{
				m_pConnection->Disconnect();
			}
		}

		RecvMsgProc();		
	}

	SetEvent(m_hExitEvent);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::RecvMsgProc()
{
	JXPacketQueue* pPacketQueue = m_pConnection->GetQueue();
	int nPacketNumber = pPacketQueue->GetNumPacket();

	for(int n = 0 ; n < nPacketNumber ; n++)
	{
		DWORD dwMsgType = 0;

		pPacketQueue->Dequeue(&m_LocalPacket);
		m_LocalPacket.ReadValue(&dwMsgType, sizeof(DWORD));

		if(dwMsgType == ON_CRASH_MAIL_TOOL)
		{
			m_LocalPacket.ReadValue(&dwMsgType, sizeof(DWORD));
			
			switch(dwMsgType)
			{
				case ON_RESP_CRASH_SEARCH_INFO :
					{						
						ON_RESP_SEARCHCRASHINFO_LIST CrashList;
						ON_RESP_SEARCHCRASHINFO		 CrashInfo[CRASH_MAX_LOAD_NUMBER];

						memset(&CrashList, 0, sizeof(ON_RESP_SEARCHCRASHINFO_LIST));

						int nCrashInfoCount = 0;

						m_LocalPacket.ReadValue(&CrashList, 
							sizeof(ON_RESP_SEARCHCRASHINFO_LIST));

						if(CrashList.byResult == 1)
						{
							nCrashInfoCount = 
								min(CRASH_MAX_LOAD_NUMBER, CrashList.nCount);

							m_LocalPacket.ReadValue(CrashInfo, 
								sizeof(ON_RESP_SEARCHCRASHINFO)*nCrashInfoCount);

							AddCrashInfo(CrashInfo, nCrashInfoCount);

							// Crash List정보를 다 받았으면..
							if(CrashList.byLastList == 1)
							{
								CrashInfoDraw(1);

								AddPageNumber();
							}
						}
						else
						{
							CrashInfoDraw(1);
							AddPageNumber();
							MessageBox("검색 실패(시스템 에러)", MB_OK);
						}
					}
					break;
				case ON_RESP_CRASH_STATISTICSINFO :
					{						
						memset(&m_sCrashStatisticsInfo, 0, sizeof(ON_RESP_CRASHSTATISTICSINFO));

						m_LocalPacket.ReadValue(&m_sCrashStatisticsInfo, 
								sizeof(ON_RESP_CRASHSTATISTICSINFO));
						if(m_sCrashStatisticsInfo.byResult == 1)
						{
							CrashStatisticsInfoDraw();
						}
						else
						{
							MessageBox("검색실패(시스템 에러)", MB_OK);
						}
					}
					break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::AddCrashInfo(const ON_P_RESP_SEARCHCRASHINFO pCrashInfo, int nCrashCount)
{	
	for(int n = 0 ; n < nCrashCount ; n++)
	{
		CCrashData Data;
		Data.Set(pCrashInfo[n]);

		m_pCrashInfo->Add(Data);
	}
}

/////////////////////////////////////////////////////////////////////////////

int CCrashMonitorClientView::GridDraw(CMainFrame* pFrm, int nRowCount)
{
	m_pGridCtrl->SetFixedRowCount(1);
	m_pGridCtrl->SetRowCount(nRowCount+1);
	int nCur		= 0;
	int nFieldCount = 0;
	
	nFieldCount = pFrm->m_pCrashTopFormView->GetFieldCount();
	m_pGridCtrl->SetColumnCount(nFieldCount);

	for(int n = 0 ; n < MAX_CRASHINFO_NUMBER ; n++)
	{
		if(pFrm->m_pCrashTopFormView->IsFieldChecked(n) == TRUE)
		{
			m_pGridCtrl->SetItemText(0, nCur, CRASH_FILED_INFO[n]);
			m_pGridCtrl->AutoSizeColumn(nCur);

			nCur++;
		}
	}

	return nFieldCount;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::StatisticsGridDraw(CMainFrame* pFrm)
{
	int n = 0;

	m_pGridCtrl->SetFixedRowCount(1);
	m_pGridCtrl->SetFixedColumnCount(1);
	m_pGridCtrl->SetRowCount(MAX_CRASH_TYPE_NUMBER+1);
	m_pGridCtrl->SetColumnCount(MAX_STATISTICS_TYPE_NUMBER);

	for(n = 0 ; n < MAX_STATISTICS_TYPE_NUMBER ; n++)
	{
		m_pGridCtrl->SetItemText(0, n, STATISTICS_TITLE_NAME[n]);
		m_pGridCtrl->AutoSizeColumn(n);
	}
	
	for(n = 0 ; n < MAX_CRASH_TYPE_NUMBER ; n++)
	{
		m_pGridCtrl->SetItemText(n+1, 0, EXCEPTION_TYPE[n]);
	}	
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::CrashStatisticsInfoDraw()
{
	CMainFrame* pFrm = GetMainFrm();
	if(pFrm == NULL)		return;	
	
	CCrashData Data;
	int		   m	= 0;
	int		   nCol = 0;
	int		   nRowCount = 0;
	char	   szBuf[256] = {0,};

	StatisticsGridDraw(pFrm);

	for(int n = 0 ; n < MAX_CRASH_TYPE_NUMBER ; n++)
	{
		m += 1;

		sprintf(szBuf, "%d", m_sCrashStatisticsInfo.nCrashCount[n]);
		SetItemText(m, 1, szBuf);

		if(m_sCrashStatisticsInfo.nCrashCount[n] == 0)
			sprintf(szBuf, "%d%%", 0);
		else
			sprintf(szBuf, "%d%%", m_sCrashStatisticsInfo.nCrashCount[n]*100/m_sCrashStatisticsInfo.nCrashCount[0]);

		SetItemText(m, 2, szBuf);
	}

	m_pGridCtrl->Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::CrashInfoDraw(int nPage)
{
	CMainFrame* pFrm = GetMainFrm();
	if(pFrm == NULL)		return;	
	
	CCrashData Data;
	int		   m	= 0;
	int		   nCol = 0;
	int		   nRowCount = 0;
	char	   szBuf[256] = {0,};

	CCrashData	Datas[CRASH_MAX_LOAD_NUMBER];	
	m_pCrashInfo->Gets(Datas, nRowCount, CRASH_MAX_LOAD_NUMBER, nPage);	

	int nFieldCount = GridDraw(pFrm, nRowCount);
	if(nFieldCount <= 0)	return;

	for(int n = 0 ; n < nRowCount ; n++)
	{
		m += 1;

		sprintf(szBuf, "%d", Datas[n].dwID);
		nCol = SetItemText(m, 0,	0, szBuf, pFrm);

		sprintf(szBuf, "%d", Datas[n].nCrashType);
		nCol = SetItemText(m, nCol, 1, szBuf, pFrm);

		nCol = SetItemText(m, nCol, 2, Datas[n].szCrashContents, pFrm);
		nCol = SetItemText(m, nCol, 3, Datas[n].szOSType, pFrm);
		nCol = SetItemText(m, nCol, 4, Datas[n].szProcessType, pFrm);
		nCol = SetItemText(m, nCol, 5, Datas[n].szIP, pFrm);

		sprintf(szBuf, "%d", Datas[n].nProcessNumber);
		nCol = SetItemText(m, nCol, 6, szBuf, pFrm);

		sprintf(szBuf, "%d", Datas[n].nProcessLevel);
		nCol = SetItemText(m, nCol, 7, szBuf, pFrm);

		sprintf(szBuf, "%d", Datas[n].nPageSize);
		nCol = SetItemText(m, nCol, 8, szBuf, pFrm);

		nCol = SetItemText(m, nCol, 9, Datas[n].szsTime, pFrm);
		nCol = SetItemText(m, nCol, 10, Datas[n].szeTime, pFrm);
		nCol = SetItemText(m, nCol, 11, Datas[n].szCrashAddress, pFrm);
		nCol = SetItemText(m, nCol, 12, Datas[n].szLineAddress, pFrm);
		nCol = SetItemText(m, nCol, 13, Datas[n].szNationCode, pFrm);
		nCol = SetItemText(m, nCol, 14, Datas[n].szSupposition, pFrm);
	}

	m_pGridCtrl->Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////

int CCrashMonitorClientView::SetItemText(int nRow, 
										 int nCol, 
										 int nSel,
										 char* pData,
										 CMainFrame* pFrm)
{
	if(pFrm->m_pCrashTopFormView->IsFieldChecked(nSel) == TRUE)
	{
		char szBuf[256] = {0,};
		memcpy(szBuf, (char*)pData, 256);
		m_pGridCtrl->SetItemText(nRow, nCol, szBuf);
		nCol++;
	}

	return nCol;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::SetItemText(int nRow, 
										  int nCol,
										  char* pData)
{
	char szBuf[256] = {0,};
	memcpy(szBuf, (char*)pData, 256);
	m_pGridCtrl->SetItemText(nRow, nCol, szBuf);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CCrashMonitorClientView::RemoveCrashInfo()
{
	m_pCrashInfo->RemoveAll();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

void CCrashMonitorClientView::AddPageNumber()
{
	CMainFrame* pFrm = GetMainFrm();

	if(pFrm != NULL)
	{
		int nRowCount = m_pCrashInfo->Count();
		pFrm->m_pCrashTopFormView->AddPageNumber(nRowCount, CRASH_MAX_LOAD_NUMBER);

		pFrm->m_pCrashTopFormView->SetSearchNumber(nRowCount);
	}
}

/////////////////////////////////////////////////////////////////////////////