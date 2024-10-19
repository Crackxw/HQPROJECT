#ifndef _POISONEFFECT_H
#define _POISONEFFECT_H


#include <windows.h>
#include <windowsx.h>

#include <directives.h>


class cltPoisonEffect{
private:
	// 중독 관련 변수. 
	BOOL  			m_bPoisonSwitch;   // 중독 여부,
	SI32 			m_siPoisonDelay;    // 중독 시간. 
	SI32			m_siPoisonPower;    // 중독된 정도. 

	// robypark 2004/8/9 13:31
	_NewID			m_PoisonAttacker;	// 해당 캐릭터에게 독공격을 하여 중독시킨 캐릭터 ID

public:
	cltPoisonEffect();

    // 중독 관련 함수. 
    void			SetPoisonSwitch(BOOL mode){m_bPoisonSwitch = mode;};
    BOOL 			GetPoisonSwitch()const{return m_bPoisonSwitch;} ;
	void			SetPoisonDelay(SI32 delay){m_siPoisonDelay = delay;};
	SI32			GetPoisonDelay()const{return m_siPoisonDelay;};
	void			SetPoisonPower(SI32 power){m_siPoisonPower = power;};
	SI32			GetPoisonPower()const{return m_siPoisonPower;};

	// robypark 2004/8/9 13:31
	// 독 공격한 캐릭터 ID 설정
	void			SetPoisonAttacker(_NewID PoisonAttacker);
	// robypark 2004/8/9 13:31
	// 독 공격한 캐릭터 ID 얻기
	_NewID			GetPoisonAttacker(void);

	void			Set(BOOL mode, SI32 delay, SI32 poisonpower);
	void			Draw(POINT drawstartpoint, RECT realimagerect);	// 중독된 모습을 그려준다.
	SI32			Action();

};



#endif
