#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_ARGS 15

void tokenizeInput(char *input, char **args, char **inputFile, char **outputFile) {
    char *token = strtok(input, " ");  // make tokens based on spaces
    int i = 0;
    *inputFile = NULL;
    *outputFile = NULL;

    // while our token exists and we haven't reached 15 args
    // we update the array with each arg passed through
    while (token != NULL && i < MAX_ARGS - 1) {

        // input redirection (feed input from right into left; ex: greeting.c < name.txt)
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token) { *inputFile = token; }

        // output redirection (feed input from left into right; ex: echo hi > hi.txt)
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
    args[i] = NULL;  // null-terminate the arguments array
}

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
            FILE* outfile = fopen(outputFile, "w");
            if (outfile == NULL) {
                perror("Failed to open output file");
                exit(1);
            }
            dup2(fileno(outfile), 1);
            fclose(outfile);
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

