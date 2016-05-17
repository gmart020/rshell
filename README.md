# rshell

This command shell is supposed to execute a chain of commands inputed by the user. The user should be able to chain as many commands as they desire using connectors ||, &&, and ;. A command after a ; is always executed. A command after a && is executed only if the previous command executed. And a command after a || will only execute if the previous command failed. Anything after a # will be ignored.

In order to use the shell, run the executable. A commmand prompt will be printed and wait for the input. It will execute the inouted commands. It will repeat this process until one of the chained commands is "exit".

#Bugs
The command shell will end whenever "exit" is inputed as one of the chained commands, no matter what the connector in front of it is. For example if "exit" comes after a "||" and the previous command executes, the shell will still end. 

There are also some commands that do not work when followed by a space and then a "#". For example "ls -l #comments" will not run, however the majority of commands will work.
