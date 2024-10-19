/************************************************************************************************
	파일명 : MakePalette.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	___MAKE_PALETTE___
#define ___MAKE_PALETTE___


typedef struct
{
	BOOL	fUse;
	SI16	ID;
	UI08	Palette[256];
} OnlinePaletteData;


class OnlinePaletteManager
{
public:
	UI08					BasePalette[768];

	OnlinePaletteData*		lpPalette;
	UI16					PaletteMaxCount;

public:
	OnlinePaletteManager();																		// Constructor
	virtual ~OnlinePaletteManager();															// Destroyer

	BOOL		Init(UI08* lpBasePalette, UI16 PaletteBufferCount);								// Init
	void		Free(void);																		// Free

	SI16		MakePalette(COLORREF MakeColorType);											// Make Palette 
	void		FreePalette(SI16 PaletteID);													// Free Palette 
	UI08*		GetPalette(SI16 PaletteID);														// Get Palette 

	SI16		FindEmptyBuffer(void);															// 빈 Buffer 검색
};


#endif
