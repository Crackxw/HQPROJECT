#ifndef _CONFIG_H
#define _CONFIG_H

#define INTERFACEMODE_1BUTTON   1
#define INTERFACEMODE_2BUTTON   2

#include "team.h"


class _Config
{
public:
     DWORD			m_uiInterfaceMode;					// ���콺�� ���� ���̽��� ��� ������ ���ΰ�?
	 DWORD			m_uiShortCutKeyShowSwitch;           // ����Ű ��ư�� ǥ���ϴ��� ���θ� �˷��ִ� ���� 

	 DWORD			m_uiCDMusicSwitch;
	 DWORD			m_uiPrevCDMusicSwitch;

	 int			m_siGamma;
	 int			m_siGameSpeed;
	 int			m_siEffectSoundVolume;			// ȿ����(Static Sound) ���� ��
	 int			m_siBackgroundMusicVolume;		// �������(Stream Sound) ���� ��

	 SI16			m_ssTeamInfo[MAX_TEAM_NUMBER][ON_MAX_FOLLOWER_NUM];			// ����� �� ���� ���嵥����. 

	 DWORD			m_uiReserved[32];

public:
	 void Init();

	 void SetGameSpeed(int speed);
	 // ���� �����Ӵ� Ŭ������ ���Ѵ�. 
     int GetRealGameSpeedClock();

	
	// CD�� ���¸� �����Ѵ�. 
    void SetCD(SHORT cdmusicswitch);
    void CDAction();   // ���� cd ���� ���¿� ���� �ൿ�� �����Ѵ�. 


	// ������ ���� �����Ѵ�.
    BOOL GammaFunction(SHORT mount);

	void SetEffectSoundVolume(int volume);		// ȿ���� ���� ����
	void SetBackgroundMusicVolume(int volume);	// ��� ���� ���� ����

	BOOL Save();
	BOOL Load();

	// �÷��̾ ����Ѵ�. 
    void SetNewPlayer(SHORT slot, char* name);
	// �÷��̾� �̸��� �����Ѵ�. 
    void ChangePlayerName(SHORT slot, char*name);
	// �÷��̾ �����Ѵ�. 
    void DeletePlayer(SHORT slot);
	// ������ �÷��̾� ��ȣ�� ������ �÷��̾��� �̸��� ���´�. 
	// ��ȿ���� ������ NULL
    char* GetPlayerName(SHORT slot);
	// ���� ��ȣ�� ������ ������ ��� ������������ �÷����ߴ��� �˷��ش�.
    int GetStageDoneNumber(SHORT slot, SHORT country);
	// �÷����� ���������� �����Ѵ�. 
    void SetStageDoneNumber(SHORT slot, SHORT country, UI32 stagenumber);
    // ���� ������� ���� ��ȣ�� ���Ѵ�.
    SHORT GetCurrentPlayerSlot();
	// ���� ������� ���� ��ȣ��  �����Ѵ�.
    void SetCurrentPlayerSlot(SHORT slot);
    // �� ������ ã�Ƽ� �����Ѵ�. 
    // �� ������ ������ -1 ���� 
    SHORT FindEmptyPlayerSlot();


	void SaveBattleNetID(char *name);
    
	// ����Ǿ� ������ TRUE, ����Ǿ� ���� ������ FALSE
	BOOL LoadBattleNetID(char *name);  


	void SetTeamInfo(SI16	siTeam);
	void GetTeamInfo();
};

#endif