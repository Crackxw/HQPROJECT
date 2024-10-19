#ifndef	SO_UTILITY_HEADER
#define SO_UTILITY_HEADER

//--------------------------------------------------------------------------
// Name: HQChannelCollection CLASS
// Desc: 채널 객체 ( 플레이어들을 체널에 등록시킨다.) 
//--------------------------------------------------------------------------
#include <Directives.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>

//#define		szDirPath	"..\\Server Log\\"

class SOUtility
{
public:

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 디렉토리 자동 생성 : 모든 서버에 디렉토리 생기면 필요하지 않음.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
	inline void CreateSvrDir(void)
	{

		if (CreateDirectory(szDirPath, NULL) == 0 )
		{ 
			DWORD dw = GetLastError(); 
			if ( dw == ERROR_ALREADY_EXISTS )
			{
				printf("Already exist directory.\n"); 
			}
			else
			{
				printf("Couldn't create new directory[%u].\n",dw); 
			}
		}
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// SOServer::WriteInfo()를 대체하기 위해 작성
	// 서버의 통계 정보를 쓴다.
	// WriteLog 사용시 주의점 : FileName에는 파일명만 쓰세요. 디렉토리명은 안 됩니다.
	// 생성위치 ==> ..\Server Log\2003 10 Command.txt
	// 예) WriteLog("Command.txt","%s","Error 발생"); 
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	inline void WriteLog(char *FileName, const char *Content,...)
	{

		char	Buf[1024];
		char	FilePath[1024];
		char	DateBuf[128];
		char	TimeBuf[128];
		int		Count;
		FILE	*fp;
		struct	tm *today;

		char	thismonth[128];
		time_t	ltime;


		va_list	arglist;
		va_start(arglist,Content);

		// Use strftime to build a customized time string. 
		time(&ltime);
		today = localtime( &ltime );
		strftime( thismonth, 128, "%Y%m", today );


	//	printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
		sprintf( FilePath, "%s%s %s", szDirPath, thismonth, FileName);

		Count = vsprintf(Buf, Content, arglist);

		fp	=	fopen(FilePath, "at");

		_strtime( TimeBuf );
		_strdate( DateBuf );
		fprintf(fp,"[%s %s] %s\n",DateBuf,TimeBuf,Buf);
		printf("[%s %s] %s\n",DateBuf,TimeBuf,Buf);
		fclose(fp);
		va_end(arglist);
	}
*/

/*
	// 정수 문자열을 정수로 바꾼다.
	inline SI32 StringToInt(const char * cStrInt){UI16 iCount=0;SI32 iNumber=0;char iNegative=0;for(iCount=0;iCount<strlen(cStrInt);iCount++){if(cStrInt[iCount]=='-')iNegative=1;if(isdigit(cStrInt[iCount])){iNumber*=10;iNumber+=(cStrInt[iCount])-48;}}if(iNegative) iNumber=-iNumber;return iNumber;};
	// Hex 문자열을  정수로 바꾼다.
	//inline UI32 HexStringToInt(const char *cID){UI16 iCountA;UI32 iNumber=0;for (iCountA=0;iCountA<strlen(cID);iCountA++) { iNumber *= 16;if(isdigit(cID[iCountA]))	iNumber += (cID[iCountA])-48;if((cID[iCountA]>=65) && (cID[iCountA]<=70)) iNumber += (cID[iCountA])-65+10;if((cID[iCountA]>=97) && (cID[iCountA]<=102)) iNumber += (cID[iCountA])-97+10;}if (cID[0]=='-') iNumber=-iNumber;return iNumber;};
	// 두 정수 사이의 랜덤값을 얻어온다.
	//inline UI32 GetRandNo(UI32 iFirst,UI32 iSecond){R32 iRand = (((R32)rand())/((R32)RAND_MAX));iRand = (iRand*((max(iSecond,iFirst)-min(iSecond,iFirst))+1))+min(iSecond,iFirst);return ((UI32)iRand);};
/////////////////////////////////////////////////////////////////////////////////////////////
// 로그파일을 만든다.
	inline void ClearLogFile(char *outfile)
	{
#ifdef _LOGFILE_ 
		FILE *fpLogFile; fpLogFile=fopen(outfile, "w");fclose(fpLogFile);
#endif
	};

	inline void LogFile(char *outfile, char *str, ...)
	{
#ifdef _LOGFILE_ 
		static SI32 uiAddCount=0;
		FILE *fpLogFile;
		char szBuffer[1024];
		char *pArguments;
		if(uiAddCount > ON_MAX_LOG_LINE) 
		{
			fpLogFile  = fopen(outfile,"w");
			uiAddCount = 0;
		}
		else
		{
			fpLogFile = fopen(outfile,"a");
		}
		if(fpLogFile==NULL) 
			return;
		pArguments=(char *)&str+sizeof(str);
		vsprintf(szBuffer,str,pArguments);

		fprintf(fpLogFile,szBuffer);
		uiAddCount++;
		fclose(fpLogFile);
#endif
	};
// 문자열을 16진수로 바꿔서 출력한다.
	inline VOID LogDataString(char *outfile, const char *cBuf,UI16 uiSize, UI32 uiPlayer, SI32 siPhase)
	{
		UI32 iCount; 
		LogFile(outfile, "Player[%d], Phase[%d]\n",uiPlayer, siPhase);
		for(iCount=0;iCount<uiSize;iCount++)
		{
			LogFile(outfile,"[%2X]",(UI08)cBuf[iCount]);
		}
		LogFile(outfile,"\n\n");
	};
/////////////////////////////////////////////////////////////////////////////////////////////
	// 문자열을 16진수로 화면에 출력한다.
	inline void DisplayStringToHex(char *msg, void *data, int length)
	{
		int i;
		printf("%s : ", msg);
		for(i=0; i<length; i++)
		{
			printf("[%2x]", ((unsigned char*)data)[i]);
		}
		printf("\n");
	};

	// 문자열을 16진수로 파일에 출력한다.
	inline void WriteStringToHex(char *filename, char *msg, void *data, int length)
	{
		FILE *fp;
		int i;
		fp = fopen(filename, "a");
		fprintf(fp, "%s : ", msg);
		for(i=0; i<length; i++)
		{
			fprintf(fp, "[%2x]", ((unsigned char*)data)[i]);
		}
		fprintf(fp,"\n");
		fclose(fp);
	};

#ifdef _WIN32
	inline UI32 GetClock() {return clock();};
#endif
#ifdef __linux__
	inline UI32 GetClock() {
		struct timeval t;gettimeofday(&t,NULL);
		return ((t.tv_sec * CLOCKS_PER_SEC) + t.tv_usec);
	};
#endif
*/

};
#endif