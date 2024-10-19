#ifndef __CWORLDINFO_PARSER__
#define __CWORLDINFO_PARSER__

#include "_FrontDefine.h"
#include "..\SCCommon\JOGersangConfig.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 월드 정보를 파일에서 읽어오는 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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