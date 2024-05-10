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

int main(int argc, char const *argv[])
{
    int fd, info;
    long n1, n2, n3;
    char Message[] = {"test message"};
    char read_buffer[READ_BUFFER_SIZE];

    if (argc < 3)
    {
        fprintf(stderr, "usage: main [e]ncrypt|[d]ecrypt message\n");
        return -1;
    }

    char mode = argv[1][0];
    char *message = argv[2];

    if (mode == 'e')
    {
        printf("Encrypting %s\n", message);
    }
    else if (mode == 'd')
    {
        printf("Decrypting %s\n", message);
    }
    else
    {
        printf("Invalid mode\n");
        return -1;
    }

    fd = open(DEV_LOCATION, O_RDWR); // read and write
    printf("Returned from open(): %d\n", fd);

    if (fd < 0)
    {
        printf("Device open error\n");
        perror("Device file open error");
        return -1;
    }
    printf("Device open success\n");

    char *text = "malloc(READ_BUFFER_SIZE)";
    write(fd, text, strlen(text));

    char *res = malloc(READ_BUFFER_SIZE);
    read(fd, res, strlen(text));

    printf("Coming from read: %s\n", res);

    // for (long i = 0; i < 30; i++)
    // {
    //     n1 = write(fd, "testing", i * 2);
    //     n2 = ioctl(fd, 3, &info);
    //     printf("Wrote to %d, that returned %ld, count %d - Return from ioctl %i\n", fd, n1, i, n2);
    // }

    close(fd);
    return 0;
}
