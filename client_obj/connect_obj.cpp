#include "connect_obj.h"


void ConnectObj::Init(struct bufferevent* bev){
	this->mCurrPos = 0;
	this->mMaxLen = 1024;
	this->mStreamBuff = (unsigned char*)malloc(1024);
	this->mBuffev = bev;

}

bool ConnectObj::Decode(int& iType,int& iStartPos,int& iLen){
	int iHeadFlat = 0x0101;
	int iEndFlat  = 0x0202;
	int iTemp = 0;
	int iPos=0;
	if(this->mCurrPos-iPos<4)
		return false;
	//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
	iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
	//printf("1 Session::Decode %d %d\n", iTemp,iHeadFlat);
	if (iTemp == iHeadFlat){
		iPos += sizeof(int);
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
		//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
		iType = iTemp;
		//printf("iType=%d\n", iType);
		iPos += sizeof(int);
		//memcpy(&iTemp,this->mStreamBuff+iPos,sizeof(int));
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos);
		iPos += sizeof(int);
		iLen = iTemp;
		//printf("iLen=%d\n", iLen);
		iStartPos = iPos;
		if (this->mCurrPos-iPos < iLen+sizeof(int)){
			//printf("%s\n", "this->mCurrPos-iPos < iLen+sizeof(int)");
			return false;
		}
		iTemp = Packet::ReadInt(this->mStreamBuff+iPos+iLen);
		//memcpy(&iTemp,this->mStreamBuff+iStartPos+iLen,sizeof(int));
		//printf("iEndFlat %d\n", iTemp);
		if (iTemp != iEndFlat){
			//printf("%s\n", "iTemp != iEndFlat");
			return false;
		}
	}else{
		return false;
	}
	return true;
}

void ConnectObj::OnReadPacket(){
	printf("%s\n", "ConnectObj::OnReadPacket");
	int iSize = this->mMaxLen - this->mCurrPos + 1;
	if(iSize <=0)
		return;
	int iReadLen = bufferevent_read(this->mBuffev,this->mStreamBuff+this->mCurrPos,iSize);
	printf("%s %d\n", "connectObj::OnReadPacket",iReadLen);

	this->mCurrPos += iReadLen;

	bool bIsOk = false;int iType=0;int iCotentLen=0;int iStartPos;
	while( (bIsOk = this->Decode(iType,iStartPos,iCotentLen)))
	{
		printf("%s\n", "bIsOk");
		Packet* packet = new Packet();
		memcpy(packet->mStreamBuff,this->mStreamBuff+iStartPos,iCotentLen);
		packet->mContenLen = iCotentLen;
		printf("%d %d\n", iStartPos,iCotentLen);
		int iLeftLen = this->mCurrPos - 4*sizeof(int) - iCotentLen;
		int iTotalLen = 4*sizeof(int)+iCotentLen;
		if(iLeftLen >0){
			memmove(this->mStreamBuff,this->mStreamBuff+iTotalLen,iLeftLen);
		}
		packet->SetType(iType);
		//packet->SetSession(this);
		this->mCurrPos = this->mCurrPos - iTotalLen;
		//this->mServer->OnRecievePacket(packet);
		this->mPacketBuff.push(packet);
	}
}

void ConnectObj::ExePacket(){
	if(this->mPacketBuff.empty()){
		return;
	}
	printf("%s\n", "ConnectObj::ExePacket");
	std::queue<Packet*>::iterator iter = this->mPacketBuff.front();
	Packet* packet = *iter;
	this->mPacketBuff.pop();
	char* str = (char*) malloc(100);
	//packet->ReadString(str);
	//printf("ConnectObj::ExePacket %s\n", str);
	int iNum = 0;
	packet->ReadInt(iNum);
	printf("%d\n", iNum);
}