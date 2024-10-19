//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - AttackAtb ������
//	File Name		: Enum_AttackAtb.cpp
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
#include "Enum_AttackAtb.h"

CLASS_ATTACKATB::DATA	CLASS_ATTACKATB::m_Data[ATTACKATB_MAX]	= {0,};	// ����ƽ ���� �ʱ�ȭ
CLASS_ATTACKATB			g_clAttackAtb;									// �ʱ�ȭ ������ �ӽ� ��������

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: �ش� ���� �ε����� �ű⿡ �Ҵ�� ���� �̸��� ��ġ��Ų��.
//		index		- ������
//		pszStr		- �ش� �������� �̸�
//-----------------------------------------------------------------------------
void	CLASS_ATTACKATB::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= ATTACKATB_FIRST || index >= ATTACKATB_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: �ش� ������ ���� �������� �޴´�.
//		pszStr		- ã�� �������� �̸�
//		return		- �ش� ������ �Ҵ�� ������. ������ ������ �Ѱ�ٸ� ATTACKATB_INVALID�� ����
//-----------------------------------------------------------------------------
ENUM_ATTACKATB	CLASS_ATTACKATB::GetData( char *pszStr )
{
	if( !pszStr )		return ATTACKATB_INVALID;
	for( register int i=ATTACKATB_FIRST; i<ATTACKATB_MAX; i++ )	
	{
		if( strcmp( m_Data[i].m_pszString, pszStr ) == 0 ) return (ENUM_ATTACKATB)i;
	}
	return ATTACKATB_INVALID;
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: �ش� ������ ���� �������� �޴´�.
//		enIndex		- ã�� ������
//		return		- �ش� �������� ������. ������ NULL�� ����. ����� �ȵƴٸ� ��Ʈ���� ""
//-----------------------------------------------------------------------------
const char* CLASS_ATTACKATB::GetData( ENUM_ATTACKATB enIndex )
{
	if( enIndex <= ATTACKATB_FIRST || enIndex >= ATTACKATB_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_ATTACKATB()
// Code: actdoll (2004-03-18)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
CLASS_ATTACKATB::~CLASS_ATTACKATB()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_ATTACKATB()
// Code: actdoll (2004-03-18)
// Desc: ������(�����߿�!!!)
//-----------------------------------------------------------------------------
CLASS_ATTACKATB::CLASS_ATTACKATB()
{
	memset( m_Data, 0, (sizeof(DATA) * ATTACKATB_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// ������ �߰��� ��� �� �κ� �� �Ʒ� (//EOF����)�� �߰���Ų��.
	
	SET_PROTOCOL( ATTACKATB_SWORD					)	//1		
	SET_PROTOCOL( ATTACKATB_STRONGGUN				)	//2		
	SET_PROTOCOL( ATTACKATB_AIRMISSILE				)	//3		
	SET_PROTOCOL( ATTACKATB_SAMMISSILE				)	//4		
	SET_PROTOCOL( ATTACKATB_CANNON2C				)	//5		�߼��� 
	SET_PROTOCOL( ATTACKATB_NEEDLE					)	//6		ħ
	SET_PROTOCOL( ATTACKATB_CANNON					)	//7		
	SET_PROTOCOL( ATTACKATB_GUN						)	//8		
	SET_PROTOCOL( ATTACKATB_MAGIC					)	//9		
	SET_PROTOCOL( ATTACKATB_MAGIC2					)	//10	���� �º��� ���ݿ� ���� 
	SET_PROTOCOL( ATTACKATB_MAGIC3					)	//11	�Ϻ� ������ ���ݿ� ���� 
	
	SET_PROTOCOL( ATTACKATB_FIRECAR					)	//13
	SET_PROTOCOL( ATTACKATB_SHIPCANNON				)	//14
	SET_PROTOCOL( ATTACKATB_FIRE					)	//15
	SET_PROTOCOL( ATTACKATB_HEAVYGUN				)	//16
	SET_PROTOCOL( ATTACKATB_FIREARROW				)	//17	��ȭ�� 
	SET_PROTOCOL( ATTACKATB_ARROW					)	//18
	SET_PROTOCOL( ATTACKATB_SELFEXP					)	//19
	SET_PROTOCOL( ATTACKATB_LIGHTNING				)	//20	������
	SET_PROTOCOL( ATTACKATB_EARTHQUAKE				)	//21	������
	SET_PROTOCOL( ATTACKATB_BIGSWORD				)	//22	ū��
	SET_PROTOCOL( ATTACKATB_ELECTRON				)	//23
	SET_PROTOCOL( ATTACKATB_THROWBOMB				)	//24	������ ��ź 
	SET_PROTOCOL( ATTACKATB_MAGICBALL				)	//25	������ ���� ���� 
	
	SET_PROTOCOL( ATTACKATB_BOMB					)	//27
	SET_PROTOCOL( ATTACKATB_TORNADO					)	//28	ǳ���
	SET_PROTOCOL( ATTACKATB_KIMBOMB           		)	//29	���ڰ�
	SET_PROTOCOL( ATTACKATB_MAGIC5J           		)	//30	������ ����������
	SET_PROTOCOL( ATTACKATB_DART					)	//31	������ ������
	SET_PROTOCOL( ATTACKATB_MAGIC8K					)	//32	��ù��� ���� ������ 
	SET_PROTOCOL( ATTACKATB_CANNONJ					)	//33	�ļ��� ����. 
	SET_PROTOCOL( ATTACKATB_FIRETANK				)	//34
	SET_PROTOCOL( ATTACKATB_COLLTANK				)	//35
	SET_PROTOCOL( ATTACKATB_CANNON2J				)	//36	�߼��� 
	SET_PROTOCOL( ATTACKATB_POISONARROW				)	//37	��ħ 
	SET_PROTOCOL( ATTACKATB_FIRETANKARROW			)	//38	���� ����� ���� 
	SET_PROTOCOL( ATTACKATB_COLLTANKJARROW			)	//39	���� ������ ���� 
	SET_PROTOCOL( ATTACKATB_GENERAL4KFIRE			)	//40	������ ���� 
	SET_PROTOCOL( ATTACKATB_GENERAL4J				)	//41	������ ���� 
	SET_PROTOCOL( ATTACKATB_GENERAL3C				)	//42	��ȿ�� ���� 
	SET_PROTOCOL( ATTACKATB_CANNONKARROW			)	//43	�������� ���� 
	SET_PROTOCOL( ATTACKATB_FLAMETANKJARROW			)	//44
	SET_PROTOCOL( ATTACKATB_CANNONC					)	//45	�Ŷ� ��ȣ���� ���� 
	SET_PROTOCOL( ATTACKATB_AXE						)	//46	���� ����
	SET_PROTOCOL( ATTACKATB_SWORDENERGY				)	//47	������ �ߵ��˱� �Ӽ�
	SET_PROTOCOL( ATTACKATB_ELEPHANT				)	//48	�ڳ��� (��2)
	SET_PROTOCOL( ATTACKATB_ANTIARMOUR				)	//49	���ī�� (��2)
	SET_PROTOCOL( ATTACKATB_CHASEFIRE				)	//50	������ ȭ���� �Ӽ� (õ��)
	SET_PROTOCOL( ATTACKATB_AIRATTACKUPARROW		)	//51	���� ����
	SET_PROTOCOL( ATTACKATB_TOWERCANNON				)	//52	����� ȭ��
	SET_PROTOCOL( ATTACKATB_UPGRADETOWERCANNON		)	//53	���׷��̵� ����� ȭǥ
	SET_PROTOCOL( ATTACKATB_GATPHAPOISON			)	//54	�¶��� - ������ ���� ������
	SET_PROTOCOL( ATTACKATB_DUEOKSINI				)	//55	�ξ�ô� ���� �Ӽ� - ������ �ڷ� �и�. 
	SET_PROTOCOL( ATTACKATB_TOAD					)	//56	���β���
	
	SET_PROTOCOL( ATTACKATB_FURHEADDEMON			)	//60	�а���. 
	SET_PROTOCOL( ATTACKATB_SNOWWOMAN				)	//61	����. 
	SET_PROTOCOL( ATTACKATB_GOLDDRAGONELECTRON		)	//62	�ݷ� ����. 
	SET_PROTOCOL( ATTACKATB_ONGHWA					)	//63	��õ ������ �ذ�.  
	SET_PROTOCOL( ATTACKATB_GHOSTCAT				)	//64	�͹� ���� ����. 
	SET_PROTOCOL( ATTACKATB_DOLL1					)	//65	���� ���� ����. 
	SET_PROTOCOL( ATTACKATB_DOLL2					)	//66	��ī�� ���� ����. 
	SET_PROTOCOL( ATTACKATB_JUSTICEK				)	//67	���� ���� ����
	SET_PROTOCOL( ATTACKATB_BUTTERFLY				)	//68	������ ���� 
	SET_PROTOCOL( ATTACKATB_WIDOW					)	//69	�Ź̿䱫 ���� 
	SET_PROTOCOL( ATTACKATB_WAVE					)	//70	�ٴٰź��� �ĵ�����. 
	SET_PROTOCOL( ATTACKATB_FIREUSINGMANA			)	//71	�Ұ���(���� ���)
	SET_PROTOCOL( ATTACKATB_LEECH					)	//72	�հŸӸ�. 
	SET_PROTOCOL( ATTACKATB_SEAHORSE				)	//73	�ظ������. 
	SET_PROTOCOL( ATTACKATB_MACHINEGUN				)	//74	���� ��� 
	SET_PROTOCOL( ATTACKATB_THROWSPEAR				)	//75	������â (���ֹ�)  
	SET_PROTOCOL( ATTACKATB_SCORPION				)	//76	������ ��. 
	SET_PROTOCOL( ATTACKATB_CACTUS					)	//77	���������� ����. 
	SET_PROTOCOL( ATTACKATB_CLAW					)	//78	������ ���ݼӼ�. 
	SET_PROTOCOL( ATTACKATB_MAGICIAN				)	//79	����� ��������.  
	SET_PROTOCOL( ATTACKATB_BIGTREE					)	//80	�Ŵ�� �������� ����. 
	SET_PROTOCOL( ATTACKATB_COBRA					)	//81	�ں�� �� ����.
	SET_PROTOCOL( ATTACKATB_VOLCANOFLAME			)	//82	ȭ���ǺҲ�.   �ҵ���.
	SET_PROTOCOL( ATTACKATB_EVIL					)	//83	�̻��� ���.
	SET_PROTOCOL( ATTACKATB_ICE						)	//84	�̻��� ��� Ư�����.
	SET_PROTOCOL( ATTACKATB_MAGIC4T					)	//85	¡Ű��ĭȥ Ư�� ���.
	SET_PROTOCOL( ATTACKATB_JAPANGHOST				)	//86	�Ϻ��ͽ� Ư�� ���.
	SET_PROTOCOL( ATTACKATB_KUKET					)	//87	����
	SET_PROTOCOL( ATTACKATB_AUTA					)	//88	�ƿ�Ÿ ����.
	SET_PROTOCOL( ATTACKATB_SUB1					)	//89	�������� ȭ����� 1
	SET_PROTOCOL( ATTACKATB_SUB2					)	//90	�������� ȭ����� 2
	SET_PROTOCOL( ATTACKATB_FIREATTACK				)	//91	ȭ������.
	SET_PROTOCOL( ATTACKATB_RUNNINGFIRE				)	//92	����
	SET_PROTOCOL( ATTACKATB_ENERGYPA				)	//93	��ǳ.
	SET_PROTOCOL( ATTACKATB_LONG_SPEAR				)	//94	����â ��â��ȯ.
	SET_PROTOCOL( ATTACKATB_PAPERDOLL				)	//95	����� ��������
	SET_PROTOCOL( ATTACKATB_NEEDLEATTACK			)	//96	ħ����
	SET_PROTOCOL( ATTACKATB_CLOUDWIND				)	//97	ǳ���
	SET_PROTOCOL( ATTACKATB_STONEMAGIC				)	//98	������
	SET_PROTOCOL( ATTACKATB_GODSPEAR				)	//99	��â�뵵
	SET_PROTOCOL( ATTACKATB_FLOWERRAIN				)	//100	��õȭ��
	SET_PROTOCOL( ATTACKATB_SOULCHANGE				)	//101	���Ǽ�
	SET_PROTOCOL( ATTACKATB_MAGICCATTACK			)	//102	������ ����
	SET_PROTOCOL( ATTACKATB_SUNBIYOUN				)	//103	���� ����
	SET_PROTOCOL( ATTACKATB_SNOW					)	//104	������

	// actdoll (2004/03/06 5:00) : 3�� ��ġ���� �߰�
	SET_PROTOCOL( ATTACKATB_PYTHON					)	//105	�̹��� ��������	// actdoll (2004/03/06 4:08) : �߰�
	SET_PROTOCOL( ATTACKATB_JINEMPEROR				)	//106	����Ȳ��ȥ ��������	// actdoll (2004/03/06 4:08) : �߰�
	SET_PROTOCOL( ATTACKATB_GUARDIAN_SNAKE			)	//107	��ȣ��(��)
	SET_PROTOCOL( ATTACKATB_GUARDIAN_PIG			)	//108	��ȣ��(����)
	SET_PROTOCOL( ATTACKATB_GUARDIAN_CHICKEN		)	//109	��ȣ��(��)

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:48) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	SET_PROTOCOL( ATTACKATB_TURTLE_VEHICLE			)	// 110	����-�ź���, �̼��� 2�� ����
	SET_PROTOCOL( ATTACKATB_THUNDER_VEHICLE			)	// 111	����-������, ���� 2�� ����
	SET_PROTOCOL( ATTACKATB_QUAKE_VEHICLE			)	// 112	�Ϻ�-������, ��Ű��ī 2�� ����
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_VEHICLE	)	// 113	�Ϻ�-�����, ���̸��� 2�� ����
	SET_PROTOCOL( ATTACKATB_CATAPULT				)	// 114	�߱�-�߼���, ����<=�̷� 2�� ����
	SET_PROTOCOL( ATTACKATB_FARANGI_VEHICLE			)	// 115	�߱�-�Ҷ�������, �̷�<=���� 2�� ����
	SET_PROTOCOL( ATTACKATB_FIRE_DRAGON_VEHICLE		)	// 116	�븸-ȭ����, ����â 2�� ����
	SET_PROTOCOL( ATTACKATB_CHINESEPHOENIX			)	// 117	�븸-��Ȳ����, �弱ȭ 2�� ����

	SET_PROTOCOL( ATTACKATB_CATDOLL					)	// 118  ����� ����.
	SET_PROTOCOL( ATTACKATB_OLDFOXDOLL				)	// 119  ����ȣ ����.
	SET_PROTOCOL( ATTACKATB_RACCOONDOLL				)   // 120	�ʱ��� ����.
	SET_PROTOCOL( ATTACKATB_NINJADOLL				)   // 121  ���� ����.
	SET_PROTOCOL( ATTACKATB_SNOWWOMANDOOL			)   // 122  ���� ����.
	SET_PROTOCOL( ATTACKATB_YANGGUIBIDOLL			)	// 123  ��ͺ� ����.
	SET_PROTOCOL( ATTACKATB_JIJANGDOLL				)	// 124  ���庸�� ����.
	SET_PROTOCOL( ATTACKATB_SNAKEWOMANDOLL			)	// 125  ��� ����.

	// robypark (2004/6/4 13:6)
	// ��õ��� ����
	SET_PROTOCOL( ATTACKATB_ROAR_OF_LAUGHTER		)	// 126	�Ҷ������� ��õ��� ����

	// robypark 2004/6/10 19:4
	// ����� õ������, �ñ����
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_ICE		)	// 127

	// robypark 2004/6/11 10:49
	// ����� õ������, ������
	SET_PROTOCOL( ATTACKATB_BLACK_DRAGON_POISON		)	// 128

	// robypark 2004/7/14 16:41
	// ���� ���� ����
	SET_PROTOCOL( ATTACKATB_DOLL_EVIS				)	// 129

	// robypark 2005/2/2 17:27
	// �޸� ���� ����
	SET_PROTOCOL( ATTACKATB_DOLL_DHARMA				)	// 130

// EOF
#undef	SET_PROTOCOL		
}
