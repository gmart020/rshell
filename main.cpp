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
    

    
}

int main()
{
    string inputs;
    
    while(getline(cin, inputs))
    {
        connector *toexecute=parse(inputs);
        toexecute->evaluate();
    }

}
