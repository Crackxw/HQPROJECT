
#ifndef __SO_CHALLENGE__
#define __SO_CHALLENGE__

#define MAX_CHALLENGE_LEVEL   30

struct sChallengeLevel
{
	
};

class CChallengeMode
{
public:
	CChallengeMode(SOWorld *pWorld);
	~CChallengeMode();
private:
	static SOWorld *m_pWorld;
	sChallengeLevel m_sChallengeLevel[MAX_CHALLENGE_LEVEL];



};

#endif