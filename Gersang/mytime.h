#ifndef _MYTIME_H
#define _MYTIME_H

#define MAX_MINUTE 540
#define MAX_HOUR   16

class _Time{
public:
	int Day;
	int OldFont;
	int Hour;
	int Minute;
	int PalVary, OldPalVary;
	int TimeStopSwitch;
	int DaySwitch;

	void Init();
	void SetTimeStop();
	void PassTime();
	void DrawTime(LPDIRECTDRAWSURFACE7 psurface);

};

#endif
