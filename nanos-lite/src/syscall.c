#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	//pa3.2
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;

  switch (a[0]) {
		case SYS_exit:	_my_debug_ printf("next: handle SYS_exit\n"); _halt(0); break;
		case SYS_yield:	_my_debug_ printf("next: handle SYS_yield\n"); _yield(); break;
		case SYS_write:	_my_debug_ printf("next: handle SYS_write\n");
									 _halt(0);break;	
		default: panic("Unhandled syscall ID = %d", a[0]);break;
  }

  return NULL;
}
