/**************************************************************
 * Class::  CSC-415-03 Spring 2024
 * Name::  Mos Kullathon
 * Student ID::  921425216
 * GitHub-Name::  mosguinz
 * Project::  Assignment 6 – Device Driver
 *
 * File::  Mos_Kullathon_HW6_main.c
 *
 * Description::  Device driver test application.
 *
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define READ_BUFFER_SIZE 512
#define DEV_LOCATION "/dev/salad"
#define MODE_ENCRYPT 'e'
#define MODE_DECRYPT 'd'

void show_help()
{
    printf("usage: main ( h | e[ncrypt] | d[ecrypt] ) text\n");
    printf("For example: use 'main e abcdef' to encrypt the text 'abcdef'\n");
    printf("             use 'main d fghijk' to decrypt the text 'fghijk'\n");
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Not enough arguments\n");
        show_help();
        return -1;
    }

    // Parse launch arguments
    const char mode = argv[1][0];
    const char *text = argv[2];

    // Show help message
    if (mode == 'h')
    {
        show_help();
        return 0;
    }

    // Incorrect mode
    if (mode != MODE_DECRYPT && mode != MODE_ENCRYPT)
    {
        fprintf(stderr, "Invalid mode: must be 'e' or 'd': got '%c'\n", mode);
        return -1;
    }

    // Open the device driver
    int fd = open(DEV_LOCATION, O_RDWR); // read and write
    if (fd < 0)
    {
        fprintf(stderr, "Device open error\n");
        return -1;
    }

    // Write text to driver
    write(fd, text, strlen(text));

    // Set mode and execute encrypt/decrypt
    ioctl(fd, mode);

    // Read from driver
    char *res = malloc(READ_BUFFER_SIZE);
    read(fd, res, strlen(text));
    printf("%s\n", res);

    // Close file descriptor
    close(fd);
    return 0;
}
