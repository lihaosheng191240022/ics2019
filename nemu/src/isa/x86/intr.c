#include "rtl/rtl.h"
void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
#ifdef MYDEBUG
	_my_debug_ printf("idtr.Limit=%d\n", cpu.idtr.Limit);
#endif
	s0 = cpu.idtr.Base + 8 * NO;
#ifdef MYDEBUG
	_my_debug_ printf("irq entry is %08x\n", s0);
#endif
	s1 = 0x0000ffff & vaddr_read(s0, 4);
	s0 += 4;
	s1 |= 0xffff0000 & vaddr_read(s0, 4);
	if((vaddr_read(s0, 4) & 0x8000) == 0){
		printf("Invalid Gate Descriptor\n");
		return;
	}

#ifdef MYDEBUG
	_my_debug_ printf("\033[36m exception entry = %08x\n\033[0m", s1);
	_my_debug_ printf("before jmp to __am_vectrap: cpu.pc=%08x\n", cpu.pc);
#endif
	rtl_j(s1);
#ifdef MYDEBUG
	_my_debug_ printf("after jmp to __am_vectrap: cpu.pc=%08x\n", cpu.pc);
#endif
	printf("after jmp: seq_pc=%08x\n", ret_addr);
}

bool isa_query_intr(void) {
  return false;
}
