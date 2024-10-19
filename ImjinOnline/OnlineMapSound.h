//
// 2004/06/16 Created by ImJunhyug
//
// MapSound��� Ŭ������ ���������, ���������� ���� ���и� �ϰ� �ֽ��ϴ�.
// ���߿� �̸��� �ٲټŵ� ������ڳ׿�. ���� �ٲٰ� �����ø�, �ʿ����ͷ� �ش� ������ ��ǥ�� �����ؾ� �մϴ�.
// ������ �ٰ������� �׸��Ŵٰ� �����Ͻð�, �� ��ǥ �迭�� ���ϼ���.
// Create, AddSoundArea, GetAttrib �Լ��� ���ӿ��� ����Ͻø� �ɰ̴ϴ�.

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

	// iWidth, iHeight : ���� ���� ���� ũ��, iPrecision : ����, 2�ϰ�� 1/2, 3�ϰ�� 1/3, defaultAttrib : ��ü �� �⺻ ��.
	void Create(int iWidth, int iHeight, int iPrecision = 1, SM_ATTRIB defaultAttrib = SMA_NONE);
	// boundNumber : �ٰ����� ��������, mapBound : �� ������ ����, innerPos : �ٰ��� ���� ������ ��. attrib : �ش� �Ӽ�.
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