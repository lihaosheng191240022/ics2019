#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 5

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

/*for debugging*/
void show_me_free();
static int wp_cnt = 1;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = 0;
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
	free_->NO = wp_cnt;
	wp_cnt++;
	find_tail->next = free_;
	free_ = free_->next;
	(find_tail->next)->next = NULL;
	/*as the func name said*/
	return find_tail->next;
}

void free_wp(WP *wp){/*need any check whether wp was exactly in free_ Llist??*/
	/*delete from head Llist*/
	WP *before_wp = head;
	while(before_wp!=NULL){/*why use while(1) is segmentation fault???*/
		if(before_wp->next==wp){
			break;
		}
		if(before_wp->next==NULL){
			printf("this watchpoint has been freed or never used\n");
			return;
		}	
		before_wp = before_wp->next;
	}
	before_wp->next = wp->next;
	/*add to free_ Llist*/
	wp->next = free_;
	free_ = wp;
}
void delete_wp(int index){
	WP *search = head->next;
	while(search!=NULL){
		if(search->NO==index){
			break;
		}
		search = search->next;
	}
	if(search==NULL){
		printf("this watchpoint has been deleted or never created\n");
		return;
	}
	free_wp(&wp_pool[index]);
}

void show_me_free(){
	WP *each = free_;
	while(each != NULL){
		printf("%d ", each->NO);
		each = each->next;
	}
	printf("\n");
}
