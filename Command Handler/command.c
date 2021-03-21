// Sends commands to the command handler
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>  // for flags


int main(){
    int sig;
    char target[10], confirm;
    // read the pid of the target from pipe
    const char* pipePath = "./pipe";
    int fd = open(pipePath, O_RDONLY);
    if (fd == -1){
        printf("Pipe open failed\n");
        return -1;
    }
    read(fd, target, 10);

    printf("%s\n", target);
    printf("Enter:\n\t1 for Image Request\n\t2 for FPGA request\n\t3 to exit\n");
    while(1) {
        scanf("%d", &sig);
        if (sig == 1){
            // create a child process
            // use it to send interrupt for image
            if(fork() == 0){
                // FIXME replace with write to stdout
                printf("Image request sent\n");
                // send the signal to command handler
                execlp("kill", "kill", "-USR1", target, NULL);
            }

        }
        else if (sig == 2){
            // similar procedure
            if (fork() == 0){
                printf("FPGA Request Sent\n");
                execlp("kill", "kill", "-USR2", target, NULL);
            }
        }
        else if(sig == 3){
            // signal to exit
            execlp("kill", "kill", "-SIGINT", target, NULL);
        }
        else {
            printf("Invalid Request\n");
        }
    }
}
