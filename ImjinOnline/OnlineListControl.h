#ifndef __LISTCONTROL_H
#define __LISTCONTROL_H

/********************************************************************************************
	���ϸ�	:	OnlineListControl.h
	�ۼ���	:	�� �� ��
	�����	:	�� �� ��
	�ۼ���	:	2001.05.31
	������	:	2002.06.04
********************************************************************************************/

 
// �ۼ��� : ������
// �κ��丮�� ���� ������ ������ ���� �������̽� Ŭ���� �̴�.

#include <iostream>
#include <vector>

using namespace std;

struct ItemInfo
{
	UI32	uiItemID;
	UI32	ItemCount;
	SI16	siColor;
	MONEY	uiBuyPrice;
	MONEY	uiSellPrice;
	BOOL	bHide;
	SI08	siType;

	OnFollowerInfo					Follower;
	OnIndustrialComplexTradeInfo	Property;

	ItemInfo::ItemInfo()
	{
		uiItemID = ItemCount = siColor = 0;
		siType = 0;
		uiBuyPrice = uiSellPrice = 0;
		bHide = FALSE;

		ZeroMemory(&Follower, sizeof(Follower));
	}

	const bool ItemInfo::operator==(const ItemInfo &rhs) const { return this->uiItemID == rhs.uiItemID ? true : false; };
};


class OnlineListControl
{
private:
	const cltOnlineWorld *m_pMyOnlineWorld;
	vector<ItemInfo>	m_SlotInfo;					// �� ĭ�� ����
	vector<RECT>		m_SlotRect;					// �� ĭ�� ũ�� �浹 ó����

	SI16	m_siWidth, m_siHeight;					// �� ĭ�� ���� ���� ũ��
	SI16	m_siXnum, m_siYnum;						// ĭ�� ����
	SI16	m_siXpos, m_siYpos;						// ���� ��ǥ
	SI32	m_siType;								// ����Ʈ ��Ʈ���� �κ��丮�� �������̳�.. ������ �뺴 ����

	bool	m_bNowDrag;								// �������� ���� �巡�� ���̴�.
	SI16	m_siDragSlot;							// ���� �̵����� ������ ���� ��ȣ��
	SI16	m_siOldDragSlot;						// �̵��ϱ� �ٷ� ���� ���� ��ȣ.

public:
	OnlineListControl(cltOnlineWorld *pOnlineWorld) : m_pMyOnlineWorld(pOnlineWorld)		{ m_bNowDrag = false; m_siDragSlot = -1; };
	~OnlineListControl()																	{ m_SlotInfo.clear(); m_SlotRect.clear(); };

	void Init(SI16 siWidth, SI16 siHeight, SI16 siXsize, SI16 siYsize, SI16 siXpos, SI16 siYpos, SI32 siType);	// �ʱ�ȭ	
	void Draw(LPDIRECTDRAWSURFACE7 pSurface);																	// �׸���
	void Draw(LPDIRECTDRAWSURFACE7 pSurface, XSPR *psprOverImage, SI32 siFontNumber = 0);						// �׸���, �̹���(psprOverImage)���� �߿� ��Ʈ��ȣ(siFontNumber)�� �ش�Ǵ� �׸��� ���׷��ش�. ������ �������� ����.
	bool DragUp();
	SI16 DragDown();
	
	const ItemInfo *GetSlotInfo(const SI16 siSlot) const;														// ������ ���Ը� ������ �ش�.
	SI16 GetDragSlot() const { return m_siDragSlot; };															// �巡�� ���� ���� �ε����� ������ �ش�.
	inline const SI16 GetSlot(const UI32 uiID) const;															// ���̵� �޾� ���° ���Կ� �ִ°��� ������ �ش�.
	const SI16 GetOldDragSlot() const { return m_siOldDragSlot; };												// �� ���� ������ ������ ��ȣ

	inline const bool AddItem(const ItemInfo &Slot, const SI16 siSlot);									        // ������ ����	
	inline void DeleteSlot( const SI16 siSlot, const UI32 siItemCount = 0 );									// �������� �����.	

	void StopDrag() { m_bNowDrag = false; m_siDragSlot = -1; };													// �巡�� ����
	void Clear()	{ ZeroMemory( &m_SlotInfo[0], sizeof(ItemInfo) * m_SlotInfo.size()); };						// ����Ʈ�� �ִ� ��� �������� ���� �ش�.
	
	void SetColor(const SI16 siSlot, const SI16 siColor) { m_SlotInfo[siSlot].siColor = siColor; };				// �������� ��� �Ҷ� ���� ������ �ش�.
	inline const SI16 GetEmptySlot(const UI32 uiID=0) const;													// ��� �ִ� ������ ���ϸ� ���ش�. ���� ������ ���̵� ���ڷ� ��� ���� ���� ���̵�� ã�� �ش�.
	inline const SI16 GetItemNum() const;																		// ����Ʈ ��Ʈ�ѿ� �� �� ���� �������� ��� �ִ��� ������ �ش�.	
	const SI16 GetSelectSlot() const;																			// ���� ������ Slot�� �˾� ����.	
	SI16 IsEmptySlot(const SI16 siSlot) const;																	// ������ ���� �ִ��� �� �ִ��� �˾� ����.
	
	inline const void ShowAllItems();																			// ���� ���� �������� ��� �� �ش�.
	inline const void HideItem( const SI16 siSlot ) { m_SlotInfo[siSlot].bHide = true; };						// ������ ������ �������� �����.

	const SI16 GetWidth() const { return m_siWidth; };
	const SI16 GetHeight() const { return m_siHeight; };
	const SI16 GetXnum() const { return m_siXnum; };
	const SI16 GetYnum() const { return m_siYnum; };	
	void CopyData(const SI16 siSlot, ItemInfo &CopyData) { CopyData = m_SlotInfo[siSlot]; };					// ���� ���Կ� �ִ� ����Ÿ�� ������ �ش�.

	void SetPos(SI16 siX,SI16 siY); // ��ġ�� �����Ѵ�.

private:
	inline void AddDragItem(const SI16 siDragSlot);																// �巡�� ���� �������� �����Ѵ�.
	
	
};

inline const void OnlineListControl::ShowAllItems()
{
	for(vector<ItemInfo>::iterator i=m_SlotInfo.begin(); i!=m_SlotInfo.end(); ++i)	i->bHide = false;
}

inline void OnlineListControl::DeleteSlot(const SI16 siSlot, const UI32 siItemCount)
{
	if(m_SlotInfo[siSlot].siType == LIST_ITEM)
	{
		if(siItemCount == 0)
			m_SlotInfo[siSlot].uiItemID = 0;
		else
			m_SlotInfo[siSlot].ItemCount -= siItemCount;
		if(!m_SlotInfo[siSlot].ItemCount)	
			m_SlotInfo[siSlot].uiItemID = 0;
	}
	else if(m_SlotInfo[siSlot].siType == LIST_FOLLOWER)
	{
		ZeroMemory(&m_SlotInfo[siSlot], sizeof(ItemInfo));
		m_SlotInfo[siSlot].siType = LIST_NOT;
	}
};

inline void OnlineListControl::AddDragItem(const SI16 siDragSlot)
{
	m_SlotInfo[siDragSlot] = m_SlotInfo[m_siDragSlot];
	m_SlotInfo[m_siDragSlot].uiItemID = 0;
	StopDrag();
}

inline const bool OnlineListControl::AddItem(const ItemInfo &Slot, const SI16 siSlot)
{ 
	if(siSlot != -1)
	{
		m_SlotInfo[siSlot] = Slot;
		return true;
	}
	return false; 
};

inline const SI16 OnlineListControl::GetSlot(const UI32 uiID) const
{
	for(SI16 i=0; i<(SI16)m_SlotInfo.size(); ++i )
		if( m_SlotInfo[i].uiItemID == uiID )	return i;

	return -1;
}

inline const SI16 OnlineListControl::GetEmptySlot(const UI32 uiID) const
{
	for( SI16 i=0; i<(SI16)m_SlotInfo.size(); i++ )	if( uiID == m_SlotInfo[i].uiItemID )	return (const)i;
	for( i=0; i<(SI16)m_SlotInfo.size(); i++ )		if( 0 == m_SlotInfo[i].uiItemID )		return (const)i;
	return -1; 
}

inline const SI16 OnlineListControl::GetItemNum() const
{
	SI16 siRet = 0;
	for(SI16 i=0; i<(SI16)m_SlotInfo.size(); i++)	
		m_SlotInfo[i].uiItemID != 0 ? siRet += 1 : 0;

	return siRet;
};

#endif
