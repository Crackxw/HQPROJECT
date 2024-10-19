/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AppocterStream.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __APPOCTETSTREAM_H_INCLUDED__
#define __APPOCTETSTREAM_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MIMEContentAgent.h"


class CAppOctetStream : public CMIMEContentAgent  
{
public:
	virtual CString GetContentTypeString();
	CAppOctetStream( int nContentType );
	virtual ~CAppOctetStream();

	virtual BOOL AppendPart( LPCTSTR szContent, 
							 LPCTSTR szParameters, 
							 int nEncoding, 
							 BOOL bPath, 
							 CString& sDestination );

protected:
	virtual void attach_file( CStdioFile* pFileAtt, int nEncoding, CString& sDestination );
	virtual CString build_sub_header( LPCTSTR szContent, 
									  LPCTSTR szParameters, 
									  int nEncoding, 
									  BOOL bPath );
};

#endif
