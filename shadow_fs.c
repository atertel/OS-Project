//  Patrick Coughlin, Cassius Ali, Richard Lancia, Alex Ertel
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
#define _XOPEN_SOURCE 500
#endif


#include "shadow_fs.h"
#include "list.h"
#include "shadowParseTest.h"
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
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

static int shadow_getattr(const char *path, struct stat *stbuf)
{
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int shadow_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{

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

/* void shadow_update() { //Unfinished; Should write back to the shadow file from Directory Structure
	int res, i=0;
	char *path = shadow_path;
	char *attr_path = malloc(20*sizeof(char));
	char *str_int = malloc(10*sizeof(char));

	for(i=0;i<lines;i++) { //lines is the length of the linked list
		strcpy(path, shadow_path);
		strcat(path, "/");
		strcat(path, "user_");
		strcat(path, "%s", itoa(i, str_int, 10));
		res = mkdir(path, S_IRWXU);
		if (res == -1) return -errno;		
		strcat(path, "/");
		
		//Name
		strcpy(attr_path, path);
		strcat(attr_path, "Username");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->user, sizeof(x->user), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Hash
		strcpy(attr_path, path);
		strcat(attr_path, "PW_Hash");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->pw_hash, sizeof(x->pw_hash), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Since_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Since_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->numDays, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Can_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Can_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->daysCanChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Must_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->daysMustChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Warn
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Warning");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
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
 */
static int shadow_init(void) {   
	int res, i=0;
	char *path;
	char *attr_path = malloc(20*sizeof(char));
	char *str_int = malloc(10*sizeof(char));
    head = parse(head);
	shadowDataNode *x = head;

	for(i=0;i<lines;i++) { //lines is the length of the linked list
		strcpy(path, shadow_path);
		strcat(path, "/");
		strcat(path, "user_");
		strcat(path, itoa(i, str_int, 10));
		res = mkdir(path, S_IRWXU);
		if (res == -1) return -errno;		
		strcat(path, "/");
		
		//Name
		strcpy(attr_path, path);
		strcat(attr_path, "Username");
		int fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->user, sizeof(x->user), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Hash
		strcpy(attr_path, path);
		strcat(attr_path, "PW_Hash");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		res = pwrite(fd, x->pw_hash, sizeof(x->pw_hash), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Since_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Since_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->numDays, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Can_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Can_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->daysCanChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Must_Change
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Change");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->daysMustChange, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		//Days_Warn
		strcpy(attr_path, path);
		strcat(attr_path, "Days_Until_Warning");
		fd = open(attr_path, O_CREAT | O_EXCL | O_RDWR);
		if (fd == -1) return -errno;
		itoa(x->daysWarn, str_int, 10);
		res = pwrite(fd, str_int, sizeof(str_int), 0);
		close(fd);
		if (res == -1) return -errno;
		
		x = x->next; //move to the ith node in the linked list
	}
	free(attr_path);
	free(str_int);
	return 0;
}

static struct fuse_operations shadow_oper = {
    .getattr	= shadow_getattr,
    .readdir	= shadow_readdir,
    .mknod		= shadow_mknod,
    .mkdir		= shadow_mkdir,
    .unlink		= shadow_unlink,
    .rmdir		= shadow_rmdir,
    .open		= shadow_open,
    .read		= shadow_read,
    .write		= shadow_write,

#ifdef HAVE_SETXATTR
    .setxattr	 = shadow_setxattr,
    .getxattr	 = shadow_getxattr,
    .listxattr	 = shadow_listxattr,
    .removexattr = shadow_removexattr,
#endif
};

int main(int argc, char *argv[])
{
	int res;
	res = shadow_init();
	if (res == -1) return -errno;
    return fuse_main(argc, argv, &shadow_oper, NULL);
}
