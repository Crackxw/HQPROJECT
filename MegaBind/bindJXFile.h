#pragma once

#pragma warning ( disable : 4786 )

#include	<iostream>
#include	<string>
#include	<map>

using namespace std;

//быбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбы
//быбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбыбы
#include "JxHeader.h"


class BindJXFile
{
public:
	typedef map<string, JXHeader, less<string> >	JX_Header;

public:
	BindJXFile()		{ _JxFile = NULL; _pData = NULL; }

	static BindJXFile	*GetInstance();	
	BOOL				openJX( string filename, char *folder = NULL );
	BOOL				create( char *folder, string jx_file );
	BYTE				*GetFile( string filename, unsigned long &size );
	FILE				*GetFile( string filename );
	void				destroy();
	void				Restoration();

private:	
	
	void				findFile( char *path );
	void				makeJxFile( string &filename );
	void				makeJxFile_writeToFile(JX_Header::iterator header);
	long				getTotalFileSize( FILE *fp );
	void				createFolder( string name );

public:
	const char			*GetFolder()	{ return _folder.c_str(); }
	const char			*GetFileName()	{ return _file.c_str(); }

private:
	static BindJXFile	*_bindJxFile;

	FILE				*_JxFile;	
	BYTE				*_pData;
	JX_Header			_headres;
	string				_file, _folder;
};