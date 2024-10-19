

#ifndef SOUNDPLAYER_COMMMON_H
#define SOUNDPLAYER_COMMMON_H


struct File_Type
{
	char id[4];
	int  nLength;
};

struct Wav_FmtChunk
{
	unsigned short FormatTag; 
	unsigned short Channels;
	unsigned long  SamplesPerSec;
	unsigned long  AvgBytesPerSec;
	unsigned short BlockAlign;
	unsigned short BitsPerSample; // 원래 Wav Fmt Chunk는 이것이 char[1]형이지만 
								  // 사용의 편리성을 위해 형변환하였다.
};

#endif