/************************************************************************************************
	���ϸ� : OnlineChar.cpp
	����� : ������
************************************************************************************************/
#ifndef	_ONLINE_CHAR_H
#define _ONLINE_CHAR_H

#include <graphics16.h>
#include "GSCDefine.h"
#include "OnlineCommonStruct.h"


#define IMOTICON_WAITTIME		5000						// �̸�Ƽ���� �� �ִ� �ð�
#define	MAX_ONLINE_CHAR			400
#define MAX_ONLINE_DEFAULT_CHAR 12

#define	MAX_ORDER_BUFFER_NUM	50

// ĳ���� ��ɵ�
#define	ON_ORDER_NONE			0		// ����� ����.
#define	ON_ORDER_WAIT			1		// ���� ���.
#define	ON_ORDER_MOVE			2		// �̵� ���.
#define	ON_ORDER_ATTACK			3		// ���� ���.
#define	ON_ORDER_USEOBJECT		4		// ������Ʈ�� ����Ѵ�.
#define	ON_ORDER_PICKUPITEM		5		// ������ �ݱ� ���.

#define	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE		6	// ���� �����ϱ�
#define	ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION	7	// ���� �����ϱ� ���� ��

#define	ON_ORDER_NPC			8		// NPC

#define ON_ORDER_FIELDATTACK_READY		9				// ���� ����� �����.
#define ON_ORDER_FIELDATTACK_ACTION		10
#define ON_ORDER_FIELDATTACK_DEAD		11

#define ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY		12  //���� ���Ÿ����� ���� ����.
#define ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION		13	//

// robypark 2004/11/9 17:2
#define ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON	14	// ������ ���� �����κ��� ����ǰ ��û���

// robypark 2004/11/9 17:2
#define ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE		15	// ������ ���� ������ ������ ���� ��û���

// robypark 2005/1/25 18:52
#define ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL			16	// ������ ���� ������ �ٸ� ���� ġ�� ��û

// robypark 2005/1/25 18:52
#define ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION	17	// ������ ���� ������ �ٸ� ���� ġ�� �ൿ

// robypark 2005/1/27 18:49
// ���� �����������̽��� ���ڴ�
#define ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE	18

// ĳ���� �̵� ���.
#define	ON_MOVE_DONE			1		// �̵��� ����Ǿ���.
#define	ON_MOVE_MOVING			2		// ��� �̵����̴�.

#define	ON_CHAR_4D_PF_NEED		1		// 4���� ��ã�Ⱑ �ʿ��ϴ�.
#define	ON_CHAR_4D_PF_NO_NEED	2		// 4���� ��ã�Ⱑ �ʿ������ʴ�.
#define	ON_CHAR_4D_PF_STOP		3		// �����ؾ� �Ѵ�.

#define	ON_CHAR_WAPR_DISTANCE	5		// 5 Ÿ���� ������ ���̰� ���� ������ �Ѵ�.

// robypark 2004/11/9 17:17
// ������ ���ְ� ���޼������� ������ ������ �Ÿ�
#define VALID_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON				6

// robypark 2005/1/25 19:59
// ������ ���ְ� ���޼������� ġ�ᰡ ������ �Ÿ�
#define VALID_HEAL_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON		3

// robypark 2004/12/6 10:23
// ������ ������ ����ǰ ���� ȿ�� ���� ���. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
#define EFFECT_CONSTANT_SUPPLYGOODS_FILLED			1
#define EFFECT_CONSTANT_SUPPLYGOODS_RANOUT			2

// ������ ���� ����Ʈ ���� ���
#define EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM		1	// ������ �������� ó�� ���ϴ� ���
#define EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM	2	// ������ ������ ���¿��� �����ϴ� ���

// robypark 2004/11/11 14:0
// ������ ������ �� �Ʊ�, ������ ���� ��� ����
#define SIEGEWARFARE_FORCE_FRIENDLY				0		// �Ʊ�
#define SIEGEWARFARE_FORCE_ENEMY				1		// ����
#define SIEGEWARFARE_FORCE_UNRELATED			2		// ������. ���� ��ܿ��� �ƴϸ� ���� ��ܿ��� �ƴϴ�.

// robypark 2005/1/27 13:19
#define VDI_ANIMATION_TYPE_REPAIR_HAMMER		1		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ��ġ��
#define VDI_ANIMATION_TYPE_REPAIR_WATCHTOWER	2		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ���� ����
#define VDI_ANIMATION_TYPE_REPAIR_SAW			3		// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ����

struct	OnOrder
{
	SI32	siOrder;
	SI32	siOrderParam1;
	SI32	siOrderParam2;
};

class	cltOnlineChar
{
private:
	BOOL	m_fShow;
	SI32	siID;					// �迭���� ID(�� �ε����̴�.)
	UI16	uiUniqueID;				// �� ĳ���͸��� ID.

	BOOL	m_bAttackMode;

	int		m_nScore;
	int		m_nRank;
	BOOL	m_bInvalidBattle;

	UI08	m_BattleFlag;					// Battle Flag
	DWORD	m_BattleFlagStartTickCount;		// Battle Flag Start Tick Count
	SI32	m_BattleFlagStartCheckCount;	// Battle Flag Start Check Count

	UI08	m_FindTargetFlag;		// Find Target Flag

	SI32	m_siDirection;		
	SI32	siClass;
	SI32	siKind;
	UI16	uiEquip[WEAR_ITEM_NUM];
	float	fRanking;
	SI32	siFightPower;	

	SI08	sifDataType;								// Data ����, 0 : �����Ͱ� ��� ����, 1 : Detail Data�� ����, 2 : Detail Data �����

	// ĳ������ ���.
	OnOrder	PrevOrder;									// ���� �������� ���.
	OnOrder	CurrentOrder;								// ���� ���
	OnOrder	OrdersBuffer[MAX_ORDER_BUFFER_NUM];			// ���� �����ӿ��� �����ؾ� �ϴ� ��ɵ�.
	SI16	siOrdersBufferCount;						// ��� ������ ī��Ʈ.

	// ĳ������ �� ������ ���� ��ǥ.
	SI16	siPrevX, siPrevY;

	// ĳ������ ���� ��ǥ.
	SI16	siPrevX1, siPrevY1;

	// ĳ������ ��ǥ.
	SI16	m_siX, m_siY;
	SI16	m_siRX, m_siRY;

	// �̵��ؾ� �� �������� �߰� Ÿ�� ��ǥ
	SI16	m_siMDX, m_siMDY;
	SI16	m_siMX, m_siMY;
	SI16	m_siMOX, m_siMOY;
	SI16	m_siMoveSpeed;
	SI16	m_siMovedPercent;
	SI32	m_siMoveableTileInfo;

	// �̵��ؾ� �� Ÿ�� ��ǥ.	
	SI16	m_siDX, m_siDY;
	SI16	m_siNX, m_siNY;

	// robypark 2004/12/17 16:52
	// �̵��Ϸ��� ��ǥ�� ������ �����Ͽ��� ���� �����ϰ� �ִ� ����
	SI16	m_siPrevSendedNX, m_siPrevSendedNY;

	// robypark 2004/12/16 10:42
	// ĳ������ ��ǥ�� �����Ͽ������� ��Ÿ���� �÷���
	BOOL	m_bIsSendedPlayerPos;

	cltOnlineWorld	*pMyOnlineWorld;		// Online World

	// ĳ���� �ִϸ��̼� ȭ���� ���� ����.	
	SI32	siAniActionCode;
	SI32	siAniDirection;
	
	// ���� �̵����� ����.	
	SI32							siMovingDirection;
	SI16							siMovingX;						// �̵����� X ��ǥ.
	SI16							siMovingY;						// �̵����� Y ��ǥ.

	// ���� ĳ������ ����
	BOOL							bBusySwitch;

	SI16							siPutSpeechCX, siPutSpeechY;	// ��縦 ���� ��ǥ.(X�� �߽� ��ǥ�̿��� CX, Y�� �ؽ�Ʈ�� �ϴ� ��ǥ�̴�.)
	CHAR							szSpeech[1024];					// ���.
	SI32							siSpeechSettingFrame;			// ����ġ�� ������ ������(���� ������ ���Ŀ� �������� �ϱ� ���ؼ��̴�.
	BOOL							siSpeechSettingfNotDel;			// ����ġ�� ��������� Flag

	SI16							siPutNameCX, siPutNameY;		// �̸��� ���� ��ǥ.
	SI16							siYetPutNameCX, siYetPutNameCY;	// ������ �̸��� ���� ��ġ
	CHAR							szCharName[ON_ID_LENGTH+1];					// �̸�
	DWORD							dwIP;							// IP

	CHAR							m_szGuild[ON_GUILDNAME_LENGTH + 1];	// ��� �̸�(��ĭ�̸� ��� �̰���)
	SI32							m_siGuildInGrade;				// ��峻���� ����
	SI32							m_siGuildFlag;					// ��� ���
	UI08							m_uiIsHostilityGuild;			// ���� ���� ��� Flag

	RECT							rcRealImage;					// ĳ������ Real Image.	

	BOOL							bMovingSwitch;					// ���� �̵��������� ����.
	UI16							MoveFlag;						// �̵� Flag

	//---------------------------------
	// ������2 ����� �ִϸ��̼ǽ� �ʿ��� ����
	SI16							m_siImKind;

	SI32							m_siImFile;
	SI32							m_siFont;						// ������ ĳ���ʹ� ĳ���� �ִϸ��̼� ���� ������ ù��° ��Ʈ ��ȣ(�� ����� ��Ʈ �� * DirectionNo)
	SI16							m_siXSize;
	SI16							m_siYSize;
	SI16							m_siAniStep;					// �ִϸ��̼� ���� �ܰ�
	SI16							m_siAniDelayer;					// �ִϸ��̼� �����̾�
	BOOL							m_bReverse;
	SI16							m_siDir;						// ĳ������ ���� ���⿡ �´� ��Ʈ��ȣ�� ����� �� �ʿ��� ����

	SI16							m_siCurrAni;					// ���� �ִϸ��̼�
	SI16 							m_siWait0CharAnimationIndex;	// ���
	SI16 							m_siMoveCharAnimationIndex;		// �̵�
	SI16 							m_siAttackCharAnimationIndex;	// ����
	SI16 							m_siMagicCharAnimationIndex;	// ���� ����

	//---------------------------------
	// ������ ĳ���� ����(Hero)
	//SI16							HeroID;							// ���° ���ΰ� ĳ�����ΰ�
	SI16							m_siHeroAniAction;				// Animation �׸��� ��� Order����
	//SI16							PreAniAction;
	SI16							m_siHeroBaseWClass;				// ���⿡ ���� �ൿ�� �� �ִ� ���� CLASS
	SI16							m_siHeroWeaponEnum;				// ĳ���� �ִϸ��̼� �ӵ�
		
	CHAR							m_cDefaultBody[5];				// ĳ������ ����Ʈ ��(��: KMN)
	
	SI16							m_siWeaponDrawID;				// (��ȭ/���� ���)  ���� �����ϰ� �ִ� ���� ID

	//
	//---------------------------------

	// �̸�Ƽ��
	DWORD							m_Imoticon_StartCount;			// �̸�Ƽ�� ���� �ð�
	UI16							m_Imoticon_Type;				// �̸�Ƽ�� Type

	// ĳ���Ͱ� ���õǾ����� �˷��ִ� ����.
	BOOL	NewSelectedFlag;
	SI32	NewSelectedDelay;

	// ���� ����� ����
	UI16							m_EquitWeapon;

	// ���λ���
	SellItemInfo					m_SellItemInfo[ON_MAX_BOOTH_ITEM];
	BOOL							m_bOpenBooth;

	//==================================================================================
	// ����ٴϴ� �뺴
	SI32							m_MercenaryKind;

	SI16							m_VMercenary_NowX;
	SI16							m_VMercenary_NowY;
	SI16							m_VMercenary_NextX;
	SI16							m_VMercenary_NextY;
	SI16							m_VMercenary_MovedPercent;
	SI16							m_VMercenary_Dir;

	SI16							m_VMercenary_TargetX;
	SI16							m_VMercenary_TargetY;

	SI16 							m_VMercenary_siWait0CharAnimationIndex;
	SI16 							m_VMercenary_siMoveCharAnimationIndex;
	SI16 							m_VMercenary_siAttackCharAnimationIndex;
	SI32 							m_VMercenary_siAniStep;
	SI32 							m_VMercenary_siAniDelayer;
	SI32 							m_VMercenary_siCurrAni;
	SI32 							m_VMercenary_siFont;
	SI32 							m_VMercenary_bReverse;

	SI32							m_siTempImFile;

	//==================================================================================
	// Effect
	SI32							m_ChangeEffectSex;
	SI32							m_ChangeEffectFrame;
	BOOL							m_bFieldAttackAni;

	// robypark 2004/11/2 13:29
	SI32							m_siSiegeWarfareTransformType;	// ������ �������� ���� �������� ��Ÿ���� �÷���.  �̹� ������ ������ ���¿��� ������ ����
	SI32							m_siEffectTransformFrame;		// ������ ���� ���� ������

	// robypark 2004/12/2 19:5
	SI32							m_siEffectSupplyGoodsType;		// ������ ������ ����ǰ ���� ȿ��. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	SI32							m_siEffectSupplyGoodsFrame;		// ������ ���� ����ǰ ���� �ִϸ��̼� ������

	// robypark 2004/11/24 18:36
	BOOL							m_bSiegeWarfareEvasion;		// ������ ���� �߰��� ��� ����Ʈ ��뿩��
	SI32							m_siEffectEvasionFrame;		// ������ ���� �߰��� ��� ����Ʈ ������

	BOOL							m_bSiegeWarfareDeathblow;	// ������ ���� ġ���� ���� ����Ʈ ��뿩��
	SI32							m_siEffectDeathblowFrame;	// ������ ���� ġ���� ���� ����Ʈ ������

	// robypark 2004/12/10 11:38
	SI32							m_siEffectVillageConterAttackType;		// ������ ���� �ݰ� Ÿ��
	SI32							m_siEffectVillageConterAttackFrame;		// ������ ���� �ݰ� �ִϸ��̼� ������

	// robypark 2004/12/16 15:2
	BOOL							m_bIsSiegeWarfareUnit;					// ������ ���������� ��Ÿ���� �÷���

	// robypark 2004/12/17 17:17
	BOOL							m_bIsInVillageDefenceInterface;			// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�?
	SI08							m_scInVillageDefenceInterfaceAniType;	// ĳ���Ͱ� ���� ���� �������̽� �� �ִ� ��� �ִϸ��̼� Ÿ��

	// robypark 2005/1/17 11:19
	BOOL							m_bIsRefugee;			// �ǳ��� ĳ�����ΰ�?
	SI32							m_siRefugeeKind;		// �ǳ��� ĳ���� Kind

	// robypark 2005/1/27 11:54
	SI08							m_scForceRelation;		// �� ĳ���Ϳ� ����ڿ��� ����

	// robypark 2005/2/17 21:44
	// ���콺 Ŀ�� �Ʒ� ĳ���������� �����Ѵ�.
	BOOL							m_bCursorHoverCharacter;

private:	
	// linus
	SI16							m_siFieldHp;									// ������ ������ ���� ü��
	// robypark 2004/10/27 15:4
	SI16							m_siFieldHp_Maximum;							// ������ ������ �ִ� ü��
	SI16							m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;	// ������ ���� ����ǰ ���緮
	SI16							m_siSiegeWarfareUnit_SupplyGoodsAmount;			// ������ ���� ����ǰ �ִ뷮
	DWORD							m_dwSiegeWarfareUnit_LastAttackDelay;			// ������ ���� ���� �����ð�
	BOOL							m_bCaptain;
//	SI16							m_siSupplyCharge;								// ���޼����� ������ �ִ� ����ǰ��

private:
	// robypark 2004/11/12 22:4
	SI16							m_siPositionGaugeX;			// ������ ������ �� �������� ü�¹� ��ġ(X)
	SI16							m_siPositionGaugeY;			// ������ ������ �� �������� ü�¹� ��ġ(Y)

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ
	SI16							m_siDrawCenterX;
	SI16							m_siDrawCenterY;

	// robypark 2005/1/27 11:1
	SI32							m_siAnimationResourceID_Blue;			// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	SI32							m_siAnimationResourceID_Red;			// �ش� ������ ���� �̹��� ����. ���� ������ ��

	// robypark 2005/2/15 16:32
	SI08							m_scMaxMoveActionSkip;					// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��
	SI08							m_scCurrentMoveActionSkip;				// �̵� ó�� �Լ� Move() ������	����. ������ skip ȿ��

private:	
	VOID	DecideFont();		// �̹� �����ӿ��� ĳ���Ͱ� ���� �� �ִϸ��̼��� ������ ���Ѵ�.	
	SI32	Move();				// ĳ���͸� �̵� ��Ų��.	
	VOID	GetMoveRatio(SI32 startx, SI32 starty, SI32 endx, SI32 endy, R64 movepercent, SI16 &rx, SI16 &ry);	// ĳ���Ͱ� �̵��������� ������ ���´�.		
	BOOL	UpdateAnimation(BOOL bForce, BOOL* fEndFrame, BOOL* fSound);		// �ִϸ��̼��� ������Ʈ �Ѵ�.(���� ��ɰ� ���� ���⿡ �����Ѵ�.)	

	BOOL	UpdateAnimationForRefugee(BOOL bForce, BOOL* fEndFrame, BOOL* fSound);		// �ǳ��� ĳ���͸� ���� �ִϸ��̼��� ������Ʈ �Ѵ�.(���� ��ɰ� ���� ���⿡ �����Ѵ�.)	
	
	BOOL	GetOrder(OnOrder *pOrder);			// ��� ���ۿ��� ��ɵ��� ���´�.	
	BOOL	IsOrderInBuffer();					// ���ۿ� ����� �� �ִ��� ����.	
	VOID	OverHead();							// ĳ������ �������.	

	VOID	SetMovingDirection(SI32 siDirection);	// ���� �̵����� ������ �����Ѵ�.
	BOOL	IsSendMovingDirection();				// �������� �̵����� ������ ������ �ϴ¶������� �˾ƿ´�.	

	BOOL	IsMyChar();								// ������� ĳ�����ΰ�?

	BOOL	IsSpeech();								// ���� ĳ���Ͱ� ���� �ϰ� �ִ°�?

	// ��ɿ� ���� �� �Լ���.
	BOOL	PreAddOrder(OnOrder *pOrder);																				// ����� �� ��� ���ۿ� �ֱ����� ���� �� �ִ��� ���캻��.
	BOOL	PreProcessorOrder(OnOrder *pOrder);																			// ����� ������ �����ϱ����� �������� �̸� ���캻��.

	SI32	IsNeed4DirectPathFind(SI32 siX1, SI32 siY1, SI32 siX2, SI32 siY2);											// 4���� ��ã�Ⱑ �ʿ��Ѱ�?
	BOOL	GetScreenRealRect(LPRECT pRect);																			// ��ũ������ RealRect�� ���Ѵ�.
	SI32	GetActionCode();																							// Action Code�� ���´�.

	VOID	DrawHero(SI32 siDrawMode, SI32 siStartX, SI32 siStartY,  UI08 *ctable, SI32 siCenterX=-1, SI32 siCenterY=-1, SI16 WaterP=-1);	// ������ ĳ���͸� �ش� ��忡 ���� �׸���.
	
public:	
	cltOnlineChar();			// ������.
	~cltOnlineChar();			// �Ҹ���.

	BOOL	Init(UI16 uiUniqueID, SI32 siID, CHAR *pszID, UI16 uiCharID, SI16 siX, SI16 siY, DWORD IP, float Ranking, SI32 FightPower, cltOnlineWorld *pWorld,UI16* lpuiEquipWeapon);		// �ʱ�ȭ.	
	BOOL	SetEquipItem(UI16* lpuiEquipWeapon);																			// ���� Item ����
	VOID	Free();																											// ĳ���͸� ���ش�.

protected:
	VOID	DrawRefugee(SI16 WaterP);																						// �ǳ��� ��ο�.		
	VOID	DrawChar(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);							// ĳ���͸� Draw(���ο��� ���� �Լ�)
	VOID	DrawCharRefugee(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);						// �ǳ��� ĳ���͸� Draw(���ο��� ���� �Լ�)
	VOID	DrawCharByVMercenary(SI16 WaterP, SI32 siStartX, SI32 siStartY, SI32 siCenterX, SI32 siCenterY);				// ����ٴϴ� �뺴�� Draw(���ο��� ���� �Լ�)

public:
	VOID	Draw(SI16 WaterP);																								// ��ο�.		
	VOID	DrawTransparent(SI16 WaterP);																					// ���������� �׸���.
	VOID	Action();																										// �׼�.	
	VOID	DrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface);																		// ��縦 ����ش�.
	VOID	DrawName(HDC hdc);																								// �̸��� ����ش�.	
	VOID	DrawGuildFlag(SI16 ScreenX = -1000, SI16 ScreenY = -1000);															// ĳ���� �̸� �տ� Guild ����� ��´�.(���� ��ǥ�� -1�̸� ID ��� �κ� �տ� ��´�.)
	VOID	GetPos(SI16 *psiX, SI16 *psiY);																					// Ÿ�� ��ǥ�� ���´�.	
	VOID	GetRPos(SI16 *psiX, SI16 *psiY);																				// Ÿ���� ���� ��ǥ�� ���´�.	
	VOID	GetDPos(SI16 *psiX, SI16 *psiY);																				// ���� �̵����� ��ǥ ��ġ�� ���´�.	
	VOID	GetNPos(SI16 *psiX, SI16 *psiY);																				// ���� �̵����� ���� ��ġ�� ���´�.	
	VOID	GetMovePersent(SI16 *ps);																						// ���� �̵����� ���� ��ġ�� ������ ���´�.	
	VOID	ClearOrder();						// ĳ������ ��� ����� �����.
	BOOL	AddOrder(SI32 siOrder, SI32 siOrderParam1, SI32 siOrderParam2);													// ĳ���� ����� ���ۿ� �ִ´�.	
	BOOL	AddOrder(OnOrder Order);																						// ĳ���� ����� ���ۿ� �ִ´�.	
	VOID	SetID(CHAR *pszID);																								// ĳ������ ID(���ڿ�)�� �����Ѵ�.
	UI16	GetUniqueID()			{ return uiUniqueID; }																	// ĳ������ UniqueID�� ���´�.
	BOOL	IsInit()				{ return  uiUniqueID != 0 ? TRUE : FALSE; }												// �ʱ�ȭ�� �Ǿ����� �˾ƿ´�.

	VOID	SetMove(SI16 siDX, SI16 siDY);																					// �̵��ؾ� �� ��ǥ�� �����ش�.	
	VOID	SetMoveByDirection(SI32 siDirection);																			// ĳ���͸� �� ������ �� Ÿ�� �̵� ��Ų��.
	SI32	GetMovingDirection();																							// ���� �̵����� ������ ���Ѵ�.	
	VOID	SetWalkingFlag(UI16 Flag);				// ĳ���͸� �̵� Flag

	VOID	InitSpeech();																									// ��縦 �ʱ�ȭ �Ѵ�.
	VOID	ActionSpeech();																									// ��� ó��,
	VOID	SetSpeech(CHAR *pString, BOOL fNotDel, CHAR* pID);																// �̹��� ���� ��縦 �־��ش�.		
	BOOL	CheckSpeech();																									// ��簡 ���ִ��� �˻�

	CHAR*	GetCharName();																									// ������� �̸��� ���´�.
	DWORD	GetIP();																										// ������� IP�� ���´�.

	SI32	GetFightPower(void);																							// ĳ������ �������� ��´�.
	BOOL	SetFightPower(SI32 FightPower);																					// ĳ������ �������� �����Ѵ�.

	BOOL	IsPointToChar(LPPOINT pptMouse, SI32 *psiDistance);																// ���� ���콺�� ����Ű�� ĳ���ʹ��� ���´�.	

	BOOL	IsWarp(SI16 siWarpX, SI16 siWarpY,SI16 sidistance);																				// ������ �ؾ��ϴ� ��Ȳ�ΰ�?
	VOID	Warp(SI16 siWarpX, SI16 siWarpY);																				// ������ �Ѵ�.
	BOOL	IsMoving();																										// ���� ĳ���Ͱ� �̵����ΰ�?

	OnOrder	GetCurrentOrder();																								// ���� �������� ����� �����Ѵ�.
	SI16	GetClass();																										// ĳ������ Ŭ������ ���´�.

	VOID	ChangeDirection(SI32 siDirection);	// ĳ������ ������ �ٲ۴�.	

	POINT	SetID(SI32 siBaseX, SI32 siBaseY);																				// BaseX, BaseY�� �������� ID�� ���� ���ο� ��ǥ�� ã�´�.
	VOID	DelID();																										// �ʿ��� �ڽ��� �����Ѵ�.

	VOID	SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo);					// �÷��̾��� ������ �����Ѵ�.
	VOID	SetPlayerInfo(OnCharPlayerDetailInfo* lpPlayerInfo);															// �÷��̾��� ������ �����Ѵ�.
	BOOL	SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound=FALSE);								// ĳ���Ϳ��� �������� ������Ų��.
	SI16	GetWeaponDrawID()	{ return m_siWeaponDrawID; };																// (��ȭ/���� ���) ���� �����ϰ� �ִ� ���� ID�� ���´�.
	VOID	SetWeaponDrawID(SI16 siWeaponDrawID)	{	m_siWeaponDrawID = siWeaponDrawID;	};								// (��ȭ/���� ���) ���� �����ϰ� �ִ� ���� ID�� �����Ѵ�.

	VOID	SetBattleFlag(UI08 Flag);																						// Set Battle Flag
	UI08	GetBattleFlag(void);																							// Get Battle Flag
	VOID	SetImoticon(UI16 uiImoticon);																					// Imoticon ���

	VOID	SetFindTargetFlag(UI08 Flag);																					// Set Battle Flag
	SI32	GetKind()					{	return siKind;	};

	// Ŭ���̾�Ʈ���� ���Ǵ� ĳ���� �ε��� ���
	SI32	GetImjinKind()				{	return m_siImKind;	};

	VOID	SetGuildData(char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag);											// ��� Data ����
	char*	GetGuildName(void);																								// ��� Name ���
	SI32	GetGuildInGrade(void);																							// ��� ���� ���
	SI32	GetGuildFlag(void);																								// ��� ��� ���

	BOOL	SetIsHostilityGuild(UI08 uiIsHostilityGuild);																	// ���� ������� �˻�
	UI08	GetIsHostilityGuild(void);																						// ���� ������� �˻�

	void	SetNewSelectedFlag(BOOL flag);
	BOOL	GetNewSelectedFlag();

	BOOL	Show(BOOL fShow);
	BOOL	GetShow(void);

	UI32	GetIP(UI16 uiUniqueID);																							// Get IP

	SellItemInfo	*GetBoothItem(UI08 uiSlot)							{ return	&m_SellItemInfo[uiSlot]; };				// ���λ��� ������ ���� ���
	VOID			SetBoothItem(UI08 uiSlot, SellItemInfo ItemInfo)	{ m_SellItemInfo[uiSlot] = ItemInfo; };				// ���λ��� ������ ���� ����
	BOOL			IsBoothOpen()										{ return	m_bOpenBooth; };						// ���λ��� ���� ���� ���
	VOID			SetBoothOpen(BOOL bOpenBooth);																			// ���λ��� ���� ���� ����

	SI08	GetDataType(void);
	void	SetDataType(SI08 fDataType);

	BOOL	SetMercenary(SI32 siKind);
	SI32	GetMercenary(void);

	void	ActionByVMercenary(void);
	void	DrawByVMercenary(SI16 WaterP);

	void	GetNPosByVMercenary(SI16 *psiX, SI16 *psiY);
	void	GetMovePersentByVMercenary(SI16 *ps);

	void	GetEquip(UI16* lpuiEquip);
	void	SetChangeEffect(void);

	void	SetAttackMode(BOOL b)	{ m_bAttackMode = b; }
	BOOL	IsAttackMode()			{ return m_bAttackMode; }

	void	SetExpoEvent(int nScore, int nRank, BOOL b) { m_nScore = nScore, m_nRank = nRank; m_bInvalidBattle = b; }

	void	SetScore(int nScore){ m_nScore = nScore; }
	void	SetRank(int nRank)	{ m_nRank = nRank; }
	int		GetScore()			{ return m_nScore; }
	int		GetRank()			{ return m_nRank; }
	BOOL	IsInvalidBattle()	{ return m_bInvalidBattle; }
	BOOL	ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis);
	
	// actdoll (2004/07/20 14:21) : �ʵ� �� �̵��ӵ� �����ų �� �ִ� �Լ�
	void	SetMoveSpeed( SI16 siMoveSpeed )		{ m_siMoveSpeed = siMoveSpeed; }
	SI16	GetMoveSpeed( void )					{ return m_siMoveSpeed; }

public:
	// linus
	void	SetSiegeWarfare_Hp(SI16	siFieldHp);	// ������ ���� ���� ü��ġ ����
	void	SetSiegeWarfare_Damage(SI16 siDamage);		// ������ ������ ü��ġ ���̱�
	SI16	GetSiegeWarfare_Hp();						// ������ ������ ���� ü��ġ ���
	BOOL	IsFieldAlive();

	// ���� /////////////////////////////////////////
	// robypark 2004/10/27 15:5
	// ������ ���� �ִ� ü��ġ ����
	void	SetSiegeWarfare_MaximumHP(SI16 siMaximumHP);
	// ������ ���� �ִ� ü��ġ ���
	SI16	GetSiegeWarfare_MaximumHP(void);
	// ������ ���� ����ǰ ���緮 ����
	void	SetSiegeWarfare_CurrentSupplyGoodsAmount(SI32 siCurrentSupplyGoodsAmount);
	// ������ ���� ����ǰ ���緮 ���
	SI32	GetSiegeWarfare_CurrentSupplyGoodsAmount(void);
	// ������ ���� ����ǰ �ִ뷮 ����
	void	SetSiegeWarfare_SupplyGoodsAmount(SI32 siSupplyGoodsAmount);
	// ������ ���� ����ǰ �ִ뷮 ���
	SI32	GetSiegeWarfare_SupplyGoodsAmount(void);

	// ������ ������ ���� ���� �ð� ����
	void	SetSiegeWarfareUnit_LastAttackDelay(DWORD dwLastAttackDelay);
	// ������ ������ ����� ���� ���� �ð� ����
	void	SetSiegeWarfareUnit_PastAttackDelay(DWORD dwPastAttackDelay);
	// ������ ������ ���� ���� �ð� ���
	DWORD	GetSiegeWarfareUnit_LastAttackDelay(void);
	// �� /////////////////////////////////////////

public:
	// robypark 2004/11/2 17:55
	// ������ �������� ���� ȿ�� ���
	void	SetSiegeWarfare_TransformEffect(SI32 siSiegeWarfareTransformType);

	// robypark 2004/11/2 18:0
	// ������ ���� ����ǰ �ڵ� �Ҹ�(1%)
	void	WasteSiegeWarfareSupplyGoods(void);

	// robypark 2004/12/10 11:41
	// ������ ������ ������ ���� ���ݹ޴� ȿ�� ����
	void	SetSiegeWarfare_VillageCountAttack(SI32 siEffectVillageConterAttackType);

	// robypark 2004/12/2 19:2
	// ������ ������ ����ǰ ���� ȿ�� ����. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	void	SetSiegeWarfare_EffectSupplyGoods(SI32 siEffectSupplyGoodsType);

	// robypark 2004/11/24 18:31
	// ������ �߰��� ��� ȿ�� ���
	void	SetSiegeWarfare_Evasion(void);

	// robypark 2004/11/24 18:31
	// ������ ġ���� ���� ȿ�� ���
	void	SetSiegeWarfare_DeathBlow(void);

	void	SetCaptain(BOOL	bCaptain);
	BOOL	GetCaptain();

//	void	SetSupplyCharge(SI16 siSupply);
//	SI16	GetSupplyCharge();

public:
	// robypark 2004/11/12 21:15
	// ������ ������  ü�� �� ����ǰ ��ġ(%) �׸���
	void	DrawSiegeWarfareUnit_StatusBar(void);

	// robypark 2004/12/16 14:33
	// ������ �����ΰ�?
	BOOL	IsSiegeWarfareUnit(void);

	// robypark 2004/12/17 17:20
	// ����ڰ� ���� ���� �������̽� �� �ִ°�?
	BOOL	IsInVillageDefenceInterface(void);

	// robypark 2004/12/17 17:20
	// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�? ����
	void	SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface);

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ���
	void	GetCharacterDrawCenterPos(SI16 &siCenterX, SI16 &siCenterY);

	// robypark 2005/2/17 21:44
	// ���콺 Ŀ�� �Ʒ� ĳ���������� �����Ѵ�.
	void	SetCursorHoverCharacter(BOOL bHover);

protected:
	// robypark 2004/11/4 16:5
	// ������ ���� �ٴ� ����Ʈ �׸���
	// [IN] SI08 scForceRelation : ����� ĳ���Ϳ� �׸����� ĳ������ ����
	//								SIEGEWARFARE_FORCE_FRIENDLY(0)	- ���� ��ܿ��̴�.
	//								SIEGEWARFARE_FORCE_ENEMY(1)		- ���� ��ܿ��̴�.
	void	DrawSiegeWarfareForceEffect(SI08 scForceRelation, SI32 siCenterX, SI32 siCenterY);
};		

#endif
