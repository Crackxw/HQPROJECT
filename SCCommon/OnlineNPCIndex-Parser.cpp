//담 당 자 : 이 상 원
#include <GSL.h>

#include	"GSCDefine.h"
#include	"OnlineVillage-Parser.h"
#include	"OnlineNPCIndex-Parser.h"
#include	"OnlineText.h"
#include	"BindJxFile.h"


OnlineNPCIndexParser::OnlineNPCIndexParser()
{
	pNPCHeader = NULL;
	siNPCNum   = 0;
	
	pVillageNPCHeader = NULL;
	siVillageNPCNum   = 0;

	pSortNPCHeader = NULL;
	siSortNPCNum   = 0;
}

OnlineNPCIndexParser::~OnlineNPCIndexParser()
{
	Free();
}

BOOL OnlineNPCIndexParser::ReadText(char *pFileName)
{
	FILE	*fp		= NULL;
	char	*pToken = NULL;
	char	szBuffer[1024];	
	SI16	i;

	if(pNPCHeader) return TRUE;

	fp = BindJXFile::GetInstance()->GetFile( pFileName );
	if(fp == NULL) return FALSE;

	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);

	while( fgets(szBuffer, 1024, fp) )
		siNPCNum++;

	fseek( fp, SEEK_CUR, 0 );
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);
	fgets(szBuffer, 1024, fp);


	if(pNPCHeader) delete[] pNPCHeader;
	pNPCHeader = new NPCIndexHeader[siNPCNum];
	ZeroMemory(pNPCHeader, sizeof(NPCIndexHeader) * siNPCNum);

	for(i = 0; i < siNPCNum; i++)
	{
		ZeroMemory(szBuffer, 1024);
		fgets(szBuffer, 1024, fp);

		// Code
		pToken = strtok(szBuffer, "\t\n\r");
		if(pToken) pNPCHeader[i].siCode = atoi(pToken);
		
		// Type
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siType = atoi(pToken);
		
		// Nation Code
		pToken = strtok(NULL, "\t\n\r");
		switch(pToken[0])
		{
			case 'K':	case 'k':
				pNPCHeader[i].siNationCode = ON_COUNTRY_KOREA;
				break;

			case 'J':	case 'j':
				pNPCHeader[i].siNationCode = ON_COUNTRY_JAPAN;
				break;

			case 'C':	case 'c':
				pNPCHeader[i].siNationCode = ON_COUNTRY_CHINA;
				break;

			case 'T':	case 't':
				pNPCHeader[i].siNationCode = ON_COUNTRY_TAIWAN;
				break;
			case 'A':	case 'a':
				pNPCHeader[i].siNationCode = ON_COUNTRY_ALL;
				break;
		}		

		// Data
		pNPCHeader[i].siDataCode = 0;
		pToken = strtok(NULL, "\t\n\r");
		switch(pNPCHeader[i].siType)
		{
			case 0:			// 건물
				// Struct Code
				if(pToken)
				{
					if(strcmp(pToken, ON_VIL_ST_KIND_CODE_BARRACK) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_BARRACK;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_MARKET) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_MARKET;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_BANK) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_BANK;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_HOSPITAL) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_HOSPITAL;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_INN) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_INN;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_WHARF) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_WHARF;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_FARM) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_FARM;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_GOVERNMENT) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_GOVERNMENT;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_WSTORE) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_WSTORE;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_RING) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_RING;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_FSTORE) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_FSTORE;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_DSTORE) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_DSTORE;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_WEAPON) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_WEAPON;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_ARMOR) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_ARMOR;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_PASTURE) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_PASTURE;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_FACTORY) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_FACTORY;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_GBARRACK) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_GBARRACK;
					}
					else if(strcmp(pToken, ON_VIL_ST_KIND_CODE_MBARRACK) == 0)
					{
						pNPCHeader[i].siDataCode = ON_VIL_ST_KIND_MBARRACK;
					}
				}
				break;

			case 1:			// 퀘스트
				pNPCHeader[i].siDataCode = atoi(pToken);
				break;

			case 2:			// 경매용
				pNPCHeader[i].siDataCode = atoi(pToken);
				break;
		}

		// Struct Name
		pToken = strtok(NULL, "\t\n\r");
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siName = atoi(pToken);

		// Map Index
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siMapIndex = atoi(pToken);

		// Map X Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siMapXPos = atoi(pToken);

		// Map Y Position
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siMapYPos = atoi(pToken);

		// 혼잣말
		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siTalk[0] = atoi(pToken);


		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siTalk[1] = atoi(pToken);

		pToken = strtok(NULL, "\t\n\r");
		if(pToken) pNPCHeader[i].siTalk[2] = atoi(pToken);

		// robypark 2004/9/9 18:15
		// 혼잣말하기 활성화여부 설정. 기본 설정으로 활성화되어있다.
		pNPCHeader[i].bTalkOneself = TRUE;
	}

	fclose(fp);

	if(pVillageNPCHeader)
	{
		delete[] pVillageNPCHeader;
		pVillageNPCHeader = NULL;
	}
	pVillageNPCHeader = NULL;
	siVillageNPCNum   = 0;

	if(pSortNPCHeader)
	{
		delete[] pSortNPCHeader;
		pSortNPCHeader = NULL;
	}
	pSortNPCHeader = NULL;
	siSortNPCNum   = 0;

	return TRUE;
}

VOID OnlineNPCIndexParser::Free()
{
	if(pNPCHeader)
	{
		delete[] pNPCHeader;
		pNPCHeader = NULL;
	}
	siNPCNum   = 0;
	
	if(pVillageNPCHeader)
	{
		delete[] pVillageNPCHeader;
		pVillageNPCHeader = NULL;
	}
	siVillageNPCNum   = 0;

	if(pSortNPCHeader)
	{
		delete[] pSortNPCHeader;
		pSortNPCHeader = NULL;
	}
	siSortNPCNum   = 0;
}

BOOL OnlineNPCIndexParser::SetMapIndex(SI16 MapIndex)
{
	SI16		TempCount;
	SI16		TempCount1;

	if(pSortNPCHeader)
	{
		delete[] pSortNPCHeader;
		pSortNPCHeader = NULL;
	}

	pSortNPCHeader = NULL;
	siSortNPCNum   = 0;

	// 우선 갯수를 검사한다.
	for(TempCount = 0; TempCount < siNPCNum; TempCount++)
	{
		if(pNPCHeader[TempCount].siMapIndex == MapIndex)
		{
			siSortNPCNum++;
		}
	}

	if(siSortNPCNum)
	{
		pSortNPCHeader = new NPCIndexHeader[siSortNPCNum];
		for(TempCount = 0, TempCount1 = 0; TempCount < siNPCNum; TempCount++)
		{
			if(pNPCHeader[TempCount].siMapIndex == MapIndex)
			{
				CopyMemory(&pSortNPCHeader[TempCount1], &pNPCHeader[TempCount], sizeof(NPCIndexHeader));
				TempCount1++;
			}
		}
	}

	return TRUE;
}

NPCIndexHeader* OnlineNPCIndexParser::GetNPC(SI16 XPos, SI16 YPos, SI16 MapIndex)
{
	for(SI16 i = 0; i < siSortNPCNum; i++)
	{
		if(pSortNPCHeader[i].siMapIndex == MapIndex)
		{
			if((pSortNPCHeader[i].siMapXPos == XPos) && (pSortNPCHeader[i].siMapYPos == YPos))
			{
				return &pSortNPCHeader[i];
			}
		}
	}

	for(i = 0; i < siVillageNPCNum; i++)
	{
		if(pVillageNPCHeader[i].siMapIndex == MapIndex)
		{
			if((pVillageNPCHeader[i].siMapXPos == XPos) && (pVillageNPCHeader[i].siMapYPos == YPos))
			{
				return &pVillageNPCHeader[i];
			}
		}
	}

	return NULL;
}

NPCIndexHeader* OnlineNPCIndexParser::GetNPC(SI16 siNPCCode)
{
	for(SI16 i = 0; i < siSortNPCNum; i++)
	{
		if(pSortNPCHeader[i].siCode == siNPCCode)
		{
			return &pSortNPCHeader[i];
		}
	}

	for(i = 0; i < siVillageNPCNum; i++)
	{
		if(pVillageNPCHeader[i].siCode == siNPCCode)
		{
			return &pVillageNPCHeader[i];
		}
	}

	return NULL;
}

NPCIndexHeader* OnlineNPCIndexParser::GetNPCByIndex(SI16 siIndex)
{
	return &pNPCHeader[siIndex];
}

NPCIndexHeader* OnlineNPCIndexParser::GetNPCAllMap(SI16 siNPCCode)
{
	for(SI16 i = 0; i < siNPCNum; i++)
	{
		if(pNPCHeader[i].siCode == siNPCCode)
		{
			return &pNPCHeader[i];
		}
	}

	return NULL;
}

NPCIndexHeader* OnlineNPCIndexParser::GetQuestNPCAllMap(SI16 siQuestNPCCode)
{
	for(SI16 i = 0; i < siNPCNum; i++)
	{
		if(pNPCHeader[i].siType == 1)
		{
			if(pNPCHeader[i].siDataCode == siQuestNPCCode)
			{
				return &pNPCHeader[i];
			}
		}
	}

	return NULL;
}

NPCIndexHeader* OnlineNPCIndexParser::GetNameNPCAllMap(OnlineText* lpText, SI16 siType, char* lpName)
{
	char*	lpNPCName;

	for(SI16 i = 0; i < siNPCNum; i++)
	{
		if(pNPCHeader[i].siType == siType)
		{
			lpNPCName = lpText->Get(pNPCHeader[i].siName);
			if(lpNPCName)
			{
				if(strcmp(lpNPCName, lpName) == 0)
				{
					return &pNPCHeader[i];
				}
			}
		}
	}

	return NULL;
}

NPCIndexHeader* OnlineNPCIndexParser::GetAuctionNPC(int number)
{
	int count = 0;
	for(SI16 i = 0; i < siSortNPCNum; i++)
	{
		if(pSortNPCHeader[i].siType == 2)
		{
			if (pSortNPCHeader[i].siDataCode == 2) {

				if (count >= number) return &pSortNPCHeader[i];
				count++;
			}
		}
	}

	
	return NULL;
}


BOOL OnlineNPCIndexParser::AddVillageMapNPC(SI16 AddNPCCount, NPCIndexHeader* lpAddNPC)
{
	if(pVillageNPCHeader)
	{
		delete[] pVillageNPCHeader;
		pVillageNPCHeader = NULL;
	}
	siVillageNPCNum = 0;

	siVillageNPCNum   = AddNPCCount;
	pVillageNPCHeader = new NPCIndexHeader[siVillageNPCNum];
	CopyMemory(pVillageNPCHeader, lpAddNPC, sizeof(NPCIndexHeader) * siVillageNPCNum);

	return TRUE;
}

