#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include "builtins.h"
#include "redir.h"



typedef void (*signalhandler_t)(int);


void handler(int signo)
{
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  printf(ANSI_COLOR_GREEN "\nCA$H$ " ANSI_COLOR_RESET);
}

void cash_welcome(void)
{
  printf(ANSI_COLOR_GREEN);
  printf(
"\n ██████╗ █████╗ ▄▄███▄▄·██╗  ██╗   The shell that can."
"\n██╔════╝██╔══██╗██╔════╝██║  ██║   Built by: -Abhishek Sajwan: 15114002"
"\n██║     ███████║███████╗███████║             -Akash Gupta: 15114004"
"\n██║     ██╔══██║╚════██║██╔══██║             -Anmol Anand: 15114013"
"\n╚██████╗██║  ██║███████║██║  ██║██╗          -Paras Chetal: 15114049"
"\n ╚═════╝╚═╝  ╚═╝╚═▀▀▀══╝╚═╝  ╚═╝╚═╝          -Shubham Garg: 15114068\n\n\n");

  printf(ANSI_COLOR_RESET);
                                   
                                   
}
/*
  reading line from terminal
*/
char* cash_readline()
{
  char *line = NULL;
  ssize_t lineSize = 0;
  getline(&line, &lineSize, stdin);
  history_commands[history_pointer] = (char *) malloc(lineSize);
  strncpy(history_commands[history_pointer], line, lineSize);
  history_pointer++;
  if(history_pointer==100)
    history_pointer=0;
  NOC++;

  return line;
}


int cash_run(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  
  int x = sizeof(builtin_str) / sizeof(char *);

  for (i = 0; i < x; i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return cash_exec(args);
}

void cash_loop(void)
{
  char *line;
  char **args;
  int status;
  status = 1;

  do {

    printf(ANSI_COLOR_GREEN "CA$H$ " ANSI_COLOR_RESET);
    line = cash_readline();
    in.flag = out_append.flag = out_overwrite.flag = pippete.flag = false;
    pippete.count = 0;
    args = cash_split(line);

    if (pippete.flag == true)
      status = pipe_command(args, pippete.count);
    else
      status = cash_run(args);

    free(line);
    free(args);
  }
  while (1);
}


int main(int argc, char* argv[], char *envp[])
{
  

  signal(SIGINT, SIG_IGN);
  signal(SIGINT, handler);
  cash_welcome();
  cash_loop();
}
