#ifndef __CWORKMGR__
#define	__CWORKMGR__

#include "../kojimodo/utility/Singleton.h"


class CWorkQueue;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CWorkMgr
{

private:
	
	CWorkQueue*		m_QueryQueue;				// 작업 요청을 관리 하는 큐 
	CWorkQueue*		m_ResultQueue;				// 작업 결과를 관리 하는 큐 


protected:
	
	CWorkMgr();
	virtual ~CWorkMgr();
	
	void			CreateQueue( int max_queue );

public:

	int				setQuery  ( AUTH::sUniqueID	gameServerUniqID, 
								AUTH::sUniqueID userUniqID, char* data, int dataSize);
	
	int				setResult ( AUTH::sUniqueID	gameServerUniqID, 
								AUTH::sUniqueID userUniqID, char* data, int dataSize);
	
	int				getQuery  (char* outData, int maxOutSize);
	int				getResult (char* outData, int maxOutSize);
};




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CAccountDBWorkMgr:public CWorkMgr, public CSingleton<CAccountDBWorkMgr>
{
private:
	bool		m_bShutdown;

public:
	CAccountDBWorkMgr( int max_queue ): m_bShutdown(FALSE)
	{
		CreateQueue( max_queue );
	}
	~CAccountDBWorkMgr() {}

	bool isShutdown() { return m_bShutdown; }
	void Shutdown()	  { m_bShutdown = TRUE; }
};




//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CAuthWorkMgr:public CWorkMgr, public CSingleton<CAuthWorkMgr>
{
private:
	bool		m_bShutdown;

public:
	CAuthWorkMgr( int max_queue ): m_bShutdown(FALSE)
	{
		CreateQueue( max_queue );
	}
	~CAuthWorkMgr() {}

	bool isShutdown() { return m_bShutdown; }
	void Shutdown()	  { m_bShutdown = TRUE; }
};


#endif