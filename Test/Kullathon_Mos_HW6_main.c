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

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: main [e]ncrypt|[d]ecrypt text\n");
        return -1;
    }

    // Parse launch arguments
    char mode = argv[1][0];
    char *text = argv[2];

    switch (mode)
    {
    case MODE_ENCRYPT:
        printf("Encrypting: %s\n", text);
        break;
    case MODE_DECRYPT:
        printf("Decrypting: %s\n", text);
        break;
    default:
        printf("Invalid mode\n");
        return -1;
    };

    // Open the device driver
    int fd = open(DEV_LOCATION, O_RDWR); // read and write
    if (fd < 0)
    {
        printf("Device open error\n");
        return -1;
    }
    printf("Device open success\n");

    // Write text to driver
    write(fd, text, strlen(text));

    // Set mode and execute encrypt/decrypt
    ioctl(fd, mode);

    // Read from driver
    char *res = malloc(READ_BUFFER_SIZE);
    read(fd, res, strlen(text));
    printf("Result: %s\n", res);

    // Now, do the reverse
    ioctl(fd, mode == MODE_ENCRYPT ? MODE_DECRYPT : MODE_ENCRYPT);
    read(fd, res, strlen(text));
    printf("Result: %s\n", res);

    close(fd);
    return 0;
}
