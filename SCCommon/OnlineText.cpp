//담 당 자 : 이 상 원
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
#include "OnlineCommonParser.h"		// actdoll (2004/06/10 15:42) : 공용 파서기 추가

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
			// actdoll (2005/02/25 22:35) : 기존 방식대로 하면 해외 문자가 깨질 수 있다.
			//	2Byte문자일 경우 그대로 넘겨받는다.
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
//	설명	:	생성자.
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
//	설명	:	소멸자.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OnlineText::~OnlineText()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	초기화
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

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 OnlineText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )		// 일단 외부 명칭이 있다면 먼저 그놈을 읽고
	{
		sprintf( pszFullPath, pszFileName_OnlineText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName_OnlineText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_OnlineText.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "OnlineText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// 정보들의 갯수를 받고 점검
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM )
		{
			ocp.PrintError( "OnlineText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// 자료 받기
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 추출한 자료를 순차적으로 집어넣는다.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineOneText	*pOT	= Texts + iCount;

			ocp.GetValue( (SI32&)pOT->uiID,							iLineNo, iColNo );		// 번호
			ocp.GetValue( pOT->szText, ON_MAX_TEXT_STRING_NUM - 1,	iLineNo, iColNo );		// 스트링
			ConvertSpecialChar( uiCodePage, pOT->szText);

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "OnlineText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;

			// 넘어갔으면 카운트 올리고
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

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 QuestText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )		// 일단 외부 명칭이 있다면 먼저 그놈을 읽고
	{
		sprintf( pszFullPath, pszFileName_QuestText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName_QuestText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_QuestText.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "QuestText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// 정보들의 갯수를 받고 점검
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_2048 )
		{
			ocp.PrintError( "QuestText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// 자료 받기
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 추출한 자료를 순차적으로 집어넣는다.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineQuestText	*pQT	= QuestTexts + iCount;

			ocp.GetValue( (SI32&)pQT->uiID,							iLineNo, iColNo );		// 번호
			ocp.GetValue( pQT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// 스트링

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "QuestText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;

			// 넘어갔으면 카운트 올리고
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
	
	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 HelpText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )		// 일단 외부 명칭이 있다면 먼저 그놈을 읽고
	{
		sprintf( pszFullPath, pszFileName_HelpText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName_HelpText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_HelpText.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "HelpText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// 정보들의 갯수를 받고 점검
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_2048 )
		{
			ocp.PrintError( "HelpText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// 자료 받기
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 추출한 자료를 순차적으로 집어넣는다.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineHelpText	*pHT	= HelpTexts + iCount;

			ocp.GetValue( (SI32&)pHT->uiID,							iLineNo, iColNo );		// 번호
			ocp.GetValue( pHT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// 스트링
			ConvertSpecialChar( uiCodePage, pHT->szText);

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "HelpText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;

			// 넘어갔으면 카운트 올리고
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

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 CharIntroductionText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )		// 일단 외부 명칭이 있다면 먼저 그놈을 읽고
	{
		sprintf( pszFullPath, pszFileName_CharIntroText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName_CharIntroText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_CharIntroText.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "CharIntroductionText Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// 정보들의 갯수를 받고 점검
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_STRING_CHAR_INFO )
		{
			ocp.PrintError( "CharIntroductionText Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// 자료 받기
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 추출한 자료를 순차적으로 집어넣는다.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineCharInfoText	*pCIT	= CharInfoTexts + iCount;

			ocp.GetValue( (SI32&)pCIT->uiID,							iLineNo, iColNo );		// 번호
			ocp.GetValue( pCIT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// 스트링
			ConvertSpecialChar( uiCodePage, pCIT->szText);

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "CharIntroductionText Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;

			// 넘어갔으면 카운트 올리고
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

	// actdoll (2004/06/10 15:49) : 공통 파서기 기준의 ObjectMarkText.dat 장착
	//	만약 여기서 pszBaseRootName이 NULL이 아닐 경우 HQ를 이용하지 않고
	//	해당 절대 경로에 들어있는 텍스트를 기반으로 한다.
	if( pszLocalPath )		// 일단 외부 명칭이 있다면 먼저 그놈을 읽고
	{
		sprintf( pszFullPath, pszFileName_ObjectMarkText, pszLocalPath );
		fp	= fopen( pszFullPath, "rb" );
	}
	if( !fp )					// 아직까지 발견을 못했다면 HQ 내부에서 읽는다.
	{
		sprintf( pszFullPath, pszFileName_ObjectMarkText, pszHQNation );
		fp	= BindJXFile::GetInstance()->GetFile( pszFullPath );
	}

	if( fp != NULL )
	{
		// actdoll (2004/08/23 14:10) : 출력될 에러파일명 적재
		ocp.SetErrFileName( "c:\\ParseErr_ObjectMark.txt" );
	
		// 파싱을 시작한다.
		if( ( iRet = ocp.Initialize( fp, iErrLine, bLess ) ) <= 0 )
		{
			ocp.PrintError( "ObjectMark Error : Cannot Init! [ %s | Ret-%d | Line-%d | Less-%d ]", pszFullPath, iRet, iErrLine, bLess );
			fclose( fp );
			return FALSE;
		}

		// 정보들의 갯수를 받고 점검
		if( ocp.GetMaxDataLineCount() >= ON_MAX_TEXT_NUM_256 )
		{
			ocp.PrintError( "ObjectMark Error : Cannot Init! LineCount is Over!" );
			fclose( fp );
			return FALSE;
		}

		// 자료 받기
		iCount	= 0;
		while(1)
		{
			if( ocp.ParseDataLine() == 0 )	break;		// 데이터 라인 추출

			// 컬럼별로 추출한 자료를 순차적으로 집어넣는다.
			int		iLineNo, iColNo;
			BOOL	bRet = false;
			OnlineMarkText	*pOMT	= OnlineMarkTexts + iCount;

			ocp.GetValue( (SI32&)pOMT->uiID,							iLineNo, iColNo );		// 번호
			ocp.GetValue( pOMT->szText, ON_MAX_TEXT_STRING_1024 - 1,	iLineNo, iColNo );		// 스트링
// by kuzimo
			ConvertSpecialChar( uiCodePage, pOMT->szText);

			// 확인사살 - 만약 지금 처리한 것이 마지막 컬럼이었다면 정상이다.
			if( !ocp.IsEndOfColumn() )
			{
				ocp.PrintError( "ObjectMark Error : Column is mismatched! [ %s | Last Line-%d | Last Column-%d ]\n", pszFullPath, ocp.GetCurrentLineNo(), ocp.GetCurrentColumnNo() );
				fclose( fp );
				return FALSE;
			}

			// 확인 사살 - 이것이 지금 마지막 라인째를 처리하고 있다면 브레이크
			if( ocp.IsEndOfLine() )		break;

			// 넘어갔으면 카운트 올리고
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
//	설명	:	아이디를 사용하여 문자열을 얻어온다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::Get(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(EmptyText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM; i++)
	{
		if		( Texts[i].uiID == uiID )			return Texts[i].szText;		// 해당 ID를 찾았으면 그 부분을 리턴
		else if	( Texts[i].uiID == 0 )				break;						// 순차적으로 적재되기 때문에 ID가 없는 자리는 빈자리다.
	}

	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	Texts[i].uiID	= uiID;
	sprintf( Texts[i].szText, "ON%08d", uiID );

	return Texts[i].szText;
}




//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이디를 사용하여 문자열을 얻어온다. Quest
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::GetQuestText(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(QuestEmptyText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM_2048; i++)
	{
		if		( QuestTexts[i].uiID == uiID )		return QuestTexts[i].szText;	// 해당 ID를 찾았으면 그 부분을 리턴
		else if	( QuestTexts[i].uiID == 0 )			break;							// 순차적으로 적재되기 때문에 ID가 없는 자리는 빈자리다.
	}

	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	QuestTexts[i].uiID	= uiID;
	sprintf( QuestTexts[i].szText, "QU%08d", uiID );

	return QuestTexts[i].szText;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	아이디를 사용하여 문자열을 얻어온다. Help
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
CHAR*	OnlineText::GetHelpText(UI32 uiID)
{
	if(uiID == ON_TEXT_UNUSED) return(EmptyHelpText);

	SI32	i;
	for(i = 0; i < ON_MAX_TEXT_NUM_2048; i++)
	{
		if		( HelpTexts[i].uiID == uiID )		return HelpTexts[i].szText;		// 해당 ID를 찾았으면 그 부분을 리턴
		else if	( HelpTexts[i].uiID == 0 )			break;							// 순차적으로 적재되기 때문에 ID가 없는 자리는 빈자리다.
	}

	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	HelpTexts[i].uiID	= uiID;
	sprintf( HelpTexts[i].szText, "HE%08d", uiID );

	return HelpTexts[i].szText;
}

CHAR*	OnlineText::GetCharInfoText(UI32 uiID)
{
	SI32	i;

	for(i = 0; i < ON_MAX_TEXT_STRING_CHAR_INFO; i++)
	{
		if		( CharInfoTexts[i].uiID == uiID )	return CharInfoTexts[i].szText;		// 해당 ID를 찾았으면 그 부분을 리턴
		else if	( CharInfoTexts[i].uiID == 0 )		break;								// 순차적으로 적재되기 때문에 ID가 없는 자리는 빈자리다.
	}

	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	CharInfoTexts[i].uiID	= uiID;
	sprintf( CharInfoTexts[i].szText, "CH%08d", uiID );

	return CharInfoTexts[i].szText;
}

CHAR* OnlineText::GetObjectMarkText(UI32 uiID)
{
	SI32	i;

	for(i = 0; i < ON_MAX_TEXT_NUM_256; i++)
	{
		if		( OnlineMarkTexts[i].uiID == uiID )	return OnlineMarkTexts[i].szText;		// 해당 ID를 찾았으면 그 부분을 리턴
		else if	( OnlineMarkTexts[i].uiID == 0 )	break;									// 순차적으로 적재되기 때문에 ID가 없는 자리는 빈자리다.
	}

	// actdoll (2004/11/23 13:32) : 
	//	이 부분을 단순히 Empty로 띄우지 않고 해당 인덱스의 수치를 약간 변형하여 사용한다.
	OnlineMarkTexts[i].uiID	= uiID;
	sprintf( OnlineMarkTexts[i].szText, "OB%08d", uiID );

	return OnlineMarkTexts[i].szText;
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//	설명	:	조사 처리
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
