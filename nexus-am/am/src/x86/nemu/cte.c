#include <am.h>
#include <x86.h>

//pa3.1
#include<klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

void __am_irq0();
void __am_vecsys();
void __am_vectrap();
void __am_vecnull();


_Context* __am_irq_handle(_Context *c) {
  _Context *next = c;
#ifdef MYDEBUG
	printf("_Context c: c->edi=%08x\n", c->edi);
	printf("_Context c: c->esi=%08x\n", c->esi);
	printf("_Context c: c->ebp=%08x\n", c->ebp);
	printf("_Context c: c->esp=%08x\n", c->esp);
	printf("_Context c: c->ebx=%08x\n", c->ebx);
	printf("_Context c: c->edx=%08x\n", c->edx);
	printf("_Context c: c->ecx=%08x\n", c->ecx);
	printf("_Context c: c->eax=%08x\n", c->eax);
	printf("_Context c: c->irq=%d\n", c->irq);
	printf("_Context c: c->eip=%08x\n", c->eip);
	printf("_Context c: c->cs=%08x\n", c->cs);
	printf("_Context c: c->eflags=%08x\n", c->eflags);
#endif
	printf("c->irq=%x\n", c->irq);
	if (user_handler) {
    _Event ev = {0};
    switch (c->irq) {
			case 0x81:
							ev.event = _EVENT_YIELD; break;	
			case 0x80:
							printf("syscall in cte.c\n");break;
			default: 
							ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  return next;
}

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  static GateDesc idt[NR_IRQ];//this is 256

  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecnull, DPL_KERN);
  }

  // ----------------------- interrupts ----------------------------
  idt[32]   = GATE(STS_IG32, KSEL(SEG_KCODE), __am_irq0,   DPL_KERN);
  // ---------------------- system call ----------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecsys, DPL_USER);
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vectrap, DPL_KERN);

  set_idt(idt, sizeof(idt));
	//----------------debug
	printf("idt base = %08x, idt limit = %d\n", idt, sizeof(idt));
	printf("__am_vectrap = %x\n", __am_vectrap);
	//----------------
	// register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
