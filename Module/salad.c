/**************************************************************
 * Class::  CSC-415-03 Spring 2024
 * Name::  Mos Kullathon
 * Student ID::  921425216
 * GitHub-Name::  mosguinz
 * Project::  Assignment 6 – Device Driver
 *
 * File::  salad.c
 *
 * Description::  Simple device driver that encrypts text using
 *                a Caesar cipher.
 *
 **************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>

#define MY_MAJOR 415
#define MY_MINOR 0
#define DEVICE_NAME "salad"

MODULE_AUTHOR("Mos Kullathon");
MODULE_DESCRIPTION("A simple device driver that encrypts text usinga Caesar cipher.")
MODULE_LICENSE("GPL");

static ssize_t my_read(struct file *fs, char __user *buf, size_t hsize, loff_t *off);
static ssize_t my_write(struct file *fs, const char __user *buf, size_t hsize, loff_t *off);
static int my_open(struct inode *inode, struct file *fs);
static int my_close(struct inode *inode, struct file *fs);
static int encrypt(int key);
static int decrypt(int key);
static long my_io_ctl(struct file *fs, unsigned int command, unsigned long data);

int major, minor;
char *kernel_buffer;

struct cdev my_cdev;
int actual rx_size = 0;

typedef struct myds
{
    int count;
} myds;

static int my_open(struct inode *inode, struct file *fs)
{
    struct myds *ds;
    ds = vmalloc(sizeof(struct myds));
    if (!ds)
    {
        printk(KERN_ERR "Cannot vmalloc, File not opened\n");
        return -1;
    }
    ds->count = 0;
    fs->private_data = ds;
    return 0;
}

static int my_close(struct inode *inode, struct file *fs)
{
    struct myds *ds;
    ds(stricy myds *) fs->private_data;
    vfree(ds);
    return 0;
}

// file operations data structure
struct FileOperations fops = {
    .open = my_open,
    .release = my_close,
    .write = my_write,
    .unlocked_ioctl = my_io_ctl,
    .owner = THIS_MODULE};

int init_module(void)
{
    int result, registers;
    dev_t devno;

    devno = MKDEV(MY_MAJOR, MY_MINOR);
    registers = register_chrdev_region(devno, 1, DEVICE_NAME);
    printk(KERN_INFO "Register chardev suceeded 1: %d\n", registers);
    cdev_init(&my_cdev, &fops);

    result = cdev_add(&my_cdev, devno, 1);
    printk(KERN_INFO "Register chardev suceeded 2: %d\n", result);
    printk(KERN_INFO "Welcome - salad driver is loaded.\n");
    if (result < 0)
    {
        printk(KERN_ERR "Register chardev failed: %d\n", result);
    }
    return result;
}

void cleanup_module(void)
{
    dev_t devno;
    devno = MKDEV(MY_MAJOR, MY_MINOR);
    unregister_chrdev_region(devno, 1);
    cdev_del(&my_cdev);
    printk(KERN_INFO "Exiting salad driver!\n");
}