#include <GSL.h>

#include	"GSCDefine.h"
#include	"OnlineTip-Parser.h"
#include	"bindJXFile.h"


OnlineTipParser::OnlineTipParser()
{
//	pOnlineTipHeader = NULL;
//	m_siTipNum		 = 0;
	pOnlineMainTipHeader = NULL;
	pOnlineTipHeader     = NULL;
	pOnlineSubContentsHeader = NULL;

	m_iMainNum     = 0;
	m_iSubjectNum  = 0;
	m_iContentsNum = 0;
}

OnlineTipParser::~OnlineTipParser()
{
	Free();
}

BOOL OnlineTipParser::Free()
{


	int i;

	if(pOnlineMainTipHeader->pOnlineTipContentsHeader)
	{
		delete[] pOnlineMainTipHeader->pOnlineTipContentsHeader;
		pOnlineMainTipHeader->pOnlineTipContentsHeader = NULL;
	}
	pOnlineMainTipHeader->pOnlineTipContentsHeader = NULL;
	pOnlineMainTipHeader->pOnlineTipContentsHeader = 0;

	if(pOnlineMainTipHeader)
	{
		delete[] pOnlineMainTipHeader; 
		pOnlineMainTipHeader = NULL;
	}
	pOnlineMainTipHeader = NULL;
	pOnlineMainTipHeader = 0;

	for(i = 0 ;i < 16; i++)
	{
		if(pOnlineTipHeader[i].pOnlineTipContentsHeader)
		{
			delete[] pOnlineTipHeader[i].pOnlineTipContentsHeader;
			pOnlineTipHeader[i].pOnlineTipContentsHeader = NULL;
		}
		pOnlineTipHeader[i].pOnlineTipContentsHeader = NULL;
		pOnlineTipHeader[i].pOnlineTipContentsHeader = 0;
	}

	if(pOnlineTipHeader)
	{
		delete[] pOnlineTipHeader;
		pOnlineTipHeader = NULL;
	}
	pOnlineTipHeader = NULL;
	pOnlineTipHeader = 0;

	for(i = 0; i < 12 ; i++)
	{
		if(pOnlineSubContentsHeader[i].pOnlineTipContentsHeader)
		{
			delete[] pOnlineSubContentsHeader[i].pOnlineTipContentsHeader;
			pOnlineSubContentsHeader[i].pOnlineTipContentsHeader = NULL;
		}
		pOnlineSubContentsHeader[i].pOnlineTipContentsHeader = NULL;
		pOnlineSubContentsHeader[i].pOnlineTipContentsHeader = 0;
	}

	if(pOnlineSubContentsHeader)
	{
		delete[] pOnlineSubContentsHeader;
		pOnlineSubContentsHeader = NULL;
	}
	pOnlineSubContentsHeader = NULL;
	pOnlineSubContentsHeader = 0;

	return TRUE;
}


BOOL OnlineTipParser::ReadText(char* filename)
{
	FILE	*fp		= NULL;
	CHAR	*pToken = NULL;
	CHAR	szBuffer[1024];
	UI32	j;
	SI16    i;

	
	if((fp = BindJXFile::GetInstance()->GetFile( filename )) == NULL) return FALSE;		

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	
	pToken						= strtok(szBuffer,"\t\n\r");
	
	pToken						= strtok(NULL,"\t\n\r");
	m_iMainNum                  = atoi(pToken);

	pToken						= strtok(NULL,"\t\n\r");
	m_iSubjectNum				= atoi(pToken);

	pToken						= strtok(NULL,"\t\n\r");
	m_iContentsNum				= atoi(pToken);


	fgets( szBuffer, 1024, fp );

	
	

	fgets( szBuffer, 1024, fp );

	pOnlineMainTipHeader		= new OnlineTipHeader[m_iMainNum];
	ZeroMemory(pOnlineMainTipHeader,sizeof(OnlineTipHeader) * m_iMainNum);
	
	pToken										=	strtok( szBuffer, "\t\n\r" );
	pOnlineMainTipHeader[0].uiMainSubjectNum		=   atoi( pToken );

	pToken										=	strtok( NULL, "\t\n\r" );
	pOnlineMainTipHeader[0].uiDescription       =   atoi(pToken);

	pToken										=   strtok(NULL,"\t\n\r");
	pOnlineMainTipHeader[0].uiTotalTipHeader      =   atoi(pToken);

	pToken										=	strtok( NULL, "\t\n\r" );			
	pOnlineMainTipHeader[0].uiMainSubjectIndex		=   atoi( pToken );

	

	pOnlineMainTipHeader[0].pOnlineTipContentsHeader = new OnlineTipContentsHeader[pOnlineMainTipHeader[0].uiTotalTipHeader];

	for(j = 0 ; j < pOnlineMainTipHeader[0].uiTotalTipHeader ; j++)
	{
		ZeroMemory(&pOnlineMainTipHeader[0].pOnlineTipContentsHeader[j],sizeof(OnlineTipContentsHeader));

		pToken	=	strtok( NULL, "\t\n\r" );
		pOnlineMainTipHeader[0].pOnlineTipContentsHeader[j].uiSubject = atoi( pToken );

		pToken	=	strtok( NULL, "\t\n\r" );
		pOnlineMainTipHeader[0].pOnlineTipContentsHeader[j].uiContents = atoi( pToken );

		pToken	=	strtok( NULL, "\t\n\r" );
		pOnlineMainTipHeader[0].pOnlineTipContentsHeader[j].uiVariable = atoi( pToken );

		pToken	=	strtok( NULL, "\t\n\r" );
		pOnlineMainTipHeader[0].pOnlineTipContentsHeader[j].uicode = atoi( pToken );
	}


	pOnlineTipHeader			= new OnlineTipHeader[m_iSubjectNum];
	ZeroMemory(pOnlineTipHeader,sizeof(OnlineTipHeader) * m_iSubjectNum);

//	for(i = 0; i < m_iSubjectNum ; i++)
//	{
//		pOnlineTipHeader[i].pOnlineTipContentsHeader = new OnlineTipContentsHeader[20];
//	}


	for(i = 0 ;i < m_iSubjectNum; i++)
	{
		fgets( szBuffer, 1024, fp );

		pToken										=	strtok( szBuffer, "\t\n\r" );
		pOnlineTipHeader[i].uiMainSubjectNum		=   atoi( pToken );

		pToken										=	strtok( NULL, "\t\n\r" );
		
		pToken										=   strtok( NULL, "\t\n\r" );
		pOnlineTipHeader[i].uiTotalTipHeader        =   atoi(pToken);

		pToken										=	strtok( NULL, "\t\n\r" );			
		pOnlineTipHeader[i].uiMainSubjectIndex		=   atoi( pToken );

		pOnlineTipHeader[i].pOnlineTipContentsHeader = new OnlineTipContentsHeader[pOnlineTipHeader[i].uiTotalTipHeader];

		for(j = 0 ; j < pOnlineTipHeader[i].uiTotalTipHeader ; j++)
		{
			ZeroMemory(&pOnlineTipHeader[i].pOnlineTipContentsHeader[j],sizeof(OnlineTipContentsHeader));

			pToken	=	strtok( NULL, "\t\n\r" );
			pOnlineTipHeader[i].pOnlineTipContentsHeader[j].uiSubject = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n\r" );
			pOnlineTipHeader[i].pOnlineTipContentsHeader[j].uiContents = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n\r" );
			pOnlineTipHeader[i].pOnlineTipContentsHeader[j].uiVariable = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n\r" );
			pOnlineTipHeader[i].pOnlineTipContentsHeader[j].uicode = atoi( pToken );
		}
	}

	pOnlineSubContentsHeader	= new OnlineTipHeader[m_iContentsNum];
	ZeroMemory(pOnlineSubContentsHeader,sizeof(OnlineTipHeader) * m_iContentsNum);

//	for(i = 0; i < m_iContentsNum ; i++) 
//	{
//		pOnlineSubContentsHeader[i].pOnlineTipContentsHeader = new OnlineTipContentsHeader[20];
//	}

	for(i = 0 ;i < m_iContentsNum; i++)
	{
		fgets( szBuffer, 1024, fp );

		pToken												=	strtok( szBuffer, "\t\n" );
		pOnlineSubContentsHeader[i].uiMainSubjectNum		=   atoi( pToken );

		pToken												=	strtok( NULL, "\t\n" );	

		pToken												=   strtok(NULL, "\t\n");
		pOnlineSubContentsHeader[i].uiTotalTipHeader        =   atoi(pToken);

		pToken												=	strtok( NULL, "\t\n" );			
		pOnlineSubContentsHeader[i].uiMainSubjectIndex		=   atoi( pToken );

		pOnlineSubContentsHeader[i].pOnlineTipContentsHeader = new OnlineTipContentsHeader[pOnlineSubContentsHeader[i].uiTotalTipHeader];
	
		for(j = 0 ; j < pOnlineSubContentsHeader[i].uiTotalTipHeader ; j++)
		{
			ZeroMemory(&pOnlineSubContentsHeader[i].pOnlineTipContentsHeader[j],sizeof(OnlineTipContentsHeader));

			pToken	=	strtok( NULL, "\t\n" );
			pOnlineSubContentsHeader[i].pOnlineTipContentsHeader[j].uiSubject = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n" );
			pOnlineSubContentsHeader[i].pOnlineTipContentsHeader[j].uiContents = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n" );
			pOnlineSubContentsHeader[i].pOnlineTipContentsHeader[j].uiVariable = atoi( pToken );

			pToken	=	strtok( NULL, "\t\n" );
			pOnlineSubContentsHeader[i].pOnlineTipContentsHeader[j].uicode = atoi( pToken );
		}
	}
	fclose(fp);

	return TRUE;
}

OnlineTipContentsHeader* OnlineTipParser::GetTipMain(int con)
{
	return &pOnlineMainTipHeader[0].pOnlineTipContentsHeader[con];
}

OnlineTipContentsHeader* OnlineTipParser::GetTipSubject(int sub,int con)
{
	return &pOnlineTipHeader[sub].pOnlineTipContentsHeader[con];
}

OnlineTipContentsHeader* OnlineTipParser::GetTipContents(int sub,int con)
{
	return &pOnlineSubContentsHeader[sub].pOnlineTipContentsHeader[con];
}

//OnlineTipHeader
UI16 OnlineTipParser::GetTipContentsSelect(UI16 code)
{
	int i;

	for(i = 0 ; i < 12 ; i++)
	{
		if(code == pOnlineSubContentsHeader[i].uiMainSubjectNum)
			return i;
	}

	return -1;
	//return NULL;
}

UI16 OnlineTipParser::GetSubjectTotal(UI16 code)
{
	return pOnlineTipHeader[code].uiTotalTipHeader;
}

UI16 OnlineTipParser::GetContentsTotal(UI16 code)
{
	return pOnlineSubContentsHeader[code].uiTotalTipHeader;
}

OnlineTipHeader* OnlineTipParser::GetTipHeader()
{
	return pOnlineMainTipHeader;
}

OnlineTipHeader* OnlineTipParser::GetTipSubjectHeader(int sub)
{
	return &pOnlineTipHeader[sub];
}

OnlineTipHeader* OnlineTipParser::GetTipContentsHeader(int sub)
{
	return &pOnlineSubContentsHeader[sub];
}