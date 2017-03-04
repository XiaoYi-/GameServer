#include "Decode.h"

class YDecode :public Decode
{
public:
	YDecode();
	~YDecode();
	//virtual int ReadHead(unsigned char* buff,size_t iBLen);
	virtual bool CheckDecode(unsigned char* buff,size_t iBLen){
		//int iLen = ReadHead(buff);
		if(iLen > iBLen)
	}
};