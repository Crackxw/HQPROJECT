#if !defined(AFX_MONITORINGCLIENTPPG_H__B4CEC5C9_9E56_4F00_92A1_A11AF676365A__INCLUDED_)
#define AFX_MONITORINGCLIENTPPG_H__B4CEC5C9_9E56_4F00_92A1_A11AF676365A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MonitoringClientPpg.h : Declaration of the CMonitoringClientPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CMonitoringClientPropPage : See MonitoringClientPpg.cpp.cpp for implementation.

class CMonitoringClientPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CMonitoringClientPropPage)
	DECLARE_OLECREATE_EX(CMonitoringClientPropPage)

// Constructor
public:
	CMonitoringClientPropPage();

// Dialog Data
	//{{AFX_DATA(CMonitoringClientPropPage)
	enum { IDD = IDD_PROPPAGE_MONITORINGCLIENT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CMonitoringClientPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORINGCLIENTPPG_H__B4CEC5C9_9E56_4F00_92A1_A11AF676365A__INCLUDED)
