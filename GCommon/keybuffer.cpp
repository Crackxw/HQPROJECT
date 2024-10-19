//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <clGame.h>


#include "..\Gersang\maindata.h"

extern _clGame* pGame;

extern	_MainData		GD;
extern	SHORT			GameMenuShowSwitch;   // 게임중에 메뉴를 보여주는지 여부를 알려준다. 
extern	SHORT			GamePauseSwitch;      // 게임이 잠시 중단되었는가?

_clKeyBuffer::_clKeyBuffer()
{
	KeyBufferIndex = 0;
}


void _clKeyBuffer::SetAKey(WPARAM key, BOOL mode)
{
	if(key >= 0 && key < 256)
	   AKey[key] = mode;
	else
	{
		clGrp.Error("Fsd2134re", "F07222");
	}
}



BOOL _clKeyBuffer::GetAKey(WPARAM key)
{
	if(key >= 0 && key < 256)
	   return AKey[key];
	else
	{
		clGrp.Error("Fsd2134re", "F07fd222");
		return FALSE;
	}
}

// 키버퍼를 지운다. 
void _clKeyBuffer::InitKeyBuffer()
{
   KeyBufferIndex=0;
}

void _clKeyBuffer::PushKeyBuffer(SHORT leftpressswitch, SHORT rightpressswitch, SHORT mousex, SHORT mousey, DWORD wparam, char moveswitch)
{
	if(pGame->IsOnlineWorld() == TRUE)
	{
	}
	else
	{
		
		// 게임 중에만 작동한다. 
		// 프레임이 10이하면 아직 게임이 제재로 실행되지 않는 것이다. 
		if(GD.Frame<10)return ;
		
		// 게임 메뉴가 불러와진 상태면 리턴
		if(GameMenuShowSwitch==TRUE)return ;
	}

	// 게임이 중단된 상태에에는 리턴 
	if(GamePauseSwitch==TRUE)return ;

	if(KeyBufferIndex>=MAX_KEY_BUFFER)
	{
		return ;
	}

	// 마우스 이동 메시지면, 
	if(KeyBufferIndex>0 && leftpressswitch==-1 && rightpressswitch==-1 && wparam==0)	
	{
		// 바로 앞의 메시지를 확인하여 마우스 이동메시지면 교체한다. 
		if(KeyBuffer[KeyBufferIndex-1].LeftPressSwitch==-1 
		&& KeyBuffer[KeyBufferIndex-1].RightPressSwitch==-1
		&& KeyBuffer[KeyBufferIndex-1].wParam==0)
		{
			KeyBuffer[KeyBufferIndex-1].Mouse_X=mousex;
	        KeyBuffer[KeyBufferIndex-1].Mouse_Y=mousey;
			KeyBuffer[KeyBufferIndex-1].wParam=wparam;
	        KeyBuffer[KeyBufferIndex-1].MoveSwitch=moveswitch;
			return ;
		}
	}

	KeyBuffer[KeyBufferIndex].LeftPressSwitch=leftpressswitch;
    KeyBuffer[KeyBufferIndex].RightPressSwitch=rightpressswitch;
	KeyBuffer[KeyBufferIndex].Mouse_X=mousex;
	KeyBuffer[KeyBufferIndex].Mouse_Y=mousey;
	KeyBuffer[KeyBufferIndex].wParam=wparam;
    KeyBuffer[KeyBufferIndex].MoveSwitch=moveswitch;

	KeyBufferIndex++;

}

BOOL _clKeyBuffer::PopKeyBuffer(SHORT& leftpressswitch, SHORT& rightpressswitch, SHORT& mousex, SHORT& mousey, DWORD& wparam, char& moveswitch)
{

	int i;

	if(KeyBufferIndex)
	{
	   leftpressswitch=KeyBuffer[0].LeftPressSwitch;
	   rightpressswitch=KeyBuffer[0].RightPressSwitch;
	   mousex=KeyBuffer[0].Mouse_X;
	   mousey=KeyBuffer[0].Mouse_Y;
	   wparam=KeyBuffer[0].wParam;
	   moveswitch=KeyBuffer[0].MoveSwitch;

	   for(i=0;i<(KeyBufferIndex-1);i++)
	   {
		   memcpy(&KeyBuffer[i], &KeyBuffer[i+1], sizeof(_KeyBuffer));
	   }
	   KeyBufferIndex--;
	   return TRUE;
	}
	return FALSE;
}