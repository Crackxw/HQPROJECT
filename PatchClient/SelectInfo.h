//////////////////////////////////////////////////////////////////////
// SelectInfo.h -- Interface for the CSelectInfo class.
//                 A dummy class for demo  

#if !defined(AFX_SELECTINFO_H__F68B2E26_A10D_11D4_8C0D_0050043EB83D__INCLUDED_)
#define AFX_SELECTINFO_H__F68B2E26_A10D_11D4_8C0D_0050043EB83D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelectInfo  
{
public:
	CStringArray m_asNames;
	CWordArray m_aRowsVar;
	CWordArray m_aColsVar;
	CWordArray m_aDataVar;
	CWordArray m_aPagesVar;
public:
	CSelectInfo();
	virtual ~CSelectInfo();

};

#endif // !defined(AFX_SELECTINFO_H__F68B2E26_A10D_11D4_8C0D_0050043EB83D__INCLUDED_)
