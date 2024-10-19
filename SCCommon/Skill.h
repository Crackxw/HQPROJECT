#ifndef __CSKILLS__
#define __CSKILLS__

#include "_SkillDefine.h"

//������������������������������������������������������������������������������������������������������������������������
//
// class : ��� ��ų�� �⺻ Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CSkill
{
private:	
	const int				m_kind;						// �� ��ų�� ���� 
	
	bool					m_bUsed;					// �� ��ų�� ��� ���� 
	
	int						m_curLevel;					// ���� �� ��ų�� ���� 
	const int				m_maxLevel;					// �� ��ų�� �ִ� ���� 

public:
	CSkill(int kind, int maxLevel);
	~CSkill();
	
	bool			isLearnSkill();						// �� ��ų�� ����°�?

	int				getKind();							// �� ��ų�� ������ ���´� 	
	void			setCurLevel(int value);				// ���� �� ��ų�� ������ �����Ѵ� 
	
	void			setUsed(bool which);				// �� ��ų ��� ���� ���� 	
	bool			getUsed();							// ���� �� ��ų�� �����¸� ���´� 


	int				LevelUp();							// ��ų�� �������Ѵ� 	
};


#endif