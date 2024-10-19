#ifndef _RIDEDATA_H
#define _RIDEDATA_H

#define MAX_CREW_NUMBER 12

class _RideData 
{
public:
	_NewID ID;
	SHORT MaxCrewNumber;
	SHORT CrewNumber;
	_NewID Crew[MAX_CREW_NUMBER];

	void Init(_NewID id);
	BOOL RideOn(_NewID id);
	void RideOut(SHORT number);
	BOOL CanRide();
	SHORT GetCrewNumber();
	SHORT GetMaxCrewNumber();
	void MoveRideOut();
	void MoveRideOutAttack();
	// 승무원들의 위치를 보정해준다. 
	void SetCrewPosition(SHORT x, SHORT y);

	// 총을 가진 탑승 유닛의 수를 구한다. 
    SHORT GetGunUnitCrewNumber();
	// 특정 종류 유닛의 수를 구한다. 
	SHORT GetUnitNumberByKind(SI32 siKind);


};


#endif