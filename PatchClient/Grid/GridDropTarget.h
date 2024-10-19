//==================================================================
//   GridDropTarget.h
//  Drag & Drop in GridCtrl
//
// Produced   by    Written by Chris Maunder  (Copyright (c) 1998.)
//==================================================================

#ifndef GRID_DROP_TARGET_H
#define GRID_DROP_TARGET_H

#include <afxole.h>


class CGridCtrl;


///==============================================================================
// CGridDropTarget Class
//	 class for Drag&Drop
//==============================================================================
class CGridDropTarget : public COleDropTarget
{
public:
    CGridDropTarget();
    virtual ~CGridDropTarget();

// Attributes
public:
    CGridCtrl* m_pGridCtrl;
    BOOL       m_bRegistered;

// Operations
public:
    BOOL Register(CGridCtrl *pGridCtrl);
    virtual void Revoke();

    BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void        OnDragLeave(CWnd* pWnd);
    DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridDropTarget)
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGridDropTarget)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

#endif 