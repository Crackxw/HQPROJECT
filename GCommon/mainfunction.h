#ifndef _MAINFUNCTION_H
#define _MAINFUNCTION_H


//--------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 윈도우 Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------
// Name: InitQuit()
// Desc: 종료를 위한 준비를 한다. 
//--------------------------------------------------------------------------
BOOL InitQuit();

//--------------------------------------------------------------------------
// Name: GetVersionInfo()
// Desc: 버전 정보를 얻어온다.
//--------------------------------------------------------------------------
char* GetVersionInfo();

//--------------------------------------------------------------------------
// Name: Restore()
// Desc: 미니마이즈된 상태에서 다시 복구할때 사용되는 함수 
//--------------------------------------------------------------------------
BOOL Restore();

//--------------------------------------------------------------------------
// Name: OnceInit()
// Desc: 게임을 통틀어 한번만 초기화한다. 
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


// 임진록 온라인 관련
VOID	GsInitOnlineWorld();
VOID	GsOnlineWorld();

void GsInitBattle();

// 게임메뉴에서 종료하기를 눌렀을 때 처리 함수. 
BOOL OperateGameMenuInitQuit();

// 게임메뉴에서 초기메뉴를 눌렀을 때 처리 함수. 
BOOL OperateGameMenuInitInitMenu();


#endif
