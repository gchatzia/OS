#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    FILE* fptr;    

    // clean up file
    fptr = fopen("pids.txt", "w");
    if (fptr != NULL) {
        fprintf(fptr, "", pid);
        fclose(fptr);
    }
    else {
        printf("Could not access file. Terminating...");
        exit(1);
    }

    // Create a new process
    pid = fork();

    if (pid < 0) { // Error in fork
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    
    // Both processes attempt to write their PIDs to the file
    while(1) {
        fptr = fopen("pids.txt", "a");
        if (fptr != NULL) {
    	    printf("Process with PID %d writes to file\n", getpid());
            fprintf(fptr, "%d\n", getpid());
            fclose(fptr);
            exit(0);
        }
        else {
            // the other process opened the file first
            // reattempt in one second
            sleep(1);
        }
    }

    return 0;
}
