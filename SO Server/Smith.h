#ifndef __SO_SMITH__
#define __SO_SMITH__

class CSmith
{

	
public:
	CSmith(SOWorld *pWorld);
	~CSmith();

	static void RecvSmithItem(SOPlayer *pPlayer,UI16 uiFollowerNum,UI16 uiItemCode);
private:
	static SOWorld *m_pWorld;


    

	

};


#endif