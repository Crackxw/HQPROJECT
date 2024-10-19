#include <GSL.h>
#include <Common.h>

//��������������������������������������������������������������������������������������������������������������
// ���� multibyte character�� �ִ��� üũ�Ѵ�. ��밡���� ��Ʈ���̸� TRUE, �ƴϸ� FALSE
//��������������������������������������������������������������������������������������������������������������
bool	CheckLeadByte( char *str )
{	
	int		i = 0;

	while( str[i] != NULL )
	{
		if( IsDBCSLeadByte(str[i]) )		// multibyte character�� ���
		{
			if( str[++i] == NULL )		return	false;
		}
		i++;
	}

	return true;
}


//��������������������������������������������������������������������������������������������������������������
// Ȭ����ǥ 1���� 2���� �ٲ��ش�. pDest�� ũ��� pSrc�� ũ�⺸�� �����(2���̻�) Ŀ���Ѵ�.
//��������������������������������������������������������������������������������������������������������������
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


//��������������������������������������������������������������������������������������������������������������
// ���Ϸ� ������ �����.
//��������������������������������������������������������������������������������������������������������������
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