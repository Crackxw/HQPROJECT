#ifndef _UPGRADE_H
#define _UPGRADE_H

#define UPGRADEATB_DEVGENERAL   1
#define UPGRADEATB_KOREA        2
#define UPGRADEATB_JAPAN        4
#define UPGRADEATB_CHINA        8

class _Upgrade {
public:
	SHORT Grade;
	SHORT Money, Tree;
	SHORT Delay;
	DWORD MinFrame;        // 이업그레이드를 실시하는 최소한의 지연 프레임 (이 프레임 이후에 이 업그레이드를 실시한다.)
	DWORD Atb;
	char  Text[128];

	void Init(SHORT grade, SHORT money, SHORT tree, SHORT delay, DWORD minframe, DWORD atb, char* text);
	SHORT GetDelay();
	BOOL IsAtb(DWORD atb){ if(Atb&atb)return TRUE; return FALSE;} ;

};


void InitUpgrade();

#endif
