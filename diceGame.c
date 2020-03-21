#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>


int main(int argc, char *argv[])
{
    void *memory = NULL;
    int *p;
    int retval;
    // Initialization of shared memory
    int schmid = shmget(IPC_PRIVATE, 10*sizeof(double), IPC_CREAT|0600);
    if(schmid < 0)
    {
        perror("Creation failed");
    }
    printf("Shared memory created");
    // Attachment to the shared memory 
    memory = schmat(schmid, NULL, 0);
    if(p == NULL)
    {
        perror("Attachment Failed");
    }
    p = (int*) memory;
    // Detaching
    retval = shmdt(p);
    if(retval < 0)
    {
        printf("Suffered Detachment");
    }
    return 0;
}