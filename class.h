#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <boost/tokenizer.hpp>

using namespace std;

class connector
{
    private:
        connector* left;
        connector* right;
        bool successful;
    
    public:
        connector()
        {
            successful = false;
        }
        virtual bool evaluate() = 0;
        void addLeft(connector* c)
        {
            left = c;
        }
        void addRight(connector* c)
	{	
            right = c;
        }
        void success()
        {
            successful=true;
        }
        void failure()
        {
            successful=false;
        }
};

class ampersand: public connector
{
    public:
    ampersand() : connector()
    {
        //constructor
    }

    bool evaluate()
    {
        bool doleft=left->evaluate();
       	bool doright=false;
        if(doleft)
        {
            doright=right->evaluate();
        }

        if(doright)
        {
            success();
        }

        return successful;
    }
};

class pipe: public connector
{
    public:
    pipe() : connector()
    {
        //constructor
    }

    bool evaluate()
    {
        bool doleft=left->evaluate();
        bool doright;
        if(!doleft)
        {
            doright=right->evaluate();
        }

        if(doright || doleft)
        {
            success();
        }

        return successful;
    }
};

class always: public connector
{
    public:
    always() : connector()
    {
        //constructful = true;or
    }

    bool evaluate()
    {
        bool doleft=left->evaluate();
        // bool something=left->evaluate();
        if(right->evaluate())
        {
            success();
        }

        return successful;
    }
};

class exit: public connector
{
    public:
    exit() : connector()
    {
        //constructor
    }

    bool evaluate()
    {
        exit(0);
        return successful;
    }
};

class commands: public connector
{
    public:
    string command;
    commands(string &com)
    {
	left = NULL;
	right = NULL;
	command = com;		
    }

    bool evaluate()
    {
        //execvp goes here
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer tokens(command, sep);
	vector<string> v;

	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
	    v.push_back(*tok_iter);
	}

	unsigned size = v.size() + 1;
        char **args = new char*[size];
	args[size - 1] = 0;

	for (unsigned i = 0; i < size - 1; ++i)
	{
	    const char *mystr = v.at(i).c_str();
	    args[i] = const_cast<char *> (&mystr[0]);
	}

	int status;
	pid_t pid;
	pid = fork();
	
        if (pid < 0)
	{
	    perror("Fork Failed");
	    failure();
	}
	else if(pid == 0)
	{
	    if (execvp(args[0], args) < 0)
	    {
		perror("-bash");
		failure();
	    }
	    else
	    {
		success();
	    }
	}
	else
	{
	    while (wait(&status) != pid)
	    {
		perror("wait");
	    }
	    success();
	}
        return successful;
    }
};

#endif
