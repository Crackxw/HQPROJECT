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

	BOOL  IsWarDay(int nDayOfWeek);               //공성하는 요일인지 검사.	
	BOOL  IsWarTime(int nTime);                   //공성하는 시간인지 검사.  
	BOOL  IsWarCheck(int nDayOfWeek, int nTime);  //공성하는 요일, 시간인지 체크.

	short GetWarStart()	{return siWarStart;};
	short GetWarEnd()		{return siWarEnd;};
	
	short GetWarUpdate()	{return siWarUpdate;};
	
	short GetPopulationStart(){return siPopulationStart;};
	short GetPopulationEnd()  {return siPopulationEnd;};

	short GetInvestUpdateStart(){return siInvestUpdateStart;};
	short GetInvestUpdateEnd(){return siInvestUpdateEnd;};
};

#endif
