////////////////////////////////////////////////////////////////////
// DDListCtrl.cpp -- implementation file
//
// Extended List Control class with OLE Drag & Drop and optional:
//             - local drag & drop
//             - automatic scrolling,
//             - Removing of dragged items 
// 
//
// Author: Jean-Claude DAUPHIN, CI/INF UNESCO
//
// The following source of information have been invaluable for
// succeeding in this task:
//
// Lstdrg.exe -- Microsoft sample which implement OLE drag & drop
// for a ListBox Control and an Edit Control.
//
// Wicked code article written by J. Prosise (MSJ August 98) 
///////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DDListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDDListCtrl


//------------------------------------------------------------------------------
// void CDDListCtrl::Initialize()
//
// Calls the Register method of m_DropTarget to register this control as a drop
// target with the OLE dll's.
//
// We need to call this method in the Dialog CMyDlg::OnInitDialog() or 
// CFormView::OnInitialUpdate for controls created by a dialog template since
// the OnCreate does not get called for template created  controls (this is
// because the windows controls get created before the CWnd wrappers).
//-----------------------------------------------------------------------------
void CDDListCtrl::Initialize()
{
    // Register the COleDropTarget object with the OLE dll's
    BOOL bSuccess = m_DropTarget.Register(this);
    if (!bSuccess)
       AfxMessageBox("Ole Register Drop Target Failed");     
}                                                                             


CDDListCtrl::CDDListCtrl()
{
	m_iDragItem  = -1;
	m_bDragging  = FALSE;
	m_pImageList = NULL;

	m_bSource = FALSE;
	m_bTarget = FALSE;

	// By default, all options are available
	m_bLocalDD   = TRUE; // Local Drag & Drop
	m_bScrolling = TRUE; // Auto Scrolling
	m_bRemove    = TRUE; // Remove items

	m_pSaveIndItem = NULL;
	m_nItem = 0;

    m_nDelayInterval = 300;  // Default delay interval = 500 milliseconds
	m_nScrollMargin = 10;    // Default scroll margin = 10 pixels
	m_nScrollInterval = 100; // Default scroll interval = 200 milliseconds
}

CDDListCtrl::~CDDListCtrl()
{
	// Delete the image list created by CreateDragImage
	if (m_pImageList != NULL)
		delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CDDListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CDDListCtrl)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDListCtrl message handlers

//---------------------------------------------------------------------
// int CDDListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
//
// THIS FUNCTION ONLY GETS CALLED FOR DYNAMICALLY CREATED CONTROLS.
//
// FOR CONTROLS CREATED BY A DIALOG TEMPLATE , Initialize() must be 
// called in the OnInitialUpdate method of the CFormView or CMyDialog.
//---------------------------------------------------------------------
int CDDListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)           
        return -1;
     
	//SetExtendedStyle(LVS_EX_GRIDLINES);
    
	// Register the COleDropTarget object with the OLE dll's
    BOOL bSuccess = m_DropTarget.Register(this);
    
    if (!bSuccess) 
    {
       AfxMessageBox("Ole Register Drop Target Failed");     
       return -1;
    }
     return 0;
}

//-------------------------------------------------------------------------------
// void CDDListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//
// LVN_BEGINDRAG notifies a list view control's parent window that a drag-and-drop
// operation involving the left mouse button is being initiated. Message 
// reflection lets us handle messages for the control, within the control itself.
//-------------------------------------------------------------------------------

void CDDListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
    int iItem =  ((NM_LISTVIEW*) pNMHDR)->iItem;

	m_iDragItem = iItem;   // Item on which D&D is started
	m_iDropItem = -1;      // Not yet known
	m_bDragging = TRUE;    // D&D started
	
	m_bSource = TRUE;      // We start D&D on the source
	m_bTarget = FALSE;     // Not yet known 

	m_nItem = GetSelectedCount();    // Used for counting number of selected items
	
	// Allocate an array for saving item indexes
	m_pSaveIndItem = new int[m_nItem];
	
	// Create global memory for sharing dragdrop data, we allocate
	// space for 2 int and nItem ListInfo structures
    HGLOBAL hgData = GlobalAlloc(GPTR,m_nItem*sizeof(ListInfo)+2*sizeof(int));   
    ASSERT(hgData!=NULL);
    char* p = (char*) GlobalLock(hgData);
    ASSERT(p!=NULL);

	// Place control type in first 2 bytes
	*(int *) (p) = CT_LIST;
     
	// Place number of list items in next 2 bytes
	*(int *) (p+sizeof(int)) = m_nItem;
	
	//  Then the nItem ListInfo structures will start at:
    ListInfo* lpData = (ListInfo*) (p + 2*sizeof(int));

	int jItem = -1;
    int i = 0;
	while ((jItem = GetNextItem(jItem,LVNI_ALL | LVNI_SELECTED)) > -1)
    {
       ASSERT(i>=0 && i <m_nItem);
	   ZeroMemory(&lpData[i], sizeof(ListInfo));
	
	   lpData[i].lvi.mask       = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	   lpData[i].lvi.stateMask  = LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
       lpData[i].lvi.pszText    = (char*) &(lpData[i].szLabel);
	   lpData[i].lvi.iItem      = jItem;
	   lpData[i].lvi.cchTextMax = 255;
	
	   BOOL bSuccess = GetItem(&(lpData[i].lvi));
	   ASSERT(bSuccess);
	   m_pSaveIndItem[i] = jItem;
       i++;
	}
	
	// Cache the data, and initiate DragDrop
    m_COleDataSource.CacheGlobalData(CF_TEXT, hgData);           

	// Set drag/drop source rectangle (screen coord)
    SetDragSourceRect();
    
    DROPEFFECT dropEffect =
       m_COleDataSource.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE,NULL);  
     
     
    if (m_bRemove && (dropEffect&DROPEFFECT_MOVE)==DROPEFFECT_MOVE)     
    {
        //  Delete items in reverse order so that indexes are preserved
	   for (i=m_nItem-1; i>=0; i--)
	   {
          jItem = m_pSaveIndItem[i];
		  // If the list is source and target and..
  	  	  // Index of dragged item will change if dropped above itself
		  if (m_bSource && m_bTarget)
		  {
		     ASSERT(m_iDropItem>=0);
		     if (jItem > m_iDropItem) jItem += m_nItem;
          }
		  BOOL bSuccess = DeleteItem(jItem);
		  ASSERT(bSuccess);
	   }
        // Set the list box selected item to the previous item
        //SetCurSel(nItem-1);                                            
    }
	m_bSource = FALSE;
	m_bTarget = FALSE;
	m_iDropItem = -1;
	delete[] m_pSaveIndItem;
	m_pSaveIndItem = NULL;
	m_nItem = 0;
	*pResult = 0;
}

//----------------------------------------------------------------------------------
// void CDDListCtrl::OnTimer(UINT nIDEvent) 
//
// Implements automatic scrolling by setting a timer when the cursor moves
// within m_nScrollMargin pixels (default=10) of the list ctrl window's top or 
// bottom border. The relevant code is in COleListCtrlDropTarget::OnDragOver:
//
// if (( point.y <= pDDList->m_nScrollMargin) ||
//		  (point.y >= cy - pDDList->m_nScrollMargin))
//	   {
//		  TRACE("\nTimer set for scrolling");
//		  pDDList->SetTimer (1, pDDList->m_nDelayInterval, NULL);
//	   }
// Initially, the timer is set to fire after 500 milliseconds. If the cursor moves
// before the timer fires, the timer is reset. If however the cursor relains 
// motionless until the timer interval expires, a WM_TIMER ensues and 
// CDDListCtrl::OnTimer scrolls the window by sending it a WS_VSCROLL message.
// OnTimer also programs the timer to fire again after 200  milliseconds so that 
// subsequent scrolls will hapen more quickly. The messages don't stop until the 
// cursor is moved or the list view is scrolled as far as it possibly can. 
//----------------------------------------------------------------------------------
void CDDListCtrl::OnTimer(UINT nIDEvent) 
{
	CListCtrl::OnTimer(nIDEvent);
	if (!m_bScrolling)
		return;
    
	// Reset the timer.
	SetTimer (1, m_nScrollInterval, NULL);

	// Get the current cursor position and window height.
	DWORD dwPos = ::GetMessagePos ();
	CPoint point (LOWORD (dwPos), HIWORD (dwPos));
	ScreenToClient (&point);

	CRect rect;
	GetClientRect (rect);
	int cy = rect.Height ();
    
	// Scroll the window if the cursor is near the top or bottom.
	if (point.y <= m_nScrollMargin) 
	{
		int iFirstVisible = GetTopIndex ();

		m_pImageList->DragShowNolock (FALSE);
		SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEUP, 0), NULL);
		m_pImageList->DragShowNolock (TRUE);

		//  Kill the timer if the window did not scroll, or redraw the
		//  drop target highlight if the window did scroll.
		if (GetTopIndex () == iFirstVisible)
			KillTimer (1);
		else 
		{
			HighlightDropTarget (point);
			return;
		}
	}
	else if (point.y >= cy - m_nScrollMargin) 
	{
		int iFirstVisible = GetTopIndex ();
		m_pImageList->DragShowNolock (FALSE);

		SendMessage (WM_VSCROLL, MAKEWPARAM (SB_LINEDOWN, 0), NULL);
		m_pImageList->DragShowNolock (TRUE);

		// Kill the timer if the window did not scroll, or redraw the
		// drop target highlight if the window did scroll.
		if (GetTopIndex () == iFirstVisible)
			KillTimer (1);
		else 
		{
			HighlightDropTarget (point);
			return;
		}
	}
}

//----------------------------------------------------------------------------------
// int CDDListCtrl::HighlightDropTarget(CPoint point)
//
// Highlights the item on which the cursor is over.
//----------------------------------------------------------------------------------
int CDDListCtrl::HighlightDropTarget(CPoint point)
{
	// Find out which item (if any) the cursor is over.
	UINT nFlags;
	int iItem = HitTest (point, &nFlags);
	if (iItem == -1)
		return iItem;
	
	//  Highlight the item, or unhighlight all items
	//  if the cursor isn't over an item.
	SelectDropTarget (iItem);

	// Return the index of the highlighted item.
	return iItem;
}


void CDDListCtrl::RemoveHighlightFromDropTarget()
{
   if (m_iDropItem >= 0)
   {
      BOOL bSuccess = SetItemState (m_iDropItem, 0, LVIS_DROPHILITED);
	  //ASSERT(bSuccess);
	  /**
       * Redraw item
	   **/
	  RedrawItems (m_iDropItem, m_iDropItem);
   }
}

//----------------------------------------------------------------------------------
// void CDDListCtrl::SelectDropTarget(int iItem)
// 
// if iItem >= 0, updates the highlighted item, else if iItem<0, unlights all items 
//----------------------------------------------------------------------------------
void CDDListCtrl::SelectDropTarget(int iItem)
{
	BOOL bSuccess;
	if (iItem >=0)
	{
	   RemoveHighlightFromDropTarget();

	   // Highlight the new one
	   bSuccess = SetItemState(iItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
	   ASSERT(bSuccess);
       RedrawItems (iItem, iItem);
       m_iDropItem = iItem;
	   UpdateWindow ();
	}
	else
	{
		for (int i=0; i<GetItemCount(); i++)
		{
			bSuccess = SetItemState (i, 0, LVIS_DROPHILITED);
			ASSERT(bSuccess);
		}

		CWnd* pOwner = GetParent();
		pOwner->SendMessage( COPY_FILE_MESSAGE );
		UpdateWindow ();
	}
}

CRect CDDListCtrl::GetDragSourceRect()
{   
     //  return the drag/drop source rect (maintained in view class)
    return m_DragSourceRect;
}

void CDDListCtrl::SetDragSourceRect()
{                                                            
    CRect ListWindowRect;
    
    GetWindowRect(&ListWindowRect);

    // Set drag/drop source rectangle (screen coord)
 	// *(maintained in the view class)
    m_DragSourceRect = ListWindowRect;    
}

/////////////////////////////////////////////////////////////////////////////
// COleListCtrlDropTarget - Enable OLE dragdrop for the ListCtrl control

COleListCtrlDropTarget::COleListCtrlDropTarget() {}

COleListCtrlDropTarget::~COleListCtrlDropTarget() {}

//-----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
// 
// Called by OLE dll's when drag cursor FIRST enters a window that is REGISTERED
//  with the OLE dll's
//
//Parameters
//----------
//
// pWnd        -- Points to the window the cursor is entering.
// pDataObject -- Points to the data object containing the data
//                that can be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the following:
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
//
// point       -- Contains the current location of the cursor in
//                client coordinates.
//
// Return Value : The effect that would result if a drop were 
//-------------   attempted at the location specified by point.
//                It can be one or more of the following: 
//
//     DROPEFFECT_NONE   -A drop would not be allowed.
//     DROPEFFECT_COPY   -A copy operation would be performed.
//     DROPEFFECT_MOVE   -A move operation would be performed.
//     DROPEFFECT_LINK   -A link from the dropped data to the 
//                        original data would be established.
//     DROPEFFECT_SCROLL -A drag scroll operation is about to occur 
//                        or is occurring in the target. 
//-----------------------------------------------------------------------------------
DROPEFFECT COleListCtrlDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{
   	CDDListCtrl* pDDList = (CDDListCtrl*) pWnd;
 
	// If local Drag&Drop not allowed and point is inside of the
	// source list ctrl, don't allow a drop
	if (!(pDDList->m_bLocalDD) && pDDList->m_bSource)
	{
		CPoint pt = point;
        pWnd->ClientToScreen(&pt); // Convert client point to screen point
        if (pDDList->GetDragSourceRect().PtInRect(pt))
           return DROPEFFECT_NONE;
	}
	
	// Check if the Control key was pressed
    if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY; // Copy source text
    else
        return DROPEFFECT_MOVE; // Move source text   
}

//---------------------------------------------------------------------------------
// void COleListCtrlDropTarget::OnDragLeave(CWnd* pWnd)
//
// Called by OLE dll's when drag cursor leaves a window that is REGISTERed with 
// the OLE dll's.
//
// pWnd points to the window the cursor is leaving.
//---------------------------------------------------------------------------------
void COleListCtrlDropTarget::OnDragLeave(CWnd* pWnd)
{
   	CDDListCtrl* pDDList = (CDDListCtrl*) pWnd;
	pDDList->RemoveHighlightFromDropTarget();

    // Call base class implementation
    COleDropTarget:: OnDragLeave(pWnd);
}

//----------------------------------------------------------------------------------
// DROPEFFECT COleListCtrlDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
//           pDataObject, DWORD dwKeyState, CPoint point )
//
// Called by OLE dll's when cursor is dragged over a window that is REGISTERed with
// the OLE dll's.
//
//Parameters
//----------
// pWnd        -- points to the window that the cursor is over.
// pDataObject -- points to the data object that contains the data
//                to be dropped.
// dwKeyState  -- Contains the state of the modifier keys.
//                This is a combination of any number of the
//                following: 
//                MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
//                MK_MBUTTON, and MK_RBUTTON.
// point       -- Contains the current location of the cursor in 
//                client coordinates.
// 
//Return Value:
//------------   The effect that would result if a drop were attempted
//               at the location specified by point. 
//               It can be one or more of the following: 
//
//DROPEFFECT_NONE   A drop would not be allowed.
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established.
//
//DROPEFFECT_SCROLL   Indicates that a drag scroll operation is about to occur or is occurring in the target. 
//
// Note: OnDragOver is like WM_MOUSEMOVE message, in that CALLS TO IT
// ARE GENERATED REPEATEDLY as the pointer moves around the window
//----------------------------------------------------------------------------------
DROPEFFECT COleListCtrlDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{                              

	CDDListCtrl* pDDList = (CDDListCtrl*) pWnd;

	pDDList->KillTimer (1);

	// If local Drag & Drop not allowed and drop point is inside 
	// of the source list ctrl, don't allow a drop.
	if (!(pDDList->m_bLocalDD) && pDDList->m_bSource)
	{
		CPoint pt = point;
	    pWnd->ClientToScreen(&pt);   // convert client to screen 
        if (pDDList->GetDragSourceRect().PtInRect(pt))
           return DROPEFFECT_NONE;
	}
	
    if (pDDList->m_bScrolling)
	{
	    // Stop the scroll timer if it's running.
	    pDDList->KillTimer (1);
	}
	// Highlight the drop target if the cursor is over an item.
	int iItem = pDDList->HighlightDropTarget (point);

	if (pDDList->m_bScrolling)
	{
	   // Set a timer if the cursor is at the top or 
	   // bottom of the window.
	   CRect rect;
	   pDDList->GetClientRect (rect);
	   int cy = rect.Height ();

	   if (( point.y <= pDDList->m_nScrollMargin) ||
		  (point.y >= cy - pDDList->m_nScrollMargin))
	   {
		  TRACE("\nTimer set for scrolling");
		  pDDList->SetTimer (1, pDDList->m_nDelayInterval, NULL);
	   }
	}
    if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;    
}

//---------------------------------------------------------------------------------- 
// BOOL COleListCtrlDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
//                 DROPEFFECT dropEffect, CPoint point )
//
// Called by OLE dll's when item is dropped in a window that is REGISTERed with the
// OLE dll's.
//
//Parameters
//----------
// pWnd        -- Points to the window the cursor is currently over.
//
// pDataObject -- points to the data object that contains the data
//                to be dropped.
//
// dropEffect  -- The effect that the user chose for the drop operation. It can be one or more of the following: 
//
//DROPEFFECT_COPY   A copy operation would be performed.
//
//DROPEFFECT_MOVE   A move operation would be performed.
//
//DROPEFFECT_LINK   A link from the dropped data to the original data would be established. 
//
// point       -- Contains the location of the cursor, in pixels,
//                relative to the screen.
//---------------------------------------------------------------------------------- 
BOOL COleListCtrlDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{           
    HGLOBAL   hGlobal;

	TRACE("\nOnDrop");

	CDDListCtrl* pDDList = (CDDListCtrl*) pWnd;

	// Stop the scroll timer if it's running.
	pDDList->KillTimer (1);
	
	// Flag the list as target
	pDDList->m_bTarget = TRUE;
	
	// Find out the drop item
	int iItem;
	if (pDDList->GetItemCount() == 0)
		iItem = 0;  // List is empty
	else
	{
	   // Find drop element in list
       UINT nFlags;
	   point.y += 5;
	   iItem = pDDList->HitTest (point, &nFlags);
	   if (iItem == -1)
	   {
		   // Get coordinates of top and bottom visible points
		   int iTopItem = pDDList->GetTopIndex();
		   int iBotItem = iTopItem + pDDList->GetCountPerPage() -1;
		   if (iBotItem > pDDList->GetItemCount()-1)
			   iBotItem = pDDList->GetItemCount() - 1;
		   CPoint topPoint, botPoint;
		   BOOL bSuccess = pDDList->GetItemPosition(iTopItem, &topPoint);
		   ASSERT(bSuccess);
		   bSuccess = pDDList->GetItemPosition(iBotItem, &botPoint);
		   ASSERT(bSuccess);

		   if (point.y <= topPoint.y+5)
			  iItem = 0;
		   else if (point.y >botPoint.y)
		      iItem = pDDList->GetItemCount(); // Add at the end
           else
		   {
   			  // If there was an error adding the text to the control
              // return FALSE so that the text is not removed from
			  // the drop source
			  return FALSE;
		   }

	   }
	}
    pDDList->m_iDropItem = iItem;
	ASSERT(pDDList->m_iDropItem>=0);

	if (pDDList->m_bSource && pDDList->m_bTarget)
	{	
		// Check that we don't drop on dragged items
		TRACE("\niItem=%d pSaveInItem[0]=%d pSaveInItem[0]=%d",
			          iItem,pDDList->m_pSaveIndItem[0],pDDList->m_pSaveIndItem[pDDList->m_nItem-1]); 
		if (iItem >= pDDList->m_pSaveIndItem[0] &&
		    iItem <= pDDList->m_pSaveIndItem[pDDList->m_nItem-1])
		{
		    // Display an error message if the move is illegal.
		    AfxMessageBox (_T ("An item can't be dropped onto itself"));
		    pDDList->SelectDropTarget(-1);
			// If there was an error adding the text to the control
            // return FALSE so that the text is not removed from
			// the drop source
			return FALSE;
		}
	 }

    // Get List data from COleDataObject
	hGlobal = pDataObject->GetGlobalData(CF_TEXT);
	
	// Get pointer to data
    PVOID p = GlobalLock(hGlobal);    
    ASSERT(p!=NULL);

	// Get the type of source control
	int ctrlType = *(int *) (p);
	if (ctrlType == CT_LIST)
	{
	   // Source Ctrl is a list, get number of items
	   int nItem = *(int *) ((char*)p + sizeof(int));
	   ListInfo* pData = (ListInfo*) ((char*)p + 2*sizeof(int));
       
	   // Add the item(s) to the list
   	   int iNewItem;
	   for (int i=0; i<nItem; i++)
	   {
	      pData[i].lvi.iItem = iItem;
	      iItem++;
	      iNewItem = pDDList->InsertItem(&(pData[i].lvi));
          ASSERT(iNewItem >= 0);
	   }
	   
	   // Unlight all list items
       pDDList->SelectDropTarget(-1);
	   
	   // Select the newly added item.
       BOOL bSuccess = pDDList->SetItemState(iNewItem, LVIS_SELECTED, LVIS_SELECTED);
    }
	else
	{
		AfxMessageBox("List -- Source control not defined");
	}
	
	// Unlock memory
    GlobalUnlock(hGlobal);
    
    return TRUE;
}
