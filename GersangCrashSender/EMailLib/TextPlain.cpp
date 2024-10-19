/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: TextPlain.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#include "StdAfx.h"
#include "TextPlain.h"
#include "MIMEMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextPlain::CTextPlain( int nContentType, UINT nWrapPos )
	:CMIMEContentAgent( nContentType )
{
	m_nWrapPos = nWrapPos;	
}

CTextPlain::~CTextPlain()
{

}

CString CTextPlain::GetContentTypeString()
{
	CString s;
	s = _T( "text/plain" );
	return s;
}

BOOL CTextPlain::AppendPart(LPCTSTR szContent, 
							LPCTSTR szParameters, 
							int nEncoding, 
							BOOL bPath, 
							CString & sDestination)
{
	CString sSubHeader;
	CString sWrapped;
	sSubHeader = build_sub_header( szContent,
								   szParameters,
								   nEncoding,
								   bPath );
	sWrapped = wrap_text( szContent );
	sDestination += (sSubHeader + sWrapped);
	return TRUE;
}

CString CTextPlain::build_sub_header(LPCTSTR szContent, 
									 LPCTSTR szParameters, 
									 int nEncoding, 
									 BOOL bPath)
{
	CString sSubHeader;
	sSubHeader.Format( _T( "Content-Type: %s%s\r\n" ),
					   (LPCTSTR)GetContentTypeString(),
					   szParameters );
	sSubHeader += _T( "Content-Transfer-Encoding: " );
	switch( nEncoding )
	{		
		default:	
		case CMIMEMessage::_7BIT:
			sSubHeader += _T( "7Bit" );
	}
	sSubHeader += _T( "\r\n\r\n" );
	return sSubHeader;
}

CString CTextPlain::wrap_text(LPCTSTR szText)
{
	CString sTemp;
	CString sLeft;
	CString sRight;
	int lp = 0;
	UINT nCount = 0;
	int nSpacePos = 0;

	ASSERT( szText != NULL );
	if( szText == NULL )
		return sTemp;
	sTemp = szText;
	while( lp < sTemp.GetLength() )
	{
		if( sTemp[ lp ] == ' ' )
			nSpacePos = lp;		
		if( sTemp.Mid( lp, 2 ) == _T( "\r\n" ) )
			nCount = 0;		
		if( nCount > m_nWrapPos )
		{
			sLeft = sTemp.Left( nSpacePos );
			sRight = sTemp.Right( sTemp.GetLength() - nSpacePos );
			sLeft.TrimRight();
			sRight.TrimLeft();
			sLeft += _T( "\r\n" );
			sTemp = sLeft + sRight;
			nCount = 0;
		}
		else
			nCount++;
		lp++;
	}
	return sTemp;
}
