//
#include <GSL.h>
#include <Main.h>
#include <OnlineKeyboard.h>

extern BOOL bActive;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineKeyboard::OnlineKeyboard()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	초기화를 수행한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineKeyboard::Init()
{
	SI32	i;

	// 윈도우 키 버퍼에 남아있는 쓰레기 값들을 모두 제거한다.
	for(i = 0; i < 256; i++)	GetAsyncKeyState(i);
		
	ZeroMemory(siKeyState, sizeof(siKeyState));
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	액션.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineKeyboard::Action()
{
	SI32	i;
	UI16	TempKey;
	UI16	TempKeyDelay;

	for(i = 0; i < 256; ++i)
	{
		TempKey      = GetAsyncKeyState(i);
		TempKeyDelay = (siKeyState[i] & 0xffff0000) >> 16;

		++TempKeyDelay;
		if((TempKey & 0x8000) == 0) TempKeyDelay = 0;

		TempKey = TempKey & 0x8000;
		siKeyState[i] = ((UI32)TempKeyDelay << 16) | (UI32)TempKey;
		if((TempKeyDelay & 0x000f) == 1) siKeyState[i] = siKeyState[i] | 0x00000080;		
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	키의 상태를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI16	OnlineKeyboard::GetKeyStatus(UI08 uiVKey)
{
	if (!bActive) return FALSE;
	return (SI16)(siKeyState[uiVKey] & 0xffff);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	키의 상태를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	OnlineKeyboard::SetKeyStatus(UI08 uiVKey, SI16 siState)
{
	siKeyState[uiVKey]	=	(UI32)siState;
}
