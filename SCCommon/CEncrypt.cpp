#include "CEncrypt.h"

CEncrypt *CEncrypt::Instence = NULL;
CEncrypt::CEncrypt()
{
}

CEncrypt::~CEncrypt()
{
//	ReleaseInstance();
}

CEncrypt *CEncrypt::GetInstance()
{
	if(Instence == NULL)
		Instence = new CEncrypt;

	return Instence;
}

void CEncrypt::ReleaseInstance()
{
	if(Instence != NULL)
		delete Instence;

	Instence = NULL;
}

void CEncrypt::SetKey(char *PersonalKey)
{	
	strcpy(PersonalKey, _DEFAULT_PKEY);
	strcat(PersonalKey, _RUNNING_KEY);
}		

int CEncrypt::EncodeMessage(char *dst, char *src, int nSize)
{
	if(nSize <= 0) return -1;
	char szKey[64] = {0,};

	SetKey(szKey);
	_xorstring(dst, src, szKey, nSize);

	return 1;
}

int CEncrypt::DecodeMessage(char *dst, char *src, int nSize)
{
	if(nSize <=0) return -1;
	char szKey[64] = {0,};
	
	SetKey(szKey);
	_xorstring(dst, src, szKey, nSize);

	return 1;
}

int CEncrypt::Encode(char *dst, char *src)
{
	int rn = rand() % 99;
	int t1, t2;
	char t3[65500] = {0,}, tz[65500] = {0,};

	_swapint(&t1, &rn, "2413");
	t2 = t1 ^ 0xffffffff;
	_256to64(tz, (char *)&t2, sizeof(int), DEFAULT_TABLE);

	_shlstring(t3, src, rn);
	strcat(tz, t3);
//	_xorstring(dst, tz);
	return 1;
}

int CEncrypt::Decode(char *dst, char *src)
{
#define INTCODESIZE (sizeof(int)*8+5)/6

	int rn;
	int *t1, t2;
	char t3[4096], t4[4096];
	char tz[65500];

	if( src[ strlen(src)-1 ] == '\n' ) src[ strlen(src)-1 ] = '\0';
//	_xorstring(tz, src);

	strncpy(t4, tz, INTCODESIZE);
	t4[INTCODESIZE] = '\0';
	_64to256(t3, t4, DEFAULT_TABLE);
	t1 = (int *)t3;
	t2 = *t1 ^ 0xffffffff;
	_swapint(&rn, &t2, "3142");
	_shrstring(dst, tz+INTCODESIZE, rn);
	return 1;
}

int CEncrypt::_256to64(char *dst, char *src, int len, char *table)
{
	unsigned int dw, dwcounter;
	int i;

	if( !dst || !src || !table ) return 0;

	dw = 0;
	dwcounter = 0;
	for(i=0; i<len; i++)
	{
		dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[dw & 0x3f];
		dw = ( dw >> 6 );

		if( i%3 == 2 )
		{
			dst[ dwcounter++ ] = table[ dw & 0x3f ];
			dw = 0;
		}
	}

	if( dw ) dst[ dwcounter++ ] = table[dw];
	dst[dwcounter] = '\0';
	return dwcounter;
}

int CEncrypt::_64to256(char *dst, char *src, char *table)
{
	unsigned int dw, dwcounter;
	int i, len;
	char *ptr = NULL;

	if( !dst || !src || !table ) return 0;

	dw = 0;
	dwcounter = 0;
	len = (int)strlen(src);

	for( i=0; i<len; i++)
	{
		ptr = (char *)strchr(table, src[i]);
		if( !ptr ) return 0;
		if( i%4 )
		{
			dw = ((unsigned int)(ptr-table) & 0x3f) << ((4-(i%4))*2) | dw;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		}
		else
		{
			dw = (unsigned int)(ptr-table) & 0x3f;
		}
	}
	if( dw ) dst[ dwcounter++ ] = dw & 0xff;
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

int CEncrypt::_256to64_shr(char *dst, char *src, int len, char *table, char *key)
{
	unsigned int dw, dwcounter;
	int i, j;

	if( 1 > strlen(key) ) return -1;

	if( !dst || !src || !table || !key ) return -1;

	dw = 0;
	dwcounter = 0;
	j = 0;

	for(i=0; i<len; i++)
	{
		dw = ( ( (unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[ ((dw&0x3f) + key[j]) %64];
		j++;
		if( !key[j] ) j=0;
		dw = (dw>>6);
		
		if( i%3 == 2 )
		{
			dst[ dwcounter++ ] = table[ ((dw & 0x3f ) + key[j]) %64 ];
			j++;
			if( !key[j] ) j=0;
			dw = 0;
		}
		
	}
	if( dw ) dst[ dwcounter++ ] = table[ (dw+key[j]) % 64 ];
	dst[dwcounter] = '\0';
	return dwcounter;
}

int CEncrypt::_256to64_shl(char *dst, char *src, int len, char *table, char *key)
{
	unsigned int dw, dwcounter;
	int i,j;

	if( 0 >= strlen(key) ) return -1;

	if( !dst || !src || !table || !key ) return -1;

	dw = 0;
	dwcounter = 0;
	j=0;

	for(i=0; i<len; i++)
	{
		dw = ( ( (unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
		dst[ dwcounter++ ] = table[ ((dw&0x3f) + 64 - key[j]) % 64 ];
		j++;
		if( !key[j] ) j=0;
		dw = (dw >> 6);
		if( i%3 == 2 )
		{
			dst[ dwcounter++ ] = table[ ((dw&0x3f) + 64 - key[j]) % 64 ];
			j++;
			if( !key[j] ) j=0;
			dw = 0;
		}
	}
	if( dw ) dst[ dwcounter++ ] = table[ (dw+64-key[j]) % 64];
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

int CEncrypt::_shl_64to256(char *dst, char *src, char *table, char *key)
{
	unsigned int dw, dwcounter, i,j, len;
	char *ptr = NULL;

	if( !key || 0 >= strlen(key) ) return 0;

	dw = 0;
	dwcounter = 0;
	j = 0;
	len = (int)strlen(src);

	if( !dst || !src || !table ) return 0;

	for(i=0; i<len; i++)
	{
		ptr = strchr(table, src[i]);
		if( !ptr ) return 0;
		if( i%4 )
		{
			dw = ((((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64) << (( 4-(i%4))*2) | dw;
			j++;
			if( !key[j] ) j=0;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		}
		else
		{
			dw = (((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64;
			j++;
			if( !key[j] ) j = 0;
		}
	}
	if( dw ) dst[ dwcounter++ ] = dw & 0xff;
	dst[ dwcounter ] = '\0';
	return dwcounter;
}

int CEncrypt::_shr_64to256(char *dst, char *src, char *table, char *key)
{
	unsigned int dw, dwcounter, i, j, len;
	char *ptr = NULL;

	if( !key || 0 >= strlen(key) ) return 0;

	dw = 0;
	dwcounter = 0;
	j = 0;

	if( !dst || !src || !table ) return 0;

	dw = 0;
	dwcounter = 0;
	j = 0;
	if( !dst || !src || !table ) return 0;

	len = (int)strlen(src);

	for(i=0; i<len; i++)
	{
		ptr = strchr(table, src[i]);
		if( !ptr ) return 0;
		if( i%4 )
		{
			dw = ((((unsigned int)(ptr-table)&0x3f) + key[j]) % 64) << ((4-(i%4))*2) | dw;
			j++;
			if( !key[j] ) j=0;
			dst[ dwcounter++ ] = dw & 0xff;
			dw = dw >> 8;
		}
		else
		{
			dw = (((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64;
			j++;
			if( !key[j] ) j=0;
		}
	}
	if( dw ) dst[ dwcounter++ ] = dw & 0xff;
	dst[dwcounter ] = '\0';
	return dwcounter;
}

int CEncrypt::_swapint(int *dst, int *src, char *rule)
{
	char *ptr, *qtr;
	int i;

	ptr = (char *)src;
	qtr = (char *)dst;

	for(i=0; i<4; i++)
		qtr[ rule[i] - '1' ] = ptr[i];
	return 1;
}

int CEncrypt::_xorstring(char *dst, char *src, char *key, int nSize)
{
	if(dst == NULL || src == NULL) return -1;
	if(key == NULL) return -1;

	int i = 0, j = 0;
	int len = (int)strlen(key);
	for(; i<nSize; i++,j++)
	{
		if(j >= len) j = 0;
		dst[i] = src[i]^key[j];
//		dst[i] = src[i] ^255;
	}
//	dst[i] = '\0';
	
	return 1;
}

int CEncrypt::_shrstring(char *dst, char *src, int offs)
{
	char *ptr;
	int len;

	len = (int)strlen(src);

	if( !dst || !src || 0 >= len ) return -1;

	offs = len - (offs%len);
	ptr = src+offs;
	strcpy(dst, ptr);
	strncat(dst, src, offs);
	dst[ len ] = '\0';
	return 1;
}

int CEncrypt::_shlstring(char *dst, char *src, int offs)
{
	char *ptr;
	int len;
	len = (int)strlen(src);

	if( !dst || !src || 0 >= len ) return -1;

	offs = offs % len;
	ptr = src + offs;
	strcpy(dst, ptr);
	strncat(dst, src, offs);
	dst[ len ] = '\0';
	return 1;
}


