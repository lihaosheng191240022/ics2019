#include "common.h"
#include <amdev.h>

#include"klib.h"
static void int2str(int x, char *p){
	size_t i = 0;
	while(x!=0){
		p[i] = '0'+(x%10);
		x /= 10;
		i++;
	}
	p[i] = '\0';
	i--;
	for(size_t j = 0;j<i;){
		char tmp = p[i]; p[i] = p[j]; p[j] = tmp;
		j++; i--;
	}
}
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
	int flag = 1;
	if((current_key & 0x7fff) == current_key){
		flag = 0;
	}
	current_key &= 0x7fff;
	assert(current_key>=0);
	if(current_key!=_KEY_NONE){
		//printf("current_key = %u\n", current_key);
		char str[] = {'\0'};
		if(flag==0){
			strcat(str, "ku ");
		}else{
			strcat(str, "kd ");
		}

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

static char dispinfo[128] __attribute__((used)) = {'\0'};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //assert(offset>=0 && (offset+len)<=128);
	//printf("in dispinfo_read: dispinfo = %s\n", dispinfo);
	//if(len>128){
	//	len = 128;
	//}
	assert(0);
	printf("offset=%u, len=%u\n", offset, len);
	printf("dispinfo=%s", dispinfo);
	memcpy(buf, dispinfo, len);
	//memset(buf+128, '\0', len);
	//memcpy(buf, dispinfo, len);
	return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  assert(0);
	return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  assert(0);
  draw_sync();
	return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
	int w = screen_width();
	int h = screen_height();
	strcat(dispinfo, "WIDTH:");
	char tmp[64] = {'\0'};
	int2str(w, tmp);
	strcat(dispinfo, tmp);
	
	strcat(dispinfo, "\nHEIGHT:");
	int2str(h, tmp);
	strcat(dispinfo, tmp);
	strcat(dispinfo, "\n");
	//printf("in init_device: dispinfo = %s", dispinfo);

}
