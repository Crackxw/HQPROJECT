#ifndef __CGAMEINFO_PARSER__
#define __CGAMEINFO_PARSER__


#include "..\SCCommon\JOGersangConfig.h"

//������������������������������������������������������������������������������������������������������������������������
//
// class : ���� ������ ���Ͽ��� �о���� Ŭ���� 
//
//������������������������������������������������������������������������������������������������������������������������
class CGameInfoParser: 
public CJOGersangConfig, 
public CSingleton<CGameInfoParser>
{

private:	
	char*						m_notice;						// ���� ����     
	WORD						m_gameVersion;					// ������ ���� ���� 
	
	WORD						m_userPort;
	WORD						m_gameServerPort;
	WORD						m_numUser;
	WORD						m_numGameServer;

public:
	CGameInfoParser( char* notice_filename, 
					 char* gameinfo_filename );

	~CGameInfoParser();
	



		
public:	
	bool						LoadGameInfo(char* filename);
	bool						LoadGameInfo(); // Config file load.
	bool						LoadNotice(); // Config file load.
	bool						LoadNotice(char* filename);

	const char*					getNotice() const;
	WORD						getGameVersion();	
	
	WORD						getUserPort()		{ return m_userPort; }
	WORD						getGameServerPort() { return m_gameServerPort; }
	WORD						getNumUser()		{ return m_numUser; }
	WORD						getNumGameServer()	{ return m_numGameServer; }

};



#endif