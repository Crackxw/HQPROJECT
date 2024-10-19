////////////////////////////////////////////////////////////////////////////////////
// ListSelectData.cpp -- implementation file for class CListSelectData
//
// 
//

#include "stdafx.h"
#include "ListSelectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ITEMLEN 40

//------------------------------------------------------------------------------
// CListSelectData::CListSelectData(CStringArray* pData)
//
//
// This constructor initializes a CListSelectData object, pData points to an
// array of strings which contains the full list of strings.
//------------------------------------------------------------------------------
CListSelectData::CListSelectData(CStringArray* pData)
{
	ASSERT(pData != NULL);
	m_pData   = pData;
}


CListSelectData::~CListSelectData()
{
}
//---------------------------------------------------------------------------
// void CListSelectData::FillListCtrl(CListCtrl& listCtrl)
//
// Fill the list ctrl with ALL strings from m_pData 
//---------------------------------------------------------------------------
void CListSelectData::FillListCtrl(CListCtrl& listCtrl)
{
	LV_ITEM lvI;
	// Initialize LV_ITEM members that are common to all items
	lvI.mask      = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvI.state     = 0;
	lvI.stateMask = 0;
	lvI.iImage    = 0;

    CString str;
	for (int i = 0; i < m_pData->GetSize(); i++)
	{
		lvI.iItem    = i;
		lvI.iSubItem = 0;
		str          = (*m_pData)[i];
		lvI.pszText  = (LPSTR)LPCTSTR(str);

		lvI.cchTextMax = MAX_ITEMLEN;
		lvI.lParam = (LPARAM) i;
        if (listCtrl.InsertItem(&lvI) == -1)
		    AfxMessageBox(_T("ListSelectData::FillListCtrl error"), MB_OK);
	}
}

//+--------------------------------------------------------------------------
// void CListSelectData::FillListCtrl(CListCtrl& listCtrl, CWordArray& aIdx)
//
// FILL THE LIST CTRL with strings from m_pData whose indexes are stored in 
// aIdx array. 
//---------------------------------------------------------------------------
void CListSelectData::FillListCtrl(CListCtrl& listCtrl, CWordArray& aIdx)
{
	LV_ITEM lvI;
	// Initialize LV_ITEM members that are common to all items
	lvI.mask      = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvI.state     = 0;
	lvI.stateMask = 0;
	lvI.iImage    = 0;

    CString str;
	for (int i = 0; i < aIdx.GetSize(); i++)
	{
		int idx = aIdx[i];
		ASSERT(idx>=0 && idx<m_pData->GetSize());

		lvI.iItem    = i;
		lvI.iSubItem = 0;
		str          = (*m_pData)[idx];
		lvI.pszText  = (LPSTR)LPCTSTR(str);

		lvI.cchTextMax = MAX_ITEMLEN;
		lvI.lParam = (LPARAM) idx;
		if (listCtrl.InsertItem(&lvI) == -1)
		    AfxMessageBox(_T("ListSelectData::FillListCtrl error"), MB_OK);
	}
}

//-----------------------------------------------------------------------------
// void CListSelectData::FillIndexArray(CListCtrl& listCtrl, CWordArray& aIdx)
//
//
// FILL THE INDEX ARRAY with the indexes (in m_pData) of the strings contained 
// in the list control.
//-----------------------------------------------------------------------------
void CListSelectData::FillIndexArray(CListCtrl& listCtrl, CWordArray& aIdx)
{
	LV_ITEM lvI;
	// Initialize LV_ITEM members that are common to all items
	lvI.mask       = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvI.state      = 0;
	lvI.stateMask  = 0;
	lvI.iImage     = 0;
	lvI.cchTextMax = MAX_ITEMLEN;
	TCHAR sText[MAX_ITEMLEN];
	int nIdx = listCtrl.GetItemCount();
	aIdx.SetSize(nIdx);

	for (int i = 0; i < listCtrl.GetItemCount(); i++)
	{
		lvI.iItem       = i;
		lvI.iSubItem    = 0;
   		lvI.pszText     = (LPSTR)LPCTSTR(sText);
		lvI.cchTextMax  = MAX_ITEMLEN;
		listCtrl.GetItem(&lvI);
		aIdx[i]         = lvI.lParam; // Store String array(m_pData) index
	}
}
