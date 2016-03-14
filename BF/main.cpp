
#include<stdio.h>
#include<stdlib.h>
#include "bloom.h"



int main(int argc,char *argv[])
{
 	
	FILE * fp_strpool;
	FILE * fp_checkedstr;
	FILE * fp_result;

	/*fp_strpool = fopen(argv[1], "r");
	fp_checkedstr = fopen(argv[2], "r");
	fp_result = fopen(argv[3], "w");
	*/
	
	fp_strpool = fopen("emaillist.dat", "r");
	fp_checkedstr = fopen("checklist.dat", "r");
	fp_result = fopen("result.txt", "w");

	bf_check(fp_strpool, fp_checkedstr, fp_result);
	
	fclose(fp_strpool);
	fclose(fp_checkedstr);
	fclose(fp_result);

	return 0;
}
