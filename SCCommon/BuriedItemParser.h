#ifndef __CBURIEDITEMPARSER__
#define	__CBURIEDITEMPARSER__

#include "_BuriedItemDefine.h"

//������������������������������������������������������������������������������������������������������������������������
//
// class : ����� ������ ���� ������ �ļ� 
//
//������������������������������������������������������������������������������������������������������������������������
class CBuriedItemParser
{
	enum { MAX_ITEM = 100 };

private:
	BURIED::ITEMINFO		m_buriedItem[MAX_ITEM];						// ����� �������� ���� ��� 
	int						m_numBuriedItem;							// ����� �������� ���� ���� 
	
	int						m_curFindPos;								// findItem() �Լ��� ���� ���� 	

public:	
	CBuriedItemParser();
	~CBuriedItemParser();

	bool				isBuriedArea(DWORD mapIndex);				
	bool				readFile(char* filename);
	
	BURIED::ITEMINFO*	findItem(DWORD mapIndex);
};




#endif 