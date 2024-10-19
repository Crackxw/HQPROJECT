#if !defined(AFX_MONITORINGCLIENTCTL_H__DF85C0AE_7C5D_4D3C_B588_6B7D23403F52__INCLUDED_)
#define AFX_MONITORINGCLIENTCTL_H__DF85C0AE_7C5D_4D3C_B588_6B7D23403F52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MonitoringClientCtl.h : Declaration of the CMonitoringClientCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CMonitoringClientCtrl : See MonitoringClientCtl.cpp for implementation.
#include <objsafe.h>
#include <Mstask.h>
#include "ViewDlg.h"
#include "ClientSocket.h"

class CMonitoringClientCtrl : public COleControl
{
	
	DECLARE_DYNCREATE(CMonitoringClientCtrl)

	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)
		STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) ( 
            /* [in] */ REFIID riid,
            /* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,
            /* [out] */ DWORD __RPC_FAR *pdwEnabledOptions
		);
        
        STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) ( 
            /* [in] */ REFIID riid,
            /* [in] */ DWORD dwOptionSetMask,
            /* [in] */ DWORD dwEnabledOptions
		);
	END_INTERFACE_PART(ObjSafe);

private:
	bool m_bFirstLoading;
	CViewDlg m_ViewDlg;
	TCHAR WindowName[128];
	CClientSocket m_ClientSocket;
// Constructor
public:
	CMonitoringClientCtrl();

	void MakeDialog();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitoringClientCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CMonitoringClientCtrl();

	DECLARE_OLECREATE_EX(CMonitoringClientCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CMonitoringClientCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CMonitoringClientCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CMonitoringClientCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CMonitoringClientCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CMonitoringClientCtrl)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CMonitoringClientCtrl)
	void FireExiting()
		{FireEvent(eventidExiting,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CMonitoringClientCtrl)
	eventidExiting = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORINGCLIENTCTL_H__DF85C0AE_7C5D_4D3C_B588_6B7D23403F52__INCLUDED)
