#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>


#define false 0
#define true 1

const char* pipePath = "./pipe";
int fd;

// to serve as flags
int accessImage = false;
int commInfo = false;

void terminate(){
    printf("Closed Pipe\n");
    close(fd);
    unlink(pipePath);
    exit(1);
}

void imageSent(){
    printf("<<<Image passed to OBC>>>\n");
    accessImage = false;
}
void communicateInfo(){
    printf("<<<Information Communicated with FPGA>>>\n");
    commInfo = false;
}

int main(){
    printf("My id is: %d\n",getpid());
    
    if (mkfifo(pipePath, 0666) == -1){
        printf("Cannot open pipe: Check Path\n");
        return -1;
    }

    fd = open(pipePath, O_WRONLY);
    if (fd == -1){
        printf("Pipe open failes \n");
        return -1;
    }
    char id[10];
    sprintf(id, "%d", getpid());
    write(fd, id, 10);

    signal(SIGUSR1, imageSent);
    signal(SIGUSR2, communicateInfo);
    signal(SIGINT, terminate);
    while (true){
        sleep(100);
    }
    return 0;
}


// pthread_mutex_t lock;
// void masterFunc(int state){
//     // only one function can access the global variables
//     pthread_mutex_lock(&lock);
//     switch(state){
//         case 1:
//             if (accessImage)
//                 imageSent();
//             if (commInfo)
//                 communicateInfo();
//             break;
//         case 2:
//             accessImage = true;
//             printf("\n...Image Request Received...\n");
//             break;
//         case 3:
//             commInfo = true;
//             printf("\n...Communicate with FPGA Request Received\n");
//             break;
//     }
//     pthread_mutex_unlock(&lock);
// }


// // For mocking interface 
// void *mockImageRequest(){
//     while(1){
//         // toggle the access Image flag
//         masterFunc(2);
//         sleep(5);
//     }
// }

// // for mocking interface
// void *mockCommInfo(){
//     // create an offset
//     sleep(2);
//     while(1){
//         // toggle the communication flag
//         masterFunc(3);
//         sleep(5);
//     }
// }

// // Temp function to check flags
// // To be replaced by interrupts
// void *mockCheckStatus(){
//     // works on polling
//     // TODO change the implementation to interrupt driven
//     while(1){
//         // Checks if some flag is true
//         masterFunc(1);
//         sleep(1);
//     }
// }





// int main (){
//     // initialize the thread
//     pthread_t threads[3];
//     int tv[3];
//     tv[0] = pthread_create(&threads[0], NULL, mockImageRequest, NULL);
//     tv[1] = pthread_create(&threads[1], NULL, mockCommInfo, NULL);
//     tv[2] = pthread_create(&threads[2], NULL, mockCheckStatus, NULL);
//     pthread_exit(NULL);
//     pthread_mutex_destroy(&lock);
//     return 0;
    
// }

// using signal handler


