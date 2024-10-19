#include <GSL.h>
#include <main.h>
#include "..\Gersang\rain.h"
#include <colortable.h>
#include <etc.h>
#include <CharOrg.h>
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <myfile.h>
#include "..\Gersang\DirectX.h"

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 
extern	unsigned char				DarkTable[][256];					// 어두운 단계 
extern	CCharFileLoadManager		g_FileLoadManager;					// 조합형 캐릭터
extern	SI32						UserPlayerNumber;					// 사용자 관련 


cltPoisonEffect::cltPoisonEffect():
	m_bPoisonSwitch(FALSE),
	m_siPoisonDelay(0),
	m_siPoisonPower(0)
{
	// robypark 2004/8/9 13:43
	// 해당 캐릭터에게 독공격을 하여 중독시킨 캐릭터 ID 초기화
	m_PoisonAttacker.ID = -1;
	m_PoisonAttacker.Unique = 0;
}


void cltPoisonEffect::Set(BOOL mode, SI32 delay, SI32 poisonpower)
{
	SetPoisonSwitch(mode);
	SetPoisonDelay(delay);
	SetPoisonPower(poisonpower);
}


// 중독된 모습을 그려준다. 
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
	// 중독과 관련. 
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
	// 중독 지속시간이 끝난 경우 중독효과 이미지를 그려주어서는 안된다.
	// 중독효과 스위치를 FALSE시킨다.
	m_bPoisonSwitch = FALSE;

	return 0;
}

// robypark 2004/8/9 13:31
// 독 공격한 캐릭터 ID 설정
void cltPoisonEffect::SetPoisonAttacker(_NewID PoisonAttacker)
{
	m_PoisonAttacker = PoisonAttacker;
}

// robypark 2004/8/9 13:31
// 독 공격한 캐릭터 ID 얻기
_NewID cltPoisonEffect::GetPoisonAttacker(void)
{
	return m_PoisonAttacker;
}
