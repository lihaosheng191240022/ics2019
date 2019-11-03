#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void itoa2(int num, char *s){
	size_t i = 0;
	char buf[16] = {'\0'};
	while(num!=0){
		buf[i] = '0' + num%10;
		num /= 10;
		i++;
	}
	size_t j;
	for(j=0;j<i;j++){
		s[j] = buf[i-1-j];
	}
}

int printf(const char *fmt, ...) {
  for(size_t i=0;i<strlen(fmt);i++){
		_putc(fmt[i]);
	}
	//_putc('\n');
	/*
	char out[64] = {'\0'};
	va_list Argv;
	va_start(Argv, fmt);
	sprintf(out, fmt, Argv);
	va_end(Argv);
	for(size_t i=0;i<64;i++){
		if(out[i]=='\0')	break;
		else							_putc(out[i]);
	}
	_putc('\n');
	*/
	return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list Argv;
	va_start(Argv, fmt);
	size_t i = 0;
	size_t len = strlen(fmt);
	size_t j = 0;//point to next to-write ch
	size_t state = 0;//normal state
	while(i < len){
		if(fmt[i]=='%'){
			state = 1;
			i++;
		}
		//based on state
		if(state==0){
			out[j] = fmt[i];
			i++;
			j++;
		}else{
			state = 1;//format state
			switch(fmt[i]){
				case '0':{
									 state = 1;//stay format state
									 break;
								 }
				case '2':{
									 state = 1;//stay format state
									 break;
								 }
				case 'd':{
									int tmpd = va_arg(Argv, int);
									char num[16] = {'\0'};
									itoa2(tmpd, num);
									for(int k = 0; k < 16; k++){
										if(num[k]=='\0'){
											break;
										}else{
											out[j] = num[k];
											j++;
										}
									}
									state = 0;
									break;
								 }
				case 's': {
									char *tmps = va_arg(Argv, char *);
									size_t len = strlen(tmps);
									for(int k = 0; k < len; k++){
										out[j] = tmps[k];
										j++;
									}
									state = 0;
									break;
									}
				default:	break;
			}
			i++;//this is necessary 
		}
		out[j] = '\0';
	}


	va_end(Argv);
	return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
