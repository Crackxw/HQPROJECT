#include <GSL.h>

#include <io.h>
#include <fcntl.h>
#include "OnlineBaseParser.h"


//-----------------------------------------------------------------------------
// Name : Open()
// Desc : ȭ���� ����
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
// Desc : ȭ���� ����
//-----------------------------------------------------------------------------
void HQArielLabelScriptSystem::Close(void)
{
	fclose(fpScript);
}
