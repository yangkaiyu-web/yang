#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // switch (st.type)
    // {
    // case T_FILE:
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    // break;

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        //printf("%s\t  %s\t  %d\n",de.name,filename,strcmp(filename,de.name));
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        int fd_temp;

         if ((fd_temp = open(buf, 0)) < 0)
        {
            fprintf(2, "find: cannot open %s\n", buf);
            return;
        }

        if (fstat(fd_temp, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            close(fd_temp);
            return;
        }

        switch (st.type)
        {
        case T_DIR:
            if(strcmp(".",de.name)&&strcmp("..",de.name))
                find(buf,filename);
            break;
        case T_FILE:{
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (strcmp(filename, de.name) == 0)
                printf("%s/%s\n", path, fmtname(buf));
            break;
        }
        default:
            break;
        }
        close(fd_temp);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    find(argv[1], argv[2]);
    exit(0);
}