#include "class.h"

int main(){
	string st = "echo Hello World";
	string st2 = "ls -al";	
	commands com(st);
	com.evaluate();
	commands com2(st2);
	com2.evaluate();
	return 0;
}
