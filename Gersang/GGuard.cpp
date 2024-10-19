#include <GSL.h>
#include <NPGameLib.h>
#include <GGuard.h>
#include "clgame.h"
#include "OnlineClient.h"

extern	_clGame* pGame;

bool bAppExit = false;

HWND g_hWnd = NULL;
CNPGameLib *pGG = NULL;
BOOL g_bGameguard = TRUE;

const	char*	DF_ERROR_GG_ALREADY_EXIST	= "프로세스가 가동 중 입니다.\n잠시 혹은 재부팅 후에 다시 실행해주세요.\n\n [GameGuard] is still running.\n Please wait a sec or reboot and try again.";
const	char*	DF_ERROR_GG_GAME_EXIST		= "게임이 중복 실행되었거나 프로세스가 이미 실행 중 입니다.\n게임 종료 후 다시 실행해주세요.\n\n We find same game or [GameGuard] is already running.\n Please shutdown your game and try again.";
const	char*	DF_ERROR_GG_INIT_FAIL		= "초기화 에러입니다.\n재부팅 후 다시 실행하거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해주세요.\n\n Initialize of [GameGuard] is failed.\n Please reboot or clear programs what can have possibility to crash with [GameGuard] and try again.";
const	char*	DF_ERROR_GG_FILE_MISMATCH	= "파일이 없거나 변조되었습니다.\n게임가드 셋업 파일을 다시 설치하세요.\n\n File is missing or doesn't exist.\n Please try to install [GameGuard] again.";
const	char*	DF_ERROR_GG_API_MISMATCH	= "윈도우의 일부 시스템 파일이 손상되었습니다.\n인터넷 익스플로러(IE)를 다시 설치하세요.\n\n Error of Windows System Files.\n Please try to install Internet Explorer(IE) again.";
const	char*	DF_ERROR_GG_EXEC_FAIL		= "실행에 실패했습니다.\n게임가드 셋업 파일을 다시 설치하세요.\n\n Fail to execute [GameGuard].\n Please try to install [GameGuard] again.";
const	char*	DF_ERROR_GG_FIND_HACK		= "불법 프로그램이 발견되었습니다.\n불필요한 프로그램을 종료한 후 다시 실행해보세요.\n\n Find illegal programs.\n Please clear programs what can have possibility to realize illegal one by [GameGuard].";
const	char*	DF_ERROR_GG_ABORT_UPDATE	= "업데이트가 취소됐습니다.\n접속이 계속 되지 않을 경우 인터넷 방화벽 설정을 조정해보세요.\n\n [GameGuard] update was aborted.\n Please check your internet firewall systems if you cannot try this.";
const	char*	DF_ERROR_GG_CONNECT_FAIL	= "업데이트 서버 접속에 실패하였습니다.\n게임 작동 중 문제가 발생할 수도 있습니다.\n\n Cannot connect [GameGuard] server.\n Maybe, It will be occured some problems when game is running.";
const	char*	DF_ERROR_GG_CANNOT_UPDATE	= "업데이트를 완료하지 못했습니다.\n백신 프로그램을 중지시킨 후 재시도, 혹은 PC 관리 프로그램을 사용하시면 설정을 조정해보세요.\n\n Cannot update [GameGuard].\n Please pause Virus Vaccine Program, etc and try again.";
const	char*	DF_ERROR_GG_LOADING_FAIL	= "검사 모듈 로딩에 실패했습니다.\n메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.\n\n Fail to load [GameGuard].\n Sometimes this should be occured by little memory or virus.";
const	char*	DF_ERROR_GG_UNKNOWN			= "프로세스 실행 중 에러가 발생하였습니다.\n게임폴더 내 GameGuard 폴더에 있는 *.erl 파일들을 game@inca.co.kr로 보내주십시오.\n\n Error occured in [GameGuard].\n Please search the error list(*.erl) in [GameGuard] folder of Game Directory and send E-Mail to (game@inca.co.kr).";

BOOL CreateGameGuard(BOOL bEnable)
{
	g_bGameguard = bEnable;
	if(g_bGameguard == FALSE) return(TRUE);

#ifdef _GAMEGUARD
	if(pGG != NULL) return(TRUE);
	pGG	= new CNPGameLib("Gersang");
	ASSERT(pGG);

	// actdoll (2004/10/19 11:23) : 이번에 제공된 게임가드는 HWND를 적재하지 않고 구동한다.
	DWORD dwResult = pGG->Init();

	if (dwResult != NPGAMEMON_SUCCESS)
    {
		TCHAR msg[256];
		LPCSTR lpszMsg;
		
		// 게임에 맞게 종료 코드
		BOOL	bRet	= false;
		bAppExit		= true;
		
		// '6. 주요에러코드'를 참조하여 상황에 맞는 메시지를 출력해줍니다.
		switch (dwResult)
		{
		case NPGAMEMON_ERROR_EXIST:
			lpszMsg = DF_ERROR_GG_ALREADY_EXIST;
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			lpszMsg = DF_ERROR_GG_GAME_EXIST;
			break;
		case NPGAMEMON_ERROR_INIT:
			lpszMsg = DF_ERROR_GG_INIT_FAIL;
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			lpszMsg = DF_ERROR_GG_FILE_MISMATCH;
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			lpszMsg = DF_ERROR_GG_API_MISMATCH;
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			lpszMsg = DF_ERROR_GG_EXEC_FAIL;
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			lpszMsg = DF_ERROR_GG_FIND_HACK;
			break;
		case NPGMUP_ERROR_ABORT:
			lpszMsg = DF_ERROR_GG_ABORT_UPDATE;
			break;
		case NPGMUP_ERROR_CONNECT:
		case NPGMUP_ERROR_DOWNCFG:
			lpszMsg = DF_ERROR_GG_CONNECT_FAIL;
			break;
		case NPGMUP_ERROR_AUTH:
			lpszMsg = DF_ERROR_GG_CANNOT_UPDATE;
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			lpszMsg = DF_ERROR_GG_LOADING_FAIL;
			break;
		default:
			// 적절한 종료 메시지 출력, UI 초기화 후라면 MessageBox 대신 게임 UI를 사용해도 됨
			lpszMsg = DF_ERROR_GG_UNKNOWN;
			break;
		}
		wsprintf(msg, "게임가드 에러(GameGuard Error) : %lu", dwResult);
		MessageBox(g_hWnd, lpszMsg, msg, MB_OK); // 첫번째 인자를 NULL로 하면 안 됨
		
		return(FALSE);
    }
#endif

	return(TRUE);
}

void SetHwndToGameGuard( HWND hWnd )
{
	if( !g_bGameguard || !hWnd )		return;

	g_hWnd	= hWnd;
	pGG->SetHwnd( hWnd );	// actdoll (2004/10/19 11:29) : 여기서부터 CALLBACK 함수가 구동된다.
}

BOOL CheckGameGuard(VOID)
{
	if(g_bGameguard == FALSE) return(TRUE);

//	MessageBox(g_hWnd, "체크", "CheckGameGuard", MB_OK);
#ifdef _GAMEGUARD
	if(pGG->Check()!=NPGAMEMON_SUCCESS)
	{
//		MessageBox(g_hWnd, "해킹툴 발견", "경고", MB_OK); // 첫번째 인자를 NULL로 하면 안 됨
		return(FALSE);
	}
#endif

	return(TRUE);
}

VOID SendIDGameGuard(LPCTSTR lpszUserId)
{
	if(g_bGameguard == FALSE) return;

#ifdef _GAMEGUARD
	if(pGG != NULL) pGG->Send(lpszUserId);
#endif
}

VOID ReleaseGameGuard(VOID)
{
#ifdef _GAMEGUARD
	if(pGG != NULL)
	{
		delete pGG;
		pGG = NULL;
	}
#endif
}

//#ifdef _GAMEGUARD

bool CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	TCHAR msg[128];
	LPCTSTR lpszMsg = NULL;

	if(g_bGameguard == FALSE) return(false);

	switch (dwMsg)
    {
	case NPGAMEMON_CHECK_CSAUTH:
		pGame->pOnlineWorld->pOnlineClient->SendGameGuardKeyResult(dwArg);
		return true;
		
	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		bAppExit = true;
		return false;
	case NPGAMEMON_INIT_ERROR:
		bAppExit = true; // 종료 코드
		wsprintf(msg, "게임가드 초기화 에러(GameGuard Initialize Error) : %lu", dwArg);
		lpszMsg = msg;
		break;
	case NPGAMEMON_SPEEDHACK:
		bAppExit = true; // 종료 코드
		lpszMsg = "스피드핵이 감지되었습니다.\n\n SPEED HACK PROGRAM FOUND!";
		break;
	case NPGAMEMON_GAMEHACK_KILLED:
/*
		bAppExit = true; // 종료 코드, 종료 또는 계속 진행
		lpszMsg = "게임핵이 발견되었습니다.";
		break;
*/
	case NPGAMEMON_GAMEHACK_DETECT:
		bAppExit = true; // 종료 코드
		if(pGG->GetInfo() != NULL) 
		{
			wsprintf(msg, "게임핵이 발견되었습니다.(We Found Game Cracking!) : (Process Name:%s)", pGG->GetInfo());
			lpszMsg = msg;
		}
		else
		{
			lpszMsg = "게임핵이 발견되었습니다.(We Found Game Cracking!)";
		}
		break;
	case NPGAMEMON_GAMEHACK_DOUBT:
		bAppExit = true; // 종료 코드
		lpszMsg = "게임이나 게임가드가 변조되었습니다.\n\n GAME OR GAMEGUARD IS INCORRECT AS ORIGINAL!";
		break;
	}

	if (lpszMsg)
	{
		if (bAppExit) pGame->pOnlineWorld->pOnlineClient->SendGameGuardUserInfo(1, lpszMsg);
		MessageBox(g_hWnd, lpszMsg, "nProtect GameGuard", MB_OK);
	}
	
	// 게임 종료시에만 false를 리턴
	if (bAppExit)
	{
		return false;
	}
	
    return true;
}

//#endif

VOID SetGameGuardKey(DWORD dwGameGuardkey)
{
#ifdef _GAMEGUARD
	if(pGG != NULL)	pGG->Auth(dwGameGuardkey);
#endif

	if((pGG == NULL) || (g_bGameguard == FALSE))
	{
		pGame->pOnlineWorld->pOnlineClient->SendGameGuardKeyResult(GAMEGUARD_PASSKEY);	// 인자값은 쓰지 않게 된다.
	}
}

BOOL OnlineClient::SendGameGuardKeyResult(DWORD dwKey)
{
	OnCheckGameGuardMsg checkGameGuard;

	checkGameGuard.dwMsgType = ON_REQUEST_CHECK_GAMEGUARD;
	checkGameGuard.dwGameGuardKey = dwKey;
//	MessageBox(g_hWnd, "send GameGuard Key", "Msg", MB_OK);

	return(Write(&checkGameGuard, sizeof(OnCheckGameGuardMsg)));
}

