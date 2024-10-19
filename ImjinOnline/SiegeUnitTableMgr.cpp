// SiegeUnitTableMgr.cpp: implementation of the CSiegeUnitTableMgr class.
//
//////////////////////////////////////////////////////////////////////

#include	<GSL.h>
#include	"GSCDefine.h"

#include	"..\EnumData\Enum_KI_Character.h"		// KindInfo - Character
#include	"bindJXFile.h"
#include	"OnlineCommonParser.h"					// ���� �ļ� �ý���
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

// �޸� ����
void	CSiegeUnitTableMgr::Free(void)
{
	m_vecSiegeUnitTableMgr.clear();
	m_vecSiegeUnitCalcHPTableMgr.clear();
	m_vecSiegeUnitCalcMPTableMgr.clear();
}

// HQ�� SiegeUnitTable.txt ���� �б�
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTable(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTable.txt";

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
		char szCharname[128];
		char szAnimationResourceID_Blue[64], szAnimationResourceID_Red[64];

		SSiegeUnitTable	sSiegeUnitTable = {0, };
		
		// �ڷ� �ޱ� ����
		ocp.GetValue( (char &)sSiegeUnitTable.m_siID,					iLineNo, iColNo );	// ���� ���� �迭 �ε���
		ocp.GetValue( szCharname,							127,		iLineNo, iColNo );	// �̸�(������)
		ocp.GetValue( (short &)sSiegeUnitTable.m_uiKind,				iLineNo, iColNo );	// ���� ���� �¶��� Kind
		ocp.GetValue( sSiegeUnitTable.m_siUnitNameTextIdx,				iLineNo, iColNo );	// ������ ���� �̸� �ؽ�Ʈ �ε���
		ocp.GetValue( sSiegeUnitTable.m_siArmyKindDescTextID,			iLineNo, iColNo );	// ������ ���� ���� �ؽ�Ʈ �ε���
		ocp.GetValue( (short &)sSiegeUnitTable.m_siPopulation,			iLineNo, iColNo );	// ���� ������ �� �ʿ��� �α���
		ocp.GetValue( (short &)sSiegeUnitTable.m_siPrice,				iLineNo, iColNo );	// ���� ������ ���
		ocp.GetValue( (char &)sSiegeUnitTable.m_scTransformOption,		iLineNo, iColNo );	// ���� ���� ĳ���� �Ӽ�
		ocp.GetValue( (char &)sSiegeUnitTable.m_scTransformLevelLimit,	iLineNo, iColNo );	// ���� ���� ���� ���� ����
		ocp.GetValue( sSiegeUnitTable.m_siTransformCost,				iLineNo, iColNo );	// ���� ���� ���� ���
		ocp.GetValue( sSiegeUnitTable.m_siBasicHP,						iLineNo, iColNo );	// ���� ���� �⺻ ü��
		ocp.GetValue( sSiegeUnitTable.m_siBasicMP,						iLineNo, iColNo );	// ���� ���� �⺻ ����
		ocp.GetValue( szAnimationResourceID_Blue,			63,			iLineNo, iColNo );	// ���� ���� �Ʊ��� �� �̹���
		ocp.GetValue( szAnimationResourceID_Red,			63,			iLineNo, iColNo );	// ���� ���� ������ �� �̹���

		if (strcmp("NULL", szAnimationResourceID_Blue) != 0)
			sSiegeUnitTable.m_siAnimationResourceIdx_Blue	=	GetValueOfGameSprNumID(szAnimationResourceID_Blue);
		else
			sSiegeUnitTable.m_siAnimationResourceIdx_Blue	= -1;

		if (strcmp("NULL", szAnimationResourceID_Red) != 0)
			sSiegeUnitTable.m_siAnimationResourceIdx_Red	=	GetValueOfGameSprNumID(szAnimationResourceID_Red);
		else
			sSiegeUnitTable.m_siAnimationResourceIdx_Red	= -1;

		// ���� ���� �������̽� �ִϸ��̼� ��ư ����. ���. Normal, Hover
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_szAnimationFilename,	127,	iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionStartFont,			iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAniamtionFontNumber,		iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_WAIT].m_siAnimationDelay,				iLineNo, iColNo );

		// ���� ���� �������̽� �ִϸ��̼� ��ư ����. ����. Pressed
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_szAnimationFilename,	127,	iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionStartFont,			iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAniamtionFontNumber,		iLineNo, iColNo );
		ocp.GetValue( sSiegeUnitTable.m_sAniamtionInfo[ANIBTN_MOVE].m_siAnimationDelay,				iLineNo, iColNo );

		// robypark 2005/2/15 16:32
		ocp.GetValue( (char &)sSiegeUnitTable.m_scMaxMoveActionSkip,		iLineNo, iColNo );	// �̵� ó�� �Լ� Move() ������ �ִ�. ������ skip ȿ��

		// �ʻ�ȭ �̹��� ���ϸ�
		ocp.GetValue( sSiegeUnitTable.m_szPortraitFileName,		255,	iLineNo, iColNo );

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// ����Ʈ�� �߰�
		m_vecSiegeUnitTableMgr.push_back( sSiegeUnitTable );	
		
		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// HQ�� SiegeUnitTableCalcHP.txt ���� �б�
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTableCalcHP(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTableCalcHP.txt";

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
		SSiegeUnitCalcTable	sSiegeUnitCalcTable = {0, };
		
		// �ڷ� �ޱ� ����
		ocp.GetValue( sSiegeUnitCalcTable.m_siMinBattlePower,		iLineNo, iColNo );	// �ּ� ������
		ocp.GetValue( sSiegeUnitCalcTable.m_siMaxBattlePower,		iLineNo, iColNo );	// �ִ� ������
		ocp.GetValue( sSiegeUnitCalcTable.m_siIncreaseValue,		iLineNo, iColNo );	// ���ġ

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// ����Ʈ�� �߰�
		m_vecSiegeUnitCalcHPTableMgr.push_back( sSiegeUnitCalcTable );	
		
		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// HQ�� SiegeUnitTableCalcMP.txt ���� �б�
BOOL	CSiegeUnitTableMgr::LoadSiegeUnitTableCalcMP(void)
{
	FILE				*fp;
	OnlineCommonParser	ocp;
	char				szDataFileName[]	=	"Online\\CharInfo\\SiegeUnitTableCalcMP.txt";

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
		SSiegeUnitCalcTable	sSiegeUnitCalcTable = {0, };
		
		// �ڷ� �ޱ� ����
		ocp.GetValue( sSiegeUnitCalcTable.m_siMinBattlePower,		iLineNo, iColNo );	// �ּ� ������
		ocp.GetValue( sSiegeUnitCalcTable.m_siMaxBattlePower,		iLineNo, iColNo );	// �ִ� ������
		ocp.GetValue( sSiegeUnitCalcTable.m_siIncreaseValue,		iLineNo, iColNo );	// ���ġ

		// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
		if( !ocp.IsEndOfColumn() )
		{
			ocp.PrintError( "Specific Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", szDataFileName, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
		}
		
		// ����Ʈ�� �߰�
		m_vecSiegeUnitCalcMPTableMgr.push_back( sSiegeUnitCalcTable );	
		
		// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
		if( ocp.IsEndOfLine() )	
			break;
	}

	fclose( fp );
	ocp.Release();

	return TRUE;
}

// ĳ���� Online Kind�� �´� LPSSiegeUnitTable���
LPSSiegeUnitTable	CSiegeUnitTableMgr::GetSiegeUnitTable(SI32 siOnlineCharKind)
{
	for( vector<SSiegeUnitTable>::iterator i = m_vecSiegeUnitTableMgr.begin(); i != m_vecSiegeUnitTableMgr.end(); ++i)
	{
		if (i->m_uiKind == siOnlineCharKind)
			return i;
	}

	return NULL;
}

// �迭 �ε����� �´� LPSSiegeUnitTable���
LPSSiegeUnitTable	CSiegeUnitTableMgr::GetSiegeUnitTableByArrayIdx(SI32 siArrayIdx)
{
	for( vector<SSiegeUnitTable>::iterator i = m_vecSiegeUnitTableMgr.begin(); i != m_vecSiegeUnitTableMgr.end(); ++i)
	{
		if (i->m_siID == siArrayIdx)
			return i;
	}

	return NULL;
}

// �����¿� ���� ü�� ���ġ ���
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

	// �ִ� �� �����º��� ū ��� �˻�
	SSiegeUnitCalcTable &CalcTable = m_vecSiegeUnitCalcHPTableMgr.back();

	if (CalcTable.m_siMaxBattlePower < siBattlePower)
	{
		return CalcTable.m_siIncreaseValue;
	}

	return 0;
}

// �����¿� ���� ���� ���ġ ���
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

	// �ִ� �� �����º��� ū ��� �˻�
	SSiegeUnitCalcTable &CalcTable = m_vecSiegeUnitCalcMPTableMgr.back();

	if (CalcTable.m_siMaxBattlePower < siBattlePower)
	{
		return CalcTable.m_siIncreaseValue;
	}

	return 0;
}

// ĳ���� ���� ���� ���
SI32	CSiegeUnitTableMgr::GetCount(void)
{
	return m_vecSiegeUnitTableMgr.size();
}
