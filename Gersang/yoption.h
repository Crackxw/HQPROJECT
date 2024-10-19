#ifndef	IMJIN2_YOPTION_HEADER
#define IMJIN2_YOPTION_HEADER


#include "YOnOffControl.h"
#include "YMIControl.h"
#include "YScrSpeedControl.h"
#include "YProgressBarControl.h" 

// robypark 2004/12/27 13:50
// 배경 음악 볼륨(Stream Sound Volume) 조절 기능 추가. CD 음악 연주 기능을 완전히 지움.
// 효과음과 배경음악 볼륨 조절을 같이 하던 것을 각 각 하도록 구분.

class _YOption
{
private:
	//Static 사운드 볼륨 조절. Effect Sound
	_YSoundVolumeProgressBar	m_YEffectSoundVolume;

	//Stream 사운드 볼륨 조절. Background Music
	_YSoundVolumeProgressBar	m_YBackgroundMusicVolume;

	//게임 감마 조절
	_YGammaProgressBar			m_YGamma;

	//CD 음악 
	_YCDOnOff					m_YCDOnOff;

	//마우스 왼쪽인지 오른쪽인지 선택하는 옵션.
	_YMouseInterface			m_MouseButtonSwitch;

	//확인 및 취소 버튼.
	_Button						m_BOk;

protected:
	//배경 스프라이트.
	XSPR						m_Xspr;
	XSPR						m_Xspr_Bkgnd;
	XSPR						m_BSpr;
	
public:
	_YOption();
	~_YOption();

	VOID					Init();
	SHORT					Action();
	VOID					Put();	
	VOID					Put(HDC hdc);
	VOID					Free();
};


#endif
