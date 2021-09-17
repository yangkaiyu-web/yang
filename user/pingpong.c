#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2];
    int p2[2];
    char buf[10];
    pipe(p1);
    pipe(p2);
    int fd=fork();
    if(fd==0)
    {
        close(p1[1]);
        close(p2[0]);
        read(p1[0],buf,1);
        fprintf(1,"%d: received ping\n",getpid());
        write(p2[1],buf,1);
        exit(0);
    }
    else
    {
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"a",1);
        read(p2[0],buf,1);
        fprintf(1,"%d: received pong\n",getpid());
    }
    exit(0);
}