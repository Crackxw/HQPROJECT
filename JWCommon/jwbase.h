#ifndef _JWBASE_H
#define _JWBASE_H

#include <winsock2.h>
#include <windows.h>
#include <process.h>

typedef double				R64;
typedef float				R32;

typedef unsigned int		UI32;
typedef signed int			SI32;

typedef unsigned short int	UI16;
typedef signed short int	SI16;

typedef unsigned char		UI08;
typedef signed char			SI08;

#ifndef JWPACKET_MAX_SIZE
#define JWPACKET_MAX_SIZE	30000
#endif

#include <stdio.h>

#endif