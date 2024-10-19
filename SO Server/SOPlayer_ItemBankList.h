#ifndef __ITEMBANKLIST_H
#define __ITEMBANKLIST_H

#include <windows.h>
#include <Directives.h>

class ItemBankList
{
public:
	ItemBankList();
	~ItemBankList();

public:
	void	Clear() { m_uiItemCount = 0; }

	void	SetList( UI32 itemCount, char *data );
	char*	GetList() { return m_pBuf; };

	UI32	GetItemCount() { return m_uiItemCount; };
	
	BOOL	FindItem( UI16 itemCode, UI16 quantity );
	
	BOOL	TakeOutItem( UI16 itemCode, UI16 quantity );
	
	BOOL	PutInItem( UI16 itemCode, UI16 quantity );

	void	PrintList();

private:
	UI32		m_uiItemCount;
	char		m_pBuf[ 1024 ];

};

#endif

