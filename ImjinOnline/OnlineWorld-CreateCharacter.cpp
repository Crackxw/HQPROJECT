
// だ橾貲 : OnlineWorld-CreateCharacter.cpp
// 氬渡濠 : 薑霞遵
// 議葛攪 儅撩 �飛�

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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 虜萄朝 蟾晦�� л熱.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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

	//議葛攪 撲貲.議葛攪 蝶囌璽 檜嘐雖.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_whitebox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox.Spr", pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxBase);

	//議葛攪 蝶囌 �香嚂� 檜嘐雖.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_up.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxUp);
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_infobox_ar_down.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgInfoBoxDn);

	//菴煎.議葛攪 儅撩 幗が 檜嘐雖.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_button.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgButtonBaseSpr);

	//議葛攪 儅撩醞殮棲棻. 檜嘐雖.
	clGrp.LoadXspr("Online\\GameSpr\\NewLogIn\\charcreate_darkline.Spr",pOnlineLogInMgr->m_CreateCharacter.m_ImgNewCharSpr);
	
	//輿檣奢 議葛攪曖 ID 檣策蝶蒂 橢橫螞棻.
	pOnlineLogInMgr->m_CreateCharacter.siCharTotalNum			= pIOnlineCharKI->GetPlayerCharIDList( pOnlineLogInMgr->m_CreateCharacter.uiCharCode );
	pOnlineLogInMgr->m_CreateCharacter.siKind					= -1;		// 摹鷗и 馨葛攪
	pOnlineLogInMgr->m_CreateCharacter.uiSlot					= 0;		// 綠橫 氈朝 蝸煜 廓��
	pOnlineLogInMgr->m_CreateCharacter.siStatus				= ON_CREATE_CHAR_INIT;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//議葛攪 葬蝶お 撚滓お 幗が 儅撩.
	for(i = 0 ; i < 8 ; i++)
		pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[i].Create(11 + ( i * 98),473 ,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[i].Header.Ysize,"", BUTTON_PUT_LEFT, TRUE );

	// ��, 雖溘 蛔曖 鼻鷓 唸薑 幗が擊 儅撩擊 и棻.
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

	// ID 殮溘 縑蛤攪
	// ID 殮溘辨 EDIT BOX 儅撩
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox			= new OnlineEditControl;
//	if( pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->CreateEditBox(1000, 1000, 100, 15, pGame->Hwnd, pGame->hInst, FALSE, FALSE, LOGIN::NAME_MAX_LENGTH -1) == FALSE)	
//		clGrp.Error("InitCreateCharacter", pOnlineText->Get(ON_TEXT_EDITBOXFAIL));
//
//	// 縑蛤お 夢蝶曖 アお蒂 唸薑
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFont( pOnlineFonts->GetFont(ON_FONT_LOGIN2));
//
//	// 縑蛤お 夢蝶曖 殮溘 僥濠翮 望檜蒂 薯и 
//	if(pGame->LanguageCode == ELANGAGECODE_JAPAN)
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetHalfCode(FALSE);
//
//	pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();

	// actdoll (2004/05/04 16:33) : [OECEX]
	OnlineEditControlEx	*pOecEx;
	RECT	rcRect = { 600, 281, 750, 296 };
	// actdoll (2004/05/04 16:33) : 虜擒 檣紫啻衛嬴 幗蟒橾 唳辦 嬴蝶酈 殮溘虜 嫡紫煙 и棻
	DWORD	dwFlag = 0;
	if( pGame->m_iNation == DF_CODE_NATION_IDENTITY_INDONESIA )	dwFlag = DF_FLAG_OEC_TYPE_ONLYASCII;
	pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName	= Handler_OnlineEditControlEx::CreateControl( dwFlag, &rcRect, LOGIN::NAME_MAX_LENGTH - 1 );
	pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
	pOecEx->SetFont( pOnlineFonts->GetFont( ON_FONT_LOGIN2 ) );
	pOecEx->SetColor( RGB(250, 250, 250) );
	pOecEx->SetExceptionChar( ' ' );
	pOecEx->SetExceptionChar( '\'' );
	// 橾獄橫曖 唳辦 奩陝 囀萄縑 渠и 撮た擊 橫飩啪 й 匙檣陛陛 婦勒檜棻. 虜擒 ж啪 腎賊 罹晦棻 撮たй 匙.


	// 蝶邐擊 蟾晦��
	pOnlineLogInMgr->m_CreateCharacter.Totalb	= ON_CHARACTER_INITBONUS;
	pOnlineLogInMgr->m_CreateCharacter.siStra	= SISTRA;
	pOnlineLogInMgr->m_CreateCharacter.siDext	= SIDEXT;
	pOnlineLogInMgr->m_CreateCharacter.siHp2	= SIHP2;
	pOnlineLogInMgr->m_CreateCharacter.siInteli	= SIINTELI;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 虜萄朝 л熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
SI08 cltOnlineWorld::CreateCharacter()
{	
	SI16	i;
	OnlineEditControlEx	*pOecEx;

	switch(pOnlineLogInMgr->m_CreateCharacter.siStatus)
	{
	case ON_CREATE_CHAR_INIT:
		{
			// 葆辦蝶蒂 犒掘и棻.
			SetCursor(LoadCursor(NULL, IDC_ARROW));	
			
			// ID殮溘 縑蛤攪 夢蝶蒂 Enable 衛酈堅 醴憮蒂 嬪纂 衛鑑棻.	
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
						case 1:			// 團繪撩
							pOnlineLogInMgr->m_CreateCharacter.siDext++;
							break;
						case 2:			// 儅貲溘
							pOnlineLogInMgr->m_CreateCharacter.siHp2++;
							break;
						case 3:			// 雖溘
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
			
			//菴煎 給嬴陛晦 幗が
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

			
			// 議葛攪 摹鷗 幗が擊 揚楝撿 唸薑 幗が擊 援蒂熱陛 檜嗡
			if (pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.Process( pOnlineLogInMgr->bMouseDown )) //唸薑 幗が檜 揚葬賊 ID蒂 羹觼и棻.
			{	
				// 嬴檜蛤 殮溘 睡碟
				ZeroMemory( pOnlineLogInMgr->m_CreateCharacter.szID, sizeof(pOnlineLogInMgr->m_CreateCharacter.szID));							// #define LOGIN::NAME_MAX_LENGTH		11
				// actdoll (2004/05/04 16:33) : [OECEX]
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);	// null terminator んлп憮 橢橫褥
				pOecEx	= Handler_OnlineEditControlEx::GetControl( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );

				
				if( pOnlineLogInMgr->m_CreateCharacter.Totalb == 0 )
				{
					// 檣紫啻衛嬴橾 唳辦 詩じ夥檜お 旋濠陛 菟橫螞棻賊 縑楝
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
			else if(pResNewCharMsg->responseKind == LOGIN::REPEAT_NAME)				// 檜嘐 棻艇 議葛攪陛 偽擎 嬴檜蛤蒂 餌辨醞檜棻.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				Handler_OnlineEditControlEx::SetFocus( pOnlineLogInMgr->m_CreateCharacter.m_hOecCharName );
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_IDOVERLAP));
			}
			else if(pResNewCharMsg->responseKind == LOGIN::RESTRICT_NAME)			// 檜嘐 棻艇 議葛攪陛 偽擎 嬴檜蛤蒂 餌辨醞檜棻.
			{
				pOnlineLogInMgr->m_CreateCharacter.m_BttnOk.SetAction(FALSE);
				pOnlineLogInMgr->m_CreateCharacter.m_BttnBack.SetAction(TRUE);
//				pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->SetFocus();		
				pOnlineLoginMsgBox->SetMsg( ON_MSGBOX_TYPE_OK,pOnlineText->Get(ON_TEXT_CHCREATEFAIL),pOnlineText->Get(ON_TEXT_CHATIDFAIL));
			}
			else																	// 議葛攪蒂 儅撩й 陽 晦顫 縑楝.
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
		// 議葛攪 葬蝶お.
		clGrp.PutSpriteT( 9,471,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Xsize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Header.Ysize,pOnlineLogInMgr->m_CreateCharacter.m_ImgCharList.Image);
		
		//
/*		for(i = 0 ; i < 8 ; i++)
			if( (pt.x >= (11 + ( 98 * i)) ) && (pt.x <= (103 + ( 98 * i)) )
				&& (pt.y >=	473) && (pt.y <= 473 + 63) )*/
		if(pOnlineLogInMgr->m_CreateCharacter.siKind >= 0)
		{
			pOnlineLogInMgr->m_CreateCharacter.m_BttnSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Put(&pOnlineLogInMgr->m_CreateCharacter.m_ImgSelectChar[pOnlineLogInMgr->m_CreateCharacter.siKind],0,0,0, BUTTON_PUT_NOMOVE);
			//贖 檜嘐雖.
			clGrp.PutSpriteT( 41,9,pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Xsize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Header.Ysize,
						pOnlineLogInMgr->m_CreateCharacter.m_ImgLargeChar[pOnlineLogInMgr->m_CreateCharacter.siKind].Image);
			clGrp.PutSpriteLightT( 57,325,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Xsize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Header.Ysize
						,pOnlineLogInMgr->m_CreateCharacter.m_ImgWhiteBox.Image);
		}	

		//議葛攪 儅撩醞殮棲棻.臢蝶お 檜嘐雖.
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
	
		// 縑蛤お 夢蝶縑憮 僥濠翮擊 橢朝棻. 議葛攪 檜葷
			// actdoll (2004/05/04 16:33) : [OECEX]
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->GetString( pOnlineLogInMgr->m_CreateCharacter.szID);
//		pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->DrawText(hdc, pOnlineFonts->GetFont(ON_FONT_LOGIN2), pOnlineFonts->GetFontUnderline(ON_FONT_LOGIN2)
//			, RGB(250, 250, 250),600,281, NULL, pOnlineLogInMgr->m_CreateCharacter.pOnlinIDEditBox->IsFocus());
		strcpy( pOnlineLogInMgr->m_CreateCharacter.szID, pOecEx->GetString() );
		pOecEx->Draw( hdc );
		
		// '億煎遴 議葛攪蒂 儅撩м棲棻.' 臢蝶お.
		pOnlineMegaText->DrawTextCenter( hdc,291 ,447 ,218,14, pOnlineText->Get(8000522), RGB(250, 250, 250));

		//棟溘纂 臢蝶お
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
		

		// 棟溘纂 轎溘
		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siStra );		// ��
		pOnlineMegaText->DrawTextCenter( hdc,580,310,25,15,szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siDext );		// 團繪
		pOnlineMegaText->DrawTextCenter( hdc,673,310,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siHp2 );			// 儅貲溘
		pOnlineMegaText->DrawTextCenter( hdc,580,335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.siInteli );		// 雖溘
		pOnlineMegaText->DrawTextCenter( hdc, 673, 335,25,15, szTemp, RGB(250, 250, 250));

		sprintf( szTemp, "%d", pOnlineLogInMgr->m_CreateCharacter.Totalb );		// 爾傘蝶
		pOnlineMegaText->DrawTextCenter( hdc, 675, 361,25,15, szTemp, RGB(250, 250, 250));

		

		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	議葛攪蒂 虜萄朝 л熱曖 詭賅葬п薯.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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
	UI32	line;									// 賃 塭檣曖 僥濠翮檣陛?
	CHAR	buffer[64][128];
	CHAR	content[1024];
	SI32	Num;
	SI32	i, contentLen;
	
	wsprintf(content, "%s", pContent);
	contentLen = strlen(pContent);
	//---------------------------------
	// 僥濠翮檜 望 唳辦 罹楝 塭檣戲煎 釭援橫 幗ぷ縑 厥朝棻.
	if( contentLen > siMaxStrLen )	
	{
		line = 0;
		Num = 0;	
		for( i=0; content[i] != '\0'; i++ )
		{
			// 嫡嬴螞 僥濠翮 醞 и 僥濠蒂 幗ぷ縑 厥朝棻.
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

			// и旋橾 唳辦
			if( UI08(content[i]) > 127 )
			{
				// и旋擊 諫撩ж雖 跤ц朝等 幗ぷ蒂 剩晦賊 檜 и旋擎 棻擠 塭檣戲煎 剩曹棻.
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
					// и 夥檜お 渦 檗朝棻.
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

		// 塭檣擊 棻 瓣辦雖 跤ж堅 僥濠翮檜 部陬擊 陽 割 僥濠蒂 蹺陛ж堅, 塭檣熱蒂 撲薑и棻.
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
	// TIP 僥濠翮 葬蝶お縑 嫡嬴螞 僥濠翮 厥晦

	for( i=0; i<(SI32)line; i++ )
	{
		// 僥濠翮 и 還擊 蹺陛и棻
		strcpy(m_szCharTextList[i], buffer[i]);
	}

	//⑷營 轎溘腎橫撿 ж朝 塭檣 撲薑

	return line;
}

//-----------------------------------------------------------------------------
// Name: IsValidCharacterNameString()
// Code: actdoll (2004-01-27)
// Desc: 輿橫霞 蝶お葭檜 議葛攪 檜葷戲煎 噩 熱 氈朝 僥濠翮檣陛?
//	л熱 檜葷檜 CheckID()縑憮 檜匙戲煎 夥船歷棻. 僥濠翮檜 奢寥 �分� 嘐葬 薯衛脹
//	噩 熱 橈朝 僥濠翮橾 唳辦蒂 羹觼и棻.
//		pszCharName	- 議葛攪 檜葷戲煎 還 僥濠翮
//		return		- 噩 熱 氈棻賊 true, 嬴棲塭賊 false
//-----------------------------------------------------------------------------
BOOL	cltOnlineWorld::IsValidCharacterNameString( char* pszCharName )
{
	SI16	i;
	if( !CheckLeadByte( pszCharName ) )		return FALSE;

	// 橾欽 菟橫螞 僥濠蒂 嶸棲囀萄煎 瞪�納狤� и旋濠噶 綠掖
	SI32	siUniTextSize;
	WCHAR	pszUniText[256] = {0,};
	siUniTextSize	= MultiByteToWideChar( pGame->m_uiCodePage, 0, pszCharName, -1, pszUniText, 256 );
	if( siUniTextSize == 0 )				return FALSE;
	for( i=0; i<siUniTextSize - 1; i++ )
	{
		// 割 高檜 諮棻賊 剪晦憮 瞼腦堅
		if( !pszUniText[i] )	break;

		// 寰腎朝 囀萄菟擎 葬欐
		switch( pszUniText[i] )
		{
#define _TEXT(x)	L ## x
			// 寥蝸楚蓮			// 蝶む檜蝶			// 濛擎評褥ル		// 贖評褥ル			// 龔				// 塭檣滲��
			case _TEXT('\\')	:	case _TEXT(' ')	:	case _TEXT('\'')	:	case _TEXT('\"')	:	case _TEXT('\t')	:	case _TEXT('\r')	:	return FALSE;
#undef	_TEXT
		}
	}

	// actdoll (2004/02/23 15:55) : 
	//	棻艇 寞衝戲煎 п渡 薑爾蒂 煎註и棻.
	// actdoll (2004/11/15 23:17) : 諼睡 HQ 褫暮檜 麵霞 唳辦 羹觼и棻.
	char	pszFullPath[MAX_PATH];
	char	*pszFileName	= "%s\\NameFilterInfo.dat";
	FILE*	pFile			= NULL;
	if( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] )	// 諼睡 HQ陛 氈棻賊
	{
		sprintf( pszFullPath, pszFileName, DF_PATH_DEBUG_OUTSIDE_HQTEXT );
		pFile	= fopen( pszFullPath, "rb" );
	}
	if( !pFile )								// 嬴霜梱雖 嫦唯擊 跤ц棻賊 HQ 頂睡縑憮 檗朝棻.
	{
		sprintf( pszFullPath, pszFileName, pGame->m_pszHQNation );
		pFile	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !pFile )	return FALSE;				// 斜楚紫 嫦唯 跤ж賊 葬欐

	// в攪 だ橾擊 檗朝棻.
	OnlineCommonParser		ocp;

	// actdoll (2004/08/23 14:10) : 轎溘腆 縑楝だ橾貲 瞳營
	ocp.SetErrFileName( "c:\\ParseErr_NameFilterInfo.txt" );

	// だ諒擊 衛濛и棻.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( pFile, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "NameFilterInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFullPath, iRet, iErrLine, bLess );
		fclose( pFile );
		return FALSE;
	}

	// 綠掖蒂 嬪п 輿橫霞 檜葷擊 歜衛 幗ぷ煎 衡曹棻.
	char	pszConvCharName[256] = {0,};
	strcpy	( pszConvCharName, pszCharName );
	strtok	( pszConvCharName, "\n\r" );		// 罹碟 橈擁堅

	// 濠猿蒂 嫡朝棻.
	while(1)
	{
		char	pszFilterLine[1024];

		if( ocp.ParseDataLine() == 0 )	break;		// 等檜攪 塭檣擊 橾欽 蹺轎ж堅
		
		// 蹺轎и 濠猿蒂 嫡朝棻.
		char	*pConvStrPtr;
		int		iLineNo, iColNo;
		int		iFilterStrLen;

		ocp.GetValue( pszFilterLine, 1023, iLineNo, iColNo );	// 踹撲 臢蝶お 嫡擠

		// 菟橫螞 旋濠蒂 и濠噶 剩橫陛賊憮 綠掖п獄棻.
		pConvStrPtr		= pszConvCharName;
		iFilterStrLen	= strlen( pszFilterLine );
		while( strlen( pConvStrPtr ) >= iFilterStrLen )
		{
			if( strnicmp( pConvStrPtr, pszFilterLine, iFilterStrLen ) == 0 )	// 葬蝶お縑 氈朝 旋濠陛 嫦唯腑棻賊
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

