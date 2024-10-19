
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: TextPlain.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __TEXTPLAIN_H_INCLUDED__
#define __TEXTPLAIN_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MIMEContentAgent.h"

// CTextPlain
class CTextPlain : public CMIMEContentAgent  
{
public:
	CTextPlain( int nContentType, UINT nWrapPos = 72 );
	virtual ~CTextPlain();

	virtual BOOL AppendPart( LPCTSTR szContent, LPCTSTR szParameters, int nEncoding, BOOL bPath, CString& sDestination );
	virtual CString GetContentTypeString();

protected:
	UINT m_nWrapPos;

	CString wrap_text( LPCTSTR szText );
	virtual CString build_sub_header( LPCTSTR szContent, LPCTSTR szParameters, int nEncoding, BOOL bPath );
};

#endif
