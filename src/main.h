void sh_loop();
char *sh_read_line();
char **sh_split_line(char *line);
int sh_execute(char **args);
int sh_launch(char **args);
int sh_num_builtins();
int sh_cd(char **args); // changes directories
int sh_help(char **args); 
int sh_exit(char **args);
extern char *builtin_str[];
extern int (*builtin_func[])(char **);