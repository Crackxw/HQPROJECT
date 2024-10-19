#ifndef	___ONLINE_IMOTICON___
#define ___ONLINE_IMOTICON___


// Imoticon Data
class	OnlineImoticon
{
protected:
	cltOnlineWorld*					m_lpWorld;

	BOOL							m_fRenderList;										// Imoticon List�� ����Ұ����� Flag
	UI16							m_UseImoticonList[9];								// ��밡���� Imoticon ó��

	SI16							siNoFieldHandle;									// ���콺 ���ù�� �κ� Handle

	XSPR*							m_lpPannel;

public:
	OnlineImoticon();																	// ������.
	virtual	~OnlineImoticon();															// �Ҹ���.
		
	virtual	BOOL					Init(cltOnlineWorld* lpWorld);						// �ʱ�ȭ
	virtual	VOID					Free(void);											// ����.

	virtual BOOL					SetImoticonKey(SI16 ImoticonKey);					// �̸�Ƽ�� ���

	virtual BOOL					SetActive(BOOL fActive);							// �̸�Ƽ�� Active
	virtual BOOL					IsActive(void);										// Active ����

	virtual BOOL					Pool(BOOL fLMouseUp);								// ó��
	virtual BOOL					Draw(LPDIRECTDRAWSURFACE7 lpSurface);				// ���
};	


#endif
