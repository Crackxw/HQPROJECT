//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>
#include <clGame.h>


#include "..\Gersang\maindata.h"

extern _clGame* pGame;

extern	_MainData		GD;
extern	SHORT			GameMenuShowSwitch;   // �����߿� �޴��� �����ִ��� ���θ� �˷��ش�. 
extern	SHORT			GamePauseSwitch;      // ������ ��� �ߴܵǾ��°�?

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

// Ű���۸� �����. 
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
		
		// ���� �߿��� �۵��Ѵ�. 
		// �������� 10���ϸ� ���� ������ ����� ������� �ʴ� ���̴�. 
		if(GD.Frame<10)return ;
		
		// ���� �޴��� �ҷ����� ���¸� ����
		if(GameMenuShowSwitch==TRUE)return ;
	}

	// ������ �ߴܵ� ���¿����� ���� 
	if(GamePauseSwitch==TRUE)return ;

	if(KeyBufferIndex>=MAX_KEY_BUFFER)
	{
		return ;
	}

	// ���콺 �̵� �޽�����, 
	if(KeyBufferIndex>0 && leftpressswitch==-1 && rightpressswitch==-1 && wparam==0)	
	{
		// �ٷ� ���� �޽����� Ȯ���Ͽ� ���콺 �̵��޽����� ��ü�Ѵ�. 
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