#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
/***pa1.2***/
#include<stdlib.h>

/***pa1.3***/
uint32_t isa_reg_str2val(const char *s, bool *success);

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_HEX, TK_REG, TK_UNEQ, TK_AND   /***pa1.2***/

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},         // equal
	
	/***pa1.2***/
  {"-", '-'},            
  {"\\*", '*'},
  {"/", '/'},
	{"0x[0-9a-fA-F]*", TK_HEX},
  {"[1-9][0-9]*|0", TK_NUM},
  {"\\(", '('},
  {"\\)", ')'},
	{"\\$e(ax|cx|dx|bx|si|di|sp|bp)", TK_REG},
	{"!=", TK_UNEQ},
	{"&&", TK_AND}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

/*memo: tokens[32]*/
static Token tokens[500] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {/***pa1.2***/
					case TK_NOTYPE:
													break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
													tokens[nr_token].type = rules[i].token_type;
													nr_token++;
													break;
				  case TK_NUM: 
					case TK_HEX:
					case TK_REG:
					case TK_UNEQ:
					case TK_AND:
													tokens[nr_token].type = rules[i].token_type;
													memset(tokens[nr_token].str, '\0', 32);
													strncpy(tokens[nr_token].str,substr_start,substr_len);
													Assert(strlen(tokens[nr_token].str)==substr_len,"strncpy wrongly used\n");
													nr_token++;
													break;
        }

        break;/*what does this mean? Oh, I got it, there is a for loop and a while loop*/
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

/***functions to calculate the "token expr"***/
static uint32_t eval(int p, int q);
static bool check_parentheses(int p, int q);
static uint32_t get_mainopt(int p, int q);

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
	*success = true;
	/***calculate the token expr***/
	printf("make token successfully\n");
	return eval(0, nr_token-1);
}

static uint32_t eval(int p, int q){
	if(p > q){
		printf("Bad expression\n");
		return 0;
	}else if(p == q){
		//printf("this is exact a number or a register\n");
		if(tokens[p].type == TK_REG){
			bool success = false;
			uint32_t regval = isa_reg_str2val(tokens[p].str, &success);
			if(!success){
				printf("invalid reg: input like $eax\n");
				return 0;
			}else{
				return regval;
			}
		}else if(tokens[p].type == TK_HEX){/*a hexadecimal number*/
			uint32_t hexval = 0;
			sscanf(tokens[p].str, "%x", &hexval);
			return hexval;

		}else{/*a decimal number*/
			return strtol(tokens[p].str, NULL, 10);/*this can be better*/
		}
	}else if(check_parentheses(p, q) == true){
		return eval(p+1, q-1);
	}else{
		//printf("proceeding\n");
		int op=get_mainopt(p, q);
		uint32_t val1=eval(p, op-1);
		uint32_t val2=eval(op+1, q);
		switch(tokens[op].type){
			case '+':
								return val1+val2;
			case '-':
								return val1-val2;
			case '*':
								return val1*val2;
			case '/':
								return val1/val2;
			case TK_EQ:
								return val1==val2;
			case TK_UNEQ:
								return val1!=val2;
			case TK_AND:
								return val1&&val2;
			default:
								Assert(0,"invalid mainopt in expr\n");
		}
	}
}
static bool check_parentheses(int p, int q){
	/*Use the stack to memorize the ( and ), but still limited*/
	if(tokens[p].type!='('||tokens[q].type!=')'){
		return false;
	}
	int ptr = p+1, matchnum = 0;
	for(; ptr < q; ptr++){
		if(tokens[ptr].type == '('){
			matchnum++;
		}else if(tokens[ptr].type == ')'){
			matchnum--;
		}

		if(matchnum<0){
			return false;
		}
	}
	if(matchnum != 0){
		return false;
	}
	return true;
}
/*this macro should be changed when pa1.3*/
//#define prior(x) ((x=='*'||x=='/')?(2):(1))/*anytime you should add ()!!!*/
static uint32_t is_opt(uint32_t x);
static uint32_t opt_prior(uint32_t opt);

static uint32_t get_mainopt(int p, int q){
	int in_parentheses = 0;
	int curpos = p;
	int curprior = 0;/*highest prior*/
	int pos = p;
	for(; pos <= q; pos++){/*pos < q???*/
		if(tokens[pos].type=='('){
			in_parentheses++;
			continue;
		}
		if(tokens[pos].type==')'){
			in_parentheses--;
			continue;
		}
		if(!is_opt(tokens[pos].type)){
			continue;
		}

		if(in_parentheses){
			continue;
		}else{
			if(opt_prior(tokens[pos].type)>=curprior){/*the least&&last prior is the mainopt*/
				curprior = opt_prior(tokens[pos].type);
				curpos = pos;
				//printf("%d %d\n",curprior,curpos);
			}else{
				continue;
			}
		}
	}
	return curpos;
}
static uint32_t is_opt(uint32_t x){
	switch(x){
		case '+':
		case '-':
		case '*':
		case '/':
		case TK_EQ:
		case TK_UNEQ:
		case TK_AND:
							return 1;
		default:
							return 0;
	}
}
static uint32_t opt_prior(uint32_t opt){
	switch(opt){
		case '*':
		case '/':
							return 3;
		case '+':
		case '-':
							return 4;
		case TK_EQ:
		case TK_UNEQ:
							return 7;
		case TK_AND:
							return 11;
		default:
							return -1;
	}
}
