#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  /*pa2.1, pa2.2*/

	if(decinfo.opcode==0x6a){//push imm8 need sign extend
		rtl_sext(&(id_dest->val), &(id_dest->val), id_dest->width);
	}
	rtl_push(&(id_dest->val));

  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&s0);
	/*pa2.2*/
	//if(id_dest->type==OP_TYPE_REG){
	//	rtl_sr(id_dest->reg, &(id_dest->val), 4);
	operand_write(id_dest, &s0);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  /*pa2.2 add.c*/
	rtl_lr(&s0, 5, id_dest->width);
	rtl_sr(4, &s0, id_dest->width);
	rtl_pop(&s1);
	rtl_sr(5, &s1, id_dest->width);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    /*pa2.2*/
		if((int16_t)(reg_w(R_AX))<0){
			reg_w(R_DX) = 0xffff;
		}else{
			reg_w(R_DX) = 0;
		}
  }
  else {
    /*pa2.2*/
		if((int32_t)(reg_l(R_EAX))<0){
			reg_l(R_EDX) = 0xffffffff;
		}else{
			reg_l(R_EDX) = 0;
		}
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    /*pa2.3 sign ext*/
		//AL->AX
		rtl_li(&s0, reg_b(R_AL));
		rtl_sext(&s1, &s0, 1);
		rtl_sr(R_AX, &s1, 2);

  }
  else {
    //AX->EAX
		rtl_li(&s0, reg_w(R_AX));
		rtl_sext(&s1, &s0, 2);
		rtl_sr(R_EAX, &s1, 4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
