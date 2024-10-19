
#include <GSL.h>
#include <Main.h>
#include "OnlineGameMsg.h"
#include "OnlineClient.h"

#include <mynet.h>

//----------------------------------------------------------------------------------------------------------------
//	����	:	����Ÿ�� ó���Ѵ�.
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

	// �����͸� ó���Ѵ�.
	if(siInBufferLen > ON_PACK_HEADER_SIZE)
	{
		uiRet = CommandProc();						// ������ ��ɾ ó���Ѵ�.		
	}

	switch ( Writeable() )
	{
		case TRUE:
			if ( FlushOutBuffer() == FALSE )				// �ϰ������� �����Ѵ�.
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
//	����	:	������ ��ɾ ó���Ѵ�.
//----------------------------------------------------------------------------------------------------------------
UI16	OnlineClient::CommandProc()
{
	int		uiRet = 0;
	OnMsg	*pOnMsg;								// � �޽��� ���� �ǵ��ϱ� ���Ͽ� �ʿ��ϴ�.

	if (UnpackMessage())							// 1 byte�� �Է¹��۷� �о�鿩 
	{			
		pOnMsg	=	(OnMsg *)cMsg;		
		uiRet   =   pOnMsg->dwMsgType;

		// ���� ���� �޽����� ������ ó���Ѵ�. 
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