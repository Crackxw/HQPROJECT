// CrashMonitorClientDoc.h : interface of the CCrashMonitorClientDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRASHMONITORCLIENTDOC_H__CD5A3C31_3AA1_479C_8D1E_594181D7A704__INCLUDED_)
#define AFX_CRASHMONITORCLIENTDOC_H__CD5A3C31_3AA1_479C_8D1E_594181D7A704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCrashMonitorClientDoc : public CDocument
{
protected: // create from serialization only
	CCrashMonitorClientDoc();
	DECLARE_DYNCREATE(CCrashMonitorClientDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashMonitorClientDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCrashMonitorClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCrashMonitorClientDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHMONITORCLIENTDOC_H__CD5A3C31_3AA1_479C_8D1E_594181D7A704__INCLUDED_)
