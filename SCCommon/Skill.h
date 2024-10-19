#ifndef __CSKILLS__
#define __CSKILLS__

#include "_SkillDefine.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
// class : 모든 스킬의 기본 클래스 
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
class CSkill
{
private:	
	const int				m_kind;						// 이 스킬의 종류 
	
	bool					m_bUsed;					// 이 스킬의 사용 여부 
	
	int						m_curLevel;					// 현재 이 스킬의 레벨 
	const int				m_maxLevel;					// 이 스킬의 최대 레벨 

public:
	CSkill(int kind, int maxLevel);
	~CSkill();
	
	bool			isLearnSkill();						// 이 스킬을 배웠는가?

	int				getKind();							// 이 스킬의 종류를 얻어온다 	
	void			setCurLevel(int value);				// 현재 이 스킬의 레벨을 설정한다 
	
	void			setUsed(bool which);				// 이 스킬 사용 여부 설정 	
	bool			getUsed();							// 현재 이 스킬의 사용상태를 얻어온다 


	int				LevelUp();							// 스킬을 레벨업한다 	
};


#endif