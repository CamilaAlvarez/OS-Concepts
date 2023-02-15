#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BASE 10
#define SHARED_MEMORY "shared_memory"
/* we assume that the data fits this size. If it doesn't we would have to call ftruncate again to extend the memoy
    To do so we need to know how many numbers fit in the first place and how many we have already. The latter is known
    by keeping an index
    We can have mmap reserve continuos memory without actually allocating it
    mmap(NULL, 1U << 30, PROT_NONE(o MAP_NORESERVE), MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
*/
#define SHARED_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "Missing input number\n");
        exit(EXIT_FAILURE);
    }
    // reset errno
    errno = 0;
    int input_number = (int)strtol(argv[1], (char **)NULL, BASE);

    if (input_number == 0 && errno != 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (input_number < 0)
    {
        fprintf(stderr, "We only accept possitve numbers!\n");
        exit(EXIT_FAILURE);
    }
    int *shared_ptr;
    int shared_fd = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);
    ftruncate(shared_fd, SHARED_SIZE);
    shared_ptr = (int *)mmap(0, SHARED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Missing input number\n");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
        while (*shared_ptr != 1)
        {
            printf("%d, ", *shared_ptr);
            shared_ptr++;
        }
        printf("%d\n", *shared_ptr);
        shm_unlink(SHARED_MEMORY);
    }
    else
    {
        int i = 0;
        while (input_number != 1)
        {
            shared_ptr[i] = input_number;
            if (input_number % 2 == 0)
            {
                input_number /= 2;
            }
            else
            {
                input_number = 3 * input_number + 1;
            }
            i++;
        }
        shared_ptr[i] = 1;
        exit(EXIT_SUCCESS);
    }
    return 0;
}