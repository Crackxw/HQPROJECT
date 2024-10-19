#ifndef __ONLINESOLDERLIST_PARSER_H
#define __ONLINESOLDERLIST_PARSER_H


#include <windows.h>
#include <directives.h>


#define MAX_SOLDER					12
#define MAX_SOLDERLIST				16

#define STRUCTCODE_NONE				0
#define STRUCTCODE_INN				1
#define STRUCTCODE_BARRACK			2
#define STRUCTCODE_GBARRACK			3
#define STRUCTCODE_MBARRACK			4
#define STRUCTCODE_GEXBARRACK		5			// STRUCTCODE_GBARRACK ���� ��� 2�� ����

#define SOILDERSTATECODE_KOREA		1
#define SOILDERSTATECODE_JAPAN		10
#define SOILDERSTATECODE_CHINA		100
#define SOILDERSTATECODE_TAIWAN		1000


typedef struct
{
	SI32	siStructorCode;																		// �ǹ� �ڵ�

	SI32	siStateCode;
	SI32	siSellSolder[MAX_SOLDER];															// �Ĵ� �뺴 ID
} SolderListHeader;


class OnlineSolderListParser
{
public:
	SolderListHeader		m_lpSolderListHeader[MAX_SOLDERLIST];								// List�� ��� ������ ���� ����ü
	
public:
	OnlineSolderListParser();
	virtual	~OnlineSolderListParser();

	BOOL					ReadText(char *pFileName);											// Ticket �ε�
	VOID					Free();																// �����

	SolderListHeader*		GetSolderList(SI32 StructCode, SI32 StateCode);						// Get Solder List
	UI16					GetID(CHAR *pToken);
};


#endif
