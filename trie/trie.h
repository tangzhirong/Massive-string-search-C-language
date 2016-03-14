#include<stdio.h>
#define VECTORSIZE 40
#define bool unsigned char

struct node 
{
	char * s;
	bool isword;
	struct node * next[VECTORSIZE];

};

typedef struct node trie;
typedef struct node * trie_ptr;


void trie_update_check(FILE * fp_strpool, FILE *  fp_checkedstr, FILE *  fp_result);
