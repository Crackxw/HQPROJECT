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

const	char*	DF_ERROR_GG_ALREADY_EXIST	= "���μ����� ���� �� �Դϴ�.\n��� Ȥ�� ����� �Ŀ� �ٽ� �������ּ���.\n\n [GameGuard] is still running.\n Please wait a sec or reboot and try again.";
const	char*	DF_ERROR_GG_GAME_EXIST		= "������ �ߺ� ����Ǿ��ų� ���μ����� �̹� ���� �� �Դϴ�.\n���� ���� �� �ٽ� �������ּ���.\n\n We find same game or [GameGuard] is already running.\n Please shutdown your game and try again.";
const	char*	DF_ERROR_GG_INIT_FAIL		= "�ʱ�ȭ �����Դϴ�.\n����� �� �ٽ� �����ϰų� �浹�� �� �ִ� �ٸ� ���α׷����� ������ �� �������ּ���.\n\n Initialize of [GameGuard] is failed.\n Please reboot or clear programs what can have possibility to crash with [GameGuard] and try again.";
const	char*	DF_ERROR_GG_FILE_MISMATCH	= "������ ���ų� �����Ǿ����ϴ�.\n���Ӱ��� �¾� ������ �ٽ� ��ġ�ϼ���.\n\n File is missing or doesn't exist.\n Please try to install [GameGuard] again.";
const	char*	DF_ERROR_GG_API_MISMATCH	= "�������� �Ϻ� �ý��� ������ �ջ�Ǿ����ϴ�.\n���ͳ� �ͽ��÷η�(IE)�� �ٽ� ��ġ�ϼ���.\n\n Error of Windows System Files.\n Please try to install Internet Explorer(IE) again.";
const	char*	DF_ERROR_GG_EXEC_FAIL		= "���࿡ �����߽��ϴ�.\n���Ӱ��� �¾� ������ �ٽ� ��ġ�ϼ���.\n\n Fail to execute [GameGuard].\n Please try to install [GameGuard] again.";
const	char*	DF_ERROR_GG_FIND_HACK		= "�ҹ� ���α׷��� �߰ߵǾ����ϴ�.\n���ʿ��� ���α׷��� ������ �� �ٽ� �����غ�����.\n\n Find illegal programs.\n Please clear programs what can have possibility to realize illegal one by [GameGuard].";
const	char*	DF_ERROR_GG_ABORT_UPDATE	= "������Ʈ�� ��ҵƽ��ϴ�.\n������ ��� ���� ���� ��� ���ͳ� ��ȭ�� ������ �����غ�����.\n\n [GameGuard] update was aborted.\n Please check your internet firewall systems if you cannot try this.";
const	char*	DF_ERROR_GG_CONNECT_FAIL	= "������Ʈ ���� ���ӿ� �����Ͽ����ϴ�.\n���� �۵� �� ������ �߻��� ���� �ֽ��ϴ�.\n\n Cannot connect [GameGuard] server.\n Maybe, It will be occured some problems when game is running.";
const	char*	DF_ERROR_GG_CANNOT_UPDATE	= "������Ʈ�� �Ϸ����� ���߽��ϴ�.\n��� ���α׷��� ������Ų �� ��õ�, Ȥ�� PC ���� ���α׷��� ����Ͻø� ������ �����غ�����.\n\n Cannot update [GameGuard].\n Please pause Virus Vaccine Program, etc and try again.";
const	char*	DF_ERROR_GG_LOADING_FAIL	= "�˻� ��� �ε��� �����߽��ϴ�.\n�޸� �����̰ų� ���̷����� ���� ������ �� �ֽ��ϴ�.\n\n Fail to load [GameGuard].\n Sometimes this should be occured by little memory or virus.";
const	char*	DF_ERROR_GG_UNKNOWN			= "���μ��� ���� �� ������ �߻��Ͽ����ϴ�.\n�������� �� GameGuard ������ �ִ� *.erl ���ϵ��� game@inca.co.kr�� �����ֽʽÿ�.\n\n Error occured in [GameGuard].\n Please search the error list(*.erl) in [GameGuard] folder of Game Directory and send E-Mail to (game@inca.co.kr).";

BOOL CreateGameGuard(BOOL bEnable)
{
	g_bGameguard = bEnable;
	if(g_bGameguard == FALSE) return(TRUE);

#ifdef _GAMEGUARD
	if(pGG != NULL) return(TRUE);
	pGG	= new CNPGameLib("Gersang");
	ASSERT(pGG);

	// actdoll (2004/10/19 11:23) : �̹��� ������ ���Ӱ���� HWND�� �������� �ʰ� �����Ѵ�.
	DWORD dwResult = pGG->Init();

	if (dwResult != NPGAMEMON_SUCCESS)
    {
		TCHAR msg[256];
		LPCSTR lpszMsg;
		
		// ���ӿ� �°� ���� �ڵ�
		BOOL	bRet	= false;
		bAppExit		= true;
		
		// '6. �ֿ信���ڵ�'�� �����Ͽ� ��Ȳ�� �´� �޽����� ������ݴϴ�.
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
			// ������ ���� �޽��� ���, UI �ʱ�ȭ �Ķ�� MessageBox ��� ���� UI�� ����ص� ��
			lpszMsg = DF_ERROR_GG_UNKNOWN;
			break;
		}
		wsprintf(msg, "���Ӱ��� ����(GameGuard Error) : %lu", dwResult);
		MessageBox(g_hWnd, lpszMsg, msg, MB_OK); // ù��° ���ڸ� NULL�� �ϸ� �� ��
		
		return(FALSE);
    }
#endif

	return(TRUE);
}

void SetHwndToGameGuard( HWND hWnd )
{
	if( !g_bGameguard || !hWnd )		return;

	g_hWnd	= hWnd;
	pGG->SetHwnd( hWnd );	// actdoll (2004/10/19 11:29) : ���⼭���� CALLBACK �Լ��� �����ȴ�.
}

BOOL CheckGameGuard(VOID)
{
	if(g_bGameguard == FALSE) return(TRUE);

//	MessageBox(g_hWnd, "üũ", "CheckGameGuard", MB_OK);
#ifdef _GAMEGUARD
	if(pGG->Check()!=NPGAMEMON_SUCCESS)
	{
//		MessageBox(g_hWnd, "��ŷ�� �߰�", "���", MB_OK); // ù��° ���ڸ� NULL�� �ϸ� �� ��
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
		bAppExit = true; // ���� �ڵ�
		wsprintf(msg, "���Ӱ��� �ʱ�ȭ ����(GameGuard Initialize Error) : %lu", dwArg);
		lpszMsg = msg;
		break;
	case NPGAMEMON_SPEEDHACK:
		bAppExit = true; // ���� �ڵ�
		lpszMsg = "���ǵ����� �����Ǿ����ϴ�.\n\n SPEED HACK PROGRAM FOUND!";
		break;
	case NPGAMEMON_GAMEHACK_KILLED:
/*
		bAppExit = true; // ���� �ڵ�, ���� �Ǵ� ��� ����
		lpszMsg = "�������� �߰ߵǾ����ϴ�.";
		break;
*/
	case NPGAMEMON_GAMEHACK_DETECT:
		bAppExit = true; // ���� �ڵ�
		if(pGG->GetInfo() != NULL) 
		{
			wsprintf(msg, "�������� �߰ߵǾ����ϴ�.(We Found Game Cracking!) : (Process Name:%s)", pGG->GetInfo());
			lpszMsg = msg;
		}
		else
		{
			lpszMsg = "�������� �߰ߵǾ����ϴ�.(We Found Game Cracking!)";
		}
		break;
	case NPGAMEMON_GAMEHACK_DOUBT:
		bAppExit = true; // ���� �ڵ�
		lpszMsg = "�����̳� ���Ӱ��尡 �����Ǿ����ϴ�.\n\n GAME OR GAMEGUARD IS INCORRECT AS ORIGINAL!";
		break;
	}

	if (lpszMsg)
	{
		if (bAppExit) pGame->pOnlineWorld->pOnlineClient->SendGameGuardUserInfo(1, lpszMsg);
		MessageBox(g_hWnd, lpszMsg, "nProtect GameGuard", MB_OK);
	}
	
	// ���� ����ÿ��� false�� ����
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
		pGame->pOnlineWorld->pOnlineClient->SendGameGuardKeyResult(GAMEGUARD_PASSKEY);	// ���ڰ��� ���� �ʰ� �ȴ�.
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

