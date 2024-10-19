////////////////////////////////////////////////////////////////////
// DDListCtrl -- Extended List Control class with local drag & drop and
//               automatic scrolling, plus OLE drap & drop
//
// Author: Jean-Claude DAUPHIN, CI/INF UNESCO
//

#if !defined(AFX_DDLISTCTRL_H__E418A1A1_BA82_11D2_B742_0000E2039B5F__INCLUDED_)
#define AFX_DDLISTCTRLEXT_H__E418A1A1_BA82_11D2_B742_0000E2039B5F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// COleListCtrlDropTarget window

#define COPY_FILE_MESSAGE	WM_USER+1

class COleListCtrlDropTarget : public COleDropTarget
{
// Construction
public:
    COleListCtrlDropTarget();

// Implementation
public:
    virtual ~COleListCtrlDropTarget();   
    
   /**
    * These members MUST be overridden for an OLE drop target
    * See DRAG and DROP section of OLE classes reference
    **/
   DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                               dwKeyState, CPoint point );
   void OnDragLeave(CWnd* pWnd);               
   
   BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );
};

//////////////////////////////////////////////
// Structure used for transfering list data
//
//typedef struct _LVITEM { 
//    UINT mask; 
//    int iItem; 
//    int iSubItem; 
//    UINT state; 
//    UINT stateMask; 
//    LPTSTR pszText; 
//    int cchTextMax; 
//    int iImage; 
//    LPARAM lParam;
//#if (_WIN32_IE >= 0x0300)
//    int iIndent;
//#endif
//} LVITEM, FAR *LPLVITEM;

struct ListInfo
{
	LV_ITEM lvi;
	char szLabel[256];
};

//////////////////////////////////
// Control types

enum {CT_LIST, CT_EDIT, CT_TREE};

/////////////////////////////////////////////////////////////////////////////
// CDDListCtrl window

class CDDListCtrl : public CListCtrl
{

// Construction
public:
	CDDListCtrl();

// Attributes
private: 
	/**
     * Needed to make this control an OLE data SOURCE 
	 * (see OnLButtonDown)
     **/
	 COleDataSource m_COleDataSource;

public:

// Operations
public:
    /*****   
     * Initialization method REQUIRED for controls created by
     * a dialog template. Dynamically created controls are  
     * initialized in the OnCreate. OnCreate is not called for 
	 * controls created by a template since the Windows controls
	 * are created before the CWnd objects.
     *****/
    void Initialize();
    
	/**
     * Get and set the source rectangle for drag/drop operation
     **/
	void  SetDragSourceRect();
    CRect GetDragSourceRect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDDListCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDDListCtrl();

    /*****
     * COleListCtrlDropTarget, derived from COleDropTarget gives us
     * the functionality to be an OLE drop target.
     * COleListCtrlDropTarget.Register() must be called to register 
     * the CWnd as an OLE drop target. 
     *****/
    COleListCtrlDropTarget m_DropTarget;
	CRect m_DragSourceRect;

    /** 
	 * Variables used for automatic scrolling
	 **/
	int m_nDelayInterval;
	int m_nScrollMargin;
	int m_nScrollInterval;

	BOOL        m_bDragging;
	CImageList* m_pImageList;
	int         m_iDragItem; // Source item index on which d&d started
	int         m_iDropItem; // Target item index

	BOOL        m_bSource;   // True if list is source
	BOOL        m_bTarget;   // True if list is target

	/**
	 * Options available
	 **/
	BOOL        m_bLocalDD;   // True if local Drag&Drop allowed
	BOOL        m_bScrolling; // True if auto scrolling
	BOOL        m_bRemove;    // True if D&D items must be removed

	void SetLocalDD(BOOL bState)   { m_bLocalDD   = bState; }
    void SetScrolling(BOOL bState) { m_bScrolling = bState; }
	void SetRemove(BOOL bState)    { m_bScrolling = bState; }

	/**
	 * Array used for saving D&D item indexes and delete them
	 **/
	int*        m_pSaveIndItem;
	int         m_nItem;

	void RemoveHighlightFromDropTarget();
	int HighlightDropTarget (CPoint point);
	void SelectDropTarget(int iItem);

	// Generated message map functions
protected:

	//{{AFX_MSG(CDDListCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDLISTCTRL_H__E418A1A1_BA82_11D2_B742_0000E2039B5F__INCLUDED_)
