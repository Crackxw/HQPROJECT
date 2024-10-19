#ifndef	IMJIN2_YOPTION_HEADER
#define IMJIN2_YOPTION_HEADER


#include "YOnOffControl.h"
#include "YMIControl.h"
#include "YScrSpeedControl.h"
#include "YProgressBarControl.h" 

// robypark 2004/12/27 13:50
// ��� ���� ����(Stream Sound Volume) ���� ��� �߰�. CD ���� ���� ����� ������ ����.
// ȿ������ ������� ���� ������ ���� �ϴ� ���� �� �� �ϵ��� ����.

class _YOption
{
private:
	//Static ���� ���� ����. Effect Sound
	_YSoundVolumeProgressBar	m_YEffectSoundVolume;

	//Stream ���� ���� ����. Background Music
	_YSoundVolumeProgressBar	m_YBackgroundMusicVolume;

	//���� ���� ����
	_YGammaProgressBar			m_YGamma;

	//CD ���� 
	_YCDOnOff					m_YCDOnOff;

	//���콺 �������� ���������� �����ϴ� �ɼ�.
	_YMouseInterface			m_MouseButtonSwitch;

	//Ȯ�� �� ��� ��ư.
	_Button						m_BOk;

protected:
	//��� ��������Ʈ.
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
