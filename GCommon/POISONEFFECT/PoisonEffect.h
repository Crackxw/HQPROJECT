#ifndef _POISONEFFECT_H
#define _POISONEFFECT_H


#include <windows.h>
#include <windowsx.h>

#include <directives.h>


class cltPoisonEffect{
private:
	// �ߵ� ���� ����. 
	BOOL  			m_bPoisonSwitch;   // �ߵ� ����,
	SI32 			m_siPoisonDelay;    // �ߵ� �ð�. 
	SI32			m_siPoisonPower;    // �ߵ��� ����. 

	// robypark 2004/8/9 13:31
	_NewID			m_PoisonAttacker;	// �ش� ĳ���Ϳ��� �������� �Ͽ� �ߵ���Ų ĳ���� ID

public:
	cltPoisonEffect();

    // �ߵ� ���� �Լ�. 
    void			SetPoisonSwitch(BOOL mode){m_bPoisonSwitch = mode;};
    BOOL 			GetPoisonSwitch()const{return m_bPoisonSwitch;} ;
	void			SetPoisonDelay(SI32 delay){m_siPoisonDelay = delay;};
	SI32			GetPoisonDelay()const{return m_siPoisonDelay;};
	void			SetPoisonPower(SI32 power){m_siPoisonPower = power;};
	SI32			GetPoisonPower()const{return m_siPoisonPower;};

	// robypark 2004/8/9 13:31
	// �� ������ ĳ���� ID ����
	void			SetPoisonAttacker(_NewID PoisonAttacker);
	// robypark 2004/8/9 13:31
	// �� ������ ĳ���� ID ���
	_NewID			GetPoisonAttacker(void);

	void			Set(BOOL mode, SI32 delay, SI32 poisonpower);
	void			Draw(POINT drawstartpoint, RECT realimagerect);	// �ߵ��� ����� �׷��ش�.
	SI32			Action();

};



#endif
