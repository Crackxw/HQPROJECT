#include <GSL.h>
#include <main.h>
#include "..\Gersang\rain.h"
#include <colortable.h>
#include <etc.h>
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 
extern	unsigned char				DarkTable[][256];					// ��ο� �ܰ� 
extern	CCharFileLoadManager		g_FileLoadManager;					// ������ ĳ����
extern	SI32						UserPlayerNumber;					// ����� ���� 


cltPoisonEffect::cltPoisonEffect():
	m_bPoisonSwitch(FALSE),
	m_siPoisonDelay(0),
	m_siPoisonPower(0)
{
	// robypark 2004/8/9 13:43
	// �ش� ĳ���Ϳ��� �������� �Ͽ� �ߵ���Ų ĳ���� ID �ʱ�ȭ
	m_PoisonAttacker.ID = -1;
	m_PoisonAttacker.Unique = 0;
}


void cltPoisonEffect::Set(BOOL mode, SI32 delay, SI32 poisonpower)
{
	SetPoisonSwitch(mode);
	SetPoisonDelay(delay);
	SetPoisonPower(poisonpower);
}


// �ߵ��� ����� �׷��ش�. 
void cltPoisonEffect::Draw(POINT drawstartpoint, RECT realimagerect)
{
	if(GetPoisonSwitch() == FALSE)return ;

	SHORT file	=	FILE_ON_POISONSMOKE;
	SHORT font	=	m_siPoisonDelay / 3 % 32;

	SHORT startx=(drawstartpoint.x + realimagerect.left + (realimagerect.right  - realimagerect.left)/2)-GetFileXsize(file)/2;
	SHORT starty=(drawstartpoint.y + realimagerect.top  + (realimagerect.bottom - realimagerect.top)/2) -GetFileYsize(file)/2;

	if(clGrp.IsInClipArea(startx, starty, startx+GetFileXsize(file)-1, starty+GetFileYsize(file)-1)==TRUE)
	{
		Draw_PutSpriteLightImjinT(startx, starty, file, font);
	}
	else
	{
		Draw_PutSpriteLightImjinCT(startx, starty, file, font);
	}
}


SI32 cltPoisonEffect::Action()
{
	// �ߵ��� ����. 
	if(m_siPoisonDelay)
	{
		m_siPoisonDelay --;
		
		if(m_siPoisonDelay % 50 == 0)
		{
			return m_siPoisonPower;
		}
		else
			return 0;
	}

	// robypark 2004/8/9 10:50
	// �ߵ� ���ӽð��� ���� ��� �ߵ�ȿ�� �̹����� �׷��־�� �ȵȴ�.
	// �ߵ�ȿ�� ����ġ�� FALSE��Ų��.
	m_bPoisonSwitch = FALSE;

	return 0;
}

// robypark 2004/8/9 13:31
// �� ������ ĳ���� ID ����
void cltPoisonEffect::SetPoisonAttacker(_NewID PoisonAttacker)
{
	m_PoisonAttacker = PoisonAttacker;
}

// robypark 2004/8/9 13:31
// �� ������ ĳ���� ID ���
_NewID cltPoisonEffect::GetPoisonAttacker(void)
{
	return m_PoisonAttacker;
}
