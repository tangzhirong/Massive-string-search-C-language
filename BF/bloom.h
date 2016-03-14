
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring> 
#include<cstdlib>

#define K 17
#define M 479240000

typedef struct Bloom_Filter 
{
	int *Hash;
	
}BF;

BF bf_create(); 
int bf_check(FILE * fp_strpool, FILE * fp_checkedstr, FILE * result);

unsigned int MurmurHash2(const void *key, int len, unsigned int seed);
