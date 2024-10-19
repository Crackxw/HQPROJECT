// ServerMonitorDlg.h : header file
//

#if !defined(AFX_SERVERMONITORDLG_H__E56439CB_4B59_4886_87B1_70561DB4ABDB__INCLUDED_)
#define AFX_SERVERMONITORDLG_H__E56439CB_4B59_4886_87B1_70561DB4ABDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorDlg dialog

class CServerMonitorDlg : public CDialog
{
// Construction
public:
	BOOL m_bFlash;
	void ClearServerList();
	WORD GetSelectedServer(WORD *pServerlist);
	HMENU m_hPopupMenu;
	int m_siSetSel;
	void PutConsoleString(COLORREF color,const TCHAR *szString,...);
	int ProcessConsoleCommand(TCHAR *szCommandString);
	void RefreshServerList();
	int InitializeServerList();
	int ConfirmServer();
	CServerMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CServerMonitorDlg)
	enum { IDD = IDD_SERVERMONITOR_DIALOG };
	CEdit	m_KickTimerEdit;
	CStatic	m_FileTransferName;
	CStatic	m_FileTransferBytes;
	CEdit	m_IntervalEdit;
	CEdit	m_AutoNoticeContent;
	CButton	m_AutoNoticeButton;
	CButton	m_SoundButton;
	CComboBox	m_MasterIpCombo;
	CStatic	m_MaxUserStatic;
	CListCtrl	m_ServerListControl2;
	CTabCtrl	m_ListTab;
	CRichEditCtrl	m_ConsoleResultEdit;
	CListCtrl	m_ServerListControl;
	CEdit	m_ServerInfoEdit;
	CEdit	m_ConsoleEdit;
	CString	m_MasterIpString;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerMonitorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnClickServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnConnectbutton();
	afx_msg void OnExitbutton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFiletransferbutton();
	afx_msg void OnRclickServerlistcontrol2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnServeroffbutton();
	afx_msg void OnServeronbutton();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnGetdispinfoServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusIpcombo();
	afx_msg void OnKillfocusAutonoticecontent();
	afx_msg void OnKillfocusIntervaledit();
	afx_msg void OnNoticesendbutton2();
	afx_msg void OnRclickServerlistcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKickbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	int m_siCurrentListMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern char g_SavePath[256];




#endif // !defined(AFX_SERVERMONITORDLG_H__E56439CB_4B59_4886_87B1_70561DB4ABDB__INCLUDED_)
