#include "rtl/rtl.h"
void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	//printf("\033[32m todo in raise_intr\n");
	s0 = cpu.idtr.Base + 8 * NO;
	s1 = 0x0000ffff & vaddr_read(s0, 4);
	s0 += 4;
	s1 |= 0xffff0000 & vaddr_read(s0, 4);
	printf("\033[36m exception entry = %08x\n\033[0m", s1);
	rtl_j(s1);
	rtl_j(ret_addr);
}

bool isa_query_intr(void) {
  return false;
}
