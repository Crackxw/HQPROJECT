//--------------------------------------------------------------------
//  ��� : ���°�
//--------------------------------------------------------------------
#include <GSL.h>
#include <Main.h>
#include <stdio.h>
#include "Sprg.h"

char cTable[15][15] = { {-1, -1, -1, -1, -1,  7,  7, 7,  7,  7, -1, -1, -1, -1, -1},
						{-1, -1, -1,  7,  7,  6,  6, 6,  6,  6,  7,  7, -1, -1, -1},
						{-1, -1,  7,  6,  6,  5,  5, 5,  5,  5,  6,  6,  7, -1, -1},
						{-1,  7,  6,  6,  5,  5,  4, 4,  4,  5,  5,  6,  6,  7, -1},
						{-1,  7,  6,  5,  5,  4,  3, 3,  3,  4,  5,  5,  6,  7, -1},
						{ 7,  6,  5,  5,  4,  3,  2, 2,  2,  3,  4,  5,  5,  6,  7},
						{ 7,  6,  5,  4,  3,  2,  2, 1,  2,  2,  3,  4,  5,  6,  7},
						{ 7,  6,  5,  4,  3,  2,  1, 0,  1,  2,  3,  4,  5,  6,  7},
						{ 7,  6,  5,  4,  3,  2,  2, 1,  2,  2,  3,  4,  5,  6,  7},
						{ 7,  6,  5,  5,  4,  3,  2, 2,  2,  3,  4,  5,  5,  6,  7},
						{-1,  7,  6,  5,  5,  4,  3, 3,  3,  4,  5,  5,  6,  7, -1},
						{-1,  7,  6,  6,  5,  5,  4, 4,  4,  5,  5,  6,  6,  7, -1},
						{-1, -1,  7,  6,  6,  5,  5, 5,  5,  5,  6,  6,  7, -1, -1},
						{-1, -1, -1,  7,  7,  6,  6, 6,  6,  6,  7,  7, -1, -1, -1},
						{-1, -1, -1, -1, -1,  7,  7, 7,  7,  7, -1, -1, -1, -1, -1} };

BOOL Circle(int nStep, int x, int y, int *StepCnt, POINT *pt)
{
	int index = 0;
	if(nStep >= 8) return FALSE;

	for(int j=0 ; j<15; j++)
		for(int i=0 ; i<15; i++)
		{
			if(nStep == cTable[i][j])
			{				
				pt[index].x = x + i - 7;
				pt[index].y = y + j - 7;

				index++;
			}
		}

	*StepCnt = index;

	return TRUE;
}

BOOL Circle2(int nStep, int x, int y, int ratio, POINT *pt)
{
	int nX, nY, nThres;
	int nIndex = 0;

	if(ratio == 0)
	{
		pt[nIndex].x = x;
		pt[nIndex].y = y;
		nIndex++;
		return TRUE;
	}

	nY = ratio;
	nThres = 3 - (ratio + ratio);

	for(nX = 0; nX<nY; nX++)
	{
		if(nThres < 0 )
			nThres += 6 + (nX << 2);
		else
		{
			nThres += 10 + ((nX - nY) << 2);
			nY--;
		}

		pt[nIndex].x = x+nX; pt[nIndex].y = y+nY; nIndex++;
		pt[nIndex].x = x+nY; pt[nIndex].y = y+nX; nIndex++;
		pt[nIndex].x = x-nX; pt[nIndex].y = y+nY; nIndex++;
		pt[nIndex].x = x-nY; pt[nIndex].y = y+nX; nIndex++;
		pt[nIndex].x = x+nX; pt[nIndex].y = y-nY; nIndex++;
		pt[nIndex].x = x+nY; pt[nIndex].y = y-nX; nIndex++;
		pt[nIndex].x = x-nX; pt[nIndex].y = y-nY; nIndex++;
		pt[nIndex].x = x-nY; pt[nIndex].y = y-nX; nIndex++;
	}

	return TRUE;
}

BOOL BLine(int nStep, int x1, int y1, int x2, int y2, POINT *pt)
{
	int nX, nY, nTemp;
	int nIndex = 0;
	int nDeltax, nDeltay, nHalf, nError = 0;

	if((x1 == x2 && y1 == y2) || nStep == 0) return FALSE;

// �׻� x2 >= x1, y2 > y1 �� �ǵ��� 
	if(x1 > x2)
	{
		nTemp = x1;
		x1 = x2;
		x2 = nTemp;
	}
	if(y1 > y2)
	{
		nTemp = y1;
		y1 = y2;
		y2 = nTemp;
	}

// x, y�� ��ȭ���� ���Ѵ�.
	nDeltax = x2 - x1;
	nDeltay = y2 - y1;


// �� ��ǥ�� ��￡ ���� ���� �ߴ´�.
	if(nDeltax > nDeltay)
	{
		nY = y1;
		nHalf = (int)(nDeltax / 2 );

		for(nX = x1+1; nX<=x2; nX++)
		{
			nError += nDeltay;
			if(nError > nHalf )
			{
				nY++;
				nError -= nDeltax;
			}
			// ����� �κ�
			pt[nIndex].x = nX;
			pt[nIndex].y = nY;
			nIndex++;
			if(nIndex == nStep) return TRUE;
		}
	}
	else
	{
		nX = x1;
		nHalf = (int)(nDeltay / 2);

		for(nY = y1+1; nY<=y2; nY++ )
		{
			nError += nDeltax;
			if(nError > nHalf )
			{
				nX++;
				nError -= nDeltay;
			}
			// ����� �κ�
			pt[nIndex].x = nX;
			pt[nIndex].y = nY;
			nIndex++;
			if(nIndex == nStep) return TRUE;
		}
	}
	return TRUE;
}

int WindEffect(int SDirect, int WDirect)
{
	int nReturnV = 0;

	switch(SDirect)
	{
	case NORTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 2;
			break;
		case NORTH|EAST:
			nReturnV = 1;
			break;
		case EAST:
			nReturnV = 0;
			break;
		case EAST|SOUTH:
			nReturnV = -1;
			break;
		case SOUTH:
			nReturnV = -2;
			break;
		case SOUTH|WEST:
			nReturnV = -1;
			break;
		case WEST:
			nReturnV = 0;
			break;
		case WEST|NORTH:
			nReturnV = 1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case NORTH|EAST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 1;
			break;
		case NORTH|EAST:
			nReturnV = 2;
			break;
		case EAST:
			nReturnV = 1;
			break;
		case EAST|SOUTH:
			nReturnV = 0;
			break;
		case SOUTH:
			nReturnV = -1;
			break;
		case SOUTH|WEST:
			nReturnV = -2;
			break;
		case WEST:
			nReturnV = -1;
			break;
		case WEST|NORTH:
			nReturnV = 0;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case EAST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 0;
			break;
		case NORTH|EAST:
			nReturnV = 1;
			break;
		case EAST:
			nReturnV = 2;
			break;
		case EAST|SOUTH:
			nReturnV = 1;
			break;
		case SOUTH:
			nReturnV = 0;
			break;
		case SOUTH|WEST:
			nReturnV = -1;
			break;
		case WEST:
			nReturnV = -2;
			break;
		case WEST|NORTH:
			nReturnV = -1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case EAST|SOUTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -1;
			break;
		case NORTH|EAST:
			nReturnV = 0;
			break;
		case EAST:
			nReturnV = 1;
			break;
		case EAST|SOUTH:
			nReturnV = 2;
			break;
		case SOUTH:
			nReturnV = 1;
			break;
		case SOUTH|WEST:
			nReturnV = 0;
			break;
		case WEST:
			nReturnV = -1;
			break;
		case WEST|NORTH:
			nReturnV = -2;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case SOUTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -2;
			break;
		case NORTH|EAST:
			nReturnV = -1;
			break;
		case EAST:
			nReturnV = 0;
			break;
		case EAST|SOUTH:
			nReturnV = 1;
			break;
		case SOUTH:
			nReturnV = 2;
			break;
		case SOUTH|WEST:
			nReturnV = 1;
			break;
		case WEST:
			nReturnV = 0;
			break;
		case WEST|NORTH:
			nReturnV = -1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case SOUTH|WEST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -1;
			break;
		case NORTH|EAST:
			nReturnV = -2;
			break;
		case EAST:
			nReturnV = -1;
			break;
		case EAST|SOUTH:
			nReturnV = 0;
			break;
		case SOUTH:
			nReturnV = 1;
			break;
		case SOUTH|WEST:
			nReturnV = 2;
			break;
		case WEST:
			nReturnV = 1;
			break;
		case WEST|NORTH:
			nReturnV = 0;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case WEST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 0;
			break;
		case NORTH|EAST:
			nReturnV = -1;
			break;
		case EAST:
			nReturnV = -2;
			break;
		case EAST|SOUTH:
			nReturnV = -1;
			break;
		case SOUTH:
			nReturnV = 0;
			break;
		case SOUTH|WEST:
			nReturnV = 1;
			break;
		case WEST:
			nReturnV = 2;
			break;
		case WEST|NORTH:
			nReturnV = 1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case WEST|NORTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 1;
			break;
		case NORTH|EAST:
			nReturnV = 0;
			break;
		case EAST:
			nReturnV = -1;
			break;
		case EAST|SOUTH:
			nReturnV = -2;
			break;
		case SOUTH:
			nReturnV = -1;
			break;
		case SOUTH|WEST:
			nReturnV = 0;
			break;
		case WEST:
			nReturnV = 1;
			break;
		case WEST|NORTH:
			nReturnV = 2;
			break;
		default:
			nReturnV = 0;
		}
		break;

	default:
		nReturnV = 0;
	}
	return nReturnV;
}
