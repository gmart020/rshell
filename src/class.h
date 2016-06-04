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

//to be done
class pipe : public connector
{
    public:
    pipe() : connector()
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
        if (v.at(0) == "test" || v.at(0) == "[")
        {
            struct stat sb;
            if (v.at(1) == "-e" || v.at(1) == "-f" || v.at(1) == "-d")
            {
                stat(v.at(2).c_str(), &sb);
                if (v.at(1) == "-e")
                {
                    if (S_ISREG(sb.st_mode) || S_ISDIR(sb.st_mode))
                    {
                        cout << "(True)" << endl;
                        return true;
                    }
                    else
                    {
                        cout << "(False)" << endl;
                        return false;
                    }
                }
                if (v.at(1) == "-d")
                {
                    if (S_ISDIR(sb.st_mode))
                    {
                        cout << "(True)" << endl;
                        return true;
                    }
                    else
                    {
                        cout << "(False)" << endl;
                        return false;
                    }
                }
                if (v.at(1) == "-f")
                {
                    if (S_ISREG(sb.st_mode))
                    {
                        cout << "(True)" << endl;
                        return true;
                    }
                    else
                    {
                        cout << "(False)" << endl;
                        return false;
                    }
                }
            }   
            else
            {
                stat(v.at(1).c_str(), &sb);
                if (S_ISREG(sb.st_mode) || S_ISDIR(sb.st_mode))
                {
                    cout << "(True)" << endl;
                    return true;
                }
                else
                {
                    cout << "(False)" << endl;
                    return false;
                }
            }
        }
        else
        {
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
        return true;
    }
};

class inout : public connector
{
    public:
    string file;
    fstream fs;
    inout(string &x)
    {
        file=x;
        right=NULL;
    }

    virtual bool evaluate() = 0;
};

//to be done
class input : public inout
{
    public
    input(string &x) : inout(x)
    {
    }

    bool evaluate()
    {
        fs.open(file);

        fs.close();
    }
};

//to be done
class output : public inout
{
    public
    output(string &x) : inout(x)
    {
    }

    bool evaluate()
    {
        fs.open(file);

        fs.close();
    }
};

//to be done
class append : public inout
{
    public
    append(string &x) : inout(x)
    {
    }

    bool evaluate()
    {
        fs.open(file);

        fs.close();
    }
};

#endif
