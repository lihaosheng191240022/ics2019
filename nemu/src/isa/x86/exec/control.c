#include "cpu/exec.h"
#include "cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  if(decinfo.opcode==0xeb||decinfo.opcode==0xe9){
		rtl_j(decinfo.jmp_pc);
	}else if(decinfo.opcode==0xff){
		printf("addr=%08x\nval=%08x\n", id_dest->addr, id_dest->val);
		rtl_j(id_dest->addr);
	}else{
		Assert(0, "pc=%08x: jmp need more function\n", cpu.pc);
	}

  print_asm("jmp %x", decinfo.jmp_pc);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decinfo.opcode & 0xf;
  rtl_setcc(&s0, cc);
  rtl_li(&s1, 0);
  rtl_jrelop(RELOP_NE, &s0, &s1, decinfo.jmp_pc);

  print_asm("j%s %x", get_cc_name(cc), decinfo.jmp_pc);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  /*pa2.1 only call rel32, but relv actually*/
	rtl_push(&decinfo.seq_pc);
	if(decinfo.opcode==0xe8){
		decinfo.seq_pc += id_dest->val;
	}else if(decinfo.opcode==0xff&&decinfo.isa.ext_opcode==2){
		decinfo.seq_pc = id_dest->val;
	}else{
		Assert(0, "pc=%08x: call need more function\n", cpu.pc);
	}

  print_asm("call %x", decinfo.jmp_pc);
}

make_EHelper(ret) {
	rtl_pop(&s0);
	Assert(s0>=0x100000, "Invalid return position\n");
	decinfo.seq_pc = s0;

  print_asm("ret");
}

make_EHelper(ret_imm) {
  TODO();

  print_asm("ret %s", id_dest->str);
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
