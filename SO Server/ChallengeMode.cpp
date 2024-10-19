#include "SOMain.h"
#include "SOWorld.h"
#include "SOPlayer.h"
#include "ChallengeMode.h"


SOWorld *CChallengeMode::m_pWorld = NULL;

CChallengeMode::CChallengeMode(SOWorld *pWorld)
{
	m_pWorld = pWorld;
}

CChallengeMode::~CChallengeMode()
{
	;
	
}