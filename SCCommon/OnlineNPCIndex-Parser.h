#ifndef __ONLINENPCINDEX_PARSER_H
#define __ONLINENPCINDEX_PARSER_H


class OnlineText;


struct	NPCIndexHeader
{
	SI16	siCode;																				// Code

	SI32	siName;																				// Name
	SI16	siType;																				// Type(0 : �ǹ�, 1 : ����Ʈ��, 2 : ��ſ�)
	SI16	siNationCode;																		// ���� �ڵ�

	SI16	siDataCode;																			// Data(�ǹ� Type�� : �ǹ� �ڵ�, ����Ʈ Type�� : ����Ʈ �ڵ�, ��� Type�� : 0 : ���� �ô� NPC, 1 : ����ϴ� NPC)

	SI16	siMapIndex;																			// Map Index
	SI16	siMapXPos;																			// Map X Pos
	SI16	siMapYPos;																			// Map Y Pos

	SI32	siTalk[3];																			// ȥ�㸻

	// robypark 2004/9/9 18:11
	// NPC ȥ�㸻 ����� Ȱ��ȭ ����
	BOOL	bTalkOneself;
};


class OnlineNPCIndexParser
{
protected:
	NPCIndexHeader			*pNPCHeader;														// NPC�� ��� ������ ���� ����ü
	SI16					siNPCNum;															// �� NPC
	
	NPCIndexHeader			*pVillageNPCHeader;													// Village NPC�� ��� ������ ���� ����ü
	SI16					siVillageNPCNum;													// �� Village NPC
	
	NPCIndexHeader			*pSortNPCHeader;													// ������ NPC�� ��� ������ ���� ����ü
	SI16					siSortNPCNum;														// �� NPC

public:
	OnlineNPCIndexParser();
	~OnlineNPCIndexParser();

	BOOL					ReadText(char *pFileName);											// NPC Index �ε�
	VOID					Free();																// �����

	BOOL					SetMapIndex(SI16 MapIndex);											// Set MapIndex
	NPCIndexHeader*			GetNPC(SI16 XPos, SI16 YPos, SI16 MapIndex);						// Get NPC
	NPCIndexHeader*			GetNPC(SI16 siNPCCode);												// Get NPC
	NPCIndexHeader*			GetNPCByIndex(SI16 siIndex);	
	NPCIndexHeader*			GetNPCAllMap(SI16 siNPCCode);										// Get NPC. ��� Map�� �ִ� NPC �˻�
	NPCIndexHeader*			GetQuestNPCAllMap(SI16 siQuestNPCCode);								// Get NPC. ��� Map�� �ִ� NPC �˻�
	NPCIndexHeader*			GetNameNPCAllMap(OnlineText* lpText, SI16 siType, char* lpName);	// Get NPC. ��� Map�� �ִ� NPC �˻�
	SI16					GetNPCNum()	{ return siNPCNum;	};
    SI16					GetVillageNPCNum()	{ return siVillageNPCNum;	};
	SI16					GetSortNPCNum()	{ return siSortNPCNum;	};

	NPCIndexHeader*         GetAuctionNPC(int number);                     // ��� ���� NPC�� �׸ʿ� ������ ���´� ������ NULL



	BOOL					AddVillageMapNPC(SI16 AddNPCCount, NPCIndexHeader* lpAddNPC);		// Add Village Map 
};


#endif
