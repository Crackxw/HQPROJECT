#ifndef	SO_CHARKI_HEADER
#define SO_CHARKI_HEADER

#include "OnlineBaseCharKI.h"

class cltSOCharKI : public cltOnlineBaseCharKI
{
private:

public:
	cltSOCharKI();						// ������.
	virtual	~cltSOCharKI();				// �Ҹ���.

	BOOL	Init(OnlineText *pOnlineText, CHAR *pszFileName);	// �ʱ�ȭ�� �Ѵ�.
	VOID	Free();						// �޸𸮸� ���� �Ѵ�.				
};

#endif
