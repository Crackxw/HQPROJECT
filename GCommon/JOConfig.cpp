/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: JOConfig.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#include "JOConfig.h"

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOConfig::CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

CJOConfig::~CJOConfig()
{
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CJOConfig::Open(LPCTSTR szConfigFileName)
{
	strcpy(m_strConfigFileName, szConfigFileName);

	return TRUE;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int CJOConfig::GetInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault /*= 0*/)
{
	return GetPrivateProfileInt(
			szSection,
			szKey,
			nDefault,
			m_strConfigFileName);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

LPCTSTR CJOConfig::GetString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault /**/)
{
	LPCTSTR	strValue;
	TCHAR		szValue[256];

	GetPrivateProfileString(
		szSection,
		szKey,
		szDefault,
		szValue,
		256,
		m_strConfigFileName);

	strValue = szValue;

	return strValue;
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CJOConfig::WriteInt(LPCTSTR szSection, LPCTSTR szKey, int nValue)
{
	char	strValue[256]={0,};

	_stprintf(strValue, "%d", nValue);

	return WriteString(szSection, szKey, strValue);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

BOOL CJOConfig::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	return WritePrivateProfileString(
		szSection, szKey, szValue, m_strConfigFileName);
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
