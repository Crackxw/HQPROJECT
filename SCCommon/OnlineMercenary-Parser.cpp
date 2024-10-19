// �� �� �� : �� �� ��
#include <GSL.h>

#include	"OnlineMercenary-Parser.h"
#include	"BindJxFile.h"  


BOOL OnlineMercenaryParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if( fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	MercenaryHeader m;

	while(  fgets(szBuffer, 1024, fp) )
	{
		// Code
		pToken = strtok(szBuffer, "\t\n\r");

		if( pToken == NULL ) break;
		m.m_Code = atoi(pToken);

		// �ִ�� ������ �ִ� �뺴��
		pToken = strtok(NULL, "\t\n\r");
		m.m_MaxFollowerNum = atoi(pToken);

		// �ʿ� �ſ뵵
		pToken = strtok(NULL, "\t\n\r");
		m.m_ReqCredit = atoi(pToken);

		// �ʿ� ����
		pToken = strtok(NULL, "\t\n\r");
		m.m_ReqLv = atoi(pToken);

		_Mercenary.push_back( m );
	}

	fclose(fp);
	return TRUE;
}


SI16 OnlineMercenaryParser::GetMaxFollowerNum(SI32 NowCredit, SI32 NowLv)
{
	SI16		TempMaxFollowerNum = 1;	
	
	for( vector< MercenaryHeader >::iterator i=_Mercenary.begin(); i!=_Mercenary.end(); ++i )	
	{
		if(TempMaxFollowerNum < i->m_MaxFollowerNum)
		{
			if((NowCredit >= i->m_ReqCredit) || (NowLv >= i->m_ReqLv))
			{
				TempMaxFollowerNum = i->m_MaxFollowerNum;
			}
		}
	}
	
	return TempMaxFollowerNum;
}


