#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Write a program that uses UNIX system calls to “ping-pong” a byte between two
// processes over a pair of pipes, one for each direction. Measure the program’s
// performance, in exchanges per second.

int main(int argc, char *argv[]) {
  int pipe1[2];
  pipe(pipe1);
  char buffer[512];

  int pid = fork();

  // child
  if (pid == 0) {
    // Child will read the bit
    close(pipe1[1]);
    size_t n = read(pipe1[0], buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    printf("Data from child: %s", buffer);
    close(pipe1[0]);
    exit(0);
  }
  // parent
  else if (pid > 0) {
    // Parent will first send a bit
    printf("Parent is sending a message to child\n");
    close(pipe1[0]);
    write(pipe1[1], "hello child\n", 12);
    close(pipe1[1]);
    wait(0);
    printf("Child received message\n");
  }
  return EXIT_SUCCESS;
}
