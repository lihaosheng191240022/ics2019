#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t i = 0;
	while(s[i] != '\0'){
		i++;
		//printf("strlen...\n");
	}
	return i;
}

char *strcpy(char* dst,const char* src) {
  /*pa2.2*/
	size_t i = 0;
	while(src[i] != '\0'){
		dst[i] = src[i];
		//printf("strcpy...\n");
	}
	dst[i] = src[i];
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  /*pa2.2*/
	size_t i;
	for(i = 0; i < n && src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	for( ; i < n; i++){
		dst[i] = '\0';
	}
	return dst;
}

char* strcat(char* dst, const char* src) {
  size_t i = strlen(dst);
	size_t j = 0;
	while(src[j] != '\0'){
		dst[i] = src[j];
		i++;
		j++;
		//printf("strcat...\n");
	}
	dst[i] = src[j];
  return dst;
}

int strcmp(const char* s1, const char* s2) {
	size_t i = 0;
	while(s1[i] == s2[i] && s1[i] != '\0'){
		i++;
		//printf("strcmp...\n");
	}
	if(s1[i] == s2[i]){
		return 0;
	}else if(s1[i] < s2[i]){
		return -1;
	}else{
		return 1;
	}
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t i;
	for(i = 0; i < n; i++){
		if(s1[i] < s2[i]){
			return -1;
		}else if(s1[i] > s2[i]){
			return 1;
		}else{
			continue;
		}
	}
	//n bytes are all same
  return 0;
}

void* memset(void* v,int c,size_t n) {
  size_t i;
	uint8_t *p = (uint8_t *)v; 
	for(i = 0; i < n; i++){
		p[i] = c;//??? is this okay???
	}
  return p;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t i;
	uint8_t *p = (uint8_t *)out;
	uint8_t *q = (uint8_t *)in;
	for(i = 0; i < n; i++){
		p[i] = q[i];//not safe!!
	}
  return p;
}

int memcmp(const void* s1, const void* s2, size_t n){
  size_t i;
	uint8_t *p = (uint8_t *)s1;
	uint8_t *q = (uint8_t *)s2;
	for(i = 0; i < n; i++){
		if(p[i] < q[i]){
			return -1;
		}else if(p[i] > q[i]){
			return 1;
		}else{
			continue;
		}
	}
  return 0;
}

#endif
