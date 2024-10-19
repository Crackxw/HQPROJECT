#include <GSL.h>

#include <main.h>
#include <findpath5.h>
#include <CharOrg.h>


BOOL _Char::SetPatrol()
{

	WorkStep = WS_DECIDE_LENGTH_TO_DEST;

	return TRUE;
}

BOOL _Char::Patrol()
{
	SHORT status;

	SHORT destx=LOWORD(Para1);
	SHORT desty=HIWORD(Para1);

	SHORT srcx=Para2.ID;
	SHORT srcy=Para2.Unique;

	switch(WorkStep)
	{
	  case WS_DECIDE_LENGTH_TO_DEST:    // 목표까지의 거리를 결정한다. 
		  SetMove(destx, desty);
		  WorkStep=WS_GOTO_TARGET;
		  break;

	  case WS_GOTO_TARGET:

		status=Move(BASIC_MOVE_BLOCK_COUNT);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}

		   // 목적지에 도착했으면
		   if(status==1)
		   {
			   WorkStep=WS_DECIDE_LENGTH_TO_SRC;
		   }
		break;

	  case WS_DECIDE_LENGTH_TO_SRC:
		  SetMove(srcx, srcy);
		  WorkStep=WS_GOTO_SRC;
		  break;

	  case WS_GOTO_SRC:
		status=Move(BASIC_MOVE_BLOCK_COUNT);
		if(MoveDone())
			m_scBusySwitch=FALSE;
		else
		{
			AttackGiveUpDelayor=0;
			m_scBusySwitch=TRUE;
		}

		   // 목적지에 도착했으면
		   if(status==1)
		   {
			   WorkStep=WS_DECIDE_LENGTH_TO_DEST;
		   }
		break;

	}

	return TRUE;
}
