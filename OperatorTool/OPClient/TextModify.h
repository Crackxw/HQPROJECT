#pragma once

#pragma warning ( disable : 4786 )

#include <iostream>
#include <map>
#include <string>

using namespace std;

struct TextType
{
	char    szText[1024];
	UINT	id;
};	

class COPClientDlg;
class TextModify
{
public:
	TextModify()	{ Open(); }
	~TextModify()	{ m_ModifyTexts.clear(); m_ModifyQuest.clear(); m_ModifyHelp.clear(); }

	void		AddText(char *pText, UINT id);
	void		AddQuest(char *pText, UINT id);
	void		AddHelp(char *pText, UINT id);

	void		Open();
	void		SaveAndFree();

private:
	void		Open(char *pFileName, map< UINT, TextType, less<UINT> > *p);
	void		SaveFile(char *pFileName, map< UINT, TextType, less<UINT> > *p);
	
	

private:	
	map< UINT, TextType, less<UINT> >		m_ModifyTexts;
	map< UINT, TextType, less<UINT> >		m_ModifyQuest;
	map< UINT, TextType, less<UINT> >		m_ModifyHelp;
};