#ifndef _CHARDRAWINFO_H
#define _CHARDRAWINFO_H

// 캐릭터 출력과 관련된 클래스, 
class cltCharDrawInfo {
private:
	SI32 			m_siFile;			// 출력되는 그림이 저장된 화일 번호 
	SI32 			m_siFont;			// 출력되는 그림이 저장된 폰트 번호 

	// 공중 유닛을 표현할 때 쓴다.
	SI32 			m_siVibrateY;       // 상하흔들림의 정도 
	BOOL 			m_bVibrateSwitch;   // 상하 흔들림이 있는가?

	BOOL 			m_bDrawSwitch;		// 그려줄 것인가 말것인가?
	BOOL 			m_bCharSmallMapDrawSwitch;	// 작은 지도에 그려줄 것인가 말것인가?

	SI32 			m_siAnimation;				// 어떤 종류의 애니메이션을 수행하는가?
	BOOL 			m_bUpdateAnimationSwitch;	// 애니메이션을 갱신할지여부를 알려준다. 
	SI32 			m_siAniStep;                    													// 애니메이션의 단계 
	SI32			m_siAniDelayor;
	SI32			*m_psiAniStep;
	SI32			m_siAniVary;

	RECT			m_rtRealImageRect;

	SI32 			m_siRealImageXPercent, m_siRealImageYPercent; // 실제 이미지가 가로 세로로 각 몇 %를 점하는가?
	BOOL 			m_bTransparentSwitch;					// 투명하게 비치는지 여부를 알려주는 변수 
	UI32			m_siDrawFrame;        					// 마지막으로 그려준 프레임 
	UI32			m_siLastAniFrame;						// 마지막으로 애니메이션 된 프레임

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
