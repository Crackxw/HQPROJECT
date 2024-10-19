#ifndef __LISTCONTROL_H
#define __LISTCONTROL_H

/********************************************************************************************
	파일명	:	OnlineListControl.h
	작성자	:	정 진 욱
	담당자	:	정 진 욱
	작성일	:	2001.05.31
	수정일	:	2002.06.04
********************************************************************************************/

 
// 작성자 : 정진욱
// 인벤토리와 장착 아이템 관리를 위한 인터페이스 클래스 이다.

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
	vector<ItemInfo>	m_SlotInfo;					// 한 칸의 정보
	vector<RECT>		m_SlotRect;					// 한 칸의 크기 충돌 처리용

	SI16	m_siWidth, m_siHeight;					// 한 칸의 가로 세로 크기
	SI16	m_siXnum, m_siYnum;						// 칸의 갯수
	SI16	m_siXpos, m_siYpos;						// 기준 좌표
	SI32	m_siType;								// 리스트 컨트롤이 인벤토리나 장착용이냐.. 나중은 용병 까지

	bool	m_bNowDrag;								// 아이템이 현재 드래그 중이다.
	SI16	m_siDragSlot;							// 현재 이동중인 아이템 슬롯 번호다
	SI16	m_siOldDragSlot;						// 이동하기 바로 전의 슬롯 번호.

public:
	OnlineListControl(cltOnlineWorld *pOnlineWorld) : m_pMyOnlineWorld(pOnlineWorld)		{ m_bNowDrag = false; m_siDragSlot = -1; };
	~OnlineListControl()																	{ m_SlotInfo.clear(); m_SlotRect.clear(); };

	void Init(SI16 siWidth, SI16 siHeight, SI16 siXsize, SI16 siYsize, SI16 siXpos, SI16 siYpos, SI32 siType);	// 초기화	
	void Draw(LPDIRECTDRAWSURFACE7 pSurface);																	// 그리기
	void Draw(LPDIRECTDRAWSURFACE7 pSurface, XSPR *psprOverImage, SI32 siFontNumber = 0);						// 그리기, 이미지(psprOverImage)파일 중에 폰트번호(siFontNumber)에 해당되는 그림을 덧그려준다. 소집권 아이템을 위함.
	bool DragUp();
	SI16 DragDown();
	
	const ItemInfo *GetSlotInfo(const SI16 siSlot) const;														// 아이템 슬롯를 리턴해 준다.
	SI16 GetDragSlot() const { return m_siDragSlot; };															// 드래그 중인 슬롯 인덱스를 리턴해 준다.
	inline const SI16 GetSlot(const UI32 uiID) const;															// 아이디를 받아 몇번째 슬롯에 있는가를 리턴해 준다.
	const SI16 GetOldDragSlot() const { return m_siOldDragSlot; };												// 이 전에 움직인 슬롯을 번호

	inline const bool AddItem(const ItemInfo &Slot, const SI16 siSlot);									        // 아이템 삽입	
	inline void DeleteSlot( const SI16 siSlot, const UI32 siItemCount = 0 );									// 아이템을 지운다.	

	void StopDrag() { m_bNowDrag = false; m_siDragSlot = -1; };													// 드래그 중지
	void Clear()	{ ZeroMemory( &m_SlotInfo[0], sizeof(ItemInfo) * m_SlotInfo.size()); };						// 리스트에 있는 모든 아이템을 지워 준다.
	
	void SetColor(const SI16 siSlot, const SI16 siColor) { m_SlotInfo[siSlot].siColor = siColor; };				// 아이템을 출력 할때 색을 지정해 준다.
	inline const SI16 GetEmptySlot(const UI32 uiID=0) const;													// 비어 있는 슬롯을 리턴를 해준다. 만약 아이템 아이디가 인자로 들어 오면 같은 아이디로 찾아 준다.
	inline const SI16 GetItemNum() const;																		// 리스트 컨트롤에 총 몇 개의 아이템이 들어 있는지 리턴해 준다.	
	const SI16 GetSelectSlot() const;																			// 현재 선택한 Slot를 알아 낸다.	
	SI16 IsEmptySlot(const SI16 siSlot) const;																	// 슬롯이 비이 있는지 차 있는지 알아 본다.
	
	inline const void ShowAllItems();																			// 현재 숨긴 아이템을 모두 모여 준다.
	inline const void HideItem( const SI16 siSlot ) { m_SlotInfo[siSlot].bHide = true; };						// 선택한 슬롯의 아이템을 숨긴다.

	const SI16 GetWidth() const { return m_siWidth; };
	const SI16 GetHeight() const { return m_siHeight; };
	const SI16 GetXnum() const { return m_siXnum; };
	const SI16 GetYnum() const { return m_siYnum; };	
	void CopyData(const SI16 siSlot, ItemInfo &CopyData) { CopyData = m_SlotInfo[siSlot]; };					// 현재 슬롯에 있는 데이타를 복사해 준다.

	void SetPos(SI16 siX,SI16 siY); // 위치를 변경한다.

private:
	inline void AddDragItem(const SI16 siDragSlot);																// 드래그 중인 아이템을 삽입한다.
	
	
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
