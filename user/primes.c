#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void f(int t)
{
    //printf("jbjds\n");
    char criterion;
    if (read(t, &criterion, sizeof(char)) == 0)
        return;
    printf("prime %d\n", (int)criterion);
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        close(fd[1]);
        f(fd[0]);
        close(fd[0]);
        exit(0);
    }
    else
    {
        close(fd[0]);
        char c;
        while (read(t, &c, sizeof(char)))
        {
            if ((int)c % (int)criterion != 0)
            {
                write(fd[1], &c, sizeof(char));
            }
        }
        close(fd[1]);
        wait(0);
    }
}

int main()
{
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        close(fd[1]);
        //printf("sjdn\n");
        f(fd[0]);
        close(fd[0]);
    }
    else
    {
        int i;
        close(fd[0]);
        for (i = 2; i <= 35; ++i)
        {
            write(fd[1], (char *)&i, sizeof(char));
        }
        close(fd[1]);
        wait(0);
    }

    exit(0);
}