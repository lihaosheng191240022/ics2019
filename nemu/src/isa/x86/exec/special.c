#include "cpu/exec.h"
#include "monitor/monitor.h"

make_EHelper(nop) {
  print_asm("nop");
}

make_EHelper(inv) {
  /* invalid opcode */

  uint32_t temp[2];
  *pc = cpu.pc;
  temp[0] = instr_fetch(pc, 4);
  temp[1] = instr_fetch(pc, 4);

  uint8_t *p = (void *)temp;
  printf("invalid opcode(PC = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n",
      cpu.pc, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

  display_inv_msg(cpu.pc);

  rtl_exit(NEMU_ABORT, cpu.pc, -1);

  print_asm("invalid opcode");
}

make_EHelper(nemu_trap) {
  difftest_skip_ref();

  rtl_exit(NEMU_END, cpu.pc, cpu.eax);

  print_asm("nemu trap");
  return;
}

make_EHelper(rol){
  rtl_li(&s0, id_src->val);//tmp count
  rtl_li(&s1, id_dest->val);
  rtlreg_t mask = (id_dest->width==16)?(0x8000):(0x80000000);
  rtlreg_t high_bit;
  while(s0!=0){
    high_bit = s1 & mask;
    s1 = s1*2 + high_bit;
    s0--;
  }
  operand_write(id_dest, &s1);

  if(id_src->val==1){
    rtl_get_CF(&s0);
    if((s1&mask) != s0){
      rtl_li(&s0, 1);
      rtl_set_OF(&s0);
    }else{
      rtl_li(&s0, 0);
      rtl_set_OF(&s0);
    }
  }
  else{
    //UB
  }
}
