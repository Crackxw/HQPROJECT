#ifndef _ELECTRONSHOCK_H
#define _ELECTRONSHOCK_H

// 번개 감전된 모습 정보. 
class cltElectronShock{
private:
	BOOL 			m_bElectronDrawSwitch;
	SI32 			m_siElectronDrawStep;
	SI32 			m_siElectronDrawRepeat;


public:
	cltElectronShock();

	void SetElectronDraw(BOOL mode);

	void Action();

	// 전기 감전된 모습을 그려준다. 
    void Draw(POINT drawstartpoint, RECT realimagerect);

};


#endif
