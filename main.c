// SO IN1 20C LAB05
// Robert Franczak
// fr47434@zut.edu.pl
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        printf("Błędna ilośc argumentów \n");
        return 1;
    }

    int arg_length = strlen(argv[1]);

    if(arg_length > 20)
    {
        printf("Argument ma za dużo znaków \n");
        return 1;
    }

    if(arg_length == 1)
        return argv[1][0];

    int first_half  = arg_length/2;
    int second_half = arg_length - first_half;

    char* tabFirst  = malloc(sizeof(char) * (first_half  + 1));
    char* tabSecond = malloc(sizeof(char) * (second_half + 1));

    strncpy(tabFirst,argv[1],first_half);
    strncpy(tabSecond,argv[1] + first_half,second_half);

    int PID1 = fork();
    int status1;

    if(0 == PID1)
    {
        execlp(argv[0],argv[0],tabFirst,NULL); // 1
        fprintf(stderr, "exec failed: %s\n", strerror(errno));
        return 1;
    }

    int PID2 = fork();
    int status2;
    if(0 == PID2)
    {
        execlp(argv[0],argv[0],tabSecond,NULL); // 2
        fprintf(stderr, "exec failed: %s\n", strerror(errno));
        return 1;
    }
    
    int child1 = waitpid(PID1,&status1,0);
    int child2 = waitpid(PID2,&status2,0);

    printf("Parent PID: %6d CHILD 1 PID: %6d STRING%6s CHAR:%c \n",getpid(),child1,tabFirst, WEXITSTATUS(status1));
    printf("Parent PID: %6d CHILD 2 PID: %6d STRING%6s CHAR:%c \n",getpid(),child2,tabSecond,WEXITSTATUS(status2));

    int result = 0;
    result = WEXITSTATUS(status1) > WEXITSTATUS(status2) ? WEXITSTATUS(status1) : WEXITSTATUS(status2);

    return result;
}


