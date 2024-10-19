
#include <windows.h>
#include <stdio.h>
#include "MegaDC.h"

void CMegaDC::Create(RECT &rt)
{
	char pal[256*3];

	ReadPal(pal);

	m_Rect = rt;

	m_pBitInfo = (BITMAPINFO*)new LPBITMAPINFO[ sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256) ];
	ZeroMemory( m_pBitInfo, sizeof( BYTE ) * ( sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 ) );

	m_pBitInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_pBitInfo->bmiHeader.biWidth		= ((rt.right - rt.left)+3)&~3;
	m_pBitInfo->bmiHeader.biHeight		= -rt.bottom -  rt.top;
	m_pBitInfo->bmiHeader.biBitCount	= 8;
	m_pBitInfo->bmiHeader.biCompression	= BI_RGB;
	m_pBitInfo->bmiHeader.biPlanes		= 1;

	for(int i=0 ; i<256 ; i++)
	{
		m_pBitInfo->bmiColors[i].rgbRed		= pal[i * 3 + 0] * 4 ;
		m_pBitInfo->bmiColors[i].rgbGreen	= pal[i * 3 + 1] * 4 ;
		m_pBitInfo->bmiColors[i].rgbBlue	= pal[i * 3 + 2] * 4 ;
	}

	m_MegaDC = ::CreateCompatibleDC(NULL);

	SetBkMode( m_MegaDC, TRANSPARENT );
	SetTextColor( m_MegaDC, RGB( 240, 240, 240 ) );

	m_hBmp		= ::CreateDIBSection(m_MegaDC, m_pBitInfo, DIB_RGB_COLORS, (void **)&m_pData, NULL, 0);
	m_hOldBmp	= (HBITMAP)SelectObject(m_MegaDC, m_hBmp);
}

void CMegaDC::ReadPal(char *pal)
{
	FILE *fp = fopen("\\\\220.85.18.3\\c$\\공유방\\거상\\pal\\imjin2.pal", "rb");
	fread(pal, 256, 3, fp);
	fclose(fp);
}

void CMegaDC::Free()
{
	if( m_hBmp )	DeleteObject( m_hBmp );
	if( m_MegaDC )	SelectObject( m_MegaDC, m_hOldBmp );
	if( m_MegaDC )	DeleteDC( m_MegaDC );

	if( m_pBitInfo )
	{
		delete []m_pBitInfo;
		m_pBitInfo	=	NULL;
	}

	m_hBmp = NULL;
	m_MegaDC = NULL;
}

void CMegaDC::PutSprite8CT(int x, int y, int xsize, int ysize, unsigned char* image)
{
	int i, j, index = 0;
	int up_skip = 0, down_skip = 0;
	int left_skip = 0, right_skip = 0;
	BYTE *vidptr = m_pData + y * m_pBitInfo->bmiHeader.biWidth + x;

	int ClipX1, ClipX2, ClipY1, ClipY2;

	ClipX1 = ClipY1 = 0;
	ClipX2 = m_pBitInfo->bmiHeader.biWidth;
	ClipY2 = -m_pBitInfo->bmiHeader.biHeight;


	if(x + xsize - 1 < ClipX1) return;
	if(y + ysize - 1 < ClipY1) return;
	if(x > ClipX2) return;
	if(y > ClipY2) return;

	if(y < ClipY1)         up_skip    = ClipY1 - y;
	if(y + ysize > ClipY2) down_skip  = y + ysize - ClipY2 - 1;
	if(x < ClipX1)         left_skip  = ClipX1 - x;
	if(x + xsize > ClipX2) right_skip = x + xsize - ClipX2 - 1;

	right_skip = xsize - right_skip;
	down_skip  = ysize - down_skip;
	for(i = 0; i < up_skip; i++)
	{
		j = 0;
		while(j < xsize)
		{
			if((*image) == 254)
			{
				image++;
				j += (*image) - 1;
			}

			image++;
			j++;
		}
		
		vidptr += m_pBitInfo->bmiHeader.biWidth;
	}
	for(i = up_skip; i < down_skip; i++)
	{
		j = 0;
		while(j < xsize)
		{
			if((*image) == 254)
			{
				image++;
				j += *image;
				image++;
			}
			else
			{
				if(j >= left_skip && j < right_skip)
				{
					vidptr[j] = *image;
				}
                
				j++;
				image++;
			}
		}
		
		vidptr += m_pBitInfo->bmiHeader.biWidth;
	}
}

void CMegaDC::clear( BYTE color )
{
	memset( m_pData, color, -(m_pBitInfo->bmiHeader.biWidth * m_pBitInfo->bmiHeader.biHeight) );
}

void CMegaDC::Show( HDC dc )
{
	BitBlt( dc, m_Rect.left, m_Rect.top, m_pBitInfo->bmiHeader.biWidth, -m_pBitInfo->bmiHeader.biHeight, m_MegaDC, 0, 0, SRCCOPY );
}
