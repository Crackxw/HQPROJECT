//==================================================================================================================================================
//	FileName	:	bullet.h
//	Discription	:	�Ѿ� ����
//	History		:	2002/02/16(�ڿ���) - Modify
//==================================================================================================================================================

#ifndef _BULLET_H
#define _BULLET_H

class	cltOnlineWorld;

#define MAX_SHOOT_RANGE							200     

//------------------------------------------------------------------------------------------------
// �Ѿ��� �۵� 
//------------------------------------------------------------------------------------------------
#define ORDER_BULLET_MOVE						1
#define ORDER_BULLET_EXP     					2
#define ORDER_BULLET_BLOCK   					3
#define ORDER_BULLET_RETURN  					4
#define ORDER_BULLET_SEARCH  					5
#define ORDER_BULLET_CHASE   					6
#define ORDER_BULLET_WAITEXP 					7
#define ORDER_BULLET_DARKFIRE 					8
#define ORDER_BULLET_FIREENERGY					9
#define ORDER_BULLET_WAITEXP4					10

//------------------------------------------------------------------------------------------------
// �Ѿ��� �۵� �߰� : robypark(2004/5/19, 12:31)
//------------------------------------------------------------------------------------------------
// �Ǿ��(����)�Ǵ� �Ѿ� �۵�(�� �� ���� ����)
// �Ѿ� �̵� �� �Ѿ��� �ִ� ���� ���� ���ֿ� �������� �ش�.
// �Ͱ�ȭ���ĸ� ���� �Ѿ� �̵�
#define ORDER_BULLET_PIERCE						11

// �������� ���� �� �������� �̵��ϴ� �Ѿ� �۵�
// �Ѿ� �̵� �� �Ѿ��� �ִ� ���� ���� ���ֿ� �������� �ش�.
// �������� �� ���� �Ѿ��� �Ҹ��Ѵ�.
// ����(����-������))�� ���� �Ѿ� �̵�
#define ORDER_BULLET_STRAIGHT					12

// ź���Ǿ��� �� Ư������� �����Ѵ�.
// ��õ��(�߱�-�߼���)�� ���� �۵�
#define ORDER_BULLET_EVENT_IMPACT				13

//------------------------------------------------------------------------------------------------
// �Ѿ��� �ִϸ��̼� ����. 
//------------------------------------------------------------------------------------------------
#define ANI_BULLET_MOVE							1
#define ANI_BULLET_WAIT							2




//------------------------------------------------------------------------------------------------
// �Ѿ� ���� 
//------------------------------------------------------------------------------------------------
#define KIND_BULLET_AIRMISSILE       			1		// ���߿� �̻��� 
#define KIND_BULLET_SAMMISSILE       			2		// õ�� �̻��� 
#define KIND_BULLET_ARROW            			3		// �Ϲ� ȭ��
#define KIND_BULLET_FIREARROW        			4		// ��ȭ��
#define KIND_BULLET_SHIPCANNON       			5		// ���� 
#define KIND_BULLET_THROWBOMB        			6		// ������ ��ź 
#define KIND_BULLET_FIRE              			7		// ȭ�� ��簡�� �� 
#define KIND_BULLET_BOMB              			8		// ��ź
#define KIND_BULLET_FIRETANKARROW     			9		// ȭ���� �ű���
#define KIND_BULLET_HEAVYGUN          			10		// ����� 
#define KIND_BULLET_MAGICBALL1        			11		// ������ ���� ���� 
#define KIND_BULLET_MAGICBALL2        			12		// ������ ���� ���� 
#define KIND_BULLET_BUDAFIRE          			13		// �º��� ���� ���� 
#define KIND_BULLET_BUDAFIRE2         			14		// ���� �º��� ���� ���� 
#define KIND_BULLET_BUDAFIRE3         			15		// �Ϻ� ������ ���� ���� 
#define KIND_BULLET_CANNON            			16 
#define KIND_BULLET_AIRMAGIC          			17

// robypark 2004/7/14 16:44
#define KIND_BULLET_DOLL_EVIS					18		// ���� ���� ���� �Ѿ�

#define KIND_BULLET_GUN               			19
#define KIND_BULLET_GUNPOISON              		20
#define KIND_BULLET_STRONGGUN         			21
#define KIND_BULLET_MAGIC6C			  			22
#define KIND_BULLET_FRAGMENT1         			23
#define KIND_BULLET_FRAGMENT2         			24
#define KIND_BULLET_FRAGMENT3         			25
#define KIND_BULLET_NEEDLE            			26
#define KIND_BULLET_GROUNDEXP         			27		// ������ 
#define KIND_BULLET_DART			  			28		// �Ϻ� ������ ������ 
#define KIND_BULLET_MAGIC8K			  			29		// ���� ��ù��� ����������
#define KIND_BULLET_BURNARROW		  			30  
#define KIND_BULLET_MAGIC5J			  			31		// ������ ����������  
#define KIND_BULLET_VOLCANO						32		// ȭ��ź 
#define KIND_BULLET_UPGRADETOWERCANNON			33		// ���׷��̵�� ȭ�� ������ ��ź
#define KIND_BULLET_AIRATTACKUPARROW			34		// ��������
#define KIND_BULLET_GATPHAPOISON				35		// ������ ���� ������
#define KIND_BULLET_FURHEADDEMON       			36		// �а����� ���� 
#define	KIND_BULLET_GOLDDRAGONELECTRON			37      // �ݷ� ����. 
#define	KIND_BULLET_ONGHWA						38      // ��õ ������ �ذ�.  
#define	KIND_BULLET_GHOSTCAT					39		// �͹� ���� ����. 
#define	KIND_BULLET_DOLL1						40		// ���� ����. 
#define	KIND_BULLET_DOLL2						41		// ��ī�� ����. 
#define KIND_BULLET_JUSTICEK        			42		// ���� ���� ����
#define KIND_BULLET_BUTTERFLY        			43		// ������
#define KIND_BULLET_WIDOW	        			44		// �Ź̿䱫.
#define KIND_BULLET_SEAHORSE	       			45		// �ظ� �����.
#define KIND_BULLET_POISONARROW	       			46		// ��ħ      (���ֹι���) 
#define KIND_BULLET_THROWSPEAR	       			47		// ������ â (���ֹ�) 
#define KIND_BULLET_SCORPIONBULLET				48		// ������ ��. 
#define KIND_BULLET_CACTUSBULLET				49		// ���������� ����. 
#define KIND_BULLET_MAGICIANBULLET				50		// ����� ��ǳ.
#define KIND_BULLET_VOLCANOFLAME				51		// ȭ���� �Ҳ�.
#define KIND_BULLET_EVIL						52		// �̻��� ���,�ƻ���(��ǳ)
#define KIND_BULLET_ICE							53		// �̻��� ��� ���� ����.
#define KIND_BULLET_DARKFIRE					54		// �������� ����ȭ����ǳ
#define KIND_BULLET_GHOST						55		// �丣�������� �ͽ�ź
#define KIND_BULLET_JAPANGHOST					56		// �Ϻ��ͽ��� �������
#define KIND_BULLET_KUKET						57		// ����
#define KIND_BULLET_AUTA						58		// �ƿ�Ÿ ����.
#define KIND_BULLET_QUEENSUB1					59		// �������� ȭ�����1
#define KIND_BULLET_QUEENSUB2					60		// �������� ȭ�����2
#define KIND_BULLET_FIREATTACK					61		// �̷� ȭ������
#define KIND_BULLET_RUNNINGFIRE					62		// �̷� ������ź
#define KIND_BULLET_ENERGYPA					63		// ��ǳ
#define KIND_BULLET_FIREENERGYPA				64		// ��ȭ��ǳ(���Ȳ���)
#define KIND_BULLET_FIREENERGYPASTART			65		// ��ȭ��ǳ(���Ȳ���) ����
#define KIND_BULLET_PAPERDOLL					66		// ����� ��������
#define KIND_BULLET_NEEDLEATTACK				67		// ħ����(�Ǽ���, ����)
#define KIND_BULLET_CLOUDWIND					68		// ���»� ǳ���
#define KIND_BULLET_GODSPEAR					69		// ���� ��â�뵵
#define KIND_BULLET_SOULCHANGE					70		// ���Ǽ�
#define KIND_BULLET_MAGICCATTACK				71		// ������ ����
#define KIND_BULLET_SUNBIYOUNATTACK				72		// ���� ����
#define KIND_BULLET_SNOW						73		// �� 

// actdoll (2004/03/06 4:38) : �����/����Ȳ�� ���� �߰�
#define KIND_BULLET_PYTHON						74		// �̹��� ��������	// actdoll (2004/03/06 4:38) : �߰�
#define KIND_BULLET_JINEMPEROR					75		// �̹��� ��������	// actdoll (2004/03/06 4:38) : �߰�

#define KIND_BULLET_GUARDIAN_TIGER				76		// ��ȣ��(ȣ����)
#define KIND_BULLET_GUARDIAN_RABBIT				77		// ��ȣ��(�䳢)

#define KIND_BULLET_GUARDIAN_CHICK				78		// ��ȣ��(��)

//----------------------------------------------------------------------------
// robypark (2004/05/18 14:53) : �߰� ĳ���� ��� bullet - ��� 2�� ���� ��� bullet
//----------------------------------------------------------------------------
#define KIND_BULLET_TURTLE_FIREBALL				79		// ����-�ź���; ���̾� ��
#define KIND_BULLET_THUNDER_BOLT				80		// ����-������; ���� ��Ʈ
#define KIND_BULLET_POISON_ICE_BULLET			81		// �Ϻ�-�����; ��, ���� ����
#define KIND_BULLET_CATAPULT_FIRE_STONE			82		// �߱�-�߼���; �Һ��� ������
#define KIND_BULLET_FARANGI_BULLET				83		// �߱�-�Ҷ�������; �Ҷ�����ź
#define KIND_BULLET_FIRE_DRAGON_MISSILE			84		// �븸-ȭ����; ȭ�����
#define KIND_BULLET_CHINESEPHOENIX_FEATHER		85		// �븸-��Ȳ����; ��Ȳ���� ���� �̻���

#define KIND_BULLET_CATDOLL						86
#define KIND_BULLET_OLDFOXDOLL					87
#define	KIND_BULLET_RACCOONDOLL					88
#define KIND_BULLET_NINJADOLL					89
#define KIND_BULLET_SNOWWOMANDOLL				90
#define KIND_BULLET_YANGGUIBIDOLL				91
#define KIND_BULLET_JIJANGDOLL					92
#define KIND_BULLET_SNAKEWOMANDOLL				93

// robypark 2004-6-1 14:12
#define KIND_BULLET_BOMB_OF_HEAVEN				94	// �߼���, ��õ�� �� ���� ��ź
#define KIND_BULLET_VOLCANO_BULLET				95	// ������, �ݳ뿰�� ȭ��ź

// robypark 2004-6-3 15:39
#define KIND_BULLET_ONSLAUGHT_FIREBALL			96	// �븸-ȭ����; �Ͱ�ȭ���� ��ź

// robypark 2004-6-4 19:12
#define KIND_BULLET_ROAR_OF_LAUGHTER_BOMB		97	// �Ҷ������� ��� ��õ��� �Ѿ�
#define KIND_BULLET_FRAGMENT4					98	// �Ҷ������� ��� ��õ��� ���ݽ� ����

// robypark 2004/10/22 11:2
// ������ �ʵ� �Ѿ�
#define KIND_BULLET_SIEGEWARFARE_ARROW			99	// ������ - �ú� ȭ��
#define KIND_BULLET_SIEGEWARFARE_STONE			100	// ������ - �߼��� ��

// robypark 2005/2/2 17:27
#define KIND_BULLET_DOLL_DHARMA					101 // �޸� ���� ���� �Ѿ�

#define MAX_BULLET_KIND_NUMBER       			128


// ź ���� ���� 			
#define COURSE_DIRECT							1		// ����ȭ�� 
#define COURSE_SLOPE							2		// ������ ȭ�� 

#define MAX_ICE_TYPE							4		// ���� ������ ����

#define MAX_HIT_ID								25		// 5*5 ����� ���Ͽ� , 
														// ���ݿ� ���Ͽ� ���ظ� �Դ� ĳ������ ���� ������ 

//--------------------------------------------------------------------------------------------------------------------
// �Ѿ� �Ӽ� 
#define BULLETATB_HITNORMAL					0x00000001 
#define BULLETATB_RANGE1					0x00000002 
#define BULLETATB_WATER						0x00000004
#define BULLETATB_ARROW						0x00000008
#define BULLETATB_NEEDLE					0x00000010
#define BULLETATB_GUN						0x00000020
#define BULLETATB_STRONGGUN					0x00000040
#define BULLETATB_MAGIC1					0x00000080
#define BULLETATB_MAGIC2					0x00000100
#define BULLETATB_MAGIC3					0x00000200
#define BULLETATB_FIRE						0x00000400
#define BULLETATB_AIRMISSILE				0x00000800
#define BULLETATB_RUNNINGFIRE				0x00001000   // ������ź.
#define BULLETATB_FIREENERGY				0x00002000
#define BULLETATB_CANNON					0x00004000
#define BULLETATB_CANNON2C	  				0x00008000
#define BULLETATB_MAGIC6C	  				0x00010000
#define BULLETATB_SOULCHANGE				0x00020000
#define BULLETATB_FIREARROW					0x00040000
#define BULLETATB_MAKEFIRE					0x00080000
#define BULLETATB_FIREATTACK				0x00100000 //  ȭ������.
#define BULLETATB_UPGRADETOWERCANNON		0x00200000
#define BULLETATB_AIRATTACKUPARROW			0x00400000
#define BULLETATB_FURHEADDEMON				0x00800000
#define BULLETATB_GOLDDRAGONELECTRON		0x01000000
#define BULLETATB_ONGHWA					0x02000000
#define BULLETATB_HALT						0x04000000
#define BULLETATB_FRAGMENT					0x08000000
#define BULLETATB_BUTTERFLY					0x10000000
#define BULLETATB_WIDOW						0x20000000
#define BULLETATB_SEAHORSE					0x40000000
#define BULLETATB_POISONARROW				0x80000000

//#define BULLET_SPEED_ARROW	30		
//#define BULLET_SPEED_GUN	40		

// �Ѿ˿� ������ ������ �Ӽ�. 
#define BULLETSMOKEATB_SMOKE_EXP0					0x00000001
#define BULLETSMOKEATB_SMOKE_EXP6					0x00000002
#define BULLETSMOKEATB_SMOKE_EXP7					0x00000004
#define BULLETSMOKEATB_SMOKE_EXP9					0x00000008
#define BULLETSMOKEATB_SMOKE_EXP10					0x00000010
#define BULLETSMOKEATB_SMOKE_ADVTOWERKEXP1			0x00000020
#define BULLETSMOKEATB_SMOKE_EXP12					0x00000040
#define BULLETSMOKEATB_SMOKE_EXP14					0x00000080
#define BULLETSMOKEATB_SMOKE_EXP15					0x00000100
#define BULLETSMOKEATB_SMOKE_GHOSTCATEXP			0x00000200
#define BULLETSMOKEATB_SMOKE_MAGIC8K_EXP			0x00000400
#define BULLETSMOKEATB_SMOKE_GOLDDRAGONELECTRONEXP	0x00000800
#define BULLETSMOKEATB_TRAIL_ARROWTRAIL				0x00001000
#define BULLETSMOKEATB_SMOKE_3						0x00002000
#define BULLETSMOKEATB_SMOKE_GHOST					0x00004000
#define BULLETSMOKEATB_SMOKE_PAPERDOLL				0x00008000
#define BULLETSMOKEATB_SMOKE_SOULCHANGE				0x00010000
#define BULLETSMOKEATB_SMOKE_JINEMPEROR				0x00020000			// actdoll (2004/03/06 9:54) : �����/����Ȳ�� ����Ȳ��ȥ
#define	BULLETSMOKEATB_SMOKE_GUARDIAN_RABBIT		0x00040000
#define BULLETSMOKEATB_SMOKE_GAURDIAN_TIGER			0x00080000
#define BULLETSMOKEATB_SMOKE_GUARDIAN_CHICK			0x00100000

//----------------------------------------------------------------------------
// robypark (2004/05/18 15:40) : �߰� ĳ���� ��� bullet smoke - ��� 2�� ���� ��� bullet smoke
//----------------------------------------------------------------------------
#define BULLETSMOKEATB_SMOKE_TURTLE_FIREBALL		0x00200000		// ����-�ź���; ���̾� ��
#define BULLETSMOKEATB_SMOKE_THUNDER_BOLT			0x00400000		// ����-������; ���� ��Ʈ
#define BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET		0x00800000		// �Ϻ�-�����; ��, ���� ����
#define BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE	0x01000000		// �߱�-�߼���; �Һ��� ������
#define BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE	0x02000000		// �븸-ȭ����; ȭ�����
#define BULLETSMOKEATB_SMOKE_CHINESEPHOENIX_FEATHER	0x04000000		// �븸-��Ȳ����; ��Ȳ���� ���� �̻���
#define BULLETSMOKEATB_SMOKE_VOLCANO_BULLET			0x08000000		// �Ϻ�-������; �ݳ뿰�� ������ ȭ��ź ��Ʈ ȿ��

// robypark 2004/6/4 19:17
#define BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB	0x10000000		// �߱�-�Ҷ������� ��� ��õ��� ����ȿ�� ���

// robypark 2004/11/24 17:40
#define BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT	0x20000000	// ������ �߼��� �� ��Ʈ ȿ�� ���

// robypark 2004/12/6 13:38
#define BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT				0x40000000	// ������ �ú� ȭ�� ��Ʈ ȿ�� ���
//--------------------------------------------------------------------------------------------------------------------

// ���ο� Bullet�߰��� HQ�ȿ� ���� InfoBullet.txt�� �߰����ָ�, �⺻���� ������ ���´�.

// bullet.cpp���� ����� Class�� bullet.h�� �ҽ� �̵�
// �Ѿ� ���� ���� Ŭ����
class _BulletKindInfo
{
public:
	// robypark 2004/9/30 18:28
	// Smoke ������ HQ���� InfoBullet.txt���� ���� ����Ѵ�.
	void Init(char *pszKindID, SI32 BulletAtb, SI32 BulletSmokeAtb);

	SHORT m_ssStepWidth;
	SHORT m_ssFile, m_ssFont;
	SI32  m_siMoveFrameNumber;
	
	// ���⸶�� �ٸ� �׸����� �����ϴ°�(TRUE), �Ѱ��� �׸��� �����ϴ°� ?(FALSE)
	BOOL m_bDirSwitch;                          

	SHORT m_ssTailSwitch, m_ssChaseSwitch, m_ssCurveSwitch, m_ssTransparentSwitch;
	SHORT m_ssHeightSwitch;  // �������� ���� ���ݷ� ���� 

	SI32 m_siAtb;
	SI32 m_siSmokeAtb;
	SI32 m_siHitEffectSoundIdx;			// �Ѿ� ���� ȿ����. Hash Table �̿��� �� ��

	SI32 m_siDelay;
	
};

class _Bullet
{
public:
	SHORT m_ssAnimation;									// � ������ �ִϸ��̼��� �����ϴ°�?
	SHORT m_ssDrawSwitch;									// �׷��� ���ΰ� �����ΰ�?
	SHORT m_ssReverseSwitch;                // ����� ���� ���ΰ�?
	SHORT m_ssTransparentSwitch;
	SHORT m_ssHeightSwitch;
	SHORT  m_ssDrawStartX, m_ssDrawStartY;      // �׸��� �׷����� ���� ��ġ 
	SHORT  m_ssDrawXsize,  m_ssDrawYsize;       // �׸��� ���� ���� ������ 
	SHORT m_ssFile, m_ssFont;
	SHORT m_ssAniStep;
	char m_scRealImageXPercent, m_scRealImageYPercent;  // ���� �̹����� ���� ���η� �� �� %�� ���ϴ°�?
	SHORT m_ssRealImageStartX, m_ssRealImageEndX;        // ���� �̹����� ���ϴ� ���� ���� ��ǥ 
	SHORT m_ssRealImageStartY, m_ssRealImageEndY; 

	SHORT  m_ssCurrentOrder;               // ���� ĳ���Ͱ� �ϰ� �ִ� �۾� 

	SHORT  m_ssID;                          // ĳ������ �ĺ� ��ȣ 
	SHORT m_ssKind;                        // � ������ obj�ΰ�?
	SHORT m_ssCenterX, m_ssCenterY;

	SHORT m_ssStepWidth;                    // �̵� ����

	char m_scArea;                        // ���߿� �ִ°� �ƴϸ� ���� �ִ°�?
	int m_siTargetDirection;               // ��ǥ�� ���� 
	int m_siTempDirection;                // 16������ �����ϴ°�?

	SHORT  m_ssX, m_ssY;                        // obj�� �߽� ��ġ 

	// �̵����� 
	char m_scMoveFrameNumber;
    SHORT m_ssMoveFile[9], m_ssMoveFont[9];

	SHORT m_ssRealX, m_ssRealY;
	SHORT m_ssRealDX, m_ssRealDY;
	SHORT m_ssMapSX, m_ssMapSY;
	SHORT m_ssMapDX, m_ssMapDY;
	SHORT m_ssOriginalRealDX, m_ssOriginalRealDY;

	SHORT m_ssCurveBulletRealX, m_ssCurveBulletRealY;
	SHORT m_ssOldCurveBulletRealX, m_ssOldCurveBulletRealY;
	SHORT m_ssCurveBulletDrawX, m_ssCurveBulletDrawY;
	SHORT m_ssCurveSwitch;

	SHORT m_ssCurveRate;					// �󸶳� ��� �̷�°�?

	_NewID m_AttackID;                    // ������ ��� ���̵� 
	SHORT m_ssApower;

	_NewID m_ShooterID;
	SHORT m_ssShooterPlayer;

	SHORT m_ssMoveStep;
	SHORT m_ssMaxMoveStep;

	DWORD m_uiSkillTime;

	SHORT m_ssType;
    SHORT m_ssHitIDIndex;
    _NewID m_HitID[MAX_HIT_ID];

	SHORT m_ssTailSwitch;					// ������ ��������� ���θ� �˷��ִ� ���� 
	SHORT m_ssChaseSwitch;					// ������ ���ΰ� �����ΰ�?
	SHORT m_ssChaseRepeatNumber;
	SHORT m_ssVaryX, m_ssVaryY;


	SHORT m_ssCourseX[MAX_SHOOT_RANGE],m_ssCourseY[MAX_SHOOT_RANGE];

	SHORT m_ssBallSize;

	SI32 m_ssHitRate;						// �Ѿ��� ������ Ȯ��. 

	// robypark (2004/6/12 12:27
	// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
	// �������ش�.
	bool	m_bLayerAboveVolcano;

	// robypark (2004/5/21 18:44) ȭ�����, �߼��� �� ���� �� �ִϸ��̼� ������ ���� �߰�
	//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	//		���� ������ ��ŵ�� �ʿ��ϴ�.
	// �̵� �ܰ迡 ���� ������ ��ȣ�� �����ϰ� �ִ�.
	SHORT		m_snFrameIndex[MAX_SHOOT_RANGE];

	// robypark (2004/5/27 18:49)
	// �Ҷ��������� 3�� ��ź�� ���� �߻縦 ���� ����
	SHORT		m_snCurrentDelay;	// ���� ��ü�� �ð�
	SHORT		m_snMaxDelay;		// ��ü�� �ð�

	protected:
		// robypark 2004/6/4 19:25
		// �Ҷ������� ��� ��õ��� ��ź ���߽� ������ �������ش�.
		void CreateFragment(void);

	public:
		_Bullet();
	  void Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooter_id, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetidir, SHORT attack_area);

	// robypark (2004/5/21 18:44) ȭ����� �ִϸ��̼� ������ ���� �߰�
	//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	//		���� ������ ��ŵ�� �ʿ��ϴ�.
	//  �̵� �ܰ迡 ���� �ִϸ��̼� �������� �����Ѵ�.
	void CreateFrameIndex_4FireDragonMissile(void);

	// robypark (2004/5/27 19:9)
	// �Ҷ��������� 3�� ��ź ���� ����
	void SetLuanchDelay(SHORT snMaxDelay);

	// robypark (2004/6/12 12:27
	// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
	// �������ش�.
	void SetLayerAboveVolcano(bool bAbove = true);

      // �ѹ� ������ ĳ���͸� �ٽ� �������� �ʱ� ���ؼ�.
      // ������ �ʱ�ȭ�Ѵ�. 
      void InitPushID();
	  void PushHitID(_NewID id);
	  BOOL IsAlreadyPushID(_NewID id);
	  // ���� ��ġ�� ����Ѵ�.
	  void CalculateDrawStartXY();

      void CalculateRealXY();
      // ���� �� �׸��� �����Ѵ�. 
      void DecideFont();
	  void DecideFontMove();
	  void DecideFontMoveNormal();
	  void DecideFontMoveFireTankArrow();

	  void SetMoveImage16(SHORT file, SHORT font, SHORT frame);
	  void SetMoveImage(SHORT ani, SHORT file, SHORT font);

	  void GetCourse();
	  void GetCourseCurve(SHORT startstep);
	  void GetCourseCurve();
	  BOOL Draw();
	  BOOL Action();
	  BOOL Hit();
      BOOL HitNormal();

	  BOOL HitFireArrow();
	  BOOL HitNeedle();
	  BOOL HitGun();
	  BOOL HitStrongGun();
	  BOOL HitAirMissile();
	  BOOL HitAttackerC();
	  BOOL HitSamMissile();
	  BOOL HitAirMagic();
	  BOOL HitCannon();    
	  BOOL HitHeavyGun();  
	  BOOL HitBomb();      
	  BOOL HitFire();      
	  BOOL HitShipCannon();
	  BOOL HitThrowBomb(); 
	  BOOL HitFireTankArrow();
	  BOOL HitMagicBall(); 
	  BOOL HitFragment1(); 

	  void SetFileNFont();

	  int GetArea(){return m_scArea;}
	  void SetInfo();
	  void DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	  void DecideDirectionFireTankArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
	  void DecideDirectionNormal(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
  
};


SHORT FindEmptyBullet();
void DeleteBullet();
void DeleteBullet(SHORT id);

SHORT SetBullet(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooterid, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetdir, char attack_area);

void InitBulletKindInfo();

// robypark 2004/6/12 12:38
extern _Bullet g_Bullet[MAX_BULLET_NUMBER]; // �Ѿ˿� ������ 

// �Ѿ� ���� ���� ���� ���� ����
extern _BulletKindInfo g_BKI[MAX_BULLET_KIND_NUMBER];

//#endif

// robypark (2004/5/27 19:11)
// �Ҷ������� 3�߻� ���� �߻� ���� �Լ�
void Bullet_SetLuanchDelay(int i, SHORT snMaxDelay);

BOOL Bullet_Action(int i);
// 2001.11.12	hojae_append
BOOL Bullet_Action(int i);
void Bullet_CalculateDrawStartXY(int i);
void Bullet_Draw(int i);
void Bullet_DrawSwitch(SHORT bulletid, BOOL TorF);
void Bullet_CurrentOrder(SHORT bulletid, SHORT DEF);
void Bullet_Fwrite(FILE* fp);
void Bullet_Read(FILE* fp);

// robypark (2004/6/12 12:27
// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
// �������ش�.
void SetLayerAboveVolcano(SI32 siBulletID, bool bAbove = true);

#endif
///////////////////////////////////////////////