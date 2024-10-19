/*********************************************************
	담당자 : 정진욱
*********************************************************/

#ifndef __ON_SKILL_H
#define __ON_SKILL_H

#include <iostream>
#include <vector>
using namespace std;

struct SkillHeader
{
	UI16	uiSkillCode;						// 스킬 코드
	UI32	uiNameCode;							// 스킬 이름
	UI32	uiSkillDesc;						// 스킬 설명
	UI08	uiNeedTradeGrade;					// 필요 신용등급
	UI08	uiMaxSkillLv;						// 최대 스킬 레벨
	UI16	uiSpendMana;						// 마나 소비량
	UI16	uiSpendMoney;						// 돈 소비
	bool	bPassive;							// 자동 사용 ( FALSE = 스킬 자동 사용 )
};


class OnlineSkillParser
{
private:
	vector<SkillHeader>	m_vSkillHeader;							// 헤더
	
public:
	OnlineSkillParser()	 {};
	~OnlineSkillParser() { m_vSkillHeader.clear(); };

	void Init();												// 초기화
	SI16		GetTotal() { return m_vSkillHeader.size(); };	// 총 수를 리턴
	SkillHeader *GetHeaderIndex(UI08 uiIndex) 	{return uiIndex != -1 ? &m_vSkillHeader[uiIndex] : NULL;};	// 헤더 리턴
	SkillHeader *GetHeaderCode(UI16 uiSkillCode);
};

#endif




