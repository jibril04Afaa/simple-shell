#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS

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
        printf("> %c");

        // 1 - read the command from the standard input
        line = sh_read_line();

        // 2 - parse, by separating the cmd str into a program and arguments
        args = sh_split_line(args);

        // 3 - run the parsed cmd
        status = sh_execute(args);

        // free up dynamically allocated memory 
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
void sh_split_line()
{
    
}
// void sh_execute();