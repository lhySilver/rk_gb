#include <stdio.h>
#include <unistd.h>
int StartOnvifDiscoveryPthread(void);
int StartOnvifPthread();
int main_test()
{
    printf("StartOnvifDiscoveryPthread start!\n");
    StartOnvifDiscoveryPthread();
    printf("StartOnvifDiscoveryPthread end!\n");

    printf("StartOnvifPthread start!\n");
    StartOnvifPthread();
    printf("StartOnvifPthread end!\n");

    while(1)
    {
        //printf("main Test!\n");
        sleep(2);
    }
    return 0;
}