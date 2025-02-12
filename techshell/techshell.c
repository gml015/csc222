#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_ARGS 10

void tokenizeInput(char *input, char **args) {
    char *token = strtok(input, " ");  // make tokens based on spaces
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // null-terminate the arguments array
}

void executeCommand(char **args) {
    pid_t pid = fork();  // create a child process

    if (pid < 0) {
        perror("Fork failed");  // error handling
        exit(1);
    } else if (pid == 0) {
        // child process executes the command
        if (execvp(args[0], args) == -1) {
            perror("Error executing command");
            exit(1);
        }
    } else {
        // parent process waits for the child
        wait(NULL);
    }
}

void changeDirectory(char **args) {
    if (args[1] == NULL) {
        char *home = getenv("HOME");
        if (home == NULL) {
            perror("Environment variable HOME not set");
            return;
        }
        if (chdir(home) != 0) {
            perror("Error changing directory");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("Error changing directory");
        }
    }
}




int main() {
    char input[255];
    char *args[MAX_ARGS];

    while (1) {

        char cwd[255];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s$ ", cwd);
        } else {
            perror("getcwd failed");
        }

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  // this handles EOF
        }

        // remove newline char from input
        input[strcspn(input, "\n")] = 0;


        tokenizeInput(input, args);

        // ignore empty input
        if (args[0] == NULL) {
            continue;
        }

        // if input is "exit", program ends
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        // if input is "cd", parse arguments through changeDirectory() function
        } else if (strcmp(args[0], "cd") == 0) {
            changeDirectory(args);
        // if input is a valid command, arguments are passed through executeCommand() function
        } else {
            executeCommand(args);
        }
    }
}

