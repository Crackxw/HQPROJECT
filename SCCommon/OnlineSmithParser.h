#ifndef __ONLINESMITH_PARSER_H
#define __ONLINESMITH_PARSER_H

#include	"OnlineCommonStruct.h"

struct SmithItemHeader
{
	UI16 usOriginalItem;
	UI16 usNormalItem;
	UI16 usSpecialItem;

	UI16 usNeedItem;

	UI32 uiNeedItemCount;

	MONEY moNeedMoney;

	UI32 uiBrokenProbablity;
	UI32 uiOriginalProbablity;
	UI32 uiNormalProbablity;
	UI32 uiSpecialProbablity;

	
};

class OnlineSmithParser
{
private:
	SmithItemHeader*		m_lpSmithItemHeader;												// 제련아이템?을 위한 자료들
	SI32					m_siSmithItemNum;													// 총 Item
	
public:
	OnlineSmithParser();
	virtual	~OnlineSmithParser();

	BOOL					ReadText(char *pFileName);											// Ticket 로드
	VOID					Free();																// 지운다

	SmithItemHeader*		GetSmithItem(UI16 uiOldItemCode);								    // Get Ticket
	
	
};



#endif
