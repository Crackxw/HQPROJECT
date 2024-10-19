// ----------------------------------
// Ʈ���� �̿��� �弳 ���͸� �˰���
//
// �̵�����(chan78@esofnet.com)�� ����
// perpet �� �׽�Ʈ
// ----------------------------------
#include <GSL.h>
#include "slanderfilter.h"
#include "mbctype.h"

#include "BindJXFile.h"
#include "..\\SCCommon\\OnlineCommonParser.h"		// actdoll (2004/06/12 10:50) : ���� �ļ���



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
	// Ʈ�� free
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

	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ OnlineText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )				// �ϴ� �ܺ� ������ ����� ����� �Դٸ� Ȯ��
	{
		sprintf( pszFullPath, pszFileName, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )						// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName, pszNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}
	if( !fp )	return FALSE;	// �׷��� ��ã�Ҵٸ� ������.
	
	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	ocp.SetErrFileName( "c:\\ParseErr_SlanderFilterInfo.txt" );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "SlanderFilter Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]\n", pszFullPath, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �޴´�.
	while(1)
	{
		int		iLineNo, iColNo;

		if( ocp.ParseDataLine() == 0 )	break;		// ������ ������ �ϴ� �����ϰ�

		// ! ���� ! - GetValue�� ����ϱ� �����ߴٸ� �ش� ������ �ڷ�� �����ؼ� �ѹ��� �޵��� �Ѵ�.
		ocp.GetValue( word,	100,	iLineNo, iColNo );	// �����

		AddWord( word );								// �ܾ �߰��Ѵ�.
	}

	fclose(fp); 
	return TRUE;
}

//���͸��� �ܾ���� �߰��Ѵ�. 
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

// ����Լ�.
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

// �峻���� * �� �ٲپ���
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
				// ����.
				break;
			}

			if(pWordNode->bTermination)
			{
				// ���͸��� �ܾ��.
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

//�峻���� * �ιٲپ���.
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
				// ����.
				break;
			}

			if(pWordNode->bTermination)
			{
				// ���͸��� �ܾ��.
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

// ���� ������ �׳� TRUE�� ������ ��.
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
				// ����.
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

// ���� ������ �׳� TRUE�� ������ ��.
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
				// ����.
				break;
			}

			if(pWordNode->bTermination)
			{
				// ���͸��� �ܾ��.
				return TRUE;
			}
		}

		// For next Loop
		pStpInString = pStpInString + SkippedBlanks + 1;
		continue;
	}

	return FALSE;
}
