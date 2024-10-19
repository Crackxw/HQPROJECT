
#pragma once

#pragma warning ( disable : 4786 )

#include "user.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

class UserCatalog
{
public:
	UserCatalog()	{};
	~UserCatalog()	{ m_Catalog.clear(); };

	void				OpenDataFile();
	void				AddUser(string id, string pw, int nGrade);
	void				DelUser(string id);
	User				&GetUser(string id);
	BOOL				IsInvaliUser(string id, string pw);
	

private:

	map< string, User, less< string > >		m_Catalog;
	User									m_ErrorUser;
};