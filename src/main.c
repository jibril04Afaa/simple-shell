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

    } while (status);
    
}