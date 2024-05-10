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

    fd = open(DEV_LOCATION, O_WRONLY);
    printf("Returned from open(): %d\n", fd);

    if (fd < 0)
    {
        printf("Device open error\n");
        perror("Device file open error");
        return -1;
    }
    printf("Device open success\n");

    for (long i = 0; i < 30; i++)
    {
        n1 = write(fd, "testing", i * 2);
        n2 = ioctl(fd, 3, &info);
        printf("Wrote to %d, that returned %ld, count %d - Return from ioctl %i\n", fd, n1, i, n2);
    }

    close(fd);
    return 0;
}
