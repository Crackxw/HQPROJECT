/*
	최종 Client_Chat 수정일 2001. 9. 17
*/
#ifndef		__CLIENTCHAT_H__
#define		__CLIENTCHAT_H__

#include <windows.h>
#include <stdio.h>

#define CLIENT_CHAT_MESSEGE_OKLOGIN			1
#define CLIENT_CHAT_MESSEGE_NOLOGIN			2
#define CLIENT_CHAT_MESSEGE_OKJOIN			3
#define CLIENT_CHAT_MESSEGE_NOJOIN			4
#define CLIENT_CHAT_MESSEGE_SEND			5
#define CLIENT_CHAT_MESSEGE_WHISPER			6
#define CLIENT_CHAT_MESSEGE_CHANNEL			7
#define CLIENT_CHAT_MESSEGE_PLAYER			8
#define CLIENT_CHAT_MESSEGE_RMPLAYER		9
#define CLIENT_CHAT_MESSEGE_OKSGAME			10
#define CLIENT_CHAT_MESSEGE_NOSGAME			11
#define CLIENT_CHAT_MESSEGE_OKEGAME			12
#define CLIENT_CHAT_MESSEGE_NOEGAME			13
#define CLIENT_CHAT_MESSEGE_PING			14
#define CLIENT_CHAT_MESSEGE_OKRANK			15
#define CLIENT_CHAT_MESSEGE_OKSQUELCH		16
#define CLIENT_CHAT_MESSEGE_OKUNSQUELCH		17
#define CLIENT_CHAT_MESSEGE_OKADDMEMBER		18
#define CLIENT_CHAT_MESSEGE_NOADDMEMBER		19
#define CLIENT_CHAT_MESSEGE_JOINGUILD		20
#define CLIENT_CHAT_MESSEGE_OKJOINGUILD		21
#define CLIENT_CHAT_MESSEGE_NOJOINGUILD		22
#define CLIENT_CHAT_MESSEGE_OKDELMEMBER		23
#define CLIENT_CHAT_MESSEGE_NODELMEMBER		24
#define CLIENT_CHAT_MESSEGE_WHERE			25

#define CLIENT_CHAT_MAX_BUFFER_SIZE			512
#define DEFAULT_CHANNEL_LENGTH				20
#define CLIENT_CHAT_MAX_OUT_BUFFER			2560
#define CLIENT_CHAT_MAX_IN_BUFFER			2560
#define CLIENT_CHAT_MAX_COMMAND_NUM			30
#define DEFAULT_ID_LENGTH					21

static char *cCommandList[] = { 
			"",
			"OK_LOGIN",
			"NO_LOGIN",
			"OK_JOIN",
			"NO_JOIN",
			"SEND",
			"WHISPER",
			"CHANNEL",
			"PLAYER",
			"RMPLAYER",
			"OK_SGAME",
			"NO_SGAME",
			"OK_EGAME",
			"NO_EGAME",
			"PING",
			"OK_RANK",
			"OK_SQUELCH",
			"OK_UNSQUELCH",
			"OK_ADDMEMBER",
			"NO_ADDMEMBER",
			"JOINGUILD",
			"OK_JOINGUILD",
			"NO_JOINGUILD",
			"OK_DELMEMBER",
			"NO_DELMEMBER",
			"WHERE"
};

class Client_Chat
{
	public:
		char m_pID[DEFAULT_ID_LENGTH];

	protected:
		// 소켓 관련
		SOCKET m_Socket;
		sockaddr_in m_SockAddr;

		timeval	m_timeout;
		fd_set  m_fdError;
		fd_set  m_fdPlayer;

		// Connection여부
		BOOL m_bConnect;

		// recv버퍼
		char m_cInBuffer[CLIENT_CHAT_MAX_IN_BUFFER];
		// send버퍼
		char m_cOutBuffer[CLIENT_CHAT_MAX_OUT_BUFFER];
		// recv버퍼 크기
		int  m_iInBufferLen;
		// send버퍼 크기
		int  m_iOutBufferLen;

		// 클래스 외부로 보내기 위한 문자열
		char m_pOutputBuffer[CLIENT_CHAT_MAX_BUFFER_SIZE];

		enum command
		{ 
			OK_LOGIN=1,
			NO_LOGIN,
			OK_JOIN,
			NO_JOIN,
			SEND,
			WHISPER,
			CHANNEL,
			PLAYER,
			RMPLAYER,
			OK_SGAME,
			NO_SGAME,
			OK_EGAME,
			NO_EGAME,
			PING,
			OK_RANK,
			OK_SQUELCH,
			OK_UNSQUELCH,
			JOINGUILD,
			OK_JOINGUILD,
			NO_JOINGUILD,
			OK_ADDMEMBER,
			NO_ADDMEMBER,
			OK_DELMEMBER,
			NO_DELMEMBER,
			WHERE,
		};

	public:
		Client_Chat();
		~Client_Chat();

		BOOL Connect(char *pAddress, WORD wPort);
		void Disconnect(void);
		BOOL IsConnect(void);

		int	 Poll(void);

		void Get_ChannelList(int *chatuser, int *alluser, char **chname);
		void Get_Channel(char **chname);
		void Get_JoinPlayer(char **name, char **info);
		void Get_OutPlayer(char **name);
		void Get_Name(char **name);
		void Get_ChatMessage(char **user, char **msg);
		void Get_Rank(char **rank, char **name, char **vic, char **def, char **dis, char **point);
		void Get_Guild(char **master, char **name);
		void Get_Where(char **name, char **gamestate, char **gametype, char **channel);

		void Msg_Login(char *guid, char *game);
		void Msg_Send(char *msg);
		void Msg_Whisper(char *id, char *msg);
		void Msg_Join(char *ch);
		void Msg_Channel(void);
		void Msg_StartGame(void);
		void Msg_EndGame(char cResult, char cPoint);
		void Msg_EndGame(void);
		void Msg_CancelGame(void);
		void Msg_Squelch(char *id);
		void Msg_UnSquelch(char *id);
		void Msg_JoinGuild(char *cGuildMaster);
		void Msg_AddMember(char *cID);
		void Msg_DelMember(void);
		void Msg_Rank(void);
		void Msg_Rank(int iRank);
		void Msg_Where(char *pID);
		void Msg(char *msg);

	protected:
		void EraseInBuffer(int iByte);
		void FlushOutBuffer(void);
		void ResetSets(void);
		BOOL Readable(void);
		BOOL Writable(void);
		int  Read(int siBytesToRead);
		BOOL Write(const void * cMsg, int iBytes);

	protected:
		int  Cmd(char *buf);
		void Cmd_Default(char *str);
		void Cmd_Player(char *str);
		void Cmd_RmPlayer(char *str); // Update
		void Cmd_OkLogin(char *str);

		char *GetParameter(char *data, int count, ...);
		void ReadLine(char *buf, int &len, char *str, int &strsize);
};

#endif