//�� �� �� : �� �� ��
#include <GSL.h>
#ifndef _IMJINONLINESERVER
#include	<clGame.h>

extern _clGame* pGame;
#endif


#include	<DebugTool.h>
#include <OnlineText.h>
#include <stdio.h>
#include <stdlib.h>
#include "BindJxFile.h"
#include "OnlineCommonParser.h"		// actdoll (2004/06/10 15:42) : ���� �ļ��� �߰�

// by kuzimo
void OnlineText::ConvertSpecialChar( UINT uiCodePage, CHAR *pSpecialString )
{
	CHAR *oriString = pSpecialString;
	bool bSpecial;
	CHAR *pWriteString;

	pWriteString = pSpecialString;
	bSpecial = false;

	while( *pSpecialString != '\0' )
	{
		if( bSpecial == true )
		{
			switch(*pSpecialString)
			{
				case '0' :
					*pWriteString++ = '\0';
					return;
				case 'n' :
					*pWriteString++ = '\n';
					*pWriteString++ = '\r';
					break;
				case 't' :
					*pWriteString++ = '\t';
					break;
				default:
					break;
			}
			bSpecial = false;
		}
		else
		{
			// actdoll (2005/02/25 22:35) : ���� ��Ĵ�� �ϸ� �ؿ� ���ڰ� ���� �� �ִ�.
			//	2Byte������ ��� �״�� �Ѱܹ޴´�.
			if( IsDBCSLeadByteEx( uiCodePage, *pSpecialString ) )
			{
				*pWriteString++	=	*pSpecialString++;
				*pWriteString++	=	*pSpecialString;
			}
			else if( *pSpecialString == '\\' )
			{
				bSpecial = true;
			}
			else
			{
				*pWriteString++ = *pSpecialString;
			}
		}
		pSpecialString++;
	}
	*pWriteString = '\0';
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	������.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

OnlineText::OnlineText()
{
/*	for(SI32 i = 0; i < ON_MAX_TEXT_NUM; i++)
	{
		ZeroMemory(Texts[i].szText, sizeof(Texts[i].szText));
		Texts[i].uiID	=	0;
	}*/
	ZeroMemory(Texts, sizeof(OnlineOneText)*ON_MAX_TEXT_NUM);
	ZeroMemory(QuestTexts, sizeof(OnlineQuestText)*ON_MAX_TEXT_NUM_2048);
	ZeroMemory(HelpTexts, sizeof(OnlineHelpText)*ON_MAX_TEXT_NUM_2048);
	ZeroMemory(CharInfoTexts, sizeof(OnlineCharInfoText)*ON_MAX_TEXT_STRING_CHAR_INFO);
	ZeroMemory(OnlineMarkTexts, sizeof(OnlineMarkText)*ON_MAX_TEXT_NUM_256);

	sprintf((char*)EmptyText,      "Empty");
	sprintf((char*)QuestEmptyText, "Empty");
	sprintf((char*)EmptyHelpText, "Empty");
	sprintf((char*)EmptyCharInfoText,"Empty");
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�Ҹ���.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineText::~OnlineText()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	�ʱ�ȭ
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL	OnlineText::Init( UINT uiCodePage, CHAR* pszHQNation, CHAR* pszLocalPath )
{
	OnlineCommonParser		ocp;
	char	pszFullPath[MAX_PATH];
	char	*pszFileName_OnlineText			= "%s\\OnlineText.dat";
	char	*pszFileName_QuestText			= "%s\\QuestText.dat";
	char	*pszFileName_HelpText			= "%s\\HelpText.dat";
	char	*pszFileName_CharIntroText		= "%s\\CharIntroductionText.dat";
	char	*pszFileName_ObjectMarkText		= "%s\\ObjectMarkText.dat";
	FILE	*fp								= NULL;

	int		iRet, iErrLine, iCount;
	BOOL	bLess;

	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ OnlineText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )		// �ϴ� �ܺ� ��Ī�� �ִٸ� ���� �׳��� �а�
	{
		sprintf( pszFullPath, pszFileName_OnlineText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName_OnlineText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_OnlineText.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "OnlineText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// �������� ������ �ް� ����
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM )
		{
			ocp.PrintError( "OnlineText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// �ڷ� �ޱ�
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� ������ �ڷḦ ���������� ����ִ´�.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineOneText	*pOT	= Texts + iCount;

			ocp.GetValue( (SI32&)pOT->uiID,							iLineNo, iColNo );		// ��ȣ
			ocp.GetValue( pOT->szText, ON_MAX_TEXT_STRING_NUM - 1,	iLineNo, iColNo );		// ��Ʈ��
			ConvertSpecialChar( uiCodePage, pOT->szText);

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;

			// �Ѿ���� ī��Ʈ �ø���
			iCount++;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "OnlineText Error : File Doesn't Exist!" );
		return FALSE;
	}

	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ QuestText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )		// �ϴ� �ܺ� ��Ī�� �ִٸ� ���� �׳��� �а�
	{
		sprintf( pszFullPath, pszFileName_QuestText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName_QuestText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_QuestText.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "QuestText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// �������� ������ �ް� ����
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_2048 )
		{
			ocp.PrintError( "QuestText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// �ڷ� �ޱ�
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� ������ �ڷḦ ���������� ����ִ´�.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineQuestText	*pQT	= QuestTexts + iCount;

			ocp.GetValue( (SI32&)pQT->uiID,							iLineNo, iColNo );		// ��ȣ
			ocp.GetValue( pQT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// ��Ʈ��

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "QuestText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;

			// �Ѿ���� ī��Ʈ �ø���
			iCount++;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "QuestText Error : File Doesn't Exist!" );
		return FALSE;
	}
	
	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ HelpText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )		// �ϴ� �ܺ� ��Ī�� �ִٸ� ���� �׳��� �а�
	{
		sprintf( pszFullPath, pszFileName_HelpText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName_HelpText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_HelpText.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "HelpText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// �������� ������ �ް� ����
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_2048 )
		{
			ocp.PrintError( "HelpText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// �ڷ� �ޱ�
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� ������ �ڷḦ ���������� ����ִ´�.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineHelpText	*pHT	= HelpTexts + iCount;

			ocp.GetValue( (SI32&)pHT->uiID,							iLineNo, iColNo );		// ��ȣ
			ocp.GetValue( pHT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// ��Ʈ��
			ConvertSpecialChar( uiCodePage, pHT->szText);

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "HelpText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;

			// �Ѿ���� ī��Ʈ �ø���
			iCount++;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "HelpText Error : File Doesn't Exist!" );
		return FALSE;
	}

	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ CharIntroductionText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )		// �ϴ� �ܺ� ��Ī�� �ִٸ� ���� �׳��� �а�
	{
		sprintf( pszFullPath, pszFileName_CharIntroText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName_CharIntroText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_CharIntroText.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "CharIntroductionText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// �������� ������ �ް� ����
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_STRING_CHAR_INFO )
		{
			ocp.PrintError( "CharIntroductionText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// �ڷ� �ޱ�
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� ������ �ڷḦ ���������� ����ִ´�.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineCharInfoText	*pCIT	= CharInfoTexts + iCount;

			ocp.GetValue( (SI32&)pCIT->uiID,							iLineNo, iColNo );		// ��ȣ
			ocp.GetValue( pCIT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// ��Ʈ��
			ConvertSpecialChar( uiCodePage, pCIT->szText);

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "CharIntroductionText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;

			// �Ѿ���� ī��Ʈ �ø���
			iCount++;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "CharIntroductionText Error : File Doesn't Exist!" );
		return FALSE;
	}

	// actdoll (2004/06/10 15:49) : ���� �ļ��� ������ ObjectMarkText.dat ����
	//	���� ���⼭ pszBaseRootName�� NULL�� �ƴ� ��� HQ�� �̿����� �ʰ�
	//	�ش� ���� ��ο� ����ִ� �ؽ�Ʈ�� ������� �Ѵ�.
	if( pszLocalPath )		// �ϴ� �ܺ� ��Ī�� �ִٸ� ���� �׳��� �а�
	{
		sprintf( pszFullPath, pszFileName_ObjectMarkText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// �������� �߰��� ���ߴٸ� HQ ���ο��� �д´�.
	{
		sprintf( pszFullPath, pszFileName_ObjectMarkText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : ��µ� �������ϸ� ����
		ocp.SetErrFileName( "c:\\ParseErr_ObjectMark.txt" );
	
		// �Ľ��� �����Ѵ�.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "ObjectMark Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// �������� ������ �ް� ����
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_256 )
		{
			ocp.PrintError( "ObjectMark Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// �ڷ� �ޱ�
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// ������ ���� ����

			// �÷����� ������ �ڷḦ ���������� ����ִ´�.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineMarkText	*pOMT	= OnlineMarkTexts + iCount;

			ocp.GetValue( (SI32&)pOMT->uiID,							iLineNo, iColNo );		// ��ȣ
			ocp.GetValue( pOMT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// ��Ʈ��
// by kuzimo
			ConvertSpecialChar( uiCodePage, pOMT->szText);

			// Ȯ�λ�� - ���� ���� ó���� ���� ������ �÷��̾��ٸ� �����̴�.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "ObjectMark Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// Ȯ�� ��� - �̰��� ���� ������ ����°�� ó���ϰ� �ִٸ� �극��ũ
			if( ocp.IsEndOfLine() )		break;

			// �Ѿ���� ī��Ʈ �ø���
			iCount++;
		}

		ocp.Release();
		fclose( fp );
		fp = NULL;
	}
	else
	{
		ocp.PrintError( "ObjectMark Error : File Doesn't Exist!" );
		return FALSE;
	}

	sprintf((char*)EmptyText,			"Empty");
	sprintf((char*)QuestEmptyText,		"Empty");
	sprintf((char*)EmptyHelpText,		"Empty");
	sprintf((char*)EmptyCharInfoText,	"Empty");
	sprintf((char*)EmptyOnlineMarkText,	"Empty");	

	return TRUE;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���̵� ����Ͽ� ���ڿ��� ���´�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::Get(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(EmptyText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM; i++)
	{
		if		( Texts[i].uiID == uiID )			return Texts[i].szText;		// �ش� ID�� ã������ �� �κ��� ����
		else if	( Texts[i].uiID == 0 )				break;						// ���������� ����Ǳ� ������ ID�� ���� �ڸ��� ���ڸ���.
	}

	// actdoll (2004/11/23 13:32) : 
	//	�� �κ��� �ܼ��� Empty�� ����� �ʰ� �ش� �ε����� ��ġ�� �ణ �����Ͽ� ����Ѵ�.
	Texts[i].uiID	= uiID;
	sprintf( Texts[i].szText, "ON%08d", uiID );

	return Texts[i].szText;
}




//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���̵� ����Ͽ� ���ڿ��� ���´�. Quest
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::GetQuestText(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(QuestEmptyText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM_2048; i++)
	{
		if		( QuestTexts[i].uiID == uiID )		return QuestTexts[i].szText;	// �ش� ID�� ã������ �� �κ��� ����
		else if	( QuestTexts[i].uiID == 0 )			break;							// ���������� ����Ǳ� ������ ID�� ���� �ڸ��� ���ڸ���.
	}

	// actdoll (2004/11/23 13:32) : 
	//	�� �κ��� �ܼ��� Empty�� ����� �ʰ� �ش� �ε����� ��ġ�� �ణ �����Ͽ� ����Ѵ�.
	QuestTexts[i].uiID	= uiID;
	sprintf( QuestTexts[i].szText, "QU%08d", uiID );

	return QuestTexts[i].szText;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���̵� ����Ͽ� ���ڿ��� ���´�. Help
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::GetHelpText(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(EmptyHelpText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM_2048; i++)
	{
		if		( HelpTexts[i].uiID == uiID )		return HelpTexts[i].szText;		// �ش� ID�� ã������ �� �κ��� ����
		else if	( HelpTexts[i].uiID == 0 )			break;							// ���������� ����Ǳ� ������ ID�� ���� �ڸ��� ���ڸ���.
	}

	// actdoll (2004/11/23 13:32) : 
	//	�� �κ��� �ܼ��� Empty�� ����� �ʰ� �ش� �ε����� ��ġ�� �ణ �����Ͽ� ����Ѵ�.
	HelpTexts[i].uiID	= uiID;
	sprintf( HelpTexts[i].szText, "HE%08d", uiID );

	return HelpTexts[i].szText;
}

CHAR*	OnlineText::GetCharInfoText(UI32 uiID)
{
	SI32	i;

	for(i = 0; i < ON_MAX_TEXT_STRING_CHAR_INFO; i++)
	{
		if		( CharInfoTexts[i].uiID == uiID )	return CharInfoTexts[i].szText;		// �ش� ID�� ã������ �� �κ��� ����
		else if	( CharInfoTexts[i].uiID == 0 )		break;								// ���������� ����Ǳ� ������ ID�� ���� �ڸ��� ���ڸ���.
	}

	// actdoll (2004/11/23 13:32) : 
	//	�� �κ��� �ܼ��� Empty�� ����� �ʰ� �ش� �ε����� ��ġ�� �ణ �����Ͽ� ����Ѵ�.
	CharInfoTexts[i].uiID	= uiID;
	sprintf( CharInfoTexts[i].szText, "CH%08d", uiID );

	return CharInfoTexts[i].szText;
}

CHAR* OnlineText::GetObjectMarkText(UI32 uiID)
{
	SI32	i;

	for(i = 0; i < ON_MAX_TEXT_NUM_256; i++)
	{
		if		( OnlineMarkTexts[i].uiID == uiID )	return OnlineMarkTexts[i].szText;		// �ش� ID�� ã������ �� �κ��� ����
		else if	( OnlineMarkTexts[i].uiID == 0 )	break;									// ���������� ����Ǳ� ������ ID�� ���� �ڸ��� ���ڸ���.
	}

	// actdoll (2004/11/23 13:32) : 
	//	�� �κ��� �ܼ��� Empty�� ����� �ʰ� �ش� �ε����� ��ġ�� �ణ �����Ͽ� ����Ѵ�.
	OnlineMarkTexts[i].uiID	= uiID;
	sprintf( OnlineMarkTexts[i].szText, "OB%08d", uiID );

	return OnlineMarkTexts[i].szText;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	����	:	���� ó��
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void	OnlineText::Josa(char* lpStr)
{
#ifndef _IMJINONLINESERVER
	switch(pGame->LanguageCode)
	{
		case ELANGAGECODE_KOREAN:
			JosaProc.JosaProc(lpStr);
			break;
	}
#endif
}
