#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //pa3.3
	for(size_t i=0;i<len;i++){
		_putc(((char *)buf)[i]);
	}
	return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  //from offset to buf
	int current_key = read_key();
	if(current_key!=_KEY_NONE){
		assert(0);
		char str[64] = {'\0'};
		strcat(str, keyname[current_key]);
		strcat(str, "\n");
		size_t l = strlen(str);
		l = (l<len)?(l):(len);
		memcpy(buf, str, l);
		return l;
	}

	
	uint32_t current_time = uptime();
	//printf("current_time is %d\n", current_time);
	char t[16] = {'\0'};
	size_t i = 0;
	while(current_time!=0){
		t[i] = '0'+current_time%10;
		current_time /= 10;
		i++;
	}
	i--;
	for(size_t j = 0;j<i;){
		char tmp = t[j]; t[j] = t[i]; t[i] = tmp;
		j++; i--;
	}
	char str2[64] = {'\0'};
	strcat(str2, "t ");
	strcat(str2, t);
	strcat(str2, "\n");
	//printf("time event is %s", str2);
	size_t l2 = strlen(str2);
	l2 = (l2<len)?(l2):(len);
	memcpy(buf, str2, l2);
	return l2;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
