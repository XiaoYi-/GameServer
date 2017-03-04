#include "Session.h"

Session::Session(){
this->mCurrPos = 0;
this->mMaxLen = MAX_PACKET_LEN * 3;
}

Session::~Session(){
	free(this->mIp);
	this->mIp = 0;
}

bool Session::Decode(int& iType,int& iStartPos,int& iLen){
	int iHeadFlat = 0x0001;
	int iEndFlat  = 0x0002;
	int iTemp = 0;
	int iPos=0;
	if(this->mCurrPos-iPos<4)
		return false;
	//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
	iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
	printf("1 Session::Decode %d %d\n", iTemp,iHeadFlat);
	if (iTemp == iHeadFlat){
		iPos += sizeof(int);
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
		//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
		iType = iTemp;
		printf("iType=%d\n", iType);
		iPos += sizeof(int);
		//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
		iPos += sizeof(int);
		iLen = iTemp;
		printf("iLen=%d\n", iLen);
		iStartPos = iPos;
		if (this->mCurrPos-iPos < iLen+sizeof(int)){
			printf("%s\n", "this->mCurrPos-iPos < iLen+sizeof(int)");
			return false;
		}
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos+iLen);
		//memcpy(&iTemp,this->mStreamBuff+iStartPos+iLen,sizeof(int));
		//printf("iEndFlat %d\n", iTemp);
		if (iTemp != iEndFlat){
			printf("%s\n", "iTemp != iEndFlat");
			return false;
		}
	}else{
		return false;
	}
	return true;
}

void Session::Init(Server* server,struct bufferevent * buffev,evutil_socket_t fd,struct sockaddr *sa,char* sIp,int iPort){
this->mBuffev = buffev;
this->mFd = fd;
this->mSa = sa;
this->mServer = server;
this->mStreamBuff = (unsigned char*)malloc(MAX_PACKET_LEN);
this->mIp = sIp;
this->mPort = iPort;
}

void Session::SendPacket(Packet* packet){
	printf("%s %d\n", "Session::SendPacket",packet->mContenLen);
	static unsigned char buff[10000];
	memcpy(buff,packet->mStreamBuff,packet->mContenLen);
	bufferevent_write(this->mBuffev,(const void *)buff,packet->mContenLen);
}

void Session::OnStreamRecieve(struct bufferevent *buffev, void *arg){

	// 为了避免读取的字节超过　缓存的内存，应该设置bufferevent的读取高水位。
	//这样读取的最大字节就是知道的，避免超出用户定义的缓存内存。

	//用户端字节定义的缓存内存是最大协议的至少两倍，读取高水位是最大协议的一倍
	//，这样就不会导致读取超出客户定义的缓存内存。

	//size_t evbuffer_get_length(const struct evbuffer *buf);

	//size_t bufferevent_read(struct bufferevent *bufev, void *data, size_t size)
	
	int iSize = this->mMaxLen - this->mCurrPos + 1;
	if(iSize <=0)
		return;
	int iReadLen = bufferevent_read(buffev,this->mStreamBuff+this->mCurrPos,iSize);
	printf("%s %d\n", "Session::OnStreamRecieve",iReadLen);

	this->mCurrPos += iReadLen;

	bool bIsOk = false;int iType=0;int iCotentLen=0;int iStartPos;
	while( (bIsOk = this->Decode(iType,iStartPos,iCotentLen)))
	{
		printf("%s\n", "bIsOk");
		Packet* packet = new Packet();
		memcpy(packet->mStreamBuff,this->mStreamBuff+iStartPos,iCotentLen);
		packet->mContenLen = iCotentLen;
		printf("packet->mStreamBuff %d %d\n",iStartPos,iCotentLen);
		int iLeftLen = this->mCurrPos - 4*sizeof(int) - iCotentLen;
		int iTotalLen = 4*sizeof(int)+iCotentLen;
		if(iLeftLen >0){
			memmove(this->mStreamBuff,this->mStreamBuff+iTotalLen,iLeftLen);
		}
		packet->SetType(iType);
		packet->SetSession(this);
		this->mCurrPos = this->mCurrPos - iTotalLen;
		this->mServer->OnRecievePacket(packet);
		break;
	}
}

/*
读取低水位：读取操作使得输入缓冲区的数据量在此级别或者更高时，读取回调将被调用。默认值为0，所以每个读取操作都会导致读取回调被调用。
读取高水位：输入缓冲区中的数据量达到此级别后，bufferevent将停止读取，直到输入缓冲区中足够量的数据被抽取，使得数据量低于此级别。默认值是无限，所以永远不会因为输入缓冲区的大小而停止读取。

写入低水位：写入操作使得输出缓冲区的数据量达到或者低于此级别时，写入回调将被调用。默认值是0，所以只有输出缓冲区空的时候才会调用写入回调。
写入高水位：bufferevent没有直接使用这个水位。它在bufferevent用作另外一个bufferevent的底层传输端口时有特殊意义。请看后面关于过滤型bufferevent的介绍。 
*/