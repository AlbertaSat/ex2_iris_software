#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>


// to serve as flags
bool accessImage = false;
bool commInfo = true;

// For mocking interface
void toggleState(bool& flag){
    // toggles the state of the flag passed
    flag = ~flag;
}

void *mockImageRequest(void){
    while(1){
        masterFunc(2);
        sleep(5);
    }
}

void *mockCommInfo(void){
    sleep(2);
    while(1){
        masterFunc(2);
        sleep(5);
    }
}

int main (){
    
    printf("%d", accessImage);
}


