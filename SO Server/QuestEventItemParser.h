#ifndef	QUEST_EVENT_ITEM_PARSER
#define QUEST_EVENT_ITEM_PARSER

#include <windows.h>
#include "../SCCommon/GSCDefine.h"


#define  MAX_ITEM_EVENT		100

struct stQuestEventItem
{
	int  siItemNum;			//������ Num.DB���̺�� ������ ����.
	BOOL bDeliveryItem;		//��޿� �������ΰ�? ���ӳ� �������ΰ�?
	char szItemName[256];	//������ �̸�.
	BOOL bLogUseItemID;	    //�α׸� ����� �������ΰ� �ƴѰ�(DB���� �̺�Ʈ ���̺� Insert���ִ� �������� �ƴѰ�� �α׸� ������ �ʴ´�.
	int	 siDropPrecent;		//���Ȯ��.
	int	 siNextItemNum;		//��÷���� ������ ��� �޴� ������.
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