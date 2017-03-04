#ifndef CYCLE_POOL_H
#define CYCLE_POOL_H

#include "../thread/mutex.h"
#include <vector>
//#include <pthread.h>

typedef struct {
	unsigned int size;
	unsigned char* buff;
} ItemBuff;

class CyclePool{
public:
	CyclePool(int,int);
	~CyclePool();

	ItemBuff* CreateItem(unsigned int iSize);
	ItemBuff* pop(unsigned int iSize);
	void put(ItemBuff* item);

private:
	int mMaxItemCount;
	int mMaxItemSize;
	std::vector<ItemBuff*> mItemBuffFront;
	std::vector<ItemBuff*> mItemBuffBack;

	//pthread_mutex_t mItemFrontMutexLock;
	//pthread_mutex_t mItemBackMutexLock;
	Mutex _frontMutex;
	Mutex _backMutex;
	
};


#endif