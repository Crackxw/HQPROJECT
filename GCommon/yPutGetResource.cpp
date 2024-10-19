//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>
#include <main.h>

#include <myfile.h>
#include <Etc.h>
#include <clGame.h>


#include <YputGetResource.h>
#include <MapInterface.h>

#define	MAX_PUT_GETRESOURCE			100

#define	GETRESOURCE_ACTION_FRAME	100


extern _clGame		*pGame;
extern	cltMapInterface			clifMap;							// 맵관련 인터페이스 

static	strPutGetResourceInfo	stPutGetResourceInfo[MAX_PUT_GETRESOURCE];

// 비어 있는 슬롯을 찾는다.
static	SI16	FindEmptyGetResourceSlot();

//------------------------------------------------------------------------------------------------
//	설명	:	얻은 자원을 출력하는데 사용되는 정보를 초기화 한다.
//------------------------------------------------------------------------------------------------
VOID	InitGetResource()
{	
	ZeroMemory(stPutGetResourceInfo, sizeof(stPutGetResourceInfo));
}

//------------------------------------------------------------------------------------------------
//	설명			:	얻은 자원을 설정한다.
//	siAbsoluteX		:	출력할 절대 좌표.
//	siAbsoluteY		:	출력할 절대 좌표.
//	siMoney			:	돈.
//	siTree			:	나무.
//	siBow1~4		:	화살 4 종류.
//------------------------------------------------------------------------------------------------
VOID	SetGetResource(SI16 siAbsoluteX, SI16 siAbsoluteY, SI16 siMoney, SI16 siTree, SI16 siBow1, SI16 siBow2, SI16 siBow3, SI16 siBow4, const char* string)
{
	SI16	siFindEmptySlot;
	SI16	siIndex = 0;

	siFindEmptySlot	=	FindEmptyGetResourceSlot();

	if(siFindEmptySlot)
	{
		ZeroMemory(stPutGetResourceInfo, sizeof(stPutGetResourceInfo));

		stPutGetResourceInfo[siFindEmptySlot].siActionAbsoluteX	=	siAbsoluteX;
		stPutGetResourceInfo[siFindEmptySlot].siActionAbsoluteY	=	siAbsoluteY;
		stPutGetResourceInfo[siFindEmptySlot].siAbsoluteX	=	siAbsoluteX;		
		stPutGetResourceInfo[siFindEmptySlot].siAbsoluteY	=	siAbsoluteY;				
		strcpy(stPutGetResourceInfo[siFindEmptySlot].String, string);				
		

		if(siMoney || siTree || siBow1 || siBow2 || siBow3 || siBow4)
		{
			stPutGetResourceInfo[siFindEmptySlot].stResourceInfo[siIndex].siFont		=	1;
			stPutGetResourceInfo[siFindEmptySlot].stResourceInfo[siIndex].siValue		=	siMoney;						
			stPutGetResourceInfo[siFindEmptySlot].stResourceInfo[siIndex].siAbsoluteX	=	siAbsoluteX;
			stPutGetResourceInfo[siFindEmptySlot].stResourceInfo[siIndex].siAbsoluteDX	=	siAbsoluteX;
			siIndex++;
		}

		stPutGetResourceInfo[siFindEmptySlot].bUse	=	TRUE;
	}		
}

//------------------------------------------------------------------------------------------------
//	설명	:	얻은 자원을 화면에 그려준다.
//------------------------------------------------------------------------------------------------
VOID	DrawGetResource(HDC hdc)
{
	SI16	i, j;
	SHORT	siScreenX, siScreenY;	
	SI16	siFont;	
	SIZE	size;
	char buffer[128];


	for(i = 1; i < MAX_PUT_GETRESOURCE; i++)
	{
		if(stPutGetResourceInfo[i].bUse)
		{
			// 곡물을 찍는다.
			for(j = 0; j < 6; j++)
			{
				if(stPutGetResourceInfo[i].stResourceInfo[j].siValue && 
					abs(stPutGetResourceInfo[i].siAbsoluteY - stPutGetResourceInfo[i].siActionAbsoluteY) > (j * 20))
				{
					siFont	=	stPutGetResourceInfo[i].stResourceInfo[j].siFont;			
					
					if(stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX ==
						stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteDX)
					{
						stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteDX	=	
							stPutGetResourceInfo[i].siAbsoluteX + (rand() % 10 - 5);
					}
					else
					{
						if(stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX < 
								stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteDX)
						{
							stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX++;
						}
						else if(stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX > 
									stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteDX)
						{
							stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX--;
						}
					}				
					
					clifMap.TransFromRealDotToScreenDot(stPutGetResourceInfo[i].stResourceInfo[j].siAbsoluteX, 
												stPutGetResourceInfo[i].siActionAbsoluteY,
												siScreenX, siScreenY);										

					// 적절한 폰트를 선택한다.
					SelectObject(hdc, pGame->pBattle->GetExpFont);
					SetBkMode(hdc, TRANSPARENT);		
					
					// 찍을 문자열을 정리한다. 
					if(stPutGetResourceInfo[i].String)
					{
						wsprintf(buffer, "%s +%d", stPutGetResourceInfo[i].String, stPutGetResourceInfo[i].stResourceInfo[j].siValue);
					}
					else
					{
						wsprintf(buffer, "+%d", stPutGetResourceInfo[i].stResourceInfo[j].siValue);
					}

					// 문자열의 사이즈를 구한다. 
					GetTextExtentPoint32(hdc, buffer, strlen(buffer), &size);				// 타이틀 글꼴의 크기를 알아낸다.

					// 찍을 위치를 정한다. 
					SI32 x	= siScreenX - size.cx/2;
					SI32 y	= siScreenY + (j * 20) + 5;
	
					SetTextColor(hdc, RGB(0, 0, 0));
					TextOut(hdc, x+1, y+1, buffer, lstrlen(buffer));

					// 실제로 텍스트를 찍는다. 
					SetTextColor(hdc, RGB(0, 200, 0));
					TextOut(hdc, x, y, buffer, lstrlen(buffer));

					

				}
			}
			
			// 위로 올라가게 한다.
			stPutGetResourceInfo[i].siActionAbsoluteY -= 2;

			if(abs(stPutGetResourceInfo[i].siAbsoluteY - stPutGetResourceInfo[i].siActionAbsoluteY) >= GETRESOURCE_ACTION_FRAME)			
			{
				// 활동을 멈춘다.
				stPutGetResourceInfo[i].bUse	=	FALSE;
			}					
		}
	}
}

//------------------------------------------------------------------------------------------------
//	설명	:	비어 있는 슬롯을 찾는다.
//	리턴	:	비어 있는 슬롯.
//------------------------------------------------------------------------------------------------
SI16	FindEmptyGetResourceSlot()
{
	SI16	i;

	for(i = 1; i < MAX_PUT_GETRESOURCE; i++)
	{
		if(!stPutGetResourceInfo[i].bUse)
			return i;
	}

	return 0;
}

