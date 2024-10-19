// ScriptParser.cpp: implementation of the ScriptParser class.
//
//////////////////////////////////////////////////////////////////////

#include <GSL.h>
#include "ScriptParser.h"
#include "stdlib.h"
#include "socket.h"
#include "SimpleParser.h"

Version	g_stVer;
Version2 g_stVer2;
Version3 g_stVer3;
IP		g_stIp;
IP2		g_stIp2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ScriptParser::ScriptParser()
{
	ZeroMemory(&g_stVer, sizeof(Version));
	ZeroMemory(&g_stIp, sizeof(IP));

	ZeroMemory(&g_stVer2, sizeof(Version2));
	ZeroMemory(&g_stIp2, sizeof(IP2));


	printf("\t\t#########################################\n");

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	m_fpIP		= fopen("IP4011.TXT", "r");
	m_fpVersion	= fopen("Version.TXT", "r");

	if(m_fpIP != NULL)
		IPParsing();

	if(m_fpVersion != NULL)
		VersionParsing();

	if(m_fpIP != NULL) fclose(m_fpIP);
	if(m_fpVersion != NULL) fclose(m_fpVersion);
}

ScriptParser::~ScriptParser()
{
	printf("\t\t#########################################\n");
}

bool ScriptParser::IPParsing()
{
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = " =\t\n";
	CHAR	*pToken;

	while(fgets(szReadBuffer, 1023, m_fpIP) != NULL)
	{
		// IP or Domain
		pToken	=	strtok(szReadBuffer, cSep);
		AppendIP(pToken, strlen(pToken));
		
		if(fgets(szReadBuffer, 1023, m_fpIP) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			AppendID(pToken, strlen(pToken));
		}

		if(fgets(szReadBuffer, 1023, m_fpIP) != NULL)
		{
			pToken	=	strtok(szReadBuffer, cSep);
			AppendPW(pToken, strlen(pToken));
		}

		++g_stIp.ipcount;
		++g_stIp2.ipcount;
	}

	return true;
}

void ScriptParser::AppendIP(char *cIP, int maxsize)
{
	hostent *	lpTemphostent;

	if(g_stIp.ipcount > MAX_IP)
	{
		return;
	}


	if(maxsize <= MAX_HOSTLEN)
		memcpy(g_stIp2.ip[g_stIp2.ipcount], cIP, maxsize);
	else
		memcpy(g_stIp2.ip[g_stIp2.ipcount], "gersang.hqteam.net", strlen("gersang.hqteam.net"));

	lpTemphostent = gethostbyname(cIP);
	if(lpTemphostent)
	{
		g_stIp.ip[g_stIp.ipcount][0] = (BYTE)lpTemphostent->h_addr_list[0][0];
		g_stIp.ip[g_stIp.ipcount][1] = (BYTE)lpTemphostent->h_addr_list[0][1];
		g_stIp.ip[g_stIp.ipcount][2] = (BYTE)lpTemphostent->h_addr_list[0][2];
		g_stIp.ip[g_stIp.ipcount][3] = (BYTE)lpTemphostent->h_addr_list[0][3];
	}
	
	printf("\t\t#\tIP [%d] = %d.%d.%d.%d \t#\n", g_stIp.ipcount, g_stIp.ip[g_stIp.ipcount][0], g_stIp.ip[g_stIp.ipcount][1], g_stIp.ip[g_stIp.ipcount][2], g_stIp.ip[g_stIp.ipcount][3]);
}

void ScriptParser::AppendID(char *cID, int maxsize)
{
	if(g_stIp2.ipcount > MAX_IP)
	{
		return;
	}
	if (maxsize <= MAX_IDLEN)
		memcpy((char*)g_stIp2.MyID[g_stIp2.ipcount], cID, maxsize);
	else
		memcpy((char*)g_stIp2.MyID[g_stIp2.ipcount], "anonymous", strlen("anonymous"));
}

void ScriptParser::AppendPW(char *cPW, int maxsize)
{
	if(g_stIp2.ipcount > MAX_IP)
	{
		return;
	}
	if (maxsize <= MAX_PWLEN)
		memcpy((char*)g_stIp2.MyPW[g_stIp2.ipcount], cPW, maxsize);
	else
		memcpy((char*)g_stIp2.MyPW[g_stIp2.ipcount], "anonymous", strlen("anonymous"));
}

bool ScriptParser::VersionParsing()
{
	CHAR	szReadBuffer[1024];
	CHAR	cSep[] = " =\t\n";
	CHAR	*pToken;
	int		ver;

	while(fgets(szReadBuffer, 1023, m_fpVersion) != NULL)
	{
		pToken	= strtok(szReadBuffer, cSep);
		ver		= atoi(pToken);
		AppendVersion(ver);
	}

	return true;
}

void ScriptParser::AppendVersion(int ver)
{
	if(g_stVer.vercount > MAX_VER)
	{
		return;
	}

	printf("\t\t#\tVersion [%d] = %d \t\t#\n", g_stVer.vercount, ver);

	g_stVer.ver[g_stVer.vercount] = ver;
	++g_stVer.vercount;

	g_stVer2.ver[g_stVer2.vercount] = ver;
	++g_stVer2.vercount;
}

