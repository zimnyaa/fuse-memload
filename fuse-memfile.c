#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "encfile.h"

static const char *filepath = "/memfile";
static const char *filename = "memfile";

char filecontent_key = 0xff;

char open_count = 0;
static char open_threshold = 1;

static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }

  if (strcmp(path, filepath) == 0) {
    stbuf->st_mode = S_IFREG | 0755;
    stbuf->st_nlink = 1;
    stbuf->st_size = filecontent_len;
    return 0;
  }

  return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  filler(buf, filename, NULL, 0);

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
  open_count++;
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {

  if (strcmp(path, filepath) == 0) {

  	if (offset == 0 && open_count > open_threshold)
  		return -ENOENT;
    size_t len = filecontent_len;
    if (offset >= len) {
      return 0;
    }

    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  return -ENOENT;
}

static struct fuse_operations fuse_memload_operations = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[], char *envp) {
  printf("main: decoding with key %x", filecontent_key);
  for (size_t i=0;i<filecontent_len;i++)
    filecontent[i] = filecontent[i] ^ filecontent_key;

  char filemountpath[1024] = {0};

  snprintf(filemountpath, 1023, "%s%s", fuse_argv[1], filepath);
  printf("main: filemountpath: %s\n", filemountpath);
  printf("main: fuse args: %s %s\n", fuse_argv[0], fuse_argv[1]);
  if (fork() == 0)
    fuse_main(2, fuse_argv, &fuse_memload_operations, NULL);
  sleep(1);
  printf("main: starting polling\n");
  while (1) {
  	printf("polling...\n");
	if (access(filemountpath, F_OK) == 0) {
		break;
	}
  	sleep(1);
  }
  printf("main: file at [%s] exists now\n", filemountpath);
  printf("main: entering...");

  const char *child_argv[2] = {"memfile", NULL};
  
  execve(filemountpath, child_argv, envp);
  return 0; // never reached
}
