#ifndef __ONLINENPCINDEX_PARSER_H
#define __ONLINENPCINDEX_PARSER_H


class OnlineText;


struct	NPCIndexHeader
{
	SI16	siCode;																				// Code

	SI32	siName;																				// Name
	SI16	siType;																				// Type(0 : 건물, 1 : 퀘스트용, 2 : 경매용)
	SI16	siNationCode;																		// 국가 코드

	SI16	siDataCode;																			// Data(건물 Type시 : 건물 코드, 퀘스트 Type시 : 퀘스트 코드, 경매 Type시 : 0 : 물건 맡는 NPC, 1 : 경매하는 NPC)

	SI16	siMapIndex;																			// Map Index
	SI16	siMapXPos;																			// Map X Pos
	SI16	siMapYPos;																			// Map Y Pos

	SI32	siTalk[3];																			// 혼잣말

	// robypark 2004/9/9 18:11
	// NPC 혼잣말 기능이 활성화 여부
	BOOL	bTalkOneself;
};


class OnlineNPCIndexParser
{
protected:
	NPCIndexHeader			*pNPCHeader;														// NPC의 모든 정보를 가진 구조체
	SI16					siNPCNum;															// 총 NPC
	
	NPCIndexHeader			*pVillageNPCHeader;													// Village NPC의 모든 정보를 가진 구조체
	SI16					siVillageNPCNum;													// 총 Village NPC
	
	NPCIndexHeader			*pSortNPCHeader;													// 정렬한 NPC의 모든 정보를 가진 구조체
	SI16					siSortNPCNum;														// 총 NPC

public:
	OnlineNPCIndexParser();
	~OnlineNPCIndexParser();

	BOOL					ReadText(char *pFileName);											// NPC Index 로드
	VOID					Free();																// 지운다

	BOOL					SetMapIndex(SI16 MapIndex);											// Set MapIndex
	NPCIndexHeader*			GetNPC(SI16 XPos, SI16 YPos, SI16 MapIndex);						// Get NPC
	NPCIndexHeader*			GetNPC(SI16 siNPCCode);												// Get NPC
	NPCIndexHeader*			GetNPCByIndex(SI16 siIndex);	
	NPCIndexHeader*			GetNPCAllMap(SI16 siNPCCode);										// Get NPC. 모든 Map에 있는 NPC 검사
	NPCIndexHeader*			GetQuestNPCAllMap(SI16 siQuestNPCCode);								// Get NPC. 모든 Map에 있는 NPC 검사
	NPCIndexHeader*			GetNameNPCAllMap(OnlineText* lpText, SI16 siType, char* lpName);	// Get NPC. 모든 Map에 있는 NPC 검사
	SI16					GetNPCNum()	{ return siNPCNum;	};
    SI16					GetVillageNPCNum()	{ return siVillageNPCNum;	};
	SI16					GetSortNPCNum()	{ return siSortNPCNum;	};

	NPCIndexHeader*         GetAuctionNPC(int number);                     // 경매 진행 NPC가 그맵에 있으면 얻어온다 없으면 NULL



	BOOL					AddVillageMapNPC(SI16 AddNPCCount, NPCIndexHeader* lpAddNPC);		// Add Village Map 
};


#endif
