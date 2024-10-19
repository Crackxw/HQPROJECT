// SiegeUnitTableMgr.cpp: implementation of the CSiegeUnitTableMgr class.
//
//////////////////////////////////////////////////////////////////////

#include	<GSL.h>
#include	"GSCDefine.h"

#include	"..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include	"bindJXFile.h"
#include	"OnlineCommonParser.h"					// 공용 파서 시스템
#include	<myfile.h>
#include	"SiegeUnitTableMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeUnitTableMgr::CSiegeUnitTableMgr()
{

}

CSiegeUnitTableMgr::~CSiegeUnitTableMgr()
{
	Free();
}

// 메모리 해제
void	CSiegeUnitTableMgr::Free(void)
{
	m_vecSiegeUnitTableMgr.clear();
	m_vecSiegeUnitCalcHPTableMgr.clear();
	m_vecSiegeUnitCalcMPTableMgr.clear();
}

// HQ의 SiegeUnitTable.txt 파일 읽기
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTable(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTable.txt";

	// 파일을 열고
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		char szCharname[128];
		char szAnimationResourceID_Blue[64], szAnimationResourceID_Red[64];

		SSiegeUnitTable	sSiegeUnitTable = {0, };
		
		// 자료 받기 개시
		ocp.GetValue( (char &)sSiegeUnitTable.m_siID,					iLineNo, iColNo );	// 공성 유닛 배열 인덱스
		ocp.GetValue( szCharname,							127,		iLineNo, iColNo );	// 이름(사용안함)
		ocp.GetValue( (short &)sSiegeUnitTable.m_uiKind,				iLineNo, iColNo );	// 공성 유닛 온라인 Kind
		ocp.GetValue( sSiegeUnitTable.m_siUnitNameTextIdx,				iLineNo, iColNo );	// 공성전 유닛 이름 텍스트 인덱스
		ocp.GetValue( sSiegeUnitTable.m_siArmyKindDescTextID,			iLineNo, iColNo );	// 공성전 유닛 설명 텍스트 인덱스
		ocp.GetValue( (short &)sSiegeUnitTable.m_siPopulation,			iLineNo, iColNo );	// 병력 모집할 때 필요한 인구수
		ocp.GetValue( (short &)sSiegeUnitTable.m_siPrice,				iLineNo, iColNo );	// 병력 모집시 비용
		ocp.GetValue( (char &)sSiegeUnitTable.m_scTransformOption,		iLineNo, iColNo );	// 공성 유닛 캐릭터 속성
		ocp.GetValue( (char &)sSiegeUnitTable.m_scTransformLevelLimit,	iLineNo, iColNo );	// 공성 유닛 변신 레벨 제한
		ocp.GetValue( sSiegeUnitTable.m_siTransformCost,				iLineNo, iColNo );	// 공성 유닛 변신 비용
		ocp.GetValue( sSiegeUnitTable.m_siBasicHP,						iLineNo, iColNo );	// 공성 유닛 기본 체력
		ocp.GetValue( sSiegeUnitTable.m_siBasicMP,						iLineNo, iColNo );	// 공성 유닛 기본 마력
		ocp.GetValue( szAnimationResourceID_Blue,			63,			iLineNo, iColNo );	// 공성 유닛 아군일 때 이미지
		ocp.GetValue( szAnimationResourceID_Red,			63,			iLineNo, iColNo );	// 공성 유닛 적군일 때 이미지

		if (strcmp("NULL", szAnimationResourceID_Blue) != 0)
			sSiegeUnitTable.m_siAnimationResourceIdx_Blue	=	GetValueOfGameSprNumID(szAnimationResourceID_Blue);
		else
			sSiegeUnitTable.m_siAnimationResourceIdx_Blue	= -1;

		if (strcmp("NULL", szAnimationResourceID_Red) != 0)
			sSiegeUnitTable.m_siAnimationResourceIdx_Red	=	GetValueOfGameSprNumID(szAnimationResourceID_Red);
		else
			sSiegeUnitTable.m_siAnimationResourceIdx_Red	= -1;

		// 공성 변신 인터페이스 애니메이션 버튼 파일. 대기. Normal, Hover
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_szAnimationFilename,	127,	iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont,			iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionFontNumber,		iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAnimationDelay,				iLineNo, iColNo );

		// 공성 변신 인터페이스 애니메이션 버튼 파일. 공격. Pressed
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_szAnimationFilename,	127,	iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionStartFont,			iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionFontNumber,		iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAnimationDelay,				iLineNo, iColNo );

		// robypark 2005/2/15 16:32
		ocp.GetValue( (char &)sSiegeUnitTable.m_scMaxMoveActionSkip,		iLineNo, iColNo );	// 이동 처리 함수 Move() 딜레이 최대. 프레임 skip 효과

		// 초상화 이미지 파일명
		ocp.GetValue( sSiegeUnitTable.m_szPortraitFileName,		255,	iLineNo, iColNo );

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// 리스트에 추가
		m_vecSiegeUnitTableMgr.push_back( sSiegeUnitTable );	
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// HQ의 SiegeUnitTableCalcHP.txt 파일 읽기
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTableCalcHP(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTableCalcHP.txt";

	// 파일을 열고
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SSiegeUnitCalcTable	sSiegeUnitCalcTable = {0, };
		
		// 자료 받기 개시
		ocp.GetValue( sSiegeUnitCalcTable.m_siMinBattlePower,		iLineNo, iColNo );	// 최소 전투력
		ocp.GetValue( sSiegeUnitCalcTable.m_siMaxBattlePower,		iLineNo, iColNo );	// 최대 전투력
		ocp.GetValue( sSiegeUnitCalcTable.m_siIncreaseValue,		iLineNo, iColNo );	// 상승치

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// 리스트에 추가
		m_vecSiegeUnitCalcHPTableMgr.push_back( sSiegeUnitCalcTable );	
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// HQ의 SiegeUnitTableCalcMP.txt 파일 읽기
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTableCalcMP(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTableCalcMP.txt";

	// 파일을 열고
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// 파싱을 시작한다.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// 자료 검색
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// 데이터 라인 추출

		SI32	iLineNo, iColNo;

		// 자료를 순차적으로 받을 준비를 한다.
		SSiegeUnitCalcTable	sSiegeUnitCalcTable = {0, };
		
		// 자료 받기 개시
		ocp.GetValue( sSiegeUnitCalcTable.m_siMinBattlePower,		iLineNo, iColNo );	// 최소 전투력
		ocp.GetValue( sSiegeUnitCalcTable.m_siMaxBattlePower,		iLineNo, iColNo );	// 최대 전투력
		ocp.GetValue( sSiegeUnitCalcTable.m_siIncreaseValue,		iLineNo, iColNo );	// 상승치

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// 리스트에 추가
		m_vecSiegeUnitCalcMPTableMgr.push_back( sSiegeUnitCalcTable );	
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// 캐릭터 Online Kind에 맞는 LPSSiegeUnitTable얻기
LPSSiegeUnitTable	CSiegeUnitTableMgr::GetSiegeUnitTable(SI32 siOnlineCharKind)
{
	for( vector<SSiegeUnitTable>::iterator i = m_vecSiegeUnitTableMgr.begin(); i != m_vecSiegeUnitTableMgr.end(); ++i)
	{
		if (i->m_uiKind == siOnlineCharKind)
			return i;
	}

	return NULL;
}

// 배열 인덱스에 맞는 LPSSiegeUnitTable얻기
LPSSiegeUnitTable	CSiegeUnitTableMgr::GetSiegeUnitTableByArrayIdx(SI32 siArrayIdx)
{
	for( vector<SSiegeUnitTable>::iterator i = m_vecSiegeUnitTableMgr.begin(); i != m_vecSiegeUnitTableMgr.end(); ++i)
	{
		if (i->m_siID == siArrayIdx)
			return i;
	}

	return NULL;
}

// 전투력에 따른 체력 상승치 얻기
SI32	CSiegeUnitTableMgr::GetIncreaseHP(SI32 siBattlePower)
{
	for( vector<SSiegeUnitCalcTable>::iterator i = m_vecSiegeUnitCalcHPTableMgr.begin(); i != m_vecSiegeUnitCalcHPTableMgr.end(); ++i)
	{
		if ((i->m_siMinBattlePower <= siBattlePower)
			&& (i->m_siMaxBattlePower >= siBattlePower))
		{
			return i->m_siIncreaseValue;
		}
	}

	// 최대 비교 전투력보다 큰 경우 검사
	SSiegeUnitCalcTable &CalcTable = m_vecSiegeUnitCalcHPTableMgr.back();

	if (CalcTable.m_siMaxBattlePower < siBattlePower)
	{
		return CalcTable.m_siIncreaseValue;
	}

	return 0;
}

// 전투력에 따른 마력 상승치 얻기
SI32	CSiegeUnitTableMgr::GetIncreaseMP(SI32 siBattlePower)
{
	for( vector<SSiegeUnitCalcTable>::iterator i = m_vecSiegeUnitCalcMPTableMgr.begin(); i != m_vecSiegeUnitCalcMPTableMgr.end(); ++i)
	{
		if ((i->m_siMinBattlePower <= siBattlePower)
			&& (i->m_siMaxBattlePower >= siBattlePower))
		{
			return i->m_siIncreaseValue;
		}
	}

	// 최대 비교 전투력보다 큰 경우 검사
	SSiegeUnitCalcTable &CalcTable = m_vecSiegeUnitCalcMPTableMgr.back();

	if (CalcTable.m_siMaxBattlePower < siBattlePower)
	{
		return CalcTable.m_siIncreaseValue;
	}

	return 0;
}

// 캐릭터 정보 개수 얻기
SI32	CSiegeUnitTableMgr::GetCount(void)
{
	return m_vecSiegeUnitTableMgr.size();
}
