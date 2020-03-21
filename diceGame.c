#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<sys/shm.h>
#include<signal.h>

void action(){};
void player(char *name, int playerId, int *sharedArray)
{
    int dice;
    long int ss = 0;
    while(1)
  {
    signal(SIGUSR1, action);
    pause();
    printf("%s: playing my dice\n", name);
    dice =(int) time(&ss)%10 + 1;
    printf("%s: got %d points\n", name, dice);
    sharedArray[playerId] +=dice;
    printf("%s: Total so far %d\n\n", name, sharedArray[playerId]);
    sleep(2);
    if(sharedArray[playerId] >= 50)
    {
      printf("%s: game over I won\n", name);
      kill(0, SIGTERM);
    }
    kill(getppid(), SIGUSR1);
  }
}
int main(int argc, char *argv[])
{
    // Initialization of shared memory
    int SharedSegment = shmget(IPC_PRIVATE, 3*sizeof(int), IPC_CREAT|0600);
    if(SharedSegment < 0)
    {
        perror("Creation failed");
        exit(1);
    }
    printf("Shared memory created\n");
    // Attachment to the shared memory 
    int *sharedArray = (int *)shmat(SharedSegment, NULL, 0);
    sharedArray[0] = 0;
    sharedArray[1] = 0;
    sharedArray[2] = 0;
    if((int)sharedArray == -1)
    {
        perror("Attachment Failed");
    }
    pid_t pid1, pid2, pid3;
    printf("This is a 3-players game with a referee\n"); 
    if((pid1=fork()) == 0) player("TATA", 0, sharedArray); 
    if((pid2=fork()) == 0) player("TITI", 1, sharedArray);
    if((pid3=fork()) == 0) player("TOTO", 2, sharedArray);
        sleep(1);
    signal(SIGUSR1, action);
    while(1)
    {
        printf("\nReferee: TATA plays\n\n");
        kill(pid1, SIGUSR1);
        pause();
        printf("\n\nReferee: TITI plays\n\n");
        kill(pid2, SIGUSR1);
        pause();
        printf("\nReferee: TOTO plays\n\n");
        kill(pid3, SIGUSR1);
        pause();
     } 
    //  Detaching
    int retval = shmdt(sharedArray);
    if(retval == -1)
    {
        printf("Detachment failed");
    }
    else
    {
      printf("Detached");
    }
    return 0;
}