#ifndef	_IP_H
#define _IP_H

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	HQNET_SERVER														// HQNet 서버로 접속한다.
//#define	JAYCEE_SERVER													// 제형 서버로 접속한다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define	DEFAULT_SERVER						0								// HQNET		서버
#define BATTLETOP_SERVER					1								// Battle Top	서버
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━



#ifdef _UNION_HQNET_
	//#define		SERVER_LOGIN				"211.62.119.48" //"login.hqteam.net"// 승교
	#define		SERVER_LOGIN				"login.hqteam.net"

	#define		GAMELIST_SERVER_IP			"211.62.119.36"
	#define		PORT_LOGIN		2342
	#define		PORT_GAMELIST	2345
#endif






#ifdef	_IMJIN2																// 임진록2
	
	#ifdef	HQNET_SERVER
		#define	BATTLENET_DOMAIN			"hqteam.hqteam.net"
		#define	BATTLENET_CHATDOMAIN		"chat.hqteam.net"
		#define BATTLENET_ADDRESS			"211.233.75.99"					// HQNet 1서버
		#define BATTLENET_CHATADDR			"211.233.75.100"				// 임시채팅서버
		//#define BATTLENET_ADDRESS			"211.240.38.71"					// HQNet 1서버
		//#define BATTLENET_CHATADDR			"211.240.38.72"				// 임시채팅서버
	#endif

	#ifdef	JAYCEE_SERVER
		#define BATTLENET_ADDRESS			"211.240.38.194"				// 제형 서버
	#endif
	
	#ifdef  _DEMO
		#ifdef _IMJIN2PLUS
			#define GAME_UNIQUE_NUMBER		16								// 임진록2확장팩 데모
			#define AUTOPATCH_NAME			"AutoPatch5.Exe"				// 오토패치 데모
		#else
			#define GAME_UNIQUE_NUMBER		12								// 임진록2 데모
			#define AUTOPATCH_NAME			"AutoPatchDemo2.Exe"			// 오토패치 데모
		#endif
	#else
		#ifdef _IMJIN2PLUS
			#define GAME_UNIQUE_NUMBER		15								// 임진록2확장팩 정식
			#define AUTOPATCH_NAME			"AutoPatch5.Exe"				// 오토패치 정식
		#else
			#define GAME_UNIQUE_NUMBER		11								// 임진록2 정식
			#define AUTOPATCH_NAME			"AutoPatch2.Exe"				// 오토패치 정식
		#endif
	#endif

	#define	ONLINE_ADDRESS					"211.240.38.232"

	#define PORT							6112							// 임진록 접속 포트
	#define FTP_ADDRESS						"211.169.248.115"				// 임진록 FTP 서버 주소
	
#endif

#ifdef _MYTH																// 천년의 신화

	#ifdef	HQNET_SERVER
		#define	BATTLENET_DOMAIN			"hqteam.hqteam.net"
		#define	BATTLENET_CHATDOMAIN		"chat.hqteam.net"
		#define BATTLENET_ADDRESS			"211.233.75.99"					// HQNet 1서버
		#define BATTLENET_CHATADDR			"211.233.75.100"				// 임시채팅서버
	#endif

	#ifdef	JAYCEE_SERVER
		#define BATTLENET_ADDRESS			"211.240.38.210"				// 제형 서버
	#endif

	#ifdef  _DEMO
		#define GAME_UNIQUE_NUMBER			14								// 천년의 신화 데모
		#define AUTOPATCH_NAME				"AutoPatchDemo2.Exe"			// 오토패치 데모
	#else
		#define GAME_UNIQUE_NUMBER			13								// 천년의 신화 정식
		#define AUTOPATCH_NAME				"AutoPatch2.Exe"				// 오토패치 정식
	#endif

	#define PORT							6112							// 천년의 신화 접속 포트
	#define FTP_ADDRESS						"211.169.248.115"				// 천년의 신화 FTP 서버 주소

#endif


#endif
