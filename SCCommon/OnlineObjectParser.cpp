/********************************************************************************************
	파일명	:	OnlineObjectParser.cpp
	작성자	:	정 진 욱
	작성일	:	2001.07.24
	수정일	:	2001.07.25
	내  용  :	오브젝트 파서
********************************************************************************************/

#include <GSL.h>

#include	"OnlineObjectParser.h"
#include	"BindJxFile.h"
#include	"OnlineText.h"

OnlineObjectParser::OnlineObjectParser()
{
	pObjectHeader	=	NULL;
	siObjectNum		=	0;
}

OnlineObjectParser::~OnlineObjectParser()
{
}

/********************************************************************************************
	오브젝트 텍스트를 받아 헤더에 삽입
********************************************************************************************/
BOOL	OnlineObjectParser::Init( CHAR *pFileName )
{
	FILE	*fp		= NULL;

	CHAR	*pToken = NULL;
	CHAR	szBuffer[1024];
	SI32	i;
	BOOL	bReadLine;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL )	return FALSE;

	// 총 몇 개의 오브젝트가 있는가 알아 본다.
	while( fgets( szBuffer, 1024, fp ) != NULL )
	{
		if( strstr( szBuffer, "ID:" ) != NULL )
			siObjectNum++;
	}


	fseek( fp, 0, SEEK_SET );											// 파일 포이터를 다쉬 처음으로 돌려 준다.
	fgets( szBuffer, 1024, fp );										// 한줄은 그냥 넘긴다.~~~	
	
	pObjectHeader	=	new ObjectHeader[ siObjectNum + 1 ];

	bReadLine = TRUE;

	for( i =1; i <= siObjectNum ; i++)
	{
		// 속성을 항상 초기화해 준다.
		pObjectHeader[i].siAttribute =	0;

		if(bReadLine == TRUE)
		{
			// 오프젝트의 이름을 얻는다.
			fgets( szBuffer, 1024, fp );		

			pToken		= strtok( szBuffer, ": \t\n\r" );
		}

#ifdef _DEBUG
		if(stricmp(pToken,"NAME") != 0)
		{
			TRACE("NAME 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken		= strtok( NULL, "\t\n:\r" );

		strcpy( pObjectHeader[i].szObjectName, pToken );

		
		// 오프젝트 ID를 얻는다.
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"ID") != 0)
		{
			TRACE("ID 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\r" );
		pObjectHeader[i].siKind		= atoi( pToken );

		if(pObjectHeader[i].siKind == 4228)
		{
			int j;
			j = 100;
		}
		if(pObjectHeader[i].siKind == 2368)
		{
			pObjectHeader[i].siKind = pObjectHeader[i].siKind;
		}

		// 파일의 이름을 얻는다.
		fgets( szBuffer, 1024, fp );		
		pToken		= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"FILENAME") != 0)
		{
			TRACE("FILENAME 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken		= strtok( NULL, ": \n\t\r" );
		strcpy( pObjectHeader[i].szObjectFileName, pToken );


		// 폰트를 얻는다.
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"FONT") != 0)
		{
			TRACE("FONT 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siFont		= atoi( pToken );

		
		// 중심 좌표 X
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"CENTERX") != 0)
		{
			TRACE("CENTERX 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siCenterX  = atoi( pToken );

		
		// 중심 좌표 Y
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"CENTERY") != 0)
		{
			TRACE("CENTERY 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siCenterY  = atoi( pToken );


		// 타일 크기 X
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"XSIZE") != 0)
		{
			TRACE("XSIZE 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siXsize	= atoi( pToken );


		// 타일 크기 Y
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"YSIZE") != 0)
		{
			TRACE("YSIZE 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siYsize	= atoi( pToken );

		// 속성
		fgets( szBuffer, 1024, fp );		
		pToken	= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"ATTRIBUTE") != 0)
		{
			TRACE("ATTRIBUTE 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken	= strtok( NULL, ": \n\t\r" );

		
		if( strstr( pToken, "O" )  != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_ISOBJECT;		// 갈수 있는가
		if( strstr( pToken, "A" )  != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_ANI;				// 애니매이션 이 되는가
		if( strstr( pToken, "N1" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC01;			// NPC Flag, Animation 1개
		if( strstr( pToken, "N2" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC02;			// NPC Flag, Animation 2개
		if( strstr( pToken, "N3" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC03;			// NPC Flag, Animation 3개
		if( strstr( pToken, "N4" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC04;			// NPC Flag, Animation 4개
		if( strstr( pToken, "N5" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC05;			// NPC Flag, Animation 5개
		if( strstr( pToken, "N6" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC06;			// NPC Flag, Animation 6개
		if( strstr( pToken, "N7" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC07;			// NPC Flag, Animation 7개
		if( strstr( pToken, "N8" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC08;			// NPC Flag, Animation 8개
		if( strstr( pToken, "N9" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC09;			// NPC Flag, Animation 9개
		if( strstr( pToken, "T" ) != NULL )		pObjectHeader[i].siAttribute |= ATB_OBJECT_TRANS;			// 투명하게 찍는 오브젝트
		
		// 총 프래임
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"TOTALFRAME") != 0)
		{
			TRACE("TOTALFRAME 구문이 빠졌습니다.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siFrame	= atoi( pToken );

		// MARK 부분.
		pObjectHeader[i].siMark		= ON_TEXT_UNUSED;

		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\r" );

		if(stricmp(pToken,"MARK") == 0)
		{
			bReadLine = TRUE;

			pToken						= strtok( NULL, ": \n\r" );
			pObjectHeader[i].siMark		= atoi( pToken );
		}
		else
		{
			bReadLine = FALSE;
		}
	}

	fclose( fp );
	return TRUE;
}

/********************************************************************************************
	오브젝트 겟수를 리턴
********************************************************************************************/
SI16	OnlineObjectParser::GetObjectNum()
{
	return siObjectNum;
}

/********************************************************************************************
	오브젝트 구조체를 날린다.
********************************************************************************************/
VOID	OnlineObjectParser::Free()
{
	if( pObjectHeader )
	{
		delete []pObjectHeader;
		pObjectHeader	=	NULL;
	}
}

/********************************************************************************************
	오브젝트 헤더를 리턴함
********************************************************************************************/
ObjectHeader	*OnlineObjectParser::GetObjectHeader()
{
	return pObjectHeader;
}
/********************************************************************************************
	오브젝트에서 스프라이트 파일 이름을 리턴을 해준다.
********************************************************************************************/
CHAR	*OnlineObjectParser::GetObjectFileName( SI16 siIndex )
{
	return pObjectHeader[ siIndex ].szObjectFileName;
}
/********************************************************************************************
	KIND 와 동일한 오브텍트 헤더를 리턴함 
********************************************************************************************/
ObjectHeader	*OnlineObjectParser::GetObject( SI16 siKind )
{
	SI16	i;
	for( i=1 ; i<siObjectNum+1 ; i++ )
	{
		if( pObjectHeader[i].siKind == siKind )
			return &pObjectHeader[i];
	}

	return NULL;
}