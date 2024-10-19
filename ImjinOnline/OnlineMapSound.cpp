// 2004/06/16 Created by ImJunhyug

#include <GSL.h>
#include "OnlineMapSound.h"

OnlineMapSound::OnlineMapSound( void )
:m_pSoundMap(NULL),m_iWidth(0),m_iHeight(0),m_iPrecision(0)
{
}

OnlineMapSound::~OnlineMapSound()
{
	Release();
}

void OnlineMapSound::Create(int iWidth, int iHeight, int iPrecision, SM_ATTRIB defaultAttrib)
{
	int i;
	int allocSize;

	Release();

	m_iPrecision = iPrecision;
	m_iWidth = ( iWidth + m_iPrecision / 2 ) / m_iPrecision;
	m_iHeight = ( iHeight + m_iPrecision / 2 ) / m_iPrecision;

	allocSize = m_iWidth * m_iHeight;
	ASSERT(allocSize);
	m_pSoundMap = new SM_ATTRIB[allocSize];
	ASSERT(m_pSoundMap);

	for( i = 0; i < allocSize; i++ )
	{
		m_pSoundMap[i] = defaultAttrib;
	}
}

void OnlineMapSound::Release(void)
{
	if(m_pSoundMap!=NULL)
	{
		delete m_pSoundMap;
		m_pSoundMap = NULL;
	}
}

bool OnlineMapSound::AddSoundArea(int boundNumber, SM_POINT *mapBound, SM_POINT innerPos, SM_ATTRIB attrib)
{
	SM_POINT startPos, endPos;
	int dVlaue = 4;
	int i;
/*
	startPos.x = 0;
	startPos.y = 0;
	endPos.x = 100;
	endPos.y = 0;
	LinePP(startPos, endPos, attrib | SMA_TOUCH);
	startPos.x = 0;
	startPos.y = 0;
	endPos.x = 0;
	endPos.y = 100;
	LinePP(startPos, endPos, attrib | SMA_TOUCH);
	for(i = 0; i < 200; i += 10 )
	{
		startPos.x = 0;
		startPos.y = 0;
		endPos.x = i;
		endPos.y = 100;
		LinePP(startPos, endPos, attrib | SMA_TOUCH);
	}
	return(true);
*/

	if( boundNumber < 2 ) return(false);

	attrib |= SMA_TOUCH;

	startPos.x = ( mapBound[boundNumber-1].x + m_iPrecision / 2 ) / m_iPrecision;
	startPos.y = ( mapBound[boundNumber-1].y + m_iPrecision / 2 ) / m_iPrecision;

	for( i = 0; i < boundNumber; i++ )
	{
		endPos.x = ( mapBound[i].x + m_iPrecision / 2 ) / m_iPrecision;
		endPos.y = ( mapBound[i].y + m_iPrecision / 2 ) / m_iPrecision;
		LinePP(startPos, endPos, attrib);
		TRACE("(%d, %d)->(%d, %d)\n", startPos, endPos);
		startPos = endPos;
	}

	startPos.x = ( innerPos.x + m_iPrecision / 2 ) / m_iPrecision;
	startPos.y = ( innerPos.y + m_iPrecision / 2 ) / m_iPrecision;
	FillPP(startPos, attrib);

	return(true);
}

bool OnlineMapSound::IsCreate(void)
{
	return((m_pSoundMap==NULL)? false : true);
}

void OnlineMapSound::SetAttribPP(SM_POINT pos, SM_ATTRIB attrib)
{
	if(InMapRange(pos)==false)
		return;

	m_pSoundMap[pos.x + pos.y * m_iWidth] = attrib;
}

void OnlineMapSound::SetAttribPPInversePos(SM_POINT inversePos, SM_ATTRIB attrib)
{
	SM_POINT pos;
	pos.x = inversePos.y;
	pos.y = inversePos.x;

	if(InMapRange(pos)==false)
		return;

	m_pSoundMap[pos.x + pos.y * m_iWidth] = attrib;
}


bool OnlineMapSound::InMapRange(SM_POINT pos)
{
	if((pos.x >= 0) && (pos.x < m_iWidth) && (pos.y >= 0) && (pos.y < m_iHeight)) return(true);

	return(false);
}

SM_ATTRIB OnlineMapSound::GetAttribPP(SM_POINT pos)
{
	if(InMapRange(pos)==false)
		return(SMA_NONE);

	return(m_pSoundMap[pos.x + pos.y * m_iWidth]);
}

void OnlineMapSound::LinePP(SM_POINT a, SM_POINT b, SM_ATTRIB attrib)
{
	SM_ATTRIB curAttrib;
	int i;
	int steep = 1;
	int sx, sy;  /* step positive or negative (1 or -1) */
	int dx, dy;  /* delta (difference in X and Y between points) */
	int e;
	int tmpswap;
	SM_POINT setPos;
	
	
#define SWAP(a,b) tmpswap = a; a = b; b = tmpswap;
	
	/*
	
	  * optimize for vertical and horizontal lines here
	  
	*/
	
	dx = abs(b.x - a.x);
	sx = ((b.x - a.x) > 0) ? 1 : -1;
	dy = abs(b.y - a.y);
	sy = ((b.y - a.y) > 0) ? 1 : -1;
	
	if (dy > dx)
	   {
		steep = 0;
		SWAP(a.x, a.y);
		SWAP(dx, dy);
		SWAP(sx, sy);
	}
	
	e = (dy << 1) - dx;
	
	for (i = 0; i < dx; i++) 
	{		
		if(steep)
		{
			setPos = a;
		}
		else
		{
			setPos.x = a.y;
			setPos.y = a.x;
		}
		curAttrib = GetAttribPP(setPos);

		if((curAttrib & attrib)==0)
		{
			SetAttribPP(setPos,attrib);
		}
		
		while (e >= 0) {
			a.y += sy;
			e -= (dx << 1);
		}
		
		a.x += sx;
		e += (dy << 1);
	}
}

void OnlineMapSound::FillPP(SM_POINT pos, SM_ATTRIB attrib)
{
	SM_ATTRIB curAttrib;
	SM_POINT comPos, popedPos;
	SM_POINT *pMapStack = new SM_POINT[m_iWidth * m_iHeight];
	int stackIndex, i;
	SM_POINT incTable[4] = { { -1, 0}, { 1, 0}, { 0, -1}, { 0, 1} };

	ASSERT(pMapStack);

	stackIndex = 0;

	comPos = pos;
	curAttrib = GetAttribPP(comPos);

	if((curAttrib & attrib)==0)
	{
		pMapStack[stackIndex] = comPos;
		stackIndex++;
		SetAttribPP(comPos, attrib);
	}

	while(stackIndex != 0)
	{
		stackIndex--;
		popedPos = pMapStack[stackIndex];

		for( i = 0; i < 4; i++ )
		{
			comPos.x = popedPos.x + incTable[i].x;
			comPos.y = popedPos.y + incTable[i].y;

			if(InMapRange(comPos) == true)
			{
				curAttrib = GetAttribPP(comPos);

				if((curAttrib & attrib)==0)
				{
					pMapStack[stackIndex] = comPos;
					stackIndex++;
					SetAttribPP(comPos, attrib);
				}
			}
		}
	}

	delete pMapStack;
}

SM_ATTRIB OnlineMapSound::GetAttrib(SM_POINT pos)
{
	pos.x =  ( pos.x + m_iPrecision / 2 ) / m_iPrecision;
	pos.y =  ( pos.y + m_iPrecision / 2 ) / m_iPrecision;

	if(InMapRange(pos) == false) return(SMA_NONE);

	return(m_pSoundMap[pos.x + pos.y * m_iWidth]);
}

#ifdef __AFX_H__

void OnlineMapSound::TestDraw(CDC *pDC)
{
	SM_POINT index;

	for(index.y = 0; index.y < m_iHeight; index.y++)
	{
		for(index.x = 0; index.x < m_iWidth; index.x++)
		{
//			if(	GetAttribPP(index) & SMA_TOUCH )
			{
				if(	GetAttribPP(index) & SMA_KOREA )
				{
					pDC->SetPixel(index.x, index.y, RGB(255,0,0));
				}
				if(	GetAttribPP(index) & SMA_JAPAN )
				{
					pDC->SetPixel(index.x, index.y, RGB(0,255,0));
				}
				if(	GetAttribPP(index) & SMA_TAIWAN )
				{
					pDC->SetPixel(index.x, index.y, RGB(0,0,255));
				}
				if(	GetAttribPP(index) & SMA_CHINA )
				{
					pDC->SetPixel(index.x, index.y, RGB(0,255,255));
				}
			}
		}
	}
}

#endif