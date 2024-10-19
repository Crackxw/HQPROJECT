/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Filename: MIMEContentAgent.cpp

Author: 檜團熱 <nshangukin@joyon.com>
Currently Managed by: 檜團熱

Description:

Requirements:

History:

Last updated:

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


#include "StdAfx.h"
#include "MIMEContentAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMIMEContentAgent::CMIMEContentAgent( int nMIMEType )
{
	m_nMIMETypeIHandle = nMIMEType;
}

CMIMEContentAgent::~CMIMEContentAgent()
{

}

BOOL CMIMEContentAgent::QueryType(int nContentType)
{
	return nContentType == m_nMIMETypeIHandle ? TRUE : FALSE;
}

