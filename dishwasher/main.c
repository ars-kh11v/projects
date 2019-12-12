#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define TABLE_LIMIT 2

typedef struct Dishes
{
    char* name;
    unsigned int wash_time;
    unsigned int dry_time;
} Dish;

//the place for different experiments
Dish plate = {"plate",1,8};
Dish cup = {"cup",1,6};
Dish knife = {"knife",1,4};
Dish spoon = {"spoon",1,4};

Dish DefineDish(char* name)
{
    if (!strcmp(name, "plate"))
        return plate;
    if (!strcmp(name, "cup"))
        return cup;
    if (!strcmp(name, "knife"))
        return knife;
    if (!strcmp(name, "spoon"))
        return spoon;
    exit(5);
}

void SemCreate(int *semid, int count)
{
    char pathname[] = "key";

    key_t key;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((*semid = semget(key, count, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }
}


void SemOperation(int semid, short operation, unsigned short sem_mumber)
{
    struct sembuf mybuf;

    mybuf.sem_op = operation;
    mybuf.sem_flg = 0;
    mybuf.sem_num = sem_mumber;
    semop(semid, &mybuf, 1);
}

int main(void)
{
    int semid;
    SemCreate(&semid, 1);
    SemOperation(semid, TABLE_LIMIT, 0);

    int msqid;
    char pathname[]="help";
    key_t  key;
    size_t len = sizeof(Dish);

    typedef struct mymsgbuf
    {
        long mtype;
        Dish current_dish;
    } msg_buf;


    key = ftok(pathname, 0);

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    pid_t pid = fork(); // parent - washer, child - dryer
    if ( pid == 0)
    {
        msg_buf rcv_buf;
        while (1)
        {
            if (msgrcv(msqid, &rcv_buf, len, 1, 0) < 0)
            {
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }
            Dish current_dish = rcv_buf.current_dish;
            printf("Dryer took the %s.\n", current_dish.name);
            sleep(current_dish.dry_time);
            printf("Dryer finished dry the %s.\n", current_dish.name);
            SemOperation(semid, 1, 0);
        }
    }
    else
    {
        FILE *fin;
        fin = fopen("dirty_heap.txt","r");
        if(errno){
            perror(":(");
            exit(1);
        }
        int N;
        fscanf(fin,"%d", &N);
        char name[MAX_STRING_SIZE];
        int count;
        Dish current_dish;
        msg_buf snd_buf;
        for (int i = 0; i < N; ++i) {
            fscanf(fin, "%s%d", name, &count);
            current_dish = DefineDish(name);
            for (int j = 0; j < count; ++j) {
                //printf("Washer took a %s (%i)\n", current_dish.name, j + 1);
                sleep(current_dish.wash_time);
                //printf("Washer washed the %s (%i).\n", current_dish.name, j + 1);

                //sending
                snd_buf.mtype = 1;
                snd_buf.current_dish = current_dish;
                if (msgsnd(msqid, &snd_buf, len, 0) < 0)
                {
                    printf("Can\'t send message to queue\n");
                    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                    exit(-1);
                }

                printf("Washer put the %s (%i) on the table.\n", current_dish.name, j + 1);
                SemOperation(semid, -1, 0);

            }
        }
        fclose(fin);
        int status;
        wait(&status);
    }

    return 0;
}

