// ----------------------------------
// 트리를 이용한 욕설 필터링 알고리즘
//
// 이동찬님(chan78@esofnet.com)이 제공
// perpet 이 테스트
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