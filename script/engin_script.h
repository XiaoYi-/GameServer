#ifndef ENGINSCRIPT_H
#define ENGINSCRIPT_H

#include <Python.h>
#include "script_oj.h"

//class ScriptObj;

static class ScriptObj* gScriptObj=NULL;
static PyObject* gExeFunc = NULL;

static void gExePacket(int iType){
  if(gExeFunc==NULL) return;
  PyObject* pArgc = Py_BuildValue("(i)",iType);
  PyObject* result = PyEval_CallObject(gExeFunc, pArgc); //call script fun
}

static PyObject* PreparePacket(PyObject* self, PyObject* args){
  int iType, result;
  if (! PyArg_ParseTuple(args, "i", &iType))
    return NULL;
  gScriptObj->PreparePacket(iType);
}
static PyObject* PacketInt(PyObject* self, PyObject* args){
  int n;
  if (! PyArg_ParseTuple(args, "i", &n)){
    printf("%s\n", "not PyObject* PacketInt");
    return NULL;
  }
  printf("PyObject* PacketInt %d\n", n);
  gScriptObj->PacketInt(n);
}
static PyObject* PacketDouble(PyObject* self, PyObject* args){}
static PyObject* PacketStr(PyObject* self, PyObject* args){
  printf("%s\n", "f PyObject* PacketStr");
  char* str;
  if (! PyArg_ParseTuple(args, "s", str)){
    printf("%s\n", "not PyObject* PacketStr");
    return NULL;
  }
  printf("PyObject* PacketStr %s\n", str);
  gScriptObj->PacketStr(str);
}
static PyObject* UnpackInt(PyObject* self, PyObject* args){  
  int n, result;
  if (! PyArg_ParseTuple(args, "i", &n))
    return NULL;
 // printf("UnpackInt %d\n", n);
  result = gScriptObj->UnpackInt();
  return Py_BuildValue("i", result);
}
static PyObject* UnpacketDouble(PyObject* self, PyObject* args){}
static PyObject* UnpacketStr(PyObject* self, PyObject* args){
  char* str = gScriptObj->UnpacketStr();
  printf("PyObject* UnpacketStr %s\n", str);
  return Py_BuildValue("s", str);
}
static PyObject* SendPacket(PyObject* self, PyObject* args){
  int iRid=0;
  //if (! PyArg_ParseTuple(args, "i", &iRid))
  //  return NULL;
  //find session by iRid and send
  printf("%s\n", "PyObject* SendPacket");
  gScriptObj->SendPacket();
}

static PyObject* FindObjById(PyObject* self, PyObject* args){}

static PyMethodDef EnginMethods[] =
{
  {"PreparePacket", PreparePacket, METH_VARARGS, "Caculate N!"},
  {"PacketInt", PacketInt, METH_VARARGS, "Caculate N!"},
  {"PacketDouble", PacketDouble, METH_VARARGS, "Caculate N!"},
  {"PacketStr", PacketStr, METH_VARARGS, "Caculate N!"},
  {"UnpackInt", UnpackInt, METH_VARARGS, "Caculate N!"},
  {"UnpacketDouble", UnpacketDouble, METH_VARARGS, "Caculate N!"},
  {"UnpacketStr", UnpacketStr, METH_VARARGS, "Caculate N!"},
  {"SendPacket", SendPacket, METH_VARARGS, "Caculate N!"},
  {"FindObjById", FindObjById, METH_VARARGS, "Caculate N!"},
  {NULL, NULL}
};

static PyObject * InitScriptServer(class ScriptObj* obj){
	gScriptObj = obj;
  //gScriptObj->PreparePacket(10);

	PyObject* pModule = NULL;
  PyObject* result = NULL;
  PyObject * pFunc = NULL;
  PyObject * pArg  = NULL;

	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./server_script')");

	pModule = Py_InitModule("engin",EnginMethods); // init module from fun arr
  if(pModule==NULL){
    printf("%s\n", "init engin mmodule error!");
    return NULL;
  }
	PyImport_AddModule("engin");

	PyObject* pInitModule = PyImport_ImportModule("init"); // import py module
  if (pInitModule == NULL){
    printf("%s\n", "import init module error!");
    return NULL;
  }

	gExeFunc = PyObject_GetAttrString(pInitModule, "ExeFunc");
  if(gExeFunc == NULL){
    printf("%s\n", "get fun error!");
  }
  obj->SetFunc(gExeFunc);
  return gExeFunc;
}


#endif