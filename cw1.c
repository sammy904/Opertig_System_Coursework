#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }

    if(pid == 0) {
        printf("\nCHILD PROCESS\n");
        printf("Child PID : %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
    }
    else {
        wait(NULL);

        printf("\nPARENT PROCESS\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n", pid);
    }

    return 0;
}
