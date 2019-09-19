#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
	
  /*pa1.3 TODO: Add more members if necessary */
	char wp_expr[32];

} WP;

#endif
