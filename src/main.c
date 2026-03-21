#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"


int main(int argc, char **argv)
{
    // run command loop
    sh_loop();
    return EXIT_SUCCESS;
}

void sh_loop()
{
    // this loop handles the commands of the shell
    char *line;
    char **args;
    int status; 

    do
    {
        printf("> ");
        
        // 1 - read the command from the standard input
        line = sh_read_line();

        // 2 - parse, by separating the cmd str into a program and arguments
        args = sh_split_line(line);

        // 3 - run the parsed cmd
        status = sh_execute(args);

        free(line);
        free(args);

    } while (status);
}

#define SH_RL_BUFSIZE 1024
// start with a block, and if the user exceeds, reallocate with more space
char *sh_read_line()
{
    int bufsize = SH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c; // stores the actual character as int cos EOF is an integer

    // check that dynamic memory allocation has not failed
    if (!buffer)
    {
        fprintf(stderr, "sh: allocation error :( \n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // read a character
        c = getchar();

        // if EOF is reached, replace with a null character and return
        if (c == EOF || c == '\n')
        {
            *(buffer + position) = '\0';
            return buffer;
        } 
        else
        {
            *(buffer + position) = c;
        }
        position++; 
    }

    // if buffer has been exceeded, reallocate
    if (position > bufsize)
    {
        bufsize += SH_RL_BUFSIZE;
        buffer = realloc(buffer, bufsize);
        if (!buffer)
        {
            fprintf(stderr, "sh: allocation error :( \n");
            exit(EXIT_FAILURE);
        }
    }
    
}

// sh_read_line with the getline() implementation
// char *sh_read_line()
// {
//     char *line = NULL;
//     ssize_t bufsize = 0;

//     if (getline(&line, &bufsize, stdin) == -1)
//     {
//         if (feof(stdin))
//         {
//             exit(EXIT_SUCCESS);
//         }
//         else 
//         {
//             perror("readline");
//             exit(EXIT_FAILURE);
//         }
//     }
// }

#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"

// this function parses the line into a list of arguments
char **sh_split_line(char *line)
{
    int bufsize = SH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*)); // array of pointers
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "sh: allocation error :(\n");
        exit(EXIT_FAILURE);
    }

    // tokenize with strtok
    token = strtok(line, SH_TOK_DELIM);
    while (token != NULL)
    {
        // store each word
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += SH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "sh: allocation error :(\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SH_TOK_DELIM);
    }

    tokens[position] = NULL; // null terminate the list of tokens
    return tokens;
}

int sh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        return 1; // an empty command was entered
    }

    for (i=0; i<sh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return sh_launch(args); 
}

int sh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // child process
        if (execvp(args[0], args) == -1)
        {
            perror("sh: fork success\n");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // fork error
        perror("sh: error forking\n");
    }
    else
    {
        // parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

/* Built-in shell commands */
char *builtin_str[] = 
{
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = 
{
    &sh_cd,
    &sh_help,
    &sh_exit
};

int sh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/* Built-in functions implementation */
int sh_cd(char **args)
{
    if (args[1] == NULL) // check that the 2nd argument exists
    {
        fprintf(stderr, "sh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0) // chdir() changes the working directory
        {
            perror("sh");
        }
    }
    return 1;
}

int sh_help(char **args)
{
    int i;
    printf("Jibril's SH\n");
    printf("Type program names and arguments, then hit enter.\n");
    printf("The following are built-in:\n");

    for (int i=0; i<sh_num_builtins(); i++)
    {
        printf("    %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int sh_exit(char **args)
{
    return 0;
}