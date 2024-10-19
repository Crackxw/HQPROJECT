
#include <GSL.h>
#include <Main.h>
#include "OnlineGameMsg.h"
#include "OnlineClient.h"

#include <mynet.h>

//----------------------------------------------------------------------------------------------------------------
//	설명	:	데이타를 처리한다.
//----------------------------------------------------------------------------------------------------------------
UI16	OnlineClient::DataProc()
{
	UI16		uiRet = 0;
	int			size;

	if ( ConnectOK == FALSE ) return FALSE;
	
	switch ( Readable() )
	{			
		case TRUE:
			size = Read(ON_MAX_IN_BUFFER - siInBufferLen);
			break;

		case 2:
			return FALSE;
			break;
	}	

	// 데이터를 처리한다.
	if(siInBufferLen > ON_PACK_HEADER_SIZE)
	{
		uiRet = CommandProc();						// 들어오는 명령어를 처리한다.		
	}

	switch ( Writeable() )
	{
		case TRUE:
			if ( FlushOutBuffer() == FALSE )				// 일괄적으로 전송한다.
			{			
				Close();			
			}
			break;

		case 2:
			break;
	}	

	return uiRet;
}

//----------------------------------------------------------------------------------------------------------------
//	설명	:	들어오는 명령어를 처리한다.
//----------------------------------------------------------------------------------------------------------------
UI16	OnlineClient::CommandProc()
{
	int		uiRet = 0;
	OnMsg	*pOnMsg;								// 어떤 메시지 인지 판독하기 위하여 필요하다.

	if (UnpackMessage())							// 1 byte를 입력버퍼로 읽어들여 
	{			
		pOnMsg	=	(OnMsg *)cMsg;		
		uiRet   =   pOnMsg->dwMsgType;

		// 전투 관련 메시지는 별도로 처리한다. 
		if(10100 <= pOnMsg->dwMsgType && pOnMsg->dwMsgType <= 10300)
		{
			PushOnlineBattleMsg(cMsg, siMsgLengh);
			return 0;
		}

/*
		if(pOnMsg->dwMsgType == 5 && siMsgLengh > 300)
		{
			FILE	*fp;
			CHAR	szDir[100];
			sprintf( szDir, "Online\\LoginData");
			fp = fopen(szDir, "wb");
			if( fp == NULL )	return 0;
			
			fwrite( pOnMsg, siMsgLengh,  1, fp );
			fclose( fp );
		}
*/
		AddMsgBuffer( pOnMsg->dwMsgType, cMsg, siMsgLengh );		
	}

	return uiRet;
}