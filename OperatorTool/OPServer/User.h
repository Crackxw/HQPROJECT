
#pragma once

#include <iostream>
#include <string>

using namespace std;

class User
{
public:
	User()	{m_nGrade = -1;};

	void	Init(string id, string pw, int nGrade)
	{
		m_Id = id;
		m_Pw = pw;
		m_nGrade = nGrade;
	}

	string	GetID()		{ return m_Id; }
	string	GetPW()		{ return m_Pw; }
	int		GetGrade()	{ return m_nGrade; }

protected:	
	string	m_Id;
	string	m_Pw;
	int		m_nGrade;
};