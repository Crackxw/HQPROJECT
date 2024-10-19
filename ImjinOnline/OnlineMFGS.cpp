/* 
	2003.07.14
	MFGS 관련 수정 정진욱	
*/

#include <GSL.h>
/*
#include <windows.h>
#include <Online.h>
#include <clGame.h>
#include "OnlineMFGS.h"
#include "MFSvc2.h"
#include "OnlineCharUI.h"
#include "OnlineText.h"

extern _clGame* pGame;
BOOL	g_bNation;
BOOL	g_bMFGS_Start = FALSE;
BOOL	g_bInit = FALSE;


int	__stdcall MFGS2_Callback(long lEventType, long lCode, long lParam, long lExParamSize, void* pExParam )
{
	switch(lEventType)
	{
	case MFGS2_EVENT_UPDATE:					
		{
			CHAR TempBuffer[1024];
			FILE *fp;
			fp = fopen("MFGS.txt", "at");
			sprintf(TempBuffer, "MFGS2_EVENT_UPDATE! lEventType : %d\r\n", lEventType);
			fprintf(fp, TempBuffer);
			fclose(fp);

			switch(lCode)
			{
			case MFGS2_EVENT_UPDATE_SUCCEED:	// 업데이트 성공
				if( MFGS2_Start() != MFGS2_ERR_OK )
				{
					pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);					
					pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "MFGS2_EVENT_UPDATE_SUCCEED", pGame->pOnlineWorld->pOnlineText->Get( 11107 ) );
				}
				else
					g_bMFGS_Start = TRUE;
				break;
					
			case MFGS2_EVENT_UPDATE_FAIL:
				{
					MFGS2_Update();
					CHAR szTempBuffer[512];
					pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);
					sprintf( szTempBuffer, "%s [%d] 에러 번호를 자유 게시판에 올려 주세요.", pGame->pOnlineWorld->pOnlineText->Get( 11104 ), GetLastError() );		
					pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "MFGS2_EVENT_UPDATE_FAIL", szTempBuffer );
				}
				break;

			case MFGS2_EVENT_UPDATE_CONNECTION_FAIL:
				pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);
				pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "MFGS2_EVENT_UPDATE_CONNECTION_FAIL", pGame->pOnlineWorld->pOnlineText->Get( 11105 ) );
				break;

			case MFGS2_EVENT_UPDATE_INVALID_EXECUTION:
				MFGS2_Update();
				break;
			}
		}
		break;

	case MFGS2_EVENT_RUNNING:					
		{
			switch(lCode)
			{
			case MFGS2_EVENT_RUNNING_STOP:
				{
					CHAR TempBuffer[1024];
					FILE *fp;
					fp = fopen("MFGS.txt", "at");
					sprintf(TempBuffer, "MFGS2_EVENT_RUNNING_STOP!\r\n");
					fprintf(fp, TempBuffer);
					fclose(fp);
					
					MFGS2_Init(g_bNation);
				}
				break;

			case MFGS2_EVENT_RUNNING_FAIL:		// MFGS2.exe 파일 문제 삭제 후 업데이트 시도				
				DeleteFile( "MFGS\\mfgs2.exe" );
				MFGS2_Update();
				break;
			}
		}
		break;

	case MFGS2_EVENT_BACKDOOR_FOUND:			// 해킹툴 감지됨
		{
			CHAR TempBuffer[1024];
			FILE *fp;
			fp = fopen("MFGS.txt", "at");
			sprintf(TempBuffer, "MFGS2_EVENT_BACKDOOR_FOUND!\r\n");
			fprintf(fp, TempBuffer);
			fclose(fp);

			if( lCode == MFGS2_EVENT_BACKDOOR_FOUND_GAME )
			{
				pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);
				pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "Hacking Tool", (char*)pExParam );
			}
		}
		break;
	}

	return 0;
}

void MFGS2_Update()
{
	int nResult = MFGS2_StartUpdate( MFGS2_UPDATE_HTTP, pGame->pOnlineWorld->ReadText("Online\\url.txt"), NULL, NULL, MFGS2_UPDATE_SHOW_HIDE, 0 );	
	CHAR	szTempBuffer[512];

	switch( nResult )
	{
	case MFGS2_ERR_OK:
	case MFGS2_ERR_ALREADY_EXECUTE_MFGS:
	case MFGS2_ERR_ALREADY_EXECUTE_UPDATE:
		break;

	case MFGS2_ERR_NOT_INITIALIZED:
		{			
			FILE *fp;
			fp = fopen("MFGS.txt", "at");
			sprintf(szTempBuffer, "MFGS2_ERR_NOT_INITIALIZED!\r\n");
			fprintf(fp, szTempBuffer);
			fclose(fp);
			MFGS2_Init(g_bNation);
		}
		break;	

	case MFGS2_ERR_NOT_INSTALLED:		
		{
			FILE *fp;			
			fp = fopen("MFGS.txt", "at");
			sprintf(szTempBuffer, "MFGS2_ERR_NOT_INSTALLED!\r\n");
			fprintf(fp, szTempBuffer);
			fclose(fp);

			pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);		
			sprintf( szTempBuffer, "%s [%d] 에러 번호를 자유 게시판에 올려 주세요.", pGame->pOnlineWorld->pOnlineText->Get( 11108 ), GetLastError() );		
			pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "MFGS2_ERR_NOT_INSTALLED", szTempBuffer );		
		}
		break;

	case MFGS2_ERR_UPDATE_EXCUTE_FAIL:		
		{
			FILE *fp;
			fp = fopen("MFGS.txt", "at");
			sprintf(szTempBuffer, "MFGS2_ERR_NOT_INITIALIZED!\r\n");
			fprintf(fp, szTempBuffer);
			fclose(fp);

			pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);
			sprintf( szTempBuffer, "%s [%d] 에러 번호를 자유 게시판에 올려 주세요.", pGame->pOnlineWorld->pOnlineText->Get( 11109 ), GetLastError() );		
			pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "MFGS2_ERR_UPDATE_EXCUTE_FAIL", szTempBuffer );
		}
		break;

	default:		
		pGame->pOnlineWorld->pOnlineCharUI->SetAction(FALSE);
		sprintf( szTempBuffer, "%s [%d] 에러 번호를 자유 게시판에 올려 주세요.", pGame->pOnlineWorld->pOnlineText->Get( 11104 ), GetLastError() );		
		pGame->pOnlineWorld->pOnlineMsgBoxError->SetMsg( ON_MSGBOX_TYPE_OK, "UPDATE_ERROR", szTempBuffer );
		break;
	}
}

VOID MFGS2_Init(BOOL bNation)
{
	CHAR TempBuffer[1024];
	FILE *fp;
	fp = fopen("MFGS.txt", "at");
	sprintf(TempBuffer, "InitStart!\r\n");
	fprintf(fp, TempBuffer);
	fclose(fp);

	g_bNation = bNation;
	g_bInit = TRUE;

	// MFGS 초기화	
	if( g_bNation == FALSE )	MFGS2_Initialize( MFGS2_Callback, false, false, 3001, "38B90C40D460112B" );		// 한국, 대만, 홍콩
	else						MFGS2_Initialize( MFGS2_Callback, false, false, 3002, "693B94653B423BE8" );		// 일본

	// 파일 무결성 체크
	if( MFGS2_CheckFileIntegrity( true ) == MFGS2_ERR_NOT_INITIALIZED )
	{
		if( g_bNation == FALSE )	MFGS2_Initialize( MFGS2_Callback, false, false, 3001, "38B90C40D460112B" );		// 한국, 대만, 홍콩
		else						MFGS2_Initialize( MFGS2_Callback, false, false, 3002, "693B94653B423BE8" );		// 일본

		MFGS2_CheckFileIntegrity( true );
	}

	MFGS2_Update();

	CHAR TempBuffer1[1024];
	FILE *fp1;
	fp1 = fopen("MFGS.txt", "at");
	sprintf(TempBuffer1, "InitEnd!\r\n");
	fprintf(fp1, TempBuffer1);
	fclose(fp1);
}

VOID MFGS2_Free()
{
	CHAR TempBuffer[1024];
	FILE *fp;
	fp = fopen("MFGS.txt", "at");
	sprintf(TempBuffer, "FreeStart!\r\n");
	fprintf(fp, TempBuffer);
	fclose(fp);

	while(g_bInit)												// MFGS2_Initialize()를 했나안했나 체크 
	{
		if(g_bMFGS_Start)										// MFGS2_Start()가 성공했나 체크
		{
			CHAR TempBuffer[1024];
			FILE *fp;
			fp = fopen("MFGS.txt", "at");
			sprintf(TempBuffer, "MFGS2_Terminate Start!\r\n");
			fprintf(fp, TempBuffer);
			fclose(fp);
			
			int error;
			if((error = MFGS2_Terminate()) == MFGS2_ERR_OK)
			{
				CHAR TempBuffer[1024];
				FILE *fp;
				fp = fopen("MFGS.txt", "at");
				sprintf(TempBuffer, "MFGS2_Terminate OK!\r\n");
				fprintf(fp, TempBuffer);
				fclose(fp);
				
				MFGS2_Uninitialize();
				break;
			}
			else if(error == MFGS2_ERR_NOT_INITIALIZED)			// MFGS2_Initialize()를 하지 않았으면 MFGS2_Terminate()와 MFGS2_Uninitialize()가 필요없음
			{
				CHAR TempBuffer[1024];
				FILE *fp;
				fp = fopen("MFGS.txt", "at");
				sprintf(TempBuffer, "MFGS2_ERR_NOT_INITIALIZED!\r\n");
				fprintf(fp, TempBuffer);
				fclose(fp);
				break;
			}
			else if(error == MFGS2_ERR_NOT_YET_RUNNING_MFGS)
			{
				CHAR TempBuffer[1024];
				FILE *fp;
				fp = fopen("MFGS.txt", "at");
				sprintf(TempBuffer, "MFGS2_ERR_NOT_YET_RUNNING_MFGS!\r\n");
				fprintf(fp, TempBuffer);
				fclose(fp);
				break;
			}
			else												// MFGS2_Terminate()실패 시 재시도
			{
				CHAR TempBuffer2[1024];
				FILE *fp2;
				fp2 = fopen("MFGS.txt", "at");
				sprintf(TempBuffer2, "FreeError! : %d\r\n", error);
				fprintf(fp2, TempBuffer2);
				fclose(fp2);
				Sleep(400);
			}
		}
		else
		{
			CHAR TempBuffer2[1024];
			FILE *fp2;
			fp2 = fopen("MFGS.txt", "at");
			sprintf(TempBuffer2, "g_bMFGS_Start Not!\r\n");
			fprintf(fp2, TempBuffer2);
			fclose(fp2);
			Sleep(100);
		}
	}
	
	g_bMFGS_Start = FALSE;
	g_bInit = FALSE;

	CHAR TempBuffer2[1024];
	FILE *fp2;
	fp2 = fopen("MFGS.txt", "at");
	sprintf(TempBuffer2, "FreeEnd!\r\n");
	fprintf(fp2, TempBuffer2);
	fclose(fp2);
}
*/