// 파일명 : OnlineWorld-SelectCharacter.cpp
// 담당자 : 정진욱
// 캐릭터 선택 화면

#include <GSL.h>

#include <clGame.h>
#include <Main.h>
#include <Mouse.h>
#include "charanimation.h"
#include <kindinfo.h>
#include <Player.h>

#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "..\Gersang\directx.h"
#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineHelp.h"
#include "OnlineFont.h"
#include "OnlineKeyboard.h"
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include "OnlineText.h"
#include "OnlineMsgBox.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include "OnlineNotice.h"
#include "OnlineETC.h"
#include "OnlineMFGS.h"
#include "OnlineVersion.h"
#include "OnlineTradeParser.h"

extern	_InputDevice			IpD;
extern	_KindInfo				KI[];								// 캐릭터 종류별 데이터 


enum
{   
	ON_SELECT_CHAR_INIT				= 0,
	ON_SELECT_CHAR_POLL,
	ON_SELECT_CHAR_DELETE
};


extern _clGame* pGame;


//----------------------------------------------------------------------------------------------------------------
//	설명	:	초기화
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::InitSelectCharacter()
{
	SI16	i;
	LOGIN::sCharInfo *pCharInfo = NULL;
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_button.Spr", pOnlineLogInMgr->m_SelectCharData.m_ImgButtonBaseSpr );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_shadow.Spr", pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBase ); 

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_chosunmale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[0] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_chosunfemale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[1] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_japanmale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[2] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_japanfemale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[3] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_taiwanmale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[4] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_taiwanfemale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[5] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_chinamale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[6] );
	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_chinafemale.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[7] );

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charselect_box.Spr",pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	pOnlineLogInMgr->m_SelectCharData.siStatus	 = ON_SELECT_CHAR_INIT;
	pOnlineLogInMgr->m_SelectCharData.siMaxCharNum = 0;																// 총 만든 케릭터 수
	pOnlineLogInMgr->m_SelectCharData.siNowChar	 = -1;																// 현재 선택한 케리터
	pOnlineLogInMgr->m_SelectCharData.pMyChar	 = new SMyChar[LOGIN::CHAR_MAX_NUM];

	for( i = 0; i < LOGIN::CHAR_MAX_NUM; i++ )
	{
		pCharInfo = pMyData->GetMyCharacters(i+1);

		if( pCharInfo == NULL )		continue;

		switch(pCharInfo->kind)
		{
			case 67:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 0;		break;
			case 323:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 1;		break;
			case 8771:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 2;		break;
			case 9027:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 3;		break;
			case 10307:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 4;		break;
			case 10563:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 5;		break;
			case 12867:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 6;		break;
			case 13123:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 7;		break;
			default:		pOnlineLogInMgr->m_SelectCharData.siSelectKind[i] = 0;		break;

		}
			   
	}

	pOnlineLogInMgr->m_SelectCharData.BSelect[0].Create(32,  78, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Xsize, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_SelectCharData.BSelect[1].Create(32, 249, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Xsize, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_SelectCharData.BSelect[2].Create(32, 420, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Xsize, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox.Header.Ysize, "", BUTTON_PUT_LEFT, TRUE );

	for( i = 0;	i < LOGIN::CHAR_MAX_NUM ; i++)
	{
		pCharInfo = pMyData->GetMyCharacters( i+1 );

		

		// siMaxCharNum를 알아 본다.
		if(pCharInfo != NULL && pCharInfo->slot != 0)
		{
			pOnlineLogInMgr->m_SelectCharData.siMaxCharNum++;
			//pOnlineLogInMgr->m_SelectCharData.BSelect[i].SetAction( TRUE );
		}
		//else
			//pOnlineLogInMgr->m_SelectCharData.BSelect[i].SetAction( FALSE );
	}

	// 현재 선택한 케릭터가 없고 
	if(pOnlineLogInMgr->m_SelectCharData.siNowChar < 0 && pOnlineLogInMgr->m_SelectCharData.siMaxCharNum != 0)
	{
		for(i = 0; i < LOGIN::CHAR_MAX_NUM; i++)
		{
			pCharInfo = pMyData->GetMyCharacters(i+1);
			if(pCharInfo)
			{
				pOnlineLogInMgr->m_SelectCharData.siNowChar = i;
				pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
				pOnlineLogInMgr->m_SelectCharData.uiSlot = pCharInfo->slot;
				break;
			}
		}
	}

	pOnlineLogInMgr->m_SelectCharData.OkButton.Create( 561, 367, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get( ON_TEXT_LOGIN ), BUTTON_PUT_LEFT, FALSE );
	pOnlineLogInMgr->m_SelectCharData.BNewChar.Create( 561, 412, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get( ON_TEXT_NEWCHAR ), BUTTON_PUT_LEFT, FALSE );
	pOnlineLogInMgr->m_SelectCharData.BDelChar.Create( 561, 457, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get( ON_TEXT_DELCHAR ), BUTTON_PUT_LEFT, FALSE );
	pOnlineLogInMgr->m_SelectCharData.BExit.Create   ( 561, 502, pOnlineLogInMgr->LButtonImage.Header.Xsize, pOnlineLogInMgr->LButtonImage.Header.Ysize, pOnlineText->Get( ON_TEXT_BACK ), BUTTON_PUT_LEFT, FALSE );

	pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel = new OnlineMsgBox;
	pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->Init( this,TRUE );

	pOnlineLogInMgr->m_SelectCharData.dwAnyTime		= timeGetTime();
	pOnlineLogInMgr->m_SelectCharData.bButtonAction	= TRUE;	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	선택 화면
//----------------------------------------------------------------------------------------------------------------
SI08 cltOnlineWorld::SelectCharacter()
{
	UI08	i;	
	char	TempBuffer[256];
	XSPR	*Character = NULL;
	LOGIN::sCharInfo *pCharInfo = NULL;

	// 헬프를 초기화 한다.(잔상이 남게 하지 않는다. 계속 호출하는데 있어야 한단다.)
	pOnlineHelp->Initialize();	

	SI16 siDelRet = pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->Process();

	switch(pOnlineLogInMgr->m_SelectCharData.siStatus)
	{
	case ON_SELECT_CHAR_INIT:
		{
			pOnlineLogInMgr->m_SelectCharData.siStatus = ON_SELECT_CHAR_POLL;
			::SetFocus(pGame->Hwnd);
		}
		break;
	case ON_SELECT_CHAR_POLL:
		{
			if(pOnlineMsgBox->IsActive())
				pOnlineLogInMgr->m_SelectCharData.OkButton.SetAction(FALSE);
			
			if(pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->IsActive())
			{
				pOnlineLogInMgr->m_SelectCharData.bButtonAction = FALSE;
			}

			// 캐릭터 삭제
			if(!pOnlineMsgBox->IsActive())
			{
				pCharInfo = pMyData->GetMyCharacters(pOnlineLogInMgr->m_SelectCharData.siNowChar + 1);
				if(	siDelRet == ON_MSGBOX_RETURN_OK && pCharInfo->slot )
				{
					pOnlineLogInMgr->m_SelectCharData.uiDelSlot = pCharInfo->slot;
					pOnlineClient->SendDeleteCharacter(pCharInfo->slot);
					pOnlineLogInMgr->m_SelectCharData.siStatus = ON_SELECT_CHAR_DELETE; 
				}
				else if( siDelRet == ON_MSGBOX_RETURN_CANCEL )
				{
					pOnlineLogInMgr->m_SelectCharData.bButtonAction = TRUE;
				}
			}

			// 캐릭터 선택
			for( i = 0; i < LOGIN::CHAR_MAX_NUM; ++i)
			{
				if( pOnlineLogInMgr->m_SelectCharData.BSelect[i].Process( pOnlineLogInMgr->bMouseDown ) && !pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->IsActive())
				{
					pCharInfo = pMyData->GetMyCharacters(i+1);
					if( pCharInfo )
					{
						pOnlineLogInMgr->m_SelectCharData.siNowChar = i;
						pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
					}
				}
			}

			// 케릭터 만들기 버튼
			if( pOnlineLogInMgr->m_SelectCharData.BNewChar.Process(pOnlineLogInMgr->bMouseDown) == TRUE )	
			{
				pOnlineLogInMgr->m_SelectCharData.bButtonAction = FALSE;
				return ON_RETURN_CREATE;
			}
			// 지우기 버튼
			else if( pOnlineLogInMgr->m_SelectCharData.BDelChar.Process(pOnlineLogInMgr->bMouseDown) == TRUE )
			{	
				pOnlineLogInMgr->m_SelectCharData.bButtonAction = FALSE;
				char TempBuffer[256];
				pCharInfo = pMyData->GetMyCharacters(pOnlineLogInMgr->m_SelectCharData.siNowChar+1);
				sprintf(TempBuffer, pOnlineText->Get(ON_TEXT_ASKDELCHAR), pCharInfo->name);
				pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->SetMsg( ON_MSGBOX_TYPE_OKCANCEL, pOnlineText->Get(ON_TEXT_DELCHAR), TempBuffer );
				break;
			}
			// 나가기 버튼을 눌렸나??
			else if( pOnlineLogInMgr->m_SelectCharData.BExit.Process(pOnlineLogInMgr->bMouseDown) == TRUE )
			{
				return ON_RETURN_EXIT;
			}
			// OK 버튼
			else if((pOnlineLogInMgr->m_SelectCharData.OkButton.Process(pOnlineLogInMgr->bMouseDown)) || LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_RETURN)))
			{
				pCharInfo = pMyData->GetMyCharacters(pOnlineLogInMgr->m_SelectCharData.siNowChar+1);
				if( pOnlineLogInMgr->m_SelectCharData.siMaxCharNum != 0 && pCharInfo)
				{
					if(pCharInfo->slot >= 1 && pCharInfo->slot <= LOGIN::CHAR_MAX_NUM)
					{
						pMyData->SetSelectMyCharacterSlot( pCharInfo->slot );
						return ON_RETURN_OK;
					}
				}
			}
			
			// 버튼 활성비활성화
			if(pOnlineLogInMgr->m_SelectCharData.bButtonAction)
			{
				// 케릭터를 3명을 만들었으면 더이상 만들지 못함
				if(pOnlineLogInMgr->m_SelectCharData.siMaxCharNum == LOGIN::CHAR_MAX_NUM)
					pOnlineLogInMgr->m_SelectCharData.BNewChar.SetAction(FALSE);
				else
					pOnlineLogInMgr->m_SelectCharData.BNewChar.SetAction(TRUE);
				
				// 케릭터가 없다면 지울수 없다.
				if(  pOnlineLogInMgr->m_SelectCharData.siMaxCharNum == 0 )
				{
					pOnlineLogInMgr->m_SelectCharData.OkButton.SetAction(FALSE);
					pOnlineLogInMgr->m_SelectCharData.BDelChar.SetAction(FALSE);
				}
				else
				{
					pCharInfo = pMyData->GetMyCharacters(pOnlineLogInMgr->m_SelectCharData.siNowChar+1);
					if(!pCharInfo)
					{
						pOnlineLogInMgr->m_SelectCharData.OkButton.SetAction(FALSE);
						pOnlineLogInMgr->m_SelectCharData.BDelChar.SetAction(FALSE);
					}
					else
					{
						pOnlineLogInMgr->m_SelectCharData.OkButton.SetAction(TRUE);
						pOnlineLogInMgr->m_SelectCharData.BDelChar.SetAction(TRUE);
					}
				}
				
				pOnlineLogInMgr->m_SelectCharData.BExit.SetAction(TRUE);
			}
			else
			{
				pOnlineLogInMgr->m_SelectCharData.OkButton.SetAction(FALSE);
				pOnlineLogInMgr->m_SelectCharData.BNewChar.SetAction(FALSE);
				pOnlineLogInMgr->m_SelectCharData.BDelChar.SetAction(FALSE);
				pOnlineLogInMgr->m_SelectCharData.BExit.SetAction(FALSE);
			}
			
			if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_ESCAPE)))
			{
				return ON_RETURN_EXIT;
			}
			else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_DOWN)))
			{
				SI16 NextChar = pOnlineLogInMgr->m_SelectCharData.siNowChar + 1;
				if(NextChar > 2)		NextChar = 2;

				pCharInfo = pMyData->GetMyCharacters(NextChar+1);
				if( pCharInfo )
				{
					pOnlineLogInMgr->m_SelectCharData.siNowChar = NextChar;
					pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
				}
				else if(NextChar == 1)
				{
					NextChar++;
					pCharInfo = pMyData->GetMyCharacters(NextChar+1);
					if( pCharInfo )
					{
						pOnlineLogInMgr->m_SelectCharData.siNowChar = NextChar;
						pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
					}
				}
			}
			else if(LOBYTE(pOnlineKeyboard->GetKeyStatus(VK_UP)))
			{
				SI16 NextChar = pOnlineLogInMgr->m_SelectCharData.siNowChar - 1;
				if(NextChar < 0)		NextChar = 0;

				pCharInfo = pMyData->GetMyCharacters(NextChar+1);	
				if( pCharInfo )
				{
					pOnlineLogInMgr->m_SelectCharData.siNowChar = NextChar;
					pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
				}
				else if(NextChar == 1)
				{
					NextChar--;
					pCharInfo = pMyData->GetMyCharacters(NextChar+1);
					if( pCharInfo )
					{
						pOnlineLogInMgr->m_SelectCharData.siNowChar = NextChar;
						pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
					}
				}
			}
		}
		break;
	case ON_SELECT_CHAR_DELETE:
		{
			LOGIN::sResponseDelChar *pResDelCharMsg = (LOGIN::sResponseDelChar*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_DEL_CHAR);
			
			if(!pResDelCharMsg)			break;
			
			if(pResDelCharMsg->responseKind == LOGIN::OK)
			{			
				pMyData->DeleteCharacter( pOnlineLogInMgr->m_SelectCharData.uiDelSlot );
				pOnlineLogInMgr->m_SelectCharData.siMaxCharNum--;

				if( pOnlineLogInMgr->m_SelectCharData.siMaxCharNum != 0 )
				{
					for( i = 0 ; i < LOGIN::CHAR_MAX_NUM; i++)
					{
						pCharInfo = pMyData->GetMyCharacters(i+1);
						if( pCharInfo != NULL )
							pOnlineLogInMgr->m_SelectCharData.BSelect[i].SetAction( TRUE );
						else
							pOnlineLogInMgr->m_SelectCharData.BSelect[i].SetAction( FALSE );
					}

					for( i = 0 ; i < LOGIN::CHAR_MAX_NUM; i++)
					{
						pCharInfo = pMyData->GetMyCharacters(i+1);
						if( pCharInfo != NULL )
						{
							pOnlineLogInMgr->m_SelectCharData.siKind = pCharInfo->kind;
							pOnlineLogInMgr->m_SelectCharData.uiSlot = pCharInfo->slot;
							pOnlineLogInMgr->m_SelectCharData.siNowChar = i;
							break;
						}
					}
				}
				else
				{
					pOnlineLogInMgr->m_SelectCharData.uiSlot = 0;
					pOnlineLogInMgr->m_SelectCharData.BSelect[pOnlineLogInMgr->m_SelectCharData.siNowChar].SetAction( FALSE );
					pOnlineLogInMgr->m_SelectCharData.siNowChar = -1;
				}
			}
			else
			{
				sprintf(TempBuffer, pOnlineText->Get(ON_TEXT_FAIL), pOnlineLogInMgr->m_SelectCharData.uiSlot);
				pOnlineLoginMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_DELCHAR), TempBuffer);
			}
			pOnlineLogInMgr->m_SelectCharData.bButtonAction = TRUE;
			pOnlineLogInMgr->m_SelectCharData.siStatus = ON_SELECT_CHAR_POLL;
		}
		break;
	}

	return ON_RETURN_NONE;
}


VOID	cltOnlineWorld::DrawSelectCharacter()
{
	HDC		hdc;
	LOGIN::sCharInfo *pCharInfo = NULL;

	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{	
		clGrp.PutSpriteT(32,79,pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBase.Header.Xsize, pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBase.Header.Ysize,pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBase.Image);
		clGrp.PutSpriteT(544,360,pOnlineLogInMgr->m_SelectCharData.m_ImgButtonBaseSpr.Header.Xsize,pOnlineLogInMgr->m_SelectCharData.m_ImgButtonBaseSpr.Header.Ysize,pOnlineLogInMgr->m_SelectCharData.m_ImgButtonBaseSpr.Image);


		pOnlineLogInMgr->m_SelectCharData.OkButton.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
		pOnlineLogInMgr->m_SelectCharData.BExit.Put	(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
		pOnlineLogInMgr->m_SelectCharData.BNewChar.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );
		pOnlineLogInMgr->m_SelectCharData.BDelChar.Put(&pOnlineLogInMgr->LButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE );


		for( SI16 i = 0 ; i < LOGIN::CHAR_MAX_NUM; i++ )
		{
			if( i == pOnlineLogInMgr->m_SelectCharData.siNowChar)
				pOnlineLogInMgr->m_SelectCharData.BSelect[i].Put(&pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox, 0, 0, 0, BUTTON_PUT_NOMOVE );
			else
				pOnlineLogInMgr->m_SelectCharData.BSelect[i].Put(&pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox, 0, 0, 0, BUTTON_PUT_NOMOVE );

			pCharInfo = pMyData->GetMyCharacters(i+1);

			if(pCharInfo)
			{
				BOOL bSelect;
				XSPR *pCharSelImage = &pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[pOnlineLogInMgr->m_SelectCharData.siSelectKind[i]];
				if(i == pOnlineLogInMgr->m_SelectCharData.siNowChar)	bSelect = TRUE;
				else													bSelect = FALSE;

				clGrp.PutSpriteT(32, 48 + (171 *i), pCharSelImage->Header.Xsize, pCharSelImage->Header.Ysize,
					&pCharSelImage->Image[pCharSelImage->Header.Start[bSelect]]);
			}
		}
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}	

	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		// 투명 모드 설정.
		SetBkMode(hdc, TRANSPARENT);				
		SelectObject( hdc, pOnlineFonts->GetFont( ON_FONT_LOGIN2 ) );

		SetTextColor( hdc, RGB( 220, 176, 106 ) );

		pOnlineLogInMgr->m_SelectCharData.OkButton.Put( hdc , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_SelectCharData.BExit.Put( hdc , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_SelectCharData.BNewChar.Put( hdc , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_SelectCharData.BDelChar.Put( hdc , (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));


		pOnlineMegaText->DrawTextCenter(hdc, 1, 0, 318, 34, pOnlineText->Get( ON_TEXT_SELECTCHAR ), RGB(10, 10, 10));
		pOnlineMegaText->DrawTextCenter(hdc, 0, 0, 318, 34, pOnlineText->Get( ON_TEXT_SELECTCHAR ), RGB(250, 250, 250));


		char szTempTemp[512], temp[512];
		for(SI16 i = 0 ; i < LOGIN::CHAR_MAX_NUM; i++ )
		{
			pCharInfo = pMyData->GetMyCharacters(i+1);
			if(pCharInfo)
			{
				ZeroMemory( szTempTemp, sizeof( szTempTemp ) );

				//sprintf( szTempTemp,pOnlineText->Get(ON_TEXT_ITEMNAME), pCharInfo->name );	
				pOnlineMegaText->DrawTextCenter( hdc, 211, 116 - 1 + (i *171) , 112, 15 -1, pCharInfo->name, RGB( 10, 10, 10 ) );
				pOnlineMegaText->DrawTextCenter( hdc, 211, 116  + (i *171) , 112, 15, pCharInfo->name, RGB(250, 250, 250) );

				if(pCharInfo->level == 0)
					sprintf(szTempTemp,"LV.%d",1);
				else
					sprintf(szTempTemp,"LV.%d",pCharInfo->level);
				pOnlineMegaText->DrawTextCenter( hdc, 356, 116 - 1 + (i *171) , 55, 15 -1, szTempTemp, RGB(10, 10, 10));
				pOnlineMegaText->DrawTextCenter( hdc, 356, 116 + (i *171) , 55, 15 , szTempTemp, RGB(250, 250, 250));
			 
				MakeMoneyText(pCharInfo->mnMoney, (char*)temp);
				sprintf(szTempTemp,"%s : %s",pOnlineText->Get(8000521),temp);
				pOnlineMegaText->DrawTextCenter( hdc, 223, 146 - 1 + (i *171) , 191, 14 -1, szTempTemp, RGB(10, 10, 10));
				pOnlineMegaText->DrawTextCenter( hdc, 223, 146 + (i *171) , 191, 14, szTempTemp, RGB(250, 250, 250));
				
				UI32 uiTradeGrade = pOnlineTradeParser->CheckCreditForLevel(pCharInfo->credit);
				sprintf(szTempTemp,"%s.%d",pOnlineText->Get(1100031),uiTradeGrade);
				
				pOnlineMegaText->DrawTextCenter( hdc, 223, 171 - 1 + (i *171) , 191, 14 -1, szTempTemp, RGB(10, 10, 10));
				pOnlineMegaText->DrawTextCenter( hdc, 223, 171 + (i *171) , 191, 14, szTempTemp, RGB(250, 250, 250));

			}
		}

		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}	

	pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->Draw(GETSCREENSURFACE(this));	
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	해제
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineWorld::FreeSelectCharacter()
{
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectCharData.m_ImgButtonBaseSpr);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBase);
	clGrp.FreeXspr(pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelectBox);
	for(int i = 0 ; i < 8 ; i ++)
		clGrp.FreeXspr(pOnlineLogInMgr->m_SelectCharData.m_ImgCharSelect[i]);

	if( pOnlineLogInMgr->m_SelectCharData.pMyChar )
	{
		delete []pOnlineLogInMgr->m_SelectCharData.pMyChar;
		pOnlineLogInMgr->m_SelectCharData.pMyChar = NULL;
	}

	if( pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel )
	{		
		pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel->Initialize();
		delete pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel;
		pOnlineLogInMgr->m_SelectCharData.pOnlineMsgBoxDel = NULL;
	}
}
