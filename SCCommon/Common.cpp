#include <GSL.h>
#include <Common.h>

//───────────────────────────────────────────────────────
// 깨진 multibyte character가 있는지 체크한다. 사용가능한 스트링이면 TRUE, 아니면 FALSE
//───────────────────────────────────────────────────────
bool	CheckLeadByte( char *str )
{	
	int		i = 0;

	while( str[i] != NULL )
	{
		if( IsDBCSLeadByte(str[i]) )		// multibyte character일 경우
		{
			if( str[++i] == NULL )		return	false;
		}
		i++;
	}

	return true;
}


//───────────────────────────────────────────────────────
// 홑따옴표 1개를 2개로 바꿔준다. pDest의 크기는 pSrc의 크기보다 충분히(2배이상) 커야한다.
//───────────────────────────────────────────────────────
void	MakeDblQuote( char* pSrc, char* pDest )
{
	int		i = 0, j = 0;

	while( pSrc[i] != NULL )
	{
		if( pSrc[i] == '\'' )
		{
			pDest[j]	=	'\'';
			pDest[j+1]	=	'\'';
			i++;
			j += 2;
		}
		else
		{
			pDest[j] = pSrc[i];
			i++;
			j++;
		}
	}
	pDest[j] = NULL;
}


//───────────────────────────────────────────────────────
// 파일로 정보를 남긴다.
//───────────────────────────────────────────────────────
/*
VOID	WriteInfoToFile(char *FileName, const char *Content,...)
{
	char	Buf[1024];
	char	DateBuf[128];
	char	TimeBuf[128];
	int		Count;
	FILE *fp;
	va_list	arglist;
	va_start(arglist,Content);
	Count = vsprintf(Buf, Content, arglist);
	fp	=	fopen(FileName, "at");
	_strtime( TimeBuf );
	_strdate( DateBuf );
	fprintf(fp,"[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
	fclose(fp);
	va_end(arglist);
}
*/