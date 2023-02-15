// gettimeofday, struct timeval
#include <sys/time.h>
// fork
#include <unistd.h>
// shared memory
#include <sys/mman.h>
// Shared memory constants
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED_MEMORY "shared_memory"
#define SHARED_MEMORY_SIZE sizeof(unsigned int)

// Shared memormy version
/*int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing command!\n");
        exit(EXIT_FAILURE);
    }
    struct timeval time;
    // shared memory: before the fork so the child has access as well
    int *shared_ptr;
    int shared_fd = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);
    ftruncate(shared_fd, SHARED_MEMORY_SIZE);
    shared_ptr = (int *)mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        gettimeofday(&time, NULL);
        shared_ptr[0] = time.tv_sec;
        shm_unlink(SHARED_MEMORY);
        // first argument is the binary to load, second argument is the name of the process, list of args
        // must end with NULL
        execlp(argv[1], argv[1], NULL);
    }
    else
    {
        wait(NULL);
        gettimeofday(&time, NULL);
        unsigned int end_time = time.tv_sec;
        unsigned int start_time = shared_ptr[0];
        printf("Elpased time: %d\n", end_time - start_time);
    }
    return 0;
}*/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Missing command!\n");
        exit(EXIT_FAILURE);
    }
    struct timeval time;
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        gettimeofday(&time, NULL);
        // Close read end
        close(fd[0]);
        // since we know that both ends of the pipe were compiled with the same compiler!
        // we can cast the time to (void *)
        write(fd[1], &(time.tv_sec), SHARED_MEMORY_SIZE);
        close(fd[1]);
        // first argument is the binary to load, second argument is the name of the process, list of args
        // must end with NULL
        execlp(argv[1], argv[1], NULL);
    }
    else
    {
        // Close write end
        close(fd[1]);
        wait(NULL);
        gettimeofday(&time, NULL);
        unsigned int end_time = time.tv_sec;
        unsigned int start_time;
        read(fd[0], &start_time, SHARED_MEMORY_SIZE);
        printf("Elpased time: %d\n", end_time - start_time);
        close(fd[0]);
    }
    return 0;
}