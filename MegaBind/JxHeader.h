#pragma once

class JXHeader
{
public:
	JXHeader()	{}

	void		create( unsigned long size, unsigned long pos ) { _nfileSize = size ;_nfilePosinJx = pos; }
	
	unsigned long	&GetFileSize()	{ return _nfileSize; };
	unsigned long	&GetFilePos()	{ return _nfilePosinJx; };
	unsigned long	&GetCompress()	{ return _nCompressSize; };

	void			SetCompressSize( unsigned long &size ) { _nCompressSize = size; }
	

private:	
	unsigned long		_nfileSize;
	unsigned long		_nfilePosinJx;
	unsigned long		_nCompressSize;
};