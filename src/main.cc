#include <iostream>
#include <string>
#include <unistd.h>

int main(){
  char HOSTNAME[128];
  gethostname(HOSTNAME, sizeof HOSTNAME);
  bool REPEAT = true;
  
  while (REPEAT){
    string input;
    cout << getlogin() << "@" << HOSTNAME << "$ ";
    getline(cin, input);
    
    cout << input << endl;
  }
  
  return 0;
}
