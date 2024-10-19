/************************************************************************************************
	だ橾貲 : clNumberRender.cpp
	氬渡濠 : 檜彌戮

	檜嘐雖 璋濠蒂 轎溘ж朝 Class
************************************************************************************************/
#include <GSL.h>

#include	<clGame.h>

#include "clNumberRender.h"


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Constructor
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
clNumberRender::clNumberRender()
{
	ZeroMemory(m_NumberRenderData, sizeof(SNumberRenderData) * MAXNUMBER);

	m_lpDestSurface = NULL;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Destroyer
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
clNumberRender::~clNumberRender()
{
	Release();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Create
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::Create(void)
{
	ZeroMemory(m_NumberRenderData, sizeof(SNumberRenderData) * MAXNUMBER);

	clGrp.LoadXspr("Online\\GameSpr\\clNumber.Spr", m_NumberImage);

	clGrp.CreateOffScreen(m_lpDestSurface, m_NumberImage.Header.Xsize * 20, m_NumberImage.Header.Ysize, TRUE, FALSE);
	clGrp.ClearSurface(m_lpDestSurface);

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Release
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void clNumberRender::Release(void)
{
	ZeroMemory(m_NumberRenderData, sizeof(SNumberRenderData) * MAXNUMBER);

	if(m_NumberImage.Image)
	{
		clGrp.FreeXspr(m_NumberImage);
		m_NumberImage.Image = NULL;
	}

	if(m_lpDestSurface)
	{
		m_lpDestSurface->Release();
		m_lpDestSurface = NULL;
	}
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Clear All Number
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
void clNumberRender::ClearAllNumber(void)
{
	ZeroMemory(m_NumberRenderData, sizeof(SNumberRenderData) * MAXNUMBER);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Add Number
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, char* lpNumberString, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* lpDesX, SI32* lpDesY))
{
	SI32	TempCount;

	for(TempCount = 0; TempCount < MAXNUMBER; TempCount++)
	{
		if(m_NumberRenderData[TempCount].m_Flag == FALSE)
		{
			ZeroMemory(&m_NumberRenderData[TempCount], sizeof(SNumberRenderData));
			m_NumberRenderData[TempCount].m_Flag          = TRUE;
			m_NumberRenderData[TempCount].m_Type          = Type;
			m_NumberRenderData[TempCount].m_Frame         = 0;
			m_NumberRenderData[TempCount].m_PosX          = x;
			m_NumberRenderData[TempCount].m_PosY          = y;
			m_NumberRenderData[TempCount].m_Scale         = Scale;
			strcpy((char*)m_NumberRenderData[TempCount].m_NumberString, lpNumberString);
			m_NumberRenderData[TempCount].m_ConvertCodFuc = ConvertCodFuc;

			return TRUE;
		}
	}

	return FALSE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Add Number
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, SI32 Number, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* lpDesX, SI32* lpDesY))
{
	char	TempBuffer[32];

	sprintf((char*)TempBuffer, "%d", Number);
	return AddNumber(Type, x, y, Scale, (char*)TempBuffer, ConvertCodFuc);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Add Number
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, __int64 Number, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* lpDesX, SI32* lpDesY))
{
	char	TempBuffer[32];

	sprintf((char*)TempBuffer, "I64%d", Number);
	return AddNumber(Type, x, y, Scale, (char*)TempBuffer, ConvertCodFuc);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Prepare
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::Prepare(void)
{
	SI32	TempCount;

	for(TempCount = 0; TempCount < MAXNUMBER; TempCount++)
	{
		if(m_NumberRenderData[TempCount].m_Flag)
		{
			m_NumberRenderData[TempCount].m_Frame++;
			switch(m_NumberRenderData[TempCount].m_Type)
			{
				case NUMBERTYPE_UP:
					if(m_NumberRenderData[TempCount].m_Frame > 32)
					{
						m_NumberRenderData[TempCount].m_Flag = FALSE;
					}
					break;

				case NUMBERTYPE_DEMAGE:
					if(m_NumberRenderData[TempCount].m_Frame > 6)
					{
						m_NumberRenderData[TempCount].m_Flag = FALSE;
					}
					break;

				default:
					m_NumberRenderData[TempCount].m_Flag = FALSE;
					break;
			}
		}
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Render
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::Render(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if(m_lpDestSurface == NULL) return FALSE;

	SI32	TempCount;
	SI32	TempLen;
	SI32	TempCount1;
	SI32	TempNumber;
	SI32	TempXPos;
	SI32	TempYPos;

	for(TempCount = 0; TempCount < MAXNUMBER; TempCount++)
	{
		if(m_NumberRenderData[TempCount].m_Flag)
		{
			// 檜嘐雖 薯濛

			if(clGrp.LockSurface(m_lpDestSurface) == TRUE)
			{
				clGrp.FillBox(0, 0, m_NumberImage.Header.Xsize * 20-1, m_NumberImage.Header.Ysize-1, TRANSCOLOR);

				TempLen = strlen((char*)m_NumberRenderData[TempCount].m_NumberString);
				for(TempCount1 = 0; TempCount1 < TempLen; TempCount1++)
				{
					TempNumber = m_NumberRenderData[TempCount].m_NumberString[TempCount1] - '0';
					clGrp.PutSpriteT(TempCount1 * m_NumberImage.Header.Xsize, 0, m_NumberImage.Header.Xsize, m_NumberImage.Header.Ysize, &m_NumberImage.Image[m_NumberImage.Header.Start[TempNumber]]);
				}
				clGrp.UnlockSurface(m_lpDestSurface);		
			}

			TempXPos = m_NumberRenderData[TempCount].m_PosX;
			TempYPos = m_NumberRenderData[TempCount].m_PosY;
			if(m_NumberRenderData[TempCount].m_ConvertCodFuc)
			{
				m_NumberRenderData[TempCount].m_ConvertCodFuc(m_NumberRenderData[TempCount].m_PosX, m_NumberRenderData[TempCount].m_PosY, &TempXPos, &TempYPos);
			}
			switch(m_NumberRenderData[TempCount].m_Type)
			{
				case NUMBERTYPE_UP:
					if(m_NumberRenderData[TempCount].m_Frame < 8)
					{
						// 還橫萄朝 婁薑
						RenderSurface(lpSurface, m_lpDestSurface, TempXPos, TempYPos, TempLen * m_NumberImage.Header.Xsize, (1.0f + ((8 - m_NumberRenderData[TempCount].m_Frame) * 0.2f)) * m_NumberRenderData[TempCount].m_Scale);
					}
					else
					{
						// 螢塭陛朝 婁薑
						RenderSurface(lpSurface, m_lpDestSurface, TempXPos, TempYPos - ((m_NumberRenderData[TempCount].m_Frame - 8) * 3), TempLen * m_NumberImage.Header.Xsize, m_NumberRenderData[TempCount].m_Scale);
					}
					break;

				case NUMBERTYPE_DEMAGE:
					if(m_NumberRenderData[TempCount].m_Frame < 2)
					{
						// 棺橫釭朝 婁薑
						RenderSurface(lpSurface, m_lpDestSurface, TempXPos, TempYPos, TempLen * m_NumberImage.Header.Xsize, (0.5f + ((float)m_NumberRenderData[TempCount].m_Frame * 0.5f)) * m_NumberRenderData[TempCount].m_Scale);
					}
					else
					{
						// 陛虜�� 氈朝 婁薑
						RenderSurface(lpSurface, m_lpDestSurface, TempXPos, TempYPos, TempLen * m_NumberImage.Header.Xsize, (1.5f) * m_NumberRenderData[TempCount].m_Scale);
					}
					break;

				default:
					m_NumberRenderData[TempCount].m_Flag = FALSE;
					break;
			}
		}
	}

	return TRUE;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	撲貲	:	Render Surface
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
BOOL clNumberRender::RenderSurface(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAWSURFACE7 lpSrcSurface, SI32 x, SI32 y, SI32 Size, float Scale)
{
	RECT		TempSrcRect;
	RECT		TempDestRect;
	RECT		TempTestRect;
	SI32		TempWidth;
	SI32		TempHeight;
	float		TempPixelPerRenderWidth;
	float		TempPixelPerRenderHeight;

	TempPixelPerRenderWidth  = Scale;
	TempPixelPerRenderHeight = Scale;

	TempWidth  = Size;
	TempHeight = m_NumberImage.Header.Ysize;
	TempDestRect.left   = 0;
	TempDestRect.top    = 0;
	TempDestRect.right  = 800;
	TempDestRect.bottom = 600;
	TempSrcRect.left    = 0;
	TempSrcRect.top     = 0;
	TempSrcRect.right   = TempWidth;
	TempSrcRect.bottom  = TempHeight;
	TempTestRect.left   = x - (SI32)((float)(TempWidth  / 2) * Scale);
	TempTestRect.top    = y - (SI32)((float)(TempHeight / 2) * Scale);
	TempTestRect.right  = x + (SI32)((float)(TempWidth  / 2) * Scale);
	TempTestRect.bottom = y + (SI32)((float)(TempHeight / 2) * Scale);

	if(TempTestRect.left   >= TempDestRect.right)  return TRUE;
	if(TempTestRect.right  <= TempDestRect.left)   return TRUE;
	if(TempTestRect.top    >= TempDestRect.bottom) return TRUE;
	if(TempTestRect.bottom <= TempDestRect.top)    return TRUE;

	if(TempTestRect.left < TempDestRect.left)
	{
		TempSrcRect.left  = TempSrcRect.left + ((TempDestRect.left - TempTestRect.left) * TempPixelPerRenderWidth);
		TempTestRect.left = TempDestRect.left;
	}
	if(TempTestRect.right > TempDestRect.right)
	{
		TempSrcRect.right  = TempSrcRect.right + ((TempDestRect.right - TempTestRect.right) * TempPixelPerRenderWidth);
		TempTestRect.right = TempDestRect.right;
	}
	if(TempTestRect.top < TempDestRect.top)
	{
		TempSrcRect.top  = TempSrcRect.top + ((TempDestRect.top - TempTestRect.top) * TempPixelPerRenderHeight);
		TempTestRect.top = TempDestRect.top;
	}
	if(TempTestRect.bottom > TempDestRect.bottom)
	{
		TempSrcRect.bottom  = TempSrcRect.bottom + ((TempDestRect.bottom - TempTestRect.bottom) * TempPixelPerRenderHeight);
		TempTestRect.bottom = TempDestRect.bottom;
	}

	lpSurface->Blt(&TempTestRect, lpSrcSurface, &TempSrcRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	return TRUE;
}
