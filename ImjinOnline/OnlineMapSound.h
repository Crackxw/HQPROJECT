//
// 2004/06/16 Created by ImJunhyug
//
// MapSound라는 클래스로 만들었지만, 실제적으로 지역 구분만 하고 있습니다.
// 나중에 이름을 바꾸셔도 상관없겠네요. 지역 바꾸고 싶으시면, 맵에디터로 해당 지역의 좌표를 설정해야 합니다.
// 지역은 다각형으로 그리신다고 생각하시고, 각 좌표 배열을 구하세요.
// Create, AddSoundArea, GetAttrib 함수만 게임에서 사용하시면 될겁니다.

#ifndef SOUNDMAP_H
#define SOUNDMAP_H

typedef struct _SM_POINT {
	int x;
	int y;
} SM_POINT;


typedef unsigned char SM_ATTRIB;

#define SMA_NONE	0x00
#define SMA_TOUCH	0x01
#define SMA_KOREA	0x10
#define SMA_JAPAN	0x20
#define SMA_TAIWAN	0x40
#define SMA_CHINA	0x80

class OnlineMapSound {
private :
	int m_iWidth, m_iHeight;
	int m_iPrecision;
	SM_ATTRIB *m_pSoundMap;

private:
	void LinePP(SM_POINT a, SM_POINT b, SM_ATTRIB attrib);
	void FillPP(SM_POINT pos, SM_ATTRIB attrib);
	void SetAttribPP(SM_POINT pos, SM_ATTRIB attrib);
	void SetAttribPPInversePos(SM_POINT pos, SM_ATTRIB attrib);
	SM_ATTRIB GetAttribPP(SM_POINT pos);

public :
	OnlineMapSound( void );
	virtual ~OnlineMapSound();

	// iWidth, iHeight : 맵의 가로 세로 크기, iPrecision : 배율, 2일경우 1/2, 3일경우 1/3, defaultAttrib : 전체 맵 기본 값.
	void Create(int iWidth, int iHeight, int iPrecision = 1, SM_ATTRIB defaultAttrib = SMA_NONE);
	// boundNumber : 다각형의 꼭지점수, mapBound : 각 꼭지점 갯수, innerPos : 다각형 내부 임의의 점. attrib : 해당 속성.
	bool AddSoundArea(int boundNumber, SM_POINT *mapBound, SM_POINT innerPos, SM_ATTRIB attrib);
	void Release(void);

	bool IsCreate(void);
	bool InMapRange(SM_POINT pos);
	SM_ATTRIB GetAttrib(SM_POINT pos);

#ifdef __AFX_H__
	void TestDraw(CDC *pDC);
#endif
};

#endif