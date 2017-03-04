#include <iostream>
#include <stdio.h>

class base{
public:
	base(){
		printf("%s\n", "base init");
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

int main(){
	base* bp = new div();
	delete bp;
}