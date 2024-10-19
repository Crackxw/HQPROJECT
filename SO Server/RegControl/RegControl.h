#ifndef REGCONTROL_H
#define REGCONTROL_H

#include "..\SOServer.h"
#include "..\SOWorld.h"
#include "..\SOGate.h"

class SOServer;
class SOGate;
class SOWorld;

class RegControl
{
private:
	HKEY			m_hKey;
	UI32			m_uiHeartBeatCount;

	SOServer	*m_clServer;
	SOGate		*m_clGate;
	SOWorld		*m_clWorld;

public:
	RegControl(SOServer *pServer);
	virtual ~RegControl();

	BOOL		RegKeyInit(VOID);
	BOOL		RegKeyServerInfo(VOID);
	BOOL		RegKeyHeartBeat(SI32 siLoopRate, SI32 siCurrentPlayerNumber, SI32 siMaxPlayerNumber);
	BOOL		RegKeyClose(VOID);
	VOID		SetRegControlReady(BOOL bReady);

};
#endif