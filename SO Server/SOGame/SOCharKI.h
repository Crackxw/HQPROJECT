#ifndef	SO_CHARKI_HEADER
#define SO_CHARKI_HEADER

#include "OnlineBaseCharKI.h"

class cltSOCharKI : public cltOnlineBaseCharKI
{
private:

public:
	cltSOCharKI();						// 생성자.
	virtual	~cltSOCharKI();				// 소멸자.

	BOOL	Init(OnlineText *pOnlineText, CHAR *pszFileName);	// 초기화를 한다.
	VOID	Free();						// 메모리를 해제 한다.				
};

#endif
