#include "UserCatalog.h"
/*
bool greater<User>::operator() (User& x, User & y)
{
	return x.compare_rank(y);
}


struct greater
{
    bool operator()(User & x, User & y) const
	{
	}
}
*/

UserCatalog::UserCatalog()
{
	vector<User>	user(3000);	
	_Users.swap(user);

	_addCount = _Users.begin();
}

void UserCatalog::AddUser(DWORD nDBAccount, int win, int lose, int score, int battle_count, char *pBirthday, int *pDBAccount, int *pBattle_count)
{
	(*_addCount).Init(nDBAccount, win, lose, score, battle_count, pBirthday, pDBAccount, pBattle_count);
	++_addCount;
}

void UserCatalog::sortByUniqueID()
{
	_sortbyUniqueID.clear();

	for(vector<User>::iterator i=_Users.begin(); i!=_Users.end(); ++i)
		_sortbyUniqueID.insert(UNIQUE::value_type(i->GetDBAccount(), i));
}

void UserCatalog::sortByScore()
{
	multimap<int, User*, greater<int> >	ScoreCompare;

	for(vector<User>::iterator i=_Users.begin(); i!=_Users.end(); ++i)
		ScoreCompare.insert(multimap<int, User*, greater<int> >::value_type(i->GetScore(), i));

	int nTempRank=1;
	for(multimap<int, User*, greater<int> >::iterator ix=ScoreCompare.begin(); ix!=ScoreCompare.end(); ++ix, ++nTempRank)
		ix->second->SetRank(nTempRank);

	ScoreCompare.clear();
}

User &UserCatalog::GetUser(int nUnqueID)
{
	UNIQUE::iterator find = _sortbyUniqueID.lower_bound(nUnqueID);
	
	if(find != _sortbyUniqueID.end() && !(_sortbyUniqueID.key_comp()(find->first, nUnqueID)))
		return *find->second;

	return _ErrorUser;
}

void UserCatalog::Free()
{
	_Users.clear();	
	_sortbyUniqueID.clear();
}