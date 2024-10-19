#pragma once

class CMegaDC
{
public:
	CMegaDC()	{}
	~CMegaDC()	{Free();}

	void	Create(RECT &rt);
	void	clear( BYTE color=0 );
	void	Show( HDC dc );	


	BYTE	*GetData()		{ return m_pData; }
	HDC		GetDC()			{ return m_MegaDC; }
	int		GetWidth()		{ return m_pBitInfo->bmiHeader.biWidth; }
	int		GetHeight()		{ return -m_pBitInfo->bmiHeader.biHeight; }
	
	void	PutSprite8CT(int x, int y, int xsize, int ysize, unsigned char* image);

private:	
	void	ReadPal(char *pal);
	void	Free();

private:
	HDC			m_MegaDC;
	BITMAPINFO	*m_pBitInfo;
	HBITMAP		m_hBmp;
	HBITMAP		m_hOldBmp;
	BYTE		*m_pData;
	RECT		m_Rect;
};