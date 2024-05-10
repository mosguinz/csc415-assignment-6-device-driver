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
#define MODE_ENCRYPT 'e'
#define MODE_DECRYPT 'd'

MODULE_AUTHOR("Mos Kullathon");
MODULE_DESCRIPTION("A simple device driver that encrypts text usinga Caesar cipher.");
MODULE_LICENSE("GPL");

static ssize_t my_read(struct file *fs, char __user *buf, size_t hsize, loff_t *off);
static ssize_t my_write(struct file *fs, const char __user *buf, size_t hsize, loff_t *off);
static int my_open(struct inode *inode, struct file *fs);
static int my_close(struct inode *inode, struct file *fs);
static long my_io_ctl(struct file *fs, unsigned int command, unsigned long data);

#define SHIFT_VALUE 5 // how many values to shift
#define MAX_SIZE 512  // buffer size
#define MAX_RANGE 255 // this is the ASCII range
static int encrypt(char *);
static int decrypt(char *);

int major, minor;

struct cdev my_cdev;
int actual_rx_size = 0;

/** Struct to hold text and whether it is encrypted. */
typedef struct myds
{
    char *text;      // text to encrypt/decrypt
    int isEncrypted; // 1 if encrypted, 0 if not
} myds;

/** File operations struct. */
struct file_operations fops = {
    .open = my_open,
    .release = my_close,
    .write = my_write,
    .read = my_read,
    .unlocked_ioctl = my_io_ctl,
    .owner = THIS_MODULE};

/** Write the text from the user into our `myds` */
static ssize_t my_write(struct file *fs, const char __user *buf, size_t hsize, loff_t *off)
{
    struct myds *ds;
    ds = (struct myds *)fs->private_data;
    copy_from_user(ds->text, buf + *off, hsize);
    return hsize;
}

/** Read the text from `myds` to return to user. */
static ssize_t my_read(struct file *fs, char __user *buf, size_t hsize, loff_t *off)
{
    struct myds *ds;
    ds = (struct myds *)fs->private_data;
    copy_to_user(buf + *off, ds->text, hsize);
    return 0;
}

/** Initialize `myds` and write to the file descriptor. */
static int my_open(struct inode *inode, struct file *fs)
{
    struct myds *ds;
    ds = vmalloc(sizeof(struct myds));
    if (!ds)
    {
        printk(KERN_ERR "Cannot vmalloc, File not opened\n");
        return -1;
    }
    ds->text = vmalloc(MAX_SIZE);
    ds->isEncrypted = 0;
    fs->private_data = ds;
    return 0;
}

/** Free `myds` and close the file descriptor. */
static int my_close(struct inode *inode, struct file *fs)
{
    struct myds *ds;
    ds = (struct myds *)fs->private_data;
    vfree(ds->text);
    vfree(ds);
    return 0;
}

/** Receive command from user and invoke the appropriate function. */
static long my_io_ctl(struct file *fs, unsigned int command, unsigned long data)
{
    int *count;
    struct myds *ds;
    ds = (struct myds *)fs->private_data;

    switch (command)
    {
    case MODE_ENCRYPT:
        encrypt(ds->text);
        ds->isEncrypted = 1;
        break;
    case MODE_DECRYPT:
        decrypt(ds->text);
        ds->isEncrypted = 0;
        break;
    default:
        printk(KERN_ERR "io_ctl: invalid mode, expecting 'e' or 'd', got %c\n",
               command);
        return -1;
    }
    return 0;
}

/** Simple Cesar cipher shift to encrypt given text. */
static int encrypt(char *text)
{
    printk(KERN_INFO "Encrypting: %s\n", text);
    for (size_t i = 0; i < strlen(text); i++)
    {
        text[i] = (text[i] + SHIFT_VALUE) % MAX_RANGE;
    }
    printk(KERN_INFO "Encrypted as: %s\n", text);
    return 0;
}

/** Simple Cesar cipher shift to decrypt given text. */
static int decrypt(char *text)
{
    printk(KERN_INFO "Decrypting: %s\n", text);
    for (size_t i = 0; i < strlen(text); i++)
    {
        text[i] = (text[i] - SHIFT_VALUE) % MAX_RANGE;
    }
    printk(KERN_INFO "Decrypted as: %s\n", text);
    return 0;
}

/** Register salad a device node in /dev */
int init_module(void)
{
    int result, registers;
    dev_t devno;

    devno = MKDEV(MY_MAJOR, MY_MINOR);
    registers = register_chrdev_region(devno, 1, DEVICE_NAME);
    printk(KERN_INFO "Register chardev suceeded 1: %d\n", registers);
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    result = cdev_add(&my_cdev, devno, 1);
    printk(KERN_INFO "Register chardev suceeded 2: %d\n", result);
    printk(KERN_INFO "Welcome - `salad` driver is loaded.\n");
    if (result < 0)
    {
        printk(KERN_ERR "Register chardev failed: %d\n", result);
    }
    return result;
}

/** Remove driver. */
void cleanup_module(void)
{
    dev_t devno;
    devno = MKDEV(MY_MAJOR, MY_MINOR);
    unregister_chrdev_region(devno, 1);
    cdev_del(&my_cdev);
    printk(KERN_INFO "Goodbye - `salad` driver is removed.\n");
}