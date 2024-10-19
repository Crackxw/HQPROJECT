#ifndef _ONLINE_PARTY_H
#define _ONLINE_PARTY_H



#include "OnlineCommonStruct.h"


class	OnlineParty
{
public:
	OnlineParty();
	~OnlineParty();
	
	void	Init(cltOnlineWorld *pOnlineWorld);					// �ʱ�ȭ.
	void	Action();
	void	Draw(LPDIRECTDRAWSURFACE7 pSurface);
	void	SetAction(BOOL flag,UI16 uiMsg);
	void	SetPartyMode(SI16 x,SI16 y,UI16 msg);
	void	SetRePosition();
	BOOL	GetPartyAction();
	void    Free();
	VOID	SetCharID(CHAR *pName);
	void	SetPartyInfo(OnPartyInfo* pOnPartyInfo,SI16 siNum);
	BOOL    GetPartyMsg();
	void	SetPartyMsg(UI16 msg){m_uiPartyMsg = msg;};
//	BOOL	GetPartyCharBaseInfo(){return m_bBaseCharInfo;};
	BOOL	GetPartyCharInfo()    {return m_bCharInfo;};
//	void    SetPartyCharInfo(BOOL bInfo);
	void	Initialize();
	SI16	GetPartyNum()	{return	m_siPartyNum;};
	SI16	GetMyCharNum()	{return m_siMyChar;};


	OnlineMsgBox		*m_pMsgBox;
	SI16				m_siNoField;
	char				m_szPartyLeader[ON_ID_LENGTH + 1];

	OnPartyInfo*		m_stPartyInfo;									// ��Ƽ ������ ����ü.

 
private:
	cltOnlineWorld		*m_pMyOnlineWorld;
	UI16				m_uiPartyMsg;
	BOOL				m_bFlag;
	BOOL				m_bMouseDown;									// ���콺 ��ư üũ
	SI16				m_siX, m_siY; 						//	���� ��ǥ,NoFiled
	POINT				pt;												// ���콺 ����Ʈ
	SI16				m_siMyChar;

	SI16				m_siCharX;
	SI16				m_siCharY;
	UI16				m_uiCharmsg;  
	SI16				m_siPartyNum;
	char				m_szCharName[ON_ID_LENGTH + 1];						//ĳ����.
	

	SI16				m_siPartyBaseX;
	SI16				m_siPartyBaseY;



	XSPR				m_BtnSpr;										// �ŷ�.��Ƽ ��ư
	XSPR				m_BtnBaseSpr;
	XSPR				m_Party_w_Base;

	XSPR				m_BookPartySpr;
	_Button				m_BPartybttn;									// ��Ƽ ��ư.
	_Button				m_BTradebttn;									// �ŷ� ��ư.
	BOOL				m_bCharInfo;
	BOOL				m_bBaseCharInfo;
	BOOL				m_bTFlag;

	_Button				m_BPartyOutBasebttn;							// ��Ƽ ������.
	_Button				m_BPartyBasebttn;								// ��Ƽ â �ݱ�.

	


	


};

#endif