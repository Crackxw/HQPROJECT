//--------------------------------------------------------------------
//  담당 : 김태곤
//--------------------------------------------------------------------
#include <GSL.h>

#include <main.h>
#include <clGame.h>

#include "..\Gersang\rain.h"
#include "..\Gersang\maindata.h"

#include <capture.h>

extern _clGame* pGame;

// 캐릭터 그림용
extern LPDIRECTDRAWSURFACE7 lpScreenBuffer;

extern LPDIRECTDRAWSURFACE7 lpCaptureSurface;

extern int WindowModeSwitch;


_clCapture::_clCapture()
{


}

// 화면을 캡춰한다. 
BOOL _clCapture::ScreenCapture()
{
	SHORT i, j;
	char buffer[FILE_NAME_SIZE];
	FILE* fp;
	static int a=0;
	
	// 화일 이름을 결정한다. 
	while(1)
	{
		wsprintf(buffer, "syw%04d.bmp", ScreenNumber);
		
		ScreenNumber++;
		
		// 같은 이름의 화일이 있는지 확인한다.
		fp=fopen(buffer, "rb");
		
		if(fp==NULL)	break;
		fclose(fp);
	}


	BITMAPIMAGE bm;
	ZeroMemory(&bm, sizeof(bm));

	bm.InfoHeader.biWidth  = clGrp.GetScreenXsize();
	bm.InfoHeader.biHeight = clGrp.GetScreenYsize();
	bm.InfoHeader.biBitCount=24;
	bm.Image=new unsigned char [clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3];

	if (WindowModeSwitch)
	{
        if (lpCaptureSurface != NULL) {
		if(clGrp.LockSurface(lpCaptureSurface)==TRUE)
		{
			
			int index=0;
			UI08 color;
			for(i=0;i<clGrp.GetScreenYsize();i++)
				for(j=0;j<clGrp.GetScreenXsize();j++)
				{
					color = clGrp.Screen8[(clGrp.GetScreenXsize()-j) + i * clGrp.GetSurfaceWidth() ];
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peRed; 
					index++;
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peGreen;
					index++;
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peBlue;
					index++;
				}
				
				clGrp.UnlockSurface(lpCaptureSurface);
		}

		}
		
	}
	else
	{
		if(clGrp.LockSurface(SURFACE_PRIMARY)==TRUE)
		{
			int		index = 0;
			UI08	color;
			for(i=0;i<clGrp.GetScreenYsize();i++)
				for(j=0;j<clGrp.GetScreenXsize();j++)
				{
					color = clGrp.Screen8[(clGrp.GetScreenXsize()-j) + i * clGrp.GetSurfaceWidth() ];
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peRed; 
					index++;
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peGreen;
					index++;
					
					bm.Image[clGrp.GetScreenXsize()*clGrp.GetScreenYsize()*3-1-index] = clGrp.PalEntries[color].peBlue;
					index++;
				}
				
				clGrp.UnlockSurface(SURFACE_PRIMARY);
		}
	}

	SaveBIT24(bm, buffer);
	delete[] bm.Image;

	return TRUE;
}


// 지도를  캡춰한다. 
BOOL _clCapture::MapCapture(unsigned char* imagebuffer, int xsize, int ysize)
{
	char buffer[FILE_NAME_SIZE];

	wsprintf(buffer, "MAP.pcx");

	PCX pcx;
	pcx.Head.hres=xsize;
	pcx.Head.vres=ysize;
	pcx.Head.xmax=xsize-1;
    pcx.Head.ymax=ysize-1;
	pcx.Head.bpl     = xsize;
    pcx.Head.shres   = xsize;
    pcx.Head.svres   = ysize;

	pcx.Image=imagebuffer;
    clGrp.SavePCX(pcx, buffer);

		return TRUE;
}

int _clCapture::SaveBIT24(BITMAPIMAGE &bit, char* filename)
{
	FILE* bitmapfile;
//	int i, j;

	bitmapfile = fopen(filename, "wb");
	if(bitmapfile==NULL) 
	{
		MessageBox(pGame->Hwnd, "파일이 없습니다.(Cannot Find Files!)", "", MB_OK);
		return FALSE;
	}

	bit.InfoHeader.biSizeImage = bit.InfoHeader.biWidth * bit.InfoHeader.biHeight * 3;

	bit.Header.bfType = 19778;
	bit.Header.bfOffBits =54;
	bit.Header.bfSize = bit.InfoHeader.biSizeImage + bit.Header.bfOffBits;
	bit.Header.bfReserved1 = 0;
	bit.Header.bfReserved2 = 0;

	bit.InfoHeader.biSize = 40;
	bit.InfoHeader.biPlanes = 1;
	//bit.InfoHeader.biBitCount = 24;
	bit.InfoHeader.biCompression = 0;
	bit.InfoHeader.biXPelsPerMeter = 2834;
	bit.InfoHeader.biYPelsPerMeter = 2834;
	bit.InfoHeader.biClrUsed = 0;
	bit.InfoHeader.biClrImportant = 0;

	fwrite(&bit.Header, sizeof(BITMAPFILEHEADER), 1, bitmapfile);
	fwrite(&bit.InfoHeader, sizeof(BITMAPINFOHEADER), 1, bitmapfile);
	fwrite(bit.Image, bit.InfoHeader.biSizeImage, 1, bitmapfile);

	if(bit.Image != NULL)
	{
		delete [] bit.Image;
		bit.Image = NULL;
	}

	fclose(bitmapfile);
	return TRUE;
}


