#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main(){
  char HOSTNAME[128];
  gethostname(HOSTNAME, sizeof HOSTNAME);
  bool REPEAT = true;
  
  while (REPEAT){
    string input;
    cout << getlogin() << "@" << HOSTNAME << "$ ";
    getline(cin, input);
    
    if (input == "exit"){
		REPEAT = false;	
	}
  }
}
