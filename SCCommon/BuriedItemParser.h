#ifndef __CBURIEDITEMPARSER__
#define	__CBURIEDITEMPARSER__

#include "_BuriedItemDefine.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 매장된 아이템 정보 파일의 파서 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CBuriedItemParser
{
	enum { MAX_ITEM = 100 };

private:
	BURIED::ITEMINFO		m_buriedItem[MAX_ITEM];						// 매장된 아이템의 종류 목록 
	int						m_numBuriedItem;							// 매장된 아이템의 종류 개수 
	
	int						m_curFindPos;								// findItem() 함수에 쓰는 변수 	

public:	
	CBuriedItemParser();
	~CBuriedItemParser();

	bool				isBuriedArea(DWORD mapIndex);				
	bool				readFile(char* filename);
	
	BURIED::ITEMINFO*	findItem(DWORD mapIndex);
};




#endif 