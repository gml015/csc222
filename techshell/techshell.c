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

void printWorkingDirectory () {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n%s\n", cwd);
}


int main() {
    char input[255];
    char *args[MAX_ARGS];

    while (1) {
        printf("$ ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  // this handles EOF
        }

        // remove newline char from input
        input[strcspn(input, "\n")] = 0;

        // if the user inputs exit
        if (strcmp(input, "exit") == 0) {
            return 0;
        }

        tokenizeInput(input, args);

        if (args[0] != NULL) {
            executeCommand(args);
        }
    }
}

