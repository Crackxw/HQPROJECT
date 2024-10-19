//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XCONTROLSERVER_H
#define _XCONTROLSERVER_H

#include "..\\XCommon\\jxdef.h"

class JXSocket;

class XControlServer 
{
public:
	XControlServer();
	~XControlServer();

public:
	void			SetSocket( JXSocket *pSocket ){ m_pSocket = pSocket; };
	JXSocket*		GetSocket() { return m_pSocket; };

private:
	JXSocket		*m_pSocket;
	

};

#endif