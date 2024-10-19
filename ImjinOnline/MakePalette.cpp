/************************************************************************************************
	파일명 : MakePalette.h
	담당자 : 이윤석

	Game에서 특수효과에 사용하는 Palette Data를 생성한다.
************************************************************************************************/
#include <GSL.h>
#include "MakePalette.h"
#include <DebugTool.h>


//========================================================
// OnlinePaletteManager::OnlinePaletteManager() Constructor
OnlinePaletteManager::OnlinePaletteManager()
{
	ZeroMemory(BasePalette, 768);

	lpPalette       = NULL;
	PaletteMaxCount = 0;
}

//========================================================
// OnlinePaletteManager::~OnlinePaletteManager() Destroyer
OnlinePaletteManager::~OnlinePaletteManager()
{
	Free();
}

//========================================================
// OnlinePaletteManager::Init() Init
BOOL OnlinePaletteManager::Init(UI08* lpBasePalette, UI16 PaletteBufferCount)
{
	Free();

	CopyMemory(BasePalette, lpBasePalette, 768);

	lpPalette       = new OnlinePaletteData[PaletteBufferCount];
	PaletteMaxCount = PaletteBufferCount;
	ZeroMemory(lpPalette, sizeof(OnlinePaletteData) * PaletteBufferCount);

	return TRUE;
}

//========================================================
// OnlinePaletteManager::Free() Free
void OnlinePaletteManager::Free(void)
{
	if(lpPalette)
	{
		delete[] lpPalette;
		lpPalette = NULL;
	}

	ZeroMemory(BasePalette, 768);

	PaletteMaxCount = 0;
}

//========================================================
// OnlinePaletteManager::MakePalette() Make Palette 
SI16 OnlinePaletteManager::MakePalette(COLORREF MakeColorType)
{
	SI16					TempCount;
	SI16					TempCount1;
	SI16					ID;
	UI08					TempColor;
	UI32					TempColorErrorVal;
	UI32					TempColorErrorVal1;
	UI08					TempColorR;					
	UI08					TempColorG;					
	UI08					TempColorB;					
	UI08					MColorR;					
	UI08					MColorG;					
	UI08					MColorB;					

	// 빈 Buffer 찾음
	ID = FindEmptyBuffer();
	if(ID == -1) return -1;

	// Gray로 변환
	MColorR = (UI08)(MakeColorType & 0x000000ff);
	MColorG = (UI08)((MakeColorType & 0x0000ff00) >> 8);
	MColorB = (UI08)((MakeColorType & 0x00ff0000) >> 16);
	MColorR /= 4;
	MColorG /= 4;
	MColorB /= 4;
	for(TempCount = 0; TempCount < 256; TempCount++)
	{
		TempColor = ((BasePalette[(TempCount * 3)] * 30) + (BasePalette[(TempCount * 3) + 1] * 59) + (BasePalette[(TempCount * 3) + 2] * 10)) / 100;

		TempColorR = (UI08)((float)TempColor * (float)MColorR / 64.0f);
		TempColorB = (UI08)((float)TempColor * (float)MColorG / 64.0f);
		TempColorG = (UI08)((float)TempColor * (float)MColorB / 64.0f);

		TempColor = 0;
		TempColorErrorVal = 100000000;
		for(TempCount1 = 0; TempCount1 < 256; TempCount1++)
		{
			TempColorErrorVal1 = (abs((SI32)TempColorR - (SI32)BasePalette[(TempCount1 * 3)]))
				+ (abs((SI32)TempColorG - (SI32)BasePalette[(TempCount1 * 3) + 1]))
				+ (abs((SI32)TempColorB - (SI32)BasePalette[(TempCount1 * 3) + 2]));

			if(TempColorErrorVal1 < TempColorErrorVal)
			{
				TempColor         = (UI08)TempCount1;
				TempColorErrorVal = TempColorErrorVal1;
			}
		}

		lpPalette[ID].Palette[TempCount] = TempColor;
	}

	lpPalette[ID].fUse = TRUE;
	lpPalette[ID].ID   = ID;

	return ID;
}

//========================================================
// OnlinePaletteManager::FreePalette() Free Palette 
void OnlinePaletteManager::FreePalette(SI16 PaletteID)
{
	UI16	TempCount;

	for(TempCount = 0; TempCount < PaletteMaxCount; TempCount++)
	{
		if(lpPalette[TempCount].fUse)
		{
			if(lpPalette[TempCount].ID == PaletteID)
			{
				ZeroMemory(&lpPalette[TempCount], sizeof(OnlinePaletteData));
			}
		}
	}
}

//========================================================
// OnlinePaletteManager::GetPalette() Get Palette 
UI08* OnlinePaletteManager::GetPalette(SI16 PaletteID)
{
	UI16	TempCount;

	for(TempCount = 0; TempCount < PaletteMaxCount; TempCount++)
	{
		if(lpPalette[TempCount].fUse)
		{
			if(lpPalette[TempCount].ID == PaletteID)
			{
				return (UI08*)lpPalette[TempCount].Palette;
			}
		}
	}

	return NULL;
}

//========================================================
// OnlinePaletteManager::FindEmptyBuffer() 빈 Buffer 검색
SI16 OnlinePaletteManager::FindEmptyBuffer(void)
{
	SI16	TempCount;

	for(TempCount = 0; TempCount < PaletteMaxCount; TempCount++)
	{
		if(lpPalette[TempCount].fUse == FALSE)
			 return TempCount;
	}

	return -1;
}
