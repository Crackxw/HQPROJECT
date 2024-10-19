
// ���ϸ� : OnlineWorld-CreateCharacter.cpp
// ����� : ������
// ĳ���� ���� ȭ��

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


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� ����� �ʱ�ȭ �Լ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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

	//ĳ���� ����.ĳ���� ����â �̹���.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_whitebox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase);

	//ĳ���� ���� ȭ��ǥ �̹���.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_up.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_down.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn);

	//�ڷ�.ĳ���� ���� ��ư �̹���.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_button.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgButtonBaseSpr);

	//ĳ���� �������Դϴ�. �̹���.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_darkline.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr);
	
	//���ΰ� ĳ������ ID �ε����� ���´�.
	pOnlineLogInMgr->m_CreateCharacter.siCharTotalNum			= pIOnlineCharKI->GetPlayerCharIDList( pOnlineLogInMgr->m_CreateCharacter.uiCharCode );
	pOnlineLogInMgr->m_CreateCharacter.siKind					= -1;		// ������ �ɸ���
	pOnlineLogInMgr->m_CreateCharacter.uiSlot					= 0;		// ��� �ִ� ���� ��ȣ
	pOnlineLogInMgr->m_CreateCharacter.siStatus				= ON_CREATE_CHAR_INIT;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ĳ���� ����Ʈ ����Ʈ ��ư ����.
	for(i = 0 ; i < 8 ; i++)
		pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[i].Create(11 + ( i * 98),473 ,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );

	// ��, ���� ���� ���� ���� ��ư�� ������ �Ѵ�.
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

	// ID �Է� ������
	// ID �Է¿� EDIT BOX ����
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox			= new OnlineEditControl;
//	if( pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->CreateEditBox(1000, 1000, 100, 15, pGame->Hwnd, pGame->hInst, FALSE, FALSE, LOGIN::NAME_MAX_LENGTH -1) == FALSE)	
//		clGrp.Error("InitCreateCharacter", pOnlineText->Get(ON_TEXT_EDITBOXFAIL));
//
//	// ����Ʈ �ڽ��� ��Ʈ�� ����
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFont( pOnlineFonts->GetFont(ON_FONT_LOGIN2));
//
//	// ����Ʈ �ڽ��� �Է� ���ڿ� ���̸� ���� 
//	if(pGame->LanguageCode == ELANGAGECODE_JAPAN)
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetHalfCode(FALSE);
//
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();

	// actdoll (2004/05/04 16:33) : [OECEX]
	OnlineEditControlEx	*pOecEx;
	RECT	rcRect = { 600, 281, 750, 296 };
	// actdoll (2004/05/04 16:33) : ���� �ε��׽þ� ������ ��� �ƽ�Ű �Է¸� �޵��� �Ѵ�
	DWORD	dwFlag = 0;
	if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA )	dwFlag = DF_FLAG_OEC_TYPE_ONLYASCII;
	pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName	= Handler_OnlineEditControlEx::CreateControl( dwFlag, &rcRect, LOGIN::NAME_MAX_LENGTH - 1 );
	pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_LOGIN2 ) );
	pOecEx->SetColor( RGB(250, 250, 250) );
	pOecEx->SetExceptionChar( ' ' );
	pOecEx->SetExceptionChar( '\'' );
	// �Ϻ����� ��� �ݰ� �ڵ忡 ���� ������ ��� �� ���ΰ��� �����̴�. ���� �ϰ� �Ǹ� ����� ������ ��.


	// ������ �ʱ�ȭ
	pOnlineLogInMgr->m_CreateCharacter.Totalb	= ON_CHARACTER_INITBONUS;
	pOnlineLogInMgr->m_CreateCharacter.siStra	= SISTRA;
	pOnlineLogInMgr->m_CreateCharacter.siDext	= SIDEXT;
	pOnlineLogInMgr->m_CreateCharacter.siHp2	= SIHP2;
	pOnlineLogInMgr->m_CreateCharacter.siInteli	= SIINTELI;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� ����� �Լ�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI08 cltOnlineWorld::CreateCharacter()
{	
	SI16	i;
	OnlineEditControlEx	*pOecEx;

	switch(pOnlineLogInMgr->m_CreateCharacter.siStatus)
	{
	case ON_CREATE_CHAR_INIT:
		{
			// ���콺�� �����Ѵ�.
			SetCursor(LoadCursor(NULL, IDC_ARROW));	
			
			// ID�Է� ������ �ڽ��� Enable ��Ű�� Ŀ���� ��ġ ��Ų��.	
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
						case 0:			// ��
							pOnlineLogInMgr->m_CreateCharacter.siStra++;
							break;
						case 1:			// ��ø��
							pOnlineLogInMgr->m_CreateCharacter.siDext++;
							break;
						case 2:			// �����
							pOnlineLogInMgr->m_CreateCharacter.siHp2++;
							break;
						case 3:			// ����
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
			
			//�ڷ� ���ư��� ��ư
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

			
			// ĳ���� ���� ��ư�� ������ ���� ��ư�� �������� �̶�
			if (pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Process( pOnlineLogInMgr->bMouseDown )) //���� ��ư�� ������ ID�� üũ�Ѵ�.
			{	
				// ���̵� �Է� �κ�
				ZeroMemory( pOnlineLogInMgr->m_CreateCharacter.szID, sizeof(pOnlineLogInMgr->m_CreateCharacter.szID));							// #define LOGIN::NAME_MAX_LENGTH		11
				// actdoll (2004/05/04 16:33) : [OECEX]
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);	// null terminator �����ؼ� ����
				pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );

				
				if( pOnlineLogInMgr->m_CreateCharacter.Totalb == 0 )
				{
					// �ε��׽þ��� ��� ��Ƽ����Ʈ ���ڰ� ���´ٸ� ����
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
			else if(pResNewCharMsg->responseKind == LOGIN::REPEAT_NAME)				// �̹� �ٸ� ĳ���Ͱ� ���� ���̵� ������̴�.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_IDOVERLAP));
			}
			else if(pResNewCharMsg->responseKind == LOGIN::RESTRICT_NAME)			// �̹� �ٸ� ĳ���Ͱ� ���� ���̵� ������̴�.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_CHATIDFAIL));
			}
			else																	// ĳ���͸� ������ �� ��Ÿ ����.
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
		// ĳ���� ����Ʈ.
		clGrp.PutSpriteT( 9,471,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Ysize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Image);
		
		//
/*		for(i = 0 ; i < 8 ; i++)
			if( (pt.x >= (11 + ( 98 * i)) ) && (pt.x <= (103 + ( 98 * i)) )
				&& (pt.y >=	473) && (pt.y <= 473 + 63) )*/
		if(pOnlineLogInMgr->m_CreateCharacter.siKind >= 0)
		{
			pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Put(&pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind],0,0,0, BUTTON_PUT_NOMOVE);
			//ū �̹���.
			clGrp.PutSpriteT( 41,9,pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Xsize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Ysize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Image);
			clGrp.PutSpriteLightT( 57,325,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Xsize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Ysize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Image);
		}	

		//ĳ���� �������Դϴ�.�ؽ�Ʈ �̹���.
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
	
		// ����Ʈ �ڽ����� ���ڿ��� ��´�. ĳ���� �̸�
			// actdoll (2004/05/04 16:33) : [OECEX]
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->DrawText(hdc, pOnlineFonts->GetFont(ON_FONT_LOGIN2), pOnlineFonts->GetFontUnderline(ON_FONT_LOGIN2)
//			, RGB(250, 250, 250),600,281, NULL, pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->IsFocus());
		strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );
		pOecEx->Draw( hdc );
		
		// '���ο� ĳ���͸� �����մϴ�.' �ؽ�Ʈ.
		pOnlineMegaText->DrawTextCenter( hdc,291 ,447 ,218,14, pOnlineText->Get(8000522), RGB(250, 250, 250));

		//�ɷ�ġ �ؽ�Ʈ
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
		

		// �ɷ�ġ ���
		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siStra );		// ��
		pOnlineMegaText->DrawTextCenter( hdc,580,310,25,15,szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siDext );		// ��ø
		pOnlineMegaText->DrawTextCenter( hdc,673,310,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siHp2 );			// �����
		pOnlineMegaText->DrawTextCenter( hdc,580,335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siInteli );		// ����
		pOnlineMegaText->DrawTextCenter( hdc, 673, 335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.Totalb );		// ���ʽ�
		pOnlineMegaText->DrawTextCenter( hdc, 675, 361,25,15, szTemp, RGB(250, 250, 250));

		

		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ���͸� ����� �Լ��� �޸�����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
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
	UI32	line;									// �� ������ ���ڿ��ΰ�?
	CHAR	buffer[64][128];
	CHAR	content[1024];
	SI32	Num;
	SI32	i, contentLen;
	
	wsprintf(content, "%s", pContent);
	contentLen = strlen(pContent);
	//---------------------------------
	// ���ڿ��� �� ��� ���� �������� ������ ���ۿ� �ִ´�.
	if( contentLen > siMaxStrLen )	
	{
		line = 0;
		Num = 0;	
		for( i=0; content[i] != '\0'; i++ )
		{
			// �޾ƿ� ���ڿ� �� �� ���ڸ� ���ۿ� �ִ´�.
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

			// �ѱ��� ���
			if( UI08(content[i]) > 127 )
			{
				// �ѱ��� �ϼ����� ���ߴµ� ���۸� �ѱ�� �� �ѱ��� ���� �������� �ѱ��.
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
					// �� ����Ʈ �� �д´�.
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

		// ������ �� ä���� ���ϰ� ���ڿ��� ������ �� �� ���ڸ� �߰��ϰ�, ���μ��� �����Ѵ�.
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
	// TIP ���ڿ� ����Ʈ�� �޾ƿ� ���ڿ� �ֱ�

	for( i=0; i<(SI32)line; i++ )
	{
		// ���ڿ� �� ���� �߰��Ѵ�
		strcpy(m_szCharTextList[i], buffer[i]);
	}

	//���� ��µǾ�� �ϴ� ���� ����

	return line;
}

//-----------------------------------------------------------------------------
// Name: IsValidCharacterNameString()
// Code: actdoll (2004-01-27)
// Desc: �־��� ��Ʈ���� ĳ���� �̸����� �� �� �ִ� ���ڿ��ΰ�?
//	�Լ� �̸��� CheckID()���� �̰����� �ٲ����. ���ڿ��� ���� Ȥ�� �̸� ���õ�
//	�� �� ���� ���ڿ��� ��츦 üũ�Ѵ�.
//		pszCharName	- ĳ���� �̸����� �� ���ڿ�
//		return		- �� �� �ִٸ� true, �ƴ϶�� false
//-----------------------------------------------------------------------------
BOOL	cltOnlineWorld::IsValidCharacterNameString( char* pszCharName )
{
	SI16	i;
	if( !CheckLeadByte( pszCharName ) )		return FALSE;

	// �ϴ� ���� ���ڸ� �����ڵ�� ��ȯ�Ͽ� �ѱ��ھ� ��
	SI32	siUniTextSize;
	WCHAR	pszUniText[256] = {0,};
	siUniTextSize	= MultiByteToWideChar( pGame->m_uiCodePage, 0, pszCharName, -1, pszUniText, 256 );
	if( siUniTextSize == 0 )				return FALSE;
	for( i=0; i<siUniTextSize - 1; i++ )
	{
		// �� ���� �Դٸ� �ű⼭ ¥����
		if( !pszUniText[i] )	break;

		// �ȵǴ� �ڵ���� ����
		switch( pszUniText[i] )
		{
#define _TEXT(x)	L ## x
			// �齽����			// �����̽�			// ��������ǥ		// ū����ǥ			// ��				// ���κ�ȯ
			case _TEXT('\\')	:	case _TEXT(' ')	:	case _TEXT('\'')	:	case _TEXT('\"')	:	case _TEXT('\t')	:	case _TEXT('\r')	:	return FALSE;
#undef	_TEXT
		}
	}

	// actdoll (2004/02/23 15:55) : 
	//	�ٸ� ������� �ش� ������ �ε��Ѵ�.
	// actdoll (2004/11/15 23:17) : �ܺ� HQ �ɼ��� ���� ��� üũ�Ѵ�.
	char	pszFullPath[MAX_PATH];
	char	*pszFileName	= "%s\\NameFilterInfo.dat";
	FILE*	pFile			= NULL;
	if( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] )	// �ܺ� HQ�� �ִٸ�
	{
		sprintf( pszFullPath, pszFileName, DF_PATH_DEBUG_OUTSIDE_HQTEXT );
		pFile	= fopen( pszFullPath, "rb" );
	}
	if( !pFile )								// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName, pGame->m_pszHQNation );
		pFile	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !pFile )	return FALSE;				// �׷��� �߰� ���ϸ� ����

	// ���� ������ �д´�.
	OnlineCommonParser		ocp;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	ocp.SetErrFileName( "c:\\ParseErr_NameFilterInfo.txt" );

	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( pFile, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "NameFilterInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFullPath, iRet, iErrLine, bLess );
		fclose( pFile );
		return FALSE;
	}

	// �񱳸� ���� �־��� �̸��� �ӽ� ���۷� �ű��.
	char	pszConvCharName[256] = {0,};
	strcpy	( pszConvCharName, pszCharName );
	strtok	( pszConvCharName, "\n\r" );		// ���� ���ְ�

	// �ڷḦ �޴´�.
	while(1)
	{
		char	pszFilterLine[1024];

		if( ocp.ParseDataLine() == 0 )	break;		// ������ ������ �ϴ� �����ϰ�
		
		// ������ �ڷḦ �޴´�.
		char	*pConvStrPtr;
		int		iLineNo, iColNo;
		int		iFilterStrLen;

		ocp.GetValue( pszFilterLine, 1023, iLineNo, iColNo );	// �弳 �ؽ�Ʈ ����

		// ���� ���ڸ� ���ھ� �Ѿ�鼭 ���غ���.
		pConvStrPtr		= pszConvCharName;
		iFilterStrLen	= strlen( pszFilterLine );
		while( strlen( pConvStrPtr ) >= iFilterStrLen )
		{
			if( strnicmp( pConvStrPtr, pszFilterLine, iFilterStrLen ) == 0 )	// ����Ʈ�� �ִ� ���ڰ� �߰ߵƴٸ�
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

