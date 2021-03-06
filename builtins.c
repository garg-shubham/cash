#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "builtins.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* builtin_str[] = {
  "cd",
  "help",
  "history",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &cash_cd,
  &cash_help,
  &cash_history,
  &cash_exit  
};

int history_pointer=0;
int NOC = 0;


/*
  Builtin function implementations.
*/
int cash_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "cash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("cash");
    }
  }
  return 1;
}

int cash_help(char **args)
{
  int i;
  printf("Shell cash\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in commands:\n");

  int x = sizeof(builtin_str) / sizeof(char *);

  for (i = 0; i < x; i++) {
    printf("  %s\n", builtin_str[i]);
    if(i==0)
    {
      printf("Changes Working Directory.\n");
    }
    if(i==1)
    {
      printf("lists all the builtins and describe their functions.\n");
    }
    if(i==2)
    {
      printf("\t 1. history : lists all previous commands.\n");
      printf("\t 2. history num : lists last #num commmands.\n");
      printf("\t 3. history clear : clears previous history.\n");
    }
    if(i==3)
    {
      printf("exits CA$H.\n");
    }
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}
int cash_history(char **args)
{
  int i;
  if(args[1] == NULL)
  {
    if(NOC < 100)
    {
      int counter=1;
      for(i=history_pointer - 1;i>=0;i--)
      {
        printf("%d. %s",counter,history_commands[i]);
        counter++;
      }
    }
    else
    {
      int counter=0;
      for(i=history_pointer-1; counter<100;i--)
      {
        if(i==-1)
          i==99;
        printf("%d. %s",counter+1,history_commands[i]);
        counter++;
      }
    }
  }
  else
  {
    if(strcmp(args[1],"clear")==0)
    {
      for(int i=0;i<100;i++)
        free(history_commands[i]);
      history_pointer = 0;
      NOC =0;
    }
    else if(atoi(args[1])>NOC)
    {
      printf("No of commands entered less than %s\n",args[1]);
    }
    else if(atoi(args[1]) > 100)
    {
      printf("Max 100 commands can be stored\n");
    }
    else if( atoi(args[1]) <=100 && atoi(args[1])<=NOC)
    {
      int counter =0;
      for(i = history_pointer-1 ; counter<atoi(args[1]);i--)
      {
        if(i==-1)
          i=99;
        printf("%d. %s",counter+1,history_commands[i]);
        counter++;
      }
    }
    else
    {
      perror(ANSI_COLOR_RED "Cash" ANSI_COLOR_RESET);
    }
  }
  return 1;
}
int cash_exit(char **args)
{
  exit(0);
}
