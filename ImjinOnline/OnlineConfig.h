/********************************************************************************************
	���ϸ�	:	OnlineConfig.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.05.23
	������	:	2002.04.04
********************************************************************************************/


class OnlineConfig
{
protected:
	char		szID_Name[ON_ACCOUNT_LENGTH+1];										// ���̵�
	UI08		uiMiniMapSize;														// �̴ϸ� ������
	UI08		uiServerType;														// ��ȣ, ���ۼ��� ����(1, 2)
	BOOL		bOpen;

public:
	OnlineConfig();
	virtual ~OnlineConfig();	

	
	BOOL		OpenConfig();														// Config ���� ����
	void		SetConfig( char *pID );												// COnfig ���� ����
	void		DeCode( UI08 *pData, UINT nSize );									// ��ȣ & �ص�
	void		SaveConfig();														// COnfig ���� ����

	void		SetMiniMapSize( UI08 uiSize )		{ if(bOpen)	uiMiniMapSize = uiSize; };	// �̴ϸ� ����� ����
	void		SetServerType( UI08 uiType )		{ if(bOpen)	uiServerType  = uiType; };	// ����Ÿ���� ����


	char		*GetID()							{ return szID_Name; }			// ���̵� ������
	UI08		GetMiniMapSIze()					{ return uiMiniMapSize;	};		// �̴ϸ� ����� ����.
	UI08		GetServerType()						{ return uiServerType;	};		// ����Ÿ���� ����.
};