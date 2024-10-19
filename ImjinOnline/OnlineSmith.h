/********************************************************************************************************
	파일명	:	OnlineSmith.h
	작성자	:	이 준 석
	담당자	:	이 준 석
	작성일	:	2003.1.2
********************************************************************************************************/

#ifndef	_ONLINE_ONLINESMITH_HEADER
#define _ONLINE_ONLINESMITH_HEADER


class OnlineListControl;


// 경매 접수 인터페이스
class OnlineSmith
{
private:
	cltOnlineWorld						*m_pMyOnlineWorld;

	SI32                                m_siX,m_siY;                // 좌표
	SI32                                m_siStatus;

	XSPR                                m_BoardSpr;                 // 뒷배경
	XSPR                                m_SlotSpr;                  // 슬롯 그림
	XSPR                                *m_pButtonSpr; 
	
	_Button                             m_BOk,m_BCancel;            // 확인 취소 버튼
	OnlineListControl					*m_pSmithList;				// 올려놓은 낡은 아이템

	BOOL                                m_bActive;                  // 활성화.

	BOOL                                m_bMouseSwitch;

	int                                 m_siNoField;                // 노 필드 핸들
	SI32                                m_siInvenSlot;              // 어떤 아이템을 올려놓았는가
	UI32                                m_uiFollowerID;	            // 아이템을 올려놓은 용병 아이디
	UI16								m_uiItemID;

	DWORD								m_WaitTickCount;

public:

	OnlineSmith();
	~OnlineSmith();

	void                                Initialize();
	void                                Init(cltOnlineWorld *pWorld);    // 초기화
	void                                Free();                          // 자원해제

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