#include <GSL.h>
#include <Main.h>
#include "OnlineFieldArea.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	������.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineFieldArea::OnlineFieldArea()
{
	puiNoFieldArea	=	NULL;
}	

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�Ҹ���.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
OnlineFieldArea::~OnlineFieldArea()
{
	Free();
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʱ�ȭ
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFieldArea::Init()
{	
	ZeroMemory(rcNoFieldArea, sizeof(rcNoFieldArea));

	puiNoFieldArea	=	new	UI08[clGrp.GetScreenXsize () * clGrp.GetScreenYsize()];
	memset(puiNoFieldArea, TRANSCOLOR, clGrp.GetScreenXsize () * clGrp.GetScreenYsize());
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFieldArea::Free()
{
	if(puiNoFieldArea)
	{
		delete [] puiNoFieldArea;
		puiNoFieldArea	=	NULL;
	}
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	����ִ� �ڵ��� ã�´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineFieldArea::FindEmptyHandle()
{
	SI32	i;

	for(i = 0; i < ON_MAX_NOFIELD_AREA; i++)
	{
		if((rcNoFieldArea[i].left == 0) &&
			(rcNoFieldArea[i].top == 0) &&
			(rcNoFieldArea[i].right == 0) &&
			(rcNoFieldArea[i].bottom == 0))
		{
			return i;
		}
	}

	return -1;
}


//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʵ� ������ �ƴ� �κ��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineFieldArea::SetNoFieldArea(SI16 siLeft, SI16 siTop, SI16 siRight, SI16 siBottom)
{
	SI16	siFindHandle;

	siFindHandle	=	FindEmptyHandle();

	if((siFindHandle >= 0) && (siFindHandle < ON_MAX_NOFIELD_AREA))
	{
		rcNoFieldArea[siFindHandle].left	=	siLeft;
		rcNoFieldArea[siFindHandle].top		=	siTop;
		rcNoFieldArea[siFindHandle].right	=	siRight;
		rcNoFieldArea[siFindHandle].bottom	=	siBottom;
	}

	return siFindHandle;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʵ� ������ �ƴ� �κ��� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
SI16	OnlineFieldArea::SetNoFieldArea(LPRECT pRect)
{
	SI16	siFindHandle;

	siFindHandle	=	FindEmptyHandle();

	if(siFindHandle >= 0)	
		memcpy(&rcNoFieldArea[siFindHandle], pRect, sizeof(RECT));	

	return siFindHandle;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	Ư�� �޸� ������ ����Ͽ� �����(254)�� �κ��� �ʵ� �������� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFieldArea::SetNoFieldArea(SI16 siX, SI16 siY, XSPR *pXspr)
{
	SI32	i, j;
	//SI32	siScreenXsize;
	UI08	uiIndex;
	
	if(clGrp.LockSurface(puiNoFieldArea, clGrp.GetScreenXsize(), clGrp.GetScreenYsize()) == TRUE)
	{
		clGrp.PutImageT(siX, siY, pXspr->Header.Xsize, pXspr->Header.Ysize, &pXspr->Image[0]);
		// �ý��� �޸𸮴� Unlock���� �ʴ´�.
	}

	// ����Ÿ�� ��ȯ�Ѵ�.
	for(i = 0; i < pXspr->Header.Ysize; i++)
		for(j = 0; j < pXspr->Header.Xsize; j++)
		{
			uiIndex		=	((i + siY) * clGrp.GetScreenXsize()) + (j + siX);

			if(puiNoFieldArea[uiIndex] != TRANSCOLOR) 
				puiNoFieldArea[uiIndex] = 0;
		}	

}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʵ� ������ �ƴ� �κ��� �ʵ� �������� �� �����Ѵ�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
VOID	OnlineFieldArea::SetFieldArea(SI16 siHandle)
{
	if((siHandle >= 0) && (siHandle < ON_MAX_NOFIELD_AREA))
	{
		rcNoFieldArea[siHandle].left = rcNoFieldArea[siHandle].top = rcNoFieldArea[siHandle].right = rcNoFieldArea[siHandle].bottom = 0;
	}
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʵ� �������� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
BOOL	OnlineFieldArea::IsFieldArea(SI16 siX, SI16 siY)
{
	SI32	i;
	LPRECT	pRect;
	//POINT	pt;
	
	if(siX <  0)                      return FALSE;
	if(siX >= clGrp.GetScreenXsize()) return FALSE;
	if(siY <  0)                      return FALSE;
	if(siY >= clGrp.GetScreenYsize()) return FALSE;

	if(puiNoFieldArea)
	{
		if(puiNoFieldArea[siY * clGrp.GetScreenXsize() + siX] != TRANSCOLOR)
			return FALSE;
	}

	for(i = 0; i < ON_MAX_NOFIELD_AREA; i++)
	{
		pRect		=	&rcNoFieldArea[i];
		
		if(pRect->left != 0 || pRect->top != 0 || pRect->right != 0 || pRect->bottom != 0)
		{
			if(siX >= pRect->left && siX <= pRect->right && siY >= pRect->top && siY <= pRect->bottom)
				return FALSE;
		}
	}

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������
//	����	:	�ʵ� ������ �ƴ� �κ��� ������ �ִ� ���ۿ��� Ư�� ��ġ�� ���� ���´�.
//��������������������������������������������������������������������������������������������������������������������������������������������������������������
UI08	OnlineFieldArea::GetNoFieldAreaInfo(SI16 siX, SI16 siY)
{
	return puiNoFieldArea[siY * clGrp.GetScreenXsize() + siX];
}
