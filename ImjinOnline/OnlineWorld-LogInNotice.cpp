//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: 임진록 온라인 [거상] - SMS 메시지 출력부
//	File Name		: OnlineWorld-LogInNotice.cpp
//	Birth Date		: 2003. 06.
//	Creator			: 이 준 석
//	Editer			: 조 현 준 (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		거상의 프론트 메인 화면에서 라이센스 관련된 문장을 출력하는 부분입니다.
//
//===================================================================================================


#include <GSL.h>

#include <Mouse.h>
#include "OnlineWorld.h"
#include "OnlineText.h"
#include "OnlineFont.h"
#include "OnlineMegaText.h"
#include "OnlineChatList.h"
#include <string>
#include "bindJXFile.h"
#include "..\Gersang\directx.h"
#include "OnlineChatList.h"
#include <clGame.h>

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : 공통 파서기


using namespace std;
extern _clGame* pGame;

extern _InputDevice			IpD;

VOID cltOnlineWorld::InitAgreeNotice(BOOL blicense)
{
	OnlineCommonParser	ocp;


	clGrp.LoadXspr( "Online\\GameSpr\\NewLogIn\\login_agreement_box.Spr", pOnlineLogInMgr->m_NoticeData.NoticeImage);

	if(blicense)
	{
		pOnlineLogInMgr->m_NoticeData.OkButton.Create(530, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_PROVISION_ACCEPT), BUTTON_PUT_LEFT, TRUE );
		pOnlineLogInMgr->m_NoticeData.ExitButton.Create(651, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_PROVISION_NO), BUTTON_PUT_LEFT, TRUE );
	}
	else
	{
		pOnlineLogInMgr->m_NoticeData.OkButton.Create(530, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_OK1), BUTTON_PUT_LEFT, TRUE );
		pOnlineLogInMgr->m_NoticeData.ExitButton.Create(651, 502, pOnlineLogInMgr->SButtonImage.Header.Xsize, pOnlineLogInMgr->SButtonImage.Header.Ysize, pOnlineText->Get(ON_TEXT_BACK), BUTTON_PUT_LEFT, TRUE );
	}

	// 공지사항 및 약관
	pOnlineLogInMgr->m_NoticeData.m_pNoticeList	= new OnlineChatList;
	pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Init(this, ON_INN_CONTENT_ONLY, -1, ON_GMAESPR_LOGIN_SCROLL_BAR,
					 16, 40, 499, 236, 752, 242, 752-5, 242, 20, 230, 499, 236);

	if(blicense)
	{
		int		iRet, iErrLine;
		BOOL	bLess;

		// 약관을 읽어서 넣어준다
		FILE *fp = NULL;
		CHAR	szContent[1024] = {0,};
		
		// actdoll (2004/02/23 15:55) : 
		//	다른 방식으로 해당 정보를 로딩한다.
		// actdoll (2004/11/15 23:17) : 외부 HQ 옵션이 켜진 경우 체크한다.
		char	pszFullPath[MAX_PATH];
		char	*pszFileName	= "%s\\LicenseText.dat";
		if( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] )
		{
			sprintf( pszFullPath, pszFileName, DF_PATH_DEBUG_OUTSIDE_HQTEXT );
			fp	= fopen( pszFullPath, "rb" );
		}
		if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
		{
			sprintf( pszFullPath, pszFileName, pGame->m_pszHQNation );
			fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
		}
		if( !fp )	return;			// 그래도 발견 못하면 리턴


		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_LicenseText.txt" );

		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "OnlineWorld-LogInNotice Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return;
		}

		// 자료 받기
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 자료 추출
			SI32	iLineNo, iColNo;

			ocp.GetValue( szContent,	1023,	iLineNo, iColNo );		// 스트링

			// 실제 데이터에 집어넣는다.
			pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Add( szContent, ON_INN_COLOR_WHITE );

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "OnlineWorld-LogInNotice Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return;
			}
			
			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Initialize();
		
		SI16 i = 0, siCnt =	0;
		CHAR szTempBuffer[ON_INN_MAX_USER_CHAT_STRING];
		ZeroMemory(szTempBuffer, sizeof(szTempBuffer));

		while(pOnlineLogInMgr->szNotice[i])
		{		
			if(pOnlineLogInMgr->szNotice[i] == '\n' || pOnlineLogInMgr->szNotice[i] == '\r')
			{			
				pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Add(szTempBuffer, ON_INN_COLOR_WHITE);
				ZeroMemory(szTempBuffer, sizeof(szTempBuffer));
				siCnt = 0;
			}
			else
			{
				szTempBuffer[siCnt] = pOnlineLogInMgr->szNotice[i];
				siCnt++;
			}
			i++;		
		}	
		pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Add(szTempBuffer, ON_INN_COLOR_WHITE);
	}
}

SI08 cltOnlineWorld::AgreeNotice()
{
	if(pOnlineLogInMgr->m_NoticeData.OkButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE || GetAsyncKeyState(VK_RETURN))
	{
		return ON_RETURN_OK;
	}

	if(pOnlineLogInMgr->m_NoticeData.ExitButton.Process(pOnlineLogInMgr->bMouseDown) == TRUE)
	{
		return ON_RETURN_EXIT;
	}
	return ON_RETURN_NONE;
}

VOID cltOnlineWorld::DrawAgreeNotice()
{
	HDC	hdc;

	if(clGrp.LockSurface(GETSCREENSURFACE(this)) == TRUE)
	{
		clGrp.PutSpriteT(483, 213, pOnlineLogInMgr->m_NoticeData.NoticeImage.Header.Xsize, pOnlineLogInMgr->m_NoticeData.NoticeImage.Header.Ysize, pOnlineLogInMgr->m_NoticeData.NoticeImage.Image);

		pOnlineLogInMgr->m_NoticeData.OkButton.Put(&pOnlineLogInMgr->SButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		pOnlineLogInMgr->m_NoticeData.ExitButton.Put(&pOnlineLogInMgr->SButtonImage, 0, 2, 1, BUTTON_PUT_NOMOVE);
		
		clGrp.UnlockSurface(GETSCREENSURFACE(this));
	}
	if(GETSCREENSURFACE(this)->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);		
		SelectObject(hdc, pOnlineFonts->GetFont(ON_FONT_CHAT));
		pOnlineLogInMgr->m_NoticeData.OkButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		pOnlineLogInMgr->m_NoticeData.ExitButton.Put(hdc, (DWORD)0, RGB(250, 250, 250), RGB(216, 216, 216));
		
		GETSCREENSURFACE(this)->ReleaseDC(hdc);
	}
	pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Process(GETSCREENSURFACE(this), ON_FONT_BUTTON);
}

VOID cltOnlineWorld::FreeAgreeNotice()
{
	clGrp.FreeXspr(pOnlineLogInMgr->m_NoticeData.NoticeImage);

	if(pOnlineLogInMgr->m_NoticeData.m_pNoticeList)
	{
		delete pOnlineLogInMgr->m_NoticeData.m_pNoticeList;
		pOnlineLogInMgr->m_NoticeData.m_pNoticeList	=	NULL;
	}
}