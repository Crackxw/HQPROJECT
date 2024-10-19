/************************************************************************************************
	파일명 : clNumberRender.h
	담당자 : 이윤석
************************************************************************************************/
#ifndef	___NUMBER_RENDER___
#define ___NUMBER_RENDER___


#define MAXNUMBER					64
#define NUMBERTYPE_UP				1				// 올라가는 숫자 출력(처음 커지면서 나오다가, 일정크기가 돼면 줄어들면서 사라진다.)
#define NUMBERTYPE_DEMAGE			2				// 데미지용 숫자 출력(처음 커지면서 나오다가, 일정크기가 돼면 줄어들면서 사라진다.)
#define NUMBERTYPE_AUCTION          3               // 경매용 숫자 출력(처음에 엄청큼 점점 원래크기로)


typedef struct
{
	SI16		m_Flag;
	SI16		m_Type;
	SI16		m_Frame;
	SI32		m_PosX;
	SI32		m_PosY;
	float		m_Scale;
	char		m_NumberString[32];
	void		(*m_ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* DesX, SI32* lpDesY);
} SNumberRenderData;


// 숫자 출력 Class
class clNumberRender
{
public:
	SNumberRenderData		m_NumberRenderData[MAXNUMBER];
	XSPR					m_NumberImage;
	LPDIRECTDRAWSURFACE7	m_lpDestSurface;

public:
	clNumberRender();																		// Constructor
	virtual ~clNumberRender();																// Destroyer

	BOOL		Create(void);																// Create
	void		Release(void);																// Release

	void		ClearAllNumber(void);														// Clear All Number
	BOOL		AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, char* lpNumberString, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* DesX, SI32* lpDesY));	// Add Number
	BOOL		AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, SI32 Number, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* DesX, SI32* lpDesY));			// Add Number
	BOOL		AddNumber(SI32 Type, SI32 x, SI32 y, float Scale, __int64 Number, void (*ConvertCodFuc)(SI32 SrcX, SI32 SrcY, SI32* DesX, SI32* lpDesY));		// Add Number

	BOOL		Prepare(void);																// Prepare
	BOOL		Render(LPDIRECTDRAWSURFACE7 lpSurface);										// Render

protected:
	BOOL		RenderSurface(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAWSURFACE7 lpSrcSurface, SI32 x, SI32 y, SI32 Size, float Scale);	// Render Surface
};	


#endif
