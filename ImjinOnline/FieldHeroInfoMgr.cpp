/***************************************************************
*
* ���ϼ���: �¶��� �ʵ忡�� ���ΰ� ĳ���͵鿡 ���� ���� ������
*
***************************************************************/

#include	<GSL.h>
#include	"GSCDefine.h"

#include	"..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include	"FieldHeroInfoMgr.h"
#include	"bindJXFile.h"
#include	"OnlineCommonParser.h"					// ���� �ļ� �ý���
#include	"music.h"

CFieldHeroInfoMgr::CFieldHeroInfoMgr()
{
}

CFieldHeroInfoMgr::~CFieldHeroInfoMgr()
{
	Free();
}

// �޸� ����
void	CFieldHeroInfoMgr::Free(void)
{
	m_vecFieldHeroInfo.clear();
}

// HQ�� FieldHeroInfo.txt ���� �б�
BOOL	CFieldHeroInfoMgr::LoadFieldHeroInfo(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\FieldHeroInfo.txt";

	// ������ ����
	fp = BindJXFile::GetInstance()->GetFile(szDataFileName);

	if(fp == NULL)
		return FALSE;	

	// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
	char	pszErrName[MAX_PATH];
	sprintf( pszErrName, "c:\\ParseErr_%s.txt", szDataFileName );
	ocp.SetErrFileName( pszErrName );
	
	// �Ľ��� �����Ѵ�.
	int		iRet, iErrLine;
	BOOL	bLess;
	if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
	{
		ocp.PrintError( "Specific Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", szDataFileName, iRet, iErrLine, bLess );
		fclose( fp );
		return FALSE;
	}

	// �ڷ� �˻�
	while (1)
	{
		if( ocp.ParseDataLine() == 0 )
			break;		// ������ ���� ����

		SI32	iLineNo, iColNo;

		// �ڷḦ ���������� ���� �غ� �Ѵ�.
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
		
		// �ڷ� �ޱ� ����
		ocp.GetValue( siOnlineHeroKind,				iLineNo, iColNo );	// ���ΰ� online kind
		ocp.GetValue( szCharname,			127,	iLineNo, iColNo );	// �̸�(������)
		ocp.GetValue( szSex,				31,		iLineNo, iColNo );	// ���� ����
		ocp.GetValue( szNation,				31,		iLineNo, iColNo );	// ���� ����
		ocp.GetValue( szSndHungriest,		127,	iLineNo, iColNo );	// ���� ����� ����
		ocp.GetValue( szSndHungrier,		127,	iLineNo, iColNo );	// ���� ����� ����
		ocp.GetValue( szSndHungry,			127,	iLineNo, iColNo );	// ���� ����� ����
		ocp.GetValue( szSndBoring,			127,	iLineNo, iColNo );	// ������ ����
		ocp.GetValue( szSndTryBattle,		127,	iLineNo, iColNo );	// ���� �õ� ����
		ocp.GetValue( szSndMove,			127,	iLineNo, iColNo );	// �̵� ����
		ocp.GetValue( siMinmapIconFontNo,			iLineNo, iColNo );	// �̴ϸ� ĳ���� ������ �̹��� ��Ʈ ��ȣ
		ocp.GetValue( szRefugeeKind,		127,	iLineNo, iColNo );	// �������� �ǳ��� �̹��� ������� ĳ���� Kind

		// ���� ����
		bool bFemale = false;
		if (stricmp(szSex, "FEMALE") == 0)
			bFemale = true;

		// ���� ����
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

		SI32 siSndHungriest = 0;	// ���� �谡 ���� �� ����
		SI32 siSndHungrier = 0;		// �谡 ���� ������ �߰��� �� ����
		SI32 siSndHungry = 0;		// �谡 ���� ���� �� ����
		SI32 siSndBoring = 0;		// ĳ���Ͱ� �� ���� ���� �ӹ� �� ����
		SI32 siSndTryBattle = 0;	// ĳ���Ͱ� ������ ���� ���� ���� Ŭ���� �� ����
		SI32 siSndMove = 0;			// ĳ���Ͱ� �̵��� �� ����

		// ���� ó��
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

		// �ǳ��� Kind�� �´� Kind Index�� ��ȯ
		SI32 siRefugeeKind	= CLASS_KIND_ON_CHARACTER::GetData( szRefugeeKind );

		if ( siRefugeeKind == KIND_ON_INVALID )
		{
			TRACE("Refugee Kind is NOT existed.\n");
		}

		// ������ ����
		FieldHeroInfo.SetFieldHeroData(siOnlineHeroKind, bFemale, siNation, 
										siSndHungriest, siSndHungrier, siSndHungry,
										siSndBoring, siSndTryBattle, siSndMove, 
										siMinmapIconFontNo, siRefugeeKind);

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// ����Ʈ�� �߰�
		m_vecFieldHeroInfo.push_back( FieldHeroInfo );	
		
		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// ĳ���� Online Kind�� �´� CFieldHeroInfo���
CFieldHeroInfo	*CFieldHeroInfoMgr::GetFieldHeroInfo(SI32 siOnlineCharKind)
{
	for( vector<CFieldHeroInfo>::iterator i = m_vecFieldHeroInfo.begin(); i != m_vecFieldHeroInfo.end(); ++i)
	{
		if (i->GetOnlineHeroKind() == siOnlineCharKind)
			return i;
	}

	return NULL;
}

// ���� ĳ���� ���� ���� ���
SI32	CFieldHeroInfoMgr::GetHeroCount(void)
{
	return m_vecFieldHeroInfo.size();
}
