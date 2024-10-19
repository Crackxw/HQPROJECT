/********************************************************************************************
	���ϸ�	:	OnlineObjectParser.cpp
	�ۼ���	:	�� �� ��
	�ۼ���	:	2001.07.24
	������	:	2001.07.25
	��  ��  :	������Ʈ �ļ�
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
	������Ʈ �ؽ�Ʈ�� �޾� ����� ����
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

	// �� �� ���� ������Ʈ�� �ִ°� �˾� ����.
	while( fgets( szBuffer, 1024, fp ) != NULL )
	{
		if( strstr( szBuffer, "ID:" ) != NULL )
			siObjectNum++;
	}


	fseek( fp, 0, SEEK_SET );											// ���� �����͸� �ٽ� ó������ ���� �ش�.
	fgets( szBuffer, 1024, fp );										// ������ �׳� �ѱ��.~~~	
	
	pObjectHeader	=	new ObjectHeader[ siObjectNum + 1 ];

	bReadLine = TRUE;

	for( i =1; i <= siObjectNum ; i++)
	{
		// �Ӽ��� �׻� �ʱ�ȭ�� �ش�.
		pObjectHeader[i].siAttribute =	0;

		if(bReadLine == TRUE)
		{
			// ������Ʈ�� �̸��� ��´�.
			fgets( szBuffer, 1024, fp );		

			pToken		= strtok( szBuffer, ": \t\n\r" );
		}

#ifdef _DEBUG
		if(stricmp(pToken,"NAME") != 0)
		{
			TRACE("NAME ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken		= strtok( NULL, "\t\n:\r" );

		strcpy( pObjectHeader[i].szObjectName, pToken );

		
		// ������Ʈ ID�� ��´�.
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"ID") != 0)
		{
			TRACE("ID ������ �������ϴ�.\n");
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

		// ������ �̸��� ��´�.
		fgets( szBuffer, 1024, fp );		
		pToken		= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"FILENAME") != 0)
		{
			TRACE("FILENAME ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken		= strtok( NULL, ": \n\t\r" );
		strcpy( pObjectHeader[i].szObjectFileName, pToken );


		// ��Ʈ�� ��´�.
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"FONT") != 0)
		{
			TRACE("FONT ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siFont		= atoi( pToken );

		
		// �߽� ��ǥ X
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"CENTERX") != 0)
		{
			TRACE("CENTERX ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siCenterX  = atoi( pToken );

		
		// �߽� ��ǥ Y
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"CENTERY") != 0)
		{
			TRACE("CENTERY ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siCenterY  = atoi( pToken );


		// Ÿ�� ũ�� X
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"XSIZE") != 0)
		{
			TRACE("XSIZE ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siXsize	= atoi( pToken );


		// Ÿ�� ũ�� Y
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"YSIZE") != 0)
		{
			TRACE("YSIZE ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siYsize	= atoi( pToken );

		// �Ӽ�
		fgets( szBuffer, 1024, fp );		
		pToken	= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"ATTRIBUTE") != 0)
		{
			TRACE("ATTRIBUTE ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken	= strtok( NULL, ": \n\t\r" );

		
		if( strstr( pToken, "O" )  != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_ISOBJECT;		// ���� �ִ°�
		if( strstr( pToken, "A" )  != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_ANI;				// �ִϸ��̼� �� �Ǵ°�
		if( strstr( pToken, "N1" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC01;			// NPC Flag, Animation 1��
		if( strstr( pToken, "N2" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC02;			// NPC Flag, Animation 2��
		if( strstr( pToken, "N3" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC03;			// NPC Flag, Animation 3��
		if( strstr( pToken, "N4" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC04;			// NPC Flag, Animation 4��
		if( strstr( pToken, "N5" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC05;			// NPC Flag, Animation 5��
		if( strstr( pToken, "N6" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC06;			// NPC Flag, Animation 6��
		if( strstr( pToken, "N7" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC07;			// NPC Flag, Animation 7��
		if( strstr( pToken, "N8" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC08;			// NPC Flag, Animation 8��
		if( strstr( pToken, "N9" ) != NULL )	pObjectHeader[i].siAttribute |= ATB_OBJECT_NPC09;			// NPC Flag, Animation 9��
		if( strstr( pToken, "T" ) != NULL )		pObjectHeader[i].siAttribute |= ATB_OBJECT_TRANS;			// �����ϰ� ��� ������Ʈ
		
		// �� ������
		fgets( szBuffer, 1024, fp );		
		pToken						= strtok( szBuffer, ": \n\t\r" );

#ifdef _DEBUG
		if(stricmp(pToken,"TOTALFRAME") != 0)
		{
			TRACE("TOTALFRAME ������ �������ϴ�.\n");
			EXIT();
		}
#endif

		pToken						= strtok( NULL, ": \n\t\r" );
		pObjectHeader[i].siFrame	= atoi( pToken );

		// MARK �κ�.
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
	������Ʈ �ټ��� ����
********************************************************************************************/
SI16	OnlineObjectParser::GetObjectNum()
{
	return siObjectNum;
}

/********************************************************************************************
	������Ʈ ����ü�� ������.
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
	������Ʈ ����� ������
********************************************************************************************/
ObjectHeader	*OnlineObjectParser::GetObjectHeader()
{
	return pObjectHeader;
}
/********************************************************************************************
	������Ʈ���� ��������Ʈ ���� �̸��� ������ ���ش�.
********************************************************************************************/
CHAR	*OnlineObjectParser::GetObjectFileName( SI16 siIndex )
{
	return pObjectHeader[ siIndex ].szObjectFileName;
}
/********************************************************************************************
	KIND �� ������ ������Ʈ ����� ������ 
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