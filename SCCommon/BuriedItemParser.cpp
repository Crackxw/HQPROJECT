#include <GSL.h>
#include "BuriedItemParser.h"
#include "bindjxfile.h"

//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CBuriedItemParser::CBuriedItemParser()
{
	memset(m_buriedItem, 0, sizeof(m_buriedItem));

	m_numBuriedItem = 0;
	m_curFindPos	= 0;
}


//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : 
//-----------------------------------------------------------------------------------------------------------------------
CBuriedItemParser::~CBuriedItemParser()
{
}


//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : mapIndex�� �ش� �ϴ� ������ ������ ���� �����ΰ�?
//-----------------------------------------------------------------------------------------------------------------------
bool  CBuriedItemParser::isBuriedArea(DWORD mapIndex)
{
	for(int i=0; i < m_numBuriedItem; ++i)
	{
		if( m_buriedItem[i].mapIndex == mapIndex ) return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : mapIndex�� �ش��ϴ� �������� ��Ͽ��� ã�´�, 
//		   ����� �˻� ���� ��ġ�� ������ ��Ͽ��� �������� ã�� ��ġ���� ���� �����Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
BURIED::ITEMINFO* CBuriedItemParser::findItem(DWORD mapIndex)
{

	for(int i = m_curFindPos; i < m_numBuriedItem; ++i)
	{
		++m_curFindPos;

		if( m_buriedItem[i].mapIndex == mapIndex ) return &m_buriedItem[i];
	}

	m_curFindPos = 0;
	return NULL;
}


//-----------------------------------------------------------------------------------------------------------------------
//	Name : 
//	Desc : ȭ�Ͽ��� ������ �о� �����Ѵ� 
//-----------------------------------------------------------------------------------------------------------------------
bool CBuriedItemParser::readFile(char* filename)
{
	FILE *fp = BindJXFile::GetInstance()->GetFile( filename );

	if( fp != NULL )
	{		
		char  buffer[1024];
		char *pToken = NULL;

		// 0. ����κ��� �Ǿ��� ������ ���� 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);		

		// 1. ����� ������ ������ ������ ���´� 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);		
		pToken        = strtok(buffer, "\t\n\r");
		pToken        = strtok(NULL  , "\t\n\r");
		
		m_numBuriedItem = atoi(pToken);


		// 2. ���� �κ��� ���� ���� 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);


		// 3. ����� �������� ������ �д´� 
		//----------------------------------------------------------------------------------------------------
		for(int i=0; i < m_numBuriedItem; ++i)
		{
			fgets(buffer, 1024, fp);
			
			// ���� �ε��� 
			pToken			= strtok(buffer, "\t\n\r");			
			m_buriedItem[i].mapIndex = atoi(pToken);

			// ���� ���� 
			pToken			= strtok(NULL, "\t\n\r");

			// ������ �ڵ� 
			pToken			= strtok(NULL, "\t\n\r");
			m_buriedItem[i].itemCode = atoi(pToken);

			// ������ ���� 
			pToken			= strtok(NULL, "\t\n\r");

			// ������ �߰� Ȯ�� 
			pToken			= strtok(NULL, "\t\n\r");
			m_buriedItem[i].findProbability = atoi(pToken);
		}
		
		fclose(fp);

		return TRUE;
	}

	return FALSE;
}

