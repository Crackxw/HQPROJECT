//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 작성자      : 이제완			
// 최종 수정일 : 2002. 7. 8
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef _XPLAYERMGR_H
#define _XPLAYERMGR_H

#include "..\\JWCommon\\JWList.h"

class XPlayer;
class SOPlayer;

class XPlayerMgr : public JWList
{
public:
	XPlayerMgr();
	virtual ~XPlayerMgr();

public:
	void			Create( SI32 siMaxPlayer );

private:
	XPlayer			*m_pXPlayer;
};

#endif 