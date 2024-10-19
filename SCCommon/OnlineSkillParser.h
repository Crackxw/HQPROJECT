/*********************************************************
	����� : ������
*********************************************************/

#ifndef __ON_SKILL_H
#define __ON_SKILL_H

#include <iostream>
#include <vector>
using namespace std;

struct SkillHeader
{
	UI16	uiSkillCode;						// ��ų �ڵ�
	UI32	uiNameCode;							// ��ų �̸�
	UI32	uiSkillDesc;						// ��ų ����
	UI08	uiNeedTradeGrade;					// �ʿ� �ſ���
	UI08	uiMaxSkillLv;						// �ִ� ��ų ����
	UI16	uiSpendMana;						// ���� �Һ�
	UI16	uiSpendMoney;						// �� �Һ�
	bool	bPassive;							// �ڵ� ��� ( FALSE = ��ų �ڵ� ��� )
};


class OnlineSkillParser
{
private:
	vector<SkillHeader>	m_vSkillHeader;							// ���
	
public:
	OnlineSkillParser()	 {};
	~OnlineSkillParser() { m_vSkillHeader.clear(); };

	void Init();												// �ʱ�ȭ
	SI16		GetTotal() { return m_vSkillHeader.size(); };	// �� ���� ����
	SkillHeader *GetHeaderIndex(UI08 uiIndex) 	{return uiIndex != -1 ? &m_vSkillHeader[uiIndex] : NULL;};	// ��� ����
	SkillHeader *GetHeaderCode(UI16 uiSkillCode);
};

#endif




