#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define MIN_PID 300
#define MAX_PID 5000
#define BITS_IN_BYTE 8

static unsigned char *map;

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);
void release_map(void);

int main(void)
{
    int status = allocate_map();
    if (status == -1)
    {
        fprintf(stderr, "Could not allocate map\n");
        exit(EXIT_FAILURE);
    }
    int pid1 = allocate_pid();
    assert(pid1 == 300);
    int pid2 = allocate_pid();
    assert(pid2 == 301);
    int pid3 = allocate_pid();
    assert(pid3 == 302);

    release_pid(pid2);

    int pid4 = allocate_pid();
    assert(pid4 == 301);
    return 0;
}

int allocate_map(void)
{
    int map_size = (int)ceilf((MAX_PID - MIN_PID) * 1.0f / BITS_IN_BYTE);
    map = (unsigned char *)malloc(map_size);
    if (map == NULL)
    {
        return -1;
    }
    // we start with only ones (so the bits we won't use stay as one)
    for (int i = 0; i < map_size; i++)
    {
        map[0] = ~0;
    }
    // we reset the bits that'll be usable
    for (int i = 0; i < (MAX_PID - MIN_PID); i++)
    {
        div_t index = div(i, BITS_IN_BYTE);
        int index_map = index.quot;
        int index_bit = index.rem;
        map[index_map] &= ~(1 << index_bit);
    }
    return 1;
}
int allocate_pid(void)
{
    // all bits inside the range are 0
    int map_size = (int)ceilf((MAX_PID - MIN_PID) * 1.0f / BITS_IN_BYTE);
    for (int i = 0; i < map_size; i++)
    {
        // There is free space, we find, update and return that spot
        // ^ exclusive or
        if (map[i] ^ ~0)
        {
            unsigned char map_segment = map[i];
            int index_count = 0;
            // while the bit is full
            while (!(~map_segment & 1))
            {
                map_segment >>= 1;
                index_count++;
            }
            map[i] |= (1 << index_count);
            return MIN_PID + (i * BITS_IN_BYTE + index_count);
        }
    }
    return -1;
}
void release_pid(int pid)
{
    if (pid < MIN_PID || pid > MAX_PID)
    {
        return;
    }
    // bit 0 == MIN_PID
    int normed_pid = pid - MIN_PID;
    div_t index = div(normed_pid, BITS_IN_BYTE);
    int index_map = index.quot;
    int index_bit = index.rem;
    map[index_map] &= ~(1 << index_bit);
}
void release_map(void)
{
    free(map);
}