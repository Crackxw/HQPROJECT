#ifndef _MOVEMARK_H
#define _MOVEMARK_H

class _MoveMark {
	SHORT X, Y;   // 이동 표시의 위치 
	int RealX, RealY;

	SHORT MoveMarkSwitch;
	SHORT Step;
	SHORT MaxStep;

	public:
	void Init();
	void DrawMoveMark();
	void Action();
	void SetMoveMark(SHORT x, SHORT y);
	// real 좌표로 설정한다. 
    void SetMoveMarkReal(SHORT x, SHORT y);

};


class _WarningMark {
public:
	SHORT X, Y;   // 이동 표시의 위치 
	SHORT WarningMarkSwitch;
	SHORT Step;
	SHORT MaxStep;

	public:
	void Init();
	void DrawWarningMark();
	void Action();
	void SetWarningMark(SHORT x, SHORT y);
};


#endif