#ifndef _ELECTRONSHOCK_H
#define _ELECTRONSHOCK_H

// ���� ������ ��� ����. 
class cltElectronShock{
private:
	BOOL 			m_bElectronDrawSwitch;
	SI32 			m_siElectronDrawStep;
	SI32 			m_siElectronDrawRepeat;


public:
	cltElectronShock();

	void SetElectronDraw(BOOL mode);

	void Action();

	// ���� ������ ����� �׷��ش�. 
    void Draw(POINT drawstartpoint, RECT realimagerect);

};


#endif
