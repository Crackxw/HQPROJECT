/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MailMessage.h

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef __MAILMESSAGE_H_INCLUDED__
#define __MAILMESSAGE_H_INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

// CMailMessage
// Formats a message compliant with RFC 822.
//
class CMailMessage  
{
public:
	CMailMessage();
	virtual ~CMailMessage();

	enum RECIPIENTS_TYPE { TO, CC, BCC }; // <JFO>

	void FormatMessage();
	int GetNumRecipients(RECIPIENTS_TYPE type = TO /* <JFO> */);
	BOOL GetRecipient( CString& sEmailAddress, CString& sFriendlyName, int nIndex = 0, RECIPIENTS_TYPE type = TO /* <JFO> */ );
	BOOL AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName = "", RECIPIENTS_TYPE type = TO /* <JFO> */ );
	BOOL AddMultipleRecipients( LPCTSTR szRecipients = NULL, RECIPIENTS_TYPE type = TO /* <JFO> */ );
	UINT GetCharsPerLine();
	void SetCharsPerLine( UINT nCharsPerLine );	

	CString m_sFrom;
	CString m_sSubject;
	CString m_sEnvelope;
	CString m_sMailerName;
	CString m_sHeader;
	CTime m_tDateTime;	
	CString m_sBody;

private:
	UINT m_nCharsPerLine;
	class CRecipient
	{
		public:
			CString m_sEmailAddress;
			CString m_sFriendlyName;
	};
	CArray <CRecipient, CRecipient&> m_Recipients;
	CArray <CRecipient, CRecipient&> m_CCRecipients;    //*** <JFO>
	CArray <CRecipient, CRecipient&> m_BCCRecipients;   //*** <JFO>

protected:	
	virtual void prepare_header();
	virtual void prepare_body();
	virtual void end_header();
	virtual void start_header();
	
	virtual void add_header_line( LPCTSTR szHeaderLine );
};

#endif
