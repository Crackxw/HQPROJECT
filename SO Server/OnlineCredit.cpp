#include	<windows.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"Directives.h"
#include	"OnlineCredit.h"


OnlineCredit::OnlineCredit()
{
	Num	=	0;
	pCreditIndex = NULL;
}

OnlineCredit::~OnlineCredit()
{
	if(pCreditIndex)
	{
		delete [] pCreditIndex;
		pCreditIndex	=	NULL;
	}
}


VOID	OnlineCredit::Init(CHAR *FileName)
{	
	FILE	*fp;
	CHAR	Buf[512];
	CHAR	*pToken = NULL;
	CHAR	seps[] = "\" '\t\n";
	SI32	Temp=0;
	SI32	i=0;

	fp	=	fopen( FileName, "rt" );
	if( fp == NULL )
	{
		printf("OnlineCredit::Init() -- Cannot Find \"%s\" File....\n",FileName);
		getchar();
		return;
	}
	
	// 앞부분 제외하고 Code만 읽는다.
	while( fgets(Buf, sizeof(Buf), fp) )
	{
		pToken	=	strtok(Buf, seps);
		if(pToken == NULL)
			continue;
		if( stricmp(pToken, "Number") == 0 )
		{
			Temp = ftell(fp);
			while( fgets(Buf, sizeof(Buf), fp) )
			{
				pToken = strtok(Buf, seps);
				if(pToken)
					Num++;
			}
			break;
		}
	}
	
	fseek(fp, Temp, SEEK_SET );
	
	pCreditIndex		=	new CreditIndex[Num];

	//	이제부터 내용이다.
	while( fgets(Buf, sizeof(Buf), fp) )
	{
		//printf("%s\n",Buf);
		pToken = strtok(Buf, seps);
		if(pToken == NULL)
			break;
		pCreditIndex[i].Serial		=	atoi(pToken);
		pToken = strtok(NULL, seps);
		pCreditIndex[i].MinCredit	=	atoi(pToken);
		pToken = strtok(NULL, seps);
		pCreditIndex[i].AddCredit	=	(float)atof(pToken);
		pToken = strtok(NULL, seps);
		pCreditIndex[i].NeedMoney	=	atoi(pToken);

		//printf("%d %d %5f %d",pCreditIndex[i].Serial,pCreditIndex[i].MinCredit,pCreditIndex[i].AddCredit,pCreditIndex[i].NeedMoney );
		i++;
	}
	return;
}