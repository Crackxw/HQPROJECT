#ifndef _MOVEMARK_H
#define _MOVEMARK_H

class _MoveMark {
	SHORT X, Y;   // �̵� ǥ���� ��ġ 
	int RealX, RealY;

	SHORT MoveMarkSwitch;
	SHORT Step;
	SHORT MaxStep;

	public:
	void Init();
	void DrawMoveMark();
	void Action();
	void SetMoveMark(SHORT x, SHORT y);
	// real ��ǥ�� �����Ѵ�. 
    void SetMoveMarkReal(SHORT x, SHORT y);

};


class _WarningMark {
public:
	SHORT X, Y;   // �̵� ǥ���� ��ġ 
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