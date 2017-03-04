#ifndef THREAD_H
#define THREAD_H

//namespace THREAD{

class Thread
{
public:
	//Thread(){};
	virtual ~Thread(){};

	virtual void Run() =0;
	virtual void Exit() =0;
};

static void* Proc(void* t){
	Thread* thread = (Thread*)(t);
	thread->Run();
}

#endif