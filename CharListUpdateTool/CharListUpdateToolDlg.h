// CharListUpdateToolDlg.h : header file
//

#if !defined(AFX_CHARLISTUPDATETOOLDLG_H__094A7432_6953_4D6E_A5FE_3E4796068674__INCLUDED_)
#define AFX_CHARLISTUPDATETOOLDLG_H__094A7432_6953_4D6E_A5FE_3E4796068674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <algorithm>
#include "structure.h"
#include "..\\JOCommon\\LogWriter.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCharListUpdateToolDlg dialog

class CAdoExecutor;

class CCharListUpdateToolDlg : public CDialog
{
// Construction
public:
	CCharListUpdateToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCharListUpdateToolDlg)
	enum { IDD = IDD_CHARLISTUPDATETOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharListUpdateToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private : 
	void	Initialize_Parser();
	void	Finialize_Parser();
	BOOL	FileLoad(char* pFileName);
	UI16	GetID(CHAR *pToken);
	SI16	GetAttri(char *pToken);
	void	Free()
	{
		vector<CharHead*>::iterator it = m_CharHeader.begin();

		while(it != m_CharHeader.end())
		{
			CharHead* pHead = *it;
			if(pHead != NULL)
			{
				delete pHead;
				pHead = NULL;

				it = m_CharHeader.erase(it);
			}
			else
				++it;
		}
	}
	void	MsgPrint(const TCHAR* szFormat,...);
	void	DBConnect();
	

	BOOL	Load84YearBirthUser(char* pCharFile);

private : 	
	DWORD					m_dwSetServerCount;
	BOOL					m_bFileOpen;
	DBINFO*					m_pDBInfo;
	CAdoExecutor*			m_pAdoConn;

	vector<CharHead*>	m_CharHeader;
	typedef vector<CharHead*>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;


	CCheckListBox	m_SelectListBox;
	CListBox		m_MsgDisplayList;

	CLogWriter		m_LogWriter;
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCharListUpdateToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileopn();
	afx_msg void OnClose();
	afx_msg void OnDbconnectbtn();
	afx_msg void OnExecutebtn();
	afx_msg void OnDestroy();
	afx_msg void OnAllcancelbtn();
	afx_msg void OnAllselectbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARLISTUPDATETOOLDLG_H__094A7432_6953_4D6E_A5FE_3E4796068674__INCLUDED_)
