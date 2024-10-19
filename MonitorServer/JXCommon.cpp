#include "jxdef.h"
#include "time.h"

HANDLE h = CreateMutex( NULL, FALSE, NULL );

void	writeInfoToFile(bool timeStamp, char* filename, char* str, ...)
{

	::WaitForSingleObject( h, INFINITE );

	if( str )
	{
		char body[1024];
		memset(body,0,sizeof(body));
		wvsprintf(body, str, (LPSTR)(&str+1) );

		FILE* fp;		
		if( (fp = fopen(filename,"at")) != NULL )		
		{
			if( timeStamp )
			{
				char time[128];
				char date[128];

				_strtime( time );
				_strdate( date );

				fprintf(fp,"[%s %s] %s\n", date, time, body);
			}
			else
			{
				fprintf(fp,"%s\n", body);
			}

			fclose(fp);
		}
	}

	::ReleaseMutex( h );
}
