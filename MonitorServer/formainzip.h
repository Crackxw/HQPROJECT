#ifndef _FORMAINZIP_H_
#define _FORMAINZIP_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef unix
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
# include <direct.h>
# include <io.h>
#endif

#include "zip.h"

#define WRITEBUFFERSIZE 16384
#define MAXFILENAME 256

#ifdef WIN32
uLong filetime(const char *f, tm_zip *tmzip, uLong *dt)
{
  int ret = 0;
  {
      FILETIME ftLocal;
      HANDLE hFind;
      WIN32_FIND_DATA  ff32;

      hFind = FindFirstFile(f,&ff32);
      if (hFind != INVALID_HANDLE_VALUE)
      {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
        FindClose(hFind);
        ret = 1;
      }
  }
  return ret;
}
#else
#ifdef unix
uLong filetime(char *f, tm_zip *tmzip, uLong *dt)
{
  int ret=0;
  struct stat s;  
  struct tm* filedate;
  time_t tm_t=0;
  
  if (strcmp(f,"-")!=0)
  {
    char name[MAXFILENAME];
    int len = strlen(f);
    strcpy(name, f);
    if (name[len - 1] == '/')
      name[len - 1] = '\0';

    if (stat(name,&s)==0)
    {
      tm_t = s.st_mtime;
      ret = 1;
    }
  }
  filedate = localtime(&tm_t);

  tmzip->tm_sec  = filedate->tm_sec;
  tmzip->tm_min  = filedate->tm_min;
  tmzip->tm_hour = filedate->tm_hour;
  tmzip->tm_mday = filedate->tm_mday;
  tmzip->tm_mon  = filedate->tm_mon ;
  tmzip->tm_year = filedate->tm_year;

  return ret;
}
#else
uLong filetime(char *f, tm_zip *tmzip, uLong *dt)
{
    return 0;
}
#endif
#endif




int check_exist_file(const char* filename)
{
	FILE* ftestexist;
    int ret = 1;
	ftestexist = fopen(filename,"rb");
	if (ftestexist==NULL)
        ret = 0;
    else
        fclose(ftestexist);
    return ret;
}



char* zipfilename;
zipFile zf;
zip_fileinfo zi;
int size_buf=WRITEBUFFERSIZE;
void* buf = NULL;

int err=0;
int dot_found=0;
int errclose;

char temps[6] = "zip.h";
const char* filenameinzip = temps;
FILE * fin;
int size_read;	

void makezipfile(char* filename_try)
{	
	buf = (void*)malloc(size_buf);

	if (check_exist_file(filename_try)!=0)		//같은 이름이 있는지를 체크
	{
		//이 부분으로 들어간다는 것은 같은 이름의 zip 파일이 존재함을 뜻함
		//우선은 없다고 가정하고서리....
	}

	zf = zipOpen(filename_try,0);
	if (zf == NULL)
	{
		//MessageBox(hDlg, "File Name Error", "Error", MB_ICONSTOP|MB_OK);
		err= ZIP_ERRNO;
	}
	else
	{
		//MessageBox(hDlg, "File Make Success", "Success", MB_OK);
	}	
	
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = zi.tmz_date.tm_mday = zi.tmz_date.tm_min = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
	
}

void insert_in_zip(char* filenameinzip)
{
	int opt_compress_level=Z_DEFAULT_COMPRESSION;
	filetime(filenameinzip,&zi.tmz_date,&zi.dosDate);

	err = zipOpenNewFileInZip(zf,filenameinzip,&zi, NULL,0,NULL,0,NULL ,(opt_compress_level != 0) ? Z_DEFLATED : 0,opt_compress_level);

	if (err != ZIP_OK)
	{
		//EndDialog(hDlg, 0);
		//PostQuitMessage(0);				
	}
	else
	{
		fin = fopen(filenameinzip,"rb");
		if (fin==NULL)
		{
			err=ZIP_ERRNO;
			//MessageBox(hDlg, "File name error", "Error", MB_ICONSTOP|MB_OK);
			//EndDialog(hDlg, 0);
			//PostQuitMessage(0);
		}
	}

	if (err == ZIP_OK)
		do
		{
			err = ZIP_OK;
			size_read = fread(buf,1,size_buf,fin);
			if (size_read < size_buf)
			{
				if (feof(fin)==0)
				{
					//MessageBox(hDlg, "Reading error", "Error", MB_ICONSTOP|MB_OK);
					err = ZIP_ERRNO;
				}
			}
			
			if (size_read>0)
			{
				err = zipWriteInFileInZip (zf,buf,size_read);
				if (err<0)
				{
					//MessageBox(hDlg, "Writing error", "Error", MB_ICONSTOP|MB_OK);
				}
			}
		}while ((err == ZIP_OK) && (size_read>0));
		
	fclose(fin);
	
	
	if (err<0)
		err=ZIP_ERRNO;
	else
	{                    
		err = zipCloseFileInZip(zf);
		//if (err!=ZIP_OK)
		//	MessageBox(hDlg, "Closing error", "Error", MB_ICONSTOP|MB_OK);		
	}								
}

void allclose()
{
	errclose = zipClose(zf,NULL);
	//if (errclose != ZIP_OK)
		//MessageBox(hDlg, "Closing error", "Error", MB_ICONSTOP|MB_OK);
	
	free(buf);
}


#endif