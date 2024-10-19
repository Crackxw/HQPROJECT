#pragma once

#pragma warning ( disable : 4786 )

#include <windows.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define MAX_USER			3000
#define MAX_BATTLE_COUNT	500

class User
{
public:
	void Init(int nDBAccount, int win, int lose, int srore, int battle_count, char *pBirthday, int *pDBAccount, int *pBattle_count)		
	{
		_nDBAccount		= nDBAccount;
		_nWin			= win;
		_nLose			= lose;
		_nScore			= srore;
		_nBattle_Count	= battle_count;
		_nBirthday		= atoi(pBirthday);
		_nRank			= 1;

		memcpy(_DBAccount, pDBAccount, sizeof(int)*MAX_USER); 
		memcpy(_Battle_count, pBattle_count, sizeof(int)*MAX_USER);
	}

	User()	
	{
		_nDBAccount = _nScore = _nRank = -1; 
		_nBattle_Count = MAX_BATTLE_COUNT;		
	}

	int		GetWin()			{ return _nWin; }
	int		GetLose()			{ return _nLose; }
	int		GetBattleCount()	{ return _nBattle_Count; }
	int		GetScore()			{ return _nScore; }
	int		GetDBAccount()		{ return _nDBAccount; }
	int		GetRank()			{ return _nRank; }
	UINT	GetBirthday()		{ return _nBirthday; }
	
	void	SetScore(int s)		{ _nScore = s; }
	void	SetRank(int s)		{ _nRank = s; }

	void	UpdateBattleCount(int nTargetDBAccount)	
	{
		if(_nDBAccount == -1) return;

		int	*pBattle_Count = GetUserList(nTargetDBAccount);
		if(pBattle_Count)	(*pBattle_Count)++;

		_nBattle_Count++;
	}

	BOOL	IsInvalidBattle(int nTargetDBAccount)
	{
		if(_nDBAccount == -1)					return FALSE;
		if(_nBattle_Count >= MAX_BATTLE_COUNT )	return FALSE;


		int	*pBattle_Count = GetUserList(nTargetDBAccount);
		if(pBattle_Count)	return (*pBattle_Count) >= 3 ? FALSE : TRUE;
		
		return FALSE;
	}	

	bool	compare_rank(User &y)
	{
		int nRet = GetScore() - y.GetScore();
		
		if(nRet == 0)
		{
			float ret = ((float)_nWin / (float)(_nWin+_nLose)) - ((float)y.GetWin() / (float)(y.GetWin()+y.GetLose()));

			if(ret == 0)
			{
				return _nBirthday > y.GetBirthday() ? false : true;
			}
			if(ret > 0)		return true;
		}
		if(nRet > 0)		return true;
		return false;
	}

	int		*GetUserList(int _nDBAccount)
	{
		for(int i=0; i<MAX_USER; ++i)
		{
			if(_nDBAccount == _DBAccount[i])
				return &_Battle_count[i];
		}

		return NULL;
	}

private:
	int		_nDBAccount;	
	int		_nWin;
	int		_nLose;
	int		_nScore;
	int		_nBattle_Count;	
	int		_nRank;
	UINT	_nBirthday;

	int    _DBAccount[MAX_USER];
	int    _Battle_count[MAX_USER];
};
