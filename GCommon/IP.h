#ifndef	_IP_H
#define _IP_H

//����������������������������������������������������������������������������
#define	HQNET_SERVER														// HQNet ������ �����Ѵ�.
//#define	JAYCEE_SERVER													// ���� ������ �����Ѵ�.
//����������������������������������������������������������������������������
#define	DEFAULT_SERVER						0								// HQNET		����
#define BATTLETOP_SERVER					1								// Battle Top	����
//����������������������������������������������������������������������������



#ifdef _UNION_HQNET_
	//#define		SERVER_LOGIN				"211.62.119.48" //"login.hqteam.net"// �±�
	#define		SERVER_LOGIN				"login.hqteam.net"

	#define		GAMELIST_SERVER_IP			"211.62.119.36"
	#define		PORT_LOGIN		2342
	#define		PORT_GAMELIST	2345
#endif






#ifdef	_IMJIN2																// ������2
	
	#ifdef	HQNET_SERVER
		#define	BATTLENET_DOMAIN			"hqteam.hqteam.net"
		#define	BATTLENET_CHATDOMAIN		"chat.hqteam.net"
		#define BATTLENET_ADDRESS			"211.233.75.99"					// HQNet 1����
		#define BATTLENET_CHATADDR			"211.233.75.100"				// �ӽ�ä�ü���
		//#define BATTLENET_ADDRESS			"211.240.38.71"					// HQNet 1����
		//#define BATTLENET_CHATADDR			"211.240.38.72"				// �ӽ�ä�ü���
	#endif

	#ifdef	JAYCEE_SERVER
		#define BATTLENET_ADDRESS			"211.240.38.194"				// ���� ����
	#endif
	
	#ifdef  _DEMO
		#ifdef _IMJIN2PLUS
			#define GAME_UNIQUE_NUMBER		16								// ������2Ȯ���� ����
			#define AUTOPATCH_NAME			"AutoPatch5.Exe"				// ������ġ ����
		#else
			#define GAME_UNIQUE_NUMBER		12								// ������2 ����
			#define AUTOPATCH_NAME			"AutoPatchDemo2.Exe"			// ������ġ ����
		#endif
	#else
		#ifdef _IMJIN2PLUS
			#define GAME_UNIQUE_NUMBER		15								// ������2Ȯ���� ����
			#define AUTOPATCH_NAME			"AutoPatch5.Exe"				// ������ġ ����
		#else
			#define GAME_UNIQUE_NUMBER		11								// ������2 ����
			#define AUTOPATCH_NAME			"AutoPatch2.Exe"				// ������ġ ����
		#endif
	#endif

	#define	ONLINE_ADDRESS					"211.240.38.232"

	#define PORT							6112							// ������ ���� ��Ʈ
	#define FTP_ADDRESS						"211.169.248.115"				// ������ FTP ���� �ּ�
	
#endif

#ifdef _MYTH																// õ���� ��ȭ

	#ifdef	HQNET_SERVER
		#define	BATTLENET_DOMAIN			"hqteam.hqteam.net"
		#define	BATTLENET_CHATDOMAIN		"chat.hqteam.net"
		#define BATTLENET_ADDRESS			"211.233.75.99"					// HQNet 1����
		#define BATTLENET_CHATADDR			"211.233.75.100"				// �ӽ�ä�ü���
	#endif

	#ifdef	JAYCEE_SERVER
		#define BATTLENET_ADDRESS			"211.240.38.210"				// ���� ����
	#endif

	#ifdef  _DEMO
		#define GAME_UNIQUE_NUMBER			14								// õ���� ��ȭ ����
		#define AUTOPATCH_NAME				"AutoPatchDemo2.Exe"			// ������ġ ����
	#else
		#define GAME_UNIQUE_NUMBER			13								// õ���� ��ȭ ����
		#define AUTOPATCH_NAME				"AutoPatch2.Exe"				// ������ġ ����
	#endif

	#define PORT							6112							// õ���� ��ȭ ���� ��Ʈ
	#define FTP_ADDRESS						"211.169.248.115"				// õ���� ��ȭ FTP ���� �ּ�

#endif


#endif
