#ifndef DECODE_H
#define DECODE_H

class Decode{
public:
	Decode();
	~Decode();

	virtual bool CheckDecode();
	virtual ReadHead( unsigned char* buff );
};


#endif