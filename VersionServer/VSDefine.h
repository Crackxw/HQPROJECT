#ifndef VSDEFINE_H
#define VSDEFINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// HQTools\\GSAutopatch Project에서도 사용합니다.	// 2004/10/5 17:6

#define MAX_CONNECTION	2048
#define MAX_BUFFERSIZE	256
#define MAX_IP			128
#define MAX_HOSTLEN		128
#define MAX_IDLEN		16
#define MAX_PWLEN		16
#define MAX_VER			100
#define MAX_FTP_PATH	128

#define CLIENT_FULL		-1

///////////////////////////////////////////////////////////////////////////////////////////
// robypark 2004/10/5 17:48
// 버전 서버 포트 번호 상수 선언하여 사용
#define PORT_NUMBER_VERSION_SERVER			4011
//#define PORT_NUMBER_VERSION_SERVER			30000


#endif