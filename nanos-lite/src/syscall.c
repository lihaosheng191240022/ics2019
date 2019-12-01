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
		case SYS_exit:	
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_exit\n"); 
#endif
									 	_halt(0); 
									break;
		case SYS_yield:	
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_yield\n"); 
#endif
									 	_yield(); 
									break;
		case SYS_open: 
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_open\n"); 
#endif
									c->GPRx = fs_open((char *)a[1], (int)a[2], (int)a[3]);
#ifdef MYDEBUG 
									_my_debug_ printf("open %s fd = %d\n",(char *)a[1],  c->GPRx); 
#endif
									break;
		case SYS_read: 
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_read\n"); 
#endif
#ifdef MYDEBUG 
									_my_debug_ printf("fd = %d, len = %u\n", (int)a[1], (size_t)a[3]); 
#endif
									c->GPRx = fs_read((int)a[1], (void *)a[2], (size_t)a[3]);
									break;
		case SYS_write:	
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_write\n"); 
#endif
#ifdef MYDEBUG 
									_my_debug_ printf("fd=%d, count=%d\n", a[1], a[3]); 
#endif
										//if(a[1]==1 || a[1]==2){
										//	for(int i=0;i<a[3];i++){
										//		char ch = *((char *)a[2]+i);
										//		_putc(ch);
										//	}
										//	c->GPRx = a[3];
										//}else{
											c->GPRx = fs_write((int)a[1], (void *)a[2], (size_t)a[3]);
										//}
									break;	
		case SYS_brk:	
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_brk\n"); 
#endif
									c->GPR1 = 0;																							
									break;
		case SYS_lseek: 
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_lseek\n"); 
#endif
#ifdef MYDEBUG 
									_my_debug_ printf("offset = %u, len = %u\n", (size_t)a[2], (size_t)a[3]); 
#endif
									c->GPRx = fs_lseek((int)a[1], (size_t)a[2], (size_t)a[3]);
									break;
		case SYS_close: 
#ifdef MYDEBUG 
									_my_debug_ printf("next: handle SYS_close\n"); 
#endif
									 c->GPRx = fs_close((int)a[1]);
									 break;
		default: panic("Unhandled syscall ID = %d", a[0]);
									break;
  }

  return NULL;
}
