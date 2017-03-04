#ifndef SCRIPTOBJ_H
#define SCRIPTOBJ_H

#include "../Object/Object.h"

#include <Python.h>

class ScriptObj{
public:
	virtual ~ScriptObj(){};
	virtual void PreparePacket(int)=0;
	virtual void PacketInt(int)=0;
	virtual void PacketDouble(double)=0;
	virtual void PacketStr(char*)=0;
	virtual int UnpackInt()=0;
	virtual double UnpacketDouble()=0;
	virtual char* UnpacketStr()=0;
	virtual void SendPacket()=0;
	virtual Object* FindObjById(double id)=0;
	virtual void SetFunc(PyObject *)=0;
	PyObject* pFuncObj;
};

#endif