//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>

#include "..\Gersang\maindata.h"
#include <colortable.h>
#include <etc.h>
#include "..\Gersang\calcattack.h"
#include "..\Gersang\music.h"

#include <mapinterface.h>
#include <charinterface.h>
#include <charOrg.h>
#include <Magic.h>
#include <bullet.h>
#include <smoke.h>
#include <myfile.h>
#include "..\Gersang\DirectX.h"
#include <map.h>
#include <KindInfo.h>
#include <yprg2.h>
#include <OnlineImjin2i.h>
#include <clGame.h>
#include <OnlineBattleMsg.h>
#include <mynet-game.h>


#include "..\EnumData\Enum_AttackAtb.h"		// AttackAtb

#include "bindJXFile.h"		// using HQ file

// robypark 2004/9/30 18:28
// 온라인공용 파서 사용
#include "../SCCommon/OnlineCommonParser.h"

extern _clGame*				pGame;

extern	_NewID				ZeroID;
extern	_MainData			GD;
extern	cltCharInterface	clifChar;								// 캐릭터 관련 인터페이스 
extern	_Char				CR[];								// 캐릭터의 포인터 
extern	cltMapInterface		clifMap;								// 맵 관련 인터페이스 
extern	unsigned char		TeamDarkTable[MAX_DARK_STEP][256];
extern	unsigned char		DarkTable[][256];						// 어두운 단계 
extern	int					MapCenterX[][MAX_MAP_YSIZE];
extern	int					MapCenterY[][MAX_MAP_YSIZE];
extern	_Map 				Map;
extern	SI32				UserPlayerNumber;					// 사용자 관련 
extern	_KindInfo			KI[];								// 캐릭터 종류별 데이터 
extern  OnlineBattleInfo	OBInfo;
extern _OnlineBattleMsgQue  OBMQue;
extern	_MyNetGame			MyNet;

_Bullet g_Bullet[MAX_BULLET_NUMBER]; // 총알용 테이터 


// robypark 2004/9/30 18:28
// Smoke 정보를 HQ에서 InfoBullet.txt에서 얻어와 사용한다.
void _BulletKindInfo::Init(char *pszKindID, SI32 BulletAtb, SI32 BulletSmokeAtb)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "InfoBullet.txt";

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
		ocp.PrintError( "BulletInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
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
		SI32	ssStepWidth, ssFont, siMoveFrameNumber, bDirSwitch, 
					ssTailSwitch, ssChaseSwitch, ssCurveSwitch, 
					ssTransparentSwitch, ssHeightSwitch, siDelay;

		char	pszDestKindID[128], pszResourceID[128], pszTemp[256], pszHitEffectSoundIdx[128];

		ocp.GetValue( pszDestKindID,		127,		iLineNo, iColNo );		// Smoke Kind ID

		//----------------------------------------------------------------------------
		// 일단 여기까지 찾고 정보가 일치하지 않으면 다음 라인 값을 추출.
		if ( stricmp(pszDestKindID, pszKindID) != 0)
		{
			continue;
		}
		// 넘어왔다면 찾은 것이다.
		//----------------------------------------------------------------------------

		ocp.GetValue( ssStepWidth,					iLineNo, iColNo );		// Move Speed
		ocp.GetValue( pszResourceID,		127,	iLineNo, iColNo );		// Resource ID
		ocp.GetValue( ssFont,						iLineNo, iColNo );		// Start Font
		ocp.GetValue( siMoveFrameNumber,			iLineNo, iColNo );		// Move Frame Number
		ocp.GetValue( ssTailSwitch,					iLineNo, iColNo );		// Tail Switch
		ocp.GetValue( ssChaseSwitch,				iLineNo, iColNo );		// Chase Switch
		ocp.GetValue( ssCurveSwitch,				iLineNo, iColNo );		// Curve Switch
		ocp.GetValue( ssTransparentSwitch,			iLineNo, iColNo );		// Tranparent Switch
		ocp.GetValue( ssHeightSwitch,				iLineNo, iColNo );		// Height Switch
		ocp.GetValue( bDirSwitch,					iLineNo, iColNo );		// Direction Switch
		ocp.GetValue( siDelay,						iLineNo, iColNo );		// Delay
		ocp.GetValue( pszHitEffectSoundIdx,	128,	iLineNo, iColNo );		// Hit Effect Sound. EffectSound.txt에서 사용되는 효과음 인덱스 텍스트 이용.
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

		m_ssStepWidth = ssStepWidth;

		m_ssFont			  = ssFont;
		m_siMoveFrameNumber	  = siMoveFrameNumber;

		m_bDirSwitch		  = bDirSwitch;      
		m_ssTailSwitch		  = ssTailSwitch;
		m_ssChaseSwitch		  = ssChaseSwitch;
		m_ssCurveSwitch		  = ssCurveSwitch;
		m_ssTransparentSwitch = ssTransparentSwitch;
		m_ssHeightSwitch	  = ssHeightSwitch;

		m_siDelay		= siDelay;

		// 히트 효과음이 없다면
		if (strcmp(pszHitEffectSoundIdx, "0") == 0)
		{
			m_siHitEffectSoundIdx	= 0;
		}
		else
		{
			m_siHitEffectSoundIdx	= GetEffectDataMgr()->GetMyHashTable()->Find(pszHitEffectSoundIdx);
		}

		m_siAtb			= BulletAtb;
		m_siSmokeAtb	= BulletSmokeAtb;

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

// 총알 정보 변수 전역 변수 선언
_BulletKindInfo g_BKI[MAX_BULLET_KIND_NUMBER];


const SI32 BulletSpeedArrow = 30;	// 화살 날아가는 속도. 
const SI32 BulletSpeedGun	= 40;	// 총알 날아가는 속도. 

// Bullet 객체를 초기화한다.
// Bullet 이미지의 경우 hq내 ImgFile.txt파일에서 처음에 로딩하도록 플래그를 설정해 주어야 한다.
// 그렇지 않으면, 이미지가 제대로 나오지 않는다.
void InitBulletKindInfo()
{
	////////////////////////////////////////////////////////////
	// robypark 2004/10/1 3:14
	// HQ의 InfoBullet.txt에서 데이터를 얻어와 사용하도록 수정.
	// 온라인 공용 파서를 이용하여 작성되었다.
	// 새로운 총알을 추가할 때에는 bullet.h에서 해당 총알 종류
	// 에 관한 Kind 상수값을 선언하여주며, InitBulletKindInfo()
	// 함수에서 초기화 부분을 추가해 준다. 또한 InfoBullet.txt에
	// 도 추가해준다.
	////////////////////////////////////////////////////////////

	g_BKI[KIND_BULLET_AIRMISSILE].Init("KIND_BULLET_AIRMISSILE",
										BULLETATB_HITNORMAL|BULLETATB_AIRMISSILE|BULLETATB_WATER|BULLETATB_RANGE1,
										BULLETSMOKEATB_SMOKE_EXP15);

	g_BKI[KIND_BULLET_SAMMISSILE].Init("KIND_BULLET_SAMMISSILE",
										BULLETATB_HITNORMAL | BULLETATB_WATER|BULLETATB_RANGE1,
										BULLETSMOKEATB_SMOKE_EXP15);

	g_BKI[KIND_BULLET_AIRMAGIC].Init(	"KIND_BULLET_AIRMAGIC",
										BULLETATB_HITNORMAL|BULLETATB_AIRMISSILE|BULLETATB_WATER|BULLETATB_RANGE1,
										BULLETSMOKEATB_SMOKE_EXP15);

	g_BKI[KIND_BULLET_ARROW].Init(		"KIND_BULLET_ARROW",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_FIREARROW].Init(	"KIND_BULLET_FIREARROW",
										BULLETATB_HITNORMAL|BULLETATB_FIREARROW,
										0);

	g_BKI[KIND_BULLET_AIRATTACKUPARROW].Init("KIND_BULLET_AIRATTACKUPARROW",
											BULLETATB_HITNORMAL|BULLETATB_AIRATTACKUPARROW,
											0);

	g_BKI[KIND_BULLET_NEEDLE].Init(		"KIND_BULLET_NEEDLE",
										BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
										0);

	g_BKI[KIND_BULLET_SHIPCANNON].Init(	"KIND_BULLET_SHIPCANNON",
										BULLETATB_HITNORMAL|BULLETATB_CANNON2C|BULLETATB_WATER|BULLETATB_RANGE1,
										BULLETSMOKEATB_SMOKE_EXP6);

	g_BKI[KIND_BULLET_THROWBOMB].Init(	"KIND_BULLET_THROWBOMB",
										BULLETATB_HITNORMAL|BULLETATB_CANNON  |BULLETATB_WATER|BULLETATB_RANGE1,
										BULLETSMOKEATB_SMOKE_EXP6);

	g_BKI[KIND_BULLET_FIRE].Init(	"KIND_BULLET_FIRE",
									BULLETATB_HITNORMAL|BULLETATB_FIRE|BULLETATB_RANGE1|BULLETATB_MAKEFIRE,
									0);

	//화산 불꽃,용암괴물이 사용하는 불덩이.
	g_BKI[KIND_BULLET_VOLCANOFLAME].Init(	"KIND_BULLET_VOLCANOFLAME",
											BULLETATB_HITNORMAL|BULLETATB_FIREARROW,
											0);

	//이사의 사념이 사용하는 장풍.
	g_BKI[KIND_BULLET_EVIL].Init(	"KIND_BULLET_EVIL",
									BULLETATB_HITNORMAL|BULLETATB_ARROW,
									0);

	g_BKI[KIND_BULLET_GOLDDRAGONELECTRON].Init(	"KIND_BULLET_GOLDDRAGONELECTRON",
												BULLETATB_HITNORMAL|BULLETATB_FIRE|BULLETATB_RANGE1,
												BULLETSMOKEATB_SMOKE_GOLDDRAGONELECTRONEXP);

	g_BKI[KIND_BULLET_BOMB].Init(	"KIND_BULLET_BOMB",
									BULLETATB_HITNORMAL|BULLETATB_CANNON|BULLETATB_WATER|BULLETATB_RANGE1,
									BULLETSMOKEATB_SMOKE_EXP14);

	g_BKI[KIND_BULLET_FIRETANKARROW].Init(	"KIND_BULLET_FIRETANKARROW",
											BULLETATB_HITNORMAL|BULLETATB_WATER|BULLETATB_RANGE1,
											BULLETSMOKEATB_SMOKE_ADVTOWERKEXP1);

	g_BKI[KIND_BULLET_HEAVYGUN].Init     (	"KIND_BULLET_HEAVYGUN",
											BULLETATB_HITNORMAL|BULLETATB_CANNON|BULLETATB_WATER|BULLETATB_RANGE1,
											BULLETSMOKEATB_SMOKE_EXP0);

	g_BKI[KIND_BULLET_MAGICBALL1].Init   (	"KIND_BULLET_MAGICBALL1",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1|BULLETATB_RANGE1,
											BULLETSMOKEATB_SMOKE_EXP10);

	g_BKI[KIND_BULLET_MAGICBALL2].Init   (	"KIND_BULLET_MAGICBALL2",
											BULLETATB_HITNORMAL|BULLETATB_CANNON|BULLETATB_RANGE1,
											BULLETSMOKEATB_SMOKE_EXP9);

	g_BKI[KIND_BULLET_BUDAFIRE].Init     (	"KIND_BULLET_BUDAFIRE",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1|BULLETATB_RANGE1,
											0);

	g_BKI[KIND_BULLET_BUDAFIRE2].Init    (	"KIND_BULLET_BUDAFIRE2",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC2|BULLETATB_RANGE1,
											0);

	g_BKI[KIND_BULLET_BUDAFIRE3].Init    (	"KIND_BULLET_BUDAFIRE3",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC3|BULLETATB_RANGE1,
											0);

	g_BKI[KIND_BULLET_CANNON].Init       (	"KIND_BULLET_CANNON",
											BULLETATB_HITNORMAL|BULLETATB_CANNON|BULLETATB_WATER|BULLETATB_RANGE1,
											BULLETSMOKEATB_SMOKE_EXP7);


	g_BKI[KIND_BULLET_UPGRADETOWERCANNON].Init(	"KIND_BULLET_UPGRADETOWERCANNON",
												BULLETATB_HITNORMAL|BULLETATB_UPGRADETOWERCANNON|BULLETATB_WATER|BULLETATB_RANGE1,
												BULLETSMOKEATB_SMOKE_EXP7);

	g_BKI[KIND_BULLET_GUN].Init          (	"KIND_BULLET_GUN",
											BULLETATB_HITNORMAL|BULLETATB_GUN,
											0);

	g_BKI[KIND_BULLET_GHOST].Init        (	"KIND_BULLET_GHOST",
											BULLETATB_HITNORMAL|BULLETATB_GUN,
											BULLETSMOKEATB_SMOKE_GHOST);

	g_BKI[KIND_BULLET_GUNPOISON].Init    (	"KIND_BULLET_GUNPOISON",
											BULLETATB_HITNORMAL|BULLETATB_GUN,
											BULLETSMOKEATB_SMOKE_3);

	g_BKI[KIND_BULLET_STRONGGUN].Init    (	"KIND_BULLET_STRONGGUN",
											BULLETATB_HITNORMAL|BULLETATB_STRONGGUN,
											0);

	g_BKI[KIND_BULLET_GROUNDEXP].Init    (	"KIND_BULLET_GROUNDEXP",
											0,
											0);

	g_BKI[KIND_BULLET_DARKFIRE].Init     (	"KIND_BULLET_DARKFIRE",
											0,
											0);

	g_BKI[KIND_BULLET_FRAGMENT1].Init    (	"KIND_BULLET_FRAGMENT1",
											BULLETATB_FRAGMENT,
											0);

	g_BKI[KIND_BULLET_FRAGMENT2].Init    (	"KIND_BULLET_FRAGMENT2",
											BULLETATB_FRAGMENT,
											0);

	g_BKI[KIND_BULLET_FRAGMENT3].Init	 (	"KIND_BULLET_FRAGMENT3",
											BULLETATB_FRAGMENT,
											0);

	g_BKI[KIND_BULLET_DART].Init		 (	"KIND_BULLET_DART",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_MAGIC8K].Init		 (	"KIND_BULLET_MAGIC8K",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1,
											BULLETSMOKEATB_SMOKE_MAGIC8K_EXP);

	g_BKI[KIND_BULLET_ENERGYPA].Init	 (	"KIND_BULLET_ENERGYPA",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											0);

	g_BKI[KIND_BULLET_FIREENERGYPASTART].Init(	"KIND_BULLET_FIREENERGYPASTART",
												BULLETATB_HITNORMAL|BULLETATB_ARROW,
												0);

	g_BKI[KIND_BULLET_FIREENERGYPA].Init(	"KIND_BULLET_FIREENERGYPA",
											BULLETATB_HITNORMAL|BULLETATB_FIREENERGY,
											0);

	g_BKI[KIND_BULLET_MAGIC6C].Init		(	"KIND_BULLET_MAGIC6C",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC6C,
											0);

	g_BKI[KIND_BULLET_BURNARROW].Init	(	"KIND_BULLET_BURNARROW",
											BULLETATB_HITNORMAL|BULLETATB_FIREARROW,
											BULLETSMOKEATB_SMOKE_MAGIC8K_EXP|BULLETSMOKEATB_TRAIL_ARROWTRAIL);

	g_BKI[KIND_BULLET_MAGIC5J].Init		(	"KIND_BULLET_MAGIC5J",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1,
											BULLETSMOKEATB_SMOKE_MAGIC8K_EXP);

	g_BKI[KIND_BULLET_VOLCANO].Init		(	"KIND_BULLET_VOLCANO",
											BULLETATB_HITNORMAL|BULLETATB_CANNON2C|BULLETATB_RANGE1|BULLETATB_MAKEFIRE,
											0);

	g_BKI[KIND_BULLET_GATPHAPOISON].Init(	"KIND_BULLET_GATPHAPOISON",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1|BULLETATB_RANGE1, 
											0);

	g_BKI[KIND_BULLET_FURHEADDEMON].Init(	"KIND_BULLET_FURHEADDEMON",
											BULLETATB_HITNORMAL|BULLETATB_FURHEADDEMON,
											0);

	g_BKI[KIND_BULLET_ICE].Init(	"KIND_BULLET_ICE",
									BULLETATB_HITNORMAL|BULLETATB_FURHEADDEMON,
									0);
	
	g_BKI[KIND_BULLET_ONGHWA].Init(	"KIND_BULLET_ONGHWA",
									BULLETATB_HITNORMAL|BULLETATB_ONGHWA,
									0);

	g_BKI[KIND_BULLET_GHOSTCAT].Init    (	"KIND_BULLET_GHOSTCAT",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC3|BULLETATB_RANGE1|BULLETATB_HALT,
											BULLETSMOKEATB_SMOKE_GHOSTCATEXP);

	g_BKI[KIND_BULLET_DOLL1].Init		(	"KIND_BULLET_DOLL1",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_DOLL2].Init		(	"KIND_BULLET_DOLL2",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_KUKET].Init		(	"KIND_BULLET_KUKET",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_CATDOLL].Init		(	"KIND_BULLET_CATDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_OLDFOXDOLL].Init	(	"KIND_BULLET_OLDFOXDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_RACCOONDOLL].Init	(	"KIND_BULLET_RACCOONDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_NINJADOLL].Init	(	"KIND_BULLET_NINJADOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_SNOWWOMANDOLL].Init(	"KIND_BULLET_SNOWWOMANDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_YANGGUIBIDOLL].Init(	"KIND_BULLET_YANGGUIBIDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_JIJANGDOLL].Init(	"KIND_BULLET_JIJANGDOLL",
										BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
										0);

	g_BKI[KIND_BULLET_SNAKEWOMANDOLL].Init(	"KIND_BULLET_SNAKEWOMANDOLL",
											BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
											0);

	g_BKI[KIND_BULLET_SNOW].Init(	"KIND_BULLET_SNOW",
									BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
									0);

	g_BKI[KIND_BULLET_JUSTICEK].Init(	"KIND_BULLET_JUSTICEK",
										BULLETATB_HITNORMAL|BULLETATB_MAGIC2|BULLETATB_RANGE1|BULLETATB_HALT,
										0);

	g_BKI[KIND_BULLET_BUTTERFLY].Init(	"KIND_BULLET_BUTTERFLY",
										BULLETATB_HITNORMAL|BULLETATB_BUTTERFLY,
										0);

	g_BKI[KIND_BULLET_WIDOW].Init(	"KIND_BULLET_WIDOW",
									BULLETATB_HITNORMAL|BULLETATB_HALT|BULLETATB_WIDOW,
									0);
	
	g_BKI[KIND_BULLET_SEAHORSE].Init(	"KIND_BULLET_SEAHORSE",
										BULLETATB_HITNORMAL|BULLETATB_SEAHORSE|BULLETATB_RANGE1,
										0);
	
	g_BKI[KIND_BULLET_POISONARROW].Init(	"KIND_BULLET_POISONARROW",
											BULLETATB_HITNORMAL|BULLETATB_POISONARROW,
											0);

	g_BKI[KIND_BULLET_THROWSPEAR].Init(	"KIND_BULLET_THROWSPEAR",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_CACTUSBULLET].Init(	"KIND_BULLET_CACTUSBULLET",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											0);

	g_BKI[KIND_BULLET_SCORPIONBULLET].Init(	"KIND_BULLET_SCORPIONBULLET",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1|BULLETATB_RANGE1|BULLETATB_HALT,	// robypark 2004/9/1 16:33 독전갈 미사일 마비 공격속성 추가
											0);

	g_BKI[KIND_BULLET_MAGICIANBULLET].Init(	"KIND_BULLET_MAGICIANBULLET",
											BULLETATB_HITNORMAL|BULLETATB_MAGIC2|BULLETATB_RANGE1,
											0);

	g_BKI[KIND_BULLET_JAPANGHOST].Init(	"KIND_BULLET_JAPANGHOST",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_AUTA].Init(	"KIND_BULLET_AUTA",
									BULLETATB_HITNORMAL|BULLETATB_ARROW,
									0);

	g_BKI[KIND_BULLET_QUEENSUB1].Init(	"KIND_BULLET_QUEENSUB1",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_QUEENSUB2].Init(	"KIND_BULLET_QUEENSUB2",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_FIREATTACK].Init(	"KIND_BULLET_FIREATTACK",
										BULLETATB_HITNORMAL | BULLETATB_FIREATTACK,
										0);

	g_BKI[KIND_BULLET_RUNNINGFIRE].Init(	"KIND_BULLET_RUNNINGFIRE",
											BULLETATB_HITNORMAL | BULLETATB_RUNNINGFIRE,
											0);

	g_BKI[KIND_BULLET_PAPERDOLL].Init(	"KIND_BULLET_PAPERDOLL",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_PAPERDOLL);

	g_BKI[KIND_BULLET_NEEDLEATTACK].Init(	"KIND_BULLET_NEEDLEATTACK",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											0);

	g_BKI[KIND_BULLET_CLOUDWIND].Init(	"KIND_BULLET_CLOUDWIND",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										0);

	g_BKI[KIND_BULLET_GODSPEAR].Init(	"KIND_BULLET_GODSPEAR",
										BULLETATB_HITNORMAL|BULLETATB_MAGIC1,
										BULLETSMOKEATB_SMOKE_MAGIC8K_EXP);

	g_BKI[KIND_BULLET_SOULCHANGE].Init(	"KIND_BULLET_SOULCHANGE",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_SOULCHANGE);

	g_BKI[KIND_BULLET_MAGICCATTACK].Init(	"KIND_BULLET_MAGICCATTACK",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											0);

	g_BKI[KIND_BULLET_SUNBIYOUNATTACK].Init(	"KIND_BULLET_SUNBIYOUNATTACK",
												BULLETATB_HITNORMAL|BULLETATB_ARROW,
												0);

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 유명계/진시황릉 관련 START
//----------------------------------------------------------------------------

	// actdoll (2004/03/06 4:39) : 이무기 마법공격 등록
	g_BKI[KIND_BULLET_PYTHON].Init		(	"KIND_BULLET_PYTHON",
											BULLETATB_HITNORMAL | BULLETATB_ARROW,
											0);

	// actdoll (2004/03/06 4:39) : 진시황의혼 마법공격 등록
	g_BKI[KIND_BULLET_JINEMPEROR].Init	(	"KIND_BULLET_JINEMPEROR",
											BULLETATB_HITNORMAL | BULLETATB_ARROW,
											BULLETSMOKEATB_SMOKE_JINEMPEROR);

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : 추가 캐릭터 - 유명계/진시황릉 관련 END
//----------------------------------------------------------------------------

	g_BKI[KIND_BULLET_GUARDIAN_TIGER].Init(	"KIND_BULLET_GUARDIAN_TIGER",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											BULLETSMOKEATB_SMOKE_GAURDIAN_TIGER);

	g_BKI[KIND_BULLET_GUARDIAN_RABBIT].Init(	"KIND_BULLET_GUARDIAN_RABBIT",
												BULLETATB_HITNORMAL|BULLETATB_ARROW,
												BULLETSMOKEATB_SMOKE_GUARDIAN_RABBIT);

	g_BKI[KIND_BULLET_GUARDIAN_CHICK].Init(	"KIND_BULLET_GUARDIAN_CHICK",
											BULLETATB_HITNORMAL|BULLETATB_ARROW,
											BULLETSMOKEATB_SMOKE_GUARDIAN_CHICK);

//-----------------------------------------------------------------------------------
// robypark (2004/05/18 15:28) : 추가 캐릭터 사용 bullet - 장수 2차 전직 사용 bullet
//-----------------------------------------------------------------------------------
	// 조선-거북차; 파이어 볼
	g_BKI[KIND_BULLET_TURTLE_FIREBALL].Init(	"KIND_BULLET_TURTLE_FIREBALL",
												BULLETATB_FIREARROW | BULLETATB_HITNORMAL | BULLETATB_RANGE1, \
												0);

	// 조선-뇌전차; 번개 볼트
	g_BKI[KIND_BULLET_THUNDER_BOLT].Init(	"KIND_BULLET_THUNDER_BOLT",
											BULLETATB_HITNORMAL | BULLETATB_ARROW, \
											BULLETSMOKEATB_SMOKE_THUNDER_BOLT);

	// 일본-흑룡차; 독, 얼음 공격
	g_BKI[KIND_BULLET_POISON_ICE_BULLET].Init(	"KIND_BULLET_POISON_ICE_BULLET",
											BULLETATB_HITNORMAL | BULLETATB_POISONARROW | BULLETATB_RANGE1, \
											BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET);

	// 중국-발석거; 불붙은 돌덩이
	g_BKI[KIND_BULLET_CATAPULT_FIRE_STONE].Init(	"KIND_BULLET_CATAPULT_FIRE_STONE",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE);

	// robypark 2004-5-27, 17:42
	// 중국-불랑기포차; 불랑기포탄
	g_BKI[KIND_BULLET_FARANGI_BULLET].Init(	"KIND_BULLET_FARANGI_BULLET",
											BULLETATB_HITNORMAL|BULLETATB_CANNON2C|BULLETATB_WATER,
											BULLETSMOKEATB_SMOKE_EXP6);

	// 대만-화룡차; 화룡출수
	g_BKI[KIND_BULLET_FIRE_DRAGON_MISSILE].Init(	"KIND_BULLET_FIRE_DRAGON_MISSILE",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE);

	// 대만-봉황비조; 봉황비조 깃털 미사일
	g_BKI[KIND_BULLET_CHINESEPHOENIX_FEATHER].Init(	"KIND_BULLET_CHINESEPHOENIX_FEATHER",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_CHINESEPHOENIX_FEATHER);

	// 2차 장수 스킬 추가 관련
	// robypark 2004-6-1 15:34
	// 승천포 포탄(중국-발석거)
	g_BKI[KIND_BULLET_BOMB_OF_HEAVEN].Init(	"KIND_BULLET_BOMB_OF_HEAVEN",
											BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
											0);

	// 격노염폭 화산탄(일본-지진차)
	g_BKI[KIND_BULLET_VOLCANO_BULLET].Init(	"KIND_BULLET_VOLCANO_BULLET",
											BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
											BULLETSMOKEATB_SMOKE_VOLCANO_BULLET);

	
	// 대만-화룡차; 맹격화룡파 포탄
	g_BKI[KIND_BULLET_ONSLAUGHT_FIREBALL].Init(	"KIND_BULLET_ONSLAUGHT_FIREBALL",
												BULLETATB_RANGE1 | BULLETATB_HITNORMAL | BULLETATB_FIREARROW, \
												0);
	
	// robypark 2004-6-4 19:12
	// 불랑기포차 기술 앙천대소 총알
	g_BKI[KIND_BULLET_ROAR_OF_LAUGHTER_BOMB].Init(	"KIND_BULLET_ROAR_OF_LAUGHTER_BOMB",
													BULLETATB_RANGE1 | BULLETATB_HITNORMAL | BULLETATB_CANNON2C, \
													BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB);

	// robypark 2004-6-4 19:12
	// 불랑기포차 기술 앙천대소 공격시 파편
	g_BKI[KIND_BULLET_FRAGMENT4].Init(	"KIND_BULLET_FRAGMENT4",
										BULLETATB_FRAGMENT,
										0);

	// robypark 2004/7/14 16:46
	// 에비스 인형 공격 총알
	g_BKI[KIND_BULLET_DOLL_EVIS].Init(	"KIND_BULLET_DOLL_EVIS",
										BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
										0);

	/////////////////////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 14:23
	// 공성전 필드 총알 추가
	// 공성전 - 궁병 화살
	g_BKI[KIND_BULLET_SIEGEWARFARE_ARROW].Init(	"KIND_BULLET_SIEGEWARFARE_ARROW",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT);

	// 공성전 - 발석거 돌
	g_BKI[KIND_BULLET_SIEGEWARFARE_STONE].Init(	"KIND_BULLET_SIEGEWARFARE_STONE",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT);
	/////////////////////////////////////////////////////////////////////////////////

	// robypark 2005/2/2 17:27
	// 달마 인형 공격 총알
	g_BKI[KIND_BULLET_DOLL_DHARMA].Init(	"KIND_BULLET_DOLL_DHARMA",
										BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
										0);
}

////////////////////////////////////////////////////////////////
///  Bullet & Smoke Class
///
///  1999. 1. 28
////////////////////////////////////////////////////////////////
extern unsigned char SqrtVary[];

////////////////////////////////////////////////////////////////
//   _Bullet
//
//	 id = 총알종류  x, y = 시작지점(도트단위)
//
//   mx, my = 언덕때문에 생기는 오차때문에 추가한 인자
//
//   dx, dy = 도착지점(도트단위)
//
//   shooter_id = 쏜아이 targetid = 맞는아이 
//
//   apower = 공격력 -> 쏜아이가 죽으면 정보가 없어지기 때문
/////////////////////////////////////////////////////////////////
_Bullet::_Bullet()
{
	ZeroMemory(this, sizeof(_Bullet));

	// robypark (2004/5/21 18:44) 화룡출수, 발석거 불 붙은 돌 애니메이션 보간을 위한 추가
	//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	//		따라 프레임 스킵이 필요하다.
	ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);
}


// void _Bullet::Init(...)
// SHORT kind: bullet 종류
// SHORT id: Bullet ID
// SHORT sx: Bullet 시작 좌표(X)
// SHORT sy: Bullet 시작 좌표(Y)
// SHORT mx: Bullet 끝 좌표(X)
// SHORT my: Bullet 끝 좌표(Y)
// SHORT x: 공격자의 위치(x)
// SHORT y: 공격자의 위치(y)
// SHORT dx: 목표의 위치(x)
// SHORT dy: 목표의 위치(y)
// _NewID shooter_id: 공격자 ID
// _NewID targetid: 목표물 ID
// SHORT apower: 공격력
// SI32 hitrate: HIT Rate(100)
// SHORT targetdir: 목표물 방향(0)
// SHORT attack_area: 공격지역 (AREA_LAND, AREA_AIR)
void _Bullet::Init(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooter_id, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetdir, SHORT attack_area)
{
	SHORT i=0;

	ZeroMemory(this, sizeof(_Bullet));

	m_ssKind=kind;
	m_scArea=(char)attack_area;
    m_ssCurrentOrder=ORDER_BULLET_MOVE;


	m_ssID=id;
	m_ssRealX=x;
	m_ssRealY=y;  
	m_ssRealDX=dx;
	m_ssRealDY=dy;
	m_ssOriginalRealDX=dx;
	m_ssOriginalRealDY=dy;

	if(clifMap.IsInMapArea(sx, sy)==TRUE)
	{
	   m_ssMapSX = sx; m_ssMapSY = sy;
	}
	else
	{
		Error("12974", "kind:%d sx:%d dy:%d", kind, sx, sy);
	}

	if(clifMap.IsInMapArea(mx, my)==TRUE)
	{
	   m_ssMapDX = mx; m_ssMapDY = my;
	}
	else
	{
		Error("12fds974", "kind:%d mx:%d my:%d", kind, mx, my);
	}

	m_ssCurveBulletRealX=x;
	m_ssCurveBulletRealY=y;
	m_ssOldCurveBulletRealX=x;
	m_ssOldCurveBulletRealY=y;
	m_ssCurveBulletDrawX=0;
	m_ssCurveBulletDrawY=0;

	m_siTargetDirection=targetdir;

	m_ssCurveRate=5;

	DecideDirection(m_ssRealX, m_ssRealY, m_ssRealDX, m_ssRealDY);

	m_ssX=0;
	m_ssY=0;
	m_ShooterID=shooter_id;

	m_ssDrawSwitch=TRUE;

	m_AttackID=targetid;

	if(IsAlive(shooter_id.ID))
	     m_ssShooterPlayer=clifChar.GetPlayerNumber(shooter_id.ID);
	else m_ssShooterPlayer=-1;

	m_ssApower=apower;
	m_ssHitRate=hitrate;
	m_ssMoveStep=0;m_ssMaxMoveStep=0;
	m_uiSkillTime = timeGetTime();
	m_ssReverseSwitch=FALSE;

	m_ssType=0;
	m_ssHitIDIndex=0;
	m_ssTransparentSwitch=TRUE;
	m_ssChaseSwitch=FALSE;
	m_ssChaseRepeatNumber=0;
	m_ssVaryX=0;
	m_ssVaryY=0;

    for(i=0;i<MAX_HIT_ID;i++)
	   m_HitID[i].Init();

	m_ssStepWidth=g_BKI[m_ssKind].m_ssStepWidth;
	m_ssFile=g_BKI[m_ssKind].m_ssFile;
	m_ssFont=g_BKI[m_ssKind].m_ssFont;
	m_ssTailSwitch=g_BKI[m_ssKind].m_ssTailSwitch;
	m_ssChaseSwitch=g_BKI[m_ssKind].m_ssChaseSwitch;
	m_ssCurveSwitch=g_BKI[m_ssKind].m_ssCurveSwitch;
	m_ssTransparentSwitch=g_BKI[m_ssKind].m_ssTransparentSwitch;
	m_ssHeightSwitch=g_BKI[m_ssKind].m_ssHeightSwitch;

	 GetCourse();

	 SetFileNFont();

	 // robypark (2004/5/21 18:44) 화룡출수, 발석거의 불 붙은 돌, 애니메이션 보간을 위한 추가
	 //		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
	 //		따라 프레임 스킵이 필요하다.
	 switch (m_ssKind)
	 {
		// 화룡출수
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// 발석거 기본 공격(불 붙은 돌)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// 발석거 승천포 포탄
		case KIND_BULLET_BOMB_OF_HEAVEN:
			// robypark (2004/5/21 18:44) 애니메이션 보간을 위한 추가
			//		단 한번의 애니메이션이 이루어져야 하며, 거리에
			//		따라 프레임 스킵이 필요하다.
			ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);

			// 거리에 맞게 프레임 조절
			CreateFrameIndex_4FireDragonMissile();
			break;

		default:
			break;
	 }

	 // 불랑기포를 위한 변수
	 m_snCurrentDelay = 0;
	 m_snMaxDelay = 0;

	// robypark (2004/6/12 12:27
	// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
	// 설정해준다.
	m_bLayerAboveVolcano = false;
}

// robypark (2004/5/21 18:44) 화룡출수, 발석거의 불 붙은 돌 애니메이션 보간을 위한 추가
//		화룡출수의 경우 단 한번의 애니메이션이 이루어져야 하며, 거리에
//		따라 프레임 스킵이 필요하다.
//  이동 단계에 따른 애니메이션 프레임을 설정한다.
void _Bullet::CreateFrameIndex_4FireDragonMissile(void)
{
	// 짧은 거리의 경우
	if (m_ssMaxMoveStep <= 2)
	{
		m_snFrameIndex[0] = 0;
		m_snFrameIndex[1] = m_scMoveFrameNumber / 2;
		m_snFrameIndex[2] = m_scMoveFrameNumber - 1;
		return;
	}

	m_snFrameIndex[0] = 0;
	m_snFrameIndex[m_ssMaxMoveStep] = m_scMoveFrameNumber - 1;

	// 한 프레임(폰트) 당 이동 단계 수
	int nStepPerFrame = (((float)m_ssMaxMoveStep / (float)m_scMoveFrameNumber) * 100);

	// 적절한 프레임을 배분해 준다.
	for (SHORT i = 1; i < m_ssMaxMoveStep; i++)
	{
		m_snFrameIndex[i] = (i * 100) / nStepPerFrame;
	}
}

// robypark (2005/5/27 19:9)
// 불랑기포차의 3발 포탄 시차 적용
void _Bullet::SetLuanchDelay(SHORT snMaxDelay)
{
	m_snMaxDelay = snMaxDelay;
}

void _Bullet::SetFileNFont()
{
	int i;

	m_scRealImageXPercent=100;
	m_scRealImageYPercent=100;
	m_ssRealImageStartX=(GetFileXsize(m_ssFile) - GetFileXsize(m_ssFile)*m_scRealImageXPercent/100)/2;
	m_ssRealImageEndX=   GetFileXsize(m_ssFile) - m_ssRealImageStartX;
	m_ssRealImageStartY=(GetFileYsize(m_ssFile) - GetFileYsize(m_ssFile)*m_scRealImageYPercent/100)/2;
	m_ssRealImageEndY=   GetFileYsize(m_ssFile) - m_ssRealImageStartY;

	// 이동용 화일과 폰트 
	m_scMoveFrameNumber = g_BKI[m_ssKind].m_siMoveFrameNumber;


	// 각 방향별로 그림이 준비되어 있다. 
	if(g_BKI[m_ssKind].m_bDirSwitch == TRUE)
	{
		SetMoveImage16(g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont, m_scMoveFrameNumber);
	}
	// 모든 방향이 같은 그림을 사용한다. 
	else
	{
		for(i=0;i<9;i++)
			SetMoveImage(i, g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont);
	}

}

BOOL _Bullet::Hit()
{


	// 고저차에 의한 효과가 있는 총알이면 높이 차이에 따라 10%의 배율로 공격력을 높여준다. 
	if(m_ssHeightSwitch==TRUE)
	{
		// 발사지점의 높이를구한다. 
	    SHORT startlevel = GD.LastLevel[m_ssMapSX][m_ssMapSY];

		// 명중지점의 높이를 구한다. 
		SHORT endlevel = GD.LastLevel[m_ssMapDX][m_ssMapDY];

		// 높이 차에 의한 공격력을 보정해준다. 
		if(startlevel>endlevel)
		{
			m_ssApower=m_ssApower+m_ssApower*(startlevel-endlevel)/10;
		}

	}

	if(g_BKI[m_ssKind].m_siAtb & BULLETATB_HITNORMAL)
	{
		return HitNormal();
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FRAGMENT)
	{
		return HitFragment1();
	}


	return FALSE;

}

BOOL _Bullet::HitFragment1()
{
	SHORT id;

	// 명중한 화염을 그린다. 
	id=FindEmptySmoke();

	// 좌표를 구한다. 
	if(clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssMapDX, &m_ssMapDY)==TRUE)
	{
	   if(id)
	   {
		// 물에 떨어지면, 
		if(clifMap.IsColl(m_ssMapDX, m_ssMapDY, BLOCK_DEEPWATER))
		{
			SetSmoke(KIND_SMOKE_WATEREXP, id, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else
		{
		   if(id & 1)
		      SetSmoke(KIND_SMOKE_EXP4, id, m_ssRealX, m_ssRealY, AREA_LAND);
		   else 
		      SetSmoke(KIND_SMOKE_SMALLEXP2, id, m_ssRealX, m_ssRealY, AREA_LAND);
		}
	   }
	}
	return TRUE;
}

BOOL _Bullet::HitMagicBall()
{

	SHORT mapx=0, mapy=0;

	SHORT smokeid=FindEmptySmoke();
	if(smokeid)
	{
		// 공격력이 높아지면, 
		if(m_ssApower > 70)
		{
			SetSmoke(KIND_SMOKE_EXP9, smokeid, m_ssRealX, m_ssRealY, m_scArea);
			
			CalculateHit(m_ShooterID, m_ssShooterPlayer, ATTACKATB_CANNON, 1, m_scArea, m_ssApower, m_AttackID, m_ssMapDX, m_ssMapDY, m_ssHitRate);
			
		}
		else
		{
			SetSmoke(KIND_SMOKE_EXP10, smokeid, m_ssRealX, m_ssRealY, m_scArea);
			CalculateHit(m_ShooterID, m_ssShooterPlayer, ATTACKATB_MAGIC, 1, m_scArea, m_ssApower, m_AttackID, m_ssMapDX, m_ssMapDY, m_ssHitRate);
			
		}
	}
	


	return TRUE;
}



BOOL _Bullet::HitNormal()
{
	SHORT mapx=0, mapy=0;
	SI32 attackatb = 0;
	SI32 range = 0;
	SI16 smokeid;

	// 효과음 처리 
	if(g_BKI[m_ssKind].m_siHitEffectSoundIdx)	// 효과음이 있다면
	{
		// 연주. Hash Table 이용한 연주
		HashCodePushEffect(g_BKI[m_ssKind].m_siHitEffectSoundIdx, m_ssX, m_ssY);
	}

	// 명중한 화염을 그린다. 
	smokeid=FindEmptySmoke();

	// 물에 떨어지면, 
	if(g_BKI[m_ssKind].m_siAtb & BULLETATB_WATER && m_scArea==AREA_LAND && clifMap.IsColl(m_ssMapDX, m_ssMapDY, BLOCK_DEEPWATER) && clifMap.GetIDMap(m_ssMapDX, m_ssMapDY)==0 )
	{
		
		SetSmoke(KIND_SMOKE_WATEREXP, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
	}
	else
	{
		// 연기 속성. 
		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SOULCHANGE)
		{
			// robypark 2004/9/13 13:32
			// 세이메이 기본공격 히트 효과 추가
			// 폭뢰격 스모크 이펙트 사용
			switch(GD.Random() % 3)
			{
				case 0:
					SetSmoke(KIND_SMOKE_KIMBOMB1, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);	
					break;
				case 1:
					SetSmoke(KIND_SMOKE_KIMBOMB2, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);	
					break;
				default:
					SetSmoke(KIND_SMOKE_KIMBOMB3, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);	
					break;
			}
		}
		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_GUARDIAN_RABBIT)
		{
			CR[m_ShooterID.ID].MagicGuardianRabbit(m_AttackID);
			return TRUE;
		}

		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_GUARDIAN_CHICK)
		{
			CR[m_ShooterID.ID].MagicGuardianChicken(m_AttackID);
			return TRUE;
		}

		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_GAURDIAN_TIGER)
		{
			//CR[AttackID.ID].m_ParmeterBox.IP.SetLife(CR[AttackID.ID].m_ParmeterBox.IP.GetLife() - CR[AttackID.ID].PB.IP.GetMa-xLifeByIP()/2);

			// 수호부[인] 공격에 맞은 smoke그려주기
			SetSmoke(KIND_SMOKE_GUARDIAN_TIGER,smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
			PushEffect(EFFECT_GUARDIAN_TIGER_END, m_ssX, m_ssY);

			// robypark 2004/6/23 16:28
			// 명중률을 근거로 타격 여부를 결정한다.
			// 수호부[인]의 공격에 명중률 적용이 되도록 수정.
			if ((GD.Random() % 100) > (UI32)m_ssHitRate)
				return TRUE;	// 적에게 수호부[인]의 공격이 명중하지 않았다.

			if(CR[m_AttackID.ID].ReduceLife(CR[m_AttackID.ID].m_ParmeterBox.IP.GetMaxLifeByIP()/2,10) == FALSE)
			{
				//수호부[인]을 맞고 캐릭터가 죽었을 경우.
				// 캐릭터 정보의 변경을 통보한다. 
				if(OBInfo.IsSingleMode == TRUE)
				{
					// 서버에 내가 누군가를 죽였음을 통보한다. 
					if(IsAlive(m_ShooterID))
					{
						// 무도장이 아니어야 한다. 
						if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
						{
							OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
								MyNet.PlayersdpId.dpId[ CR[m_ShooterID.ID].GetPlayerNumber()],CR[m_ShooterID.ID].m_siOnlineID, 
								MyNet.PlayersdpId.dpId[ CR[m_AttackID.ID].GetPlayerNumber()],	CR[m_AttackID.ID].m_siOnlineID);
							
							// 킬러에게 합당한 경험치등을 제공한다. 
							pGame->pBattle->ExpOperate(m_ShooterID.ID, m_AttackID.ID);		
						}
					}
				}
				// robypark 2004/7/9 수호부[인]을 가진 캐릭터들이 파티를 맺고 전투시 수호부[인] 기술 효과가
				// 제대로 발동되지 않았다. 이는 이 else문의 위치를 if(IsAlive(ShooterID))문장의 else문으로 
				// 잘못 붙여넣기를 하여 생긴 문제였다.
				else
				{
					pGame->pBattle->PushUpdateCharInfo(m_AttackID.ID, m_ShooterID.ID);
				}
			}
		}

/*		if(g_BKI[Kind].siSmokeAtb & BULLETSMOKEATB_SMOKE_RABBIT)
		{
			if(CR[AttackID.ID].m_bRabbitAttack == false)
			{
				SI32 siWidth = CR[AttackID.ID].CharDrawInfo.GetRealImageRect().right - CR[AttackID.ID].CharDrawInfo.GetRealImageRect().left;
				SI32 siHeight = CR[AttackID.ID].CharDrawInfo.GetRealImageRect().bottom - CR[AttackID.ID].CharDrawInfo.GetRealImageRect().top;

				//공격 당하하였는가의 여부.
				CR[AttackID.ID].m_bRabbitAttack   = true;
				//공격한 사람(수호부 토끼)의 주인 아이디를 저장한다.
				CR[AttackID.ID].m_RabbitShooterId = CR[ShooterID.ID].HostID.ID;

				//공격딜레이와 무브딜레이를 줄인다.
				CR[AttackID.ID].m_MoveDelay   = 0.5;
				CR[AttackID.ID].m_AttackDelay = 2;

				SetSmoke(KIND_SMOKE_RABBIT, smokeid, RealX+siWidth/3, RealY, AREA_LAND);
				Smoke_AttackID(smokeid, AttackID);
				Smoke_SetYIndent(KIND_SMOKE_RABBIT,siHeight*2/3);
			}

			return TRUE;
		}
*/

		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_GHOST)
		{
			if(GD.Random() % 4 == 0)
			{
				if(CR[m_ShooterID.ID].m_ParmeterBox.IP.GetMana() < MAGICPOWER_GHOST)		return TRUE;

				SI32 siWidth = CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().right - CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().left;
				SI32 siHeight = CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().bottom - CR[m_AttackID.ID].m_cltCharDrawInfo.GetRealImageRect().top;

				CR[m_AttackID.ID].GhostTime = timeGetTime();
				SetSmoke(KIND_SMOKE_GHOST, smokeid, m_ssRealX+siWidth/3, m_ssRealY, AREA_LAND);
				Smoke_AttackID(smokeid, m_AttackID);
				Smoke_SetYIndent(KIND_SMOKE_GHOST,siHeight*2/3);
				CR[m_ShooterID.ID].ReduceMana(MAGICPOWER_GHOST);
			}
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_PAPERDOLL)
		{
			SetSmoke(KIND_SMOKE_PAPERDOLL_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP0)
		{
			SetSmoke(KIND_SMOKE_EXP0, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP6)
		{
			SetSmoke(KIND_SMOKE_EXP6, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP7)
		{
			if(GD.Random()%2)
			   SetSmoke(KIND_SMOKE_EXP7, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
			else
			   SetSmoke(KIND_SMOKE_EXP8, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP9)
		{
            SetSmoke(KIND_SMOKE_EXP9, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);			
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP10)
		{
            SetSmoke(KIND_SMOKE_EXP10, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);			
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_ADVTOWERKEXP1)
		{
            SetSmoke(KIND_SMOKE_ADVTOWERKEXP1, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);			

			SI32 tempid = FindEmptySmoke();
			if(tempid)
				SetSmoke(KIND_SMOKE_ADVTOWERKEXP2, tempid, m_ssRealX, m_ssRealY, AREA_LAND);			
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP12)
		{
			switch(GD.Random()%2)
			{
			case 0:
				SetSmoke(KIND_SMOKE_EXP12, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
				break;
			case 1:
				SetSmoke(KIND_SMOKE_EXP13, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
				break;
			}
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP14)
		{
			SetSmoke(KIND_SMOKE_EXP14, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}

		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_EXP15)
		{
			SetSmoke(KIND_SMOKE_EXP15, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_MAGIC8K_EXP)
		{
			SetSmoke(KIND_SMOKE_MAGIC8K_EXP, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if( g_BKI[m_ssKind].m_siAtb & BULLETSMOKEATB_SMOKE_GOLDDRAGONELECTRONEXP)
		{
			SetSmoke(KIND_SMOKE_GOLDDRAGONELECTRONEXP, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_GHOSTCATEXP)
		{
			SetSmoke(KIND_SMOKE_GHOSTCATEXP, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_3)
		{
			SetSmoke(KIND_SMOKE_SMOKE3, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}
		
		// actdoll (2004/03/06 10:03) : 유명계/진시황릉 관련
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_JINEMPEROR)		// 진시황의혼 마법공격 히트
		{
			SetSmoke(KIND_SMOKE_JINEMPEROR_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}

		// robypark (2004/5/19, 13:32) : 굉뇌(조선-뇌전차, 허준 2차 전직)에 맞았을 경우
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_THUNDER_BOLT)
		{
			// 뇌전차 공격에 맞았을 경우의 히트 효과를 보여준다.
			SetSmoke(KIND_SMOKE_THUNDER_BOLT_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}

		// robypark (2004/5/19, 16:50) : 화룡출수(대만-화룡차, 손유창 2차 전직)에 맞았을 경우
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE)
		{
			SetSmoke(KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}		

		// robypark (2004/5/19, 16:50) : 독, 냉기(일본-흑룡차, 세이메이 2차 전직)에 맞았을 경우
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET)
		{
			SetSmoke(KIND_SMOKE_POISON_ICE_BULLET_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}		

		// robypark (2004/5/21, 16:15) : 발석거 공격-불붙은 돌(중국-발석거, 순비연 2차 전직)에 맞았을 경우
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE)
		{
		   SetSmoke(KIND_SMOKE_EXP7, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
/*		   
		   if (GD.Random() & 3)
		   {
				smokeid = FindEmptySmoke();
				if (smokeid
*/
		}
		// robypark 2004/6/1 17:47 : 격노염폭 화산탄 히트 효과(일본-지진차)
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_VOLCANO_BULLET)
		{
			SetSmoke(KIND_SMOKE_VOLCANO_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND, -1, TRUE);
		}
		// robypark 2004/6/4 19:19 : 앙천대소 폭탄 히트 효과
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB)
		{
			SetSmoke(KIND_SMOKE_ROAR_OF_LAUGHTER_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);

			// 파편을 생성한다.
			CreateFragment();
		}
	}

	if( g_BKI[m_ssKind].m_siAtb & BULLETATB_MAKEFIRE )
	{
		_NewID zeroID;
		zeroID.ID = -1;
		zeroID.Unique = 0;
		clifMap.PutFire(m_ssMapDX, m_ssMapDY, 1, GROUNDFIRE_LIFETIME/2,zeroID);
	}
	// 맞았을 때 잠시 서있게 되는 속성이면, 
	if( g_BKI[m_ssKind].m_siAtb & BULLETATB_HALT )
	{
//		if(GD.CharInfo[AttackID.ID] != KIND_ON_AGILETIGER)
//		{
			if(IsAlive(m_AttackID) && IsFriendToByPlayerNumber(m_ssShooterPlayer, clifChar.GetPlayerNumber(m_AttackID.ID) ) == FALSE)
			{
				CR[m_AttackID.ID].SetHalt(HALT_REASON_IVY, 15);
				OrderStop(CR[m_AttackID.ID].m_NewID, BY_PLAYER);
			}
//		}
	}


	if( g_BKI[m_ssKind].m_siAtb & BULLETATB_ARROW )
	{
		attackatb = ATTACKATB_ARROW;
	}
	else if( g_BKI[m_ssKind].m_siAtb & BULLETATB_ONGHWA)
	{
		attackatb = ATTACKATB_ONGHWA;
	}
	else if( g_BKI[m_ssKind].m_siAtb & BULLETATB_AIRATTACKUPARROW )
	{
		attackatb = ATTACKATB_AIRATTACKUPARROW;
	}
	// 불화살인 경우, 
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FIREARROW)
	{
		attackatb = ATTACKATB_FIREARROW;

		if(GD.Random() % 3 == 0)
		{
			_NewID zeroID;
			zeroID.ID = -1;
			zeroID.Unique = 0;
			// 땅바닥에 불을 붙여준다. 
			clifMap.PutFire(m_ssX, m_ssY, GD.Random()%2+1, GROUNDFIRE_LIFETIME / 2 , m_ShooterID, m_ssShooterPlayer);
//			clifMap.PutFire(MapDX, MapDY, GD.Random()%2+1, GROUNDFIRE_LIFETIME / 2 ,ShooterID,ShooterPlayer);
		}

	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_NEEDLE)
	{
		attackatb = ATTACKATB_NEEDLE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FURHEADDEMON)
	{
		attackatb = ATTACKATB_FURHEADDEMON;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_GUN)
	{
		attackatb = ATTACKATB_GUN;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_STRONGGUN)
	{
		attackatb = ATTACKATB_STRONGGUN;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_MAGIC1)
	{
		attackatb = ATTACKATB_MAGIC;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_MAGIC2)
	{
		attackatb = ATTACKATB_MAGIC2;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_MAGIC3)
	{
		attackatb = ATTACKATB_MAGIC3;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FIRE)
	{
		attackatb = ATTACKATB_FIRE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_AIRMISSILE)
	{
		attackatb = ATTACKATB_AIRMISSILE;
	}
//	else if(g_BKI[m_ssKind].siAtb & BULLETATB_SAMMISSILE)
//	{
//		attackatb = ATTACKATB_SAMMISSILE;
//	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_CANNON)
	{
		attackatb = ATTACKATB_CANNON;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_UPGRADETOWERCANNON)
	{
		attackatb = ATTACKATB_UPGRADETOWERCANNON;
	}
/*	else if(g_BKI[Kind].siAtb & BULLETATB_FIRECAR)
	{
		attackatb = ATTACKATB_FIRECAR;
	}
*/
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_CANNON2C)
	{
		attackatb = ATTACKATB_CANNON2C;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_BUTTERFLY)
	{
		attackatb = ATTACKATB_BUTTERFLY;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_WIDOW)
	{
		attackatb = ATTACKATB_BUTTERFLY;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_SEAHORSE)
	{
		attackatb = ATTACKATB_SEAHORSE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_POISONARROW)
	{
		attackatb = ATTACKATB_POISONARROW;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_MAGIC6C)
	{
		 // 마법에 의해서 레벨이 오르지 않도록 ZeroID를 넣는다. 
		 SetMagic6c(m_ssMapDX, m_ssMapDY, ZeroID, m_ssShooterPlayer, AREA_LAND, m_ssApower, m_ssHitRate, ZeroID);
		 return TRUE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FIREATTACK)
	{
		SetMagicFireAttack(m_ssMapDX, m_ssMapDY, m_ShooterID, m_ssShooterPlayer, AREA_LAND, m_ssApower, m_ssHitRate, m_AttackID);
		return TRUE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_RUNNINGFIRE)
	{
		SetMagicRunningFire(m_ssMapDX, m_ssMapDY, m_ShooterID, m_ssShooterPlayer, AREA_LAND, m_ssApower, m_ssHitRate, m_AttackID);
		return TRUE;
	}
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FIREENERGY)
	{
		SetMagicFireEnergyPa(m_ssMapDX, m_ssMapDY, m_ShooterID, m_ssShooterPlayer, AREA_LAND, m_ssApower, m_ssHitRate, m_AttackID);
		return TRUE;
	}
/*	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_SOULCHANGE)
	{
		SetMagicSoulChange(m_ssMapDX, m_ssMapDY, m_ShooterID, m_ssShooterPlayer, AREA_LAND, m_ssApower, m_ssHitRate, m_AttackID);
		return TRUE;
	}
*/
	else 
	{
		Error("!21432534", "fsdf39u2:[%d][%d]", m_ssKind, g_BKI[m_ssKind].m_siAtb);
		attackatb = 0;
	}


	if(g_BKI[m_ssKind].m_siAtb & BULLETATB_RANGE1)
	{
		range = 1;
	}
	else
	{
		range = 0;
	}

	// robypark 2004/6/10 17:13
	// 2차 장수에 맞도록 공격속성을 설정한다.
	switch (m_ssKind)
	{
		// 조선-거북차; 파이어 볼
		case KIND_BULLET_TURTLE_FIREBALL:
			attackatb = ATTACKATB_TURTLE_VEHICLE;	// 물리, 스플래쉬 존
			break;

		// 조선-뇌전차; 번개 볼트
		case KIND_BULLET_THUNDER_BOLT:
			attackatb = ATTACKATB_THUNDER_VEHICLE;	// 물리

			// robypark 2004/6/11 10:41
			// 굉뇌 번개는 마법데미지를 준다. 뇌전차 번개 기본공격(물리데미지)과 달리
			if (m_ssCurrentOrder == ORDER_BULLET_STRAIGHT)
				attackatb = ATTACKATB_MAGIC;	// 마법
			break;

		// 일본-흑룡차; 독, 얼음 공격
		case KIND_BULLET_POISON_ICE_BULLET:
			attackatb = ATTACKATB_BLACK_DRAGON_VEHICLE;	// 물리, 스플래쉬 존, 독, 냉기 공격
			break;

		// 중국-발석거; 불붙은 돌덩이
		case KIND_BULLET_CATAPULT_FIRE_STONE:
			attackatb = ATTACKATB_CATAPULT;	// 물리, 스플래쉬 존
			break;

		// 중국-불랑기포차; 불랑기포탄
		case KIND_BULLET_FARANGI_BULLET:
			attackatb = ATTACKATB_FARANGI_VEHICLE;	// 물리
			break;

		// 대만-화룡차; 화룡출수
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
			attackatb = ATTACKATB_FIRE_DRAGON_VEHICLE;	// 물리, 스프래쉬 존
			break;

		// robypark 2004/9/1 16:58 독전갈 미사일 원거리 공격에 대한 공격속성 추가
		case KIND_BULLET_SCORPIONBULLET:
		// 대만-화룡차; 맹격화룡파 포탄
		case KIND_BULLET_ONSLAUGHT_FIREBALL:
			attackatb = ATTACKATB_FIREATTACK;	// 마법, 스플래쉬 존
			break;

		// 대만-봉황비조; 봉황비조 깃털 미사일
		case KIND_BULLET_CHINESEPHOENIX_FEATHER:
			attackatb = ATTACKATB_CHINESEPHOENIX;	// 물리, 스플래쉬 존
			break;

		// robypark 2004/6/11 10:46
		// 격노염폭 화산탄(일본-지진차)은 마법데미지를 준다.
		case KIND_BULLET_VOLCANO_BULLET:
			attackatb = ATTACKATB_FIREATTACK;	// 마법, 스플래쉬 존
			break;

		// 불랑기포차 기술 앙천대소 총알
		case KIND_BULLET_ROAR_OF_LAUGHTER_BOMB:
			attackatb = ATTACKATB_ROAR_OF_LAUGHTER;	// 물리, 스플래쉬 존

			// 앙천대소 총알 스플래쉬 범위
			range = 2;
			break;

		default:
			break;
	}

	// 맹격화룡파를 위한 체크
	// 피어싱 속성의 총알의 경우 총알이 이동하는 지점에서 충돌체크를 항상한다.
	if (m_ssCurrentOrder == ORDER_BULLET_PIERCE)
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssX, m_ssY, m_ssHitRate);
	// 굉뇌를 위한 데미지 체크
	else if (m_ssCurrentOrder == ORDER_BULLET_STRAIGHT)
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssX, m_ssY, m_ssHitRate);
	else
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssMapDX, m_ssMapDY, m_ssHitRate);

	return TRUE;
}


//
// 총알의 종류를 넣는다.
//

void _Bullet::SetInfo()
{
	GD.BulletInfo[m_ssID]=m_ssKind;
}

//
// 한번 공격한 캐릭터를 다시 공격하지 않기 위해서.
// 스택을 초기화한다. 
//

void _Bullet::InitPushID()
{
	m_ssHitIDIndex=0;
}

//
// id = 총알의 id
// 
//

void _Bullet::PushHitID(_NewID id)
{

	m_HitID[m_ssHitIDIndex]=id;

	if(m_ssHitIDIndex<(MAX_HIT_ID-1))
	   m_ssHitIDIndex++;
}

//
// id = 총알의 id
// 총알이 목표에 맞았나?
//

BOOL _Bullet::IsAlreadyPushID(_NewID id)
{
	int i=0;

	for(i=0;i<m_ssHitIDIndex;i++)
	{
		if(m_HitID[i].IsSame(id) )
			return TRUE;
	}

	return FALSE;
}

//
// startstep = 
// 가는길을 설정(곡선)
//

void _Bullet::GetCourseCurve(SHORT startstep)
{
	SHORT quarter=0;
	double dx, dy;
	SHORT x=0, y=0;
	SHORT i;

	m_ssCourseX[startstep]=m_ssX;
	m_ssCourseY[startstep]=m_ssY;
	startstep++;

	double varya, varyb;

	dx=(double)(m_ssRealDX-m_ssRealX);
	dy=(double)(m_ssRealY-m_ssRealDY);

	// decide 1/4
	if(m_ssRealDX>m_ssRealX && m_ssRealDY<m_ssRealY)
	{
	  quarter=1;
	}
	else if(m_ssRealDX<m_ssRealX && m_ssRealDY<m_ssRealY)
	{
	  quarter=2;
	}
	else if(m_ssRealDX<m_ssRealX && m_ssRealDY>m_ssRealY)
	{
	  quarter=3;
	}
	else if(m_ssRealDX>m_ssRealX && m_ssRealDY>m_ssRealY)
	{
	  quarter=4;
	}
	else if(m_ssRealDX==m_ssRealX && m_ssRealDY>=m_ssRealY){quarter=5;}
	else if(m_ssRealDX==m_ssRealX && m_ssRealDY<m_ssRealY){quarter=6;}
	else if(m_ssRealDX>=m_ssRealX && m_ssRealDY==m_ssRealY){quarter=7;}
	else if(m_ssRealDX<m_ssRealX && m_ssRealDY==m_ssRealY){quarter=8;}

	switch(quarter)
	{
	  case 1:   // y=ax*x, x=by*y;
		 varya=dy/(dx*dx);
		 varyb=dx/(dy*dy);
		 if(varya>varyb)
		 {
			for(i=0;i<=dy;i+=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(varyb*i*i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 else
		 {
			for(i=0;i<=dx;i+=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-varya*i*i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 break;
	  case 2:   // y=ax*x, x=-by*y
		 varya=dy/(dx*dx);
		 varyb=-dx/(dy*dy);
		 if(varya>varyb)
		 {
			 for(i=0;i<=dy;i+=m_ssStepWidth)
			 {
			   m_ssCourseX[startstep]=(SHORT)(-varyb*i*i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			 }
		 }
		 else
		 {
			for(i=0;i>=dx;i-=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-varya*i*i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 break;
	  case 3:  // y=-ax*x, x=-by*y;
		 varya=dy/(dx*dx);
		 varyb=dx/(dy*dy);


		 if(varya<varyb)
		 {
			for(i=0;i>=dy;i-=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(varyb*i*i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 else
		 {
			for(i=0;i>=dx;i-=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-varya*i*i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 break;
	  case 4: // y=-ax*x, x=by*y
		 varya=-dy/(dx*dx);
		 varyb=dx/(dy*dy);

		 if(varya>varyb)
		 {
			for(i=0;i>=dy;i-=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(varyb*i*i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(-i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 else
		 {
			for(i=0;i<=dx;i+=m_ssStepWidth)
			{
			   m_ssCourseX[startstep]=(SHORT)(i+m_ssRealX);
			   m_ssCourseY[startstep]=(SHORT)(varya*i*i+m_ssRealY);
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
			}
		 }
		 break;
	  case 5:  // down y axis
		 for(i=0;i>=dy;i-=m_ssStepWidth)
		 {
		   m_ssCourseX[startstep]=m_ssRealX;
		   m_ssCourseY[startstep]=-i+m_ssRealY;
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
		 }
		 break;
	  case 6: //up y axis
		 for(i=0;i<=dy;i+=m_ssStepWidth)
		 {
		   m_ssCourseX[startstep]=m_ssRealX;
		   m_ssCourseY[startstep]=-i+m_ssRealY;
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
		 }
		 break;
	  case 7:  // right x axis
		for(i=0;i<=dx;i+=m_ssStepWidth)
		{
		  m_ssCourseX[startstep]=i+m_ssRealX;
		  m_ssCourseY[startstep]=m_ssRealY;
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
		}
		break;
	  case 8:  // left x axis
		startstep=0;
		for(i=0;i>=dx;i-=m_ssStepWidth)
		{
		  m_ssCourseX[startstep]=i+m_ssRealX;
		  m_ssCourseY[startstep]=m_ssRealY;
				if(startstep<MAX_SHOOT_RANGE)
				   startstep++;
		}
		break;
	}

	m_ssMaxMoveStep=startstep-1;
	
	if(m_ssMaxMoveStep==0)
	{
		Error("bullet.cpp", "maxmovestep:%d", m_ssMaxMoveStep);
	}

}
//
// 가는길을 설정(직선)  .     .     .  -> 이렇게...
//

void _Bullet::GetCourse()
{
	SHORT x1=m_ssRealX,  y1=m_ssRealY;
	SHORT x2=m_ssRealDX, y2=m_ssRealDY;
	SHORT x, y;
	SHORT delta_x, delta_y, half, error=0;
	SHORT step=0;
	SHORT index=0;

	if(m_ssKind==KIND_BULLET_GODSPEAR)
	{
		if(x2>x1 && y1>y2 || x2>x1 && y1<y2)			// 1, 4사분면
		{
			y2 += (float)(y2-y1) / (x2-x1) * 60;
			x2 += 60;
		}
		else if(x2<x1 && y1>y2 || x2<x1 && y1<y2)		// 2, 3사분면
		{
			y2 -= (float)(y2-y1) / (x2-x1) * 60;
			x2 -= 60;
		}
	}

	if(x1>x2)	delta_x=x1-x2;
	else     	delta_x=x2-x1;

	if(y1>y2)	delta_y=y1-y2;
	else		delta_y=y2-y1;

	m_ssCourseX[step]=x1;
	m_ssCourseY[step]=y1;
	step++;

	if(delta_x>delta_y)
	{
	   y=y1;
	   half=(SHORT)(delta_x/2);

	   if(x1<x2)
	   {
		   for(x=x1+1;x<=x2;x++)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;

			  }
		  }
	   }
	   else
	   {
		   for(x=x1+1;x>=x2;x--)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	}
	else
	{
	   x=x1;
	   half=(SHORT)(delta_y/2);

	   if(y1<y2)
	   {
		  for(y=y1+1;y<=y2;y++)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	   else
	   {
		  for(y=y1+1;y>=y2;y--)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
			  ++index%=m_ssStepWidth;
			  if(index==0)
			  {
				m_ssCourseX[step]=x;
				m_ssCourseY[step]=y;
				if(step<MAX_SHOOT_RANGE)
				   step++;
			  }
		  }
	   }
	}

	m_ssMaxMoveStep=step-1;
}


//
// 찍을 위치를 계산한다. 
//

void _Bullet::CalculateDrawStartXY()
{
	SHORT screenx=0, screeny=0;
	clifMap.TransFromRealDotToScreenDot(m_ssRealX, m_ssRealY, screenx, screeny);
	m_ssDrawStartX=screenx-m_ssDrawXsize/2;
	m_ssDrawStartY=screeny-m_ssDrawYsize/2;

	if(m_ssCurveSwitch==TRUE)
	{
	   clifMap.TransFromRealDotToScreenDot(m_ssCurveBulletRealX, m_ssCurveBulletRealY, screenx, screeny);
	   m_ssCurveBulletDrawX=screenx-m_ssDrawXsize/2;
	   m_ssCurveBulletDrawY=screeny-m_ssDrawYsize/2;
	}

}

void _Bullet::CalculateRealXY()
{

	m_ssDrawXsize = GetFileXsize(m_ssFile);
	m_ssDrawYsize = GetFileYsize(m_ssFile);

	// X, Y좌표를 구한다. 
	clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);


	m_ssCenterX=m_ssRealX;
	m_ssCenterY=m_ssRealY;


	////////////////////////////////////////////	
	// 곡선으로 발사된 총알의 경우 그림자는 직선으로 이동하지만 
	// 총알 자신은 포물선을 그린다. 
	// 위치에 따라서 방향이 계속바뀐다. 

	if(m_ssCurveSwitch==TRUE)
	{
	SHORT incy;
	SHORT vary;

	// 현재의 총알 위치를 구한다. 
	if(m_ssMoveStep<( (m_ssMaxMoveStep) /2))
	{
		vary=m_ssMoveStep*m_ssStepWidth;
		if(vary<MAX_SQRT_RANGE)
		  incy=SqrtVary[vary]*m_ssCurveRate;
		else
			Error("1974672", "vary:%d", vary);
	}
	else
	{
		vary=(m_ssMaxMoveStep-m_ssMoveStep)*m_ssStepWidth;
		if(vary<MAX_SQRT_RANGE)
		  incy=SqrtVary[vary]*m_ssCurveRate;
		else
			Error("19734672", "vary:%d", vary);

	}
	
	m_ssCurveBulletRealX=m_ssCenterX;
	m_ssCurveBulletRealY=m_ssCenterY-incy;


	if(m_ssKind==KIND_BULLET_FIRETANKARROW)
	{

		 switch(m_siTargetDirection)
		 {
		 case SOUTH|EAST:
			 m_siTempDirection=0;
			 break;

		 case SOUTH:
			 m_siTempDirection=9-m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

		 case NORTH:
			 m_siTempDirection=18+m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

		 case NORTH|WEST:
			 m_siTempDirection=16;
			 break;

		 case WEST:
			 m_siTempDirection=14-m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

		 case EAST:
			 m_siTempDirection=22+m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

		 case NORTH|EAST:
			 m_siTempDirection=20+m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

		 case SOUTH|WEST:
			 m_siTempDirection=12-m_ssMoveStep*8/(m_ssMaxMoveStep);
			 break;

	 }

	}

	m_ssOldCurveBulletRealX=m_ssCurveBulletRealX;
	m_ssOldCurveBulletRealY=m_ssCurveBulletRealY;



	}
///////////////////////////////////////////


}

BOOL _Bullet::Draw()
{
	// 이미지가 준비되지 않은 것은 그리지 않는다. 
	if(m_ssFile == 0)return FALSE;


	unsigned char *darktable;
	// 어둔 단계를 설정한다. 
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	darktable = TeamDarkTable[darkstep];

	if(m_ssDrawSwitch==FALSE)
		return FALSE;

	if(m_ssCurveSwitch==TRUE)
	{

		// 그림자 찍기 
        if(m_ssReverseSwitch==FALSE)
		{
		     if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
			 {
				 Draw_PutSpriteShadowT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		     else
			 {
				 Draw_PutSpriteShadowCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }

		}
        else if(m_ssReverseSwitch==TRUE)
		{
		     if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
			 { 
				 Draw_PutSpriteShadowRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		     else
			 {
				 Draw_PutSpriteShadowCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, DarkTable[SHADOW_DARK_STEP]);
			 }
		}

		// 실제 화살 찍기 
		if(m_ssReverseSwitch==FALSE)
		{
//			Draw_PutSpriteT(CurveBulletDrawX, CurveBulletDrawY, File, Font, darktable);
			if(clGrp.IsInClipArea(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssCurveBulletDrawX+GetFileXsize(m_ssFile)-1,
															m_ssCurveBulletDrawY+GetFileYsize(m_ssFile)-1)==TRUE)
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
			else
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinCT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
		}
		else if(m_ssReverseSwitch==TRUE)
		{
//			Draw_PutSpriteRT(CurveBulletDrawX, CurveBulletDrawY, File, Font, darktable);
			if(clGrp.IsInClipArea(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssCurveBulletDrawX+GetFileXsize(m_ssFile)-1,
															m_ssCurveBulletDrawY+GetFileYsize(m_ssFile)-1)==TRUE)
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
			else
			{
				if(m_ssTransparentSwitch==TRUE)
				{
					Draw_PutSpriteLightImjinCRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont);
				}
				else
				{
					Draw_PutSpriteRT(m_ssCurveBulletDrawX, m_ssCurveBulletDrawY, m_ssFile, m_ssFont, darktable);
				}
			}
		}
	}
	// 곡선 탄도가 아닐때 
	else
	{
             if(m_ssReverseSwitch==FALSE)
			 {
		        if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
		        else
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinCT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}

			 }
             else if(m_ssReverseSwitch==TRUE)
			 {
		        if(clGrp.IsInClipArea(m_ssDrawStartX, m_ssDrawStartY, 
		        m_ssDrawStartX+GetFileXsize(m_ssFile)-1,
		        m_ssDrawStartY+GetFileYsize(m_ssFile)-1)==TRUE)
				{ 
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
		        else
				{
					if(m_ssTransparentSwitch==TRUE)
					{
						Draw_PutSpriteLightImjinCRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont);
					}
					else
					{
						Draw_PutSpriteRT(m_ssDrawStartX, m_ssDrawStartY, m_ssFile, m_ssFont, darktable);
					}
				}
			 }

	}
   return TRUE;
}



//
// 찍어야 할 그림을 선정한다. 
//

void _Bullet::DecideFont()
{
	switch(m_ssAnimation)
	{
	case ANI_BULLET_MOVE:
		DecideFontMove();
    	break;
	default:
		break;
	}
}

//
// 결정된 방향의 폰트를 불러온다. 
//

void _Bullet::DecideFontMove()
{
	switch(m_ssKind)
	{
		case KIND_BULLET_FIRETANKARROW:
				DecideFontMoveFireTankArrow();
				break;

		// 화룡출수
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// 발석거 기본 공격(불 붙은 돌)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// 발석거 승천포 포탄
		case KIND_BULLET_BOMB_OF_HEAVEN:
				m_ssAniStep = m_snFrameIndex[m_ssMoveStep];
				DecideFontMoveNormal();
				break;

		default:
				DecideFontMoveNormal();
				break;
	}
}

void _Bullet::DecideFontMoveNormal()
{

		switch(m_siTempDirection)
		{
		case SOUTH:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2] + m_ssAniStep;
			break;
		case SOUTH|WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[4];
			m_ssFont=m_ssMoveFont[4] + m_ssAniStep;
			break;
		case WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[6];
			m_ssFont=m_ssMoveFont[6]+ m_ssAniStep;
			break;
		case NORTH|WEST:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[8];
			m_ssFont=m_ssMoveFont[8]+ m_ssAniStep;
			break;
		case NORTH:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[6];
			m_ssFont=m_ssMoveFont[6]+ m_ssAniStep;
			break;
		case NORTH|EAST:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[4];
			m_ssFont=m_ssMoveFont[4]+ m_ssAniStep;
			break;
		case EAST:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2]+ m_ssAniStep;
			break;
		case EAST|SOUTH:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[0];
			m_ssFont=m_ssMoveFont[0]+ m_ssAniStep;
			break;
		case SSW:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[3];
			m_ssFont=m_ssMoveFont[3]+ m_ssAniStep;
			break;
		case EEN:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[3];
			m_ssFont=m_ssMoveFont[3]+ m_ssAniStep;
			break;
		case WWS:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[5];
			m_ssFont=m_ssMoveFont[5]+ m_ssAniStep;
			break;
		case NNE:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[5];
			m_ssFont=m_ssMoveFont[5]+ m_ssAniStep;
			break;
		case WWN:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[7];
			m_ssFont=m_ssMoveFont[7]+ m_ssAniStep;
			break;
		case NNW:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[7];
			m_ssFont=m_ssMoveFont[7]+ m_ssAniStep;
			break;
		case SSE:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[1];
			m_ssFont=m_ssMoveFont[1]+ m_ssAniStep;
			break;
		case EES:
			m_ssReverseSwitch=TRUE;
			m_ssFile=m_ssMoveFile[1];
			m_ssFont=m_ssMoveFont[1]+ m_ssAniStep;
			break;
		default:
			m_ssReverseSwitch=FALSE;
			m_ssFile=m_ssMoveFile[2];
			m_ssFont=m_ssMoveFont[2]+ m_ssAniStep;

			SetMessage("잘못된 총알 폰트가 들어가고 있습니다.(Error Bullet Fonts!)");

		   break;
		}

}

void _Bullet::DecideFontMoveFireTankArrow()
{
	m_ssReverseSwitch=FALSE;
	m_ssFile=m_ssMoveFile[0];
	m_ssFont=m_siTempDirection;
}

//
// 방향을 설정 한다.
//

void _Bullet::DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	switch(m_ssKind)
	{
	case KIND_BULLET_FIRETANKARROW: DecideDirectionFireTankArrow(x1, y1, x2, y2);  break;
	default: DecideDirectionNormal(x1, y1, x2,  y2); break;
	}
}

void _Bullet::DecideDirectionFireTankArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	m_siTempDirection=DecideDirection32(x1, y1, x2, y2);
}



void _Bullet::DecideDirectionNormal(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	SHORT dir=DecideDirection16(x1, y1, x2, y2);
	
	switch(dir)
	{
	     case 0:
			  m_siTempDirection=SOUTH|EAST;
			  break;
	     case 1:
			  m_siTempDirection=SSE;
			  break;
	     case 2:
			  m_siTempDirection=SOUTH;
			  break;
	     case 3:
			  m_siTempDirection=SSW;
			  break;
	     case 4:
			  m_siTempDirection=SOUTH|WEST;
			  break;
	     case 5:
			  m_siTempDirection=WWS;
			  break;
	     case 6:
			  m_siTempDirection=WEST;
			  break;
	     case 7:
			  m_siTempDirection=WWN;
			  break;
	     case 8:
			  m_siTempDirection=WEST|NORTH;
			  break;
	     case 9:
			  m_siTempDirection=NNW;
			  break;
	     case 10:
			  m_siTempDirection=NORTH;
			  break;
	     case 11:
			  m_siTempDirection=NNE;
			  break;
	     case 12:
			  m_siTempDirection=NORTH|EAST;
			  break;
	     case 13:
			  m_siTempDirection=EEN;
			  break;
	     case 14:
			  m_siTempDirection=EAST;
			  break;
	     case 15:
			  m_siTempDirection=EES;
			  break;
	     default:
			  m_siTempDirection=SOUTH|EAST;
			  break;
	}
}

//
// 총알을 이동시켜줌.
//

void _Bullet::SetMoveImage16(SHORT file, SHORT font, SHORT frame)
{
	SHORT i;

	for(i=0;i<9;i++)
		SetMoveImage(i, file, font+frame*i);
}

void _Bullet::SetMoveImage(SHORT ani, SHORT file, SHORT font)
{
	m_ssMoveFile[ani]=file;
	m_ssMoveFont[ani]=font;
}

BOOL _Bullet::Action()
{
	SHORT i, j;
	_NewID id, min_id;
	SHORT smokex, smokey;
	SHORT min_length, length;
	SHORT x, y, effectx, effecty;
	SHORT smokeid;


   switch(m_ssCurrentOrder)
   {
	//------------------------------------------------------------------------------------------------
	// 총알의 작동 추가 : robypark(2004/5/19, 18:48)
	//------------------------------------------------------------------------------------------------
	// 탄착되었을 때 특수기술을 생성한다.
	// 승천포(중국-발석거)를 위한 작동
	case ORDER_BULLET_EVENT_IMPACT:
		{
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// 좌표를 결정한다. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// 탄착되었을 때 특수기술을 생성한다.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				// 승천포 기술을 사용한다
				SetMagic_BOMB_OF_HEAVEN(m_ssMapDX, m_ssMapDY, \
							m_ShooterID, m_ssShooterPlayer, AREA_LAND, \
							m_ssApower, m_ssHitRate, m_AttackID);

				return FALSE;
			}

			m_ssMoveStep++;
		}
		break;

	//------------------------------------------------------------------------------------------------
	// 총알의 작동 추가 : robypark(2004/5/19, 13:06)
	//------------------------------------------------------------------------------------------------
	// 피어싱(관통)되는 총알 작동(맵 끝 까지 진행)
	// 총알 이동 중 총알이 있는 곳의 적들 유닛에 데미지를 준다.
	// 맹격화룡파를 위한 총알 이동
	case ORDER_BULLET_PIERCE:
		{
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// 좌표를 결정한다. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// X, Y좌표를 구한다. 
			clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);

			// 총알 위치에 존재하는 적 캐릭터에게 데미지를 준다.
			SI32 siID = Map.GetIDMap(m_ssX, m_ssY);
//			if ((IsAlive(ID))
//				&& (IsFriendToByPlayerNumber(ShooterPlayer, CR[ID].GetPlayerNumber()) == FALSE))
			{
				m_AttackID = CR[siID].m_NewID;

				// 적 캐릭터에게 데미지를 준다.
//				Hit();
			}

			// 적 캐릭터에게 데미지를 준다.
			Hit();

			// 총알이 맵 끝까지 도달했다면 총알 객체 소멸을 명시한다.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				return FALSE;
			}

			m_ssMoveStep++;
		}
		break;

	//------------------------------------------------------------------------------------------------
	// 총알의 작동 추가 : robypark(2004/5/19, 13:06)
	//------------------------------------------------------------------------------------------------
	// 직선으로 맵의 한 끝점으로 이동하는 총알 작동
	// 총알 이동 중 총알이 있는 곳의 적들 유닛에 데미지를 준다.
	// 데미지를 준 이후 총알은 소멸한다.
	// 굉뇌를 위한 총알 이동
   case ORDER_BULLET_STRAIGHT:
	   {
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				// 굉뇌 사용시 번개가 내리친 이후 굉뇌번개가 발사되도록 수정한다.
				if (m_ssKind == KIND_BULLET_THUNDER_BOLT)
				{
					// 대기시간이 지났는가?
					if (m_snCurrentDelay < m_snMaxDelay)
					{
						m_snCurrentDelay++;
						return TRUE;
					}

					// 대기시간이 지났으므로 그려주도록 설정한다.
					m_ssDrawSwitch = TRUE;
				}

				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// 좌표를 결정한다. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// X, Y좌표를 구한다. 
			clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);
			// 총알 위치에 존재하는 적 캐릭터에게 데미지를 준다.
//			SHORT siTileX = 0, siTileY = 0;
//			Map.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &siTileX, &siTileY);
			SI32 ID = Map.GetIDMap(m_ssX, m_ssY);
			if ((IsAlive(ID))
				&& (IsFriendToByPlayerNumber(m_ssShooterPlayer, CR[ID].GetPlayerNumber()) == FALSE))
			{
				m_AttackID = CR[ID].m_NewID;

				// 적 캐릭터에게 데미지를 준다.
				Hit();

				return FALSE;
			}

			// 총알이 맵 끝까지 도달했다면 총알 객체 소멸을 명시한다.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				return FALSE;
			}

			m_ssMoveStep++;
	   }
	   break;

	   // 암흑화염폭풍
   case ORDER_BULLET_DARKFIRE:
	   {
		   m_ssAnimation = ANI_BULLET_WAIT;

		   SHORT DifSkillTime = (timeGetTime() - m_uiSkillTime)/1000;

		   if(DifSkillTime >= 6)		
			   return FALSE;

		   for(i = -DifSkillTime ; i <= DifSkillTime; i++)
		   {
			   for(j = -DifSkillTime ; j <= DifSkillTime; j++)
			   {
				   x = m_ssMapDX + j;
				   y = m_ssMapDY + i;
				   effectx = m_ssMapDX + (j/2)*2;
				   effecty = m_ssMapDY + (i/2)*2;
				   
				   if(clifMap.IsInMapArea(x, y) == FALSE)	continue;

				   if(!DifSkillTime)
				   {
					   if(GD.Random() % 6 == 0)
					   {
						   smokeid = FindEmptySmoke();
						   if(smokeid)
							   SetSmoke(KIND_SMOKE_DARKFIRE, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   smokeid = FindEmptySmoke();
						   if(smokeid)
							   SetSmoke(KIND_SMOKE_LIGHTNING, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_AIR);
						   smokeid = FindEmptySmoke();
						   if(smokeid)
							   SetSmoke(KIND_SMOKE_LIGHTEXP1, smokeid, MapCenterX[x][y], MapCenterY[x][y]-20, AREA_LAND);
					   }
				   }
				   
				   smokeid = FindEmptySmoke();
				   if(!smokeid)								continue;

				   
				   SetSmoke(KIND_SMOKE_DARKFIRE, smokeid, MapCenterX[effectx][effecty], MapCenterY[effectx][effecty], AREA_LAND);

				   if(GD.Random() % 20 != 0)				continue;

/*				   if(GD.Random() % 10 == 0)
				   {
					   smokeid = FindEmptySmoke();
					   if(smokeid)
						   SetSmoke(KIND_SMOKE_LIGHTEXP1, smokeid, MapCenterX[x][y], MapCenterY[x][y]-20, AREA_LAND);
				   }
*/				   
				   // 화염이 있는 곳의 유닛을 구한다. 

				   SHORT temp_id = clifMap.GetIDMap(x, y);
				   
				   // 유닛이 적이면 타격을 준다. 
				   if(IsSee(temp_id) && IsFriendToByPlayerNumber(m_ssShooterPlayer, clifChar.GetPlayerNumber(temp_id)) == FALSE)
				   {
					   SHORT TempPower = m_ssApower / (max(abs(i), abs(j))+1);
					   CalculateHit(m_ShooterID, m_ssShooterPlayer, ATTACKATB_MAGIC, 1, m_scArea, TempPower, clifChar.GetID(temp_id), x, y, m_ssHitRate);
				   }
			   }
		   }
	   }
	   break;
   case ORDER_BULLET_WAITEXP4:
	   m_ssAnimation = ANI_BULLET_WAIT;

	   // 수명이 다 되었다. 
	   if(m_ssMoveStep > 250)
	   {
		   return FALSE;
	   }

	   m_ssMoveStep ++;
	   
	   for(i =  -4 ;i <= 4;i++)
		   for(j = -4 ; j <= 4; j++)
		   {
			   x = m_ssMapDX + j;
			   y = m_ssMapDY + i;
			   
			   if(clifMap.IsInMapArea(x, y) == FALSE)continue;
			   
			   if(GD.Random() % 40 == 0)
			   {
				   smokeid = FindEmptySmoke();
				   if(smokeid)
				   { 
					   switch(GD.Random() %3 )
					   {
					   case 0:
						   SetSmoke(KIND_SMOKE_GROUNDEXP1, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;
					   case 1:	
						   SetSmoke(KIND_SMOKE_GROUNDEXP2, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;	
					   case 2:
						   SetSmoke(KIND_SMOKE_GROUNDEXP3, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;
					   }
					   // 바닥에 불을 붙여준다.
					   if(GD.Random() % 25 == 0)
					   {
						   clifMap.PutFire(x, y, 1, GROUNDFIRE_LIFETIME,m_ShooterID,m_ssShooterPlayer);
					   }

					   // 화염이 있는 곳의 유닛을 구한다. 
					   SHORT temp_id = clifMap.GetIDMap(x, y);

					   // 유닛이 적이면 타격을 준다. 
					   if(IsSee(temp_id) && IsFriendToByPlayerNumber(m_ssShooterPlayer, clifChar.GetPlayerNumber(temp_id)) == FALSE)
					   {
						   CalculateHit(m_ShooterID, m_ssShooterPlayer, ATTACKATB_MAGIC, 1, m_scArea, m_ssApower, clifChar.GetID(temp_id), x, y, m_ssHitRate);
					   }
				   }
			   }
			   
		   }
	   break;

   case ORDER_BULLET_WAITEXP:
	   m_ssAnimation = ANI_BULLET_WAIT;

	   // 수명이 다 되었다. 
	   if(m_ssMoveStep > 250)
	   {
		   return FALSE;
	   }

	   m_ssMoveStep ++;
	   
	   for(i =  -3 ;i <= 3;i++)
		   for(j = -3 ; j <= 3; j++)
		   {
			   x = m_ssMapDX + j;
			   y = m_ssMapDY + i;
			   
			   if(clifMap.IsInMapArea(x, y) == FALSE)continue;
			   
			   if(GD.Random() % 40 == 0)
			   {
				   smokeid = FindEmptySmoke();
				   if(smokeid)
				   { 
					   switch(GD.Random() %3 )
					   {
					   case 0:
						   SetSmoke(KIND_SMOKE_GROUNDEXP1, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;
					   case 1:	
						   SetSmoke(KIND_SMOKE_GROUNDEXP2, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;	
					   case 2:
						   SetSmoke(KIND_SMOKE_GROUNDEXP3, smokeid, MapCenterX[x][y], MapCenterY[x][y], AREA_LAND);
						   break;
					   }
					   // 바닥에 불을 붙여준다.
					   if(GD.Random() % 25 == 0)
					   {
						   clifMap.PutFire(x, y, 1, GROUNDFIRE_LIFETIME,m_ShooterID,m_ssShooterPlayer);
					   }

					   // 화염이 있는 곳의 유닛을 구한다. 
					   SHORT temp_id = clifMap.GetIDMap(x, y);

					   // 유닛이 적이면 타격을 준다. 
					   if(IsSee(temp_id) && IsFriendToByPlayerNumber(m_ssShooterPlayer, clifChar.GetPlayerNumber(temp_id)) == FALSE)
					   {
						   CalculateHit(m_ShooterID, m_ssShooterPlayer, ATTACKATB_MAGIC, 1, m_scArea, m_ssApower, clifChar.GetID(temp_id), x, y, m_ssHitRate);
					   }
				   }
			   }
			   
		   }
	   break;

	   case ORDER_BULLET_MOVE:
			m_ssAnimation=ANI_BULLET_MOVE;

			// 대기시간이 지났는가?
			if (m_snCurrentDelay < m_snMaxDelay)
			{
				m_snCurrentDelay++;
				return TRUE;
			}

			// 대기시간이 지났으므로 그려주도록 설정한다.
			m_ssDrawSwitch = TRUE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

		   // 유도탄인 경우에, 
		   if(m_ssChaseSwitch==TRUE)
		   {
		      if(m_ssMoveStep>2 && m_ssChaseRepeatNumber<15)
			  {
				   if(IsSee(m_AttackID.ID) && m_AttackID.IsSame(clifChar.GetID(m_AttackID.ID)) == TRUE)
				   {
					   clifChar.GetHitPoint(m_AttackID.ID, &m_ssRealDX, &m_ssRealDY);
				       m_ssChaseRepeatNumber++;

					   m_ssMapDX = clifChar.GetX(m_AttackID.ID);
					   m_ssMapDY = clifChar.GetY(m_AttackID.ID);

				       m_ssRealDX+=m_ssVaryX;
				       m_ssRealDY+=m_ssVaryY;

				       m_ssMoveStep=0;m_ssMaxMoveStep=0;
					   DecideDirection(m_ssRealX, m_ssRealY, m_ssRealDX, m_ssRealDY);

			           GetCourse();			
				   }
			  }
		   }

		   // 좌표를 결정한다. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			if(m_ssMoveStep==m_ssMaxMoveStep)
			{
#ifdef _IMJIN2
				// 다시 목표료를 탐지하기 시작한다. 

				if(m_ssKind==KIND_BULLET_AIRMAGIC)
				{
				    min_length=10000;
				    min_id.Init();

				    for(i=0;i<GD.AirCharNumber;i++)
					{
			            id=GD.AirCharID[i];
			            if(GD.CharInfo[id.ID])
						{
					        // 거리가 가깝고 
					        length=max(abs(clifChar.GetX(id.ID)- m_ssMapDX), abs(clifChar.GetY(id.ID) - m_ssMapDY));

					        if(length<5 && IsFriendToByID(m_ShooterID.ID, id.ID)==FALSE)
							{
						       if(length<min_length)
							   {
						          min_length=length;
						          min_id=id;
							   }
							}

						}
					} 

					// 가까이에서 공중 캐릭터를 찾으면, 
					if(min_id.IsEmpty()==FALSE)
					{
						SHORT dx, dy;
						int centerx, centery;

						if(m_ssMapSX<m_ssMapDX)
							 dx=m_ssMapDX+2;
						else if(m_ssMapSX>m_ssMapDX)
							 dx=m_ssMapDX-2;
						else dx=m_ssMapDX;

						if(m_ssMapSY<m_ssMapDY)
							 dy=m_ssMapDY+2;
						else if(m_ssMapSY>m_ssMapDY)
							 dy=m_ssMapDY-2;
						else dy=m_ssMapDY;

						if(clifMap.IsInMapArea(dx, dy)==TRUE && clifMap.GetRealDotCenterXY(dx, dy, centerx, centery))
						{

			               SetBullet(KIND_BULLET_AIRMISSILE, m_ssID, 
							         m_ssMapDX, m_ssMapDY, 
							         dx, dy, 
					                 m_ssRealX, m_ssRealY,
							         centerx, centery, m_ShooterID, min_id, m_ssApower, m_ssHitRate, 0, m_scArea);
						}
						else 
						{
							Hit();
  				            return FALSE;
						}
					}
					else
					{
						Hit();
				        return FALSE;
					}
				}
				else
				{
				    Hit();
				    return FALSE;
				}
#endif


			}
			else
			{

				// 꼬리를 만들어야 한다면, 
				if(m_ssTailSwitch==TRUE)
				{
				   SHORT smoke_id=FindEmptySmoke();
				   if(smoke_id)
				   {
					   if(m_ssCurveSwitch==TRUE)
					   {
						   smokex=m_ssCurveBulletRealX;
						   smokey=m_ssCurveBulletRealY;
					   }
					   else
					   {
						   smokex=m_ssRealX;
						   smokey=m_ssRealY;
					   }

					   // 함포인 경우 , 
					   if(m_ssKind==KIND_BULLET_SHIPCANNON)
					   {
					     if(m_ssMoveStep%3==0)
					         SetSmoke(KIND_SMOKE_SMOKE0, smoke_id, smokex, smokey, m_scArea);
					   }
					   // 던지는 폭탄인 경우 
					   if(m_ssKind==KIND_BULLET_THROWBOMB)
					   {
					     if(m_ssMoveStep%3==0)
					         SetSmoke(KIND_SMOKE_SMOKE0, smoke_id, smokex, smokey, m_scArea);
					   }
					   // 화염 방사기인  경우 , 
					   else if(m_ssKind==KIND_BULLET_FIRE)
					   {
					     if(m_ssMoveStep%2==0)
					         SetSmoke(KIND_SMOKE_FIRE, smoke_id, smokex + 8 - (GD.Random()%16), smokey + 8 - (GD.Random()%16),  m_scArea);
					   }
					   // 골드 드래곤의 전기인 경우, 
					   else if(m_ssKind==KIND_BULLET_GOLDDRAGONELECTRON)
					   {
					     if(m_ssMoveStep%2==0)
					         SetSmoke(KIND_SMOKE_GOLDDRAGONELECTRON, smoke_id, smokex + 8 - (GD.Random()%16), smokey + 8 - (GD.Random()%16),  m_scArea);
					   }
					   else if(m_ssKind==KIND_BULLET_FIREARROW)
					   {

						   // 연기를 표현한다. 
						   if(m_ssMoveStep%3==0)
				              SetSmoke(KIND_SMOKE_SMOKE10, smoke_id, smokex, smokey, m_scArea);
					   }
					   else if(m_ssKind==KIND_BULLET_CANNON)
					   {

						   // 연기를 표현한다. 
						   if(m_ssMoveStep%2==0)
				              SetSmoke(KIND_SMOKE_SMOKE2, smoke_id, smokex, smokey, m_scArea);

					   }
					   else if(m_ssKind==KIND_BULLET_AIRMISSILE)
					   {
						   // 연기를 표현한다. 
						   if(m_ssMoveStep%3==0)
				              SetSmoke(KIND_SMOKE_SMOKE1, smoke_id, smokex, smokey, m_scArea);
					   }

					   else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_TRAIL_ARROWTRAIL)
					   {
						   // 연기를 표현한다. 
						   //if(MoveStep % 2==0)
				              SetSmoke(KIND_SMOKE_ARROWTRAIL, smoke_id, smokex, smokey, m_scArea);
					   }
					   else
					   {
						    if(m_ssMoveStep%3==0)
						         SetSmoke(KIND_SMOKE_SMOKE0, smoke_id, smokex, smokey,  m_scArea);
					   }

				   }
				}

			    m_ssMoveStep++;

			}

		   break;
   }

   CalculateRealXY();
   DecideFont();
   return TRUE;
}

// robypark 2004/6/4 19:25
// 불랑기포차 기술 앙천대소 폭탄 폭발시 파편을 생성해준다.
void _Bullet::CreateFragment(void)
{
	SI32		i = 0;
	SI32		siBulletID = 0;
	SI32		firex, firey;
	SI32		attackx, attacky;
	SI32		vary;
	SI16		siMaxFragment = (GD.Random() & 7) + 4;
	
	firex = m_ssMapDX - 1;
	firey = m_ssMapDY - 1;

	if (Map.IsInMapArea(firex, firey) == FALSE)
	{
		firex = m_ssMapDX;
		firey = m_ssMapDY;
	}

	// 무작위로 파편을 생성한다.
	for (i = 0; i < siMaxFragment; i++)
	{
		vary = 5 - GD.Random() % 8;

		attackx = firex + vary + 2;
			
		vary = 5 - GD.Random() % 8;
			
		attacky = firey + vary + 2;

		// 생성하려는 파편의 목표점이 맵 안에 있다면
		if(Map.IsInMapArea(attackx, attacky) == TRUE)
		{
			// 파편 생성
			siBulletID = FindEmptyBullet();
			if (siBulletID)
			{
				switch (GD.Random() & 3)
				{
					case 0:
						// 파편에 의한 데미지는 없다.
						SetBullet(KIND_BULLET_FRAGMENT1, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 1:
						// 파편에 의한 데미지는 없다.
						SetBullet(KIND_BULLET_FRAGMENT2, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 2:
						// 파편에 의한 데미지는 없다.
						SetBullet(KIND_BULLET_FRAGMENT3, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 3:
						// 파편에 의한 데미지는 없다.
						SetBullet(KIND_BULLET_FRAGMENT3, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
				}

				// 총알 발사에 시차를 두기 위한 작업
				Bullet_SetLuanchDelay(siBulletID, GD.Random() & 3);
				Bullet_DrawSwitch(siBulletID, FALSE);
			}
		}
	}
}


// robypark (2004/6/12 12:27
// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
// 설정해준다.
void _Bullet::SetLayerAboveVolcano(bool bAbove/* = true*/)
{
	m_bLayerAboveVolcano = bAbove;
}








//
// 살아있는 총알객체를 알려줌
//

SHORT FindEmptyBullet()
{
	int i;

	for(i=1;i<MAX_BULLET_NUMBER;i++)
	{
		if(GD.BulletInfo[i]==0)return i;
	}
	return 0;

}

//
// 총알 죽이기
//

void DeleteBullet(SHORT id)
{
	GD.BulletInfo[id]=0;
}

//
// 모든 총알 죽이기
//
void DeleteBullet()
{
	SHORT i;

	for(i=0;i<MAX_BULLET_NUMBER;i++)
	{
		DeleteBullet(i);
	}
}

SHORT SetBullet(SHORT kind, SHORT id, SHORT sx, SHORT sy, SHORT mx, SHORT my, SHORT x, SHORT y, SHORT dx, SHORT dy, _NewID shooterid, _NewID targetid, SHORT apower, SI32 hitrate, SHORT targetdir, char attack_area)
{
	if(id >= MAX_BULLET_NUMBER)
	{
		Error("Bullet", "dfsje65 : %d[%d]", id, kind);
		return 0;
	}


	g_Bullet[id].Init(kind, id, sx, sy, mx, my, x, y, dx, dy, shooterid, targetid, apower, hitrate, targetdir, attack_area);
	g_Bullet[id].SetInfo();
	return 1;
}

//---------------------------------------------------------------------------------------------------
// 2001.11.12	hojae_append
//---------------------------------------------------------------------------------------------------
BOOL Bullet_Action(SI32 i)
{
	return g_Bullet[i].Action();
}

void Bullet_CalculateDrawStartXY(SI32 i)
{
	g_Bullet[i].CalculateDrawStartXY();
}

void Bullet_Draw(SI32 i)
{
	g_Bullet[i].Draw();
}

void Bullet_DrawSwitch(SHORT bulletid, BOOL TorF)
{
	g_Bullet[bulletid].m_ssDrawSwitch = TorF;
				 
}

void Bullet_CurrentOrder(SHORT bulletid, SHORT DEF)
{
	g_Bullet[bulletid].m_ssCurrentOrder = DEF;
}

void Bullet_Fwrite(FILE* fp)
{
	fwrite(g_Bullet, sizeof(_Bullet), MAX_BULLET_NUMBER, fp);
}

void Bullet_Read(FILE* fp)
{
	fread(g_Bullet, sizeof(_Bullet), MAX_BULLET_NUMBER, fp);
}

/////////////////////////////////////
// robypark (2004/5/27 19:11)
// 불랑기포차 3발사 시차 발사 위한 함수
void Bullet_SetLuanchDelay(int i, SHORT snMaxDelay)
{
	g_Bullet[i].SetLuanchDelay(snMaxDelay);
}

// robypark (2004/6/12 12:27
// 격노염폭을 통하여 생성되는 총알을 화산(Smoke) 위(앞)에 그려야할 경우 
// 설정해준다.
void SetLayerAboveVolcano(SI32 siBulletID, bool bAbove/* = true*/)
{
	g_Bullet[siBulletID].SetLayerAboveVolcano(bAbove);
}

/////////////////////////////////////