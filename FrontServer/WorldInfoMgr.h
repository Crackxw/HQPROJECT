#ifndef __CWORLDINFO_MGR__
#define __CWORLDINFO_MGR__

#include "_FrontDefine.h"



//������������������������������������������������������������������������������������������������������������������������
//
// class : ���� ������ �����ϴ� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CWorldInfoMgr: public CSingleton<CWorldInfoMgr>
{

private:		
    WORD						m_numWorld;						// ������ ���� �� 		
	
	
	FRONT::sWorldInfo2*			m_WorldList;					// ���� ����Ʈ 
	CRITICAL_SECTION			m_csWorldList;					// ���� ����Ʈ ���� ũ��Ƽ�� ���� 
	


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