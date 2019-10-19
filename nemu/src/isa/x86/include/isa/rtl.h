#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include "rtl/rtl.h"

/* RTL pseudo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(dest, &reg_l(r)); return;
    case 1: rtl_host_lm(dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline void rtl_sr(int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(&reg_l(r), src1); return;
    case 1: rtl_host_sm(&reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(&reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline void rtl_push(const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  /*pa2.1*/
	cpu.esp -= 4;
	t0 = *src1;
	for(int i=cpu.esp;i<cpu.esp+4;i++){
		pmem[i] = t0 & 0xff;
		t0 >>= 8;
	}	
}

static inline void rtl_pop(rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
	uint8_t *tmp_p = &pmem[cpu.esp];
  rtlreg_t tmp = *((rtlreg_t *)tmp_p);
	*dest = tmp;
	cpu.esp += 4;
}

static inline void rtl_is_sub_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
	/*pa2.2*/
	t0 = *src1;
	t1 = *src2;
	//switch(width){
	//	case 1:	t0 &= 0xff; t1 &= 0xff; break;
	//	case 2: t0 &= 0xffff; t1 &=0xffff; break;
	//	case 4:	break;
	//	default:	Assert(0, "in isa/rtl.h rtl_is_sub_overflow is wrong\n");
	//}
	switch(width){
		case 1:	if(((int8_t)t0<(int8_t)t1 && (int8_t)(*res)>0)
								||((int8_t)t0>(int8_t)t1 && (int8_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		case 2:	if(((int16_t)t0<(int16_t)t1 && (int16_t)(*res)>0)
								||((int16_t)t0>(int16_t)t1 && (int16_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		case 4:	if(((int32_t)t0<(int32_t)t1 && (int32_t)(*res)>0)
								||((int32_t)t0>(int32_t)t1 && (int32_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		default:	Assert(0, "in isa/rtl.h rtl_is_sub_overflow is wrong\n");
	}

	
}

static inline void rtl_is_sub_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 - src2)
  /*pa2.2*/
	t0 = *src1;
	t1 = *res;
	if(t1 > t0)
	{	*dest = true;	}
	else
	{	*dest = false; }
}

static inline void rtl_is_add_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
  /*pa2.2*/
	t0 = *src1;
	t1 = *src2;
	//switch(width){
	//	case 1:	t0 &= 0xff; t1 &= 0xff; break;
	//	case 2: t0 &= 0xffff; t1 &=0xffff; break;
	//	case 4:	break;
	//	default:	Assert(0, "in isa/rtl.h rtl_is_add_overflow is wrong\n");
	//}
	switch(width){
		case 1:	if(((int8_t)t0<0 && (int8_t)t1<0 && (int8_t)(*res)>0)
								||((int8_t)t0>0 && (int8_t)t1>0 && (int8_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		case 2:	if(((int16_t)t0<0 && (int16_t)t1<0 && (int16_t)(*res)>0)
								||((int16_t)t0>0 && (int16_t)t1>0 && (int16_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		case 4:	if(((int32_t)t0<0 && (int32_t)t1<0 && (int32_t)(*res)>0)
								||((int32_t)t0>0 && (int32_t)t1>0 && (int32_t)(*res)<0))
						{	*dest = true;	}/*true==1*/
						else
						{	*dest = false; }/*false==0*/
						break;
		default:	Assert(0, "in isa/rtl.h rtl_is_add_overflow is wrong\n");
	}

}

static inline void rtl_is_add_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 + src2)
  t0 = *src1;
	t1 = *res;
	if(t1 < t0)
	{	*dest = true;	}
	else
	{	*dest = false; }
}

#define eflags_CF 0
#define eflags_ZF 6 
#define eflags_SF 7
#define eflags_IF 9
#define eflags_OF 11

#define make_rtl_setget_eflags(f) \
  static inline void concat(rtl_set_, f) (const rtlreg_t* src) { \
   /* *src==0 or 1*/Assert((*src==0)||(*src==1),"set_eflags is not correct\n");\
	  int index = concat(eflags_, f);\
		rtlreg_t mask = 1<<index;\
		cpu.EFLAGS = (*src==1)?(cpu.EFLAGS | mask):(cpu.EFLAGS & (~mask));\
  } \
  static inline void concat(rtl_get_, f) (rtlreg_t* dest) { \
   /**/\
		int index = concat(eflags_, f);\
		rtlreg_t mask = 1<<index;\
		*dest = ((cpu.EFLAGS & mask)==0)?(0):(1);\
  }

make_rtl_setget_eflags(CF)
make_rtl_setget_eflags(OF)
make_rtl_setget_eflags(ZF)
make_rtl_setget_eflags(SF)

static inline void rtl_update_ZF(const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
	/*pa2.1*/
	t0 = -1;
	//rtlreg_t setZF = 0x00000040 | cpu.EFLAGS; after update set_xF, these are no need
	//rtlreg_t clrZF = 0xffffffbf & cpu.EFLAGS;
	t0 >>= ((4-width)*8);
	if((t0 & *result)==0){
		t1 = 1;
		rtl_set_ZF(&t1);
	}else{
		t1 = 0;
		rtl_set_ZF(&t1);
	}
}

static inline void rtl_update_SF(const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  t0 = *result;
	t0 >>= (8*width-1);
	//rtlreg_t setSF = 0x00000080 | cpu.EFLAGS;
	//rtlreg_t clrSF = 0xffffff7f & cpu.EFLAGS;
	if(t0==1){
		t1 = 1;
		rtl_set_SF(&t1);
	}else{
		t1 = 0;
		rtl_set_SF(&t1);
	}
}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width) {
  rtl_update_ZF(result, width);
  rtl_update_SF(result, width);
}

#endif
