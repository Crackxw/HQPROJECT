/************************************************************************************************
	파일명 : OnlineChar.cpp
	담당자 : 이윤석

	현재 Game에서 존재하는 Player의 정보를 관리한다.
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

// 공성 유닛 관리자
#include	"SiegeUnitTableMgr.h"

//-----------------------------------------------------------------------------
// actdoll (2004/03/11 13:58) : 크리티컬 버그를 잡기위한 디버그용 파일 생성 클래스
//-----------------------------------------------------------------------------
#include	"FieldHeroInfoMgr.h"

extern	_KindInfo					KI[];								// 캐릭터 종류별 데이터 

const	R32		DF_VALUE_CHAR_MOVE_SPEED_IN_SPECIAL_BOOTS	= 1.5f;		// 시간제 신발 아이템 착용 시 이동 속도(기존의 1.5배)

// robypark 2004/10/28 15:3
// 수치를 게이지 형태로 그려준다.
extern void DrawGauge(SI32 siX, SI32 siY, SI32 siWidth, SI32 siHeight, R32 fRate, UI08 uiClrGaugeFilled, UI08 uiClrGaugeEmpty);

// robypark 2005/1/7 18:41
// 수치(10%단위로)를 게이지 형태로 그려준다.
// 공성전 유닛 체력, 보급 게이지에서 사용
extern void DrawGaugeCharacter(SI32 siX, SI32 siY, SI32 siRate10, XSPR *psprImage);

// robypark 2005/1/7 18:41
// 수치를 게이지 형태로 그려준다.
// 마을 방어력, 수비병 게이지 출력에서 사용
extern void DrawGaugeVillage(SI32 siX, SI32 siY, SI32 siRate, XSPR *psprImage);


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
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
	m_siSiegeWarfareTransformType = 0;	// 공성전 유닛으로 변신 중인지를 나타내는 플래그
	m_siEffectTransformFrame = 0;		// 공성전 유닛 변신 프레임

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// 공성전 유닛이 보급품 관련 효과. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	m_siEffectSupplyGoodsFrame = 0;		// 공성전 유닛 보급품 관련 애니메이션 프레임
	
	m_bSiegeWarfareEvasion = FALSE;		// 공성전 유닛 견고한 방어 이펙트 사용여부
	m_siEffectEvasionFrame = 0;			// 공성전 유닛 견고한 방어 이펙트 프레임

	m_bSiegeWarfareDeathblow = FALSE;	// 공성전 유닛 치명적 공격 이펙트 사용여부
	m_siEffectDeathblowFrame = 0;		// 공성전 유닛 치명적 공격 이펙트 프레임

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// 공성전 마을 반격 타입
	m_siEffectVillageConterAttackFrame	= 0;		// 공성전 마을 반격 애니메이션 프레임

	// robypark 2004/12/16 10:42
	// 캐릭터의 좌표를 전송하였는지를 나타내는 플래그
	m_bIsSendedPlayerPos = FALSE;

	m_nScore	= 0;
	m_nRank		= 0;
	m_bInvalidBattle = FALSE;
	ZeroMemory(uiEquip, sizeof(UI16)*WEAR_ITEM_NUM);

	m_bCaptain = FALSE;

	m_siPositionGaugeX = m_siPositionGaugeY = 0;

	// robypark 2004/12/16 15:2
	m_bIsSiegeWarfareUnit	= FALSE;					// 공성전 유닛인지를 나타내는 플래그

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// 캐릭터가 현재 수성 인터페이스 상에 있는가?

	m_bIsRefugee		= FALSE;
	m_siRefugeeKind		= 0;

	// robypark 2005/1/27 11:1
	m_siAnimationResourceID_Blue	=	-1;			// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	m_siAnimationResourceID_Red		=	-1;			// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// 이 캐릭터와 사용자와의 관계

	// 공성전 수성인터페이스 마을 수리 중 캐릭터 애니메이션. 망치질
	m_scInVillageDefenceInterfaceAniType	=	VDI_ANIMATION_TYPE_REPAIR_HAMMER;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과
	m_scCurrentMoveActionSkip	= 0;				// 이동 처리 함수 Move() 딜레이	현재. 프레임 skip 효과

	m_bCursorHoverCharacter	= FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	생성자.
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
	m_siSiegeWarfareTransformType = 0;	// 공성전 유닛으로 변신 중인지를 나타내는 플래그
	m_siEffectTransformFrame = 0;		// 공성전 유닛 변신 프레임

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// 공성전 유닛이 보급품 관련 효과. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	m_siEffectSupplyGoodsFrame = 0;	// 공성전 유닛 보급품 관련 애니메이션 프레임

	m_bSiegeWarfareEvasion = FALSE;		// 공성전 유닛 견고한 방어 이펙트 사용여부
	m_siEffectEvasionFrame = 0;			// 공성전 유닛 견고한 방어 이펙트 프레임

	m_bSiegeWarfareDeathblow = FALSE;	// 공성전 유닛 치명적 공격 이펙트 사용여부
	m_siEffectDeathblowFrame = 0;		// 공성전 유닛 치명적 공격 이펙트 프레임

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// 공성전 마을 반격 타입
	m_siEffectVillageConterAttackFrame	= 0;		// 공성전 마을 반격 애니메이션 프레임

	m_bCaptain = FALSE;

	// robypark 2004/12/16 10:42
	// 캐릭터의 좌표를 전송하였는지를 나타내는 플래그
	m_bIsSendedPlayerPos = FALSE;

	// robypark 2004/12/16 15:2
	m_bIsSiegeWarfareUnit	= FALSE;					// 공성전 유닛인지를 나타내는 플래그

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// 캐릭터가 현재 수성 인터페이스 상에 있는가?

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치 설정
	m_siDrawCenterX	=	0;
	m_siDrawCenterY	=	0;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과
	m_scCurrentMoveActionSkip	= 0;				// 이동 처리 함수 Move() 딜레이	현재. 프레임 skip 효과

	m_bCursorHoverCharacter	= FALSE;
}


//----------------------------------------------------------------------------------------------------------------
//	설명		:	초기화.
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

	// 공성전 관련 값 초기화
	m_siFieldHp										= 0;	// 공성전 유닛의 현재 체력
	m_siFieldHp_Maximum								= 0;	// 공성전 유닛의 최대 체력
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount	= 0;	// 공성전 유닛 보급품 현재량
	m_siSiegeWarfareUnit_SupplyGoodsAmount			= 0;	// 공성전 유닛 보급품 최대량
	m_dwSiegeWarfareUnit_LastAttackDelay			= 0;	// 공성전 유닛 공격 지연시간
	m_bCaptain										= FALSE;

	// robypark 2004/11/9 21:34
	m_siSiegeWarfareTransformType = 0;	// 공성전 유닛으로 변신 중인지를 나타내는 플래그
	m_siEffectTransformFrame = 0;		// 공성전 유닛 변신 프레임

	// robypark 2004/12/2 19:10
	m_siEffectSupplyGoodsType = 0;		// 공성전 유닛이 보급품 관련 효과. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	m_siEffectSupplyGoodsFrame = 0;	// 공성전 유닛 보급품 관련 애니메이션 프레임

	m_bSiegeWarfareEvasion = FALSE;		// 공성전 유닛 견고한 방어 이펙트 사용여부
	m_siEffectEvasionFrame = 0;			// 공성전 유닛 견고한 방어 이펙트 프레임

	m_bSiegeWarfareDeathblow = FALSE;	// 공성전 유닛 치명적 공격 이펙트 사용여부
	m_siEffectDeathblowFrame = 0;		// 공성전 유닛 치명적 공격 이펙트 프레임

	// robypark 2004/12/10 11:38
	m_siEffectVillageConterAttackType	= 0;		// 공성전 마을 반격 타입
	m_siEffectVillageConterAttackFrame	= 0;		// 공성전 마을 반격 애니메이션 프레임

	// robypark 2004/12/16 10:42
	// 캐릭터의 좌표를 전송하였는지를 나타내는 플래그
	m_bIsSendedPlayerPos = FALSE;

	// robypark 2004/12/17 17:17
	m_bIsInVillageDefenceInterface	= FALSE;			// 캐릭터가 현재 수성 인터페이스 상에 있는가?

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치 설정
	m_siDrawCenterX	=	0;
	m_siDrawCenterY	=	0;

	MoveFlag    = 0;

	sifDataType = 0;

	m_ChangeEffectSex   = 0;
	m_ChangeEffectFrame = 0;

	// 캐릭터 선택을 표시하는 변수 관련 초기화. 
	NewSelectedFlag		=  FALSE;
	NewSelectedDelay	=  0;

	m_siMoveCharAnimationIndex		= 0;
	m_siWait0CharAnimationIndex		= 0;
	m_siAttackCharAnimationIndex	= 0;
	m_siMagicCharAnimationIndex		= 0;

	// robypark 2005/1/27 11:1
	m_siAnimationResourceID_Blue	=	-1;			// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	m_siAnimationResourceID_Red		=	-1;			// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// 이 캐릭터와 사용자와의 관계

	// 수성인터페이스에 있는 캐릭터일 경우 랜덤하게 그 수성 중인 애니메이션을 보여주도록 한다.
	m_scInVillageDefenceInterfaceAniType = rand() % 3;

	// robypark 2005/2/15 16:32
	m_scMaxMoveActionSkip		= 0;					// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과
	m_scCurrentMoveActionSkip	= 0;				// 이동 처리 함수 Move() 딜레이	현재. 프레임 skip 효과

	m_bCursorHoverCharacter	= FALSE;

	// 캐릭터가 이동할 수 있는 지역을 설정한다.
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

	// 맵에 자기의 ID를 설정한다.
	ptCharPos					=	SetID(siX, siY);
	
	m_Imoticon_StartCount = 0;
	m_Imoticon_Type       = 0;

	if(ptCharPos.x != -1 && ptCharPos.y != -1)
	{
		//DWriteStringToFile("cltOnlineChar::Init 1 - 1");		
		//if(ISSHIP(uiUniqueID) == TRUE)
		//	clGrp.Error("배 좌표", "[%d] [%d] [%d] [%d]", siX, siY, ptCharPos.x, ptCharPos.y);

		// 캐릭터를 맵에 정상적으로 심었다.
		siX		=	SI16(ptCharPos.x);
		siY		=	SI16(ptCharPos.y);		

		//m_siFile			=	0;	
		//siFrameNum			=	0;	
		//m_bReverseSwitch	=	FALSE;

		siCharKindIndex		=	pMyOnlineWorld->pIOnlineCharKI->GetIndex(uiCharID);

		if(siCharKindIndex == 0)
			clGrp.Error("", pMyOnlineWorld->pOnlineText->Get(ON_TEXT_UNKNOWN_CHAR_KIND), siCharKindIndex, uiCharID);

		// 실제 플레이어일 경우에만 인자로 넘어오는 spzID를 사용한다.
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
			// KindInfo에서 얻어와야 한다.
			uiName			=	pMyOnlineWorld->pIOnlineCharKI->GetName(siCharKindIndex);
			strncpy(szCharName, pMyOnlineWorld->pOnlineText->Get(uiName), ON_ID_LENGTH);			
		}

		// 캐릭터 애니메이션 데이타에서 내 캐릭터 부분의 포인터를 얻어온다.	
		//pCharAniInfo	=	pMyOnlineWorld->pOnlineCharAni->GetCharAniInfo(uiCharID);		

		//if(pCharAniInfo == NULL)
		//{
		//	clGrp.Error("애니메이션 정보를 얻는데 실패했습니다.", "[%d]", uiCharID);
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
		// 이동하려는 좌표를 이전에 전송하였던 값을 저장하고 있는 변수
		m_siPrevSendedNX=	siX;
		m_siPrevSendedNY=	siY;

		bMovingSwitch	=	FALSE;

		// 캐릭터 Kind Index 설정
		// 플레이어나 몬스터일 경우에만 있다.
		if(ISPLAYER(uiUniqueID) || ISMONSTER(uiUniqueID) || ISSHIP(uiUniqueID))
			m_siImKind			=	TransKindOnlineToImjin(siKind);
		else
			m_siImKind			=	0;

		// robypark 2004/12/16 15:2
		m_bIsSiegeWarfareUnit	= pMyOnlineWorld->IsSiegeWarfareUnit(m_siImKind);	// 공성전 유닛인지를 나타내는 플래그 설정

		// actdoll (2005/02/14 16:11) : 무영풍 신발 속도에 대한 계산식을 %식으로 수정하여 처리
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
					// 공성전 유닛 아닐 경우 이동속도를 올려주는 아이템(무영풍)에 영향을 받지 않도록 수정
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
		// 공성전 시간이라면,
		if (pMyOnlineWorld->IsInTimeSiegeWarfare())
		{
			// 본 맵이라면,
			if (0 == pMyOnlineWorld->pOnlineMap->m_MapIndex)
			{
				// 주인공 관련 데이터 관리자 얻기
				CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(siKind);
				// 주인공 캐릭터라면,
				if (NULL != pFieldHeroInfo)
				{
					if ((pFieldHeroInfo->GetRefugeeKind())								// 피난민 코드 존재
						&& (pMyOnlineWorld->pMyData->GetMyCharUniqueID() != uiUniqueID)	// 사용자 캐릭터 아님
						&& (pMyOnlineWorld->pMyData->IsSiegeWarfareDay()))				// 사용자가 공성을 한다면,
					{
						// 피난민 캐릭터로 작동한다.
						m_bIsRefugee = TRUE;
						m_siRefugeeKind = pFieldHeroInfo->GetRefugeeKind();
					}
				}
			}
		}

		// 피난민 캐릭터가 아니라면
		if (FALSE == m_bIsRefugee)
		{
			// 조합형 캐릭터 정보 초기화
			if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
			{
				m_siHeroAniAction		= HERO_ACT_N1;
				m_siHeroBaseWClass		= HERO_CLASS_HTH;			
				m_siHeroWeaponEnum		= CHAR_NORMALSPEED;
				SI32 direction			= 0;

				m_siWeaponDrawID		= -1;

				//-----------------------------
				// 디폴트 몸 코드를 무엇으로 할 것인가 결정
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


				

				// 이미지의 가로 세로 사이즈를 구한다. 
				filexsize = 192;
				fileysize = 142;
			}
			// 조합 아닌 일반 캐릭터
			else
			{
				SI32 file;
				// 캐릭터에 필요한 파일이 없다면 불러온다.
				for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
				{
					file = KI[m_siImKind].m_clCharAnimation[i].siFile;
					if( file )
					{
						if( Return_Image(file) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
						{
							// 불필요하게 오래된 이미지를 지운다. 
							FreeOldImageByTime();
							
							// 필요한 이미지를 로딩한다. 
							LoadImgFile(file);
						}
					}
				}

				filexsize	= GetFileXsize(KI[m_siImKind].m_clCharAnimation[1].siFile);
				fileysize	= GetFileYsize(KI[m_siImKind].m_clCharAnimation[1].siFile);

				// robypark 2005/1/27 11:43
				// 공성전 유닛일 경우
				CSiegeUnitTableMgr	*pSiegeUnitTableMgr = pMyOnlineWorld->GetSiegeUnitTableMgr();

				LPSSiegeUnitTable lpsSiegeUnitTable = pSiegeUnitTableMgr->GetSiegeUnitTable(siKind);
				if (lpsSiegeUnitTable)
				{
					// robypark 2005/2/3 12:29
					// 최대 체력, 최대 마력 값 설정
					m_siFieldHp_Maximum						= lpsSiegeUnitTable->m_siBasicHP + pSiegeUnitTableMgr->GetIncreaseHP(siFightPower);
					m_siSiegeWarfareUnit_SupplyGoodsAmount	= lpsSiegeUnitTable->m_siBasicMP + pSiegeUnitTableMgr->GetIncreaseMP(siFightPower);

					// robypark 2005/2/15 16:32
					m_scMaxMoveActionSkip		= lpsSiegeUnitTable->m_scMaxMoveActionSkip;		// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과
					m_scCurrentMoveActionSkip	= 0;				// 이동 처리 함수 Move() 딜레이	현재. 프레임 skip 효과

					// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
					m_siAnimationResourceID_Blue	=	lpsSiegeUnitTable->m_siAnimationResourceIdx_Blue;

					if( -1 != m_siAnimationResourceID_Blue )
					{
						if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
						{
							// 불필요하게 오래된 이미지를 지운다. 
							FreeOldImageByTime();
							
							// 필요한 이미지를 로딩한다. 
							LoadImgFile(m_siAnimationResourceID_Blue);
						}
					}
					
					// 해당 공성전 유닛 이미지 파일. 적군 유닛일 때
					m_siAnimationResourceID_Red		=	lpsSiegeUnitTable->m_siAnimationResourceIdx_Red;

					if( -1 != m_siAnimationResourceID_Red )
					{
						if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
						{
							// 불필요하게 오래된 이미지를 지운다. 
							FreeOldImageByTime();
							
							// 필요한 이미지를 로딩한다. 
							LoadImgFile(m_siAnimationResourceID_Red);
						}
					}
				}
			}

			// 캐릭터의 영역을 구한다. 
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
		else	// 피난민 캐릭터다.
		{
			SI32 file;
			// 캐릭터에 필요한 파일이 없다면 불러온다.
			for( i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
			{
				file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
				if( file )
				{
					if( Return_Image(file) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
					{
						// 불필요하게 오래된 이미지를 지운다. 
						FreeOldImageByTime();
						
						// 필요한 이미지를 로딩한다. 
						LoadImgFile(file);
					}
				}
			}

			filexsize	= GetFileXsize(KI[m_siRefugeeKind].m_clCharAnimation[1].siFile);
			fileysize	= GetFileYsize(KI[m_siRefugeeKind].m_clCharAnimation[1].siFile);

			// 캐릭터의 영역을 구한다. 
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

		// 명령 버퍼를 초기화 한다.
		ClearOrder();	

		// 현재 명령을 초기화해준다.
		ZeroMemory(&CurrentOrder, sizeof(CurrentOrder));
		CurrentOrder.siOrder	=	ON_ORDER_WAIT;		

		// 애니메이션을 갱신한다.		
		UpdateAnimation(TRUE, NULL, NULL);	

		AddOrder(ON_ORDER_WAIT, 0, 0);	

		pMyOnlineWorld->pOnlineMegaText->CreateGDIPage(uiUniqueID);						

		return TRUE;
	}
	else
	{

		// cltOnlineChar::uiUniqueID를 0으로 세팅한다.
		cltOnlineChar::uiUniqueID	=	0;

		// 캐릭터를 맵에 설정할 수 없다.
		return FALSE;
	}

	m_MercenaryKind = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명		:	장착 Item 지정
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
//	설명		:	캐릭터를 없앤다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::Free()
{
//	DWriteStringToFile("Free :[%u]", uiUniqueID);
	
	if(uiUniqueID != 0)
	{
		// Chatting Server에 알림
		if(ISPLAYER(uiUniqueID))
		{
/*			FILE* fp;
			fp = fopen("ShareFile.txt","at");
			fprintf(fp,"cltCharOrg::Free() [%d] \n",siID);
			fclose(fp);*/
			//캐릭터를 Free()할때 마지막인자가 모든 파일을 초기화 시켜주는지 여부를 판단한다.
			//지누기 임시로 막음.. 여기서 따운뎀 
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

		// 맵에서 ID를 삭제한다.
		DelID();

		// 그리고 현재 예약해둔 자리가 있다면 그 자리도 삭제해 준다.
		if(m_siX != m_siMX || m_siY != m_siMY)			
		{
			// 현재 예약했던 자리를 없앤다.			
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
		
		// 명령 버퍼를 초기화 한다.
		ClearOrder();	
		
		// 현재 명령을 초기화해준다.
		ZeroMemory(&CurrentOrder, sizeof(CurrentOrder));
		CurrentOrder.siOrder	=	ON_ORDER_WAIT;
	}
}

//----------------------------------------------------------------------------------------------------------------
//	설명		:	캐릭터를 화면에 그린다.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::Draw(SI16 WaterP)
{
	if(m_fShow == FALSE) 
	{
		return;
	}

	// 피난민 캐릭터 처리일 경우
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

	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for( SI32 i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[m_siImKind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				// 필요한 이미지를 로딩한다. 
				LoadImgFile(file);
			}
		}
	}

	// robypark 2005/1/27 11:49
	// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	if( -1 != m_siAnimationResourceID_Blue )
	{
		if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
		{
			// 필요한 이미지를 로딩한다. 
			LoadImgFile(m_siAnimationResourceID_Blue);
		}
	}

	// robypark 2005/1/27 11:49
	// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	if( -1 != m_siAnimationResourceID_Red )
	{
		if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
		{
			// 필요한 이미지를 로딩한다. 
			LoadImgFile(m_siAnimationResourceID_Red);
		}
	}

	// 화면에 출력할 위치를 계산한다.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// 현재 맵이 나에게 포커스를 맞추고 있다면
	// RX, RY를 수정하지 않는다.
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

	// 일반 캐릭터(피난민이 아닐) 경우
	// 실제로 Character 화면에 찍혀질 시작 위치를 다시 계산한다.
	siStartX			=	siCenterX - m_siXSize / 2;
	siStartY			=	siCenterY - m_siYSize + KI[m_siImKind].m_ssBottomIndent;

	// 대사를 찍을 좌표를 구한다.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 90;

	// 이름을 찍을 좌표를 구한다.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치 설정
	m_siDrawCenterX	=	siCenterX;
	m_siDrawCenterY	=	siCenterY - 10;

	XSPR*		lpGuildClassEffect;

	// robypark 2005/1/27 11:54
	m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;		// 이 캐릭터와 사용자와의 관계

	// 길드 직급에 관한 표현
	lpGuildClassEffect = NULL;

	// robypark 2004/10/27 17:22
	// 공성전 유닛이 아닌 경우 상단 직급 표현을 해준다. 수성 인터페이스에 있는 경우에도
	if (FALSE == m_bIsSiegeWarfareUnit)
	{
		if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
		{
			// 행수
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HNS);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
		{
			// 대방
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DAI);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
		{
			// 행동대장
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HND);
		}
		else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
		{
			// 대행수
			lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DHS);
		}
	}
	// 공성전 유닛의 바닥 이펙트 처리
	else
	{
		// 상단에 가입한 게임 사용자 캐릭터라면,
//		if (pMyOnlineWorld->pMyData->GetGuildName())
		//if (FALSE == m_bIsInVillageDefenceInterface)	// 수성 인터페이스에 있지 않은 캐릭터일 경우만 그림
		{
			// robypark 2004/11/9 19:49
			// 공성전 유닛의 공격 방법 값 얻기
			// 대장효과(공격력 업), 대장인지 등의 효과 처리 추가
			SI16 scGeneralEffect = pMyOnlineWorld->SiegeWarfare_IsInAreaGeneralEffect(GetUniqueID());

			// 적대 길드인지 검사
			if (TRUE == GetIsHostilityGuild())
			{
				// 적대 상단이므로 붉은 색 바닥 이펙트를 사용한다.
				m_scForceRelation = SIEGEWARFARE_FORCE_ENEMY;
			}
			// 같은 상단인지 검사
			else if (((char*)GetGuildName()) && (pMyOnlineWorld->pMyData->GetGuildName()))
			{
				if (strcmp((char*)GetGuildName(), pMyOnlineWorld->pMyData->GetGuildName()) == 0)
				{
					// 같은 상단이므로 파란 색 바닥 이펙트를 사용한다.
					m_scForceRelation = SIEGEWARFARE_FORCE_FRIENDLY;
				}
			}
			else
			{
				// 다른 상단 공성 유닛
				m_scForceRelation = SIEGEWARFARE_FORCE_UNRELATED;
			}

			// 중립 상단원인 경우 바닥 이펙트 안그린다.
			if (SIEGEWARFARE_FORCE_UNRELATED != m_scForceRelation)
			{
				// 대장효과(공격력 업)일 경우
				if (ON_ATTACK_TYPE_NEARCAPTAIN == scGeneralEffect)
				{
					// 대장 효과 스프라이트 리소스 얻기
					XSPR *pSprEffectGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_AURA_BLUE + m_scForceRelation);

					if (NULL != pSprEffectGeneral)
					{
						// 이펙트 그리기
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprEffectGeneral->Header.Xsize / 2), siCenterY - (pSprEffectGeneral->Header.Ysize / 2) - 24, 
													pSprEffectGeneral->Header.Xsize, pSprEffectGeneral->Header.Ysize, 
													&pSprEffectGeneral->Image[pSprEffectGeneral->Header.Start[((GetTickCount() / 100) % pSprEffectGeneral->Header.font_number)]]);
					}
				}
				// 대장일 경우
				else if (ON_ATTACK_TYPE_CAPTAIN == scGeneralEffect)
				{
					// 대장 표시 스프라이트 리소스 얻기
					XSPR *pSprGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_GENERAL_BLUE + m_scForceRelation);

					if (NULL != pSprGeneral)
					{
						// 이펙트 그리기
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprGeneral->Header.Xsize / 2), siCenterY - (pSprGeneral->Header.Ysize / 2) - 24, 
													pSprGeneral->Header.Xsize, pSprGeneral->Header.Ysize, 
													&pSprGeneral->Image[pSprGeneral->Header.Start[((GetTickCount() / 100) % pSprGeneral->Header.font_number)]]);
					}
				}
				else	// 사환 등의 졸병 유닛인 경우
				{
					// 졸병 표시 스프라이트 리소스 얻기
					XSPR *pSprGeneral = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_SOLDIER_BLUE_EFFECT + m_scForceRelation);

					if (NULL != pSprGeneral)
					{
						// 이펙트 그리기
						clGrp.PutSpriteLightImjinCT(siCenterX - (pSprGeneral->Header.Xsize / 2), siCenterY - (pSprGeneral->Header.Ysize / 2) - 24, 
													pSprGeneral->Header.Xsize, pSprGeneral->Header.Ysize, 
													&pSprGeneral->Image[pSprGeneral->Header.Start[((GetTickCount() / 100) % pSprGeneral->Header.font_number)]]);
					}
				}

				// 아군 적군 표시
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

	// 수호부 이펙트
	UI16 uiGuardianItem = uiEquip[ON_WEAR_GUARDIAN_ITEM];
	CItemHeader *pItemHeader = pMyOnlineWorld->pItemsInfo->GetItem(uiGuardianItem);
	// 수호부 이펙트는 공성전 유닛이 아닐 경우에만 그려지도록 한다. 또한 수성 인터페이스에 있지 않은 경우
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

	// 주인공 캐릭터를 그린다.
	DrawChar(WaterP, siStartX, siStartY, siCenterX, siCenterY);

	switch(m_BattleFlag)
	{
		case 1:		// 현재 전투 대기중이면 그에 대한 표시
			{
				XSPR*		pWaitEffect;

				pWaitEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEWAITEFFECT);

				clGrp.PutSpriteLightImjinCT(siCenterX - (pWaitEffect->Header.Xsize / 2), siCenterY - 10 - (pWaitEffect->Header.Ysize / 2), 
										pWaitEffect->Header.Xsize, 
										pWaitEffect->Header.Ysize, 
										&pWaitEffect->Image[pWaitEffect->Header.Start[(GetTickCount() / 50) % pWaitEffect->Header.font_number]]);


				// 현재 이 캐릭터가 주인공 캐릭이면
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

					// 사운드 처리
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
		case 1:		// 몬스터가 Target을 찾을시
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

	// 이모티콘
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

// 피난민 드로우.
VOID	cltOnlineChar::DrawRefugee(SI16 WaterP)
{
	SI32	siStartX,  siStartY;
	SI32	siCenterX, siCenterY;
	SI32	siRelateX,	siRelateY;
	SI32	siCurrentMapX, siCurrentMapY;
	RECT	rcScreen;
	
	SI32 file;

	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for( SI32 i=0; i<MAX_ANIMATION_PER_UNIT; i++ )
	{
		file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
		if( file )
		{
			if( Return_Image(file) == NULL )
			{
				// 필요한 이미지를 로딩한다. 
				LoadImgFile(file);
			}
		}
	}

	// 화면에 출력할 위치를 계산한다.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// 현재 맵이 나에게 포커스를 맞추고 있다면
	// RX, RY를 수정하지 않는다.
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


	// 실제로 Character 화면에 찍혀질 시작 위치를 다시 계산한다.
	siStartX			=	siCenterX - m_siXSize / 2;
	siStartY			=	siCenterY - m_siYSize + KI[m_siRefugeeKind].m_ssBottomIndent;

	// 대사를 찍을 좌표를 구한다.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 60;

	// 이름을 찍을 좌표를 구한다.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// robypark 2005/1/14 18:39
	// 캐릭터를 그릴 때 그려지지는 중심 위치 설정
	m_siDrawCenterX	=	siCenterX;
	m_siDrawCenterY	=	siCenterY - 10;

	XSPR*		lpGuildClassEffect;

	// 길드 직급에 관한 표현
	lpGuildClassEffect = NULL;

	// robypark 2004/10/27 17:22
	// 공성전 유닛이 아닌 경우 상단 직급 표현을 해준다. 수성 인터페이스에 있는 경우에도
	if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HNS))
	{
		// 행수
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HNS);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DAI))
	{
		// 대방
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DAI);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_HND))
	{
		// 행동대장
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_HND);
	}
	else if(GetGuildInGrade() == pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_DHS))
	{
		// 대행수
		lpGuildClassEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GUILDEFFECT_DHS);
	}

	if(lpGuildClassEffect)
	{
		clGrp.PutSpriteLightImjinCT(siCenterX - (lpGuildClassEffect->Header.Xsize / 2), siCenterY - (lpGuildClassEffect->Header.Ysize / 2), 
								lpGuildClassEffect->Header.Xsize, lpGuildClassEffect->Header.Ysize, 
								&lpGuildClassEffect->Image[lpGuildClassEffect->Header.Start[(GetTickCount() / 100) % lpGuildClassEffect->Header.font_number]]);
	}

	// 주인공 캐릭터를 그린다.
	DrawChar(WaterP, siStartX, siStartY, siCenterX, siCenterY);

	switch(m_BattleFlag)
	{
		case 1:		// 현재 전투 대기중이면 그에 대한 표시
			{
				XSPR*		pWaitEffect;

				pWaitEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_BATTLEWAITEFFECT);

				clGrp.PutSpriteLightImjinCT(siCenterX - (pWaitEffect->Header.Xsize / 2), siCenterY - 10 - (pWaitEffect->Header.Ysize / 2), 
										pWaitEffect->Header.Xsize, 
										pWaitEffect->Header.Ysize, 
										&pWaitEffect->Image[pWaitEffect->Header.Start[(GetTickCount() / 50) % pWaitEffect->Header.font_number]]);


				// 현재 이 캐릭터가 주인공 캐릭이면
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

					// 사운드 처리
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
		case 1:		// 몬스터가 Target을 찾을시
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

	// 이모티콘
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	반투명으로 그린다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineChar::DrawTransparent(SI16 WaterP)
{
	if(m_fShow == FALSE) return;

	SI32	siStartX,  siStartY;
	SI32	siCenterX, siCenterY;
	SI32	siRelateX,	siRelateY;
	SI32	siCurrentMapX, siCurrentMapY;
	RECT	rcScreen;
	
	// 화면에 출력할 위치를 계산한다.
	pMyOnlineWorld->pOnlineMap->GetCurrentPos(&siCurrentMapX, &siCurrentMapY);
	SI32	siDrawX = m_siX - siCurrentMapX;	
	SI32	siDrawY = m_siY - siCurrentMapY;

	// 자신을 중심으로 화면에 찍어주므로 자신의 상대 이동량은 없다.	
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

	// 실제로 Character 화면에 찍혀질 시작 위치를 다시 계산한다.
	siStartX	=	siCenterX - m_siXSize / 2;
	siStartY	=	siCenterY - m_siYSize + KI[m_siImKind].m_ssBottomIndent;


	// 대사를 찍을 좌표를 구한다.
	siPutSpeechCX	=	siCenterX - 4;
	siPutSpeechY	=	siCenterY  + 15 - 90;

	// 이름을 찍을 좌표를 구한다.
	siPutNameCX		=	siCenterX;
	siPutNameY		=	siCenterY + 5;

	// Character를 화면에 찍는다.
	if(WaterP >= 100)
	{
		if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == TRUE )
		{
			//DrawHero(DRAW_MODE_TRANSPARENT, siStartX, siStartY, siCenterX, siCenterY);	서피스로 옮긴다음 반투명으로 찍는 방법으로 수정하기 전까지 막아둠.
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
			//DrawHero(...);	 서피스로 옮긴다음 반투명으로 찍는 방법으로 수정하기 전까지 막아둠.
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
// 공성전 유닛 바닥 이펙트 관련 상수 선언
#define FORCE_EFFECT_FONT_TOTALFONT_MOVE		13		// 이동시 공성전 유닛 바닥 이펙트 폰트 수
#define FORCE_EFFECT_FONT_TOTALFONT_ATTACK		13		// 공격시 공성전 유닛 바닥 이펙트 폰트 수
#define FORCE_EFFECT_FONT_TOTALFONT_WAIT		8		// 대기시 공성전 유닛 바닥 이펙트 폰트 수
#define FORCE_EFFECT_FONT_BEGIN_MOVE_RED		13		// 홍군 이동시 공성전 유닛 바닥 이펙트 시작 폰트 번호
#define FORCE_EFFECT_FONT_BEGIN_ATTACK_RED		0		// 홍군 공격시 공성전 유닛 바닥 이펙트 시작 폰트 번호
#define FORCE_EFFECT_FONT_BEGIN_WAIT_RED		26		// 홍군 대기시 공성전 유닛 바닥 이펙트 시작 폰트 번호
#define FORCE_EFFECT_FONT_BEGIN_MOVE_BLUE		52		// 청군 이동시 공성전 유닛 바닥 이펙트 시작 폰트 번호
#define FORCE_EFFECT_FONT_BEGIN_ATTACK_BLUE		39		// 청군 공격시 공성전 유닛 바닥 이펙트 시작 폰트 번호
#define FORCE_EFFECT_FONT_BEGIN_WAIT_BLUE		65		// 청군 대기시 공성전 유닛 바닥 이펙트 시작 폰트 번호

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
// robypark 2004/11/4 16:5
// 공성전 유닛 바닥 이펙트 그리기
// [IN] SI08 scForceRelation : 사용자 캐릭터와 그리려는 캐릭터의 관계
//								SIEGEWARFARE_FORCE_FRIENDLY(0)	- 같은 상단원이다.
//								SIEGEWARFARE_FORCE_ENEMY(1)		- 적대 상단원이다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	cltOnlineChar::DrawSiegeWarfareForceEffect(SI08 scForceRelation, SI32 siCenterX, SI32 siCenterY)
{
	SI16	ssStartFontNo				 = 0;		// 이펙트 시작 폰트 번호
	SI16	ssTotalFontNo				 = 1;		// 이펙트 총 폰트 수
	XSPR*	lpSiegeWafareUnitForceEffect = NULL;	// 이펙트 스프라이트 리소스

	switch (scForceRelation)
	{
		// 같은 상단원인 경우
		case SIEGEWARFARE_FORCE_FRIENDLY:
			// 현재 캐릭터 동작에 따른 처리
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

		// 적대 상단원인 경우
		case SIEGEWARFARE_FORCE_ENEMY:
			// 현재 캐릭터 동작에 따른 처리
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

		// 구분되지 않는 공성전 유닛일 경우
		default:
			return;
	}
/*
	// 리소스 얻기
	lpSiegeWafareUnitForceEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_FORCE_EFFECT);

	if (NULL != lpSiegeWafareUnitForceEffect)
	{
		// 이펙트 그리기
		clGrp.PutSpriteLightImjinCT(siCenterX - (lpSiegeWafareUnitForceEffect->Header.Xsize / 2), siCenterY - (lpSiegeWafareUnitForceEffect->Header.Ysize / 2) - 14, 
										lpSiegeWafareUnitForceEffect->Header.Xsize, lpSiegeWafareUnitForceEffect->Header.Ysize, 
										&lpSiegeWafareUnitForceEffect->Image[lpSiegeWafareUnitForceEffect->Header.Start[ssStartFontNo + ((GetTickCount() / 100) % ssTotalFontNo)]]);
	}
*/
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	대사를 찍어준다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::DrawSpeech(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if(m_fShow == FALSE) return;

	// 수성인터페이스에 있는 사용자가 말한 경우 말 풍선 안그린다.
//	if (TRUE == m_bIsInVillageDefenceInterface)
//		return;

	// 말을 하고 있을때에만 찍어준다.
	if(IsSpeech() == TRUE)		
	{		
		pMyOnlineWorld->pOnlineMegaText->Show(uiUniqueID, lpSurface, siPutSpeechCX, siPutSpeechY + 5);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	사용자의 이름을 찍어준다.
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

	// 사용자의 이름을 찍어준다.
	if(pMyOnlineWorld)
	{
		// 공성전 유닛이 아닌 경우
		if (FALSE == m_bIsSiegeWarfareUnit)
		{
			CHAR *pCharName	= pMyOnlineWorld->pMyData->GetMyCharName();
			// 운영자일 경우 글자색을 흰색으로 출력하도록 검사
			if(!strcmp(pCharName, pMyOnlineWorld->pOnlineText->Get(8000495)))	SetTextColor(hdc, RGB(255,255,255));	// 영자일 경우 하얀색

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
				// Text Size를 구함
				TempMaxSize = 0;
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));
					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
					TempMaxSize = TextSize.cx;
				}

				// 아이디 출력
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

				// 길드 출력
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

				// 아이디 출력
				if(pMyOnlineWorld->m_GMFlag == ON_USERGRADE_NORMAL) sprintf((char*)siBuffer, "%s", szCharName);
				else                                                sprintf((char*)siBuffer, "%s(%d)", szCharName, uiUniqueID);

				siTextX		=	siPutNameCX - TempMaxSize;
				siTextY		=	siPutNameY + AddHeight;
				if(siYetPutNameCX > siTextX) siYetPutNameCX = siTextX;

				// actdoll (2004/03/03 18:57) : 글자가 잘 안보여 디버그시만 잘 보이게 해놓았다.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
			}	// if(LineAll)
			else
			{
				// 길드 출력
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

				// 아이디 출력
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

				// actdoll (2004/03/03 18:57) : 글자가 잘 안보여 디버그시만 잘 보이게 해놓았다.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// 공성전 체력 표시.
//				sprintf(siBuffer,"몸빵 = %d",m_siFieldHp);
//				TextOut(hdc, siTextX + 1, siTextY + 16, siBuffer, strlen(siBuffer));
			}	// if(LineAll) => else
		}	// if (FALSE == m_bIsSiegeWarfareUnit)
		// 공성전 유닛일 경우
		else
		{
			CHAR *pCharName	= pMyOnlineWorld->pMyData->GetMyCharName();
			// 운영자일 경우 글자색을 흰색으로 출력하도록 검사
			if(!strcmp(pCharName, pMyOnlineWorld->pOnlineText->Get(8000495)))	SetTextColor(hdc, RGB(255,255,255));	// 영자일 경우 하얀색

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
				// 상단 깃발이 있는 경우
				// Text Size를 구함
				TempMaxSize = 0;
				/*
				if(m_szGuild[0] && (pMyOnlineWorld->m_cltIGuildClass.GetNumericClassCode(SO_GUILD_SC_JWJ) != m_siGuildInGrade))
				{
					sprintf((char*)siBuffer, "[%s:%s]", m_szGuild, pMyOnlineWorld->pOnlineText->Get(pMyOnlineWorld->m_cltIGuildClass.GetClassName(m_siGuildInGrade)));
					GetTextExtentPoint32(hdc, siBuffer, strlen(siBuffer), &TextSize);
					TempMaxSize = TextSize.cx;
				}
				*/

				// 아이디 출력
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

				// 길드 출력
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

					// 공성전 보급 수레일 경우 혹은 사용자 자신일 경우에만 보급품 게이지를 그린다.
					// 상단 깃발 위치 보정
					if ((GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
						|| (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS)))
					{
						siYetPutNameCY += 4;
					}
//					TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));
				}
				/**/

				// 아이디 출력
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

				// actdoll (2004/03/03 18:57) : 글자가 잘 안보여 디버그시만 잘 보이게 해놓았다.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG

				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// 공성전 유닛 체력, 보급 게이지 출력 위치
				m_siPositionGaugeX = siPutNameCX - 26;//siTextX + 1;
				m_siPositionGaugeY = siTextY + 14;
				// 공성전 유닛 체력, 보급 게이지 출력
//				DrawGauge(siTextX + 1, siTextY + 14, 75, 4, (float)m_siFieldHp / (float)m_siFieldHp_Maximum, 235, 253);
//				if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
//					DrawGauge(siTextX + 1, siTextY + 19, 75, 4, (float)m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount, 68, 253);
			}	// if(LineAll)
			else
			{
				// 상단 깃발이 없는 경우
				// 길드 출력
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

				// 아이디 출력
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

				// actdoll (2004/03/03 18:57) : 글자가 잘 안보여 디버그시만 잘 보이게 해놓았다.
#ifdef	_DEBUG
				SetTextColor( hdc, RGB( 0, 0, 0 ) );
				TextOut(hdc, siTextX + 1, siTextY + 1, siBuffer, strlen(siBuffer));
				SetTextColor( hdc, RGB( 255, 255, 255 ) );
#endif//_DEBUG
				TextOut(hdc, siTextX, siTextY, siBuffer, strlen(siBuffer));

				// 공성전 유닛 체력, 보급 게이지 출력 위치
				m_siPositionGaugeX = siPutNameCX - 26;//siTextX + 1;
				m_siPositionGaugeY = siTextY + 14;
				// 공성전 체력, 보급품 표시.
//				sprintf(siBuffer,"몸빵 = %d",m_siFieldHp);
//				TextOut(hdc, siTextX + 1, siTextY + 16, siBuffer, strlen(siBuffer));
//				DrawGauge(siTextX + 1, siTextY + 14, 75, 4, (float)m_siFieldHp / (float)m_siFieldHp_Maximum, 235, 253);
//				if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
//					DrawGauge(siTextX + 1, siTextY + 19, 75, 4, (float)m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount, 68, 253);
			}
		}
	}	// if(pMyOnlineWorld)
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터 이름 앞에 Guild 깃발을 찍는다.(만약 좌표가 -1이면 ID 찍는 부분 앞에 찍는다.)
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
//	설명	:	지금 캐릭터가 말을 하고 있는가?
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsSpeech()
{
	if(szSpeech[0] != NULL)
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 오버헤드.(Action으로 넘어가기전에 해주어야 할 일등을 해준다.)
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::OverHead()
{	
	OnOrder	TempOrderBuffer;	
	SI32	siDirection = 0;

	// 명령 버퍼에서 명령을 얻어온다.
	// 다만 명령이 있을때에만 수행한다.
	// 현재 명령을 받을 수 있는 상황이여야 한다.
	if(bBusySwitch == FALSE && IsOrderInBuffer() == TRUE)		 
	{
		while(IsOrderInBuffer() == TRUE)
		{
			GetOrder(&TempOrderBuffer);		

			if(m_bFieldAttackAni == TRUE)	break;

			// 명령의 프리프로세서.
			if(PreProcessorOrder(&TempOrderBuffer) == TRUE)
			{
				CurrentOrder	=	TempOrderBuffer;

				switch(CurrentOrder.siOrder)
				{		
				case ON_ORDER_WAIT:
					// 정지 하도록 설정한다.
					SetMove(m_siX, m_siY);						
					break;
				case ON_ORDER_MOVE:
					// 이동 하도록 설정한다.
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

						// 현재 공격하려는 캐릭터의 위치를 얻어온다.
						pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siAttackCharX, &siAttackCharY);						

						SetMove(siAttackCharX, siAttackCharY);
						//m_siActionCdoe	=	ON_CHAR_ACTION_CODE_MOVE;
					}
					break;
				case ON_ORDER_FIELDATTACK_READY:
					{
						SI16	siTargetCharX, siTargetCharY;

						// 현재 공격하려는 캐릭터의 위치를 얻어온다.
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
						
						// 오브젝트의 ID를 구한다.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);

						// 이동중이 아니라면 이동을 시킨다.						
						// 마을인가 알아온다.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// 현재 나의 위치와 오브젝트가 있는 위치가 2타일 차이가 나면 이동한다.
							/*if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// 이동하라고 설정해준다.
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
								// 현재 명령을 초기화해준다.
								SetMove(m_siX, m_siY);
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// 이동하라고 설정해준다.
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
						
						// 아이템이 있는 위치로 이동한다.
						SetMove(siItemX, siItemY);
					}
					break;
				case ON_ORDER_USEOBJECT:			
				case ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE:
					{
						SI32	siObjectID;

						// 오브젝트의 ID를 구한다.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

						// 이동중이 아니라면 이동을 시킨다.						
						// 마을인가 알아온다.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// 현재 나의 위치와 오브젝트가 있는 위치가 2타일 차이가 나면 이동한다.
							if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 2)
							{
								// 이동하라고 설정해준다.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// 이동하라고 설정해준다.
								//SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);			
							}
						}											

					}					
					// 명령이 바꼈다면 Break를 수행한다.
					break;				

				case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE:
					{
						SI32	siObjectID;
						
						// 오브젝트의 ID를 구한다.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);

						// 이동중이 아니라면 이동을 시킨다.						
						// 마을인가 알아온다.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// 현재 나의 위치와 오브젝트가 있는 위치가 2타일 차이가 나면 이동한다.
							if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// 이동하라고 설정해준다.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);											
							}
						}
						else
						{
							if(CurrentOrder.siOrderParam1 != m_siX || CurrentOrder.siOrderParam2 != m_siY)
							{
								//SetMessage("Moving");
								
								// 이동하라고 설정해준다.
								//SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
						}											

					}					
					// 명령이 바꼈다면 Break를 수행한다.
					break;

				case ON_ORDER_FIELDATTACK_ACTION:
				case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION:
				case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION:
				case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION:
					m_bFieldAttackAni = TRUE;

					siDirection = 0;

					// 방향 전환
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
				// 공성전 유닛 보급 수레에 보급품 요청보내기
				case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_SUPPLYWAGON:
					{
						SI16	siTargetCharX, siTargetCharY;

						// 현재 공격하려는 캐릭터의 위치를 얻어온다.
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
				// 공성전 유닛 보급 수레가 마을에 보급품 요청보내기
				case ON_ORDER_REQUEST_SUPPLYGOODS_FROM_VILLAGE:
					{
						SI32	siObjectID;
						SI16	siVillagePosX,siVillagePosY;
						SI16	siVillageSizeX,siVillageSizeY;
						
						// 오브젝트의 ID를 구한다.
						siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

						// 이동중이 아니라면 이동을 시킨다.						
						// 마을인가 알아온다.
						if(pMyOnlineWorld->pIOnlineObject->IsAtb(siObjectID, ATB_OBJECT_VILLAGE) == TRUE)
						{
							// 현재 나의 위치와 오브젝트가 있는 위치가 2타일 차이가 나면 이동한다.
							/*if(max(abs(CurrentOrder.siOrderParam1 - m_siX), abs(CurrentOrder.siOrderParam2 - m_siY)) >= 5)
							{
								// 이동하라고 설정해준다.
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
								
								// 이동하라고 설정해준다.
								SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
							}
						}
					}
					break;
				// robypark 2005/1/25 19:8
				// 공성전 유닛 보급 수레가 다른 유닛 치료하기
				case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
					{
						SI16	siTargetCharX, siTargetCharY;

						// 현재 공격하려는 캐릭터의 위치를 얻어온다.
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
							// 현재 나의 위치와 NPC가 있는 위치가 5타일 차이가 나면 이동한다.
							if(max(abs(siNPCX - m_siX), abs(siNPCY - m_siY)) > 5)
							{
								// 이동하라고 설정해준다.
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
								// 이동하라고 설정해준다.
								SetMove(siNPCX, siNPCY);
							}
						}
					}					
					break;
				}
			}
		}
	}

	// 대사를 처리한다.
	ActionSpeech();

	// 현재 좌표가 이전 좌표와 다르다면 서버에게 현재의 좌표를 알려준다.
	if (IsMyChar() == TRUE)
	{
		// 조금이라도 이동했으며, 아직 이동한다고 메시지를 보내지 않았다면,
		if ((m_siMovedPercent != 0) && (FALSE == m_bIsSendedPlayerPos))
		{
			// 현재 마을이나 포탈을 드러가는 중이 아니면은
			if(((pMyOnlineWorld->pMyData->IsGotoVillage() && (pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsGotoPortal() && (pMyOnlineWorld->pMyData->IsWaitGotoPortalResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsSelectNPC() && (pMyOnlineWorld->pMyData->IsWaitSelectNPCResponse() == TRUE))
				) == FALSE)
			{
				// robypark 2004/12/17 16:52
				// 이동하려는 좌표를 이전에 전송하였던 값을 전송하기 않았다면
				if ((m_siPrevSendedNX != m_siNX) || (m_siPrevSendedNY != m_siNY))
				{
					// 사용자 캐릭터가 이동하려는 좌표를 전송
					pMyOnlineWorld->pOnlineClient->SendPlayerPos(m_siNX, m_siNY, m_bIsSiegeWarfareUnit);
					m_bIsSendedPlayerPos = TRUE;

					m_siPrevSendedNX	=	m_siNX;
					m_siPrevSendedNY	=	m_siNY;

					// 공성전 유닛이라면 이동효과음 출력
					if (m_bIsSiegeWarfareUnit)
					{
						// 이동 효과음이 존재
						if (KI[m_siImKind].m_siMoveEffect)
						{
							// hash table을 이용하여 연주해야 한다.
							// HQ의 charinfo.txt에서 설정한 값 이용
							HashCodePushEffect(KI[m_siImKind].m_siMoveEffect, 0, 0);
						}
					}
				}
			}
		}
		else if (siPrevX != m_siX || siPrevY != m_siY)
		{
			//clGrp.Error("내 위치 메시지 보냄", "[%d] [%d]", m_siX, m_siY);

			// 현재 마을이나 포탈을 드러가는 중이 아니면은
			if(((pMyOnlineWorld->pMyData->IsGotoVillage() && (pMyOnlineWorld->pMyData->IsWaitGotoVillageResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsGotoPortal() && (pMyOnlineWorld->pMyData->IsWaitGotoPortalResponse() == TRUE))
				|| (pMyOnlineWorld->pMyData->IsSelectNPC() && (pMyOnlineWorld->pMyData->IsWaitSelectNPCResponse() == TRUE))
				) == FALSE)
			{
	/*			UI16 DifX = abs(m_siX-siPrevX);
				UI16 DifY = abs(m_siY-siPrevY);

				if(DifX > 5 || DifY > 5)
					clGrp.Error("Move", "이전좌표랑 차이 : DifX(%d), DifY(%d)", DifX, DifY);
	*/
				// 내 캐릭터의 위치는 변경되었으나 위치를 서버에 전송하지 않은 경우
				if (FALSE == m_bIsSendedPlayerPos)
				{
					// 내 캐릭터의 대한 정보를 서버에게 보낸다.
					pMyOnlineWorld->pOnlineClient->SendPlayerPos();
				}
				else
					m_bIsSendedPlayerPos = FALSE;	// 다시 초기화

				// 이전에 있던 타일 좌표가 변경되었다.
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
//	설명		:	캐릭터의 액션.
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
	// 공성전 유닛 변신 애니메이션 처리
	if (0 != m_siSiegeWarfareTransformType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		if (EFFECT_CONSTANT_SIEGEWARFARE_TRANSFORM == m_siSiegeWarfareTransformType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_TRANSFORM);	// 공성전 유닛으로 처음 변하는 경우
		else	// EFFECT_CONSTANT_SIEGEWARFARE_RETRANSFORM == m_siSiegeWarfareTransformType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_RETRANSFORM);	// 공성전 유닛인 상태에서 변신하는 경우

		// 애니메이션이 끝났는지 검사
		m_siEffectTransformFrame++;
		if (m_siEffectTransformFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_siSiegeWarfareTransformType = 0;
			m_siEffectTransformFrame = 0;
		}
	}

	// robypark 2004/12/2 19:10
	// 공성전 유닛 보급품 관련 애니메이션 처리
	if (0 != m_siEffectSupplyGoodsType)		// 공성전 유닛이 보급품 관련 효과. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// 보급품 아이콘 이펙트 리소스 얻기
		if (EFFECT_CONSTANT_SUPPLYGOODS_FILLED == m_siEffectSupplyGoodsType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_FILLED);	// 공성전 유닛이 보급품을 받았다
		else	// EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_SUPPLYGOODS_RANOUT);	// 공성전 유닛의 보급품이 바닥났다.

		// 애니메이션이 끝났는지 검사
		m_siEffectSupplyGoodsFrame++;
		if (m_siEffectSupplyGoodsFrame >= (lpSprEffect->Header.font_number * 3))
		{
			if (EFFECT_CONSTANT_SUPPLYGOODS_RANOUT == m_siEffectSupplyGoodsType)	// 보급품 바닥난 효과의 경우에는 무한 반복 애니메이션된다. 보급 받을 때 까지
				m_siEffectSupplyGoodsFrame = 0;
			else
				m_siEffectSupplyGoodsType = 0;
		}
	}

	// robypark 2004/12/10 14:53
	// 공성전 유닛이 마을에서 받는 반격 관련 애니메이션 처리
	if (0 != m_siEffectVillageConterAttackType)
	{
		// Effect
		XSPR*		lpSprEffect = NULL;
		
		// 공성전 유닛이 마을에서 받는 반격 관련 이미지 리소스 얻기
		if (ON_VILLAGE_ATTACK_OIL == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILlAGE_OIL);		// 공성전 유닛이 마을 반격을 받았다. 기름
		else if (ON_VILLAGE_ATTACK_STONE == m_siEffectVillageConterAttackType)
			lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_VILLAGE_STONE);	// 공성전 유닛이 마을 반격을 받았다. 돌

		// 애니메이션이 끝났는지 검사
		m_siEffectVillageConterAttackFrame++;
		if (m_siEffectVillageConterAttackFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_siEffectVillageConterAttackFrame = 0;
			m_siEffectVillageConterAttackType = 0;
		}
	}

	// robypark 2004/11/24 18:47
	// 공성전 유닛 견고한 방어 이펙트 애니메이션 처리
	if (TRUE == m_bSiegeWarfareEvasion)
	{
		// Effect
		XSPR*		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_EVASION);

		// 애니메이션이 끝났는지 검사
		m_siEffectEvasionFrame++;
		if (m_siEffectEvasionFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_bSiegeWarfareEvasion = FALSE;
			m_siEffectEvasionFrame = 0;
		}
	}

	// robypark 2004/11/24 18:47
	// 공성전 유닛 치명적 공격 이펙트 애니메이션 처리
	if (TRUE == m_bSiegeWarfareDeathblow)
	{
		// Effect
		XSPR*		lpSprEffect = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_SIEGEWARFARE_EFFECT_DEATHBLOW);

		// 애니메이션이 끝났는지 검사
		m_siEffectDeathblowFrame++;
		if (m_siEffectDeathblowFrame >= (lpSprEffect->Header.font_number * 3))
		{
			m_bSiegeWarfareDeathblow = FALSE;
			m_siEffectDeathblowFrame = 0;
		}
	}

	// 위치
	if(pMyOnlineWorld->pOnlineMap->GetCurrentFocusCharID() == uiUniqueID)
	{
//		DWriteStringToFile("위치 : [%d] [%d]", m_siX, m_siY);
	}

	// 맵에서 아이디를 삭제한다.
	DelID();
	//pMyOnlineWorld->pOnlineMap->DelID(m_siX, m_siY, 1, 1, ON_MAP_DELID_CHAR);

	// 우선 오버헤드를 수행한다.	
	OverHead();	

	// 이모티콘
	if(m_Imoticon_Type)
	{
		if((GetTickCount() - m_Imoticon_StartCount) > IMOTICON_WAITTIME)
		{
			m_Imoticon_StartCount = 0;
			m_Imoticon_Type       = 0;
		}
	}

	// 새로 선택된 것에 대한 처리. 
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
	// 공성전 유닛일 경우 이동처리 프레임 스킵하여 딜레이 주기
	m_scCurrentMoveActionSkip++;
	if (m_scCurrentMoveActionSkip >= m_scMaxMoveActionSkip)
	{
		// 이동 중일때는 이동을 처리한다.	
		siMoveResult = Move();

		m_scCurrentMoveActionSkip = 0;
	}

	if(m_siMovedPercent == 0)			bBusySwitch	=	FALSE;	
	else								bBusySwitch	=	TRUE;		

	// 이동 완료되었다면
	if (ON_MOVE_DONE == siMoveResult)
		m_scCurrentMoveActionSkip = 0;	// 이동 딜레이 초기화

	switch(CurrentOrder.siOrder)
	{
	case ON_ORDER_WAIT:
		break;
	case ON_ORDER_MOVE:		
		// 현재 자판인지 검사
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
			// 버퍼에 명령이 없다면 대기 명령을 내린다.
			if(IsOrderInBuffer() == FALSE)			
				AddOrder(ON_ORDER_WAIT, 0, 0);																	
		}				
		break;
	case ON_ORDER_ATTACK:
		{
			SI16	siAttackCharX, siAttackCharY;

			// 우선 공격하려는 캐릭터가 유효한지 알아온다.
			if(pMyOnlineWorld->pIOnlineChar->IsExistUniqueID(CurrentOrder.siOrderParam1) == TRUE)
			{
				// 이동이 완료되었다면 정지 명령을 내려준다.
				if(siMoveResult == ON_MOVE_DONE)
				{
//					SetMessage("ON_MOVE_DONE");

					if(IsOrderInBuffer() == FALSE)			
						AddOrder(ON_ORDER_WAIT, 0, 0);
				}
				else
				{
					// 계속적으로 그 캐릭터를 공격하게 한다.
					pMyOnlineWorld->pIOnlineChar->GetPos(CurrentOrder.siOrderParam1, &siAttackCharX, &siAttackCharY);					
					SetMove(siAttackCharX, siAttackCharY);
				}
			}
			else
			{
				// 캐릭터가 유효하지 않고 
				// 대기 명령이 없다면 정지 시킨다.
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

			// 현재 공격하려는 캐릭터의 위치를 얻어온다.
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
			
			// 오브젝트의 ID를 구한다.
			siObjectID	=	pMyOnlineWorld->pOnlineMap->GetObjectID(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);																					

			// 이동중이 아니라면 이동을 시킨다.						
			// 마을인가 알아온다.
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
					
					// 이동하라고 설정해준다.
					SetMove(CurrentOrder.siOrderParam1, CurrentOrder.siOrderParam2);
				}
			}
		}
		break;

	case ON_ORDER_USEOBJECT:		
	case ON_ORDER_USEOBJECT_VILLAGE_DEFENCE_INTERFACE:
		// 현재 자판인지 검사
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
	
		// 이동을 해준다.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// 버퍼에 명령이 없다면 대기 명령을 내린다.
			if(IsOrderInBuffer() == FALSE)				
				AddOrder(ON_ORDER_WAIT, 0, 0);							
		}			
		break;
	case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGE:
		// 이동을 해준다.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// 버퍼에 명령이 없다면 대기 명령을 내린다.
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

			// 현재 그 위치에 아이템이 없다면 이동을 중지시킨다.
			uiItemID	=	pMyOnlineWorld->pOnlineMap->GetItemID(siItemX, siItemY);

			// 그 자리에 아이템이 없거나 이동이 중지되면 선다.
			if((siMoveResult == ON_MOVE_DONE) || (uiItemID == 0))
			{	
				// 버퍼에 명령이 없다면 대기 명령을 내린다.
				if(IsOrderInBuffer() == FALSE)				
					AddOrder(ON_ORDER_WAIT, 0, 0);							
			}					
		}
		break;
	// robypark 2005/1/25 19:8
	// 공성전 유닛 보급 수레가 다른 유닛 치료하기
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL:
		{
			SI16	siTargetCharX, siTargetCharY;

			// 현재 공격하려는 캐릭터의 위치를 얻어온다.
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
		// 이동을 해준다.
		if(siMoveResult == ON_MOVE_DONE)
		{	
			// 버퍼에 명령이 없다면 대기 명령을 내린다.
			if(IsOrderInBuffer() == FALSE)
				AddOrder(ON_ORDER_WAIT, 0, 0);
		}
		break;
	}	
		
	// 애니메이션을 시킨다.
	UpdateAnimation(FALSE, &fEndFrame, &fSound);
	switch(CurrentOrder.siOrder)
	{
	case ON_ORDER_SHORTDISTANCE_ATTACKVILLAGEACTION:
	case ON_ORDER_LONGDISTANCE_ATTACKVILLAGE_ACTION:
	case ON_ORDER_FIELDATTACK_ACTION:
	case ON_ORDER_REQUEST_SUPPLYWAGON_ORDER_HEAL_ACTION:
		// 애니메이션이 끝나면 대기로 수정
		if(fEndFrame)
		{
			AddOrder(ON_ORDER_WAIT, 0, 0);

			// 이 캐릭터가 자신의 캐릭터 이면
			if(uiUniqueID == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
			{
				pMyOnlineWorld->pOnlineCharUI->SetAction(TRUE);
				pMyOnlineWorld->m_VillageAttackDelay = 10;
			}
			m_bFieldAttackAni = FALSE;
		}

		if(fSound)
		{
			// 사운드 출력
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
	
	// 맵에서 id를 설정한다.
	if(pMyOnlineWorld->pOnlineMap->GetCharID(m_siX, m_siY) == 0)
		pMyOnlineWorld->pOnlineMap->SetID(m_siX, m_siY, 1, 1, siID, 0, 0, 0);
}


//----------------------------------------------------------------------------------------------------------------
//	설명			:	이동해야 할 지점을 정해준다.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetMove(SI16 siDX, SI16 siDY)
{
	m_siDX	=	siDX;
	m_siDY	=	siDY;
}

//----------------------------------------------------------------------------------------------------------------
//	설명			:	타일 좌표를 얻어온다.	
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siX;
	*psiY	=	m_siY;
}

//----------------------------------------------------------------------------------------------------------------
//	설명			:	타일의 변위 좌표를 얻어온다.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetRPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siRX;
	*psiY	=	m_siRY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명			:	현재 이동중인 목표 위치를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetDPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siDX;
	*psiY	=	m_siDY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명			:	현재 이동중인 다음 위치를 얻어온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::GetNPos(SI16 *psiX, SI16 *psiY)
{
	*psiX	=	m_siNX;
	*psiY	=	m_siNY;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명			:	현재 이동중인 다음 위치의 비율을 얻어온다.	
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
//	설명	:	애니메이션을 업데이트 한다.(현재 명령과 현재 방향에 의존한다.)
//----------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::UpdateAnimation(BOOL bForce, BOOL* fEndFrame, BOOL* fSound)
{
	// 피난민 캐릭터로 변신한 경우
	if (m_bIsRefugee)
		return UpdateAnimationForRefugee(bForce, fEndFrame, fSound);

	SI32	file;
	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[m_siImKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// 필요한 이미지를 로딩한다. 
				LoadImgFile(file);
			}
		}
	}

	// robypark 2005/1/27 11:49
	// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	if( -1 != m_siAnimationResourceID_Blue )
	{
		if( Return_Image(m_siAnimationResourceID_Blue) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
		{
			// 필요한 이미지를 로딩한다. 
			LoadImgFile(m_siAnimationResourceID_Blue);
		}
	}

	// robypark 2005/1/27 11:49
	// 해당 공성전 유닛 이미지 파일. 아군 유닛일 때
	if( -1 != m_siAnimationResourceID_Red )
	{
		if( Return_Image(m_siAnimationResourceID_Red) == NULL )		// GameSpr[]에 해당파일이 이미 로딩되어있나 검사
		{
			// 필요한 이미지를 로딩한다. 
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

		// 애니메이션 현재 단계 설정
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
			//	버그 발생 부분중에 하나다. 나머지 결산값이 틀릴수도 있으므로 일단 무시무시한(?) 값이 떨어지면 기준값을 1로 잡아준다.
			//	다음 버그 리포팅에 이 부분에서 버그가 안나오거나 조치에 따른 특별한 문제가 없을 경우 이 주석을 제거해도 좋다.
			SI16	siRetData	= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siTotalFrame[m_siHeroAniAction][m_siHeroBaseWClass][m_siHeroWeaponEnum];
			if( siRetData <= 0 )	siRetData = 1;
			m_siAniStep %= siRetData;
		}
		else
			m_siAniDelayer++;


		//-----------------------------
		// 그림 사이즈를 구한다. (m_siXSize, m_siYSize)
		BOOL bExist;

		SI32 siCount, siFileID;
		SI32 siBodyPart, siTotalPiece;

		siTotalPiece	= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siTotalPiece[m_siHeroAniAction][m_siHeroBaseWClass];
		bExist = FALSE;
		for( siCount=0; siCount < siTotalPiece; siCount++ )
		{
			// 찍을 Bodypart를 얻어온다.
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
			// 마법 시전 동작 애니메이션이 있는 경우
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

		// 그림 사이즈를 구한다. (m_siXSize, m_siYSize)
		m_siXSize	= GetFileXsize(m_siImFile);
		m_siYSize	= GetFileYsize(m_siImFile);
	}



	if((bChangeAction == FALSE) && (bChangeDirection == FALSE))
	{
		// 캐릭터의 액션과 방향이 변경되지 않았으므로 업데이트 할 필요가 없다.
		return FALSE;
	}

	if(bChangeAction == TRUE)
	{
		// 액션을 업데이트한다.
		siAniActionCode				=	CurrentActionCode;

		// 액션의 포인터를 새로이 얻어온다.		
		//pCharActionInfo				=	pMyOnlineWorld->pOnlineCharAni->GetCharActionInfo(pCharAniInfo, siAniActionCode);

		// 화일은 액션이 변경되었을 시에만 얻어온다.		
		//m_siFile					=	pCharActionInfo->siFile;		

		m_siAniStep = 0;

	}

	if(bChangeDirection == TRUE)
	{
		// 방향을 업데이트한다.
		siAniDirection			=	m_siDirection;	

		// 방향의 포인터를 새로이 얻어온다.		
		//pCharActionDirectionInfo	=	pMyOnlineWorld->pOnlineCharAni->GetCharDirectionInfo(pCharActionInfo, siAniDirection);		

		//if(pCharActionDirectionInfo == NULL)
		//{
			// 방향 포인터 얻는데 실패

		//	clGrp.Error("", "방향 포인터 얻는데 실패 : [%s], [%d] [%d]", szCharName, siAniActionCode, siAniDirection);
		//}
	}	

	// 폰트 배열을 얻어온다.	
	//siFrameNum					=	pCharActionDirectionInfo->siFrameNum;	
	//psiFrames					=	pCharActionDirectionInfo->siFrame;	
			
	// Reverse 해서 출력할건지의 여부를 설정한다.
	//if(pCharActionDirectionInfo->siParameter1 & ON_CHAR_ANI_REVERSE)	m_bReverseSwitch	=	TRUE;
	//else																m_bReverseSwitch	=	FALSE;

	
	//-----------------------
	// 조합형 캐릭터일 경우
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

		// 한 방향당 총 폰트수를 얻어온다.
		siFontCount		= pMyOnlineWorld->pCharFileLoadManager->m_HeroConfig[KI[m_siImKind].m_ssHeroID].siFontCount[m_siHeroAniAction][m_siHeroBaseWClass];


		// 그 방향의 첫 폰트 번호
		m_siFont = siFontCount * m_siDir;
	}

	//
	//--------------

	return TRUE;
}

// 피난민 캐릭터를 위한 애니메이션을 업데이트 한다.(현재 명령과 현재 방향에 의존한다.)	
BOOL	cltOnlineChar::UpdateAnimationForRefugee(BOOL bForce, BOOL* fEndFrame, BOOL* fSound)
{
	SI32	file;
	// 캐릭터에 필요한 파일이 없다면 불러온다.
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[m_siRefugeeKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// 필요한 이미지를 로딩한다. 
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

	// 그림 사이즈를 구한다. (m_siXSize, m_siYSize)
	m_siXSize	= GetFileXsize(m_siImFile);
	m_siYSize	= GetFileYsize(m_siImFile);


	if((bChangeAction == FALSE) && (bChangeDirection == FALSE))
	{
		// 캐릭터의 액션과 방향이 변경되지 않았으므로 업데이트 할 필요가 없다.
		return FALSE;
	}

	if(bChangeAction == TRUE)
	{
		// 액션을 업데이트한다.
		siAniActionCode				=	CurrentActionCode;

		m_siAniStep = 0;

	}

	if(bChangeDirection == TRUE)
	{
		// 방향을 업데이트한다.
		siAniDirection			=	m_siDirection;	
	}	

	return TRUE;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	방향을 바꾼다.
//----------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::ChangeDirection(SI32 siDirection)
{
	m_siDirection	=	siDirection;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 ID(문자열)을 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetID(CHAR *pszID)
{
	strncpy(szCharName, pszID, ON_ID_LENGTH);
}

/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터의 UniqueID를 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
UI16	cltOnlineChar::GetUniqueID()
{
	return uiUniqueID;
}
*/
/*
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	초기화가 되었는지 알아온다.
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
//	설명	:	현재 이동중인 방향을 설정한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetMovingDirection(SI32 siDirection)
{
	siMovingDirection	=	siDirection; 
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	현재 이동중인 방향을 구한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
SI32	cltOnlineChar::GetMovingDirection()
{		
	return  siMovingDirection;	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	서버에게 이동중인 방향을 보내야 하는때인지를 알아온다.	
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsSendMovingDirection()
{
	if(siMovingDirection != 0)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 이동 Flag
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::SetWalkingFlag(UI16 Flag)
{
	MoveFlag = Flag;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	사용자의 캐릭터인가?
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::IsMyChar()
{
	if(pMyOnlineWorld->pMyData->GetMyCharUniqueID() == uiUniqueID)
		return TRUE;
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	캐릭터를 그 방향을 한 타일 이동 시킨다.
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

	// 이동하도록 명령을 준다. 
	AddOrder(ON_ORDER_MOVE, siMoveX, siMoveY);	
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	대사를 초기화 한다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::InitSpeech()
{
	ZeroMemory(szSpeech, sizeof(szSpeech));
	siSpeechSettingFrame	=	0;
	siSpeechSettingfNotDel	=	0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	이번에 말할 대사를 넣어준다.	
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
//	설명	:	대사가 떠있는지 검사
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	cltOnlineChar::CheckSpeech()
{
	return IsSpeech();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	대사 처리,
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
VOID	cltOnlineChar::ActionSpeech()
{
	if(IsSpeech() == TRUE)
	{
		if(siSpeechSettingfNotDel == FALSE)
		{
			if(pMyOnlineWorld->GetTotalFrame() - siSpeechSettingFrame > 350)
			{
				// 대사를 초기화 한다.
				InitSpeech();
			}
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 명령을 실지로 수행하기전에 적절한지 미리 살펴본다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::PreProcessorOrder(OnOrder *pOrder)
{
	SI16	 siMX, siMY;	

	switch(pOrder->siOrder)
	{
	case ON_ORDER_MOVE:
		// 현재 이동 상태이고 목적지가 지금의 목적지와 같다면 명령을 수행하지 않는다.
		// 또한 목적지와 지금 위치라도 수행하지 않는다.
		if(pOrder->siOrderParam1 == m_siX && pOrder->siOrderParam2 == m_siY)
			return FALSE;
		
		if(memcmp(pOrder, &pOrder->siOrderParam1, sizeof(OnOrder)) == 0)
			return FALSE;
		
		pMyOnlineWorld->pclFindPath->FindPathL(m_siX, m_siY, pOrder->siOrderParam1, pOrder->siOrderParam2, &siMX, &siMY, NULL, 160, 8, 1, m_siMoveableTileInfo);		

		if(m_siX == siMX && m_siY == siMY)
			return FALSE;
		break;
	case ON_ORDER_ATTACK:
		// 현재 공격 상태이고 지금 공격하는 캐릭터가 공격하려는 캐릭터와 같으면 명령을 수행하지 않는다.
		if(memcmp(pOrder, &pOrder->siOrderParam1, sizeof(OnOrder)) == 0)
			return FALSE;

		// 그리고 지금 공격하려는 캐릭터가 유효한지 알아온다.
		if(pMyOnlineWorld->pIOnlineChar->IsExistUniqueID(pOrder->siOrderParam1) == FALSE)
			return FALSE;
		break;	
	}

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용자의 이름을 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CHAR*	cltOnlineChar::GetCharName()
{
	return szCharName;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	사용자의 IP를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
DWORD	cltOnlineChar::GetIP()
{
	return dwIP;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	스크린상의 RealRect를 구한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::GetScreenRealRect(LPRECT pRect)
{
	SI32	siScreenX, siScreenY;

	// 현재 캐릭터가 있는 타일의 위치(스크린 좌표)를 구한다.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 마우스가 가리키는 캐릭터는지 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::IsPointToChar(LPPOINT pptMouse, SI32 *psiDistance)
{
	RECT	rcImage;	
	SI32	siCharCX, siCharCY;
	BOOL	bGetScreenRealRect;
	BOOL	bRet = FALSE;

	// 현재 캐릭터가 있는 타일의 위치(스크린 좌표)를 구한다.
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 해야하는 상황인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::IsWarp(SI16 siWarpX, SI16 siWarpY,SI16 siDistance)
{
	// ON_CHAR_WAPR_DISTANCE 만큼 차이가 나면 워프를 시도한다.
	if(abs(siWarpX - m_siX) >= siDistance || abs(siWarpY - m_siY) >= siDistance)
		return TRUE;
	else
		return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	워프를 한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineChar::Warp(SI16 siWarpX, SI16 siWarpY)
{
	POINT	ptPos;

//	// actdoll (2004/03/16 19:05) : 여기에 문제가 발생할 수 있는 오류를 넣는다.
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

	// 이동하려는 타일에 갈 수 있는지 알아본다.
	if( !pMyOnlineWorld || !pMyOnlineWorld->pOnlineMap )							return;
	if(pMyOnlineWorld->pOnlineMap->IsColl(siWarpX, siWarpY, m_siMoveableTileInfo) == FALSE)
	{
		// 우선 맵에서 자기의 ID를 삭제한다.
		DelID();

		// 자기가 설정한 예약된 자리도 없앤다.
		if(m_siX != m_siMX || m_siY != m_siMY)			
		{
			// 현재 예약했던 자리를 없앤다.			
			pMyOnlineWorld->pOnlineMap->SetTileAtb(m_siMX, m_siMY, 1, 1, ON_TILE_ATB_CHAR_RESERVE, FALSE);
		}
		
		m_siX	=	siWarpX;
		m_siY	=	siWarpY;

		// 맵에 ID를 다시 설정한다.	
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 수행중인 명령을 리턴한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
OnOrder	cltOnlineChar::GetCurrentOrder()
{
	return CurrentOrder;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	현재 캐릭터가 이동중인가?
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::IsMoving()
{
	return bMovingSwitch;	
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Action Code를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
		// 마법 시전이라면
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 클래스를 얻어온다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI16	cltOnlineChar::GetClass()
{
	return siClass;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	BaseX, BaseY를 기준으로 ID를 심을 새로운 좌표를 찾는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
POINT	cltOnlineChar::SetID(SI32 siBaseX, SI32 siBaseY)
{
	POINT	ptPos;

	// 캐릭터가 심을 수 있는 위치를 찾는다.	
	// 캐릭터가 이동 가능한 타일의 종류를 넣어준다.
	if(ISSHIP(uiUniqueID) == TRUE)
		ptPos	=	pMyOnlineWorld->pOnlineMap->SearchMap(siBaseX, siBaseY, ON_MAP_SI_CHAR, ON_TILE_KIND_SEA);
	else
	{
		ptPos	=	pMyOnlineWorld->pOnlineMap->SearchMap(siBaseX, siBaseY, ON_MAP_SI_CHAR, ON_TILE_KIND_GROUND);
		DWriteStringToFile("[%s] [%d] [%d] : [%d] [%d]", szCharName, siBaseX, siBaseY, ptPos.x, ptPos.y);
	}


	if(ptPos.x != -1 && ptPos.y != -1)
	{
		// 캐릭터를 심을 위치를 찾았다.
		// 캐릭터를 심는다.				
		pMyOnlineWorld->pOnlineMap->SetID(SI16(ptPos.x), SI16(ptPos.y), 1, 1, siID, 0, 0, 0);	
	}

	return ptPos;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	맵에서 자신을 삭제한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineChar::DelID()
{
	// 맵에서 얻어온 좌표가 자기 일때만 삭제한다.
	if(pMyOnlineWorld->pOnlineMap->GetCharID(m_siX, m_siY) == siID)
	{
		// 맵에서 삭제해도 괜찮다.
		pMyOnlineWorld->pOnlineMap->DelID(m_siX, m_siY, 1, 1, ON_MAP_DELID_CHAR);		
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	플레이어의 정보를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	
	// 이미 캐릭터가 있다면 그 캐릭터를 이동 시킨다.			
	AddOrder(ON_ORDER_MOVE, lpBaseInfo->siX, lpBaseInfo->siY);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	플레이어의 정보를 설정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Description	:	캐릭터에게 아이템을 장착시킨다.
// Parameter	:	bEquip			- FALSE이면 장착해제
//					siEquipKind		- 장착부위
//					siEquipID		- 아이템 아이디, -1일 경우 디폴트 아이템 착용
//					bEffectSound	- 장착시 효과음을 낼 것인가 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::SetEquip(BOOL bEquip, SI32 siEquipKind, SI32 siEquipID, BOOL bEffectSound)
{
	BOOL bSet = FALSE;

	if( KI[m_siImKind].IsKindAtb(KINDATB_HERO) == FALSE ) return FALSE;

	// robypark 2005/1/17 13:4
	// 피난민 캐릭터일 경우 장비 처리 안함.
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
		// 장착 해제 명령이거나, 아이템 아이디가 -1일 경우 기본 무기(맨손)로 설정 
		if( bEquip == FALSE	|| siEquipID == -1 )
		{
			siEquipID = KI[m_siImKind].m_ssDefaultWeapon;
			bSet = TRUE;
		}
		// 그림이 다 있지 않으므로.
		else
		{
			bSet = TRUE; 	
		}

		m_siWeaponDrawID		= siEquipID;

		// 평화모드이면서 자신의 아이디일 경우 기본 무기(맨손)로 설정
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
	
	// m_siHeroBaseWClass 설정
	m_siHeroBaseWClass	= pMyOnlineWorld->pCharFileLoadManager->GetHeroClassCode(&pMyOnlineWorld->pCharFileLoadManager->m_HeroCurrEquip[siID]);	

	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 전투력을 얻는다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltOnlineChar::GetFightPower(void)
{
	return siFightPower;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	캐릭터의 전투력을 지정한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::SetFightPower(SI32 FightPower)
{
	siFightPower = FightPower;
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Set Battle Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get Battle Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	cltOnlineChar::GetBattleFlag(void)
{
	return m_BattleFlag;
}
 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Imoticon 출력
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineChar::SetImoticon(UI16 uiImoticon)
{
	m_Imoticon_StartCount = GetTickCount();
	m_Imoticon_Type       = uiImoticon + 1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Set Find Target Flag
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
VOID	cltOnlineChar::SetFindTargetFlag(UI08 Flag)
{
	m_FindTargetFlag = Flag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 Data 지정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 Name 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
char*	cltOnlineChar::GetGuildName(void)
{
	if(m_szGuild[0]) return (char*)m_szGuild;

	return NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 직급 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltOnlineChar::GetGuildInGrade(void)
{
	return m_siGuildInGrade;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	길드 깃발 얻기
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
SI32	cltOnlineChar::GetGuildFlag(void)
{
	return m_siGuildFlag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	적대 길드인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::SetIsHostilityGuild(UI08 uiIsHostilityGuild)
{
	m_uiIsHostilityGuild = uiIsHostilityGuild;
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	적대 길드인지 검사
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI08	cltOnlineChar::GetIsHostilityGuild(void)
{
	return m_uiIsHostilityGuild;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 설정 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 얻어옴.  
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::GetNewSelectedFlag()
{
	return NewSelectedFlag;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 얻어옴.  
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::Show(BOOL fShow)
{
	m_fShow = fShow;
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	새로 선택된 캐릭터여부를 얻어옴.  
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL	cltOnlineChar::GetShow(void)
{
	return m_fShow;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	Get IP
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
UI32	cltOnlineChar::GetIP(UI16 uiUniqueID)
{
	return dwIP;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	설명	:	개인상점 개설 여부 지정
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
	// 이전에 없었다면, 새로 초기화
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

	// Player와 1칸 이상 떨어져 있으면 주인공 위치로 이동
	if((abs(m_siX - m_VMercenary_TargetX) > 1) || (abs(m_siY - m_VMercenary_TargetY) > 1))
	{
		SI16			siTileScore[9];
		SI32			TempX;
		SI32			TempY;
		SI32			TempXCount;
		SI32			TempYCount;

		// 주인공 주변 1칸 위치를 선정, 각 지점마다 점수를 준다.
		ZeroMemory(siTileScore, sizeof(siTileScore));

		// 현재 주인공 위치에 -100
		siTileScore[(1 * 3) + 1] -= 100;

		// 주인공이 이전에 있었던 위치는 점수를 1 더함
		if((abs(siPrevX1 - m_siX) <= 1) && (abs(siPrevY1 - m_siY) <= 1))
		{
			siTileScore[((siPrevY1 - m_siY + 1) * 3) + (siPrevX1 - m_siX + 1)] += 1;
		}

		// 현재 용병위치와 가장 가까운곳에 2 더함
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
					case 0:		// 이동 불가능 지역이면, 선정 불능
						siTileScore[(TempYCount * 3) + TempXCount] -= 100;
						break;

					case 1:		// 다른 User가 있으면, 그 지점에 점수를 3씩 깍음
						siTileScore[(TempYCount * 3) + TempXCount] -= 3;
						siTileScore[(TempYCount * 3) + TempXCount] -= pMyOnlineWorld->pOnlineMap->VMercenaryGetCount(TempX, TempY);
						break;

					case 2:		// 그 지점에 용병이 있으면, 점수를 깍음.
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

	// 이동중이면, 이동 처리
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
			// 방향 지정
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

	// 현재 위치에서 이동해야하는 상황이면
	if((m_VMercenary_NowX == m_VMercenary_NextX) && (m_VMercenary_NowY == m_VMercenary_NextY))
	{
		if((m_VMercenary_NowX != m_VMercenary_TargetX) || (m_VMercenary_NowY != m_VMercenary_TargetY))
		{
			// 만약 10 이상의 거리가 차이나면 Warp
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

	// 캐릭터에 필요한 파일이 없다면 불러온다.
	siTempImKind = TransKindOnlineToImjin(m_MercenaryKind);
	for(SI32 i = 0; i < MAX_ANIMATION_PER_UNIT; i++)
	{
		file = KI[siTempImKind].m_clCharAnimation[i].siFile;
		if(file)
		{
//			ImgFileListCount(file);
			if(Return_Image(file) == NULL)
			{
				// 필요한 이미지를 로딩한다. 
				LoadImgFile(file);
			}
		}
	}

	// 현재 이동중인지 대기중인지 검사
	if((m_VMercenary_NowX == m_VMercenary_NextX) && (m_VMercenary_NowY == m_VMercenary_NextY))
	{
		switch( siAniActionCode )
		{
			case ON_CHAR_ACTION_CODE_ATTACK:
			case ON_CHAR_ACTION_CODE_SPELL_MAGIC:
				// 방향 지정
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
		// 이동중
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

	// 공성전 유닛은 단군의 거울 효과에 의한 용병을 그리지 않는다.
	if (TRUE == m_bIsSiegeWarfareUnit)
	{
		return;
	}

	// 수성인터페이스에 있는 경우에도 단군의 거울 효과에 의한 용병을 그리지 않는다.
	if (TRUE == m_bIsInVillageDefenceInterface)
	{
		return;
	}

	// 피난민은 단군의 거울 효과를 그리지 않는다.
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
	
	// 화면에 출력할 위치를 계산한다.
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

	// 실제로 Character 화면에 찍혀질 시작 위치를 다시 계산한다.
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
	// 현재 남자인지 여자인지 검사
	m_ChangeEffectSex = 1;			// <-- 기본은 남자로

	// 주인공 관련 데이터 관리자 얻기
	CFieldHeroInfo *pFieldHeroInfo = pMyOnlineWorld->GetFieldHeroInfoMgr()->GetFieldHeroInfo(GetKind());

	if (pFieldHeroInfo)
	{
		// 여자 캐릭터인가?
		if (pFieldHeroInfo->IsHeroFemale())
			m_ChangeEffectSex = 2;
	}

	m_ChangeEffectFrame = 0;
}

BOOL cltOnlineChar::ManContactvillage(int vx, int vy, int wi, int hi, int cx, int cy, int dis)
{
	// 마을을 포함하고 좌, 우, 상, 하로 dis만큼 떨어져 있는 상자를 생각하자.
	if( (cx>=vx-dis && cx<=vx+wi+dis-1) && (cy>=vy-dis && cy<=vy+hi+dis-1) ) return TRUE;
	else return FALSE;
}

void cltOnlineChar::SetSiegeWarfare_Hp(SI16 siFieldHp)
{
	m_siFieldHp = siFieldHp;

	// 최대 값보다 현재 값이 큰 경우
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

// 시작 /////////////////////////////////////////
// robypark 2004/10/27 15:13
// 공성전 유닛 최대 체력치 설정
void cltOnlineChar::SetSiegeWarfare_MaximumHP(SI16 siMaximumHP)
{
	m_siFieldHp_Maximum = siMaximumHP;
}

// 공성전 유닛 최대 체력치 얻기
SI16 cltOnlineChar::GetSiegeWarfare_MaximumHP(void)
{
	return m_siFieldHp_Maximum;
}

// 공성전 유닛 보급품 현재량 설정
void cltOnlineChar::SetSiegeWarfare_CurrentSupplyGoodsAmount(SI32 siCurrentSupplyGoodsAmount)
{
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = siCurrentSupplyGoodsAmount;
//	m_siSupplyCharge = m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;

	/*
	if (m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount <= 0)
	{
		m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = 0;

		// 보급 수레(메딕) 일 경우에만
		if (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// 보급품 바닥난 효과 사용
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

// 공성전 유닛 보급품 현재량 얻기
SI32 cltOnlineChar::GetSiegeWarfare_CurrentSupplyGoodsAmount(void)
{
	return m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount;
}

// 공성전 유닛 보급품 최대량 설정
void cltOnlineChar::SetSiegeWarfare_SupplyGoodsAmount(SI32 siSupplyGoodsAmount)
{
	m_siSiegeWarfareUnit_SupplyGoodsAmount = siSupplyGoodsAmount;
}

// 공성전 유닛 보급품 최대량 얻기
SI32 cltOnlineChar::GetSiegeWarfare_SupplyGoodsAmount(void)
{
	return m_siSiegeWarfareUnit_SupplyGoodsAmount;
}

// 공성전 유닛의 공격 지연 시간 설정
void cltOnlineChar::SetSiegeWarfareUnit_LastAttackDelay(DWORD dwLastAttackDelay)
{
	m_dwSiegeWarfareUnit_LastAttackDelay = dwLastAttackDelay;
}

// 공성전 유닛의 경과된 공격 지연 시간 설정
void cltOnlineChar::SetSiegeWarfareUnit_PastAttackDelay(DWORD dwPastAttackDelay)
{
	if (m_dwSiegeWarfareUnit_LastAttackDelay > dwPastAttackDelay)
		m_dwSiegeWarfareUnit_LastAttackDelay -= dwPastAttackDelay;
	else
		m_dwSiegeWarfareUnit_LastAttackDelay = 0;
}

// 공성전 유닛의 공격 지연 시간 얻기
DWORD cltOnlineChar::GetSiegeWarfareUnit_LastAttackDelay(void)
{
	return m_dwSiegeWarfareUnit_LastAttackDelay;
}
// 끝 /////////////////////////////////////////

// robypark 2004/11/2 17:55
// 공성전 유닛으로 변신 효과 사용
void cltOnlineChar::SetSiegeWarfare_TransformEffect(SI32 siSiegeWarfareTransformType)
{
	m_siSiegeWarfareTransformType = siSiegeWarfareTransformType;
	m_siEffectTransformFrame = 0;
}

// robypark 2004/11/2 18:4
// 공성전 유닛 보급품 자동 소모(1%)
void cltOnlineChar::WasteSiegeWarfareSupplyGoods(void)
{
	m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount -= (m_siSiegeWarfareUnit_SupplyGoodsAmount * 1 / 100);

	if (0 > m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount)
	{
		m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount = 0;

		// robypark 2005/1/6 13:20
		// 보급품이 없으니 보급수레에서 보급받아라....
		if (GetUniqueID() == pMyOnlineWorld->pMyData->GetMyCharUniqueID())
		{
			if (FALSE == IsSpeech())	// 말하는 중이 아니라면
				SetSpeech(pMyOnlineWorld->pOnlineText->Get(ON_TEXT_SPEECH_RANOUT_SUPPLYGOODS), FALSE, NULL);
		}

		// 보급품 바닥난 효과 사용
		if (EFFECT_CONSTANT_SUPPLYGOODS_RANOUT != m_siEffectSupplyGoodsType)
		{
			SetSiegeWarfare_EffectSupplyGoods(EFFECT_CONSTANT_SUPPLYGOODS_RANOUT);
		}
	}
}

// robypark 2004/12/10 11:41
// 공성전 유닛이 마을로 부터 공격받는 효과 설정
void	cltOnlineChar::SetSiegeWarfare_VillageCountAttack(SI32 siEffectVillageConterAttackType)
{
	m_siEffectVillageConterAttackType	=	siEffectVillageConterAttackType;
	m_siEffectVillageConterAttackFrame	=	0;
}

// robypark 2004/12/2 19:2
// 공성전 유닛이 보급품 관련 효과 설정. 보급품을 받았거나(1) 보급품이 바닥난 경우(2)
void	cltOnlineChar::SetSiegeWarfare_EffectSupplyGoods(SI32 siEffectSupplyGoodsType)
{
	m_siEffectSupplyGoodsType	= siEffectSupplyGoodsType;
	m_siEffectSupplyGoodsFrame	= 0;
}

// robypark 2004/11/24 18:31
// 공성전 견고한 방어 효과 사용
void	cltOnlineChar::SetSiegeWarfare_Evasion(void)
{
	m_bSiegeWarfareEvasion = TRUE;
	m_siEffectEvasionFrame = 0;
}

// robypark 2004/11/24 18:31
// 공성전 치명적 공격 효과 사용
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
// 공성전 유닛의  체력 및 보급품 수치(%) 그리기
void	cltOnlineChar::DrawSiegeWarfareUnit_StatusBar(void)
{
	// 배경 이미지 얻기
	XSPR	*psprBackGround = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_BACKGROUND);

	// 사용자일 경우
	if (pMyOnlineWorld->pMyData->GetMyCharUniqueID() == GetUniqueID())
	{
		// 공성전 보급 수레일 경우에만 보급품 게이지를 그린다.
		if (KI[m_siImKind].IsAttribute1(ATB_STORE_AND_SUPPLY_SUPPLYGOODS))
		{
			// 배경 이미지 출력
			if (psprBackGround)
			{
				clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
									psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
									psprBackGround->Image);
			}

			// 공성전 유닛 체력 게이지 출력
			XSPR	*psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);

			if (psprGaugeHealth)
			{
				// 그려져야할 퍼센트 계산
				SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

				// 그리기
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
			}

			// 공성전 유닛 보급품 게이지 출력
			XSPR	*psprGaugeSupplyGoodsAmount = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_SUPPLYGOODS_AMOUNT);

			if (psprGaugeSupplyGoodsAmount)
			{
				// 그려져야할 퍼센트 계산
				SI32	siPct = (m_siSiegeWarfareUnit_CurrentSupplyGoodsAmount * 10) / (float)m_siSiegeWarfareUnit_SupplyGoodsAmount;

				// 그리기
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1 + 3 + 1, siPct + 1, psprGaugeSupplyGoodsAmount);
			}
		}
		else	// 체력 게이지만 그린다.
		{
			// 배경 이미지 출력
			if (psprBackGround)
			{
				clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
									psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
									0, 0,
									psprBackGround->Header.Xsize, 5,
									psprBackGround->Image);
			}

			// 공성전 유닛 체력 게이지만 출력
			XSPR	*psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);

			if (psprGaugeHealth)
			{
				// 그려져야할 퍼센트 계산
				SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

				// 그리기
				DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
			}
		}
	}
	else	// 사용자외의 유닛은 체력 게이지만 출력
	{
		// 배경 이미지 출력
		if (psprBackGround)
		{
			clGrp.PutSpriteJinC(m_siPositionGaugeX, m_siPositionGaugeY,
								psprBackGround->Header.Xsize, psprBackGround->Header.Ysize,
								0, 0,
								psprBackGround->Header.Xsize, 5,
								psprBackGround->Image);
		}

		// 공성전 유닛 체력 게이지만 출력
		XSPR	*psprGaugeHealth = NULL;

		// 사용자와 같은 상단인가?
		if (pMyOnlineWorld->IsMemberOfSameGuild(GetUniqueID()))
		{
			psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH);
		}
		// 적대 상단원인가?
		else if (GetIsHostilityGuild())
		{
			psprGaugeHealth = pMyOnlineWorld->pOnlineResource->GetxsprGame(ON_GAMESPR_GAUGE_UNIT_HEALTH_ENEMY);
		}
		else	// 그 밖의 경우 안그린다.
			psprGaugeHealth = NULL;

		if (psprGaugeHealth)
		{
			// 그려져야할 퍼센트 계산
			SI32	siPct = (m_siFieldHp * 10) / (float)m_siFieldHp_Maximum;

			// 그리기
			DrawGaugeCharacter(m_siPositionGaugeX + 1, m_siPositionGaugeY + 1, siPct + 1, psprGaugeHealth);
		}		
	}
}

// robypark 2004/12/16 14:33
// 공성전 유닛인가?
BOOL	cltOnlineChar::IsSiegeWarfareUnit(void)
{
	return m_bIsSiegeWarfareUnit;
}

// robypark 2004/12/17 17:20
// 사용자가 현재 수성 인터페이스 상에 있는가?
BOOL	cltOnlineChar::IsInVillageDefenceInterface(void)
{
	return m_bIsInVillageDefenceInterface;
}

// robypark 2004/12/17 17:20
// 캐릭터가 현재 수성 인터페이스 상에 있는가? 설정
void	cltOnlineChar::SetInVillageDefenceInterface(BOOL bIsInVillageDefenceInterface)
{
	m_bIsInVillageDefenceInterface = bIsInVillageDefenceInterface;
}

// robypark 2005/1/14 18:39
// 캐릭터를 그릴 때 그려지지는 중심 위치 얻기
void	cltOnlineChar::GetCharacterDrawCenterPos(SI16 &siCenterX, SI16 &siCenterY)
{
	siCenterX = m_siDrawCenterX;
	siCenterY = m_siDrawCenterY;
}

// robypark 2005/2/17 21:44
// 마우스 커서 아래 캐릭터인지를 설정한다.
void	cltOnlineChar::SetCursorHoverCharacter(BOOL bHover)
{
	m_bCursorHoverCharacter	= bHover;
}
