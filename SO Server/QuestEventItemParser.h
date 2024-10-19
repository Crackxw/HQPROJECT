#ifndef	QUEST_EVENT_ITEM_PARSER
#define QUEST_EVENT_ITEM_PARSER

#include <windows.h>
#include "../SCCommon/GSCDefine.h"


#define  MAX_ITEM_EVENT		100

struct stQuestEventItem
{
	int  siItemNum;			//아이템 Num.DB테이블과 동일한 순서.
	BOOL bDeliveryItem;		//배달용 아이템인가? 게임내 아이템인가?
	char szItemName[256];	//아이템 이름.
	BOOL bLogUseItemID;	    //로그를 남기는 아이템인가 아닌가(DB에서 이벤트 테이블에 Insert해주는 아이템이 아닌경우 로그를 남기지 않는다.
	int	 siDropPrecent;		//드롭확률.
	int	 siNextItemNum;		//당첨되지 못했을 경우 받는 아이템.
};

class CQuestEventItemParser	
{
private:
	
public:
	CQuestEventItemParser();
	~CQuestEventItemParser();
	int					Load(char* szFilename,int siQuestCode);
	stQuestEventItem*   GetItemName(int siItemNum);
	BOOL				IsLogUseItem(int siItemNum);
	int					GetNextItemID(int siItemNum);

	stQuestEventItem	sQuestEventItem[MAX_ITEM_EVENT];
	int					m_siQuestCode;
	int					m_siEventQuestItemIDCount;
};

class CQuestEventItemMgr
{
private:
		
public:
	CQuestEventItemMgr();
	~CQuestEventItemMgr();

	VOID					Init(short siCount);
	VOID					Load(char* szFilename,int siQuestCode);
	CQuestEventItemParser*  GetEventQuest(int QuestCode);
	

	CQuestEventItemParser*	m_pQuestEventItem;
	int						m_siEventQuestMgrCount;


};


#endif