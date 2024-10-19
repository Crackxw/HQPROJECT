#pragma once

#include "UserCatalog.h"

class ExpoRank
{
public:
	ExpoRank()	{}
	~ExpoRank()	{}

	int		CalcScore(int win_DBAccount, int lose_DBAccount);		// 점수 계산

	void	AddUser(DWORD nDBAccount, int win, int lose, int score, int battle_count, char *pBirthday, int *pDBAccount, int *pBattle_count)
			{ _UserCatalog.AddUser(nDBAccount, win, lose, score, battle_count, pBirthday, pDBAccount, pBattle_count); }	

	int		GetRank(int nDBAccount)			{ return _UserCatalog.GetUser(nDBAccount).GetRank(); }
	int		GetScore(int nDBAccount)		{ return _UserCatalog.GetUser(nDBAccount).GetScore(); }
	int		GetWin(int nDBAccount)			{ return _UserCatalog.GetUser(nDBAccount).GetWin(); }
	int		GetLose(int nDBAccount)			{ return _UserCatalog.GetUser(nDBAccount).GetLose(); }


	void	GetAllRank(int *pDBAccount, int *pRank);

	void	sortData()						{ _UserCatalog.sortByScore(); _UserCatalog.sortByUniqueID(); }	
	BOOL	IsInvalidBattle(int nMyDBAccount, int TargetDBAccount);

private:	
	UserCatalog _UserCatalog;
};