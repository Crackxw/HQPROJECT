// MonitoringClientCtl.cpp : Implementation of the CMonitoringClientCtrl ActiveX Control class.

#include "stdafx.h"
#include "MonitoringClient.h"
#include "MonitoringClientCtl.h"
#include "MonitoringClientPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMonitoringClientCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CMonitoringClientCtrl, COleControl)
	//{{AFX_MSG_MAP(CMonitoringClientCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CMonitoringClientCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CMonitoringClientCtrl)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CMonitoringClientCtrl, COleControl)
	//{{AFX_EVENT_MAP(CMonitoringClientCtrl)
	EVENT_CUSTOM("Exiting", FireExiting, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CMonitoringClientCtrl, 1)
	PROPPAGEID(CMonitoringClientPropPage::guid)
END_PROPPAGEIDS(CMonitoringClientCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CMonitoringClientCtrl, "MONITORINGCLIENT.MonitoringClientCtrl.1",
	0x2a0563a6, 0x6d8e, 0x4d2d, 0x82, 0xe2, 0x25, 0xf3, 0xbb, 0xd, 0xff, 0x2e)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CMonitoringClientCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DMonitoringClient =
		{ 0x51eacfb3, 0x49fc, 0x4a17, { 0x9e, 0xcf, 0x53, 0x8d, 0xd6, 0x2d, 0x3d, 0xd6 } };
const IID BASED_CODE IID_DMonitoringClientEvents =
		{ 0xe2029650, 0xe928, 0x4814, { 0xa4, 0x97, 0xae, 0x5f, 0xc, 0xf3, 0x9c, 0xc1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwMonitoringClientOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMonitoringClientCtrl, IDS_MONITORINGCLIENT, _dwMonitoringClientOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::CMonitoringClientCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CMonitoringClientCtrl

BOOL CMonitoringClientCtrl::CMonitoringClientCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_MONITORINGCLIENT,
			IDB_MONITORINGCLIENT,
			afxRegApartmentThreading,
			_dwMonitoringClientOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

////////////////////////////////////////////////////////////////////////////
// Interface map for IObjectSafety

BEGIN_INTERFACE_MAP(CMonitoringClientCtrl, COleControl)
	INTERFACE_PART(CMonitoringClientCtrl, IID_IObjectSafety, ObjSafe)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IObjectSafety member functions

// Delegate AddRef, Release, QueryInterface

ULONG FAR EXPORT CMonitoringClientCtrl::XObjSafe::AddRef()
{
    METHOD_PROLOGUE(CMonitoringClientCtrl, ObjSafe)
    return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CMonitoringClientCtrl::XObjSafe::Release()
{
    METHOD_PROLOGUE(CMonitoringClientCtrl, ObjSafe)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CMonitoringClientCtrl::XObjSafe::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CMonitoringClientCtrl, ObjSafe)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

const DWORD dwSupportedBits = 
		INTERFACESAFE_FOR_UNTRUSTED_CALLER |
		INTERFACESAFE_FOR_UNTRUSTED_DATA;
const DWORD dwNotSupportedBits = ~ dwSupportedBits;
		
/////////////////////////////////////////////////////////////////////////////
// COcxPuzCtrl::XObjSafe::GetInterfaceSafetyOptions
// Allows container to query what interfaces are safe for what. We're
// optimizing significantly by ignoring which interface the caller is
// asking for.
HRESULT STDMETHODCALLTYPE 
	CMonitoringClientCtrl::XObjSafe::GetInterfaceSafetyOptions( 
		/* [in] */ REFIID riid,
        /* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
        /* [out] */ DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CMonitoringClientCtrl, ObjSafe)

	HRESULT retval = ResultFromScode(S_OK);

	// does interface exist?
	IUnknown FAR* punkInterface;
	retval = pThis->ExternalQueryInterface(&riid, 
					(void * *)&punkInterface);
	if (retval != E_NOINTERFACE) {	// interface exists
		punkInterface->Release(); // release it--just checking!
	}
	
	// we support both kinds of safety and have always both set,
	// regardless of interface
	*pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;

	return retval; // E_NOINTERFACE if QI failed
}

/////////////////////////////////////////////////////////////////////////////
// COcxPuzCtrl::XObjSafe::SetInterfaceSafetyOptions
// Since we're always safe, this is a no-brainer--but we do check to make
// sure the interface requested exists and that the options we're asked to
// set exist and are set on (we don't support unsafe mode).
HRESULT STDMETHODCALLTYPE 
	CMonitoringClientCtrl::XObjSafe::SetInterfaceSafetyOptions( 
        /* [in] */ REFIID riid,
        /* [in] */ DWORD dwOptionSetMask,
        /* [in] */ DWORD dwEnabledOptions)
{
    METHOD_PROLOGUE(CMonitoringClientCtrl, ObjSafe)
	
	// does interface exist?
	IUnknown FAR* punkInterface;
	pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);
	if (punkInterface) {	// interface exists
		punkInterface->Release(); // release it--just checking!
	}
	else { // interface doesn't exist
		return ResultFromScode(E_NOINTERFACE);
	}

	// can't set bits we don't support
	if (dwOptionSetMask & dwNotSupportedBits) { 
		return ResultFromScode(E_FAIL);
	}
	
	
	// can't set bits we do support to zero
	dwEnabledOptions &= dwSupportedBits;
	// (we already know there are no extra bits in mask )
	if ((dwOptionSetMask & dwEnabledOptions) !=
		 dwOptionSetMask) {
		return ResultFromScode(E_FAIL);
	}
	// don't need to change anything since we're always safe
	return ResultFromScode(S_OK);
}

/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::CMonitoringClientCtrl - Constructor

CMonitoringClientCtrl::CMonitoringClientCtrl()
{
	InitializeIIDs(&IID_DMonitoringClient, &IID_DMonitoringClientEvents);

	// TODO: Initialize your control's instance data here.

	m_bFirstLoading = true;
	_tcscpy(WindowName, _T("Viewer"));

}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::~CMonitoringClientCtrl - Destructor

CMonitoringClientCtrl::~CMonitoringClientCtrl()
{
	// TODO: Cleanup your control's instance data here.

	
	
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::OnDraw - Drawing function

void CMonitoringClientCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	/*
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
	*/
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::DoPropExchange - Persistence support

void CMonitoringClientCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl::OnResetState - Reset control to default state

void CMonitoringClientCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl message handlers

int CMonitoringClientCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	OnActivateInPlace (TRUE, NULL); // == UI-Activate the control   

	SendMessage(WM_PAINT);

	return 0;
}

void CMonitoringClientCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	if(m_bFirstLoading)
	{
		m_bFirstLoading = false;

		//AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
	
		
		if ( m_ViewDlg.DoModal() == IDOK)
		{
		}
		else
		{
			
			//AfxGetApp()->GetMainWnd()->DestroyWindow();
			FireExiting();
			PostQuitMessage(0);
			

		}
		
	}
	else
	{
	}
	// Do not call COleControl::OnPaint() for painting messages
}

BOOL CMonitoringClientCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	/*
	if(!m_bFirstLoading)
	{
		if(m_ClientSocket.IsConnected())
		{
			//if(pMsg->message == WM_KEYUP)
			if(pMsg->message == WM_KEYDOWN)
			{
				switch (pMsg->wParam)
				{
				case VK_TAB:
					m_pViewDlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
					//m_pViewForm->SetFocusChange();
					return TRUE;
				case VK_F5:
					return TRUE;
					
				case VK_RETURN:
					switch ( m_pViewForm->Where_is_chage() )		//IP 받는데가 바꼈냐? 아님 채팅이 바꼈냐를 받아옮
					{
					case 0:					//안 바뀐 경우
						break;
					case 1:					//IP 부분 새로 입력
						break;
					case 2:					//채팅 새로 입력
						break;
					case 3:					//둘다 새로 입력
						break;
					}
					return TRUE;
					
				}
			}
		}
		else				//Connect 되지 않고, 엔터키 치는 경우는 IP 입력하고, 엔터치는 경우라고 볼 수 있음
		{
			
			if(pMsg->message == WM_KEYDOWN)
			{
				switch (pMsg->wParam)
				{
				case VK_TAB:
					m_pViewDlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
					//m_pViewForm->SetFocusChange();
					return TRUE;
				case VK_F5:
					return TRUE;
				
				case VK_RETURN:
										
					return TRUE;
				
				}
			}
		}
	}
	*/

	return COleControl::PreTranslateMessage(pMsg);
}

int CMonitoringClientCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bUIActive)
	{
		OnActivateInPlace (TRUE, NULL); // == UI-Activate the control
	}
	
	return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMonitoringClientCtrl::MakeDialog()
{

	return;
}

void CMonitoringClientCtrl::OnDestroy() 
{
	COleControl::OnDestroy();
	
	// TODO: Add your message handler code here
	

	
	return;
}
