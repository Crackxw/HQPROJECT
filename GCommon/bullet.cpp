//--------------------------------------------------------------------
//  ��� : ���°�
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
// �¶��ΰ��� �ļ� ���
#include "../SCCommon/OnlineCommonParser.h"

extern _clGame*				pGame;

extern	_NewID				ZeroID;
extern	_MainData			GD;
extern	cltCharInterface	clifChar;								// ĳ���� ���� �������̽� 
extern	_Char				CR[];								// ĳ������ ������ 
extern	cltMapInterface		clifMap;								// �� ���� �������̽� 
extern	unsigned char		TeamDarkTable[MAX_DARK_STEP][256];
extern	unsigned char		DarkTable[][256];						// ��ο� �ܰ� 
extern	int					MapCenterX[][MAX_MAP_YSIZE];
extern	int					MapCenterY[][MAX_MAP_YSIZE];
extern	_Map 				Map;
extern	SI32				UserPlayerNumber;					// ����� ���� 
extern	_KindInfo			KI[];								// ĳ���� ������ ������ 
extern  OnlineBattleInfo	OBInfo;
extern _OnlineBattleMsgQue  OBMQue;
extern	_MyNetGame			MyNet;

_Bullet g_Bullet[MAX_BULLET_NUMBER]; // �Ѿ˿� ������ 


// robypark 2004/9/30 18:28
// Smoke ������ HQ���� InfoBullet.txt���� ���� ����Ѵ�.
void _BulletKindInfo::Init(char *pszKindID, SI32 BulletAtb, SI32 BulletSmokeAtb)
{
	FILE*	fp;
	OnlineCommonParser	ocp;
	char lpFileName[] = "InfoBullet.txt";

	// HQ���� ������ ����.
	fp = BindJXFile::GetInstance()->GetFile( lpFileName );
	if ( !fp )
		return;

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", lpFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "BulletInfo Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", lpFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return;
	}

	// �ڷ� �˻�
	while(1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� �޴´�.
		SI32	ssStepWidth, ssFont, siMoveFrameNumber, bDirSwitch, 
					ssTailSwitch, ssChaseSwitch, ssCurveSwitch, 
					ssTransparentSwitch, ssHeightSwitch, siDelay;

		char	pszDestKindID[128], pszResourceID[128], pszTemp[256], pszHitEffectSoundIdx[128];

		ocp.GetValue( pszDestKindID,		127,		iLineNo, iColNo );		// Smoke Kind ID

		//----------------------------------------------------------------------------
		// �ϴ� ������� ã�� ������ ��ġ���� ������ ���� ���� ���� ����.
		if ( stricmp(pszDestKindID, pszKindID) != 0)
		{
			continue;
		}
		// �Ѿ�Դٸ� ã�� ���̴�.
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
		ocp.GetValue( pszHitEffectSoundIdx,	128,	iLineNo, iColNo );		// Hit Effect Sound. EffectSound.txt���� ���Ǵ� ȿ���� �ε��� �ؽ�Ʈ �̿�.
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

		// ��Ʈ ȿ������ ���ٸ�
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
	clGrp.Error("Effect Image Information Setting Error", "�Ż� Ŭ���̾�Ʈ�� ������ �ǽɵ˴ϴ�. ��ġ�� �Ż� Ŭ���̾�Ʈ�� Ȯ�����ּ���.\nGersang(Kyosyoden) Client doubt modified. Please Check your client.");
#endif
}

// �Ѿ� ���� ���� ���� ���� ����
_BulletKindInfo g_BKI[MAX_BULLET_KIND_NUMBER];


const SI32 BulletSpeedArrow = 30;	// ȭ�� ���ư��� �ӵ�. 
const SI32 BulletSpeedGun	= 40;	// �Ѿ� ���ư��� �ӵ�. 

// Bullet ��ü�� �ʱ�ȭ�Ѵ�.
// Bullet �̹����� ��� hq�� ImgFile.txt���Ͽ��� ó���� �ε��ϵ��� �÷��׸� ������ �־�� �Ѵ�.
// �׷��� ������, �̹����� ����� ������ �ʴ´�.
void InitBulletKindInfo()
{
	////////////////////////////////////////////////////////////
	// robypark 2004/10/1 3:14
	// HQ�� InfoBullet.txt���� �����͸� ���� ����ϵ��� ����.
	// �¶��� ���� �ļ��� �̿��Ͽ� �ۼ��Ǿ���.
	// ���ο� �Ѿ��� �߰��� ������ bullet.h���� �ش� �Ѿ� ����
	// �� ���� Kind ������� �����Ͽ��ָ�, InitBulletKindInfo()
	// �Լ����� �ʱ�ȭ �κ��� �߰��� �ش�. ���� InfoBullet.txt��
	// �� �߰����ش�.
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

	//ȭ�� �Ҳ�,��ϱ����� ����ϴ� �ҵ���.
	g_BKI[KIND_BULLET_VOLCANOFLAME].Init(	"KIND_BULLET_VOLCANOFLAME",
											BULLETATB_HITNORMAL|BULLETATB_FIREARROW,
											0);

	//�̻��� ����� ����ϴ� ��ǳ.
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
											BULLETATB_HITNORMAL|BULLETATB_MAGIC1|BULLETATB_RANGE1|BULLETATB_HALT,	// robypark 2004/9/1 16:33 ������ �̻��� ���� ���ݼӼ� �߰�
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
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - �����/����Ȳ�� ���� START
//----------------------------------------------------------------------------

	// actdoll (2004/03/06 4:39) : �̹��� �������� ���
	g_BKI[KIND_BULLET_PYTHON].Init		(	"KIND_BULLET_PYTHON",
											BULLETATB_HITNORMAL | BULLETATB_ARROW,
											0);

	// actdoll (2004/03/06 4:39) : ����Ȳ��ȥ �������� ���
	g_BKI[KIND_BULLET_JINEMPEROR].Init	(	"KIND_BULLET_JINEMPEROR",
											BULLETATB_HITNORMAL | BULLETATB_ARROW,
											BULLETSMOKEATB_SMOKE_JINEMPEROR);

//----------------------------------------------------------------------------
// actdoll (2004/03/04 15:04) : �߰� ĳ���� - �����/����Ȳ�� ���� END
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
// robypark (2004/05/18 15:28) : �߰� ĳ���� ��� bullet - ��� 2�� ���� ��� bullet
//-----------------------------------------------------------------------------------
	// ����-�ź���; ���̾� ��
	g_BKI[KIND_BULLET_TURTLE_FIREBALL].Init(	"KIND_BULLET_TURTLE_FIREBALL",
												BULLETATB_FIREARROW | BULLETATB_HITNORMAL | BULLETATB_RANGE1, \
												0);

	// ����-������; ���� ��Ʈ
	g_BKI[KIND_BULLET_THUNDER_BOLT].Init(	"KIND_BULLET_THUNDER_BOLT",
											BULLETATB_HITNORMAL | BULLETATB_ARROW, \
											BULLETSMOKEATB_SMOKE_THUNDER_BOLT);

	// �Ϻ�-�����; ��, ���� ����
	g_BKI[KIND_BULLET_POISON_ICE_BULLET].Init(	"KIND_BULLET_POISON_ICE_BULLET",
											BULLETATB_HITNORMAL | BULLETATB_POISONARROW | BULLETATB_RANGE1, \
											BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET);

	// �߱�-�߼���; �Һ��� ������
	g_BKI[KIND_BULLET_CATAPULT_FIRE_STONE].Init(	"KIND_BULLET_CATAPULT_FIRE_STONE",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_CATAPULT_FIRE_STONE);

	// robypark 2004-5-27, 17:42
	// �߱�-�Ҷ�������; �Ҷ�����ź
	g_BKI[KIND_BULLET_FARANGI_BULLET].Init(	"KIND_BULLET_FARANGI_BULLET",
											BULLETATB_HITNORMAL|BULLETATB_CANNON2C|BULLETATB_WATER,
											BULLETSMOKEATB_SMOKE_EXP6);

	// �븸-ȭ����; ȭ�����
	g_BKI[KIND_BULLET_FIRE_DRAGON_MISSILE].Init(	"KIND_BULLET_FIRE_DRAGON_MISSILE",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE);

	// �븸-��Ȳ����; ��Ȳ���� ���� �̻���
	g_BKI[KIND_BULLET_CHINESEPHOENIX_FEATHER].Init(	"KIND_BULLET_CHINESEPHOENIX_FEATHER",
													BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
													BULLETSMOKEATB_SMOKE_CHINESEPHOENIX_FEATHER);

	// 2�� ��� ��ų �߰� ����
	// robypark 2004-6-1 15:34
	// ��õ�� ��ź(�߱�-�߼���)
	g_BKI[KIND_BULLET_BOMB_OF_HEAVEN].Init(	"KIND_BULLET_BOMB_OF_HEAVEN",
											BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
											0);

	// �ݳ뿰�� ȭ��ź(�Ϻ�-������)
	g_BKI[KIND_BULLET_VOLCANO_BULLET].Init(	"KIND_BULLET_VOLCANO_BULLET",
											BULLETATB_HITNORMAL | BULLETATB_FIREARROW | BULLETATB_RANGE1, \
											BULLETSMOKEATB_SMOKE_VOLCANO_BULLET);

	
	// �븸-ȭ����; �Ͱ�ȭ���� ��ź
	g_BKI[KIND_BULLET_ONSLAUGHT_FIREBALL].Init(	"KIND_BULLET_ONSLAUGHT_FIREBALL",
												BULLETATB_RANGE1 | BULLETATB_HITNORMAL | BULLETATB_FIREARROW, \
												0);
	
	// robypark 2004-6-4 19:12
	// �Ҷ������� ��� ��õ��� �Ѿ�
	g_BKI[KIND_BULLET_ROAR_OF_LAUGHTER_BOMB].Init(	"KIND_BULLET_ROAR_OF_LAUGHTER_BOMB",
													BULLETATB_RANGE1 | BULLETATB_HITNORMAL | BULLETATB_CANNON2C, \
													BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB);

	// robypark 2004-6-4 19:12
	// �Ҷ������� ��� ��õ��� ���ݽ� ����
	g_BKI[KIND_BULLET_FRAGMENT4].Init(	"KIND_BULLET_FRAGMENT4",
										BULLETATB_FRAGMENT,
										0);

	// robypark 2004/7/14 16:46
	// ���� ���� ���� �Ѿ�
	g_BKI[KIND_BULLET_DOLL_EVIS].Init(	"KIND_BULLET_DOLL_EVIS",
										BULLETATB_HITNORMAL|BULLETATB_NEEDLE,
										0);

	/////////////////////////////////////////////////////////////////////////////////
	// robypark 2004/10/22 14:23
	// ������ �ʵ� �Ѿ� �߰�
	// ������ - �ú� ȭ��
	g_BKI[KIND_BULLET_SIEGEWARFARE_ARROW].Init(	"KIND_BULLET_SIEGEWARFARE_ARROW",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_SIEGEWARFARE_ARROW_HIT);

	// ������ - �߼��� ��
	g_BKI[KIND_BULLET_SIEGEWARFARE_STONE].Init(	"KIND_BULLET_SIEGEWARFARE_STONE",
										BULLETATB_HITNORMAL|BULLETATB_ARROW,
										BULLETSMOKEATB_SMOKE_SIEGEWARFARE_CATAPULT_STONE_HIT);
	/////////////////////////////////////////////////////////////////////////////////

	// robypark 2005/2/2 17:27
	// �޸� ���� ���� �Ѿ�
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
//	 id = �Ѿ�����  x, y = ��������(��Ʈ����)
//
//   mx, my = ��������� ����� ���������� �߰��� ����
//
//   dx, dy = ��������(��Ʈ����)
//
//   shooter_id = ����� targetid = �´¾��� 
//
//   apower = ���ݷ� -> ����̰� ������ ������ �������� ����
/////////////////////////////////////////////////////////////////
_Bullet::_Bullet()
{
	ZeroMemory(this, sizeof(_Bullet));

	// robypark (2004/5/21 18:44) ȭ�����, �߼��� �� ���� �� �ִϸ��̼� ������ ���� �߰�
	//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	//		���� ������ ��ŵ�� �ʿ��ϴ�.
	ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);
}


// void _Bullet::Init(...)
// SHORT kind: bullet ����
// SHORT id: Bullet ID
// SHORT sx: Bullet ���� ��ǥ(X)
// SHORT sy: Bullet ���� ��ǥ(Y)
// SHORT mx: Bullet �� ��ǥ(X)
// SHORT my: Bullet �� ��ǥ(Y)
// SHORT x: �������� ��ġ(x)
// SHORT y: �������� ��ġ(y)
// SHORT dx: ��ǥ�� ��ġ(x)
// SHORT dy: ��ǥ�� ��ġ(y)
// _NewID shooter_id: ������ ID
// _NewID targetid: ��ǥ�� ID
// SHORT apower: ���ݷ�
// SI32 hitrate: HIT Rate(100)
// SHORT targetdir: ��ǥ�� ����(0)
// SHORT attack_area: �������� (AREA_LAND, AREA_AIR)
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

	 // robypark (2004/5/21 18:44) ȭ�����, �߼����� �� ���� ��, �ִϸ��̼� ������ ���� �߰�
	 //		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
	 //		���� ������ ��ŵ�� �ʿ��ϴ�.
	 switch (m_ssKind)
	 {
		// ȭ�����
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// �߼��� �⺻ ����(�� ���� ��)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// �߼��� ��õ�� ��ź
		case KIND_BULLET_BOMB_OF_HEAVEN:
			// robypark (2004/5/21 18:44) �ִϸ��̼� ������ ���� �߰�
			//		�� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
			//		���� ������ ��ŵ�� �ʿ��ϴ�.
			ZeroMemory(m_snFrameIndex, MAX_SHOOT_RANGE);

			// �Ÿ��� �°� ������ ����
			CreateFrameIndex_4FireDragonMissile();
			break;

		default:
			break;
	 }

	 // �Ҷ������� ���� ����
	 m_snCurrentDelay = 0;
	 m_snMaxDelay = 0;

	// robypark (2004/6/12 12:27
	// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
	// �������ش�.
	m_bLayerAboveVolcano = false;
}

// robypark (2004/5/21 18:44) ȭ�����, �߼����� �� ���� �� �ִϸ��̼� ������ ���� �߰�
//		ȭ������� ��� �� �ѹ��� �ִϸ��̼��� �̷������ �ϸ�, �Ÿ���
//		���� ������ ��ŵ�� �ʿ��ϴ�.
//  �̵� �ܰ迡 ���� �ִϸ��̼� �������� �����Ѵ�.
void _Bullet::CreateFrameIndex_4FireDragonMissile(void)
{
	// ª�� �Ÿ��� ���
	if (m_ssMaxMoveStep <= 2)
	{
		m_snFrameIndex[0] = 0;
		m_snFrameIndex[1] = m_scMoveFrameNumber / 2;
		m_snFrameIndex[2] = m_scMoveFrameNumber - 1;
		return;
	}

	m_snFrameIndex[0] = 0;
	m_snFrameIndex[m_ssMaxMoveStep] = m_scMoveFrameNumber - 1;

	// �� ������(��Ʈ) �� �̵� �ܰ� ��
	int nStepPerFrame = (((float)m_ssMaxMoveStep / (float)m_scMoveFrameNumber) * 100);

	// ������ �������� ����� �ش�.
	for (SHORT i = 1; i < m_ssMaxMoveStep; i++)
	{
		m_snFrameIndex[i] = (i * 100) / nStepPerFrame;
	}
}

// robypark (2005/5/27 19:9)
// �Ҷ��������� 3�� ��ź ���� ����
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

	// �̵��� ȭ�ϰ� ��Ʈ 
	m_scMoveFrameNumber = g_BKI[m_ssKind].m_siMoveFrameNumber;


	// �� ���⺰�� �׸��� �غ�Ǿ� �ִ�. 
	if(g_BKI[m_ssKind].m_bDirSwitch == TRUE)
	{
		SetMoveImage16(g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont, m_scMoveFrameNumber);
	}
	// ��� ������ ���� �׸��� ����Ѵ�. 
	else
	{
		for(i=0;i<9;i++)
			SetMoveImage(i, g_BKI[m_ssKind].m_ssFile, g_BKI[m_ssKind].m_ssFont);
	}

}

BOOL _Bullet::Hit()
{


	// �������� ���� ȿ���� �ִ� �Ѿ��̸� ���� ���̿� ���� 10%�� ������ ���ݷ��� �����ش�. 
	if(m_ssHeightSwitch==TRUE)
	{
		// �߻������� ���̸����Ѵ�. 
	    SHORT startlevel = GD.LastLevel[m_ssMapSX][m_ssMapSY];

		// ���������� ���̸� ���Ѵ�. 
		SHORT endlevel = GD.LastLevel[m_ssMapDX][m_ssMapDY];

		// ���� ���� ���� ���ݷ��� �������ش�. 
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

	// ������ ȭ���� �׸���. 
	id=FindEmptySmoke();

	// ��ǥ�� ���Ѵ�. 
	if(clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssMapDX, &m_ssMapDY)==TRUE)
	{
	   if(id)
	   {
		// ���� ��������, 
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
		// ���ݷ��� ��������, 
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

	// ȿ���� ó�� 
	if(g_BKI[m_ssKind].m_siHitEffectSoundIdx)	// ȿ������ �ִٸ�
	{
		// ����. Hash Table �̿��� ����
		HashCodePushEffect(g_BKI[m_ssKind].m_siHitEffectSoundIdx, m_ssX, m_ssY);
	}

	// ������ ȭ���� �׸���. 
	smokeid=FindEmptySmoke();

	// ���� ��������, 
	if(g_BKI[m_ssKind].m_siAtb & BULLETATB_WATER && m_scArea==AREA_LAND && clifMap.IsColl(m_ssMapDX, m_ssMapDY, BLOCK_DEEPWATER) && clifMap.GetIDMap(m_ssMapDX, m_ssMapDY)==0 )
	{
		
		SetSmoke(KIND_SMOKE_WATEREXP, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
	}
	else
	{
		// ���� �Ӽ�. 
		if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_SOULCHANGE)
		{
			// robypark 2004/9/13 13:32
			// ���̸��� �⺻���� ��Ʈ ȿ�� �߰�
			// ���ڰ� ����ũ ����Ʈ ���
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

			// ��ȣ��[��] ���ݿ� ���� smoke�׷��ֱ�
			SetSmoke(KIND_SMOKE_GUARDIAN_TIGER,smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
			PushEffect(EFFECT_GUARDIAN_TIGER_END, m_ssX, m_ssY);

			// robypark 2004/6/23 16:28
			// ���߷��� �ٰŷ� Ÿ�� ���θ� �����Ѵ�.
			// ��ȣ��[��]�� ���ݿ� ���߷� ������ �ǵ��� ����.
			if ((GD.Random() % 100) > (UI32)m_ssHitRate)
				return TRUE;	// ������ ��ȣ��[��]�� ������ �������� �ʾҴ�.

			if(CR[m_AttackID.ID].ReduceLife(CR[m_AttackID.ID].m_ParmeterBox.IP.GetMaxLifeByIP()/2,10) == FALSE)
			{
				//��ȣ��[��]�� �°� ĳ���Ͱ� �׾��� ���.
				// ĳ���� ������ ������ �뺸�Ѵ�. 
				if(OBInfo.IsSingleMode == TRUE)
				{
					// ������ ���� �������� �׿����� �뺸�Ѵ�. 
					if(IsAlive(m_ShooterID))
					{
						// �������� �ƴϾ�� �Ѵ�. 
						if(pGame->pOnlineWorld->CheckPKFieldMap() == FALSE)
						{
							OBMQue.SendBattleServerGameMsg_ONLINE_BATTLE_MSG_KILL(
								MyNet.PlayersdpId.dpId[ CR[m_ShooterID.ID].GetPlayerNumber()],CR[m_ShooterID.ID].m_siOnlineID, 
								MyNet.PlayersdpId.dpId[ CR[m_AttackID.ID].GetPlayerNumber()],	CR[m_AttackID.ID].m_siOnlineID);
							
							// ų������ �մ��� ����ġ���� �����Ѵ�. 
							pGame->pBattle->ExpOperate(m_ShooterID.ID, m_AttackID.ID);		
						}
					}
				}
				// robypark 2004/7/9 ��ȣ��[��]�� ���� ĳ���͵��� ��Ƽ�� �ΰ� ������ ��ȣ��[��] ��� ȿ����
				// ����� �ߵ����� �ʾҴ�. �̴� �� else���� ��ġ�� if(IsAlive(ShooterID))������ else������ 
				// �߸� �ٿ��ֱ⸦ �Ͽ� ���� ��������.
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

				//���� �����Ͽ��°��� ����.
				CR[AttackID.ID].m_bRabbitAttack   = true;
				//������ ���(��ȣ�� �䳢)�� ���� ���̵� �����Ѵ�.
				CR[AttackID.ID].m_RabbitShooterId = CR[ShooterID.ID].HostID.ID;

				//���ݵ����̿� ��������̸� ���δ�.
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
		
		// actdoll (2004/03/06 10:03) : �����/����Ȳ�� ����
		else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_JINEMPEROR)		// ����Ȳ��ȥ �������� ��Ʈ
		{
			SetSmoke(KIND_SMOKE_JINEMPEROR_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}

		// robypark (2004/5/19, 13:32) : ����(����-������, ���� 2�� ����)�� �¾��� ���
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_THUNDER_BOLT)
		{
			// ������ ���ݿ� �¾��� ����� ��Ʈ ȿ���� �����ش�.
			SetSmoke(KIND_SMOKE_THUNDER_BOLT_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}

		// robypark (2004/5/19, 16:50) : ȭ�����(�븸-ȭ����, ����â 2�� ����)�� �¾��� ���
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_FIRE_DRAGON_MISSILE)
		{
			SetSmoke(KIND_SMOKE_FIRE_DRAGON_MISSILE_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}		

		// robypark (2004/5/19, 16:50) : ��, �ñ�(�Ϻ�-�����, ���̸��� 2�� ����)�� �¾��� ���
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_POISON_ICE_BULLET)
		{
			SetSmoke(KIND_SMOKE_POISON_ICE_BULLET_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);
		}		

		// robypark (2004/5/21, 16:15) : �߼��� ����-�Һ��� ��(�߱�-�߼���, ���� 2�� ����)�� �¾��� ���
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
		// robypark 2004/6/1 17:47 : �ݳ뿰�� ȭ��ź ��Ʈ ȿ��(�Ϻ�-������)
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_VOLCANO_BULLET)
		{
			SetSmoke(KIND_SMOKE_VOLCANO_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND, -1, TRUE);
		}
		// robypark 2004/6/4 19:19 : ��õ��� ��ź ��Ʈ ȿ��
		else if (g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_SMOKE_ROAR_OF_LAUGHTER_BOMB)
		{
			SetSmoke(KIND_SMOKE_ROAR_OF_LAUGHTER_HIT, smokeid, m_ssRealX, m_ssRealY, AREA_LAND);

			// ������ �����Ѵ�.
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
	// �¾��� �� ��� ���ְ� �Ǵ� �Ӽ��̸�, 
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
	// ��ȭ���� ���, 
	else if(g_BKI[m_ssKind].m_siAtb & BULLETATB_FIREARROW)
	{
		attackatb = ATTACKATB_FIREARROW;

		if(GD.Random() % 3 == 0)
		{
			_NewID zeroID;
			zeroID.ID = -1;
			zeroID.Unique = 0;
			// ���ٴڿ� ���� �ٿ��ش�. 
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
		 // ������ ���ؼ� ������ ������ �ʵ��� ZeroID�� �ִ´�. 
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
	// 2�� ����� �µ��� ���ݼӼ��� �����Ѵ�.
	switch (m_ssKind)
	{
		// ����-�ź���; ���̾� ��
		case KIND_BULLET_TURTLE_FIREBALL:
			attackatb = ATTACKATB_TURTLE_VEHICLE;	// ����, ���÷��� ��
			break;

		// ����-������; ���� ��Ʈ
		case KIND_BULLET_THUNDER_BOLT:
			attackatb = ATTACKATB_THUNDER_VEHICLE;	// ����

			// robypark 2004/6/11 10:41
			// ���� ������ ������������ �ش�. ������ ���� �⺻����(����������)�� �޸�
			if (m_ssCurrentOrder == ORDER_BULLET_STRAIGHT)
				attackatb = ATTACKATB_MAGIC;	// ����
			break;

		// �Ϻ�-�����; ��, ���� ����
		case KIND_BULLET_POISON_ICE_BULLET:
			attackatb = ATTACKATB_BLACK_DRAGON_VEHICLE;	// ����, ���÷��� ��, ��, �ñ� ����
			break;

		// �߱�-�߼���; �Һ��� ������
		case KIND_BULLET_CATAPULT_FIRE_STONE:
			attackatb = ATTACKATB_CATAPULT;	// ����, ���÷��� ��
			break;

		// �߱�-�Ҷ�������; �Ҷ�����ź
		case KIND_BULLET_FARANGI_BULLET:
			attackatb = ATTACKATB_FARANGI_VEHICLE;	// ����
			break;

		// �븸-ȭ����; ȭ�����
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
			attackatb = ATTACKATB_FIRE_DRAGON_VEHICLE;	// ����, �������� ��
			break;

		// robypark 2004/9/1 16:58 ������ �̻��� ���Ÿ� ���ݿ� ���� ���ݼӼ� �߰�
		case KIND_BULLET_SCORPIONBULLET:
		// �븸-ȭ����; �Ͱ�ȭ���� ��ź
		case KIND_BULLET_ONSLAUGHT_FIREBALL:
			attackatb = ATTACKATB_FIREATTACK;	// ����, ���÷��� ��
			break;

		// �븸-��Ȳ����; ��Ȳ���� ���� �̻���
		case KIND_BULLET_CHINESEPHOENIX_FEATHER:
			attackatb = ATTACKATB_CHINESEPHOENIX;	// ����, ���÷��� ��
			break;

		// robypark 2004/6/11 10:46
		// �ݳ뿰�� ȭ��ź(�Ϻ�-������)�� ������������ �ش�.
		case KIND_BULLET_VOLCANO_BULLET:
			attackatb = ATTACKATB_FIREATTACK;	// ����, ���÷��� ��
			break;

		// �Ҷ������� ��� ��õ��� �Ѿ�
		case KIND_BULLET_ROAR_OF_LAUGHTER_BOMB:
			attackatb = ATTACKATB_ROAR_OF_LAUGHTER;	// ����, ���÷��� ��

			// ��õ��� �Ѿ� ���÷��� ����
			range = 2;
			break;

		default:
			break;
	}

	// �Ͱ�ȭ���ĸ� ���� üũ
	// �Ǿ�� �Ӽ��� �Ѿ��� ��� �Ѿ��� �̵��ϴ� �������� �浹üũ�� �׻��Ѵ�.
	if (m_ssCurrentOrder == ORDER_BULLET_PIERCE)
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssX, m_ssY, m_ssHitRate);
	// ������ ���� ������ üũ
	else if (m_ssCurrentOrder == ORDER_BULLET_STRAIGHT)
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssX, m_ssY, m_ssHitRate);
	else
		CalculateHit(m_ShooterID, m_ssShooterPlayer, attackatb, range, m_scArea, m_ssApower, m_AttackID, m_ssMapDX, m_ssMapDY, m_ssHitRate);

	return TRUE;
}


//
// �Ѿ��� ������ �ִ´�.
//

void _Bullet::SetInfo()
{
	GD.BulletInfo[m_ssID]=m_ssKind;
}

//
// �ѹ� ������ ĳ���͸� �ٽ� �������� �ʱ� ���ؼ�.
// ������ �ʱ�ȭ�Ѵ�. 
//

void _Bullet::InitPushID()
{
	m_ssHitIDIndex=0;
}

//
// id = �Ѿ��� id
// 
//

void _Bullet::PushHitID(_NewID id)
{

	m_HitID[m_ssHitIDIndex]=id;

	if(m_ssHitIDIndex<(MAX_HIT_ID-1))
	   m_ssHitIDIndex++;
}

//
// id = �Ѿ��� id
// �Ѿ��� ��ǥ�� �¾ҳ�?
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
// ���±��� ����(�)
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
// ���±��� ����(����)  .     .     .  -> �̷���...
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
		if(x2>x1 && y1>y2 || x2>x1 && y1<y2)			// 1, 4��и�
		{
			y2 += (float)(y2-y1) / (x2-x1) * 60;
			x2 += 60;
		}
		else if(x2<x1 && y1>y2 || x2<x1 && y1<y2)		// 2, 3��и�
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
// ���� ��ġ�� ����Ѵ�. 
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

	// X, Y��ǥ�� ���Ѵ�. 
	clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);


	m_ssCenterX=m_ssRealX;
	m_ssCenterY=m_ssRealY;


	////////////////////////////////////////////	
	// ����� �߻�� �Ѿ��� ��� �׸��ڴ� �������� �̵������� 
	// �Ѿ� �ڽ��� �������� �׸���. 
	// ��ġ�� ���� ������ ��ӹٲ��. 

	if(m_ssCurveSwitch==TRUE)
	{
	SHORT incy;
	SHORT vary;

	// ������ �Ѿ� ��ġ�� ���Ѵ�. 
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
	// �̹����� �غ���� ���� ���� �׸��� �ʴ´�. 
	if(m_ssFile == 0)return FALSE;


	unsigned char *darktable;
	// ��� �ܰ踦 �����Ѵ�. 
	SHORT darkstep=GD.GetBlackMap2(m_ssX, m_ssY);
	darkstep = darkstep * (MAX_DARK_STEP - 1)/ FOG2_BLACK_STEP ;

	darktable = TeamDarkTable[darkstep];

	if(m_ssDrawSwitch==FALSE)
		return FALSE;

	if(m_ssCurveSwitch==TRUE)
	{

		// �׸��� ��� 
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

		// ���� ȭ�� ��� 
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
	// � ź���� �ƴҶ� 
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
// ���� �� �׸��� �����Ѵ�. 
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
// ������ ������ ��Ʈ�� �ҷ��´�. 
//

void _Bullet::DecideFontMove()
{
	switch(m_ssKind)
	{
		case KIND_BULLET_FIRETANKARROW:
				DecideFontMoveFireTankArrow();
				break;

		// ȭ�����
		case KIND_BULLET_FIRE_DRAGON_MISSILE:
		// �߼��� �⺻ ����(�� ���� ��)
		case KIND_BULLET_CATAPULT_FIRE_STONE:
		// �߼��� ��õ�� ��ź
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

			SetMessage("�߸��� �Ѿ� ��Ʈ�� ���� �ֽ��ϴ�.(Error Bullet Fonts!)");

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
// ������ ���� �Ѵ�.
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
// �Ѿ��� �̵�������.
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
	// �Ѿ��� �۵� �߰� : robypark(2004/5/19, 18:48)
	//------------------------------------------------------------------------------------------------
	// ź���Ǿ��� �� Ư������� �����Ѵ�.
	// ��õ��(�߱�-�߼���)�� ���� �۵�
	case ORDER_BULLET_EVENT_IMPACT:
		{
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// ��ǥ�� �����Ѵ�. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// ź���Ǿ��� �� Ư������� �����Ѵ�.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				// ��õ�� ����� ����Ѵ�
				SetMagic_BOMB_OF_HEAVEN(m_ssMapDX, m_ssMapDY, \
							m_ShooterID, m_ssShooterPlayer, AREA_LAND, \
							m_ssApower, m_ssHitRate, m_AttackID);

				return FALSE;
			}

			m_ssMoveStep++;
		}
		break;

	//------------------------------------------------------------------------------------------------
	// �Ѿ��� �۵� �߰� : robypark(2004/5/19, 13:06)
	//------------------------------------------------------------------------------------------------
	// �Ǿ��(����)�Ǵ� �Ѿ� �۵�(�� �� ���� ����)
	// �Ѿ� �̵� �� �Ѿ��� �ִ� ���� ���� ���ֿ� �������� �ش�.
	// �Ͱ�ȭ���ĸ� ���� �Ѿ� �̵�
	case ORDER_BULLET_PIERCE:
		{
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// ��ǥ�� �����Ѵ�. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// X, Y��ǥ�� ���Ѵ�. 
			clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);

			// �Ѿ� ��ġ�� �����ϴ� �� ĳ���Ϳ��� �������� �ش�.
			SI32 siID = Map.GetIDMap(m_ssX, m_ssY);
//			if ((IsAlive(ID))
//				&& (IsFriendToByPlayerNumber(ShooterPlayer, CR[ID].GetPlayerNumber()) == FALSE))
			{
				m_AttackID = CR[siID].m_NewID;

				// �� ĳ���Ϳ��� �������� �ش�.
//				Hit();
			}

			// �� ĳ���Ϳ��� �������� �ش�.
			Hit();

			// �Ѿ��� �� ������ �����ߴٸ� �Ѿ� ��ü �Ҹ��� ����Ѵ�.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				return FALSE;
			}

			m_ssMoveStep++;
		}
		break;

	//------------------------------------------------------------------------------------------------
	// �Ѿ��� �۵� �߰� : robypark(2004/5/19, 13:06)
	//------------------------------------------------------------------------------------------------
	// �������� ���� �� �������� �̵��ϴ� �Ѿ� �۵�
	// �Ѿ� �̵� �� �Ѿ��� �ִ� ���� ���� ���ֿ� �������� �ش�.
	// �������� �� ���� �Ѿ��� �Ҹ��Ѵ�.
	// ������ ���� �Ѿ� �̵�
   case ORDER_BULLET_STRAIGHT:
	   {
			m_ssAnimation=ANI_BULLET_MOVE;

			if(GD.Frame % 2 == 0)
			{
				// ���� ���� ������ ����ģ ���� ���������� �߻�ǵ��� �����Ѵ�.
				if (m_ssKind == KIND_BULLET_THUNDER_BOLT)
				{
					// ���ð��� �����°�?
					if (m_snCurrentDelay < m_snMaxDelay)
					{
						m_snCurrentDelay++;
						return TRUE;
					}

					// ���ð��� �������Ƿ� �׷��ֵ��� �����Ѵ�.
					m_ssDrawSwitch = TRUE;
				}

				++m_ssAniStep %= m_scMoveFrameNumber;
			}

			// ��ǥ�� �����Ѵ�. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			// X, Y��ǥ�� ���Ѵ�. 
			clifMap.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &m_ssX, &m_ssY);
			// �Ѿ� ��ġ�� �����ϴ� �� ĳ���Ϳ��� �������� �ش�.
//			SHORT siTileX = 0, siTileY = 0;
//			Map.GetRealTileByRealDot(m_ssRealX, m_ssRealY, &siTileX, &siTileY);
			SI32 ID = Map.GetIDMap(m_ssX, m_ssY);
			if ((IsAlive(ID))
				&& (IsFriendToByPlayerNumber(m_ssShooterPlayer, CR[ID].GetPlayerNumber()) == FALSE))
			{
				m_AttackID = CR[ID].m_NewID;

				// �� ĳ���Ϳ��� �������� �ش�.
				Hit();

				return FALSE;
			}

			// �Ѿ��� �� ������ �����ߴٸ� �Ѿ� ��ü �Ҹ��� ����Ѵ�.
			if (m_ssMoveStep == m_ssMaxMoveStep)
			{
				return FALSE;
			}

			m_ssMoveStep++;
	   }
	   break;

	   // ����ȭ����ǳ
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
				   // ȭ���� �ִ� ���� ������ ���Ѵ�. 

				   SHORT temp_id = clifMap.GetIDMap(x, y);
				   
				   // ������ ���̸� Ÿ���� �ش�. 
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

	   // ������ �� �Ǿ���. 
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
					   // �ٴڿ� ���� �ٿ��ش�.
					   if(GD.Random() % 25 == 0)
					   {
						   clifMap.PutFire(x, y, 1, GROUNDFIRE_LIFETIME,m_ShooterID,m_ssShooterPlayer);
					   }

					   // ȭ���� �ִ� ���� ������ ���Ѵ�. 
					   SHORT temp_id = clifMap.GetIDMap(x, y);

					   // ������ ���̸� Ÿ���� �ش�. 
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

	   // ������ �� �Ǿ���. 
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
					   // �ٴڿ� ���� �ٿ��ش�.
					   if(GD.Random() % 25 == 0)
					   {
						   clifMap.PutFire(x, y, 1, GROUNDFIRE_LIFETIME,m_ShooterID,m_ssShooterPlayer);
					   }

					   // ȭ���� �ִ� ���� ������ ���Ѵ�. 
					   SHORT temp_id = clifMap.GetIDMap(x, y);

					   // ������ ���̸� Ÿ���� �ش�. 
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

			// ���ð��� �����°�?
			if (m_snCurrentDelay < m_snMaxDelay)
			{
				m_snCurrentDelay++;
				return TRUE;
			}

			// ���ð��� �������Ƿ� �׷��ֵ��� �����Ѵ�.
			m_ssDrawSwitch = TRUE;

			if(GD.Frame % 2 == 0)
			{
				++m_ssAniStep %= m_scMoveFrameNumber;
			}

		   // ����ź�� ��쿡, 
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

		   // ��ǥ�� �����Ѵ�. 
			m_ssRealX=m_ssCourseX[m_ssMoveStep];
			m_ssRealY=m_ssCourseY[m_ssMoveStep];

			if(m_ssMoveStep==m_ssMaxMoveStep)
			{
#ifdef _IMJIN2
				// �ٽ� ��ǥ�Ḧ Ž���ϱ� �����Ѵ�. 

				if(m_ssKind==KIND_BULLET_AIRMAGIC)
				{
				    min_length=10000;
				    min_id.Init();

				    for(i=0;i<GD.AirCharNumber;i++)
					{
			            id=GD.AirCharID[i];
			            if(GD.CharInfo[id.ID])
						{
					        // �Ÿ��� ������ 
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

					// �����̿��� ���� ĳ���͸� ã����, 
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

				// ������ ������ �Ѵٸ�, 
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

					   // ������ ��� , 
					   if(m_ssKind==KIND_BULLET_SHIPCANNON)
					   {
					     if(m_ssMoveStep%3==0)
					         SetSmoke(KIND_SMOKE_SMOKE0, smoke_id, smokex, smokey, m_scArea);
					   }
					   // ������ ��ź�� ��� 
					   if(m_ssKind==KIND_BULLET_THROWBOMB)
					   {
					     if(m_ssMoveStep%3==0)
					         SetSmoke(KIND_SMOKE_SMOKE0, smoke_id, smokex, smokey, m_scArea);
					   }
					   // ȭ�� ������  ��� , 
					   else if(m_ssKind==KIND_BULLET_FIRE)
					   {
					     if(m_ssMoveStep%2==0)
					         SetSmoke(KIND_SMOKE_FIRE, smoke_id, smokex + 8 - (GD.Random()%16), smokey + 8 - (GD.Random()%16),  m_scArea);
					   }
					   // ��� �巡���� ������ ���, 
					   else if(m_ssKind==KIND_BULLET_GOLDDRAGONELECTRON)
					   {
					     if(m_ssMoveStep%2==0)
					         SetSmoke(KIND_SMOKE_GOLDDRAGONELECTRON, smoke_id, smokex + 8 - (GD.Random()%16), smokey + 8 - (GD.Random()%16),  m_scArea);
					   }
					   else if(m_ssKind==KIND_BULLET_FIREARROW)
					   {

						   // ���⸦ ǥ���Ѵ�. 
						   if(m_ssMoveStep%3==0)
				              SetSmoke(KIND_SMOKE_SMOKE10, smoke_id, smokex, smokey, m_scArea);
					   }
					   else if(m_ssKind==KIND_BULLET_CANNON)
					   {

						   // ���⸦ ǥ���Ѵ�. 
						   if(m_ssMoveStep%2==0)
				              SetSmoke(KIND_SMOKE_SMOKE2, smoke_id, smokex, smokey, m_scArea);

					   }
					   else if(m_ssKind==KIND_BULLET_AIRMISSILE)
					   {
						   // ���⸦ ǥ���Ѵ�. 
						   if(m_ssMoveStep%3==0)
				              SetSmoke(KIND_SMOKE_SMOKE1, smoke_id, smokex, smokey, m_scArea);
					   }

					   else if(g_BKI[m_ssKind].m_siSmokeAtb & BULLETSMOKEATB_TRAIL_ARROWTRAIL)
					   {
						   // ���⸦ ǥ���Ѵ�. 
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
// �Ҷ������� ��� ��õ��� ��ź ���߽� ������ �������ش�.
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

	// �������� ������ �����Ѵ�.
	for (i = 0; i < siMaxFragment; i++)
	{
		vary = 5 - GD.Random() % 8;

		attackx = firex + vary + 2;
			
		vary = 5 - GD.Random() % 8;
			
		attacky = firey + vary + 2;

		// �����Ϸ��� ������ ��ǥ���� �� �ȿ� �ִٸ�
		if(Map.IsInMapArea(attackx, attacky) == TRUE)
		{
			// ���� ����
			siBulletID = FindEmptyBullet();
			if (siBulletID)
			{
				switch (GD.Random() & 3)
				{
					case 0:
						// ���� ���� �������� ����.
						SetBullet(KIND_BULLET_FRAGMENT1, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 1:
						// ���� ���� �������� ����.
						SetBullet(KIND_BULLET_FRAGMENT2, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 2:
						// ���� ���� �������� ����.
						SetBullet(KIND_BULLET_FRAGMENT3, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
					case 3:
						// ���� ���� �������� ����.
						SetBullet(KIND_BULLET_FRAGMENT3, siBulletID, firex, firey, attackx, attacky, 
										MapCenterX[firex][firey], MapCenterY[firex][firey], 
										MapCenterX[attackx][attacky], MapCenterY[attackx][attacky], \
										ZeroID, ZeroID, 0, 0, 0, AREA_LAND);
						break;
				}

				// �Ѿ� �߻翡 ������ �α� ���� �۾�
				Bullet_SetLuanchDelay(siBulletID, GD.Random() & 3);
				Bullet_DrawSwitch(siBulletID, FALSE);
			}
		}
	}
}


// robypark (2004/6/12 12:27
// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
// �������ش�.
void _Bullet::SetLayerAboveVolcano(bool bAbove/* = true*/)
{
	m_bLayerAboveVolcano = bAbove;
}








//
// ����ִ� �Ѿ˰�ü�� �˷���
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
// �Ѿ� ���̱�
//

void DeleteBullet(SHORT id)
{
	GD.BulletInfo[id]=0;
}

//
// ��� �Ѿ� ���̱�
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
// �Ҷ������� 3�߻� ���� �߻� ���� �Լ�
void Bullet_SetLuanchDelay(int i, SHORT snMaxDelay)
{
	g_Bullet[i].SetLuanchDelay(snMaxDelay);
}

// robypark (2004/6/12 12:27
// �ݳ뿰���� ���Ͽ� �����Ǵ� �Ѿ��� ȭ��(Smoke) ��(��)�� �׷����� ��� 
// �������ش�.
void SetLayerAboveVolcano(SI32 siBulletID, bool bAbove/* = true*/)
{
	g_Bullet[siBulletID].SetLayerAboveVolcano(bAbove);
}

/////////////////////////////////////