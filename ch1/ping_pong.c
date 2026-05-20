#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  char buffer2[512];

  int pid = fork();

  // child
  if (pid == 0) {

    // Child will read the bit
    close(pipe1[1]);
    close(pipe2[0]);

    size_t n = read(pipe1[0], buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    printf("Data from child: %s", buffer);

    write(pipe2[1], buffer, n);
    printf("Sending message to parent\n");

    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
  }
  // parent
  else if (pid > 0) {

    // Parent will first send a bit
    printf("Parent starts with sending message\n");
    close(pipe1[0]);
    close(pipe2[1]);

    write(pipe1[1], "Sending message\n", 16);
    wait(0);

    size_t n = read(pipe2[0], buffer2, sizeof(buffer2) - 1);
    buffer2[n] = '\0';
    printf("Reading message from child: %s", buffer2);

    close(pipe1[1]);
    close(pipe2[0]);
  }
  return EXIT_SUCCESS;
}
