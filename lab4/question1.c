#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s file\n", argv[0]);
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    return 1;
  }
  if (pid == 0) {
    printf("Child %d:\n". getpid());
    fflush(stdout);
    execl(argv[1], argv[1], NULL);
    perror("execl failed");
    exit(1);
  } else {
    wait(NULL);
  }

  return 0;
}