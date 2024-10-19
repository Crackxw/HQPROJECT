#ifndef _DIRECTX_H
#define _DIRECTX_H


SI32 GetTextFormat(char *nation, char *name, char *kind);
BOOL GetTextFormatFont(char *nation, char *name, char *font);

BOOL CreateSurface();
// 임의로 만든 서피스를 모두 해제한다. 
void CustomSurfaceRelease();
void UpdateScreen();
void SetGamePalette(unsigned char *pal);

// 게임에 사용될 팔레트를 불러온다. 
void LoadPalette();

// 흰색을 바꿔준다. 
void InitFadeOut();
BOOL FadeOut();
void InitFadeIn();
BOOL FadeIn();

// Append to hojae	2001.11.09
UI08*			Return_Image(SI32 file);
XSPR_HEADER*	Return_Header(SI32 file);		// add actdoll(021229)
SI32			GetFileXsize(SI32 file);
SI32			GetFileYsize(SI32 file);

void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 file, SI32 font);
void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 file, SI32 font);

void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);

void Draw_PutSpriteShadowT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteShadowCT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteShadowRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteShadowCRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable);

void Draw_PutSpriteLightImjinT(SI32 startx, SI32 starty, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinCT(SI32 startx, SI32 starty, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinRT(SI32 startx, SI32 starty, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinCRT(SI32 startx, SI32 starty, SI32 file, SI32 font);


void Draw_PutImage32(SI32 startx, SI32 starty, SI32 file, SI32 font);
void Draw_PutImage32(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);

void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);

void Draw_PutSpriteT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);

void Draw_PutSpriteLightT(SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightCT(SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightRT(SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightCRT(SI32 xsize, SI32 ysize, SI32 file, SI32 font);

void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);

void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 file, SI32 font, unsigned char* ctable, SHORT step);

void Draw_PutSpriteLightT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step);
void Draw_PutSpriteLightCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable, SHORT step);

void Draw_PutSpriteDotT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteDotCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteDotRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);
void Draw_PutSpriteDotCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char* ctable);

void Draw_PutSpriteLightImjinT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);
void Draw_PutSpriteLightImjinCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font);

void Draw_PutSpriteWaterShadowT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256]);
void Draw_PutSpriteWaterShadowCT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256]);
void Draw_PutSpriteWaterShadowRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256]);
void Draw_PutSpriteWaterShadowCRT(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, SI32 file, SI32 font, unsigned char *ctable, unsigned char watershadowtable[][256]);

BOOL Error(char * szCaption, char*szFormat,... );

BOOL IsInClipArea(SI32 file, SI32 x, SI32 y);

#endif
