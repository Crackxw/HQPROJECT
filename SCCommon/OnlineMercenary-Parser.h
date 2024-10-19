#ifndef __ONLINEMERCENARY_PARSER_H
#define __ONLINEMERCENARY_PARSER_H

#include <GSCDefine.h>

#pragma warning ( disable : 4786 )

#include <iostream>
#include <vector>

using namespace std;


typedef struct
{
	SI16		m_Code;					// Code
	SI16		m_MaxFollowerNum;		// �ִ�� ������ �ִ� �뺴��
	SI32		m_ReqCredit;			// �ʿ� �ſ뵵
	SI32		m_ReqLv;				// �ʿ� ����
} MercenaryHeader;


class OnlineMercenaryParser
{
public:
	vector< MercenaryHeader >	_Mercenary;

public:
	OnlineMercenaryParser()		{}
	~OnlineMercenaryParser()	{ Free(); }

	BOOL					ReadText(char *pFileName);											// Mercenary �ε�
	VOID					Free() { _Mercenary.clear(); }										// �����

	SI16					GetMaxFollowerNum(SI32 NowCredit, SI32 NowLv);						// ���� ���·� ������ �ִ� �ִ� �뺴�� ���
};


#endif
