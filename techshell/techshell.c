// Garrett Lee King
// CSC 222

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 20

// tokenizes user input - splits user's input into separate strings based on spaces
// handles redirection characters
void tokenizeInput(char *input, char **args, char **inputFile, char **outputFile) {
    char *token = strtok(input, " ");  // make tokens based on spaces
    int i = 0;
    *inputFile = NULL;
    *outputFile = NULL;

    // while valid tokens exist,
    // we update the array with each arg/token passed through
    while (token != NULL) {

        // input redirection (feed input from right into left; ex: greeting.c < name.txt)
        // inputFile points to same string as this token
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token) { *inputFile = token; }

        // output redirection (feed input from left into right; ex: echo hi > hi.txt)
        // outputFile points to same string as this token
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token) { *outputFile = token; }

        // all other cases, assign tokens as args
        } else {
            args[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // null-terminate the last index of args[] for execvp (requires NULL at the end)
}

// creates child process that is able to execute arguments by passing them through execvp
// handles execution of i/o redirection and the process of reading/writing files
void executeCommand(char **args, char *inputFile, char *outputFile) {
    pid_t pid = fork();  // create a child process

    if (pid < 0) {
        perror("Fork failed");  // error handling
        exit(1);
    } else if (pid == 0) {
        // child process

        // handling input redirection (<)
        if (inputFile) {
            FILE* inFile = fopen(inputFile, "r");
            if (inFile == NULL) {
                perror("Failed to open input file");
                exit(1);
            }
            dup2(fileno(inFile), 0);
            fclose(inFile);
        }

        // handling output redirection (>)
        if (outputFile) {
            FILE* outFile = fopen(outputFile, "w");
            if (outFile == NULL) {
                perror("Failed to open output file");
                exit(1);
            }
            dup2(fileno(outFile), 1);
            fclose(outFile);
        }

        // execution for commands
        if (execvp(args[0], args) == -1) {
            perror("Error executing command");
            exit(1);
        }
    } else {
        // parent process waits for the child
        wait(NULL);
    }
}

// self-explanatory, handles cd; if no specific directory is listed, travels to home directory
void changeDirectory(char **args) {
    // if the arg for cd is empty, we go to HOME
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
        // otherwise we take the arg for cd and go to the directory if it exists
        if (chdir(args[1]) != 0) {
            perror("Error changing directory");
        }
    }
}


int main() {
    char input[255];
    char *args[MAX_ARGS];
    char *inputFile, *outputFile;

    while (1) {

        // getcwd retrieves current working directory; we print it before our $ on input line
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

        // user input is tokenized in our args array
        tokenizeInput(input, args, &inputFile, &outputFile);

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
            executeCommand(args, inputFile, outputFile);
        }
    }
}

