/*
*   Name: Thiago André Ferreira Medeiros
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* the size (in bytes) of shared memory object */
const int SIZE = 4096;

/* name of the shared memory object */
const char *shMemName = "tmedeiros";

const char *childMessage = "From Child 1";

bool validateParameters(int argc, char **argv) {
    if (argc != 2) {
        printf("Received number of parameters different than 1.\n");

        return false;
    }

    int n = atoi(argv[1]);

    if (n <= 0 || n >= 40) {
        printf("Parameter is not a positive integer less than 40.\n");

        return false;
    }

    return true;
}


int main(int argc, char *argv[]) {
    if (!validateParameters(argc, argv)) {
        return -1;
    }

    int n = atoi(argv[1]);

    /* create the shared memory object */
    int fd = shm_open(shMemName, O_CREAT | O_RDWR,0666);
    /* configure the size of the shared memory object */
    ftruncate(fd, SIZE);
    /* pointer to shared memory object */
    char *mainptr = (char *) mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid = fork();

    if (pid < 0) { /* error occurred */
        perror("Fork failed");
    }

    else if (pid == 0) { /* child process */
        mainptr += sprintf(mainptr,"%s init n=%d, ", childMessage, n);

        while (n != 1) {
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = (n * 3) + 1;
            }
            mainptr += sprintf(mainptr,"%s n=%d, ", childMessage, n);
        }
    }

    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);

        printf("%s",(char *)mainptr);

        shm_unlink(shMemName);
    }

    return 0;
}
