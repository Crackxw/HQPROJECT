/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MIMEContentAgent.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __MIMECONTENTAGENT_H_INCLUDED__
#define __MIMECONTENTAGENT_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CMIMEContentAgent
// Abstract base class. Content agents support MIME
//
class CMIMEContentAgent  
{
public:
	CMIMEContentAgent( int nMIMEType );
	virtual ~CMIMEContentAgent();

	BOOL QueryType( int nContentType );

	virtual BOOL AppendPart( LPCTSTR szContent,
							 LPCTSTR szParameters, 
							 int nEncoding, 
							 BOOL bPath, 
							 CString& sDestination ) = 0;
	virtual CString GetContentTypeString() = 0;

protected:
	virtual CString build_sub_header( LPCTSTR szContent,
									  LPCTSTR szParameters,
									  int nEncoding,
									  BOOL bPath ) = 0;


private:
	int m_nMIMETypeIHandle;
};

#endif
