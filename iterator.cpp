using namespace std;
#include <iostream>
#include <vector>
#include <string>

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
			else
			{
				rain+=*i;
			}
		}
		if (rain!="") comm.push_back(rain);

		exit=1;
	}

	return 0;
}
