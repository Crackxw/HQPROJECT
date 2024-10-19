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

	// 그룹으로 만든다. 
    void AddTeam(SI32	number);
    void AddTeam(SI32	number, _NewID id);
	// 그룹을 선택한다.
    void SelectTeam(SHORT number);
	// 그룹있는 곳으로 이동한다.
    void GotoTeam(SHORT number);

	// 팀을 깨끗이 비운다.
    void DeleteTeamAll(SHORT teamnumber);

	// id를 팀에서 제거한다. 
    void DeleteTeam(_NewID id);
	BOOL GetCenter(SHORT number, SHORT&x, SHORT &y);

	// 팀원의 수를 구한다. 
    SHORT GetTeamMemberCount(SHORT number);

	BOOL Action();


	// 팀의 정보를 분석한다. 
    void GetTeamInfo();

};


#endif