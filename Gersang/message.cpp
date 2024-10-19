#include <GSL.h>
#include <main.h>
#include <clGame.h>

#include "maindata.h"
#include <etc.h>
#include "music.h"

#include <map.h>
#include <charinterface.h>
#include "text.h"
#include <mynet-Game.h>

#include "charanimation.h"
#include <kindinfo.h>

// �ǳ� ���� ǥ���� �̸� �ؽ�Ʈ ���� ��ǥ
#ifdef _IMJIN2PLUS
	#define		NAME_TEXT_X			316
	#define		NAME_TEXT_Y			468
#else
	#define		NAME_TEXT_X			215
	#define		NAME_TEXT_Y			370
#endif

extern	_clGame					*pGame;
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_MainData			GD;									// ������ �ֵ����� 
extern	_Text 				Text;								// �ؽ�Ʈ 
extern	_MyNetGame			MyNet;
extern	_NewID 				UnderMouseID;
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern	cltCharInterface	clifChar;							// ĳ���� ���� �������̽� 
extern	SHORT 				UnderMouseCropType;
extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;			// ĳ���� �׸���


// �޽������� �ʱ�ȭ�Ѵ�. 
void InitMessage()
{
	GD.MessageIndex = 0;
	GD.Message2Delay = 0;
	GD.PlayerMessageDelay = 0;
}

// �޽����� ǥ���Ѵ�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage(char* message, SHORT p_number, SHORT effect, COLORREF rgb)
{
	if(p_number!=-1 && p_number!=UserPlayerNumber)
		return ;

	if(message==NULL)
		return ;

	if(effect==0)
       PushEffect(EFFECT_MESSAGE, 0, 0);
	else 
#ifndef _USE_HASHCODE_EFFECTSOUND
	   PushEffect(effect, 0, 0);
#else
	   HashCodePushEffect(effect, 0, 0);
#endif

	if(GD.MessageIndex<MAX_MESSAGE_NUMBER)
	{
		if(GD.MessageIndex<0 || GD.MessageIndex>=MAX_MESSAGE_NUMBER)
		{
			clGrp.Error("fds93hj22r", "ds0f37ud:%d", GD.MessageIndex);
		}
		else
		{
			if(lstrlen(message)>128)
				clGrp.Error("fdsiwjds", "dsf983ha");
			else
			{
			   strcpy(GD.Message[GD.MessageIndex],message);
			   GD.MessageColor[GD.MessageIndex] = rgb;
	           GD.MessageDelay=MAX_MESSAGE_DELAY;
	           GD.MessageIndex++;
			}
		}
	}
	else
	{
		ScrollMessage();
		if(GD.MessageIndex<0 || GD.MessageIndex>=MAX_MESSAGE_NUMBER)
		{
			clGrp.Error("fds93hj22r", "ds0f37ufsad");
		}
		else
		{
			strcpy(GD.Message[GD.MessageIndex],message);
			GD.MessageColor[GD.MessageIndex] = rgb;
			GD.MessageDelay = MAX_MESSAGE_DELAY;
			GD.MessageIndex++;
		}
	}
}

// �޽����� ǥ���Ѵ�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage2(char* message, SHORT p_number, SHORT effect)
{

	if(p_number!=-1 && p_number!=UserPlayerNumber)return ;

	if(message==NULL)return ;

	if(effect==0)
       PushEffect(EFFECT_MESSAGE, 0, 0);
	else 
#ifndef _USE_HASHCODE_EFFECTSOUND
	   PushEffect(effect, 0, 0);
#else
	   HashCodePushEffect(effect, 0, 0);
#endif

    strcpy(GD.Message2,message);
    GD.Message2Delay=MAX_MESSAGE2_DELAY;

}

// �޽����� ǥ���Ѵ�. 
void SetMessage3(char* message, SHORT effect)
{
	if(message==NULL)return ;

	if(effect==0)
       PushEffect(EFFECT_MESSAGE, 0, 0);
	else 
#ifndef _USE_HASHCODE_EFFECTSOUND
	   PushEffect(effect, 0, 0);
#else
	   HashCodePushEffect(effect, 0, 0);
#endif

    strcpy(GD.ObjectiveMessage, message);
	GD.bObjectiveMessageSwitch = TRUE;
}

// �� ���ο� ���Ǵ� ����
#define MAX_ALLOWED_LINE_WIDTH		480

// robypark 2005/1/12 13:51
// �޽����� ǥ���Ѵ�. �޽����� ���̰� �� ��쿡 ���� �������� �����ش�. p_number�� -1�̰ų� ������ ��쿡�� ǥ���Ѵ�. 
void SetMessage4(char * pString, HDC hDC, SHORT p_number/* = -1*/,  SHORT effect/* = 0*/, COLORREF rgb/* = NORMAL_TEXT_COLOR*/)
{
	SIZE	TextSize;	
	SI16	siCnt    = 0;
	SI16	siLine   = 0;
	char	szLineBuffer[MAX_MESSAGE_TEXT_LENGTH];

	if ((p_number != -1) && (p_number != UserPlayerNumber))
		return ;

	if(pString == NULL)
		return ;

	if(effect==0)
       PushEffect(EFFECT_MESSAGE, 0, 0);
	else 
#ifndef _USE_HASHCODE_EFFECTSOUND
	   PushEffect(effect, 0, 0);
#else
	   HashCodePushEffect(effect, 0, 0);
#endif

	SetBkMode(hDC, TRANSPARENT);

	HFONT hOldFont	=	(HFONT)SelectObject(hDC, pGame->pBattle->HelpFont);

	SetTextColor( hDC, RGB( 210, 210, 210 ) );
	GetTextExtentPoint32( hDC, pString, strlen( pString ), &TextSize );

	if (TextSize.cx > MAX_ALLOWED_LINE_WIDTH)
	{
		while( *pString != NULL )
		{			
			if( siLine > 10 )
				break;

			ZeroMemory( szLineBuffer, MAX_MESSAGE_TEXT_LENGTH );
			
			while( pString[siCnt] != NULL )
			{				
				// ���� ���ڰ� �ѱ��̸� �� ����Ʈ�� �о� ���δ�.
				if( UI08(pString[siCnt]) > 127 )
				{	
					GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// ����ؼ� ���ۿ� �ִ� �ؽ�Ʈ ���̸� ���Ѵ�.
					
					if( TextSize.cx >= MAX_ALLOWED_LINE_WIDTH )				break;						// �ؽ�Ʈ ���� ���� ���ۿ� ���̰� ũ�� ���� �ٷ� �ѱ��.
					
					szLineBuffer[siCnt] = pString[siCnt++];
					szLineBuffer[siCnt] = pString[siCnt++];					
				}
				else																		// Ư�� ���� �� ����
				{
					GetTextExtentPoint32( hDC, szLineBuffer, siCnt, &TextSize );			// ����ؼ� ���ۿ� �ִ� �ؽ�Ʈ ���̸� ���Ѵ�.
					
					if( TextSize.cx >= MAX_ALLOWED_LINE_WIDTH )				break;						// �ؽ�Ʈ ���� ���� ���ۿ� ���̰� ũ�� ���� �ٷ� �ѱ��.

					if( pString[siCnt] == '\t' )
					{
						pString[siCnt] = ' ';
					}
					

					if( pString[siCnt] == '\n' || pString[siCnt] == '\r' )
						if( pString[siCnt+1] == '\n' || pString[siCnt+1] == '\r' )
						{
							siCnt += 2;
							break;
						}
							

					szLineBuffer[siCnt] = pString[siCnt++];					
				}							
			}
			
			pString += siCnt;
			siCnt    = 0;		
			
			// ù��° ���ڰ� �����̸� ��ĭ�� �о� �ش�.
			if( szLineBuffer[0] == ' '  || szLineBuffer[0] == ',' || szLineBuffer[0] == '.' )
				memmove( szLineBuffer, szLineBuffer + 1, MAX_MESSAGE_TEXT_LENGTH - 1 );
			
			// �޽��� ���� ������ ũ�⸦ ����ٸ�,
			if (GD.MessageIndex >= MAX_MESSAGE_NUMBER)
			{
				// �޽������� �ϳ��� ���� �̵�
				ScrollMessage();
			}

			// ���ۿ� ����
			strcpy(GD.Message[GD.MessageIndex], szLineBuffer);
			GD.MessageColor[GD.MessageIndex] = rgb;
			GD.MessageDelay=MAX_MESSAGE_DELAY;
			GD.MessageIndex++;

			siLine++;
		}

		// �޽��� ������ ����
		GD.MessageDelay=MAX_MESSAGE_DELAY;
	}
	else
	{
		// �޽��� �ε����� �������� ���� ���ٸ�,
		if ((GD.MessageIndex < 0) || (GD.MessageIndex > MAX_MESSAGE_NUMBER))
		{
			clGrp.Error("fds93hj22r", "ds0f37ud:%d", GD.MessageIndex);
			return;
		}

		// �޽��� ���� ������ ũ�⸦ ����ٸ�,
		if (GD.MessageIndex >= MAX_MESSAGE_NUMBER)
		{
			// �޽������� �ϳ��� ���� �̵�
			ScrollMessage();
		}

		// ���ۿ� ����
		strcpy(GD.Message[GD.MessageIndex], pString);
		GD.MessageColor[GD.MessageIndex] = rgb;
		GD.MessageDelay=MAX_MESSAGE_DELAY;
		GD.MessageIndex++;
	}

	SelectObject(hDC, hOldFont);
}

void SetMessage3Off()
{
	GD.bObjectiveMessageSwitch = FALSE;
}

void ScrollMessage()
{
	int i;

	for(i=0;i<GD.MessageIndex;i++)
	{
		if(i>0)
		{
           strcpy(GD.Message[i-1], GD.Message[i]);
		   GD.MessageColor[i-1] = GD.MessageColor[i];

		}
    }
	if(GD.MessageIndex>0)GD.MessageIndex--;

}


void ShowMessage(SI32 main_message_start_x, SI32 main_message_start_y, LPDIRECTDRAWSURFACE7 surface)
{
	int i;
	SHORT index=0;
	HDC hdc;
	char* text=NULL;
	char buffer[128];
	SIZE Size;

	// ��ũ�ѵǴ� �޽����� ó�� 

	  if(GD.MessageDelay>0)
	  {
		  GD.MessageDelay--;
		  if(GD.MessageDelay==0)
		  {
			  ScrollMessage();
			  GD.MessageDelay=MAX_MESSAGE_DELAY;
		  }
	  }
	  
	  if(GD.MessageIndex > 0)
	  {
		  if(surface->GetDC(&hdc)==DD_OK)
		  {
	       SelectObject(hdc, pGame->pBattle->HelpFont);

		   
   	       SetBkMode(hdc, TRANSPARENT);
		   
	       for(i=0;i<GD.MessageIndex;i++)
		   {
			   SetTextColor( hdc, GD.MessageColor[i]);
			  TextOut(hdc, main_message_start_x, main_message_start_y + i*20, GD.Message[i], lstrlen(GD.Message[i]));
		   }
	  
		   surface->ReleaseDC(hdc);
		}
	  }
	  
	  // ���� ���� ǥ�ÿ� �޽���
	  if(GD.bObjectiveMessageSwitch)
	  {
		  if(surface->GetDC(&hdc)==DD_OK)
		  {
			  SelectObject(hdc, pGame->pBattle->HelpFont);
			  
			  SetTextColor( hdc, NORMAL_TEXT_COLOR );
			  SetBkMode(hdc, TRANSPARENT);

			  
			  GetTextExtentPoint32(hdc, GD.ObjectiveMessage, strlen(GD.ObjectiveMessage), &Size);		// Ÿ��Ʋ �۲��� ũ�⸦ �˾Ƴ���.
			  TextOut(hdc, clGrp.GetScreenXsize() - Size.cx - 30, 10, GD.ObjectiveMessage, lstrlen(GD.ObjectiveMessage));
			  
			  surface->ReleaseDC(hdc);
		  }
	  }

	  // ȭ�� �ϴܿ� ������ �޽����� ó�� 
	  if(GD.Message2Delay)
	  {
		  GD.Message2Delay--;

		  if(surface->GetDC(&hdc)==DD_OK)
		  {
			  SelectObject(hdc, pGame->pBattle->HelpFont);
			  
			  SetTextColor( hdc, NORMAL_TEXT_COLOR );
			  SetBkMode(hdc, TRANSPARENT);
			  
			  GetTextExtentPoint32(hdc, GD.Message2, strlen(GD.Message2), &Size);

			  TextOut(hdc, (clGrp.GetScreenXsize()-Size.cx)/2, NAME_TEXT_Y - 40 , GD.Message2, lstrlen(GD.Message2));
	  
		   surface->ReleaseDC(hdc);
		}

		  //Help.PutExtendHelp(GD.Message2);////////////////////////////<--------------------------
	  }

		  
	  // ���� ���� �÷��̾��� ������ ǥ���Ѵ�.
	  if(GD.PlayerMessageDelay)
	  {
		  GD.PlayerMessageDelay--;
	  

	      if(surface->GetDC(&hdc)==DD_OK)
		  {
	
		     SelectObject(hdc, pGame->pBattle->SaveInfoFont);
   	         SetBkMode(hdc, TRANSPARENT);
		   
	          for(i=0;i<MAX_MULTI_PLAYER;i++)
			  {
				  // �¶��� ����̰� 7�� �÷��̾��̸� ���� �̹Ƿ� ǥ������ �ʴ´�. 
				  if(i == MAX_MULTI_PLAYER-1)continue;

			      
				  if(GD.Player[i].PI.Country != COUNTRY_NO)
				  {

				   switch(i)
				   {
				   case 0: SetTextColor( hdc, PLAYER0_TEAM_COLOR );break;
				   case 1: SetTextColor( hdc, PLAYER1_TEAM_COLOR );break;
				   case 2: SetTextColor( hdc, PLAYER2_TEAM_COLOR );break;
				   case 3: SetTextColor( hdc, PLAYER3_TEAM_COLOR );break;
				   case 4: SetTextColor( hdc, PLAYER4_TEAM_COLOR );break;
				   case 5: SetTextColor( hdc, PLAYER5_TEAM_COLOR );break;
				   case 6: SetTextColor( hdc, PLAYER6_TEAM_COLOR );break;
				   case 7: SetTextColor( hdc, PLAYER7_TEAM_COLOR );break;
				   }

				   if(i == UserPlayerNumber)
				   {
					   text = Text.Player.Get();
				   }
				   else if(IsFriendToByPlayerNumber(UserPlayerNumber, i)==TRUE)
				   {
					   text = Text.Ally.Get();
				   }
				   else 
				   {
					   text = Text.Enemy.Get();
				   }

				   if(MyNet.PlayersdpId.dpId[i])
				      wsprintf(buffer, "%s -[%s]", text, MyNet.PlayersName.Name[i]);
				   else
					  wsprintf(buffer, "%s -[%s]", text, Text.Computer.Get());


			       TextOut(hdc, 10, 200+index*18, buffer, lstrlen(buffer));
				   index++;
			   }
			  }

			  
		     surface->ReleaseDC(hdc);

		  }
	  }

}


// ���콺 �ؿ� �ִ� ��� ������ �����ش�.
void ShowCropInfo(LPDIRECTDRAWSURFACE7 surface)
{
	HDC hdc;
	char *text=NULL;
//	char buffer[128];

	if(UnderMouseID.IsEmpty() == FALSE)
	{
		if(surface->GetDC(&hdc) == DD_OK)
		{
	        SelectObject(hdc, pGame->pBattle->HelpFont);

   	        SetBkMode(hdc, TRANSPARENT);
		   

			// �÷��̾� �̰� ù���� ĳ�����̸� ���ΰ��̱� ������ ���ΰ��� �̸��� ǥ���Ѵ�. 
			if(ISPLAYER(MyNet.PlayersdpId.dpId[ clifChar.GetPlayerNumber(UnderMouseID.ID) ] ) == TRUE  && clifChar.GetOnlineID(UnderMouseID.ID) == 0)
			{
				text = MyNet.PlayersName.Name[clifChar.GetPlayerNumber(UnderMouseID.ID)];
			}
			else
			{
				text = KI[clifChar.GetKind(UnderMouseID.ID)].m_pszName;
			}
			

/*			if(clifChar.GetKind(UnderMouseID.ID) == KIND_FLAGMAN)
			{
				wsprintf(buffer, "[%s]-%s", MyNet.PlayersName.Name[clifChar.GetOriginalPlayerNumber(UnderMouseID.ID)], text);
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, NAME_TEXT_X+1, NAME_TEXT_Y, buffer, lstrlen(buffer));
		        SetTextColor( hdc, RGB( 180, 180, 0 ) );
				TextOut(hdc, NAME_TEXT_X, NAME_TEXT_Y, buffer, lstrlen(buffer));
			}
			else
*/			{
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, NAME_TEXT_X+1, NAME_TEXT_Y, text, lstrlen(text));
		        SetTextColor( hdc, RGB( 180, 180, 0 ) );
				TextOut(hdc, NAME_TEXT_X, NAME_TEXT_Y, text, lstrlen(text));
			}
  
	        surface->ReleaseDC(hdc);
		}
	}
	else if(UnderMouseCropType)
	{
	
		if(surface->GetDC(&hdc)==DD_OK)
		{
	        switch(UnderMouseCropType)
			{
	          case CROP_TYPE_RICE:   text=Text.Rice.Get();         break;
		   
	          case CROP_TYPE_POTATO: text=Text.Potato.Get();       break;

	          case CROP_TYPE_TREE:   text=Text.Tree.Get();         break;

			  case CROP_TYPE_BAMBOO: text=Text.Bamboo.Get();       break;

			  case CROP_TYPE_TEMPLE: text=Text.TempleEvent.Get();  break;

			  case CROP_TYPE_SIGN1:  text=Text.Sign.Get();         break;

	          default:               text=NULL;         break;
			}

			if(text)
			{
				SelectObject(hdc, pGame->pBattle->HelpFont);
   				SetBkMode(hdc, TRANSPARENT);
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, NAME_TEXT_X+1, NAME_TEXT_Y, text, lstrlen(text));
				SetTextColor( hdc, RGB( 180, 180, 0 ) );
				TextOut(hdc, NAME_TEXT_X, NAME_TEXT_Y, text, lstrlen(text));
			}


	        surface->ReleaseDC(hdc);
		}
	}


}

// ���� �ð��� �����Ѵ�.
void PassPlayTime()
{
	//���� �ð��� ���Ѵ�.
	GD.PlayTime_CurrentTime = GD.Frame * DEFAULT_MULTI_PLAY_DELAY_TIME;
}

// ���� �ð��� ǥ���Ѵ�. 
void ShowPlayTime(SI32 x, SI32 y, LPDIRECTDRAWSURFACE7 surface)
{
	int lasttime;
	char buffer[128];

	//���� �ð��� ���Ѵ�.
	lasttime = RETREAT_PERMIT_CLOCK - GD.PlayTime_CurrentTime;
	
	if(lasttime >= 0)
	{
		wsprintf(buffer, "%d", lasttime/1000);
		
		SI32 percent = (GD.PlayTime_CurrentTime % 1000) / 10;
		
		SI32 xsize = 22  * percent / 100; 
		SI32 ysize = 12  * percent / 100; 
		
		RECT destrect = {x - xsize, y - ysize, x+xsize, y + ysize};
		
		// ����Ѵ�. 
		DrawTextInRect(surface, pGame->pBattle->SaveInfoFont, buffer, destrect);
	}
}

BOOL GetPlayTime()
{
	int lasttime;
	lasttime = RETREAT_PERMIT_CLOCK - GD.PlayTime_CurrentTime;

	if(lasttime >= 0)
		return TRUE;
	else 
		return FALSE;
}




