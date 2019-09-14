#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
/***pa1.1***/
extern void isa_reg_display(void);


/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {   /*line_read != NULL???*/
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

/*My commands begin*/
static int cmd_info(char *args);
static int cmd_si(char *args);
static int cmd_x(char *args);
static int cmd_p(char *args);
/*My commands end*/

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  /* TODO: Add more commands */
  { "info", "Show state of register/watchpoint", cmd_info },
  { "si", "Step one instruction", cmd_si },
	{ "x", "Print the content of memory(pmem)", cmd_x},
	{ "p", "Print the value of expression", cmd_p}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_info(char *args){
		/*printf("this is cmd_info\n");*/	
  char *arg = strtok(NULL, " ");

  if(arg == NULL){
    /* no argument given: show both registers and watchpoints*/
		printf("registers:\n");
		isa_reg_display();
		printf("watchpoints:\n");
  }else{
		 	if(strcmp(arg,"r") == 0){
				isa_reg_display();
			}else if(strcmp(arg,"w") == 0){
				printf("watchpoints:\n");
		  }else{
				printf("Unknown command '%s'\n", arg);
		  }
  }
  return 0;
}
static int cmd_si(char *args){
	char *arg = strtok(NULL, " ");
	if(arg == NULL){
		cpu_exec(1);
	}	else {
		uint32_t num;
		if(sscanf(arg,"%u",&num)!=1){
			printf("invalid argument: input si [num]\n");
			return 0;
		} else {
			cpu_exec(num);
		}
	}
	
	return 0;
}
static int cmd_x(char *args){
		char *arg = strtok(NULL, " ");

		if (arg == NULL) {
    /* no argument given:show warning information*/
				printf("invalid argument: input x <num> <addr>\n");
				return 0;
		}
		else{
				uint32_t num;
				if(sscanf(arg,"%u",&num)!=1){
						printf("invalid argument: input x <num> <addr>\n");
						return 0;
				}
				arg = strtok(NULL, " ");
				uint32_t addr;
				if((arg == NULL) || sscanf(arg,"%x",&addr)!=1){
						printf("invalid argument: input x <num> <addr>\n");
						return 0;
				}
				for(int i=0;i<num;i++){
						//printf("0x%08x:  %08x\n",addr+4*i,*((unsigned *)(&pmem[addr+4*i])));
						printf("0x%08x:  ",addr+4*i);
						for(int j=0;j<4;j++){
								printf("%02x ",*(unsigned char *)(&pmem[addr+4*i+j]));
						}
						printf("\n");
				}

				//printf("Unknown command '%s'\n", arg);
		}
		return 0;
}
static int cmd_p(char *args){
  char *arg = strtok(NULL, "\0");/*can't be split by <SPACE>!!, modify it later'*/
	uint32_t value = 0;
  if(arg == NULL){
    /* no argument given*/
		printf("invalid argument: input p <expr>\n");
		return 0;
  }else{
		bool success = false;
		value = expr(arg, &success);
		if(success){
			printf("value:%u\n",value);/*I choose unsigned instead of int, any bugs??*/
			return 0;
		}else{
			printf("invalid expr\n");
			return 0;
		}
	}
}
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
