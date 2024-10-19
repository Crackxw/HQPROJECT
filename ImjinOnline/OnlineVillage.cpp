/************************************************************************************************
	���ϸ� : OnlineVillage.cpp
	����� : ������

	���� �ȿ��� ó��.
************************************************************************************************/


#include <GSL.h>

#include <Mouse.h>
#include <SoundManager.h>
#include <OnlineWorld.h>

#include "..\Gersang\message.h"
#include "OnlineHelp.h"
#include "OnlineMegaText.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include "OnlineVillageStruct-Make.h"
#include "OnlineTradeBook.h"
#include "OnlineMiniMap.h"
#include	"OnlineListControl.h"
#include "OnlineInventory.h"
#include "OnlineNumberControl.h"
#include "OnlineFieldEffect.h"
#include "OnlineETC.h"
#include "OnlineFont.h"
#include "OnlineSMS.h"		// actdoll (2004/06/15 10:46) : SMS �� ������ �߰�
#include "OnlinePannel.h"
#include "OnlineNoticeList.h"
#include "OnlineClient.h"
#include "OnlineKeyboard.h"
#include "OnlineVillage.h"
#include "OnlineFieldChat.h"
#include "OnlineMsgBox.h"
#include "OnlineFieldArea.h"
#include "OnlineVillageManager.h"
#include "OnlineTrade.h"
#include "OnlineMessenger.h"
#include "OnlineTip.h"
#include "OnlineParty.h"
#include "OnlineMercenary.h"

#ifdef _OGG_PLAY
#include "BgmMgr_PreHeader.h"
#endif

#define ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE		1

extern	SoundManager	   *g_pSoundManager;
extern _InputDevice				IpD;

//----------------------------------------------------------------------------------------------------------------
//	����	:	������.
//----------------------------------------------------------------------------------------------------------------
OnlineVillage::OnlineVillage()
{
	m_fStructLeaveVillageLeave = FALSE;

	ZeroMemory(szName, sizeof(szName));
	ZeroMemory(szStructName, sizeof(szStructName));
	ZeroMemory(siStructCode, sizeof(siStructCode));	

	BackImageSpr       = NULL;
	BackImageSprWidth  = 0;
	BackImageSprHeight = 0;
	ZeroMemory(StructSpr,     sizeof(XSPR) * ON_MAX_VIL_STRUCTNUM);
	ZeroMemory(StructSprAni,  sizeof(XSPR) * ON_MAX_VIL_STRUCTNUM * 3);
	ZeroMemory(StructSprBack, sizeof(XSPR) * ON_MAX_VIL_STRUCTNUM);
	ZeroMemory(StructSprBackMoveXPosition, sizeof(SI16) * ON_MAX_VIL_STRUCTNUM);
	ZeroMemory(StructSprBackMoveYPosition, sizeof(SI16) * ON_MAX_VIL_STRUCTNUM);

	siKind								=	0;	
	siNationCode                        =   0;
	siStructNum							=	0;
	fDead                               =   FALSE;
	siSubStructButtonNum				=	0;
	BackgroundMusic						=	0;
	bMousePush							=	FALSE;
	pMyOnlineWorld						=	NULL;		
	psiSubStructCode					=	NULL;	
	pVillageStructManager				=	NULL;
	pExit								=	NULL;
	ppStruct							=	NULL;
	pMakeList							=	NULL;
	siMakeStatus						=	0;
	siSkip								=   0;	


	NowScrollX  = 0.0f;
	NowScrollY  = 0.0f;
	NowScrollXI = 0;
	NowScrollYI = 0;
	dwDrawTime	= 0;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//----------------------------------------------------------------------------------------------------------------
OnlineVillage::~OnlineVillage()
{
	Free();
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ.
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineVillage::Init(UI16 uiVillageCode, OnRecvVillageInfo* lpOnRecvVillageInfo, cltOnlineWorld *pOnlineWorld)
{
	CHAR						szStructFileName[1024];
	char						TempBuffer[256];
	SI32						i,j;
	SI32						ix, iy;
	OnlineButtonFont			ButtonFontInfo = {0, 1, 2, FALSE, FALSE, FALSE};
	VillageType					*pVillageType;	
	SI32						siX, siY;	
	UI08						*pTemp = NULL;
	FILE*						fp;

	pMyOnlineWorld	=	pOnlineWorld;	
	pVillageHeader	=	pOnlineWorld->pOnlineVillageManager->GetVillageByCode(uiVillageCode);
	siNationCode    =   pOnlineWorld->GetVillageStateCode(uiVillageCode);
	fDead           =   FALSE;

#ifdef _OGG_PLAY
	switch(siNationCode)
	{
		case ON_COUNTRY_JAPAN:
			GetBgmMgr()->Play(OGGBGM_FILENAME_JAPANVIALLGE, TRUE);
			break;

		case ON_COUNTRY_CHINA:
			GetBgmMgr()->Play(OGGBGM_FILENAME_CHINAVIALLGE, TRUE);
			break;

		case ON_COUNTRY_TAIWAN:
			GetBgmMgr()->Play(OGGBGM_FILENAME_TAIWANVIALLGE, TRUE);
			break;

		case ON_COUNTRY_KOREA:
		default:
			GetBgmMgr()->Play(OGGBGM_FILENAME_KOREAVIALLGE, TRUE);
			break;
	}
#else
	// ��� ������ �ٽ� �ε� �Ѵ�.
	if(g_pSoundManager)		
	{
		g_pSoundManager->StopSound(pMyOnlineWorld->GetSoundID());	
	}

	switch(siNationCode)
	{
		case ON_COUNTRY_JAPAN:
			BackgroundMusic	= g_pSoundManager->AddPlayID( "music\\japanvillage.YAV", FALSE, TRUE, FALSE );
			break;

		case ON_COUNTRY_CHINA:
			BackgroundMusic	= g_pSoundManager->AddPlayID( "music\\japanvillage.YAV", FALSE, TRUE, FALSE );
			break;

		case ON_COUNTRY_TAIWAN:
			BackgroundMusic	= g_pSoundManager->AddPlayID( "music\\taiwanvillage.YAV", FALSE, TRUE, FALSE );
			break;

		case ON_COUNTRY_KOREA:
		default:
			BackgroundMusic	= g_pSoundManager->AddPlayID( "music\\koreavillage.YAV", FALSE, TRUE, FALSE );
			break;
	}
	g_pSoundManager->PlaySound( BackgroundMusic );
#endif

	m_fStructLeaveVillageLeave = FALSE;

	// ��� â�� �ݾ� �ش�.
	LeaveMiniMapFlag = FALSE;
	if( pMyOnlineWorld->pOnlineMiniMap->IsActive() )		{pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE ); LeaveMiniMapFlag = TRUE;}
	else													LeaveMiniMapFlag = FALSE;

	if(pMyOnlineWorld->pOnlineTip->IsAction())		pMyOnlineWorld->pOnlineTip->SetAction(FALSE);

	// ������ �̸�
	strcpy(szName, pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));
	pMyOnlineWorld->pOnlinePannel->SetVillagePos(pVillageHeader->siX, pVillageHeader->siY);


	// ������ ��� �׸��� �ε��Ѵ�.
	bIsNight	=	pMyOnlineWorld->IsNight();
	LoadBackImageSprite( bIsNight );	
	

	// ���� �ǹ����� ��ǥ ������ ���´�,	
	pVillageType		=	pOnlineWorld->pOnlineVillageManager->GetVillageStructPosType(pVillageHeader->siType, siNationCode);	

	// �̹��� ��ư�� 
	ppStruct			=	new UI08*[ON_MAX_VIL_STRUCTNUM];

	// ���� ���� �ִ� �ǹ� ���� ��ư�� �����Ѵ�.
	for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
	{		
		ppStruct[i]			= NULL;

		// �ش� �ǹ��� �׸��� �ε��Ѵ�.
		if(pVillageHeader->szVillage[i][0] != NULL)
		{	
			if(pVillageHeader->siStructureKinds[i] == 0)
			{
				clGrp.Error(pOnlineWorld->pOnlineText->Get(ON_TEXT_ERROR_NOTKNOWSTRUCT), "%s", pVillageHeader->szVillage[i]);
			}

			switch(siNationCode)
			{
				case ON_COUNTRY_JAPAN:
					sprintf(szStructFileName, "Online\\VillageStruct\\Japan\\%s", pVillageHeader->szVillage[i]);
					break;

				case ON_COUNTRY_CHINA:
					sprintf(szStructFileName, "Online\\VillageStruct\\China\\%s", pVillageHeader->szVillage[i]);
					break;

				case ON_COUNTRY_TAIWAN:
					sprintf(szStructFileName, "Online\\VillageStruct\\Taiwan\\%s", pVillageHeader->szVillage[i]);
					break;

				case ON_COUNTRY_KOREA:
				default:
					sprintf(szStructFileName, "Online\\VillageStruct\\Korea\\%s", pVillageHeader->szVillage[i]);
					break;
			}

			clGrp.LoadXspr(szStructFileName, StructSpr[i]);

			// Make Animation Sprite
			ZeroMemory(TempBuffer, 256);
			if(MakeStructAnimationSprite(szStructFileName, (char*)TempBuffer, 0))
			{
				fp = fopen((char*)TempBuffer, "rb");
				if(fp)
				{
					fclose(fp);
					clGrp.LoadXspr((char*)TempBuffer, StructSprAni[i][0]);
				}
			}			
			if(MakeStructAnimationSprite(szStructFileName, (char*)TempBuffer, 1))
			{
				fp = fopen((char*)TempBuffer, "rb");
				if(fp)
				{
					fclose(fp);
					clGrp.LoadXspr((char*)TempBuffer, StructSprAni[i][1]);
				}
			}			
			if(MakeStructAnimationSprite(szStructFileName, (char*)TempBuffer, 2))
			{
				fp = fopen((char*)TempBuffer, "rb");
				if(fp)
				{
					fclose(fp);
					clGrp.LoadXspr((char*)TempBuffer, StructSprAni[i][2]);
				}
			}			

			//=========================================================================================
			ppStruct[i] = new UI08[ StructSpr[i].Header.Xsize * StructSpr[i].Header.Ysize ];
			pTemp		= ppStruct[i];

			if( clGrp.LockSurface( ppStruct[i], StructSpr[i].Header.Xsize, StructSpr[i].Header.Ysize ) == TRUE )
				clGrp.PutImageT( 0, 0, StructSpr[i].Header.Xsize, StructSpr[i].Header.Ysize, &StructSpr[i].Image[ StructSpr[i].Header.Start[ 0 ] ] );

			for( iy = 0 ; iy<StructSpr[i].Header.Ysize ; iy++ )
				for( ix = 0; ix<StructSpr[i].Header.Xsize ; ix++) 
				{
					if( pTemp[ iy * StructSpr[i].Header.Xsize + ix ] != TRANSCOLOR )
						pTemp[ iy * StructSpr[i].Header.Xsize + ix ]	=	ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE;
				}

			//=========================================================================================

			// ��ư�� ��ǥ�� ���´�.
			siX								=	pVillageType->Xpos[i];
			siY								=	pVillageType->Ypos[i];
			BStruct[i].Create(siX, siY, StructSpr[i].Header.Xsize, StructSpr[i].Header.Ysize, "", ppStruct[i], ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE, BUTTON_PUT_CENTER );
		}		
	}
	
	// �������� �ǹ����� �����ϴ� �Ŵ��� ��ü�� �����Ѵ�.
	pVillageStructManager	=	new	VillageStructManager;

	// �������� �ǹ����� �����ϴ� �Ŵ��� ��ü�� �ʱ�ȭ �Ѵ�.
	pVillageStructManager->Init(uiVillageCode, pVillageHeader->siStructureKinds, pOnlineWorld);

	// ���� ��ư�� �����.	
	pExitSpr  =	pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_EXITBUTTON );
	
	// �̹��� ��ư��
	pExit = new UI08[ pExitSpr->Header.Xsize * pExitSpr->Header.Ysize ];

	if( clGrp.LockSurface( pExit, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize ) == TRUE )
		clGrp.PutImageT( 0, 0, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize, &pExitSpr->Image[ pExitSpr->Header.Start[ 0 ] ] );

	for( j=0 ; j<pExitSpr->Header.Ysize; j++)
		for( i=0 ; i<pExitSpr->Header.Xsize ; i++ )
		{
			if( pExit[ j * pExitSpr->Header.Xsize + i ] != TRANSCOLOR )
				pExit[ j * pExitSpr->Header.Xsize + i ]	=	ON_PANNEL_BUTTON_CHECK_SAMPLE_VALUE;
		}


	siX								=	pVillageType->ExitXpos;
	siY								=	pVillageType->ExitYpos;

	// �̹��� �ε�
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Town_Wallpaper.Spr", m_WallPaperImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Town_Top_Panel.Spr", m_TopImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\Town_Top_Board.Spr", m_TopBoardImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_EXIT_BASE.Spr", m_ExitBackImage);
	clGrp.LoadXspr( "Online\\GameSpr\\Village\\TOWN_EXIT_button.Spr", m_ExitImage);

	BExit.Create(691, 474, m_ExitImage.Header.Xsize, m_ExitImage.Header.Ysize, pMyOnlineWorld->pOnlineText->Get(8000529)/*pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_OUT)*/,  BUTTON_PUT_LEFT, TRUE);

	CopyMemory(&m_OnRecvVillageInfo, lpOnRecvVillageInfo, sizeof(OnRecvVillageInfo));

	// Game Status�� �����Ѵ�.
	siStatus					=	ON_VILLAGE_STATUS_IN_VILLAGE;		

	// �⺻ ���콺�� �����Ѵ�.
	pMyOnlineWorld->pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);

	// Sign Animtion ó��
	m_VillageSignAnimationFlag = 1;
	m_VillageSignTickCount     = GetTickCount();

	Scroll(0.0f, (float)(BackImageSprHeight - 600));

	/////////////////////////////////////������ ����
	pMakeList	=	new OnlineVillageStructMake;
	pMakeList->Init( pOnlineWorld );

	// ��� â�� �ݴ´�
	if( pMyOnlineWorld->pOnlineInventory->IsActive() )	pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
	if( pMyOnlineWorld->pOnlineTradeBook->IsAction() )	pMyOnlineWorld->pOnlineTradeBook->SetAction( FALSE );
	if( pMyOnlineWorld->pOnlineMiniMap->IsActive() )	pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE );

	pMyOnlineWorld->pOnlineHelp->Initialize();
	pMyOnlineWorld->pOnlineMsgBox->Initialize();

	pMyOnlineWorld->pOnlineMiniMap->MakeTargetList();

	// ���� �ŷ� ���̶��..
	pMyOnlineWorld->pOnlineTrade->m_pMsgBox->Initialize();

/*	if( pMyOnlineWorld->pOnlineParty->GetPartyCharBaseInfo() == TRUE)
	{
		pMyOnlineWorld->pOnlineParty->SetPartyCharInfo(FALSE);
	}
*/
}
//----------------------------------------------------------------------------------------------------------------
//	����	:	����
//----------------------------------------------------------------------------------------------------------------
VOID	OnlineVillage::Free()
{
	// ��� ������ �ٽ� �ε� �Ѵ�.
	g_pSoundManager->DelPlayID(BackgroundMusic);
	pMyOnlineWorld->SetBackGroundMusic(siNationCode, TRUE);

	SI32	i;

	siNationCode = 0;
	fDead        = FALSE;

	NowScrollX  = 0.0f;
	NowScrollY  = 0.0f;
	NowScrollXI = 0;
	NowScrollYI = 0;

	// ��� �׸��� �޸𸮸� �����Ѵ�.
	if(BackImageSpr)
	{
		BackImageSpr->Release();
	}
	BackImageSpr       = NULL;
	BackImageSprWidth  = 0;
	BackImageSprHeight = 0;

	// �ǹ����� �׸��� �޸𸮸� �����Ѵ�.
	if( ppStruct != NULL )
	{
		for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			if(IsVaildStruct(i) == TRUE)
			{
				if(StructSpr[i].Image)
					clGrp.FreeXspr(StructSpr[i]);

				if(StructSprAni[i][0].Image)
					clGrp.FreeXspr(StructSprAni[i][0]);

				if(StructSprAni[i][1].Image)
					clGrp.FreeXspr(StructSprAni[i][1]);

				if(StructSprAni[i][2].Image)
					clGrp.FreeXspr(StructSprAni[i][2]);

				if(StructSprBack[i].Image)
					clGrp.FreeXspr(StructSprBack[i]);

				if(ppStruct[i])
					delete [] ppStruct[i];

				StructSpr[i].Image	     = NULL;
				StructSprAni[i][0].Image = NULL;
				StructSprAni[i][1].Image = NULL;
				StructSprAni[i][2].Image = NULL;
				StructSprBack[i].Image   = NULL;
				ppStruct[i]			     = NULL;

			}	
		}

		delete [] ppStruct;
		ppStruct = NULL;
	}

	if(psiSubStructCode)
	{
		delete [] psiSubStructCode;
		psiSubStructCode	=	NULL;
	}

	if(pVillageStructManager)
	{
		pVillageStructManager->Free();

		delete pVillageStructManager;
		pVillageStructManager	=	NULL;
	}

	if( pExit )
	{
		delete [] pExit;
		pExit	=	NULL;
	}

	clGrp.FreeXspr(m_WallPaperImage);
	clGrp.FreeXspr(m_TopImage);
	clGrp.FreeXspr(m_TopBoardImage);
	clGrp.FreeXspr(m_ExitImage);
	clGrp.FreeXspr(m_ExitBackImage);

	if( pMakeList )
	{
		pMakeList->Free();
		delete pMakeList;
		pMakeList	=	NULL;
	}

	if( pMyOnlineWorld->pOnlineInventory->IsActive() )		pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	���� �Լ�
//----------------------------------------------------------------------------------------------------------------
BOOL	OnlineVillage::Poll(UI16 uiClientRet)
{
	BOOL	bRet = FALSE;
	SI16	siOrder = 0;	
	SI16	siTempStatus = 0;		

	// ��/���� ����ȴٸ� ��� �׸��� ������ �ε��Ѵ�.
	if( pMyOnlineWorld->IsNight() != bIsNight )
	{		
		bIsNight	=	pMyOnlineWorld->IsNight();

		LoadBackImageSprite( bIsNight );		
	}

	// Ű������ �޽����� ���� ���¸� �˾ƿ´�.
	pMyOnlineWorld->pOnlineKeyboard->Action();

	// ���콺�� �����Ѵ�.
	pMyOnlineWorld->pIOnlineMouse->Action();
	pMyOnlineWorld->pOnlineHelp->Initialize();
	pMyOnlineWorld->pOnlineMsgBoxPay->Process();

	if(siStatus == ON_VILLAGE_STATUS_IN_VILLAGE)
		pMyOnlineWorld->pOnlineInventory->Action();

	// �������� List
//	if(pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->IsFocus() == FALSE)
	if(Handler_OnlineEditControlEx::IsFocus(pMyOnlineWorld->pOnlinePannel->GetHOECChat()) == FALSE)
		pMyOnlineWorld->pOnlineNoticeList->Poll(GETSCREENSURFACE(pMyOnlineWorld));

	if( pMyOnlineWorld->IsFindNow() )	pMyOnlineWorld->FindTradeBook();

	pMyOnlineWorld->pOnlineFieldEffect->Pool();

	// �ǳ� Poll
	if(!pMyOnlineWorld->m_bMiniGameOn)
		pMyOnlineWorld->pOnlinePannel->Poll();

	DWORD dwTemp = timeGetTime();

	switch(siStatus)
	{
	case ON_VILLAGE_STATUS_IN_VILLAGE:
		// ���� ���� �ȿ� �ִ�.	
		if( (dwTemp - dwDrawTime ) < 50 )
			siTempStatus = InVillagePoll(uiClientRet);
		
		dwTemp = dwDrawTime;
		dwTemp = timeGetTime();

		// �޽���
		pMyOnlineWorld->pOnlineMessenger->Action();

		if( (dwTemp - dwDrawTime) < 30 || siSkip < 5 )
		{
			ViewVillagePoll(FALSE);
			DrawVillageInfo(GETSCREENSURFACE(pMyOnlineWorld), FALSE, FALSE);
		}

		if(siTempStatus != 0)
		{
			if(siTempStatus == ON_VILLAGE_STATUS_IN_EXIT)	bRet = TRUE;
			else											siStatus = siTempStatus;	
		}
		break;

	case ON_VILLAGE_STATUS_IN_STRUCTURE:		
		// �켱 �������� ������ �׸���.(����� �ӽ÷� �ǿ��� ó���Ѵ�.)
		if( (dwTemp - dwDrawTime) < 30 || siSkip < 5 )
		{
			if( siMakeStatus != ON_VIL_ST_KIND_WEAPON || siMakeStatus != ON_VIL_ST_KIND_ARMOR || siMakeStatus != ON_VIL_ST_KIND_PASTURE || 
				siMakeStatus != ON_VIL_ST_KIND_FARM || siMakeStatus != ON_VIL_ST_KIND_FACTORY )
				ViewVillagePoll(TRUE);
		}

		dwDrawTime		=	dwTemp;
		siTempStatus	=	InStructurePoll();
		pMyOnlineWorld->pOnlineMessenger->Action();

		if(siTempStatus != 0)
		{
			if(m_fStructLeaveVillageLeave)
			{
				m_VillageSignAnimationFlag = 2;
				m_VillageSignFlag          = 0xff;
				pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
				pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);

				siStatus = siTempStatus;
				break;
			}

			if(siTempStatus == ON_VILLAGE_STATUS_IN_EXIT)	bRet = TRUE;
			else											siStatus = siTempStatus;
		}				
		break;
	}

	pMyOnlineWorld->m_pMercenary->Action();

	// �ǳ� ���
	if(!pMyOnlineWorld->m_bMiniGameOn)
	{
		// �̴ϸ�
		pMyOnlineWorld->pOnlineMiniMap->Pool();
		pMyOnlineWorld->pOnlineMiniMap->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		pMyOnlineWorld->pOnlinePannel->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		if(siStatus == ON_VILLAGE_STATUS_IN_VILLAGE)
		{
			// �뺴
			pMyOnlineWorld->m_pMercenary->Draw(GETSCREENSURFACE(pMyOnlineWorld));
			
			// �κ��丮.
			if(pMyOnlineWorld->pOnlineInventory->IsActive())
			{
				pMyOnlineWorld->pOnlineInventory->SetFlagVillage(TRUE);
				pMyOnlineWorld->pOnlineInventory->Draw(GETSCREENSURFACE(pMyOnlineWorld));
			}
		}
	}
	

	pMyOnlineWorld->pOnlineNumPad->pNumberControl->Draw(GETSCREENSURFACE(pMyOnlineWorld));	
	pMyOnlineWorld->pOnlineHelp->Show( GETSCREENSURFACE(pMyOnlineWorld));
	pMyOnlineWorld->pOnlineMsgBox->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	dwDrawTime = dwTemp;
	dwTemp     = timeGetTime();
	if( ((dwTemp - dwDrawTime) < 50) || siSkip < 5 )
	{	
		pMyOnlineWorld->pOnlineMessenger->Draw(GETSCREENSURFACE(pMyOnlineWorld));

		// ���콺�� �׷��ش�.	
		if(clGrp.LockSurface(GETSCREENSURFACE(pMyOnlineWorld)) == TRUE)
		{
			if(!pMyOnlineWorld->m_bMiniGameOn)
				pMyOnlineWorld->pIOnlineMouse->Draw();		
			clGrp.UnlockSurface(GETSCREENSURFACE(pMyOnlineWorld));
		}		

		HDC	hdc;

		if(GETSCREENSURFACE(pMyOnlineWorld)->GetDC(&hdc) == DD_OK)
		{
			//if(pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->IsFocus() == FALSE)
			if(Handler_OnlineEditControlEx::IsFocus(pMyOnlineWorld->pOnlinePannel->GetHOECChat()) == FALSE)
			{
				SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
				pMyOnlineWorld->pOnlineNoticeList->Draw(hdc);
			}
			
			GETSCREENSURFACE(pMyOnlineWorld)->ReleaseDC( hdc );
		}

		::ShowMessage(100, 50, GETSCREENSURFACE(pMyOnlineWorld));
		pMyOnlineWorld->pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(pMyOnlineWorld));
		pMyOnlineWorld->UpdateScreen(GETSCREENSURFACE(pMyOnlineWorld));	

		siSkip = 0;
	}
	else
		siSkip++;

	dwDrawTime = dwTemp;
	if(IpD.LeftPressSwitch)	 bMousePush = TRUE;	
	else					 bMousePush = FALSE;	

	return bRet;
}
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������ ID.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	OnlineVillage::GetID()
{
	return siID;
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ְ� ������ ID ���
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
char*	OnlineVillage::GetMaxInvestmentID(void)
{
	return (char*)m_OnRecvVillageInfo.m_MaxInvestmentID;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� �ǹ��� ��ư�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineVillage::CreateSubStructButton()
{
	/*
	VillageStructDataCode		TempStructDataCode;
	SI32						i;
	XSPR						*pButtonSpr;

	if(psiSubStructCode)
	{
		delete [] psiSubStructCode;
		psiSubStructCode	=	NULL;
	}
	
	psiSubStructCode		=	new SI32[siSubStructButtonNum];	
	*/
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� �ǹ����� �����ϴ� �����ڸ� �����Ѵ�.,
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineVillage::CreateVillageStructManager(SI32 siStructID)
{

	// ��ư�� �����Ѵ�.
	CreateSubStructButton();	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� �ǹ��� ��ư�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	OnlineVillage::FreeSubStructButton()
{ 
	/*
	// ������ ������� �ʴ´ٰ� �������ش�.
	SubBorderInfo.siBorderSprIndex	=	-1;	


	if(psiSubStructCode)
	{
		delete [] psiSubStructCode;
		psiSubStructCode	=	NULL;
	}	
	*/
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ȿ�� �ǹ����� �˾ƿ´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillage::IsVaildStruct(SI32 siIndex)
{
	if(StructSpr[siIndex].Image != NULL) return TRUE;
	else                                 return FALSE;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �ȿ� �������� ó��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineVillage::InVillagePoll(UI16 uiClientRet)
{
	SI16	siLocalStatus = 0;
	SI32	i;
	POINT	ptMouse;
	BOOL	fLiveMouse;

	// �޽����ڽ��� �� ������ �ǹ� ����.
	if(pMyOnlineWorld->pOnlineMsgBox->IsActive())	return 0;

	// ���콺 ��ǥ�� ���´�.
	pMyOnlineWorld->pIOnlineMouse->GetPos( &ptMouse );
	// ������ �ʱ�ȭ �Ѵ�.

	// Interface�� ���콺 üũ
	fLiveMouse = TRUE;
	if(pMyOnlineWorld->pOnlineFieldArea->IsFieldArea((SI16)ptMouse.x, (SI16)ptMouse.y) == FALSE) fLiveMouse = FALSE;

	// ���� ���콺�� ȭ�� ���� ������ ��ũ�� ó��
	if(ptMouse.x < 100)
	{
		if(NowScrollX < 1.0f)      Scroll(0.0f, NowScrollY);
		else if(NowScrollX > 1.0f) Scroll(NowScrollX - (NowScrollX / 24.0f), NowScrollY);
	}
	if(ptMouse.x > 700)
	{
		if(NowScrollX > (float)(BackImageSprWidth - 801))      Scroll((float)(BackImageSprWidth - 800), NowScrollY);
		else if(NowScrollX < (float)(BackImageSprWidth - 801)) Scroll(((float)((BackImageSprWidth - 800) - NowScrollX) / 24.0f) + NowScrollX, NowScrollY);
	}

	if(ptMouse.y < 500)
	{
		if(NowScrollY < 1.0f)      Scroll(NowScrollX, 0.0f);
		else if(NowScrollY > 1.0f) Scroll(NowScrollX, NowScrollY - (NowScrollY / 24.0f));
	}
	if(ptMouse.y > 550)
	{
		if(NowScrollY > (float)(BackImageSprHeight - 601))      Scroll(NowScrollX, (float)(BackImageSprHeight - 600));
		else if(NowScrollY < (float)(BackImageSprHeight - 601)) Scroll(NowScrollX, ((float)((BackImageSprHeight - 600) - NowScrollY) / 24.0f) + NowScrollY);
	}

	// ��ư���� ���������� �ش� ������ ������ üũ�Ѵ�.
	if(fLiveMouse && pMakeList->IsActive() == FALSE )
	{
		for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{			
			// �켱 ��ȿ�� �ǹ����� �˾ƿ´�.
			if(ppStruct[i] != NULL )
			{
				//  �ǹ� ���� ���콺�� ��� ������ �˾ƺ���.
				if( BStruct[i].IsInMouse() )
				{
					switch( pVillageHeader->siStructureKinds[i]	)
					{
						case ON_VIL_ST_KIND_BARRACK:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BARRACK ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_BARRACK ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_MARKET:
							{
								char		TempBuffer[256];

								sprintf((char*)TempBuffer, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_MARKET ), szName);
								pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MARKET ), (char*)TempBuffer, (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							}
							break;

						case ON_VIL_ST_KIND_BANK:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_BANK ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_HOSPITAL:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HOSPITAL ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_HOSPITAL ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_RING:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HOSPITAL ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_HOSPITAL ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_INN:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INN ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_INN ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_WHARF:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WHARF ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_WHARF ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_FARM:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FARM ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_FARM ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_GOVERNMENT:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GOVERNMENT ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_GOVERNMENT ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_WSTORE:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WSTORE ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_WSTORE ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_DSTORE:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_DSTORE ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_DSTORE ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_FSTORE:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FSTORE ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_FSTORE ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;
					
						case ON_VIL_ST_KIND_WEAPON:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_WEAPON ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_WEAPON ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_ARMOR:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_ARMOR ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_HELP_ARMOR ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_PASTURE:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_PASTURE ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_HELP_RANCH ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_FACTORY:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_FACTORY ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MAKE_HELP_FACTORY ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_GBARRACK:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GBARRACK ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_GBARRACK ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_MBARRACK:
							pMyOnlineWorld->pOnlineHelp->SetText( pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MBARRACK ), pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HELP_MBARRACK ), (SI16)ptMouse.x, (SI16)ptMouse.y, ON_HELP_LEFT | ON_HELP_VCENTER, ON_HELP_WHITE );
							break;

						case ON_VIL_ST_KIND_UNKNOWN:	
							clGrp.Error( "", pMyOnlineWorld->pOnlineText->Get( ON_TEXT_UNKNOWN_STRUCTURE ) );
							break;
					}
				}
				
				if(m_fStructLeaveVillageLeave == FALSE)
				{
					if(BStruct[i].Process(bMousePush) == TRUE) //&& !(pMyOnlineWorld->pOnlineMessenger->IsAction() && !pMyOnlineWorld->pOnlineMessenger->IsMini()))		
					{				
						if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_FARM )
						{
							pMakeList->SetList( ON_VIL_ST_KIND_FARM, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );					
							siMakeStatus	=	i;
						}
						else if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_WEAPON )
						{
							pMakeList->SetList( ON_VIL_ST_KIND_WEAPON, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );
							siMakeStatus	=	i;
						}
						else if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_ARMOR )
						{
							pMakeList->SetList( ON_VIL_ST_KIND_ARMOR, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );
							siMakeStatus	=	i;

						}
						else if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_PASTURE )
						{
							pMakeList->SetList( ON_VIL_ST_KIND_PASTURE, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );
							siMakeStatus	=	i;

						}
						else if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_FACTORY )
						{
							pMakeList->SetList( ON_VIL_ST_KIND_FACTORY, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );
							siMakeStatus	=	i;
						}
/*						else if( pVillageHeader->siStructureKinds[i] == ON_VIL_ST_KIND_INN )
						{
						}
*/
						else
						{
							// �ǹ� ��ư�� �������� üũ�ϰ� �����ٸ��� �� ������ ����.
							// Status ���� �Ű��ش�.	
							m_VillageSignAnimationFlag = 2;
							m_VillageSignFlag          = i;
						}
					}
				}
			}		
		}
	}

	// '������' ��ư�� �������� üũ�Ѵ�.
/*	if( BExit.Process(bMousePush) || ( LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) ) )
	{
		// ���� ������ ������.
		m_VillageSignAnimationFlag = 2;
		m_VillageSignFlag          = 0xff;
		pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
		pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
	}
*/

	// robypark 2004/8/25 18:10
	// ��ư�� �켱 �˻��Ͽ� ó���Ѵ�.
	if ( BExit.Process(bMousePush) )
	{
		// ���� ������ ������.
		m_VillageSignAnimationFlag = 2;
		m_VillageSignFlag          = 0xff;
		pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
		pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
	}
	// robypark 2004/8/25 18:10
	// ���� �ü� ����� Ȱ��ȭ �� ��� ESCŰ�� ������ ���� ������ ������ ���� �ذ�
	else if ( LOBYTE(pMyOnlineWorld->pOnlineKeyboard->GetKeyStatus(VK_ESCAPE) ) )
	{
		// ���� �ü� ����� Ȱ��ȭ�� ��쿡�� ��� ������ �ݾ��ش�.
		if(pMakeList->IsActive())
		{
			pMakeList->CloseList_ProductionEstablishments();
		}
		else
		{
			// ���� ������ ������.
			m_VillageSignAnimationFlag = 2;
			m_VillageSignFlag          = 0xff;
			pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
			pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
		}
	}

	// Sign Animtion ó��
	switch(m_VillageSignAnimationFlag)
	{
		case 0:
			m_VillageSignTickCount     = GetTickCount();
			break;

		case 1:
			if((GetTickCount() - m_VillageSignTickCount) > 450)
			{
				m_VillageSignAnimationFlag = 0;
			}
			break;

		case 2:
			if(m_VillageSignFlag == 0xff)
			{
				if(m_fStructLeaveVillageLeave == FALSE)
				{
					if((GetTickCount() - m_VillageSignTickCount) > 450)
					{
//						pMyOnlineWorld->GotoField();
						siLocalStatus	           = ON_VILLAGE_STATUS_IN_EXIT;
						m_VillageSignAnimationFlag = 3;

						pMyOnlineWorld->pOnlineHelp->Initialize();
					}
				}
				else
				{
//					pMyOnlineWorld->GotoField();
					siLocalStatus	           = ON_VILLAGE_STATUS_IN_EXIT;
					m_VillageSignAnimationFlag = 3;

					pMyOnlineWorld->pOnlineHelp->Initialize();
				}
			}
			else
			{
				pVillageStructManager->EnterStructure(m_VillageSignFlag);
				siLocalStatus              = ON_VILLAGE_STATUS_IN_STRUCTURE;
				m_VillageSignAnimationFlag = 3;
			}
			break;

		case 3:
			break;
	}

	return siLocalStatus;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �� ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void	OnlineVillage::ViewVillagePoll(BOOL fStructure)
{
	SI16	siLocalStatus = 0;
	SI32	i;
	SI16	SignStartPos;
	DWORD	TickCount;
	SI16	TempX;
	SI16	TempY;
	float	TempFloat;
	BOOL	fRenderFlag;
	SI32	TempAddPosText;

	fRenderFlag    = FALSE;
	TempAddPosText = 0;

	//=============================================================================================
	// ��Ȳ�� ���� ����
	if(fStructure == TRUE)
	{
		BExit.SetAction(FALSE);
	}

	//=============================================================================================
	// Sign Animtion ó��
	SignStartPos = 0;
	TickCount    = 0;
	if(fStructure == FALSE)
	{
		switch(m_VillageSignAnimationFlag)
		{
			case 0:
				break;

			case 1:
				TickCount    = GetTickCount() - m_VillageSignTickCount;
				if(TickCount > 450) TickCount = 450;

				TempFloat    = (float)sin(((float)(TickCount) / 5.0f) * 0.017444f);
				SignStartPos = -222 + (int)(TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * 222.0f);
				break;

			case 2:
				if(m_fStructLeaveVillageLeave == FALSE)
				{
					if(m_VillageSignFlag == 0xff)
					{
						TickCount    = GetTickCount() - m_VillageSignTickCount;
						if(TickCount > 450) TickCount = 450;

						TempFloat    = 1.0f - (float)sin(((float)(450 - TickCount) / 5.0f) * 0.017444f);
						SignStartPos = -(int)(TempFloat * 222.0f);
					}
				}
				break;

			case 3:
				SignStartPos = -((int)(222.0f));
				break;
		}
	}

	// ��� �׸��� ��´�.
	RECT		DrawRect;

	DrawRect.left   = NowScrollXI;
	DrawRect.top    = NowScrollYI;
	DrawRect.right  = NowScrollXI + 800;
	DrawRect.bottom = NowScrollYI + 600;

	/*
	if(BackImageSpr == NULL)
	{
		int		ClipLeft, ClipTop, ClipRight, ClipBottom;
		XSPR	BackImageBuffer;

		if(clGrp.LoadXspr(szBackImageSprFileName, BackImageBuffer) == FALSE) clGrp.Error(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_BACKGROUND_ROAD_FAIL), "");
		clGrp.CreateOffScreen(BackImageSpr, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, TRUE, FALSE);
		clGrp.ClearSurface(BackImageSpr);
		BackImageSprWidth  = BackImageBuffer.Header.Xsize;
		BackImageSprHeight = BackImageBuffer.Header.Ysize;

		//Ŭ���� ������ �ٲ۴�.
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

		if(clGrp.LockSurface(BackImageSpr))
		{
			clGrp.SetClipArea(0, 0, BackImageSprWidth - 1, BackImageSprHeight - 1);

				
			
			if( pMyOnlineWorld->IsNight() == TRUE )
			{
				// ���� ���̶��
				siBackImageFont		=	0;				
				
			}
			else
			{
				// ���� ���̶��
				siBackImageFont		=	1;					
			}

			// �ش� ��Ʈ�� �׸��� �ִ��� ���� �˻��Ѵ�.
			if( siBackImageFont >= BackImageBuffer.Header.font_number )	siBackImageFont	=	0;

			clGrp.PutSpriteT(0, 0, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, &BackImageBuffer.Image[ BackImageBuffer.Header.Start[ siBackImageFont ] ] );


			clGrp.UnlockSurface(BackImageSpr);
		}

		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
		clGrp.FreeXspr(BackImageBuffer);
	}
	if(BackImageSpr)
	{
		if(BackImageSpr->IsLost() != DDERR_SURFACELOST)
		{
			if(GETSCREENSURFACE(pMyOnlineWorld)->BltFast(0, 0, BackImageSpr, &DrawRect, DDBLTFAST_WAIT) != DD_OK)
			{
				BackImageSpr->Release();
				BackImageSpr = NULL;
			}
		}
		else
		{
			BackImageSpr->Release();
			BackImageSpr = NULL;
		}
	}
	
	*/


	GETSCREENSURFACE(pMyOnlineWorld)->BltFast(0, 0, BackImageSpr, &DrawRect, DDBLTFAST_WAIT);

	if(clGrp.LockSurface(GETSCREENSURFACE(pMyOnlineWorld)) == TRUE)
	{
		clGrp.SetClipArea(0, 0, 799, 599);

		// �ǹ� �ٴ�
		for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			if(ppStruct[i] != NULL)
			{
				if(StructSprBack[i].Image)
				{
					TempX =  BStruct[i].GetX() + StructSprBackMoveXPosition[i];
					TempY =  BStruct[i].GetY() + StructSprBackMoveYPosition[i];
					TempX += (StructSpr[i].Header.Xsize >> 1);
					TempY += (StructSpr[i].Header.Ysize >> 1);
					TempX -= (StructSprBack[i].Header.Xsize >> 1);
					TempY -= (StructSprBack[i].Header.Ysize >> 1);

					clGrp.PutSpriteT(TempX, TempY, StructSprBack[i].Header.Xsize, StructSprBack[i].Header.Ysize, StructSprBack[i].Image);
				}
			}
		}

		// �ǹ� Animation
		for(i = 0 ; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			if(ppStruct[i] != NULL)
			{
				if(fStructure == TRUE) BStruct[i].Put(&StructSpr[i], 0, 0, 0, BUTTON_PUT_NOMOVE);
				else                   BStruct[i].Put(&StructSpr[i], 0, 2, 1, BUTTON_PUT_NOMOVE);

				if(StructSprAni[i][0].Image)
				{
					TempX = BStruct[i].GetX();
					TempY = BStruct[i].GetY();

					clGrp.PutSpriteT(TempX, TempY
						, StructSprAni[i][0].Header.Xsize, StructSprAni[i][0].Header.Ysize
						, &StructSprAni[i][0].Image[StructSprAni[i][0].Header.Start[(GetTickCount() / 200) % StructSprAni[i][0].Header.font_number]]);
				}

				if(StructSprAni[i][1].Image)
				{
					TempX = BStruct[i].GetX();
					TempY = BStruct[i].GetY();

					clGrp.PutSpriteLightImjinCT(TempX, TempY
						, StructSprAni[i][1].Header.Xsize, StructSprAni[i][1].Header.Ysize
						, &StructSprAni[i][1].Image[StructSprAni[i][1].Header.Start[(GetTickCount() / 200) % StructSprAni[i][1].Header.font_number]]);
				}

				if(StructSprAni[i][2].Image)
				{
					TempX = BStruct[i].GetX();
					TempY = BStruct[i].GetY();

					clGrp.PutSpriteT(TempX, TempY
						, StructSprAni[i][2].Header.Xsize, StructSprAni[i][2].Header.Ysize
						, &StructSprAni[i][2].Image[StructSprAni[i][2].Header.Start[(GetTickCount() / 200) % StructSprAni[i][2].Header.font_number]]);
				}
			}
		}

		clGrp.UnlockSurface(GETSCREENSURFACE(pMyOnlineWorld));
	}

	if(fStructure == FALSE)
	{
		if(pMakeList->IsActive())
		{
			pMakeList->Poll(GETSCREENSURFACE(pMyOnlineWorld));
			if( pMakeList->IsEnter() )
			{			
				m_VillageSignAnimationFlag = 2;
				m_VillageSignFlag          = (UI08)siMakeStatus;
			}
			else
			{
				if(pMakeList->IsActive() == FALSE)
				{
					if(m_fStructLeaveVillageLeave)
					{
						m_VillageSignAnimationFlag = 2;
						m_VillageSignFlag          = 0xff;
						pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
						pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
					}
				}
			}
		}

		// ���
		if(pMyOnlineWorld->pOnlineTradeBook->IsAction())
		{
			pMyOnlineWorld->pOnlineTradeBook->Poll();
			pMyOnlineWorld->pOnlineTradeBook->Draw(GETSCREENSURFACE(pMyOnlineWorld));
		}

		// Pannel�� ����Ѵ�.
		pMyOnlineWorld->pOnlinePannel->SetFlagVillage(TRUE, ONLINEPANNEL_DISABLEFLAG_TIP | ONLINEPANNEL_DISABLEFLAG_OPTION | ONLINEPANNEL_DISABLEFLAG_IMOTICON );

		pMyOnlineWorld->pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}

	//=============================================================================================
	// ����
	if(fStructure == TRUE)
	{
		BExit.SetAction(TRUE);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǹ� �ȿ� �������� ó��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineVillage::InStructurePoll(BOOL fVer)
{
	SI32	siLocalStatus = 0;
	BOOL	bRet1 = FALSE, bRet2 = FALSE;
	DWORD	dwTemp = timeGetTime();

	if( (dwTemp - dwDrawTime) < 50 || (siSkip > 5) || (fVer == FALSE))
	{
		bRet1 = pVillageStructManager->Action();
		dwDrawTime = dwTemp;
	}

	dwTemp = timeGetTime();

	if( (dwTemp - dwDrawTime) < 60 || (siSkip > 5) || (fVer == FALSE))
	{
		bRet2 = pVillageStructManager->Poll(GETSCREENSURFACE(pMyOnlineWorld));

		dwDrawTime = dwTemp;
		pMyOnlineWorld->pOnlineMsgBox->Draw(GETSCREENSURFACE(pMyOnlineWorld));
		pMyOnlineWorld->pOnlineMsgBoxPay->Draw(GETSCREENSURFACE(pMyOnlineWorld));
		pMyOnlineWorld->pOnlineNumPad->pNumberControl->Draw(GETSCREENSURFACE(pMyOnlineWorld));
	}
	
	if(bRet1 || bRet2)
	{
		// �ǹ����� ���Դ�.
		// Status ���� �Ű��ش�.
		m_VillageSignAnimationFlag = 0;
		m_VillageSignTickCount     = GetTickCount();
		siLocalStatus	           = ON_VILLAGE_STATUS_IN_VILLAGE;

		pMyOnlineWorld->pOnlineMiniMap->MakeTargetList();
	}

	return siLocalStatus;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������¸� ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineVillage::GetStatus(void)
{
	return siStatus;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Scroll
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillage::Scroll(float x, float y)
{
	SI16	TempCount;

	// ��� ��ư�� ���� ��ġ�� ����
	for(TempCount = 0; TempCount < ON_MAX_VIL_STRUCTNUM; TempCount++)
	{
		if(ppStruct[TempCount])
		{
			BStruct[TempCount].SetX(BStruct[TempCount].GetX() + NowScrollXI);
			BStruct[TempCount].SetY(BStruct[TempCount].GetY() + NowScrollYI);
		}
	}

	// ��ũ��
	NowScrollX  = x;
	NowScrollY  = y;
	NowScrollXI = (SI16)x;
	NowScrollYI = (SI16)y;

	// ��� ��ư�� ���� ��ũ�� ��ġ ���¿� �°� ����
	for(TempCount = 0; TempCount < ON_MAX_VIL_STRUCTNUM; TempCount++)
	{
		if(ppStruct[TempCount])
		{
			BStruct[TempCount].SetX(BStruct[TempCount].GetX() - NowScrollXI);
			BStruct[TempCount].SetY(BStruct[TempCount].GetY() - NowScrollYI);
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Move In Struct
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineVillage::MoveInStruct(SI32 siStructKind, BOOL fStructLeaveVillageLeave, BOOL fNoneSelect)
{
	SI32		siIndex;
	SI32		TempCount;

	// Find Struct
	siIndex = -1;
	for(TempCount = 0; TempCount < ON_MAX_VIL_STRUCTNUM; TempCount++)
	{			
		// �켱 ��ȿ�� �ǹ����� �˾ƿ´�.
		if(pVillageHeader->szVillage[TempCount][0] != NULL)
		{
			//  �ǹ� ���� ���콺�� ��� ������ �˾ƺ���.
			if(pVillageHeader->siStructureKinds[TempCount] == siStructKind)
			{
				siIndex = TempCount;
			}
		}
	}

	// Move In Struct
	m_fStructLeaveVillageLeave = fStructLeaveVillageLeave;

	if(siIndex != -1)
	{
		if(fNoneSelect)
		{
			m_VillageSignAnimationFlag = 0;
			switch(siStructKind)
			{
				case ON_VIL_ST_KIND_ARMOR:
				case ON_VIL_ST_KIND_WEAPON:
				case ON_VIL_ST_KIND_PASTURE:
				case ON_VIL_ST_KIND_FACTORY:
					pMakeList->SetList( siStructKind, (SI16)IpD.Mouse_X, (SI16)IpD.Mouse_Y );
					siMakeStatus	=	siIndex;
					break;

				default:
					pVillageStructManager->EnterStructure(siIndex);
					siStatus                   = ON_VILLAGE_STATUS_IN_STRUCTURE;
					m_VillageSignAnimationFlag = 3;
					break;
			}
		}
		else
		{
			pVillageStructManager->EnterStructure(siIndex);
			siStatus                   = ON_VILLAGE_STATUS_IN_STRUCTURE;
			m_VillageSignAnimationFlag = 3;
		}
	}
	else
	{
		if(m_fStructLeaveVillageLeave)
		{
			// ������ ������.
			m_VillageSignAnimationFlag = 2;
			m_VillageSignFlag          = 0xff;
			pMyOnlineWorld->pOnlineMsgBox->Initialize(); // ���� ������ ������ ����� �޽��� â�� �����ش�. 
			pMyOnlineWorld->pOnlineInventory->SetExitButton(TRUE);
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǹ��� ��� Sprite �̸� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillage::MakeStrcutBackSprite(char* lpName, char* lpSaveName, SI16 Index, SI16 StructType)
{
	if(lpName         == NULL) return FALSE;
	if(lpName[0]      == NULL) return FALSE;
	if(strlen(lpName) <= 4)    return FALSE;

	if(stricmp(&lpName[strlen(lpName) - 4], ".Spr") == 0)
	{
		CopyMemory(lpSaveName, lpName, strlen(lpName) - 4);
		strcpy(&lpSaveName[strlen(lpName) - 4], "Field.Spr");

		switch(StructType)
		{
			case ON_VIL_ST_KIND_PASTURE:
				StructSprBackMoveXPosition[Index] = 54;
				StructSprBackMoveYPosition[Index] = -1;
				return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ǹ��� Animation Sprite �̸� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillage::MakeStructAnimationSprite(char* lpName, char* lpSaveName, int AniType)
{
	if(lpName         == NULL) return FALSE;
	if(lpName[0]      == NULL) return FALSE;
	if(strlen(lpName) <= 4)    return FALSE;

	if(stricmp(&lpName[strlen(lpName) - 4], ".Spr") == 0)
	{
		CopyMemory(lpSaveName, lpName, strlen(lpName) - 4);
		switch(AniType)
		{
			case 0:
				strcpy(&lpSaveName[strlen(lpName) - 4], "_Ani.Spr");
				return TRUE;

			case 1:
				strcpy(&lpSaveName[strlen(lpName) - 4], "_Ani1.Spr");
				return TRUE;

			case 2:
				strcpy(&lpSaveName[strlen(lpName) - 4], "_Ani2.Spr");
				return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ���� �ü��� ������� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineVillageStructMake	*OnlineVillage::GetMakeList()
{
	return pMakeList;
}
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VillageHeader	*OnlineVillage::GetVillageHeader()
{
	return pVillageHeader;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineVillage::MakeVillageInStructor(char* lpChar)
{
	int			i;
	BOOL		FirstFlag;

	FirstFlag = TRUE;
	for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
	{			
		// �켱 ��ȿ�� �ǹ����� �˾ƿ´�.
		if(pVillageHeader->szVillage[i][0] != NULL)
		{
			switch( pVillageHeader->siStructureKinds[i])
			{
				case ON_VIL_ST_KIND_BARRACK:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BARRACK ));
					break;

				case ON_VIL_ST_KIND_MARKET:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MARKET ));
					break;

				case ON_VIL_ST_KIND_BANK:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_BANK ));
					break;

				case ON_VIL_ST_KIND_HOSPITAL:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HOSPITAL ));
					break;
				case ON_VIL_ST_KIND_RING:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_HOSPITAL ));
					break;

				case ON_VIL_ST_KIND_INN:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_INN ));
					break;

				case ON_VIL_ST_KIND_WHARF:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WHARF ));
					break;

				case ON_VIL_ST_KIND_FARM:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FARM ));
					break;

				case ON_VIL_ST_KIND_GOVERNMENT:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GOVERNMENT ));
					break;

				case ON_VIL_ST_KIND_WSTORE:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_WSTORE ));
					break;

				case ON_VIL_ST_KIND_FSTORE:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_FSTORE ));
					break;

				case ON_VIL_ST_KIND_DSTORE:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_DSTORE ));
					break;

				case ON_VIL_ST_KIND_GBARRACK:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_GBARRACK ));
					break;

				case ON_VIL_ST_KIND_MBARRACK:
					if(FirstFlag == FALSE) strcat((char*)lpChar, ", ");
					if(FirstFlag == TRUE)  FirstFlag = FALSE;
					strcat((char*)lpChar, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_MBARRACK ));
					break;
			}
		}		
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �̸��� ������
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
char*	OnlineVillage::GetVillageName(void)
{
	return szName;
}

BOOL	OnlineVillage::InitMapStruct(UI16 VillageCode, SI16 siStructCode, cltOnlineWorld* lpWorld, OnlineVillageInfoData* lpOnlineVillageInfoData)
{
	OnlineButtonFont			ButtonFontInfo = {0, 1, 2, FALSE, FALSE, FALSE};
	VillageType					*pVillageType;	
	UI08						*pTemp = NULL;
//	char						TempBuffer[256];
	XSPR						BackImageBuffer;
	int							ClipLeft, ClipTop, ClipRight, ClipBottom;

	pMyOnlineWorld	=	lpWorld;	
	pVillageHeader	=	lpWorld->pOnlineVillageManager->GetVillageByCode(VillageCode);
	siNationCode    =   lpWorld->GetVillageStateCode(pVillageHeader->uiVillageCode);
	fDead           =   FALSE;

	// ��� â�� �ݾ� �ش�.
	LeaveMiniMapFlag = FALSE;
	if( pMyOnlineWorld->pOnlineMiniMap->IsActive() )		{pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE ); LeaveMiniMapFlag = TRUE;}
	else													LeaveMiniMapFlag = FALSE;

	// ������ �̸�
	strcpy(szName, pMyOnlineWorld->pOnlineText->Get(pVillageHeader->siVillageName));
	pMyOnlineWorld->pOnlinePannel->SetVillagePos(pVillageHeader->siX, pVillageHeader->siY);

	// ������ ��� �׸��� �ε��Ѵ�.
	sprintf(szBackImageSprFileName, "Online\\VillageBack\\%s", pVillageHeader->szConfig);	
	if(clGrp.LoadXspr(szBackImageSprFileName, BackImageBuffer) == FALSE) clGrp.Error(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_BACKGROUND_ROAD_FAIL), "");
	BackImageSpr = NULL;
	if(clGrp.CreateOffScreen(BackImageSpr, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, TRUE, FALSE))
	{
		clGrp.ClearSurface(BackImageSpr);
		BackImageSprWidth  = BackImageBuffer.Header.Xsize;
		BackImageSprHeight = BackImageBuffer.Header.Ysize;

		//Ŭ���� ������ �ٲ۴�.
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

		if(clGrp.LockSurface(BackImageSpr))
		{
			clGrp.SetClipArea(0, 0, BackImageSprWidth - 1, BackImageSprHeight - 1);

			clGrp.PutSpriteT(0, 0, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, BackImageBuffer.Image);
			clGrp.UnlockSurface(BackImageSpr);
		}
		else
		{
			BackImageSpr->Release();
			BackImageSpr = NULL;
		}

		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
	}
	clGrp.FreeXspr(BackImageBuffer);

	// ���� �ǹ����� ��ǥ ������ ���´�,	
	pVillageType		=	lpWorld->pOnlineVillageManager->GetVillageStructPosType(pVillageHeader->siType, siNationCode);	

	// �������� �ǹ����� �����ϴ� �Ŵ��� ��ü�� �����Ѵ�.
	pVillageStructManager	=	new	VillageStructManager;

	// �������� �ǹ����� �����ϴ� �Ŵ��� ��ü�� �ʱ�ȭ �Ѵ�.
	pVillageStructManager->Init(pVillageHeader->uiVillageCode, pVillageHeader->siStructureKinds, lpWorld);

	CopyMemory(&m_OnRecvVillageInfo, &lpOnlineVillageInfoData->m_VillageInfo, sizeof(OnRecvVillageInfo));

	// Game Status�� �����Ѵ�.
	siStatus					=	ON_VILLAGE_STATUS_IN_STRUCTURE;		

	// �⺻ ���콺�� �����Ѵ�.
	pMyOnlineWorld->pIOnlineMouse->SetMouse(ON_MOUSE_BASIC);

	/////////////////////////////////////������ ����
	pMakeList	=	new OnlineVillageStructMake;
	pMakeList->Init( lpWorld );

	// ��� â�� �ݴ´�
	if( pMyOnlineWorld->pOnlineInventory->IsActive() )	pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
	if( pMyOnlineWorld->pOnlineTradeBook->IsAction() )	pMyOnlineWorld->pOnlineTradeBook->SetAction( FALSE );
	if( pMyOnlineWorld->pOnlineMiniMap->IsActive() )	pMyOnlineWorld->pOnlineMiniMap->SetActive( FALSE );
	
	pMyOnlineWorld->pOnlineHelp->Initialize();
	pMyOnlineWorld->pOnlineMsgBox->Initialize();

	pMyOnlineWorld->pOnlineMiniMap->MakeTargetList();

	// ���� �ŷ� ���̶��..
	pMyOnlineWorld->pOnlineTrade->m_pMsgBox->Initialize();

	LeaveMessengerFlag = FALSE;
	if(pMyOnlineWorld->pOnlineMessenger->IsMini())
	{
		pMyOnlineWorld->pOnlineMessenger->SetMini(TRUE);
		LeaveMessengerFlag = TRUE;
	}

	// �ǹ�������
	MoveInStruct(siStructCode, TRUE);

	return TRUE;
}

void	OnlineVillage::FreeMapStruct(void)
{
	SI32	i;

	fDead        = FALSE;

	NowScrollX  = 0.0f;
	NowScrollY  = 0.0f;
	NowScrollXI = 0;
	NowScrollYI = 0;

	// ��� �׸��� �޸𸮸� �����Ѵ�.
	if(BackImageSpr)
	{
		BackImageSpr->Release();
	}
	BackImageSpr       = NULL;
	BackImageSprWidth  = 0;
	BackImageSprHeight = 0;

	// �ǹ����� �׸��� �޸𸮸� �����Ѵ�.
	if( ppStruct != NULL )
	{
		for(i = 0; i < ON_MAX_VIL_STRUCTNUM; i++)
		{
			if(IsVaildStruct(i) == TRUE)
			{
				if(StructSpr[i].Image)
					clGrp.FreeXspr(StructSpr[i]);

				if(StructSprAni[i][0].Image)
					clGrp.FreeXspr(StructSprAni[i][0]);

				if(StructSprAni[i][1].Image)
					clGrp.FreeXspr(StructSprAni[i][1]);

				if(StructSprAni[i][2].Image)
					clGrp.FreeXspr(StructSprAni[i][2]);

				if(StructSprBack[i].Image)
					clGrp.FreeXspr(StructSprBack[i]);

				if(ppStruct[i])
					delete [] ppStruct[i];

				StructSpr[i].Image	     = NULL;
				StructSprAni[i][0].Image = NULL;				
				StructSprAni[i][1].Image = NULL;				
				StructSprAni[i][2].Image = NULL;				
				StructSprBack[i].Image   = NULL;
				ppStruct[i]			     = NULL;

			}	
		}

		delete [] ppStruct;
		ppStruct = NULL;
	}

	if(psiSubStructCode)
	{
		delete [] psiSubStructCode;
		psiSubStructCode	=	NULL;
	}

	if(pVillageStructManager)
	{
		pVillageStructManager->Free();

		delete pVillageStructManager;
		pVillageStructManager	=	NULL;
	}

	if( pExit )
	{
		delete [] pExit;
		pExit	=	NULL;
	}

	clGrp.FreeXspr(m_WallPaperImage);
	clGrp.FreeXspr(m_TopImage);
	clGrp.FreeXspr(m_TopBoardImage);

	if( pMakeList )
	{
		pMakeList->Free();
		delete pMakeList;
		pMakeList	=	NULL;
	}

	if( pMyOnlineWorld->pOnlineInventory->IsActive() )		pMyOnlineWorld->pOnlineInventory->SetActive( FALSE );
}

BOOL	OnlineVillage::CheckFieldRender(void)
{
	return FALSE;
}

BOOL	OnlineVillage::PollField(void)
{
	BOOL	bRet = TRUE;
	SI16	siOrder = 0;	
	SI16	siTempStatus = 0;	

	// ���콺�� �����Ѵ�.
	pMyOnlineWorld->pOnlineHelp->Initialize();
	pMyOnlineWorld->pOnlineMsgBoxPay->Process();
	pMyOnlineWorld->m_pMercenary->Action();

	// �������� List
//	if(pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->IsFocus() == FALSE)
	if(Handler_OnlineEditControlEx::IsFocus(pMyOnlineWorld->pOnlinePannel->GetHOECChat()) == FALSE)
		pMyOnlineWorld->pOnlineNoticeList->Poll(GETSCREENSURFACE(pMyOnlineWorld));

	if( pMyOnlineWorld->IsFindNow() )	pMyOnlineWorld->FindTradeBook();

	RECT		DrawRect;

	DrawRect.left   = 0;
	DrawRect.top    = 0;
	DrawRect.right  = 800;
	DrawRect.bottom = 600;
	if(BackImageSpr)
	{
		GETSCREENSURFACE(pMyOnlineWorld)->BltFast(0, 0, BackImageSpr, &DrawRect, DDBLTFAST_WAIT);
	}

	if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode)
	{
		if(pMyOnlineWorld->pOnlineMap->m_MapInfo.uiVillageCode == pMyOnlineWorld->pOnlineVillageInfoData->m_uiVillageCode)
		{
			float		TempFloat;
			SI16		SignStartPos;
			XSPR*		lpBoard;
			HDC			hdc;
			CHAR		szBuffer[128];
			CHAR		szBuffer2[128];

			SignStartPos = 0;
			if(pMyOnlineWorld->pOnlineVillageInfoData->m_siFrame < 16)
			{
				TempFloat    = (float)sin(((float)pMyOnlineWorld->pOnlineVillageInfoData->m_siFrame / 5.625f) * 0.017444f);
				SignStartPos = -222 + (int)(TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * TempFloat * 222.0f);
			}

			lpBoard = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_VILLAGEBOARDER);
			if(clGrp.LockSurface(GETSCREENSURFACE(pMyOnlineWorld)) == TRUE)
			{
				// ���� ����
				clGrp.PutSpriteT(-1 + SignStartPos, 0, lpBoard->Header.Xsize, lpBoard->Header.Ysize, lpBoard->Image);

				clGrp.UnlockSurface(GETSCREENSURFACE(pMyOnlineWorld));
			}

			// ���� �̸��� ������ش�.
			if(GETSCREENSURFACE(pMyOnlineWorld)->GetDC(&hdc) == DD_OK)
			{
				::SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
				::SetBkMode(hdc, TRANSPARENT);
				::SetTextColor(hdc, RGB(240, 240, 240));

				strcpy(szBuffer, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->pOnlineVillageInfoData->m_lpVillageHeader->siVillageName));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 23 + SignStartPos, 12, 120, 17, szBuffer);		

				MakeMoneyText(pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.uiPopulation, (char*)szBuffer);
				sprintf((char*)szBuffer2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_PEOPLE), szBuffer);

				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 63, szBuffer2, RGB(10, 10, 10));		
				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 62, szBuffer2, RGB(210, 210, 210));		

				if(pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID[0] == NULL) sprintf((char*)szBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NONE));
				else                                                                                   sprintf((char*)szBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_MAXINVEST_NAME), pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.m_MaxInvestmentID);
				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 83, szBuffer, RGB(10, 10, 10));
				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 82, szBuffer, RGB(210, 210, 210));

				if(pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.szGuildID[0] == NULL) sprintf((char*)szBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NONE));
				else                                                                           sprintf((char*)szBuffer, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_GUILD_NAME), pMyOnlineWorld->pOnlineVillageInfoData->m_VillageInfo.szGuildID);
				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 21 + SignStartPos, 103, szBuffer, RGB(10, 10, 10));
				pMyOnlineWorld->pOnlineMegaText->DrawText( hdc, 20 + SignStartPos, 102, szBuffer, RGB(210, 210, 210));

				GETSCREENSURFACE(pMyOnlineWorld)->ReleaseDC(hdc);
			}
		}
	}

	siTempStatus	=	InStructurePoll(FALSE);

	if(siTempStatus != 0)
	{
		if((siTempStatus == ON_VILLAGE_STATUS_IN_EXIT) || (siTempStatus == ON_VILLAGE_STATUS_IN_VILLAGE))
		{
			bRet = FALSE;
		}
	}				

	pMyOnlineWorld->pOnlineMessenger->Draw(GETSCREENSURFACE(pMyOnlineWorld));

	// ���콺�� �׷��ش�.	
	if(clGrp.LockSurface(GETSCREENSURFACE(pMyOnlineWorld)) == TRUE)
	{
		pMyOnlineWorld->pIOnlineMouse->Draw();		
		clGrp.UnlockSurface(GETSCREENSURFACE(pMyOnlineWorld));
	}		

	HDC	hdc;

	if(GETSCREENSURFACE(pMyOnlineWorld)->GetDC(&hdc) == DD_OK)
	{
//		if(pMyOnlineWorld->pOnlinePannel->pOnlineChatEditControl->IsFocus() == FALSE)
		if(Handler_OnlineEditControlEx::IsFocus(pMyOnlineWorld->pOnlinePannel->GetHOECChat()) == FALSE)
		{
			SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
			pMyOnlineWorld->pOnlineNoticeList->Draw(hdc);
		}
		
		GETSCREENSURFACE(pMyOnlineWorld)->ReleaseDC( hdc );
	}

	::ShowMessage(100, 50, GETSCREENSURFACE(pMyOnlineWorld));
	pMyOnlineWorld->pOnlineFieldEffect->RenderInterfaceEffect(GETSCREENSURFACE(pMyOnlineWorld));
	pMyOnlineWorld->UpdateScreen(GETSCREENSURFACE(pMyOnlineWorld));	

	if(IpD.LeftPressSwitch)	 bMousePush = TRUE;	
	else					 bMousePush = FALSE;	

	return bRet;
}

VOID OnlineVillage::DrawVillageInfo(LPDIRECTDRAWSURFACE7 pSurface, BOOL bStruct, BOOL bFarm)
{
	HDC	hdc;
	CHAR szBuffer1[128], szBuffer2[128];

	if(clGrp.LockSurface(pSurface) == TRUE)
	{
		// WallPaper �� ��������
		if(bStruct)
			clGrp.PutSpriteT(0, 0, m_WallPaperImage.Header.Xsize, m_WallPaperImage.Header.Ysize, m_WallPaperImage.Image);
		else if(!bFarm)
		{
			if(!pMyOnlineWorld->pOnlineMiniMap->IsActive() || !pMyOnlineWorld->pOnlineMiniMap->ISFullMode())
			{
				BExit.SetAction(TRUE);
				clGrp.PutSpriteT(686, 469, m_ExitBackImage.Header.Xsize, m_ExitBackImage.Header.Ysize, m_ExitBackImage.Image);
				BExit.Put(&m_ExitImage, 0, 0, 1, BUTTON_PUT_NOMOVE);
			}
			else
				BExit.SetAction(FALSE);
		}

		if(bFarm)
			clGrp.PutSpriteT(306, 0, m_TopBoardImage.Header.Xsize, m_TopBoardImage.Header.Ysize, &m_TopBoardImage.Image[m_TopBoardImage.Header.Start[1]]);
		else
			clGrp.PutSpriteT(306, 0, m_TopBoardImage.Header.Xsize, m_TopBoardImage.Header.Ysize, &m_TopBoardImage.Image[m_TopBoardImage.Header.Start[0]]);

		clGrp.PutSpriteT(0, 0, m_TopImage.Header.Xsize, m_TopImage.Header.Ysize, m_TopImage.Image);

		clGrp.UnlockSurface(GETSCREENSURFACE(pMyOnlineWorld));
	}

	if(pSurface->GetDC(&hdc) == DD_OK)
	{
		::SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_CHAT));
		::SetBkMode(hdc, TRANSPARENT);
		
		// �����̸�
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 329, 3, 140, 16, szName, RGB(10, 10, 10));
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 330, 4, 140, 16, szName, RGB(250, 250, 250));
		
		// �α�
		MakeMoneyText(m_OnRecvVillageInfo.uiPopulation, (char*)szBuffer1);
		sprintf((char*)szBuffer2, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILINFO_PEOPLE), szBuffer1);
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 346, 22, 106, 11, szBuffer2, RGB(10, 10, 10));		
		pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 347, 23, 106, 11, szBuffer2, RGB(250, 250, 250));
		
		if(bFarm)
		{
			// ����ü� ����
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 129, 4, 65, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_MASTER), RGB(10, 10, 10));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 130, 5, 65, 13, pMyOnlineWorld->pOnlineText->Get(ON_TEXT_MAKE_MASTER), RGB(250, 250, 250));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 194, 4, 99, 13, pMakeList->GetMakeList()->m_szID, RGB(10, 10, 10));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 195, 5, 99, 13, pMakeList->GetMakeList()->m_szID, RGB(250, 250, 250));
			
			// ����ü� ����
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 512, 4, 65, 13, pMakeList->GetMakeList()->m_szFarmName, RGB(10, 10, 10));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 513, 5, 65, 13, pMakeList->GetMakeList()->m_szFarmName, RGB(250, 250, 250));
		}
		else
		{
			// �ְ�������
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 129, 4, 65, 13, pMyOnlineWorld->pOnlineText->Get(8000286), RGB(10, 10, 10));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 130, 5, 65, 13, pMyOnlineWorld->pOnlineText->Get(8000286), RGB(250, 250, 250));
			
			if(m_OnRecvVillageInfo.m_MaxInvestmentID[0] != NULL)
			{
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 194, 4, 99, 13, m_OnRecvVillageInfo.m_MaxInvestmentID, RGB(10, 10, 10));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 195, 5, 99, 13, m_OnRecvVillageInfo.m_MaxInvestmentID, RGB(250, 250, 250));
			}
			
			// �Ҽӻ��
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 512, 4, 52, 13, pMyOnlineWorld->pOnlineText->Get(8000528), RGB(10, 10, 10));
			pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 513, 5, 52, 13, pMyOnlineWorld->pOnlineText->Get(8000528), RGB(250, 250, 250));
			
			if(m_OnRecvVillageInfo.szGuildID[0] != NULL)
			{
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 570, 4, 99, 13, m_OnRecvVillageInfo.szGuildID, RGB(10, 10, 10));
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc, 571, 5, 99, 13, m_OnRecvVillageInfo.szGuildID, RGB(250, 250, 250));
			}
		}

		if(!bStruct && !bFarm)
		{
			if(!pMyOnlineWorld->pOnlineMiniMap->IsActive() || !pMyOnlineWorld->pOnlineMiniMap->ISFullMode())
			{
				::SelectObject(hdc, pMyOnlineWorld->pOnlineFonts->GetFont(ON_FONT_NUMBERC));
				BExit.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
			}
		}
		
		pSurface->ReleaseDC(hdc);
	}
}

VOID	OnlineVillage::LoadBackImageSprite(  BOOL local_bIsNight )
{
	int		ClipLeft, ClipTop, ClipRight, ClipBottom;
	XSPR	BackImageBuffer;	
	SI32	siBackImageFont;

	// ������ ��� �׸��� �ε��Ѵ�.
	sprintf(szBackImageSprFileName, "Online\\VillageBack\\%s", pVillageHeader->szConfig);	
	if(clGrp.LoadXspr(szBackImageSprFileName, BackImageBuffer) == FALSE) clGrp.Error(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_VILLAGE_BACKGROUND_ROAD_FAIL), "");
	BackImageSpr = NULL;
	if(clGrp.CreateOffScreen(BackImageSpr, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, TRUE, FALSE))
	{
		clGrp.ClearSurface(BackImageSpr);
		BackImageSprWidth  = BackImageBuffer.Header.Xsize;
		BackImageSprHeight = BackImageBuffer.Header.Ysize;

		//Ŭ���� ������ �ٲ۴�.
		clGrp.GetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);

		if(clGrp.LockSurface(BackImageSpr))
		{
			clGrp.SetClipArea(0, 0, BackImageSprWidth - 1, BackImageSprHeight - 1);

			if( local_bIsNight == TRUE )
			{
				// ���� ���̶��
				siBackImageFont		=	1;				
				
			}
			else
			{
				// ���� ���̶��
				siBackImageFont		=	0;					
			}

			clGrp.PutSpriteT(0, 0, BackImageBuffer.Header.Xsize, BackImageBuffer.Header.Ysize, 
							&BackImageBuffer.Image[ BackImageBuffer.Header.Start[ siBackImageFont ] ] );
			clGrp.UnlockSurface(BackImageSpr);
		}
		else
		{
			BackImageSpr->Release();
			BackImageSpr = NULL;
		}

		clGrp.SetClipArea(ClipLeft, ClipTop, ClipRight, ClipBottom);
	}
	clGrp.FreeXspr(BackImageBuffer);	

}