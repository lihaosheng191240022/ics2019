#include "common.h"
#include "syscall.h"

//pa3.3
#include"fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	//pa3.2
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

  switch (a[0]) {
		case SYS_exit:	_my_debug_ printf("next: handle SYS_exit\n"); _halt(0); 
									break;
		case SYS_yield:	_my_debug_ printf("next: handle SYS_yield\n"); _yield(); 
									break;
		case SYS_open: _my_debug_ printf("next: handle SYS_open\n");
									c->GPRx = fs_open((char *)a[1], (int)a[2], (int)a[3]);
									_my_debug_ printf("open %s fd = %d\n",(char *)a[1],  c->GPRx);
									break;
		case SYS_read: _my_debug_ printf("next: handle SYS_read\n");
									_my_debug_ printf("fd = %d, len = %u\n", (int)a[1], (size_t)a[3]);
									c->GPRx = fs_read((int)a[1], (void *)a[2], (size_t)a[3]);
									break;
		case SYS_write:	_my_debug_ printf("next: handle SYS_write\n");
										_my_debug_ printf("fd=%d, count=%d\n", a[1], a[3]);
										/*fd*/if(a[1]==1 || a[1]==2){
											for(int i=0;i<a[3];i++){
												char ch = *((char *)a[2]+i);
												_putc(ch);
											}
											c->GPRx = a[3];
										}else{
											c->GPRx = fs_write((int)a[1], (void *)a[2], (size_t)a[3]);
										}
									break;	
		case SYS_brk:	_my_debug_ printf("next: handle SYS_brk\n");
									c->GPR1 = 0;																							
									break;
		case SYS_lseek: _my_debug_ printf("next: handle SYS_lseek\n");
									_my_debug_ printf("offset = %u, len = %u\n", (size_t)a[2], (size_t)a[3]);
									c->GPRx = fs_lseek((int)a[1], (size_t)a[2], (size_t)a[3]);
									break;
		case SYS_close: _my_debug_ printf("next: handle SYS_close\n");
									 c->GPRx = fs_close((int)a[1]);
									 break;
		default: panic("Unhandled syscall ID = %d", a[0]);
									break;
  }

  return NULL;
}
