/********************************************************************************************************
	���ϸ�	:	OnlineSmith.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2003.1.2
********************************************************************************************************/

#ifndef	_ONLINE_ONLINESMITH_HEADER
#define _ONLINE_ONLINESMITH_HEADER


class OnlineListControl;


// ��� ���� �������̽�
class OnlineSmith
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                m_siX,m_siY;                // ��ǥ
	SI32                                m_siStatus;

	XSPR                                m_BoardSpr;                 // �޹��
	XSPR                                m_SlotSpr;                  // ���� �׸�
	XSPR                                *m_pButtonSpr; 
	
	_Button                             m_BOk,m_BCancel;            // Ȯ�� ��� ��ư
	OnlineListControl					*m_pSmithList;				// �÷����� ���� ������

	BOOL                                m_bActive;                  // Ȱ��ȭ.

	BOOL                                m_bMouseSwitch;

	int                                 m_siNoField;                // �� �ʵ� �ڵ�
	SI32                                m_siInvenSlot;              // � �������� �÷����Ҵ°�
	UI32                                m_uiFollowerID;	            // �������� �÷����� �뺴 ���̵�
	UI16								m_uiItemID;

	DWORD								m_WaitTickCount;

public:

	OnlineSmith();
	~OnlineSmith();

	void                                Initialize();
	void                                Init(cltOnlineWorld *pWorld);    // �ʱ�ȭ
	void                                Free();                          // �ڿ�����

	void                                SetAction(BOOL bActive);       
	void                                SetPos(SI32 siX,SI32 siY);
	
	SI32                                GetX(SI32 siX) {return m_siX;};
	SI32                                GetY(SI32 siY) {return m_siY;};

	BOOL                                IsActive() {return m_bActive;};
		
	void                                Poll();
	void								Draw(LPDIRECTDRAWSURFACE7 pSurface);

	void								RefineItem(BYTE *pMsg);
};

#endif