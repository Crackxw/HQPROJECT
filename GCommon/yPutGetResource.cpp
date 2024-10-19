//--------------------------------------------------------------------
//  ��� : ���°�
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
extern	cltMapInterface			clifMap;							// �ʰ��� �������̽� 

static	strPutGetResourceInfo	stPutGetResourceInfo[MAX_PUT_GETRESOURCE];

// ��� �ִ� ������ ã�´�.
static	SI16	FindEmptyGetResourceSlot();

//------------------------------------------------------------------------------------------------
//	����	:	���� �ڿ��� ����ϴµ� ���Ǵ� ������ �ʱ�ȭ �Ѵ�.
//------------------------------------------------------------------------------------------------
VOID	InitGetResource()
{	
	ZeroMemory(stPutGetResourceInfo, sizeof(stPutGetResourceInfo));
}

//------------------------------------------------------------------------------------------------
//	����			:	���� �ڿ��� �����Ѵ�.
//	siAbsoluteX		:	����� ���� ��ǥ.
//	siAbsoluteY		:	����� ���� ��ǥ.
//	siMoney			:	��.
//	siTree			:	����.
//	siBow1~4		:	ȭ�� 4 ����.
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
//	����	:	���� �ڿ��� ȭ�鿡 �׷��ش�.
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
			// ��� ��´�.
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

					// ������ ��Ʈ�� �����Ѵ�.
					SelectObject(hdc, pGame->pBattle->GetExpFont);
					SetBkMode(hdc, TRANSPARENT);		
					
					// ���� ���ڿ��� �����Ѵ�. 
					if(stPutGetResourceInfo[i].String)
					{
						wsprintf(buffer, "%s +%d", stPutGetResourceInfo[i].String, stPutGetResourceInfo[i].stResourceInfo[j].siValue);
					}
					else
					{
						wsprintf(buffer, "+%d", stPutGetResourceInfo[i].stResourceInfo[j].siValue);
					}

					// ���ڿ��� ����� ���Ѵ�. 
					GetTextExtentPoint32(hdc, buffer, strlen(buffer), &size);				// Ÿ��Ʋ �۲��� ũ�⸦ �˾Ƴ���.

					// ���� ��ġ�� ���Ѵ�. 
					SI32 x	= siScreenX - size.cx/2;
					SI32 y	= siScreenY + (j * 20) + 5;
	
					SetTextColor(hdc, RGB(0, 0, 0));
					TextOut(hdc, x+1, y+1, buffer, lstrlen(buffer));

					// ������ �ؽ�Ʈ�� ��´�. 
					SetTextColor(hdc, RGB(0, 200, 0));
					TextOut(hdc, x, y, buffer, lstrlen(buffer));

					

				}
			}
			
			// ���� �ö󰡰� �Ѵ�.
			stPutGetResourceInfo[i].siActionAbsoluteY -= 2;

			if(abs(stPutGetResourceInfo[i].siAbsoluteY - stPutGetResourceInfo[i].siActionAbsoluteY) >= GETRESOURCE_ACTION_FRAME)			
			{
				// Ȱ���� �����.
				stPutGetResourceInfo[i].bUse	=	FALSE;
			}					
		}
	}
}

//------------------------------------------------------------------------------------------------
//	����	:	��� �ִ� ������ ã�´�.
//	����	:	��� �ִ� ����.
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

