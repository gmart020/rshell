#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stack>
#include "class.h"
using namespace std;

connector parse(string input)
{
    connector returner;
    string leftinput, rightinput, conntype;
    int parenlayer=0;
    int sharpindex=input.size()-1;
    bool quote=false;

    for(string::iterator i=input.begin(); i!=input.end(); i++)
    {

    }

    
    return;
}

int main()
{
    string inputs;
    
    while(getline(cin, inputs))
    {
        connector *toexecute=parse(inputs);
        toexecute->evaluate();
    }

    return 0;
}
