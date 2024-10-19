#include <GSL.h>

#include <io.h>
#include <fcntl.h>
#include "OnlineBaseParser.h"


//-----------------------------------------------------------------------------
// Name : Open()
// Desc : 화일을 연다
//-----------------------------------------------------------------------------
BOOL HQArielLabelScriptSystem::Open(char * filename)
{
	if ( (fpScript= fopen(filename, "rb")) == NULL )	return FALSE;
	fsize = filelength(fileno(fpScript));

	if ( !PreProcess() ) return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name : Close()
// Desc : 화일을 연다
//-----------------------------------------------------------------------------
void HQArielLabelScriptSystem::Close(void)
{
	fclose(fpScript);
}
