//-----------------------------------------------------------------------------
// MurmurHash2, by Austin Appleby

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

#include<iostream>	
#include "bloom.h"
	
using namespace std;

void bf_hash(BF bf, string line);
int bf_search(BF bf, string line);
//hash映射函数 
void bf_hash(BF bf, string line)
{
	int i;
	for (i = 0; i < K; i++)
	{
		int pos=MurmurHash2(line.c_str(), strlen(line.c_str()), i + 1) % (M / K);   //将整个Bloom数组分为K段，每次hash得到一个段内偏移值pos 
		*(bf.Hash+pos+i*(M/K))= 1;   //将hash到的位置置1 
	}
}

//创建并初始化Bloom Filter 
BF bf_create()
{
	int i;
	BF bf;
	bf.Hash=(int *)malloc(sizeof(int)*M);
	for (i = 0; i < M; i++)
		bf.Hash[i]=0;

	return bf;
}

//检验email地址函数 
int bf_check(FILE * fp_strpool, FILE * fp_checkedstr, FILE * result)
{
	int num=0;
	int pos = -1;
	unsigned int count = 0;
	char line[1024];
	BF bf= bf_create();
	while (fgets(line, 1024, fp_strpool))
	{
		bf_hash(bf, line);
		count++;
		if(count%1000000==0)
			printf("No: %u\n", count);
	}
	printf("all hash has done!\n\n\n");
	
	while (fgets(line, 1024, fp_checkedstr))
	{
		pos++;
		if (bf_search(bf, line))
		{
			fprintf(result, "Yes %d:\t %s\n", pos, line);
			printf("yes\n");
			num++;
		}
		else
		{
			fprintf(result, "No %d:\t %s\n", pos, line);
			printf("no\n");
		}
	}
	printf("%d",num);
	return 0;
}

//查找函数 
int bf_search(BF bf, string line)//有返回1，无返回0
{
	int i,flag=1;
	int result = 1;
	for (i = 0; i < K&&flag; i++)
	{
		if (*(bf.Hash+i*(M/K)+MurmurHash2(line.c_str(), strlen(line.c_str()), i + 1) % (M / K))==0)
		{
			result = 0;
			flag=0;
		}
	}
	return result;
}




unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 
