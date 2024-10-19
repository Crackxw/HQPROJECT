#pragma once


#pragma warning ( disable : 4786 )

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#include "user.h"

class UserCatalog
{
public:
	UserCatalog();
	~UserCatalog()					{ Free(); }

	void	AddUser(DWORD nDBAccount, int win, int lose, int score, int battle_count, char *pBirthday, int *pDBAccount, int *pBattle_count);	
	User	&GetUser(int nUnqueID);
	User	&GetIndex(int index)	{ return _Users[index]; }

private:
	void	Free();

private:	
	friend class ExpoRank;

	void	sortByScore();	
	void	sortByUniqueID();	

private:	
	typedef map<int, User*, less<int> >			UNIQUE;


	vector<User>			_Users;	
	vector<User>::iterator	_addCount;
	UNIQUE					_sortbyUniqueID;

	User					_ErrorUser;
};