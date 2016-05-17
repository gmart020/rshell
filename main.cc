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
    
    //Loop will only break if the user enters 'exit' in the inputted command.
    while (1){
        string i;
        cout << getlogin() << "@" << HOSTNAME << "$ ";
        getline(cin, i);
        string w = "";
        vector <string> word; //This will an individual command after parsing. 
        vector< vector<string> > comm; //After parsing this will hold commands in the order they were inputed.
        bool quote = false; 
        
        /* This for loop will iterate through the inputed string and separate commands
           by the connectors. When a connector is found vector word which holds a 
           single command will be pushed back into vector comm which will eventually
           hold all the concatenated commands. After word is pushed back, the 
           appropriate connector will be pushed into comm as well.
        */
        for (unsigned j = 0; j < i.size(); ++j){
            //If the end is reached pushes data into comm.
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
                    //If semicolon is found push created vector into com.
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
                    //If && is found push created vector into com.
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
                    //If || is found push created vector into com.
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
                    //If a space is found push created word into word.
                    else if (i.at(j) == ' '){
                        if (!(w.empty())){
                            word.push_back(w);
                            w.clear();
                        }
                    }
                    //If a # is found outside of quotes, stops parsing process and
                    //begins execution.
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
        
        bool didItExecute = true; //Checks if the current command executed or not.
        bool shouldItExecute = true; //Determines if the current command should execute or not.
        
        //Commands are executed here.
        for (unsigned j = 0; j < comm.size(); ++j){
            //If command is exit, execution is stopped and outer while loop restarts.
            if (comm.at(j).size() == 1 && comm.at(j).at(0) == "exit"){
                exit(0);
            }
            //If a semicolon is found, automatically set shouldItExecute to true
            //so next command will execute.
            else if (comm.at(j).size() == 1 && comm.at(j).at(0) == ";"){
                shouldItExecute = true;
            }
            //If || is found checks didItExecute to see if previous command executed.
            //If it did it will set shouldItExecute to false, true otherwise.
            else if (comm.at(j).size() == 1 && comm.at(j).at(0) == "||"){
                if (didItExecute){
                    shouldItExecute = false;
                }
                else{
                    shouldItExecute = true;
                }
            }
            //If && is found checks didItExecute to see if previous command executed.
            //If it did it will set shouldItExecute to true, false otherwise.
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
                char**args = new char*[size]; //Creates args of type char** which will be inputed to execvp.
                args[size - 1] = 0;
                //stores each command in comm into char to beable to be executed.
                for (unsigned i = 0; i < size - 1; ++i){
                    const char *mystr = comm.at(j).at(i).c_str();
                    args[i] = const_cast<char *> (&mystr[0]);
                }
                //If shouldItExecute is true, which would be set by the connectors
                //above, then the current command is executed.
                if (shouldItExecute){
                    int status;
                    pid_t pid;
                    pid = fork(); //Forks a child process.
                    if (pid < 0) //Fork failed, exits program.
                    {
                        perror("Fork Failed");
                        exit(1);
                    }
                    else if (pid == 0) //Fork is successful.
                    {
                        if (execvp(args[0], args) < 0){ //executes the command.
                            perror("-bash"); //Execution failed if it takes this branch.
                            didItExecute = false; //Update execution information for following commands.
                        }
                        else{
                            didItExecute = true; //Execution successful. Updates execution info.
                        }
                    }
                    else
                    {   //Makes the parent wait until child process completes.
                        while ( wait(&status) != pid)
                        {
                            perror("wait");
                        }
                        didItExecute = true;
                        delete [] args;
                    }   
                }
            }
        }
    }
    return 0;
}
