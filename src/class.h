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
#include <fstream>

using namespace std;

class connector
{
    public:
        connector* left;
        connector* right;
    
    public:
        connector()
        {
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
        boost::char_separator<char> sep("|");
        tokenizer tokens(command, sep);
        vector<vector<string> > v;

        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
        {
            typedef boost::tokenizer<boost::char_separator<char> > tokenizerSpace;
            boost::char_separator<char> sepSpace(" ");
            tokenizer tokensSpace(*tok_iter, sepSpace);
            vector<string> vSpace;

            for (tokenizer::iterator space_iter = tokensSpace.begin(); space_iter != tokensSpace.end(); ++space_iter)
            {
                vSpace.push_back(*space_iter);
            }

            v.push_back(vSpace);
        }

        int p[2];
        pid_t pid;
        int fd_in = 0;

        for(unsigned j = 0; j < v.size(); j++)
        {
            unsigned size = v[j].size() + 1;
            char **args = new char*[size];
            args[size - 1] = 0;

            for (unsigned i = 0; i < size - 1; ++i)
            {
                const char *mystr = v[j].at(i).c_str();
                args[i] = const_cast<char *> (&mystr[0]);
            }

            pipe(p);
            if((pid=fork()) < -1)
            {
                perror("Fork Failed");
                return false;
            }
            else if(pid == 0)
            {
                dup2(fd_in, 0);
                if(j<v.size()-1)
                {
                    dup2(p[1], 1);
                }
                close(p[0]);
                execvp(args[0], args);
                exit(0);
            }
            else
            {
                wait(NULL);
                close(p[1]);
                fd_in = p[0];
            }
        }
        return true;
    }
};

#endif
