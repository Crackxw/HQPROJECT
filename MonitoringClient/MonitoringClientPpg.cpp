// MonitoringClientPpg.cpp : Implementation of the CMonitoringClientPropPage property page class.

#include "stdafx.h"
#include "MonitoringClient.h"
#include "MonitoringClientPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMonitoringClientPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CMonitoringClientPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CMonitoringClientPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CMonitoringClientPropPage, "MONITORINGCLIENT.MonitoringClientPropPage.1",
	0x2ef76f9f, 0xf880, 0x44f2, 0xb2, 0xd3, 0xdb, 0x48, 0xac, 0xe1, 0x16, 0x36)


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientPropPage::CMonitoringClientPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CMonitoringClientPropPage

BOOL CMonitoringClientPropPage::CMonitoringClientPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MONITORINGCLIENT_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientPropPage::CMonitoringClientPropPage - Constructor

CMonitoringClientPropPage::CMonitoringClientPropPage() :
	COlePropertyPage(IDD, IDS_MONITORINGCLIENT_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CMonitoringClientPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientPropPage::DoDataExchange - Moves data between page and properties

void CMonitoringClientPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CMonitoringClientPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientPropPage message handlers
