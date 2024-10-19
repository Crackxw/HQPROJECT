
#include "Client_GameList_Sockets.h"

//Listen,Connect 중 하나로 TCP 소켓을 연다. 
ckBOOL TCP_C :: Init(SocketType sType, char * cIP, short cPort)
{

	IsActive = false;
	IsConnect = false;

	SocketState = sType;

	
	if( sType == Type_Listen ) //Listen 으로 소켓을 연다..
	{

		
		//리슨용 소켓에서 포트를 지정해주지 않으면 에러를 출력하고 리턴한다.
		if(cPort < 1024) {//내서버의 포트..
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
		MySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( MySocket < 0)	{ return false;	}
		
		// IP와 Port로 bind한다.
		int siErrCode = bind(MySocket, (struct sockaddr *)&MySocketAddr, sizeof(MySocketAddr));		
		if(siErrCode < 0)		{	siErrCode = 0; return false;	}		
		
		// connection들을 listen을 하고 대기자를 LOGIN_BACKLOG 수만큼 대기가능하게 한다.
		listen(MySocket, LOGIN_BACKLOG);

		IsActive = true;		
		
	}
	else if(sType == Type_Connect) //Connect용으로 소켓을 연다.
	{
		if(cPort < 1024) {//접속할 곳의 포트..
			//printf("Listen 소켓에서 포트를 지정해주지 않았거나 허용된 범위의 포트가 아니다\n");
			return false;
		}
		
		sockaddr_in ServerSA;//접속할 서버의 소켓구조체.. SocketAdress <- 현재 컴의 소켓구조체..
		
		memset(&ServerSA, 0, sizeof(ServerSA));
		memset(&MySocketAddr, 0, sizeof(MySocketAddr));
	
		//접속할 서버의 주소 구조체..
		ServerSA.sin_family = AF_INET;
		ServerSA.sin_addr.s_addr = inet_addr(cIP);
		ServerSA.sin_port = htons(cPort);
		

		MySocketAddr.sin_family = AF_INET;
		MySocketAddr.sin_addr.s_addr = htons(INADDR_ANY);
		
		//자동으로 포트를 지정한다..		
		MySocketAddr.sin_port = 0;
		
		
		//소켓을 생성해서 소켓 기술자인 MySocket에 넣는다..
		MySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( MySocket == INVALID_SOCKET )	{ return false;	}
		
		// IP와 Port로 bind한다.
		int siErrCode = bind(MySocket, (struct sockaddr *)&MySocketAddr, sizeof(MySocketAddr));		
		if(siErrCode < 0)		{	siErrCode = 0; return false;	}				
		
		//주어진 서버의 IP와 포트로 접속을 시도한다.
		int err = connect(MySocket,(sockaddr *)&ServerSA,sizeof(sockaddr));
		if(err != 0) {			//에러
			//printf("주어진 서버의 IP와 포트로 접속을 할수 없습니다\n");
			return false;
		}
		
		//Connect 소켓을 쓸때는 자신의 MySocket으로 데이타를 주고받기때문..
		procsocket = MySocket; 

		IsActive = true;		
		IsConnect = true;
	}
	else return false;

	//버퍼의 크기를 조절하는 부분.. 별로 필요 없어보여서 끈다..
/*	int rcvb=32768,sndb=32768,optlen=sizeof(int);
	setsockopt(MySocket,SOL_SOCKET,SO_RCVBUF,(char *)&rcvb,optlen);
	setsockopt(MySocket,SOL_SOCKET,SO_SNDBUF,(char *)&sndb,optlen);

	getsockopt(MySocket,SOL_SOCKET,SO_RCVBUF,(char *)&rcvb,&optlen);
	getsockopt(MySocket,SOL_SOCKET,SO_SNDBUF,(char *)&sndb,&optlen);
	printf("받기 버퍼 크기: %d, 보내기 버퍼크기: %d\n",rcvb,sndb);
	*/

	ResetSets(MySocket);		
	return true;

}


ckBOOL TCP_C :: Init(SOCKET s) 
{
	//이미 접속이 허용된 소켓을 받아서 멤버소켓핸들에 넣는다..
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
	ErrorCode = 0;	
	
	MySocket = s;
	procsocket = s;
	
	SocketState = Type_Accept;
	IsActive = true;
    IsConnect = true;
	
	return true;
	
}




//소켓에 데이타를 보낸다.
ckBOOL TCP_C :: SendData()
{

	if(IsActive == false) return false;
	if(IsConnect == false) return false;
	
	if(WriteBufferLen <= 0) return false; //혹시나 하고..
	if(Writeable(procsocket) == false) return false;

	int sendsize;
	
	sendsize = ::send(procsocket,(char *)WriteBuf,WriteBufferLen,0);
	if(sendsize == SOCKET_ERROR)
	{
		//printf("SendData()에러..");
		ErrorCode = WSAGetLastError();
		//여기에서 소켓을 끝낼까 말까..
		return false;
	}
	
	WriteBufferLen -=sendsize;
	//보낸만큼 버퍼를 지운다.
	MemCpyInSameBuf((char *)WriteBuf,(char *)(&WriteBuf[sendsize]),WriteBufferLen);

	return true;
}

ckBOOL TCP_C :: RecvData()
{
	if(IsActive == false) 	return false;
	if(IsConnect == false)	return false;
	if(ReadBufferLen >= CK_MAX_BUFFER_SIZE) {	return false;	}
	if(ReadBufferLen < 0) ReadBufferLen = 0; //혹시나하고..
	if(Readable(procsocket) == false) return false;	
	int recvsize;
	recvsize = ::recv(procsocket,(char *)&ReadBuf[ReadBufferLen],CK_MAX_BUFFER_SIZE-ReadBufferLen,0);		
	if(recvsize == SOCKET_ERROR ) 
	{
		//printf("Recv 에러에러!: %d\n",WSAGetLastError());
		return false;
	}	
	//else printf("%d 만큼의 데이타를 받음..\n",recvsize);
	//받은 데이타가 없으면 역시 리턴..
	if(recvsize == 0) return false;
	ReadBufferLen += recvsize;	
	return true;
}


ckBOOL TCP_C :: ReadFromBuffer(PacketHeader *ph,char *Data)
{
	if( IsActive == false  ) return false;
	if( IsConnect == false ) return false;
	
	//패킷을 풀어서 임시 버퍼와 임시 헤더에 복사한다.
	if( UnPackData() == false) return false;

	//헤더와 데이타를 복사한다.
	*ph = Tmp_ReadHeader;
	memcpy(Data,Tmp_ReadBuf,Tmp_ReadBufferLen);
	return true;
}




//버퍼에 데이타를 패킷화 시켜서 버퍼에 넣는다..
ckBOOL TCP_C :: WriteToBuffer(char *Data,short datalen,short Cmd)
{
	//패킷 헤더를 만들고 수치를 저장..
	if(datalen > CK_MAX_BUFFER_SIZE) return false;
	PacketHeader pack_header;
	pack_header.Header_Size_u8 = CK_PACK_HEADER_SIZE;
	pack_header.Command_u16 = Cmd;
	pack_header.Data_Size_u16 = datalen;
	pack_header.CheckSum_s8 = 0;
	
	memcpy(Tmp_WriteBuf,Data,datalen);
	Tmp_WriteBufferLen = datalen;
	
	//그냥 보기좋으라고 두줄로..
	ckBOOL ret = PackData(&pack_header) ;	
	return ret;
}



ckBOOL  TCP_C :: Accept(SOCKET *s)
{
	sockaddr_in newSockAddr;
	int sizeofsock=sizeof(sockaddr_in);
	if(IsActive == false) return false;
	if(Readable(MySocket) == false) return false;
	SOCKET sct = accept(MySocket,(sockaddr *)&newSockAddr,&sizeofsock);
	if (sct == INVALID_SOCKET) {
		//printf("소켓을 생성할수 없소!");
		return false;
	}
	*s = sct;
	IsConnect = true;
	
	return true;
}