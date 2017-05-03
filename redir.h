#ifndef REDIRECTION
#define REDIRECTION

#define CASH_TOK_BUFSIZE 64
#define CASH_TOK_DELIM " \t\r\n"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct{
  bool flag;
  int point;
}redir;
redir in, out_overwrite,out_append;

typedef struct{
  bool flag;
  int count;
}pipes;
pipes pippete;


int pipe_command(char **args, int pipes);
int cash_exec(char **args);
char** cash_split(char* line);

#endif
