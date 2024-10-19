//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include	<stdio.h>
#include	<stdlib.h>
#include	<search.h>

#include	<HQFileManager.h>


#define _ON_CODE	0x10

HQFileManager::HQFileManager()
{
	pHead	  =	NULL;
	pFileHead = NULL;
	pFileData = NULL;

	TotalFileNum = 0;
}

HQFileManager::~HQFileManager()
{

}

FILE *HQFileManager::GetData(char *FileName)
{
	UINT	i = 0;

	while( stricmp( pHead[i++].szFileName, FileName ) )
	{
		fseek( pFileData, SEEK_SET, pHead[i].FIlePos );
		return pFileData;
	}
	
	return NULL;
}

BOOL HQFileManager::HqFileOpen(char *pFileNameHead, char *pFileNameData)
{
	if( ( pFileHead = fopen( pFileNameHead, "r" ) ) == NULL )  return FALSE;
	if( ( pFileData = fopen( pFileNameData, "rb" ) ) == NULL ) return FALSE;

	CHAR	LineBuffer[512];	
	CHAR	*pToken = NULL;
	UINT	nCount  = 0;

	//먼저 총 파일 수를 읽는다
	fgets( LineBuffer, 512, pFileHead );

	DeCode( LineBuffer );

	pToken = strtok( LineBuffer, "[]\n:" );
	pToken = strtok( NULL, "[]\n:" );


	TotalFileNum = atoi( pToken );
	
	pHead = new FILEHEADER[TotalFileNum];

	while( ( fgets( LineBuffer, 512, pFileHead ) ) != NULL )	
		sscanf( LineBuffer, "%s\t[%d]\t[%d]", pHead[nCount].szFileName, &pHead[nCount].FileSize, &pHead[nCount].FIlePos );

	fclose( pFileHead );
	fclose( pFileData );

	pFileHead	=	NULL;
	pFileData	=	NULL;

	return TRUE;
}

VOID HQFileManager::DeCode( char *pCode )
{
	SI32	i;

	for( i=0 ; i<(SI32)strlen(pCode); i++ )
	{
		if( pCode[ i ] & _ON_CODE )					pCode[i] &=~ _ON_CODE;
		else										pCode[i] |=  _ON_CODE;
	}
}

VOID	HQFileManager::FileDeCode( char *pFileNameHead, char *pFileNameData )
{
	FILE	*pFile;
	FILE	*pMergeFile;
	pFile		=	fopen( pFileNameHead, "r" );
	pMergeFile	=	fopen( pFileNameData, "rb" );


	if( pFile == NULL )			return;
	if( pMergeFile == NULL )	return;

	FILE* Write =	NULL;	
	
	char	Line[1024];
	char	Copy[1024];
	char   *ptoken = NULL;

	LONG			dummy;
	fpos_t			pos;
	BOOL			flag;

	BYTE*	pTemp		= NULL;		

	UINT	size		= 0;
	UINT	ix          = 0;
	UINT	uCnt		= 0;

	char	Dir[1024]	= {0};


	//총 라인수를 그냥 무시 하고 넘긴다.
	fgets( Line, 1024, pFile );
	DeCode( Line );


	sscanf( Line, "%s : [%d]", Copy, &TotalFileNum );
	pHead = new FILEHEADER[TotalFileNum];
	
	ZeroMemory( pHead, sizeof( FILEHEADER ) * TotalFileNum );
	
	while( ( fgets( Line, 1024, pFile ) ) != NULL )
	{
		strcpy( Copy, Line );

		for(ix=0 ; ix < strlen( Copy ) ; ix++)
		{
			if( Copy[ix] & 0x10 )		Copy[ix] &=~ 0x10;
			else						Copy[ix] |=  0x10;

			if( Line[ix] & 0x10 )		Line[ix] &=~ 0x10;
			else						Line[ix] |=  0x10;
		}

		ptoken		= strtok( Line, "\t[]" );		
		strcpy( pHead[uCnt].szFileName, ptoken );					// 헤더에 파일 이름을 복사한다.		

		strcpy( Dir, ptoken );

		flag = Createdirectory( Dir, uCnt );
		uCnt++;

		if( ( Write = fopen( Dir, "ra" ) ) != NULL )
		{
			fclose(Write);
			if( DeleteFile( Dir ) == FALSE )
			{
				SetFileAttributes(Dir, FILE_ATTRIBUTE_ARCHIVE);
				DeleteFile( Dir );
			}

		}
		if( ( Write = fopen( Dir, "wb" ) ) == NULL )	continue;

		if( !flag )	
		{
			//폴더를 생성을 하지 않을 경우
			ptoken  = strtok( NULL, "\t[]" );
			size    = atoi( ptoken );

			ptoken  = strtok( NULL, "\t\n[]" );
			dummy	= atoi( ptoken );
		}

		else		
		{
			//폴더를 생성을 한다.
			//약간 이상해서.. 따로 한다.. 이유는 몰러... ㅡㅡ;
			//토큰한 내용이 변해서..copy에 저장을 했다강...
			ptoken  = strtok( Copy, "\t[]" );

			ptoken  = strtok( NULL, "\t[]" );
			size    = atoi( ptoken );			

			ptoken  = strtok( NULL, "\t[]\n" );
			dummy	= atoi( ptoken );
		}

		pTemp   = new BYTE[ size ];	
		BYTE *pTempBuffer = new BYTE[size];		
		memset( pTemp, 0, sizeof( BYTE ) * size );		
		pos     = dummy;

		
		fsetpos(pMergeFile, &pos);
		fread( pTemp, size, 1, pMergeFile );
		memcpy(pTempBuffer, pTemp, size);
		
		for(ix=0; ix< size ; ix++)
		{
			if( pTempBuffer[ix] & 0x10 )		pTempBuffer[ix] &=~ 0x10;
			else								pTempBuffer[ix] |=  0x10;
		}

		fwrite( pTempBuffer, size, 1, Write );

		fclose( Write );
		
		delete []pTemp;
		delete []pTempBuffer;
		pTemp = NULL;
	}
	fclose( pFile );
	fclose( pMergeFile );
}

//========================================================================================
//	함수명 : Createdirectory
//	내  용 : 디렉토를 만들어 준다.
//========================================================================================
BOOL HQFileManager::Createdirectory(char *dir, UINT index, BOOL bDir )
{
	CHAR	*pToken;	
	CHAR	CopiedDir[1024];	
	CHAR	ChildDir[1024];		
	
	if(strstr(dir,"\\") == NULL)		return FALSE;

	strcpy(CopiedDir, dir);

	// 폴더 생성.
	if( bDir )
	{
		pToken = strtok( CopiedDir, "\\" );
		wsprintf( ChildDir, "%s", pToken );		

		while( ( pToken = strtok( NULL, "\n\\" ) ) != NULL )
		{
			if( strstr( pToken, "." ) == NULL )
			{			
				wsprintf( ChildDir, "%s\\%s", ChildDir, pToken );
				strcpy( pHead[index].szDirectory, ChildDir );
				CreateDirectory(ChildDir, NULL);
			}
		}
		
		return TRUE;
	}
	else
	{
		pToken	= strtok(CopiedDir, "\\");
		CreateDirectory(pToken, NULL);
		pToken  = strtok(NULL, "\n" );
	}

	if( strstr( pToken, "\\" ) == NULL )	return TRUE;
	else									Createdirectory(dir, index, TRUE);

	return TRUE;
}

//========================================================================================
//	함수명 : Free
//	내  용 : 만들었던 모든 디렉토리와 파일을 제거 한다.
//========================================================================================
VOID	HQFileManager::Free()
{
	UINT	i;
	CHAR	szDirectory[1024];
	CHAR	szTemp[1024];

	GetCurrentDirectory( 1024, szDirectory );

	for( i = 0; i<TotalFileNum; i++ )	
	{
		wsprintf( szTemp, "%s\\%s", szDirectory, pHead[i].szFileName );
		DeleteFile( szTemp );			
	}
	
	// 디렉토리에 파일이 남아 있으면 폴더가 제거가 되지 않기 때문에 나중에 다쉬 지워 준다.
	for( i = 0; i<TotalFileNum; i++)	
		RemoveDirectory( pHead[i].szDirectory );
	

	if( pHead )
	{
		delete []pHead;
		pHead	=	NULL;
	}
}