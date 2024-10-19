
#include "Client_GameList_Sockets.h"
//소켓 최상위 클래스 구현..


  

Socket_C :: Socket_C() 
{
	//멤버 변수,배열 초기화..
	memset(&MySocketAddr,0,sizeof(sockaddr_in));
	ReadBufferLen = 0;
	WriteBufferLen = 0;
	Tmp_ReadBufferLen  = 0;
	Tmp_WriteBufferLen = 0;

	memset(WriteBuf,0,sizeof(WriteBuf));
	memset(ReadBuf,0,sizeof(ReadBuf));
	memset(Tmp_WriteBuf,0,sizeof(Tmp_ReadBuf));
	memset(Tmp_ReadBuf,0,sizeof(Tmp_ReadBuf));


	memset(&Tmp_ReadHeader ,0,sizeof(Tmp_ReadHeader));
	MySocket = 0;
	IsActive = false;
	IsConnect = false;
	ErrorCode = 0;	
}

Socket_C :: ~Socket_C ()
{
	EndSocket();	
}


ckBOOL Socket_C :: EndSocket()
{
	if( IsActive == true)	{
		closesocket(MySocket);		
	}
	WSACleanup();
	IsActive = false;
	return true;
}



void Socket_C :: PrintfError()
{
	//printf("마지막으로 리턴된 에러코드: %d\n", ErrorCode);
}

//현재 임시 읽기 버퍼와 임시 헤더의 내용을 화면에  출력한다.
void Socket_C :: PrintfTmpReadBuffer()
{/*
	char aa[CK_MAX_BUFFER_SIZE];
	memcpy(aa,Tmp_ReadBuf,Tmp_ReadBufferLen);
	aa[Tmp_ReadBufferLen] = '\0';
	//printf("버퍼내용: %s\n 헤더크기: %d, 체크섬: %d, 데이타 길이: %d, 커맨드: %d",\
		aa,Tmp_ReadHeader.Header_Size_u8,Tmp_ReadHeader.CheckSum_s8,Tmp_ReadHeader.Data_Size_u16,Tmp_ReadHeader.Command_u16);
*/
}

//읽기 버퍼삭제
void Socket_C :: EraseBuf_R(int Length)
{
	if(IsActive == false ) return;
	if(Length > ReadBufferLen) return;
	
	ReadBufferLen -= Length;
	MemCpyInSameBuf((char *)ReadBuf,(char *)&ReadBuf[Length],ReadBufferLen);	
}

//쓰기 버퍼삭제
void Socket_C :: ErasenBuf_W(int Length)
{
	if(IsActive == false ) return;
	if(Length > WriteBufferLen) return;
	
	WriteBufferLen -= Length;
	MemCpyInSameBuf((char *)WriteBuf,(char *)&WriteBuf[Length],WriteBufferLen);		  
}

//임시 읽기 버퍼 삭제
void Socket_C :: EraseBuf_TR(int Length)
{  
	if(IsActive == false ) return;
	if(Length > Tmp_ReadBufferLen ) return;
	
	Tmp_ReadBufferLen  -= Length;
	MemCpyInSameBuf((char *)Tmp_WriteBuf,(char *)&Tmp_WriteBuf[Length],Tmp_ReadBufferLen );		  


}

//임시 쓰기 버퍼
void Socket_C :: EraseBuf_TW(int Length)
{
	if(IsActive == false ) return;
	if(Length > Tmp_WriteBufferLen ) return;
	
	Tmp_WriteBufferLen  -= Length;
	MemCpyInSameBuf((char *)Tmp_WriteBuf,(char *)&Tmp_WriteBuf[Length],Tmp_WriteBufferLen );

}


//임시 쓰기 버퍼에있는 데이타를 패킷화 해서 쓰기 버퍼에쓴다..
//헤더에는 커맨드가 꼭 들어와야 한다.
ckBOOL Socket_C :: PackData(PacketHeader *pack_header)
{
	if(IsActive == false)
	{
		MessageBox( NULL, "", "IsActive == false", MB_OK|MB_TOPMOST );
		return false;
	}
	//UDP 일때도 있으니..
	//if(IsConnect == false ) return false;
	
	//패킷 헤더 검사..------------------------------------------------------
	if(pack_header->Header_Size_u8 != CK_PACK_HEADER_SIZE)
	{
		MessageBox( NULL, "", "pack_header->Header_Size_u8 != CK_PACK_HEADER_SIZE", MB_OK|MB_TOPMOST );
		return false;
	}	
	if(pack_header->Data_Size_u16 <= 0)
	{
		MessageBox( NULL, "", "pack_header->Data_Size_u16 <= 0", MB_OK|MB_TOPMOST );
		return false;
	}
	if(WriteBufferLen > CK_MAX_BUFFER_SIZE )
	{
		MessageBox( NULL, "", "WriteBufferLen > CK_MAX_BUFFER_SIZE", MB_OK|MB_TOPMOST );
		return false;
	}
	//if(pack_header->Header.Command_u16 == PH_COMMAND_ERROR) { //printf("PackData() 잘못된 COMMAND입니다\n"));						  return false; }

	pack_header->Data_Size_u16 = Tmp_WriteBufferLen;

	// CheckSum을 구한다.
	pack_header->CheckSum_s8 = 0;
	for(int i=0; i< Tmp_WriteBufferLen; i++)
	{
		pack_header->CheckSum_s8 ^= Tmp_WriteBuf[i];
	}

	if( WriteBufferLen+Tmp_WriteBufferLen >= CK_MAX_BUFFER_SIZE) {
		//pirntf("PackData() 버퍼가 다찰거 같아서 미리 보냄\n");
		SendData();
	}

	//헤더와 임시버퍼의 내용을 쓰기버퍼에 쓴다.	
	memcpy(&WriteBuf[WriteBufferLen],pack_header,CK_PACK_HEADER_SIZE);
	WriteBufferLen +=CK_PACK_HEADER_SIZE;

	memcpy(&WriteBuf[WriteBufferLen],Tmp_WriteBuf,Tmp_WriteBufferLen);
	WriteBufferLen +=Tmp_WriteBufferLen;	
	return true;
}





//패킷을 푼다..읽이 버퍼에서 읽어서 임시 읽기 버퍼에 쓴다..
//데이타는 Tmp_ReadBuf, 헤더는 Tmp_ReadHeader로 들어간다..
ckBOOL Socket_C :: UnPackData() 
{
	if(IsActive == false) return false;
	
	//if(IsConnect == false ) return false;	
	
	PacketHeader pack_header;// 패킷 헤더	
	
	//임시버퍼 관련..초기화.
	Tmp_ReadBufferLen = 0;
	//memset(Tmp_ReadBuf, 0, sizeof(Tmp_ReadBuf)); <- 구지 할 필요가 없을거 같아서..

	/////////////////////////////////////////////////////////////////////////////////
	//헤더화일 만큼 들어왔는지 검사..

	if( ReadBufferLen > CK_PACK_HEADER_SIZE)
	{
		pack_header = *((PacketHeader *)ReadBuf);

		//헤더가 올바른지 검사한다.
		if( pack_header.Header_Size_u8 != CK_PACK_HEADER_SIZE || pack_header.Header_Size_u8 == 0) 
		{
			//printf("헤더의 크기가 올바르지 않네요?? 미어미어 \n");
			return false;
		}		
		if( pack_header.Data_Size_u16 == 0) 
		{
			
			////printf("데이타의 크기가 0인것이 들어와부렀다..\n");
			
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	//데이타가 아직 덜 들어왔다면 더 기둘린다.

	if(ReadBufferLen < pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE) 
	{
		////printf("아직 데이타가 부족해요 이잉~현재 읽기 버퍼 크기:%d\n",ReadBufferLen);
		return false;		
	}

	/////////////////////////////////////////////////////////////////////////////////
	//버퍼가 다 찼다면 버린다...

	if( pack_header.Data_Size_u16 > CK_MAX_BUFFER_SIZE) 
	{
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		//printf("버퍼 다 찼떠여 지금 온건 버릴께여..아깝..>,<\n");
		return false;
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	// CheckSum을 비교한다.
	char CheckSum=0;
	for(int i=CK_PACK_HEADER_SIZE ; i<pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE; i++) 
	{ 
		CheckSum ^= ReadBuf[i];
	}
	if(CheckSum == pack_header.CheckSum_s8)
	{
		//버퍼 길이 설정
		Tmp_ReadBufferLen = pack_header.Data_Size_u16;
		//헤더 복사.
		Tmp_ReadHeader = pack_header;
		//임시 버퍼에 복사한다.
		memcpy(&Tmp_ReadBuf,&ReadBuf[CK_PACK_HEADER_SIZE],Tmp_ReadBufferLen);		
		////printf("받은 문자열: %s\n",cMsg);
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		////printf("받은 패킷 헤더 HeaderSize %d, Cmd %d,DataSize \n %d,CheckSum %d",Tmp_ReadHeader.Header_Size_u16,Tmp_ReadHeader.Command_u8,Tmp_ReadHeader.Data_Size_u16,Tmp_ReadHeader.CheckSum_s8);
		return true;
	}
	else
	{
		// CheckSum값이 틀리면 버린다.
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		//printf("우띠 체크섬이 틀려! 어느넘이 바꿔치기 했지???\n");
	}
	

	return false;
}


