//
//  shadow_fs.c
//  
//
//  Team 6
//  CSCI 3411 - Operating Systems
//  Fall 2014
//

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif


#include "shadow_fs.h"
#include "list.h"
#include “Cash_list.h”
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h> //struct for file
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

//need global shadow file?

typedef struct linked_alloc{ //holds head directory
    struct shadow_f *head;
} linked_dir;

typedef struct shadow_f{ //struct to hold all file information
    char user_name [50];
    char password [50];
    int days_since_change;
    int min;
    int max;
    int warn;
    int inactive;
    int expire;
    struct shadow_f *next_f, *prev_f;
}shadow_f;


//edit
static int shadow_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
	//returns a pointer to the next entry
	
    DIR *dp;
    struct dirent *de;
    
    (void) offset;
    (void) fi;
    
    dp = opendir(path);
    if (dp == NULL)
        return -errno;
    
    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }
    
    closedir(dp);
    return 0;
}


static int shadow_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;

    if (S_ISREG(mode)) {
        res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0)
            res = close(res);
    } else if (S_ISFIFO(mode))
        res = mkfifo(path, mode);
    else
        res = mknod(path, mode, rdev);
    if (res == -1)
        return -errno;
    
    return 0;
}


static int shadow_mkdir(const char *path, mode_t mode)
{
    int res;
    
    res = mkdir(path, mode);
    if (res == -1)
        return -errno;
}


static int shadow_unlink(const char *path)
{
    int res;
    
    res = unlink(path);
    if (res == -1)
        return -errno;
    
    return 0;
}


static int shadow_rmdir(const char *path)
{
    int res;
    
    res = rmdir(path);
    if (res == -1)
        return -errno;
    
    return 0;
}


static int shadow_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    
    res = open(path, fi->flags);
    if (res == -1)
        return -errno;
    
    close(res);
    return 0;
}


static int shadow_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    if(strcmp(path, hello_path) != 0)
        return -ENOENT;
    
    len = strlen(hello_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, hello_str + offset, size);
    } else
        size = 0;
    
    return size;
}


static int shadow_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    
    (void) fi;
    fd = open(path, O_WRONLY);
    if (fd == -1)
        return -errno;
    
    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    
    close(fd);
    return res;
}

/*
shadow_f *shadow_create(void)
{
    shadow_f *makelist = malloc(sizeof(linked_alloc));
    
}*/



static struct fuse_operations shadow_oper = {
    .getattr	= shadow_getattr,
    .access		= shadow_access,
    .readlink	= shadow_readlink,
    
    .readdir	= shadow_readdir, // need these
    .mknod		= shadow_mknod,
    .mkdir		= shadow_mkdir,
    
    .symlink	= shadow_symlink,
   
    .unlink		= shadow_unlink, // need this
    .rmdir		= shadow_rmdir,
    
    .rename		= shadow_rename,
    .link		= shadow_link,
    .chmod		= shadow_chmod, //possibly
    .chown		= shadow_chown,
    .truncate	= shadow_truncate,
    .utimens	= shadow_utimens,
    
    .open		= shadow_open, // need these
    .read		= shadow_read,
    .write		= shadow_write,
    
    .statfs		= shadow_statfs, //possibly
    .release	= shadow_release,
    .fsync		= shadow_fsync,
#ifdef HAVE_SETXATTR
    .setxattr	= shadow_setxattr,
    .getxattr	= shadow_getxattr,
    .listxattr	= shadow_listxattr,
    .removexattr	= shadow_removexattr,
#endif
    
int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &shadow_oper, NULL);
}

