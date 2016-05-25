#include "class.h"

int main(){
	ampersand* a = new ampersand(); 
	string st = "echo Hello World";
	string st2 = "ls -al";	
	commands* com = new commands(st);
	//com.evaluate();
	commands* com2 = new commands(st2);
	//com2.evaluate();
	a->addLeft(com);
	a->addRight(com2);
	a->evaluate();
	return 0;
}
