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
    public:
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
};

class ampersand : public connector
{
    public:
    ampersand() : connector()
    {
    }

    bool evaluate()
    {
        if(left->evaluate())
        {
            if (right->evaluate())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;   
        }
    }
};

class pipes : public connector
{
    public:
    pipes() : connector()
    {
    }

    bool evaluate()
    {
        if(!(left->evaluate()))
        {
            if (right->evaluate())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
};

class always : public connector
{
    public:
    always() : connector()
    {
    }

    bool evaluate()
    {
        left->evaluate();
        bool didRightExecute = right->evaluate();
        if (didRightExecute)
        {
            return  true;
        }
        else
        {
            return false;
        }
    }
};

class terminator : public connector
{
    public:
    terminator() : connector()
    {
    }

    bool evaluate()
    {
        exit(0);
        return true;
    }
};

class commands : public connector
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
            return false;
	}
	else if(pid == 0)
	{
	    if (execvp(args[0], args) < 0)
	    {
		perror("-bash");
                return false;
	    }
	    else
	    {
                return true;
	    }
	}
	else
	{
	    while (wait(&status) != pid)
	    {
	    }
            return true;
	}
    }
};

#endif
