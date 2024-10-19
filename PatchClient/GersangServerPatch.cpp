// GersangServerMonitor.cpp: implementation of the CGersangServerPatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatchTool.h"
#include "GersangServerPatch.h"
#include "PatchToolDlg.h"
#include <mmsystem.h>

#include "..\ServerMonitor\monitorprotocol.h"
#include "..\ServerMonitor\MyPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGersangServerPatch g_GersangServerPatch;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


static DWORD WINAPI ConnectThread(LPVOID lpVoid)
{

	CGersangServerPatch *pSocket = (CGersangServerPatch *)lpVoid;
	
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
	CGersangServerPatch *pSocket = (CGersangServerPatch *)lpVoid;
	int ret;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),10) == WAIT_OBJECT_0) {
			pSocket->Close();
			ExitThread(0);
		}

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


CGersangServerPatch::CGersangServerPatch()
{
	m_siServerGroupNum = 0;
	m_siServerPerGroup = 0;
	m_wFileSendServerNum = 0;	

	m_ppszServerUsage = NULL;
	m_bIsConnected = FALSE;	
}

CGersangServerPatch::~CGersangServerPatch()
{
	if (m_ppszServerUsage != NULL)
	{
		for (int a = 0; a < m_siServerPerGroup; a++)
		{
			delete [] m_ppszServerUsage[a];
		}
		delete [] m_ppszServerUsage;
		m_ppszServerUsage = NULL;
	}
}

int CGersangServerPatch::Init()
{		
	return 1;
}

int CGersangServerPatch::Process()
{
	CPatchToolDlg *pDlg = (CPatchToolDlg *)AfxGetMainWnd();
	DWORD dwExitCode;

	// 끊겨있을때
	if (IsConnected() == FALSE)
	{		
		if (m_hThread) {
			GetExitCodeThread(m_hThread,&dwExitCode);
			// 접속이 실패했을시
			if (dwExitCode == 0)
			{
				pDlg->MsgPrint(_T("Connection failed.\n"));
				m_hThread = NULL;
				return PROCESS_CONNECTION_FAILED;
			}
			else if (dwExitCode == 1)
			{
				char szBuf[64];
				ZeroMemory(szBuf,64);
				m_hThread = CreateThread(NULL,0,SendNRecvThread,this,0,&m_dwThreadID);
				m_bIsConnected = TRUE;
				_tcscpy((TCHAR *)szBuf,m_szID);
				_tcscpy((TCHAR *)&szBuf[32],m_szPassword);
				CMyPacket Packet;
				Packet.Pack(szBuf,MMC_QUERY_LOGIN,64);
				PutPacket(&Packet);

	//			pDlg->MsgPrint(_T("Connection succeed.\n"));

				return PROCESS_CONNECTION_SUCCEEDED;
			}
		}
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
				ResetEvent(GetExitEventHandle());
				m_hThread = NULL;
				
				m_bIsConnected = FALSE;
//				pDlg->ClearServerList();
//				pDlg->PutConsoleString(RGB(255,0,0),_T("Connection lost.\n"));
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

						ByteToSend = min(GetSendBufferSpace() - 16,8192);

						ByteToSend = min(ByteToSend,RestByte);

						if (ByteToSend > 0) {
							
							CMyPacket Packet;
							
							//fread(szBuf,ByteToSend,1,m_fp);
							
							Packet.Pack(m_fp,MONITOR_COMMAND_FILETRANSFER,ByteToSend);			

							if (SendPacket(m_wFileSendServerNum,m_pwFileSendServerList,&Packet))
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
						case MMC_REPORT_LOGIN:
						{
							switch (buf[4]) 
							{
							case 0:
								{
									CMyPacket Packet;
									Packet.Pack(buf,MMC_QUERY_WORLD_LIST,0);
									PutPacket(&Packet);
									Packet.Pack(buf,MMC_QUERY_SERVER_STRUCT,0);
									PutPacket(&Packet);												
						//			Packet.Pack(buf,MMC_QUERY_CONNECTUSERLIST,0);
						//			PutPacket(&Packet);								

									pDlg->MsgPrint(_T("Login succeed!"));									
								}
								break;
							case 1: // 아이디 없음.
							case 2:	// 패스워드 잘못 되었음.
								pDlg->MsgPrint(_T("Invalid ID or wrong password"));								
								SetEvent(GetExitEventHandle());
								break;
							case 3:	// 이미 접속한 아이디.
								pDlg->MsgPrint(_T("Already Connected"));								
								SetEvent(GetExitEventHandle());
								break;							
							}
						}
					break;
					case MMC_REPORT_WORLD_LIST:
						{
							char *worldname = &buf[8];
							m_siServerGroupNum = *(int *)&buf[4];
							int a = 0;
							while (a < m_siServerGroupNum)
							{
								strncpy(m_szServerName[a], worldname, 32);
								worldname += 32;
								a++;
							}
						}
						break;

					case MMC_REPORT_SERVER_STRUCT:
						{
							int servernum = *(int *)&buf[4];
							m_siServerPerGroup = servernum;

							pDlg->AddServer();
						}
						break;

					case MMC_SERVER_TO_CLIENT:
						{							
							char *packetbuf = &buf[4];

							int servercode = *(WORD *)packetbuf;
							packetbuf += 2;
							int realpacketsize = *(WORD *)packetbuf;
							packetbuf += 2;
							WORD wServerCommand = *(WORD *)packetbuf;
							packetbuf += 2;
							int group = servercode / 10;
							int servernum = servercode % 10;

							switch (wServerCommand)
							{
							case MONITOR_COMMAND_FILETRANSFEREND:
								{
									Monitor_Command_FileTransferEnd *mcfe = 
										(Monitor_Command_FileTransferEnd *)packetbuf;

									pDlg->FileTransferState(group, mcfe->szFileName);
								}
								break;
							}
						}
						break;
					case MMC_REPORT_USER_ENTER:
						{
							char *szID = &buf[4];
							pDlg->MsgPrint(_T("%s(%s) has connected \n"),szID,&szID[32]);				
						}
						break;
					}
					
					delete pPacket;
				}				
			}		
		}
	}

	return 0;

}

int CGersangServerPatch::ConnectToServer()
{
	CPatchToolDlg *pDlg = (CPatchToolDlg *)AfxGetMainWnd();
    if (m_hThread == NULL) {
		pDlg->MsgPrint(_T("Connecting...\n"));
		m_hThread = CreateThread(NULL,0,ConnectThread,this,0,&m_dwThreadID);
	}
	else
	{
		pDlg->MsgPrint(_T("Already connected or trying to connect.\n"));
	}

	return 1;

}


int CGersangServerPatch::SendPacket(int servernum, WORD *serverlist, CMyPacket *pPacket)
{
	if (servernum <= 0) return 0;

	CMyPacket Packet;

	int size = servernum * 2 + 2 + 2 + pPacket->GetLength();

	char *temp = (char *)malloc(size);

	WORD *buf = (WORD *)temp;

	// 서버갯수기록
	*buf = servernum;
	buf++;

	// 서버리스트 기록
	memcpy(buf,serverlist,servernum * 2);
	buf += servernum;

	// 패킷 크기기록
	*buf = pPacket->GetLength();
	buf++;

	// 패킷 복사
	pPacket->Expand((char *)buf);

	// 다시 팩 
    Packet.Pack(temp,MMC_CLIENT_TO_SERVER,size);

	free(temp);

	return PutPacket(&Packet);
}

int CGersangServerPatch::SendFile(int servernum, 
								  WORD *serverlist, 
								  BYTE newversion,
								  CString &destpath,
								  CString &filename,
								  CString &filenamenopath)
{
	if (!IsConnected()) return 0;
    
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
	
#ifdef _UNICODE
	char mbsfilename[256];
	ZeroMemory(mbsfilename,sizeof(mbsfilename));
	wcstombs(mbsfilename,filename,256);
	m_fp = fopen(mbsfilename,"rb");
	if (m_fp == NULL) return 0;

#else
	m_fp = fopen(filename,"rb");
	if (m_fp == NULL) return 0;
#endif

	m_bFileTransfer = TRUE;
	m_szTransferFileName = filename;
	m_szTransferFileNameNoPath = filenamenopath;
	m_siSendFileByte = 0;
	m_wFileSendServerNum = servernum;
	memcpy(m_pwFileSendServerList,serverlist,servernum * 2);

	long siStart = ftell(m_fp);

	fseek(m_fp,0,SEEK_END);
	
	m_siSendFileByteTotal = ftell(m_fp) - siStart;

	fseek(m_fp,0,SEEK_SET);

	CMyPacket Packet;
	Patch_Command_FileTransferStart mcf;
	ZeroMemory(&mcf,sizeof(mcf));

	mcf.siFileSize = m_siSendFileByteTotal;


#ifdef _UNICODE
	
	ZeroMemory(mbsfilename,sizeof(mbsfilename));
	wcstombs(mbsfilename,m_szTransferFileNameNoPath,256);
	strcpy(mcf.szFileName,mbsfilename);
#else
	strcpy(mcf.szFileName,m_szTransferFileNameNoPath);
#endif

	mcf.byNewVersion = newversion;
	strcpy(mcf.szDirectory,destpath);

	Packet.Pack
		(
			&mcf,
			PATCH_COMMAND_FILETRANSFERSTART,
			sizeof(Patch_Command_FileTransferStart)
		);

	SendPacket(servernum,serverlist,&Packet);

	return 1;
}

int CGersangServerPatch::GetFileTransferStatus(char *szBuf)
{
	if (!m_bFileTransfer) return 0;
	if (m_fp == NULL) return 0;
	sprintf(szBuf,"%s , (%ld / %ld)",m_szTransferFileNameNoPath,m_siSendFileByte,m_siSendFileByteTotal);
	return 1;	
}


