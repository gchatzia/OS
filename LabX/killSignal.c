#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define FILE_PATH "message.txt"

// Signal handler for SIGUSR1 in child
void sigusr1_handler(int sig) {
    char messageBuf[100];
    FILE *file = fopen(FILE_PATH, "r"); // Open the file for reading
    if (file == NULL) {
        perror("Child could not open the file");
        exit(EXIT_FAILURE);
    }

    // Read the message from the file
    if (fgets(messageBuf, sizeof(messageBuf), file) != NULL) {
        printf("Child received SIGUSR1 and message: %s\n", messageBuf);
    } else {
        printf("Child received SIGUSR1 but file was empty or unreadable.\n");
    }

    fclose(file); // Close the file
    exit(0);
}

int main() {
    pid_t pid;

    pid = fork();

    if (pid == -1) { // Error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        // Register SIGUSR1 handler
        if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
            perror("signal");
            exit(EXIT_FAILURE);
        }

        printf("Child process ID: %d. Waiting for SIGUSR1...\n", getpid());

        // Infinite loop to keep child alive
        while (1) {
            pause(); // Wait for signals
        }
    } else { // Parent process
        printf("Parent process ID: %d. Child PID: %d\n", getpid(), pid);

        // Sleep for 3 seconds before sending SIGUSR1 and message
        sleep(3);

        // Write the message to the file
        FILE *file = fopen(FILE_PATH, "w");
        if (file == NULL) {
            perror("Parent could not open the file");
            exit(EXIT_FAILURE);
        }

        char *message = "Parent sends hello!";
        fprintf(file, "%s\n", message);
        fclose(file); // Close the file

        printf("Parent sending SIGUSR1 to child.\n");
        if (kill(pid, SIGUSR1) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }

        // Wait for the child process to terminate
        wait(NULL);
        printf("Child process terminated.\n");
    }

    return 0;
}
