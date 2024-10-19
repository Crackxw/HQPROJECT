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
	SI16		m_MaxFollowerNum;		// 최대로 가질수 있는 용병수
	SI32		m_ReqCredit;			// 필요 신용도
	SI32		m_ReqLv;				// 필요 레벨
} MercenaryHeader;


class OnlineMercenaryParser
{
public:
	vector< MercenaryHeader >	_Mercenary;

public:
	OnlineMercenaryParser()		{}
	~OnlineMercenaryParser()	{ Free(); }

	BOOL					ReadText(char *pFileName);											// Mercenary 로드
	VOID					Free() { _Mercenary.clear(); }										// 지운다

	SI16					GetMaxFollowerNum(SI32 NowCredit, SI32 NowLv);						// 현재 상태로 가질수 있는 최대 용병수 얻기
};


#endif
