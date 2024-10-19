#ifndef		__CLIENTLOGIN_H__
#define		__CLIENTLOGIN_H__

#include	<time.h>

#define		MAX_BUFFER_SIZE			256
#define		PACK_HEADER_SIZE		3

// 유저에 요구에 대한 서버의 대답
#define	SUCCESS_CONNECT							1
#define ERROR_CONNECT_WRONG_CDKEY				2
#define ERROR_CONNECT_WRONG_RELEASE				3
#define ERROR_CONNECT_NOT_SUPPORT_GAME			4
#define ERROR_CONNECT_WRONG_VERSION				5
#define ERROR_CONNECT_INUSE_CDKEY				6

#define	SUCCESS_LOGIN							7
#define ERROR_LOGIN_UNEXPECT					8
#define ERROR_LOGIN_ALREADY_CONNECT				9
#define ERROR_LOGIN_WRONG_ID_FORMAT				10
#define	ERROR_LOGIN_WRONG_PW_FORMAT				11

#define	SUCCESS_NEWACCOUNT						12
#define	ERROR_NEWACCOUNT_UNEXPECT				13
#define	ERROR_NEWACCOUNT_ALREADY_EXIST			14
#define	ERROR_NEWACCOUNT_WRONG_ID_FORMAT		15
#define	ERROR_NEWACCOUNT_WRONG_PW_FORMAT		16

#define	SUCCESS_CHANGEPASSWORD					17
#define	ERROR_CHANGEPASSWORD_UNEXPECT			18
#define	ERROR_CHANGEPASSWORD_WRONG_ID_FORMAT	19
#define	ERROR_CHANGEPASSWORD_WRONG_PW_FORMAT	20

#define ERROR_UNEXPECT_PACKET					-1


//받는 패킷 정의
static const char *gamename[]	=	{"SYW2","Syw2Plus","Legend","Gears","Tazaro","Kohan"};
static const char *usertype[]	=	{"release","demo"};
static const char *kindquery[]	=	{"gamename","gamever","usertype","hqcdkey"};
static const char *loginquery[]	=	{"loginname","loginpass"};
static const char *newaccquery[]=	{"newid","newpass"};
static const char *changepwquery[]=	{"changeid","nowpassword","newpassword"};

//보내는 패킷 정의
static const char *response[]	=
	{"success","failed","present","input_err1","input_err2","cdkey_err","user_err","game_err","version_err","cdkeyinuse"};
static const char *result[] = {"inforesult","loginresult","entryresult","changpwresult"};


class Client_Login
{
public:
	int SockOpterr;
	unsigned long _Sec;

	protected:
		BOOL			m_bConnect;
		BOOL			m_bCDKeyCheck;

		char			m_cChatServerAddress[20];
		char			m_cNotice[2048];
		char			m_cGUID[36];

		char			m_cID[12];
		char			m_cPass[12];
		char			m_cCDKey[20];
		char			m_cGameVer[20];
		char			m_cGameName[20];
		char			m_cUserType[20];
		char			m_cOutBuffer[MAX_BUFFER_SIZE];
		char			m_cInBuffer[MAX_BUFFER_SIZE];
		int				m_iOutBufLen;
		int				m_iInBufLen;

		//Packing 관련
		char			m_cMsg[MAX_BUFFER_SIZE];

		//Socket
		fd_set			m_fdPlayer;
		fd_set			m_fdError;
		SOCKET			m_Socket;
		sockaddr_in		m_SockAddr;
		timeval			m_timeout;

	public:
		Client_Login	(char *cGameName,char *cGameVer, char *cUserType, char *cCDKey);
		~Client_Login	();

		BOOL			Connect(char *pAddress, WORD wPort);
		void			Disconnect(void);
		BOOL			IsConnect(void);

		int				Poll(void);

		void			SendNewAccount(char *cID, char *cPass);
		void			SendAccount(char *cID, char *cPass);
		void			SendChangePassword(char *cID, char *nowPass, char *newPass);
		void			SendLogOut(void);

		void			Get_Notice(char **pNotice);
		void			Get_ChatServerAddress_Guid(char **pAddress, char **pGuid);

	protected:
		int				RecvChangePassword(char *Buf);
		void			EraseInBuffer(int iByte);
		int				RecvNewID(char *Buf);
		int				RecvLogIn(char *Buf);
		int				RecvKind(char *Buf);
		void			Read(void);
		void			FlushOutBuffer(void);
		void			Write(char *Msg, int iByte);
		void			SendGameInfo(void);
		void			PackMessage(char *cSrcMsg, short siSrcSize, char *cPackMsg, short *siPackSize);
		BOOL			UnpackMessage(void);


	protected:
		void ResetSets(void);
		BOOL Readable(void);
		BOOL Writable(void);
};

#endif
