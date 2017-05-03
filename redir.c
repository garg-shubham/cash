#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include "redir.h"
#include "builtins.h"


char** cash_split(char* line)
{
  int position = 0;
  char *token, **tokens_temp;
  int bufsize = CASH_TOK_BUFSIZE;
  char **tokens = malloc(bufsize * sizeof(char* ));
  if (!tokens)
  {
    fprintf(stderr,ANSI_COLOR_RED "cash:memory error\n" ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  token = strtok(line, CASH_TOK_DELIM);
  while(token != NULL)
    {
      tokens[position] = token;
      if(strcmp(tokens[position],"<")==0)
      {
        tokens[position] = NULL;
        in.flag = true;
        in.point = position;
      }
      else if(strcmp(tokens[position],">")==0)
      {
        tokens[position] = NULL;
        out_overwrite.flag = true;
        out_overwrite.point = position;
      }
      else if(strcmp(tokens[position],">>")==0)
      {
        tokens[position] = NULL;
        out_append.flag = true;
        out_append.point = position;
      }
      else if(strcmp(tokens[position],"=")==0)
      {
      	pippete.flag = true;
      	pippete.count++;
      }
      position++;
      if (position >= bufsize)
      {
        bufsize+=CASH_TOK_BUFSIZE;
        tokens_temp = tokens;
        tokens = realloc(tokens, bufsize*sizeof(char*));
        if (!tokens)
          {
            free(tokens_temp);
            fprintf(stderr,ANSI_COLOR_RED "cash:memory error\n" ANSI_COLOR_RESET);
            exit(EXIT_FAILURE);
          }
      }
          
      token = strtok(NULL, CASH_TOK_DELIM);
    }

  tokens[position] = NULL;
  return tokens;
}

int pipe_command(char **args, int pipes)
{
  const int commands = pipes + 1;
  int i = 0;

  int pipefds[2*pipes];

  for(i = 0; i < pipes; i++)
  {
    if(pipe(pipefds + i*2) < 0) 
    {
      perror(ANSI_COLOR_RED "Couldn't Pipe" ANSI_COLOR_RESET);
      return 1;
    }
  }

  int pid;
  int status;

  int j = 0;
  int k = 0;
  int s = 1;
  int place;
  int commandStarts[10];
  commandStarts[0] = 0;

  while (args[k] != NULL)
  {
    if(!strcmp(args[k], "="))
    {
      args[k] = NULL;
      commandStarts[s] = k+1;
      s++;
    }
    k++;
  }


  for (i = 0; i < commands; ++i)
  {
    place = commandStarts[i];

    pid = fork();
    if(pid == 0)
    {
      if(i < pipes)
      {
        if(dup2(pipefds[j + 1], 1) < 0)
        {
          perror(ANSI_COLOR_RED "dup2" ANSI_COLOR_RESET);
          return 1;
        }
      }
      if(j != 0 )
      {
        if(dup2(pipefds[j-2], 0) < 0)
        {
          perror(ANSI_COLOR_RED "dup2" ANSI_COLOR_RESET);
          return 1;
        }
      }

      int q;
      for(q = 0; q < 2*pipes; q++)
      {
        close(pipefds[q]);
      }   
      if( execvp(args[place], args + place) < 0 )
      {
        perror(*args);
        return 1;
      }
    }
    else if(pid < 0)
    {
      perror(ANSI_COLOR_RED "error" ANSI_COLOR_RESET);
      return 1;
    }
    j+=2;
  }

  for(i = 0; i < 2 * pipes; i++)
  {
    close(pipefds[i]);
  }

  for(i = 0; i < pipes + 1; i++)
  {
    wait(&status);
  }

  return 1;
}


int cash_exec(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();


  if (pid == 0) //child
  {
    if(in.flag)
    {
      int fd0 = open(args[in.point +1], O_RDONLY, 0);
      dup2(fd0, STDIN_FILENO);
      close(fd0);
      in.flag = false;
    }
    if(out_overwrite.flag)
    {
      int fd1 = creat(args[out_overwrite.point +1], 0644);
      dup2(fd1, STDOUT_FILENO);
      close(fd1);
      out_overwrite.flag = false;
    }
    else if(out_append.flag)
    {
      int fd1 = open(args[out_append.point +1],O_WRONLY | O_APPEND);
      if(fd1==-1)
      fd1 = creat(args[out_append.point +1],0644);

      dup2(fd1, STDOUT_FILENO);
      close(fd1);
      out_append.flag = false; 
    }
    

    if (execvp(args[0], args) == -1)
    {
      if(strcmp(args[0],builtin_str[0])==0)
      {      
        return cash_cd(args);
      }
      else if(strcmp(args[0],builtin_str[1])==0)
      {
        return cash_help(args);
      }
      else if(strcmp(args[0],builtin_str[2])==0)
      {
        return cash_exit(args);
      }
      else
        perror(ANSI_COLOR_RED "cash" ANSI_COLOR_RESET);
    }
    
    exit(EXIT_FAILURE);
  }
  else if (pid < 0) //fork error
  {
    perror(ANSI_COLOR_RED "cash" ANSI_COLOR_RESET);
  }
  else
  {
   do{
       waitpid(pid, &status, WUNTRACED);
     } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}