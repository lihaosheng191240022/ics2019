#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define bool int
#define true 1
#define false 0
#define ZEROPAD 1		//Pad with zero
#define SIGN		2		//Unsigned/signed long
#define PLUS		4		//Show plus
#define SPACE		8		//Space if plus
#define LEFT		16	//Left justified
#define SPECIAL 32	//0x
#define LARGE		64	//Use 'ABCD' instead of 'abcd'
static char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static inline int is_space(int ch){
	return (unsigned long)(ch - 9) < 5u || ' '==ch;
}
static inline int is_digit(int ch){
	return ch >= '0' && ch <= '9';
}
/*
static int my_atoi(char *str){
	int sign;
	int n;
	char *p = str;
	
	while(is_space(*p))	p++;

	sign = ('-'==*p) ? -1 : 1;
	if('+'==*p || '-'==*p)	p++;

	for(n = 0; is_digit(*p); p++)
		n = 10 * n + (*p - '0');

	return sign*n;
}
*/
static int skip_atoi(const char **s){
	int i = 0;
	while(is_digit(**s)){
		i = i*10 + *((*s)++) - '0';
	}
	return i;
}

static char * number(char *str, long num, int base, int size, int precision, int type)
{
	char c, sign, tmp[66];
	char *dig = digits;
	int i;

	if(type & LARGE) dig = upper_digits;
	if(type & LEFT) type &= ~ZEROPAD;
	if(base < 2 || base > 36)	return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if(type & SIGN){
		if(num < 0){
			sign = '-';
			num = -num;
			size--;
		}
		else if(type & PLUS){
			sign = '+';
			size--;
		}
		else if(type & SPACE){
			sign = ' ';
			size--;
		}
	}

	if(type & SPECIAL){
		if(16 == base)
			size -= 2;
		else if(8==base)
			size--;
	}

	i = 0;

	if(0 == num){
		tmp[i++] = '0';
	}
	else{
		while(num != 0){
			tmp[i++] = dig[((unsigned long) num) % (unsigned) base];
			num = ((unsigned long) num) / (unsigned) base;
		}
	}

	if(i > precision)	precision = i;
	size -= precision;
	if(!(type & (ZEROPAD | LEFT))){
		while(size-- > 0)	*str++ = ' ';
	}
	if(sign) *str++ = sign;

	if(type & SPECIAL){
		if(8 == base){
			*str++ = '0';
		}
		else if(16 == base){
			*str++ = '0';
			*str++ = digits[33];
		}
	}

	if(!(type & LEFT)){
		while(size-- > 0)	*str++ = c;
	}
	while(i < precision--)	*str++ = '0';
	while(i-- > 0)	*str++ = tmp[i];
	while(size-- > 0)	*str++ = ' ';
	
	return str;
}

//useless
/*static void itoa2(int num, char *s){
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
*/
int printf(const char *fmt, ...) {
  //for(size_t i=0;i<strlen(fmt);i++){
	//	_putc(fmt[i]);
	//}
	char buf[128] = {'\0'};
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	for(size_t i=0;i<128;i++){
		if(buf[i]=='\0')	break;
		else							_putc(buf[i]);
	}
	va_end(args);
	return n;
	/*
	char buf[128] = {'\0'};
	va_list Argv;
	va_start(Argv, fmt);
	sprintf(buf, fmt, Argv);
	va_end(Argv);
	for(size_t i=0;i<64;i++){
		if(buf[i]=='\0')	break;
		else							_putc(buf[i]);
	}
	//_putc('\n'); no need
	*/
	return 0;
}

int vsprintf(char *buf, const char *fmt, va_list args) {
  
	char *str;
	int field_width;	/*width of output field*/

	for(str=buf;*fmt;fmt++){
		unsigned long num;
		int base = 10;
		int flags = 0;
		int qualifier = -1;
		int precision = -1;
		bool bFmt = true;
		if(*fmt != '%'){
			*str++ = *fmt;
			continue;
		}

		bFmt = true;
		while(bFmt){
			fmt++;/*This also skips first '%'*/
			switch(*fmt){
				case '0':	flags |= ZEROPAD;break;
				case '-':
				case '+':
				case ' ':
				case '#':
									_putc('$');break;
				default:	bFmt = false;break;
			}
		}

		/*Get field width*/
		field_width = -1;
		if(is_digit(*fmt)){
			field_width = skip_atoi(&fmt);
		}else if('*'==*fmt){
			fmt++;
			field_width = va_arg(args, int);
			if(field_width<0){
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/*Get the precision*/
		precision = -1;
		if('.' == *fmt){
			_putc('$');
		}
		/*Get the conversion qualifier*/
		qualifier = -1;
		if('h' == *fmt || 'l' == *fmt || 'L' == *fmt){
			qualifier = *fmt;
			fmt++;
		}

		/*Default base*/
		base = 10;
		switch(*fmt){
			case 'c':
				{
					if(!(flags & LEFT))	while(--field_width > 0) *str++ = ' ';
					*str++ = (unsigned char) va_arg(args, int);
					while(--field_width > 0) *str++ = ' ';
					continue;
				}
			case 's':
				{
					int len;
					char *s = va_arg(args, char *);
					if(!s) s = "<NULL>";
					len = strlen(s);
					if(!(flags & LEFT))	while(len < field_width--) *str++ = ' ';
					for(int i = 0; i < len; ++i)	*str++ = *s++;
					while(len < field_width--) *str++ = ' ';
					continue;
				}
			case 'p':
			case 'n':
			case 'A':
			case 'a':
			case 'o':
			case 'X':
				_putc('$');break;
			case 'x':
				{
					base = 16;
					break;
				}
			case 'd':
			case 'i':
				{
					flags |= SIGN;/*no break*/
				}
			case 'u':
				{
					break;
				}
			default:
				{
					if(*fmt != '%')	*str++ = '%';

					if(*fmt){
						*str++ = *fmt;
					}else{
						--fmt;
					}
					continue;
				}
		}/*end of switch(*fmt)*/

		if(qualifier == 'l'){
			num = va_arg(args, unsigned long);
		}
		else if(qualifier == 'h'){
			if(flags & SIGN)
				num = va_arg(args, int);
			else
				num = va_arg(args, unsigned);
		}
		else if(flags & SIGN){
			num = va_arg(args, int);
		}
		else{
			num = va_arg(args, unsigned long);
		}

		str = number(str, num, base, field_width, precision, flags);
	}/* end of for (str = buf; *fmt; fmt++) */
	*str = '\0';
	return str - buf;
}

int sprintf(char *buf, const char *fmt, ...) {
  //strcpy(out, fmt);
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);

	return n;
	/*
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
*/	
	//return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  assert(0);
	return 0;
}

#endif
