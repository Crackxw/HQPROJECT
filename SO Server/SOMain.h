#ifndef	SOMAIN_H
#define SOMAIN_H

//����������������������������������������������������������������������������
// ���� ����
//����������������������������������������������������������������������������
#define SYW_ONLINE										// ������ �¶��� �������� ����
#define BATTLE_SERVER_NUMBER		1					// ���������� �ϷĹ�ȣ�� ����
//����������������������������������������������������������������������������
// ������Ʈ ����
//����������������������������������������������������������������������������
#define CONNECT_PORT			6115					// Ŭ���̾�Ʈ�� ���Ӽ������� ������Ʈ
#define BATTLE_CONNECT_PORT		6120					// ���Ӽ����� ������������ ������Ʈ




//�׽�Ʈ��					
//#define CONNECT_PORT			6116					

//����������������������������������������������������������������������������
// Ŭ���� ����
//����������������������������������������������������������������������������
class	SOWorld;
/*
class	SOUtility;
class	HQSQLDB;
class	SOServer;
class	SODatabase;
class	SOGate;
class	SOPlayer;
class	SOMap;
class	SOWorld;
class	SOBattle;
class	CItem;
class	SOVillage;

struct	VillageHeader;
struct	OnlineReceiveFieldItemInfo;
*/
//����������������������������������������������������������������������������
// ��� ����
//����������������������������������������������������������������������������
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <strstrea.h>
#include <process.h>

#include <winsock2.h>
#include <winbase.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#include <winsvc.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>

#define  DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>

#include <Directives.h>
#include "MSSQLDB.h"
#include "SOGameServerParser.h"


#define		szDirPath	"..\\Server Log\\"



//����������������������������������������������������������������������������

//����������������������������������������������������������������������������
// Extern
extern	SOGameServerParser	g_GameServerInfoParser;
extern	bool				g_forceShutdown;
extern	bool				g_bShutdown;



void	createSvrDir();
void	writeInfoToFile(char *FileName, char *Content , ...);
#endif