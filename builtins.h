#ifndef BUILTINS
#define BUILTINS

int cash_cd(char **args);
int cash_help(char **args);
int cash_exit(char **args);
int cash_history(char **args);

int (*builtin_func[4]) (char **);
char* builtin_str[4];
char *history_commands[100];
int history_pointer;
int NOC;
#endif
