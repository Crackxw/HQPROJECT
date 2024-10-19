#ifndef VERSIONINF_H
#define VERSIONINF_H

// HQTools\\GSAutopatch Project에서도 사용합니다.	// 2004/10/5 17:6

enum {
	AUTOPATCH_FILE = 0,
	GERSANG_FILE = 1,
	MAX_FILE,
};

// 버전 및 FTP 패치 파일 경로 정보 구조체
// 거상 클라이언트와 거상 런처 패치 관련 정보를 저장한다.
typedef struct {
	int  version;					// 최신 버전(거상 클라이언트, 거상 런처)
	char patchPath[MAX_FTP_PATH];	// 패치 파일 경로(거상 클라이언트용, 거상 런처용)
} FileInf;

// FTP 서버 접속 관련 정보 구조체
typedef struct {
	BYTE myIP[MAX_HOSTLEN];	// FTP 서버 주소
	BYTE myID[MAX_IDLEN];	// FTP 접속 ID
	BYTE myPW[MAX_PWLEN];	// FTP 접속 비밀번호
} FTP_Inf;

#define MAX_FTP_NUMBER 3

// 버전 서버에서 파싱(VServer.txt)해서 얻은 거상 런처에 전달할 패치 정보 구조체
typedef struct {
	WORD	ftpInfRealNumber;						// 실제 제공되는 FTP 서버 개수
	FTP_Inf ftpInf[MAX_FTP_NUMBER];					// 접속 가능한 FTP 서버 정보 목록
	BYTE	szFrontServerAddress[MAX_HOSTLEN];		// 프론트 서버 주소. robypark 2004/10/5 16:27
	FileInf fileInf[MAX_FILE];						// 거상 클라이언트와 거상 런처의 버전 및 FTP 패치 경로 정보를 각각 저장한다.
} VersionInf;

// ScriptParser.h에 존재하던 구조체를 VersionInf.h로 이동
// 거상 런처로 전달해주는 패치관련 정보 구조체
typedef struct {
	FTP_Inf ftpInf;
	BYTE	szFrontServerAddress[MAX_HOSTLEN];		// 프론트 서버 주소. robypark 2004/10/5 16:27
	FileInf fileInf[MAX_FILE];
} Version3;

#endif