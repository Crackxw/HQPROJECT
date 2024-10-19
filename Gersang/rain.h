#ifndef _RAIN_H
#define _RAIN_H


#define MAX_RAIN_STEP   10

// ������ �ܰ� 
#define MAX_WATER_LEVEL_STEP  5


class _Rain 
{
public:
	SHORT Step;
	BOOL RainSwitch;       // �� ���� �ִ°� ���°�?
	BOOL AutoRainSwitch;   // �ڿ������� �� ���°�?
	SHORT RainDelay;
	SHORT WaterLevel;     // ���� (������ ���� )
	                      // ���ڰ� Ŭ���� ������ ����. (0: ������ �������)

	DWORD RainFrame;      // �� ������ �����ϴ� ������ 

	SHORT ThunderSwitch;
	SHORT ThunderStep;

	BOOL CloudSwitch[MAX_CLOUD_NUMBER];
	POINT CloudPos[MAX_CLOUD_NUMBER];  // ���� 
	UI32  CloudFrame[MAX_CLOUD_NUMBER];  // ���� 


	void Init();
	void Draw();
	void SetRainSwitch(BOOL mode, SHORT delay=0);
	void MakeThunder(_NewID myid, SI32 myplayernumber, _NewID attackid, SHORT x, SHORT y, SHORT apower);
	void Action();

};


#endif