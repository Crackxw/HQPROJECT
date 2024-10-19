
#include "ExpoRank.h"
#include "Point.h"

int ExpoRank::CalcScore(int win_DBAccount, int lose_DBAccount)
{
	// 3번이하 전투인 경우면 점수를 업데이트
	if(_UserCatalog.GetUser(win_DBAccount).IsInvalidBattle(lose_DBAccount) == FALSE)	return 0;
	if(_UserCatalog.GetUser(lose_DBAccount).IsInvalidBattle(win_DBAccount) == FALSE)	return 0;
	
	Point			CalcClass;
	PlayersPoint	CalcPoint(_UserCatalog.GetUser(win_DBAccount).GetScore(), 
							  _UserCatalog.GetUser(lose_DBAccount).GetScore());

	// 전투 점수 계산
	PlayersPoint Result = CalcClass.Calc(CalcPoint);


	// 새로운 점수를 업데이트
	_UserCatalog.GetUser(win_DBAccount).UpdateBattleCount(lose_DBAccount);
	_UserCatalog.GetUser(lose_DBAccount).UpdateBattleCount(win_DBAccount);

	_UserCatalog.GetUser(win_DBAccount).SetScore(Result.siPlayer1);
	_UserCatalog.GetUser(lose_DBAccount).SetScore(Result.siPlayer2);

	_UserCatalog.sortByScore();


	return Result.siPlayerPoint;
}

BOOL ExpoRank::IsInvalidBattle(int nMyDBAccount, int TargetDBAccount)
{
	if(_UserCatalog.GetUser(TargetDBAccount).GetBattleCount() >= MAX_BATTLE_COUNT)	return FALSE;
	return _UserCatalog.GetUser(nMyDBAccount).IsInvalidBattle(TargetDBAccount);
}

void ExpoRank::GetAllRank(int *pDBAccount, int *pRank)
{
	for(int i=0; i<MAX_USER; ++i)	
	{
		pDBAccount[i]	= _UserCatalog.GetIndex(i).GetDBAccount();
		pRank[i]		= _UserCatalog.GetIndex(i).GetRank();
	}
}