// GersangServerMonitor.cpp: implementation of the CGersangServerMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerMonitor.h"
#include "GersangServerMonitor.h"
#include "GersangServerStatus.h"
#include "ServerMonitorDlg.h"
//#include "MonitorProtocol.h"
#include "MyPacket.h"
#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGersangServerMonitor g_GersangServerMonitor;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


static DWORD WINAPI ConnectThread(LPVOID lpVoid)
{

	CGersangServerMonitor *pSocket = (CGersangServerMonitor *)lpVoid;
	
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

	CGersangServerMonitor *pSocket = (CGersangServerMonitor *)lpVoid;
	int ret;
	
	while (1) {

		if (WaitForSingleObject(pSocket->GetExitEventHandle(),100) == WAIT_OBJECT_0) {
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


CGersangServerMonitor::CGersangServerMonitor()
{
	m_siServerGroupNum = 0;
	m_siServerPerGroup = 0;
	m_wFileSendServerNum = 0;
	m_pServer = NULL;

	//sms관련.
	m_siPopPos		=	0;
	m_siPushPos		=	0;
	m_siCount		=	0;

	ZeroMemory(&m_stMonitorSms,sizeof(MonitorSmsReq) * MAX_SMS_BUFFER);

	m_ppszServerUsage = NULL;
	m_bIsConnected = FALSE;

	
}

CGersangServerMonitor::~CGersangServerMonitor()
{
	if (m_pServer != NULL)
	{
		delete [] m_pServer;
		m_pServer = NULL;

	}

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

int CGersangServerMonitor::Init()
{
	m_pServer = new CGersangServerStatus[MAX_SERVER];
	if (m_pServer == NULL) return 0;

	
	return 1;
}

int CGersangServerMonitor::Process()
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
				pDlg->PutConsoleString(RGB(255,0,0),_T("Connection failed.\n"));
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
				
				// 서버가 어떤상태인지 알수없다.		
				pDlg->PutConsoleString(RGB(0,0,255),_T("Connection succeeded.\n"));

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
				pDlg->ClearServerList();
				pDlg->PutConsoleString(RGB(255,0,0),_T("Connection lost.\n"));
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
							switch ( buf[4]) 
							{
							case 0:
								{
								
								
								CMyPacket Packet;
								Packet.Pack(buf,MMC_QUERY_WORLD_LIST,0);
								PutPacket(&Packet);
								Packet.Pack(buf,MMC_QUERY_SERVER_STRUCT,0);
								PutPacket(&Packet);												
								Packet.Pack(buf,MMC_QUERY_CONNECTUSERLIST,0);
								PutPacket(&Packet);
								
								}
								break;
							case 1:
							case 2:
								AfxMessageBox(_T("Invalid ID or wrong password"));
								SetEvent(GetExitEventHandle());
								break;
							case 3:
								AfxMessageBox(_T("Already Connected"));
								SetEvent(GetExitEventHandle());
								break;
							
							
							}
						}
						break;
					case MMC_REPORT_ADDUSER:
					    {
							if ( buf[4] == 0)
							{
								pDlg->PutConsoleString(RGB(0,0,255),_T("AddUser Success!!\n"));
								
							}
							else
							{
								pDlg->PutConsoleString(RGB(255,0,0),_T("AddUser Failed!!\n"));

							}
						}						
						break;
					case MMC_REPORT_DELUSER:
						{
							if ( buf[4] == 0)
							{
								pDlg->PutConsoleString(RGB(0,0,255),_T("DelUser Success!!\n"));
								
							}
							else
							{
								pDlg->PutConsoleString(RGB(255,0,0),_T("DelUser Failed!!\n"));

							}
							
						}						
						break;
					case MMC_REPORT_USERLIST:
						{
							char *username = &buf[8];
							int usernum = *(int *)&buf[4];
							int a = 0;
							pDlg->PutConsoleString(RGB(255,0,0),_T("Registered User : %d\n"),usernum);
							while (a < usernum)
							{
								pDlg->PutConsoleString(RGB(0,0,0),_T("ID : %s  Name : %s\n"),username,&username[32]);
								
								username += 64;
								a++;
							}

						}
						break;
					case MMC_REPORT_CONNECTUSERLIST:
						{
							char *username = &buf[8];
							int usernum = *(int *)&buf[4];
							int a = 0;
							pDlg->PutConsoleString(RGB(255,0,0),_T("Connected User : %d\n"),usernum);
							while (a < usernum)
							{
								pDlg->PutConsoleString(RGB(0,0,0),_T("ID : %s Name : %s\n"),username,&username[32]);
								
								username += 64;
								a++;
							}

						}
						break;
			
					case MMC_REPORT_CHAT:
						{
							char *szID = &buf[4];
							CTime time = CTime::GetCurrentTime();

							pDlg->PutConsoleString(RGB(0,0,255),_T("%s(%s %2.2d:%2.2d:%2.2d) "),szID,&szID[32 * sizeof(TCHAR)],time.GetHour(),time.GetMinute(),time.GetSecond());

							PlaySound(_T("chat.wav"),NULL,SND_ASYNC | SND_FILENAME);


								
							szID += 64 * sizeof(TCHAR);
							TCHAR chat[256];
							ZeroMemory(chat,sizeof(chat));

							int chatsize = *(WORD *)szID;
							memcpy(chat,(TCHAR *)&szID[2],chatsize);
							pDlg->PutConsoleString(RGB(0,0,0),_T("%s\n"),chat);
							
							

						}
						break;
					case MMC_REPORT_USER_ENTER:
						{
							char *szID = &buf[4];
							pDlg->PutConsoleString(RGB(0,0,255),_T("%s(%s) has connected \n"),szID,&szID[32]);
								
							
							

						}
						break;
					case MMC_REPORT_USER_EXIT:
						{
							char *szID = &buf[4];
							pDlg->PutConsoleString(RGB(0,0,255),_T("%s(%s) has disconnected\n"),szID,&szID[32]);
								
							
							

						}
						break;

					case MMC_REPORT_SMS:
						{
							MonitorSmsReq*	SmsReq = (MonitorSmsReq*)&buf[4];
							pDlg->PutConsoleString(RGB(0,0,255),_T("num = %s Message = %s\n"),SmsReq->szOrgPhone,SmsReq->szCallMessage);
//							PushSmsData(SmsReq->szCallMessage,SmsReq->szOrgPhone);
						}
						break;
					
					case MMC_REPORT_WORLD_LIST:
						{
							char *worldname = &buf[8];
							m_siServerGroupNum = *(int *)&buf[4];
							int a = 0;
							while (a < m_siServerGroupNum)
							{
								m_pServer[a].SetServerName(worldname);
								worldname += 32;
								a++;
							}
						}


						break;
					case MMC_REPORT_SERVER_STRUCT:
						{
							int servernum = *(int *)&buf[4];
							m_siServerPerGroup = servernum;
							if (m_ppszServerUsage) delete [] m_ppszServerUsage;

							
							m_ppszServerUsage = new PTCHAR[servernum];
							CGersangServerStatus::SetServerNum(servernum);
							

							char *worldname = &buf[8];

							for (int b = 0; b < m_siServerGroupNum; b++)
							{
								m_pServer[b].Init(b);
							}

							for (int a = 0; a < servernum; a++)
							{
							
								m_ppszServerUsage[a] = new TCHAR[256];
								ZeroMemory(m_ppszServerUsage[a],sizeof(TCHAR) * 256);
#ifdef UNICODE
								mbstowcs(m_ppszServerUsage[a],worldname,strlen(worldname));
#else
								strcpy(m_ppszServerUsage[a],worldname);
#endif
								worldname += 32;
							}

							
							

							pDlg->InitializeServerList();
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

							if (m_pServer == NULL)
							{
								break;
							}

							if (group == 5) {
								int p = 0;
								p = 3;
							}

							if (m_pServer[group].m_psiServerStates == NULL && wServerCommand != MONITOR_COMMAND_SERVER_ON_TIME)
							{
								break;
							}


							switch (wServerCommand)
							{
							
							case MONITOR_COMMAND_SERVER_DEAD:
								{
									m_pServer[group].m_psiServerStates[servernum] = SERVER_STATE_DEATH;
								}
								break;
							case MONITOR_COMMAND_SERVER_LIVE:
								{
									m_pServer[group].m_psiServerStates[servernum] = SERVER_STATE_LIVE;
								}
								break;		
							case MONITOR_COMMAND_FILETRANSFEREND:
								{
									
										Monitor_Command_FileTransferEnd *mcfe = (Monitor_Command_FileTransferEnd *)packetbuf;
										
										pDlg->PutConsoleString(RGB(0,0,0),_T("%s transferred to %s %s server successfully.  \n"),mcfe->szFileName,m_pServer[group].GetServerName(),m_ppszServerUsage[servernum]);
									
								/*m_bFileTransfer = FALSE;
								fclose(m_fp);
								m_fp = NULL;
								m_siSendFileByte = 0;
								m_siSendFileByteTotal = 0;*/
								}
								break;
							case MONITOR_COMMAND_SERVER_CURRENTNUM:
								{
								    WORD usernum = *(WORD *)packetbuf;

									m_pServer[group].m_siDifferance = (usernum - m_pServer[group].m_siUserNum);							
									if (m_pServer[group].m_siDifferance == 0)
									{
										m_pServer[group].m_siSameUserCount++;
									}
									else
									{
										m_pServer[group].m_siSameUserCount = 0;
									}

									m_pServer[group].m_siUserNum = usernum;

									if (m_pServer[group].m_siUserNum > m_pServer[group].m_siMaxUserNum)
									{
										m_pServer[group].m_siMaxUserNum = m_pServer[group].m_siUserNum;
									}


									m_pServer[group].m_dwHeartBeatTime = timeGetTime();
								}
								
								
								break;
							case MONITOR_COMMAND_SERVER_TIMEOUT:
								m_pServer[group].m_psiServerStates[servernum] = SERVER_STATE_TIMEOUT;
								break;
							case MONITOR_COMMAND_ANSWER_EXECFILENAME:
								{
									
									Monitor_Command_Answer_Execfilename *mcae = (Monitor_Command_Answer_Execfilename *)packetbuf;
									pDlg->PutConsoleString(RGB(0,0,0),_T("%s %s  : %s \n"),m_pServer[group].GetServerName(),m_ppszServerUsage[servernum],mcae->szFileName);
									
								}
								break;
							case MONITOR_COMMAND_SERVER_ON_TIME:
								{
									if (servernum == 0) {
									Monitor_Command_Server_On_Time *pTime;
									pTime = (Monitor_Command_Server_On_Time *)packetbuf;

									m_pServer[group].m_ServerOnTime = pTime->timeServerOn;
									}
									
									
								}
								break;
							case MONITOR_COMMAND_SERVER_QUEUE_STATUS:
								{
									Monitor_Command_Server_Queue_Status *pQueue;
									pQueue = (Monitor_Command_Server_Queue_Status *)packetbuf;

									m_pServer[group].m_siGateQueue = pQueue->GateQueueSize;
									m_pServer[group].m_siSubQueue = pQueue->SubQueueSize;


								}
								break;
								
								
							}

						}
						break;
						
					
				
				/*	case MONITOR_COMMAND_SERVER_DEAD:
						m_siServerState = SERVER_STATE_DEATH;	
						break;
					case MONITOR_COMMAND_SERVER_LIVE:
						m_siServerState = SERVER_STATE_LIVE;	
						break;		
					case MONITOR_COMMAND_FILETRANSFEREND:
						m_bFileTransfer = FALSE;
						fclose(m_fp);
						m_fp = NULL;
						m_siSendFileByte = 0;
						m_siSendFileByteTotal = 0;
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
						break;*/
					
					}
					
					delete pPacket;
				}
			
				
			}
			
		}
		
	}

	return 0;

}

const TCHAR *CGersangServerMonitor::GetServerName(int num)
{
	if (num >= 0 && num < m_siServerGroupNum)
	{
		return m_pServer[num].GetServerName();

	}
	else
	{
		return NULL;
	}
}
int CGersangServerMonitor::GetServerState(int num,int serverkind)
{
	if (num >= 0 && num < m_siServerGroupNum)
	{
		return m_pServer[num].GetState(serverkind);

	}
	else
	{
		return 0;
	}
}
int CGersangServerMonitor::GetServerInfo(int num,TCHAR *szBuf)
{
	if (num >= 0 && num < m_siServerGroupNum)
	{
		m_pServer[num].GetServerInfo(szBuf);
		return 1;

	}
	else
	{
		return 0;
	}
}
TCHAR *CGersangServerMonitor::GetServerUsage(int num)
{
	if (num >= 0 && num < m_siServerPerGroup)
	{
		return m_ppszServerUsage[num];
		
	}
	else
	{
		return NULL;
	}

}

/*int CGersangServerMonitor::ConnectAll()
{
	for (int a = 0; a < m_siServerGroupNum; a++)
	{
		for (int b = 0; b < m_siServerPerGroup; b++)
		{
			m_pServer[a].Connect(b);
			

		}
	}
	return 1;

}*/

int CGersangServerMonitor::ConnectToServer()
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();
    if (m_hThread == NULL) {
		pDlg->PutConsoleString(RGB(0,0,0),_T("Connecting...\n"));
		m_hThread = CreateThread(NULL,0,ConnectThread,this,0,&m_dwThreadID);
	}
	else
	{
		pDlg->PutConsoleString(RGB(255,0,0),_T("Already connected or trying to connect.\n"));

	}

	return 1;

}


int GetQuotedString(char *szSrc,char *szDest)
{
	char *szTemp = szSrc;
	int flag = 0;
	while (*szTemp)
	{
		if (flag == 0) {
			if (*szTemp == '"')
			{
				flag = 1;
			}
		}
		else if (flag == 1)
		{
			if (*szTemp == '"')
			{
				flag = 2;
				break;
			}
			else
			{
				*szDest = *szTemp;
				szDest++;
			}

		}
		

		szTemp++;
	}
	if (flag == 2)
	{
		*szDest = 0;
		return 1;
	}
	return 0;
}




int CGersangServerMonitor::SendPacket(int servernum, WORD *serverlist, CMyPacket *pPacket)
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

	//return m_pServer[group].SendPacket(kind,pPacket);
	
}

int CGersangServerMonitor::SendFile(int servernum, WORD *serverlist, CString &filename,CString &filenamenopath)
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
	Monitor_Command_FileTransferStart mcf;
	ZeroMemory(&mcf,sizeof(mcf));

	mcf.siFileSize = m_siSendFileByteTotal;


#ifdef _UNICODE
	
	ZeroMemory(mbsfilename,sizeof(mbsfilename));
	wcstombs(mbsfilename,m_szTransferFileNameNoPath,256);
	strcpy(mcf.szFileName,mbsfilename);
#else
	strcpy(mcf.szFileName,m_szTransferFileNameNoPath);
#endif


	strcpy(mcf.szDirectory,g_SavePath);



	
	
	
	Packet.Pack(&mcf,MONITOR_COMMAND_FILETRANSFERSTART,sizeof(Monitor_Command_FileTransferStart));
	

	SendPacket(servernum,serverlist,&Packet);

	return 1;

	//return m_pServer[group].SendFile(kind,filename,filenamenopath);
	
}

int CGersangServerMonitor::GetFileTransferStatus(char *szBuf)
{
	if (!m_bFileTransfer) return 0;
	if (m_fp == NULL) return 0;
	sprintf(szBuf,"%s , (%ld / %ld)",m_szTransferFileNameNoPath,m_siSendFileByte,m_siSendFileByteTotal);
	return 1;
	
}
int CGersangServerMonitor::SendChat(TCHAR *szChat)
{
	char chat[512];
	
	
	ZeroMemory(chat,sizeof(chat));
	
	
	WORD chatsize = _tcslen(szChat) * sizeof(TCHAR) + 1;

	CMyPacket Packet;

	*(WORD *)chat = chatsize;

	memcpy(&chat[2],szChat,chatsize);
		
	Packet.Pack(chat,MMC_QUERY_CHAT,chatsize + 2);
	
	PutPacket(&Packet);

	return 1;
	

}


int CGersangServerMonitor::SendNotice(char *szNotice)
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();
	WORD serverlist[256];
	char notice[256];
	
	if (strlen(szNotice) > 0) {

		ZeroMemory(notice,256);
		strcpy(notice,szNotice);
		
		ZeroMemory(serverlist,256 * 2);
		
		CMyPacket Packet;
		Packet.Pack(notice,MONITOR_COMMAND_NOTICE,256);
		
		pDlg->PutConsoleString(RGB(0,0,0),_T(" \" %s \" noticed \n"),notice);
		
		WORD servercount = pDlg->GetSelectedServer(serverlist);
		g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	}
	
	
	return 1;
	
}

int CGersangServerMonitor::SendNotice(int servercount,WORD *serverlist,char *szNotice)
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();

	char notice[256];
	
	if (strlen(szNotice) > 0) {

		ZeroMemory(notice,256);
		strcpy(notice,szNotice);
				
		CMyPacket Packet;
		Packet.Pack(notice,MONITOR_COMMAND_NOTICE,256);
		
		pDlg->PutConsoleString(RGB(0,0,0),_T(" \" %s \" noticed \n"),notice);
				
		g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	}
	
	return 1;
	
}

int CGersangServerMonitor::SendLogout()
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();
	WORD serverlist[256];
	char notice[256];
	
	
	ZeroMemory(serverlist,256 * 2);
	
	CMyPacket Packet;
	Packet.Pack(notice,MONITOR_QUERY_LOGOUT,0);
	
	WORD servercount = pDlg->GetSelectedServer(serverlist);
	g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	
	pDlg->PutConsoleString(RGB(0,0,0),_T("Logout Sended\n"));
	
	return 1;
	
}

int CGersangServerMonitor::SendLogout(int servercount,WORD *serverlist)
{
	CServerMonitorDlg *pDlg = (CServerMonitorDlg *)AfxGetMainWnd();

	char notice[256];	
	
	
	CMyPacket Packet;
	Packet.Pack(notice,MONITOR_QUERY_LOGOUT,0);
	
	g_GersangServerMonitor.SendPacket(servercount,serverlist,&Packet);
	
	pDlg->PutConsoleString(RGB(0,0,0),_T("Logout Sended\n"));
	
	return 1;
}
/*
void CGersangServerMonitor::PushSmsData(char* szCallMsg,char* szOrgPhone)
{
	MonitorSmsReq* pstMonitorSms;
	
	pstMonitorSms = &m_stMonitorSms[m_siPushPos];

	sprintf(pstMonitorSms->szCallMessage,"%s",szCallMsg);
	sprintf(pstMonitorSms->szOrgPhone,"%s",szOrgPhone);

	m_siPushPos++;
	m_siCount++;

}

MonitorSmsReq* CGersangServerMonitor::PopSmsData()
{
	if(m_siCount <= 0)		return FALSE;

	MonitorSmsReq* pstMonitorSms;
	
	pstMonitorSms = &m_stMonitorSms[m_siPopPos];

	m_siPopPos++;
	m_siCount--;

	return pstMonitorSms;
}*/

