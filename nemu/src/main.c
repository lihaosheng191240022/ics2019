/*=======*/
#include<stdio.h>
#include"nemu.h"
/*=======*/


int init_monitor(int, char *[]);
void ui_mainloop(int);
/***pa1.2 random test***/
uint32_t expr(char *e, bool *success);
static void rand_expr_test();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

	/*Random test expr*/
  rand_expr_test();	
	
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}

static void rand_expr_test(){
	unsigned val;
	char str[500];
	unsigned num;
	bool success;	
	for(int i = 0;i < 0;i++){
		if(scanf("%u %[^\n]", &num, str)!=EOF){
			val = expr(str, &success);
		}
		if(num != val){
			printf("%d: error\n", i+1);
		} else {
			printf("success%d: %u\n", i+1, val);
		}
	}

}
