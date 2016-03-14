# Massive-string-search-C-language
Problem Description:
      Using bloom filter and trie algorithm, to find out whether the given 1000 email address are in the 1500w email address

问题描述：
    利用尽量低的时间和空间复杂度（低内存占用和快计算速率），判断给定的1000个email地址是否在给定的1500万个email地址中，输出YES or NO。

一、Trie树实现

    一开始，我们组先从最简单的Trie树开始编写建树和查询，但经过计算，消耗内存太大，明显不能满足要求。为了减少内存的消耗，我们采取了对Trie树进行压缩，即合并那些有公共前缀的节点。
    压缩Trie树的数据结构如下：
    struct uptrie
    {
	char * s;
	bool isword;
	struct uptrie * next[VECTORSIZE];
    };

    我们用s来记录节点元素，用isword记录该节点是否为一个完整的地址，用一个指针数组链接下面的节点。
    但如何在每次插入一行email地址时，都能循环地进行节点合并呢？我们思考了如下几个问题：
    （1）在合并过程中需要考虑哪几种不同的节点数据情况？且是否考虑周全？
    （2）针对不同的节点数据情况，如何进行节点的拆分？
    （3）合并和拆分节点时，指针如何修改？
    （4）压缩后，内存能少多少？
     针对这些问题，我们给出了4种节点数据情况以及处理方法：:
     a.如果两个串相等：
         合并两个节点为一个，将该节点的isword置为true。
     b.如果email地址是树节点元素的子串：
         拆分树节点元素为两部分，前一部分为email地址，后一部分为剩余的节点数据。将前一部分创建一个新节点，由前一个节点指向；后一部分为原树节点，由新节点指向。
     c.如果email地址是树节点元素的母串：
         同b情况的道理，进行节点的拆分与新旧节点的指针链接。
     d.如果两个串相应位置的元素不相等:
         找到开始不等的位置，email地址的元素指针前移到该位置，树节点元素指针也前移，重进循环即可。
     经过调试与运行，发现耗用内存9.5G左右，耗时31s左右，在linux环境下能够顺利运行，且匹配email地址条数的结果为：853。
     经过讨论课后，我们发现有很多更好的方法，可以让内存占用和耗时更少。如：三叉压缩Trie树、二叉压缩Patricia Trie等，我们也进行了研究。

二、Bloom Filter实现

    BF过滤器算法是实现大规模数据匹配的很经典的方法。我们用MurmurHash2得到了一组hash函数，按照基本的思路进行了编程。
    为了让错误率降低到0.001%，我们选取了关键参数的值：K=17、M=479240000。
    BF过滤器的数据结构很简单，我们认为它就是一个指向整型的指针（或数组）。结构如下：
    typedef struct Bloom_Filter 
    {
	int *Hash;
    }BF;
    由于每个hash函数需要相互独立，我们采用如下的方法进行BF数组填充，即：将整个Bloom数组分为K段，每次hash得到一个段内偏移值pos ，K次映射到K个不同的段中。
    
    核心代码如下：
    for (i = 0; i < K; i++)
    {
         int pos=MurmurHash2(line.c_str(), strlen(line.c_str()), i + 1) % (M / K);
         *(bf.Hash+pos+i*(M/K))= 1;		//将hash到的位置置1 
    }

    最后，由于只需要一个BF数组,只需要30M左右的内存，就能跑完整个程序。
