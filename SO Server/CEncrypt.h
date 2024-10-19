#ifndef __CENCRYPT_H__
#define __CENCRYPT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DEFAULT_PKEY	    "gersang"
#define _RUNNING_KEY	    "87654321"
#define DEFAULT_TABLE       "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}"

#define DEFAULTFUNCBEGIN	"&"
#define DEFAULTFUNCEND		"#"

class CEncrypt
{
protected:
	static CEncrypt *Instence;

public:
	static CEncrypt *GetInstance();

private:
	CEncrypt();
	~CEncrypt();
	

public:

	void ReleaseInstance();
	void SetKey(char *key);
	int EncodeMessage(char *dst, char *src, int nSize);
	int DecodeMessage(char *dst, char *src, int nSize);

	int Encode(char *dst, char *src);
	int Decode(char *dst, char *src);
	int _256to64(char *dst, char *src, int len, char *table);
	int _64to256(char *dst, char *src, char *table);

	int _256to64_shr(char *dst, char *src, int len, char *table, char *key);
	int _256to64_shl(char *dst, char *src, int len, char *table, char *key);

	int _shl_64to256(char *dst, char *src, char *table, char *key);
	int _shr_64to256(char *dst, char *src, char *table, char *key);

	int _swapint(int *dst, int *src, char *rule);
	int _xorstring(char *dst, char *src, char *key, int nSize);
	int _shrstring(char *dst, char *src, int offs);
	int _shlstring(char *dst, char *src, int offs);
};

#endif