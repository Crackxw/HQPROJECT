#ifndef	WAR_TIME_PARSER_H
#define WAR_TIME_PARSER_H


#include <windows.h>
#include "../SCCommon/GSCDefine.h"

#define  MAX_WAR_DAY           2
class	CWarTimeParser
{
private:
	
	short   siWarDay[MAX_WAR_DAY];
	short	siWarStart;
	short	siWarEnd;

	short	siWarUpdate;

	short	siPopulationStart;
	short	siPopulationEnd;


	short	siInvestUpdateStart;
	short	siInvestUpdateEnd;

public:
	CWarTimeParser();
	~CWarTimeParser();
	void Load(char* szFineName);

	BOOL  IsWarDay(int nDayOfWeek);               //�����ϴ� �������� �˻�.	
	BOOL  IsWarTime(int nTime);                   //�����ϴ� �ð����� �˻�.  
	BOOL  IsWarCheck(int nDayOfWeek, int nTime);  //�����ϴ� ����, �ð����� üũ.

	short GetWarStart()	{return siWarStart;};
	short GetWarEnd()		{return siWarEnd;};
	
	short GetWarUpdate()	{return siWarUpdate;};
	
	short GetPopulationStart(){return siPopulationStart;};
	short GetPopulationEnd()  {return siPopulationEnd;};

	short GetInvestUpdateStart(){return siInvestUpdateStart;};
	short GetInvestUpdateEnd(){return siInvestUpdateEnd;};
};

#endif
