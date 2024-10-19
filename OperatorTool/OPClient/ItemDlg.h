#if !defined(AFX_ITEMDLG_H__FEEF3100_7CE3_4B5B_A620_1BBFB5583DD4__INCLUDED_)
#define AFX_ITEMDLG_H__FEEF3100_7CE3_4B5B_A620_1BBFB5583DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDlg.h : header file
//

#include "MegaDC.h"

/////////////////////////////////////////////////////////////////////////////
// CItemDlg dialog
struct CItemHeader;
class ItemParser;
class COPClientDlg;
class XSPR;
class CItemDlg : public CDialog
{
// Construction
public:
	CItemDlg(CWnd* pParent = NULL);   // standard constructor

	CItemHeader	*GetItem(int id);
	CItemHeader	*GetItemIndex(int index);
	int			GetItemNun();


	void		FillControls(CItemHeader *pItem);

	void		SetGroup(CItemHeader *pItem);
	void		GetGroup(CItemHeader *pItem);	

	void		DrawItem(CItemHeader *pItem);

	BOOL		CheckAll();
	void		DisableControl(BOOL b);


	void		SetReadOnlyMode(BOOL b);

// Dialog Data
	//{{AFX_DATA(CItemDlg)
	enum { IDD = IDD_DIALOG_ITEM };
	CComboBox	m_Magic;
	CComboBox	m_AttackType;
	CComboBox	m_Unit;
	CComboBox	m_Kind;
	CComboBox	m_Attribute;
	CComboBox	m_Group;
	CComboBox	m_ImageS;
	CComboBox	m_ImageL;
	CComboBox	m_Tool[3];
	CComboBox	m_Element[5];
	CComboBox	m_ItemName;
	int		m_nID;
	int		m_ElementNum[5];
	int		m_nWork;
	int		m_nHp;
	int		m_nMp;
	int		m_nStr;
	int		m_nDex;
	int		m_nInt;
	int		m_nVit;
	int		m_nAc;
	int		m_nDamageRegist;
	int		m_nMagicRegist;
	int		m_nSound;
	int		m_nHealth;
	int		m_nFontL;
	int		m_nFontS;
	int		m_nMaxQuantity;
	int		m_nAttUp;
	int		m_nDefUp;
	int		m_nMinD;
	int		m_nMaxD;
	int		m_nAttackRange;
	int		m_nReqLv;
	int		m_nMagicAttack;
	CString m_strModify;
	CString	m_strDescription;
	CString	m_strMagicDescription;
	int		m_nDefence;
	int		m_nDrawID;
	int		m_nSource;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDlg)
	public:
	virtual BOOL Create(UINT nID, CWnd* pParentWnd);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	ItemParser		*m_pItemParser;
	COPClientDlg	*m_pDlg;
	XSPR			*m_pSpr;

	CMegaDC			m_DC;	

	// Generated message map functions
	//{{AFX_MSG(CItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeItemName();
	afx_msg void OnPaint();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeComboGroup();		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDLG_H__FEEF3100_7CE3_4B5B_A620_1BBFB5583DD4__INCLUDED_)
