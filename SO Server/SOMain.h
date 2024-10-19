#ifndef	SOMAIN_H
#define SOMAIN_H

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 서버 종류
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define SYW_ONLINE										// 임진록 온라인 서버임을 선언
#define BATTLE_SERVER_NUMBER		1					// 전투서버의 일렬번호를 설정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 접속포트 설정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define CONNECT_PORT			6115					// 클라이언트와 게임서버와의 접속포트
#define BATTLE_CONNECT_PORT		6120					// 게임서버와 전투서버와의 접속포트




//테스트용					
//#define CONNECT_PORT			6116					

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 클레스 선언
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 헤더 선언
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Extern
extern	SOGameServerParser	g_GameServerInfoParser;
extern	bool				g_forceShutdown;
extern	bool				g_bShutdown;



void	createSvrDir();
void	writeInfoToFile(char *FileName, char *Content , ...);
#endif