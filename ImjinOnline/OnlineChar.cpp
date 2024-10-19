/************************************************************************************************
	���ϸ� : OnlineChar.cpp
	����� : ������

	���� Game���� �����ϴ� Player�� ������ �����Ѵ�.
************************************************************************************************/
#include <GSL.h>
#include <Main.h>
#include <DebugTool.h>

#include "OnlineWorld.h"
#include "ChattingServer\\HQChattingServerClientSocket.h"
#include "charanimation.h"
#include <kindinfo.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <Player.h>

#include "OnlineCharActionCode.h"
#include "OnlineMath.h"
#include "OnlineMegaText.h"
#include <colortable.h>
#include "..\Gersang\Music.h"
#include "OnlineCharKI.h"
#include "OnlineCommonStruct.h"
#include "OnlineTrade.h"
#include "OnlineBooth.h"


#include <graphics16.h>

#include "OnlineChar.h"
#include "OnlineMyData.h"
#include "OnlineClient.h"
#include "OnlineItem.h"
#include "OnlineResource.h"
#include "OnlineCharUI.h"
#include "..\Gersang\CharInfo\CharFileLoadManager.h"
#include <myfile.h>

// ���� ���� ������
#include	"SiegeUnitTableMgr.h"

//-----------------------------------------------------------------------------
// actdoll (2004/03/11 13:58) : ũ��Ƽ�� ���׸� ������� ����׿� ���� ���� Ŭ����
//-----------------------------------------------------------------------------
#include	"FieldHeroInfoMgr.h"

extern	_KindInfo					KI[];								// ĳ���� ������ ������ 

const	R32		DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS	= 1.5f;		// �ð��� �Ź� ������ ���� �� �̵� �ӵ�(������ 1.5��)

// robypark 2004/10/28 15:3
// ��ġ�� ������ ���·� �׷��ش�.
extern void DrawGauge(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight, R32 fRate, UI08 uiClrGaugeFilled, UI08 uiClrGaugeEmpty);

// robypark 2005/1/7 18:41
// ��ġ(10%������)�� ������ ���·� �׷��ش�.
// ������ ���� ü��, ���� ���������� ���
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// robypark 2005/1/7 18:41
// ��ġ�� ������ ���·� �׷��ش�.
// ���� ����, ���� ������ ��¿��� ���
extern void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
cltOnlineChar::cltOnlineChar()
{
	m_fShow      = TRUE;
	m_bOpenBooth = FALSE;	

	uiUniqueID	 = 0;
	siID		 = 0;
	dwIP         = 0;

	sifDataType = 0;
	
	ZeroMemory(m_szGuild, ON_GUILDNAME_LENGTH + 1);
	m_siGuildInGrade     = 0;
	m_siGuildFlag        = 0;
	m_uiIsHostilityGuild = 0;

	m_BattleFlag     = 0;
	m_FindTargetFlag = 0;

	siKind       = 0;

	m_Imoticon_StartCount = 0;
	m_Imoticon_Type       = 0;

	m_EquitWeapon = 0;

	m_MercenaryKind = 0;

	m_ChangeEffectSex   = 0;
	m_ChangeEffectFrame = 0;

	// robypark 2004/11/9 21:34
	m_siSiegeWarfareTransformType = 0;	// ������ �������� ���� �������� ��Ÿ���� �÷���
	m_siEffectTransformFrame = 0;		// ������ ���� ���� ������

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// ������ ������ ����ǰ ���� ȿ��. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	m_siEffectSupplyGoodsFrame = 0;		// ������ ���� ����ǰ ���� �ִϸ��̼� ������
	
	m_bSiegeWarfareEvasion = FALSE;		// ������ ���� �߰��� ��� ����Ʈ ��뿩��
	m_siEffectEvasionFrame = 0;			// ������ ���� �߰��� ��� ����Ʈ ������

	m_bSiegeWarfareDeathblow = FALSE;	// ������ ���� ġ���� ���� ����Ʈ ��뿩��
	m_siEffectDeathblowFrame = 0;		// ������ ���� ġ���� ���� ����Ʈ ������

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// ������ ���� �ݰ� Ÿ��
	m_siEffectVillageConterAttackFrame	= 0;		// ������ ���� �ݰ� �ִϸ��̼� ������

	// robypark 2004/12/16 10:42
	// ĳ������ ��ǥ�� �����Ͽ������� ��Ÿ���� �÷���
	m_bIsSendedPlayerPos = FALSE;

	m_nScore	= 0;
	m_nRank		= 0;
	m_bInvalidBattle = FALSE;
	ZeroMemory(uiEquip, sizeof(UI16)*WEAR_ITEM_NUM);

	m_bCaptain = FALSE;

	m_siPositionGaugeX = m_siPositionGaugeY = 0;

	// robypark 2004/12/16 15:2
	m_bIsSiegeWarfareUnit	= FALSE;					// ������ ���������� ��Ÿ���� �÷���

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�?

	m_bIsRefugee		= FALSE;
	m_siRefugeeKind		= 0;

	// robypark 2005/1/27 11:1
	m_siAnimationResourceID_Blue	=	-1;			// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	m_siAnimationResourceID_Red		=	-1;			// �ش� ������ ���� �̹��� ����. ���� ������ ��

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// �� ĳ���Ϳ� ����ڿ��� ����

	// ������ �����������̽� ���� ���� �� ĳ���� �ִϸ��̼�. ��ġ��
	m_scInVillageDefenceInterfaceAniType	=	VDI_ANIMATION_TYPE_REPAIR_HAMMER;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��
	m_scCurrentMoveActionSkip	= 0;				// �̵� ó�� �Լ� Move() ������	����. ������ skip ȿ��

	m_bCursorHoverCharacter	= FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
cltOnlineChar::~cltOnlineChar()
{
	Free(); 

	m_fShow      = TRUE;

	uiUniqueID	 = 0;
	siID		 = 0;
	dwIP         = 0;
	
	sifDataType = 0;

	ZeroMemory(m_szGuild, ON_GUILDNAME_LENGTH);
	m_siGuildInGrade     = 0;
	m_siGuildFlag        = 0;
	m_uiIsHostilityGuild = 0;

	m_BattleFlag     = 0;
	m_FindTargetFlag = 0;

	siKind       = 0;

	m_Imoticon_StartCount = 0;
	m_Imoticon_Type       = 0;

	m_EquitWeapon = 0;

	m_MercenaryKind = 0;

	m_ChangeEffectSex   = 0;
	m_ChangeEffectFrame = 0;

	// robypark 2004/11/9 21:34
	m_siSiegeWarfareTransformType = 0;	// ������ �������� ���� �������� ��Ÿ���� �÷���
	m_siEffectTransformFrame = 0;		// ������ ���� ���� ������

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// ������ ������ ����ǰ ���� ȿ��. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	m_siEffectSupplyGoodsFrame = 0;	// ������ ���� ����ǰ ���� �ִϸ��̼� ������

	m_bSiegeWarfareEvasion = FALSE;		// ������ ���� �߰��� ��� ����Ʈ ��뿩��
	m_siEffectEvasionFrame = 0;			// ������ ���� �߰��� ��� ����Ʈ ������

	m_bSiegeWarfareDeathblow = FALSE;	// ������ ���� ġ���� ���� ����Ʈ ��뿩��
	m_siEffectDeathblowFrame = 0;		// ������ ���� ġ���� ���� ����Ʈ ������

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// ������ ���� �ݰ� Ÿ��
	m_siEffectVillageConterAttackFrame	= 0;		// ������ ���� �ݰ� �ִϸ��̼� ������

	m_bCaptain = FALSE;

	// robypark 2004/12/16 10:42
	// ĳ������ ��ǥ�� �����Ͽ������� ��Ÿ���� �÷���
	m_bIsSendedPlayerPos = FALSE;

	// robypark 2004/12/16 15:2
	m_bIsSiegeWarfareUnit	= FALSE;					// ������ ���������� ��Ÿ���� �÷���

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�?

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ����
	m_siDrawCenterX	=	0;
	m_siDrawCenterY	=	0;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��
	m_scCurrentMoveActionSkip	= 0;				// �̵� ó�� �Լ� Move() ������	����. ������ skip ȿ��

	m_bCursorHoverCharacter	= FALSE;
}


//----------------------------------------------------------------------------------------------------------------
//	����		:	�ʱ�ȭ.
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::Init(UI16 uiUniqueID, SI32 siID, CHAR *pszID, UI16 uiCharID, SI16 siX, SI16 siY, DWORD IP, float Ranking, SI32 FightPower, cltOnlineWorld *pWorld,UI16* lpuiEquipWeapon)
{	
	SI32	siCharKindIndex;
	UI32	uiName;	
	POINT	ptCharPos;	
	SI32	i;	
	SI32	filexsize, fileysize;
	SI32	TempXSize, TempYSize;

	ZeroMemory(uiEquip, sizeof(UI16)*WEAR_ITEM_NUM);

	//DWriteStringToFile("cltOnlineChar::Init 1");
	m_EquitWeapon = 0;

	pMyOnlineWorld				=	pWorld;
	m_fShow                     =   TRUE;
	cltOnlineChar::siID			=	siID;	
	cltOnlineChar::uiUniqueID	=	uiUniqueID;	
	m_bFieldAttackAni			=   FALSE;
	
	ZeroMemory(szCharName, sizeof(szCharName));
	ZeroMemory(m_szGuild, ON_GUILDNAME_LENGTH + 1);
	m_siGuildInGrade     = 0;
	m_siGuildFlag        = 0;
	m_uiIsHostilityGuild = 0;
	dwIP                 = 0;

	// ������ ���� �� �ʱ�ȭ
	m_siFieldHp										= 0;	// ������ ������ ���� ü��
	m_siFieldHp_Maximum								= 0;	// ������ ������ �ִ� ü��
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount	= 0;	// ������ ���� ����ǰ ���緮
	m_siSiegeWarfareUnit_SupplyGoodsAmount			= 0;	// ������ ���� ����ǰ �ִ뷮
	m_dwSiegeWarfareUnit_LastAttackDelay			= 0;	// ������ ���� ���� �����ð�
	m_bCaptain										= FALSE;

	// robypark 2004/11/9 21:34
	m_siSiegeWarfareTransformType = 0;	// ������ �������� ���� �������� ��Ÿ���� �÷���
	m_siEffectTransformFrame = 0;		// ������ ���� ���� ������

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// ������ ������ ����ǰ ���� ȿ��. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	m_siEffectSupplyGoodsFrame = 0;	// ������ ���� ����ǰ ���� �ִϸ��̼� ������

	m_bSiegeWarfareEvasion = FALSE;		// ������ ���� �߰��� ��� ����Ʈ ��뿩��
	m_siEffectEvasionFrame = 0;			// ������ ���� �߰��� ��� ����Ʈ ������

	m_bSiegeWarfareDeathblow = FALSE;	// ������ ���� ġ���� ���� ����Ʈ ��뿩��
	m_siEffectDeathblowFrame = 0;		// ������ ���� ġ���� ���� ����Ʈ ������

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// ������ ���� �ݰ� Ÿ��
	m_siEffectVillageConterAttackFrame	= 0;		// ������ ���� �ݰ� �ִϸ��̼� ������

	// robypark 2004/12/16 10:42
	// ĳ������ ��ǥ�� �����Ͽ������� ��Ÿ���� �÷���
	m_bIsSendedPlayerPos = FALSE;

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�?

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ����
	m_siDrawCenterX	=	0;
	m_siDrawCenterY	=	0;

	MoveFlag    = 0;

	sifDataType = 0;

	m_ChangeEffectSex   = 0;
	m_ChangeEffectFrame = 0;

	// ĳ���� ������ ǥ���ϴ� ���� ���� �ʱ�ȭ. 
	NewSelectedFlag		=  FALSE;
	NewSelectedDelay	=  0;

	m_siMoveCharAnimationIndex		= 0;
	m_siWait0CharAnimationIndex		= 0;
	m_siAttackCharAnimationIndex	= 0;
	m_siMagicCharAnimationIndex		= 0;

	// robypark 2005/1/27 11:1
	m_siAnimationResourceID_Blue	=	-1;			// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	m_siAnimationResourceID_Red		=	-1;			// �ش� ������ ���� �̹��� ����. ���� ������ ��

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// �� ĳ���Ϳ� ����ڿ��� ����

	// �����������̽��� �ִ� ĳ������ ��� �����ϰ� �� ���� ���� �ִϸ��̼��� �����ֵ��� �Ѵ�.
	m_scInVillageDefenceInterfaceAniType = rand() % 3;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��
	m_scCurrentMoveActionSkip	= 0;				// �̵� ó�� �Լ� Move() ������	����. ������ skip ȿ��

	m_bCursorHoverCharacter	= FALSE;

	// ĳ���Ͱ� �̵��� �� �ִ� ������ �����Ѵ�.
	if(ISPLAYER(cltOnlineChar::uiUniqueID) == TRUE)
	{
		m_siMoveableTileInfo		=	ON_TILE_KIND_GROUND;
	}
	else if(ISMONSTER(cltOnlineChar::uiUniqueID) == TRUE)
	{
		m_siMoveableTileInfo		=	ON_TILE_KIND_GROUND;
	}
	else if(ISSHIP(cltOnlineChar::uiUniqueID) == TRUE)
	{
		m_siMoveableTileInfo		=	ON_TILE_KIND_SEA;
	}

	m_siPositionGaugeX = m_siPositionGaugeY = 0;

	// �ʿ� �ڱ��� ID�� �����Ѵ�.
	ptCharPos					=	SetID(siX, siY);
	
	m_Imoticon_StartCount = 0;
	m_Imoticon_Type       = 0;

	if(ptCharPos.x != -1 && ptCharPos.y != -1)
	{
		//DWriteStringToFile("cltOnlineChar::Init 1 - 1");		
		//if(ISSHIP(uiUniqueID) == TRUE)
		//	clGrp.Error("�� ��ǥ", "[%d] [%d] [%d] [%d]", siX, siY, ptCharPos.x, ptCharPos.y);

		// ĳ���͸� �ʿ� ���������� �ɾ���.
		siX		=	SI16(ptCharPos.x);
		siY		=	SI16(ptCharPos.y);		

		//m_siFile			=	0;	
		//siFrameNum			=	0;	
		//m_bReverseSwitch	=	FALSE;

		siCharKindIndex		=	pMyOnlineWorld->pIOnlineCharKI->GetIndex(uiCharID);

		if(siCharKindIndex == 0)
			clGrp.Error("", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNKNOWN_CHAR_KIND), siCharKindIndex, uiCharID);

		// ���� �÷��̾��� ��쿡�� ���ڷ� �Ѿ���� spzID�� ����Ѵ�.
		m_BattleFlag        = 0;
		m_FindTargetFlag    = 0;

		siKind              =   uiCharID;
		siClass				=	pMyOnlineWorld->pIOnlineCharKI->GetClass(siCharKindIndex);
		fRanking            =   Ranking;
		siFightPower        =   FightPower;

		if(ISPLAYER(uiUniqueID))
		{
			strncpy(szCharName, pszID, ON_ID_LENGTH);	
			dwIP = IP;

			sifDataType = 1;
		}
		else
		{
			// KindInfo���� ���;� �Ѵ�.
			uiName			=	pMyOnlineWorld->pIOnlineCharKI->GetName(siCharKindIndex);
			strncpy(szCharName, pMyOnlineWorld->pOnlineText->Get(uiName), ON_ID_LENGTH);			
		}

		// ĳ���� �ִϸ��̼� ����Ÿ���� �� ĳ���� �κ��� �����͸� ���´�.	
		//pCharAniInfo	=	pMyOnlineWorld->pOnlineCharAni->GetCharAniInfo(uiCharID);		

		//if(pCharAniInfo == NULL)
		//{
		//	clGrp.Error("�ִϸ��̼� ������ ��µ� �����߽��ϴ�.", "[%d]", uiCharID);
		//}
		
		
		m_siDirection			=	ON_SOUTH;	
		siAniDirection			=	ON_SOUTH;	
		siAniActionCode			=	ON_CHAR_ACTION_CODE_WAIT;		
		
		siPrevX			=	siX;
		siPrevY			=	siY;
		siPrevX1		=	siX;
		siPrevY1		=	siY;
		m_siX			=	siX;
		m_siY			=	siY;
		m_siDX			=	siX;
		m_siDY			=	siY;
		m_siNX			=	siX;
		m_siNY			=	siY;
		m_siMDX			=	siX;
		m_siMDY			=	siY;
		m_siMOX			=	siX;
		m_siMOY			=	siY;
		m_siMX			=	siX;
		m_siMY			=	siY;

		// robypark 2004/12/17 16:52
		// �̵��Ϸ��� ��ǥ�� ������ �����Ͽ��� ���� �����ϰ� �ִ� ����
		m_siPrevSendedNX=	siX;
		m_siPrevSendedNY=	siY;

		bMovingSwitch	=	FALSE;

		// ĳ���� Kind Index ����
		// �÷��̾ ������ ��쿡�� �ִ�.
		if(ISPLAYER(uiUniqueID) || ISMONSTER(uiUniqueID) || ISSHIP(uiUniqueID))
			m_siImKind			=	TransKindOnlineToImjin(siKind);
		else
			m_siImKind			=	0;

		// robypark 2004/12/16 15:2
		m_bIsSiegeWarfareUnit	= pMyOnlineWorld->IsSiegeWarfareUnit(m_siImKind);	// ������ ���������� ��Ÿ���� �÷��� ����

		// actdoll (2005/02/14 16:11) : ����ǳ �Ź� �ӵ��� ���� ������ %������ �����Ͽ� ó��
		SI16	siOriginalMoveSpd	= pMyOnlineWorld->pIOnlineCharKI->GetMoveSpeed(siCharKindIndex);
		if(lpuiEquipWeapon == NULL)
		{
			SetMoveSpeed( siOriginalMoveSpd );
		}
		else
		{
			if(lpuiEquipWeapon[ON_WEAR_SHOES_ITEM] != 0)
			{
				if(pMyOnlineWorld->pItemsInfo->IsTimeLimitItem(lpuiEquipWeapon[ON_WEAR_SHOES_ITEM]))
				{
					// robypark 2004/11/4 11:26
					// ������ ���� �ƴ� ��� �̵��ӵ��� �÷��ִ� ������(����ǳ)�� ������ ���� �ʵ��� ����
					if (FALSE == m_bIsSiegeWarfareUnit)		SetMoveSpeed( siOriginalMoveSpd * DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS );
					else									SetMoveSpeed( siOriginalMoveSpd );
				}
				else
					SetMoveSpeed( siOriginalMoveSpd );
			}
			else
			{
				SetMoveSpeed( siOriginalMoveSpd );	
			}
		}


		m_siMovedPercent	=	0;	

		m_siRX		=	0;
		m_siRY		=	0;	
		bBusySwitch	=	FALSE;

		m_bReverse				= FALSE;
		m_siImFile				= 0;
		m_siFont				= 0;
		m_siAniStep				= 0;
		m_siAniDelayer          = 0;

		m_bIsRefugee			= FALSE;
		m_siRefugeeKind			= 0;

		// robypark 2005/1/17 11:25
		// ������ �ð��̶��,
		if (pMyOnlineWorld->IsInTimeSiegeWarfare())
		{
			// �� ���̶��,
			if (0 == pMyOnlineWorld->pOnlineMap->m_MapIndex)
			{
				// ���ΰ� ���� ������ ������ ���
				CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(siKind);
				// ���ΰ� ĳ���Ͷ��,
				if (NULL != pFieldHeroInfo)
				{
					if ((pFieldHeroInfo->GetRefugeeKind())								// �ǳ��� �ڵ� ����
						&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)	// ����� ĳ���� �ƴ�
						&& (pMyOnlineWorld->pMyData->IsSiegeWarfareDay()))				// ����ڰ� ������ �Ѵٸ�,
					{
						// �ǳ��� ĳ���ͷ� �۵��Ѵ�.
						m_bIsRefugee = TRUE;
						m_siRefugeeKind = pFieldHeroInfo->GetRefugeeKind();
					}
				}
			}
		}

		// �ǳ��� ĳ���Ͱ� �ƴ϶��
		if (FALSE == m_bIsRefugee)
		{
			// ������ ĳ���� ���� �ʱ�ȭ
			if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
			{
				m_siHeroAniAction		= HERO_ACT_N1;
				m_siHeroBaseWClass		= HERO_CLASS_HTH;			
				m_siHeroWeaponEnum		= CHAR_NORMALSPEED;
				SI32 direction			= 0;

				m_siWeaponDrawID		= -1;

				//-----------------------------
				// ����Ʈ �� �ڵ带 �������� �� ���ΰ� ����
				switch( KI[m_siImKind].m_ssNation ) 
				{
				case COUNTRY_KOREA:		strcpy(m_cDefaultBody, "KD01");		break;
				case COUNTRY_JAPAN:		strcpy(m_cDefaultBody, "JD01");		break;
				case COUNTRY_CHINA:		strcpy(m_cDefaultBody, "CD01");		break;
				case COUNTRY_TAIWAN:	strcpy(m_cDefaultBody, "TD01");		break;
				default:				break;
				}

	/*			FILE* fp;
				fp = fopen("ShareFile.txt","at");
				fprintf(fp,"cltOnlineChar::Init()  siID = [%d]\n",siID);
				fclose(fp);*/

				pMyOnlineWorld->pCharFileLoadManager->SetAllChange(KI[m_siImKind].m_ssDefaultArmor, KI[m_siImKind].m_ssDefaultHelmet, 
 					KI[m_siImKind].m_ssDefaultWeapon,  KI[m_siImKind].m_ssHeroID, siID, m_cDefaultBody,TRUE);	


				

				// �̹����� ���� ���� ����� ���Ѵ�. 
				filexsize = 192;
				fileysize = 142;
			}
			// ���� �ƴ� �Ϲ� ĳ����
			else
			{
				SI32 file;
				// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
				for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
				{
					file = KI[m_siImKind].m_clCharAnimation[i].siFile;
					if( file )
					{
						if( Return_Image(file) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
						{
							// ���ʿ��ϰ� ������ �̹����� �����. 
							FreeOldImageByTime();
							
							// �ʿ��� �̹����� �ε��Ѵ�. 
							LoadImgFile(file);
						}
					}
				}

				filexsize	= GetFileXsize(KI[m_siImKind].m_clCharAnimation[1].siFile);
				fileysize	= GetFileYsize(KI[m_siImKind].m_clCharAnimation[1].siFile);

				// robypark 2005/1/27 11:43
				// ������ ������ ���
				CSiegeUnitTableMgr	*pSiegeUnitTableMgr = pMyOnlineWorld->GetSiegeUnitTableMgr();

				LPSSiegeUnitTable lpsSiegeUnitTable = pSiegeUnitTableMgr->GetSiegeUnitTable(siKind);
				if (lpsSiegeUnitTable)
				{
					// robypark 2005/2/3 12:29
					// �ִ� ü��, �ִ� ���� �� ����
					m_siFieldHp_Maximum						= lpsSiegeUnitTable->m_siBasicHP + pSiegeUnitTableMgr->GetIncreaseHP(siFightPower);
					m_siSiegeWarfareUnit_SupplyGoodsAmount	= lpsSiegeUnitTable->m_siBasicMP + pSiegeUnitTableMgr->GetIncreaseMP(siFightPower);

					// robypark 2005/2/15 16:32
					m_scMaxMoveActionSkip		= lpsSiegeUnitTable->m_scMaxMoveActionSkip;		// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��
					m_scCurrentMoveActionSkip	= 0;				// �̵� ó�� �Լ� Move() ������	����. ������ skip ȿ��

					// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
					m_siAnimationResourceID_Blue	=	lpsSiegeUnitTable->m_siAnimationResourceIdx_Blue;

					if( -1 != m_siAnimationResourceID_Blue )
					{
						if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
						{
							// ���ʿ��ϰ� ������ �̹����� �����. 
							FreeOldImageByTime();
							
							// �ʿ��� �̹����� �ε��Ѵ�. 
							LoadImgFile(m_siAnimationResourceID_Blue);
						}
					}
					
					// �ش� ������ ���� �̹��� ����. ���� ������ ��
					m_siAnimationResourceID_Red		=	lpsSiegeUnitTable->m_siAnimationResourceIdx_Red;

					if( -1 != m_siAnimationResourceID_Red )
					{
						if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
						{
							// ���ʿ��ϰ� ������ �̹����� �����. 
							FreeOldImageByTime();
							
							// �ʿ��� �̹����� �ε��Ѵ�. 
							LoadImgFile(m_siAnimationResourceID_Red);
						}
					}
				}
			}

			// ĳ������ ������ ���Ѵ�. 
			TempXSize = filexsize;
			TempYSize = fileysize;

			SI32 siRealImageWidth			= filexsize * (KI[m_siImKind].m_ssXsizePercent)/100;
			SI32 siRealImageHeight			= fileysize * (KI[m_siImKind].m_ssYsizePercent)/100;

			rcRealImage.left			=	(TempXSize - siRealImageWidth)  / 2;
			rcRealImage.right			=	rcRealImage.left + siRealImageWidth;
			rcRealImage.bottom			=	fileysize - KI[m_siImKind].m_ssBottomIndent;
			rcRealImage.top				=	rcRealImage.bottom - siRealImageHeight;

			for( i=1; i<MAX_ANIMATION_PER_UNIT; i++ )
			{
				switch( KI[m_siImKind].m_siCharAnimationInfo[i] )
				{
				case ANI_MOVE:			m_siMoveCharAnimationIndex		= i;			break;
				case ANI_WAIT0:			m_siWait0CharAnimationIndex		= i;			break;
				case ANI_ATTACK:		m_siAttackCharAnimationIndex	= i;			break;
				case ANI_MAGIC:			m_siMagicCharAnimationIndex		= i;			break;
				default:				break;
				}
			}
		}
		else	// �ǳ��� ĳ���ʹ�.
		{
			SI32 file;
			// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
			for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
			{
				file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
				if( file )
				{
					if( Return_Image(file) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
					{
						// ���ʿ��ϰ� ������ �̹����� �����. 
						FreeOldImageByTime();
						
						// �ʿ��� �̹����� �ε��Ѵ�. 
						LoadImgFile(file);
					}
				}
			}

			filexsize	= GetFileXsize(KI[m_siRefugeeKind].m_clCharAnimation[1].siFile);
			fileysize	= GetFileYsize(KI[m_siRefugeeKind].m_clCharAnimation[1].siFile);

			// ĳ������ ������ ���Ѵ�. 
			TempXSize = filexsize;
			TempYSize = fileysize;

			SI32 siRealImageWidth			= filexsize * (KI[m_siRefugeeKind].m_ssXsizePercent)/100;
			SI32 siRealImageHeight			= fileysize * (KI[m_siRefugeeKind].m_ssYsizePercent)/100;

			rcRealImage.left			=	(TempXSize - siRealImageWidth)  / 2;
			rcRealImage.right			=	rcRealImage.left + siRealImageWidth;
			rcRealImage.bottom			=	fileysize - KI[m_siRefugeeKind].m_ssBottomIndent;
			rcRealImage.top				=	rcRealImage.bottom - siRealImageHeight;

			for( i=1; i<MAX_ANIMATION_PER_UNIT; i++ )
			{
				switch( KI[m_siRefugeeKind].m_siCharAnimationInfo[i] )
				{
				case ANI_MOVE:			m_siMoveCharAnimationIndex		= i;			break;
				case ANI_WAIT0:			m_siWait0CharAnimationIndex		= i;			break;
				case ANI_ATTACK:		m_siAttackCharAnimationIndex	= i;			break;
				default:				break;
				}
			}
		}

		m_siCurrAni		= m_siWait0CharAnimationIndex;


		InitSpeech();

		// ��� ���۸� �ʱ�ȭ �Ѵ�.
		ClearOrder();	

		// ���� ����� �ʱ�ȭ���ش�.
		ZeroMemory(&CurrentOrder, sizeof(CurrentOrder));
		CurrentOrder.siOrder	=	ON_ORDER_WAIT;		

		// �ִϸ��̼��� �����Ѵ�.		
		UpdateAnimation(TRUE, NULL, NULL);	

		AddOrder(ON_ORDER_WAIT, 0, 0);	

		pMyOnlineWorld->pOnlineMegaText->CreateGDIPage(uiUniqueID);						

		return TRUE;
	}
	else
	{

		// cltOnlineChar::uiUniqueID�� 0���� �����Ѵ�.
		cltOnlineChar::uiUniqueID	=	0;

		// ĳ���͸� �ʿ� ������ �� ����.
		return FALSE;
	}

	m_MercenaryKind = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����		:	���� Item ����
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::SetEquipItem(UI16* lpuiEquipWeapon)
{
	SI16		siEquip;

	pMyOnlineWorld->pItemsInfo->GetItemInfo( lpuiEquipWeapon[0], siEquip );
	if(lpuiEquipWeapon[0] == 0)		
		SetEquip(FALSE, ON_WEAR_HELMET_ITEM, 0, FALSE);
	else							
		SetEquip(TRUE,  ON_WEAR_HELMET_ITEM, siEquip, FALSE);

	pMyOnlineWorld->pItemsInfo->GetItemInfo( lpuiEquipWeapon[1], siEquip );
	if(lpuiEquipWeapon[1] == 0) 	
		SetEquip(FALSE, ON_WEAR_WEAPON_ITEM, 0, FALSE);
	else	
		SetEquip(TRUE,  ON_WEAR_WEAPON_ITEM, siEquip, FALSE);
			
	pMyOnlineWorld->pItemsInfo->GetItemInfo( lpuiEquipWeapon[2], siEquip );
	if(lpuiEquipWeapon[2] == 0) 	
		SetEquip(FALSE, ON_WEAR_ARMOR_ITEM,  0, FALSE);
	else							
		SetEquip(TRUE,  ON_WEAR_ARMOR_ITEM,  siEquip, FALSE);

	CopyMemory(uiEquip, lpuiEquipWeapon, sizeof(UI16) * WEAR_ITEM_NUM);
	m_EquitWeapon = lpuiEquipWeapon[1];

	return TRUE;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����		:	ĳ���͸� ���ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::Free()
{
//	DWriteStringToFile("Free :[%u]", uiUniqueID);
	
	if(uiUniqueID != 0)
	{
		// Chatting Server�� �˸�
		if(ISPLAYER(uiUniqueID))
		{
/*			FILE* fp;
			fp = fopen("ShareFile.txt","at");
			fprintf(fp,"cltCharOrg::Free() [%d] \n",siID);
			fclose(fp);*/
			//ĳ���͸� Free()�Ҷ� ���������ڰ� ��� ������ �ʱ�ȭ �����ִ��� ���θ� �Ǵ��Ѵ�.
			//������ �ӽ÷� ����.. ���⼭ ��� 
			pMyOnlineWorld->pCharFileLoadManager->SetAllChange(KI[m_siImKind].m_ssDefaultArmor, KI[m_siImKind].m_ssDefaultHelmet, 
				KI[m_siImKind].m_ssDefaultWeapon,  KI[m_siImKind].m_ssHeroID, siID, m_cDefaultBody,FALSE);

		}
//		else
//		{
//			FILE* fp;
//			fp = fopen("ShareFile.txt","at");
//			fprintf(fp,"uiUniqueID ===!!!!!!!!!!!!!!!!!!!!!!!!\n",siID);
//			fclose(fp);
//		}

		// �ʿ��� ID�� �����Ѵ�.
		DelID();

		// �׸��� ���� �����ص� �ڸ��� �ִٸ� �� �ڸ��� ������ �ش�.
		if(m_siX != m_siMX || m_siY != m_siMY)			
		{
			// ���� �����ߴ� �ڸ��� ���ش�.			
			pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siMX, m_siMY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, FALSE);
		}

		pMyOnlineWorld->pOnlineMegaText->Free(uiUniqueID);

		m_fShow      = TRUE;
		uiUniqueID	 = 0;
		siID		 = 0;	
		dwIP         = 0;

		sifDataType = 0;

		m_BattleFlag     = 0;
		m_FindTargetFlag = 0;

		siKind       = 0;

		m_Imoticon_StartCount = 0;
		m_Imoticon_Type       = 0;

		ZeroMemory(szCharName, sizeof(szCharName));	
		ZeroMemory(m_szGuild, ON_GUILDNAME_LENGTH + 1);
		m_siGuildInGrade     = 0;
		m_siGuildFlag        = 0;
		m_uiIsHostilityGuild = 0;

		m_EquitWeapon = 0;

		m_MercenaryKind = 0;

		m_ChangeEffectSex   = 0;
		m_ChangeEffectFrame = 0;
		MoveFlag    = 0;

		NewSelectedFlag		=  FALSE;
		NewSelectedDelay	=  0;
		m_Imoticon_StartCount = 0;
		m_Imoticon_Type       = 0;

		InitSpeech();
		
		// ��� ���۸� �ʱ�ȭ �Ѵ�.
		ClearOrder();	
		
		// ���� ����� �ʱ�ȭ���ش�.
		ZeroMemory(&CurrentOrder, sizeof(CurrentOrder));
		CurrentOrder.siOrder	=	ON_ORDER_WAIT;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����		:	ĳ���͸� ȭ�鿡 �׸���.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::Draw(SI16 WaterP)
{
	if(m_fShow == FALSE) 
	{
		return;
	}

	// �ǳ��� ĳ���� ó���� ���
	if (m_bIsRefugee)
	{
		DrawRefugee(WaterP);
		return;
	}

	SI32	siStartX,  siStartY;
	SI32	siCenterX, siCenterY;
	SI32	siRelateX,	siRelateY;
	SI32	siCurrentMapX, siCurrentMapY;
	RECT	rcScreen;
	
	SI32 file;

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	for( SI32 i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[m_siImKind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				// �ʿ��� �̹����� �ε��Ѵ�. 
				LoadImgFile(file);
			}
		}
	}

	// robypark 2005/1/27 11:49
	// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	if( -1 != m_siAnimationResourceID_Blue )
	{
		if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
		{
			// �ʿ��� �̹����� �ε��Ѵ�. 
			LoadImgFile(m_siAnimationResourceID_Blue);
		}
	}

	// robypark 2005/1/27 11:49
	// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	if( -1 != m_siAnimationResourceID_Red )
	{
		if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
		{
			// �ʿ��� �̹����� �ε��Ѵ�. 
			LoadImgFile(m_siAnimationResourceID_Red);
		}
	}

	// ȭ�鿡 ����� ��ġ�� ����Ѵ�.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// ���� ���� ������ ��Ŀ���� ���߰� �ִٸ�
	// RX, RY�� �������� �ʴ´�.
	if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() == uiUniqueID)
	{
		siRelateX = 0;	
		siRelateY = 0; 	
	}
	else
	{			
		siRelateX = pMyOnlineWorld->pOnlineMap->GetRX() - m_siRX;
		siRelateY = pMyOnlineWorld->pOnlineMap->GetRY() - m_siRY; 	
	}

	rcScreen	=	pMyOnlineWorld->pOnlineMap->GetScreenRect();

	siCenterX = 
		 siDrawX * ONLINE_TILE_XSIZE_HALF
		-siDrawY * ONLINE_TILE_XSIZE_HALF
		+(rcScreen.right - rcScreen.left + 1) / 2	- siRelateX;
	
	siCenterY = 
 		 siDrawX * ONLINE_TILE_YSIZE_HALF 
		+siDrawY * ONLINE_TILE_YSIZE_HALF 
		+(rcScreen.bottom - rcScreen.top + 1) / 2	- siRelateY ;

	// �Ϲ� ĳ����(�ǳ����� �ƴ�) ���
	// ������ Character ȭ�鿡 ������ ���� ��ġ�� �ٽ� ����Ѵ�.
	siStartX			=	siCenterX - m_siXSize / 2;
	siStartY			=	siCenterY - m_siYSize + KI[m_siImKind].m_ssBottomIndent;

	// ��縦 ���� ��ǥ�� ���Ѵ�.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 90;

	// �̸��� ���� ��ǥ�� ���Ѵ�.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ����
	m_siDrawCenterX	=	siCenterX;
	m_siDrawCenterY	=	siCenterY - 10;

	XSPR*		lpGuildClassEffect;

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// �� ĳ���Ϳ� ����ڿ��� ����

	// ��� ���޿� ���� ǥ��
	lpGuildClassEffect = NULL;

	// robypark 2004/10/27 17:22
	// ������ ������ �ƴ� ��� ��� ���� ǥ���� ���ش�. ���� �������̽��� �ִ� ��쿡��
	if (FALSE == m_bIsSiegeWarfareUnit)
	{
		if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		{
			// ���
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HNS);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		{
			// ���
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DAI);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
		{
			// �ൿ����
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HND);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
		{
			// �����
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DHS);
		}
	}
	// ������ ������ �ٴ� ����Ʈ ó��
	else
	{
		// ��ܿ� ������ ���� ����� ĳ���Ͷ��,
//		if (pMyOnlineWorld->pMyData->GetGuildName())
		//if (FALSE == m_bIsInVillageDefenceInterface)	// ���� �������̽��� ���� ���� ĳ������ ��츸 �׸�
		{
			// robypark 2004/11/9 19:49
			// ������ ������ ���� ��� �� ���
			// ����ȿ��(���ݷ� ��), �������� ���� ȿ�� ó�� �߰�
			SI16 scGeneralEffect = pMyOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect(GetUniqueID());

			// ���� ������� �˻�
			if (TRUE == GetIsHostilityGuild())
			{
				// ���� ����̹Ƿ� ���� �� �ٴ� ����Ʈ�� ����Ѵ�.
				m_scForceRelation = SIEGEWARFARE_FORCE_ENEMY;
			}
			// ���� ������� �˻�
			else if (((char*)GetGuildName()) && (pMyOnlineWorld->pMyData->GetGuildName()))
			{
				if (strcmp((char*)GetGuildName(), pMyOnlineWorld->pMyData->GetGuildName()) == 0)
				{
					// ���� ����̹Ƿ� �Ķ� �� �ٴ� ����Ʈ�� ����Ѵ�.
					m_scForceRelation = SIEGEWARFARE_FORCE_FRIENDLY;
				}
			}
			else
			{
				// �ٸ� ��� ���� ����
				m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;
			}

			// �߸� ��ܿ��� ��� �ٴ� ����Ʈ �ȱ׸���.
			if (SIEGEWARFARE_FORCE_UNRELATED != m_scForceRelation)
			{
				// ����ȿ��(���ݷ� ��)�� ���
				if (ON_ATTACK_TYPE_NEARCAPTAIN == scGeneralEffect)
				{
					// ���� ȿ�� ��������Ʈ ���ҽ� ���
					XSPR *pSprEffectGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_BLUE + m_scForceRelation);

					if (NULL != pSprEffectGeneral)
					{
						// ����Ʈ �׸���
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprEffectGeneral->Header.Xsize / 2), siCenterY - (pSprEffectGeneral->Header.Ysize / 2) - 24, 
													pSprEffectGeneral->Header.Xsize, pSprEffectGeneral->Header.Ysize, 
													&pSprEffectGeneral->Image[pSprEffectGeneral->Header.Start[((GetTickCount() / 100) % pSprEffectGeneral->Header.font_number)]]);
					}
				}
				// ������ ���
				else if (ON_ATTACK_TYPE_CAPTAIN == scGeneralEffect)
				{
					// ���� ǥ�� ��������Ʈ ���ҽ� ���
					XSPR *pSprGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_BLUE + m_scForceRelation);

					if (NULL != pSprGeneral)
					{
						// ����Ʈ �׸���
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprGeneral->Header.Xsize / 2), siCenterY - (pSprGeneral->Header.Ysize / 2) - 24, 
													pSprGeneral->Header.Xsize, pSprGeneral->Header.Ysize, 
													&pSprGeneral->Image[pSprGeneral->Header.Start[((GetTickCount() / 100) % pSprGeneral->Header.font_number)]]);
					}
				}
				else	// ��ȯ ���� ���� ������ ���
				{
					// ���� ǥ�� ��������Ʈ ���ҽ� ���
					XSPR *pSprGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_SOLDIER_BLUE_EFFECT + m_scForceRelation);

					if (NULL != pSprGeneral)
					{
						// ����Ʈ �׸���
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprGeneral->Header.Xsize / 2), siCenterY - (pSprGeneral->Header.Ysize / 2) - 24, 
													pSprGeneral->Header.Xsize, pSprGeneral->Header.Ysize, 
													&pSprGeneral->Image[pSprGeneral->Header.Start[((GetTickCount() / 100) % pSprGeneral->Header.font_number)]]);
					}
				}

				// �Ʊ� ���� ǥ��
//				DrawSiegeWarfareForceEffect(m_scForceRelation, siCenterX, siCenterY);
			}
		}
	}

	if(lpGuildClassEffect)
	{
		clGrp.PutSpriteLightImjinCT(siCenterX - (lpGuildClassEffect->Header.Xsize / 2), siCenterY - (lpGuildClassEffect->Header.Ysize / 2), 
								lpGuildClassEffect->Header.Xsize, lpGuildClassEffect->Header.Ysize, 
								&lpGuildClassEffect->Image[lpGuildClassEffect->Header.Start[(GetTickCount() / 100) % lpGuildClassEffect->Header.font_number]]);
	}


//	DrawChar(WaterP, siStartX, siStartY, siCenterX, siCenterY);

	// ��ȣ�� ����Ʈ
	UI16 uiGuardianItem = uiEquip[ON_WEAR_GUARDIAN_ITEM];
	CItemHeader *pItemHeader = pMyOnlineWorld->pItemsInfo->GetItem(uiGuardianItem);
	// ��ȣ�� ����Ʈ�� ������ ������ �ƴ� ��쿡�� �׷������� �Ѵ�. ���� ���� �������̽��� ���� ���� ���
	if ((pItemHeader) && (FALSE == m_bIsSiegeWarfareUnit) && (FALSE == m_bIsInVillageDefenceInterface))
	{
		if(pItemHeader->m_pGuardian)
		{
			pItemHeader->m_pGuardian->m_siKind;
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUARDIAN_EFFECT);

			if(lpGuildClassEffect)
			{
				if(lpGuildClassEffect->Header.font_number > 0)
				{
					clGrp.PutSpriteLightImjinCT(siCenterX - (lpGuildClassEffect->Header.Xsize / 2), siCenterY - lpGuildClassEffect->Header.Ysize - 7, 
						lpGuildClassEffect->Header.Xsize, lpGuildClassEffect->Header.Ysize, 
						&lpGuildClassEffect->Image[lpGuildClassEffect->Header.Start[(GetTickCount() / 100) % lpGuildClassEffect->Header.font_number]]);
				}
			}
		}
	}

	// ���ΰ� ĳ���͸� �׸���.
	DrawChar(WaterP, siStartX, siStartY, siCenterX, siCenterY);

	switch(m_BattleFlag)
	{
		case 1:		// ���� ���� ������̸� �׿� ���� ǥ��
			{
				XSPR*		pWaitEffect;

				pWaitEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEWAITEFFECT);

				clGrp.PutSpriteLightImjinCT(siCenterX - (pWaitEffect->Header.Xsize / 2), siCenterY - 10 - (pWaitEffect->Header.Ysize / 2), 
										pWaitEffect->Header.Xsize, 
										pWaitEffect->Header.Ysize, 
										&pWaitEffect->Image[pWaitEffect->Header.Start[(GetTickCount() / 50) % pWaitEffect->Header.font_number]]);


				// ���� �� ĳ���Ͱ� ���ΰ� ĳ���̸�
				if(pMyOnlineWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
				{
					SI32		TempFrame;
					XSPR*		pCount;

					pCount = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_WARCOUNT);
					if((GetTickCount() - m_BattleFlagStartTickCount) >= ON_MAX_WAITING_BATTLE_TIME)
					{
						TempFrame = pCount->Header.font_number - 1;
					}
					else
					{
						TempFrame = ((GetTickCount() - m_BattleFlagStartTickCount) * pCount->Header.font_number) / ON_MAX_WAITING_BATTLE_TIME;
					}

					clGrp.PutSpriteT(siCenterX - (pCount->Header.Xsize / 2), siPutSpeechY - pCount->Header.Ysize + 20, 
											pCount->Header.Xsize, 
											pCount->Header.Ysize, 
											&pCount->Image[pCount->Header.Start[TempFrame]]);

					// ���� ó��
					TempFrame = 5 - (SI32)(((float)TempFrame / (float)pCount->Header.font_number) * 5.0f);
					if(m_BattleFlagStartCheckCount != TempFrame)
					{
						m_BattleFlagStartCheckCount = TempFrame;

						PushEffect(EFFECT_COUNTDOWN, 0, 0);
					}
				}
			}
			break;
	}

	switch(m_FindTargetFlag)
	{
		case 1:		// ���Ͱ� Target�� ã����
			{
				XSPR*		pFindEffect;

				pFindEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEFINDFFECT);

				clGrp.PutSpriteT(siPutSpeechCX - (pFindEffect->Header.Xsize / 2) + 5, siPutSpeechY - pFindEffect->Header.Ysize, 
										pFindEffect->Header.Xsize, 
										pFindEffect->Header.Ysize, 
										&pFindEffect->Image[pFindEffect->Header.Start[(GetTickCount() / 50) % pFindEffect->Header.font_number]]);
			}
			break;
	}

	// �̸�Ƽ��
	if (m_Imoticon_Type)
	{
		XSPR*		pImoticon;
		SI16		AddHeight;

		pImoticon = pMyOnlineWorld->pOnlineResource->GetxsprImoticon(m_Imoticon_Type - 1);
		if(pImoticon)
		{
			if(pImoticon->Image)
			{
				AddHeight = 0;
				clGrp.PutSpriteT(siPutSpeechCX - (pImoticon->Header.Xsize / 2) + 1, siPutSpeechY - pImoticon->Header.Ysize + AddHeight + 20 + 24, 
										pImoticon->Header.Xsize, 
										pImoticon->Header.Ysize, 
										&pImoticon->Image[pImoticon->Header.Start[((GetTickCount() - m_Imoticon_StartCount) / 50) % pImoticon->Header.font_number]]);
			}
		}
	}
}

// �ǳ��� ��ο�.
VOID	cltOnlineChar::DrawRefugee(SI16 WaterP)
{
	SI32	siStartX,  siStartY;
	SI32	siCenterX, siCenterY;
	SI32	siRelateX,	siRelateY;
	SI32	siCurrentMapX, siCurrentMapY;
	RECT	rcScreen;
	
	SI32 file;

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	for( SI32 i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				// �ʿ��� �̹����� �ε��Ѵ�. 
				LoadImgFile(file);
			}
		}
	}

	// ȭ�鿡 ����� ��ġ�� ����Ѵ�.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// ���� ���� ������ ��Ŀ���� ���߰� �ִٸ�
	// RX, RY�� �������� �ʴ´�.
	if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() == uiUniqueID)
	{
		siRelateX = 0;	
		siRelateY = 0; 	
	}
	else
	{			
		siRelateX = pMyOnlineWorld->pOnlineMap->GetRX() - m_siRX;
		siRelateY = pMyOnlineWorld->pOnlineMap->GetRY() - m_siRY; 	
	}

	rcScreen	=	pMyOnlineWorld->pOnlineMap->GetScreenRect();

	siCenterX = 
		 siDrawX * ONLINE_TILE_XSIZE_HALF
		-siDrawY * ONLINE_TILE_XSIZE_HALF
		+(rcScreen.right - rcScreen.left + 1) / 2	- siRelateX;
	
	siCenterY = 
 		 siDrawX * ONLINE_TILE_YSIZE_HALF 
		+siDrawY * ONLINE_TILE_YSIZE_HALF 
		+(rcScreen.bottom - rcScreen.top + 1) / 2	- siRelateY ;


	// ������ Character ȭ�鿡 ������ ���� ��ġ�� �ٽ� ����Ѵ�.
	siStartX			=	siCenterX - m_siXSize / 2;
	siStartY			=	siCenterY - m_siYSize + KI[m_siRefugeeKind].m_ssBottomIndent;

	// ��縦 ���� ��ǥ�� ���Ѵ�.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 60;

	// �̸��� ���� ��ǥ�� ���Ѵ�.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// robypark 2005/1/14 18:39
	// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ����
	m_siDrawCenterX	=	siCenterX;
	m_siDrawCenterY	=	siCenterY - 10;

	XSPR*		lpGuildClassEffect;

	// ��� ���޿� ���� ǥ��
	lpGuildClassEffect = NULL;

	// robypark 2004/10/27 17:22
	// ������ ������ �ƴ� ��� ��� ���� ǥ���� ���ش�. ���� �������̽��� �ִ� ��쿡��
	if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
	{
		// ���
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HNS);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
	{
		// ���
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DAI);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
	{
		// �ൿ����
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HND);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
	{
		// �����
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DHS);
	}

	if(lpGuildClassEffect)
	{
		clGrp.PutSpriteLightImjinCT(siCenterX - (lpGuildClassEffect->Header.Xsize / 2), siCenterY - (lpGuildClassEffect->Header.Ysize / 2), 
								lpGuildClassEffect->Header.Xsize, lpGuildClassEffect->Header.Ysize, 
								&lpGuildClassEffect->Image[lpGuildClassEffect->Header.Start[(GetTickCount() / 100) % lpGuildClassEffect->Header.font_number]]);
	}

	// ���ΰ� ĳ���͸� �׸���.
	DrawChar(WaterP, siStartX, siStartY, siCenterX, siCenterY);

	switch(m_BattleFlag)
	{
		case 1:		// ���� ���� ������̸� �׿� ���� ǥ��
			{
				XSPR*		pWaitEffect;

				pWaitEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEWAITEFFECT);

				clGrp.PutSpriteLightImjinCT(siCenterX - (pWaitEffect->Header.Xsize / 2), siCenterY - 10 - (pWaitEffect->Header.Ysize / 2), 
										pWaitEffect->Header.Xsize, 
										pWaitEffect->Header.Ysize, 
										&pWaitEffect->Image[pWaitEffect->Header.Start[(GetTickCount() / 50) % pWaitEffect->Header.font_number]]);


				// ���� �� ĳ���Ͱ� ���ΰ� ĳ���̸�
				if(pMyOnlineWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
				{
					SI32		TempFrame;
					XSPR*		pCount;

					pCount = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_WARCOUNT);
					if((GetTickCount() - m_BattleFlagStartTickCount) >= ON_MAX_WAITING_BATTLE_TIME)
					{
						TempFrame = pCount->Header.font_number - 1;
					}
					else
					{
						TempFrame = ((GetTickCount() - m_BattleFlagStartTickCount) * pCount->Header.font_number) / ON_MAX_WAITING_BATTLE_TIME;
					}

					clGrp.PutSpriteT(siCenterX - (pCount->Header.Xsize / 2), siPutSpeechY - pCount->Header.Ysize + 20, 
											pCount->Header.Xsize, 
											pCount->Header.Ysize, 
											&pCount->Image[pCount->Header.Start[TempFrame]]);

					// ���� ó��
					TempFrame = 5 - (SI32)(((float)TempFrame / (float)pCount->Header.font_number) * 5.0f);
					if(m_BattleFlagStartCheckCount != TempFrame)
					{
						m_BattleFlagStartCheckCount = TempFrame;

						PushEffect(EFFECT_COUNTDOWN, 0, 0);
					}
				}
			}
			break;
	}

	switch(m_FindTargetFlag)
	{
		case 1:		// ���Ͱ� Target�� ã����
			{
				XSPR*		pFindEffect;

				pFindEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEFINDFFECT);

				clGrp.PutSpriteT(siPutSpeechCX - (pFindEffect->Header.Xsize / 2) + 5, siPutSpeechY - pFindEffect->Header.Ysize, 
										pFindEffect->Header.Xsize, 
										pFindEffect->Header.Ysize, 
										&pFindEffect->Image[pFindEffect->Header.Start[(GetTickCount() / 50) % pFindEffect->Header.font_number]]);
			}
			break;
	}

	// �̸�Ƽ��
	if (m_Imoticon_Type)
	{
		XSPR*		pImoticon;
		SI16		AddHeight;

		pImoticon = pMyOnlineWorld->pOnlineResource->GetxsprImoticon(m_Imoticon_Type - 1);
		if(pImoticon)
		{
			if(pImoticon->Image)
			{
				AddHeight = 0;
				clGrp.PutSpriteT(siPutSpeechCX - (pImoticon->Header.Xsize / 2) + 1, siPutSpeechY - pImoticon->Header.Ysize + AddHeight + 20 + 24, 
										pImoticon->Header.Xsize, 
										pImoticon->Header.Ysize, 
										&pImoticon->Image[pImoticon->Header.Start[((GetTickCount() - m_Imoticon_StartCount) / 50) % pImoticon->Header.font_number]]);
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���������� �׸���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::DrawTransparent(SI16 WaterP)
{
	if(m_fShow == FALSE) return;

	SI32	siStartX,  siStartY;
	SI32	siCenterX, siCenterY;
	SI32	siRelateX,	siRelateY;
	SI32	siCurrentMapX, siCurrentMapY;
	RECT	rcScreen;
	
	// ȭ�鿡 ����� ��ġ�� ����Ѵ�.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// �ڽ��� �߽����� ȭ�鿡 ����ֹǷ� �ڽ��� ��� �̵����� ����.	
	if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() == uiUniqueID)
	{
		siRelateX = 0;	
		siRelateY = 0; 	
	}
	else
	{			
		siRelateX = pMyOnlineWorld->pOnlineMap->GetRX() - m_siRX;
		siRelateY = pMyOnlineWorld->pOnlineMap->GetRY() - m_siRY; 	
	}

	rcScreen	=	pMyOnlineWorld->pOnlineMap->GetScreenRect();

	siCenterX = 
		 siDrawX * ONLINE_TILE_XSIZE_HALF
		-siDrawY * ONLINE_TILE_XSIZE_HALF
		+(rcScreen.right - rcScreen.left + 1) / 2	- siRelateX;
	
	siCenterY = 
 		 siDrawX * ONLINE_TILE_YSIZE_HALF 
		+siDrawY * ONLINE_TILE_YSIZE_HALF 
		+(rcScreen.bottom - rcScreen.top + 1) / 2	- siRelateY ;

	// ������ Character ȭ�鿡 ������ ���� ��ġ�� �ٽ� ����Ѵ�.
	siStartX	=	siCenterX - m_siXSize / 2;
	siStartY	=	siCenterY - m_siYSize + KI[m_siImKind].m_ssBottomIndent;


	// ��縦 ���� ��ǥ�� ���Ѵ�.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 90;

	// �̸��� ���� ��ǥ�� ���Ѵ�.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// Character�� ȭ�鿡 ��´�.
	if(WaterP >= 100)
	{
		if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
		{
			//DrawHero(DRAW_MODE_TRANSPARENT, siStartX, siStartY, siCenterX, siCenterY);	���ǽ��� �ű���� ���������� ��� ������� �����ϱ� ������ ���Ƶ�.
		}
		else
		{
			if( m_bReverse == FALSE )
				Draw_PutSpriteLightImjinCT(siStartX, siStartY, 		m_siXSize, m_siYSize, m_siImFile, m_siFont);
			else
				Draw_PutSpriteLightImjinCRT(siStartX, siStartY, 		m_siXSize, m_siYSize, m_siImFile, m_siFont);
		}

	}
	else
	{
		if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
		{
			//DrawHero(...);	 ���ǽ��� �ű���� ���������� ��� ������� �����ϱ� ������ ���Ƶ�.
		}
		else
		{
			if( m_bReverse == FALSE )
				Draw_PutSpriteLightImjinCT(siStartX, siStartY + (m_siYSize - ((m_siYSize * WaterP) / 100)), 		
					m_siXSize, (m_siYSize * WaterP) / 100, m_siImFile, m_siFont);
			else
				Draw_PutSpriteLightImjinCRT(siStartX, siStartY + (m_siYSize - ((m_siYSize * WaterP) / 100)), 		
					m_siXSize, (m_siYSize * WaterP) / 100, m_siImFile, m_siFont);
		}
	}
}

// robypark 2004/11/4 16:17
// ������ ���� �ٴ� ����Ʈ ���� ��� ����
#define FORCE_EFFECT_FONT_TOTALFONT_MOVE		13		// �̵��� ������ ���� �ٴ� ����Ʈ ��Ʈ ��
#define FORCE_EFFECT_FONT_TOTALFONT_ATTACK		13		// ���ݽ� ������ ���� �ٴ� ����Ʈ ��Ʈ ��
#define FORCE_EFFECT_FONT_TOTALFONT_WAIT		8		// ���� ������ ���� �ٴ� ����Ʈ ��Ʈ ��
#define FORCE_EFFECT_FONT_BEGIN_MOVE_RED		13		// ȫ�� �̵��� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ
#define FORCE_EFFECT_FONT_BEGIN_ATTACK_RED		0		// ȫ�� ���ݽ� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ
#define FORCE_EFFECT_FONT_BEGIN_WAIT_RED		26		// ȫ�� ���� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ
#define FORCE_EFFECT_FONT_BEGIN_MOVE_BLUE		52		// û�� �̵��� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ
#define FORCE_EFFECT_FONT_BEGIN_ATTACK_BLUE		39		// û�� ���ݽ� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ
#define FORCE_EFFECT_FONT_BEGIN_WAIT_BLUE		65		// û�� ���� ������ ���� �ٴ� ����Ʈ ���� ��Ʈ ��ȣ

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// robypark 2004/11/4 16:5
// ������ ���� �ٴ� ����Ʈ �׸���
// [IN] SI08 scForceRelation : ����� ĳ���Ϳ� �׸����� ĳ������ ����
//								SIEGEWARFARE_FORCE_FRIENDLY(0)	- ���� ��ܿ��̴�.
//								SIEGEWARFARE_FORCE_ENEMY(1)		- ���� ��ܿ��̴�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	cltOnlineChar::DrawSiegeWarfareForceEffect(SI08 scForceRelation, SI32 siCenterX, SI32 siCenterY)
{
	SI16	ssStartFontNo				 = 0;		// ����Ʈ ���� ��Ʈ ��ȣ
	SI16	ssTotalFontNo				 = 1;		// ����Ʈ �� ��Ʈ ��
	XSPR*	lpSiegeWafareUnitForceEffect = NULL;	// ����Ʈ ��������Ʈ ���ҽ�

	switch (scForceRelation)
	{
		// ���� ��ܿ��� ���
		case SIEGEWARFARE_FORCE_FRIENDLY:
			// ���� ĳ���� ���ۿ� ���� ó��
			switch( siAniActionCode )
			{
				case ON_CHAR_ACTION_CODE_MOVE:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_MOVE_BLUE;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_MOVE;
					break;
				case ON_CHAR_ACTION_CODE_WAIT:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_WAIT_BLUE;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_WAIT;
					break;
				case ON_CHAR_ACTION_CODE_ATTACK:
				case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_ATTACK_BLUE;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_ATTACK;
					break;
			}
			break;

		// ���� ��ܿ��� ���
		case SIEGEWARFARE_FORCE_ENEMY:
			// ���� ĳ���� ���ۿ� ���� ó��
			switch( siAniActionCode )
			{
				case ON_CHAR_ACTION_CODE_MOVE:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_MOVE_RED;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_MOVE;
					break;
				case ON_CHAR_ACTION_CODE_WAIT:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_WAIT_RED;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_WAIT;
					break;
				case ON_CHAR_ACTION_CODE_ATTACK:
				case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
					ssStartFontNo	= FORCE_EFFECT_FONT_BEGIN_ATTACK_RED;
					ssTotalFontNo	= FORCE_EFFECT_FONT_TOTALFONT_ATTACK;
					break;
			}
			break;

		// ���е��� �ʴ� ������ ������ ���
		default:
			return;
	}
/*
	// ���ҽ� ���
	lpSiegeWafareUnitForceEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_FORCE_EFFECT);

	if (NULL != lpSiegeWafareUnitForceEffect)
	{
		// ����Ʈ �׸���
		clGrp.PutSpriteLightImjinCT(siCenterX - (lpSiegeWafareUnitForceEffect->Header.Xsize / 2), siCenterY - (lpSiegeWafareUnitForceEffect->Header.Ysize / 2) - 14, 
										lpSiegeWafareUnitForceEffect->Header.Xsize, lpSiegeWafareUnitForceEffect->Header.Ysize, 
										&lpSiegeWafareUnitForceEffect->Image[lpSiegeWafareUnitForceEffect->Header.Start[ssStartFontNo + ((GetTickCount() / 100) % ssTotalFontNo)]]);
	}
*/
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��縦 ����ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if(m_fShow == FALSE) return;

	// �����������̽��� �ִ� ����ڰ� ���� ��� �� ǳ�� �ȱ׸���.
//	if (TRUE == m_bIsInVillageDefenceInterface)
//		return;

	// ���� �ϰ� ���������� ����ش�.
	if(IsSpeech() == TRUE)		
	{		
		pMyOnlineWorld->pOnlineMegaText->Show(uiUniqueID, lpSurface, siPutSpeechCX, siPutSpeechY + 5);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������� �̸��� ����ش�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawName(HDC hdc)
{
	if(m_fShow == FALSE) return;

	SIZE	TextSize;
	SI32	siTextX, siTextY;	
	char	siBuffer[128];
	SI32	AddHeight;
	BOOL	LineAll;
	SI32	TempSPRSize;
	SI32	TempMaxSize;

	// ������� �̸��� ����ش�.
	if(pMyOnlineWorld)
	{
		// ������ ������ �ƴ� ���
		if (FALSE == m_bIsSiegeWarfareUnit)
		{
			CHAR *pCharName	= pMyOnlineWorld->pMyData->GetMyCharName();
			// ����� ��� ���ڻ��� ������� ����ϵ��� �˻�
			if(!strcmp(pCharName, pMyOnlineWorld->pOnlineText->Get(8000495)))	SetTextColor(hdc, RGB(255,255,255));	// ������ ��� �Ͼ��

			AddHeight   = 0;
			LineAll     = FALSE;
			TempSPRSize = 0;
			if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
			{
				XSPR*		lpXSPR;

				lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(m_siGuildFlag);
				if(lpXSPR)
				{
					if(lpXSPR->Image)
					{
						LineAll     = TRUE;
						TempSPRSize = lpXSPR->Header.Ysize;
					}
				}
			}

			if(LineAll)
			{
				// Text Size�� ����
				TempMaxSize = 0;
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));
					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
					TempMaxSize = TextSize.cx;
				}

				// ���̵� ���
				if(pMyOnlineWorld->m_bEvent)
				{
					sprintf((char*)siBuffer, "%s(%d)(%d)", szCharName, GetRank(), GetScore());
				}
				else
				{
					if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)siBuffer, "%s", szCharName);
					else                                                sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);
				}
				GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
				if(TempMaxSize < TextSize.cx) TempMaxSize = TextSize.cx;
				TempMaxSize -= TempSPRSize;
				TempMaxSize /= 2;

				// ��� ���
				AddHeight = -4;
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));

					siTextX		=	siPutNameCX - TempMaxSize;
					siTextY		=	siPutNameY - 8;
					AddHeight   =   4;

					siYetPutNameCX = siTextX;
					siYetPutNameCY = siPutNameY + 5;
					TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
				}

				// ���̵� ���
				if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)siBuffer, "%s", szCharName);
				else                                                sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);

				siTextX		=	siPutNameCX - TempMaxSize;
				siTextY		=	siPutNameY + AddHeight;
				if(siYetPutNameCX > siTextX) siYetPutNameCX = siTextX;

				// actdoll (2004/03/03 18:57) : ���ڰ� �� �Ⱥ��� ����׽ø� �� ���̰� �س��Ҵ�.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
			}	// if(LineAll)
			else
			{
				// ��� ���
				AddHeight = -4;
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));

					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);

					siTextX		=	siPutNameCX - (TextSize.cx / 2);
					siTextY		=	siPutNameY - 8;
					AddHeight   =   4;

					siYetPutNameCX = siTextX;
					siYetPutNameCY = siPutNameY;
					TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
				}

				// ���̵� ���
				if(pMyOnlineWorld->m_bEvent)
				{
					sprintf((char*)siBuffer, "%s(%d)(%d)", szCharName, GetRank(), GetScore());
				}
				else
				{
					if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)siBuffer, "%s", szCharName);
					else                                                sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);
				}

				GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);

				siTextX		=	siPutNameCX - (TextSize.cx / 2);
				siTextY		=	siPutNameY + AddHeight;
				if(siYetPutNameCX > siTextX) siYetPutNameCX = siTextX;

				// actdoll (2004/03/03 18:57) : ���ڰ� �� �Ⱥ��� ����׽ø� �� ���̰� �س��Ҵ�.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// ������ ü�� ǥ��.
//				sprintf(siBuffer,"���� = %d",m_siFieldHp);
//				TextOut(hdc, siTextX + 1, siTextY + 16, siBuffer, strlen(siBuffer));
			}	// if(LineAll) => else
		}	// if (FALSE == m_bIsSiegeWarfareUnit)
		// ������ ������ ���
		else
		{
			CHAR *pCharName	= pMyOnlineWorld->pMyData->GetMyCharName();
			// ����� ��� ���ڻ��� ������� ����ϵ��� �˻�
			if(!strcmp(pCharName, pMyOnlineWorld->pOnlineText->Get(8000495)))	SetTextColor(hdc, RGB(255,255,255));	// ������ ��� �Ͼ��

			AddHeight   = 0;
			LineAll     = FALSE;
			TempSPRSize = 0;
			if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
			{
				XSPR*		lpXSPR;

				lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(m_siGuildFlag);
				if(lpXSPR)
				{
					if(lpXSPR->Image)
					{
						LineAll     = TRUE;
						TempSPRSize = lpXSPR->Header.Ysize;
					}
				}
			}

			if(LineAll)
			{
				// ��� ����� �ִ� ���
				// Text Size�� ����
				TempMaxSize = 0;
				/*
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));
					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
					TempMaxSize = TextSize.cx;
				}
				*/

				// ���̵� ���
				if(pMyOnlineWorld->m_bEvent)
				{
					sprintf((char*)siBuffer, "%s(%d)(%d)", szCharName, GetRank(), GetScore());
				}
				else
				{
					if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)siBuffer, "%s", szCharName);
					else                                                sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);
				}
				GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
				if(TempMaxSize < TextSize.cx) TempMaxSize = TextSize.cx;
				TempMaxSize -= TempSPRSize;
				TempMaxSize /= 2;

				// ��� ���
				AddHeight = -4;
				/**/
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
//					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));

//					siTextX		=	siPutNameCX - TempMaxSize;
//					siTextY		=	siPutNameY - 8;
//					AddHeight   =   4;

					siYetPutNameCX = siPutNameCX + 3;
					siYetPutNameCY = siPutNameY + 5;

					// ������ ���� ������ ��� Ȥ�� ����� �ڽ��� ��쿡�� ����ǰ �������� �׸���.
					// ��� ��� ��ġ ����
					if ((GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
						|| (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS)))
					{
						siYetPutNameCY += 4;
					}
//					TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
				}
				/**/

				// ���̵� ���
				if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL)
				{
					sprintf((char*)siBuffer, "%s", szCharName);
				}
				else
				{
					sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);
				}

				siTextX		=	siPutNameCX - TempMaxSize;
				siTextY		=	siPutNameY + AddHeight;
				if(siYetPutNameCX > siTextX) siYetPutNameCX = siTextX + 3;

				// actdoll (2004/03/03 18:57) : ���ڰ� �� �Ⱥ��� ����׽ø� �� ���̰� �س��Ҵ�.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG

				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// ������ ���� ü��, ���� ������ ��� ��ġ
				m_siPositionGaugeX = siPutNameCX - 26;//siTextX + 1;
				m_siPositionGaugeY = siTextY + 14;
				// ������ ���� ü��, ���� ������ ���
//				DrawGauge(siTextX + 1, siTextY + 14, 75, 4, (float)m_siFieldHp / (float)m_siFieldHp_Maximum, 235, 253);
//				if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
//					DrawGauge(siTextX + 1, siTextY + 19, 75, 4, (float)m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount, 68, 253);
			}	// if(LineAll)
			else
			{
				// ��� ����� ���� ���
				// ��� ���
				AddHeight = -4;
				/**/
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
//					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));

//					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);

//					siTextX		=	siPutNameCX - (TextSize.cx / 2);
//					siTextY		=	siPutNameY - 8;
//					AddHeight   =   4;

					siYetPutNameCX = siPutNameCX + 3;
					siYetPutNameCY = siPutNameY + 5;
//					TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
				}
				/**/

				// ���̵� ���
				if(pMyOnlineWorld->m_bEvent)
				{
					sprintf((char*)siBuffer, "%s(%d)(%d)", szCharName, GetRank(), GetScore());
				}
				else
				{
					if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL)
					{
						sprintf((char*)siBuffer, "%s", szCharName);
					}
					else
					{
						sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);
					}
				}

				GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);

				siTextX		=	siPutNameCX - (TextSize.cx / 2);
				siTextY		=	siPutNameY + AddHeight;
				if(siYetPutNameCX > siTextX) siYetPutNameCX = siTextX + 3;

				// actdoll (2004/03/03 18:57) : ���ڰ� �� �Ⱥ��� ����׽ø� �� ���̰� �س��Ҵ�.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// ������ ���� ü��, ���� ������ ��� ��ġ
				m_siPositionGaugeX = siPutNameCX - 26;//siTextX + 1;
				m_siPositionGaugeY = siTextY + 14;
				// ������ ü��, ����ǰ ǥ��.
//				sprintf(siBuffer,"���� = %d",m_siFieldHp);
//				TextOut(hdc, siTextX + 1, siTextY + 16, siBuffer, strlen(siBuffer));
//				DrawGauge(siTextX + 1, siTextY + 14, 75, 4, (float)m_siFieldHp / (float)m_siFieldHp_Maximum, 235, 253);
//				if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
//					DrawGauge(siTextX + 1, siTextY + 19, 75, 4, (float)m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount, 68, 253);
			}
		}
	}	// if(pMyOnlineWorld)
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���� �̸� �տ� Guild ����� ��´�.(���� ��ǥ�� -1�̸� ID ��� �κ� �տ� ��´�.)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawGuildFlag(SI16 ScreenX, SI16 ScreenY)
{
	if(m_fShow == FALSE) return;

	if((ScreenX == -1000) || (ScreenY == -1000))
	{
		ScreenX = siYetPutNameCX - 15;
		ScreenY = siYetPutNameCY;
	}

	if(m_szGuild[0])
	{
		XSPR*		lpXSPR;

		lpXSPR = pMyOnlineWorld->pOnlineResource->GetxsprFlagM(m_siGuildFlag);
		if(lpXSPR)
		{
			if(lpXSPR->Image)
			{
				clGrp.PutSpriteT(ScreenX - (lpXSPR->Header.Xsize / 2), ScreenY - (lpXSPR->Header.Ysize / 2), lpXSPR->Header.Xsize, lpXSPR->Header.Ysize, &lpXSPR->Image[lpXSPR->Header.Start[(GetTickCount() / 120) % lpXSPR->Header.font_number]]);
			}
		}						
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ĳ���Ͱ� ���� �ϰ� �ִ°�?
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsSpeech()
{
	if(szSpeech[0] != NULL)
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	ĳ������ �������.(Action���� �Ѿ������ ���־�� �� �ϵ��� ���ش�.)
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::OverHead()
{	
	OnOrder	TempOrderBuffer;	
	SI32	siDirection = 0;

	// ��� ���ۿ��� ����� ���´�.
	// �ٸ� ����� ���������� �����Ѵ�.
	// ���� ����� ���� �� �ִ� ��Ȳ�̿��� �Ѵ�.
	if(bBusySwitch == FALSE && IsOrderInBuffer() == TRUE)		 
	{
		while(IsOrderInBuffer() == TRUE)
		{
			GetOrder(&TempOrderBuffer);		

			if(m_bFieldAttackAni == TRUE)	break;

			// ����� �������μ���.
			if(PreProcessorOrder(&TempOrderBuffer) == TRUE)
			{
				CurrentOrder	=	TempOrderBuffer;

				switch(CurrentOrder.siOrder)
				{		
				case ON_ORDER_WAIT:
					// ���� �ϵ��� �����Ѵ�.
					SetMove(m_siX, m_siY);						
					break;
				case ON_ORDER_MOVE:
					// �̵� �ϵ��� �����Ѵ�.
					if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
					{
						if( !pMyOnlineWorld->pOnlineBooth->IsActionIn() )
						{
							SetMove(SI16(CurrentOrder.siOrderParam1), SI16(CurrentOrder.siOrderParam2));
						}
					}
					else
					{
						SetMove(SI16(CurrentOrder.siOrderParam1), SI16(CurrentOrder.siOrderParam2));
					}
					break;
				case ON_ORDER_ATTACK:
					{
						SI16	siAttackCharX, siAttackCharY;

						// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
						pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siAttackCharX, &siAttackCharY);						

						SetMove(siAttackCharX, siAttackCharY);
						//m_siActionCdoe	=	ON_CHAR_ACTION_CODE_MOVE;
					}
					break;
				case ON_ORDER_FIELDATTACK_READY:
					{
						SI16	siTargetCharX, siTargetCharY;

						// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
						pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siTargetCharX, &siTargetCharY);						
						
						if(ManContactvillage(siTargetCharX, siTargetCharY,
											1,1, m_siX,m_siY, KI[m_siImKind].GetAttackRange(0)) == FALSE)
						{
							SetMove(siTargetCharX, siTargetCharY);
						}
						else
						{
							//AddOrder(ON_ORDER_WAIT, 0, 0);
							SetMove(m_siX, m_siY);
						}

//						SetMove(siAttackCharX, siAttackCharY);
					}
					break;
				case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY:
					{						
						SI32	siObjectID;
						SI16	siVillagePosX,siVillagePosY;
						SI16	siVillageSizeX,siVillageSizeY;
						
						// ������Ʈ�� ID�� ���Ѵ�.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);

						// �̵����� �ƴ϶�� �̵��� ��Ų��.						
						// �����ΰ� �˾ƿ´�.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// ���� ���� ��ġ�� ������Ʈ�� �ִ� ��ġ�� 2Ÿ�� ���̰� ���� �̵��Ѵ�.
							/*if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}*/
							pMyOnlineWorld->pIOnlineObject->GetPos(siObjectID,&siVillagePosX,&siVillagePosY);

							pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &siVillageSizeX, &siVillageSizeY);

							if(ManContactvillage(siVillagePosX - siVillageSizeX / 2,
											siVillagePosY - siVillageSizeY /2,
											siVillageSizeX,siVillageSizeY,m_siX,m_siY, KI[m_siImKind].GetAttackRange(0)) == FALSE)
							{
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
							else
							{
								//AddOrder(ON_ORDER_WAIT, 0, 0);
								// ���� ����� �ʱ�ȭ���ش�.
								SetMove(m_siX, m_siY);
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
						}											
						
					}
					break;
				case ON_ORDER_PICKUPITEM:
					{
						SI32	siItemX, siItemY;		
				
						siItemX		=	LOWORD(CurrentOrder.siOrderParam1);
						siItemY		=	HIWORD(CurrentOrder.siOrderParam1);
						
						// �������� �ִ� ��ġ�� �̵��Ѵ�.
						SetMove(siItemX, siItemY);
					}
					break;
				case ON_ORDER_USEOBJECT:			
				case ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE:
					{
						SI32	siObjectID;

						// ������Ʈ�� ID�� ���Ѵ�.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

						// �̵����� �ƴ϶�� �̵��� ��Ų��.						
						// �����ΰ� �˾ƿ´�.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// ���� ���� ��ġ�� ������Ʈ�� �ִ� ��ġ�� 2Ÿ�� ���̰� ���� �̵��Ѵ�.
							if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 2)
							{
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// �̵��϶�� �������ش�.
								//SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);			
							}
						}											

					}					
					// ����� �ٲ��ٸ� Break�� �����Ѵ�.
					break;				

				case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE:
					{
						SI32	siObjectID;
						
						// ������Ʈ�� ID�� ���Ѵ�.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);

						// �̵����� �ƴ϶�� �̵��� ��Ų��.						
						// �����ΰ� �˾ƿ´�.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// ���� ���� ��ġ�� ������Ʈ�� �ִ� ��ġ�� 2Ÿ�� ���̰� ���� �̵��Ѵ�.
							if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// �̵��϶�� �������ش�.
								//SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
						}											

					}					
					// ����� �ٲ��ٸ� Break�� �����Ѵ�.
					break;

				case ON_ORDER_FIELDATTACK_ACTION:
				case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION:
				case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION:
				case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION:
					m_bFieldAttackAni = TRUE;

					siDirection = 0;

					// ���� ��ȯ
					if(m_siX < CurrentOrder.siOrderParam1)
					{	
						siDirection |= ON_EAST;
					}
					else if(m_siX > CurrentOrder.siOrderParam1)
					{
						siDirection |= ON_WEST;
					}

					if(m_siY < CurrentOrder.siOrderParam2)
					{
						siDirection |= ON_SOUTH;
					}
					else if(m_siY > CurrentOrder.siOrderParam2)
					{
						siDirection |= ON_NORTH;
					}	

					if(siDirection)
					{
						ChangeDirection(siDirection);		
					}
					break;
				// robypark 2004/11/9 17:4
				// ������ ���� ���� ������ ����ǰ ��û������
				case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON:
					{
						SI16	siTargetCharX, siTargetCharY;

						// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
						pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siTargetCharX, &siTargetCharY);						
						
						if(ManContactvillage(siTargetCharX, siTargetCharY,
											1,1, m_siX,m_siY, VALID_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == FALSE)
						{
							SetMove(siTargetCharX, siTargetCharY);
						}
						else
						{
							//AddOrder(ON_ORDER_WAIT, 0, 0);
							SetMove(m_siX, m_siY);
						}

//						SetMove(siAttackCharX, siAttackCharY);
					}
					break;
				// robypark 2004/11/9 18:22
				// ������ ���� ���� ������ ������ ����ǰ ��û������
				case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE:
					{
						SI32	siObjectID;
						SI16	siVillagePosX,siVillagePosY;
						SI16	siVillageSizeX,siVillageSizeY;
						
						// ������Ʈ�� ID�� ���Ѵ�.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

						// �̵����� �ƴ϶�� �̵��� ��Ų��.						
						// �����ΰ� �˾ƿ´�.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// ���� ���� ��ġ�� ������Ʈ�� �ִ� ��ġ�� 2Ÿ�� ���̰� ���� �̵��Ѵ�.
							/*if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}*/
							pMyOnlineWorld->pIOnlineObject->GetPos(siObjectID,&siVillagePosX,&siVillagePosY);

							pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &siVillageSizeX, &siVillageSizeY);

							if(ManContactvillage(siVillagePosX - siVillageSizeX / 2,
											siVillagePosY - siVillageSizeY /2,
											siVillageSizeX,siVillageSizeY,m_siX,m_siY, 2) == FALSE)
							{
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
							else
							{
								//AddOrder(ON_ORDER_WAIT, 0, 0);
								SetMove(m_siX, m_siY);

							}

						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// �̵��϶�� �������ش�.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
						}
					}
					break;
				// robypark 2005/1/25 19:8
				// ������ ���� ���� ������ �ٸ� ���� ġ���ϱ�
				case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
					{
						SI16	siTargetCharX, siTargetCharY;

						// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
						pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siTargetCharX, &siTargetCharY);
						
						if(ManContactvillage(siTargetCharX, siTargetCharY,
											1,1, m_siX,m_siY, VALID_HEAL_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == FALSE)
						{
							SetMove(siTargetCharX, siTargetCharY);
						}
						else
						{
							//AddOrder(ON_ORDER_WAIT, 0, 0);
							SetMove(m_siX, m_siY);
						}

//						SetMove(siAttackCharX, siAttackCharY);
					}
					break;
				case ON_ORDER_NPC:
					{
						SI32	siNPCX, siNPCY;
						SI16	siNPCCode;
				
						siNPCX    = LOWORD(CurrentOrder.siOrderParam1);
						siNPCY    = HIWORD(CurrentOrder.siOrderParam1);
						siNPCCode = CurrentOrder.siOrderParam2;

						if(pMyOnlineWorld->pOnlineMap->m_NPCParser.GetNPC(siNPCCode) != NULL)
						{
							// ���� ���� ��ġ�� NPC�� �ִ� ��ġ�� 5Ÿ�� ���̰� ���� �̵��Ѵ�.
							if(max(abs(siNPCX - m_siX), abs(siNPCY - m_siY)) > 5)
							{
								// �̵��϶�� �������ش�.
								SetMove(siNPCX, siNPCY);											
							}
							else
							{
								SetMove(m_siX, m_siY);
							}
						}
						else
						{
							if(siNPCX != m_siX || siNPCY != m_siY)
							{
								// �̵��϶�� �������ش�.
								SetMove(siNPCX, siNPCY);
							}
						}
					}					
					break;
				}
			}
		}
	}

	// ��縦 ó���Ѵ�.
	ActionSpeech();

	// ���� ��ǥ�� ���� ��ǥ�� �ٸ��ٸ� �������� ������ ��ǥ�� �˷��ش�.
	if (IsMyChar() == TRUE)
	{
		// �����̶� �̵�������, ���� �̵��Ѵٰ� �޽����� ������ �ʾҴٸ�,
		if ((m_siMovedPercent != 0) && (FALSE == m_bIsSendedPlayerPos))
		{
			// ���� �����̳� ��Ż�� �巯���� ���� �ƴϸ���
			if(((pMyOnlineWorld->pMyData->IsGotoVillage() && (pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsGotoPortal() && (pMyOnlineWorld->pMyData->IsWaitGotoPortalResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsSelectNPC() && (pMyOnlineWorld->pMyData->IsWaitSelectNPCResponse() == TRUE))
				) == FALSE)
			{
				// robypark 2004/12/17 16:52
				// �̵��Ϸ��� ��ǥ�� ������ �����Ͽ��� ���� �����ϱ� �ʾҴٸ�
				if ((m_siPrevSendedNX != m_siNX) || (m_siPrevSendedNY != m_siNY))
				{
					// ����� ĳ���Ͱ� �̵��Ϸ��� ��ǥ�� ����
					pMyOnlineWorld->pOnlineClient->SendPlayerPos(m_siNX, m_siNY, m_bIsSiegeWarfareUnit);
					m_bIsSendedPlayerPos = TRUE;

					m_siPrevSendedNX	=	m_siNX;
					m_siPrevSendedNY	=	m_siNY;

					// ������ �����̶�� �̵�ȿ���� ���
					if (m_bIsSiegeWarfareUnit)
					{
						// �̵� ȿ������ ����
						if (KI[m_siImKind].m_siMoveEffect)
						{
							// hash table�� �̿��Ͽ� �����ؾ� �Ѵ�.
							// HQ�� charinfo.txt���� ������ �� �̿�
							HashCodePushEffect(KI[m_siImKind].m_siMoveEffect, 0, 0);
						}
					}
				}
			}
		}
		else if (siPrevX != m_siX || siPrevY != m_siY)
		{
			//clGrp.Error("�� ��ġ �޽��� ����", "[%d] [%d]", m_siX, m_siY);

			// ���� �����̳� ��Ż�� �巯���� ���� �ƴϸ���
			if(((pMyOnlineWorld->pMyData->IsGotoVillage() && (pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsGotoPortal() && (pMyOnlineWorld->pMyData->IsWaitGotoPortalResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsSelectNPC() && (pMyOnlineWorld->pMyData->IsWaitSelectNPCResponse() == TRUE))
				) == FALSE)
			{
	/*			UI16 DifX = abs(m_siX-siPrevX);
				UI16 DifY = abs(m_siY-siPrevY);

				if(DifX > 5 || DifY > 5)
					clGrp.Error("Move", "������ǥ�� ���� : DifX(%d), DifY(%d)", DifX, DifY);
	*/
				// �� ĳ������ ��ġ�� ����Ǿ����� ��ġ�� ������ �������� ���� ���
				if (FALSE == m_bIsSendedPlayerPos)
				{
					// �� ĳ������ ���� ������ �������� ������.
					pMyOnlineWorld->pOnlineClient->SendPlayerPos();
				}
				else
					m_bIsSendedPlayerPos = FALSE;	// �ٽ� �ʱ�ȭ

				// ������ �ִ� Ÿ�� ��ǥ�� ����Ǿ���.
				siPrevX1 =  siPrevX;
				siPrevY1 =  siPrevY;

				siPrevX	 =	m_siX;
				siPrevY	 =	m_siY;

				pMyOnlineWorld->pOnlineClientChatting->MessageSend_MoveSell(pMyOnlineWorld->pOnlineMap->m_MapIndex, m_siX, m_siY);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
//	����		:	ĳ������ �׼�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::Action()
{		
	SI32	siMoveResult = ON_MOVE_MOVING;
	BOOL	fEndFrame;
	BOOL	fSound;

	fEndFrame = FALSE;
	fSound    = FALSE;

	// Effect
	if(m_ChangeEffectSex != 0)
	{
		XSPR*		lpSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_CHANGEEFFECT);

		m_ChangeEffectFrame++;
		if(m_ChangeEffectFrame >= (lpSpr->Header.font_number * 3))
		{
			m_ChangeEffectSex = 0;
		}
	}

	// robypark 2004/11/2 13:31
	// ������ ���� ���� �ִϸ��̼� ó��
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// ������ �������� ó�� ���ϴ� ���
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// ������ ������ ���¿��� �����ϴ� ���

		// �ִϸ��̼��� �������� �˻�
		m_siEffectTransformFrame++;
		if (m_siEffectTransformFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_siSiegeWarfareTransformType = 0;
			m_siEffectTransformFrame = 0;
		}
	}

	// robypark 2004/12/2 19:10
	// ������ ���� ����ǰ ���� �ִϸ��̼� ó��
	if (0 != m_siEffectSupplyGoodsType)		// ������ ������ ����ǰ ���� ȿ��. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// ����ǰ ������ ����Ʈ ���ҽ� ���
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// ������ ������ ����ǰ�� �޾Ҵ�
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// ������ ������ ����ǰ�� �ٴڳ���.

		// �ִϸ��̼��� �������� �˻�
		m_siEffectSupplyGoodsFrame++;
		if (m_siEffectSupplyGoodsFrame >= (lpSprEffect->Header.font_number * 3))
		{
			if (EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType)	// ����ǰ �ٴڳ� ȿ���� ��쿡�� ���� �ݺ� �ִϸ��̼ǵȴ�. ���� ���� �� ����
				m_siEffectSupplyGoodsFrame = 0;
			else
				m_siEffectSupplyGoodsType = 0;
		}
	}

	// robypark 2004/12/10 14:53
	// ������ ������ �������� �޴� �ݰ� ���� �ִϸ��̼� ó��
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// ������ ������ �������� �޴� �ݰ� ���� �̹��� ���ҽ� ���
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// ������ ������ ���� �ݰ��� �޾Ҵ�. �⸧
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// ������ ������ ���� �ݰ��� �޾Ҵ�. ��

		// �ִϸ��̼��� �������� �˻�
		m_siEffectVillageConterAttackFrame++;
		if (m_siEffectVillageConterAttackFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_siEffectVillageConterAttackFrame = 0;
			m_siEffectVillageConterAttackType = 0;
		}
	}

	// robypark 2004/11/24 18:47
	// ������ ���� �߰��� ��� ����Ʈ �ִϸ��̼� ó��
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		// �ִϸ��̼��� �������� �˻�
		m_siEffectEvasionFrame++;
		if (m_siEffectEvasionFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_bSiegeWarfareEvasion = FALSE;
			m_siEffectEvasionFrame = 0;
		}
	}

	// robypark 2004/11/24 18:47
	// ������ ���� ġ���� ���� ����Ʈ �ִϸ��̼� ó��
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		// �ִϸ��̼��� �������� �˻�
		m_siEffectDeathblowFrame++;
		if (m_siEffectDeathblowFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_bSiegeWarfareDeathblow = FALSE;
			m_siEffectDeathblowFrame = 0;
		}
	}

	// ��ġ
	if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() == uiUniqueID)
	{
//		DWriteStringToFile("��ġ : [%d] [%d]", m_siX, m_siY);
	}

	// �ʿ��� ���̵� �����Ѵ�.
	DelID();
	//pMyOnlineWorld->pOnlineMap->DelID(m_siX, m_siY, 1, 1, ON_MAP_DELID_CHAR);

	// �켱 ������带 �����Ѵ�.	
	OverHead();	

	// �̸�Ƽ��
	if(m_Imoticon_Type)
	{
		if((GetTickCount() - m_Imoticon_StartCount) > IMOTICON_WAITTIME)
		{
			m_Imoticon_StartCount = 0;
			m_Imoticon_Type       = 0;
		}
	}

	// ���� ���õ� �Ϳ� ���� ó��. 
	if(NewSelectedFlag == TRUE)
	{
		if(pMyOnlineWorld->GetTotalFrame() % 2 == 0)
		{
			if(NewSelectedDelay > 0)
			{
				NewSelectedDelay--;
			}
			else
			{
				SetNewSelectedFlag(FALSE);
			}
		}
	}

	// robypark 2005/2/15 16:38
	// ������ ������ ��� �̵�ó�� ������ ��ŵ�Ͽ� ������ �ֱ�
	m_scCurrentMoveActionSkip++;
	if (m_scCurrentMoveActionSkip >= m_scMaxMoveActionSkip)
	{
		// �̵� ���϶��� �̵��� ó���Ѵ�.	
		siMoveResult = Move();

		m_scCurrentMoveActionSkip = 0;
	}

	if(m_siMovedPercent == 0)			bBusySwitch	=	FALSE;	
	else								bBusySwitch	=	TRUE;		

	// �̵� �Ϸ�Ǿ��ٸ�
	if (ON_MOVE_DONE == siMoveResult)
		m_scCurrentMoveActionSkip = 0;	// �̵� ������ �ʱ�ȭ

	switch(CurrentOrder.siOrder)
	{
	case ON_ORDER_WAIT:
		break;
	case ON_ORDER_MOVE:		
		// ���� �������� �˻�
		if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
		{
			if( !pMyOnlineWorld->pOnlineBooth->IsActionIn() )
			{
				if(m_siMovedPercent < 0)
				{
					if(pMyOnlineWorld->pOnlineBooth->WhererU( m_siX, m_siY ))
					{
						SetMove(m_siX, m_siY);
					}
				}
			}
		}

		if(siMoveResult == ON_MOVE_DONE)
		{	
			// ���ۿ� ����� ���ٸ� ��� ����� ������.
			if(IsOrderInBuffer() == FALSE)			
				AddOrder(ON_ORDER_WAIT, 0, 0);																	
		}				
		break;
	case ON_ORDER_ATTACK:
		{
			SI16	siAttackCharX, siAttackCharY;

			// �켱 �����Ϸ��� ĳ���Ͱ� ��ȿ���� �˾ƿ´�.
			if(pMyOnlineWorld->pIOnlineChar->IsExistUniqueID(CurrentOrder.siOrderParam1) == TRUE)
			{
				// �̵��� �Ϸ�Ǿ��ٸ� ���� ����� �����ش�.
				if(siMoveResult == ON_MOVE_DONE)
				{
//					SetMessage("ON_MOVE_DONE");

					if(IsOrderInBuffer() == FALSE)			
						AddOrder(ON_ORDER_WAIT, 0, 0);
				}
				else
				{
					// ��������� �� ĳ���͸� �����ϰ� �Ѵ�.
					pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siAttackCharX, &siAttackCharY);					
					SetMove(siAttackCharX, siAttackCharY);
				}
			}
			else
			{
				// ĳ���Ͱ� ��ȿ���� �ʰ� 
				// ��� ����� ���ٸ� ���� ��Ų��.
				if(IsOrderInBuffer() == FALSE)
				{
					AddOrder(ON_ORDER_WAIT, 0, 0);											
				}			
			}	
		}
		break;

	case ON_ORDER_FIELDATTACK_READY:
		{
			SI16	siTargetCharX, siTargetCharY;

			// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
			pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siTargetCharX, &siTargetCharY);						
			
			if(ManContactvillage(siTargetCharX, siTargetCharY,
								1,1, m_siX,m_siY, KI[m_siImKind].GetAttackRange(0)) == FALSE)
			{
				SetMove(siTargetCharX, siTargetCharY);
			}
			else
			{
				//AddOrder(ON_ORDER_WAIT, 0, 0);
				SetMove(m_siX, m_siY);
			}
		}
		break;

	case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_READY:
		{
			SI32	siObjectID;
			SI16	siVillagePosX,siVillagePosY;
			SI16	siVillageSizeX,siVillageSizeY;
			
			// ������Ʈ�� ID�� ���Ѵ�.
			siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

			// �̵����� �ƴ϶�� �̵��� ��Ų��.						
			// �����ΰ� �˾ƿ´�.
			if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
			{
				pMyOnlineWorld->pIOnlineObject->GetPos(siObjectID,&siVillagePosX,&siVillagePosY);

				pMyOnlineWorld->pIOnlineObject->GetSize(siObjectID, &siVillageSizeX, &siVillageSizeY);

				if(ManContactvillage(siVillagePosX - siVillageSizeX / 2,
								siVillagePosY - siVillageSizeY /2,
								siVillageSizeX,siVillageSizeY,m_siX,m_siY, KI[m_siImKind].GetAttackRange(0)) == FALSE)
				{
					SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
				}
				else
				{
//					AddOrder(ON_ORDER_WAIT, 0, 0);
					SetMove(m_siX, m_siY);
				}

			}
			else
			{
				if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
				{
					//SetMessage("Moving");
					
					// �̵��϶�� �������ش�.
					SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
				}
			}
		}
		break;

	case ON_ORDER_USEOBJECT:		
	case ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE:
		// ���� �������� �˻�
		if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
		{
			if( !pMyOnlineWorld->pOnlineBooth->IsActionIn() )
			{
				if(m_siMovedPercent < 0)
				{
					if(pMyOnlineWorld->pOnlineBooth->WhererU( m_siX, m_siY ))
					{
						SetMove(m_siX, m_siY);
					}
				}
			}
		}
	
		// �̵��� ���ش�.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// ���ۿ� ����� ���ٸ� ��� ����� ������.
			if(IsOrderInBuffer() == FALSE)				
				AddOrder(ON_ORDER_WAIT, 0, 0);							
		}			
		break;
	case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE:
		// �̵��� ���ش�.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// ���ۿ� ����� ���ٸ� ��� ����� ������.
			if(IsOrderInBuffer() == FALSE)
				AddOrder(ON_ORDER_WAIT, 0, 0);
		}
		break;
//	case ON_ORDER_ATTACKVILLAGEACTION:
//		break;
	case ON_ORDER_PICKUPITEM:		
		{
			UI32		uiItemID;
			SI32		siItemX, siItemY;

			siItemX		=	LOWORD(CurrentOrder.siOrderParam1);
			siItemY		=	HIWORD(CurrentOrder.siOrderParam1);

			// ���� �� ��ġ�� �������� ���ٸ� �̵��� ������Ų��.
			uiItemID	=	pMyOnlineWorld->pOnlineMap->GetItemID(siItemX, siItemY);

			// �� �ڸ��� �������� ���ų� �̵��� �����Ǹ� ����.
			if((siMoveResult == ON_MOVE_DONE) || (uiItemID == 0))
			{	
				// ���ۿ� ����� ���ٸ� ��� ����� ������.
				if(IsOrderInBuffer() == FALSE)				
					AddOrder(ON_ORDER_WAIT, 0, 0);							
			}					
		}
		break;
	// robypark 2005/1/25 19:8
	// ������ ���� ���� ������ �ٸ� ���� ġ���ϱ�
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
		{
			SI16	siTargetCharX, siTargetCharY;

			// ���� �����Ϸ��� ĳ������ ��ġ�� ���´�.
			pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siTargetCharX, &siTargetCharY);						
			
			if(ManContactvillage(siTargetCharX, siTargetCharY,
								1,1, m_siX,m_siY, VALID_HEAL_DISTANCE_SIEGEWAFAREUNIT_FROM_SUPPLYWAGON) == FALSE)
			{
				SetMove(siTargetCharX, siTargetCharY);
			}
			else
			{
				//AddOrder(ON_ORDER_WAIT, 0, 0);
				SetMove(m_siX, m_siY);
			}
		}
		break;
	case ON_ORDER_NPC:
		// �̵��� ���ش�.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// ���ۿ� ����� ���ٸ� ��� ����� ������.
			if(IsOrderInBuffer() == FALSE)
				AddOrder(ON_ORDER_WAIT, 0, 0);
		}
		break;
	}	
		
	// �ִϸ��̼��� ��Ų��.
	UpdateAnimation(FALSE, &fEndFrame, &fSound);
	switch(CurrentOrder.siOrder)
	{
	case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION:
	case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION:
	case ON_ORDER_FIELDATTACK_ACTION:
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION:
		// �ִϸ��̼��� ������ ���� ����
		if(fEndFrame)
		{
			AddOrder(ON_ORDER_WAIT, 0, 0);

			// �� ĳ���Ͱ� �ڽ��� ĳ���� �̸�
			if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
			{
				pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
				pMyOnlineWorld->m_VillageAttackDelay = 10;
			}
			m_bFieldAttackAni = FALSE;
		}

		if(fSound)
		{
			// ���� ���
			CItemHeader*		lpItem = NULL;
			UI16 uiWearItems[WEAR_ITEM_NUM];


			if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
			{
				if(!pMyOnlineWorld->pMyData->GetFollowersInterface()->GetWearItems(0, uiWearItems))					
				{
					lpItem = pMyOnlineWorld->pItemsInfo->GetItem(uiWearItems[ON_WEAR_WEAPON_ITEM]);
				}
			}
			else
			{
				if(m_EquitWeapon != 0)
				{
					lpItem = pMyOnlineWorld->pItemsInfo->GetItem(m_EquitWeapon);
				}
			}

			if(lpItem)
			{
				if(lpItem->pWeaponHeader)
				{
					switch(lpItem->pWeaponHeader->siAttackAtb)
					{
						case 8:
							PushEffect(EFFECT_GUN, 0, 0);
							break;

						case 18:
							PushEffect(EFFECT_ARROW, 0, 0);
							break;

						default:
							PushEffect(EFFECT_BIGSWORD, 0, 0);
							break;
					}
				}
				else
				{
					PushEffect(EFFECT_BIGSWORD, 0, 0);
				}
			}
			else
			{				
				PushEffect(EFFECT_BIGSWORD, 0, 0);
			}
		}
		break;
	}
	
	// �ʿ��� id�� �����Ѵ�.
	if(pMyOnlineWorld->pOnlineMap->GetCharID(m_siX, m_siY) == 0)
		pMyOnlineWorld->pOnlineMap->SetID(m_siX, m_siY, 1, 1, siID, 0, 0, 0);
}


//----------------------------------------------------------------------------------------------------------------
//	����			:	�̵��ؾ� �� ������ �����ش�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetMove(SI16 siDX, SI16 siDY)
{
	m_siDX	=	siDX;
	m_siDY	=	siDY;
}

//----------------------------------------------------------------------------------------------------------------
//	����			:	Ÿ�� ��ǥ�� ���´�.	
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siX;
	*psiY	=	m_siY;
}

//----------------------------------------------------------------------------------------------------------------
//	����			:	Ÿ���� ���� ��ǥ�� ���´�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetRPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siRX;
	*psiY	=	m_siRY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����			:	���� �̵����� ��ǥ ��ġ�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetDPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siDX;
	*psiY	=	m_siDY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����			:	���� �̵����� ���� ��ġ�� ���´�.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetNPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siNX;
	*psiY	=	m_siNY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����			:	���� �̵����� ���� ��ġ�� ������ ���´�.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetMovePersent(SI16 *ps)
{
	*ps = (SI16)m_siMovedPercent;
}


//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------
void	cltOnlineChar::GetMoveRatio(SI32 siStartx, SI32 siStarty, SI32 siEndx, SI32 siEndy, R64 rMovepercent, SI16 &siRelX, SI16 &siRelY)
{
	siRelX = (SI32)((double)(siEndx - siStartx)*rMovepercent/100);
	siRelY = (SI32)((double)(siEndy - siStarty)*rMovepercent/100);
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	�ִϸ��̼��� ������Ʈ �Ѵ�.(���� ��ɰ� ���� ���⿡ �����Ѵ�.)
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::UpdateAnimation(BOOL bForce, BOOL* fEndFrame, BOOL* fSound)
{
	// �ǳ��� ĳ���ͷ� ������ ���
	if (m_bIsRefugee)
		return UpdateAnimationForRefugee(bForce, fEndFrame, fSound);

	SI32	file;
	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[m_siImKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// �ʿ��� �̹����� �ε��Ѵ�. 
				LoadImgFile(file);
			}
		}
	}

	// robypark 2005/1/27 11:49
	// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	if( -1 != m_siAnimationResourceID_Blue )
	{
		if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
		{
			// �ʿ��� �̹����� �ε��Ѵ�. 
			LoadImgFile(m_siAnimationResourceID_Blue);
		}
	}

	// robypark 2005/1/27 11:49
	// �ش� ������ ���� �̹��� ����. �Ʊ� ������ ��
	if( -1 != m_siAnimationResourceID_Red )
	{
		if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]�� �ش������� �̹� �ε��Ǿ��ֳ� �˻�
		{
			// �ʿ��� �̹����� �ε��Ѵ�. 
			LoadImgFile(m_siAnimationResourceID_Red);
		}
	}

	BOOL	bChangeAction, bChangeDirection;
	SI32	CurrentActionCode;

	CurrentActionCode	=	GetActionCode();

	if(fEndFrame)
	{
		*fEndFrame = FALSE;
	}
	if(fSound)
	{
		*fSound = FALSE;
	}	

	if(bForce == TRUE)
	{
		bChangeAction		=	TRUE;
		bChangeDirection	=	TRUE;
	}
	else
	{
		if(siAniDirection == m_siDirection)			bChangeDirection	=	FALSE;
		else										bChangeDirection	=	TRUE;

		if(siAniActionCode	== CurrentActionCode)	bChangeAction		=	FALSE;	
		else
		{		
			bChangeAction		=	TRUE;
			bChangeDirection	=	TRUE;
		}
	}

	
	if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
	{
		switch( siAniActionCode )
		{
		case ON_CHAR_ACTION_CODE_MOVE:
			m_siCurrAni	= m_siMoveCharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_WAIT:
			m_siCurrAni	= m_siWait0CharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_ATTACK:
		case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
			m_siCurrAni	= m_siAttackCharAnimationIndex;
			break;
		default:
			clGrp.Error("", "Animation Error code17[%d]", siAniActionCode);
			break;
		}

		// �ִϸ��̼� ���� �ܰ� ����
		if( m_siAniDelayer >= KI[m_siImKind].m_clCharAnimation[m_siCurrAni].siDelay )
		{
			m_siAniDelayer = 0;

			m_siAniStep++;
			if(fEndFrame)
			{
				if(m_siAniStep >= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siTotalFrame[m_siHeroAniAction][m_siHeroBaseWClass][m_siHeroWeaponEnum])
				{
					*fEndFrame = TRUE;
				}
			}
			if(fSound)
			{
				if(m_siAniStep == 5)
				{
					*fSound = TRUE;
				}
			}
			// actdoll (2004/03/15 16:51) : Critical Error Recovering at Ver.10205
			//	���� �߻� �κ��߿� �ϳ���. ������ ��갪�� Ʋ������ �����Ƿ� �ϴ� ���ù�����(?) ���� �������� ���ذ��� 1�� ����ش�.
			//	���� ���� �����ÿ� �� �κп��� ���װ� �ȳ����ų� ��ġ�� ���� Ư���� ������ ���� ��� �� �ּ��� �����ص� ����.
			SI16	siRetData	= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siTotalFrame[m_siHeroAniAction][m_siHeroBaseWClass][m_siHeroWeaponEnum];
			if( siRetData <= 0 )	siRetData = 1;
			m_siAniStep %= siRetData;
		}
		else
			m_siAniDelayer++;


		//-----------------------------
		// �׸� ����� ���Ѵ�. (m_siXSize, m_siYSize)
		BOOL bExist;

		SI32 siCount, siFileID;
		SI32 siBodyPart, siTotalPiece;

		siTotalPiece	= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siTotalPiece[m_siHeroAniAction][m_siHeroBaseWClass];
		bExist = FALSE;
		for( siCount=0; siCount < siTotalPiece; siCount++ )
		{
			// ���� Bodypart�� ���´�.
			siBodyPart = pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siPartDrawSort[m_siHeroAniAction][m_siHeroBaseWClass][m_siDir][siCount];
			
			if( pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].bPart[m_siHeroAniAction][m_siHeroBaseWClass][siBodyPart] == TRUE )
			{
				siFileID	=  pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID].siFileID[m_siHeroAniAction][siBodyPart];
				
				if( siFileID <= 0 )	continue;
				m_siXSize	= pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID].Header.Xsize;
				m_siYSize	= pMyOnlineWorld->pCharFileLoadManager->m_XSpr[siFileID].Header.Ysize;
				bExist		= TRUE;
				break;
			}
		}


		if( bExist == FALSE )
		{

//			clGrp.Error("UpdateAnimation", "ID:[%s] Unknown ImageFile Size[HeroID:%d][Action:%d][m_siHeroBaseWClass:%d]", 
//				(char*)szCharName,KI[m_siImKind].HeroID, m_siHeroAniAction, m_siHeroBaseWClass);
		}

	}
	else
	{
		if( m_siAniDelayer >= KI[m_siImKind].m_clCharAnimation[m_siCurrAni].siDelay )
		{
			m_siAniDelayer = 0;
			//if( KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber() )
			//	++m_siAniStep %= KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber();
			if( KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber() )
			{
				++m_siAniStep;
				if(m_siAniStep >= KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber())
				{
					m_siAniStep = 0;
					*fEndFrame = TRUE;
				}
			}

		}
		else
			m_siAniDelayer++;
	

		switch( siAniDirection )
		{
		case ON_SOUTH:				m_siDir = SOUTH;		break;
		case ON_WEST | ON_SOUTH:	m_siDir = SOUTH|WEST;	break;
		case ON_WEST:				m_siDir = WEST;			break;
		case ON_WEST | ON_NORTH:	m_siDir = NORTH|WEST;	break;
		case ON_NORTH:				m_siDir = NORTH;		break;
		case ON_EAST | ON_NORTH:	m_siDir = NORTH|EAST;	break;
		case ON_EAST:				m_siDir = EAST;			break;
		case ON_EAST | ON_SOUTH:	m_siDir = SOUTH|EAST;	break;
		default:					clGrp.Error("Animation Error14", "No Dir: %d", siAniDirection);	break;
		}
		switch( siAniActionCode )
		{
		case ON_CHAR_ACTION_CODE_MOVE:
			KI[m_siImKind].m_clCharAnimation[m_siMoveCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siMoveCharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_WAIT:
			KI[m_siImKind].m_clCharAnimation[m_siWait0CharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siWait0CharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_ATTACK:
			KI[m_siImKind].m_clCharAnimation[m_siAttackCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siAttackCharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
			// ���� ���� ���� �ִϸ��̼��� �ִ� ���
			if (m_siMagicCharAnimationIndex)
			{
				KI[m_siImKind].m_clCharAnimation[m_siMagicCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
				m_siCurrAni	= m_siMagicCharAnimationIndex;
			}
			else
			{
				KI[m_siImKind].m_clCharAnimation[m_siAttackCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
				m_siCurrAni	= m_siAttackCharAnimationIndex;
			}
			break;

		default:
			clGrp.Error("", "Animation Error code13[%d]", siAniActionCode);
			break;
		}

		// �׸� ����� ���Ѵ�. (m_siXSize, m_siYSize)
		m_siXSize	= GetFileXsize(m_siImFile);
		m_siYSize	= GetFileYsize(m_siImFile);
	}



	if((bChangeAction == FALSE) && (bChangeDirection == FALSE))
	{
		// ĳ������ �׼ǰ� ������ ������� �ʾ����Ƿ� ������Ʈ �� �ʿ䰡 ����.
		return FALSE;
	}

	if(bChangeAction == TRUE)
	{
		// �׼��� ������Ʈ�Ѵ�.
		siAniActionCode				=	CurrentActionCode;

		// �׼��� �����͸� ������ ���´�.		
		//pCharActionInfo				=	pMyOnlineWorld->pOnlineCharAni->GetCharActionInfo(pCharAniInfo, siAniActionCode);

		// ȭ���� �׼��� ����Ǿ��� �ÿ��� ���´�.		
		//m_siFile					=	pCharActionInfo->siFile;		

		m_siAniStep = 0;

	}

	if(bChangeDirection == TRUE)
	{
		// ������ ������Ʈ�Ѵ�.
		siAniDirection			=	m_siDirection;	

		// ������ �����͸� ������ ���´�.		
		//pCharActionDirectionInfo	=	pMyOnlineWorld->pOnlineCharAni->GetCharDirectionInfo(pCharActionInfo, siAniDirection);		

		//if(pCharActionDirectionInfo == NULL)
		//{
			// ���� ������ ��µ� ����

		//	clGrp.Error("", "���� ������ ��µ� ���� : [%s], [%d] [%d]", szCharName, siAniActionCode, siAniDirection);
		//}
	}	

	// ��Ʈ �迭�� ���´�.	
	//siFrameNum					=	pCharActionDirectionInfo->siFrameNum;	
	//psiFrames					=	pCharActionDirectionInfo->siFrame;	
			
	// Reverse �ؼ� ����Ұ����� ���θ� �����Ѵ�.
	//if(pCharActionDirectionInfo->siParameter1 & ON_CHAR_ANI_REVERSE)	m_bReverseSwitch	=	TRUE;
	//else																m_bReverseSwitch	=	FALSE;

	
	//-----------------------
	// ������ ĳ������ ���
	if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
	{
		SI32 siFontCount;

		switch( siAniActionCode )
		{
		case ON_CHAR_ACTION_CODE_MOVE:
			m_siHeroAniAction = HERO_ACT_WL;
			m_siCurrAni	= m_siMoveCharAnimationIndex;
			break;

		case ON_CHAR_ACTION_CODE_WAIT:
			m_siHeroAniAction = HERO_ACT_N1;
			m_siCurrAni	= m_siWait0CharAnimationIndex;
			break;

		case ON_CHAR_ACTION_CODE_ATTACK:
		case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
			m_siHeroAniAction = HERO_ACT_A1;
			m_siCurrAni	= m_siAttackCharAnimationIndex;	
			break;

		default:
			clGrp.Error("", "Animation Error code34[%d]", siAniActionCode);	
			break;
		}

		switch( siAniDirection )
		{
		case ON_SOUTH:				m_siDir = 1; 	m_bReverse = FALSE;		break;
		case ON_WEST | ON_SOUTH:	m_siDir = 2; 	m_bReverse = FALSE;		break;
		case ON_WEST:				m_siDir = 3; 	m_bReverse = FALSE;		break;
		case ON_WEST | ON_NORTH:	m_siDir = 4; 	m_bReverse = FALSE;		break;
		case ON_NORTH:				m_siDir = 3;	m_bReverse = TRUE;		break;
		case ON_EAST | ON_NORTH:	m_siDir = 2;	m_bReverse = TRUE;		break;
		case ON_EAST:				m_siDir = 1;	m_bReverse = TRUE;		break;
		case ON_EAST | ON_SOUTH:	m_siDir = 0; 	m_bReverse = FALSE;		break;
		default:					clGrp.Error("Animation Error12", "No Dir");		break;
		}

		// �� ����� �� ��Ʈ���� ���´�.
		siFontCount		= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siFontCount[m_siHeroAniAction][m_siHeroBaseWClass];


		// �� ������ ù ��Ʈ ��ȣ
		m_siFont = siFontCount * m_siDir;
	}

	//
	//--------------

	return TRUE;
}

// �ǳ��� ĳ���͸� ���� �ִϸ��̼��� ������Ʈ �Ѵ�.(���� ��ɰ� ���� ���⿡ �����Ѵ�.)	
BOOL	cltOnlineChar::UpdateAnimationForRefugee(BOOL bForce, BOOL* fEndFrame, BOOL* fSound)
{
	SI32	file;
	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// �ʿ��� �̹����� �ε��Ѵ�. 
				LoadImgFile(file);
			}
		}
	}

	BOOL	bChangeAction, bChangeDirection;
	SI32	CurrentActionCode;

	CurrentActionCode	=	GetActionCode();

	if(fEndFrame)
	{
		*fEndFrame = FALSE;
	}
	if(fSound)
	{
		*fSound = FALSE;
	}	

	if(bForce == TRUE)
	{
		bChangeAction		=	TRUE;
		bChangeDirection	=	TRUE;
	}
	else
	{
		if(siAniDirection == m_siDirection)			bChangeDirection	=	FALSE;
		else										bChangeDirection	=	TRUE;

		if(siAniActionCode	== CurrentActionCode)	bChangeAction		=	FALSE;	
		else
		{		
			bChangeAction		=	TRUE;
			bChangeDirection	=	TRUE;
		}
	}

	if( m_siAniDelayer >= KI[m_siRefugeeKind].m_clCharAnimation[m_siCurrAni].siDelay )
	{
		m_siAniDelayer = 0;
		//if( KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber() )
		//	++m_siAniStep %= KI[m_siImKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber();
		if( KI[m_siRefugeeKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber() )
		{
			++m_siAniStep;
			if(m_siAniStep >= KI[m_siRefugeeKind].m_clCharAnimation[m_siCurrAni].GetFrameNumber())
			{
				m_siAniStep = 0;
				*fEndFrame = TRUE;
			}
		}

	}
	else
		m_siAniDelayer++;


	switch( siAniDirection )
	{
		case ON_SOUTH:				m_siDir = SOUTH;		break;
		case ON_WEST | ON_SOUTH:	m_siDir = SOUTH|WEST;	break;
		case ON_WEST:				m_siDir = WEST;			break;
		case ON_WEST | ON_NORTH:	m_siDir = NORTH|WEST;	break;
		case ON_NORTH:				m_siDir = NORTH;		break;
		case ON_EAST | ON_NORTH:	m_siDir = NORTH|EAST;	break;
		case ON_EAST:				m_siDir = EAST;			break;
		case ON_EAST | ON_SOUTH:	m_siDir = SOUTH|EAST;	break;
		default:					clGrp.Error("Animation Error14", "No Dir: %d", siAniDirection);	break;
	}

	switch( siAniActionCode )
	{
		case ON_CHAR_ACTION_CODE_MOVE:
			KI[m_siRefugeeKind].m_clCharAnimation[m_siMoveCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siMoveCharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_WAIT:
			KI[m_siRefugeeKind].m_clCharAnimation[m_siWait0CharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siWait0CharAnimationIndex;
			break;
		case ON_CHAR_ACTION_CODE_ATTACK:
		case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
			KI[m_siRefugeeKind].m_clCharAnimation[m_siAttackCharAnimationIndex].DecideFontAni(m_siDir, m_siAniStep, &m_siImFile, &m_siFont, &m_bReverse);
			m_siCurrAni	= m_siAttackCharAnimationIndex;
			break;
		default:
			clGrp.Error("", "Animation Error code13[%d]", siAniActionCode);
			break;
	}

	// �׸� ����� ���Ѵ�. (m_siXSize, m_siYSize)
	m_siXSize	= GetFileXsize(m_siImFile);
	m_siYSize	= GetFileYsize(m_siImFile);


	if((bChangeAction == FALSE) && (bChangeDirection == FALSE))
	{
		// ĳ������ �׼ǰ� ������ ������� �ʾ����Ƿ� ������Ʈ �� �ʿ䰡 ����.
		return FALSE;
	}

	if(bChangeAction == TRUE)
	{
		// �׼��� ������Ʈ�Ѵ�.
		siAniActionCode				=	CurrentActionCode;

		m_siAniStep = 0;

	}

	if(bChangeDirection == TRUE)
	{
		// ������ ������Ʈ�Ѵ�.
		siAniDirection			=	m_siDirection;	
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	����	:	������ �ٲ۴�.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::ChangeDirection(SI32 siDirection)
{
	m_siDirection	=	siDirection;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ĳ������ ID(���ڿ�)�� �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetID(CHAR *pszID)
{
	strncpy(szCharName, pszID, ON_ID_LENGTH);
}

/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ĳ������ UniqueID�� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	cltOnlineChar::GetUniqueID()
{
	return uiUniqueID;
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ�� �Ǿ����� �˾ƿ´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsInit()
{
	if(uiUniqueID != 0)
		return TRUE;
	else
		return FALSE;
}
*/

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� �̵����� ������ �����Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetMovingDirection(SI32 siDirection)
{
	siMovingDirection	=	siDirection; 
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� �̵����� ������ ���Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	cltOnlineChar::GetMovingDirection()
{		
	return  siMovingDirection;	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�������� �̵����� ������ ������ �ϴ¶������� �˾ƿ´�.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsSendMovingDirection()
{
	if(siMovingDirection != 0)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���͸� �̵� Flag
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetWalkingFlag(UI16 Flag)
{
	MoveFlag = Flag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������� ĳ�����ΰ�?
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsMyChar()
{
	if(pMyOnlineWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	ĳ���͸� �� ������ �� Ÿ�� �̵� ��Ų��.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetMoveByDirection(SI32 siDirection)
{
	SI32	siMoveX, siMoveY;
	SI32	siMapXSize, siMapYSize;


	siMoveX		=	m_siX;
	siMoveY		=	m_siY;
	
	pMyOnlineWorld->pOnlineMap->GetSize(&siMapXSize, &siMapYSize);

	if(siDirection & ON_WEST)
		siMoveX	=	max(0, siMoveX - 1);
	else if(siDirection & ON_EAST)
		siMoveX	=	min(siMapXSize - 1, siMoveX + 1);

	if(siDirection & ON_NORTH)
		siMoveY	=	max(0, siMoveY - 1);
	else if(siDirection & ON_SOUTH)
		siMoveY	=	min(siMapYSize - 1, siMoveY + 1);

	// �̵��ϵ��� ����� �ش�. 
	AddOrder(ON_ORDER_MOVE, siMoveX, siMoveY);	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��縦 �ʱ�ȭ �Ѵ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::InitSpeech()
{
	ZeroMemory(szSpeech, sizeof(szSpeech));
	siSpeechSettingFrame	=	0;
	siSpeechSettingfNotDel	=	0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�̹��� ���� ��縦 �־��ش�.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetSpeech(CHAR *pString, BOOL fNotDel, CHAR* pID)
{
	if(pString == NULL || pString[0] == 0)
	{
		szSpeech[0]             =       NULL;
		siSpeechSettingfNotDel  =       0;
	}
	else
	{
		strcpy(szSpeech, pString);
		siSpeechSettingFrame	=		pMyOnlineWorld->GetTotalFrame();	
		siSpeechSettingfNotDel  =       fNotDel;

		if(fNotDel)
			pMyOnlineWorld->pOnlineMegaText->DrawText(uiUniqueID, pString, RGB(255,100,0), TRUE);
		else
			pMyOnlineWorld->pOnlineMegaText->DrawText(uiUniqueID, pString, RGB(200,200,200), TRUE);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��簡 ���ִ��� �˻�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::CheckSpeech()
{
	return IsSpeech();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	��� ó��,
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::ActionSpeech()
{
	if(IsSpeech() == TRUE)
	{
		if(siSpeechSettingfNotDel == FALSE)
		{
			if(pMyOnlineWorld->GetTotalFrame() - siSpeechSettingFrame > 350)
			{
				// ��縦 �ʱ�ȭ �Ѵ�.
				InitSpeech();
			}
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ����� ������ �����ϱ����� �������� �̸� ���캻��.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::PreProcessorOrder(OnOrder *pOrder)
{
	SI16	 siMX, siMY;	

	switch(pOrder->siOrder)
	{
	case ON_ORDER_MOVE:
		// ���� �̵� �����̰� �������� ������ �������� ���ٸ� ����� �������� �ʴ´�.
		// ���� �������� ���� ��ġ�� �������� �ʴ´�.
		if(pOrder->siOrderParam1 == m_siX && pOrder->siOrderParam2 == m_siY)
			return FALSE;
		
		if(memcmp(pOrder, &pOrder->siOrderParam1, sizeof(OnOrder)) == 0)
			return FALSE;
		
		pMyOnlineWorld->pclFindPath->FindPathL(m_siX, m_siY, pOrder->siOrderParam1, pOrder->siOrderParam2, &siMX, &siMY, NULL, 160, 8, 1, m_siMoveableTileInfo);		

		if(m_siX == siMX && m_siY == siMY)
			return FALSE;
		break;
	case ON_ORDER_ATTACK:
		// ���� ���� �����̰� ���� �����ϴ� ĳ���Ͱ� �����Ϸ��� ĳ���Ϳ� ������ ����� �������� �ʴ´�.
		if(memcmp(pOrder, &pOrder->siOrderParam1, sizeof(OnOrder)) == 0)
			return FALSE;

		// �׸��� ���� �����Ϸ��� ĳ���Ͱ� ��ȿ���� �˾ƿ´�.
		if(pMyOnlineWorld->pIOnlineChar->IsExistUniqueID(pOrder->siOrderParam1) == FALSE)
			return FALSE;
		break;	
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������� �̸��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
CHAR*	cltOnlineChar::GetCharName()
{
	return szCharName;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������� IP�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
DWORD	cltOnlineChar::GetIP()
{
	return dwIP;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��ũ������ RealRect�� ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::GetScreenRealRect(LPRECT pRect)
{
	SI32	siScreenX, siScreenY;

	// ���� ĳ���Ͱ� �ִ� Ÿ���� ��ġ(��ũ�� ��ǥ)�� ���Ѵ�.
	if(pMyOnlineWorld->pOnlineMap->TransformMapXYToScreenDotCenterXY(m_siX, m_siY, siScreenX, siScreenY) == TRUE)
	{
		if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() != uiUniqueID)
		{
			siScreenX	-=	(pMyOnlineWorld->pOnlineMap->GetRX() - m_siRX);
			siScreenY	-=	(pMyOnlineWorld->pOnlineMap->GetRY() - m_siRY);			
		}

		pRect->left		=	siScreenX - ((rcRealImage.right - rcRealImage.left) / 2);
		pRect->right	=	pRect->left + (rcRealImage.right - rcRealImage.left);
		pRect->top		=	(siScreenY) - ((rcRealImage.bottom - rcRealImage.top));
		pRect->bottom	=	pRect->top + (rcRealImage.bottom - rcRealImage.top);		
		return TRUE;	
	}
	else
		return FALSE;

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���콺�� ����Ű�� ĳ���ʹ��� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::IsPointToChar(LPPOINT pptMouse, SI32 *psiDistance)
{
	RECT	rcImage;	
	SI32	siCharCX, siCharCY;
	BOOL	bGetScreenRealRect;
	BOOL	bRet = FALSE;

	// ���� ĳ���Ͱ� �ִ� Ÿ���� ��ġ(��ũ�� ��ǥ)�� ���Ѵ�.
	if((bGetScreenRealRect = GetScreenRealRect(&rcImage)) == TRUE)
	{
		if(PtInRect(&rcImage, *pptMouse) == TRUE)
		{
			siCharCX		=	rcImage.left + (rcImage.right - rcImage.left ) / 2;
			siCharCY		=	rcImage.top + (rcImage.bottom - rcImage.top) / 2;

			*psiDistance	=	SI32(ON_MATH_GETDISTANCE(siCharCX, siCharCY, pptMouse->x, pptMouse->y));
			bRet			=	TRUE;			
		}
	}	
	
	return bRet;	
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �ؾ��ϴ� ��Ȳ�ΰ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::IsWarp(SI16 siWarpX, SI16 siWarpY,SI16 siDistance)
{
	// ON_CHAR_WAPR_DISTANCE ��ŭ ���̰� ���� ������ �õ��Ѵ�.
	if(abs(siWarpX - m_siX) >= siDistance || abs(siWarpY - m_siY) >= siDistance)
		return TRUE;
	else
		return FALSE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������ �Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::Warp(SI16 siWarpX, SI16 siWarpY)
{
	POINT	ptPos;

//	// actdoll (2004/03/16 19:05) : ���⿡ ������ �߻��� �� �ִ� ������ �ִ´�.
//	if( !pMyOnlineWorld )
//	{
//		char	pszError[256];
//		sprintf( pszError, "cltOnlineChar::Warp() Error >> pMyOnlineWorld = 0x%X, siWarpXY = %d/%d, MyUniqueID = %d, ArrayID = %d",
//																	pMyOnlineWorld, siWarpX, siWarpY, uiUniqueID, siID );
//		SetSupposition( pszError );
//	}
//	else if( !pMyOnlineWorld->pOnlineMap )
//	{
//		char	pszError[256];
//		sprintf( pszError, "cltOnlineChar::Warp() Error >> pMyOnlineWorld = 0x%X, pOnlineMap = 0x%X, siWarpXY = %d/%d, MyUniqueID = %d, ArrayID = %d",
//																	pMyOnlineWorld, pMyOnlineWorld->pOnlineMap, siWarpX, siWarpY, uiUniqueID, siID );
//		SetSupposition( pszError );
//	}

	// �̵��Ϸ��� Ÿ�Ͽ� �� �� �ִ��� �˾ƺ���.
	if( !pMyOnlineWorld || !pMyOnlineWorld->pOnlineMap )							return;
	if(pMyOnlineWorld->pOnlineMap->IsColl(siWarpX, siWarpY, m_siMoveableTileInfo) == FALSE)
	{
		// �켱 �ʿ��� �ڱ��� ID�� �����Ѵ�.
		DelID();

		// �ڱⰡ ������ ����� �ڸ��� ���ش�.
		if(m_siX != m_siMX || m_siY != m_siMY)			
		{
			// ���� �����ߴ� �ڸ��� ���ش�.			
			pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siMX, m_siMY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, FALSE);
		}
		
		m_siX	=	siWarpX;
		m_siY	=	siWarpY;

		// �ʿ� ID�� �ٽ� �����Ѵ�.	
		ptPos	=	SetID(siWarpX, siWarpY);
		
		m_siX	=	SI16(ptPos.x);
		m_siY	=	SI16(ptPos.y);		

		m_siDX  =   SI16(ptPos.x);
		m_siDY  =   SI16(ptPos.y);

		m_siMovedPercent = 0;

		SetMove(m_siX, m_siY);
		AddOrder(ON_ORDER_WAIT, 0, 0);																	

		m_VMercenary_NowX  = m_siX - 1;
		m_VMercenary_NowY  = m_siY - 1;
		m_VMercenary_NextX = m_siX - 1;
		m_VMercenary_NextY = m_siY - 1;
		m_VMercenary_MovedPercent = 0;
		m_VMercenary_TargetX = m_siX - 1;
		m_VMercenary_TargetY = m_siY - 1;
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� �������� ����� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnOrder	cltOnlineChar::GetCurrentOrder()
{
	return CurrentOrder;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ĳ���Ͱ� �̵����ΰ�?
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::IsMoving()
{
	return bMovingSwitch;	
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Action Code�� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineChar::GetActionCode()
{
	if(bMovingSwitch == TRUE)
	{
		return ON_CHAR_ACTION_CODE_MOVE;
	}
	else
	{
		if(CurrentOrder.siOrder == ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION)
		{
			return ON_CHAR_ACTION_CODE_ATTACK;
		}
		else if(CurrentOrder.siOrder == ON_ORDER_FIELDATTACK_ACTION)
		{
			return ON_CHAR_ACTION_CODE_ATTACK;
		}
		else if(CurrentOrder.siOrder == ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION)
		{
			return ON_CHAR_ACTION_CODE_ATTACK;
		}
		// ���� �����̶��
		else if(CurrentOrder.siOrder == ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION)
		{
			return ON_CHAR_ACTION_CODE_SPELL_MAGIC;
		}
		else
		{
			return ON_CHAR_ACTION_CODE_WAIT;
		}

	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ Ŭ������ ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	cltOnlineChar::GetClass()
{
	return siClass;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	BaseX, BaseY�� �������� ID�� ���� ���ο� ��ǥ�� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
POINT	cltOnlineChar::SetID(SI32 siBaseX, SI32 siBaseY)
{
	POINT	ptPos;

	// ĳ���Ͱ� ���� �� �ִ� ��ġ�� ã�´�.	
	// ĳ���Ͱ� �̵� ������ Ÿ���� ������ �־��ش�.
	if(ISSHIP(uiUniqueID) == TRUE)
		ptPos	=	pMyOnlineWorld->pOnlineMap->SearchMap(siBaseX, siBaseY, ON_MAP_SI_CHAR, ON_TILE_KIND_SEA);
	else
	{
		ptPos	=	pMyOnlineWorld->pOnlineMap->SearchMap(siBaseX, siBaseY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);
		DWriteStringToFile("[%s] [%d] [%d] : [%d] [%d]", szCharName, siBaseX, siBaseY, ptPos.x, ptPos.y);
	}


	if(ptPos.x != -1 && ptPos.y != -1)
	{
		// ĳ���͸� ���� ��ġ�� ã�Ҵ�.
		// ĳ���͸� �ɴ´�.				
		pMyOnlineWorld->pOnlineMap->SetID(SI16(ptPos.x), SI16(ptPos.y), 1, 1, siID, 0, 0, 0);	
	}

	return ptPos;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʿ��� �ڽ��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::DelID()
{
	// �ʿ��� ���� ��ǥ�� �ڱ� �϶��� �����Ѵ�.
	if(pMyOnlineWorld->pOnlineMap->GetCharID(m_siX, m_siY) == siID)
	{
		// �ʿ��� �����ص� ������.
		pMyOnlineWorld->pOnlineMap->DelID(m_siX, m_siY, 1, 1, ON_MAP_DELID_CHAR);		
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�÷��̾��� ������ �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetPlayerInfo(OnCharBaseInfo* lpBaseInfo, SI16 fType, OnCharPlayerDetailInfo* lpPlayerInfo)
{
	OnCharMonsterInfo*			lpOnCharMonsterInfo;
	OnCharPlayerInfo*			lpOnCharPlayerInfo;
	OnCharShipInfo*				lpOnCharShipInfo;
	char						TempBuffer[256];

	fRanking     = 0;
	siFightPower = 0;
	switch(fType)
	{
		case 0:				// Player
			lpOnCharPlayerInfo = (OnCharPlayerInfo*)lpBaseInfo;
			siFightPower = lpOnCharPlayerInfo->siFightingPower;
			dwIP         = lpOnCharPlayerInfo->siDBAccount;

			SetEquipItem((UI16*)lpOnCharPlayerInfo->uiEquipWeapon);
			SetGuildData(GetGuildName(), lpOnCharPlayerInfo->siGuildInGrade, GetGuildFlag());

			if(lpPlayerInfo)
			{
				ZeroMemory(szCharName, sizeof(szCharName));
				strncpy(szCharName, lpPlayerInfo->szID, ON_ID_LENGTH);

				ZeroMemory(TempBuffer, sizeof(256));
				CopyMemory(TempBuffer, lpPlayerInfo->szGuild, ON_GUILDNAME_LENGTH);
				SetGuildData((char*)TempBuffer, GetGuildInGrade(), lpPlayerInfo->siGuildFlag);
				SetIsHostilityGuild(lpPlayerInfo->uiIsHostilityGuild);				
				SetMercenary(lpOnCharPlayerInfo->siVMercenaryKind);
				SetExpoEvent(lpPlayerInfo->siEventScore, lpPlayerInfo->siEventRanking, (BOOL)lpPlayerInfo->uiIsHostilityGuild);

				sifDataType = 0;
			}
			break;

		case 1:				// Monster
			lpOnCharMonsterInfo = (OnCharMonsterInfo*)lpBaseInfo;
			siFightPower = lpOnCharMonsterInfo->siFightingPower;
			break;

		case 2:				// Ship
			lpOnCharShipInfo = (OnCharShipInfo*)lpBaseInfo;
			break;					
	}
	
	// �̹� ĳ���Ͱ� �ִٸ� �� ĳ���͸� �̵� ��Ų��.			
	AddOrder(ON_ORDER_MOVE, lpBaseInfo->siX, lpBaseInfo->siY);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�÷��̾��� ������ �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetPlayerInfo(OnCharPlayerDetailInfo* lpPlayerInfo)
{
	char						TempBuffer[256];

	if(lpPlayerInfo)
	{
		ZeroMemory(szCharName, sizeof(szCharName));
		strncpy(szCharName, lpPlayerInfo->szID, ON_ID_LENGTH);

		ZeroMemory(TempBuffer, sizeof(256));
		CopyMemory(TempBuffer, lpPlayerInfo->szGuild, ON_GUILDNAME_LENGTH);
		SetGuildData((char*)TempBuffer, GetGuildInGrade(), lpPlayerInfo->siGuildFlag);
		SetIsHostilityGuild(lpPlayerInfo->uiIsHostilityGuild);
		sifDataType = 0;

		SetExpoEvent(lpPlayerInfo->siEventScore, lpPlayerInfo->siEventRanking, (BOOL)lpPlayerInfo->uiIsHostilityGuild);
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
// Description	:	ĳ���Ϳ��� �������� ������Ų��.
// Parameter	:	bEquip			- FALSE�̸� ��������
//					siEquipKind		- ��������
//					siEquipID		- ������ ���̵�, -1�� ��� ����Ʈ ������ ����
//					bEffectSound	- ������ ȿ������ �� ���ΰ� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound)
{
	BOOL bSet = FALSE;

	if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == FALSE ) return FALSE;

	// robypark 2005/1/17 13:4
	// �ǳ��� ĳ������ ��� ��� ó�� ����.
	if (m_bIsRefugee)
		return FALSE;

	switch( siEquipKind )
	{
	case ON_WEAR_ARMOR_ITEM: //2
		bSet = TRUE;
		if( bEquip == FALSE || siEquipID == -1 )
		{
			siEquipID = KI[m_siImKind].m_ssDefaultArmor;
		}
		break;
	case ON_WEAR_HELMET_ITEM: //0
		bSet = TRUE;
		if( bEquip == FALSE || siEquipID == -1 )
		{
			siEquipID = KI[m_siImKind].m_ssDefaultHelmet;

		}

		break;
	case ON_WEAR_WEAPON_ITEM: //1
		// ���� ���� ����̰ų�, ������ ���̵� -1�� ��� �⺻ ����(�Ǽ�)�� ���� 
		if( bEquip == FALSE	|| siEquipID == -1 )
		{
			siEquipID = KI[m_siImKind].m_ssDefaultWeapon;
			bSet = TRUE;
		}
		// �׸��� �� ���� �����Ƿ�.
		else
		{
			bSet = TRUE; 	
		}

		m_siWeaponDrawID		= siEquipID;

		// ��ȭ����̸鼭 �ڽ��� ���̵��� ��� �⺻ ����(�Ǽ�)�� ����
		if( !m_bAttackMode )
		{
			siEquipID = KI[m_siImKind].m_ssDefaultWeapon;
			bSet = TRUE;
		}
		break;

	default:
		return FALSE;
	}	

	if( bEffectSound == TRUE )
		PushEffect(EFFECT_ON_ITEMONOFF, 0, 0);
	if( bSet )
	{
/*		FILE* fp;
		fp = fopen("ShareFile.txt","at");
		fprintf(fp,"cltOnlineChar::SetEquip m_siImKind = %d bEquip = %d siEquipKind = %d \n",m_siImKind,bEquip,siEquipKind);
		fclose(fp);*/

		pMyOnlineWorld->pCharFileLoadManager->SetChange(siEquipKind, siEquipID, KI[m_siImKind].m_ssHeroID, siID, m_cDefaultBody);
	}
	
	// m_siHeroBaseWClass ����
	m_siHeroBaseWClass	= pMyOnlineWorld->pCharFileLoadManager->GetHeroClassCode(&pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID]);	

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ �������� ��´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineChar::GetFightPower(void)
{
	return siFightPower;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	ĳ������ �������� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::SetFightPower(SI32 FightPower)
{
	siFightPower = FightPower;
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Set Battle Flag
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetBattleFlag(UI08 Flag)
{
	m_BattleFlag                = Flag;
	m_BattleFlagStartTickCount  = GetTickCount();
	m_BattleFlagStartCheckCount = 5;

	if(pMyOnlineWorld)
	{
		if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
		{
			PushEffect(EFFECT_STARTBATTLE, 0, 0);
			PushEffect(EFFECT_COUNTDOWN, 0, 0);
		}
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get Battle Flag
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	cltOnlineChar::GetBattleFlag(void)
{
	return m_BattleFlag;
}
 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Imoticon ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetImoticon(UI16 uiImoticon)
{
	m_Imoticon_StartCount = GetTickCount();
	m_Imoticon_Type       = uiImoticon + 1;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Set Find Target Flag
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetFindTargetFlag(UI08 Flag)
{
	m_FindTargetFlag = Flag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� Data ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetGuildData(char* lpGuildName, SI32 siGuildInGrade, SI32 siGuildFlag)
{
	if(lpGuildName)
	{
		strncpy(m_szGuild, lpGuildName, ON_GUILDNAME_LENGTH);
		m_szGuild[strlen(lpGuildName)] = NULL;
	}
	else
	{
		ZeroMemory(m_szGuild, ON_GUILDNAME_LENGTH);
	}
	m_siGuildInGrade = siGuildInGrade;
	m_siGuildFlag    = siGuildFlag;
	if(m_siGuildFlag < 0) m_siGuildFlag = 0;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� Name ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
char*	cltOnlineChar::GetGuildName(void)
{
	if(m_szGuild[0]) return (char*)m_szGuild;

	return NULL;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ���� ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineChar::GetGuildInGrade(void)
{
	return m_siGuildInGrade;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	��� ��� ���
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI32	cltOnlineChar::GetGuildFlag(void)
{
	return m_siGuildFlag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::SetIsHostilityGuild(UI08 uiIsHostilityGuild)
{
	m_uiIsHostilityGuild = uiIsHostilityGuild;
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ������� �˻�
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	cltOnlineChar::GetIsHostilityGuild(void)
{
	return m_uiIsHostilityGuild;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���õ� ĳ���Ϳ��θ� ���� 
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
void	cltOnlineChar::SetNewSelectedFlag(BOOL flag)
{
	NewSelectedFlag = flag; 

	if(flag == TRUE)
	{
		NewSelectedDelay = NEW_SELECTED_DELAY;
	}
	else
	{
		NewSelectedDelay = 0;
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���õ� ĳ���Ϳ��θ� ����.  
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::GetNewSelectedFlag()
{
	return NewSelectedFlag;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���õ� ĳ���Ϳ��θ� ����.  
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::Show(BOOL fShow)
{
	m_fShow = fShow;
	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���� ���õ� ĳ���Ϳ��θ� ����.  
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	cltOnlineChar::GetShow(void)
{
	return m_fShow;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Get IP
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI32	cltOnlineChar::GetIP(UI16 uiUniqueID)
{
	return dwIP;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	���λ��� ���� ���� ����
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	cltOnlineChar::SetBoothOpen(BOOL bOpenBooth)
{
	m_bOpenBooth = bOpenBooth;

	if(!bOpenBooth)
		ZeroMemory(m_SellItemInfo, sizeof(SellItemInfo)*ON_MAX_BOOTH_ITEM);
}

SI08	cltOnlineChar::GetDataType(void)
{
	return sifDataType;
}

void	cltOnlineChar::SetDataType(SI08 fDataType)
{
	sifDataType = fDataType;
}

BOOL	cltOnlineChar::SetMercenary(SI32 Kind)
{
	// ������ �����ٸ�, ���� �ʱ�ȭ
	if((m_MercenaryKind == 0) || (Kind == 0))
	{
		m_VMercenary_NowX  = m_siX - 1;
		m_VMercenary_NowY  = m_siY - 1;
		m_VMercenary_NextX = m_siX - 1;
		m_VMercenary_NextY = m_siY - 1;
		m_VMercenary_MovedPercent = 0;
		m_VMercenary_Dir          = SOUTH;
		m_VMercenary_siAniStep    = 0;
		m_VMercenary_siAniDelayer = 0;
		m_VMercenary_siCurrAni    = 0;
		m_VMercenary_siFont       = 0;
		m_VMercenary_bReverse     = 0;

		m_VMercenary_TargetX = m_siX - 1;
		m_VMercenary_TargetY = m_siY - 1;
	}

	m_MercenaryKind = Kind;
	if(m_MercenaryKind == siKind) m_MercenaryKind = 0;

	if(m_MercenaryKind != 0)
	{
		SI32	siTempImKind;

		siTempImKind = TransKindOnlineToImjin(m_MercenaryKind);
		for(SI16 i = 1; i < MAX_ANIMATION_PER_UNIT; i++)
		{
			switch(KI[siTempImKind].m_siCharAnimationInfo[i])
			{
				case ANI_MOVE:			m_VMercenary_siMoveCharAnimationIndex   = i;		break;
				case ANI_WAIT0:			m_VMercenary_siWait0CharAnimationIndex  = i;		break;
				case ANI_ATTACK:		m_VMercenary_siAttackCharAnimationIndex = i;		break;
			}
		}
		m_VMercenary_siAniStep    = 0;
		m_VMercenary_siAniDelayer = 0;
		m_VMercenary_siCurrAni    = m_VMercenary_siWait0CharAnimationIndex;
		m_VMercenary_siFont       = 0;
		m_VMercenary_bReverse     = 0;
	}

	return TRUE;
}

SI32	cltOnlineChar::GetMercenary(void)
{
	return m_MercenaryKind;
}

void	cltOnlineChar::ActionByVMercenary(void)
{
	if(m_MercenaryKind == 0)     return;

	// Player�� 1ĭ �̻� ������ ������ ���ΰ� ��ġ�� �̵�
	if((abs(m_siX - m_VMercenary_TargetX) > 1) || (abs(m_siY - m_VMercenary_TargetY) > 1))
	{
		SI16			siTileScore[9];
		SI32			TempX;
		SI32			TempY;
		SI32			TempXCount;
		SI32			TempYCount;

		// ���ΰ� �ֺ� 1ĭ ��ġ�� ����, �� �������� ������ �ش�.
		ZeroMemory(siTileScore, sizeof(siTileScore));

		// ���� ���ΰ� ��ġ�� -100
		siTileScore[(1 * 3) + 1] -= 100;

		// ���ΰ��� ������ �־��� ��ġ�� ������ 1 ����
		if((abs(siPrevX1 - m_siX) <= 1) && (abs(siPrevY1 - m_siY) <= 1))
		{
			siTileScore[((siPrevY1 - m_siY + 1) * 3) + (siPrevX1 - m_siX + 1)] += 1;
		}

		// ���� �뺴��ġ�� ���� �������� 2 ����
		if((abs(m_VMercenary_NowX - m_siX) > 1) || (abs(m_VMercenary_NowY - m_siY) > 1))
		{
			TempX = 0;
			if(m_siX > m_VMercenary_NowX) TempX = -1;
			if(m_siX < m_VMercenary_NowX) TempX = 1;
			TempY = 0;
			if(m_siY > m_VMercenary_NowY) TempY = -1;
			if(m_siY < m_VMercenary_NowY) TempY = 1;
		}
		else
		{
			TempX = m_VMercenary_NowX - m_siX + 1;
			TempY = m_VMercenary_NowY - m_siY + 1;
		}
		siTileScore[(TempY * 3) + TempX] += 2;

		TempX = m_siX - 1;
		TempY = m_siY - 1;
		for(TempYCount = 0; TempYCount < 3; TempYCount++)
		{
			for(TempXCount = 0; TempXCount < 3; TempXCount++)
			{
				switch(pMyOnlineWorld->pOnlineMap->CollCheckType(TempX, TempY))
				{
					case 0:		// �̵� �Ұ��� �����̸�, ���� �Ҵ�
						siTileScore[(TempYCount * 3) + TempXCount] -= 100;
						break;

					case 1:		// �ٸ� User�� ������, �� ������ ������ 3�� ����
						siTileScore[(TempYCount * 3) + TempXCount] -= 3;
						siTileScore[(TempYCount * 3) + TempXCount] -= pMyOnlineWorld->pOnlineMap->VMercenaryGetCount(TempX, TempY);
						break;

					case 2:		// �� ������ �뺴�� ������, ������ ����.
						siTileScore[(TempYCount * 3) + TempXCount] -= pMyOnlineWorld->pOnlineMap->VMercenaryGetCount(TempX, TempY);
						break;
				}

				TempX++;
			}

			TempX = m_siX - 1;
			TempY++;
		}

		SI32		Index;
		SI32		MaxScore;

		Index    = -1;
		MaxScore = -200;
		for(TempYCount = 0; TempYCount < 3; TempYCount++)
		{
			for(TempXCount = 0; TempXCount < 3; TempXCount++)
			{
				if(MaxScore < siTileScore[(TempYCount * 3) + TempXCount])
				{
					Index    = (TempYCount * 3) + TempXCount;
					MaxScore = siTileScore[(TempYCount * 3) + TempXCount];
				}
			}
		}
		if(Index != -1)
		{
			m_VMercenary_TargetX = m_siX + (Index % 3) - 1;
			m_VMercenary_TargetY = m_siY + (Index / 3) - 1;
		}
	}

	// �̵����̸�, �̵� ó��
	if((m_VMercenary_NowX != m_VMercenary_NextX) || (m_VMercenary_NowY != m_VMercenary_NextY))
	{
		m_VMercenary_MovedPercent += m_siMoveSpeed;

		if(m_VMercenary_MovedPercent >= 100)
		{
			m_VMercenary_MovedPercent -= 100;

			m_VMercenary_NowX = m_VMercenary_NextX;
			m_VMercenary_NowY = m_VMercenary_NextY;
			if((m_VMercenary_NowX == m_VMercenary_TargetX) && (m_VMercenary_NowY == m_VMercenary_TargetY))
			{
				m_VMercenary_MovedPercent = 0;
			}
		}
		else
		{
			// ���� ����
			m_VMercenary_Dir = 0;

			if(m_VMercenary_NowX < m_VMercenary_NextX)
			{
				m_VMercenary_Dir |= EAST;
			}
			else if(m_VMercenary_NowX > m_VMercenary_NextX)
			{
				m_VMercenary_Dir |= WEST;
			}

			if(m_VMercenary_NowY < m_VMercenary_NextY)
			{
				m_VMercenary_Dir |= SOUTH;
			}
			else if(m_VMercenary_NowY > m_VMercenary_NextY)
			{
				m_VMercenary_Dir |= NORTH;
			}
		}
	}

	// ���� ��ġ���� �̵��ؾ��ϴ� ��Ȳ�̸�
	if((m_VMercenary_NowX == m_VMercenary_NextX) && (m_VMercenary_NowY == m_VMercenary_NextY))
	{
		if((m_VMercenary_NowX != m_VMercenary_TargetX) || (m_VMercenary_NowY != m_VMercenary_TargetY))
		{
			// ���� 10 �̻��� �Ÿ��� ���̳��� Warp
			if(abs(m_VMercenary_NowX - m_VMercenary_TargetX) + abs(m_VMercenary_NowY - m_VMercenary_TargetY) > 10)
			{
				m_VMercenary_NowX  = m_siX - 1;
				m_VMercenary_NowY  = m_siY - 1;
				m_VMercenary_NextX = m_siX - 1;
				m_VMercenary_NextY = m_siY - 1;
				m_VMercenary_MovedPercent = 0;
				m_VMercenary_Dir          = SOUTH;

				m_VMercenary_TargetX = m_siX - 1;
				m_VMercenary_TargetY = m_siY - 1;
			}
			else
			{
				m_VMercenary_NextX = 0;
				if(m_VMercenary_TargetX > m_VMercenary_NowX) m_VMercenary_NextX = 1;
				if(m_VMercenary_TargetX < m_VMercenary_NowX) m_VMercenary_NextX = -1;
				m_VMercenary_NextY = 0;
				if(m_VMercenary_TargetY > m_VMercenary_NowY) m_VMercenary_NextY = 1;
				if(m_VMercenary_TargetY < m_VMercenary_NowY) m_VMercenary_NextY = -1;

				m_VMercenary_NextX += m_VMercenary_NowX;
				m_VMercenary_NextY += m_VMercenary_NowY;
			}
		}
	}

	if(m_VMercenary_MovedPercent > 50)
	{
		pMyOnlineWorld->pOnlineMap->VMercenaryAdd(m_VMercenary_NextX, m_VMercenary_NextY, siID);			
	}
	else
	{
		pMyOnlineWorld->pOnlineMap->VMercenaryAdd(m_VMercenary_NowX, m_VMercenary_NowY, siID);			
	}

	SI32	file;
	SI32	siTempImKind;

	// ĳ���Ϳ� �ʿ��� ������ ���ٸ� �ҷ��´�.
	siTempImKind = TransKindOnlineToImjin(m_MercenaryKind);
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[siTempImKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// �ʿ��� �̹����� �ε��Ѵ�. 
				LoadImgFile(file);
			}
		}
	}

	// ���� �̵������� ��������� �˻�
	if((m_VMercenary_NowX == m_VMercenary_NextX) && (m_VMercenary_NowY == m_VMercenary_NextY))
	{
		switch( siAniActionCode )
		{
			case ON_CHAR_ACTION_CODE_ATTACK:
			case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
				// ���� ����
				switch( siAniDirection )
				{
					case ON_SOUTH:				m_VMercenary_Dir = SOUTH;			break;
					case ON_WEST | ON_SOUTH:	m_VMercenary_Dir = SOUTH | WEST;	break;
					case ON_WEST:				m_VMercenary_Dir = WEST;			break;
					case ON_WEST | ON_NORTH:	m_VMercenary_Dir = NORTH | WEST;	break;
					case ON_NORTH:				m_VMercenary_Dir = NORTH;			break;
					case ON_EAST | ON_NORTH:	m_VMercenary_Dir = NORTH | EAST;	break;
					case ON_EAST:				m_VMercenary_Dir = EAST;			break;
					case ON_EAST | ON_SOUTH:	m_VMercenary_Dir = SOUTH | EAST;	break;
					default:					clGrp.Error("Animation Error14", "No Dir: %d", siAniDirection);	break;
				}

				KI[siTempImKind].m_clCharAnimation[m_VMercenary_siAttackCharAnimationIndex].DecideFontAni(m_VMercenary_Dir, m_VMercenary_siAniStep, &m_siTempImFile, &m_VMercenary_siFont, &m_VMercenary_bReverse);
				m_VMercenary_siCurrAni = m_VMercenary_siAttackCharAnimationIndex;				
				break;

			default:
				KI[siTempImKind].m_clCharAnimation[m_VMercenary_siWait0CharAnimationIndex].DecideFontAni(m_VMercenary_Dir, m_VMercenary_siAniStep, &m_siTempImFile, &m_VMercenary_siFont, &m_VMercenary_bReverse);
				m_VMercenary_siCurrAni = m_VMercenary_siWait0CharAnimationIndex;
				break;
		}
	}
	else
	{
		// �̵���
		KI[siTempImKind].m_clCharAnimation[m_VMercenary_siMoveCharAnimationIndex].DecideFontAni(m_VMercenary_Dir, m_VMercenary_siAniStep, &m_siTempImFile, &m_VMercenary_siFont, &m_VMercenary_bReverse);
		m_VMercenary_siCurrAni = m_VMercenary_siMoveCharAnimationIndex;
	}

	if(m_VMercenary_siAniDelayer >= KI[siTempImKind].m_clCharAnimation[m_VMercenary_siCurrAni].siDelay)
	{
		m_VMercenary_siAniDelayer = 0;
		if(KI[siTempImKind].m_clCharAnimation[m_VMercenary_siCurrAni].GetFrameNumber())
		{
			++m_VMercenary_siAniStep %= KI[siTempImKind].m_clCharAnimation[m_VMercenary_siCurrAni].GetFrameNumber();
		}
	}
	else
	{
		m_VMercenary_siAniDelayer++;
	}	
}

void	cltOnlineChar::DrawByVMercenary(SI16 WaterP)
{
	if(m_fShow         == FALSE) return;
	if(m_MercenaryKind == 0)     return;

	// ������ ������ �ܱ��� �ſ� ȿ���� ���� �뺴�� �׸��� �ʴ´�.
	if (TRUE == m_bIsSiegeWarfareUnit)
	{
		return;
	}

	// �����������̽��� �ִ� ��쿡�� �ܱ��� �ſ� ȿ���� ���� �뺴�� �׸��� �ʴ´�.
	if (TRUE == m_bIsInVillageDefenceInterface)
	{
		return;
	}

	// �ǳ����� �ܱ��� �ſ� ȿ���� �׸��� �ʴ´�.
	if (TRUE == m_bIsRefugee)
		return;

	SI32	siStartX;
	SI32	siStartY;
	SI32	siCenterX;
	SI32	siCenterY;
	SI32	siRelateX;
	SI32	siRelateY;
	SI32	siCurrentMapX;
	SI32	siCurrentMapY;
	RECT	rcScreen;
	SI32	siTempImKind;
	SI32	siDrawX;	
	SI32	siDrawY;
	SI32	siVMXSize;
	SI32	siVMYSize;
	SI32	siNextTileDisX;
	SI32	siNextTileDisY;

	siTempImKind = TransKindOnlineToImjin(m_MercenaryKind);

	siVMXSize = GetFileXsize(m_siTempImFile);
	siVMYSize = GetFileYsize(m_siTempImFile);
	
	// ȭ�鿡 ����� ��ġ�� ����Ѵ�.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);

	siNextTileDisX = 0;
	siNextTileDisY = 0;
	if(m_VMercenary_MovedPercent < 50)
	{
		siDrawX = m_VMercenary_NowX - siCurrentMapX;
		siDrawY = m_VMercenary_NowY - siCurrentMapY;

		if(m_VMercenary_NowX > m_VMercenary_NextX)
		{
			siNextTileDisX += -32;
			siNextTileDisY += -16;
		}
		else if(m_VMercenary_NowX < m_VMercenary_NextX)
		{
			siNextTileDisX += 32;
			siNextTileDisY += 16;
		}

		if(m_VMercenary_NowY > m_VMercenary_NextY)
		{
			siNextTileDisX += 32;
			siNextTileDisY += -16;
		}
		else if(m_VMercenary_NowY < m_VMercenary_NextY)
		{
			siNextTileDisX += -32;
			siNextTileDisY += 16;
		}

		siRelateX = pMyOnlineWorld->pOnlineMap->GetRX() - (siNextTileDisX * m_VMercenary_MovedPercent) / 100;
		siRelateY = pMyOnlineWorld->pOnlineMap->GetRY() - (siNextTileDisY * m_VMercenary_MovedPercent) / 100;
	}
	else
	{
		siDrawX = m_VMercenary_NextX - siCurrentMapX;
		siDrawY = m_VMercenary_NextY - siCurrentMapY;

		if(m_VMercenary_NowX < m_VMercenary_NextX)
		{
			siNextTileDisX += -32;
			siNextTileDisY += -16;
		}
		else if(m_VMercenary_NowX > m_VMercenary_NextX)
		{
			siNextTileDisX += 32;
			siNextTileDisY += 16;
		}

		if(m_VMercenary_NowY < m_VMercenary_NextY)
		{
			siNextTileDisX += 32;
			siNextTileDisY += -16;
		}
		else if(m_VMercenary_NowY > m_VMercenary_NextY)
		{
			siNextTileDisX += -32;
			siNextTileDisY += 16;
		}

		siRelateX = pMyOnlineWorld->pOnlineMap->GetRX() - (siNextTileDisX * (100 - m_VMercenary_MovedPercent)) / 100;
		siRelateY = pMyOnlineWorld->pOnlineMap->GetRY() - (siNextTileDisY * (100 - m_VMercenary_MovedPercent)) / 100;
	}	

	rcScreen = pMyOnlineWorld->pOnlineMap->GetScreenRect();

	siCenterX = (siDrawX * ONLINE_TILE_XSIZE_HALF) + (-siDrawY * ONLINE_TILE_XSIZE_HALF) + ((rcScreen.right - rcScreen.left + 1) / 2) - siRelateX;
	siCenterY = (siDrawX * ONLINE_TILE_YSIZE_HALF) + (siDrawY * ONLINE_TILE_YSIZE_HALF)  + ((rcScreen.bottom - rcScreen.top + 1) / 2) - siRelateY;

	// ������ Character ȭ�鿡 ������ ���� ��ġ�� �ٽ� ����Ѵ�.
	siStartX = siCenterX - (siVMXSize / 2);
	siStartY = siCenterY - siVMYSize + KI[siTempImKind].m_ssBottomIndent;

	DrawCharByVMercenary(WaterP, siStartX, siStartY, siCenterX, siCenterY);
}

void	cltOnlineChar::GetNPosByVMercenary(SI16 *psiX, SI16 *psiY)
{
	*psiX = m_VMercenary_NextX;
	*psiY = m_VMercenary_NextY;
}

void	cltOnlineChar::GetMovePersentByVMercenary(SI16 *ps)
{
	*ps = (SI16)m_VMercenary_MovedPercent;
}

void	cltOnlineChar::GetEquip(UI16* lpuiEquip)
{
	CopyMemory(lpuiEquip, uiEquip, sizeof(UI16) * WEAR_ITEM_NUM);
}

void	cltOnlineChar::SetChangeEffect(void)
{
	// ���� �������� �������� �˻�
	m_ChangeEffectSex = 1;			// <-- �⺻�� ���ڷ�

	// ���ΰ� ���� ������ ������ ���
	CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(GetKind());

	if (pFieldHeroInfo)
	{
		// ���� ĳ�����ΰ�?
		if (pFieldHeroInfo->IsHeroFemale())
			m_ChangeEffectSex = 2;
	}

	m_ChangeEffectFrame = 0;
}

BOOL cltOnlineChar::ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis)
{
	// ������ �����ϰ� ��, ��, ��, �Ϸ� dis��ŭ ������ �ִ� ���ڸ� ��������.
	if( (cx>=vx-dis && cx<=vx+wi+dis-1) && (cy>=vy-dis && cy<=vy+hi+dis-1) ) return TRUE;
	else return FALSE;
}

void cltOnlineChar::SetSiegeWarfare_Hp(SI16 siFieldHp)
{
	m_siFieldHp = siFieldHp;

	// �ִ� ������ ���� ���� ū ���
	if (m_siFieldHp_Maximum < m_siFieldHp)
		m_siFieldHp_Maximum = m_siFieldHp;
}

void cltOnlineChar::SetSiegeWarfare_Damage(SI16 siDamage)
{
	m_siFieldHp -= siDamage;
}

SI16 cltOnlineChar::GetSiegeWarfare_Hp()
{
	return m_siFieldHp;
}

BOOL cltOnlineChar::IsFieldAlive()
{
	if(m_siFieldHp > 0)		return TRUE;
	else					return FALSE;
}

// ���� /////////////////////////////////////////
// robypark 2004/10/27 15:13
// ������ ���� �ִ� ü��ġ ����
void cltOnlineChar::SetSiegeWarfare_MaximumHP(SI16 siMaximumHP)
{
	m_siFieldHp_Maximum = siMaximumHP;
}

// ������ ���� �ִ� ü��ġ ���
SI16 cltOnlineChar::GetSiegeWarfare_MaximumHP(void)
{
	return m_siFieldHp_Maximum;
}

// ������ ���� ����ǰ ���緮 ����
void cltOnlineChar::SetSiegeWarfare_CurrentSupplyGoodsAmount(SI32 siCurrentSupplyGoodsAmount)
{
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = siCurrentSupplyGoodsAmount;
//	m_siSupplyCharge = m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;

	/*
	if (m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount <= 0)
	{
		m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = 0;

		// ���� ����(�޵�) �� ��쿡��
		if (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// ����ǰ �ٴڳ� ȿ�� ���
			if (EFFECT_CONSTANT_SUPPLYGOODS_RANOUT != m_siEffectSupplyGoodsType)
			{
				SetSiegeWarfare_EffectSupplyGoods(EFFECT_CONSTANT_SUPPLYGOODS_RANOUT);
			}
		}
	}
	*/

	if (m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount > m_siSiegeWarfareUnit_SupplyGoodsAmount)
		m_siSiegeWarfareUnit_SupplyGoodsAmount = m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;
}

// ������ ���� ����ǰ ���緮 ���
SI32 cltOnlineChar::GetSiegeWarfare_CurrentSupplyGoodsAmount(void)
{
	return m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;
}

// ������ ���� ����ǰ �ִ뷮 ����
void cltOnlineChar::SetSiegeWarfare_SupplyGoodsAmount(SI32 siSupplyGoodsAmount)
{
	m_siSiegeWarfareUnit_SupplyGoodsAmount = siSupplyGoodsAmount;
}

// ������ ���� ����ǰ �ִ뷮 ���
SI32 cltOnlineChar::GetSiegeWarfare_SupplyGoodsAmount(void)
{
	return m_siSiegeWarfareUnit_SupplyGoodsAmount;
}

// ������ ������ ���� ���� �ð� ����
void cltOnlineChar::SetSiegeWarfareUnit_LastAttackDelay(DWORD dwLastAttackDelay)
{
	m_dwSiegeWarfareUnit_LastAttackDelay = dwLastAttackDelay;
}

// ������ ������ ����� ���� ���� �ð� ����
void cltOnlineChar::SetSiegeWarfareUnit_PastAttackDelay(DWORD dwPastAttackDelay)
{
	if (m_dwSiegeWarfareUnit_LastAttackDelay > dwPastAttackDelay)
		m_dwSiegeWarfareUnit_LastAttackDelay -= dwPastAttackDelay;
	else
		m_dwSiegeWarfareUnit_LastAttackDelay = 0;
}

// ������ ������ ���� ���� �ð� ���
DWORD cltOnlineChar::GetSiegeWarfareUnit_LastAttackDelay(void)
{
	return m_dwSiegeWarfareUnit_LastAttackDelay;
}
// �� /////////////////////////////////////////

// robypark 2004/11/2 17:55
// ������ �������� ���� ȿ�� ���
void cltOnlineChar::SetSiegeWarfare_TransformEffect(SI32 siSiegeWarfareTransformType)
{
	m_siSiegeWarfareTransformType = siSiegeWarfareTransformType;
	m_siEffectTransformFrame = 0;
}

// robypark 2004/11/2 18:4
// ������ ���� ����ǰ �ڵ� �Ҹ�(1%)
void cltOnlineChar::WasteSiegeWarfareSupplyGoods(void)
{
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount -= (m_siSiegeWarfareUnit_SupplyGoodsAmount * 1 / 100);

	if (0 > m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount)
	{
		m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = 0;

		// robypark 2005/1/6 13:20
		// ����ǰ�� ������ ���޼������� ���޹޾ƶ�....
		if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
		{
			if (FALSE == IsSpeech())	// ���ϴ� ���� �ƴ϶��
				SetSpeech(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SPEECH_RANOUT_SUPPLYGOODS), FALSE, NULL);
		}

		// ����ǰ �ٴڳ� ȿ�� ���
		if (EFFECT_CONSTANT_SUPPLYGOODS_RANOUT != m_siEffectSupplyGoodsType)
		{
			SetSiegeWarfare_EffectSupplyGoods(EFFECT_CONSTANT_SUPPLYGOODS_RANOUT);
		}
	}
}

// robypark 2004/12/10 11:41
// ������ ������ ������ ���� ���ݹ޴� ȿ�� ����
void	cltOnlineChar::SetSiegeWarfare_VillageCountAttack(SI32 siEffectVillageConterAttackType)
{
	m_siEffectVillageConterAttackType	=	siEffectVillageConterAttackType;
	m_siEffectVillageConterAttackFrame	=	0;
}

// robypark 2004/12/2 19:2
// ������ ������ ����ǰ ���� ȿ�� ����. ����ǰ�� �޾Ұų�(1) ����ǰ�� �ٴڳ� ���(2)
void	cltOnlineChar::SetSiegeWarfare_EffectSupplyGoods(SI32 siEffectSupplyGoodsType)
{
	m_siEffectSupplyGoodsType	= siEffectSupplyGoodsType;
	m_siEffectSupplyGoodsFrame	= 0;
}

// robypark 2004/11/24 18:31
// ������ �߰��� ��� ȿ�� ���
void	cltOnlineChar::SetSiegeWarfare_Evasion(void)
{
	m_bSiegeWarfareEvasion = TRUE;
	m_siEffectEvasionFrame = 0;
}

// robypark 2004/11/24 18:31
// ������ ġ���� ���� ȿ�� ���
void	cltOnlineChar::SetSiegeWarfare_DeathBlow(void)
{
	m_bSiegeWarfareDeathblow = TRUE;
	m_siEffectDeathblowFrame = 0;
}

void    cltOnlineChar::SetCaptain(BOOL	bCaptain)
{
	m_bCaptain = bCaptain;
}

BOOL	cltOnlineChar::GetCaptain()
{
	return m_bCaptain;
}
/*
void	cltOnlineChar::SetSupplyCharge(SI16 siSupply)
{
	m_siSupplyCharge = siSupply;
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = m_siSupplyCharge;
}

SI16	cltOnlineChar::GetSupplyCharge()
{
	return m_siSupplyCharge;
}
*/
// robypark 2004/11/12 21:15
// ������ ������  ü�� �� ����ǰ ��ġ(%) �׸���
void	cltOnlineChar::DrawSiegeWarfareUnit_StatusBar(void)
{
	// ��� �̹��� ���
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_BACKGROUND);

	// ������� ���
	if (pMyOnlineWorld->pMyData->GetMyCharUniqueID() == GetUniqueID())
	{
		// ������ ���� ������ ��쿡�� ����ǰ �������� �׸���.
		if (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// ��� �̹��� ���
			if (psprBackGround)
			{
				clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
									psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
									psprBackGround->Image);
			}

			// ������ ���� ü�� ������ ���
			XSPR	*psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);

			if (psprGaugeHealth)
			{
				// �׷������� �ۼ�Ʈ ���
				SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

				// �׸���
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
			}

			// ������ ���� ����ǰ ������ ���
			XSPR	*psprGaugeSupplyGoodsAmount = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

			if (psprGaugeSupplyGoodsAmount)
			{
				// �׷������� �ۼ�Ʈ ���
				SI32	siPct = (m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount * 10) / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount;

				// �׸���
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1 + 3 + 1, siPct + 1, psprGaugeSupplyGoodsAmount);
			}
		}
		else	// ü�� �������� �׸���.
		{
			// ��� �̹��� ���
			if (psprBackGround)
			{
				clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
									psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
									0, 0,
									psprBackGround->Header.Xsize, 5,
									psprBackGround->Image);
			}

			// ������ ���� ü�� �������� ���
			XSPR	*psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);

			if (psprGaugeHealth)
			{
				// �׷������� �ۼ�Ʈ ���
				SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

				// �׸���
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
			}
		}
	}
	else	// ����ڿ��� ������ ü�� �������� ���
	{
		// ��� �̹��� ���
		if (psprBackGround)
		{
			clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
								psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
								0, 0,
								psprBackGround->Header.Xsize, 5,
								psprBackGround->Image);
		}

		// ������ ���� ü�� �������� ���
		XSPR	*psprGaugeHealth = NULL;

		// ����ڿ� ���� ����ΰ�?
		if (pMyOnlineWorld->IsMemberOfSameGuild(GetUniqueID()))
		{
			psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
		}
		// ���� ��ܿ��ΰ�?
		else if (GetIsHostilityGuild())
		{
			psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
		}
		else	// �� ���� ��� �ȱ׸���.
			psprGaugeHealth = NULL;

		if (psprGaugeHealth)
		{
			// �׷������� �ۼ�Ʈ ���
			SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

			// �׸���
			DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
		}		
	}
}

// robypark 2004/12/16 14:33
// ������ �����ΰ�?
BOOL	cltOnlineChar::IsSiegeWarfareUnit(void)
{
	return m_bIsSiegeWarfareUnit;
}

// robypark 2004/12/17 17:20
// ����ڰ� ���� ���� �������̽� �� �ִ°�?
BOOL	cltOnlineChar::IsInVillageDefenceInterface(void)
{
	return m_bIsInVillageDefenceInterface;
}

// robypark 2004/12/17 17:20
// ĳ���Ͱ� ���� ���� �������̽� �� �ִ°�? ����
void	cltOnlineChar::SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface)
{
	m_bIsInVillageDefenceInterface = bIsInVillageDefenceInterface;
}

// robypark 2005/1/14 18:39
// ĳ���͸� �׸� �� �׷������� �߽� ��ġ ���
void	cltOnlineChar::GetCharacterDrawCenterPos(SI16 &siCenterX, SI16 &siCenterY)
{
	siCenterX = m_siDrawCenterX;
	siCenterY = m_siDrawCenterY;
}

// robypark 2005/2/17 21:44
// ���콺 Ŀ�� �Ʒ� ĳ���������� �����Ѵ�.
void	cltOnlineChar::SetCursorHoverCharacter(BOOL bHover)
{
	m_bCursorHoverCharacter	= bHover;
}
