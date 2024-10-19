// CrashMonitorClientDoc.cpp : implementation of the CCrashMonitorClientDoc class
//

#include "stdafx.h"
#include "CrashMonitorClient.h"

#include "CrashMonitorClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientDoc

IMPLEMENT_DYNCREATE(CCrashMonitorClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CCrashMonitorClientDoc, CDocument)
	//{{AFX_MSG_MAP(CCrashMonitorClientDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientDoc construction/destruction

CCrashMonitorClientDoc::CCrashMonitorClientDoc()
{
	// TODO: add one-time construction code here

}

CCrashMonitorClientDoc::~CCrashMonitorClientDoc()
{
}

BOOL CCrashMonitorClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientDoc serialization

void CCrashMonitorClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientDoc diagnostics

#ifdef _DEBUG
void CCrashMonitorClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCrashMonitorClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCrashMonitorClientDoc commands
