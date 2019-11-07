#include "nemu.h"
#include "monitor/diff-test.h"

extern void difftest_getregs(void *r);
extern const char *regsl[];
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
 //TODO()
	printf("do diff-test\n");
	uint32_t r[10];/*8 gpr and pc and EFLAGS*/
	difftest_getregs(r);
	CPU_state *ref_ptr = ref_r;
	int flag = 0;
	int i;
	for(i=0;i<9;i++){
		//if(r[i]!=*((uint32_t *)ref_ptr+i)){
		//	printf("diff:%s\n%08x in NEMU\n%08x in QEMU\n", regsl[i], r[i], *((uint32_t *)ref_ptr+i));
		//	flag = 1;
		//}
		Assert(r[i]==(*((uint32_t *)ref_ptr+i)), "diff:%s in NEMU and QEMU\n", regsl[i]);
	}
	//compare EFLAGS
	uint32_t eflags_QEMU = *((uint32_t *)ref_ptr + i);
	Assert((r[9]&0x40) == (eflags_QEMU&0x40), "diff: ZF in NEMU and QEMU\n");
	Assert((r[9]&0x80) == (eflags_QEMU&0x80), "diff: SF in NEMU and QEMU\n");

	/*
	if((r[9]&0x00000040) != (eflags_QEMU&0x00000040)){//ZF=6
		printf("diff:ZF differ\n");
		printf("in QEMU, ZF=%d\n", ((eflags_QEMU&0x00000040)==0)?(0):(1));
		printf("in NEMU, ZF=%d\n", ((r[9]&0x00000040)==0)?(0):(1));
		flag = 1;
	}
	if((r[9]&0x00000080) != (eflags_QEMU&0x00000080)){//SF=7
		printf("diff:SF differ\n");
		printf("in QEMU, SF=%d\n", ((eflags_QEMU&0x00000080)==0)?(0):(1));
		printf("in NEMU, SF=%d\n", ((r[9]&0x00000080)==0)?(0):(1));
		flag = 1;
	}
	*/
 	return (flag==0)?(true):(false);
}

void isa_difftest_attach(void) {
}
