#include "SOPlayer_ItemBankList.h"

ItemBankList::ItemBankList()
{
	m_uiItemCount = 0;	
}

ItemBankList::~ItemBankList()
{

}

void ItemBankList::SetList( UI32 itemCount, char *pData )
{

	memcpy( m_pBuf, pData, itemCount * 4 );	

	m_uiItemCount = itemCount;
}

BOOL ItemBankList::FindItem( UI16 itemCode, UI16 quantity = 0 )
{
	BOOL bRet = FALSE;

	UI16 *pItem;
	UI32 i;

	for( i = 0; i < m_uiItemCount; ++i ) {
		
		pItem = (UI16 *)m_pBuf + ( i * 2 );
		
		if( itemCode == *pItem ) {

			if( quantity <= *(pItem + 1) ) bRet = TRUE;

			break;
		}

	}

	return bRet;
}

BOOL ItemBankList::TakeOutItem( UI16 itemCode, UI16 quantity = 0 )
{
	
	BOOL bRet = FALSE;

	UI16 *pItem;
	UI32 i;

	for( i = 0; i < m_uiItemCount; ++i ) {
		
		pItem = (UI16 *)m_pBuf + ( i * 2 );

		if( itemCode == *pItem ) {
			
			if( *(pItem + 1) < quantity ) break;
			
			if( *(pItem + 1) == quantity || quantity == 0 ) {
				
				--m_uiItemCount;

				memcpy( pItem, pItem + 2, ( m_uiItemCount * 4 ) - ( (UI32)pItem - (UI32)m_pBuf ) );

			} else {
				
				*(pItem + 1) -= quantity;

			}

			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL ItemBankList::PutInItem( UI16 itemCode, UI16 quantity )
{
	UI16 *pItem;

	pItem = (UI16 *)m_pBuf + ( m_uiItemCount * 2 );

	memcpy( pItem, &itemCode, 2 );
	memcpy( pItem + 1, &quantity, 2 );

	++m_uiItemCount;

	return TRUE;
}

void ItemBankList::PrintList()
{
/*
	UI16 *pItem;

	printf( "List....\n" );

	for( UI32 i = 0; i < m_uiItemCount; ++i ) {
		
		pItem = (UI16 *)m_pBuf + ( i * 2 );
		printf( "ItemCode : %d ," , *pItem );

		++pItem;
		printf( "ItemQuantity : %d\n", *pItem );
	}
*/
}
