#include "ObjectHolder.h"

std::map<int,Object*> ObjectHolder::GObjectCollection = std::map<int,Object*>();

Object* ObjectHolder::FindObjById(double id){
	std::map<int,Object*>::iterator item=ObjectHolder::GObjectCollection.find(id);
	if(item != ObjectHolder::GObjectCollection.end()){
		return item->second;
	}
	return 0;
}

void ObjectHolder::InsertObj(Object* obj,double id){
	ObjectHolder::GObjectCollection.insert(std::make_pair<int,Object*>(id,obj));
}