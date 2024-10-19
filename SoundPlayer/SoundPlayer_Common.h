

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
	unsigned short BitsPerSample; // ���� Wav Fmt Chunk�� �̰��� char[1]�������� 
								  // ����� ������ ���� ����ȯ�Ͽ���.
};

#endif