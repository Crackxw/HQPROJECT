// OPClientDlg.h : header file
//

#if !defined(AFX_OPCLIENTDLG_H__7A96A697_29B7_4F9F_90F3_585B6B3C3462__INCLUDED_)
#define AFX_OPCLIENTDLG_H__7A96A697_29B7_4F9F_90F3_585B6B3C3462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COPClientDlg dialog

#include "Client.h"
#include "undo.h"
#include "TextDlg.h"
#include "CharDlg.h"
#include "ItemDlg.h"
#include "ModifyDlg.h"


struct CItemHeader;
class OnlineText;
class CTextDlg;
class COPClientDlg : public CDialog
{
// Construction
public:
	COPClientDlg(CWnd* pParent = NULL);	// standard constructor

	void		disableLogin(int disable);

	UINT		GetEmptyTextID()			{ return m_TextDlg.GetEmptyTextID(); }
	char		*GetText(UINT ID)			{ return m_TextDlg.GetText(ID); }
	OnlineText	*GetOnlineText()			{ return m_TextDlg.GetOnlineText(); }
	void		TextSave()					{ m_TextDlg.TextSave(); }
	void		ModifySave(char *p, UINT i) { m_TextDlg.ModifySave(p, i); }

	CItemHeader	*GetItem(int id)			{ return m_ItemDlg.GetItem(id); }
	CItemHeader	*GetItemIndex(int index)	{ return m_ItemDlg.GetItemIndex(index); }
	int			GetItemNun()				{ return m_ItemDlg.GetItemNun(); }

// Dialog Data
	//{{AFX_DATA(COPClientDlg)
	enum { IDD = IDD_OPCLIENT_DIALOG };
	CString	m_strID;
	CString	m_strPW;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CModifyDlg	m_ModifyDlg;

protected:
	HICON		m_hIcon;
	
	CClient		m_ClientSocket;
	BOOL		m_bConnect;	

	friend class UnDo;
	CTextDlg	m_TextDlg;
	CCharDlg	m_CharDlg;
	CItemDlg	m_ItemDlg;
	

	// Generated message map functions
	//{{AFX_MSG(COPClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonLogin();
	afx_msg void OnButtonText();
	afx_msg void OnDestroy();
	afx_msg void OnButtonChar();
	afx_msg void OnButtonQuit();
	afx_msg void OnButtonItem();
	afx_msg	void OnConnect(WPARAM, LPARAM);
	afx_msg	void OnLogin(WPARAM, LPARAM);
	afx_msg	void OnItemEdit(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

BOOL		loadspr(char *file, XSPR &xspr);
void		SetFileName(char *pFileName);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPCLIENTDLG_H__7A96A697_29B7_4F9F_90F3_585B6B3C3462__INCLUDED_)
