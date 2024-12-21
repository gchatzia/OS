#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
   pid_t pid;

   for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) { // Child process
            printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
            exit(0);
        }
        else { // Parent process
           printf("Parent Process: PID = %d, Created Child with PID: %d\n", 
                   getpid(), pid);
        }
   }

    return 0;
}
