#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h> 

int niksh_cd(char **args);
int niksh_help(char **args);
int niksh_exit(char **args);

void run_shell();
char* take_input();
char **split_args(char *inp);
int niksh_run(char **args);
int niksh_builtins();
int niksh_fork(char **args);
int niksh_cd(char **args);
int niksh_help(char **args);
int niksh_exit(char **args);
int niksh_mkdir(char **args);