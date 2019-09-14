/*=======*/
#include<stdio.h>
#include"nemu.h"
/*=======*/


int init_monitor(int, char *[]);
void ui_mainloop(int);
/***pa1.2 random test***/
uint32_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
	
	/*=============================================*/
	unsigned val;
	char str[500];
	unsigned num;
	bool success;	
	for(int i = 0;i < 100;i++){
		scanf("%u %[^\n]", &num, str);
		val = expr(str, &success);
		if(num != val){
			printf("%d: error\n", i);
		}
	}
	/*============================================*/
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
