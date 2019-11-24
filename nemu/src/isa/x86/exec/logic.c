#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  /*pa2.2: test only modify EFLAGS*/
	if(decinfo.opcode==0xf6){
		
		// Assert(0, "seq_pc=%08x\n", decinfo.seq_pc);
	}else if(decinfo.opcode==0xf7){
		id_src->val = instr_fetch(&decinfo.seq_pc, id_dest->width);
	}
	rtl_and(&s0, &(id_dest->val), &(id_src->val));
	rtl_li(&s1, 0);
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZF(&s0, id_dest->width);
	rtl_update_SF(&s0, id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
  /*pa2.2 just copy xor below*/
	rtl_and(&s0, &(id_dest->val), &(id_src->val));
	operand_write(id_dest, &s0);
	//assert(id_dest->reg<=7);
	//if(id_dest->type==OP_TYPE_REG){
	//	rtl_sr(id_dest->reg, &s0, id_dest->width);
	//}else{
	//	Assert(0, "pc=%08x: and need more function\n", cpu.pc);
	//}
	rtl_li(&s1, 0);
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZF(&s0, id_dest->width);
	rtl_update_SF(&s0, id_dest->width);

	print_asm_template2(and);
}

make_EHelper(xor) {
	/*pa2.1*/
	rtl_xor(&s0, &(id_dest->val), &(id_src->val));
	//rtl_sr(id_dest->reg, &s0, id_dest->width);
	operand_write(id_dest, &s0);
	rtl_li(&s1, 0);
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZF(&s0, id_dest->width);
	rtl_update_SF(&s0, id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
	/*pa2.2*/
	rtl_or(&s0, &(id_dest->val), &(id_src->val));
	//rtl_sr(id_dest->reg, &s0, id_dest->width);
	
	operand_write(id_dest, &s0);
	rtl_li(&s1, 0);
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZF(&s0, id_dest->width);
	rtl_update_SF(&s0, id_dest->width);

  print_asm_template2(or);
}

make_EHelper(sar) {
  /*pa2.2*/
	Assert(decinfo.isa.ext_opcode==7, "pc=%08x: sar has wrong ext_opcode\n", cpu.pc);
	rtl_li(&s1, id_dest->val);	
	s0 = 0x80000000 & s1;
	if(s0==0){
		for(int i=0;i<id_src->val;i++){
			s1 >>= 1;/*Is here really logic shift??*/
		}
	}else{
		for(int i=0;i<id_src->val;i++){
			s1 >>= 1;
			s1 |= s0;
		}
	}
	operand_write(id_dest, &s1);
	//if(id_dest->type==OP_TYPE_REG){
	//	rtl_sr(id_dest->reg, &(id_dest->val), id_dest->width);
	//}else{
	//	Assert(0, "pc=%08x: sar(shr) need more function\n", cpu.pc);
	//}
  // unnecessary to update CF and OF in NEMU
	rtl_update_ZFSF(&s1, id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  /*pa2.2*/
	rtl_li(&s0, id_dest->val);
	for(int i=0;i<id_src->val;i++){
		s0 *= 2;
	}
	//printf("pc=%08x: val=%08x\n", cpu.pc, id_dest->val);
	//assert(id_src->val>=0);
	//s0 = s0 << (id_src->val);
	if(id_dest->type==OP_TYPE_REG){
		rtl_sr(id_dest->reg, &s0, id_dest->width);
	}else{
		Assert(0, "pc=%08x: sal(shl) need more function\n", cpu.pc);
	}

  // unnecessary to update CF and OF in NEMU
	//printf("pc=%08x: val=%08x\n", cpu.pc, s0);
	if(id_src->val!=0){
		rtl_update_ZFSF(&s0, id_dest->width);
	}else{
		rtl_li(&s1, 0);
		rtl_update_ZFSF(&s1, id_dest->width);
	}
  print_asm_template2(shl);
}

make_EHelper(shr) {
  /*pa2.2*/
	for(int i=0;i<id_src->val;i++){
		id_dest->val >>= 1;
	}
	operand_write(id_dest, &(id_dest->val));
	//if(id_dest->type==OP_TYPE_REG){
	//	rtl_sr(id_dest->reg, &(id_dest->val), id_dest->width);
	//}else{
	//	Assert(0, "pc=%08x: shr need more function\n", cpu.pc);
	//}
  // unnecessary to update CF and OF in NEMU
	rtl_update_ZFSF(&(id_dest->val), id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);
	if(id_dest->type==OP_TYPE_REG){
		//printf("pc=%08x: s0=%u\n", cpu.pc, s0);
		rtl_sr(id_dest->reg, &s0, id_dest->width);
	}else{
		Assert(0, "pc=%08x: setcc need more function\n", cpu.pc);
	}
  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  /*pa2.2*/
	rtl_not(&s1, &(id_dest->val));
	if(id_dest->type==OP_TYPE_REG){
		rtl_sr(id_dest->reg, &s1, id_dest->width);
	}else{
		Assert(0, "pc=%08x: not need more functions\n", cpu.pc);
	}

  print_asm_template1(not);
}
