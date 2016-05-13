#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

void deleteSpaces(vector<string> &v){
	for (unsigned j = 0; j < v.size(); ++j){
		string tmp = v.at(j);
		if (tmp.at(0) == ' '){
			v.at(j) = tmp.substr(1, tmp.size());
		}
	}
}

int main(){
  char HOSTNAME[128];
  gethostname(HOSTNAME, sizeof HOSTNAME);
  
  while (1){
    string input;
    cout << getlogin() << "@" << HOSTNAME << "$ ";
    getline(cin, input);
    
    if (input == "exit"){
		break;	
	}

	string AND = "&&";
	string PIPE = "||";
	string COLON = ";";

	vector<string> components;
  	string command = "";

	for (unsigned j = 0; j < input.size(); ++j){
		if (j == input.size() - 1){
			command.push_back(input.at(j));
			components.push_back(command);
			break;
		}

		else{

			if (input.at(j) != ' '){
				if (input.at(j) == '\"'){
					command.push_back(input.at(j));
					for (unsigned k = j + 1; k < input.size(); ++k){
						if ((k > 0 && (input.at(k) == '\"' && input.at(k-1) != '\\')) || k == input.size() - 1){
							command.push_back(input.at(k));
							j = k;
							break;
						}	 
						command.push_back(input.at(k));
					
					}
				}
				else if (input.at(j) == ';' || j == input.size() - 1){
					if (!command.empty()) components.push_back(command);
					command.clear();
					components.push_back(COLON);
				}
				else if ((input.at(j) == '|' && input.at(j+1) == '|') || j == input.size() - 1){
				if (!command.empty()) components.push_back(command);
					command.clear();
					components.push_back(PIPE);
					j += 1;
				}
				else if ((input.at(j) == '&' && input.at(j+1) == '&') || j == input.size() - 1){
					if (!command.empty()) components.push_back(command);
					command.clear();
					components.push_back(AND);
					j += 1;
				}
				else if (input.at(j) == '#' || j == input.size() - 1){
					if (!command.empty()){
						components.push_back(command);
					}
					command.clear();
					break;
				}
				else{
					command.push_back(input.at(j));
				}
			}
			else{
				command = command + " ";
			}
		}
	}

	deleteSpaces(components);	

	/*
	for (unsigned k = 0; k < commands.size(); ++k){
		int pid = fork();
		char *c = commands.at(k);
		if (pid == -1){
			perror("fork()");
			break;
		}
		else if (pid == 0){
			execvp(c[0], c)
		}
		else{
			waitpid();
		}
	}
	*/

	/*
	for (unsigned k = 0; k < commands.size(); ++k){
		cout << commands.at(k) << endl;
	}
	*/
  } 
  return 0;
}	
