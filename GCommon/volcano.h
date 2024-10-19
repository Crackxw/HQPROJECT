#ifndef _VOLCANO   
#define _VOLCANO			1


#define MAX_VOLCANO_NUMBER   10

class _Volcano
{
public:
	BOOL bValidSwitch ;
	SI32 siID;
	SI32 siX, siY;
	BOOL FireBulletSwitch;
	BOOL FireSwitch;
	SI32 FireStartFrame;
	SI32 FireInterval;
	SI32 FireDelay;

	void Init()
	{
		ZeroMemory(this, sizeof(_Volcano));
	};
	
	BOOL Action();

};

void InitVolcano();
void VolcanoAction();
BOOL SetVolcano(SI32 x, SI32 y);

void SaveVolcano(FILE* fp);
void LoadVolcano(FILE* fp);
void VolcanoBlackCheck();

#endif
