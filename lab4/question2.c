#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void compute_checksum(char *filename) {
    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed\n");
        exit(1); // just exits the program
    }
    if (pid == 0) {
        printf("MD5 (%s) = ", filename);   // formatting for the output of checksum
        fflush(stdout);
        execlp("md5sum", "md5sum", filename, NULL); // runs md5sum command on inputted file
        perror("execlp failed"); // outputs an error message as to why execl fails
        exit(1);
    } else {
        wait(NULL); // for the parent to terminate only after the child process
    }
}

void copy_file(char *source, char *dest) {
  FILE *sourceFile = fopen(source, "rb");   // our source file is in read binary mode
  if (!sourceFile) {
    perror("Error opening source file");
    exit(1);
  }
  FILE *destFile = fopen(dest, "wb");       // our destination file is in write binary mode
  if (!destFile) {
    perror("Error opening destination file");
    exit(1);
  }

  int ch;
  while((ch = fgetc(sourceFile)) != EOF) {  // this reads all characters in our source file until end of file
    fputc(ch, destFile);                    // this puts all the characters we read into the destination file
  }

  fclose(sourceFile);
  fclose(destFile);
  printf("Copied file successfully\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {   // in the case the user does not provide the correct amount of arguments
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);  // shows user proper usage of running question2 "./question2 <source_file> <destination_file>"
        return 1;
    }

    copy_file(argv[1], argv[2]);

    compute_checksum(argv[1]);
    compute_checksum(argv[2]);

    return 0;
}
