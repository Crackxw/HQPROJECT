//===================================================================================================
//
//	*** Comments Ver 3.01 ***
//	Project Name	:	������ �¶��� [�Ż�] - ������ ���� �����ͺ��̽� �ļ���
//	File Name		:	ReadIP.cpp
//	Birth Date		:	2004. 01. 01.
//	Creator			:	�� �� ��
//	Editer			:	�� �� ��
//	Copyright		:	Joyon Entertainments. Co. 2000. 2004.
//	Comments		: 
//		HQ ���� ������ ���� �����ͺ��̽� ����(ServiceInfo.cfg)���� 
//		ntn.con�� ������ �÷��״�� ��� �� ���� �Լ��Դϴ�.
//
//===================================================================================================

#ifndef	__READIP_H__
#define	__READIP_H__

typedef struct
{
	int			m_WorldID;
	WORD		m_ChatPort;
	WORD		m_GatePort;
	char		m_ServerName[128];					// ������

	int			m_iServiceNation;					// ���� ���� �ڵ�
	int			m_iServiceType;						// ���� Ÿ��
	int			m_iServiceLanguage;					// ���� ���
	int			m_iServiceCP;						// ���� �ڵ�������

	char		m_FrontServerIP[128];				// ����Ʈ ���� IP
	char		m_ServiceName[128];					// ���� ��ü �̸�
	char		m_ServiceURL[512];					// ���� ��ü�� URL
	char		m_ServiceCreateAccountURL[512];		// ���� ��ü�� ID ���� URL
	char		m_ServiceShopName[128];				// ���� ��ü�� ���θ� �̸�
	char		m_ServiceShopURL[512];				// ���� ��ü�� ���θ� URL
} SConnectServerInfo;

extern	SConnectServerInfo	g_stConnectServerInfo;

#endif//__READIP_H__
