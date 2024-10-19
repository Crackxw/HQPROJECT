#ifndef	SO_GM_LIST
#define	SO_GM_LIST

#include <windows.h>


#define	MAX_GM_USER_NUM		1000

class cltGMManager
{
private:
	DWORD	m_dwIP[ MAX_GM_USER_NUM + 1 ];	
	DWORD	m_dwCurrentGMUserNum;

private:
	DWORD	ConveterStringIPToNumericIP( CHAR *pszIP );

public:
	cltGMManager();

	BOOL	AddGM( CHAR *pszIP );

	BOOL	IsGM( CHAR *pszIP );

};



#endif

