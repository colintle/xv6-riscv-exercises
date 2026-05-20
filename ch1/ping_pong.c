#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Write a program that uses UNIX system calls to “ping-pong” a byte between two
// processes over a pair of pipes, one for each direction. Measure the program’s
// performance, in exchanges per second.

// If the child starts first, it gets blocked and goes to sleep
// Kernel scheduler then choses the parent to start
// The kernel wakes up any processes that are waiting on that read descriptor

int main(int argc, char *argv[]) {
  int pipe1[2];
  pipe(pipe1);
  char buffer[512];

  int pipe2[2];
  pipe(pipe2);

  int pid = fork();

  // child
  if (pid == 0) {

    // Child will read the bit
    close(pipe1[1]);
    close(pipe2[0]);

    for (int i = 0; i < 100000; i++) {
      read(pipe1[0], buffer, sizeof(buffer) - 1);
      write(pipe2[1], buffer, 1);
    }

    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
  }
  // parent
  else if (pid > 0) {

    // Parent will first send a bit
    close(pipe1[0]);
    close(pipe2[1]);

    strcpy(buffer, "a");

    for (int i = 0; i < 100000; i++) {
      write(pipe1[1], buffer, 1);
      read(pipe2[0], buffer, sizeof(buffer) - 1);
    }
    wait(NULL);
    close(pipe1[1]);
    close(pipe2[0]);
  }
  return EXIT_SUCCESS;
}
