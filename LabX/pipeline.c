#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // message
    char* message = "Parent sends hello!";
    int messageSize = 20; // one extra for the null character

    // Create a pipe for synchronization 
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid == -1) { // Error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        char messageBuf[messageSize];
        // Infinite loop to keep child alive
        while (1) {
            if (read(fd[0], messageBuf, messageSize) == messageSize) {
                // message received: display and exit
                printf("Child Process received message: %s\n", messageBuf);
                exit(0);
            }
        }
    } else { // Parent process
        printf("Parent process ID: %d. Child PID: %d\n", getpid(), pid);

        // Sleep for 3 seconds before sending a message with pipe
        sleep(3);

        printf("Parent sending message to child.\n");
	write(fd[1], message, messageSize);

        // Wait for the child process to read the message and terminate
        wait(NULL);
        printf("Child process terminated.\n");
    }

    // close file descriptors
    close(fd[0]);
    close(fd[1]);

    return 0;
}
