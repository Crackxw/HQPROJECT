//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//				   윈속을 관리하는 클래스..					
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#ifndef __CLIENT_GAMELIST_SOCKETS__
#define __CLIENT_GAMELIST_SOCKETS__


//#include <winsock2.h>

struct PacketHeader //2001.12.3일자 새로운 패킷헤더..
{
	//이 구조체의 사이즈 생성할때마다 sizeof 로 넣어줘야 한다.
	unsigned char Header_Size_u8;

	
	//체크섬 을 위한 데이타..
	char CheckSum_s8;	

	//패킷에 딸려오는 데이타 사이즈 이 헤더의 크기는 포함 안됨.
	unsigned short Data_Size_u16;

	//요청된 명령의 종류을 가진 구조체
	// 0으로 정의된 명령은 무조건 에러 코드로 정의한다..
	unsigned short Command_u16;

	
	//2바이트 정렬을 위해서 사용한다....
	//현재 아무런 용도도 없다(..)
	//6바이트가 채워쳐서 안쓴다..
	//char None;

	//더 필요한게 있으면 이 아래에 추가한다
};

#define CK_MAX_BUFFER_SIZE					512
// 패킷 헤더 길이..
#define CK_PACK_HEADER_SIZE				(sizeof(PacketHeader) )
#define	LOGIN_BACKLOG					50
#define	DEFAULT_TIMEOUT					30

//Bool 형 대신 쓰려고..
typedef  char     ckBOOL;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	TCP 클래스 선언..
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//어떤 타입의 소켓인지..(LISTEN: 접속을 받아들이는 소켓,CONNECT  다른서버로 접속하는 소켓)
//ACCEPT : 접속이 들어와서 accept 로 리턴된 소켓
enum SocketType { Type_Listen = 0, Type_Connect, Type_Accept };


#include <winsock.h>

class Socket_C 
{
//변수 선언━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:

protected:
	
	timeval		ck_timeout;
	fd_set		ck_fdPlayer;
	fd_set		ck_fdError;	
	
	int ReadBufferLen;//버퍼의 현재 길이..
	int WriteBufferLen;
	//임시 버퍼의 현재 길이..
	int Tmp_ReadBufferLen;
	int Tmp_WriteBufferLen;

	//소켓에서 쓰는 버퍼
	unsigned char WriteBuf[CK_MAX_BUFFER_SIZE];
	//소켓에서 읽는 버퍼
	unsigned char ReadBuf[CK_MAX_BUFFER_SIZE];
	
	//Pack,UnPack 함수에서 사용하는 임시 공간.
	unsigned char Tmp_WriteBuf[CK_MAX_BUFFER_SIZE];
	unsigned char Tmp_ReadBuf[CK_MAX_BUFFER_SIZE];
	PacketHeader Tmp_ReadHeader;

	SOCKET MySocket;

	//쓸수있는 상태인지 아닌지....
	ckBOOL IsActive;
	//다른 소켓과 연결된 상태인지 아닌지..
	ckBOOL IsConnect; 

	//현재 소켓 상태(UDP,TCP 와 Listen 용인지 Connect 용인지..)
	SocketType SocketState;
	
	//가장 최근의 에러를 가지는 넘..
	int ErrorCode;
public:
	sockaddr_in MySocketAddr;


//메소드 선언━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
private:
protected:
	//임시 쓰기 버퍼에있는 데이타를 패킷화 해서 쓰기 버퍼에쓴다..
	//헤더에는 커맨드가 꼭 들어와야한당..
	virtual ckBOOL PackData(PacketHeader *pack_header);
	//읽기 버퍼에서  패킷을 잘라내서 임시 읽ㄱ 버퍼에 넣는다.	
	//데이타는 버퍼에, 페허ㄷ는 Tmp_ReadHeader 에 넣는다.
	virtual ckBOOL UnPackData();


public:
	
	Socket_C();
	~Socket_C();

	//소켓 초기화..및 접속.. 일단은 자식 클래스에서 따로선언하게..
	//virtual ckBOOL	Init(SocketType sState;,char * cIP, short cPort) const;	
	//접속을 끊는다.
	virtual ckBOOL	EndSocket () ;
		

	//소켓에서 데이타를 받고 보낸다.
	
	virtual ckBOOL SendData() = 0;	
	virtual ckBOOL RecvData() = 0;

	//버퍼를 전송한다. 일단은 SendData를 쓴다..
	//virtual void	FlushOutBuffer();

	//UDP는 데이타를 패킷화시킨후 바로바로 FlushOut한다.
	//TCP는 특별한 경우나 폴링이 한번 돌아올때마다 보낸다	


	////데이타를 패킷구조에 맞게 정리해서 쓰기 버퍼에 넣는다. Cmd: 커맨드 코드..
	//virtual ckBOOL	WriteToBuffer(char *Data,short datalen,char Cmd);
	
	
	
	
	//주어진 길이만큼 삭제한다.
	void			EraseBuf_R(int Length);   //읽기 버퍼
	void			ErasenBuf_W(int Length);  //쓰기 버퍼
	void			EraseBuf_TR(int Length);  //임시 읽기 버퍼 
	void			EraseBuf_TW(int Length);  //임시 쓰기 버퍼

	//가장 최근의 에러를 출력한후 ErrorCode를 초기화 시킨다. 	
	void PrintfError();	

	//헤더를 떼어낸 임시 읽기 버퍼와 헤더의 값을 출력한다.
	void PrintfTmpReadBuffer();

	//현재 소켓을 리턴..
	inline SOCKET GetMySocket() { return MySocket;}

	//현재 읽기 버퍼를 복사해서 리턴해준다..
	inline void GetReadBuffer(char *data,int *len){ memcpy(data,ReadBuf,ReadBufferLen); *len = ReadBufferLen;}

	//현재 읽기 버퍼를 받아서 복사한다..
	inline void SetReadBuffer(char *data,int len){ memcpy(ReadBuf,data,len); ReadBufferLen = len;}
	
	//버퍼 초기화	
	inline void ClearReadBuffer() { memset(ReadBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearWriteBUffer() { memset(WriteBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearReadBuffer_Tmp() { memset(Tmp_ReadBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearWriteBUffer_Tmp() { memset(Tmp_WriteBuf,0,CK_MAX_BUFFER_SIZE);}
	
	
	
	inline void			ResetSets(SOCKET psock)				{ FD_ZERO(& ck_fdError); FD_ZERO(& ck_fdPlayer); FD_SET(psock,& ck_fdError); FD_SET(psock,& ck_fdPlayer); return; };
	inline ckBOOL		Readable(SOCKET psock)				{ ResetSets(psock); select(0,& ck_fdPlayer,NULL,& ck_fdError,&ck_timeout); if(FD_ISSET(psock,& ck_fdError)) { EndSocket(); return false; } else if(FD_ISSET(psock,& ck_fdPlayer)) return true; return false;};
	inline ckBOOL		Writeable(SOCKET psock)				{ ResetSets(psock); select(0,NULL,& ck_fdPlayer,& ck_fdError,&ck_timeout); if(FD_ISSET(psock,& ck_fdError)) { EndSocket(); return false; } else if(FD_ISSET(psock,& ck_fdPlayer)) return true; return false;};	

	//같은 버퍼에서 데이타를 이동시킬때 사용한다. 
	inline void		MemCpyInSameBuf(char * dest,char * src, int len)
	{
		//일단은 이렇게 .. 버퍼의 크기가 작아서 속도에 영향이 클거 같지는 않다.
		for(int i=0;i< len; i++)	dest[i] = src[i];		
	}
};





//---------------------------------------------------------------------------------------------------------------------
//TCP 구현 소켓..
//Listen용으로 소켓을 열었을때는 SetProcSocket 를 써서 데이타를 보내고 받을 소켓을 매번 지정해주어야한다.
//Connect 일때는 procsocket = MySocket 으로 초기화 함수에서 자동으로 초기화해준돠..
//데이타를 받을때는 RecvData를 먼저 호출한후 ReadFromBuffer 로 데이타를 가져오고
//다이타를 보낼때는 SetProcSocket로 먼저 보낼 소켓을 지정하고, WriteToBuffer 로 데이타를 쓴후에 SendData 로 보낸다..

class TCP_C : public Socket_C 
{
private:
	
public:
	SOCKET procsocket;

	TCP_C() : Socket_C() { }; //아직은 하는일 없음..
	~TCP_C() { closesocket(procsocket); EndSocket();}

	//listen용일때는 IP는 필없고 Port(내서버의 포트) 만 지정해주면된다..
	//connect 용일때는 IP와 Port(접속할곳의 포트) 를 모두 지정해 주어야 한다. Port 
	ckBOOL Init(SocketType sState,char * cIP, short cPort);

	//이미 접속된 소켓을 받아서 클래스를 초기화한다. 
	ckBOOL Init(SOCKET s);
		
	//접속을 허용하고 소켓을 돌려준다 소켓이 접속되었다면 true, 그렇지 않을경우엔 false;
	ckBOOL Accept(SOCKET *s);

	//버퍼에서 하나의 헤더와 그에딸린 데이타를 임시버퍼에 넣은후..
	//임시 읽기 버퍼에서 가져온다.(UnPackData())
	ckBOOL	ReadFromBuffer(PacketHeader *ph,char *Data);

	ckBOOL	WriteToBuffer(char *Data,short datalen,short Cmd);
	//데이타 입출력을 담당할 소켓을 설정한다.
	void SetProcSocket(SOCKET s) { procsocket = s;}

	//버퍼에 데이타를 받고 보낸다.	
	ckBOOL	 SendData();	
	ckBOOL	 RecvData();
};






//---------------------------------------------------------------------------------------------------------------------
//UDP 구현 소켓..
//WriteToBuffer 를 호출하면 자동으로 Send를 하고
//ReadFromBUffer 를 호출하면 자동으로 Recv한다.
class UDP_C : public Socket_C
{

private:
	
	//데이타를 보낼 주소구조체
	sockaddr_in SendAddr;

	//데이타를 받은 주소의 구조체..
	//RecvData 에서 이것을 설정하고 ReadFromBuffer 에서 이 구조체를 복사해서 유저에게 보낸다....
	sockaddr_in RecvAddr;
	
	//버퍼에 데이타를 받고 보낸다.	
	//UDP 소켓은 버퍼에 쌓이는 일 없이 바로 보내기때문에 WriteToBuffer,ReadFromBuffer 을 호출하자마자
	//바로 보내버린돠..
	
	ckBOOL	 SendData();	
	ckBOOL	 RecvData();

public:
	UDP_C()	: Socket_C() {  };
	~UDP_C() {EndSocket();}

	ckBOOL	Init(SocketType sState,short port);

	
	//쓰기 버퍼에 데이타 패킷으로 만들어서 복사한후에 보낸다.(UDP라서 바로바로 보낸다..)
	//sa 는 데이타를 보낼 주소의 구조체
	ckBOOL	WriteToBuffer(sockaddr_in *sa,char *Data,short datalen,short Cmd);

	//RecvData로 소켓에서 데이타를 복사하고 
	//버퍼에서 하나의 헤더와 그에딸린 데이타를 임시버퍼에 넣은후..
	//임시 읽기 버퍼에서 가져온다.(UnPackData())
	//sa 는 받은 주소의 구조체 
	ckBOOL	ReadFromBuffer(sockaddr_in *sa,char *Data,PacketHeader *ph);

};



#endif