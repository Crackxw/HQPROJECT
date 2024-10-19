/*************************************************************************************************
	작성자 : 정진욱
	담당자 : 이상원
	만든날 : 2002.06.21
	클래스명 : OnlineTradeParser
*************************************************************************************************/

#ifndef __ON_TRADE_PARSER_H
#define __ON_TRADE_PARSER_H

#pragma warning ( disable : 4786 )

//#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct TradeHeader
{
	UI08	uiTradeGrade;										// 신용도 등급
	UI32	uiAccuCredit;									

	TradeHeader::TradeHeader()	{ uiTradeGrade = uiAccuCredit = 0; };
	const bool TradeHeader::operator < (const TradeHeader &rhs) const { return rhs.uiTradeGrade < this->uiTradeGrade ? true : false; };
};

class OnlineTradeParser
{
private:
	vector<TradeHeader>				m_vTradeHeader;

	typedef vector<TradeHeader>::iterator VWlter;
	typedef pair<VWlter, VWlter> VWlterPair;
public:
	OnlineTradeParser()  { Init(); };
	~OnlineTradeParser() { Free(); };


	void					Init();											// 초기화
	void					Free() { m_vTradeHeader.clear(); };				// 날림

	const TradeHeader* GetHeader(const UI32 uiAccuCredit)
	{ return uiAccuCredit == 0 ? NULL : CheckTradeGrade(uiAccuCredit); };	// 헤더를 얻어 온다.
	inline const TradeHeader* GetCredit(const UI08 uiGrade)
	{ return uiGrade == 0 ? NULL : &m_vTradeHeader[uiGrade]; };				// 등급을 받아 신용도를 리턴해 준다.		


	inline TradeHeader* CheckTradeGrade(const UI32 uiAccuCredit);
	inline UI32			CheckCreditForLevel(const UI32 uiAccuCredit);
};


inline TradeHeader* OnlineTradeParser::CheckTradeGrade(const UI32 uiAccuCredit)
{

	for(SI16 i = 0; i < (SI16)m_vTradeHeader.size() -1; i++)
	{
		if( m_vTradeHeader[i].uiAccuCredit <= uiAccuCredit && m_vTradeHeader[i+1].uiAccuCredit > uiAccuCredit )
			return &m_vTradeHeader[i];
	}

	if(m_vTradeHeader[m_vTradeHeader.size() -1].uiAccuCredit <= uiAccuCredit)	return &m_vTradeHeader[m_vTradeHeader.size() -1];
	
	return NULL;
}

inline UI32	OnlineTradeParser::CheckCreditForLevel(const UI32 uiAccuCredit)
{
	for(SI16 i = 0; i < (SI16)m_vTradeHeader.size() -1; i++)
	{
		if( m_vTradeHeader[i].uiAccuCredit <= uiAccuCredit && m_vTradeHeader[i+1].uiAccuCredit > uiAccuCredit )
			return m_vTradeHeader[i].uiTradeGrade;
		
	}
	if(m_vTradeHeader[m_vTradeHeader.size() -1].uiAccuCredit <= uiAccuCredit)	
		return m_vTradeHeader[m_vTradeHeader.size() -1].uiTradeGrade;

	if(uiAccuCredit == 0)
		return 1;
	

	return 0;
}

#endif