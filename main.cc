#include <iostream>
#include <vector>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
using namespace std;

int main(){

    string COLON = ";";
    string PIPE = "||";
    string AND = "&&";
    
    char HOSTNAME[128];
    gethostname(HOSTNAME, sizeof HOSTNAME);
    
    while (1){
        string i;
        cout << getlogin() << "@" << HOSTNAME << "$ ";
        getline(cin, i);
        
        string w = "";
        vector <string> word;
        vector< vector<string> > comm;
        bool quote = false;
        
        for (unsigned j = 0; j < i.size(); ++j){
            if (j == i.size() - 1){
                if(i.at(j)!='\"') w.push_back(i.at(j));
                word.push_back(w);
                comm.push_back(word);
            }
            else{
                if (quote){ //In quotes
                   // if ((i.at(j) == '\"' && i.at(j - 1) != '\\') || j == i.size() - 1){
                    if(i.at(j) == '\"' && i.at(j-1) != '\\'){
                       // w.push_back(i.at(j));
                        word.push_back(w);
                        w.clear();
                        quote = !quote;
                    }
                    else{
                        w.push_back(i.at(j));
                    }
                }
                else{ //Not in quotes
                    if (i.at(j) == '\"'){
                        quote = !quote;
                        j++;
                        w.push_back(i.at(j));
                    }
                    else if (i.at(j) == ';'){
                        word.push_back(w);
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                        word.push_back(COLON);
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                    }
                    else if ((i.at(j) == '&' && i.at(j + 1) == '&')){
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                        word.push_back(AND);
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                        j+=1;
                    }
                    else if ((i.at(j) == '|' && i.at(j + 1) == '|')){
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                        word.push_back(PIPE);
                        comm.push_back(word);
                        word.clear();
                        w.clear();
                        j+=1;
                    }
                    else if (i.at(j) == ' '){
                        if (!(w.empty())){
                            word.push_back(w);
                            w.clear();
                        }
                    }
                    else if (i.at(j) == '#'){
                        if (!(w.empty())){
                            word.push_back(w);
                        }
                        break;
                    }
                    else{
                        w.push_back(i.at(j));
                    }
                }
            }
        }
            
        // for (unsigned l = 0; l < comm.size(); ++l){
        //     for (unsigned m = 0; m < comm.at(l).size(); ++m){
        //         cout << comm.at(l).at(m) << " ";
        //     }
        //     cout << endl;
        // }
        
        bool didItExecute = true;
        bool shouldItExecute = true;
        
        for (unsigned j = 0; j < comm.size(); ++j){
            if (comm.at(j).size() == 1 && comm.at(j).at(0) == "exit"){
                exit(0);
            }
            
            else if (comm.at(j).size() == 1 && comm.at(j).at(0) == ";"){
                shouldItExecute = true;
            }
            
            else if (comm.at(j).size() == 1 && comm.at(j).at(0) == "||"){
                if (didItExecute){
                    shouldItExecute = false;
                }
                else{
                    shouldItExecute = true;
                }
            }
            
            else if (comm.at(j).size() == 1 && comm.at(j).at(0) == "&&"){
                if (didItExecute){
                    shouldItExecute = true;
                }
                else{
                    shouldItExecute = false;
                }
            }
            
            else{
                unsigned size = comm.at(j).size() + 1;
                char**args = new char*[size];
        
                for (unsigned i = 0; i < size - 1; ++i){
                    const char *mystr = comm.at(j).at(i).c_str();
                    args[i] = const_cast<char *> (&mystr[0]);
                }
    
                args[size - 1] = 0;
                
                if (shouldItExecute){
                    int status;
                    pid_t c_pid, pid; // Where c_pid is child
                    c_pid = fork();
                    if (c_pid < 0)
                    {
                        perror("fork failed");
                        exit(1);
                    }
                    else if (c_pid == 0) // Child process
                    {
                        execvp(args[0], args);
                        perror("-bash");
                        exit(1);
                    }
                    else if (c_pid > 0)
                    {
                        if ( (pid = wait(&status)) < 0)
                        {
                            perror("wait");
                            exit(1);
                        }
                        
                        if (WEXITSTATUS(status) != 0){
                            didItExecute = false;
                        }
                        else{
                            didItExecute = true;
                        }
                        delete [] args;
                    }   
                }
            }
        }
    }
    return 0;
}
