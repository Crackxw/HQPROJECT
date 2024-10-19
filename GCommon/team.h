#ifndef _TEAM_H
#define _TEAM_H

#define MAX_TEAM_NUMBER   10

class _Team
{
public:
	SHORT CurrentTeamNumber;
	_NewID ID[MAX_TEAM_NUMBER][MAX_SELECTED_CHAR_NUMBER];
	SHORT CharNumber[MAX_TEAM_NUMBER];
	SHORT CenterX[MAX_TEAM_NUMBER], CenterY[MAX_TEAM_NUMBER];


	void Init();
	void Init(SHORT teamnumber);

	// �׷����� �����. 
    void AddTeam(SI32	number);
    void AddTeam(SI32	number, _NewID id);
	// �׷��� �����Ѵ�.
    void SelectTeam(SHORT number);
	// �׷��ִ� ������ �̵��Ѵ�.
    void GotoTeam(SHORT number);

	// ���� ������ ����.
    void DeleteTeamAll(SHORT teamnumber);

	// id�� ������ �����Ѵ�. 
    void DeleteTeam(_NewID id);
	BOOL GetCenter(SHORT number, SHORT&x, SHORT &y);

	// ������ ���� ���Ѵ�. 
    SHORT GetTeamMemberCount(SHORT number);

	BOOL Action();


	// ���� ������ �м��Ѵ�. 
    void GetTeamInfo();

};


#endif