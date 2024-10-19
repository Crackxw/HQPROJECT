////////////////////////////////////////////////////
//  ���� ȭ�� ���� 
////////////////////////////////////////////////////

#include <GSL.h>

#include <main.h>

OPENFILENAME MapFileOpen;     // ���� ȭ�Ͽ� 


void PopMapFileInitialize(HWND hwnd)
{
	static char szFilter[]="map file(*.,map)\0*.map\0" \
		                   "total file(*.*)\0*.*\0\0";

	MapFileOpen.lStructSize=sizeof(OPENFILENAME);
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.hInstance=NULL;
	MapFileOpen.lpstrFilter=szFilter;
	MapFileOpen.lpstrCustomFilter=NULL;
	MapFileOpen.nMaxCustFilter=0;
	MapFileOpen.nFilterIndex=0;
	MapFileOpen.lpstrFile=NULL;
	MapFileOpen.nMaxFile=_MAX_PATH;
	MapFileOpen.lpstrFileTitle=NULL;
	MapFileOpen.nMaxFileTitle=_MAX_FNAME+_MAX_EXT;
	MapFileOpen.lpstrInitialDir=NULL;
	MapFileOpen.lpstrTitle=NULL;
	MapFileOpen.Flags=0;
	MapFileOpen.nFileOffset=0;
	MapFileOpen.nFileExtension=0;
	MapFileOpen.lpstrDefExt="map";
	MapFileOpen.lCustData=0L;
	MapFileOpen.lpfnHook=NULL;
	MapFileOpen.lpTemplateName=NULL;
}

//// ����ȭ�� �ҷ�����
BOOL PopMapFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.lpstrFile=pstrFileName;
	MapFileOpen.lpstrFileTitle=pstrTitleName;
	MapFileOpen.Flags=OFN_HIDEREADONLY|OFN_CREATEPROMPT;

	return GetOpenFileName(&MapFileOpen);

}

//// ����ȭ�� �����ϱ� 
BOOL PopMapFileSaveDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
	MapFileOpen.hwndOwner=hwnd;
	MapFileOpen.lpstrFile=pstrFileName;
	MapFileOpen.lpstrFileTitle=pstrTitleName;
	MapFileOpen.Flags=OFN_OVERWRITEPROMPT;
	
	return GetSaveFileName(&MapFileOpen);
}
