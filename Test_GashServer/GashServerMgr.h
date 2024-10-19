

#ifndef __GASH_SERVERMGR_H_INCLUDED__
#define __GASH_SERVERMGR_H_INCLUDED__

class CGashServer;

class CGashServerMgr
{
public : 
	CGashServerMgr();
	~CGashServerMgr();


	void Init();
	void Start();
	void StartThread();
	void StopThread();

private : 
	CGashServer*		m_pGashServer;
	short				m_wPort[3];
	char				m_szOutputString[3][256];
};

extern CGashServerMgr g_GahsServerMgr;
#endif // __GASH_SERVERMGR_H_INCLUDED__