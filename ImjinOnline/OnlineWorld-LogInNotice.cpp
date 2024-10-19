//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	: ������ �¶��� [�Ż�] - SMS �޽��� ��º�
//	File Name		: OnlineWorld-LogInNotice.cpp
//	Birth Date		: 2003. 06.
//	Creator			: �� �� ��
//	Editer			: �� �� �� (actdoll@joyon.com)
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		�Ż��� ����Ʈ ���� ȭ�鿡�� ���̼��� ���õ� ������ ����ϴ� �κ��Դϴ�.
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

#include "../SCCommon/OnlineCommonParser.h"		// actdoll (2004/06/21 15:15) : ���� �ļ���


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

	// �������� �� ���
	pOnlineLogInMgr->m_NoticeData.m_pNoticeList	= new OnlineChatList;
	pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Init(this, ON_INN_CONTENT_ONLY, -1, ON_GMAESPR_LOGIN_SCROLL_BAR,
					 16, 40, 499, 236, 752, 242, 752-5, 242, 20, 230, 499, 236);

	if(blicense)
	{
		int		iRet, iErrLine;
		BOOL	bLess;

		// ����� �о �־��ش�
		FILE *fp = NULL;
		CHAR	szContent[1024] = {0,};
		
		// actdoll (2004/02/23 15:55) : 
		//	�ٸ� ������� �ش� ������ �ε��Ѵ�.
		// actdoll (2004/11/15 23:17) : �ܺ� HQ �ɼ��� ���� ��� üũ�Ѵ�.
		char	pszFullPath[MAX_PATH];
		char	*pszFileName	= "%s\\LicenseText.dat";
		if( DF_PATH_DEBUG_OUTSIDE_HQTEXT[0] )
		{
			sprintf( pszFullPath, pszFileName, DF_PATH_DEBUG_OUTSIDE_HQTEXT );
			fp	= fopen( pszFullPath, "rb" );
		}
		if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
		{
			sprintf( pszFullPath, pszFileName, pGame->m_pszHQNation );
			fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
		}
		if( !fp )	return;			// �׷��� �߰� ���ϸ� ����


		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_LicenseText.txt" );

		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "OnlineWorld-LogInNotice Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return;
		}

		// �ڷ� �ޱ�
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� �ڷ� ����
			SI32	iLineNo, iColNo;

			ocp.GetValue( szContent,	1023,	iLineNo, iColNo );		// ��Ʈ��

			// ���� �����Ϳ� ����ִ´�.
			pOnlineLogInMgr->m_NoticeData.m_pNoticeList->Add( szContent, ON_INN_COLOR_WHITE );

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "OnlineWorld-LogInNotice Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return;
			}
			
			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
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