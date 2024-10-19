#ifndef __CSEED_H__
#define __CSEED_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "stdafx.h"
/********************* Type Definitions **********************/
#ifndef TYPE_DEFINITION
#define TYPE_DEFINITION
#if defined(__alpha)
typedef unsigned int        DWORD;
typedef unsigned short      WORD;
#else
typedef unsigned long int   DWORD;
typedef unsigned short int  WORD;
#endif
typedef unsigned char           BYTE;
#endif


/******************* Constant Definitions *********************/
#define NoRounds         16
#define NoRoundKeys      (NoRounds*2)
#define SeedBlockSize    16    /* in bytes */
#define SeedBlockLen     128   /* in bits */


/********************** Common Macros ************************/
#if defined(_MSC_VER)
#define ROTL(x, n)     (_lrotl((x), (n)))
#define ROTR(x, n)     (_lrotr((x), (n)))
#else
#define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
#define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
#endif


/**************** Function Prototype Declarations **************/
#ifndef PROTOTYPES
#define PROTOTYPES  1
#endif

#if PROTOTYPES
#define PROTO_LIST(list)    list
#else
#define PROTO_LIST(list)    ()
#endif

//************ CSeed class *****************************************/
class CSeed
{
protected:
	static CSeed *Instance;

public:
	CSeed();
	~CSeed();

	static CSeed *GetInstance();
	void ReleaseInstance();

	void SeedEncrypt PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));         //암호화 한다.
	void SeedDecrypt PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));         //복호화 한다. 

	void SeedEncRoundKey PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));	 //라운드키를 암호화 한다.
};
/******************************************************************/


#endif
