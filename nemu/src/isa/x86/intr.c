#include "rtl/rtl.h"
void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	//printf("\033[32m todo in raise_intr\n");
#ifdef MYDEBUG
	printf("idtr.Limit=%d\n", cpu.idtr.Limit);
#endif
	s0 = cpu.idtr.Base + 8 * NO;
#ifdef MYDEBUG
	printf("irq entry is %08x\n", s0);
#endif
	s1 = 0x0000ffff & vaddr_read(s0, 4);
	s0 += 4;
	s1 |= 0xffff0000 & vaddr_read(s0, 4);
#ifdef MYDEBUG
	printf("\033[36m exception entry = %08x\n\033[0m", s1);
	printf("before jmp to __am_vectrap: cpu.pc=%08x\n", cpu.pc);
#endif
	rtl_j(s1);
#ifdef MYDEBUG
	printf("after jmp to __am_vectrap: cpu.pc=%08x\n", cpu.pc);
#endif
	//printf("\033[38m hello\n\033[0m");
}

bool isa_query_intr(void) {
  return false;
}
