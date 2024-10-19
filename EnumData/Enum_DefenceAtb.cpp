//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: 임진록 온라인 [거상] - DefenceAtb 수열값
//	File Name		: Enum_DefenceAtb.cpp
//	Birth Date		: 2004. 03. 29.	
//	Creator			: 조 현 준
//	Editer			: 조 현 준
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		거상에서 사용되는 수열값 데이터이며, 이 내용은 프로그램 컴파일에 첨부됩니다.
//		수열을 추가할 때에는 특별한 경우를 제외하고 끝부분과 MAX값 사이에 추가하시기 바랍니다.				
//						
//===================================================================================================						

#include <GSL.h>

#include <string.h>
#include "Enum_DefenceAtb.h"

CLASS_DEFENCEATB::DATA		CLASS_DEFENCEATB::m_Data[DEFENCEATB_MAX]	= {0,};	// 스태틱 변수 초기화
CLASS_DEFENCEATB			g_clDefenceAtb;									// 초기화 가동용 임시 전역변수

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: 해당 수열 인덱스와 거기에 할당될 실제 이름을 배치시킨다.
//		index		- 수열값
//		pszStr		- 해당 수열값의 이름
//-----------------------------------------------------------------------------
void	CLASS_DEFENCEATB::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= DEFENCEATB_FIRST || index >= DEFENCEATB_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		pszStr		- 찾을 수열값의 이름
//		return		- 해당 수열명에 할당된 수열값. 범위가 수열을 넘겼다면 DEFENCEATB_INVALID를 리턴
//-----------------------------------------------------------------------------
ENUM_DEFENCEATB	CLASS_DEFENCEATB::GetData( char *pszStr )
{
	if( !pszStr )		return DEFENCEATB_INVALID;
	for( register int i=DEFENCEATB_FIRST; i<DEFENCEATB_MAX; i++ )	
	{
		if( strcmp( m_Data[i].m_pszString, pszStr ) == 0 ) return (ENUM_DEFENCEATB)i;
	}
	return DEFENCEATB_INVALID;
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: 해당 수열명에 배당된 수열값을 받는다.
//		enIndex		- 찾을 수열값
//		return		- 해당 수열값의 수열명. 없으면 NULL을 리턴. 배당이 안됐다면 스트링은 ""
//-----------------------------------------------------------------------------
const char* CLASS_DEFENCEATB::GetData( ENUM_DEFENCEATB enIndex )
{
	if( enIndex <= DEFENCEATB_FIRST || enIndex >= DEFENCEATB_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_DEFENCEATB()
// Code: actdoll (2004-03-18)
// Desc: 소멸자
//-----------------------------------------------------------------------------
CLASS_DEFENCEATB::~CLASS_DEFENCEATB()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_DEFENCEATB()
// Code: actdoll (2004-03-18)
// Desc: 생성자(아주중요!!!)
//-----------------------------------------------------------------------------
CLASS_DEFENCEATB::CLASS_DEFENCEATB()
{
	memset( m_Data, 0, (sizeof(DATA) * DEFENCEATB_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// 수열이 추가될 경우 이 부분 맨 아래 (//EOF앞쪽)에 추가시킨다.
	
	SET_PROTOCOL( DEFENCEATB_HUMAN				)	//1		
	SET_PROTOCOL( DEFENCEATB_ARMOUR    			)	//2		
	SET_PROTOCOL( DEFENCEATB_HORSE    			)	//3		
	SET_PROTOCOL( DEFENCEATB_AIRPLANE   		)	//4		
	SET_PROTOCOL( DEFENCEATB_OILARMOUR  		)	//5
	SET_PROTOCOL( DEFENCEATB_FLOAT		 		)	//6
	SET_PROTOCOL( DEFENCEATB_ANIMAL    			)	//7		
	SET_PROTOCOL( DEFENCEATB_GHOST	  			)	//8		
	SET_PROTOCOL( DEFENCEATB_SOFT		  		)	//9		
		
// EOF
#undef	SET_PROTOCOL		
}
