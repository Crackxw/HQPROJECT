/***************************************************************
*
* 파일설명: 온라인 필드에서 주인공 캐릭터들에 관한 정보 관리자
*
***************************************************************/

#include	<GSL.h>
#include	"GSCDefine.h"

#include	"..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include	"FieldHeroInfoMgr.h"
#include	"bindJXFile.h"
#include	"OnlineCommonParser.h"					// 공용 파서 시스템
#include	"music.h"

CFieldHeroInfoMgr::CFieldHeroInfoMgr()
{
}

CFieldHeroInfoMgr::~CFieldHeroInfoMgr()
{
	Free();
}

// 메모리 해제
void	CFieldHeroInfoMgr::Free(void)
{
	m_vecFieldHeroInfo.clear();
}

// HQ의 FieldHeroInfo.txt 파일 읽기
BOOL	CFieldHeroInfoMgr::LoadFieldHeroInfo(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\FieldHeroInfo.txt";

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
		SI32	siOnlineHeroKind;
		char	szCharname[128];
		char	szSex[32];
		char	szNation[32];
		char	szSndHungriest[128];
		char	szSndHungrier[128];
		char	szSndHungry[128];
		char	szSndBoring[128];
		char	szSndTryBattle[128];
		char	szSndMove[128];
		SI16	siMinmapIconFontNo;
		char	szRefugeeKind[128];

		CFieldHeroInfo	FieldHeroInfo;
		
		// 자료 받기 개시
		ocp.GetValue( siOnlineHeroKind,				iLineNo, iColNo );	// 주인공 online kind
		ocp.GetValue( szCharname,			127,	iLineNo, iColNo );	// 이름(사용안함)
		ocp.GetValue( szSex,				31,		iLineNo, iColNo );	// 남녀 구분
		ocp.GetValue( szNation,				31,		iLineNo, iColNo );	// 국가 구분
		ocp.GetValue( szSndHungriest,		127,	iLineNo, iColNo );	// 아주 배고픔 사운드
		ocp.GetValue( szSndHungrier,		127,	iLineNo, iColNo );	// 보통 배고픔 사운드
		ocp.GetValue( szSndHungry,			127,	iLineNo, iColNo );	// 조금 배고픔 사운드
		ocp.GetValue( szSndBoring,			127,	iLineNo, iColNo );	// 지루함 사운드
		ocp.GetValue( szSndTryBattle,		127,	iLineNo, iColNo );	// 공격 시도 사운드
		ocp.GetValue( szSndMove,			127,	iLineNo, iColNo );	// 이동 사운드
		ocp.GetValue( siMinmapIconFontNo,			iLineNo, iColNo );	// 미니맵 캐릭터 아이콘 이미지 폰트 번호
		ocp.GetValue( szRefugeeKind,		127,	iLineNo, iColNo );	// 공성전중 피난민 이미지 사용위한 캐릭터 Kind

		// 남녀 구분
		bool bFemale = false;
		if (stricmp(szSex, "FEMALE") == 0)
			bFemale = true;

		// 국가 구분
		SI32 siNation = ON_COUNTRY_KOREA;
		if(strcmp(szNation, "KOREA") == 0)
			siNation = ON_COUNTRY_KOREA;
		else if(strcmp(szNation, "JAPAN") == 0)
			siNation = ON_COUNTRY_JAPAN;
		else if(strcmp(szNation, "TAIWAN") == 0)
			siNation = ON_COUNTRY_TAIWAN;
		else if(strcmp(szNation, "CHINA") == 0)
			siNation = ON_COUNTRY_CHINA;
		else
			TRACE("Invalid Country:FieldHeroInfo", "%s", szNation);

		SI32 siSndHungriest = 0;	// 아주 배가 고플 때 사운드
		SI32 siSndHungrier = 0;		// 배가 고픈 정도가 중간일 때 사운드
		SI32 siSndHungry = 0;		// 배가 조금 고플 때 사운드
		SI32 siSndBoring = 0;		// 캐릭터가 한 곳에 오래 머물 때 사운드
		SI32 siSndTryBattle = 0;	// 캐릭터가 전투를 위해 몬스터 등을 클릭할 때 사운드
		SI32 siSndMove = 0;			// 캐릭터가 이동할 때 사운드

		// 사운드 처리
		if (strcmp(szSndHungriest, "0") != 0)
		{
			siSndHungriest	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndHungriest);
		}

		if (strcmp(szSndHungrier, "0") != 0)
		{
			siSndHungrier	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndHungrier);
		}

		if (strcmp(szSndHungry, "0") != 0)
		{
			siSndHungry	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndHungry);
		}

		if (strcmp(szSndBoring, "0") != 0)
		{
			siSndBoring	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndBoring);
		}

		if (strcmp(szSndTryBattle, "0") != 0)
		{
			siSndTryBattle	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndTryBattle);
		}

		if (strcmp(szSndMove, "0") != 0)
		{
			siSndMove	= GetEffectDataMgr()->GetMyHashTable()->Find(szSndMove);
		}

		// 피난민 Kind에 맞는 Kind Index로 변환
		SI32 siRefugeeKind	= CLASS_KIND_ON_CHARACTER::GetData( szRefugeeKind );

		if ( siRefugeeKind == KIND_ON_INVALID )
		{
			TRACE("Refugee Kind is NOT existed.\n");
		}

		// 데이터 설정
		FieldHeroInfo.SetFieldHeroData(siOnlineHeroKind, bFemale, siNation, 
										siSndHungriest, siSndHungrier, siSndHungry,
										siSndBoring, siSndTryBattle, siSndMove, 
										siMinmapIconFontNo, siRefugeeKind);

		// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// 리스트에 추가
		m_vecFieldHeroInfo.push_back( FieldHeroInfo );	
		
		// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// 캐릭터 Online Kind에 맞는 CFieldHeroInfo얻기
CFieldHeroInfo	*CFieldHeroInfoMgr::GetFieldHeroInfo(SI32 siOnlineCharKind)
{
	for( vector<CFieldHeroInfo>::iterator i = m_vecFieldHeroInfo.begin(); i != m_vecFieldHeroInfo.end(); ++i)
	{
		if (i->GetOnlineHeroKind() == siOnlineCharKind)
			return i;
	}

	return NULL;
}

// 주인 캐릭터 정보 개수 얻기
SI32	CFieldHeroInfoMgr::GetHeroCount(void)
{
	return m_vecFieldHeroInfo.size();
}
