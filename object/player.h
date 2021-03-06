#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "../session/session.h"
#include <Python.h>

class Session;

class Player:public Object{
public:
	Player(){}
	int GetId();
	Session* GetSession();
	PyObject* GetInPyObj();

	void SetId(int id);
	void SetSession(Session*);
	void SetInPyObj(PyObject*);
private:
	int mId;
	Session* mSession;
	PyObject* mInPyObj;
};

#endif