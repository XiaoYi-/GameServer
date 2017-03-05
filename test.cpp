#include <iostream>
#include <stdio.h>
#include <queue>
#define MAX 10000
class base{
public:
	base(){
		//printf("%s\n", "base init");
	}
	virtual ~base(){
		printf("%s\n", "base dest");
	}
};

class div:public base{
public:
	div(){printf("%s\n", "div init");}
	~div(){printf("%s\n", "div dest");}
};

void qf(){
	std::queue<base*> q;
	//srand( (unsigned)time( NULL ) );
	for(int i=0;i<100;i++){
		//int r = rand() % MAX;
		base* b = new base();
		q.push(b);
	}

	int i = 0;
	while(not q.empty()){
		base* e=q.front();
		q.pop();
		//printf("%d\n", e);
		i++;
		if(i >= 10)break;
	}
	base* b = q.front();
	q.back();
	printf("%d\n", (int)q.size());
	delete b;
};

int main(){
	base* bp = new div();
	delete bp;

	qf();
}