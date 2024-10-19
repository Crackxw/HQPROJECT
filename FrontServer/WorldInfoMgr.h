#ifndef __CWORLDINFO_MGR__
#define __CWORLDINFO_MGR__

#include "_FrontDefine.h"



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 월드 정보를 관리하는 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CWorldInfoMgr: public CSingleton<CWorldInfoMgr>
{

private:		
    WORD						m_numWorld;						// 월드의 현재 수 		
	
	
	FRONT::sWorldInfo2*			m_WorldList;					// 월드 리스트 
	CRITICAL_SECTION			m_csWorldList;					// 월드 리스트 관리 크리티컬 섹션 
	


public:
	CWorldInfoMgr();
	~CWorldInfoMgr();
	


	int				 			getWorldsInfo(FRONT::sWorldInfo* worldsInfo);


	bool						addWorld	  (FRONT::sWorldInfo2* pWorldInfo);
	void						removeWorld	  (int world_id);
	
		
	bool						isValidWorld  (int world_id);
	

	WORD						getNumWorld();
	
	bool						getGateIP	  (int world_id, char* gateIP);	
	bool						getGatePort   (int world_id, WORD* chatPort);

	bool						getChatIP	  (int world_id, char* chatIP);		
	bool						getChatPort   (int world_id, WORD* chatPort);


	bool						setConcurrent (int world_id, int concurrent);	
	bool						setWorldState (int world_id, int state);	

};



#endif