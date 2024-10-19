/*************************************************************************************************
	�ۼ��� : ������
	����� : �̻��
	���糯 : 2002.06.21
	��  �� : �ſ� ��� ���� �ļ�
*************************************************************************************************/

#include <GSL.h>

#include "OnlineTradeParser.h"
#include "bindJxFile.h"

void OnlineTradeParser::Init()
{
	FILE *fp;

	fp = BindJXFile::GetInstance()->GetFile( "Online\\CharInfo\\GradeForCredit.txt" );
	if( fp == NULL )	return;

	char szTempBuffer[1024];
	char *pToken = NULL;
	TradeHeader AddTradeHeader;

	fgets(szTempBuffer, 1024, fp); 
	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);
	fgets(szTempBuffer, 1024, fp);   // �����Ұ� 100���� �׳�����

	m_vTradeHeader.reserve(200);
	fgets(szTempBuffer, 1024, fp);
	
	while(fgets(szTempBuffer, 1024, fp))
	{
		pToken = strtok(szTempBuffer, "\t\n\r" );
		AddTradeHeader.uiTradeGrade = atoi(pToken);

		pToken = strtok(NULL, "\t\n\r" );
		AddTradeHeader.uiAccuCredit = atoi(pToken);

		m_vTradeHeader.push_back(AddTradeHeader);
	}

	fclose(fp);
}
