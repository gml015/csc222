#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    printf("fork failed\n");
  }
  if (pid == 0) {
    printf("I am the child process\n");
  } else {
    printf("I am the parent process\n");
  }

  return 0;
}