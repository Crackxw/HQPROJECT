//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - KindInfo ������ (ĳ���� ID)		
//	File Name		: Enum_KI_Character.cpp
//	Birth Date		: 2004. 03. 18.	
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		�Ż󿡼� ���Ǵ� ���ӿ� ĳ�����ε����� ����ȭ ��Ų �������̸�, �� ������ ���α׷� �����Ͽ� ÷�ε˴ϴ�.
//		ĳ���͸� �߰��� ������ Ư���� ��츦 �����ϰ� ���κа� KIND_ON_MAX ���̿� �߰��Ͻñ� �ٶ��ϴ�.				
//						
//===================================================================================================						

#include <GSL.h>

#include <string.h>
#include "Enum_KI_Character.h"

CLASS_KIND_ON_CHARACTER::DATA	CLASS_KIND_ON_CHARACTER::m_Data[KIND_ON_MAX]	= {0,};	// ����ƽ ���� �ʱ�ȭ
CLASS_KIND_ON_CHARACTER			g_clKICharacter;										// �ʱ�ȭ ������ �ӽ� ��������

//-----------------------------------------------------------------------------
// Name: SetData()													[private]
// Code: actdoll (2004-03-18)
// Desc: �ش� ���� �ε����� �ű⿡ �Ҵ�� ���� �̸��� ��ġ��Ų��.
//		index		- ������
//		pszStr		- �ش� �������� �̸�
//-----------------------------------------------------------------------------
void	CLASS_KIND_ON_CHARACTER::SetData( int index, char *pszStr )	
{ 
	if( !pszStr || index <= KIND_ON_FIRST || index >= KIND_ON_MAX ) return;
	strcpy( m_Data[index].m_pszString, pszStr ); 
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: �ش� ������ ���� �������� �޴´�.
//		pszStr		- ã�� �������� �̸�
//		return		- �ش� ������ �Ҵ�� ������. ������ ������ �Ѱ�ٸ� KIND_ON_INVALID�� ����
//-----------------------------------------------------------------------------
ENUM_KIND_ON_CHARACTER	CLASS_KIND_ON_CHARACTER::GetData( char *pszStr )
{
	if( !pszStr )		return KIND_ON_INVALID;
	for( register int i=KIND_ON_FIRST; i<KIND_ON_MAX; i++ )	
	{
		if( strcmp( m_Data[i].m_pszString, pszStr ) == 0 ) return (ENUM_KIND_ON_CHARACTER)i;
	}
	return KIND_ON_INVALID;
}

//-----------------------------------------------------------------------------
// Name: GetData()
// Code: actdoll (2004-03-18)
// Desc: �ش� ������ ���� �������� �޴´�.
//		enIndex		- ã�� ������
//		return		- �ش� �������� ������. ������ NULL�� ����. ����� �ȵƴٸ� ��Ʈ���� ""
//-----------------------------------------------------------------------------
const char* CLASS_KIND_ON_CHARACTER::GetData( ENUM_KIND_ON_CHARACTER enIndex )
{
	if( enIndex <= KIND_ON_FIRST || enIndex >= KIND_ON_MAX )		return NULL;
	return (const char*)(m_Data[enIndex].m_pszString);
}

//-----------------------------------------------------------------------------
// Name: ~CLASS_KIND_ON_CHARACTER()
// Code: actdoll (2004-03-18)
// Desc: �Ҹ���
//-----------------------------------------------------------------------------
CLASS_KIND_ON_CHARACTER::~CLASS_KIND_ON_CHARACTER()
{

}

//-----------------------------------------------------------------------------
// Name: CLASS_KIND_ON_CHARACTER()
// Code: actdoll (2004-03-18)
// Desc: ������(�����߿�!!!)
//-----------------------------------------------------------------------------
CLASS_KIND_ON_CHARACTER::CLASS_KIND_ON_CHARACTER()
{
	memset( m_Data, 0, (sizeof(DATA) * KIND_ON_MAX) );

#define SET_PROTOCOL( index )	SetData( index, #index );
		
// ������ �߰��� ��� �� �κ� �� �Ʒ� (//EOF����)�� �߰���Ų��.
	
	SET_PROTOCOL( KIND_PLAYERSTART					)	//1		�ĺ���(���а�)
		
	// ��� 					
	SET_PROTOCOL( KIND_MANAGER1						)	//2		���1
	
	// ���ΰ�					
	SET_PROTOCOL( KIND_ON_KEON						)	//3		��ö��
	SET_PROTOCOL( KIND_ON_MYUNGHWA					)	//4		�̸�ȭ
	SET_PROTOCOL( KIND_ON_RYO						)	//5		�ø��� ��
	SET_PROTOCOL( KIND_ON_HANAHINE					)	//6		�ճ� �ϳ�����
	SET_PROTOCOL( KIND_ON_SORYUNG					)	//7		�ռҷ�
	SET_PROTOCOL( KIND_ON_MIRYUNG					)	//8		�չ̷�
	SET_PROTOCOL( KIND_ON_DANG						)	//9		��ȣ��
	SET_PROTOCOL( KIND_ON_SUYOUN					)	//10	�����
	
	// ���� �뺴 					
	SET_PROTOCOL( KIND_ON_HORSEARCHERK				)	//15	�⸶�ü�
	SET_PROTOCOL( KIND_ON_JUSTICEK					)	//16	���� 
	SET_PROTOCOL( KIND_ON_BUDAK						)	//17	�İ��
	SET_PROTOCOL( KIND_ON_SWORDK					)	//18	â���� 
	SET_PROTOCOL( KIND_ON_ARCHERK					)	//19	Ȱ����
	SET_PROTOCOL( KIND_ON_BUTCHER					)	//20	Į����
	SET_PROTOCOL( KIND_ON_PORTERK					)	//21	��������
	
	SET_PROTOCOL( KIND_ON_HORSEARCHERUPK			)	//22	��ޱ⸶�ü�
	SET_PROTOCOL( KIND_ON_JUSTICEUPK				)	//23	�������
	SET_PROTOCOL( KIND_ON_BUDAUPK					)	//24	����İ��
	SET_PROTOCOL( KIND_ON_SWORDUPK					)	//25	���â����
	SET_PROTOCOL( KIND_ON_ARCHERUPK					)	//26	���Ȱ����
	
	SET_PROTOCOL( KIND_GENERAL1K					)	//27	����
	SET_PROTOCOL( KIND_GENERAL2K					)	//28	�̼���
	SET_PROTOCOL( KIND_GENERAL3K					)	//29	������
	SET_PROTOCOL( KIND_GENERAL4K					)	//30	�����
	SET_PROTOCOL( KIND_GENERAL5K					)	//31	�Ÿ�
	SET_PROTOCOL( KIND_GENERAL6K					)	//32	����(�Ⱦ�.�Ʒ��ٸ�����������)
	SET_PROTOCOL( KIND_GENERAL7K					)	//33	�����(�Ⱦ�)
	SET_PROTOCOL( KIND_GENERAL8K					)	//34	��ù�
	SET_PROTOCOL( KIND_CANNON2K						)	//35	���屺��(�Ⱦ�)
	
	// �Ϻ� �뺴 					
	SET_PROTOCOL( KIND_ON_SWORDJ					)	//40	�˼�����
	SET_PROTOCOL( KIND_ON_GUNJ						)	//41	���ѳ���
	SET_PROTOCOL( KIND_ON_NINJAJ					)	//42	����
	SET_PROTOCOL( KIND_ON_HORSESWORDJ				)	//43	���볶��
	SET_PROTOCOL( KIND_ON_ADVBUDAJ					)	//44	�𸶻� 
	SET_PROTOCOL( KIND_ON_DSWORD					)	//45	�⸶����
	SET_PROTOCOL( KIND_ON_PORTERJ					)	//46	�Ϻ�����
	
	SET_PROTOCOL( KIND_ON_SWORDUPJ					)	//47	��ް˼�����
	SET_PROTOCOL( KIND_ON_GUNUPJ					)	//48	������ѳ���
	SET_PROTOCOL( KIND_ON_NINJAUPJ					)	//49	��޴���
	SET_PROTOCOL( KIND_ON_HORSESWORDUPJ				)	//50	��޴��볶��
	SET_PROTOCOL( KIND_ON_ADVBUDAUPJ				)	//51	����𸶻� 
	
	SET_PROTOCOL( KIND_GENERAL1J					)	//52	��Ͻ�
	SET_PROTOCOL( KIND_GENERAL2J					)	//53	�����
	SET_PROTOCOL( KIND_GENERAL3J					)	//54	��Ű��ī
	SET_PROTOCOL( KIND_GENERAL4J					)	//55	���̼��
	SET_PROTOCOL( KIND_GENERAL5J					)	//56	����(�Ⱦ�)
	SET_PROTOCOL( KIND_GENERAL6J					)	//57	������
	SET_PROTOCOL( KIND_GENERAL7J					)	//58	��������(�Ⱦ�)
	SET_PROTOCOL( KIND_GENERAL8J					)	//59	�ƻ���
	
	// �븸 �뺴 					
	SET_PROTOCOL( KIND_ON_BUDAT						)	//63	�ּ���
	SET_PROTOCOL( KIND_ON_AMAZONT					)	//64	������
	SET_PROTOCOL( KIND_ON_AXET						)	//65	�������� 
	SET_PROTOCOL( KIND_ON_SWORDT					)	//66	������
	SET_PROTOCOL( KIND_ON_GUNT						)	//67	�����Ѽ�
	SET_PROTOCOL( KIND_ON_PORTERT					)	//68	�븸����
	SET_PROTOCOL( KIND_ON_BEASTWARRIOR				)	//69	�߼�����
	
	SET_PROTOCOL( KIND_ON_BUDAUPT					)	//70	����ּ���
	SET_PROTOCOL( KIND_ON_AMAZONUPT					)	//71	��޿�����
	SET_PROTOCOL( KIND_ON_AXEUPT					)	//72	��޵������� 
	SET_PROTOCOL( KIND_ON_SWORDUPT					)	//73	��޺�����
	SET_PROTOCOL( KIND_ON_GUNUPT					)	//74	��޼����Ѽ�
	
	SET_PROTOCOL( KIND_ON_GENERAL1T					)	//75	�հ��� 
	SET_PROTOCOL( KIND_ON_GENERAL2T					)	//76	�丣������
	SET_PROTOCOL( KIND_ON_GENERAL3T					)	//77	������
	SET_PROTOCOL( KIND_ON_GENERAL4T					)	//78	������
	SET_PROTOCOL( KIND_ON_GENERAL5T					)	//79	������
	SET_PROTOCOL( KIND_ON_GENERAL6T					)	//80	����â
	
	// �߱� �뺴					
	SET_PROTOCOL( KIND_ON_FAN_C						)	//85	���谡
	SET_PROTOCOL( KIND_ON_BUDAK_C					)	//86	������
	SET_PROTOCOL( KIND_ON_MUDOGA_C					)	//87	������
	SET_PROTOCOL( KIND_ON_SWORD_C					)	//88	ūĮ����
	SET_PROTOCOL( KIND_ON_FIREGUN_C					)	//89	ȭ����
	SET_PROTOCOL( KIND_ON_MAGIC_C					)	//90	������
	SET_PROTOCOL( KIND_ON_PORTER_C					)	//91	�߱�����
	
	SET_PROTOCOL( KIND_ON_FANUP_C					)	//92	��޸��谡
	SET_PROTOCOL( KIND_ON_BUDAKUP_C					)	//93	��޼�����
	SET_PROTOCOL( KIND_ON_MUDOGAUP_C				)	//94	��޹�����
	SET_PROTOCOL( KIND_ON_SWORDUP_C					)	//95	���ūĮ����
	SET_PROTOCOL( KIND_ON_FIREGUNUP_C				)	//96	���ȭ����
	
	SET_PROTOCOL( KIND_ON_GENERAL1_C				)	//97	�������
	SET_PROTOCOL( KIND_ON_GENERAL2_C				)	//98	�̷�
	SET_PROTOCOL( KIND_ON_GENERAL3_C				)	//99	��������
	SET_PROTOCOL( KIND_ON_GENERAL4_C				)	//100	������
	SET_PROTOCOL( KIND_ON_GENERAL5_C				)	//101	����
	SET_PROTOCOL( KIND_ON_GENERAL6_C				)	//102	����
	
	// ���� 					
	SET_PROTOCOL( KIND_ON_HARUBANG					)	//107	���Ϸ��
	SET_PROTOCOL( KIND_ON_CRAB						)	//108	�ٴٰ�
	SET_PROTOCOL( KIND_ON_LEECH						)	//109	�հŸӸ�
	SET_PROTOCOL( KIND_ON_SEAHORSE					)	//110	�ظ� 
	SET_PROTOCOL( KIND_WHITETIGER					)	//111	��ȣ 
	SET_PROTOCOL( KIND_GHOSTCAT						)	//112	�͹� 
	SET_PROTOCOL( KIND_TUTTLE						)	//113	�ٴٰź���
	SET_PROTOCOL( KIND_BUTTERFLY					)	//114	������
	SET_PROTOCOL( KIND_WIDOW						)	//115	�Ź̿䱫
	SET_PROTOCOL( KIND_INSECT						)	//116	�縶��
	SET_PROTOCOL( KIND_WORRIORGHOST					)	//117	������ȥ
	SET_PROTOCOL( KIND_SINGERMGHOST					)	//118	�Ű���ȥ
	SET_PROTOCOL( KIND_MAMMOTH						)	//119	���ڳ���
	SET_PROTOCOL( KIND_WILDBOAR						)	//120	�����
	SET_PROTOCOL( KIND_ON_THIEF						)	//121	������ 
	SET_PROTOCOL( KIND_ON_SEAKING					)	//122	������ 
	SET_PROTOCOL( KIND_ON_SWORDPIRATE				)	//123	�ֱ�
	SET_PROTOCOL( KIND_ON_GUNPIRATE					)	//124	���ѿֱ�
	SET_PROTOCOL( KIND_ON_SNOWWOMAN					)	//125	����
	SET_PROTOCOL( KIND_ON_EATERDEMON				)	//126   �ı�
	SET_PROTOCOL( KIND_ON_WEIRDPLANT				)	//127	�����
	SET_PROTOCOL( KIND_ON_HUNGRYDEMON				)	//128	�Ʊ�
	SET_PROTOCOL( KIND_ON_TIGER						)	//129	ȣ����
	SET_PROTOCOL( KIND_ON_DOGDEMON					)	//130	�߽�
	SET_PROTOCOL( KIND_ON_ONGHWA					)	//131	��õ������
	SET_PROTOCOL( KIND_ON_BANDIT1					)	//132	��������
	SET_PROTOCOL( KIND_ON_BANDIT2					)	//133	����
	SET_PROTOCOL( KIND_ON_BANDIT3					)	//134	ö�����
	SET_PROTOCOL( KIND_ON_BANDIT4					)	//135	�ü�����
	SET_PROTOCOL( KIND_ON_GACKDAGUI					)	//136	���ٱ�
	SET_PROTOCOL( KIND_ON_GATPHA					)	//137	����
	SET_PROTOCOL( KIND_ON_BOGY						)	//138	������
	SET_PROTOCOL( KIND_ON_FOX						)	//139	����ȣ
	SET_PROTOCOL( KIND_ON_STARFISH					)	//140	�Ұ��縮
	SET_PROTOCOL( KIND_ON_SHIP1						)	//141	��1
	SET_PROTOCOL( KIND_ON_ONI						)	//142	����
	SET_PROTOCOL( KIND_ON_SNOWDEMON					)	//143	����
	SET_PROTOCOL( KIND_ON_FURHEADDEMON				)	//144	�а���
	SET_PROTOCOL( KIND_ON_FIREDRAGON				)	//145	ȭ��
	SET_PROTOCOL( KIND_ON_GOLDDRAGON				)	//146	Ȳ��
	SET_PROTOCOL( KIND_ON_BAT						)	//147	����
	SET_PROTOCOL( KIND_ON_TOAD						)	//148	�β���
	SET_PROTOCOL( KIND_ON_CENTIPEDE					)	//149	������
	SET_PROTOCOL( KIND_ON_DUEOKSINI					)	//150	�ξ�ô�
	SET_PROTOCOL( KIND_ON_GANGSI					)	//151	����
	SET_PROTOCOL( KIND_ON_RACCOONDOG				)	//152	�ʱ���
	SET_PROTOCOL( KIND_ON_DEER						)	//153	�罿
	SET_PROTOCOL( KIND_ON_BEAR						)	//154	�ݴް� 
	SET_PROTOCOL( KIND_ON_HORSESPEAR				)	//155	�⸶â�� 
	SET_PROTOCOL( KIND_ON_CAT						)	//156	���ϰ����
	SET_PROTOCOL( KIND_ON_DUCK						)	//157	�븸����
	SET_PROTOCOL( KIND_ON_HORNET					)	//158	�븸�չ�
	SET_PROTOCOL( KIND_ON_MBANDIT1					)	//159	�����θ� 
	SET_PROTOCOL( KIND_ON_MBANDIT2					)	//160	��ġ���� 
	SET_PROTOCOL( KIND_ON_MBANDIT3					)	//161	�Ѽ����� 
	SET_PROTOCOL( KIND_ON_CHAWOO					)	//162	�������� 
	SET_PROTOCOL( KIND_ON_PIRATES					)	//163	���� 
	SET_PROTOCOL( KIND_ON_PANDER					)	//164	�Ҵ� 
	SET_PROTOCOL( KIND_ON_SNAKE						)	//165	������ 
	SET_PROTOCOL( KIND_ON_CROCODILE					)	//166	���ξǾ� 
	SET_PROTOCOL( KIND_ON_HOOK						)	//167	�������� 
	SET_PROTOCOL( KIND_ON_SMALLGANG					)	//168	��������
	SET_PROTOCOL( KIND_ON_HORSEMAN					)	//169	������
	SET_PROTOCOL( KIND_ON_BABARIAN					)	//170	���ֹ�
	SET_PROTOCOL( KIND_ON_BABARIANSHAMAN			)	//171	���ֹι���
	SET_PROTOCOL( KIND_ON_BADDEER					)	//172	�絹��
	SET_PROTOCOL( KIND_ON_CACTUS					)	//173	����������
	SET_PROTOCOL( KIND_ON_SCORPION					)	//174	������ 
	SET_PROTOCOL( KIND_ON_SKYNINE					)	//175	õ��
	SET_PROTOCOL( KIND_ON_ICETREE					)	//176	��������
	SET_PROTOCOL( KIND_ON_SNOWLIZARD				)	//177	��������
	SET_PROTOCOL( KIND_ON_MADBULL					)	//178	����
	SET_PROTOCOL( KIND_ON_BIGTREE					)	//179	�Ŵ��
	SET_PROTOCOL( KIND_ON_MAGICIAN					)	//180	����� 
	SET_PROTOCOL( KIND_ON_BADMERCHANT				)	//181	�Ǵ����� 
	SET_PROTOCOL( KIND_ON_RUNNERHAND				)	//182	����������
	SET_PROTOCOL( KIND_ON_SANDGOD					)	//183	���ǽ�
	SET_PROTOCOL( KIND_ON_SEAENVOY					)	//184	�ٴ��ǻ��
	SET_PROTOCOL( KIND_ON_VIPER						)	//185	�칫��
	SET_PROTOCOL( KIND_ON_AMAKUSA					)	//186	�Ƹ����
	SET_PROTOCOL( KIND_ON_LAVAMONSTER				)	//187	��ϱ���
	SET_PROTOCOL( KIND_ON_EVIL						)	//188	�̻��� ���
	SET_PROTOCOL( KIND_ON_BIRD						)	//189	�θ���
	SET_PROTOCOL( KIND_ON_WEASEL					)	//190	������
	SET_PROTOCOL( KIND_ON_COBRA						)	//191	�ں��
	SET_PROTOCOL( KIND_ON_SOUL						)	//192	¡�⽺ĭ�� ȥ
	SET_PROTOCOL( KIND_ON_VOLCANOFLAME				)	//193	ȭ���� �Ҳ�
	SET_PROTOCOL( KIND_ON_REDMADBULL				)	//194	��������
	SET_PROTOCOL( KIND_ON_MAGICOFFICER				)	//195	������
	SET_PROTOCOL( KIND_ON_CAPBEAR					)	//196	����ݴް�
	SET_PROTOCOL( KIND_ON_KOREANGHOST				)	//197	ó��ͽ�
	SET_PROTOCOL( KIND_ON_SKELETON					)	//198	�ذ�
	SET_PROTOCOL( KIND_ON_SKELETONSOLDIER			)	//199	�ذ񺴻�
	SET_PROTOCOL( KIND_ON_PIRATECAPTAIN				)	//200	��������
	SET_PROTOCOL( KIND_ON_TAIWANGHOST				)	//201	�븸�ͽ�
	SET_PROTOCOL( KIND_ON_JAPANGHOST				)	//202	�Ϻ��ͽ�
	SET_PROTOCOL( KIND_ON_AUTA						)	//203	�ƿ�Ÿ
	SET_PROTOCOL( KIND_ON_JUCK						)	//204	������
	SET_PROTOCOL( KIND_ON_FEMALESHAMAN				)	//205	����
	SET_PROTOCOL( KIND_ON_QUEEN						)	//206	��������
	SET_PROTOCOL( KIND_ON_DONKEY					)	//207	�糪��
	SET_PROTOCOL( KIND_ON_AGILETIGER				)	//208	��ȣ
	SET_PROTOCOL( KIND_ON_PAWNTIGER					)	//209	����ȣ����
	SET_PROTOCOL( KIND_ON_OWL						)	//210	�û���
	SET_PROTOCOL( KIND_ON_LEOPARD					)	//211	ǥ��
	SET_PROTOCOL( KIND_ON_HUNSWARRIOR				)	//212	�������
	SET_PROTOCOL( KIND_ON_HUNSHORSE					)	//213	���⸶��
	SET_PROTOCOL( KIND_ON_HUNSHORSEARCHER			)	//214	����ú�
	SET_PROTOCOL( KIND_ON_GATEKEEPER				)	//215	������
	SET_PROTOCOL( KIND_ON_CLAYFIGURE				)	//216	�⸶������
	SET_PROTOCOL( KIND_ON_PHOENIX					)	//217	�һ���
	SET_PROTOCOL( KIND_ON_JINEMPEROR				)	//218	����Ȳ��ȥ
	SET_PROTOCOL( KIND_ON_MEDICALMAN				)	//219	�Ǽ���
	SET_PROTOCOL( KIND_ON_HEOJUN					)	//220	����
	SET_PROTOCOL( KIND_ON_COSMIC					)	//221	�����
	SET_PROTOCOL( KIND_ON_SEIMEI					)	//222	���̸���
	SET_PROTOCOL( KIND_ON_YUMRUCK					)	//223	���»�
	SET_PROTOCOL( KIND_ON_JANGSUNHWA				)	//224	�弱ȭ
	SET_PROTOCOL( KIND_ON_LONGSPEAR					)	//225	��â����
	SET_PROTOCOL( KIND_ON_KOAKHU					)	//226	����
	SET_PROTOCOL( KIND_ON_SNOWMAN1					)	//227	�����1
	SET_PROTOCOL( KIND_ON_SNOWMAN2					)	//228	�����2
	SET_PROTOCOL( KIND_ON_SANTA1					)	//229	��¥��Ÿ1
	SET_PROTOCOL( KIND_ON_SANTA2					)	//230	��¥��Ÿ2
	SET_PROTOCOL( KIND_ON_SANTA3					)	//231	��¥��Ÿ3
	SET_PROTOCOL( KIND_ON_MOTHER_DEER1				)	//232	�����絹��1
	SET_PROTOCOL( KIND_ON_MOTHER_DEER2				)	//233	�����絹��2
	SET_PROTOCOL( KIND_ON_MOTHER_DEER3				)	//234	�����絹��3
	SET_PROTOCOL( KIND_ON_MOTHER_DEER4				)	//235	�����絹��4
	SET_PROTOCOL( KIND_ON_SANTA4					)	//236	��¥��Ÿ 4
	SET_PROTOCOL( KIND_ON_SANTA_TW					)	//237	�븸��¥��Ÿ
	SET_PROTOCOL( KIND_ON_MOTHER_DEER_TW			)	//238	�븸�����絹��
	SET_PROTOCOL( KIND_ON_SHIP2						)	//239	��2
	SET_PROTOCOL( KIND_ON_SHIP3						)	//240	��3
	SET_PROTOCOL( KIND_ON_SHIP4						)	//241	��4
	SET_PROTOCOL( KIND_ON_SHIP5						)	//242	��5
	SET_PROTOCOL( KIND_ON_SHIP6						)	//243	��6
	SET_PROTOCOL( KIND_ON_SHIP7						)	//244	��7
	SET_PROTOCOL( KIND_ON_SHIP8						)	//245	��8
	SET_PROTOCOL( KIND_ON_FLYDRAGON					)	//246	�ͷ�
	SET_PROTOCOL( KIND_ON_RAPTOR					)	//247	����
	SET_PROTOCOL( KIND_ON_TIRANO					)	//248	Ƽ����츣��
	SET_PROTOCOL( KIND_ON_MONKEY					)	//249	������
	
	// actdoll (2004/02/26 11:12) : �߰��� ���� - ����� �ʿ�					
	SET_PROTOCOL( KIND_ON_HELLKING					)	//250	������
	SET_PROTOCOL( KIND_ON_HELLMESSENGER				)	//251	���»���
	SET_PROTOCOL( KIND_ON_GHOSTSAMURAI				)	//252	���� �繫����
	SET_PROTOCOL( KIND_ON_PYTHON					)	//253	�̹���
	SET_PROTOCOL( KIND_ON_GHOSTELEPHANT				)	//254	�����ڳ���
	SET_PROTOCOL( KING_ON_GHOSTSEAKING				)	//255	����������
	SET_PROTOCOL( KIND_ON_GHOSTPIRATECAPTAIN		)	//256	������������
	SET_PROTOCOL( KIND_ON_GHOSTSKELETON				)	//257	�����ذ�
	SET_PROTOCOL( KIND_ON_GHOSTSKELETONSOLDIER		)	//258	�����ذ񺴻�
	SET_PROTOCOL( KIND_ON_GHOSTFEMALESHAMAN			)	//269	���ɹ���
	SET_PROTOCOL( KIND_ON_GHOSTVOLCANOFLAME			)	//260	���ɺҲ�
	
	// ��ȣ�� �߰�ĳ����			
	SET_PROTOCOL( KIND_ON_GUARDIAN_MOUSE			)	//261	��ȣ�ο�(��)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW1				)	//262	��ȣ�ο�(��1)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW2				)	//263	��ȣ�ο�(��2)
	SET_PROTOCOL( KIND_ON_GUARDIAN_CAW3				)	//264	��ȣ�ο�(��3)
	
	SET_PROTOCOL( KIND_ON_GUARDIAN_CHICK			)	//265	��ȣ�ο�(���Ƹ�)
	SET_PROTOCOL( KIND_ON_GUARDIAN_PIG				)	//266	��ȣ�ο�(����)
	SET_PROTOCOL( KIND_ON_GUARDIAN_SNAKE			)	//267	��ȣ�ο�(��)
	SET_PROTOCOL( KIND_ON_GUARDIAN_MONKEY			)	//268	��ȣ�ο�(������)
	SET_PROTOCOL( KIND_ON_JAP_INVADERS				)	//269	ħ���ֱ�
		
//	��� 2�� ����(2004-5-11, robypark)
	SET_PROTOCOL( KIND_ON_TURTLE_VEHICLE			)	// 270	U102	����-�ź���
	SET_PROTOCOL( KIND_ON_THUNDER_VEHICLE			)	// 271	U103	����-������
	SET_PROTOCOL( KIND_ON_QUAKE_VEHICLE				)	// 272	U104	�Ϻ�-������
	SET_PROTOCOL( KIND_ON_BLACK_DRAGON_VEHICLE		)	// 273	U105	�Ϻ�-�����
	SET_PROTOCOL( KIND_ON_CATAPULT					)	// 274	U106	�߱�-�߼���
	SET_PROTOCOL( KIND_ON_FARANGI_VEHICLE			)	// 275	U107	�߱�-�Ҷ�������
	SET_PROTOCOL( KIND_ON_FIRE_DRAGON_VEHICLE		)	// 276	U108	�븸-ȭ����
	SET_PROTOCOL( KIND_ON_CHINESEPHOENIX			)	// 277	U109	�븸-��Ȳ����

// 2�� ��� ��ų �߰� ����
	// robypark 2004/6/4 20:29
	// �Ҷ������� ��� ��õ��Ҹ� ���� ĳ����
	SET_PROTOCOL( KIND_ON_ROAR_OF_LAUGHTER			)	// 278

//#ifdef _SET_CHARACTER_LIST
	// robypark 2004/7/22 16:18
	// �߱��뺴 - ������(���� 20�̻��� �Ǹ� ��޿뺴�̹����� ����ȴ�.)
	SET_PROTOCOL( KIND_ON_MAGIC_C_UP				)	// 279(U110)

	// robypark 2004/8/26 18:58
	// �븸 �뺴 - �߼����� => ��� �߼�����
	SET_PROTOCOL( KIND_ON_BEASTWARRIOR_UP			)	// 280(U111)
	// �븸 �뺴 - ���»� => ��� ���»�
	SET_PROTOCOL( KIND_ON_YUMRUCK_UP				)	// 281(U112)
	// �Ϻ� �뺴 - ����� => ��� �����
	SET_PROTOCOL( KIND_ON_COSMIC_UP					)	// 282(U113)
	// ���� �뺴 - �Ǽ��� => ��� �Ǽ���
	SET_PROTOCOL( KIND_ON_MEDICALMAN_UP				)	// 283(U114)
	// ���� �뺴 - Į���� => ��� Į����
	SET_PROTOCOL( KIND_ON_BUTCHER_UP				)	// 284(U115)
	// �߱� �뺴 - ��â���� => ��� ��â����
	SET_PROTOCOL( KIND_ON_LONGSPEAR_UP				)	// 285(U116)
//#endif

	// robypark 2004/9/17 13:35
	// ���� �� ������
	SET_PROTOCOL( KIND_ON_RED_EYE_SEABEDMAN			)	// 286(M147)

	// ��¥ �������
	SET_PROTOCOL( KIND_ON_HUNSWARRIOR_FAKE			)	// 287(M148)

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 16:27
	// ������ ���� ĳ���� �߰�
	// ������ - �˺�
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SWORDMAN		)	// 288
	// ������ - â��
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SPEARMAN		)	// 289
	// ������ - �⸶��
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_HORSEMAN		)	// 290
	// ������ - �ú�
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_ARCHERMAN	)	// 291
	// ������ - ����
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SIEGE_TOWER	)	// 292
	// ������ - �߼���
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_CATAPULT		)	// 293
	// ������ - ���� ����
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_SUPPLY_WAGON	)	// 294
	// �� /////////////////////////////////////////////////////////////////

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/11/4 17:0
	// ��ȭ ���� �̺�Ʈ ����
	// ���Ȱ� �����, M149
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE1			)	// 295
	// ���Ȱ� ��ݺ�, M150
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE2			)	// 296
	// �� /////////////////////////////////////////////////////////////////

	// ������ - ����, U121
	SET_PROTOCOL( KIND_ON_SIEGEWARFARE_GARRISON_ARMY)	// 297

	// robypark 2004/11/22 18:6
	// ���Ȱ� ����ǳü����, M151
	SET_PROTOCOL( KIND_ON_JEOPALGE_TYPE3			)	// 298

	// robypark 2005/1/14 17:49
	// ������ �ǳ��� �̹���Ȱ���� ���� ĳ���� �߰�
	SET_PROTOCOL( KIND_ON_REFUGEE_KEON				)	// 299, �ǳ��� ��ö��
	SET_PROTOCOL( KIND_ON_REFUGEE_MYUNGHWA			)	// 300, �ǳ��� �̸�ȭ
	SET_PROTOCOL( KIND_ON_REFUGEE_RYO				)	// 301, �ǳ��� �ø��ٷ�
	SET_PROTOCOL( KIND_ON_REFUGEE_HANAHINE			)	// 302, �ǳ��� �ճ��ϳ�����
	SET_PROTOCOL( KIND_ON_REFUGEE_SORYUNG			)	// 303, �ǳ��� �ռҷ�
	SET_PROTOCOL( KIND_ON_REFUGEE_MIRYUNG			)	// 304, �ǳ��� �չ̷�
	SET_PROTOCOL( KIND_ON_REFUGEE_DANG				)	// 305, �ǳ��� ��ȣ��
	SET_PROTOCOL( KIND_ON_REFUGEE_SUYOUN			)	// 306, �ǳ��� �����

// EOF
#undef	SET_PROTOCOL		
}
