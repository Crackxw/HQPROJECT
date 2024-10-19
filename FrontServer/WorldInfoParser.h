#ifndef __CWORLDINFO_PARSER__
#define __CWORLDINFO_PARSER__

#include "_FrontDefine.h"
#include "..\SCCommon\JOGersangConfig.h"



//������������������������������������������������������������������������������������������������������������������������
//
// class : ���� ������ ���Ͽ��� �о���� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CWorldInfoParser: 
public CJOGersangConfig,
public CSingleton<CWorldInfoParser>
{

public:
	CWorldInfoParser( char* filename );	
	~CWorldInfoParser();
	

private:
	bool						LoadWorldInfo(char* filename);
	bool						LoadWorldInfo(); // Config file load.
		
};



#endif