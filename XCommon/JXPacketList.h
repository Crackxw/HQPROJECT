//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 7
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


#ifndef _JXPACKETLIST_H
#define _JXPACKETLIST_H

#include "JXList.h"

class JXPacket;

class JXPacketList : public JXList
{
public:
	JXPacketList();
	virtual ~JXPacketList();

public:
	void Create( SI32 siMaxPacketNum );

private:
	JXPacket		*m_pPacket;
};

#endif