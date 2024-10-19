/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: AppocterStream.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/



#include "StdAfx.h"
#include "AppOctetStream.h"
#include "Base64.h"
#include "MIMEMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define BYTES_TO_READ 54 // This number guarantess output won't
						 // won't exceed line-length limit

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppOctetStream::CAppOctetStream( int nContentType )
	:CMIMEContentAgent( nContentType )
{
}

CAppOctetStream::~CAppOctetStream()
{
}

BOOL CAppOctetStream::AppendPart(LPCTSTR szContent, 
								 LPCTSTR szParameters, 
								 int nEncoding, 
								 BOOL bPath, 
								 CString & sDestination)
{
	CStdioFile fAttachment;

	ASSERT( szContent != NULL );
	
	if( szContent == NULL )
		return FALSE;
	if( !fAttachment.Open( szContent, (CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) ) )
		return FALSE;
	sDestination += build_sub_header( szContent,
								      szParameters,
									  nEncoding,
									  TRUE );
	attach_file( &fAttachment, CMIMEMessage::BASE64, sDestination );
	fAttachment.Close();
	return TRUE;
}


CString CAppOctetStream::build_sub_header(LPCTSTR szContent, 
										  LPCTSTR szParameters, 
										  int nEncoding, 
										  BOOL bPath)
{
	CString sSubHeader;
	CString sTemp;
	TCHAR szFName[ _MAX_FNAME ];
	TCHAR szExt[ _MAX_EXT ];

	_tsplitpath( szContent, NULL, NULL, szFName, szExt );

	if( bPath )
		sTemp.Format( "; file=%s%s", szFName, szExt );
	else
		sTemp = _T( "" );
	sSubHeader.Format( _T( "Content-Type: %s%s\r\n" ),
						(LPCTSTR)GetContentTypeString(),
						(LPCTSTR)sTemp );
	sSubHeader += _T( "Content-Transfer-Encoding: base64\r\n" );
	sTemp.Format( _T( "Content-Disposition: attachment; filename=%s%s\r\n" ),
				  szFName, szExt );
	sSubHeader += sTemp;	
	sSubHeader += _T( "\r\n" );
	return sSubHeader;
}

CString CAppOctetStream::GetContentTypeString()
{
	CString s;
	s = _T( "application/octet-stream" );
	return s;
}


void CAppOctetStream::attach_file(CStdioFile* pFileAtt, 
								  int nEncoding, 
								  CString & sDestination)
{
	CMIMECode* pEncoder;
	int nBytesRead;
	TCHAR szBuffer[ BYTES_TO_READ + 1 ];

	ASSERT( pFileAtt != NULL );
	if( pFileAtt == NULL )
		return;
	switch( nEncoding )
	{	
		default:			
		case CMIMEMessage::BASE64:
			try 
			{
				pEncoder = new CBase64;
			}
			catch( CMemoryException* e )
			{
				delete e;
				return;
			}
	}
	if( pEncoder == NULL )
		return;
	do
	{
		try
		{
			nBytesRead = pFileAtt->Read( szBuffer, BYTES_TO_READ );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		szBuffer[ nBytesRead ] = 0;
		sDestination += pEncoder->Encode( szBuffer, nBytesRead );
		sDestination += _T( "\r\n" );
	} while( nBytesRead == BYTES_TO_READ );
	sDestination += _T( "\r\n" );
	delete pEncoder;
}
