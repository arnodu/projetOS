#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>

/* 
** un element de la liste
*/	
typedef struct mylist{
	int val;
	CIRCLEQ_ENTRY(mylist) next_and_prev;
} mylist;

/*
** tete et fin de liste
*/
CIRCLEQ_HEAD(list_head_last, mylist) list_head_last;

struct list_head_last* head_last;

int main() {
	head_last = malloc(sizeof(list_head_last));
	/* initialise tete et fin*/
	CIRCLEQ_INIT(head_last);
	
	int  i;
	mylist* curr;
	for(i=0; i<100; i++){
		curr = malloc(sizeof(mylist));
		curr->val = i;
		/* insere l'element a la fin de la liste*/
		CIRCLEQ_INSERT_TAIL(head_last, curr, next_and_prev);
	}

	/* boucle sur la liste*/
	CIRCLEQ_FOREACH(curr, head_last, next_and_prev){
		printf("prev %d current %d next %d\n", CIRCLEQ_PREV(curr, next_and_prev)->val, curr->val, CIRCLEQ_NEXT(curr, next_and_prev)->val);		
	}
	
	/* enleve les nombres pairs*/
	CIRCLEQ_FOREACH(curr, head_last, next_and_prev){
		if(curr->val %2 == 0)
			CIRCLEQ_REMOVE(head_last, curr, next_and_prev);
	}

	CIRCLEQ_FOREACH(curr, head_last, next_and_prev){
		printf("prev %d current %d next %d\n", CIRCLEQ_PREV(curr, next_and_prev)->val, curr->val, CIRCLEQ_NEXT(curr, next_and_prev)->val);		
	}
	return 0;
}