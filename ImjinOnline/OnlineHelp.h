#ifndef	_ON_HELP_H
#define	_ON_HELP_H

/********************************************************************************************
	파일명	:	OnlineHelp.h
	작성자	:	정 진 욱
	담당자  :	정 진 욱
	작성일	:	2001.08.09
	수정일	:	2002.03.16
********************************************************************************************/


// 풍선 도움말 표시 위치 옵션 플래그
// 가로(X)축 관련
#define ON_HELP_LEFT		0x00000001	// 마우스 커서 좌측
#define ON_HELP_RIGHT		0x00000002	// 마우스 커서 우측
#define ON_HELP_CENTER		0x00000004	// 마우스 커서 위치(X축)를 중심으로
// 세로(Y)축 관련
#define ON_HELP_UP			0x00000010	// 마우스 커서 위쪽
#define ON_HELP_DOWN		0x00000020	// 마우스 커서 아래쪽
#define ON_HELP_VCENTER		0x00000040	// 마우스 커서 위치(Y축)를 중심으로

#define	ON_HELP_RED			0x00001000
#define	ON_HELP_BLUE		0x00000100
#define	ON_HELP_WHITE		0x00000010
#define	ON_HELP_GREEN		0x00010000
#define	ON_HELP_WHITERED	0x00100000


struct _SSiegeUnitTable; 
typedef struct _SSiegeUnitTable		*LPSSiegeUnitTable;

struct CItemHeader;
struct ItemInfo;
class OnlineHelp
{
private:
	cltOnlineWorld		*pMyOnlineWorld;

	UI08				*pTextBuffer;
	UINT				uiID;
	SI16				siLine;
	SI16				siX, siY;
	SI16				siDirection;

	HDC					MegaDC;
	HBITMAP				hBmp;

	HBITMAP				hOldBmp;
	LPBITMAPINFO		m_pBitInfo;

	SI16				siTextWidth;
	SI16				siTextHeight;
	
	BOOL				bAction;

	SI16*				m_lpSelectCharacterFlagRenderPosX;
	SI16*				m_lpSelectCharacterFlagRenderPosY;

	static	UI08		m_ColorTable[256];

public:
	OnlineHelp();
	~OnlineHelp();

	BOOL				Init(cltOnlineWorld *pOnlineWorld);														// 도움말 클래스 초기화
	VOID				FreeHelp();																				// 도움말 클래스 날린다.
	int 				Show(LPDIRECTDRAWSURFACE7 lpSurface, SI16 *siDrawedPosX = NULL, SI16 *siDrawedPosY = NULL);// 도움말을 출력을 한다.
	VOID				Initialize();																			// 버퍼를 지워준다.

	BOOL				IsActive();																				// 현재 활성화 되어 있는가?

	// robypark 2005/1/6 11:6
	// 타이틀 텍스트, 내용 텍스트를 구분하여 다른 색으로 출력한다.
	// CHAR *pTitleString: 타이틀 텍스트
	// CHAR *pContentsString: 내용 텍스트
	// SI16 siX: 그려질 위치
	// SI16 siY: 그려질 위치
	// SI16 siDirect: 텍스트 출력 위치 (default: ON_HELP_LEFT)
	// BOOL fMultiLine: 내용 텍스트를 여러 줄로 나누어 보주는가? (default: TRUE)
	// DWORD dwColorTitle: 타이틀 텍스트 칼라 (default: RGB(252, 252, 212))
	// DWORD dwColorContents: 내용 텍스트 칼라 (default: RGB(255, 255, 255))
	VOID				SetTextHasTitle(CHAR *pTitleString, CHAR *pContentsString, SI16 siX, SI16 siY, SI16 siDirect = ON_HELP_LEFT, BOOL fMultiLine = TRUE, DWORD dwColorTitle = RGB(252, 252, 212), DWORD dwColorContents = RGB(255, 255, 255));

	// robypark 2005/1/29 16:37
	// 변신하려는 공성 유닛의 간략한 정보 출력
	// LPSSiegeUnitTable lpsSiegeUnitTable: 병과 정보 구조체
	// SI16 siX: 그려질 위치
	// SI16 siY: 그려질 위치
	// BOOL bCaptain: 대장 변신 가능한 사용자인가?
	// SI16 siDirect: 텍스트 출력 위치 (default: ON_HELP_LEFT)
	VOID				SetTextSiegeUnit(LPSSiegeUnitTable lpsSiegeUnitTable, SI16 siX, SI16 siY, BOOL bCaptain, SI16 siDirect = ON_HELP_LEFT);

	VOID				SetTextFieldItem( CItemHeader *pItem, SI16 siItemCount, SI16 siX, SI16 siY );						// 텍스트를 버퍼에 입력을 한다.
	VOID				SetText(CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, BOOL fMultiLine = FALSE);	// 텍스트를 버퍼에 입력을 한다.
	VOID				SetText(CHAR *pString, char *pdesc, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor);				// 텍스트를 버퍼에 입력을 한다.
	// kuzimo
	VOID				SetTextMark(CHAR *pString, SI16 siX, SI16 siY, SI32 siColor);
	VOID				SetTextID(CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor);			// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextID(CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, SI16 FlagXSize, SI16 FlagYSize, SI16* FlagRenderPosX, SI16* FlagRenderPosY);	// 텍스트를 버퍼에 입력을 한다.(ID 입력시 깃발위치까지 검사)
	VOID				SetText(CHAR *pString, char *pdesc, char *pdesc2, char *pdesc3, SI16 siX, SI16 siY);				// 텍스트를 버퍼에 입력을 한다.

	VOID				SetTextBuySolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit);										// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextBuyMonster(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit, char* ItemName, SI32 UseItem);		// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextDismissalSolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Lv, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT);	// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextChangeJobSolder(SI16 siX, SI16 siY, SI32 Slot, SI32* lpJobList, UI08* lpJobListFlag, SI32* lpJobListLevel);		// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextChangeJobGeneral(SI16 siX, SI16 siY, CHAR *pName, SI32 Lev, SI32 MaxLife, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT);																	// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextHospital(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, MONEY iRCost, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana);									// 텍스트를 버퍼에 입력을 한다.
	VOID				SetTextMercenary( SI16 siX, SI16 siY, SI32 siFollowerDBSlot );	// actdoll

	VOID				SetTextItem( CItemHeader *pItem, ItemInfo *pSlot, SI16 siX, SI16 siY, SI16 siDirect, BOOL fBuyPrice, BOOL fSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );
	VOID				SetTextAuctionItem( CItemHeader *pItem, SI16 siX,SI16 siY,SI16 siDirect );
	VOID				SetText(CHAR *pName, CHAR *pDesc, SI16 siX, SI16 siY );
	VOID				SetTextNoSell(CItemHeader *pItem, ItemInfo *pSlot,SI16 siX,SI16 siY,SI16 siDirect);
	VOID				SetTextBooth(SellItemInfo *pItemInfo, SI16 siX, SI16 siY);								// 좌판용
	
	VOID				SetFollower(ItemInfo *pSlot);
	VOID				SetProperty(ItemInfo *pSlot);
	VOID				SetCharInfo(SI16 siX, SI16 siY, CHAR *pCharName, CHAR *pGuildName, SI32	siGuildClass, SI32 siLevel);

	// robypark 2004/11/8 14:48
	// 커서 아래 캐릭터가 공성전 유닛일 경우 풍선 도움말 텍스트 설정
	// [IN]  CHAR *pString: 캐릭터 이름 정보
	// [IN]  SI16 siX: 출력할 위치
	// [IN]  SI16 siY: 출력할 위치
	// [IN]  SI16 siDirect: 출력할 방향 플래그
	// [IN]  SI32 siColor: 텍스트 칼라
	// [OUT] SI08 &siDrawGaugeHealthPosX: 체력 바 위치
	// [OUT] SI08 &siDrawGaugeHealthPosY: 체력 바 위치
	// [IN]  bool bDrawSupplyGoods: 공성전 유닛 남은 보급품(%)을 그려줘야하는지. 보급수레만 해당된다.
	// [IN]  SI16 FlagXSize = -1: 상단 깃발 크기
	// [IN]  SI16 FlagYSize = -1: 상단 깃발 크기
	// [OUT] SI16* FlagRenderPosX = NULL: 상단 깃발 그려지는 위치
	// [OUT] SI16* FlagRenderPosY = NULL: 상단 깃발 그려지는 위치
	VOID				SetSiegeWarfareUnitToolTipText(CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor,
															SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
															SI16 FlagXSize = -1, SI16 FlagYSize = -1,
															SI16* FlagRenderPosX = NULL, SI16* FlagRenderPosY = NULL);

	// robypark 2005/2/17 11:51
	// 커서 아래 캐릭터가 상단에 가입한 공성전 유닛일 경우 풍선 도움말 텍스트 설정
	// [IN]  CHAR *pString: 캐릭터 이름 정보
	// [IN]	 CHAR *pGuildName: 상단정보
	// [IN]  SI16 siX: 출력할 위치
	// [IN]  SI16 siY: 출력할 위치
	// [IN]  SI16 siDirect: 출력할 방향 플래그
	// [IN]  SI32 siColor: 텍스트 칼라
	// [OUT] SI08 &siDrawGaugeHealthPosX: 체력 바 위치
	// [OUT] SI08 &siDrawGaugeHealthPosY: 체력 바 위치
	// [IN]  bool bDrawSupplyGoods: 공성전 유닛 남은 보급품(%)을 그려줘야하는지. 보급수레만 해당된다.
	// [IN]  SI16 FlagXSize = -1: 상단 깃발 크기
	// [IN]  SI16 FlagYSize = -1: 상단 깃발 크기
	// [OUT] SI16* FlagRenderPosX = NULL: 상단 깃발 그려지는 위치
	// [OUT] SI16* FlagRenderPosY = NULL: 상단 깃발 그려지는 위치
	VOID				SetSiegeWarfareUnitToolTipText(CHAR *pString, CHAR *pGuildName, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor,
															SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
															SI16 FlagXSize = -1, SI16 FlagYSize = -1,
															SI16* FlagRenderPosX = NULL, SI16* FlagRenderPosY = NULL);

private:
	VOID				SetDirection(SI16 &x, SI16 &y);															// 도움말이 출력될 좌표를 정한다.

	VOID				WeaponHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );	// 무기에 대한 도움말을
	VOID				OthersHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );	// 기타(갑옷, 악세사리, 소모품) 대한 도움말을
	VOID				TradeHelp(CItemHeader *pItem, ItemInfo *pSlot);											// 장부용
	VOID                DescriptionHelp( CItemHeader *pItem, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );
};


#endif