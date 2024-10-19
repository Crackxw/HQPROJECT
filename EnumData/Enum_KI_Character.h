//===================================================================================================				
//				
//	*** Comments Ver 3.01 ***			
//	Project Name	: ������ �¶��� [�Ż�] - KindInfo ������ (ĳ���� ID)		
//	File Name		: Enum_KI_Character.h
//	Birth Date		: 2004. 03. 18.	
//	Creator			: �� �� ��
//	Editer			: �� �� ��
//	Copyright		: Joyon Entertainments. Co. 2000. 2004.	
//	Comments		: 	
//		�Ż󿡼� ���Ǵ� ���ӿ� ĳ�����ε����� ����ȭ ��Ų �������̸�, �� ������ ���α׷� �����Ͽ� ÷�ε˴ϴ�.
//		ĳ���͸� �߰��� ������ Ư���� ��츦 �����ϰ� ���κа� KIND_ON_MAX ���̿� �߰��Ͻñ� �ٶ��ϴ�.				
//						
//===================================================================================================						

#ifndef	__ENUM_KI_CHARACTER_H__
#define	__ENUM_KI_CHARACTER_H__

//-----------------------------------------------------------------------------
// Name: ENUM_KIND_ON_CHARACTER
// Code: actdoll (2004-01-01)
// Desc: 
//		������ �����ϴ� ���ڼ��� �������� 47�ڸ� �Ѿ�� �ȵȴ�!!!!
//		�̰��� ������ �߰��� ��� Enum_KI_Character.cpp�� ������ �κп���
//		SET_PROTOCOL�� �̿��� �ش� �������� �ݵ�� ����ؾ� �Ѵ�. ��������!!!
//-----------------------------------------------------------------------------
typedef enum	ENUM_KIND_ON_CHARACTER		
{						
	KIND_ON_INVALID					= -1	,	//		�������̴�.(�� ���� �׾ ����Һ�!!!)
	KIND_ON_FIRST					= 0		,	//		�Ⱦ��� ��. �������� ���� ���� ���̴�.
	KIND_ON_NONE					= 0		,	//		���ð�. �� ���� �������� ���� �۵��� ���ϰԲ� �Ѵ�.

	KIND_PLAYERSTART				= 1		,	//1		�ĺ���(���а�)

	// ��� 					
	KIND_MANAGER1							,	//2		���1

	// ���ΰ�					
	KIND_ON_KEON							,	//3		��ö��
	KIND_ON_MYUNGHWA						,	//4		�̸�ȭ
	KIND_ON_RYO								,	//5		�ø��� ��
	KIND_ON_HANAHINE						,	//6		�ճ� �ϳ�����
	KIND_ON_SORYUNG							,	//7		�ռҷ�
	KIND_ON_MIRYUNG							,	//8		�չ̷�
	KIND_ON_DANG							,	//9		��ȣ��
	KIND_ON_SUYOUN							,	//10	�����

	// ���� �뺴 					
	KIND_ON_HORSEARCHERK			= 15	,	//15	�⸶�ü�
	KIND_ON_JUSTICEK						,	//16	���� 
	KIND_ON_BUDAK							,	//17	�İ��
	KIND_ON_SWORDK							,	//18	â���� 
	KIND_ON_ARCHERK							,	//19	Ȱ����
	KIND_ON_BUTCHER							,	//20	Į����
	KIND_ON_PORTERK							,	//21	��������

	KIND_ON_HORSEARCHERUPK					,	//22	��ޱ⸶�ü�
	KIND_ON_JUSTICEUPK						,	//23	�������
	KIND_ON_BUDAUPK							,	//24	����İ��
	KIND_ON_SWORDUPK						,	//25	���â����
	KIND_ON_ARCHERUPK						,	//26	���Ȱ����

	KIND_GENERAL1K							,	//27	����
	KIND_GENERAL2K							,	//28	�̼���
	KIND_GENERAL3K							,	//29	������
	KIND_GENERAL4K							,	//30	�����
	KIND_GENERAL5K							,	//31	�Ÿ�
	KIND_GENERAL6K							,	//32	����(�Ⱦ�.�Ʒ��ٸ�����������)
	KIND_GENERAL7K							,	//33	�����(�Ⱦ�)
	KIND_GENERAL8K							,	//34	��ù�
	KIND_CANNON2K							,	//35	���屺��(�Ⱦ�)

	// �Ϻ� �뺴 					
	KIND_ON_SWORDJ					= 40	,	//40	�˼�����
	KIND_ON_GUNJ							,	//41	���ѳ���
	KIND_ON_NINJAJ							,	//42	����
	KIND_ON_HORSESWORDJ						,	//43	���볶��
	KIND_ON_ADVBUDAJ						,	//44	�𸶻� 
	KIND_ON_DSWORD							,	//45	�⸶����
	KIND_ON_PORTERJ							,	//46	�Ϻ�����

	KIND_ON_SWORDUPJ						,	//47	��ް˼�����
	KIND_ON_GUNUPJ							,	//48	������ѳ���
	KIND_ON_NINJAUPJ						,	//49	��޴���
	KIND_ON_HORSESWORDUPJ					,	//50	��޴��볶��
	KIND_ON_ADVBUDAUPJ						,	//51	����𸶻� 

	KIND_GENERAL1J							,	//52	��Ͻ�
	KIND_GENERAL2J							,	//53	�����
	KIND_GENERAL3J							,	//54	��Ű��ī
	KIND_GENERAL4J							,	//55	���̼��
	KIND_GENERAL5J							,	//56	����(�Ⱦ�)
	KIND_GENERAL6J							,	//57	������
	KIND_GENERAL7J							,	//58	��������(�Ⱦ�)
	KIND_GENERAL8J							,	//59	�ƻ���

	// �븸 �뺴 					
	KIND_ON_BUDAT					= 63	,	//63	�ּ���
	KIND_ON_AMAZONT							,	//64	������
	KIND_ON_AXET							,	//65	�������� 
	KIND_ON_SWORDT							,	//66	������
	KIND_ON_GUNT							,	//67	�����Ѽ�
	KIND_ON_PORTERT							,	//68	�븸����
	KIND_ON_BEASTWARRIOR					,	//69	�߼�����

	KIND_ON_BUDAUPT							,	//70	����ּ���
	KIND_ON_AMAZONUPT						,	//71	��޿�����
	KIND_ON_AXEUPT							,	//72	��޵������� 
	KIND_ON_SWORDUPT						,	//73	��޺�����
	KIND_ON_GUNUPT							,	//74	��޼����Ѽ�

	KIND_ON_GENERAL1T						,	//75	�հ��� 
	KIND_ON_GENERAL2T						,	//76	�丣������
	KIND_ON_GENERAL3T						,	//77	������
	KIND_ON_GENERAL4T						,	//78	������
	KIND_ON_GENERAL5T						,	//79	������
	KIND_ON_GENERAL6T						,	//80	����â

	// �߱� �뺴					
	KIND_ON_FAN_C					= 85	,	//85	���谡
	KIND_ON_BUDAK_C							,	//86	������
	KIND_ON_MUDOGA_C						,	//87	������
	KIND_ON_SWORD_C							,	//88	ūĮ����
	KIND_ON_FIREGUN_C						,	//89	ȭ����
	KIND_ON_MAGIC_C							,	//90	������
	KIND_ON_PORTER_C						,	//91	�߱�����

	KIND_ON_FANUP_C							,	//92	��޸��谡
	KIND_ON_BUDAKUP_C						,	//93	��޼�����
	KIND_ON_MUDOGAUP_C						,	//94	��޹�����
	KIND_ON_SWORDUP_C						,	//95	���ūĮ����
	KIND_ON_FIREGUNUP_C						,	//96	���ȭ����

	KIND_ON_GENERAL1_C						,	//97	�������
	KIND_ON_GENERAL2_C						,	//98	�̷�
	KIND_ON_GENERAL3_C						,	//99	��������
	KIND_ON_GENERAL4_C						,	//100	������
	KIND_ON_GENERAL5_C						,	//101	����
	KIND_ON_GENERAL6_C						,	//102	����

	// ���� 					
	KIND_ON_HARUBANG				= 107	,	//107	���Ϸ��
	KIND_ON_CRAB							,	//108	�ٴٰ�
	KIND_ON_LEECH							,	//109	�հŸӸ�
	KIND_ON_SEAHORSE						,	//110	�ظ� 
	KIND_WHITETIGER							,	//111	��ȣ 
	KIND_GHOSTCAT							,	//112	�͹� 
	KIND_TUTTLE								,	//113	�ٴٰź���
	KIND_BUTTERFLY							,	//114	������
	KIND_WIDOW								,	//115	�Ź̿䱫
	KIND_INSECT								,	//116	�縶��
	KIND_WORRIORGHOST						,	//117	������ȥ
	KIND_SINGERMGHOST						,	//118	�Ű���ȥ
	KIND_MAMMOTH							,	//119	���ڳ���
	KIND_WILDBOAR							,	//120	�����
	KIND_ON_THIEF							,	//121	������ 
	KIND_ON_SEAKING							,	//122	������ 
	KIND_ON_SWORDPIRATE						,	//123	�ֱ�
	KIND_ON_GUNPIRATE						,	//124	���ѿֱ�
	KIND_ON_SNOWWOMAN						,	//125	����
	KIND_ON_EATERDEMON						,	//126   �ı�
	KIND_ON_WEIRDPLANT						,	//127	�����
	KIND_ON_HUNGRYDEMON						,	//128	�Ʊ�
	KIND_ON_TIGER							,	//129	ȣ����
	KIND_ON_DOGDEMON						,	//130	�߽�
	KIND_ON_ONGHWA							,	//131	��õ������
	KIND_ON_BANDIT1							,	//132	��������
	KIND_ON_BANDIT2							,	//133	����
	KIND_ON_BANDIT3							,	//134	ö�����
	KIND_ON_BANDIT4							,	//135	�ü�����
	KIND_ON_GACKDAGUI						,	//136	���ٱ�
	KIND_ON_GATPHA							,	//137	����
	KIND_ON_BOGY							,	//138	������
	KIND_ON_FOX								,	//139	����ȣ
	KIND_ON_STARFISH						,	//140	�Ұ��縮
	KIND_ON_SHIP1							,	//141	��1
	KIND_ON_ONI								,	//142	����
	KIND_ON_SNOWDEMON						,	//143	����
	KIND_ON_FURHEADDEMON					,	//144	�а���
	KIND_ON_FIREDRAGON						,	//145	ȭ��
	KIND_ON_GOLDDRAGON						,	//146	Ȳ��
	KIND_ON_BAT								,	//147	����
	KIND_ON_TOAD							,	//148	�β���
	KIND_ON_CENTIPEDE						,	//149	������
	KIND_ON_DUEOKSINI						,	//150	�ξ�ô�
	KIND_ON_GANGSI							,	//151	����
	KIND_ON_RACCOONDOG						,	//152	�ʱ���
	KIND_ON_DEER							,	//153	�罿
	KIND_ON_BEAR							,	//154	�ݴް� 
	KIND_ON_HORSESPEAR						,	//155	�⸶â�� 
	KIND_ON_CAT								,	//156	���ϰ����
	KIND_ON_DUCK							,	//157	�븸����
	KIND_ON_HORNET							,	//158	�븸�չ�
	KIND_ON_MBANDIT1						,	//159	�����θ� 
	KIND_ON_MBANDIT2						,	//160	��ġ���� 
	KIND_ON_MBANDIT3						,	//161	�Ѽ����� 
	KIND_ON_CHAWOO							,	//162	�������� 
	KIND_ON_PIRATES							,	//163	���� 
	KIND_ON_PANDER							,	//164	�Ҵ� 
	KIND_ON_SNAKE							,	//165	������ 
	KIND_ON_CROCODILE						,	//166	���ξǾ� 
	KIND_ON_HOOK							,	//167	�������� 
	KIND_ON_SMALLGANG						,	//168	��������
	KIND_ON_HORSEMAN						,	//169	������
	KIND_ON_BABARIAN						,	//170	���ֹ�
	KIND_ON_BABARIANSHAMAN					,	//171	���ֹι���
	KIND_ON_BADDEER							,	//172	�絹��
	KIND_ON_CACTUS							,	//173	����������
	KIND_ON_SCORPION						,	//174	������ 
	KIND_ON_SKYNINE							,	//175	õ��
	KIND_ON_ICETREE							,	//176	��������
	KIND_ON_SNOWLIZARD						,	//177	��������
	KIND_ON_MADBULL							,	//178	����
	KIND_ON_BIGTREE							,	//179	�Ŵ��
	KIND_ON_MAGICIAN						,	//180	����� 
	KIND_ON_BADMERCHANT						,	//181	�Ǵ����� 
	KIND_ON_RUNNERHAND						,	//182	����������
	KIND_ON_SANDGOD							,	//183	���ǽ�
	KIND_ON_SEAENVOY						,	//184	�ٴ��ǻ��
	KIND_ON_VIPER							,	//185	�칫��
	KIND_ON_AMAKUSA							,	//186	�Ƹ����
	KIND_ON_LAVAMONSTER						,	//187	��ϱ���
	KIND_ON_EVIL							,	//188	�̻��� ���
	KIND_ON_BIRD							,	//189	�θ���
	KIND_ON_WEASEL							,	//190	������
	KIND_ON_COBRA							,	//191	�ں��
	KIND_ON_SOUL							,	//192	¡�⽺ĭ�� ȥ
	KIND_ON_VOLCANOFLAME					,	//193	ȭ���� �Ҳ�
	KIND_ON_REDMADBULL						,	//194	��������
	KIND_ON_MAGICOFFICER					,	//195	������
	KIND_ON_CAPBEAR							,	//196	����ݴް�
	KIND_ON_KOREANGHOST						,	//197	ó��ͽ�
	KIND_ON_SKELETON						,	//198	�ذ�
	KIND_ON_SKELETONSOLDIER					,	//199	�ذ񺴻�
	KIND_ON_PIRATECAPTAIN					,	//200	��������
	KIND_ON_TAIWANGHOST						,	//201	�븸�ͽ�
	KIND_ON_JAPANGHOST						,	//202	�Ϻ��ͽ�
	KIND_ON_AUTA							,	//203	�ƿ�Ÿ
	KIND_ON_JUCK							,	//204	������
	KIND_ON_FEMALESHAMAN					,	//205	����
	KIND_ON_QUEEN							,	//206	��������
	KIND_ON_DONKEY							,	//207	�糪��
	KIND_ON_AGILETIGER						,	//208	��ȣ
	KIND_ON_PAWNTIGER						,	//209	����ȣ����
	KIND_ON_OWL								,	//210	�û���
	KIND_ON_LEOPARD							,	//211	ǥ��
	KIND_ON_HUNSWARRIOR						,	//212	�������
	KIND_ON_HUNSHORSE						,	//213	���⸶��
	KIND_ON_HUNSHORSEARCHER					,	//214	����ú�
	KIND_ON_GATEKEEPER						,	//215	������
	KIND_ON_CLAYFIGURE						,	//216	�⸶������
	KIND_ON_PHOENIX							,	//217	�һ���
	KIND_ON_JINEMPEROR						,	//218	����Ȳ��ȥ
	KIND_ON_MEDICALMAN						,	//219	�Ǽ���
	KIND_ON_HEOJUN							,	//220	����
	KIND_ON_COSMIC							,	//221	�����
	KIND_ON_SEIMEI							,	//222	���̸���
	KIND_ON_YUMRUCK							,	//223	���»�
	KIND_ON_JANGSUNHWA						,	//224	�弱ȭ
	KIND_ON_LONGSPEAR						,	//225	��â����
	KIND_ON_KOAKHU							,	//226	����
	KIND_ON_SNOWMAN1						,	//227	�����1
	KIND_ON_SNOWMAN2						,	//228	�����2
	KIND_ON_SANTA1							,	//229	��¥��Ÿ1
	KIND_ON_SANTA2							,	//230	��¥��Ÿ2
	KIND_ON_SANTA3							,	//231	��¥��Ÿ3
	KIND_ON_MOTHER_DEER1					,	//232	�����絹��1
	KIND_ON_MOTHER_DEER2					,	//233	�����絹��2
	KIND_ON_MOTHER_DEER3					,	//234	�����絹��3
	KIND_ON_MOTHER_DEER4					,	//235	�����絹��4
	KIND_ON_SANTA4							,	//236	��¥��Ÿ 4
	KIND_ON_SANTA_TW						,	//237	�븸��¥��Ÿ
	KIND_ON_MOTHER_DEER_TW					,	//238	�븸�����絹��
	KIND_ON_SHIP2							,	//239	��2
	KIND_ON_SHIP3							,	//240	��3
	KIND_ON_SHIP4							,	//241	��4
	KIND_ON_SHIP5							,	//242	��5
	KIND_ON_SHIP6							,	//243	��6
	KIND_ON_SHIP7							,	//244	��7
	KIND_ON_SHIP8							,	//245	��8
	KIND_ON_FLYDRAGON						,	//246	�ͷ�
	KIND_ON_RAPTOR							,	//247	����
	KIND_ON_TIRANO							,	//248	Ƽ����츣��
	KIND_ON_MONKEY							,	//249	������

	// actdoll (2004/02/26 11:12) : �߰��� ���� - ����� �ʿ�					
	KIND_ON_HELLKING						,	//250	������
	KIND_ON_HELLMESSENGER					,	//251	���»���
	KIND_ON_GHOSTSAMURAI					,	//252	���� �繫����
	KIND_ON_PYTHON							,	//253	�̹���
	KIND_ON_GHOSTELEPHANT					,	//254	�����ڳ���
	KING_ON_GHOSTSEAKING					,	//255	����������
	KIND_ON_GHOSTPIRATECAPTAIN				,	//256	������������
	KIND_ON_GHOSTSKELETON					,	//257	�����ذ�
	KIND_ON_GHOSTSKELETONSOLDIER			,	//258	�����ذ񺴻�
	KIND_ON_GHOSTFEMALESHAMAN				,	//269	���ɹ���
	KIND_ON_GHOSTVOLCANOFLAME				,	//260	���ɺҲ�

	// ��ȣ�� �߰�ĳ����					
	KIND_ON_GUARDIAN_MOUSE					,	//261	��ȣ�ο�(��)
	KIND_ON_GUARDIAN_CAW1					,	//262	��ȣ�ο�(��1)
	KIND_ON_GUARDIAN_CAW2					,	//263	��ȣ�ο�(��2)
	KIND_ON_GUARDIAN_CAW3					,	//264	��ȣ�ο�(��3)

	KIND_ON_GUARDIAN_CHICK					,	//265	��ȣ�ο�(���Ƹ�)
	KIND_ON_GUARDIAN_PIG					,	//266	��ȣ�ο�(����)
	KIND_ON_GUARDIAN_SNAKE					,	//267	��ȣ�ο�(��)
	KIND_ON_GUARDIAN_MONKEY					,	//268	��ȣ�ο�(������)
	KIND_ON_JAP_INVADERS					,	//269	ħ���ֱ�

//	��� 2�� ����(2004-5-11, robypark)
	KIND_ON_TURTLE_VEHICLE					,	// 270	U102	����-�ź���
	KIND_ON_THUNDER_VEHICLE					,	// 271	U103	����-������
	KIND_ON_QUAKE_VEHICLE					,	// 272	U104	�Ϻ�-������
	KIND_ON_BLACK_DRAGON_VEHICLE			,	// 273	U105	�Ϻ�-�����
	KIND_ON_CATAPULT						,	// 274	U106	�߱�-�߼���
	KIND_ON_FARANGI_VEHICLE					,	// 275	U107	�߱�-�Ҷ�������
	KIND_ON_FIRE_DRAGON_VEHICLE				,	// 276	U108	�븸-ȭ����
	KIND_ON_CHINESEPHOENIX					,	// 277	U109	�븸-��Ȳ����

// 2�� ��� ��ų �߰� ����
	// robypark 2004/6/4 20:29
	// �Ҷ������� ��� ��õ��Ҹ� ���� ĳ����
	// �Ҷ��������� ������ ĳ���� �ε����� ����Ѵ�.(U107)
	// �Ҷ��������� �׸� �� ����� �ٸ���.
	KIND_ON_ROAR_OF_LAUGHTER				,	// 278

//#ifdef _SET_CHARACTER_LIST
	// robypark 2004/7/22 16:15
	// ��� �뺴 �߰�
	// �߱��뺴 ������(���� 20�̻��� �Ǹ� ��޿뺴�̹����� ����ȴ�.)
	KIND_ON_MAGIC_C_UP						,	// 279(U110)

	// robypark 2004/8/26 18:58
	// �븸 �뺴 �߼����� => ��� �߼�����
	KIND_ON_BEASTWARRIOR_UP					,	// 280(U111)
	// �븸 �뺴 ���»� => ��� ���»�
	KIND_ON_YUMRUCK_UP						,	// 281(U112)
	// �Ϻ� �뺴 - ����� => ��� �����
	KIND_ON_COSMIC_UP						,	// 282(U113)
	// ���� �뺴 - �Ǽ��� => ��� �Ǽ���
	KIND_ON_MEDICALMAN_UP					,	// 283(U114)
	// ���� �뺴 - Į���� => ��� Į����
	KIND_ON_BUTCHER_UP						,	// 284(U115)
	// �߱� �뺴 - ��â���� => ��� ��â����
	KIND_ON_LONGSPEAR_UP					,	// 285(U116)

//#endif

	// robypark 2004/9/17 13:35
	// ���� �� ������
	KIND_ON_RED_EYE_SEABEDMAN				,	// 286(M147)

	// ��¥ �������
	KIND_ON_HUNSWARRIOR_FAKE				,	// 287(M148)

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 16:19
	// ������ ���� ĳ���� �߰�
	// ������ - �˺�, U117
	KIND_ON_SIEGEWARFARE_SWORDMAN			,	// 288
	// ������ - â��, U118
	KIND_ON_SIEGEWARFARE_SPEARMAN			,	// 289
	// ������ - �⸶��, U119
	KIND_ON_SIEGEWARFARE_HORSEMAN			,	// 290
	// ������ - �ú�, U120
	KIND_ON_SIEGEWARFARE_ARCHERMAN			,	// 291
	// ������ - ����, U122
	KIND_ON_SIEGEWARFARE_SIEGE_TOWER		,	// 292
	// ������ - �߼���, U123
	KIND_ON_SIEGEWARFARE_CATAPULT			,	// 293
	// ������ - ���� ����, U124
	KIND_ON_SIEGEWARFARE_SUPPLY_WAGON		,	// 294
	// �� /////////////////////////////////////////////////////////////////

	// ���� /////////////////////////////////////////////////////////////////
	// robypark 2004/11/4 4:57
	// ��ȭ ���� �̺�Ʈ ����
	// ���Ȱ� �����, M149
	KIND_ON_JEOPALGE_TYPE1					,	// 295
	// ���Ȱ� ��ݺ�, M150
	KIND_ON_JEOPALGE_TYPE2					,	// 296
	// �� /////////////////////////////////////////////////////////////////

	// ������ - ����, U121
	KIND_ON_SIEGEWARFARE_GARRISON_ARMY		,	// 297

	// robypark 2004/11/22 18:6
	// ���Ȱ� ����ǳü����, M151
	KIND_ON_JEOPALGE_TYPE3					,	// 298

	// robypark 2005/1/14 17:40
	// ������ �ǳ��� �̹���Ȱ���� ���� ĳ���� �߰�
	KIND_ON_REFUGEE_KEON					,	// 299, �ǳ��� ��ö��
	KIND_ON_REFUGEE_MYUNGHWA				,	// 300, �ǳ��� �̸�ȭ
	KIND_ON_REFUGEE_RYO						,	// 301, �ǳ��� �ø��ٷ�
	KIND_ON_REFUGEE_HANAHINE				,	// 302, �ǳ��� �ճ��ϳ�����
	KIND_ON_REFUGEE_SORYUNG					,	// 303, �ǳ��� �ռҷ�
	KIND_ON_REFUGEE_MIRYUNG					,	// 304, �ǳ��� �չ̷�
	KIND_ON_REFUGEE_DANG					,	// 305, �ǳ��� ��ȣ��
	KIND_ON_REFUGEE_SUYOUN					,	// 306, �ǳ��� �����

	KIND_ON_MAX									//		�� ĳ������ ���� ����(���� ������ �ƴϴ�. �̰� ������ ĳ���͸� �߰��ϸ� ���� �ȵȴ�!!!!)
};						

//-----------------------------------------------------------------------------
// Name: CLASS_KIND_ON_CHARACTER
// Code: actdoll (2004-01-01)
// Desc: ������ ���� ��Ʈ�� ���ڸ� �����ϴ� Ŭ������.
//-----------------------------------------------------------------------------
class CLASS_KIND_ON_CHARACTER
{
private:
	struct DATA
	{
		char	m_pszString[48];
	};
	static DATA	m_Data[KIND_ON_MAX];

private:
	static	void	SetData( int index, char *pszStr );

public:
	static	ENUM_KIND_ON_CHARACTER	GetData		( char *pszStr );
	static	const char*				GetData		( ENUM_KIND_ON_CHARACTER enIndex );
	static	int						GetMaxCount	( void )	{ return KIND_ON_MAX; }

	CLASS_KIND_ON_CHARACTER();
	~CLASS_KIND_ON_CHARACTER();
};

#endif//__ENUM_KI_CHARACTER_H__