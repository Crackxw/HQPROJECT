#ifndef __ONLINEGENERAL_PARSER_H
#define __ONLINEGENERAL_PARSER_H

struct	GeneralHeader
{
	SI32	siGeneralCharacterCode;															// ��� Character Code

	SI32	siGeneralBaseCharacterCode;														// ����� ��ȯ������ Character Code
	SI32	siGeneralChangeNeedCharacterLv;													// ����� ��ȯ������ Character �� �ּ� ����

	SI32	siGeneralChangeNeedTradeGrade;													// ����� ��ȯ�Ҷ� �ʿ��� �ſ���
};


class OnlineGeneralParser
{
public:
	GeneralHeader			*pGeneralHeader;												// General�� ��� ������ ���� ����ü
	SI16					siGeneralNum;													// �� General
	
public:
	OnlineGeneralParser();
	virtual	~OnlineGeneralParser();

	BOOL					ReadText(char *pFileName);										// General �ε�
	VOID					Free();															// �����

	GeneralHeader*			GetGeneral(SI32 siGeneralCharacterCode);						// Get General
	SI32					GetID(char *pToken);											// �ɸ����� ���̵� ���� �޴´�.
	GeneralHeader*			GetGeneralChar(SI32 siCharCode);
};


#endif
