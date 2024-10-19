#include "XUser.h"
#include "XClient.h"
#include <conio.h>

#include "..\\XCommon\\JXPacket.h"
#include "..\\XCommon\\JXSocket.h"

#include "MonitorServer.h"

#include "..\\Servermonitor\\MonitorProtocol.h"
#include <direct.h>

extern char g_OriginalPath[256];

static CreateTempFileName(char *filename,char *buffer)
{
	static char *prefix = "Temp";
	sprintf(buffer,"%s%s",prefix,filename);
	
}

static CreateDirectoryFromFilename(char *filename2)
{
	static char cd[256];
	static char backcd[256];
	static char buffer[256];
	char filenamebuf[256];
	strcpy(filenamebuf,filename2);
	char *filename = filenamebuf;
	BOOL refresholdptr = TRUE;
	char *oldptr;
	
	GetCurrentDirectory(256,cd);
	strcpy(backcd,cd);

	
	while (*filename != NULL)
	{
		if (refresholdptr)
		{
			oldptr = filename;

		}
		refresholdptr = FALSE;
		if (*filename == '/')
		{
			*filename = 0;
			sprintf(buffer,"%s\\%s",cd,oldptr);
		
		
			
			if (!_mkdir(buffer)) {
				printf("%s ���丮 ����\n",buffer);
				
			}
			strcpy(cd,buffer);
			SetCurrentDirectory(cd);
			refresholdptr = TRUE;


		}
		filename++;

	}

	


	SetCurrentDirectory(backcd);

}



XUser::XUser()
{
	m_bConnect = FALSE;
	m_fp = NULL;
}

XUser::~XUser()
{
	Disconnect();
}


void XUser::Destroy() 
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}


void XUser::Init()
{
	ZeroMemory(m_szFileName,256);
	m_fp = NULL;
	m_bFileTransferMode = FALSE;

}

void XUser::Connect(XClient *pClient ,MonitorServer *pMonitor)
{
	m_bConnect = TRUE;
	Init();

	char buf[260];
	*(WORD *)&buf[0] = 4 + sizeof(SYSTEMTIME);

	*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_ON_TIME;

	memcpy(&buf[4],&pMonitor->m_ServerOnTime,sizeof(SYSTEMTIME));				
	SendPacket(buf,4 + sizeof(SYSTEMTIME),pClient);
				

}

void XUser::Disconnect()
{
	if( m_bConnect == FALSE ) return;

	m_bConnect = FALSE;
	Destroy();
}

void XUser::Process( char *buf, int len, XClient *pClient ,MonitorServer *pMonitor)
{
	if( m_bConnect == FALSE ) return;

	// �Ʒ����� �ڵ� �ۼ��ض�.. �ٿ���

	
	char buf2[ 512 ];
	memset( buf2, 0, 512 );

	WORD wCommand = *(WORD *)&buf[2];


	switch (wCommand) {
	case MONITOR_COMMAND_NOTICE:
		{

			UI16 usCmd = MONITOR_QUERY_NOTICE;		
			local_packet.ClearPacket();
			local_packet.WriteValue(&usCmd,2);
			local_packet.WriteValue(&buf[4],256);
			local_packet.CompletePacket();
			pMonitor->SendToServer(&local_packet);
			
			memcpy( buf2, &buf[4], 256 );
			printf(" Notice: %s", buf2 );
		}

		break;
	case MONITOR_QUERY_LOGOUT:
		{

			UI16 usCmd = MONITOR_QUERY_LOGOUT;		
			local_packet.ClearPacket();
			local_packet.WriteValue(&usCmd,2);			
			local_packet.CompletePacket();
			pMonitor->SendToServer(&local_packet);
			
			
		}

		break;
	
		
	case MONITOR_COMMAND_SERVER_ON:
		writeInfoToFile( TRUE, "log.txt", "Turn On Server" );
		pMonitor->TurnOnServer();
		break;
	case MONITOR_COMMAND_SERVER_OFF:
		writeInfoToFile( TRUE, "log.txt", "Turn Off Server" );
		pMonitor->TurnOffServer();
		break;
	case MONITOR_COMMAND_FILETRANSFERSTART:
		{
			SetCurrentDirectory(g_OriginalPath);
			Monitor_Command_FileTransferStart *pmcf = (Monitor_Command_FileTransferStart *)&buf[4];
			
			FileTransferStart(pmcf);
			printf(" ���� ���� ����! %s [ %ld Bytes ]\n",pmcf->szFileName,pmcf->siFileSize);
		}
		break;		
		// ��ġ ������ ������ �޼���(Leeminsu)
	case PATCH_COMMAND_FILETRANSFERSTART:
		{
			SetCurrentDirectory(g_OriginalPath);
			Patch_Command_FileTransferStart *pmcf = 
				(Patch_Command_FileTransferStart *)&buf[4];
			
			PatchFileTransferStart(pmcf);
			printf("(Patch Client)���� ���� ����! %s [ %ld Bytes ]\n",
				pmcf->szFileName,pmcf->siFileSize);
		}
		break;
	case MONITOR_COMMAND_FILETRANSFER:
		{
			
		/*	char *temp = (char *)malloc(len - 4);
			local_packet.ReadValue(temp,len - 4);*/
			WriteFile(&buf[4],len - 4,pClient);
		//	free(temp);
		}
		break;
	case MONITOR_COMMAND_REBOOT:
		{
			SetCurrentDirectory(g_OriginalPath);
			STARTUPINFO si;
			ZeroMemory(&si,sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEFILLATTRIBUTE;
			si.dwFillAttribute = FOREGROUND_GREEN;
			si.wShowWindow = SW_MINIMIZE;

			

			PROCESS_INFORMATION pi;
			if (CreateProcess("monitorupdate.exe",&buf[4],NULL,NULL,NULL,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
			{
				pMonitor->Stop();
			}
			
		}
		break;
	case MONITOR_COMMAND_QUERY_EXECFILENAME:
		{
			SetCurrentDirectory(g_OriginalPath);

			FILE *fp;
			fp = fopen("serverexec.txt","rt");
			if (fp != NULL) {
				
				
				char buf[260];
				*(WORD *)&buf[0] = 260;
			    *(WORD *)&buf[2] = MONITOR_COMMAND_ANSWER_EXECFILENAME;
				ZeroMemory(&buf[4],256);
				fgets(&buf[4],256,fp);

				fclose(fp);

				SendPacket(buf,260,pClient);
			}

		}
		break;
	}
		
}





void XUser::SendPacket( char *pPacket, int len, XClient *pClient )
{

	UI16 usCmd = MMC_MONITOR_TO_MASTER_CLIENT;
	UI16 usLen = len;

	printf("Send to Client: usercode %d\n", m_usercode );

	local_packet.ClearPacket();
	local_packet.WriteValue( &usCmd, 2 );
	local_packet.WriteValue( &m_usercode, 2 );
	local_packet.WriteValue( &usLen, 2 );
	local_packet.WriteValue( pPacket, len );
	local_packet.CompletePacket();

	pClient->GetSocket()->SendPacket( &local_packet );
}

void  XUser::FileTransferStart(Monitor_Command_FileTransferStart *pMcf)
{
	if (m_bFileTransferMode)
	{
		printf("�̹� ���� �������Դϴ�.\n");		
		return;
	}
	strcpy(m_szFileName,pMcf->szFileName);
	strcpy(m_szDirectory,pMcf->szDirectory);
	m_siMaxFileSize = pMcf->siFileSize;
	m_siNowFileSize = 0;
	if (!SetCurrentDirectory(m_szDirectory)) {
		
		printf("��� %s �� �������� �ʽ��ϴ�!\n",m_szDirectory);
		SetCurrentDirectory(g_OriginalPath);
	}
	else
	{
		printf("��� %s �� �޽��ϴ�!\n",m_szDirectory);
	}
	

	char filename[256];

	CreateTempFileName(pMcf->szFileName,filename);


	m_fp = fopen(filename,"wb");

	// ���丮 ���󺹱�
	SetCurrentDirectory(g_OriginalPath);

	if (m_fp == NULL)
	{
		printf("���� ���� ����\n");		
		return;
	}
	m_bFileTransferMode = TRUE;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ��	 �� : ������ġ�� ���� ����
// �� �� �� : 2004. 01.09 PM __Lee_min_su__
//
void  XUser::PatchFileTransferStart(Patch_Command_FileTransferStart *pMcf)
{
	if (m_bFileTransferMode)
	{
		printf("�̹� ���� �������Դϴ�.\n");		
		return;
	}
	strcpy(m_szFileName,pMcf->szFileName);
	strcpy(m_szDirectory,pMcf->szDirectory);
	m_siMaxFileSize = pMcf->siFileSize;
	m_siNowFileSize = 0;

	if(pMcf->byNewVersion == 1) // �� ������ ��� ������ �����.
		CreateDirectory(m_szDirectory, NULL);

	if (!SetCurrentDirectory(m_szDirectory)) {
		
		printf("��� %s �� �������� �ʽ��ϴ�!\n",m_szDirectory);
		SetCurrentDirectory(g_OriginalPath);
	}
	else
	{
		printf("��� %s �� �޽��ϴ�!\n",m_szDirectory);
	}
	

	char filename[256];

	CreateTempFileName(pMcf->szFileName,filename);


	m_fp = fopen(filename,"wb");

	// ���丮 ���󺹱�
	SetCurrentDirectory(g_OriginalPath);

	if (m_fp == NULL)
	{
		printf("���� ���� ����\n");		
		return;
	}
	m_bFileTransferMode = TRUE;
}

void  XUser::FileTransferStop()
{

	
	m_bFileTransferMode = FALSE;
	if (m_fp)
	{
		fclose(m_fp);
	}
	m_fp = NULL;


	// �۾� ���丮�� ���� ���丮�� ����

	SetCurrentDirectory(m_szDirectory);
	char tempfilename[256];

	// �ӽ������� �������� ���Ϸ� ����
	
	CreateTempFileName(m_szFileName,tempfilename);
	CopyFile(tempfilename,m_szFileName,FALSE);

	// �ӽ����� ����
	_unlink(tempfilename);

	// �������丮�� ����
	SetCurrentDirectory(g_OriginalPath);	
}
void  XUser::WriteFile(char *buf,int length,XClient *pClient)
{
	if (m_bFileTransferMode)
	{
		if (m_fp)
		{
			
			fwrite(buf,1,length,m_fp);
			m_siNowFileSize += length;
			printf(" %d byte ���� \n",m_siNowFileSize);
			if (m_siNowFileSize >= m_siMaxFileSize)
			{
				printf("�ޱ� �Ϸ�!!\n",m_siNowFileSize);

				char buf[260];
				*(WORD *)&buf[0] = 260;
			    *(WORD *)&buf[2] = MONITOR_COMMAND_FILETRANSFEREND;
				ZeroMemory(&buf[4],256);
				strncpy(&buf[4],m_szFileName,256);
				

				SendPacket(buf,260,pClient);

				FileTransferStop();
				int filenamelen;
			    if ((filenamelen = strlen(m_szFileName)) >= 4)
				{
					char extstr[256];

					
					
					strcpy(extstr,&m_szFileName[filenamelen - 4]);
					if (!_stricmp(extstr,".zip"))
					{

						SetCurrentDirectory(m_szDirectory);

						char *filenamebuffer = (char *)malloc(1024);
						char *fileexpandbuffer = (char *)malloc(65536);

						printf("Zip �����Դϴ�. ������ Ǳ�ϴ�.\n");
						unzFile uf;
						FILE *fp;
						uf = unzOpen(m_szFileName);
						if (uf == NULL)
						{
							printf("unzOpen Failed\n");
							
						}
						else {
							
							unzGoToFirstFile(uf);
								
							
							do {
								unzOpenCurrentFile(uf);

								unzGetCurrentFileInfo(uf,NULL,filenamebuffer,1024,NULL,0,NULL,0);
								printf(" %s ���� Ǯ�����!\n",filenamebuffer);
								CreateDirectoryFromFilename(filenamebuffer);

								fp = fopen(filenamebuffer,"wb");

								if (fp != NULL) {
									int ret;

									while ( (ret = unzReadCurrentFile(uf,fileexpandbuffer,65536))  > 0)
									{
										fwrite(fileexpandbuffer,1,ret,fp);

									}
									fclose(fp);
								}
								else
								{
									printf("���� ���� ����\n");

								}


								


								unzCloseCurrentFile(uf);
								

								printf(" %s ���� Ǯ�ⳡ\n",filenamebuffer);
									
									
							} while (unzGoToNextFile(uf) == UNZ_OK);
							
						} 
						
						if (!uf) unzClose(uf);
						free(filenamebuffer);
						free(fileexpandbuffer);

					}


				}
				SetCurrentDirectory(g_OriginalPath);
				
			}
		}
	}
}

void XUser::Process()
{
	DWORD nowTime = timeGetTime();
	/*DWORD m_lasttime;
	if (nowTime - m_lasttime >= 1000)
	{
		char buf[4];
		*(WORD *)&buf[0] = 4;
		*(WORD *)&buf[2] = MONITOR_COMMAND_SERVER_LIVE;
		m_pSocket->SendPacket(buf,4);
	;
				
				

	}*/


}

