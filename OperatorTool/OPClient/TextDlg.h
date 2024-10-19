#if !defined(AFX_TEXTDLG_H__C1492258_A318_4BDD_A67C_1D9F9691B954__INCLUDED_)
#define AFX_TEXTDLG_H__C1492258_A318_4BDD_A67C_1D9F9691B954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextDlg.h : header file
//
#include "MyEdit.h"
#include "Undo.h"

/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog

class TextModify;
class OnlineText;
class CText;
class COPClientDlg;
class CTextDlg : public CDialog
{
// Construction
public:
	CTextDlg(CWnd* pParent = NULL);   // standard constructor	

	char		*GetText(UINT ID);
	UINT		GetEmptyTextID();
	OnlineText	*GetOnlineText()	{ return (OnlineText*)m_pText; }	
	void		TextSave();
	void		ModifySave(char *p, UINT id);

// Dialog Data
	//{{AFX_DATA(CTextDlg)
	enum { IDD = IDD_DIALOG_TEXT };
	CMyEdit	m_Edit;	
	CListBox	m_ListDesc;	
	CString	m_strEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(UINT nID, CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	friend class UnDo;
	enum {TEXT_MODE, QUEST_MODE, HELP_MODE, MINIGAME_MODE};

	TextModify		*m_pModify;
	COPClientDlg	*m_pDlg;
	CText			*m_pText;

	int				m_nNowStuts;
	UINT			m_nModifyTextID;	

	// Generated message map functions
	//{{AFX_MSG(CTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonText();
	afx_msg void OnButtonQuest();
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonMini();
	afx_msg void OnDblclkListDesc();
	afx_msg void OnReturnMsg(WPARAM, LPARAM);
	virtual void OnOK();
	afx_msg void OnButtonExit();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void		sortText();
	void		sortQuest();
	void		sortHelp();
	void		sortMiniGame();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDLG_H__C1492258_A318_4BDD_A67C_1D9F9691B954__INCLUDED_)
