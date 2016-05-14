using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h> //execvp and fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //pid_t type
#include <stdio.h> //error checking


//trying to figure out success/failure of execvp
int runCommands(char **com)
{
   pid_t child=fork();
   if(child==0)
   {
      execvp(com[0], com);
      perror("execvp");
      exit(0);
   }
   else
   {
      pid_t w8=wait(0);
   }
}

int main()
{
   string inputs;
   int exit=0;

   while(exit==0 && getline(cin, inputs))
   {
      bool quote=false;
      string rain;
      vector<string> comm;

      for(string::iterator i=inputs.begin(); i!=inputs.end(); i++)
      {
         if(*i=='\\' && *i+1=='\"')
	 {
            rain+="\\\"";
	    i++;
	 }
	 else if(*i=='\"')
	 {
	    quote=!quote;
	 }
	 else if(*i==' ' && !quote)
	 {
	    comm.push_back(rain);
	    rain="";
	 }
	 else if(*i=='#')
	 {
	    i=inputs.end();
	 }
	 else
	 {
	    rain+=*i;
	 }
      }
      if (rain!="")
      {
	 comm.push_back(rain);
	 rain="";
      }

      vector<vector<char* > > commands;
      vector<char* > first;
      commands.push_back(first);
      int comindex=0;

      for(int i=0; i < comm.size(); i++)
      {
         if(comm.at(i)=="exit")
	 {
	    i=comm.size();
	    exit=1;
	 }
	 else if(comm.at(i)!=";" && comm.at(i)!="&&" && comm.at(i)!="||")
	 {
	    commands[comindex].push_back(const_cast<char*>(comm.at(i).c_str()));
	 }
	 else
	 {
	    char **toBeRun = &commands.at(comindex)[0];
	    vector<char* > next;
	    commands.push_back(next);
	    comindex++;

	    //this is going to change depending on what runCommands() ends up looking like
	    if( runCommands(toBeRun) < 0 )
	    {
	       if(comm.at(i)=="&&") i=comm.size();
	    }
	    else
	    {
	       if(comm.at(i)=="||") i=comm.size();
	    }
	 }
      }

      exit=1; //used for testing; will be removed later
   }

   return 0;
}
