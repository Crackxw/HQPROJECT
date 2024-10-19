// ScriptParser.h: interface for the ScriptParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTPARSER_H__7A42E86A_E142_4E19_8753_5C2E7E78FF7E__INCLUDED_)
#define AFX_SCRIPTPARSER_H__7A42E86A_E142_4E19_8753_5C2E7E78FF7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>
#include "VSDefine.h"
#include <map>
#include "VersionInf.h"
//#include "..\SCCommon\JOGersangConfig.h"

struct Version
{
	unsigned char ip[4];
	int vercount;
	int ver[MAX_VER];
};

struct Version2
{
	unsigned char MyIP[MAX_HOSTLEN];
	char MyID[MAX_IDLEN];
	char MyPW[MAX_PWLEN];
	int vercount;
	int ver[MAX_VER];
};

struct IP
{
	int ipcount;
	unsigned char ip[MAX_IP][4];
};

struct IP2
{
	int ipcount;
	char MyID[MAX_IP][MAX_IDLEN];
	char MyPW[MAX_IP][MAX_PWLEN];
	unsigned char ip[MAX_IP][MAX_HOSTLEN];
};



class ScriptParser // : public CJOGersangConfig
{
public:
	ScriptParser();
	~ScriptParser();

public:
	void	AppendIP(char *cIP, int maxsize);
	void	AppendID(char *cID, int maxsize);
	void	AppendPW(char *cPW, int maxsize);

	void	AppendVersion(int ver);
	bool	IPParsing();
	bool	VersionParsing();
	DWORD	m_siVersionCount;	// ¹öÀüÀÇ ÃÑ ¼ö

private:
	FILE	*m_fpIP;
	FILE	*m_fpVersion;
};

#endif // !defined(AFX_SCRIPTPARSER_H__7A42E86A_E142_4E19_8753_5C2E7E78FF7E__INCLUDED_)

