//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - AttackAtb ������
//	File Name		: Enum_AttackAtb.h	
//	Birth Date		: 2004. 03. 29.	
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		�Ż󿡼� ���Ǵ� ������ �������̸�, �� ������ ���α׷� �����Ͽ� ÷�ε˴ϴ�.
//		������ �߰��� ������ Ư���� ��츦 �����ϰ� ���κа� MAX�� ���̿� �߰��Ͻñ� �ٶ��ϴ�.				
//						
//===================================================================================================						

#ifndef	__ENUM_ATTACKATB_H__
#define	__ENUM_ATTACKATB_H__

//-----------------------------------------------------------------------------
// Name: ENUM_ATTACKATB
// Code: actdoll (2004-01-01)
// Desc: 
//		������ �����ϴ� ���ڼ��� �������� 47�ڸ� �Ѿ�� �ȵȴ�!!!!
//		�̰��� ������ �߰��� ��� Enum_AttackAtb.cpp�� ������ �κп���
//		SET_PROTOCOL�� �̿��� �ش� �������� �ݵ�� ����ؾ� �Ѵ�. ��������!!!
//-----------------------------------------------------------------------------
typedef enum	ENUM_ATTACKATB		
{						
	ATTACKATB_INVALID				= -1	,	//		�������̴�.(�� ���� �׾ ����Һ�!!!)
	ATTACKATB_FIRST					= 0		,	//		�Ⱦ��� ��. �������� ���� ���� ���̴�.
	ATTACKATB_NONE					= 0		,	//		���ð�. �� ���� �������� ���� �۵��� ���ϰԲ� �Ѵ�.

	ATTACKATB_SWORD					= 1		,	//1		
	ATTACKATB_STRONGGUN						,	//2		
	ATTACKATB_AIRMISSILE					,	//3
	ATTACKATB_SAMMISSILE					,	//4
	ATTACKATB_CANNON2C						,	//5		�߼��� 
	ATTACKATB_NEEDLE						,	//6		ħ
	ATTACKATB_CANNON						,	//7
	ATTACKATB_GUN							,	//8
	ATTACKATB_MAGIC							,	//9
	ATTACKATB_MAGIC2						,	//10	���� �º��� ���ݿ� ���� 
	ATTACKATB_MAGIC3						,	//11	�Ϻ� ������ ���ݿ� ���� 

	ATTACKATB_FIRECAR				= 13	,	//13
	ATTACKATB_SHIPCANNON					,	//14
	ATTACKATB_FIRE							,	//15
	ATTACKATB_HEAVYGUN						,	//16
	ATTACKATB_FIREARROW						,	//17	��ȭ�� 
	ATTACKATB_ARROW							,	//18
	ATTACKATB_SELFEXP						,	//19
	ATTACKATB_LIGHTNING						,	//20	������
	ATTACKATB_EARTHQUAKE					,	//21	������
	ATTACKATB_BIGSWORD						,	//22	ū��
	ATTACKATB_ELECTRON						,	//23
	ATTACKATB_THROWBOMB						,	//24	������ ��ź 
	ATTACKATB_MAGICBALL						,	//25	������ ���� ���� 

	ATTACKATB_BOMB					= 27	,	//27
	ATTACKATB_TORNADO						,	//28	ǳ���
	ATTACKATB_KIMBOMB           			,	//29	���ڰ�
	ATTACKATB_MAGIC5J           			,	//30	������ ����������
	ATTACKATB_DART							,	//31	������ ������
	ATTACKATB_MAGIC8K						,	//32	��ù��� ���� ������ 
	ATTACKATB_CANNONJ						,	//33	�ļ��� ����. 
	ATTACKATB_FIRETANK						,	//34
	ATTACKATB_COLLTANK						,	//35
	ATTACKATB_CANNON2J						,	//36	�߼��� 
	ATTACKATB_POISONARROW					,	//37	��ħ 
	ATTACKATB_FIRETANKARROW					,	//38	���� ����� ���� 
	ATTACKATB_COLLTANKJARROW				,	//39	���� ������ ���� 
	ATTACKATB_GENERAL4KFIRE					,	//40	������ ���� 
	ATTACKATB_GENERAL4J						,	//41	������ ���� 
	ATTACKATB_GENERAL3C						,	//42	��ȿ�� ���� 
	ATTACKATB_CANNONKARROW					,	//43	�������� ���� 
	ATTACKATB_FLAMETANKJARROW				,	//44
	ATTACKATB_CANNONC						,	//45	�Ŷ� ��ȣ���� ���� 
	ATTACKATB_AXE							,	//46	���� ����
	ATTACKATB_SWORDENERGY					,	//47	������ �ߵ��˱� �Ӽ�
	ATTACKATB_ELEPHANT						,	//48	�ڳ��� (��2)
	ATTACKATB_ANTIARMOUR					,	//49	���ī�� (��2)
	ATTACKATB_CHASEFIRE						,	//50	������ ȭ���� �Ӽ� (õ��)
	ATTACKATB_AIRATTACKUPARROW				,	//51	���� ����
	ATTACKATB_TOWERCANNON					,	//52	����� ȭ��
	ATTACKATB_UPGRADETOWERCANNON			,	//53	���׷��̵� ����� ȭǥ
	ATTACKATB_GATPHAPOISON					,	//54	�¶��� - ������ ���� ������
	ATTACKATB_DUEOKSINI						,	//55	�ξ�ô� ���� �Ӽ� - ������ �ڷ� �и�. 
	ATTACKATB_TOAD							,	//56	���β���

	ATTACKATB_FURHEADDEMON			= 60	,	//60	�а���. 
	ATTACKATB_SNOWWOMAN						,	//61	����. 
	ATTACKATB_GOLDDRAGONELECTRON			,	//62	�ݷ� ����. 
	ATTACKATB_ONGHWA						,	//63	��õ ������ �ذ�.  
	ATTACKATB_GHOSTCAT						,	//64	�͹� ���� ����. 
	ATTACKATB_DOLL1							,	//65	���� ���� ����. 
	ATTACKATB_DOLL2							,	//66	��ī�� ���� ����. 
	ATTACKATB_JUSTICEK						,	//67	���� ���� ����
	ATTACKATB_BUTTERFLY						,	//68	������ ���� 
	ATTACKATB_WIDOW							,	//69	�Ź̿䱫 ���� 
	ATTACKATB_WAVE							,	//70	�ٴٰź��� �ĵ�����. 
	ATTACKATB_FIREUSINGMANA					,	//71	�Ұ���(���� ���)
	ATTACKATB_LEECH							,	//72	�հŸӸ�. 
	ATTACKATB_SEAHORSE						,	//73	�ظ������. 
	ATTACKATB_MACHINEGUN					,	//74	���� ��� 
	ATTACKATB_THROWSPEAR					,	//75	������â (���ֹ�)  
	ATTACKATB_SCORPION						,	//76	������ ��. 
	ATTACKATB_CACTUS						,	//77	���������� ����. 
	ATTACKATB_CLAW							,	//78	������ ���ݼӼ�. 
	ATTACKATB_MAGICIAN						,	//79	����� ��������.  
	ATTACKATB_BIGTREE						,	//80	�Ŵ�� �������� ����. 
	ATTACKATB_COBRA							,	//81	�ں�� �� ����.
	ATTACKATB_VOLCANOFLAME					,	//82	ȭ���ǺҲ�.   �ҵ���.
	ATTACKATB_EVIL							,	//83	�̻��� ���.
	ATTACKATB_ICE							,	//84	�̻��� ��� Ư�����.
	ATTACKATB_MAGIC4T						,	//85	¡Ű��ĭȥ Ư�� ���.
	ATTACKATB_JAPANGHOST					,	//86	�Ϻ��ͽ� Ư�� ���.
	ATTACKATB_KUKET							,	//87	����
	ATTACKATB_AUTA							,	//88	�ƿ�Ÿ ����.
	ATTACKATB_SUB1							,	//89	�������� ȭ����� 1
	ATTACKATB_SUB2							,	//90	�������� ȭ����� 2
	ATTACKATB_FIREATTACK					,	//91	ȭ������.
	ATTACKATB_RUNNINGFIRE					,	//92	����
	ATTACKATB_ENERGYPA						,	//93	��ǳ.
	ATTACKATB_LONG_SPEAR					,	//94	����â ��â��ȯ.
	ATTACKATB_PAPERDOLL						,	//95	����� ��������
	ATTACKATB_NEEDLEATTACK					,	//96	ħ����
	ATTACKATB_CLOUDWIND						,	//97	ǳ���
	ATTACKATB_STONEMAGIC					,	//98	������
	ATTACKATB_GODSPEAR						,	//99	��â�뵵
	ATTACKATB_FLOWERRAIN					,	//100	��õȭ��
	ATTACKATB_SOULCHANGE					,	//101	���Ǽ�
	ATTACKATB_MAGICCATTACK					,	//102	������ ����
	ATTACKATB_SUNBIYOUN						,	//103	���� ����
	ATTACKATB_SNOW							,	//104	������

	// actdoll (2004/03/06 5:00) : 3�� ��ġ���� �߰�
	ATTACKATB_PYTHON						,	//105	�̹��� ��������	// actdoll (2004/03/06 4:08) : �߰�
	ATTACKATB_JINEMPEROR					,	//106	����Ȳ��ȥ ��������	// actdoll (2004/03/06 4:08) : �߰�
	ATTACKATB_GUARDIAN_SNAKE				,	//107	��ȣ��(��)
	ATTACKATB_GUARDIAN_PIG					,	//108	��ȣ��(����)
	ATTACKATB_GUARDIAN_CHICKEN				,	//109	��ȣ��(��)

//----------------------------------------------------------------------------
// robypark (2004/05/17 16:48) : �߰� ��� - ��� 2�� ����
//----------------------------------------------------------------------------
	ATTACKATB_TURTLE_VEHICLE				,	// 110	����-�ź���, �̼��� 2�� ����
	ATTACKATB_THUNDER_VEHICLE				,	// 111	����-������, ���� 2�� ����
	ATTACKATB_QUAKE_VEHICLE					,	// 112	�Ϻ�-������, ��Ű��ī 2�� ����
	ATTACKATB_BLACK_DRAGON_VEHICLE			,	// 113	�Ϻ�-�����, ���̸��� 2�� ����
	ATTACKATB_CATAPULT						,	// 114	�߱�-�߼���, ����<=�̷� 2�� ����
	ATTACKATB_FARANGI_VEHICLE				,	// 115	�߱�-�Ҷ�������, �̷�<=���� 2�� ����
	ATTACKATB_FIRE_DRAGON_VEHICLE			,	// 116	�븸-ȭ����, ����â 2�� ����
	ATTACKATB_CHINESEPHOENIX				,	// 117	�븸-��Ȳ����, �弱ȭ 2�� ����

	ATTACKATB_CATDOLL						,
	ATTACKATB_OLDFOXDOLL					,
	ATTACKATB_RACCOONDOLL					,
	ATTACKATB_NINJADOLL						,
	ATTACKATB_SNOWWOMANDOOL					,
	ATTACKATB_YANGGUIBIDOLL					,
	ATTACKATB_JIJANGDOLL					,
	ATTACKATB_SNAKEWOMANDOLL				,

	// robypark (2004/6/4 13:6)
	// ��õ��� ����, �Ҷ�������
	ATTACKATB_ROAR_OF_LAUGHTER				,	// 126

	// robypark 2004/6/10 19:4
	// ����� õ������, �ñ����
	ATTACKATB_BLACK_DRAGON_ICE				,	// 127

	// robypark 2004/6/11 10:49
	// ����� õ������, ������
	ATTACKATB_BLACK_DRAGON_POISON			,	// 128

	// robypark 2004/7/14 16:35
	// ���� ���� ����
	ATTACKATB_DOLL_EVIS						,	// 129

	// robypark 2005/2/2 17:27
	// �޸� ���� ����
	ATTACKATB_DOLL_DHARMA					,	// 130

	// <����> ����� char-attack, calcattack�� RealApower�� �����ؾ� ��, ������� �������̾�� ��.

	ATTACKATB_MAX								//		�� ĳ������ ���� ����(���� ������ �ƴϴ�. �̰� ������ ĳ���͸� �߰��ϸ� ���� �ȵȴ�!!!!)
};						

//-----------------------------------------------------------------------------
// Name: CLASS_ATTACKATB
// Code: actdoll (2004-01-01)
// Desc: ������ ���� ��Ʈ�� ���ڸ� �����ϴ� Ŭ������.
//-----------------------------------------------------------------------------
class CLASS_ATTACKATB
{
private:
	struct DATA
	{
		char	m_pszString[48];
	};
	static DATA	m_Data[ATTACKATB_MAX];

private:
	static	void	SetData( int index, char *pszStr );

public:
	static	ENUM_ATTACKATB	GetData		( char *pszStr );
	static	const char*		GetData		( ENUM_ATTACKATB enIndex );
	static	int				GetMaxCount	( void )	{ return ATTACKATB_MAX; }

	CLASS_ATTACKATB();
	~CLASS_ATTACKATB();
};

#endif//__ENUM_ATTACKATB_H__