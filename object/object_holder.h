#ifndef OBJECTHOLDER_H
#define OBJECTHOLDER_H

#include "object.h"

#include <map>

class ObjectHolder{
public:
	static Object* FindObjById(double id);
	static void InsertObj(Object*,double id);
private:
	static std::map<int,Object*> GObjectCollection;
};

#endif