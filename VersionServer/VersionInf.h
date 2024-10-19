#ifndef VERSIONINF_H
#define VERSIONINF_H

// HQTools\\GSAutopatch Project������ ����մϴ�.	// 2004/10/5 17:6

enum {
	AUTOPATCH_FILE = 0,
	GERSANG_FILE = 1,
	MAX_FILE,
};

// ���� �� FTP ��ġ ���� ��� ���� ����ü
// �Ż� Ŭ���̾�Ʈ�� �Ż� ��ó ��ġ ���� ������ �����Ѵ�.
typedef struct {
	int  version;					// �ֽ� ����(�Ż� Ŭ���̾�Ʈ, �Ż� ��ó)
	char patchPath[MAX_FTP_PATH];	// ��ġ ���� ���(�Ż� Ŭ���̾�Ʈ��, �Ż� ��ó��)
} FileInf;

// FTP ���� ���� ���� ���� ����ü
typedef struct {
	BYTE myIP[MAX_HOSTLEN];	// FTP ���� �ּ�
	BYTE myID[MAX_IDLEN];	// FTP ���� ID
	BYTE myPW[MAX_PWLEN];	// FTP ���� ��й�ȣ
} FTP_Inf;

#define MAX_FTP_NUMBER 3

// ���� �������� �Ľ�(VServer.txt)�ؼ� ���� �Ż� ��ó�� ������ ��ġ ���� ����ü
typedef struct {
	WORD	ftpInfRealNumber;						// ���� �����Ǵ� FTP ���� ����
	FTP_Inf ftpInf[MAX_FTP_NUMBER];					// ���� ������ FTP ���� ���� ���
	BYTE	szFrontServerAddress[MAX_HOSTLEN];		// ����Ʈ ���� �ּ�. robypark 2004/10/5 16:27
	FileInf fileInf[MAX_FILE];						// �Ż� Ŭ���̾�Ʈ�� �Ż� ��ó�� ���� �� FTP ��ġ ��� ������ ���� �����Ѵ�.
} VersionInf;

// ScriptParser.h�� �����ϴ� ����ü�� VersionInf.h�� �̵�
// �Ż� ��ó�� �������ִ� ��ġ���� ���� ����ü
typedef struct {
	FTP_Inf ftpInf;
	BYTE	szFrontServerAddress[MAX_HOSTLEN];		// ����Ʈ ���� �ּ�. robypark 2004/10/5 16:27
	FileInf fileInf[MAX_FILE];
} Version3;

#endif