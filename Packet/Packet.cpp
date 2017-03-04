#include "Packet.h"

Packet::Packet(){
	this->mContenLen == 0;
	this->mCurrPos = 0;
}
Packet::~Packet(){}

void Packet::SetId(int id){
	this->mId = id;
}

void Packet::SetSession(Session* sesseion){
	this->mSession = sesseion;
}

int Packet::GetType(){
	return this->mType;
}

void Packet::SetType(int type){
	this->mType = type;
}

void Packet::PushInt(int& i){

	unsigned char i1 = (i >> 24) & 0x00ff;
	unsigned char i2 = (i >> 16) & 0x00ff;
	unsigned char i3 = (i >> 8) & 0x00ff;
	unsigned char i4 = (i >> 0) & 0x00ff;

	this->mStreamBuff[this->mContenLen] = i1;this->mContenLen++;
	this->mStreamBuff[this->mContenLen] = i2;this->mContenLen++;
	this->mStreamBuff[this->mContenLen] = i3;this->mContenLen++;
	this->mStreamBuff[this->mContenLen] = i4;this->mContenLen++;

	//memcpy(this->mStreamBuff +this->mContenLen, &i, sizeof(int));
	//this->mContenLen += sizeof(int);
}


int Packet::ReadInt(int& i){
	if(this->mCurrPos > (this->mContenLen - sizeof(int)) )
	{
		printf("%s %d %d\n", "ReadInt error!",this->mCurrPos,int(this->mContenLen - sizeof(int)));
		//return ;
	}
	
	int ii =0;
	/*memcpy(&ii,this->mStreamBuff+this->mCurrPos,sizeof(int));
	this->mCurrPos += sizeof(int);
	i = ii;
	return ii;*/

	unsigned char i1 = this->mStreamBuff[this->mCurrPos];this->mCurrPos++;
	unsigned char i2 = this->mStreamBuff[this->mCurrPos];this->mCurrPos++;
	unsigned char i3 = this->mStreamBuff[this->mCurrPos];this->mCurrPos++;
	unsigned char i4 = this->mStreamBuff[this->mCurrPos];this->mCurrPos++;

	ii = i1;
	ii = (ii<<8)|i2;
	ii = (ii<<8)|i3;
	ii = (ii<<8)|i4;
	i = ii;
	return ii;
}

void Packet::PushFloat(float& f){
	memcpy(this->mStreamBuff+this->mContenLen,&f,sizeof(float));
	this->mContenLen += sizeof(float);
}

float Packet::ReadFloat(float& ff){
	if(this->mCurrPos > (this->mContenLen-sizeof(float)) ){
		//printf("%s\n", "ReadFloat error!");
		printf("%s %d %d\n", "ReadFloat error!",this->mCurrPos,int(this->mContenLen - sizeof(int)));
		//return;
	}
	float f;
	memcpy(&f,this->mStreamBuff+this->mCurrPos,sizeof(float));
	//printf("---ReadFloat---float=%f\n",f);
	this->mCurrPos += sizeof(float);
	ff = f;
	return f;
}

void Packet::PushString(char* str){ //end with /0
	size_t len = strlen(str); //include '/0'
	if(str[len] != '\0')
	{
		printf("%s\n", "PushString error arg str must end with '\0'");
		return ;
	}
	memcpy(this->mStreamBuff+this->mContenLen,str,len+1);
	this->mContenLen += (len+1);
	printf("%s %d\n", "Packet::PushString",(int)len);
}

char* Packet::ReadString(char* str){
	int iIndex = this->mCurrPos;
	while(iIndex < this->mContenLen && this->mStreamBuff[iIndex++] != '\0');
	int iStrLen = iIndex - this->mCurrPos + 1;
	//printf("%s %d %d\n", "ReadString",iIndex,iStrLen);
	if (iStrLen == 0)
	{
		//*str = '\0';
	}
	else{
		memcpy(str,this->mStreamBuff+this->mCurrPos,iStrLen);
	}
	*(str+iStrLen) = '\0';
	this->mCurrPos += iStrLen;
	printf("ReadString str= %s\n", str);
	return str;
}

void Packet::AddHead(int iType){
	int iHead = 1;
	this->PushInt(iHead); //
	this->PushInt(iType);
	int i = 0;
	printf("AddHead %d\n", this->mContenLen);
	this->PushInt(i); //iLen
	this->SetType(iType);

}
void Packet::AddTail(){
	int iTemp = this->mContenLen;
	this->mContenLen = 8;
	int iLen = iTemp - 12;
	this->PushInt(iLen);
	printf("AddTail %d %d\n", iLen,iTemp);
	this->mContenLen = iTemp;
	int i=2;
	this->PushInt(i);
}

Session* Packet::GetSession(){
	return this->mSession;
}


//void *memcpy(void *dest, const void *src, size_t n);