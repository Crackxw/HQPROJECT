#ifndef _CONFIG_H
#define _CONFIG_H

#define INTERFACEMODE_1BUTTON   1
#define INTERFACEMODE_2BUTTON   2

#include "team.h"


class _Config
{
public:
     DWORD			m_uiInterfaceMode;					// 마우스의 인터 페이스를 어떻게 설정할 것인가?
	 DWORD			m_uiShortCutKeyShowSwitch;           // 단축키 버튼을 표시하는지 여부를 알려주는 변수 

	 DWORD			m_uiCDMusicSwitch;
	 DWORD			m_uiPrevCDMusicSwitch;

	 int			m_siGamma;
	 int			m_siGameSpeed;
	 int			m_siEffectSoundVolume;			// 효과음(Static Sound) 볼륨 값
	 int			m_siBackgroundMusicVolume;		// 배경음악(Stream Sound) 볼륨 값

	 SI16			m_ssTeamInfo[MAX_TEAM_NUMBER][ON_MAX_FOLLOWER_NUM];			// 사용자 팀 정보 저장데이터. 

	 DWORD			m_uiReserved[32];

public:
	 void Init();

	 void SetGameSpeed(int speed);
	 // 실제 프레임당 클럭수를 구한다. 
     int GetRealGameSpeedClock();

	
	// CD의 상태를 설정한다. 
    void SetCD(SHORT cdmusicswitch);
    void CDAction();   // 현재 cd 설정 상태에 따라 행동을 조정한다. 


	// 감마의 양을 설정한다.
    BOOL GammaFunction(SHORT mount);

	void SetEffectSoundVolume(int volume);		// 효과음 볼륨 설정
	void SetBackgroundMusicVolume(int volume);	// 배경 음악 볼륨 설정

	BOOL Save();
	BOOL Load();

	// 플레이어를 등록한다. 
    void SetNewPlayer(SHORT slot, char* name);
	// 플레이어 이름을 변경한다. 
    void ChangePlayerName(SHORT slot, char*name);
	// 플레이어를 제거한다. 
    void DeletePlayer(SHORT slot);
	// 슬롯의 플레이어 번호를 가지고 플레이어의 이름을 얻어온다. 
	// 유효하지 않으면 NULL
    char* GetPlayerName(SHORT slot);
	// 슬롯 번호와 국가를 가지고 어느 스테이지까지 플레이했는지 알려준다.
    int GetStageDoneNumber(SHORT slot, SHORT country);
	// 플레이한 스테이지를 설정한다. 
    void SetStageDoneNumber(SHORT slot, SHORT country, UI32 stagenumber);
    // 현재 사용자의 슬롯 번호를 구한다.
    SHORT GetCurrentPlayerSlot();
	// 현재 사용자의 슬롯 번호를  설정한다.
    void SetCurrentPlayerSlot(SHORT slot);
    // 빈 슬롯을 찾아서 리턴한다. 
    // 빈 슬롯이 없으면 -1 리턴 
    SHORT FindEmptyPlayerSlot();


	void SaveBattleNetID(char *name);
    
	// 저장되어 있으면 TRUE, 저장되어 있지 않으면 FALSE
	BOOL LoadBattleNetID(char *name);  


	void SetTeamInfo(SI16	siTeam);
	void GetTeamInfo();
};

#endif