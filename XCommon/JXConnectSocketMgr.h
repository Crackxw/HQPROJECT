//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _JXCONNECTSOCKETMGR_H
#define _JXCONNECTSOCKETMGR_H

#include "JXList.h"

class JXConnectSocket;

class JXConnectSocketMgr : public JXList
{
public:
	JXConnectSocketMgr();
	virtual ~JXConnectSocketMgr();

public:
	void				Create( SI32 siMaxSocketNum );
	JXConnectSocket*	AddSocket( char *pszIP, UI16 usPort, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 siPacketQueueSize );


private:
	JXConnectSocket		*m_pConnectSocket;
};

#endif