#ifndef _RAIN_H
#define _RAIN_H


#define MAX_RAIN_STEP   10

// 수위의 단계 
#define MAX_WATER_LEVEL_STEP  5


class _Rain 
{
public:
	SHORT Step;
	BOOL RainSwitch;       // 비가 오고 있는가 없는가?
	BOOL AutoRainSwitch;   // 자연적으로 비가 오는가?
	SHORT RainDelay;
	SHORT WaterLevel;     // 수위 (수면의 높이 )
	                      // 숫자가 클수록 수위가 높다. (0: 수위가 정상상태)

	DWORD RainFrame;      // 비가 내리기 시작하는 프레임 

	SHORT ThunderSwitch;
	SHORT ThunderStep;

	BOOL CloudSwitch[MAX_CLOUD_NUMBER];
	POINT CloudPos[MAX_CLOUD_NUMBER];  // 구름 
	UI32  CloudFrame[MAX_CLOUD_NUMBER];  // 구름 


	void Init();
	void Draw();
	void SetRainSwitch(BOOL mode, SHORT delay=0);
	void MakeThunder(_NewID myid, SI32 myplayernumber, _NewID attackid, SHORT x, SHORT y, SHORT apower);
	void Action();

};


#endif