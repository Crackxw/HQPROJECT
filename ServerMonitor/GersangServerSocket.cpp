// GersangServerSocket.cpp: implementation of the CGersangServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "GersangServerStatus.h"
#include "GersangServerSocket.h"
#include "ServerMonitorDlg.h"
#include "MyPacket.h"
#include "MonitorProtocol.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static DWORD WINAPI ConnectThread(LPVOID lpVoid)
{

	CGersangServerSocket *pSocket = (CGersangServerSocket *)lpVoid;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),0) == WAIT_OBJECT_0) ExitThread(0);

		switch (pSocket->Connect())
		{
		case -1:
			ExitThread(0);
			break;
		case 0:
			ExitThread(1);
			break;
		}
	}
	
}

static DWORD WINAPI SendNRecvThread(LPVOID lpVoid)
{

	CGersangServerSocket *pSocket = (CGersangServerSocket *)lpVoid;
	int ret;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),100) == WAIT_OBJECT_0) ExitThread(0);

		ret = pSocket->Select();

		if (ret & 2)
		{
			if (!pSocket->Send())
			{
				pSocket->Close();
				ExitThread(0);

			}

		}
		if (ret & 1)
		{
			if (!pSocket->Recv())
			{
				pSocket->Close();
				ExitThread(0);
			}
		}




	
		
	}
	
}



CGersangServerSocket::CGersangServerSocket()
{
	m_siServerState = SERVER_STATE_NOTCONNECTED;

	m_bIsConnected = FALSE;

	m_bGetUserNum = FALSE;


	m_fp = NULL;
	m_bFileTransfer = FALSE;
	m_siSendFileByte = 0;
	m_szTransferFileName.Empty();

	m_siUserNum = 0;
	m_siMaxUserNum = 0;


}

CGersangServerSocket::~CGersangServerSocket()
{
	if (m_fp) {
		fclose(m_fp);
		m_fp = NULL;
	}

}

void CGersangServerSocket::ConnectToServer()
{
	if (m_hThread == NULL) {
		m_hThread = CreateThread(NULL,0,ConnectThread,this,0,&m_dwThreadID);
	}
}


int CGersangServerSocket::Process()
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();
	DWORD dwExitCode;

	// 끊겨있을때
	if (IsConnected() == FALSE)
	{
		
		if (m_hThread) {
			GetExitCodeThread(m_hThread,&dwExitCode);
			// 접속이 실패했을시
			if (dwExitCode == 0)
			{
				m_hThread = NULL;
				m_siServerState = SERVER_STATE_NOTCONNECTED;

				
				return PROCESS_CONNECTION_FAILED;
			}
			else if (dwExitCode == 1)
			{
				m_hThread = CreateThread(NULL,0,SendNRecvThread,this,0,&m_dwThreadID);
				m_bIsConnected = TRUE;

				// 서버가 어떤상태인지 알수없다.
				m_siServerState = SERVER_STATE_UNKNOWN;	
				return PROCESS_CONNECTION_SUCCEEDED;
			}
			else if (dwExitCode == STILL_ACTIVE) {
				m_siServerState = SERVER_STATE_CONNECTING;	

			}
			
		}
		m_bGetUserNum = FALSE;
	}
	// 접속이 된 상태였을때
	else
	{
		if (m_hThread) {
			GetExitCodeThread(m_hThread,&dwExitCode);
			// 접속 성공일때에는~ 데이터를 주고받는 스레드를 돌린다.
			
			
			// ExitEvent를 셋하였거나 끊겼을때
			if (dwExitCode == 0)
			{
				Close();				
				m_hThread = NULL;
				m_siServerState = SERVER_STATE_NOTCONNECTED;	
				m_bIsConnected = FALSE;
				return PROCESS_CONNECTION_LOST;
			}
			// 주고받는 스레드가 돌아가고있을때에는 받은메시지를 처리한다.
			else if (dwExitCode == STILL_ACTIVE)
			{
				CMyPacket *pPacket;
				

				if (m_bFileTransfer)
				{
					int RestByte = m_siSendFileByteTotal - m_siSendFileByte;
					if (RestByte > 0)
					{
						int ByteToSend;

						ByteToSend = min(GetSendBufferSpace() - 16,4096);

						ByteToSend = min(ByteToSend,RestByte);

						if (ByteToSend > 0) {
							
							CMyPacket Packet;
							
							//fread(szBuf,ByteToSend,1,m_fp);
							
							Packet.Pack(m_fp,MONITOR_COMMAND_FILETRANSFER,ByteToSend);
							
							if (PutPacket(&Packet))
							{
								
								m_siSendFileByte += ByteToSend;
							}
							else
							{
								fseek(m_fp,-ByteToSend,SEEK_CUR);
							}

							
							
						}

						// 다보냈으면 땡
						if (m_siSendFileByte >= m_siSendFileByteTotal)
						{
							m_bFileTransfer = FALSE;
							fclose(m_fp);
							m_fp = NULL;	
							m_siSendFileByte = 0;
							m_siSendFileByteTotal = 0;
						
							
						}

						
					}

				}

				
				while ((pPacket = PopPacket()) != NULL)
				{

					char *buf = pPacket->GetBuffer();

					WORD wCommand = *(WORD *)&buf[2];

					switch (wCommand) {
					case MONITOR_COMMAND_SERVER_DEAD:
						m_siServerState = SERVER_STATE_DEATH;	
						break;
					case MONITOR_COMMAND_SERVER_LIVE:
						m_siServerState = SERVER_STATE_LIVE;	
						break;		
					// 전송이 끝났음을 표시만함
					case MONITOR_COMMAND_FILETRANSFEREND:
				/*		m_bFileTransfer = FALSE;
						fclose(m_fp);
						m_fp = NULL;
						m_siSendFileByte = 0;
						m_siSendFileByteTotal = 0;*/
						break;
					case MONITOR_COMMAND_SERVER_CURRENTNUM:
						m_bGetUserNum = TRUE;
						m_siUserNum = *(WORD *)&buf[4];
						if (m_siUserNum > m_siMaxUserNum)
						{
							m_siMaxUserNum = m_siUserNum;
						}
						
						break;
					case MONITOR_COMMAND_SERVER_TIMEOUT:
						m_siServerState = SERVER_STATE_TIMEOUT;
						break;
				
						
						break;
					
					}
					
					delete pPacket;
				}
			
				
			}
			
		}
		
	}

	return 0;

}

int CGersangServerSocket::SendFile(CString &string,CString &filenamenopath)
{
	if (!IsConnected()) return 0;
    
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	m_fp = fopen(string,"rb");
	if (m_fp == NULL) return 0;

	m_bFileTransfer = TRUE;
	m_szTransferFileName = string;
	m_szTransferFileNameNoPath = filenamenopath;
	m_siSendFileByte = 0;

	long siStart = ftell(m_fp);

	fseek(m_fp,0,SEEK_END);
	
	m_siSendFileByteTotal = ftell(m_fp) - siStart;

	fseek(m_fp,0,SEEK_SET);

	CMyPacket Packet;
	Monitor_Command_FileTransferStart mcf;
	ZeroMemory(&mcf,sizeof(mcf));

	mcf.siFileSize = m_siSendFileByteTotal;
	strcpy(mcf.szFileName,m_szTransferFileNameNoPath);
	strcpy(mcf.szDirectory,g_SavePath);

	Packet.Pack(&mcf,MONITOR_COMMAND_FILETRANSFERSTART,sizeof(Monitor_Command_FileTransferStart));
	PutPacket(&Packet);

	


	return 1;


}
int CGersangServerSocket::GetFileTransferStatus(char *szBuf)
{
	if (!m_bFileTransfer) return 0;
	if (m_fp == NULL) return 0;
	sprintf(szBuf,"%s , (%ld / %ld)",m_szTransferFileNameNoPath,m_siSendFileByte,m_siSendFileByteTotal);
	return 1;
	
}
