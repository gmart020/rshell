# rshell

This command shell is supposed to execute a chain of commands inputed by the user. The user should be able to chain as many commands as they desire using connectors ||, &&, and ;. A command after a ; is always executed. A command after a && is executed only if the previous command executed. And a command after a || will only execute if the previous command failed. Anything after a # will be ignored. The program is now able to run test commands as well including the abbreviated form.

In order to use the shell, run the executable. A commmand prompt will be printed and wait for the input. It will execute the inouted commands. It will repeat this process until one of the chained commands is "exit".

#Bugs
The parsing function works by splitting the inputed string into substrings until each substring contains a single command without any connectors. However since we approached the parsing this way, the program requires the user to input the commands in the correct format (command, connector, command). Ending a command with a connector will cause the program to abort. Inputing extra unnecessary parenthesis will also cause it to abort. For example, (((((echo a))))), will cause an error. If parenthesis are not paired, it will also cause an error. In case of an error, the program must be restarted.
