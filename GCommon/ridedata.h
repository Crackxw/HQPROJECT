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
	// �¹������� ��ġ�� �������ش�. 
	void SetCrewPosition(SHORT x, SHORT y);

	// ���� ���� ž�� ������ ���� ���Ѵ�. 
    SHORT GetGunUnitCrewNumber();
	// Ư�� ���� ������ ���� ���Ѵ�. 
	SHORT GetUnitNumberByKind(SI32 siKind);


};


#endif