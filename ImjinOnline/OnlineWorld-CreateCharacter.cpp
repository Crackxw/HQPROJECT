
// 파일명 : OnlineWorld-CreateCharacter.cpp
// 담당자 : 정진욱
// 캐릭터 생성 화면

#include <GSL.h>

#include <Main.h>
#include <Mouse.h>
#include <clGame.h>

#include "..\Gersang\directx.h"
#include "OnlineWorld.h"
#include "OnlineMegaText.h"
#include "OnlineFont.h"
#include "OnlineClient.h"
#include "OnlineText.h"
#include "OnlineMsgBox.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "OnlineVersion.h"
#include <Common.h>

#include "bindJXFile.h"
#include "OnlineCommonParser.h"


#define ON_SELECTCHAR_MAX_CHAR				2
#define ON_SELECTCHAR_ANI_DELAY				9
#define BYTECOLOR							1

#define SISTRA								15
#define SIDEXT								15
#define SIHP2								15
#define SIINTELI							 5
#define COUNT								 0
#define TEMP								 0

#define ON_CREATE_CHAR_INIT					0
#define ON_CREATE_CHAR_SCROLL_DOWN			1
#define ON_CREATE_CHAR_POLL					2
#define ON_CREATE_CHAR_CREATE				3

extern _clGame* pGame;
extern _InputDevice				IpD;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 만드는 초기화 함수.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::InitCreateCharacter()
{	
	CHAR	tempSprBuf[100];

	ZeroMemory(m_szCharTextList, 10 * 256);

	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\charcreate_selbox_off.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList );

	for(int i = 0 ; i < 8 ; i++)
	{
		sprintf(tempSprBuf,"Online\\GameSpr\\NewLogIn\\charcreate_selbox_on_%d.spr",i+1);
		clGrp.LoadXspr(tempSprBuf,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i]);
		
		sprintf(tempSprBuf,"Online\\GameSpr\\NewLogIn\\charcreate_pic_%d.Spr",i+1);
		clGrp.LoadXspr(tempSprBuf,pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[i]);
	}

	//캐릭터 설명.캐릭터 스탯창 이미지.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_whitebox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase);

	//캐릭터 스탯 화살표 이미지.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_up.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_down.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn);

	//뒤로.캐릭터 생성 버튼 이미지.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_button.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgButtonBaseSpr);

	//캐릭터 생성중입니다. 이미지.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_darkline.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr);
	
	//주인공 캐릭터의 ID 인덱스를 얻어온다.
	pOnlineLogInMgr->m_CreateCharacter.siCharTotalNum			= pIOnlineCharKI->GetPlayerCharIDList( pOnlineLogInMgr->m_CreateCharacter.uiCharCode );
	pOnlineLogInMgr->m_CreateCharacter.siKind					= -1;		// 선택한 케릭터
	pOnlineLogInMgr->m_CreateCharacter.uiSlot					= 0;		// 비어 있는 슬롯 번호
	pOnlineLogInMgr->m_CreateCharacter.siStatus				= ON_CREATE_CHAR_INIT;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//캐릭터 리스트 셀렉트 버튼 생성.
	for(i = 0 ; i < 8 ; i++)
		pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[i].Create(11 + ( i * 98),473 ,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );

	// 힘, 지력 등의 상태 결정 버튼을 생성을 한다.
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[0].Create(608,308,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[1].Create(701,308,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[2].Create(608,333,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[3].Create(701,333,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[4].Create(701,358,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );

	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[0].Create(608,318,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[1].Create(701,318,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[2].Create(608,343,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[3].Create(701,343,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[4].Create(701,368,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn.Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );

	
	pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.Create(248,557,pOnlineLogInMgr->SButtonImage.Header.Xsize,pOnlineLogInMgr->SButtonImage.Header.Ysize,pOnlineText->Get(1000114),BUTTON_PUT_LEFT, TRUE );
	pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Create(481,557,pOnlineLogInMgr->SButtonImage.Header.Xsize,pOnlineLogInMgr->SButtonImage.Header.Ysize,pOnlineText->Get(1000115),BUTTON_PUT_LEFT, FALSE );

	// ID 입력 에디터
	// ID 입력용 EDIT BOX 생성
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox			= new OnlineEditControl;
//	if( pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->CreateEditBox(1000, 1000, 100, 15, pGame->Hwnd, pGame->hInst, FALSE, FALSE, LOGIN::NAME_MAX_LENGTH -1) == FALSE)	
//		clGrp.Error("InitCreateCharacter", pOnlineText->Get(ON_TEXT_EDITBOXFAIL));
//
//	// 에디트 박스의 폰트를 결정
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFont( pOnlineFonts->GetFont(ON_FONT_LOGIN2));
//
//	// 에디트 박스의 입력 문자열 길이를 제한 
//	if(pGame->LanguageCode == ELANGAGECODE_JAPAN)
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetHalfCode(FALSE);
//
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();

	// actdoll (2004/05/04 16:33) : [OECEX]
	OnlineEditControlEx	*pOecEx;
	RECT	rcRect = { 600, 281, 750, 296 };
	// actdoll (2004/05/04 16:33) : 만약 인도네시아 버젼일 경우 아스키 입력만 받도록 한다
	DWORD	dwFlag = 0;
	if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA )	dwFlag = DF_FLAG_OEC_TYPE_ONLYASCII;
	pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName	= Handler_OnlineEditControlEx::CreateControl( dwFlag, &rcRect, LOGIN::NAME_MAX_LENGTH - 1 );
	pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_LOGIN2 ) );
	pOecEx->SetColor( RGB(250, 250, 250) );
	pOecEx->SetExceptionChar( ' ' );
	pOecEx->SetExceptionChar( '\'' );
	// 일본어의 경우 반각 코드에 대한 세팅을 어떻게 할 것인가가 관건이다. 만약 하게 되면 여기다 세팅할 것.


	// 스텟을 초기화
	pOnlineLogInMgr->m_CreateCharacter.Totalb	= ON_CHARACTER_INITBONUS;
	pOnlineLogInMgr->m_CreateCharacter.siStra	= SISTRA;
	pOnlineLogInMgr->m_CreateCharacter.siDext	= SIDEXT;
	pOnlineLogInMgr->m_CreateCharacter.siHp2	= SIHP2;
	pOnlineLogInMgr->m_CreateCharacter.siInteli	= SIINTELI;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 만드는 함수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI08 cltOnlineWorld::CreateCharacter()
{	
	SI16	i;
	OnlineEditControlEx	*pOecEx;

	switch(pOnlineLogInMgr->m_CreateCharacter.siStatus)
	{
	case ON_CREATE_CHAR_INIT:
		{
			// 마우스를 복구한다.
			SetCursor(LoadCursor(NULL, IDC_ARROW));	
			
			// ID입력 에디터 박스를 Enable 시키고 커서를 위치 시킨다.	
//			pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();
			// actdoll (2004/05/04 16:33) : [OECEX]
			Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );

			pOnlineLogInMgr->m_CreateCharacter.siStatus = ON_CREATE_CHAR_POLL;
		}
		break;
	case ON_CREATE_CHAR_POLL:
		{
			for(i = 0 ; i < 8 ; i++)			
			{
				if(pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[i].Process(pOnlineLogInMgr->bMouseDown) == TRUE)
					pOnlineLogInMgr->m_CreateCharacter.siKind	= i;
			}

			for( i=0; i < 4; i++ )
			{
				if( pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[i].Process(pOnlineLogInMgr->bMouseDown ) )
				{
					if( pOnlineLogInMgr->m_CreateCharacter.Totalb > 0 )
					{
						switch( i )
						{
						case 0:			// 힘
							pOnlineLogInMgr->m_CreateCharacter.siStra++;
							break;
						case 1:			// 민첩성
							pOnlineLogInMgr->m_CreateCharacter.siDext++;
							break;
						case 2:			// 생명력
							pOnlineLogInMgr->m_CreateCharacter.siHp2++;
							break;
						case 3:			// 지력
							pOnlineLogInMgr->m_CreateCharacter.siInteli++;
							break;
						}
						
						pOnlineLogInMgr->m_CreateCharacter.Totalb--;
					}
				}
				if( pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[i].Process(pOnlineLogInMgr->bMouseDown ) )
				{
					switch( i )
					{
					case 0:
						pOnlineLogInMgr->m_CreateCharacter.siStra--;
						if( pOnlineLogInMgr->m_CreateCharacter.siStra < SISTRA ) 		pOnlineLogInMgr->m_CreateCharacter.siStra = SISTRA;
						else												pOnlineLogInMgr->m_CreateCharacter.Totalb++;		
						break;
					case 1:
						pOnlineLogInMgr->m_CreateCharacter.siDext--;
						if( pOnlineLogInMgr->m_CreateCharacter.siDext < SIDEXT ) 		pOnlineLogInMgr->m_CreateCharacter.siDext = SIDEXT;
						else												pOnlineLogInMgr->m_CreateCharacter.Totalb++;		
						break;
					case 2:
						pOnlineLogInMgr->m_CreateCharacter.siHp2--;
						if( pOnlineLogInMgr->m_CreateCharacter.siHp2 < SIHP2 ) 		pOnlineLogInMgr->m_CreateCharacter.siHp2 = SIHP2;
						else												pOnlineLogInMgr->m_CreateCharacter.Totalb++;		
						break;
					case 3:
						pOnlineLogInMgr->m_CreateCharacter.siInteli--;
						if( pOnlineLogInMgr->m_CreateCharacter.siInteli < SIINTELI ) 	pOnlineLogInMgr->m_CreateCharacter.siInteli = SIINTELI;
						else												pOnlineLogInMgr->m_CreateCharacter.Totalb++;		
						break;
					}
				}
			}			
			
			if(pOnlineLoginMsgBox->IsActive())
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(FALSE);
				// actdoll (2004/05/04 16:33) : [OECEX]
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->EnableWindow(FALSE);
				Handler_OnlineEditControlEx::ClearCurrentFocus();
			}
			
			if(m_siMsgBoxStatus == ON_MSGBOX_RETURN_OK)
			{ 
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(TRUE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
				// actdoll (2004/05/04 16:33) : [OECEX]
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->EnableWindow(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();
				Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
			}
			
			//뒤로 돌아가기 버튼
			if( pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.Process(pOnlineLogInMgr->bMouseDown) == TRUE )
				return ON_RETURN_EXIT;


			if( pOnlineLogInMgr->m_CreateCharacter.Totalb == 0 )
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(TRUE);
			}
			else
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
			}

			
			// 캐릭터 선택 버튼을 눌러야 결정 버튼을 누를수가 이뜸
			if (pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Process( pOnlineLogInMgr->bMouseDown )) //결정 버튼이 눌리면 ID를 체크한다.
			{	
				// 아이디 입력 부분
				ZeroMemory( pOnlineLogInMgr->m_CreateCharacter.szID, sizeof(pOnlineLogInMgr->m_CreateCharacter.szID));							// #define LOGIN::NAME_MAX_LENGTH		11
				// actdoll (2004/05/04 16:33) : [OECEX]
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);	// null terminator 포함해서 얻어옴
				pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );

				
				if( pOnlineLogInMgr->m_CreateCharacter.Totalb == 0 )
				{
					// 인도네시아일 경우 멀티바이트 글자가 들어온다면 에러
					if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA )
					{
						for( int i=0; i<strlen( pOnlineLogInMgr->m_CreateCharacter.szID ); i++ )
						{
							if( IsDBCSLeadByte( pOnlineLogInMgr->m_CreateCharacter.szID[i] ) )
							{
								pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHATID), pOnlineText->Get(ON_TEXT_CHATIDFAIL) );
								break;
							}
						}
					}

					if(pOnlineLogInMgr->m_CreateCharacter.siKind == -1)
						pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHCREATEFAIL), pOnlineText->Get(8000523));
					else if(strlen(pOnlineLogInMgr->m_CreateCharacter.szID) == 0 )
						pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHCREATEFAIL), pOnlineText->Get(ON_TEXT_NO_INPUT_NAME));
					else if( !IsValidCharacterNameString( pOnlineLogInMgr->m_CreateCharacter.szID ) )
						pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHATID), pOnlineText->Get(ON_TEXT_CHATIDFAIL) );
					else
					{
						SI16 TotStat = pOnlineLogInMgr->m_CreateCharacter.siStra + pOnlineLogInMgr->m_CreateCharacter.siDext + 
							pOnlineLogInMgr->m_CreateCharacter.siHp2 + pOnlineLogInMgr->m_CreateCharacter.siInteli; 
						if(TotStat == 60)
						{
							pOnlineLogInMgr->m_CreateCharacter.uiSlot = pMyData->GetEmptySlot();
							if(pOnlineLogInMgr->m_CreateCharacter.uiSlot)
							{
								pOnlineClient->SendCreateNewCharacter(pOnlineLogInMgr->m_CreateCharacter.szID, (SI32)pOnlineLogInMgr->m_CreateCharacter.uiCharCode[(pOnlineLogInMgr->m_CreateCharacter.siCharTotalNum - 1) - pOnlineLogInMgr->m_CreateCharacter.siKind], 
									pOnlineLogInMgr->m_CreateCharacter.siStra - SISTRA, pOnlineLogInMgr->m_CreateCharacter.siDext - SIDEXT, pOnlineLogInMgr->m_CreateCharacter.siHp2 - SIHP2, 
									pOnlineLogInMgr->m_CreateCharacter.siInteli - SIINTELI ); 
								
								pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction( FALSE );
								pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction( FALSE );
								
								// actdoll (2004/05/04 16:33) : [OECEX]
//								::SetFocus( pGame->Hwnd );
								Handler_OnlineEditControlEx::ClearCurrentFocus();

								pOnlineLogInMgr->m_CreateCharacter.siStatus = ON_CREATE_CHAR_CREATE;
							}
						}
						else
							pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHCREATEFAIL), "Invalid Parameter");
					}
				}
			}
		}
		break;
	case ON_CREATE_CHAR_CREATE:
		{
			LOGIN::sResponseNewChar	*pResNewCharMsg = (LOGIN::sResponseNewChar*)pOnlineClient->GetMsgBuffer((DWORD)LOGIN::RESPONSE_NEW_CHAR);
			
			if(!pResNewCharMsg)		break;

			// actdoll (2004/05/04 16:33) : [OECEX]
//			pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetString("");
			pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
			pOecEx->Clear();

			if(pResNewCharMsg->responseKind == LOGIN::OK)
			{
				pOnlineLogInMgr->m_CreateCharacter.uiSlot = pResNewCharMsg->newSlot;
				pMyData->AddMyCharacter(pOnlineLogInMgr->m_CreateCharacter.szID, pOnlineLogInMgr->m_CreateCharacter.uiCharCode[pOnlineLogInMgr->m_CreateCharacter.siCharTotalNum - pOnlineLogInMgr->m_CreateCharacter.siKind - 1], pResNewCharMsg->newSlot);

				return ON_RETURN_EXIT;
			}
			else if(pResNewCharMsg->responseKind == LOGIN::REPEAT_NAME)				// 이미 다른 캐릭터가 같은 아이디를 사용중이다.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_IDOVERLAP));
			}
			else if(pResNewCharMsg->responseKind == LOGIN::RESTRICT_NAME)			// 이미 다른 캐릭터가 같은 아이디를 사용중이다.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_CHATIDFAIL));
			}
			else																	// 캐릭터를 생성할 때 기타 에러.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK, pOnlineText->Get(ON_TEXT_CHCREATEFAIL), pOnlineText->Get(ON_TEXT_DONOTKNOW));	
			}

			if( pResNewCharMsg->responseKind != LOGIN::OK )
			{
				Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
			}


			pOnlineLogInMgr->m_CreateCharacter.siStatus = ON_CREATE_CHAR_POLL;
		}
		break;
	}

	return ON_RETURN_NONE;
}

VOID	cltOnlineWorld::DrawCreateCharacter()
{
	HDC		hdc;	
	CHAR	szTemp[256];
	SI16	siTemp;

	POINT	pt;
	pt.x = IpD.Mouse_X;
	pt.y = IpD.Mouse_Y;

	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );

	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		// 캐릭터 리스트.
		clGrp.PutSpriteT( 9,471,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Ysize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Image);
		
		//
/*		for(i = 0 ; i < 8 ; i++)
			if( (pt.x >= (11 + ( 98 * i)) ) && (pt.x <= (103 + ( 98 * i)) )
				&& (pt.y >=	473) && (pt.y <= 473 + 63) )*/
		if(pOnlineLogInMgr->m_CreateCharacter.siKind >= 0)
		{
			pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Put(&pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind],0,0,0, BUTTON_PUT_NOMOVE);
			//큰 이미지.
			clGrp.PutSpriteT( 41,9,pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Xsize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Ysize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Image);
			clGrp.PutSpriteLightT( 57,325,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Xsize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Ysize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Image);
		}	

		//캐릭터 생성중입니다.텍스트 이미지.
		clGrp.PutSpriteT( 227,442,pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr.Header.Ysize,pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr.Image);

		clGrp.PutSpriteT(523, 256,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase.Header.Ysize,pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase.Image);

		pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Put(&pOnlineLogInMgr->SButtonImage, 0, 1, 0, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.Put(&pOnlineLogInMgr->SButtonImage, 0, 1, 0, BUTTON_PUT_NOMOVE);

		for( SI16 i=0; i<4; i++ )
		{       
			pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxUp[i].Put(&pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp,0,0,0,BUTTON_PUT_CENTER );
			pOnlineLogInMgr->m_CreateCharacter.m_BttnInfoBoxDn[i].Put(&pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn,0,0,0,BUTTON_PUT_CENTER );
		}		
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}

	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject( hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
		ZeroMemory(szTemp, sizeof(szTemp));

		if(pOnlineLogInMgr->m_CreateCharacter.siKind >= 0)
		{

			sprintf(szTemp,"0000000%d",pOnlineLogInMgr->m_CreateCharacter.siKind + 1);
			siTemp = atoi(szTemp);
			pOnlineMegaText->DrawText( hdc,76 ,339 , pOnlineText->GetCharInfoText(siTemp), RGB(0, 0, 0));	

			sprintf(szTemp,"000000%d0",pOnlineLogInMgr->m_CreateCharacter.siKind + 1);
			siTemp =atoi(szTemp);

			SI16 num = DevideByLine(pOnlineText->GetCharInfoText(siTemp),64);
			
			for(int i = 0 ; i <num ; i++)
				pOnlineMegaText->DrawText( hdc,71 ,362 +(i*15) , m_szCharTextList[i], RGB(0, 0, 0));
		}

		pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.Put(hdc,(DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
	
		// 에디트 박스에서 문자열을 얻는다. 캐릭터 이름
			// actdoll (2004/05/04 16:33) : [OECEX]
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->DrawText(hdc, pOnlineFonts->GetFont(ON_FONT_LOGIN2), pOnlineFonts->GetFontUnderline(ON_FONT_LOGIN2)
//			, RGB(250, 250, 250),600,281, NULL, pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->IsFocus());
		strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );
		pOecEx->Draw( hdc );
		
		// '새로운 캐릭터를 생성합니다.' 텍스트.
		pOnlineMegaText->DrawTextCenter( hdc,291 ,447 ,218,14, pOnlineText->Get(8000522), RGB(250, 250, 250));

		//능력치 텍스트
		pOnlineMegaText->DrawTextCenter( hdc,551 - 1,282 - 1,26,14, pOnlineText->Get(ON_TEXT_NAME), RGB( 10, 10, 10 ) );
		pOnlineMegaText->DrawTextCenter( hdc,532 - 1,311 - 1,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_STR)   , RGB( 10, 10, 10 ) );
		pOnlineMegaText->DrawTextCenter( hdc,625 - 1,311 - 1,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), RGB( 10, 10, 10 ) );
		pOnlineMegaText->DrawTextCenter( hdc,532 - 1,336 - 1,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_VIT), RGB( 10, 10, 10 ) );
		pOnlineMegaText->DrawTextCenter( hdc,625 - 1,336 - 1,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_INT), RGB( 10, 10, 10 ) );
		pOnlineMegaText->DrawTextCenter( hdc,625 - 1,365 - 1,43,14, pOnlineText->Get(ON_TEXT_BOUNS), RGB( 10, 10, 10 ) );

		pOnlineMegaText->DrawTextCenter( hdc,551 ,282 ,26,14, pOnlineText->Get(ON_TEXT_NAME), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc,532 ,311 ,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_STR), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc,625 ,311 ,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_DEX), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc,532 ,336 ,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_VIT), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc,625 ,336 ,43,14, pOnlineText->Get(ON_TEXT_ITEMHELP_INT), RGB(250, 250, 250));
		pOnlineMegaText->DrawTextCenter( hdc,625 ,365 ,43,14, pOnlineText->Get(ON_TEXT_BOUNS), RGB(250, 250, 250));
		

		// 능력치 출력
		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siStra );		// 힘
		pOnlineMegaText->DrawTextCenter( hdc,580,310,25,15,szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siDext );		// 민첩
		pOnlineMegaText->DrawTextCenter( hdc,673,310,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siHp2 );			// 생명력
		pOnlineMegaText->DrawTextCenter( hdc,580,335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siInteli );		// 지력
		pOnlineMegaText->DrawTextCenter( hdc, 673, 335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.Totalb );		// 보너스
		pOnlineMegaText->DrawTextCenter( hdc, 675, 361,25,15, szTemp, RGB(250, 250, 250));

		

		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터를 만드는 함수의 메모리해제.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineWorld::FreeCreateCharacter()
{
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgButtonBaseSpr);
	clGrp.FreeXspr( pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr);

	for(SI16 i = 0 ; i < 8 ; i++)
	{
		clGrp.FreeXspr(pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i]);
		clGrp.FreeXspr(pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[i]);
	}

	// actdoll (2004/05/04 16:33) : [OECEX]
//	if(pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox)
//	{
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->Free();
//		delete pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox;	 
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox	=	NULL;
//	}
	Handler_OnlineEditControlEx::KillControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
}


UI32 cltOnlineWorld::DevideByLine(CHAR *pContent, SI32	siMaxStrLen)
{
	UI32	line;									// 몇 라인의 문자열인가?
	CHAR	buffer[64][128];
	CHAR	content[1024];
	SI32	Num;
	SI32	i, contentLen;
	
	wsprintf(content, "%s", pContent);
	contentLen = strlen(pContent);
	//---------------------------------
	// 문자열이 길 경우 여러 라인으로 나누어 버퍼에 넣는다.
	if( contentLen > siMaxStrLen )	
	{
		line = 0;
		Num = 0;	
		for( i=0; content[i] != '\0'; i++ )
		{
			// 받아온 문자열 중 한 문자를 버퍼에 넣는다.
			buffer[line][Num] = content[i];
			if(Num == 0)
			{
				if(buffer[line][Num] == ' ')
				{
					//i++;
					Num = 0;
					continue;
				}
			}
			Num++;

			// 한글일 경우
			if( UI08(content[i]) > 127 )
			{
				// 한글을 완성하지 못했는데 버퍼를 넘기면 이 한글은 다음 라인으로 넘긴다.
				if( Num >= siMaxStrLen )
				{
					buffer[line][Num-1] = '\0';
					line++;
					Num = 0;
					if(content[i] != ' ')
						i--;
					continue;
				}
			
				else
				{
					// 한 바이트 더 읽는다.
					buffer[line][Num++] = content[++i];
				}
			}

			if( Num >= siMaxStrLen )
			{
				buffer[line][Num] = '\0';
				line++;
				Num = 0;
			}
		}

		// 라인을 다 채우지 못하고 문자열이 끝났을 때 널 문자를 추가하고, 라인수를 설정한다.
		if( Num != 0 )
		{
			buffer[line][Num] = '\0';
			line++;
		}
	}
	else
	{
		wsprintf(buffer[0], "%s", content);
		line = 1;
	}
	
	//---------------------------------
	// TIP 문자열 리스트에 받아온 문자열 넣기

	for( i=0; i<(SI32)line; i++ )
	{
		// 문자열 한 줄을 추가한다
		strcpy(m_szCharTextList[i], buffer[i]);
	}

	//현재 출력되어야 하는 라인 설정

	return line;
}

//-----------------------------------------------------------------------------
// Name: IsValidCharacterNameString()
// Code: actdoll (2004-01-27)
// Desc: 주어진 스트링이 캐릭터 이름으로 쓸 수 있는 문자열인가?
//	함수 이름이 CheckID()에서 이것으로 바뀌었다. 문자열이 공백 혹은 미리 제시된
//	쓸 수 없는 문자열일 경우를 체크한다.
//		pszCharName	- 캐릭터 이름으로 줄 문자열
//		return		- 쓸 수 있다면 true, 아니라면 false
//-----------------------------------------------------------------------------
BOOL	cltOnlineWorld::IsValidCharacterNameString( char* pszCharName )
{
	SI16	i;
	if( !CheckLeadByte( pszCharName ) )		return FALSE;

	// 일단 들어온 문자를 유니코드로 전환하여 한글자씩 비교
	SI32	siUniTextSize;
	WCHAR	pszUniText[256] = {0,};
	siUniTextSize	= MultiByteToWideChar( pGame->m_uiCodePage, 0, pszCharName, -1, pszUniText, 256 );
	if( siUniTextSize == 0 )				return FALSE;
	for( i=0; i<siUniTextSize - 1; i++ )
	{
		// 널 값이 왔다면 거기서 짜르고
		if( !pszUniText[i] )	break;

		// 안되는 코드들은 리턴
		switch( pszUniText[i] )
		{
#define _TEXT(x)	L ## x
			// 백슬래쉬			// 스페이스			// 작은따옴표		// 큰따옴표			// 탭				// 라인변환
			case _TEXT('\\')	:	case _TEXT(' ')	:	case _TEXT('\'')	:	case _TEXT('\"')	:	case _TEXT('\t')	:	case _TEXT('\r')	:	return FALSE;
#undef	_TEXT
		}
	}

	// actdoll (2004/02/23 15:55) : 
	//	다른 방식으로 해당 정보를 로딩한다.
	// actdoll (2004/11/15 23:17) : 외부 HQ 옵션이 켜진 경우 체크한다.
	char	pszFullPath[MAX_PATH];
	char	*pszFileName	= "%s\\NameFilterInfo.dat";
	FILE*	pFile			= NULL;
	if( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] )	// 외부 HQ가 있다면
	{
		sprintf( pszFullPath, pszFileName, DF_PATH_DEBUG_OUTSIDE_HQTEXT );
		pFile	= fopen( pszFullPath, "rb" );
	}
	if( !pFile )								// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName, pGame->m_pszHQNation );
		pFile	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !pFile )	return FALSE;				// 그래도 발견 못하면 리턴

	// 필터 파일을 읽는다.
	OnlineCommonParser		ocp;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	ocp.SetErrFileName( "c:\\ParseErr_NameFilterInfo.txt" );

	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( pFile, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "NameFilterInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFullPath, iRet, iErrLine, bLess );
		fclose( pFile );
		return FALSE;
	}

	// 비교를 위해 주어진 이름을 임시 버퍼로 옮긴다.
	char	pszConvCharName[256] = {0,};
	strcpy	( pszConvCharName, pszCharName );
	strtok	( pszConvCharName, "\n\r" );		// 여분 없애고

	// 자료를 받는다.
	while(1)
	{
		char	pszFilterLine[1024];

		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인을 일단 추출하고
		
		// 추출한 자료를 받는다.
		char	*pConvStrPtr;
		int		iLineNo, iColNo;
		int		iFilterStrLen;

		ocp.GetValue( pszFilterLine, 1023, iLineNo, iColNo );	// 욕설 텍스트 받음

		// 들어온 글자를 한자씩 넘어가면서 비교해본다.
		pConvStrPtr		= pszConvCharName;
		iFilterStrLen	= strlen( pszFilterLine );
		while( strlen( pConvStrPtr ) >= iFilterStrLen )
		{
			if( strnicmp( pConvStrPtr, pszFilterLine, iFilterStrLen ) == 0 )	// 리스트에 있는 글자가 발견됐다면
			{
				fclose( pFile );
				return FALSE;
			}

			pConvStrPtr	= CharNextExA( pGame->m_uiCodePage, pConvStrPtr, 0 );
		}
	}

	ocp.Release();
	fclose( pFile );
	return TRUE;
}

