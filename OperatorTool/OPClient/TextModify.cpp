
#include "stdafx.h"
#include "OPClient.h"
#include <afx.h>

#include "TextModify.h"
#include "utility.h"
#include "OPClientDlg.h"
#include "ModifyDlg.h"

void TextModify::AddText(char *pText, UINT id)
{
	TextType	text;

	text.id = id;
	strcpy(text.szText, pText);

	efficientAddOrUpdata(m_ModifyTexts, id, text);
}

void TextModify::AddQuest(char *pText, UINT id)
{
	TextType	text;

	text.id = id;
	strcpy(text.szText, pText);

	efficientAddOrUpdata(m_ModifyQuest, id, text);
}

void TextModify::AddHelp(char *pText, UINT id)
{
	TextType	text;

	text.id = id;
	strcpy(text.szText, pText);

	efficientAddOrUpdata(m_ModifyHelp, id, text);
}

void TextModify::Open()
{
	Open("����\\Text.txt", &m_ModifyTexts);
	Open("����\\Quest.txt", &m_ModifyQuest);
	Open("����\\Help.txt", &m_ModifyHelp);
}

void TextModify::Open(char *pFileName, map< UINT, TextType, less<UINT> > *p)
{
	FILE *fp = fopen( pFileName, "rt");
	if(!fp)	return;

	char	szTempBuffer[2048];
	char	text[1024];
	UINT	id;

	while(fgets(szTempBuffer, 1024, fp))
	{
		sscanf(szTempBuffer, "%08d\t%s\n", &id, text);
	}
	
	fclose(fp);
}

void TextModify::SaveAndFree()
{
	SaveFile("����\\Text.txt", &m_ModifyTexts);
	SaveFile("����\\Quest.txt", &m_ModifyQuest);
	SaveFile("����\\Help.txt", &m_ModifyHelp);
}

void TextModify::SaveFile(char *pFileName, map< UINT, TextType, less<UINT> > *p)
{
	FILE *fp = fopen( pFileName, "wt");
	if(!fp)	return;

	for(map< UINT, TextType, less<UINT> >::iterator i=p->begin(); i!=p->end(); ++i)
	{
		fprintf(fp, "%08d\t%s\n", i->second.id, i->second.szText);
	}
	
	fclose(fp);
}