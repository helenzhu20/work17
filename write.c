#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define KEY 24601

int main(){
    int semd = semget(KEY, 1, 0);
    if (semd == -1) {
        printf("Error: semaphore does not exist\n");
        return 0;
    }
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    
    int shmd = shmget(KEY, 0, 0);
    int *data = shmat(shmd, 0, 0);

    char line[100];
    int file = open("story.txt", O_RDWR | O_APPEND);

    struct stat st;
    stat("story.txt", &st);
    if (st.st_size > 0) {
        lseek(file, -(*data), SEEK_END);
        read(file, line, sizeof(line));
        printf("Previous line: %s\n", line);
    }

    fgets(line, sizeof(line), stdin);
    write(file, line, strlen(line));
    *data = strlen(line);

    close(file);
    shmdt(data);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
}