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

int create() {

    int v, r; 
    int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
        printf("Error: Semaphore exists\n");
        return 0;
    }
    union semun us;
    us.val = 1;
    r = semctl(semd, 0, SETVAL, us);
    printf("Created semaphore\n");

    int shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0600);
    printf("Created shared memory\n");

    int file = open("story.txt", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
    printf("Created file\n");

    return 0;
}

int remove_files() {
    int semd = semget(KEY, 1, 0);
    semctl(semd, IPC_RMID, 0);
    printf("Removed semaphore\n");

    int shmd = shmget(KEY, 0/*?*/, 0);
    shmctl(shmd, IPC_RMID, 0);
    printf("Removed shared memory\n");
    
    struct stat st;
    stat("story.txt", &st);
    if (st.st_size > 0) {
        int file = open("story.txt", O_RDONLY);
        char lines[st.st_size + 1];
        lines[st.st_size] = '\0';
        read(file, lines, st.st_size);
        printf("%s\n", lines);
        close(file);
    }

    remove("story.txt");
    printf("Removed file\n");

    return 0;
}
//hi
//my
//name is paul
//i like pineapples

/*

Control program:
Should take a command line argument to determine if it is creating or removing the resources.
If creating:
If removing
Remove the shared memory and the semaphore
Display the full contents of the story.


The writing program
Will attempt to access the resources using the semaphore
Once in, it should display the last line added to the file (the shared memory should contain the size of the last line).
Then prompt the use for the next line.
Once a new line is added, it should write that to the file, update the shared memory and then release the semaphore

*/



int main(int argc, char *argv[]){
    if (argv[1] && !strcmp(argv[1], "create")) {
        create();
    } else if (argv[1] && !strcmp(argv[1], "remove")) {
        remove_files();
    } else {
        printf("Invalid Input: Please input either 'create' or 'remove'\n");
    }

    return 0;
}