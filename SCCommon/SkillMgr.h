#ifndef __CSKILLMGR__
#define __CSKILLMGR__

//������������������������������������������������������������������������������������������������������������������������
//
// class : ��ų ������ 
//
//������������������������������������������������������������������������������������������������������������������������
class SOPlayer;
class CSkill;
class CSkillMgr
{
private:
	enum			{ MAX_SKILL = 2 };
	CSkill*			m_SkillList[MAX_SKILL];

public:
	CSkillMgr(SOPlayer* owner);
	~CSkillMgr();

	CSkill*			getSkill(int which);
};

#endif 