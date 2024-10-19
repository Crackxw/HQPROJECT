//����������������������������������������������������������������������������
//				   ������ �����ϴ� Ŭ����..					
//����������������������������������������������������������������������������

#ifndef __CLIENT_GAMELIST_SOCKETS__
#define __CLIENT_GAMELIST_SOCKETS__


//#include <winsock2.h>

struct PacketHeader //2001.12.3���� ���ο� ��Ŷ���..
{
	//�� ����ü�� ������ �����Ҷ����� sizeof �� �־���� �Ѵ�.
	unsigned char Header_Size_u8;

	
	//üũ�� �� ���� ����Ÿ..
	char CheckSum_s8;	

	//��Ŷ�� �������� ����Ÿ ������ �� ����� ũ��� ���� �ȵ�.
	unsigned short Data_Size_u16;

	//��û�� ����� ������ ���� ����ü
	// 0���� ���ǵ� ����� ������ ���� �ڵ�� �����Ѵ�..
	unsigned short Command_u16;

	
	//2����Ʈ ������ ���ؼ� ����Ѵ�....
	//���� �ƹ��� �뵵�� ����(..)
	//6����Ʈ�� ä���ļ� �Ⱦ���..
	//char None;

	//�� �ʿ��Ѱ� ������ �� �Ʒ��� �߰��Ѵ�
};

#define CK_MAX_BUFFER_SIZE					512
// ��Ŷ ��� ����..
#define CK_PACK_HEADER_SIZE				(sizeof(PacketHeader) )
#define	LOGIN_BACKLOG					50
#define	DEFAULT_TIMEOUT					30

//Bool �� ��� ������..
typedef  char     ckBOOL;

//����������������������������������������������������������������������������
//	TCP Ŭ���� ����..
//����������������������������������������������������������������������������

//� Ÿ���� ��������..(LISTEN: ������ �޾Ƶ��̴� ����,CONNECT  �ٸ������� �����ϴ� ����)
//ACCEPT : ������ ���ͼ� accept �� ���ϵ� ����
enum SocketType { Type_Listen = 0, Type_Connect, Type_Accept };


#include <winsock.h>

class Socket_C 
{
//���� ���𦬦�������������������������������������������������������������������������
private:

protected:
	
	timeval		ck_timeout;
	fd_set		ck_fdPlayer;
	fd_set		ck_fdError;	
	
	int ReadBufferLen;//������ ���� ����..
	int WriteBufferLen;
	//�ӽ� ������ ���� ����..
	int Tmp_ReadBufferLen;
	int Tmp_WriteBufferLen;

	//���Ͽ��� ���� ����
	unsigned char WriteBuf[CK_MAX_BUFFER_SIZE];
	//���Ͽ��� �д� ����
	unsigned char ReadBuf[CK_MAX_BUFFER_SIZE];
	
	//Pack,UnPack �Լ����� ����ϴ� �ӽ� ����.
	unsigned char Tmp_WriteBuf[CK_MAX_BUFFER_SIZE];
	unsigned char Tmp_ReadBuf[CK_MAX_BUFFER_SIZE];
	PacketHeader Tmp_ReadHeader;

	SOCKET MySocket;

	//�����ִ� �������� �ƴ���....
	ckBOOL IsActive;
	//�ٸ� ���ϰ� ����� �������� �ƴ���..
	ckBOOL IsConnect; 

	//���� ���� ����(UDP,TCP �� Listen ������ Connect ������..)
	SocketType SocketState;
	
	//���� �ֱ��� ������ ������ ��..
	int ErrorCode;
public:
	sockaddr_in MySocketAddr;


//�޼ҵ� ���𦬦�������������������������������������������������������������������������
private:
protected:
	//�ӽ� ���� ���ۿ��ִ� ����Ÿ�� ��Ŷȭ �ؼ� ���� ���ۿ�����..
	//������� Ŀ�ǵ尡 �� ���;��Ѵ�..
	virtual ckBOOL PackData(PacketHeader *pack_header);
	//�б� ���ۿ���  ��Ŷ�� �߶󳻼� �ӽ� �Ф� ���ۿ� �ִ´�.	
	//����Ÿ�� ���ۿ�, ���㤧�� Tmp_ReadHeader �� �ִ´�.
	virtual ckBOOL UnPackData();


public:
	
	Socket_C();
	~Socket_C();

	//���� �ʱ�ȭ..�� ����.. �ϴ��� �ڽ� Ŭ�������� ���μ����ϰ�..
	//virtual ckBOOL	Init(SocketType sState;,char * cIP, short cPort) const;	
	//������ ���´�.
	virtual ckBOOL	EndSocket () ;
		

	//���Ͽ��� ����Ÿ�� �ް� ������.
	
	virtual ckBOOL SendData() = 0;	
	virtual ckBOOL RecvData() = 0;

	//���۸� �����Ѵ�. �ϴ��� SendData�� ����..
	//virtual void	FlushOutBuffer();

	//UDP�� ����Ÿ�� ��Ŷȭ��Ų�� �ٷιٷ� FlushOut�Ѵ�.
	//TCP�� Ư���� ��쳪 ������ �ѹ� ���ƿö����� ������	


	////����Ÿ�� ��Ŷ������ �°� �����ؼ� ���� ���ۿ� �ִ´�. Cmd: Ŀ�ǵ� �ڵ�..
	//virtual ckBOOL	WriteToBuffer(char *Data,short datalen,char Cmd);
	
	
	
	
	//�־��� ���̸�ŭ �����Ѵ�.
	void			EraseBuf_R(int Length);   //�б� ����
	void			ErasenBuf_W(int Length);  //���� ����
	void			EraseBuf_TR(int Length);  //�ӽ� �б� ���� 
	void			EraseBuf_TW(int Length);  //�ӽ� ���� ����

	//���� �ֱ��� ������ ������� ErrorCode�� �ʱ�ȭ ��Ų��. 	
	void PrintfError();	

	//����� ��� �ӽ� �б� ���ۿ� ����� ���� ����Ѵ�.
	void PrintfTmpReadBuffer();

	//���� ������ ����..
	inline SOCKET GetMySocket() { return MySocket;}

	//���� �б� ���۸� �����ؼ� �������ش�..
	inline void GetReadBuffer(char *data,int *len){ memcpy(data,ReadBuf,ReadBufferLen); *len = ReadBufferLen;}

	//���� �б� ���۸� �޾Ƽ� �����Ѵ�..
	inline void SetReadBuffer(char *data,int len){ memcpy(ReadBuf,data,len); ReadBufferLen = len;}
	
	//���� �ʱ�ȭ	
	inline void ClearReadBuffer() { memset(ReadBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearWriteBUffer() { memset(WriteBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearReadBuffer_Tmp() { memset(Tmp_ReadBuf,0,CK_MAX_BUFFER_SIZE); }
	inline void ClearWriteBUffer_Tmp() { memset(Tmp_WriteBuf,0,CK_MAX_BUFFER_SIZE);}
	
	
	
	inline void			ResetSets(SOCKET psock)				{ FD_ZERO(& ck_fdError); FD_ZERO(& ck_fdPlayer); FD_SET(psock,& ck_fdError); FD_SET(psock,& ck_fdPlayer); return; };
	inline ckBOOL		Readable(SOCKET psock)				{ ResetSets(psock); select(0,& ck_fdPlayer,NULL,& ck_fdError,&ck_timeout); if(FD_ISSET(psock,& ck_fdError)) { EndSocket(); return false; } else if(FD_ISSET(psock,& ck_fdPlayer)) return true; return false;};
	inline ckBOOL		Writeable(SOCKET psock)				{ ResetSets(psock); select(0,NULL,& ck_fdPlayer,& ck_fdError,&ck_timeout); if(FD_ISSET(psock,& ck_fdError)) { EndSocket(); return false; } else if(FD_ISSET(psock,& ck_fdPlayer)) return true; return false;};	

	//���� ���ۿ��� ����Ÿ�� �̵���ų�� ����Ѵ�. 
	inline void		MemCpyInSameBuf(char * dest,char * src, int len)
	{
		//�ϴ��� �̷��� .. ������ ũ�Ⱑ �۾Ƽ� �ӵ��� ������ Ŭ�� ������ �ʴ�.
		for(int i=0;i< len; i++)	dest[i] = src[i];		
	}
};





//---------------------------------------------------------------------------------------------------------------------
//TCP ���� ����..
//Listen������ ������ ���������� SetProcSocket �� �Ἥ ����Ÿ�� ������ ���� ������ �Ź� �������־���Ѵ�.
//Connect �϶��� procsocket = MySocket ���� �ʱ�ȭ �Լ����� �ڵ����� �ʱ�ȭ���ص�..
//����Ÿ�� �������� RecvData�� ���� ȣ������ ReadFromBuffer �� ����Ÿ�� ��������
//����Ÿ�� �������� SetProcSocket�� ���� ���� ������ �����ϰ�, WriteToBuffer �� ����Ÿ�� ���Ŀ� SendData �� ������..

class TCP_C : public Socket_C 
{
private:
	
public:
	SOCKET procsocket;

	TCP_C() : Socket_C() { }; //������ �ϴ��� ����..
	~TCP_C() { closesocket(procsocket); EndSocket();}

	//listen���϶��� IP�� �ʾ��� Port(�������� ��Ʈ) �� �������ָ�ȴ�..
	//connect ���϶��� IP�� Port(�����Ұ��� ��Ʈ) �� ��� ������ �־�� �Ѵ�. Port 
	ckBOOL Init(SocketType sState,char * cIP, short cPort);

	//�̹� ���ӵ� ������ �޾Ƽ� Ŭ������ �ʱ�ȭ�Ѵ�. 
	ckBOOL Init(SOCKET s);
		
	//������ ����ϰ� ������ �����ش� ������ ���ӵǾ��ٸ� true, �׷��� ������쿣 false;
	ckBOOL Accept(SOCKET *s);

	//���ۿ��� �ϳ��� ����� �׿����� ����Ÿ�� �ӽù��ۿ� ������..
	//�ӽ� �б� ���ۿ��� �����´�.(UnPackData())
	ckBOOL	ReadFromBuffer(PacketHeader *ph,char *Data);

	ckBOOL	WriteToBuffer(char *Data,short datalen,short Cmd);
	//����Ÿ ������� ����� ������ �����Ѵ�.
	void SetProcSocket(SOCKET s) { procsocket = s;}

	//���ۿ� ����Ÿ�� �ް� ������.	
	ckBOOL	 SendData();	
	ckBOOL	 RecvData();
};






//---------------------------------------------------------------------------------------------------------------------
//UDP ���� ����..
//WriteToBuffer �� ȣ���ϸ� �ڵ����� Send�� �ϰ�
//ReadFromBUffer �� ȣ���ϸ� �ڵ����� Recv�Ѵ�.
class UDP_C : public Socket_C
{

private:
	
	//����Ÿ�� ���� �ּұ���ü
	sockaddr_in SendAddr;

	//����Ÿ�� ���� �ּ��� ����ü..
	//RecvData ���� �̰��� �����ϰ� ReadFromBuffer ���� �� ����ü�� �����ؼ� �������� ������....
	sockaddr_in RecvAddr;
	
	//���ۿ� ����Ÿ�� �ް� ������.	
	//UDP ������ ���ۿ� ���̴� �� ���� �ٷ� �����⶧���� WriteToBuffer,ReadFromBuffer �� ȣ�����ڸ���
	//�ٷ� ����������..
	
	ckBOOL	 SendData();	
	ckBOOL	 RecvData();

public:
	UDP_C()	: Socket_C() {  };
	~UDP_C() {EndSocket();}

	ckBOOL	Init(SocketType sState,short port);

	
	//���� ���ۿ� ����Ÿ ��Ŷ���� ���� �������Ŀ� ������.(UDP�� �ٷιٷ� ������..)
	//sa �� ����Ÿ�� ���� �ּ��� ����ü
	ckBOOL	WriteToBuffer(sockaddr_in *sa,char *Data,short datalen,short Cmd);

	//RecvData�� ���Ͽ��� ����Ÿ�� �����ϰ� 
	//���ۿ��� �ϳ��� ����� �׿����� ����Ÿ�� �ӽù��ۿ� ������..
	//�ӽ� �б� ���ۿ��� �����´�.(UnPackData())
	//sa �� ���� �ּ��� ����ü 
	ckBOOL	ReadFromBuffer(sockaddr_in *sa,char *Data,PacketHeader *ph);

};



#endif