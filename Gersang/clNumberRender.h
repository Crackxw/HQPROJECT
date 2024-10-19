/************************************************************************************************
	���ϸ� : clNumberRender.h
	����� : ������
************************************************************************************************/
#ifndef	___NUMBER_RENDER___
#define ___NUMBER_RENDER___


#define MAXNUMBER					64
#define NUMBERTYPE_UP				1				// �ö󰡴� ���� ���(ó�� Ŀ���鼭 �����ٰ�, ����ũ�Ⱑ �Ÿ� �پ��鼭 �������.)
#define NUMBERTYPE_DEMAGE			2				// �������� ���� ���(ó�� Ŀ���鼭 �����ٰ�, ����ũ�Ⱑ �Ÿ� �پ��鼭 �������.)
#define NUMBERTYPE_AUCTION          3               // ��ſ� ���� ���(ó���� ��ûŭ ���� ����ũ���)


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


// ���� ��� Class
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
