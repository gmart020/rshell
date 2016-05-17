all:
  rm -rf bin
  mkdir bin
  g++ -Wall -Werror -ansi -pedantic src/main.cc -o bin/rshell
  
rshell: main.cc
  g++ -Wall -Werror -ansi -pedantic src/main.cc -o bin/rshell
