//  Patrick Coughlin
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
#include "shadowParseTest.h"
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

static const char *shadow_path = "/shadow";

shadowDataNode *head;

/* typedef struct linked_alloc{ //holds head directory
    struct shadow_f *head;
} linked_dir;

typedef struct shadow_f{ //struct to hold all file information
    char user_name [50];
    char pwhash [50];
    int days_since_change;
    int min;
    int max;
    int warn;
    int inactive;
    int expire;
    struct shadow_f *next_f, *prev_f;
}shadow_f; */


//edit
static int shadow_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
	//returns a pointer to the next entry?
    //don't know how to edit for linked list?

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
    //edit to add a file node in a directory
    //malloc space for file?
    //
    
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
    //add dir to end of linked list
    //add loop to count to end of ll, call mkdir at last node
    
    int res;

    res = mkdir(path, mode);
    if (res == -1)
        return -errno;
}


static int shadow_unlink(const char *path)
{
    //delete file
    //anything special for linked list?
    
    int res;

    res = unlink(path);
    if (res == -1)
        return -errno;

    return 0;
}


static int shadow_rmdir(const char *path)
{
    //remove directory
    //need to move pointer to next before deleting
    
    int res;

    res = rmdir(path);
    if (res == -1)
        return -errno;

    return 0;
}


static int shadow_open(const char *path, struct fuse_file_info *fi)
{
    //anything special for linked list?
    
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
    //????
    
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}


static int shadow_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    //?????
    
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

void shadow_init(void) {   
	int res, i=0;
	shadow_node *x = head;
	char *path = shadow_path;
	char *attr_path = malloc(20*sizeof(char));
	char *str_int = malloc(10*sizeof(char));
    head = parse(head);
	shadowDataNode *x = head;

	for(i=0;i<lines;i++) { //lines is the length of the linked list
		strcpy(path, shadow_path);
		strcat(path, "/");
		strcat(path, "user_");
		strcat(path, "%s", itoa(i, str_int, 10));
		strcat(path, "/");
		res = mkdir(path, /* mode */ S_IRWXU);
		if (res == -1) return -errno;		
		
		//Name
		strcpy(attr_path, path);
		strcat(attr_path, "Username");
/* 		res = mknod(path, S_IRWXU, 0); //dev gets ignored, so we put in a simple value
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->user, sizeof(x->user), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Hash
		strcpy(attr_path, path);
		strcat(attr_path, "PW_Hash");
/* 		res = mknod(path, S_IRWXU, 0);
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->pw_hash, sizeof(x->pw_hash), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Since_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Since_Change");
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		itoa(x->numDays, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Can_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Can_Change");
/* 		res = mknod(path, S_IRWXU, 0);
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		itoa(x->daysCanChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Must_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Change");
/* 		res = mknod(path, S_IRWXU, 0);
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		itoa(x->daysMustChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Warn
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Warning");
/* 		res = mknod(path, S_IRWXU, 0);
		if (res == -1) return -errno;
 */		fd = open(path, O_CREAT | O_EXCL | O_WRONLY);
		if (fd == -1) return -errno;
		itoa(x->daysWarn, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		x = x->next //move to the ith node in the linked list
	}
	free(attr_path);
	free(str_int);
}
		
/*
To Do;
	- Set variables for system functions
	- Edit the functions above to work with our directory structure
*/


static struct fuse_operations shadow_oper = {
/*     .getattr	= shadow_getattr,
    .access		= shadow_access,
    .readlink	= shadow_readlink, */

    .readdir	= shadow_readdir, // need these
    .mknod		= shadow_mknod,
    .mkdir		= shadow_mkdir,

//    .symlink	= shadow_symlink,

    .unlink		= shadow_unlink, // need this
    .rmdir		= shadow_rmdir,

/*     .rename		= shadow_rename,
    .link		= shadow_link,
    .chmod		= shadow_chmod, //possibly
    .chown		= shadow_chown,
    .truncate	= shadow_truncate,
    .utimens	= shadow_utimens, */

    .open		= shadow_open, // need these
    .read		= shadow_read,
    .write		= shadow_write,

/*     .statfs		= shadow_statfs, //possibly
    .release	= shadow_release,
    .fsync		= shadow_fsync, */
#ifdef HAVE_SETXATTR
    .setxattr	= shadow_setxattr,
    .getxattr	= shadow_getxattr,
    .listxattr	= shadow_listxattr,
    .removexattr	= shadow_removexattr,
#endif
};

int main(int argc, char *argv[])
{
    shadow_init();
    return fuse_main(argc, argv, &shadow_oper, NULL);
}

