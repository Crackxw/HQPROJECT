#ifndef	ON_SMPOINT_PARSER_HEADER
#define ON_SMPOINT_PARSER_HEADER

#include <windows.h>
#include <directives.h>

class	cltShipMovePointParser
{
private:
	SI32				siShipMovePointNum;
	POINT				*ptShipMovePoint;
public:
	cltShipMovePointParser();				// ������.
	~cltShipMovePointParser();				// �Ҹ���.

	BOOL				Load(CHAR *pszFileName);		// ȭ���� �ε��Ѵ�.
	VOID				Free();							// �޸𸮸� �����Ѵ�.
	SI32				GetPointNum();					// ��ü ����Ʈ ������ ���´�.
	POINT*				GetPoint();						// ����Ʈ �迭�� �����͸� ���´�.
};

#endif
