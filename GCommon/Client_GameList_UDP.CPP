
#include "Client_GameList_Sockets.h"

//Listen,Connect 중 하나로 TCP 소켓을 연다. 
ckBOOL UDP_C :: Init(SocketType sType, short cPort)
{

	IsActive = false;
	
	WSAData wsadata;
	int ret;

	ret=WSAStartup(MAKEWORD(2,2),&wsadata);

	if(ret!=0)
	{
		return false;
	}
	if(LOBYTE(wsadata.wVersion)!=2 || HIBYTE(wsadata.wVersion)!=2)
	{
		WSACleanup();
		return false;
	}

	ck_timeout.tv_sec	= 0;
	ck_timeout.tv_usec	= DEFAULT_TIMEOUT;
	

	SocketState = sType;

	
	if( sType == Type_Listen ) //Listen 으로 소켓을 연다..
	{

		
		//리슨용 소켓에서 포트를 지정해주지 않으면 에러를 출력하고 리턴한다.
		if(cPort < 1024) {
			//printf("Listen 소켓에서 포트를 지정해주지 않았거나 허용된 범위의 포트가 아니다\n");
			return false;
		}
		//구조체 초기화..
		memset(&MySocketAddr, 0, sizeof(MySocketAddr));
	
		MySocketAddr.sin_family = AF_INET;
		MySocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(cIP);INADDR_ANY 는 아무거나임..
		//포트를 설정하고
		MySocketAddr.sin_port = htons(cPort);
		
		//소켓을 생성해서 소켓 기술자인 MySocket에 넣는다..
		MySocket = socket(AF_INET, SOCK_DGRAM, 0);
		if( MySocket < 0)	{ return false;	}
		
		// IP와 Port로 bind한다.
		int siErrCode = bind(MySocket, (struct sockaddr *)&MySocketAddr, sizeof(MySocketAddr));		
		if(siErrCode < 0)		{	siErrCode = 0; return false;	}		
				
		IsActive = true;		
		
	}
	else if(sType == Type_Connect) //Connect용으로 소켓을 연다.
	{	
		//memset(&ServerSA, 0, sizeof(ServerSA));
		memset(&MySocketAddr, 0, sizeof(MySocketAddr));
	
		//접속할 서버의 주소 구조체 설정
		//이지만.. 여기서 하지않고 SetSendAddress 인가 그 함수에서 한다.
		//ServerSA.sin_family = AF_INET;
		//ServerSA.sin_addr.s_addr = inet_addr(cIP);
		//ServerSA.sin_port = htons(cPort);
		
		MySocketAddr.sin_family = AF_INET;
		MySocketAddr.sin_addr.s_addr = htons(INADDR_ANY);
		
		//자동으로 포트를 지정한다..		
		MySocketAddr.sin_port = 0;
		
		
		//소켓을 생성해서 소켓 기술자인 MySocket에 넣는다..
		MySocket = socket(AF_INET, SOCK_DGRAM, 0);
		if( MySocket < 0)	{ return false;	}
		
		// IP와 Port로 bind한다.
		int siErrCode = bind(MySocket, (struct sockaddr *)&MySocketAddr, sizeof(MySocketAddr));		
		if(siErrCode < 0)		{	siErrCode = 0; return false;	}				
		
		IsActive = true;		
		
	}
	else return false;
	//버퍼의 크기를 조절하는 부분.. 별로 필요 없어보여서 끈다..
/*
	int rcvb=32768,sndb=32768,optlen=sizeof(int);
	setsockopt(MySocket,SOL_SOCKET,SO_RCVBUF,(char *)&rcvb,optlen);
	setsockopt(MySocket,SOL_SOCKET,SO_SNDBUF,(char *)&sndb,optlen);

	getsockopt(MySocket,SOL_SOCKET,SO_RCVBUF,(char *)&rcvb,&optlen);
	getsockopt(MySocket,SOL_SOCKET,SO_SNDBUF,(char *)&sndb,&optlen);
	printf("받기 버퍼 크기: %d, 보내기 버퍼크기: %d\n",rcvb,sndb);
*/
	ResetSets(MySocket);		
	return true;

}

/*
ckBOOL UDP_C :: SetSendAddress(sockaddr_in *sockaddr)
{	
	if(ServerSA->sin_family != AF_INE) {
		printf("문제문제.. AF_INE이 아니다..");
		return false;
	}	
	ServerSA.sin_family = *sockaddr;
}
*/

//버퍼에서 하나의 헤더와 그에딸린 데이타를 임시버퍼에 넣은후..
//임시 읽기 버퍼에서 가져온다.(UnPackData())
ckBOOL UDP_C :: ReadFromBuffer(sockaddr_in *sa,char *Data,PacketHeader *ph)
{
	if( IsActive == false  ) return false;
	
	//데이타를 소켓에서 받고.	
	RecvData();

	//패킷을 풀어서 임시 버퍼와 임시 헤더에 복사한다.
	if( UnPackData() == false) return false;

	//데이타를 받아온 주소의 구조체를 sa 에 복사해서 돌려보낸다..
	*sa = RecvAddr;
		
	//헤더와 데이타를 복사한다.
	*ph = Tmp_ReadHeader;
	memcpy(Data,Tmp_ReadBuf,Tmp_ReadBufferLen);

	return true;
}

//버퍼에 데이타를 패킷화 시켜서 버퍼에 넣고 보낸다..(UDP 라서 바로바로 보낸다..)
ckBOOL UDP_C :: WriteToBuffer(sockaddr_in *sa,char *Data,short datalen,short Cmd)
{
	//패킷 헤더를 만들고 수치를 저장..
	if(datalen > CK_MAX_BUFFER_SIZE)
	{
		return false;
	}


	PacketHeader pack_header;
	pack_header.Header_Size_u8 = CK_PACK_HEADER_SIZE;
	pack_header.Command_u16 = Cmd;
	pack_header.Data_Size_u16 = datalen;
	pack_header.CheckSum_s8 = 0;
	
	memcpy(Tmp_WriteBuf,Data,datalen);
	Tmp_WriteBufferLen = datalen;
	
	//패킷화 시킨후..
	if(PackData(&pack_header) ==false )
	{
		return false ;	
	}

	//보낼 주소의 구조체를 설정한후..
	SendAddr = *sa; 
	//데이타를 보낸다..
	SendData();
	return true;

}


//소켓에 데이타를 보낸다.
ckBOOL UDP_C :: SendData()
{
	int sizeofsa = sizeof(sockaddr_in);

	if(IsActive == false) return false;
		
	if(WriteBufferLen <= 0) return false; //혹시나 하고..
	if(Writeable(MySocket) == false) return false;

	int sendsize;
	
	sendsize = ::sendto(MySocket,(char *)WriteBuf,WriteBufferLen,0,(sockaddr *)&SendAddr,sizeofsa);
	if(sendsize == SOCKET_ERROR)
	{
		//printf("SendData()에러..: %d\n",ErrorCode = WSAGetLastError());				
		return false;
	}
	
	WriteBufferLen -=sendsize;
	//보낸만큼 버퍼를 지운다.
	MemCpyInSameBuf((char *)WriteBuf,(char *)(&WriteBuf[sendsize]),WriteBufferLen);

	return true;
}

ckBOOL UDP_C :: RecvData()
{
	int sizeofsa = sizeof(sockaddr_in);
	if(IsActive == false) return false;
	
	if(ReadBufferLen >= CK_MAX_BUFFER_SIZE) {	return false;	}
	if(ReadBufferLen < 0) ReadBufferLen = 0; //혹시나하고..
	if(Readable(MySocket) == false) return false;	
	int recvsize;
	recvsize = ::recvfrom(MySocket,(char *)&ReadBuf[ReadBufferLen],CK_MAX_BUFFER_SIZE-ReadBufferLen,0,(sockaddr *)&RecvAddr,&sizeofsa);	
	if(recvsize == SOCKET_ERROR ) 
	{
		//printf("Recv 에러에러!: %d",WSAGetLastError());
		return false;
	}	
	//받은 데이타가 없으면 역시 리턴..
	if(recvsize == 0) return false;
	ReadBufferLen += recvsize;	
	return true;
}
