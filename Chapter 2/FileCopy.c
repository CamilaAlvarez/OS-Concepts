#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_FILENAME_LENGTH 151

int main(void)
{
    char source[MAX_FILENAME_LENGTH], dest[MAX_FILENAME_LENGTH];
    FILE *input_file, *dest_file;
    int read_char;
    printf("Enter a source file: ");
    // TODO: Might want to make something more sophisticated in case the inout doesn't fit
    if (fgets(source, MAX_FILENAME_LENGTH, stdin) == NULL)
    {
        printf("Could not obtain source file\n");
        exit(EXIT_FAILURE);
    }
    // Remove newline from source. fopen won't work if we keep the newline
    // strcspn return the index of the first char in source that appears in "\r\n"
    source[strcspn(source, "\r\n")] = '\0';
    if ((input_file = fopen(source, "r")) == NULL)
    {
        printf("Could not open source file! %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Enter a destination file: ");
    if (fgets(dest, MAX_FILENAME_LENGTH, stdin) == NULL)
    {
        printf("Could not obtain destination file\n");
        exit(EXIT_FAILURE);
    }
    // Remove newline from dest
    dest[strcspn(dest, "\r\n")] = '\0';
    if ((dest_file = fopen(dest, "w")) == NULL)
    {
        printf("Could not open dest file! %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((read_char = fgetc(input_file)) != EOF)
    {
        fputc(read_char, dest_file);
    }
    fclose(input_file);
    fclose(dest_file);
    return 0;
}