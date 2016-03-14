

#include<stdio.h>
#include<stdlib.h>
#include "trie.h"



int main(int argc,char *argv[])
{
 	
	FILE * fp_strpool;
	FILE * fp_checkedstr;
	FILE * fp_result;

	if (argc != 4)
	{
		printf("argv error\n");
		return 0;
	}

	fp_strpool = fopen(argv[1], "r");
	fp_checkedstr = fopen(argv[2], "r");
	fp_result = fopen(argv[3], "w");
	

	trie_update_check(fp_strpool, fp_checkedstr, fp_result);
	

	fclose(fp_strpool);
	fclose(fp_checkedstr);
	fclose(fp_result);
	
	return 0;
}

