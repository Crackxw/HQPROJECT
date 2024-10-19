#ifndef	_ON_HELP_H
#define	_ON_HELP_H

/********************************************************************************************
	���ϸ�	:	OnlineHelp.h
	�ۼ���	:	�� �� ��
	�����  :	�� �� ��
	�ۼ���	:	2001.08.09
	������	:	2002.03.16
********************************************************************************************/


// ǳ�� ���� ǥ�� ��ġ �ɼ� �÷���
// ����(X)�� ����
#define ON_HELP_LEFT		0x00000001	// ���콺 Ŀ�� ����
#define ON_HELP_RIGHT		0x00000002	// ���콺 Ŀ�� ����
#define ON_HELP_CENTER		0x00000004	// ���콺 Ŀ�� ��ġ(X��)�� �߽�����
// ����(Y)�� ����
#define ON_HELP_UP			0x00000010	// ���콺 Ŀ�� ����
#define ON_HELP_DOWN		0x00000020	// ���콺 Ŀ�� �Ʒ���
#define ON_HELP_VCENTER		0x00000040	// ���콺 Ŀ�� ��ġ(Y��)�� �߽�����

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

	BOOL				Init(cltOnlineWorld *pOnlineWorld);														// ���� Ŭ���� �ʱ�ȭ
	VOID				FreeHelp();																				// ���� Ŭ���� ������.
	int 				Show(LPDIRECTDRAWSURFACE7 lpSurface, SI16 *siDrawedPosX = NULL, SI16 *siDrawedPosY = NULL);// ������ ����� �Ѵ�.
	VOID				Initialize();																			// ���۸� �����ش�.

	BOOL				IsActive();																				// ���� Ȱ��ȭ �Ǿ� �ִ°�?

	// robypark 2005/1/6 11:6
	// Ÿ��Ʋ �ؽ�Ʈ, ���� �ؽ�Ʈ�� �����Ͽ� �ٸ� ������ ����Ѵ�.
	// CHAR *pTitleString: Ÿ��Ʋ �ؽ�Ʈ
	// CHAR *pContentsString: ���� �ؽ�Ʈ
	// SI16 siX: �׷��� ��ġ
	// SI16 siY: �׷��� ��ġ
	// SI16 siDirect: �ؽ�Ʈ ��� ��ġ (default: ON_HELP_LEFT)
	// BOOL fMultiLine: ���� �ؽ�Ʈ�� ���� �ٷ� ������ ���ִ°�? (default: TRUE)
	// DWORD dwColorTitle: Ÿ��Ʋ �ؽ�Ʈ Į�� (default: RGB(252, 252, 212))
	// DWORD dwColorContents: ���� �ؽ�Ʈ Į�� (default: RGB(255, 255, 255))
	VOID				SetTextHasTitle(CHAR *pTitleString, CHAR *pContentsString, SI16 siX, SI16 siY, SI16 siDirect = ON_HELP_LEFT, BOOL fMultiLine = TRUE, DWORD dwColorTitle = RGB(252, 252, 212), DWORD dwColorContents = RGB(255, 255, 255));

	// robypark 2005/1/29 16:37
	// �����Ϸ��� ���� ������ ������ ���� ���
	// LPSSiegeUnitTable lpsSiegeUnitTable: ���� ���� ����ü
	// SI16 siX: �׷��� ��ġ
	// SI16 siY: �׷��� ��ġ
	// BOOL bCaptain: ���� ���� ������ ������ΰ�?
	// SI16 siDirect: �ؽ�Ʈ ��� ��ġ (default: ON_HELP_LEFT)
	VOID				SetTextSiegeUnit(LPSSiegeUnitTable lpsSiegeUnitTable, SI16 siX, SI16 siY, BOOL bCaptain, SI16 siDirect = ON_HELP_LEFT);

	VOID				SetTextFieldItem( CItemHeader *pItem, SI16 siItemCount, SI16 siX, SI16 siY );						// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetText(CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, BOOL fMultiLine = FALSE);	// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetText(CHAR *pString, char *pdesc, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor);				// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	// kuzimo
	VOID				SetTextMark(CHAR *pString, SI16 siX, SI16 siY, SI32 siColor);
	VOID				SetTextID(CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor);			// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextID(CHAR *pString, CHAR *pString1, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor, SI16 FlagXSize, SI16 FlagYSize, SI16* FlagRenderPosX, SI16* FlagRenderPosY);	// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.(ID �Է½� �����ġ���� �˻�)
	VOID				SetText(CHAR *pString, char *pdesc, char *pdesc2, char *pdesc3, SI16 siX, SI16 siY);				// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.

	VOID				SetTextBuySolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit);										// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextBuyMonster(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Life, SI32 Mana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT, SI32 MinLevel, SI32 NowLevel, SI32 MinCredit, SI32 NowCredit, char* ItemName, SI32 UseItem);		// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextDismissalSolder(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, SI32 Lv, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT);	// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextChangeJobSolder(SI16 siX, SI16 siY, SI32 Slot, SI32* lpJobList, UI08* lpJobListFlag, SI32* lpJobListLevel);		// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextChangeJobGeneral(SI16 siX, SI16 siY, CHAR *pName, SI32 Lev, SI32 MaxLife, SI32 MaxMana, SI32 STR, SI32 DEX, SI32 VIT, SI32 INT);																	// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextHospital(SI16 siX, SI16 siY, CHAR *pName, MONEY iCost, MONEY iRCost, SI32 Life, SI32 MaxLife, SI32 Mana, SI32 MaxMana);									// �ؽ�Ʈ�� ���ۿ� �Է��� �Ѵ�.
	VOID				SetTextMercenary( SI16 siX, SI16 siY, SI32 siFollowerDBSlot );	// actdoll

	VOID				SetTextItem( CItemHeader *pItem, ItemInfo *pSlot, SI16 siX, SI16 siY, SI16 siDirect, BOOL fBuyPrice, BOOL fSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );
	VOID				SetTextAuctionItem( CItemHeader *pItem, SI16 siX,SI16 siY,SI16 siDirect );
	VOID				SetText(CHAR *pName, CHAR *pDesc, SI16 siX, SI16 siY );
	VOID				SetTextNoSell(CItemHeader *pItem, ItemInfo *pSlot,SI16 siX,SI16 siY,SI16 siDirect);
	VOID				SetTextBooth(SellItemInfo *pItemInfo, SI16 siX, SI16 siY);								// ���ǿ�
	
	VOID				SetFollower(ItemInfo *pSlot);
	VOID				SetProperty(ItemInfo *pSlot);
	VOID				SetCharInfo(SI16 siX, SI16 siY, CHAR *pCharName, CHAR *pGuildName, SI32	siGuildClass, SI32 siLevel);

	// robypark 2004/11/8 14:48
	// Ŀ�� �Ʒ� ĳ���Ͱ� ������ ������ ��� ǳ�� ���� �ؽ�Ʈ ����
	// [IN]  CHAR *pString: ĳ���� �̸� ����
	// [IN]  SI16 siX: ����� ��ġ
	// [IN]  SI16 siY: ����� ��ġ
	// [IN]  SI16 siDirect: ����� ���� �÷���
	// [IN]  SI32 siColor: �ؽ�Ʈ Į��
	// [OUT] SI08 &siDrawGaugeHealthPosX: ü�� �� ��ġ
	// [OUT] SI08 &siDrawGaugeHealthPosY: ü�� �� ��ġ
	// [IN]  bool bDrawSupplyGoods: ������ ���� ���� ����ǰ(%)�� �׷�����ϴ���. ���޼����� �ش�ȴ�.
	// [IN]  SI16 FlagXSize = -1: ��� ��� ũ��
	// [IN]  SI16 FlagYSize = -1: ��� ��� ũ��
	// [OUT] SI16* FlagRenderPosX = NULL: ��� ��� �׷����� ��ġ
	// [OUT] SI16* FlagRenderPosY = NULL: ��� ��� �׷����� ��ġ
	VOID				SetSiegeWarfareUnitToolTipText(CHAR *pString, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor,
															SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
															SI16 FlagXSize = -1, SI16 FlagYSize = -1,
															SI16* FlagRenderPosX = NULL, SI16* FlagRenderPosY = NULL);

	// robypark 2005/2/17 11:51
	// Ŀ�� �Ʒ� ĳ���Ͱ� ��ܿ� ������ ������ ������ ��� ǳ�� ���� �ؽ�Ʈ ����
	// [IN]  CHAR *pString: ĳ���� �̸� ����
	// [IN]	 CHAR *pGuildName: �������
	// [IN]  SI16 siX: ����� ��ġ
	// [IN]  SI16 siY: ����� ��ġ
	// [IN]  SI16 siDirect: ����� ���� �÷���
	// [IN]  SI32 siColor: �ؽ�Ʈ Į��
	// [OUT] SI08 &siDrawGaugeHealthPosX: ü�� �� ��ġ
	// [OUT] SI08 &siDrawGaugeHealthPosY: ü�� �� ��ġ
	// [IN]  bool bDrawSupplyGoods: ������ ���� ���� ����ǰ(%)�� �׷�����ϴ���. ���޼����� �ش�ȴ�.
	// [IN]  SI16 FlagXSize = -1: ��� ��� ũ��
	// [IN]  SI16 FlagYSize = -1: ��� ��� ũ��
	// [OUT] SI16* FlagRenderPosX = NULL: ��� ��� �׷����� ��ġ
	// [OUT] SI16* FlagRenderPosY = NULL: ��� ��� �׷����� ��ġ
	VOID				SetSiegeWarfareUnitToolTipText(CHAR *pString, CHAR *pGuildName, SI16 siX, SI16 siY, SI16 siDirect, SI32 siColor,
															SI08 &siDrawGaugeHealthPosX, SI08 &siDrawGaugeHealthPosY, bool bDrawSupplyGoods,
															SI16 FlagXSize = -1, SI16 FlagYSize = -1,
															SI16* FlagRenderPosX = NULL, SI16* FlagRenderPosY = NULL);

private:
	VOID				SetDirection(SI16 &x, SI16 &y);															// ������ ��µ� ��ǥ�� ���Ѵ�.

	VOID				WeaponHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );	// ���⿡ ���� ������
	VOID				OthersHelp( CItemHeader *pItem, MONEY siBuyPrice, MONEY siSellPrice, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );	// ��Ÿ(����, �Ǽ��縮, �Ҹ�ǰ) ���� ������
	VOID				TradeHelp(CItemHeader *pItem, ItemInfo *pSlot);											// ��ο�
	VOID                DescriptionHelp( CItemHeader *pItem, OnlineFollower *pOF = NULL, SI16 siItemWearType = -1 );
};


#endif