#ifndef _CHARORG_H
#define _CHARORG_H

#include <GSL.h>

#include <CharAnimation.h>
#include "../SCCommon/ParametaBox.h"
#include "../SCCommon/OnlineMyWeapons.h"

#include "..\Gersang\order.h"

#include <Main.h>
#include <kindinfo.h>
#include "../GCommon/ElectronShock/ElectronShock.h"
#include "../GCommon/CharDraw/CharDrawInfo.h"
#include "../GCommon/PoisonEffect/PoisonEffect.h"

#define MAX_BUFFER_ORDER_NUMBER 10   // ����� ������ �� �� �ִ� �ִ� ���� 

#define ON_GUARDIAN_MOUSE			1
#define ON_GUARDIAN_CAW				2
#define ON_GUARDIAN_TIGER			3
#define ON_GUARDIAN_RABBIT			4
#define ON_GUARDIAN_DRAGON			5
#define ON_GUARDIAN_SNAKE			6
#define ON_GUARDIAN_HORSE			7
#define ON_GUARDIAN_SHEEP			8
#define ON_GUARDIAN_MONKEY			9
#define ON_GUARDIAN_CHICKEN			10
#define ON_GUARDIAN_DOG				11
#define ON_GUARDIAN_PIG				12

#define ON_GUARDIAN_GAME			1			//���� �߿� �¾�����.
#define ON_GUARDIAN_START			2			//������ ���۵��� ����.

#define CHAR_DYING					0
#define CHAR_GENERALEX				1
#define CHAR_DEAD					2

// ĳ���Ͱ� ����� ����. 
enum HaltReason {	HALT_REASON_ICE = 1, 
					HALT_REASON_IVY = 2,

					// robypark 2004-6-3 16:15
					// õ������ �ñ⿡ ���� ���� smoke, ����� �⺻ ���ݿ� ���� ���� smoke
					HALT_REASON_FOG_OF_ICE = 3
				};

// ĳ���� �̵��� ���� ���ϰ�. 
enum MoveInfo {	MOVEINFO_MOVING  =  0, /* �̵����� ������ �ִ�. */
				MOVEINFO_MOVEDONE = 1, // �̵��� �Ϸ�Ǿ���. 
				MOVEINFO_HALFDONE = 2, // �߰� �������� �����ߴ�. 
				MOVEINFO_REALMOVE = 3};// ������ ������ �̵������� �˷��ش�.

#define MAX_FLY_VIBRATE     8


// ĳ������ ���� ����. 
class cltCharStatus{
private:
	//-----------------
	// ĳ���� ���� ����. 
	//-----------------
	SI32	m_siKind;			         // � ������ obj�ΰ�?
	
	//-------------------
	// �÷��̾� �ĺ� ����. 
	//-------------------
	SI32	m_siPlayerNumber;                // ��� ���ΰ�?
	SI32	m_siOriginalPlayerNumber;        // ������ ��ÿ� ��� �÷��̾� �Ҽ��̾��°�?
	DWORD	m_uiChangedPlayerFrame;         // �÷��̾ ����� ������     

	//-------------------
	// ��ġ ���� 
	//-------------------
	etArea 	m_eArea;                        // ���߿� �ִ°� �ƴϸ� ���� �ִ°�?
	SHORT 	m_ssCenterX, m_ssCenterY;            // ĳ���� �̹����� �߽� ��ǥ (������ǥ) 


	//------------------
	// �������� 
	//------------------

	SI32 			m_siWorkDelay;
	SI32 			m_siWorkDelayor;

public:

	// ������ �����ش�.
	SI32 GetKind()const{return m_siKind;};
	void SetKind(SI32 kind){m_siKind = kind;}

	SI32 GetPlayerNumber()const{return m_siPlayerNumber;}
	void SetPlayerNumber(SI32 pnumber){m_siPlayerNumber = pnumber;}

	SI32 GetOriginalPlayerNumber()const{return m_siOriginalPlayerNumber;}
	void SetOriginalPlayerNumber(SI32 pnumber){m_siOriginalPlayerNumber = pnumber;}

	DWORD GetChangedPlayerFrame()const{return m_uiChangedPlayerFrame;}
	void SetChangedPlayerFrame(DWORD frame){m_uiChangedPlayerFrame = frame;}

	etArea GetArea()const{return m_eArea;}
	void   SetArea(etArea area){m_eArea = area;}

	void SetCenterX(SHORT centerx){m_ssCenterX = centerx;}
	SHORT GetCenterX()const{return m_ssCenterX;}

	void SetCenterY(SHORT centery){m_ssCenterY = centery;}
	SHORT GetCenterY()const{return m_ssCenterY;}

	void SetWorkDelay(SI32 workdelay){m_siWorkDelay = workdelay;}
	SI32 GetWorkDelay()const{return m_siWorkDelay;}

	void SetWorkDelayor(SI32 delayor){m_siWorkDelayor = delayor;}
	SI32 GetWorkDelayor()const{return m_siWorkDelayor;}

};


class cltCharOrg{

protected:
	SI32			m_siUpdateInfo;
	SI32			m_siOldUpdateInfo;

	char 			m_scSelectedSwitch;               // ���õǾ� �ִ��� ���θ� �˷��ִ� ���� 

	// robypark 2004/6/14 15:35
	// �ױ� ���� �� ĳ���Ͱ� ������ �Ǿ� �־����� ��Ÿ���� �÷���
	BOOL			m_bAlreadySelectedBeforeDead;

	// �ǰݽ� �ð� ȿ�� ���� 
	BOOL			m_bBeAttackedDrawSwitch;
	SI32 			m_siBeAttackedDrawStep;

	// ���� ����. 
	SI32			m_siWeaponAttackDelay;
	SI32 			m_siAttackDelayor;


	SI32 			m_siSearchRange;	// �þ�

	char 			m_szName[32];		// ĳ���� ��

	SI08 			m_scSeeMode[MAX_MULTI_PLAYER];
	SI08 			m_scSeeStep[MAX_MULTI_PLAYER];
	SI32 			m_siDarkStep;		// Not Used
	SI32 			m_siTimerDelay;		// �� Ÿ�̸Ӱ� �۵����̸� ĳ���ʹ� ���Ѻ� �λ��̵ȴ�. 


public:

	// ���� ���� ���� 
	cltElectronShock m_cltElectronShock;

	// �ߵ� ���� 
	cltPoisonEffect  m_cltPoisonEffect;

	// ĳ���� ��°� ���õ� ����. 
	cltCharDrawInfo  m_cltCharDrawInfo;

	// ĳ���� ���� ����. 
	cltCharStatus	m_cltCharStatus;

	_ParametaBox	m_ParmeterBox;				  // ĳ������ �ൿ�� �����ϴ� �Ķ��Ÿ���� Ŭ����. 

	MyWeapons		MW;				  // ĳ������ �κ��丮 ��Ȳ ����. 

//-------------------------------------------------

	unsigned char	ChangeDirectionDelayor, ChangeDirectionDelay;  // ���� ��ȯ ���� 
	
	BOOL			ShallowWaterSwitch;         // ���� �� ���� �ִ°� ?
	BOOL			SwimSwitch;					// �������ΰ� ?


	DWORD			m_uiAtb, m_uiAtb2, m_uiAtb3, m_uiAtb4, m_uiAtb5;            // ��ü�� �Ӽ� 
	SI32			m_siAttackAtb1, m_siAttackAtb2;
	DWORD			m_uiDefenceAtb;
	SI32			DyingAtb;
	DWORD			WorkStep;					// ���� ��� �۾��� �ϰ� �ִ°�?

	SI32			ShieldDelay;				// ���� �ִ°�?

	BOOL			ClimbSwitch;				// ����� �ö󰡴� ���ΰ�?

	SI32			m_siCurrentAttack;			// ���� �������� ������ ����(������ ������ ������ ��)
    // ���� ����. 
	SI32 			m_siAttackRange1;			// ���� �����Ÿ�1 
	SI32 			m_siAttackRange2;			// ���� �����Ÿ�2 
	SHORT			m_ssPriority_Attack_Type;	// robypark 2004/10/18 12:36 - �� ���� ���� �Ӽ��� ���� ��� �ΰ����� ó�� ���� ����

	// ���ݰ��� 
	_NewID 			AttackID;                   // ������ ��� ���̵� 
	char  			AttackArea;                 // ������ ���� 
	SHORT 			AttackGiveUpDelayor;		// ���� ���� �����ð�
	SHORT 			AttackGiveUpDelay;			// ���� ���� �����ð�
	SHORT 			AttackMode;
	SHORT 			HitPointX, HitPointY;
	SHORT 			m_ssAttackX, m_ssAttackY;	// ���� ��ǥ ��ġ ��ǥ
	SI08			m_scMagicApplyRate;			// ���� �����(���� Damage ��꿡 ����ȴ�)

	char 			FireRepeatNumber;			// �� �� �ݺ��ؼ� �����ߴ°�?
	char 			MaxFireRepeatNumber;		// �ݺ�ȸ���� �ִ밪 
	SHORT 			FireRepeatDelay;
	SHORT 			FireRepeatDelayor;

	char 			StrongAttackFrameNumber;

	UI32 			LastStrongAttackFrame;		// ���������� �ʻ�⸦ ����� ������.  

	// �ͽ�ź
	DWORD			GhostTime;

	// ǳ���
	DWORD			m_CloudWindFrame;

	// ���� ����. 
	BOOL 			HideSwitch;
	DWORD 			HideFrame;
	BOOL 			bAssistHideSwitch;
	DWORD 			AssistHideFrame;

	// ���� ���� 
	BOOL 			CoverSwitch;

	//----------------------------------------------------------------
	//   �ִϸ��̼� ���� 
	//----------------------------------------------------------------
	// ���� ���� 
	SHORT 			MaxWaitType;   // �� ������ ���� �ִϸ��̼����� ���� �� �ִ� �ִ��� ���� 
	SI32 			WaitType;       // ���� �� ������ ���ϰ� �ִ� ���� �ִ� 
	SI32 			NextWaitType;   // ������ �� ������ ����  ���� �ִ� 
	BOOL			WaitAnimationChangedSwitch;		// ���� �ִϸ��̼ǰ��� ������ �ߴ��� ����. 

	// �������� 
	char 			DyingOperationSwitch;
	SI32 			DyingStep;
	
	SI32			DeadDelay;			// ���� �Ŀ� �󸶳� ��ü�ϴ°�. 
	SI32			DeadDelayor;			// ���� �Ŀ� �󸶳� ��ü�ϴ°�. 

	// Ž������ 
	SHORT			BlackCheckX, BlackCheckY; 

	// �� �̵� ���� 
	BOOL			LowMoveSwitch;

	// robypark 2004/6/4 13:33
	// �Ҷ������� ���� �ִϸ��̼ǰ� ���õ� ����
	SI32			m_siMagicAniStep;	// ���� �ִϸ��̼� font�ε���

	// ���� ������ ���ϴ� �ൿ�� ���� 
	char			ReadyToFireStep;

	SHORT			CurrentOrder;               				// ���� ĳ���Ͱ� �ϰ� �ִ� �۾� 
	SHORT			Step;                         				// ���� �������� �ܰ� 
    char			HostMode;                     				// ����� �� ���� ���ΰ� �÷��̾��ΰ�?
	BOOL			m_bReverseSwitch;                			// ����� ���� ���ΰ�?

    _NewID			m_NewID;									// ĳ���� �ڽ��� ���̵�
	SHORT			CharOrderNumber;             				// ������ �迭���� ��� ��ġ�� �ִ°�?
	SI32			m_siOnlineID;								// �¶��� ĳ���ͷμ����Ϸù�ȣ. 

	SHORT			m_ssX, m_ssY;                  				// obj�� �߽� ��ġ 
	SI32			OldX, OldY;                   				// ������ ���� ��ġ. 
	SHORT			DX, DY;                       				// ������ 
	SHORT			PX, PY;                       				// ������ �ִ� ��ġ 
	SHORT			MOX, MOY;                     				// �̵��� ������ �Ǵ� ���� ��ǥ 
	SHORT			MDX, MDY;                     				// �̵��� ������ �Ǵ� ���� ��ǥ 

	char			m_scXsize, m_scYsize;                		// obj�� ũ�� 
	SHORT			m_ssDrawStartX, m_ssDrawStartY;      		// �׸��� �׷����� ���� ��ġ 
	SI32			ImageXsize, ImageYsize;       				// �׼ǽÿ� ���Ǵ� �׸��� ���� ���� ������ 
	SHORT			RX, RY;                       				// ĭ ������ �̵��ϴ� ĳ������ ���� 
	char			StepWidth;                    				// �̵� ����
		
	char			m_scIDXsize, m_scIDYsize;            		// ���̵� ���� ǥ�õǴ� ���� ���� ������. ���� �ʿ��� ĳ���Ͱ� �����ϴ� Ÿ�� ����
		
	SHORT			Direction;                     				// ���� 
	SHORT			TempDirection;                				// 16������ �����ϴ°�?
	SHORT			TargetDirection;               				// ��ǥ�� ���� 
		


	//---------------------------------
	// ������ ĳ���� ����(Hero)
	SI16			HeroID;										// ���° ���ΰ� ĳ�����ΰ�
	SI16			DirectionNo;								// ĳ������ ���� ���⿡ �´� ��Ʈ��ȣ�� ����� �� �ʿ��� ����
	SI16			AniAction;									// Animation �׸��� ��� Order����
	SI16			CharBaseWClass;								// ���⿡ ���� �ൿ�� �� �ִ� ���� CLASS
	SI16			CharWeaponEnum;								// ĳ���� �ִϸ��̼� �ӵ�
		
	SI16			AniStartFont;								// ĳ���� �ִϸ��̼� ���� ������ ù��° ��Ʈ ��ȣ(�� ����� ��Ʈ �� * DirectionNo)
	SI16			AniStep_Hero;								// �ִϸ��̼� ���� �ܰ�
	SI16			ReserveAniStep;								// ������ ���� animation font��ȣ 

	CHAR			DefaultBody[5];								// ĳ������ ����Ʈ ��(��: KD01)


	_NewID			m_GuardianAttackID;

	SI16			m_ssWeaponID;									// ���� ������ ����
	SI16			m_ssPreWeaponID;								// ������ ������ ����
	SI16			m_siPreGuardianID;
	SI16			m_siGuardianID;									// ��ȣ�� ID
	SI16			m_siArmorDrawID;
	SI16			m_siPrevArmorDrawID;
	SI16			m_siHelmetDrawID;
	SI16			m_siPrevHelmetDrawID;
	SI16			m_siWeaponDrawID;
	SI16			m_siPrevWeaponDrawID;
	//
	//---------------------------------


	//-------------------------------------------------------------
	// �浹 ���� ����. 
	//------------------------------------------------------------
	SHORT 			SelfBlock;                   				// ��ü �浹 �Ӽ� 
	SHORT 			Block;                         				// �浹�ϴ� �Ӽ� 

	char			m_scSeeSwitch;                				// ���忡 ���� �ִ°� ���°�?
	char			m_scBusySwitch;                  			// ���� �ٻ۰� �ٻ��� ������?

	int				Para1;
	_NewID			Para2;                  					// ����� �Ķ��Ÿ 1, 2 


	RECT			RealRect;


	SHORT			AutoSwitch;									// ������� ����� �޴°�?
	SHORT			EventSwitch;								// �̺�Ʈ�� �߻��ϴ� �����̴�. 
																// ���� �������� �ʴ´�. 

	// ĳ���� ���ð��� 
	clock_t			SelectedClock;								// ���õ� �ð� 
	char 			NewSelectedSwitch;
	char 			NewSelectedDelay;
	SI32 			Group;										// ��� ���� �ҼӵǾ� �ִ°�?


	// ���� ����
	SHORT 			CarcassDelay, CarcassDelayor;
		

	// ġ����� 
	SHORT 			HealDrawSwitch;                      // ġ�� �޴� ����� �׸� ���ΰ�?
	SHORT 			HealDrawStep;                   
		
	// (�Ż�) ������ ����
	SHORT 			LevelUpDrawSwitch;                      // ġ�� �޴� ����� �׸� ���ΰ�?
	SHORT 			LevelUpDrawStep;                   

	// ���ڼ� ���� 
	SI32			m_siHaltDelay;                          // �����ϰ� �־�� �ϴ� �ð�. 
	SI32			m_siHaltReason;					  	  // ���� ������ ����Ǿ��°�?
	SI32			m_siHaltStep;								

	//�߼��� ȥ	
	SI32			m_siSoulDelay;						  // �߼��� ȥ ���� �ð�.	
	bool			m_bSoulUse;							  // �߼��� ȥ�� ������ΰ�.
	SI32			m_siFontCount;						  // �߼��� ȥ �׷��� ��Ʈ.
		
	// ��ȣ�� ���� 
	SHORT 			ProtectorDrawSwitch;
	SHORT 			ProtectorDrawStep;

	// ��ɰ��� 
	SHORT 			BufferOrderIndex;                       
	SHORT 			PushOrderMode;
	_OrderDataInfo	BufferOrder;
	_OrderDataInfo	OldOrder;                              // ������ �ϴ� ����� ������ ���� 

	//��ȣ�θ� ����ϴ� �����ΰ�.
	bool			m_bUseGuardian;
	SI16			m_siCount;
	DWORD			m_dwGuardianTime;

	SI16			m_siGuardianKind;
	SI16			m_siGuardianCondition;
	SI16			m_siGuardianSkillNum;

	UI16			m_uiGuardianRecallKind;
	SI16			m_siGuardianRecallNum;
	SI16			m_siGuardianNHitrate;
	SI16			m_siGuardianBHitrate;
	SI16			m_siGuardianSHitrate;
	SI16			m_siGuardianNoHaltHitrate;
	SI16			m_siGuardianAliveTime;

	bool			m_siGuardianTimeOut;
	bool			m_siGuardianNoTimeOutSkillUse;

	SI16			m_GuardianStartFile;		//��ȣ�� ĳ���� ��Ʈ.

	SI32			targetid[50];
	SI32			targetidCount;

	//��ȣ�η� ������� ĳ�����ΰ�.
	bool			m_bGuardianChar;

	//��ȣ�θ� ������ �ֳ�.
	bool			m_bGuardianEquip;	

	//��ȣ�� (�䳢)����.
	SHORT			MoveSpeed;		    // �̵� �ӵ� 
	float			m_MoveDelay;		// �����̴� �ӵ�.
	float			m_AttackDelay;		// �����ϴ� �ӵ�.
	
	bool			m_bRabbitAttack;	// ��ĳ���Ͱ� rabbit�� �¾Ҵ°�.
	SHORT			m_RabbitShooterId;	// rabbit�� ����� ĳ������ ���̵� �����Ѵ�.

	//��ȣ�� (��)����.
	DWORD			m_dwCawTimer;		
	SHORT			m_siCawNumber;
	_NewID			m_idCaw;
	char			m_hostCaw;

	bool			m_bHorseAttack;		// �츮�� ĳ���Ͱ� Horse�� ������ �ִ°�.

	SHORT			m_siDeadCawX;
	SHORT			m_siDeadCawY;
	
	SHORT			m_siAttackX_ForGuardianDragon;	//��ĳ������ X��ǥ. ��ȣ�� ���� �������� ���� ��ǥ ��
	SHORT			m_siAttackY_ForGuardianDragon;	//��ĳ������ Y��ǥ. ��ȣ�� ���� �������� ���� ��ǥ ��

	bool			m_bGuardianLifeUp;			// ��ȣ�� LifeUp
	bool			m_bMagicResist;				// ��ȣ�� ���� Up
	bool			m_bDamageResist;			// ��ȣ�� Ÿ�� Up

	SI16			m_siGuardianMaxLife;	// Guardian�� ������Ʈ �Ҽ� �ִ� Max Life
	SI16			m_siGuardianLife;		// Guardian�� ������Ʈ �Ҽ� �ִ�  Life

	// ��ȣ��(����)
	DWORD			m_dwPigAttackDelay;		// ������ ���� ��Ű�� �ð�(3��)
	BOOL			m_bPigAttack;			// ��ȣ�� ������ ���� �����ΰ�(����)
	BOOL			m_bPigDefence;			// ��ȣ�� ������ ���� �����ΰ�(����)

	bool			m_bTargetAttack;

	// ��ȣ��(��)
	_NewID			m_ChickenID[3];			// ���Ƹ��� ���� ���� ���̵�
	SI32			m_siChickPrevKind;		// ���Ƹ��� ���ϱ� �� Kind

	// ���� ��� 
	SHORT			ReserveOrderIndex;                     // ������ۿ� ����� ��� �� �ִ°�?
	_OrderDataInfo	ReserveOrder[MAX_BUFFER_ORDER_NUMBER];

	_NewID 			HostID;                                // ������ �Ǵ� ĳ���� (�н�)
	_NewID 			SlaveID;									// �����̺��� ���̵� 
	_NewID 			Slave2ID;								// �����̺��� ���̵� 
			
	char  			OtherCollSwitch;                        // �ڱ� �ڸ� �ܿ� �ٸ� �������� �浹 üũ�� �ϴ°�?
	POINT 			OtherCollPoint;                         // �ڱ� �ڸ� �ܿ� �ٸ� ������ ǥ�õǴ� �浹 üũ�� ��ġ 
	SHORT 			SetIDSwitch;                            // ������ ������ �����ߴ��� ���θ� ����Ѵ�.
			
	SHORT 			MenuItem   [MAX_MENU_NUMBER];
	SHORT 			MenuMode   [MAX_MENU_NUMBER];
			
	SHORT 			MenuNumber;
	SHORT 			UpdateMenuOrder;

	// �ִϸ��̼� ����. 
	SI32 			Wait0CharAnimationIndex;
	SI32 			Wait1CharAnimationIndex;
	SI32 			MoveCharAnimationIndex;
	SI32 			LowMoveCharAnimationIndex;
	SI32 			DyingCharAnimationIndex;
	SI32 			AttackCharAnimationIndex;
	SI32 			Attack2CharAnimationIndex;
	SI32 			SwimCharAnimationIndex;
	SI32 			SwimWaitCharAnimationIndex;
	SI32 			AmbushCharAnimationIndex;
	SI32 			MagicCharAnimationIndex;
	SI32			Magic2CharAnimationIndex;
	SI32 			HealCharAnimationIndex;
	SI32			ManaCharAnimationIndex;
	SI32 			LowAttackCharAnimationIndex;
	SI32 			ReadyToFireCharAnimationIndex;

	SI32 			SatJEtcCharAnimationIndex;					// ���̰��� ���� �غ�Ǿ����� �˷��ִ� ����. 
	SI32 			MagicEtcCharAnimationIndex;				// ���� ��� ĳ������ Ư�� ȿ��. 
	SI32 			FireHouseKEtcCharAnimationIndex;			// ���� ��ȭ�� ȭ�� ȿ��. 
	SI32 			EtcCharAnimationIndex;
	SI32 			ShipyardKEtcCharAnimationIndex;
	SI32 			TowerCEtcCharAnimationIndex;
	SI32 			AttackEtcCharAnimationIndex;
	SI32 			DyingEtcCharAnimationIndex;
	SI32 			General4kMagicEtcCharAnimationIndex;
	SI32 			General4jMagicEtcCharAnimationIndex;
	SI32 			InvalidCharAnimationIndex;
	SI32 			MakePortalCharAnimationIndex;				// ������ �� �ⱸ

	SI32			InGroundCharAnimationIndex;
	SI32			OutGroundCharAnimationIndex;

	// �ڸ���� ���� 
	SHORT			HoldSwitch;

	// �ź�����. 
	BOOL 			AmbushSwitch;
	SI32 			AmbushDelay;

	// ��� ����
	SHORT 			GuardSwitch;               // ��踦 ������ ���θ� �˷��ش�. 
	SHORT 			GuardX, GuardY;            // ��踦 ���� ���� 

	// �ǰݰ��� 
	SHORT			AttackMePlayer;
	_NewID			AttackMeID;
	SHORT 			BeAttackedSwitch;
	SHORT 			NeedHelpSwitch;
    SHORT 			NeedHelpDelay;
	DWORD 			BeAttackedFrame;
	_NewID			PrevAttackMeID;

	_NewID			FireID;
	SHORT 			FireSwitch;
	SHORT 			FireDelay;

	SHORT 			NX, NY;       // ĳ������ ���� ��ġ 
	SHORT 			MX, MY;       // �̵��� �߰� ���� 
	SHORT 			MoveBlockX, MoveBlockY;
			

	SHORT 			MoveBlockDelay;
	SHORT 			MoveBlockCount;

	SHORT 			FindAnotherPathSwitch;   // �ٸ� ��θ� ã���� ���θ� �����Ѵ�. 
				
	SHORT 			MoveDelayor;
			
		

	SHORT			MoveSpeedVary;   // �������� ��Ȳ���� ���� ����Ǵ� ����ġ 
	
	SHORT			MovePercent;  // �� %�� �̵��Ͽ��°�? 

	SHORT 			WaitCount;
	SHORT 			Wait;


	SHORT 			SelfExpPercent;  // ��������� ���涧������ �ۼ�Ʈ 
			

	SHORT 			Discover[MAX_MULTI_PLAYER];        // ���̴��� ���� 
	SHORT 			DiscoverDelay[MAX_MULTI_PLAYER];   // �󸶳� ���̴°�?

			
	// ��Ÿ ��°��� 
	SHORT 			EtcDrawSwitch;
	SHORT 			EtcDrawType;
	SHORT 			EtcDrawStep;
			
	BOOL 			SearchForwardSwitch;	// ���� Ž���� �ǽ��� ���ΰ�?

	SI16			m_siReCallCount;

	BOOL			m_bChangePlayer;
			

public:
	cltCharOrg();
	void Init(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber,_ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid);

protected:
	//-------------------------------------------------------------------------
	// �ź� ���� 
	BOOL			Ambush();

	//-------------------------------------------------------------------------
	// ���� Ž�� ����. 
	void 			DetectMine();
	BOOL 			CanDetectMine();

	void			HeroInit();								// ������ ĳ���� ���� �ʱ�ȭ

public:
	
	// �Ӽ��� ���Ѵ�. 
    BOOL 			IsAtb(DWORD atb) const
	{			
		if( m_uiAtb & atb )
			return TRUE;
		return FALSE;
	}
	
	// �Ӽ��� ���Ѵ�. 
    BOOL 			IsAtb2(DWORD atb) const
	{			
		if( m_uiAtb2 & atb )
			return TRUE;
		return FALSE;
	}
	
	// �Ӽ��� ���Ѵ�. 
    BOOL 			IsAtb3(DWORD atb) const
	{			
		if( m_uiAtb3 & atb )	
			return TRUE;
		return FALSE;
	}

	// �Ӽ��� ���Ѵ�. 
	BOOL			IsAtb4(DWORD atb) const
	{
		if( m_uiAtb4 & atb ) 
			return TRUE;
		return FALSE;
	}
	
	// �Ӽ��� ���Ѵ�. 
	BOOL			IsAtb5(DWORD atb) const
	{
		if( m_uiAtb5 & atb ) 
			return TRUE;
		return FALSE;
	}

	// ��� ������ �����Ǿ�� 
	BOOL 			IsAtbAnd(DWORD atb) const
	{			
		if((m_uiAtb&atb)==atb)return TRUE;
		return FALSE;
	}
	// �Ӽ��� ���Ѵ�. 
     BOOL 			IsDyingAtb(SHORT atb)const
	{			
	  if(DyingAtb&atb)return TRUE;
	  return FALSE;
	}

	// ������� ���θ� �˷��ش�. 
	 BOOL 			IsGeneral() const;
			
    BOOL 			IsAttackAtb(SI32 atb) const{	if(GetAttackAtb() ==atb) return TRUE;	return FALSE; };
    BOOL 			IsDefenceAtb(DWORD atb) const{	if(m_uiDefenceAtb==atb)return TRUE;	return FALSE; };

	/////////////   // ���� ���� �Լ�  ////////////////////////////////////
	void 			_SetWaitImage  (SHORT waittype, SHORT file, SHORT font, SHORT frame);
	void 			_SetWaitImage16(SHORT waittype, SHORT file, SHORT font, SHORT frame);
	void 			_SetWaitImage  (SHORT waittype, SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	/////////////   // �̵� ���� �Լ�  ////////////////////////////////////
	void 			SetMoveImage(SHORT file, SHORT font, SHORT frame);
    void 			SetMoveImage16(SHORT file, SHORT font, SHORT frame);
	void 			SetMoveImage(SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	//////////////// ���� ���� �Լ�  /////////////////////////////////////
	void 			SetAttackImage(SHORT file, SHORT font, SHORT frame);
	void 			SetAttackImage16(SHORT file, SHORT font, SHORT frame);
	void 			SetAttackImage(SHORT ani, SHORT file, SHORT font, SHORT frame);
			
	void 			DecideFontMove();
	void 			DecideFontWait();
	void 			DecideFontWaitNormal();
	void 			DecideFontWaitFly();

	void 			DecideFontAttackNormal();
	void 			DecideFontAttack16();

	// ���� ȿ������ ���´�. 
	SI32 GetAttackEffect() const;

	// ���� ������ ���´�. 
	SI32 GetAttackRange() const;
	// ���� ������ �����Ѵ�.
	void SetAttackRange(SI32 range);

	// ���� �Ӽ���  ���´�. 
	SI32 GetAttackAtb() const;
	// ���� �Ӽ��� �����Ѵ�.
	void SetAttackAtb(SI32 atb);




////////////////////////////////////////////////////////////////////////////
///    ���� ���� ���� 
////////////////////////////////////////////////////////////////////////////


	// ����� ���ۿ� �ִ´�. 
    BOOL			PushOrder(_OrderDataInfo order, SHORT mode, SHORT debuginfo);
	// ���� ����� �����Ѵ�. 
    void			ClearReserveOrder();
    // ���� ����� ������ ���� ���۷� �ű��. 
    void			PopReserveOrder();
    // ����� ���� ���ۿ� �ִ´�. 
    BOOL			PushReserveOrder(_OrderDataInfo order);
	// �־��� ����� ���� ���ۿ� �̹� ��� �ִ��� Ȯ���Ѵ�. 
    BOOL			HaveReserveOrder(SHORT order);
	// ���� �������� ����� �����̾����Ŀ� ���� �������� ������ �����ش�. 
    void			RecoverPrevOrder();

	BOOL			IncreaseLife(SHORT amount);
	BOOL			ReduceLife(SHORT amount, SHORT unique);

	BOOL			IncreaseMana(SHORT amount);
	BOOL			ReduceMana(SHORT amount);

	// ������ȯ ���� /////////////////////
    // ĳ������ ������ ��ȯ�Ѵ�. 
    BOOL			ChangeDirection(SHORT destdir);
    // ĳ������ ������ ��ȯ�Ѵ�. 
    BOOL			ChangeDirectionNormal(SHORT destdir);
    // ĳ������ ������ ��ȯ�Ѵ�. 
    BOOL			ChangeDirection16(SHORT destdir);

	// ���ְ� ������ ���� ���͸� ���Ѵ�. 
    SHORT			GetNearLandSector();

	//���� ��ǥ�� ���� ��ǥ���� ����� �� ������ ���Ѵ�. 
    void			GetMoveRatio(int startx, int starty, int endx, int endy, int movepercent, SHORT& rx, SHORT& ry);


	// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� (���� ��ǥ)
    BOOL			IsAbsPtInObj(SHORT x, SHORT y, SHORT& length);
	// � �� ���� obj ���� �ִ��� ���θ� �˷��ִ� �Լ� 
    BOOL			IsPtInObj(SHORT x, SHORT y);
    // � rect���� ������ �����ϴ°�?
    BOOL			IsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
    // � rect���� ������ �����ϴ°�?
    BOOL			IsAbsPtInObj(SHORT x1, SHORT y1, SHORT x2, SHORT y2);



	void			SetSeeSwitch(SHORT mode);

	// ĳ���Ͱ� x, y��ġ�� ���� �� �ִ°� ���°�?
    BOOL			IsColl(SHORT x, SHORT y);
	// ĳ���Ͱ� x, y��ġ�� ���� �� �ִ°� ���°�?
    BOOL			IsColl(SHORT x, SHORT y, SHORT block);

	// ���� ��ġ x,y���� �̷������ ��ġ�� ���Ѵ�. 
    void			OperateMapPosition(SHORT x, SHORT y);

	BOOL			SetSelect(int mode, int effectswitch);

	// ���� �����ܿ� �ִ� ����� ���ش�. 
    void			SetShortCutUnselect();

//------------------------------------------------------------------------
//  ���� ����. 
//------------------------------------------------------------------------
	void			GetTreasure(SHORT treasure);

	// �α��� �� �ڸ��� ������ ���� ���´�. 
    void			PutTreasureNear(SHORT type, SI32 amount);

	// ���� �� �ڸ��� �������� ���� ���´�.
	BOOL			ReleaseItem();

	// �ǰݽ� ǥ�� ���� �Լ�. 
	void SetBeAttackedDrawSwitch(BOOL mode){m_bBeAttackedDrawSwitch = mode;}
	void SetBeAttackedDrawStep(SI32 step){m_siBeAttackedDrawStep = step;};

	// �޴��� ������Ʈ�Ѵ�. 
    BOOL			UpdateMenu(SHORT order);
	// ���׷��̵� �޴��� �߰��� �� �ִ��� ���θ� �˷��ش�. 
    BOOL			CanPushUpgradeMenu(SHORT upgrade);
	// ��� �Ʒ��� �޴��� �߰��� �� �ִ��� ���θ� �˷��ش�. 
    BOOL			CanPushGeneralMakeMenu(SHORT kind);

	BOOL 			UpdateMenuNormal(SHORT order);


	BOOL 			SelectOperation();
	void 			PushMenu(SHORT number, int menu, SHORT upgrade=0, SHORT level=0, SHORT kind=0, SHORT needkind=0, SHORT money=0, SHORT tree=0);
	void 			PushMenuByOrder(SHORT number, int menu);


//--------------------------------------------------
//   �̵� ���� �Լ� 
//--------------------------------------------------
	// ĳ���͸� �̵��ϰ� �����. 
    BOOL			SetMove(SHORT dx, SHORT dy);
	int 			Move(SHORT moveblockcount, SHORT range=0);
	int 			MoveNormal(SHORT moveblockcount, SHORT range);
	int 			MoveFly(SHORT moveblockcount, SHORT range);
	int				GetMovePercent(){return MovePercent;}
	
	SI32			UnderGroundMove();				// ���Ϸ� �̵��ϴ� �� ó��.


	void			CharMove();

	BOOL 			IsPreMoveOk();
	BOOL 			IsPreMoveOkSAMC();
	BOOL 			IsPreMoveOkFireTankK();

	void 			DoPreMove();
	BOOL			MoveDone();

//---------------------------------------------------
//  ��� ���� 
//---------------------------------------------------

	// ���ڵ� ������ ��� �� ����� �����ش�. 
    void			DrawHalt();


//-----------------------------------------------------------------------------------
//  ���� ���� 
//-----------------------------------------------------------------------------------
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y);
	BOOL 			FindEmptyArea(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize, BOOL diff_level_mode = TRUE);
	BOOL 			FindEmptyAreaTreasure(SHORT& x, SHORT &y);

	void			SetFileNFont();


//-----------------------------------------------------------------------------------
//  �Ҳ��� ���� 
//-----------------------------------------------------------------------------------
	BOOL 			SetExtinguish();
	BOOL			FindNearFire(int* six, int* siy);
	// ����� �������븦 ã�´�. 
	BOOL			FindNearFireNo(int* six, int* siy);
	// �ΰ��������� ���� ����. 
	BOOL			ExtinguishAI();


	// �� ���� �ڸ��� �������� ��ó ���. 
	BOOL			FireOperation(int x,int y);

	//-------------------------------------------------------------------------
	// �ʻ�� ����

	// Ư�� ����� ������ ��´�. 
	SI32 GetSpecialInfo(SI32 siAttackAtb, SI32 infotype);

	BOOL SpecialOperation(SI32 siSecial, _NewID id, SI32 x=0, SI32 y=0);
	BOOL CanSpecial(SI32 siSecial, _NewID id, SI32 *pReturnVal=NULL);
	BOOL SetSpecial(SI32 siSecial, _NewID id, SI32 *pReturnVal=NULL);
	BOOL Special(SI32 siSecial);
	SI32 SpecialAct(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y);

	BOOL CanSpecialAtb4(SI32 siSpecial, _NewID id);
	BOOL SetSpecialAtb4(SI32 siSpecial, _NewID id);
	BOOL SpecialAtb4(SI32 siSpecial);
	BOOL SpecialOperationAtb4(SI32 siSpecial, _NewID id, SI32 x=0, SI32 y=0);
	SI32 SpecialActAtb4(SI32 siAttackAtb, _NewID id, SI32 x, SI32 y);

			
//-----------------------------------------------------------------------------------
// ���� �ΰ����� ����. 
//-----------------------------------------------------------------------------------
	// �þ߸� ó���Ѵ�
	void BlackCheck(); 
    void 			BlackCheckNormal();			// �Ϲ� ����
	void 			BlackCheckHeroMode();			// �ϱ��� ��� ����
			
	// ���� Ž�� :��ó�� ���� �ִ��� ���θ� �����Ѵ�. 
    BOOL			SearchForward();

	// ���� �� �غ� �Ǿ� �ִ°�?
    BOOL			CanWork();

	// ���� �߰����� ��� ��� ��ó�ϴ��� �����Ѵ�. 
    // TRUE:���� �߰��ϰ� ������ ��ġ�� ���ߴ�. 
    // FALSE:������ ��ġ�� ���� �� ���� ����̴�. 
    BOOL			OperateFindEnemy(SHORT x, SHORT y, _NewID id);

	BOOL			IsInRange(SHORT id, SHORT range);


	BOOL 			CanAttack(_NewID id);
    BOOL 			CanAttack(SHORT x, SHORT y);
	BOOL			CanAttackGround(SI32 six, SI32 siy)
	{	// �� �ڽ��� ���� ���� ���¶��, 
		if(m_scSeeSwitch==FALSE)return FALSE;
		
		return TRUE;
	}

			
	// ���� �������� ���� id�߿��� id�� �����ϴ� ���� �� ���ڴ°�?
    BOOL			IsGoodAttackNewEnemy(_NewID id);


	void			SetOldOrder(SHORT order, SHORT hostmode, SHORT clearswitch,
							int para1, _NewID para2);

	// Ư�� �������� ĳ���͸� �о��. 
	BOOL Pull(SI32 dir);

	// robypark 2004/6/1 17:11
	// Ư�� �������� ĳ���͸� �о��. 
	// SI32 dir[in]: ����
	// SI32 siDistance[in]: �о �Ÿ�
	BOOL Pull(SI32 dir, SI32 siDistance);

//--------------------------------------------------------------------------------------------
// �ִϸ��̼� ����. 
//--------------------------------------------------------------------------------------------
	void			SetAnimation(SI32 ani, SI32 * pdelayor = NULL);
	BOOL			DoAnimationDelay(BOOL animode, BOOL atb = 0);
	SI32			GetAniInstant();


	BOOL			FindEmptyAreaWater(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize);
	// ���߿��� ���ڸ��� ã�´�. 
    BOOL			FindEmptyAreaAir(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize);

	//------------------------------------------
	// ������ ĳ���� ����
	BOOL			SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound);	// ĳ���Ϳ��� ������ ������ ������ �����Ѵ�.

	// ĳ������ ����ġ�� �÷��ְ� ���� ������ ����� �뺸�Ѵ�. 
	// showresultswitch: TRUE�� ��� ȭ�鿡 ����ġ�� ����ٴ� �޽����� �����ش�. 
	void IncreaseExp(SI32 exp, BOOL serversendswitch, BOOL showresultswitch);

	SI32 GetApower();
	SI32 GetDpower();

	// ĳ������ �̸��� ���´�. 
	char* GetName() const;
	void SetName(char* name, SI32 namelength);

	// ���� ����. 
	void SetWeaponAttackDelay(SI32 delay){m_siWeaponAttackDelay = delay;}

	SI32 GetTotalAttackDelay();
	SI32 GetTotalSearchRange()const{return m_siSearchRange;}
	void SetSearchRange(SI32 range){m_siSearchRange = range;}


	// ���� ���� �Լ�. 
	void SetHalt(SI32 haltmode, SI32 haltdelay);

	void SetUpdateInfo()
	{
		m_siUpdateInfo = m_ParmeterBox.IP.GetLife() + m_ParmeterBox.IP.GetMana() + (m_ssX/3) + (m_ssY/3);
	}

	void SetOldUpdateInfo()
	{
		m_siOldUpdateInfo = m_siUpdateInfo;
	}

	SI32 GetUpdateInfo()const
	{
		return m_siUpdateInfo;
	}

	SI32 GetOldUpdateInfo()const
	{
		return m_siOldUpdateInfo;
	}


	BOOL GetSeeStep(SI32 player) const;
	SI32 GetBlackByPlayerNumber(SI32 pnumber, SI32 x, SI32 y) const;
	BOOL GetSeeModeByPlayerNumber(SI32 pnumber) const;


	SI32 GetPlayerNumber()const{return m_cltCharStatus.GetPlayerNumber();}
	SI32 GetTimerDelay()const{return m_siTimerDelay;}

	void SetGuardianStartImage(SI16 m_siGuardianKind);
	SI16 GetGuardianStartImage();


//----------------------------------------------------------------------------
// robypark (2004/05/18 16:53) : �߰� ��� - ��� 2�� ���� Ư�� ���
//----------------------------------------------------------------------------
	// �Լ���: CreateBullet_SpecialNOVA
	// �Ķ����:
	//			SI32 siX[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(X)
	//			SI32 siY[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(Y)
	//			_NewID OwnerID[in]: ����� ����ϴ� ĳ���� ID
	// ����: ����(����-������, ���� 2�� ����): 16�������� ������ �߻��Ѵ�.
	void CreateBullet_SpecialNOVA(SI32 siX, SI32 siY, _NewID OwnerID);
	
//----------------------------------------------------------------------------
// robypark (2004/05/19 15:05) : �߰� ��� - ��� 2�� ���� Ư�� ��� ����,
//								 �Ͱ�ȭ���ĸ� ���� �Լ�
//----------------------------------------------------------------------------
	// �Լ���: GetDirection_ONSLAUGHT_FIREBALL
	// Parameter
	//			SI32 siX[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(X)
	//			SI32 siY[in]: ����� ����ϴ� ĳ���� Ÿ�� ��ġ(Y)
	//			SI32 siTargetX[in]: ��ǥ Ÿ�� ��ġ(X)
	//			SI32 siTargetY[in]: ��ǥ Ÿ�� ��ġ(Y)
	//			_NewID OwnerID[in]: ����� ����ϴ� ĳ���� ID
	// ����: �Ͱ�ȭ���ĸ� �߻��Ѵ�.
	void CreateBullet_ONSLAUGHT_FIREBALL(SI32 siX, SI32 siY, SI32 siTargetX, SI32 siTargetY, _NewID OwnerID);

	// robypark 2004-5-29 13:5 ���� ����
	// ĳ������ ��ǥ�� ��ǥ���� ��ǥ�� �Է¹޾� �� ���� �̷�� ������ ���� �� ���а��� �������� ���ؼ� �����ش�.
	// SI32 x1[in]: �߻��Ϸ��� ĳ���� ��ǥ(X)
	// SI32 y1[in]: �߻��Ϸ��� ĳ���� ��ǥ(Y)
	// SI32 x2[in]: ��ǥ�� ��ǥ(X)
	// SI32 y2[in]: ��ǥ�� ��ǥ(Y)
	// SI32 &siTargetX[out]: �� ��ǥ�� �̷�� ������ �� �� ���а� ������(X)
	// SI32 &siTargetY[out]: �� ��ǥ�� �̷�� ������ �� �� ���а� ������(Y)
	void CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 x2, SI32 y2, SI32 &siTargetX, SI32 &siTargetY);

	// robypark 2004-5-29 13:15
	// ĳ������ ��ǥ�� ��ǥ���� ��ǥ�� �Է¹޾� �� ���� �̷�� ������ ���� �� ���а��� �������� ���ؼ� �����ش�.
	// SI32 x1[in]: �߻��Ϸ��� ĳ���� ��ǥ(X)
	// SI32 y1[in]: �߻��Ϸ��� ĳ���� ��ǥ(Y)
	// SI32 siDelta_X[in]: �߻��ϴ� ĳ���� ��ǥ�� ��ǥ���� ��ġ ����(X)
	// SI32 siDelta_Y[in]: �߻��ϴ� ĳ���� ��ǥ�� ��ǥ���� ��ġ ����(Y)
	// float m[in]: ���� m
	// SI32 x2[in]: ��ǥ�� ��ǥ(X)
	// SI32 y2[in]: ��ǥ�� ��ǥ(Y)
	// SI32 &siTargetX[out]: ��(x1, y1)�� ���� m�� �̷�� ������ �� �� ���а� ������(X)
	// SI32 &siTargetY[out]: ��(x1, y1)�� ���� m�� �̷�� ������ �� �� ���а� ������(Y)
	void CreateTarget_byLinear(SI32 x1, SI32 y1, SI32 siDelta_X, SI32 siDelta_Y, float m, SI32 &siTargetX, SI32 &siTargetY);
};

// x, y�ֺ��� ��� �󸶳� �ִ��� Ȯ���Ѵ�. 
SHORT	GetNearCropCount(SHORT x, SHORT y);





// ĳ������ Ŭ���� 
class _Char : public cltCharOrg
{
private:

public:

	// ���� ���� ���� 
	BOOL ElectronSwitch ;
	BOOL ElectronInfo1 ;
	BOOL ElectronInfo2 ;
	BOOL ElectronInfo3 ;
	BOOL ElectronInfo4 ;


	// ����ġ ���ѱ� ���� 
	SHORT StealExpDrawSwitch;
	SHORT StealExpDrawStep;                   

///////////////////////////////////////////////////////////////


	_Char();
	void CharInit(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid);

	// robypark 2004/6/9 12:32
	// ĳ���Ͱ� ������ �� ���
	// �� �Լ��� CharInit(...)�Լ��� �����ϴ�. ���� SetInfo()�Լ��� ȣ������ �ʴ´�.
	// SHORT CharOrderNumber: ������ �迭���� ��� ��ġ�� �ִ°�?
	void CharInit_DISGUISE(SHORT kind, SHORT id, unsigned short unique, SHORT x, SHORT y, SHORT dir, SHORT makepercent, SHORT pnumber, _ImportantParameta * pip, MyWeapons *pmw, SI32 onlineid, SHORT usCharOrderNumber, SI16 siPrevWeaponID);

	SHORT GetX()const{return m_ssX;}
	SHORT GetY()const{return m_ssY;}

	// ���� ��ǥ�� ����Ѵ�. 
    void CalculateRealXY();
	// ���� ��ǥ�� ����Ѵ�. 
    void CalculateDrawStartXY();
	void CalculateDrawStartXYInMapCaptureMode();


    void GetHitPoint(SHORT & x, SHORT &y);

	UI08 GetTeamColor()const;

	
	// ��ü��  x,y�� ������ dx, dy������ �ִ� �Ÿ� 
    int  GetLengthToDest(SHORT x, SHORT y, SHORT dx, SHORT dy)
	{
	   return (dx-x)*(dx-x)+(dy-y)*(dy-y);
	}


	// id�� ���� �ּ� �����ȿ� �ִ��� Ȯ���Ѵ�. 
    BOOL IsInMinRange(SHORT id, SHORT minrange);

	// ����� ���� �����ϱ� ���� �ؾ��� �ʱ�ȭ 
    void InitPreGetOrder();



	void DecideAttackPoint(int port, int& x, int&y );



	///////////// ���� �� �׸��� �����Ѵ�. /////////////////////
    void DecideFont();
	void DecideFontNormal();
	void DecideFontLowMove();
	void DecideFontAttack(SI32 *file, SI32* font, BOOL* reverseswitch);
	void DecideFontAttack2(SI32 *file, SI32* font, BOOL* reverseswitch);
	void DecideFontSell();
	void DecideFontBuild();
	void DecideFontDead();

	//---------------------------------
	// ������ ĳ���� ����
    void DecideFontHero();



	void InitMenu(){};
	BOOL DrawShadow(SHORT darkstep);
	BOOL DrawShadowHero();			// ������ ĳ����(���ΰ�)�� �׸��� ���
	

    // �ʻ�⸦ ����Ͽ��� �ϴ� ��Ȳ�ΰ� �ƴѰ�?
	BOOL IsStrongAttackAvailable();

	BOOL  Fire();
	//  ���� �Ķ��Ÿ�� �ʱ�ȭ�Ѵ�. 
    BOOL ParametaAction();

	void GeneralEffect();


	BOOL CanSelfExp();
	BOOL SelfExp();
	BOOL SetSelfExp(SHORT x, SHORT y);

//////////////////////////////////////////////////////////
	// �ΰ����� ���� 
    BOOL AI();
/////////////////////////////////////////////////////////


	// player_number�� ������ ���� ���̰� �Ѵ�. 
    void SetDiscover(SHORT player_number);

	// ������ ���ݹް� ������ ȣ���ϴ� �Լ� 
    void SetBeAttackedSwitch(SHORT enemyid, SHORT attackmeplayer);
	// ������ ���ݹ������� ��ġ 
    void BeAttackedOperation();
	
	void OldOperation();

	// ĳ������ ������ ����� 
    BOOL SetID();
	// ĳ������ ������ ���ش�. 
    BOOL DeleteID();



	// ĳ���Ͱ� �����Ǿ����� �˸���. 
    void SetInfo();


	BOOL Action();

	// ����� ����� ������ �����Ѵ�. 
    BOOL GetOrderBuffer();

////////////////////////////////////////////////////////////////////////////
// ȭ�鿡 �׷��ֱ� 
///////////////////////////////////////////////////////////////////////////
	// ĳ���͸� ȭ�鿡 �׷��ش�. 
	BOOL Draw();
	// ������ ĳ����
	BOOL DrawHero(unsigned char *darktable, SHORT shadowysize);
	void DrawEtcInChar(SHORT darkstep, unsigned char* darktable);
	
	SI32 DrawHealthBar();
	// ��ȣ���� �׷��ش�.
    void DrawShield();
	// ġ����� �����ش�. 
    SI32 DrawHealBar(SI32 yindent);
	// ���� �������� �����ش�. 
    SI32 DrawShieldBar(SI32 yindent);
	// ����ġ�� ǥ���Ѵ�. 
    SI32 DrawExp(SI32 yindent);

	// �δ� ��ȣ�� ǥ���Ѵ�. 
    void DrawTeamNumber();
	// �δ� ��ȣ�� ǥ���Ѵ�. 
    void DrawLevel();
	void DrawSelectedBox(SHORT startx, SHORT starty, SHORT endx, SHORT endy);
	void DrawHeal();			// ġ��޴� ����� �׷��ش�. 
	void DrawStealExp();
	void DrawProtector();

	void DrawGuardian();
	// �¶��ο��� �������� ȿ��
	void DrawLevelUp();
	void DrawWangSoul();
	void DrawWangSoulBottom();
	BOOL GetSoulUse(){return m_bSoulUse;}

	// �ʻ�ȭ�� �׸���. 
    void DrawPortrait(LPDIRECTDRAWSURFACE7 surface);
    void DrawHealth();
	// �������� �����ش�. 
    void DrawMagic();
	void DrawDpower();
	void DrawApower();
	void DrawCrew();
	void DrawCharInfo();		// �¶��� ���� ��忡�� ĳ���� ����â�� �����ش�.
	// ĳ���� ������ �����ش�. 
    void ShowCharInfo(LPDIRECTDRAWSURFACE7 surface) ;

	// robypark 2004/10/6 15:28
	// ĳ������ ������ ������ �����ش�.
	// �ʻ�, ü��, ����
	void ShowCharInfoSimple(LPDIRECTDRAWSURFACE7 lpSurface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:42
	// ��ġ, ũ�� ���� ���
	// �ʻ�ȭ�� �׸���. 
	void DrawPortrait(LPDIRECTDRAWSURFACE7 surface, SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:24
	// ���� ü���� �����ش�. 
	void DrawHealth(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	// robypark 2004/10/6 17:24
	// �������� �����ش�. 
	void DrawMagic(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight);

	void SetHealDraw(SHORT mode);
	// (�Ż�) ������ ȿ���� �׷��ִ°�
	void SetLevelUpDraw(SHORT mode);	
	void SetProtectorDraw(SHORT mode);
	void SetStealExpDraw(SHORT mode);

	SHORT GetSelectedSwitch(){return m_scSelectedSwitch;}
	void SetNewSelectedSwitch(SHORT mode);

	/////////////////////////////////////////////////////
    ////// �Ǽ� ���� 
	BOOL Sell();
	BOOL Build();
	// �����ǹ��� �Էµ� ��ġ(x,y)�� ���� ���� ��ǥ�� ���Ѵ�. 
    BOOL GetBuildAddOnXY(SHORT kind, SHORT x, SHORT y, SHORT &dx, SHORT &dy);
	// kind �����ǹ��� ������ �ִ°�?
    // �ǹ��μ� ��ȿ�ؾ� �Ѵ�. 
    BOOL HaveAddOnBuild(SHORT kind);
	BOOL Repair();
	void BuildToCar();

	// �ǹ��� ���� �� �ִ°�?
    BOOL CanMake();
	BOOL SetMake(SHORT x, SHORT y, SHORT kind);
	void Make();
	void MakeNormal();
	void MakeHQC();
	void HelpBuild();

//////////////////////////////////////////////////////////////

	// ���� ���� ���� 
	void  Train();

	////////////// ���� ���� �Լ�  ///////////////////////////////////////
    //ĳ���� ���� 
    BOOL Attack();
	BOOL TryAttack();
	BOOL TryAttackGround();
	BOOL Escape();

	// fire������ Ȯ�� 
    BOOL ReadyToFire();

    ////  attack_id ���� �����ϰ� ����� �Լ� 
    BOOL SetAttack(_NewID id);
	////  ������ �����ϰ� ����� �Լ� 
	BOOL SetAttackGround(SI32 six, SI32 siy);

    // �������� ������ ���� ���� ó���ؾ� �� ���� ó���Ѵ�. 
    BOOL PreAttack();

	// robypark 2004/6/4 13:33
	// �Ҷ������� ������ ���� �Լ�
	// ��õ��� ������ => �Ϲ� �Ҷ�������
	BOOL ChangeModeFarangi(void);

	// robypark 2004/6/4 13:33
	// �Ҷ������� ������ ���� �Լ�
	// �Ϲ� �Ҷ������� => ��õ��� ������
	BOOL ChangeModeRoarOfLaughter(void);

	// robypark 2004/6/10 11:14
	// ��õ��ҿ��� ��� �Ҷ��������� �����Ѵ�.
	BOOL ChangeModeFarangi_Immediately(void);

	// robypark 2004/6/15 18:13
	// �Ҷ����������� ��� ��õ��ҷ� �����Ѵ�.
	BOOL ChangeModeRoarOfLaughter_Immediately(void);

	/////////////  ���� ���� �Լ� ///////////////////////////////////////
	BOOL DyingOperation();
	BOOL DyingOperationNormal();
	SI16 Dying();
	BOOL Dead();
	BOOL GeneralExDying();
	// ����� �Ұ� �Ǵ� ����ġ�� ���ϴ� �Լ�. (������ ���� ���������� ����ġ�� �Ұ� �ȴ�.)
	SI32 GetExpReduceForDying();

	// ĳ���� ������
	void GetOnlineUserMoney(MONEY money);

 
    //////// �� �ǹ� ���ɿ� ���� �Լ� /////////////////////////////////////
	// id�� ������ �� �ִ°� ?
    BOOL CanCaptureBuild(_NewID id);
	BOOL SetCapture(_NewID id);
	BOOL CaptureBuild();
    // �÷��̾� ��ȣ�� �ٲ��ش�. 
    BOOL ChangePlayer(SHORT destpnumber);
	BOOL ChangePlayerMagic(SHORT destpnumber,_NewID id );

	BOOL SetBetray(_NewID id, int* preturnval);
	// id�� ��Ȥ�� �� �ִ°�?
    BOOL CanBetray(_NewID id,  int* preturnval);
	BOOL Betray();
	BOOL BetrayOperation(_NewID id);


	BOOL SetGoIntoHQ(_NewID id);
	BOOL CanGoIntoHQ(_NewID id);
	BOOL GoIntoHQ();

///////////////////  �������� ���  ///////////////////////////////////////
	BOOL SetAbsorb(_NewID id);
    // id�� ü���� ����� �� �ִ°�?
    BOOL CanAbsorb(_NewID id);
	BOOL Absorb();
	BOOL AbsorbOperation(_NewID id);

///////////////  �����ġ����� �Լ�  ////////////////////////////////////
    BOOL SetStealTech(_NewID id);
	// id�� ����� ��ĥ �� �ִ°�?
    BOOL CanStealTech(_NewID id);
	BOOL StealTech();
	BOOL StealTechOperation(_NewID id);


	///// �������� �Լ� /////////////////////////////////////////////////////
	BOOL SetPatrol();
	BOOL Patrol();

	///// ���� ���� ����  /////////////////////////////////////////////////
	// �ֺ��� ���� ĳ���ͷκ��� �������� ���´�.
    void UseBuda();
	// ���� ������ ���� �� �ִ��� ���θ� �˷��ش�. 
    BOOL CanUseBuda();

	//// ����ġ ���ѱ� ////////////////////////////////////////////////////
	BOOL CanStealExp();
	void StealExp();

	//// ���� ���� ���� ////////////////////////////////////////////////////
    void AirMagic();
	// ���� ������ ���� �� �ִ��� ���θ� �˷��ش�. 
    BOOL CanAirMagic();

    /// ġ�� ���� �Լ� /////////////////////////////////////////////////////
	BOOL HealOperation(_NewID id);
	// �ڽ��� ġ�ᰡ���Ѱ�?
    BOOL CanHeal();
    // id�� ġ���� �� �ִ°�? 
	BOOL CanHeal(_NewID id);
	BOOL SetHeal(_NewID id);
	BOOL Heal();
	void MassHeal();
	BOOL HealAI();

	//���� ȸ�� ���� �Լ�.
	BOOL ManaOperation(_NewID id);
	// �ڽ��� ���� ġ�ᰡ���Ѱ�?
    BOOL CanMana();
	// id�� ���� ġ���� �� �ִ°�? 
	BOOL CanMana(_NewID id);
	BOOL SetMana(_NewID id);
	BOOL Mana();
	void MassMana();




	////// ����� ����  ///////////////////////////////////////////////////
	BOOL CanRainMake();
	void RainMake();

	//////////////////////////////////
	//  ���� ���� 
	//////////////////////////////////
	BOOL EarthQuakeOperation(_NewID id);
	BOOL EarthQuake();
	// �ڽ��� ���������� �����Ѱ�?
    BOOL CanEarthQuake();
	BOOL CanEarthQuake(_NewID id); 
	BOOL SetEarthQuake(_NewID id);

	//////////////////////////////////
	//  ȭ������
	//////////////////////////////////
	BOOL FireAttackOperation(_NewID id);
	BOOL FireAttack();

	BOOL CanFireAttack();
	BOOL CanFireAttack(_NewID id);
	BOOL SetFireAttack(_NewID id);

	//////////////////////////////////
	//  ���� ���� 
	//////////////////////////////////
	BOOL LightningOperation(_NewID id);
	BOOL Lightning();
	// �ڽ��� ���������� �����Ѱ�?
    BOOL CanLightning();
	BOOL CanLightning(_NewID id);
	BOOL SetLightning(_NewID id);

	//�߼��� ȥ.
	BOOL Soul();
	BOOL SoulOperation(_NewID id);
	BOOL CanSoul();
	BOOL SetSoul(SI32 siSoulDelay);

	//��â��ȯ
	BOOL LongSpear();
	BOOL LongSpearOperation(_NewID id);

	BOOL CanLongSpear();
	BOOL CanLongSpear(_NewID id);
	BOOL SetLongSpear(_NewID id);




    ///////////////////////////////////////////////////////////////////////
	// ���׷��̵���� 
	//////////////////////////////////////////////////////////////////////
	void  UpgwadeFunction();


	//////////////////////////////////////////////////////////////////////
	// �ڽ��� �нż� ���� 
	//////////////////////////////////////////////////////////////////////
	BOOL CanClon(_NewID targetnewid);
	BOOL Clon();
	//////////////////////////////////////////////////////////////////////
	// ��ȯ 
	//////////////////////////////////////////////////////////////////////
	BOOL CanReCallDamduk();
	BOOL ReCallDamduk();		// ��� ��ȯ ����
	BOOL CanReCall();
	BOOL ReCall();
	BOOL ReCallPanda();
	BOOL ReCallGuardianChar();						    //	��ȣ�� ĳ����(��,�䳢)
	BOOL PandaRecallOperation(_NewID targetnewid);		//	��ȣ�� ĳ����(��,�䳢)
	BOOL RecallGuardianCaw();						    //	��ȣ�� ĳ����(��) Order�� ���� ȣ��Ǵ� �Լ�.
	BOOL StartRecallGurdianCaw(_NewID id,char host); 	//	��ȣ�� ĳ����(��)�� ���۵ȴ�.
	BOOL IsRecallGuardianCaw(){return m_siCawNumber;};	//  ��ȣ�� ĳ���Ͱ� �ִ°�.
	BOOL DeleteRecallGuardianChar();					//  ��ȣ�� �����ۿ� ���� ������ ĳ���͸� �����Ѵ�.
	BOOL DeleteRecallGuardianCaw();						//  ��ȣ�� (��) �� �״´�.

	BOOL GetAtbSkill(SI16 id,SI16 targetid);

	BOOL AttackMagicGuardianRabbit();
	BOOL DeleteGuardianRabbit();						//
	BOOL MagicGuardianRabbit(_NewID attackID);			//  ��ȣ�� 

	BOOL DeleteGuardianHorse();
	BOOL MagicGuardianHorse();

	BOOL DeleteGuardianLifeUp();					//  ��ȣ�� (��) ü�� ��
	BOOL MagicGuardianLifeUp();						

	BOOL MagicGuardianMagicResistUp();					//  ��ȣ�� (��) ���� ��
	BOOL DeleteGuardianMagicResistUp();

	BOOL MagicGuardianDamageResistUp();					//  ��ȣ�� (��) Ÿ�� ��
	BOOL DeleteGuardianDamageResistUp();					
	
	BOOL DrawGuardianUse();

	void GuardianBreakless(SI16 siKind);

	
	BOOL	AttackMagicGuardianTiger();
	BOOL	AttackMagicGuardianChicken();
	BOOL    MagicGuardianChicken(_NewID attackID);

	_NewID	FindNearEnemy(SI16 siX,SI16 siY);
	
	GroundExp();
	GroundExpOperation(_NewID id,SI16 x,SI16 y);
    ///////////////////////////////////////////////////////////////////////
    //  �� ���� �Լ� 
    //////////////////////////////////////////////////////////////////////
    BOOL SetShield(_NewID id);
	BOOL CanShield();
	// id���� ���� �� �� �ִ°�? 
    BOOL CanShield(_NewID id);
	BOOL Shield();
	BOOL ShieldOperation(_NewID id);
 

	///////////////////////////////////////////////////////////////////////
    //  �����̵� ���� �Լ� 
    BOOL SetWarp();
	BOOL CanWarp();
	BOOL Warp();


	///////////////////////////////////////////////////////////////////////
	//  ���� ���� 
	BOOL CanMine();
	// ���ڸ� ���� �� �ִ°�?
    BOOL	CanMine(SHORT x, SHORT y);
	BOOL	SetMine(SHORT x, SHORT y);
	BOOL	Mine();
	BOOL	MineOperation(SHORT x, SHORT y);
	BOOL	SearchForMineSpot(SI32 *px, SI32 *py);						// ���ڸ� �ż��� �� �ִ� ��Ҹ� ã�´�.

	// ������ ���� �̺�Ʈ ��ũ�� ���������������� ����Ѵ�. 			 
    void	SetBattleFieldNEventMark();

	// ���ݿ� ���� ������ �����Ѵ�. �ٸ� ������ ���� �� ������ ������������ �Ǵ��ϴ� �ٰ�..
    void	SetFireInfo(_NewID id);

	// ������ �ൿ 
    BOOL	OperateWait();
	SI16	GetUpgradeMakeKind(BOOL bUpgrade);

	// damduk : ��ȣ��(����)(��)
	BOOL	ActionPig();
//	BOOL	AttackGuardianChicken();

	void FireOperation_Sword(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Dueoksini(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Toad(SI32 attackinstant, SI32 hitrate);
	void FireOperation_CannonJ(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Elephant(SI32 attackinstant, SI32 hitrate);
	void FireOperation_BigSword(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SeaKing(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Gun(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Electron(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GatphaPoison(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GhostCat(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic3(SI32 attackinstant, SI32 hitrate);
	void FireOperation_MagicBall(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Arrow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Needle(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Snowman(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FurHeadDemon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Dart(SI32 attackinstant, SI32 hitrate);
	void FireOperation_AirMissile(SI32 attackinstant, SI32 hitrate);
	void FireOperation_AttackerC(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SamMissile(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_TowerCannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_HeavyGun(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ShipCannon(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ThrowBomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cannon2C(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Bomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FireCar(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Fire(SI32 attackinstant, SI32 hitrate);
	void FireOperation_FireUsingMana(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GoldDragonElectron(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Onghwa(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Doll1(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Doll2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Kuket(SI32 attackinstant, SI32 hitrate);

	//���� ���� �߰�. 
	///////////////////////////////////////////////////////////////////////
	void FireOperation_CatDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_OldFoxDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_RaccoonDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_NinjaDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SnowWomanDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_YangGuiBiDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_JijangDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SnakeWomanDoll(SI32 attackinstant, SI32 hitrate);
	///////////////////////////////////////////////////////////////////////

	void FireOperation_Snow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Justicek(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Butterfly(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Widow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Wave(SI32 attackinstant, SI32 hitrate);
	void FireOperation_KimBomb(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Leech(SI32 attackinstant, SI32 hitrate);
	void FireOperation_SeaHorse(SI32 attackinstant, SI32 hitrate);
	void FireOperation_PoisonArrow(SI32 attackinstant, SI32 hitrate);
	void FireOperation_ThrowSpear(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Cactus(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Scorpion(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Claw(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magician(SI32 attackinstant, SI32 hitrate);
	void FireOperation_BigTree(SI32 attackinstant, SI32 hitrate);
	void FireOperation_VolcanoFlame(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Evil(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Ice(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Magic4T(SI32 attackinstant, SI32 hitrate);
	void FireOperation_JapanGhost(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Auta(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sub1(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sub2(SI32 attackinstant, SI32 hitrate);
	void FireOperation_PaperDoll(SI32 attackinstant, SI32 hitrate);
	void FireOperation_NeedleAttack(SI32 attackinstant, SI32 hitrate);
	void FireOperation_CloudWind(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Seimei(SI32 attackinstant, SI32 hitrate);
	void FireOperation_MagicCAttack(SI32 attackinstant, SI32 hitrate);
	void FireOperation_Sunbiyoun(SI32 attackinstant, SI32 hitrate);

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����� ���� START
//----------------------------------------------------------------------------

	void	FireOperation_Python		( SI32 attackinstant, SI32 hitrate );
	void	FireOperation_JinEmperor	( SI32 attackinstant, SI32 hitrate );

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - ����� ���� END
//----------------------------------------------------------------------------

//	void FireOperation_Rabbit(SI32 attackinstant, SI32 hitrate);
	void FireOperation_GuardianSnake(SI32 attackinstant, SI32 hitrate);

//----------------------------------------------------------------------------
// damduk : �߰� ĳ���� - ��ȣ�� ����
//----------------------------------------------------------------------------
	void FireOperation_GuardianPig(SI32 attackinstant, SI32 hitrate);


//----------------------------------------------------------------------------
// robypark (2004/5/19 17:33) : ��� 2�� ���� ���� �߰�
//----------------------------------------------------------------------------
	// ����-�ź���, �̼��� 2�� ����
	void FireOperation_TURTLE_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// ����-������, ���� 2�� ����
	void FireOperation_THUNDER_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// �Ϻ�-������, ��Ű��ī 2�� ����
	void FireOperation_QUAKE_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// �Ϻ�-�����, ���̸��� 2�� ����
	void FireOperation_BLACK_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// �߱�-�߼���, �̷� 2�� ����
	void FireOperation_CATAPULT(SI32 attackinstant, SI32 hitrate);
	// �߱�-�Ҷ�������, ���� 2�� ����
	void FireOperation_FARANGI_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// �븸-ȭ����, ����â 2�� ����
	void FireOperation_FIRE_DRAGON_VEHICLE(SI32 attackinstant, SI32 hitrate);
	// �븸-��Ȳ����, �弱ȭ 2�� ����
	void FireOperation_CHINESEPHOENIX(SI32 attackinstant, SI32 hitrate);

	// robypark (2004/6/4 13:6)
	// ��õ��� ����, �Ҷ�������, 126
	// ATTACKATB_ROAR_OF_LAUGHTER
	void FireOperation_ROAR_OF_LAUGHTER(SI32 attackinstant, SI32 hitrate);

	// robypark 2004/7/14 16:34
	// ���� ���� ����
	// ATTACKATB_DOLL_EVIS
	void FireOperation_DOLL_EVIS(SI32 attackinstant, SI32 hitrate);

	// robypark 2005/2/2 17:27
	// �޸� ���� ����
	// ATTACKATB_DOLL_DHARMA
	void FireOperation_DOLL_DHARMA(SI32 attackinstant, SI32 hitrate);
};



// id�� ġ���� �� �ִ°�?
// id��ü�� �������� ������ Ȯ���Ѵ�. 
BOOL CanHealGlobal(_NewID id);
BOOL CanManaGlobal(_NewID id);

#endif
