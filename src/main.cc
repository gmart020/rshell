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
#include <boost/tokenizer.hpp>
#include "class.h"

using namespace std;
/*Helper function that checks to see whether or not the inputed string 
 *contains a single command without any parenthesis or connector. Also
 *eliminates any comments from the input.
*/
void checkString(string &s, bool &hasPar, bool &hasConn)
{
    bool quoteFound = false;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        if (s.at(i) == '(' && !quoteFound)
        {
            hasPar = true;
        }
        else if (s.at(i) == ';' && !quoteFound)
        {
            hasConn = true;
        }
        else if (s.at(i) == '&' && !quoteFound)
        {
            ++i;
            if (i < s.size() && s.at(i) == '&')
            {
                hasConn = true;
            }
        }
        else if (s.at(i) == '|' && !quoteFound)
        {
            ++i;
            if (i < s.size() && s.at(i) == '|')
            {
                hasConn = true;
            }
        }
        else if (s.at(i) == '#' && !quoteFound)
        {
            s = s.substr(0, i);
            while (s.at(s.size() - 1) == ' ')
            {
                s = s.substr(0, s.size() - 1);
            }
            return;
        }
        else if (s.at(i) == '\"')
        {
            quoteFound = !quoteFound;
        }
    }
}

/*Helper function that removes any useless parenthesis. These include 
 *the following: (echo a && echo b), (echo a). This function checks to
 *see if it is possible to lose the outer parenthesis without affecting
 *execution and does so if possible.
 */
void removeUselessPar(string &s)
{
    if (s.at(0) == '(' && s.at(s.size() - 1) == ')')
    {
        string temp = s.substr(1, s.size() - 2);
        int leftPar = 0;
        int rightPar = 0;
        for (unsigned i = 0; i < temp.size(); ++i)
        {
            if (temp.at(i) == '(')
            {
                ++leftPar;
            }
            if (temp.at(i) == ')')
            {
                ++rightPar;
            }
            if (rightPar > leftPar)
            {
                return;
            }
        }
        if (leftPar == rightPar)
        {
            s = temp;
        }
        if (temp.find('(') == string::npos && temp.find(')') == string::npos)
        {
            s = temp;
        }
    }
}

/*This function splits a concatenated string of commands based on connectors.
 *If no connector is found the string is left alone. If a connector is found
 *the string is split in two along the index of the connector found.
 */
void parseCommands(string s, string &left, string &right, string &c)
{
    int parCount = 0;
    bool quoteFound = false;
    for (unsigned i = s.size() - 1; i > 0; --i)
    {
        if (s.at(i) == '(' && !quoteFound)
        {
            parCount++;
        }
        else if (s.at(i) == ')' && !quoteFound)
        {
            parCount--;
        }
        else if ((s.at(i) == ';' && !quoteFound) && parCount == 0)
        {
            left = s.substr(0, i);
            right = s.substr(i + 1, s.size());
            c = ";";
            return;
        }
        else if (s.at(i) == '&' && !quoteFound)
        {
            --i;
            if ((i >= 0 && s.at(i) == '&') && parCount == 0)
            {
                left = s.substr(0, i);
                right = s.substr(i+2, s.size());
                c = "&&";
                return;
            }
        }
        else if ((s.at(i) == '|' && !quoteFound) && parCount == 0)
        {
            --i;
            if ((i >= 0) && s.at(i) == '|')
            {
                left = s.substr(0,i);
                right = s.substr(i+2, s.size());
                c = "||";
                return;    
            }
        }
        else if (s.at(i) == '\"')
        {
            quoteFound = !quoteFound;
        }
    }
}
//Removes unnecessay spaces from beginning and end of string. Helper function.
void removeSpaces(string &s)
{
    while (s.at(0) == ' ')
    {
        s = s.substr(1, s.size());
    }
    while (s.at(s.size() - 1) == ' ')
    {
        s = s.substr(0, s.size() - 1); 
    }
}

//Creates a tree from the concatenated commands. Returns a pointer to the root.
//Inner nodes consist of connector objects, while leaves consist of command 
//objects.
connector * createTree(string s)
{
    bool hasPar = false;
    bool hasConn = false;
    string c = "";
    string left = "";
    string right = "";
    checkString(s, hasPar, hasConn);
    if (!hasPar && !hasConn) //Checks to see if a string is a single command.
    {
        if (s == "exit") //If string contains 'exit' creates an object that will exit.
        {
            terminator *e = new terminator();
            return e;
        }
        else //Make a command object that will execute when it evaluates.
        {
            commands *c = new commands(s);
            return c;
        }
    }
    removeUselessPar(s); //Removes useless parenthesis. 
    parseCommands(s, left, right, c); //Splits list of commands into smaller lists.
    //Create objects based on the connector found. Set left and right child by 
    //continuing to parse the strings.
    if (c == ";")
    {
        always *con = new always();
        removeSpaces(left);
        removeSpaces(right);
        con->addLeft(createTree(left));
        con->addRight(createTree(right));
        return con;
    }
    else if (c == "&&")
    {
        ampersand *con = new ampersand();
        removeSpaces(left);
        removeSpaces(right);
        con->addLeft(createTree(left));
        con->addRight(createTree(right));
        return con;
    }
    else
    {
        pipes *con = new pipes();
        removeSpaces(left);
        removeSpaces(right);
        con->addLeft(createTree(left));
        con->addRight(createTree(right));
        return con;
    }
}

int main()
{
    char HOSTNAME[128];
    gethostname(HOSTNAME, sizeof HOSTNAME);
    string input;
    while(input != "exit")
    {
        cout << getlogin() << "@" << HOSTNAME << "$ ";
        getline(cin, input);

        if(input!="")
        {
            connector *c = createTree(input);
            c->evaluate();
        }
    }
    return 0;
}
