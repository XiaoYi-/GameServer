#include "player.h"

int Player::GetId(){
	return this->mId;
}
Session* Player::GetSession(){
	return this->mSession;
}
PyObject* Player::GetInPyObj(){
	return this->mInPyObj;
}

void Player::SetId(int id){
	this->mId = id;
}

void Player::SetSession(Session* s){
	this->mSession = s;
}
void Player::SetInPyObj(PyObject* p){
	this->mInPyObj = p;
}