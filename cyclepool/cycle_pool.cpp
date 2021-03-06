#include "cycle_pool.h"

CyclePool::CyclePool(int iMaxCount,int iMaxSize):mMaxItemCount(iMaxCount),mMaxItemSize(iMaxSize){
	//pthread_mutex_init(&this->mItemFrontMutexLock,NULL);
	//pthread_mutex_init(&this->mItemBackMutexLock,NULL);
}

CyclePool::~CyclePool(){
	//std::vector<ItemBuff*>::iterator iter = this->mItemBuffFront.begin();
	while(not this->mItemBuffFront.empty()){
		ItemBuff* t = this-<mItemBuffFront.front();
		free(t->buff);
		//free(*iter);
		this->mItemBuffFront.pop();
	}

	//std::vector<ItemBuff*>::iterator iter = this->mItemBuffBack.begin();
	while(not this->mItemBuffBack.empty()){
		ItemBuff* t = this->mItemBuffBack.front();
		free(t->buff);
		this->mItemBuffBack.pop();
	}

	//pthread_mutex_free(mItemFrontMutexLock);
	//pthread_mutex_free(mItemBackMutexLock);
}

ItemBuff* CyclePool::CreateItem(unsigned int iSize){
	ItemBuff* item = (ItemBuff*)malloc(sizeof(ItemBuff));
	item->size = iSize;
	item->buff = (unsigned char*)malloc(iSize);
	if(item->buff == NULL){
		free(item);
		item = NULL;
	}
	return item;
}
ItemBuff* CyclePool::pop(unsigned int iSize){
	ItemBuff* item = NULL;
	ItemBuff* iter = NULL ;
	if(iSize > this->mMaxItemSize){
		item = this->CreateItem(iSize);
		return item;
	}else{
		
		{
			//pthread_mutex_lock(&this->mItemFrontMutexLock);
			MutexLock frontMutex(_frontMutex);
			if(this->mItemBuffFront.empty()){
				//pthread_mutex_lock(&this->mItemBackMutexLock);
				MutexLock backMutex(_backMutex);
				if(this->mItemBuffBack.empty()){
					for(int i=0;i<10;i++){
						ItemBuff* newItem = this->CreateItem(iSize);
						if(newItem!=NULL){
							this->mItemBuffBack.push(newItem);
						}
					}
				}
				//iter = this->mItemBuffBack.front();
				for(not this->mItemBuffBack.empty())
				{
					iter = this->mItemBuffBack.front();
					this->mItemBuffFront.push(iter);
					this->mItemBuffBack.pop();	
				}
				//pthread_mutex_unlock(&this->mItemBackMutexLock);
			}
			if(not this->mItemBuffFront.empty()){
				item = this->mItemBuffFront.pop();
				//item = iter;
			}
			//pthread_mutex_unlock(&this->mItemFrontMutexLock);
		}

	}
	return item;
	
}
void CyclePool::put(ItemBuff* item){
	//pthread_mutex_unlock(&this->mItemBackMutexLock);
	MutexLock backMutex(_backMutex);
	this->mItemBuffBack.push(item);
	//pthread_mutex_unlock(&this->mItemBackMutexLock);
}

//pthread_mutex_t mItemFrontMutexLock;
//pthread_mutex_t mItemBackMutexLock;