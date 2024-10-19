#ifndef _CHARDRAWINFO_H
#define _CHARDRAWINFO_H

// ĳ���� ��°� ���õ� Ŭ����, 
class cltCharDrawInfo {
private:
	SI32 			m_siFile;			// ��µǴ� �׸��� ����� ȭ�� ��ȣ 
	SI32 			m_siFont;			// ��µǴ� �׸��� ����� ��Ʈ ��ȣ 

	// ���� ������ ǥ���� �� ����.
	SI32 			m_siVibrateY;       // ������鸲�� ���� 
	BOOL 			m_bVibrateSwitch;   // ���� ��鸲�� �ִ°�?

	BOOL 			m_bDrawSwitch;		// �׷��� ���ΰ� �����ΰ�?
	BOOL 			m_bCharSmallMapDrawSwitch;	// ���� ������ �׷��� ���ΰ� �����ΰ�?

	SI32 			m_siAnimation;				// � ������ �ִϸ��̼��� �����ϴ°�?
	BOOL 			m_bUpdateAnimationSwitch;	// �ִϸ��̼��� �����������θ� �˷��ش�. 
	SI32 			m_siAniStep;                    													// �ִϸ��̼��� �ܰ� 
	SI32			m_siAniDelayor;
	SI32			*m_psiAniStep;
	SI32			m_siAniVary;

	RECT			m_rtRealImageRect;

	SI32 			m_siRealImageXPercent, m_siRealImageYPercent; // ���� �̹����� ���� ���η� �� �� %�� ���ϴ°�?
	BOOL 			m_bTransparentSwitch;					// �����ϰ� ��ġ���� ���θ� �˷��ִ� ���� 
	UI32			m_siDrawFrame;        					// ���������� �׷��� ������ 
	UI32			m_siLastAniFrame;						// ���������� �ִϸ��̼� �� ������

	SI32 			m_siDrawCharInfoStartY;

public:


	SI32 GetFile()const {return m_siFile;}
	void SetFile(SI32 file){m_siFile = file;}

	SI32 GetFont()const {return m_siFont;}
	void SetFont(SI32 font){m_siFont = font;}

	SI32 GetVibrateY()const{return m_siVibrateY;}
	void SetVibrateY(SI32 vib){m_siVibrateY = vib;}

	BOOL GetVibrateSwitch()const{return m_bVibrateSwitch;}
	void SetVibrateSwitch(BOOL mode){m_bVibrateSwitch = mode;}

	void SetDrawSwitch(BOOL mode){m_bDrawSwitch= mode;}
	BOOL GetDrawSwitch()const{return m_bDrawSwitch;}

	void SetCharSmallMapDrawSwitch(BOOL mode){m_bCharSmallMapDrawSwitch= mode;}
	BOOL GetCharSmallMapDrawSwitch()const{return m_bCharSmallMapDrawSwitch;}

	void SetAnimation(SI32 ani){m_siAnimation = ani;}
	SI32 GetAnimation()const{return m_siAnimation;}

	void SetUpdateAnimationSwitch(BOOL mode){m_bUpdateAnimationSwitch = mode;}
	BOOL GetUpdateAnimationSwitch()const{return m_bUpdateAnimationSwitch;}

	void SetAniStep(SI32 anistep){m_siAniStep = anistep;}
	SI32 GetAniStep()const{return m_siAniStep;}

	void SetAniVary(SI32 anivary){m_siAniVary = anivary;}
	SI32 GetAniVary()const{return m_siAniVary;}

	void SetpAniStep(SI32* panistep){m_psiAniStep = panistep;}
	SI32* GetpAniStep()const{return m_psiAniStep;}

	void SetAniDelayor(SI32 anidelayor){m_siAniDelayor = anidelayor;}
	SI32 GetAniDelayor()const{return m_siAniDelayor;}

	RECT GetRealImageRect()const{return m_rtRealImageRect;}
	void SetRealImageRect(RECT rect){m_rtRealImageRect = rect;}

	void SetRealImageXPercent(SI32 xpercent){m_siRealImageXPercent = xpercent;}
	SI32 GetRealImageXPercent()const{return m_siRealImageXPercent ;}

	void SetRealImageYPercent(SI32 ypercent){m_siRealImageYPercent = ypercent;}
	SI32 GetRealImageYPercent()const{return m_siRealImageYPercent ;}

	void SetTransparentSwitch(BOOL mode){m_bTransparentSwitch = mode;}
	BOOL GetTransparentSwitch()const{return m_bTransparentSwitch;}

	void SetDrawFrame(UI32 frame){m_siDrawFrame = frame;}
	UI32 GetDrawFrame()const{return m_siDrawFrame;}

	void SetLastAniFrame(UI32 frame){m_siLastAniFrame = frame;}
	UI32 GetLastAniFrame()const{return m_siLastAniFrame;}

	void SetDrawCharInfoStartY(UI32 starty){m_siDrawCharInfoStartY = starty;}
	UI32 GetDrawCharInfoStartY()const{return m_siDrawCharInfoStartY;}

};


#endif
