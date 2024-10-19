#include <GSL.h>
#include <MiniGame1.h>
#include <clGame.h>
#include "..\Gersang\text.h"
#include "OnlineMsgBox.h"
#include "OnlineMegaText.h"
#include <OnlineText.h>

#ifdef _OGG_PLAY
#include <list>
#include <queue>
#include "BgmMgr_PreHeader.h"
#endif

extern	_MainData				GD;									// 게임의 주데이터 
extern _InputDevice			IpD;
extern clock_t StartClock, CurrentClock, PrevClock;

extern	SHORT 				GameMouseX, GameMouseY;
extern	DWORD 				GameWParam;
extern	SHORT 				GameLeftPressSwitch;
extern	SHORT 				GameRightPressSwitch;
extern	_clGame				*pGame;
extern _Text				Text;										// 텍스트 클래스 선언 
extern  int                 WindowModeSwitch;
extern BOOL                 bActive;


class RNumberImage {
private:
	char					m_cCharValue;
	LPDIRECTDRAWSURFACE7	m_stNumberSurface;
	RECT m_rect;

public:
	RNumberImage (void);
	~RNumberImage ();

	void Create(char cChar, int keyColor);
	void Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos);
	void ClearCharValue(void);
	char GetCharValue(void);

	int GetWidth(void);
};

RNumberImage::RNumberImage(void)
{
	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_rect.left + 45;
	m_rect.bottom = m_rect.top + 50;
}

RNumberImage::~RNumberImage()
{
}

UI08 FindNearColor(UI08 r, UI08 g, UI08 b, PALETTEENTRY *pal)
{

	SHORT i;
	UI08 min_color=0;
	int min_diff=100000;
	int diff;

	TRACE("Source RGB(%d, %d, %d)\n", r, g, b);

	for(i = 0;i < 256;i++)
	{
		diff= abs(pal[i].peRed-r) * abs(pal[i].peRed-r) + abs(pal[i].peGreen-g) * abs(pal[i].peGreen-g)
			+ abs(pal[i].peBlue-b) * abs(pal[i].peBlue-b);

		if(diff<min_diff)
		{
			min_diff=diff;
			min_color=(unsigned char)i;
		}
	}

	return min_color;
}

void RNumberImage::Create(char cChar, int keyColor)
{
	HDC hdc;
	char msg[128];

	m_cCharValue = cChar;

	clGrp.CreateOffScreen(m_stNumberSurface, m_rect.right, m_rect.bottom, TRUE, FALSE);

	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = TRANSCOLOR;

	m_stNumberSurface->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

	if(m_stNumberSurface->GetDC(&hdc)==DD_OK)
	{
		HFONT hFont;	

		SI32	siFont = rand()%17;
		SI32	siSize = rand()%4 + 13;
		SI32	siTemp		= GetDeviceCaps( hdc, LOGPIXELSY );
		SI32	siHeight	= -MulDiv( siSize, siTemp, 72 );
		SI32	siDir, siNum;

		siDir = rand()%2 + 1;

		if(siDir == 1)
			siNum = rand()%9;
		else if(siDir == 2)
			siNum = rand()%6 + 30;

		siDir = siNum * 100;

		HRGN rgn = CreateRectRgn(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

		SelectObject(hdc, rgn);
		hFont = CreateFont(siHeight, 0, siDir, 0, FW_NORMAL, (rand() % 2 == 0)? TRUE : FALSE, FALSE, 0, 1, 0, 0, 0, 0, pGame->pOnlineWorld->pOnlineFonts->GetFontName(siFont));

		COLORREF fontColor;
		fontColor = RGB(rand() % 180 + 60, 0, 0);
		SetTextColor( hdc, fontColor ); // NORMAL_TEXT_COLOR );
		SelectObject( hdc, hFont );
		SetBkMode( hdc, TRANSPARENT );
// kuzimo
		wsprintf(msg, "%c", m_cCharValue);

		TextOut(hdc, m_rect.right / 2, m_rect.bottom / 2, msg, strlen(msg));


		m_stNumberSurface->ReleaseDC(hdc);

		DeleteObject(rgn);
		DeleteObject(hFont);

		int x, y;
		int transColorRepeatCount;
		BYTE *pCharSurface;
		BYTE transColor;

		transColorRepeatCount = 1000;

		if(clGrp.LockSurface(m_stNumberSurface) == TRUE)
		{
			pCharSurface = (BYTE*)clGrp.SurfaceDesc.lpSurface;
			for(y = 0; y < m_rect.bottom; y++)
			{
				for(x = 0; x < m_rect.right; x++)
				{
					if( *pCharSurface != TRANSCOLOR )
					{
						if(transColorRepeatCount++ >= 20)
						{
							transColor = FindNearColor( rand() % 50 + 200, rand() % 150 + 100, rand() % 50 + 50, clGrp.PalEntries );
							transColorRepeatCount = 0;
						}
						*pCharSurface = transColor;
					}
					pCharSurface++;
				}
			}
			clGrp.UnlockSurface(m_stNumberSurface);
		}
	}
}

void RNumberImage::Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos)
{
	HRESULT hr = pTargetsurface->BltFast( pos.x, pos.y, m_stNumberSurface, &m_rect, DDBLTFAST_SRCCOLORKEY );
}

int RNumberImage::GetWidth(void)
{
	return(m_rect.right - m_rect.left);
}

void RNumberImage::ClearCharValue(void)
{
	m_cCharValue = 0;
}

char RNumberImage::GetCharValue(void)
{
	return(m_cCharValue);
}

#define RNUMBER_IMAGE_NUMBER (26+10)	// 알파벳+숫자
#define RNUMBER_CYPER 4

class RNumber {
private:
	int				m_iCheckSum;
	RNumberImage	*m_pNumberImage[RNUMBER_CYPER];

public:
	RNumber(void);
	~RNumber();

	void Create(char *pInCharTable, RNumberImage *pInNumberImage[RNUMBER_IMAGE_NUMBER]);
	int Encode(char *pInPassString);

	void Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos);

	bool IsCheckSum(char *pInPassString);
};

RNumber::RNumber(void)
:m_iCheckSum(0)
{
}

RNumber::~RNumber()
{
}

void RNumber::Create(char *pInCharTable, RNumberImage *pInNumberImage[RNUMBER_IMAGE_NUMBER])
{
	int i, j;
	RNumberImage *pNumberImage;
	char passString[RNUMBER_CYPER + 1];

	for(i = 0; i < RNUMBER_CYPER; i++)
	{
		passString[i] = pInCharTable[rand() % RNUMBER_IMAGE_NUMBER];
	}
	passString[i] = '\0';

	m_iCheckSum = Encode(passString);

	for( i = 0; i < RNUMBER_CYPER; i++)
	{
		pNumberImage = NULL;

		for( j = 0; j < RNUMBER_IMAGE_NUMBER; j++ )
		{
			if(pInNumberImage[j]->GetCharValue() == passString[i])
			{
				pNumberImage = pInNumberImage[j];
			}
		}
		ASSERT(pNumberImage);
		m_pNumberImage[i] = pNumberImage;
	}

	TRACE(passString);
	TRACE("\n");
}

void RNumber::Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos)
{
	int i;

	for(i = 1; i <= RNUMBER_CYPER; i++)
	{
		m_pNumberImage[RNUMBER_CYPER-i]->Draw(pTargetsurface, pos);
		pos.x += m_pNumberImage[RNUMBER_CYPER-i]->GetWidth();
	}
}

int RNumber::Encode(char *pInPassString)
{
	int iCheckSum, i;

	i = 0;
	iCheckSum = 1;

	while((*pInPassString != '\0') && (i < RNUMBER_CYPER))
	{
		iCheckSum *= *pInPassString;
		pInPassString++;
		i++;
	}

	return(iCheckSum);
}

bool RNumber::IsCheckSum(char *pInPassString)
{
	return((m_iCheckSum == Encode(pInPassString))? true : false);
}

class RNumberManager {
private:
	int				m_iShakeNumber;
	int				m_iMaxNumber;
	RNumber			**m_ppNumber;
	RNumberImage	*m_pNumberImage[RNUMBER_IMAGE_NUMBER];

public:
	RNumberManager(void);
	~RNumberManager();

	void Create(int iInMaxNumber);
	void Release(void);

	void Shake(void);
	void Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos);
	bool IsCheckSum(char *pInPassString);
};

RNumberManager::RNumberManager(void)
:m_iShakeNumber(0),m_iMaxNumber(0),m_ppNumber(NULL)
{
	int i;

	for(i = 0; i < RNUMBER_IMAGE_NUMBER; i++)
	{
		m_pNumberImage[i] = NULL;
	}
}

RNumberManager::~RNumberManager()
{
	Release();
}

void RNumberManager::Create(int iInMaxNumber)
{
	int i;
	RNumber *pRNumber;
	char charTable[RNUMBER_IMAGE_NUMBER] = 
	{
		'1', '2', '3', '4', '5', 'A', '3', '8', 'A', '8',
		'A', 'T', 'C', 'D', 'E', 'F', '4', 'H', '1', 'J', 'K', '3', 'M', 'N',
		'3', 'H', '2', 'R', 'S', 'T', 'A', 'E', 'C', 'X', 'Y', '8' 
	};

	for(i = 0; i < RNUMBER_IMAGE_NUMBER; i++)
	{
		m_pNumberImage[i] = new RNumberImage();
		ASSERT(m_pNumberImage[i]);
		m_pNumberImage[i]->Create(charTable[i], 200);
	}

	m_iMaxNumber = iInMaxNumber;

	m_ppNumber = new RNumber*[m_iMaxNumber];
	ASSERT(m_ppNumber);

	for(i = 0; i < m_iMaxNumber; i++)
	{
		pRNumber = new RNumber();
		ASSERT(pRNumber);
		
		pRNumber->Create(charTable, m_pNumberImage);
		*(m_ppNumber + i) = pRNumber;
	}

	for(i = 0; i < RNUMBER_IMAGE_NUMBER; i++)
	{
		m_pNumberImage[i]->ClearCharValue();
		ASSERT(m_pNumberImage[i]);
	}

	Shake();
}

void RNumberManager::Release(void)
{
	int i;
	RNumber *pRNumber;

	for(i = 0; i < m_iMaxNumber; i++)
	{
		pRNumber = *(m_ppNumber + i);
		if(pRNumber != NULL)
		{
			delete pRNumber;
		}

		ASSERT(pRNumber);
	}
	delete m_ppNumber;

	for(i = 0; i < RNUMBER_IMAGE_NUMBER; i++)
	{
		if(m_pNumberImage[i] != NULL)
		{
			delete m_pNumberImage[i];
		}
	}
}

void RNumberManager::Shake(void)
{
	m_iShakeNumber = rand() % m_iMaxNumber;
}

void RNumberManager::Draw(LPDIRECTDRAWSURFACE7 pTargetsurface, POINT pos)
{
	RNumber *pRNumber = *(m_ppNumber + m_iShakeNumber);

	ASSERT(pRNumber);

	pRNumber->Draw(pTargetsurface, pos);	
}

bool RNumberManager::IsCheckSum(char *pInPassString)
{
	RNumber *pRNumber;

	pRNumber = *(m_ppNumber + m_iShakeNumber);
	return(pRNumber->IsCheckSum(pInPassString));
}

RNumberManager *m_pNumberManager = NULL;

//-----------------

_MiniGame2::_MiniGame2(SI32 minigametype, SI32 standardscore, cltOnlineWorld *pOnlineWorld):_MiniGame(minigametype, standardscore, pOnlineWorld)
{

}
 
_MiniGame2::~_MiniGame2()
{
	
}

void _MiniGame2::Init()
{
	SI32 index;

	SI32 data1[] = {	0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 
		10, 11,  12, 13, 14, 15, 16, 17, 18, 19, 
		20, 21,  22, 23, 24, 25, 26, 27, 28, 29,
		30, 31,  32, 33, 34,
		-1};
	

	SI32 data2[] = {	6, 4, 6*4*4/3,
		6, 4, 6*4*4/3,
		6, 6, 6*6*4/3,
		6, 6, 6*6*4/3,
		8, 5, 8*5*4/3,
		8, 5, 8*5*4/3,
		8, 6, 8*6*4/3,
		8, 6, 8*6*4/3,
		10, 6, 10*6*4/3,
		10, 6, 10*6*4/3,
		10, 8, 10*8*4/3,
		10, 8, 10*8*4/3,
		12, 7, 12*7*4/3,
		12, 7, 12*7*4/3,
		12, 8, 12*8*4/3,
		12, 8, 12*8*4/3,
		14, 8, 14*8*4/3,
		14, 8, 14*8*4/3,
		-1,
	};
	char  *data3[] = 
	{
		"online\\minigame\\minigame2\\minigame2_board.spr",
		"online\\minigame\\minigame2\\MINIGAME2_CARD.Spr",
		""
	};


	
	ChipInMouse= -1;
	PressedChip= -1;

	ShowChipNumber		= 0;
	ReadyToHideNumber	= 0;

	index = 0; 
	while(1)
	{
		ImagePool[index] =  data1[index];
		if(data1[index] == -1)break;
		index++;
	}

	index = 0; 
	while(1)
	{
		DifficultyLevel[index] =  data2[index];
		if(data2[index] == -1)break;
		index++;
	}

	index=0;
	while(strcmp(data3[index], "")  != 0)
	{
		strcpy(MiniGameFileName[index], data3[index]);
		index++;
	}


	// 키버퍼를 초기화하여 이전의 입력을 무시한다. 
	GD.KB.InitKeyBuffer();

	// 필요한 화일을 불러온다. 
	for(int i = 0;i < MAX_FILE_NUMBER_MINIGAME2;i++)
	{
		pMiniGameSpr[i] = new XSPR;
		if(pMiniGameSpr[i]  == NULL)
		{
			clGrp.Error("fd93722jf", "Fd398575:%s", MiniGameFileName[i]);
			break;
		}
		
		if(clGrp.LoadXspr(MiniGameFileName[i], *pMiniGameSpr[i])==FALSE)
		{
			clGrp.Error("12fd93722jf", "Fd398575:%s", MiniGameFileName[i]);
			break;
		}
	}

	if(pMiniGameSpr[FILE_MINIGAME2_CARD])
	{
		ChipXsize = pMiniGameSpr[FILE_MINIGAME2_CARD]->Header.Xsize;
		ChipYsize = pMiniGameSpr[FILE_MINIGAME2_CARD]->Header.Ysize;
	}

	ChipStartX = (clGrp.GetScreenXsize() - ChipXsize * MINIGAME2_MAX_XNUMBER) / 2;
	ChipStartY = (clGrp.GetScreenYsize() - ChipYsize * MINIGAME2_MAX_YNUMBER) / 2;

	MiniGame2StartSwitch	= FALSE;
// actdoll (2004/05/05 16:00) : [OECEX]
	m_hOecMiniGame			= NULL;
	m_siSelect				= MINIGAME2_WAIT;
	m_bSkip				    = FALSE;
	m_bFinishedswitch		= FALSE;
	m_YetEditHWND			= NULL;
	m_YetEditHWNDEnable		= 0;
	m_YetEditLockShortKey	= 0;
	m_siTextNum				= 0;

	SelectedChip1.x = -1;
	SelectedChip1.y = -1;

	SelectedChip2.x = -1;
	SelectedChip2.y = -1;

	// 이미지의 갯수를 파악한다. 
	ImageNumberInPool = 0;
	*m_siKey		  =	0;
	m_siCount		  = 0;
	bSendHackMsg = FALSE;
	
	index = 0;
	while(1)
	{
		if(ImagePool[index] >= 0)
		{
			ImageNumberInPool++;
		}
		else
		{
			break;
		}

		index++;
	}

	// 레벨에 따라 변수를 설정한다. 
	DecideVaryByLevel_MiniGame2();

	// 칩을 초기화한다. 
	InitChip_MiniGame2();

	// 칩에 이미지를 배열한다. 
	ArrangeRandomImage_MiniGame2();

	clGrp.LoadXspr("Online\\MiniGame\\MiniGame2\\MiniGameNumberChecker.Spr", m_ImgCheck); 

	m_siX = (800 - m_ImgCheck.Header.Xsize) / 2;
	m_siY = (600 - m_ImgCheck.Header.Ysize) / 2;

	pOnlineMsgBox					=	new OnlineMsgBox; 
	pOnlineMsgBox->Init(pMyOnlineWorld);


	//나가기 버튼 그림
	pExitSpr = pMyOnlineWorld->pOnlineResource->GetxsprGame( ON_GAMESPR_BOOKEXIT );		
	BExit.Create( 525, 517, pExitSpr->Header.Xsize, pExitSpr->Header.Ysize, pMyOnlineWorld->pOnlineText->Get( ON_TEXT_EXIT ), BUTTON_PUT_LEFT, TRUE );

	// 에디트 박스 설정
	// actdoll (2004/05/05 16:00) : [OECEX]
	//	이제 만국 공통으로 숫자 배열을 4자리만 쓰며 숫자영문만 가능하다.
	RECT	rcRect = { m_siX + 71,m_siY + 183, m_siX + 71 + 200, m_siY + 183 + 15 };
	m_hOecMiniGame	= Handler_OnlineEditControlEx::CreateControl( DF_FLAG_OEC_TYPE_ONLYASCII, &rcRect, 4 );
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecMiniGame );
	pOecEx->SetFont( pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_CHAT ) );
	pOecEx->Clear();

	m_szMiniGameEditBoxString = new CHAR[10];
	ZeroMemory(m_szMiniGameEditBoxString, 10);

#ifdef _OGG_PLAY
	GetBgmMgr()->Play(OGGBGM_FILENAME_MINIGAME, TRUE);
#else
	// 음악을 연주한다. 
	PlayMusic("music\\MiniGame1.YAV");
#endif
	
	m_siFailTimes = 0;
	m_siPrevConnectTime = 0;
	m_siTotConnectDiff = 0;
	m_siAverage = 0;
	m_siDelayConnectTimes = 0;

	m_bHackMonitor = FALSE;
	m_siPreMouseX = 0;
	m_siPreMouseY = 0;
	m_siCenterHackCount = 0;
	m_siRepeatHackCount = 0;

	m_pNumberManager = new RNumberManager();
	ASSERT(m_pNumberManager);

	m_pNumberManager->Create(10);
}

void _MiniGame2::Free()
{	
	if(m_pNumberManager != NULL)
	{
		delete m_pNumberManager;
		m_pNumberManager = NULL;
	}

	Handler_OnlineEditControlEx::KillControl( m_hOecMiniGame );
	if(m_ImgCheck.Image)
	{
		clGrp.FreeXspr(m_ImgCheck);       
		ZeroMemory(&m_ImgCheck,sizeof(XSPR));
	}
	if(pOnlineMsgBox)
	{
		delete pOnlineMsgBox;
		pOnlineMsgBox = NULL;
	}
	
	if(m_szMiniGameEditBoxString)	delete [] m_szMiniGameEditBoxString;
}


void _MiniGame2::DecideVaryByLevel_MiniGame2()
{
	if(DifficultyLevel[GetMiniGameLevel() * 3] == -1)
	{
		SetMiniGameLevel(0);
		SetHackMiniGameLevel(0);
	}

	XNumber		= DifficultyLevel[GetMiniGameLevel() * 3];
	YNumber		= DifficultyLevel[GetMiniGameLevel() * 3 + 1];
	SetPlayTime(DifficultyLevel[GetMiniGameLevel() * 3 + 2] * 1000);
	SetHackMiniGamePlayTime(DifficultyLevel[GetMiniGameLevel() * 3 + 2] * 1000);
}

void _MiniGame2::InitChip_MiniGame2()
{
	SI32 i, j;

	for(i = 0;i<MINIGAME2_MAX_YNUMBER;i++)
		for(j=0;j<MINIGAME2_MAX_XNUMBER;j++)
		{
			Chip[j][i].siIndex	= -1;
			Chip[j][i].siKind	= 0;
		}

	m_siCenterHackCount = 0;
	m_siRepeatHackCount = 0;
}



BOOL _MiniGame2::IsMouseInChip_MiniGame2(SI32 x, SI32 y)
{
	SI32 StartX, StartY, CenterX, CenterY, SameX, SameY;
	StartX = ChipStartX + x*ChipXsize;
	StartY = ChipStartY + y*ChipYsize;
	CenterX = StartX + x*ChipXsize/2;
	CenterY = StartY + y*ChipYsize/2;
	SameX = GameMouseX - StartX;
	SameY = GameMouseY - StartY;

	if(StartX <= GameMouseX && GameMouseX <= StartX + ChipXsize	&& StartY <= GameMouseY && GameMouseY <= StartY + ChipYsize)
	{
		if(m_bHackMonitor==FALSE)
		{
			if(IpD.LeftPressSwitch==TRUE && Chip[x][y].siIndex >= 0)
			{
				// 가운데 찍나 검사
				if(GameMouseX >= (CenterX-2) && GameMouseX <= (CenterX+2) && GameMouseY >= (CenterY-2) && GameMouseY <= (CenterY+2))
				{
					m_siCenterHackCount++;
					if(m_siCenterHackCount == (rand()%10 + 8))		// 8~17
						m_bHackMonitor = TRUE;
				}
				else
					m_siCenterHackCount=0;
				// 같은 점을 찍나 검사
				if(m_siPreMouseX == SameX && m_siPreMouseY == SameY)
					m_siRepeatHackCount++;
				else
					m_siRepeatHackCount--;
				if(m_siRepeatHackCount == (rand()%10 + 8))		// 8~17
					m_bHackMonitor = TRUE;
				m_siPreMouseX = SameX;
				m_siPreMouseY = SameY;
				IpD.LeftPressSwitch = FALSE;
			}
		}
		return TRUE;
	}

	return FALSE;
}


BOOL _MiniGame2::Action()
{
	SI32 i, j; 

	if(!CheckHacking())	return FALSE;

	InputOperate();

	if(MiniGame2StartSwitch && !m_siPrevConnectTime)
		m_siPrevConnectTime = CurrentClock;
	
	// 마우스가 눌린 칩을 찾는다. 
	ChipInMouse = -1;
	for(i = 0; i < MINIGAME2_MAX_YNUMBER;i++)
	{
		for(j = 0;j < MINIGAME2_MAX_XNUMBER;j++)
		{
			if(IsMouseInChip_MiniGame2(j, i) == TRUE)
				ChipInMouse = j + i * MINIGAME2_MAX_XNUMBER;
		}
	}
	
	// 마우스로 선택한 칩을 찾는다. 
	if(GameLeftPressSwitch == FALSE && PressedChip >=0 && (PressedChip == ChipInMouse) )
	{
		// 게임이 작동중이다. 
		MiniGame2StartSwitch = TRUE;
		
		SI32 x = ChipInMouse%MINIGAME2_MAX_XNUMBER;
		SI32 y = ChipInMouse/MINIGAME2_MAX_XNUMBER;
		
		if(Chip[x][y].siIndex >= 0)
		{
			// 첫번쨰 칩이 아직 선택되지 않았거나 두번째 칩이 첫번째와 같은 위치에 있다면, 
			if(SelectedChip1.x == -1 )
			{
				
				MiniGame2PathIndex		= 0;
				MiniGame2DrawPathIndex	= 0; 
				
				SelectedChip1.x = x;
				SelectedChip1.y = y;
			}
			// 첫번째 칩과 같은 위치에 있는 칩이면 취소. 
			else if(SelectedChip1.x  == x && SelectedChip1.y == y)
			{
				MiniGame2PathIndex		= 0;
				MiniGame2DrawPathIndex	= 0;
				
				SelectedChip1.x = -1;
				SelectedChip1.y = -1;
				m_siFailTimes++;
			}
			// 두번째 칩과 같은 위치에 있는 칩이면 취소. 
			else if(SelectedChip2.x  == x && SelectedChip2.y == y)
			{
				MiniGame2PathIndex		= 0;
				MiniGame2DrawPathIndex	= 0;
				
				SelectedChip2.x = -1;
				SelectedChip2.y = -1;
				m_siFailTimes++;
			}
			else
			{
				SelectedChip2.x = x;
				SelectedChip2.y = y;
			}
		}
	}
	
	if(GameLeftPressSwitch == TRUE && ChipInMouse >= 0)
		PressedChip = ChipInMouse;
	else
		PressedChip = -1;

	// 나가기 버튼을 눌렀을 때
	if( BExit.Process( pMyOnlineWorld->fLeftUpSwitch ) == TRUE )
	{
		// actdoll (2004/05/05 16:00) : [OECEX]
		Handler_OnlineEditControlEx::ClearCurrentFocus();
		pMyOnlineWorld->SetShortKey( FALSE );	

		return FALSE;
	}

	// 두개의 칩이 모두 선택되었다면, 
	if(SelectedChip1.x >= 0 && SelectedChip2.x >= 0)
	{
		for(i=0;i<MINIGAME2_MAX_YNUMBER;i++)
		{
			for(j=0;j<MINIGAME2_MAX_XNUMBER;j++)
				TempPath[j][i] = FALSE;
		}
			
		// 선택된 칩이 서로 다르면, 
		if(Chip[SelectedChip1.x][SelectedChip1.y].siIndex != Chip[SelectedChip2.x][SelectedChip2.y].siIndex)
		{
			SelectedChip1.x = -1;
			SelectedChip1.y = -1;
				
			SelectedChip2.x = -1;
			SelectedChip2.y = -1;

			// 감점한다. 
			SetMiniGamePoint( max(0, GetMiniGamePoint() - 2) );
			SetHackMiniGamePoint( max(0, GetHackMiniGamePoint() - 2) );
			m_siFailTimes++;
		}
		// 두개의 칩이 연결 될 수 있는것이라면, 
		else if(MiniGame2DrawPathIndex == 0 )
		{
			if(CheckConnect(SelectedChip1, SelectedChip2, -1, 0) == TRUE)
			{
				MiniGame2Path[MiniGame2PathIndex] = SelectedChip1;
				MiniGame2PathIndex++;
			}
			else
			{
				SelectedChip1.x = -1;
				SelectedChip1.y = -1;
				
				SelectedChip2.x = -1;
				SelectedChip2.y = -1;
			}
		}
	}

	// 경로를 그린다.
	if(MiniGame2DrawPathIndex == MiniGame2PathIndex)
	{
		MiniGame2DrawPathIndex = 0; 
		MiniGame2PathIndex	   = 0;
	}

	if(MiniGame2DrawPathIndex + 3 <= MiniGame2PathIndex)
		MiniGame2DrawPathIndex += 3;
	else
		MiniGame2DrawPathIndex = MiniGame2PathIndex;

	if(MiniGame2PathIndex && (MiniGame2DrawPathIndex == MiniGame2PathIndex))
	{
		if(SelectedChip1.x >=0 && SelectedChip2.x >=0)
		{
			Chip[SelectedChip1.x][SelectedChip1.y].siIndex = -1;
			Chip[SelectedChip2.x][SelectedChip2.y].siIndex = -1;

			SelectedChip1.x = -1;
			SelectedChip1.y = -1;
			SelectedChip2.x = -1;
			SelectedChip2.y = -1;

			// 칩이 사라지면 점수를 준다. 
			SetMiniGamePoint(GetMiniGamePoint() + ((GetPlayTime()/1000) / 2) );
			SetHackMiniGamePoint(GetHackMiniGamePoint() + ((GetPlayTime()/1000) / 2) );

			// 칩연결하는데 걸린 시간누적
			if(m_siPrevConnectTime)
			{
				SI32 DiffConnectTime = CurrentClock - m_siPrevConnectTime;
				if(GetMiniGameLevel() >= 4)
				{
					if(DiffConnectTime >= m_siAverage * 1.5 || DiffConnectTime <= m_siAverage * 0.5) 
						m_siDelayConnectTimes++;
				}
				m_siTotConnectDiff += DiffConnectTime;
			}
			m_siPrevConnectTime = CurrentClock;
		}
	}

	// 마우스를 출력한다. 
	POINT point;
	GetCursorPos(&point);
	if (WindowModeSwitch)
	{
		ScreenToClient(pGame->Hwnd,&point);
	}
	
	IpD.Mouse_X=(SHORT)point.x;
	IpD.Mouse_Y=(SHORT)point.y;
	DrawMousePrimary();


	// 일정 프레임을 유지한다. 
	SI32 diff = abs(CurrentClock - PrevClock);
	if(GetPlayTime() >= diff)
	{
		if(MiniGame2StartSwitch == TRUE)
		{
			SetPlayTime(GetPlayTime() - min(30*2, diff));
			SetHackMiniGamePlayTime(GetHackMiniGamePlayTime() - min(30*2, diff));
		}
	}
	else
	{
		SetPlayTime(0);
		SetHackMiniGamePlayTime(0);
		
		// actdoll (2004/05/05 16:00) : [OECEX]
		Handler_OnlineEditControlEx::ClearCurrentFocus();
		pMyOnlineWorld->SetShortKey( FALSE );
		::SetFocus(pGame->Hwnd);

		return FALSE;
	}

	if(m_bSkip == FALSE)
	{
	m_bFinishedswitch = FALSE;

	for(i = 0; i < MINIGAME2_MAX_YNUMBER;i++)
		for(j = 0;j < MINIGAME2_MAX_XNUMBER;j++)
		{
			if(Chip[j][i].siIndex != -1)
				m_bFinishedswitch = TRUE;
		}

//
	}
	if(m_bFinishedswitch == FALSE)
	{
		// 시간제한에 걸리는지 체크한다(마작매크로 작업지연 시키기)

		// 8게임째 한번도 틀린적이 없다면 매크로로 의심한다.
		if(GetMiniGameLevel() >= 11 && !m_siFailTimes)
		{
			pMyOnlineWorld->pOnlineMsgBoxError->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(2283));
			return FALSE;
		}

		// 짝을 맞추는데 걸린 평균시간을 구하고 매크로 사용의 유무를 추측한다.
		SI08 siCrossTimes = 0;

		// 미니게임이 3단계 이상이면 1/2확률로 숫자 입력창을 생성한다.
		if((GetMiniGameLevel() + 1) % 5 == 0)
		{
			m_siSelect = MINIGAME2_WAIT;
			//랜덤한 수자 폰트 크기등을 생성한다.
			InitCheckNumber();
		}
		else
			m_siSelect = MINIGAME2_NEXT;

		// 숫자입력에 필요한 시간을 다시 세팅한다.
		SetPlayTime(99000);
		SetHackMiniGamePlayTime(99000);

		m_bFinishedswitch = TRUE;
		m_bSkip = TRUE;

		// 점수를 작업으로 전환한다. 
		MakeWork();

		SetMiniGameLevel(GetMiniGameLevel() + 1);
		SetHackMiniGameLevel(GetHackMiniGameLevel() + 1);
	}

	OnlineEditControlEx		*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecMiniGame );

	switch(m_siSelect)
	{
	//숫자 입력창이 생기는 경우.
	case MINIGAME2_WAIT:
		// actdoll (2004/05/05 16:00) : [OECEX]
		if( !Handler_OnlineEditControlEx::IsFocus( m_hOecMiniGame ) )
		{
			// 현재 Focus 저장
			m_YetEditLockShortKey = pMyOnlineWorld->GetShortKey();
			m_YetEditHWND         = GetFocus();
			m_YetEditHWNDEnable   = IsWindowEnabled(m_YetEditHWND);

			pMyOnlineWorld->SetShortKey(TRUE);
		// actdoll (2004/05/05 16:00) : [OECEX]
			Handler_OnlineEditControlEx::SetFocus( m_hOecMiniGame );
		}

		//3번 이상 실패하면 입력을 받지 않는다.
		// actdoll (2004/05/05 16:00) : [OECEX]
		if( pOecEx->IsPushEnter() && (m_siCount < 3) )
		{
			// actdoll (2004/05/05 16:00) : [OECEX]
			strcpy( m_szMiniGameEditBoxString, pOecEx->GetString() );

			char *psUpper = _strupr( m_szMiniGameEditBoxString );
 
			if(m_pNumberManager->IsCheckSum(psUpper) == true)
			{
				m_siCount = 0;
				m_siSelect = MINIGAME2_NEXT;
			}
			else
			{
				m_siCount++;
			}

			//3번이상 실패 했으므로 종료.
			if(m_siCount >= 3)
			{
				pOnlineMsgBox->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(8000278));
				SetMiniGameWork(0);
				return(FALSE);				
			}

			// actdoll (2004/05/05 16:00) : [OECEX]
			pOecEx->Clear();
		}
		if(pOnlineMsgBox->Process()  == ON_MSGBOX_RETURN_OK && m_siCount >= 3)
		{
			// actdoll (2004/05/05 16:00) : [OECEX]
			Handler_OnlineEditControlEx::ClearCurrentFocus();

			pMyOnlineWorld->SetShortKey( FALSE );	

			return FALSE;
		}
			

		break;
	//자동으로 다음스테이지로 넘어가거나 숫자 입력에 성공한 경우.
	case MINIGAME2_NEXT:

		MiniGame2StartSwitch = FALSE;

		MiniGame2PathIndex = 0;

		// 최종단계까지 가면 미니겜을 끝낸다.
		if(DifficultyLevel[GetMiniGameLevel() * 3] == -1)		
		{
			// actdoll (2004/05/05 16:00) : [OECEX]
			Handler_OnlineEditControlEx::ClearCurrentFocus();

			pMyOnlineWorld->SetShortKey( FALSE );

			return FALSE;
		}
		// 레벨에 따라 변수를 설정한다. 
		DecideVaryByLevel_MiniGame2();
		// 칩을 초기화한다. 
		InitChip_MiniGame2();
		// 칩에 이미지를 배열한다. 
		ArrangeRandomImage_MiniGame2();

		m_siSelect = MINIGAME2_SKIP;
		m_bSkip	   = FALSE;

		break;
	case MINIGAME2_SKIP:
		break;
	}
	
	return TRUE;
}

void _MiniGame2::InitCheckNumber()
{
	SI16	siDir;
	UI16	siNum;
	SI16	siTextNum;

	SI16	RGB[5] = {0,225,210,240,255};

	srand(timeGetTime());

	SetMiniGameCheckNumber(((rand()*rand())%899999)+100000);

	siTextNum = rand()%120 + 1;

	siDir = rand()%2 + 1;

	if(siDir == 1)
		siNum = rand()%9;
	else if(siDir == 2)
		siNum = rand()%6 + 30;

	m_siDir = siNum * 100;


	m_siFont = rand()%17;
	m_siSize = rand()%10 +12;

	m_siTextNum = siTextNum;

	m_pNumberManager->Shake();
}


void _MiniGame2::Draw(LPDIRECTDRAWSURFACE7 psurface)
{
	SI32 i, j, k, l;
	HDC hdc;
	char buffer[128];
	OnlineEditControlEx	*pOecEx	= Handler_OnlineEditControlEx::GetControl( m_hOecMiniGame );

	// 판넬을 그린다. 
	if(clGrp.LockSurface(psurface) == TRUE)
	{
		SI32 startx = (clGrp.GetScreenXsize() - pMiniGameSpr[FILE_MINIGAME2_BACK]->Header.Xsize)/ 2;
		SI32 starty = (clGrp.GetScreenYsize() - pMiniGameSpr[FILE_MINIGAME2_BACK]->Header.Ysize)/ 2;

		clGrp.PutSpriteT(startx, starty, 
						pMiniGameSpr[FILE_MINIGAME2_BACK]->Header.Xsize,
						pMiniGameSpr[FILE_MINIGAME2_BACK]->Header.Ysize,
						pMiniGameSpr[FILE_MINIGAME2_BACK]->Image);
		
		clGrp.UnlockSurface(psurface);
	}

	// 남은 시간을 표시한다. 
	wsprintf(buffer, "%d", GetPlayTime()/1000);

	SI32 percent = (GetPlayTime() % 1000) / 10;
	
	SI32 xsize, ysize;
	if(GetPlayTime()/1000 > 10)
	{
		xsize = 15 ;
		ysize = 8  ;
	}
	else
	{
		xsize = 22  * percent / 100; 
		ysize = 12  * percent / 100; 
	}

	RECT destrect = {400-xsize, 80 - ysize, 400+xsize, 80 + ysize};
	DrawTextInRect(psurface, pGame->pBattle->SaveInfoFont, buffer, destrect);

	// 점수를 표시한다. 
	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SelectObject(hdc, pGame->pBattle->HelpFont);
		
		SetTextColor( hdc, NORMAL_TEXT_COLOR );
		SetBkMode(hdc, TRANSPARENT);
		
		wsprintf(buffer, Text.Scores.Get());
		DrawTextInCenter(hdc, 203, 500, buffer);
		wsprintf(buffer, "%d",GetMiniGamePoint());
		DrawTextInCenter(hdc, 278, 500, buffer);
		
		wsprintf(buffer, Text.Difficulty.Get());
		DrawTextInCenter(hdc, 345, 500, buffer);
		wsprintf(buffer, "%d",GetMiniGameLevel());
		DrawTextInCenter(hdc, 409, 500, buffer);
		
		wsprintf(buffer, Text.Work.Get());
		DrawTextInCenter(hdc, 474, 500, buffer);
		wsprintf(buffer, "%d",GetMiniGameWork());
		DrawTextInCenter(hdc, 567, 500, buffer);
		
		psurface->ReleaseDC(hdc);
	}

	if(clGrp.LockSurface(psurface) == TRUE)
	{
		
		// 칸을 표시한다. 
		for(i = 0; i < MINIGAME2_MAX_YNUMBER;i++)
		{
			for(j = 0;j < MINIGAME2_MAX_XNUMBER;j++)
			{
				clGrp.Box(ChipStartX + j*ChipXsize,				ChipStartY + i*ChipYsize, 
					ChipStartX + j*ChipXsize+ChipXsize-1, ChipStartY + i*ChipYsize+ChipYsize-1, MINIGAME2_BORDER_COLOR);
			}
		}
			
		// 칩을 보여준다. 
		for(i = 0; i < MINIGAME2_MAX_YNUMBER;i++)
		{
			for(j = 0;j < MINIGAME2_MAX_XNUMBER;j++)
			{
				if(Chip[j][i].siIndex != -1)
				{
					SI32 font = Chip[j][i].siKind * 72 + Chip[j][i].siIndex * 2;
					
					// 선택된 칩이면, 
					if((j == SelectedChip1.x && i == SelectedChip1.y) || (j == SelectedChip2.x && i == SelectedChip2.y))
						font++;
					
					clGrp.PutSpriteT(ChipStartX + j*ChipXsize, ChipStartY + i*ChipYsize, ChipXsize, ChipYsize, 
						&pMiniGameSpr[FILE_MINIGAME2_CARD]->Image[ pMiniGameSpr[FILE_MINIGAME2_CARD]->Header.Start[font] ]);
				}
			}
		}
	
		// 경로를 표시한다. 
		for(i = 0; i<MiniGame2DrawPathIndex;i++)
		{
			if(i < (MiniGame2DrawPathIndex-1))
			{
				for(k=-1;k<=1;k++)
					for(l=-1;l<+1;l++)
					{
						clGrp.Line(	ChipStartX + MiniGame2Path[i].x*ChipXsize	+ ChipXsize/2+k, 
							ChipStartY + MiniGame2Path[i].y*ChipYsize	+ ChipYsize/2+k, 
							ChipStartX + MiniGame2Path[i+1].x*ChipXsize + ChipXsize/2+l,
							ChipStartY + MiniGame2Path[i+1].y*ChipYsize + ChipYsize/2+l,  
							COLOR_RED);
					}
			}
		}
		
		BExit.Put( pExitSpr,  0, 2, 1, BUTTON_PUT_NOMOVE );
		 
		clGrp.UnlockSurface(psurface);
	}
	

	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );

		switch(m_siSelect)
		{
			case MINIGAME2_WAIT:
			{
				// actdoll (2004/11/29 16:59) : 이제 단일 루틴으로 돌아간다.
				pMyOnlineWorld->pOnlineMegaText->DrawText(hdc,m_siX +81, m_siY + 90, "" ,RGB(255,255,255));
				clGrp.PutSpriteT(m_siX, m_siY, m_ImgCheck.Header.Xsize, m_ImgCheck.Header.Ysize, m_ImgCheck.Image);
				pMyOnlineWorld->pOnlineMegaText->DrawTextCenter(hdc,m_siX + 10, m_siY + 8,184,14, pMyOnlineWorld->pOnlineText->Get(2284), RGB(255, 255, 255));
				POINT pos = {310, 250};
				strcpy( m_szMiniGameEditBoxString, pOecEx->GetString() );
				if( Handler_OnlineEditControlEx::IsFocus( m_hOecMiniGame ) )
				{
					pOecEx->Draw( hdc );
				}
				psurface->ReleaseDC(hdc);
				m_pNumberManager->Draw(psurface, pos);
			}
			break;

			case MINIGAME2_NEXT:
			case MINIGAME2_SKIP:
			{
				psurface->ReleaseDC(hdc);
			}
			break;
		}
	}
	ASSERT(m_pNumberManager);

	if(psurface->GetDC(&hdc)==DD_OK)
	{
		SetBkMode( hdc, TRANSPARENT );
		SelectObject( hdc, pMyOnlineWorld->pOnlineFonts->GetFont( ON_FONT_BUTTON ) );
		BExit.Put( hdc );
		psurface->ReleaseDC(hdc);
	}
	pOnlineMsgBox->Draw(psurface);

	pMyOnlineWorld->pOnlineMsgBoxError->Draw(psurface);

	// 마우스를 그린다. 
	DrawMouse(psurface);
}


void _MiniGame2::Quit()
{
	SI32 i;

	// 로드한 화일들을 해제한다. 
	for(i = 0;i < MAX_FILE_NUMBER_MINIGAME2;i++)
	{
		if(pMiniGameSpr[i])
		{
			if(pMiniGameSpr[i]->Image)
			{
				clGrp.FreeXspr(*pMiniGameSpr[i]);
				pMiniGameSpr[i]->Image = NULL;
			}

			delete pMiniGameSpr[i];
			pMiniGameSpr[i] = NULL;

		}
	}
	Free();
	_MiniGame::Quit();
}


BOOL _MiniGame2::CheckConnect(POINT pt1, POINT pt2, SI32 dir, SI32 count)
{
	POINT pt_temp1;

	SI32 x, y;
	SI32 i;

	for(i = 0; i< 4;i++)
	{
		SI32 temp_dir = i;

		// 북쪽. 
		if(i == 0)
		{
			x = pt1.x;
			y = pt1.y - 1;
		}
		// 동쪽. 
		else if(i == 1)
		{
			x = pt1.x + 1;
			y = pt1.y ;
		}
		// 남쪽. 
		else if(i == 2)
		{
			x = pt1.x;
			y = pt1.y + 1 ;
		}
		// 서쪽  
		else if(i == 3)
		{
			x = pt1.x - 1;
			y = pt1.y ;
		}


		
		pt_temp1.x  = x;
		pt_temp1.y  = y;

		SI32 tempcount = count;
		if(temp_dir != dir)
		{
			tempcount++;
		}
		
		// 여러번 꺾이면 실패. 
		if(tempcount > 3)
		{
			continue;
		}
		
		// 목적지에 도착했다. 
		if(pt2.x == pt_temp1.x && pt2.y == pt_temp1.y)
		{
			MiniGame2Path[MiniGame2PathIndex] = pt2;
			MiniGame2PathIndex++;

			return TRUE;
		}
		

		SI32 startx = (MINIGAME2_MAX_XNUMBER - XNumber) / 2;
		SI32 starty = (MINIGAME2_MAX_YNUMBER - YNumber) / 2;

		// 새로운 이동 지점에 문제가 있으면,
		// 다른 칩에 의해 막히면 실패. 
		if(x < (startx-1) || x > (startx + XNumber) 
		|| y < (starty-1) || y > (starty + YNumber)
		|| Chip[x][y].siIndex >= 0)
		{
		   continue;
		}
		else if(TempPath[x][y])
		{
			continue;
		}
		else
		{
			TempPath[x][y] = TRUE;

			if(CheckConnect(pt_temp1, pt2, temp_dir, tempcount) == TRUE)
			{
				MiniGame2Path[MiniGame2PathIndex] = pt_temp1;
				MiniGame2PathIndex++;

				return TRUE;
			}
			else
			{
				TempPath[x][y] = FALSE;
			}
		}
	}

	return FALSE;
}

void _MiniGame2::ArrangeRandomImage_MiniGame2()
{
	SI32 i, j, k;
	SI32 count;
	POINT point1, point2;

	// 필요한 이미지 갯수를 얻는다. 
	SI32 needimagenumber = XNumber * YNumber / 4;

	SI32 xpos, ypos;
	SI32 startx, starty;

	startx = (MINIGAME2_MAX_XNUMBER - XNumber) / 2;
	starty = (MINIGAME2_MAX_YNUMBER - YNumber) / 2;

	while(needimagenumber)
	{
		for(i = 0; i < 4; i++)
		{
			count = 0;
			while(1)
			{
				xpos = startx + rand() % XNumber;
				ypos = starty + rand() % YNumber;

				// 첫번째 칩인 경우에는, 
				if((i % 2) == 0)
				{
					if(Chip[xpos][ypos].siIndex == -1)
					{
						Chip[xpos][ypos].siIndex = ImagePool[needimagenumber - 1];
						if((i % 4) == 0)		Chip[xpos][ypos].siKind = 0;
						else if((i % 4) == 2)	Chip[xpos][ypos].siKind = 2;
						
						point1.x = xpos;
						point1.y = ypos;
						break;
					}
				}

				// 두번째 칩인 경우에는 .
				if((i % 2) == 1)
				{
					if(Chip[xpos][ypos].siIndex == -1)
					{
						count++;
						point2.x = xpos;
						point2.y = ypos;

						Chip[xpos][ypos].siIndex = ImagePool[needimagenumber - 1];
						if((i % 4) == 1)		Chip[xpos][ypos].siKind = 1;
						else if((i % 4) == 3)	Chip[xpos][ypos].siKind = 3;

						for(j=0;j<MINIGAME2_MAX_YNUMBER;j++)
						{
							for(k=0;k<MINIGAME2_MAX_XNUMBER;k++)
							{
								TempPath[k][j] = FALSE;
							}
						}

						MiniGame2PathIndex = 0;

						if(count > 30 || CheckConnect(point1, point2, -1, 0) == TRUE)		break;
						else																Chip[xpos][ypos].siIndex = -1;
					}
				}
			}

			MiniGame2PathIndex		= 0;
			MiniGame2DrawPathIndex	= 0;


		}

		needimagenumber --;
	}
}

void _MiniGame2::ApplyHack()
{
	// 매크로가 감지되었을 경우 처리
	if(m_bHackMonitor)
	{
		if(rand()%7 != 0)
		{
			SetMiniGameWork(0);
			SetMiniGamePoint(0);
			pMyOnlineWorld->pOnlineMsgBoxError->SetMsg(ON_MSGBOX_TYPE_OK, "", pMyOnlineWorld->pOnlineText->Get(2283));
		}
	}
}

void _MiniGame2::CompareNumber(UI32 uiTextID,UI32 uiEditTextID)
{
	OnlineEditControlEx	*pOecEx = Handler_OnlineEditControlEx::GetControl( m_hOecMiniGame );
	pOecEx->Clear();
	if(uiTextID == uiEditTextID)
	{
		// actdoll (2004/05/05 16:00) : [OECEX]
		m_siSelect = MINIGAME2_NEXT;
		m_siCount = 0;	
	}
	else
	{
		// actdoll (2004/05/05 16:00) : [OECEX]
		// 숫자 입력에 실패한다면 카운트를 증가시킨다.
		m_siCount++;
	}
}