//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------

#include <GSL.h>

#include <main.h>

#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <etc.h>
#include "..\Gersang\calcattack.h"

#include <myfile.h>
#include "..\Gersang\DirectX.h"

#include <smoke.h>
#include <mapinterface.h>
#include <charinterface.h>
#include <CharAnimation.h>
#include <CharOrg.h>
#include "music.h"

#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

#include "bindJXFile.h"		// using HQ file

// robypark 2004/9/30 18:14
// 온라인공용 파서 사용
#include "../SCCommon/OnlineCommonParser.h"

/*
// 연기의 애니메이션 속도의 상수.
#define	SMOKE_DELAY_1	1
#define SMOKE_DELAY_2	2
#define SMOKE_DELAY_3	3
#define	SMOKE_DELAY_4	4
#define	SMOKE_DELAY_5	5
#define	SMOKE_DELAY_10	10
*/

extern	_MainData			GD;
extern	cltCharInterface	clifChar;         // 캐릭터 관련 인터페이스 
extern	_NewID				ZeroID;
extern	cltMapInterface		clifMap;          // 맵     관련 인터페이스 
extern	unsigned char		TeamDarkTable[][256];
extern	_Char				CR[];

_Smoke  g_Smoke[MAX_SMOKE_NUMBER];   // 연기용 데이터 

/*
#define SMOKE_TRANSPARENT_MODE_NORMAL     1
#define SMOKE_TRANSPARENT_MODE_OLDTRANS   2
#define SMOKE_TRANSPARENT_MODE_HALFTRANS  3
*/
/*
class _SmokeKindInfo
{
public:
	void Init(CHAR *file, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI32 siyindent);

	// robypark 2004/9/30 18:12
	// Smoke 정보를 HQ에서 InfoSmoke.txt에서 얻어와 사용한다.
	void Init(char *pszKindID);

	SHORT m_ssFile, m_ssStartFont;
	SHORT m_ssMoveFrameNumber;
	SHORT m_ssMaxRepeatNumber;
	SHORT m_ssRisingSwitch;
	SI32  m_siTransparentMode;
	SI32  m_siStepDelay;
	SI32  m_siYIndent;


};
*/
void _SmokeKindInfo::Init(CHAR *file, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI32 siyindent)
{
	m_ssFile			= GetValueOfGameSprNumID(file);

	m_ssStartFont		= startfont;
	m_ssMoveFrameNumber	= moveframenumber;
	m_ssMaxRepeatNumber	= maxrepeatnumber;
	m_ssRisingSwitch	= risingswitch;
	m_siTransparentMode	= transparentmode;
	m_siStepDelay		= siStepDelay;
	m_siYIndent			= siyindent;
}

// robypark 2004/9/30 18:12
// Smoke 정보를 HQ에서 InfoSmoke.txt에서 얻어와 사용한다.
void _SmokeKindInfo::Init(char *pszKindID)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "InfoSmoke.txt";

	// HQ에서 파일을 연다.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return;

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "SmokeInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return;
	}

	// 자료 검색
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받는다.
		SI32	ssStartFont, ssMoveFrameNumber, ssMaxRepeatNumber, ssRisingSwitch, siTransparentMode, siStepDelay, siYIndent;

		char	pszDestKindID[128], pszResourceID[128], pszTemp[256];

		ocp.GetValue( pszDestKindID,		127,		iLineNo, iColNo );		// Smoke Kind ID

		//----------------------------------------------------------------------------
		// 일단 여기까지 찾고 정보가 일치하지 않으면 다음 라인 값을 추출.
		if ( stricmp(pszDestKindID, pszKindID) != 0)
		{
			continue;
		}
		// 넘어왔다면 찾은 것이다.
		//----------------------------------------------------------------------------

		ocp.GetValue( pszResourceID,		127,	iLineNo, iColNo );		// Resource ID
		ocp.GetValue( ssStartFont,					iLineNo, iColNo );		// Start Font
		ocp.GetValue( ssMoveFrameNumber,			iLineNo, iColNo );		// Move Frame Number
		ocp.GetValue( ssMaxRepeatNumber,			iLineNo, iColNo );		// Map Repeat Number
		ocp.GetValue( siStepDelay,					iLineNo, iColNo );		// Step Delay Constant Value
		ocp.GetValue( ssRisingSwitch,				iLineNo, iColNo );		// Rising Switch
		ocp.GetValue( siTransparentMode,			iLineNo, iColNo );		// Transparent Mode
		ocp.GetValue( siYIndent,					iLineNo, iColNo );		// Y Indent
		ocp.GetValue( pszTemp,				255,	iLineNo, iColNo );		// Description
		
		fclose( fp );
		ocp.Release();


		if (strcmp(pszResourceID, "0") == 0)
		{
			m_ssFile = 0;
		}
		else
		{
			m_ssFile = GetValueOfGameSprNumID(pszResourceID);
		}

		m_ssStartFont		= ssStartFont;
		m_ssMoveFrameNumber	= ssMoveFrameNumber;
		m_ssMaxRepeatNumber	= ssMaxRepeatNumber;
		m_ssRisingSwitch	= ssRisingSwitch;
		m_siTransparentMode	= siTransparentMode;
		m_siStepDelay		= siStepDelay;
		m_siYIndent			= siYIndent;

		return;
	}

	fclose( fp );
	ocp.Release();

#ifdef _DEBUG
	clGrp.Error("Error: smoke.cpp Init()", "smokeInfo [%s] : Not registered.", pszKindID);
#else
	clGrp.Error("Effect Image Information Setting Error", "거상 클라이언트의 변조가 의심됩니다. 설치된 거상 클라이언트를 확인해주세요.\nGersang(Kyosyoden) Client doubt modified. Please Check your client.");
#endif
}

_SmokeKindInfo g_SKI[MAX_SMOKE_KIND_NUMBER];

void InitSmokeKindInfo()
{
	////////////////////////////////////////////////////////////
	// robypark 2004/10/1 3:14
	// HQ의 InfoSmoke.txt에서 데이터를 얻어와 사용하도록 수정.
	// 온라인 공용 파서를 이용하여 작성되었다.
	// 새로운 이펙트를 추가할 때에는 smoke.h에서 해당 Smoke 종류
	// 에 관한 Kind 상수값을 선언하여주며, InitSmokeKindInfo()
	// 함수에서 초기화 부분을 추가해 준다. 또한 InfoSmoke.txt에
	// 도 추가해준다.
	////////////////////////////////////////////////////////////

	g_SKI[KIND_SMOKE_EXP0].Init("KIND_SMOKE_EXP0");
	g_SKI[KIND_SMOKE_EXP1].Init("KIND_SMOKE_EXP1");
	g_SKI[KIND_SMOKE_BIGEXP2].Init("KIND_SMOKE_BIGEXP2");
	g_SKI[KIND_SMOKE_EXP2].Init("KIND_SMOKE_EXP2");
	g_SKI[KIND_SMOKE_EXP3].Init("KIND_SMOKE_EXP3");
	g_SKI[KIND_SMOKE_EXP4].Init("KIND_SMOKE_EXP4");
	g_SKI[KIND_SMOKE_EXP5].Init("KIND_SMOKE_EXP5");
	g_SKI[KIND_SMOKE_EXP6].Init("KIND_SMOKE_EXP6");
	g_SKI[KIND_SMOKE_EXP7].Init("KIND_SMOKE_EXP7");
	g_SKI[KIND_SMOKE_EXP8].Init("KIND_SMOKE_EXP8");
	g_SKI[KIND_SMOKE_EXP9].Init("KIND_SMOKE_EXP9");
	g_SKI[KIND_SMOKE_EXP10].Init("KIND_SMOKE_EXP10");
    g_SKI[KIND_SMOKE_EXP11].Init("KIND_SMOKE_EXP11");
    g_SKI[KIND_SMOKE_EXP12].Init("KIND_SMOKE_EXP12");
    g_SKI[KIND_SMOKE_EXP13].Init("KIND_SMOKE_EXP13");
	g_SKI[KIND_SMOKE_EXP14].Init("KIND_SMOKE_EXP14");
	g_SKI[KIND_SMOKE_EXP15].Init("KIND_SMOKE_EXP15");
	g_SKI[KIND_SMOKE_SMALLEXP2].Init("KIND_SMOKE_SMALLEXP2");
	g_SKI[KIND_SMOKE_GUNSMOKE].Init("KIND_SMOKE_GUNSMOKE");
	g_SKI[KIND_SMOKE_GUNHIT].Init("KIND_SMOKE_GUNHIT");
	g_SKI[KIND_SMOKE_SMOKE0].Init("KIND_SMOKE_SMOKE0");
	g_SKI[KIND_SMOKE_SMOKE1].Init("KIND_SMOKE_SMOKE1");
	g_SKI[KIND_SMOKE_SMOKE2].Init("KIND_SMOKE_SMOKE2");
	g_SKI[KIND_SMOKE_SMOKE3].Init("KIND_SMOKE_SMOKE3");
	g_SKI[KIND_SMOKE_SMOKE10].Init("KIND_SMOKE_SMOKE10");
	g_SKI[KIND_SMOKE_RISINGSMOKE0].Init("KIND_SMOKE_RISINGSMOKE0");
	g_SKI[KIND_SMOKE_RISINGSMOKE1].Init("KIND_SMOKE_RISINGSMOKE1");
	g_SKI[KIND_SMOKE_FIRE].Init("KIND_SMOKE_FIRE");
	g_SKI[KIND_SMOKE_BIGEXP].Init("KIND_SMOKE_BIGEXP");
	g_SKI[KIND_SMOKE_MANFIRE].Init("KIND_SMOKE_MANFIRE");
	g_SKI[KIND_SMOKE_SINK].Init("KIND_SMOKE_SINK");
	g_SKI[KIND_SMOKE_WATEREXP].Init("KIND_SMOKE_WATEREXP");
	g_SKI[KIND_SMOKE_EARTHQUAKE].Init("KIND_SMOKE_EARTHQUAKE");
	g_SKI[KIND_SMOKE_LIGHTNING].Init("KIND_SMOKE_LIGHTNING");
	g_SKI[KIND_SMOKE_BUILDINGEXP].Init("KIND_SMOKE_BUILDINGEXP");
	g_SKI[KIND_SMOKE_LIGHTEXP1].Init("KIND_SMOKE_LIGHTEXP1");
	g_SKI[KIND_SMOKE_LIGHTEXP2].Init("KIND_SMOKE_LIGHTEXP2");
	g_SKI[KIND_SMOKE_LIGHTEXP3].Init("KIND_SMOKE_LIGHTEXP3");
	g_SKI[KIND_SMOKE_GROUNDEXP1].Init("KIND_SMOKE_GROUNDEXP1");
	g_SKI[KIND_SMOKE_GROUNDEXP2].Init("KIND_SMOKE_GROUNDEXP2");
	g_SKI[KIND_SMOKE_GROUNDEXP3].Init("KIND_SMOKE_GROUNDEXP3");
	g_SKI[KIND_SMOKE_TORNADO].Init("KIND_SMOKE_TORNADO");
	g_SKI[KIND_SMOKE_SMALL_TORNADO].Init("KIND_SMOKE_SMALL_TORNADO");
	g_SKI[KIND_SMOKE_KIMBOMB1].Init("KIND_SMOKE_KIMBOMB1");
	g_SKI[KIND_SMOKE_KIMBOMB2].Init("KIND_SMOKE_KIMBOMB2");
	g_SKI[KIND_SMOKE_KIMBOMB3].Init("KIND_SMOKE_KIMBOMB3");
	g_SKI[KIND_SMOKE_MAGIC8K_EXP].Init("KIND_SMOKE_MAGIC8K_EXP");
	g_SKI[KIND_SMOKE_MAGIC6C1].Init("KIND_SMOKE_MAGIC6C1");
	g_SKI[KIND_SMOKE_MAGIC6C2].Init("KIND_SMOKE_MAGIC6C2");
	g_SKI[KIND_SMOKE_MAGIC6C3].Init("KIND_SMOKE_MAGIC6C3");
	g_SKI[KIND_SMOKE_MAGIC4C].Init("KIND_SMOKE_MAGIC4C");
	g_SKI[KIND_SMOKE_SNOWMAGIC].Init("KIND_SMOKE_SNOWMAGIC");
	g_SKI[KIND_SMOKE_MAGIC4CEXP].Init("KIND_SMOKE_MAGIC4CEXP");
	g_SKI[KIND_SMOKE_SNOWMAGICEXP].Init("KIND_SMOKE_SNOWMAGICEXP");
	g_SKI[KIND_SMOKE_ADVTOWERKEXP1].Init("KIND_SMOKE_ADVTOWERKEXP1");
	g_SKI[KIND_SMOKE_ADVTOWERKEXP2].Init("KIND_SMOKE_ADVTOWERKEXP2");
	g_SKI[KIND_SMOKE_GROUNDFIRESMOKE].Init("KIND_SMOKE_GROUNDFIRESMOKE");
	g_SKI[KIND_SMOKE_ARROWTRAIL].Init("KIND_SMOKE_ARROWTRAIL");
	g_SKI[KIND_SMOKE_SPECIAL].Init("KIND_SMOKE_SPECIAL");
	g_SKI[KIND_SMOKE_WATEREFFECT].Init("KIND_SMOKE_WATEREFFECT");
	g_SKI[KIND_SMOKE_DUST].Init("KIND_SMOKE_DUST");
	g_SKI[KIND_SMOKE_DETECTMINEEFFECT].Init("KIND_SMOKE_DETECTMINEEFFECT");
	g_SKI[KIND_SMOKE_CLONAPPEAR].Init("KIND_SMOKE_CLONAPPEAR");
	g_SKI[KIND_SMOKE_GUARDIAN_CHICK].Init("KIND_SMOKE_GUARDIAN_CHICK");
	g_SKI[KIND_SMOKE_CLONDIE].Init("KIND_SMOKE_CLONDIE");
	g_SKI[KIND_SMOKE_GOLDDRAGONELECTRON].Init("KIND_SMOKE_GOLDDRAGONELECTRON");
	g_SKI[KIND_SMOKE_GOLDDRAGONELECTRONEXP].Init("KIND_SMOKE_GOLDDRAGONELECTRONEXP");
	g_SKI[KIND_SMOKE_GHOSTCATEXP].Init("KIND_SMOKE_GHOSTCATEXP");
	g_SKI[KIND_SMOKE_WAVEEFFECT].Init("KIND_SMOKE_WAVEEFFECT");
	g_SKI[KIND_SMOKE_BIGTREE].Init("KIND_SMOKE_BIGTREE");
	g_SKI[KIND_SMOKE_MAGIC4T].Init("KIND_SMOKE_MAGIC4T");
	g_SKI[KIND_SMOKE_DARKFIRE].Init("KIND_SMOKE_DARKFIRE");
	g_SKI[KIND_SMOKE_GHOST].Init("KIND_SMOKE_GHOST");
	g_SKI[KIND_SMOKE_PANDA].Init("KIND_SMOKE_PANDA");
	g_SKI[KIND_SMOKE_FIREATTACK].Init("KIND_SMOKE_FIREATTACK");
	g_SKI[KIND_SMOKE_RUNNINGFIRE].Init("KIND_SMOKE_RUNNINGFIRE");
	g_SKI[KIND_SMOKE_DOUBLE_SWORD].Init("KIND_SMOKE_DOUBLE_SWORD");
	g_SKI[KIND_SMOKE_ICE_BARRIER].Init("KIND_SMOKE_ICE_BARRIER");
	g_SKI[KIND_SMOKE_LONG_SPEAR].Init("KIND_SMOKE_LONG_SPEAR");
	g_SKI[KIND_SMOKE_LONG_SPEAR_HIT].Init("KIND_SMOKE_LONG_SPEAR_HIT");
	g_SKI[KIND_SMOKE_PAPERDOLL_HIT].Init("KIND_SMOKE_PAPERDOLL_HIT");
	g_SKI[KIND_SMOKE_CLOUD_WIND_HIT].Init("KIND_SMOKE_CLOUD_WIND_HIT");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN1].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN1");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN2].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN2");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN3].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN3");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN4].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN4");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN5].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN5");
	g_SKI[KIND_SMOKE_FLOWER_NEEDLE_RAIN6].Init("KIND_SMOKE_FLOWER_NEEDLE_RAIN6");
	g_SKI[KIND_SMOKE_SOUL_CHANGE_HIT].Init("KIND_SMOKE_SOUL_CHANGE_HIT");
	g_SKI[KIND_SMOKE_STONE_MAGIC_STONE].Init("KIND_SMOKE_STONE_MAGIC_STONE");
	g_SKI[KIND_SMOKE_STONE_MAGIC_SMOKE].Init("KIND_SMOKE_STONE_MAGIC_SMOKE");
	g_SKI[KIND_SMOKE_STONE_MAGIC_SMOKE].Init("KIND_SMOKE_STONE_MAGIC_SMOKE");
	g_SKI[KIND_SMOKE_GOD_SPEAR].Init("KIND_SMOKE_GOD_SPEAR");
	g_SKI[KIND_FIREENERGYPA_HIT1].Init("KIND_FIREENERGYPA_HIT1");
	g_SKI[KIND_FIREENERGYPA_HIT2].Init("KIND_FIREENERGYPA_HIT2");

	// actdoll (2004/03/06 9:58) : 유명계/진시황릉
	g_SKI[KIND_SMOKE_JINEMPEROR_HIT].Init("KIND_SMOKE_JINEMPEROR_HIT");
	g_SKI[KIND_SMOKE_GUARDIAN_CAW].Init("KIND_SMOKE_GUARDIAN_CAW");
	g_SKI[KIND_SMOKE_GUARDIAN_TIGER].Init("KIND_SMOKE_GUARDIAN_TIGER");

//----------------------------------------------------------------------------
// robypark (2004/05/18 11:04) : 추가 기술의 SMOKE - 장수 2차 전직 특수 기술
//----------------------------------------------------------------------------
	g_SKI[KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT].Init("KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT");
	g_SKI[KIND_SMOKE_POISON_ICE_BULLET_HIT].Init("KIND_SMOKE_POISON_ICE_BULLET_HIT");
	g_SKI[KIND_SMOKE_EARTHQUAKE_WAVE].Init("KIND_SMOKE_EARTHQUAKE_WAVE");
	g_SKI[KIND_SMOKE_THUNDER_BOLT_HIT].Init("KIND_SMOKE_THUNDER_BOLT_HIT");

// 2차 장수 스킬 추가 관련
	g_SKI[KIND_SMOKE_FOG_OF_ICE_HIT].Init("KIND_SMOKE_FOG_OF_ICE_HIT");

	// 천지독살
	g_SKI[KIND_SMOKE_FOG_OF_POISON].Init("KIND_SMOKE_FOG_OF_POISON");
	g_SKI[KIND_SMOKE_FOG_OF_ICE].Init("KIND_SMOKE_FOG_OF_ICE");

	// 승천포
	g_SKI[KIND_SMOKE_MUSHROOM_CLOUD].Init("KIND_SMOKE_MUSHROOM_CLOUD");

	// 강강수월래
	g_SKI[KIND_SMOKE_HEAD_OF_DRAGON].Init("KIND_SMOKE_HEAD_OF_DRAGON");
	g_SKI[KIND_SMOKE_FIRE_ROUND_DANCE_FLOOR].Init("KIND_SMOKE_FIRE_ROUND_DANCE_FLOOR");

	// 천벌화시
	g_SKI[KIND_SMOKE_FIRERAIN_OF_GOD].Init("KIND_SMOKE_FIRERAIN_OF_GOD");

	// 격노염폭
	g_SKI[KIND_SMOKE_CREATE_VOLCANO].Init("KIND_SMOKE_CREATE_VOLCANO");
	g_SKI[KIND_SMOKE_VOLCANO_HIT].Init("KIND_SMOKE_VOLCANO_HIT");

	// 승천포 작은 폭발
	g_SKI[KIND_SMOKE_BOMB_SMOKE1].Init("KIND_SMOKE_BOMB_SMOKE1");
	g_SKI[KIND_SMOKE_BOMB_SMOKE2].Init("KIND_SMOKE_BOMB_SMOKE2");
	g_SKI[KIND_SMOKE_BOMB_SMOKE3].Init("KIND_SMOKE_BOMB_SMOKE3");

	// robypark 2004/6/4 19:21
	// 불랑기포차 기술 앙천대소 포탄 폭발 smoke
	g_SKI[KIND_SMOKE_ROAR_OF_LAUGHTER_HIT].Init("KIND_SMOKE_ROAR_OF_LAUGHTER_HIT");

	// robypark 2004/6/18 12:39
	// 지진차 격노염폭 기술 사용시 생성되는 화산 위에 덧그릴 먼지(연기)
	g_SKI[KIND_SMOKE_CREATE_VOLCANO_DUST].Init("KIND_SMOKE_CREATE_VOLCANO_DUST");

	// robypark 2004/11/15 15:43
	// 공성전 전투 중에 공격 히트 효과
	g_SKI[KIND_SMOKE_SIEGEWARFARE_IN_FIGHT].Init("KIND_SMOKE_SIEGEWARFARE_IN_FIGHT");

	// robypark 2004/11/24 17:36
	// 공성전 발석거 돌 히트 효과
	g_SKI[KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT].Init("KIND_SMOKE_SIEGEWARFARE_CATAPULT_STRONE_HIT");

	// robypark 2004/12/6 11:27
	// 공성전 궁병 화살 히트 효과
	g_SKI[KIND_SMOKE_SIEGEWARFARE_ARROW_HIT].Init("KIND_SMOKE_SIEGEWARFARE_ARROW_HIT");
}

_Smoke::_Smoke()
{
	ZeroMemory(this, sizeof(_Smoke));
}

void _Smoke::Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, char area) 
{
	ZeroMemory(this, sizeof(_Smoke));

		
	m_siSpeed				=	10;	

	m_ssID					=	id;
	m_ssKind				=	kind;
	m_ssFile				=	g_SKI[m_ssKind].m_ssFile;
	m_ssStartFont			=	g_SKI[m_ssKind].m_ssStartFont;
    m_scMoveFrameNumber		=	(char)g_SKI[m_ssKind].m_ssMoveFrameNumber;
    m_ssMaxRepeatNumber		=	g_SKI[m_ssKind].m_ssMaxRepeatNumber;
	m_ssRisingSwitch		=	g_SKI[m_ssKind].m_ssRisingSwitch;
	m_ssTransparentMode		=	g_SKI[m_ssKind].m_siTransparentMode;
	m_siStepDelay			=	g_SKI[m_ssKind].m_siStepDelay;


	m_scDrawSwitch=TRUE;

	m_ssRealX=realx;
	m_ssRealY=realy;

	m_scArea=area;

	m_AttackID.Init();
	m_AttackerID.Init();

	m_ssCurrentOrder=ORDER_SMOKE_EXP;

	m_uiLastChangedFrame = GD.Frame;
}

void _Smoke::SetInfo()
{
	GD.SmokeInfo[m_ssID] = m_ssKind;
}

BOOL _Smoke::Action()
{
	switch(m_ssCurrentOrder)
	{
	case ORDER_SMOKE_EXP:
		
		// 위로 올라간다. 
		if(m_ssRisingSwitch==TRUE)
		{
			m_ssRealY-=3;
		}
		// 풍백술인 경우 랜덤하게 움직인다. 
		if(m_ssKind == KIND_SMOKE_TORNADO || m_ssKind == KIND_SMOKE_SMALL_TORNADO)
		{
			// 타겟이 유효하면. 
			if(IsAlive(m_AttackID))
			{
//				if(Kind == KIND_SMOKE_TORNADO)
//				{
					if(clifChar.GetCenterX(m_AttackID.ID) > m_ssRealX)
						m_ssRealX+=5;
					if(clifChar.GetCenterX(m_AttackID.ID) < m_ssRealX)
						m_ssRealX-=5;
					if(clifChar.GetCenterY(m_AttackID.ID) > m_ssRealY)
						m_ssRealY+=5;
					if(clifChar.GetCenterY(m_AttackID.ID) < m_ssRealY)
						m_ssRealY-=5;
//				}
			}
			else
			{
				if(m_ssKind == KIND_SMOKE_TORNADO)
				{
					m_ssRealX += (4 - GD.Random() % 8);
					m_ssRealY += (4 - GD.Random() % 8);
				}
				else if(m_ssKind == KIND_SMOKE_SMALL_TORNADO)
				{
					m_ssRealX += (4 - GD.Random() % 8);
					m_ssRealY += (4 - GD.Random() % 8);
				}

			}

			if(m_ssStep % 10 == 0)
			{	
				if(m_ssKind == KIND_SMOKE_TORNADO)
					CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_TORNADO, 2, AREA_LAND, m_ssApower, ZeroID, m_ssX, m_ssY, 70);
				else if(m_ssKind == KIND_SMOKE_SMALL_TORNADO)
					CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_TORNADO, 1, AREA_LAND, m_ssApower, ZeroID, m_ssX, m_ssY, 70);
			}

		}
		else if(m_ssKind == KIND_SMOKE_LONG_SPEAR)
		{
			m_siSpeed = (float)(m_siSpeed * 1.1);
			m_ssRealY += (SHORT)m_siSpeed; 
			/*
			if(m_ssStep==m_ssMoveFrameNumber)
			{
				//SHORT smokeid=FindEmptySmoke();
				//SetSmoke(KIND_SMOKE_LONG_SPEAR_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
				//CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_LONG_SPEAR, 2, AREA_LAND, m_siApower * 4, ZeroID, m_ssX, m_ssY, 100);
			}*/
		}
		else if(m_ssKind == KIND_SMOKE_SOUL_CHANGE_HIT)
		{
			if(IsAlive(m_AttackID))
			{
				//SI32 siWidth = CR[m_AttackID.ID].CharDrawInfo.GetRealImageRect().right - CR[m_AttackID.ID].CharDrawInfo.GetRealImageRect().left;
				//SI32 siHeight = CR[m_AttackID.ID].CharDrawInfo.GetRealImageRect().bottom - CR[m_AttackID.ID].CharDrawInfo.GetRealImageRect().top;
				m_ssRealX = clifChar.GetCenterX(m_AttackID.ID);
				m_ssRealY = clifChar.GetCenterY(m_AttackID.ID);
			}
			else
				return FALSE;
		}
		// 원심분쇄술이면, 
		else if(m_ssKind == KIND_SMOKE_MAGIC4T)
		{
			// 타겟이 유효하면. 
			if(IsAlive(m_AttackID))
			{
				if(clifChar.GetCenterX(m_AttackID.ID) > m_ssRealX)
					m_ssRealX+=4;
				if(clifChar.GetCenterX(m_AttackID.ID) < m_ssRealX)
					m_ssRealX-=4;
				if(clifChar.GetCenterY(m_AttackID.ID) > m_ssRealY)
					m_ssRealY+=4;
				if(clifChar.GetCenterY(m_AttackID.ID) < m_ssRealY)
					m_ssRealY-=4;
			}
			else
			{
				m_ssRealX += (3 - GD.Random() % 7);
				m_ssRealY += (3 - GD.Random() % 7);
			}

			if(m_ssStep % 10 == 0)
			{
				CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_TORNADO, 2, AREA_LAND, m_ssApower, ZeroID, m_ssX, m_ssY, 70);
			}
		}
		else if(m_ssKind == KIND_SMOKE_SOUL_CHANGE_HIT)
		{
			if(IsAlive(m_AttackID))
			{
				m_ssRealX = clifChar.GetCenterX(m_AttackID.ID);
				m_ssRealY = clifChar.GetCenterY(m_AttackID.ID);
			}
			else
				return FALSE;
		}
		else if(m_ssKind == KIND_SMOKE_GHOST)
		{
			if(IsAlive(m_AttackID))
			{
				if(!clifChar.GetGhostTime(m_AttackID.ID))	return FALSE;

				SHORT DifGhostTime = (SHORT)((timeGetTime() - clifChar.GetGhostTime(m_AttackID.ID))/1000);

				if(DifGhostTime <= 5)
				{
					m_ssRealX = clifChar.GetCenterX(m_AttackID.ID);
					m_ssRealY = clifChar.GetCenterY(m_AttackID.ID);
				}
				else
				{
					clifChar.SetGhostTime(m_AttackID.ID, 0);
					return FALSE;
				}
			}
			else
				return FALSE;
		}
		// actdoll (2004/03/06 10:09) : 일단 사용하지 않으므로 임시로 막았다
/*
		else if(Kind == KIND_SMOKE_RABBIT)
		{
			if(IsAlive(AttackID))
			{
				if(CR[AttackID.ID].m_bRabbitAttack == true)
				{
					RealX = clifChar.GetCenterX(AttackID.ID);
					RealY = clifChar.GetCenterY(AttackID.ID);
				}
				else 
					return FALSE;
			}
			else
				return FALSE;

		}
*/
		m_ssAnimation = ANI_EXP;

		if(abs(m_uiLastChangedFrame-GD.Frame) >= m_siStepDelay)
		{
			m_uiLastChangedFrame=GD.Frame;
		    m_ssStep++;
		}

		if(m_ssStep == m_scMoveFrameNumber)
		{
			// 애니메이션이 끝났다는 것을 플래그 변수에 저장
			m_bIsDoneAnimation = true;

			if(m_ssKind == KIND_SMOKE_LONG_SPEAR)
			{
				// 화면 흔들림을 만든다. 
 				GD.EarthQuake.SetEarthQuake();
				PushEffect(EFFECT_ATTACK_GENERAL_J4_EXT, clifChar.GetX(m_AttackerID.ID), clifChar.GetY(m_AttackerID.ID));
				
				SHORT smokeid=FindEmptySmoke();
				SetSmoke(KIND_SMOKE_LONG_SPEAR_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
				CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_LONG_SPEAR, 1, AREA_LAND, m_siApower, ZeroID, m_ssX + 2, m_ssY + 2, 100);
				//CalculateHit(AttackerID, PlayerNumber, ATTACKATB_LONG_SPEAR, 1, AREA_LAND, m_siApower, 
				//	ZeroID, CR[AttackID.ID].GetX(), CR[AttackID.ID].GetY(), 100);
			}
			// robypark 2004-6-1
			// 천벌화시에 맞았을 때 데미지 주기
			else if (m_ssKind == KIND_SMOKE_FIRERAIN_OF_GOD)
			{
				SI32 siID = clifMap.GetIDMap(m_ssX, m_ssY);
//				if ((IsAlive(siID))
//					&& (IsFriendToByPlayerNumber(CR[AttackerID.ID].GetPlayerNumber(), CR[siID].GetPlayerNumber()) == FALSE))
				{
					m_AttackID = CR[siID].m_NewID;
					CalculateHit(m_AttackerID, m_ssPlayerNumber, ATTACKATB_TURTLE_VEHICLE, 1, AREA_LAND, m_siApower, m_AttackID, m_ssX, m_ssY, 100);
//					CalculateHit(AttackerID, PlayerNumber, ATTACKATB_THUNDER_VEHICLE, 0, AREA_LAND, m_siApower, AttackID, X, Y, 100);
				}

				// 불을 낸다.
				if ((GD.Random() & 7) == 0)
					clifMap.PutFire(m_ssX, m_ssY, 1, GROUNDFIRE_LIFETIME / 2, m_AttackerID, m_ssPlayerNumber);
			}
			// robypark 2004-6-1
			// 격노염폭에서 화산 생성관련 처리
			else if (m_ssKind == KIND_SMOKE_CREATE_VOLCANO)
			{
				// 한번 애니메이션 후, 마지막 화산 이미지만 보이도록 한다.
				m_ssStep = m_scMoveFrameNumber - 1;
				CalculateRealXY();
				m_ssFont = m_ssStartFont + m_scMoveFrameNumber - 1;
				return TRUE;
				break;
			}

			if(m_ssRepeatNumber<m_ssMaxRepeatNumber)
			{
				m_ssRepeatNumber++;
				m_ssStep=0;
			}
			else
			  return FALSE;
		}
		break;
	default:
		break;
	}

	CalculateRealXY();

	if(m_ssKind == KIND_SMOKE_ICE_BARRIER && m_ssRepeatNumber)
		m_ssFont = m_ssStartFont + m_scMoveFrameNumber - 1;
	else
		DecideFont();

	return TRUE;
}

void _Smoke::CalculateRealXY()
{
	CalculateRealXYNormal();  
}

void _Smoke::CalculateDrawStartXY()
{
	SHORT screenx=0, screeny=0;

	clifMap.TransFromRealDotToScreenDot(m_ssRealX, m_ssRealY, screenx, screeny);
	m_ssDrawStartX = screenx - m_ssDrawXsize/2;
	m_ssDrawStartY = screeny - m_ssDrawYsize/2-g_SKI[m_ssKind].m_siYIndent;
}

void SetSmokeRealXY(SHORT id, SHORT realx, SHORT realy)
{
	g_Smoke[id].m_ssRealX = realx;
	g_Smoke[id].m_ssRealY = realy;
}

// 찍을 위치를 계산한다. 
void _Smoke::CalculateRealXYNormal()
{
//	int centerx, centery;  // 현재 위치의 중심 좌표 

	// X, Y좌표를 구한다. 
	clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);

	// hojae_repair
	m_ssDrawXsize=GetFileXsize(m_ssFile);
	m_ssDrawYsize=GetFileYsize(m_ssFile);

	m_ssCenterX = m_ssRealX;
	m_ssCenterY = m_ssRealY;
}


void _Smoke::DecideFont()
{
	DecideFontNormal();
}

void _Smoke::DecideFontNormal()
{
	switch(m_ssAnimation)
	{
	case ANI_EXP:
		m_ssFont = m_ssStartFont + m_ssStep;
		break;
	default:
		break;
	}
}


BOOL _Smoke::Draw()
{
	unsigned char *darktable;
	// 어둔 단계를 설정한다. 
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	darktable=TeamDarkTable[darkstep];

	if(m_scDrawSwitch==FALSE)return FALSE;

   if(m_scReverseSwitch==FALSE)
   {
		if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX + GetFileXsize(m_ssFile)-1, m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)	// hojae_repair
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		else
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
   }
   else if(m_scReverseSwitch==TRUE)
   {
		if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, m_ssDrawStartX+GetFileXsize(m_ssFile)-1, m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)	// hojae_repair
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		else
		{
			switch(m_ssTransparentMode)
			{
			case SMOKE_TRANSPARENT_MODE_OLDTRANS:
				Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			case SMOKE_TRANSPARENT_MODE_HALFTRANS:
				Draw_PutSpriteLightCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);	// hojae_append
				break;
			default:
				Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);	// hojae_repair
				break;
			}
		}
		
   }

	return TRUE;
}

SHORT FindEmptySmoke()
{
	int i;

	for(i=1;i<MAX_SMOKE_NUMBER;i++)
	{
		if (GD.SmokeInfo[i] == 0)
			return i;
	}
	return 0;

}

BOOL IsLiveSmoke(SHORT id)
{
	if(id < 0)
		return FALSE;

	if (GD.SmokeInfo[id] == 0)
		return FALSE;
	else
		return TRUE;
}

void DeleteSmoke(SHORT id)
{
	GD.SmokeInfo[id]=0;
}

void DeleteSmoke()
{
	SHORT i;

	for(i=0;i<MAX_SMOKE_NUMBER;i++)
	{
		DeleteSmoke(i);
	}
}

// Smoke를 설정한다.
// SI16 siKind[in]: SMOKE 종류
// SI16 id[in]: Smoke ID
// SI16 x[in]: Smoke를 생성할 위치(x)
// SI16 y[in]: Smoke를 생성할 위치(y)
// SI08 area[in]:
// SI16 siPlayerNumber[in]: Smoke를 생성한 플레이어 번호(default -1)
//
// robypark 2005-5-24, 16:37 지진차의 지진 공격 효과를 위하여 추가
// SI08 bFloor[in]: 해당 Smoke의 출력이 바닥(Floor), 즉 오브젝트 전에 그려야 할 경우 TRUE를 설정한다.
//					(default FALSE)
SHORT SetSmoke(SI16 siKind, SI16 id, SI16 x, SI16 y, SI08 area, SI16 siPlayerNumber/* = -1*/, SI08 bFloorSmoke/* = FALSE*/)
{
	if(id >= MAX_SMOKE_NUMBER)
	{
		clGrp.Error("Smoke", "%d %d", id, siKind);
		return 0;
	}

	// 중복 smoke는 제외
	if(siKind == KIND_SMOKE_DARKFIRE)
	{
		for(int i=1;i<=MAX_SMOKE_KIND_NUMBER;i++)
		{
			if(g_Smoke[i].m_ssRealX == x && g_Smoke[i].m_ssRealY == y)
			{
				return 0;
			}
		}
	}

	g_Smoke[id].Init(siKind, id, x, y, area);
	
	if( siKind == KIND_SMOKE_FIRE )
		g_Smoke[id].m_ssStartFont=(GD.Random()%5)*g_SKI[KIND_SMOKE_FIRE].m_ssMoveFrameNumber;

	// robypark 2004/8/27 10:53
	// 일부 스모크의 실제 맵 타일 위치가 계산되기 전에 데미지 계산 함수가 호출될 경우 타일 위치(0, 0)의
	// 위치에 항상 데미지를 주는 문제가 발생한다.
	// 타일 위치 변수인 X, Y의 값을 설정하여 준다.
	// 흑룡대차륜 기술 시전 시 발생하는 문제 해결(12시 맵 꼭지점에 적 캐릭터가 존재하여 흑룡대차륜 기술이 시전될 경우의 문제)
	g_Smoke[id].CalculateRealXY();

	g_Smoke[id].SetInfo();
	
	if( siPlayerNumber != -1 )
	{
		// 연기를 만든 플레이어를 지정한다. 
		g_Smoke[id].m_ssPlayerNumber = siPlayerNumber;
	}

	// robypark 2004-6-3 15:54
	// 지진차 격노염폭 기술 사용시 생성되는 화산을 위한 플래그 설정
	// smoke 애니메이션이 끝났는가? 를 나타내는 플래그
	// set value: true = 애니메이션이 끝났다.
	//			: false = 애니메이션이 끝나지 않았다.
	g_Smoke[id].m_bIsDoneAnimation = false;

	// robypark 2005-5-24, 16:37 지진차의 지진 공격 효과를 위하여 추가
	// 해당 Smoke의 출력이 바닥(Floor), 즉 오브젝트 전에 그려야 할 경우(bFloorSmoke = TRUE)의 Smoke인지 설정한다.
	g_Smoke[id].m_bFloorSmoke = bFloorSmoke;

	return 1;
}

//-----------------------------------------------------------------------------
// 2001.11.12 hojae_appned
//-----------------------------------------------------------------------------
BOOL Smoke_Action(SI32 i)
{
	return g_Smoke[i].Action();
}
void Smoke_CalculateDrawStartXY(SI32 i)
{
	g_Smoke[i].CalculateDrawStartXY();
}

void Smoke_Draw(SI32 i)
{
	g_Smoke[i].Draw();
}

void Smoke_PlayerNumber(SHORT smokeid, SHORT PlayerNumber)
{
	g_Smoke[smokeid].m_ssPlayerNumber = PlayerNumber;
}

void Smoke_Apower(SHORT smokeid, SHORT level)
{
	g_Smoke[smokeid].m_ssApower = level;
}

void SetApower(SI16 smokeid,SI16 siApower)
{
	g_Smoke[smokeid].m_siApower = siApower;
}

void Smoke_AttackID(SHORT smokeid, _NewID id)
{
	for(SHORT i=0; i<MAX_SMOKE_NUMBER; i++)
	{
		if(g_Smoke[i].m_ssKind == KIND_SMOKE_DARKFIRE)
		{			
			if(g_Smoke[i].m_AttackID.IsSame(id))
			{
				DeleteSmoke(i);
				break;
			}
		}
	}
	g_Smoke[smokeid].m_AttackID = id;
}

void Smoke_AttackerID(SHORT smokeid, _NewID id)
{
	g_Smoke[smokeid].m_AttackerID = id;
}

void Smoke_Fwrite(FILE* fp)
{
    fwrite(g_Smoke, sizeof(_Smoke),  MAX_SMOKE_NUMBER, fp);
}

void Smoke_Read(FILE* fp)
{
    fread(g_Smoke, sizeof(_Smoke),  MAX_SMOKE_NUMBER, fp);
}

void Smoke_SetYIndent(SI16 kind,SI32 YIndent)
{
	g_SKI[kind].m_siYIndent = YIndent;
}

// robypark 2004-6-3 15:54
// 지진차 격노염폭 기술 사용시 생성되는 화산을 위한 플래그 얻기
// smoke 애니메이션이 끝났는가? 를 나타내는 플래그 얻기
// return value: true = 애니메이션이 끝났다.
//			   : false = 애니메이션이 끝나지 않았다.
SI08 GetSmoke_IsDoneAnimation(SI32 siID)
{
	return g_Smoke[siID].m_bIsDoneAnimation;
}
