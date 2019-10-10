#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_STRING_SIZE 200


int main(int argc, char *argv[])
{
    int fd;
    pid_t result;
    char* string;
    int ind = atoi(argv[1]);
    char* fifo[2];

    fifo[ind] = "1.fifo";
    fifo[1 - ind] = "2.fifo";
    
    for (int i = 0; i < 2; ++i)
    {
    	if ((mknod(fifo[i], S_IFIFO | 0666, 0) < 0) && (errno != EEXIST))
    	{
        	printf("Can\'t create FIFO\n");
        	exit(-1);
    	}
	}

    if((result = fork()) < 0)
    {
        exit(-1);
    }
    if (result > 0)
    {
        fd = open(fifo[1], O_WRONLY);
        if (fd  == -1)
        {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }
        string = (char*)calloc(MAX_STRING_SIZE, sizeof(char));
        while(1)
        {
            fgets(string, MAX_STRING_SIZE, stdin);
            write(fd, string, strlen(string));
        }
        free(string);
        close(fd);

    }
    else
    {
        fd = open(fifo[0], O_RDONLY);
        if (fd  == -1)
        {
            printf("Can\'t open FIFO for reading\n");
            exit(-1);
        }
        string = (char*)calloc(MAX_STRING_SIZE, sizeof(char));
        while(1)
        {
            int size = read(fd, string, MAX_STRING_SIZE);
            if (size == 0)
            {
                printf("Another programm exited\n");
                free(string);
                exit(0);
            }
            printf("- %s", string);
        }
        free(string);
        close(fd);
    }
    return 0;
}
