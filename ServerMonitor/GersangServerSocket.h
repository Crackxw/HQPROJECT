// GersangServerSocket.h: interface for the CGersangServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GERSANGSERVERSOCKET_H__75FDA39F_251C_49B5_B867_C6602D9D278F__INCLUDED_)
#define AFX_GERSANGSERVERSOCKET_H__75FDA39F_251C_49B5_B867_C6602D9D278F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySocket.h"

enum process_result
{
	PROCESS_NOPROBLEM = 0,
	PROCESS_CONNECTION_FAILED,
	PROCESS_CONNECTION_SUCCEEDED,
	PROCESS_CONNECTION_LOST
	

};

class CGersangServerSocket : public CMySocket  
{
public:
	int Process();
	CGersangServerSocket();
	virtual ~CGersangServerSocket();

	
	int GetFileTransferStatus(char *);
	int GetServerState() {return m_siServerState;};
	void ConnectToServer();
	BOOL IsConnected() {return m_bIsConnected;};
	int SendFile(CString &string,CString &filenamenopath);

	BOOL CanGetUserNum() {return m_bGetUserNum;}
	int GetMaxUserNum() {return m_siMaxUserNum;}
	int GetUserNum() {return m_siUserNum;}

private:

	int m_siServerState;
	int m_siUserNum;
	int m_siMaxUserNum;
	
	BOOL m_bGetUserNum;
	BOOL m_bIsConnected;

	// 파일전송
	BOOL m_bFileTransfer;
	CString m_szTransferFileName;
	CString m_szTransferFileNameNoPath;
	FILE *m_fp;
	int m_siSendFileByte;
	int m_siSendFileByteTotal;

	

};

#endif // !defined(AFX_GERSANGSERVERSOCKET_H__75FDA39F_251C_49B5_B867_C6602D9D278F__INCLUDED_)
