
#include <windows.h>
#include "UserCatalog.h"
#include "utility.h"
#include "..\\common\\defines.h"

void UserCatalog::AddUser(string id, string pw, int nGrade)
{
	User AddUser;

	AddUser.Init(id, pw, nGrade);

	efficientAddOrUpdata(m_Catalog, id, AddUser);
}

void UserCatalog::DelUser(string id)
{
	map< string, User, less< string > >::iterator Find = m_Catalog.lower_bound(id);

	if(Find != m_Catalog.end() && !(m_Catalog.key_comp()(id, Find->first)))
		m_Catalog.erase(Find);
}

User &UserCatalog::GetUser(string id)
{
	map< string, User, less< string > >::iterator Find = m_Catalog.lower_bound(id);

	if(Find != m_Catalog.end() && !(m_Catalog.key_comp()(id, Find->first)))
	{
		return Find->second;
	}

	return m_ErrorUser;
}

int UserCatalog::IsInvaliUser(string id, string pw)
{
	User user = GetUser(id);

	if(user.GetGrade() == -1)	return ERROR_ID;
	if(user.GetPW() != pw)		return ERROR_PW;

	return user.GetGrade();
}

void UserCatalog::OpenDataFile()
{
	m_Catalog.clear();

	FILE *fp = fopen("usercatalog.ini", "rt");

	char szID[512];
	char szPW[512];
	char szGrade[512];
	char szIP[512];

	while(fgets(szID, 512, fp))
	{
		// 비번
		fgets(szPW, 512, fp);

		// 등급
		fgets(szGrade, 512, fp);

		// IP
		fgets(szIP, 512, fp);

		szID[strlen(szID)-1] = 0;
		szPW[strlen(szPW)-1] = 0;
		szGrade[strlen(szGrade)-1] = 0;

		AddUser(szID, szPW, atoi(szGrade));
	}
}