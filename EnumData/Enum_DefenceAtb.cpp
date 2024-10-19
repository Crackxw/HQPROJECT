//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - DefenceAtb ������
//	File Name		: Enum_DefenceAtb.cpp
//	Birth Date		: 2004. 03. 29.	
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		�Ż󿡼� ���Ǵ� ������ �������̸�, �� ������ ���α׷� �����Ͽ� ÷�ε˴ϴ�.
//		������ �߰��� ������ Ư���� ��츦 �����ϰ� ���κа� MAX�� ���̿� �߰��Ͻñ� �ٶ��ϴ�.				
//						
//===================================================================================================						

#include <GSL.h>

#include <string.h>
#include "Enum_DefenceAtb.h"

CLASS_DEFENCEATB::DATA		CLASS_DEFENCEATB::m_Data[DEFENCEATB_MAX]	= {0,};	// ����ƽ ���� �ʱ�ȭ
CLASS_DEFENCEATB			g_clDefenceAtb;									// �ʱ�ȭ ������ �ӽ� ��������

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: �ش� ���� �ε����� �ű⿡ �Ҵ�� ���� �̸��� ��ġ��Ų��.
//		index		- ������
//		pszStr		- �ش� �������� �̸�
//-----------------------------------------------------------------------------
void	CLASS_DEFENCEATB::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= DEFENCEATB_FIRST || index >= DEFENCEATB_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: �ش� ������ ���� �������� �޴´�.
//		pszStr		- ã�� �������� �̸�
//		return		- �ش� ������ �Ҵ�� ������. ������ ������ �Ѱ�ٸ� DEFENCEATB_INVALID�� ����
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
// Desc: �ش� ������ ���� �������� �޴´�.
//		enIndex		- ã�� ������
//		return		- �ش� �������� ������. ������ NULL�� ����. ����� �ȵƴٸ� ��Ʈ���� ""
//-----------------------------------------------------------------------------
const char* CLASS_DEFENCEATB::GetData( ENUM_DEFENCEATB enIndex )
{
	if( enIndex <= DEFENCEATB_FIRST || enIndex >= DEFENCEATB_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_DEFENCEATB()
// Code: actdoll (2004-03-18)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
CLASS_DEFENCEATB::~CLASS_DEFENCEATB()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_DEFENCEATB()
// Code: actdoll (2004-03-18)
// Desc: ������(�����߿�!!!)
//-----------------------------------------------------------------------------
CLASS_DEFENCEATB::CLASS_DEFENCEATB()
{
	memset( m_Data, 0, (sizeof(DATA) * DEFENCEATB_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// ������ �߰��� ��� �� �κ� �� �Ʒ� (//EOF����)�� �߰���Ų��.
	
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
