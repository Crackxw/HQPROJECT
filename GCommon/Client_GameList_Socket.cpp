
#include "Client_GameList_Sockets.h"
//���� �ֻ��� Ŭ���� ����..


  

Socket_C :: Socket_C() 
{
	//��� ����,�迭 �ʱ�ȭ..
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
	//printf("���������� ���ϵ� �����ڵ�: %d\n", ErrorCode);
}

//���� �ӽ� �б� ���ۿ� �ӽ� ����� ������ ȭ�鿡  ����Ѵ�.
void Socket_C :: PrintfTmpReadBuffer()
{/*
	char aa[CK_MAX_BUFFER_SIZE];
	memcpy(aa,Tmp_ReadBuf,Tmp_ReadBufferLen);
	aa[Tmp_ReadBufferLen] = '\0';
	//printf("���۳���: %s\n ���ũ��: %d, üũ��: %d, ����Ÿ ����: %d, Ŀ�ǵ�: %d",\
		aa,Tmp_ReadHeader.Header_Size_u8,Tmp_ReadHeader.CheckSum_s8,Tmp_ReadHeader.Data_Size_u16,Tmp_ReadHeader.Command_u16);
*/
}

//�б� ���ۻ���
void Socket_C :: EraseBuf_R(int Length)
{
	if(IsActive == false ) return;
	if(Length > ReadBufferLen) return;
	
	ReadBufferLen -= Length;
	MemCpyInSameBuf((char *)ReadBuf,(char *)&ReadBuf[Length],ReadBufferLen);	
}

//���� ���ۻ���
void Socket_C :: ErasenBuf_W(int Length)
{
	if(IsActive == false ) return;
	if(Length > WriteBufferLen) return;
	
	WriteBufferLen -= Length;
	MemCpyInSameBuf((char *)WriteBuf,(char *)&WriteBuf[Length],WriteBufferLen);		  
}

//�ӽ� �б� ���� ����
void Socket_C :: EraseBuf_TR(int Length)
{  
	if(IsActive == false ) return;
	if(Length > Tmp_ReadBufferLen ) return;
	
	Tmp_ReadBufferLen  -= Length;
	MemCpyInSameBuf((char *)Tmp_WriteBuf,(char *)&Tmp_WriteBuf[Length],Tmp_ReadBufferLen );		  


}

//�ӽ� ���� ����
void Socket_C :: EraseBuf_TW(int Length)
{
	if(IsActive == false ) return;
	if(Length > Tmp_WriteBufferLen ) return;
	
	Tmp_WriteBufferLen  -= Length;
	MemCpyInSameBuf((char *)Tmp_WriteBuf,(char *)&Tmp_WriteBuf[Length],Tmp_WriteBufferLen );

}


//�ӽ� ���� ���ۿ��ִ� ����Ÿ�� ��Ŷȭ �ؼ� ���� ���ۿ�����..
//������� Ŀ�ǵ尡 �� ���;� �Ѵ�.
ckBOOL Socket_C :: PackData(PacketHeader *pack_header)
{
	if(IsActive == false)
	{
		MessageBox( NULL, "", "IsActive == false", MB_OK|MB_TOPMOST );
		return false;
	}
	//UDP �϶��� ������..
	//if(IsConnect == false ) return false;
	
	//��Ŷ ��� �˻�..------------------------------------------------------
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
	//if(pack_header->Header.Command_u16 == PH_COMMAND_ERROR) { //printf("PackData() �߸��� COMMAND�Դϴ�\n"));						  return false; }

	pack_header->Data_Size_u16 = Tmp_WriteBufferLen;

	// CheckSum�� ���Ѵ�.
	pack_header->CheckSum_s8 = 0;
	for(int i=0; i< Tmp_WriteBufferLen; i++)
	{
		pack_header->CheckSum_s8 ^= Tmp_WriteBuf[i];
	}

	if( WriteBufferLen+Tmp_WriteBufferLen >= CK_MAX_BUFFER_SIZE) {
		//pirntf("PackData() ���۰� ������ ���Ƽ� �̸� ����\n");
		SendData();
	}

	//����� �ӽù����� ������ ������ۿ� ����.	
	memcpy(&WriteBuf[WriteBufferLen],pack_header,CK_PACK_HEADER_SIZE);
	WriteBufferLen +=CK_PACK_HEADER_SIZE;

	memcpy(&WriteBuf[WriteBufferLen],Tmp_WriteBuf,Tmp_WriteBufferLen);
	WriteBufferLen +=Tmp_WriteBufferLen;	
	return true;
}





//��Ŷ�� Ǭ��..���� ���ۿ��� �о �ӽ� �б� ���ۿ� ����..
//����Ÿ�� Tmp_ReadBuf, ����� Tmp_ReadHeader�� ����..
ckBOOL Socket_C :: UnPackData() 
{
	if(IsActive == false) return false;
	
	//if(IsConnect == false ) return false;	
	
	PacketHeader pack_header;// ��Ŷ ���	
	
	//�ӽù��� ����..�ʱ�ȭ.
	Tmp_ReadBufferLen = 0;
	//memset(Tmp_ReadBuf, 0, sizeof(Tmp_ReadBuf)); <- ���� �� �ʿ䰡 ������ ���Ƽ�..

	/////////////////////////////////////////////////////////////////////////////////
	//���ȭ�� ��ŭ ���Դ��� �˻�..

	if( ReadBufferLen > CK_PACK_HEADER_SIZE)
	{
		pack_header = *((PacketHeader *)ReadBuf);

		//����� �ùٸ��� �˻��Ѵ�.
		if( pack_header.Header_Size_u8 != CK_PACK_HEADER_SIZE || pack_header.Header_Size_u8 == 0) 
		{
			//printf("����� ũ�Ⱑ �ùٸ��� �ʳ׿�?? �̾�̾� \n");
			return false;
		}		
		if( pack_header.Data_Size_u16 == 0) 
		{
			
			////printf("����Ÿ�� ũ�Ⱑ 0�ΰ��� ���ͺη���..\n");
			
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	//����Ÿ�� ���� �� ���Դٸ� �� ��Ѹ���.

	if(ReadBufferLen < pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE) 
	{
		////printf("���� ����Ÿ�� �����ؿ� ����~���� �б� ���� ũ��:%d\n",ReadBufferLen);
		return false;		
	}

	/////////////////////////////////////////////////////////////////////////////////
	//���۰� �� á�ٸ� ������...

	if( pack_header.Data_Size_u16 > CK_MAX_BUFFER_SIZE) 
	{
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		//printf("���� �� á���� ���� �°� ��������..�Ʊ�..>,<\n");
		return false;
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	// CheckSum�� ���Ѵ�.
	char CheckSum=0;
	for(int i=CK_PACK_HEADER_SIZE ; i<pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE; i++) 
	{ 
		CheckSum ^= ReadBuf[i];
	}
	if(CheckSum == pack_header.CheckSum_s8)
	{
		//���� ���� ����
		Tmp_ReadBufferLen = pack_header.Data_Size_u16;
		//��� ����.
		Tmp_ReadHeader = pack_header;
		//�ӽ� ���ۿ� �����Ѵ�.
		memcpy(&Tmp_ReadBuf,&ReadBuf[CK_PACK_HEADER_SIZE],Tmp_ReadBufferLen);		
		////printf("���� ���ڿ�: %s\n",cMsg);
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		////printf("���� ��Ŷ ��� HeaderSize %d, Cmd %d,DataSize \n %d,CheckSum %d",Tmp_ReadHeader.Header_Size_u16,Tmp_ReadHeader.Command_u8,Tmp_ReadHeader.Data_Size_u16,Tmp_ReadHeader.CheckSum_s8);
		return true;
	}
	else
	{
		// CheckSum���� Ʋ���� ������.
		EraseBuf_R(pack_header.Data_Size_u16 + CK_PACK_HEADER_SIZE);
		//printf("��� üũ���� Ʋ��! ������� �ٲ�ġ�� ����???\n");
	}
	

	return false;
}


