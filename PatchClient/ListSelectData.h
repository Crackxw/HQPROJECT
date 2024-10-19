/////////////////////////////////////////////////////////////////////
// ListSelectData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListSelectData
//
// CListSelectData objects store a pointer to the CStringArray object, 
// initialized externally to contain the complete list of strings from which
// the selection  with occur.
 
class CListSelectData 
{
protected:
   // Attributes
   CStringArray* m_pData;

   // Implementation
public:
	CListSelectData(CStringArray* pData);        

	virtual ~CListSelectData();

	// Extract list item indexes
    virtual void FillIndexArray(CListCtrl& listCtrl, CWordArray& aIdx);
   
	// Fill List Ctrl 
	virtual void FillListCtrl(CListCtrl& listCtrl);
    virtual void FillListCtrl(CListCtrl& listCtrl, CWordArray& aIdx);
};
