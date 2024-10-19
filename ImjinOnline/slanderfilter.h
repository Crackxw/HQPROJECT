// ----------------------------------
// Ʈ���� �̿��� �弳 ���͸� �˰���
//
// �̵�����(chan78@esofnet.com)�� ����
// perpet �� �׽�Ʈ
// ----------------------------------
//#pragma once
#ifndef _SLANDERFILTER
#define	_SLANDERFILTER


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


typedef struct WordNode
{
	BOOL	bTermination;
	struct	WordNode *pNext[256];
} WORDNODE, *LPWORDNODE;


class CWordSearchTree
{
public:
	DWORD			dwNumOfWords;
	LPWORDNODE		pRoot;
	int				m_nMistakecount;
	int				m_nAllocateSize;


public:
	CWordSearchTree();
	~CWordSearchTree();

	BOOL			LoadReferenceWord( CHAR* pszNation, CHAR* pszLocalPath );
	BOOL			AddWord(char* szWord);
	void			ReleaseWordNode( LPWORDNODE pWordNode );

	char*			ReplaceString(char* szString);
	char*			ReplaceStringIgnoreBlank(char* szString);
	BOOL			JustSearchString(char* szString);
	BOOL			JustSearchStringIgnoreBlank(char* szString);
};


#endif