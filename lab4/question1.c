#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {   // in the case the user provides no executable for the child process to run
    printf("Usage: %s <file>\n", argv[0]);  // shows user proper usage of running question1 "./question1 <file>"
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    return 1;
  }
  if (pid == 0) {
    printf("Child %d:\n", getpid());  // child prints out its process id
    fflush(stdout);                          // ensure that the output of our file is printed before executing
    execl(argv[1], argv[1], NULL); // child takes in executable and executes
    perror("execl failed"); // outputs an error message as to why execl fails
    exit(1);  // just exits the program
  } else {
    wait(NULL); // for the parent to terminate only after the child process
  }

  return 0;
}