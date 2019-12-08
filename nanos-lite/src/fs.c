#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);

extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
extern size_t fbsync_write(const void *buf, size_t offset, size_t len);


/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static int get_fd(char *n){
  for(int i=0;i<NR_FILES;i++){
    if(strcmp(n, file_table[i].name)==0){
      return i;
    }
  }
  return -1;
}
void init_fs() {
  // TODO: initialize the size of /dev/fb
  int fd_dev_fb = get_fd("/dev/fb");
  if(fd_dev_fb!=-1){
    file_table[fd_dev_fb].size = screen_width()*screen_height()*4;
  }
  /*
  int fd_proc_dispinfo = get_fd("/proc/dispinfo");
  if(fd_proc_dispinfo!=-1){
    file_table[fd_proc_dispinfo].size = 128;
  }
  int fd_dev_events = get_fd("/dev/events");
  if(fd_dev_events!=-1){
    file_table[fd_dev_events].size = 1000000;
  }
  int fd_dev_fbsync = get_fd("/dev/fbsync");
  if(fd_dev_fbsync!=-1){
    file_table[fd_dev_fbsync].size = 1000000;
  }
  */
}

//pa3.3
int fs_open(const char *pathname, int flags, int mode){
  for(int i=0;i<NR_FILES;i++){
    if(strcmp(pathname, file_table[i].name)==0){
      file_table[i].open_offset = 0;
      return i;
    }
  }
  return -1;
}

int fs_close(int fd){
  assert(fd>=0&&fd<NR_FILES);
  file_table[fd].open_offset = 0;
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len){
  assert(fd>=0&&fd<NR_FILES);
  /*read from file to buf*/
  /*
  if(len>file_table[fd].size){
    len = file_table[fd].size;
  }
  if(file_table[fd].open_offset+len>file_table[fd].size){
    len = file_table[fd].size - file_table[fd].open_offset;
  }*/
  ReadFn read_f = file_table[fd].read;
  if(strcmp(file_table[fd].name, "/dev/events")==0){
    read_f = events_read;
  }
  else if(strcmp(file_table[fd].name, "/proc/dispinfo")==0){
    read_f = dispinfo_read;
  }
  else if(read_f==NULL){
    read_f = ramdisk_read;
  }

  size_t ret;
  if(read_f==ramdisk_read){
    ret =  read_f(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  }else{
    ret = read_f(buf, file_table[fd].open_offset, len);
  }
  file_table[fd].open_offset += len;
  //assert(file_table[fd].open_offset<=file_table[fd].size);
  return ret;
}

size_t fs_write(int fd, void *buf, size_t len){
  assert(fd>=0&&fd<NR_FILES);
  WriteFn write_f = file_table[fd].write;
  if(strcmp(file_table[fd].name, "/dev/fb")==0){
    write_f = fb_write;
  }
  else if(strcmp(file_table[fd].name, "/dev/fbsync")==0){
    write_f = fbsync_write;
  }
  else if(write_f==NULL){
    write_f = ramdisk_write;
  }

  size_t ret;
  if(write_f==ramdisk_write){
    ret =  write_f(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  }else{
    ret = write_f(buf, file_table[fd].open_offset, len);
  }

  file_table[fd].open_offset += len;
  //if(fd>2){
   // assert(file_table[fd].open_offset<=file_table[fd].size);
  //}
  return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  assert(fd>=0&&fd<NR_FILES);

  switch(whence){
    case SEEK_SET:  file_table[fd].open_offset = offset; 
                                  break;
    case SEEK_CUR:  file_table[fd].open_offset  += offset;
                                  break;
    case SEEK_END:  file_table[fd].open_offset = file_table[fd].size + offset;
                                  break;
    default:                assert(0);
      
  }
  return file_table[fd].open_offset;
}