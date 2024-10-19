//==================================================================
//   TitleTip.h
//  Title Tip Window
//
// Produced   by    Written by Chris Maunder  (Copyright (c) 1998.)
//==================================================================

#ifndef TITLETIP_H
#define TITLETIP_H


///==============================================================================
// CTitleTip Class
//	 Title Tip Window
//==============================================================================
class CTitleTip : public CWnd
{
// Construction
public:
	CTitleTip();
	virtual ~CTitleTip();

// Attributes
protected:
	CWnd *m_pParentWnd;
	CRect m_rectTitle;
    CRect m_rectHover;

// Operations
public:
	virtual BOOL Create( CWnd *pParentWnd);
	void Show(CRect rectTitle, LPCTSTR lpszTitleText, 
              int xoffset = 0, LPRECT lpHoverRect = NULL, LPLOGFONT lpLogFont = NULL);
    void Hide();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleTip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
// Generated message map functions
protected:
	//{{AFX_MSG(CTitleTip)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif 