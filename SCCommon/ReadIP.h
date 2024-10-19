//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	임진록 온라인 [거상] - 국가별 정보 데이터베이스 파서기
//	File Name		:	ReadIP.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	거 상 팀
//	Editer			:	조 현 준
//	Copyright		:	Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		HQ 내의 국가별 정보 데이터베이스 파일(ServiceInfo.cfg)에서 
//		ntn.con에 설정된 플래그대로 골라낼 때 쓰는 함수입니다.
//
//===================================================================================================

#ifndef	__READIP_H__
#define	__READIP_H__

typedef struct
{
	int			m_WorldID;
	WORD		m_ChatPort;
	WORD		m_GatePort;
	char		m_ServerName[128];					// 서버명

	int			m_iServiceNation;					// 서비스 국가 코드
	int			m_iServiceType;						// 서비스 타입
	int			m_iServiceLanguage;					// 서비스 언어
	int			m_iServiceCP;						// 서비스 코드페이지

	char		m_FrontServerIP[128];				// 프론트 서버 IP
	char		m_ServiceName[128];					// 서비스 업체 이름
	char		m_ServiceURL[512];					// 서비스 업체의 URL
	char		m_ServiceCreateAccountURL[512];		// 서비스 업체의 ID 생성 URL
	char		m_ServiceShopName[128];				// 서비스 업체의 쇼핑몰 이름
	char		m_ServiceShopURL[512];				// 서비스 업체의 쇼핑몰 URL
} SConnectServerInfo;

extern	SConnectServerInfo	g_stConnectServerInfo;

#endif//__READIP_H__
