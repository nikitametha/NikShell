#include "shellrequirements.h"

#define BUFSIZE 64

char *builtin_str[] =
{
  "cd",
  "help",
  "exit",
  "mkdir"
};

int (*builtin_func[]) (char **) = 
{
  &niksh_cd,
  &niksh_help,
  &niksh_exit,
  &niksh_mkdir
};



int main(int argc, char *argv[])
{
    //nothing to configure for simple shell
    run_shell();

    return 1; //check
}

void run_shell()
{
    char *input;
    char **args;
    int status;

    do
    {
        printf("> ");
        input = take_input();
        args = split_args(input);
        status = niksh_run(args);
        free(input); free(args);
    }
    while(status!=0);
}

char* take_input()
{
  char *inp = NULL;
  ssize_t inpsize = 0; // have getline allocate a buffer for us
  getline(&inp, &inpsize, stdin);
  return inp;
}

char **split_args(char *inp)
{
    int bufsize = BUFSIZE;
    int pos = 0;
    char **args = malloc(bufsize * sizeof(char*));
    char *arg;

    if (args==0) 
    {
        fprintf(stderr, "NikSh: Error in malloc\n");
        exit(EXIT_FAILURE);
    }

    arg = strtok(inp, "\t\n ");
    while (arg != NULL)
    {
      args[pos] = arg;
        pos++;

        if (pos >= bufsize) 
        {
            bufsize += BUFSIZE;
            args = realloc(args, bufsize * sizeof(char*));
            if (args==0) 
            {
                fprintf(stderr, "NikSh: Error in realloc\n");
                exit(EXIT_FAILURE);
            }
        }

    arg = strtok(NULL, "\t\n ");

    }

    args[pos] = NULL;
    return args;

}


int niksh_run(char **args)
{
    int i;

    if (args[0] == NULL) 
    {
    // An empty command was entered.
        return 1;
    }
    printf("arg[0]= %s\n",args[0]);

    printf("arg[1]= %s\n",args[1]);

    for (i = 0; i < niksh_builtins(); i++) 
    {
        if (strcmp(args[0], builtin_str[i]) == 0) 
        {
            printf("%s called\n",args[0]);
            return (*builtin_func[i])(args);
        }
    }
    printf("%s called- not builtin\n",args[0]);
    return niksh_fork(args);
}



int niksh_builtins() 
{
    return sizeof(builtin_str) / sizeof(char *);
}

int niksh_fork(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) 
    { // Child 
        if (execvp(args[0], args) == -1) 
        {
            perror("NikSh exec error");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) 
    {
        // Error forking
        perror("NikSh fork error");
    }
    else 
    {
        // Parent
        do 
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } 
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


int niksh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "NikSh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("NikSh");
    }
  }
  return 1;
}


int niksh_mkdir(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "NikSh: expected argument to \"mkdir\"\n");
  } else {
    if (mkdir(args[1],0777) != 0) {
      perror("NikSh");
    }
  }
  return 1;
}




int niksh_help(char **args)
{
  printf("Made by 185/191/192\n");
  printf("Built in commands:\n");

  for (int i = 0; i < niksh_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man pages for info on other programs.\n");
  return 1;
}


int niksh_exit(char **args)
{
  return 0;
}

