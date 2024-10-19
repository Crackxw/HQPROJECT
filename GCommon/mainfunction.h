#ifndef _MAINFUNCTION_H
#define _MAINFUNCTION_H


//--------------------------------------------------------------------------
// Name: WindowProc()
// Desc: ������ Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------
// Name: InitQuit()
// Desc: ���Ḧ ���� �غ� �Ѵ�. 
//--------------------------------------------------------------------------
BOOL InitQuit();

//--------------------------------------------------------------------------
// Name: GetVersionInfo()
// Desc: ���� ������ ���´�.
//--------------------------------------------------------------------------
char* GetVersionInfo();

//--------------------------------------------------------------------------
// Name: Restore()
// Desc: �̴ϸ������ ���¿��� �ٽ� �����Ҷ� ���Ǵ� �Լ� 
//--------------------------------------------------------------------------
BOOL Restore();

//--------------------------------------------------------------------------
// Name: OnceInit()
// Desc: ������ ��Ʋ�� �ѹ��� �ʱ�ȭ�Ѵ�. 
//--------------------------------------------------------------------------
void OnceInit();

void GsGame();
void GsInitQuit();
void GsQuit();
void GsInitStaff();
void GsStaff();
void GsInitMenu();
void GsInitLoadGameInitMenu();
void GsLoadGameInitMenu();
void GsInitChooseNation();
void GsChooseNation();
void GsInitPreBriefing();
void GsPreBriefing();
void GsInitSetMulti();
void GsSetMulti();
void GsInitSetSession();
void GsSetSession();
void GsInitSetGame();
void GsSetGame();
void GsInitStageGame();
void GsInitOnlineBattleGame();
void GsInitRestart();
void GsRestart();
void GsInitWin();
void GsWin();
void GsInitLose();
void GsLose();
void GsInitResult();
void GsResult();
void GsInitGameMenu();
void GsGameMenu();
void GsInitFadeOut();
void GsFadeOut();
void GsInitCDMessage();
void GsCDMessage();
void GsInitLaunchCDCheck();
void GsLaunchCDCheck();
void GsInitDpLaunchNotConnected();
void GsDpLaunchNotConnected();
void GsInitBattleConnect();
void GsBattleConnect();
void GsInitBattleCDKeyCheck();
void GsBattleCDKeyCheck();
void GsInitBattleMain();
void GsBattleMain();
void GsInitBattleNewAccount();
void GsBattleNewAccount();
void GsInitBattleChangePW();
void GsBattleChangePW();
void GsInitBattleLobby();
void GsBattleLobby();
void GsInitBattleConf();
void GsBattleConf();
void GsInitBattleJoin();
void GsBattleJoin();
void GsInitBattleMake();
void GsBattleMake();
void GsInitBattleStart();
void GsBattleStart();
void GsInitBattleReturn();
void GsBattleReturn();


void GsInitBattleNewID();
void GsBattleNewID();
void GsInitBattleCheckID();
void GsBattleCheckID();


// ������ �¶��� ����
VOID	GsInitOnlineWorld();
VOID	GsOnlineWorld();

void GsInitBattle();

// ���Ӹ޴����� �����ϱ⸦ ������ �� ó�� �Լ�. 
BOOL OperateGameMenuInitQuit();

// ���Ӹ޴����� �ʱ�޴��� ������ �� ó�� �Լ�. 
BOOL OperateGameMenuInitInitMenu();


#endif
