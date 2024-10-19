/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MailMessage.cpp

Author: �̹μ� <nshangukin@joyon.com>
Currently Managed by: �̹μ�

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#include "StdAfx.h"
#include "MailMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMailMessage::CMailMessage()
{
	m_sMailerName = _T( "WC Mail" );
	SetCharsPerLine( 76 );
}

CMailMessage::~CMailMessage()
{

}

BOOL CMailMessage::AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName, RECIPIENTS_TYPE type /* <JFO> */)
{
	ASSERT( szEmailAddress != NULL );
	ASSERT( szFriendlyName != NULL );
	CRecipient to;
	to.m_sEmailAddress = szEmailAddress;
	to.m_sFriendlyName = szFriendlyName;

   //*** Begin <JFO>
   //*** old line -> m_Recipients.Add( to );
   {
      switch(type)
      {
         case TO: m_Recipients.Add( to );    break;
         case CC: m_CCRecipients.Add( to );  break;
         case BCC:m_BCCRecipients.Add( to ); break;
      }
   }
   //*** End

	return TRUE;
}

BOOL CMailMessage::GetRecipient(CString & sEmailAddress, CString & sFriendlyName, int nIndex, RECIPIENTS_TYPE type /* <JFO> */)
{
	CRecipient to;
	if( nIndex < 0 || nIndex > m_Recipients.GetUpperBound() )
		return FALSE;

	//*** Begin <JFO>
   //*** old line -> to = m_Recipients[ nIndex ];
   {
      switch(type)
      {
         case TO:  to = m_Recipients[ nIndex ];    break;
         case CC:  to = m_CCRecipients[ nIndex ];  break;
         case BCC: to = m_BCCRecipients[ nIndex ]; break;
      }
   }
   //*** End

	sEmailAddress = to.m_sEmailAddress;
	sFriendlyName = to.m_sFriendlyName;

	return TRUE;
}

int CMailMessage::GetNumRecipients(RECIPIENTS_TYPE type /* <JFO> */)
{
   //*** Begin <JFO>
   //*** old line ->	return m_Recipients.GetSize();
      int number = 0;

      switch(type)
      {
         case TO:  number = m_Recipients.GetSize();    break;
         case CC:  number = m_CCRecipients.GetSize();  break;
         case BCC: number = m_BCCRecipients.GetSize(); break;
      }

      return number;

   //*** End
}

BOOL CMailMessage::AddMultipleRecipients(LPCTSTR szRecipients, RECIPIENTS_TYPE type /* <JFO> */)
{
	TCHAR* buf;
	UINT pos;
	UINT start;
	CString sTemp;
	CString sEmail;
	CString sFriendly;
	UINT length;
	int nMark;
	int nMark2;

	ASSERT( szRecipients != NULL );
	
	// Add Recipients
	//
	length = strlen( szRecipients );
	buf = new TCHAR[ length + 1 ];
	strcpy( buf, szRecipients );
	for( pos = 0, start = 0; pos <= length; pos++ )
	{
		if( buf[ pos ] == ';' ||
			buf[ pos ] == 0 )
		{			
			buf[ pos ] = 0;
			sTemp = &buf[ start ];

			nMark = sTemp.Find( '<' );
			if( nMark >= 0 )
			{
				sFriendly = sTemp.Left( nMark );
				nMark2 = sTemp.Find( '>' );
				if( nMark2 < nMark )
				{
					delete[] buf;
					return FALSE;
				}
				
				nMark2 > -1 ? nMark2 = nMark2 : nMark2 = sTemp.GetLength() - 1;
				sEmail = sTemp.Mid( nMark + 1, nMark2 - (nMark + 1) );
			}
			else
			{
				sEmail = sTemp;
				sFriendly = _T( "" );
			}
			AddRecipient( sEmail, sFriendly, type /* <JFO> */ );
			start = pos + 1;
		}
	}
	delete[] buf;
	return TRUE;
}

void CMailMessage::FormatMessage()
{
	start_header();
	prepare_header();
	end_header();
	prepare_body();
}

void CMailMessage::SetCharsPerLine(UINT nCharsPerLine)
{
	m_nCharsPerLine = nCharsPerLine;
}	

UINT CMailMessage::GetCharsPerLine()
{
	return m_nCharsPerLine;
}

void CMailMessage::prepare_header()
{
   
	CString sTemp;

	sTemp = _T( "" );
	
	sTemp = _T( "From: " ) + m_sFrom;
	add_header_line( (LPCTSTR)sTemp );

	sTemp = _T( "To: " );
	CString sEmail = _T( "" );
	CString sFriendly = _T( "" );
	for( int i = 0; i < GetNumRecipients(); i++ )
	{
		GetRecipient( sEmail, sFriendly, i );
		sTemp += ( i > 0 ? _T( "," ) : _T( "" ) );
		sTemp += sFriendly;
		sTemp += _T( "<" );
		sTemp += sEmail;
		sTemp += _T( ">" );
	}
	add_header_line( (LPCTSTR)sTemp );


   //*** Begin <JFO>
   {
	   sTemp = _T( "Cc: " );
	   CString sEmail = _T( "" );
	   CString sFriendly = _T( "" );
	   for( int i = 0; i < GetNumRecipients(CC); i++ )
	   {
		   GetRecipient( sEmail, sFriendly, i, CC );
		   sTemp += ( i > 0 ? _T( "," ) : _T( "" ) );
		   sTemp += sFriendly;
		   sTemp += _T( "<" );
		   sTemp += sEmail;
		   sTemp += _T( ">" );
	   }
	   add_header_line( (LPCTSTR)sTemp );
   }
   //*** End


	m_tDateTime = m_tDateTime.GetCurrentTime();
	
	sTemp = _T( "Date: " );
	sTemp += m_tDateTime.Format( "%a, %d %b %y %H:%M:%S %Z" );
	add_header_line( (LPCTSTR)sTemp );

	
	sTemp = _T( "Subject: " ) + m_sSubject;
	add_header_line( (LPCTSTR)sTemp );

	
	sTemp = _T( "X-Mailer: " ) + m_sMailerName;
	add_header_line( (LPCTSTR)sTemp );
}

void CMailMessage::prepare_body()
{	
	if( m_sBody.Right( 2 ) != _T( "\r\n" ) )
		m_sBody += _T( "\r\n" );

}


void CMailMessage::start_header()
{
	m_sHeader = _T( "" );
}

void CMailMessage::end_header()
{
	m_sHeader += _T( "\r\n" );
}

void CMailMessage::add_header_line(LPCTSTR szHeaderLine)
{
	CString sTemp;
	sTemp.Format( _T( "%s\r\n" ), szHeaderLine );
	m_sHeader += sTemp;
}

