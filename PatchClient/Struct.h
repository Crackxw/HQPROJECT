


#ifndef __PATCH_TOOL_STRUCT_H__
#define __PATCH_TOOL_STRUCT_H__

#define SERVER_NAME_MAX_SIZE			128
#define JOB_SUBSTANCE_MAX_SIZE			1024
#define JOB_APPLYSERVER_MAX_SIZE		2048
#define JOB_MAKDER_MAX_SIZE				26
#define JOB_DATE_MAX_SIZE				128

#define JOB_MAKER_SEARCH_TYPE			1
#define JOB_DATE_SEARCH_TYPE			2
#define JOB_SEARCH_MAX_SIZE				50

struct S_PATCH_INFO{
	int		nJobID;
	char    szMaker[JOB_MAKDER_MAX_SIZE];
	char	szSubstance[JOB_SUBSTANCE_MAX_SIZE];
	char	szApplyServer[JOB_APPLYSERVER_MAX_SIZE];
	char    szDate[JOB_DATE_MAX_SIZE];
};

typedef S_PATCH_INFO *PS_PATCH_INFO;

#endif