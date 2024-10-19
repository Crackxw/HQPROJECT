
#include <GSL.h>

#include	"Directives.h"
#include	"OnlineEconomy.h"
#include	"bindJxFile.h"
 
OnlineEconomy::OnlineEconomy()
{
	pEconomyHeader	=	NULL;
	siTotalNum		=	0;
	siAccount		=	0;
	siInstallment	=	0;
	siLoan			=	0;
}


OnlineEconomy::~OnlineEconomy()
{
	Free();
}

VOID	OnlineEconomy::Init( CHAR *pFileName )
{
	FILE	*fp;
	CHAR	szBuffer[1024];
	CHAR	*pToken = NULL;
	CHAR	seps[] = "'\t\n\r";	
	SI16	i;


	fp = BindJXFile::GetInstance()->GetFile( pFileName );

	if( fp == NULL )		return;

	ZeroMemory( szBuffer, 1024 );

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );

	

	// ÃÑ °¹¼ö¸¦ ±¸ÇÑ´Ù.
	while( fgets( szBuffer, 1024, fp ) != NULL )	
		siTotalNum++;

	pEconomyHeader	=	new EconomyHeader[ siTotalNum ];

	ZeroMemory( pEconomyHeader, sizeof( EconomyHeader ) * siTotalNum );


	fseek( fp, SEEK_SET, 0 );

	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );
	fgets( szBuffer, 1024, fp );


	for( i=0 ; i<siTotalNum ; i++ )	
	{
		fgets( szBuffer, 1024, fp );

		pToken		= strtok( szBuffer, seps );
		pEconomyHeader[ i ].siCode				= atoi( pToken );



		pToken		= strtok( NULL, seps );
		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siNameCode			= atoi( pToken );


		
		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siType				= atoi( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].bMultiAccount		= atoi( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siMinMoney			= _atoi64( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].uiMaxMoney			= _atoi64( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siUnitMoney		= _atoi64( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].fFixedInterest	= (float)atof( pToken );


		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].fFlexInterest  = (float)atof( pToken );



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siFixedPeriod		= atoi( pToken );
		


		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siUnitPeriod		= atoi( pToken );				


		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siMinPeriod		= atoi( pToken );		



		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siMaxPeriod		= atoi( pToken );


		pToken		= strtok( NULL, seps );
		pEconomyHeader[ i ].siDescription	= atoi( pToken );
	}

	fclose( fp );



	for( i=0 ; i<siTotalNum ; i++ )
	{
		switch( pEconomyHeader[i].siType )
		{
		case 1:			siAccount++;		break;
		case 2:			siInstallment++;	break;
		case 3:			siLoan++;			break;
		}
	}
}

EconomyHeader	*OnlineEconomy::GetHeader( SI32 siIndex )
{
	return &pEconomyHeader[ siIndex ];
}

EconomyHeader	*OnlineEconomy::GetHeaderCode( SI16 siCode )
{
	SI32		TempCount;

	for(TempCount = 0; TempCount < siTotalNum; TempCount++)
	{
		if(pEconomyHeader[TempCount].siCode == siCode)
		{
			return &pEconomyHeader[TempCount];
		}
	}
	
	return NULL;
}

SI32	OnlineEconomy::GetTotalNum()
{
	return siTotalNum;
}


VOID	OnlineEconomy::Free()
{
	if( pEconomyHeader )
	{
		delete [] pEconomyHeader;
		pEconomyHeader	=	NULL;
	}
}


SI32	OnlineEconomy::GetAccount()
{
	return siAccount;
}


SI32	OnlineEconomy::GetInstallment()
{
	return siInstallment;
}


SI32	OnlineEconomy::GetLoan()
{
	return siLoan;
}