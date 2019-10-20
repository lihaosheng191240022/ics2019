#include "nemu.h"
#include "monitor/diff-test.h"

extern void difftest_getregs(void *r);

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
 //TODO()
	uint32_t r[9];/*8 gpr and pc*/
	difftest_getregs(r);
	CPU_state *ref_ptr = ref_r;
	for(int i=0;i<9;i++){
		if(r[i]!=*((uint32_t *)ref_ptr+i)){
			return false;
		}
	}
 	return true;
}

void isa_difftest_attach(void) {
}
