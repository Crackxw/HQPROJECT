// ----------------------------------
// 트리를 이용한 욕설 필터링 알고리즘
//
// 이동찬님(chan78@esofnet.com)이 제공
// perpet 이 테스트
// ----------------------------------
#include <GSL.h>
#include "slanderfilter.h"
#include "mbctype.h"

#include "BindJXFile.h"
#include "..\\SCCommon\\OnlineCommonParser.h"		// actdoll (2004/06/12 10:50) : 공용 파서기



CWordSearchTree::CWordSearchTree()
{
	this->dwNumOfWords = 0;
	this->pRoot        = new WORDNODE;
	memset(this->pRoot, 0, sizeof(WORDNODE));

	m_nAllocateSize = 0;
	m_nAllocateSize += sizeof(WORDNODE);
	m_nMistakecount =  0;
	m_nAllocateSize =  0;
}

CWordSearchTree::~CWordSearchTree()
{
	// 트리 free
	this->ReleaseWordNode(this->pRoot);
}

BOOL CWordSearchTree::LoadReferenceWord( CHAR* pszNation, CHAR* pszLocalPath )
{
	OnlineCommonParser	ocp;
	FILE*	fp								= NULL;
	char	*pToken							= NULL;
	char	word[100]						= {0,};
	char	*pszFileName					= "%s\\SlanderFilterInfo.dat";
	char	pszFullPath[MAX_PATH];

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 OnlineText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )				// 일단 외부 폴더를 쓰라고 명령이 왔다면 확인
	{
		sprintf( pszFullPath, pszFileName, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )						// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName, pszNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !fp )	return FALSE;	// 그래도 못찾았다면 에러다.
	
	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	ocp.SetErrFileName( "c:\\ParseErr_SlanderFilterInfo.txt" );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "SlanderFilter Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFullPath, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 받는다.
	while(1)
	{
		int		iLineNo, iColNo;

		if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인을 일단 추출하고

		// ! 주의 ! - GetValue를 사용하기 시작했다면 해당 라인의 자료는 연속해서 한번에 받도록 한다.
		ocp.GetValue( word,	100,	iLineNo, iColNo );	// 설명글

		AddWord( word );								// 단어를 추가한다.
	}

	fclose(fp); 
	return TRUE;
}

//필터링할 단어들을 추가한다. 
BOOL CWordSearchTree::AddWord(char *szWord)
{
	char*			pDummy;
	LPWORDNODE		pWordNode;
	LPWORDNODE		pBeforeNode;

	for(pWordNode = this->pRoot, pDummy = szWord; pDummy && ((*pDummy != '\0') && (*pDummy != '\n')); pDummy++)
	{
		pBeforeNode = pWordNode;

		if((pWordNode = pWordNode->pNext[(BYTE)(*pDummy)]) == NULL)
		{
			pWordNode = new WORDNODE;
			memset(pWordNode, 0, sizeof(WORDNODE));
			m_nAllocateSize                     += sizeof(WORDNODE);
			pBeforeNode->pNext[(BYTE)(*pDummy)] =  pWordNode;
		}
	}
	pWordNode->bTermination = TRUE;

	return TRUE;
}

// 재귀함수.
void CWordSearchTree::ReleaseWordNode(LPWORDNODE pWordNode)
{
	for(DWORD i = 0; i < 256; i++)
	{
		if(pWordNode)
		{
			if(pWordNode->pNext[i])
			{
				ReleaseWordNode(pWordNode->pNext[i]);
			}
		}
	}

	delete pWordNode;
}

// 욕내용을 * 로 바꾸어줌
char* CWordSearchTree::ReplaceString(char* szString)
{
	char*		pStpInString;
	char*		pStpInWord;
	LPWORDNODE	pWordNode;
	DWORD		dwLength;

	for(pStpInString = szString; pStpInString && ((*pStpInString != '\0') && (*pStpInString != '\n'));)
	{
		for(pWordNode = this->pRoot, pStpInWord = pStpInString, dwLength = 1; pStpInWord && ((*pStpInWord != '\0') && (*pStpInWord != '\n') && (*pStpInWord != ' ')); pStpInWord++, dwLength++)
		{
			if((pWordNode = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{
				// 없다.
				break;
			}

			if(pWordNode->bTermination)
			{
				// 필터링할 단어다.
				for(DWORD dwDummy = 0; dwDummy < dwLength; dwDummy++)
				{
					*pStpInString++ = '*';
				}

				goto SomethingFiltered;
			}
		}

		// For next Loop
		pStpInString++;
		continue;

SomethingFiltered:
		continue;
	}

	return szString;
}

//욕내용을 * 로바꾸어줌.
char* CWordSearchTree::ReplaceStringIgnoreBlank(char* szString)
{
	char*			pStpInString;
	char*			pStpInWord;
	LPWORDNODE		pWordNode;
	DWORD			dwLength;
	DWORD			SkippedBlanks;
	int				addCount;

	for(pStpInString = szString; pStpInString && ((*pStpInString != '\0') && (*pStpInString != '\n'));)
	{
		addCount = 0;
		if(_ismbblead(*pStpInString)) addCount = 1;

		for(pWordNode = this->pRoot, SkippedBlanks = 0, pStpInWord = pStpInString, dwLength = 1; pStpInWord && ((*pStpInWord != '\0') && (*pStpInWord != '\n')); pStpInWord++, dwLength++)
		{
			if(*pStpInWord == ' ')
			{
				continue;
			}

			if((pWordNode = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{
				// 없다.
				break;
			}

			if(pWordNode->bTermination)
			{
				// 필터링할 단어다.
				for(DWORD dwDummy = 0; dwDummy < (dwLength+SkippedBlanks); dwDummy++)
				{
					*pStpInString++ = ((*pStpInString == ' ') ? ' ' : '*');
				}

				goto SomethingFiltered;
			}
		}

		// For next Loop
		pStpInString = pStpInString + SkippedBlanks + 1 + addCount;

SomethingFiltered:
		continue;
	}

	return szString;
}

// 욕이 있으면 그냥 TRUE로 나오게 함.
BOOL CWordSearchTree::JustSearchString(char* szString)
{
	char*			pStpInString;
	char*			pStpInWord;
	LPWORDNODE		pWordNode;
	DWORD			dwLength;

	for(pStpInString = szString; pStpInString && ((*pStpInString != '\0') && (*pStpInString != '\n'));)
	{
		for(pWordNode = this->pRoot, pStpInWord = pStpInString, dwLength = 1; pStpInWord && ((*pStpInWord != '\0') && (*pStpInWord != '\n') && (*pStpInWord != ' ')); pStpInWord++, dwLength++)
		{
			if((pWordNode = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{
				// 없다.
				break;
			}

			if(pWordNode->bTermination)
			{
				return TRUE;
			}
		}

		// For next Loop
		pStpInString++;
		continue;
	}

	return FALSE;
}

// 욕이 있으면 그냥 TRUE로 나오게 함.
BOOL CWordSearchTree::JustSearchStringIgnoreBlank(char* szString)
{
	char*			pStpInString;
	char*			pStpInWord;
	LPWORDNODE		pWordNode;
	DWORD			dwLength;
	DWORD			SkippedBlanks;

	for(pStpInString = szString; pStpInString && ((*pStpInString != '\0') && (*pStpInString != '\n'));)
	{
		for(pWordNode = this->pRoot, SkippedBlanks = 0, pStpInWord = pStpInString, dwLength = 1; pStpInWord && ((*pStpInWord != '\0') && (*pStpInWord != '\n')); pStpInWord++, dwLength++)
		{
			if(*pStpInWord == ' ')
			{
				continue;
			}

			if((pWordNode = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{
				// 없다.
				break;
			}

			if(pWordNode->bTermination)
			{
				// 필터링할 단어다.
				return TRUE;
			}
		}

		// For next Loop
		pStpInString = pStpInString + SkippedBlanks + 1;
		continue;
	}

	return FALSE;
}
