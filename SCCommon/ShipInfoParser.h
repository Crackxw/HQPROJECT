#ifndef	SHIPINFO_PARSER_HEADER
#define SHIPINFO_PARSER_HEADER

#include <GSCDefine.h>
#include <windows.h>
#include <directives.h>


struct	strShipInfo
{
	SI32	siID;											// ID
	SI32	siName;											// Name
	UI16	uiMaxCustom;									// �ִ� ���� �ο�
	SI16	siMoveSpeed;									// ���� �̵� �ӵ�.
};

class	cltShipInfoParser
{
private:
	SI32			siTotalShipNum;					// �ִ� ���� ��.
	strShipInfo		*pstShipInfo;					// ���� ����.

public:
	cltShipInfoParser();					// ������.
	virtual	~cltShipInfoParser();			// �Ҹ���.

	BOOL			Init(CHAR *pszFileName);		// ȭ���� �ʱ�ȭ �Ѵ�.
	VOID			Free();							// �޸𸮸� �����Ѵ�.	
	strShipInfo*	GetShipInfo(SI32 siIndex);		// ���� ������ ���´�.
	SI32			GetTotalShipNum();				// �� ���� ���� ���´�.
	UI16			GetID( CHAR *pToken );			// �ؽ�Ʈ���� 2����Ʈ ID�� �����.(���߿� OnlineChar-Parser �� ���� �����ؾ� �� �κ��̴�.)
};

#endif
