#if !defined(AFX_CHARDLG_H__D0D710F4_579B_471F_ABD2_3FB9200E9474__INCLUDED_)
#define AFX_CHARDLG_H__D0D710F4_579B_471F_ABD2_3FB9200E9474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharDlg.h : header file
//

#include "MegaDC.h"

/////////////////////////////////////////////////////////////////////////////
// CCharDlg dialog
struct CharHead;
class XSPR;
class COPClientDlg;
class CharParser;
class CCharDlg : public CDialog
{
// Construction
public:
	CCharDlg(CWnd* pParent = NULL);   // standard constructor

	void	FillControl(CharHead *pHeader);
	void	DrawChar(CharHead *pHeader);
	void	DrawItem(int index, int nItemID);


	void	SaveToFile(FILE *fp);
	void	SaveToFile(FILE *fp, CharHead *pHeader );
	char	*GetWeaponType(int type);

	// Dialog Data
	//{{AFX_DATA(CCharDlg)
	enum { IDD = IDD_DIALOG_CHAR };
	CComboBox	m_ItemList[3];
	CComboBox	m_CharList;
	int		m_nStr;
	int		m_nDex;
	int		m_nVit;
	int		m_nInt;
	int		m_nHp;
	int		m_nMp;
	int		m_nAc;
	int		m_nSpeed;
	int		m_nDR;
	int		m_MR;
	int		m_nMinD;
	int		m_nMaxD;
	CString	m_strDesc;
	int		m_nCost;
	int		m_nPercent[3];
	int		m_nLift;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create(UINT nID, CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	enum { CHAR, ITEM01, ITEM02, ITEM03, MAX_DC };

	COPClientDlg	*m_pDlg;
	CharParser		*m_pCharParser;	
	XSPR			*m_pSpr;

	CMegaDC			m_DC[MAX_DC];
	
	// Generated message map functions
	//{{AFX_MSG(CCharDlg)
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCharName();
	afx_msg void OnPaint();
	afx_msg void OnButtonModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARDLG_H__D0D710F4_579B_471F_ABD2_3FB9200E9474__INCLUDED_)
