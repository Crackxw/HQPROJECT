#ifndef _JXDEF_H
#define _JXDEF_H

#include <winsock2.h>
#include <windows.h>
#include <process.h>


typedef double				R64;
typedef float				R32;

typedef unsigned long		UL32;
typedef signed long			SL32;

typedef unsigned int		UI32;
typedef signed int			SI32;

typedef unsigned short int	UI16;
typedef signed short int	SI16;

typedef unsigned char		UI08;
typedef signed char			SI08;
 
typedef double				RD;
typedef float				RF;

typedef unsigned long		UL;
typedef signed long			SL;	

typedef unsigned int		UI;
typedef signed int			SI;	

typedef unsigned short		US;
typedef signed short		SS;

typedef unsigned char		UC;
typedef signed char			SC;


#define JXPACKET_MAX_SIZE	30000


// JXLib 기본 메시지 ( 0 - 16 )

#define JXCMD_CONNECT				1
#define JXCMD_DISCONNECT			2

#include <stdio.h>

//extern void	writeInfoToFile(bool timeStamp, char* filename, char* str, ...);

#endif
