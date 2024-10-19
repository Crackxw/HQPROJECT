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
//	Desc : mapIndex에 해당 하는 지역이 아이템 매장 지역인가?
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
//	Desc : mapIndex에 해당하는 아이템을 목록에서 찾는다, 
//		   목록의 검색 시작 위치는 이전에 목록에서 아이템을 찾은 위치에서 부터 시작한다 
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
//	Desc : 화일에서 정보를 읽어 설정한다 
//-----------------------------------------------------------------------------------------------------------------------
bool CBuriedItemParser::readFile(char* filename)
{
	FILE *fp = BindJXFile::GetInstance()->GetFile( filename );

	if( fp != NULL )
	{		
		char  buffer[1024];
		char *pToken = NULL;

		// 0. 설명부분이 맨앞의 네줄을 띄운다 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);
		fgets(buffer, 1024, fp);		

		// 1. 매장된 아이템 종류의 갯수를 얻어온다 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);		
		pToken        = strtok(buffer, "\t\n\r");
		pToken        = strtok(NULL  , "\t\n\r");
		
		m_numBuriedItem = atoi(pToken);


		// 2. 설명 부분을 한줄 띄운다 
		//----------------------------------------------------------------------------------------------------
		fgets(buffer, 1024, fp);


		// 3. 매장된 아이템의 정보를 읽는다 
		//----------------------------------------------------------------------------------------------------
		for(int i=0; i < m_numBuriedItem; ++i)
		{
			fgets(buffer, 1024, fp);
			
			// 맵의 인덱스 
			pToken			= strtok(buffer, "\t\n\r");			
			m_buriedItem[i].mapIndex = atoi(pToken);

			// 맵의 설명 
			pToken			= strtok(NULL, "\t\n\r");

			// 아이템 코드 
			pToken			= strtok(NULL, "\t\n\r");
			m_buriedItem[i].itemCode = atoi(pToken);

			// 아이템 설명 
			pToken			= strtok(NULL, "\t\n\r");

			// 아이템 발견 확률 
			pToken			= strtok(NULL, "\t\n\r");
			m_buriedItem[i].findProbability = atoi(pToken);
		}
		
		fclose(fp);

		return TRUE;
	}

	return FALSE;
}

