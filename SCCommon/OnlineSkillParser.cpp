/*********************************************************
	����� : ������
*********************************************************/
#include <GSL.h>

#include "OnlineSkillParser.h"
#include "BindJxFile.h"

void OnlineSkillParser::Init()
{
	FILE *fp = BindJXFile::GetInstance()->GetFile( "Online\\CharInfo\\Skill.txt" );	

	if(!fp )	return;

	char szTempBuffer[1024];
	char *pToken = NULL;
	SkillHeader AddSkill;

	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);

	while(fgets(szTempBuffer, 1024, fp))
	{
		pToken = strtok(szTempBuffer, "\t\n\r");
		AddSkill.uiSkillCode = atoi(pToken);						// ��ų �ڵ�

		pToken = strtok(NULL, "\t\n\r");
		pToken = strtok(NULL, "\t\n\r");	
		AddSkill.uiNameCode = atoi(pToken);							// ��ų �̸�

		pToken = strtok(NULL, "\t\n\r");
		pToken = strtok(NULL, "\t\n\r");
		AddSkill.uiSkillDesc = atoi(pToken);						// ��ų ����

		pToken = strtok(NULL, "\t\n\r");
		AddSkill.uiNeedTradeGrade = atoi(pToken);					// �ʿ� �ſ� ���

		pToken = strtok(NULL, "\t\n\r");
		AddSkill.uiMaxSkillLv = atoi(pToken);						// �ִ� ��ų ����

		pToken = strtok(NULL, "\t\n\r");
		AddSkill.uiSpendMana = atoi(pToken);						// ���� �Һ�

		pToken = strtok(NULL, "\t\n\r");
		AddSkill.uiSpendMoney = atoi(pToken);						// �� �Һ�

		pToken = strtok(NULL, "\t\n\r");
		AddSkill.bPassive = atoi(pToken) == 0 ? false : true;		// �ڵ���� ����

		m_vSkillHeader.push_back(AddSkill);
	}

	fclose(fp);
}

SkillHeader* OnlineSkillParser::GetHeaderCode(UI16 uiSkillCode)
{
	vector<SkillHeader>::iterator it;

	for(it = m_vSkillHeader.begin(); it != m_vSkillHeader.end(); it++)
	{
		if(it->uiSkillCode == uiSkillCode)
			return it;
	}

	return NULL;
}
