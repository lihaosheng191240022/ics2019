#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = (WP *)malloc(sizeof(WP));/*pa1.3: Llist with head node*/
	head->next = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP *new_wp(){
	Assert(free_ != NULL, "watchpoint pool shortage\n");
	WP *find_tail = head;
	while(find_tail->next != NULL){
		find_tail = find_tail->next;
	}
	find_tail->next = free_;
	free_ = free_->next;
	(find_tail->next)->next = NULL;
	return find_tail->next;
}

void free_wp(WP *wp){/*need any check whether wp was exactly in free_ Llist??*/
	/*delete from head Llist*/
	WP *before_wp = head;
	while(before_wp->next != wp){
		before_wp = before_wp->next;
	}
	before_wp->next = wp->next;
	/*add to free_ Llist*/
	wp->next = free_;
	free_ = wp;
}

