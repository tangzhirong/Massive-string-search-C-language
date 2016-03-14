
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"trie.h"

//翻转字符串 
char * string_reverse(char *line)
{
    int i;
    int t;
    int j;
    int k;
    char ch;
    if(!line)
        return "";
    i = strlen(line);
    t=!(i%2)?1:0;
    for(j=i-1,k=0;j>(i/2-t);j--)
    {
        ch = line[j];
        line[j]=line[k];
        line[k++]=ch;
    }
    return line;
}
 
//获取ascii码 
int getNum(char a)
{
	if (a >= '0'&&a <= '9')
		return a - '0';
	else if (a >= 'A'&&a <= 'Z')
		return a - 'A' + 10;
	else if (a >= 'a'&&a <= 'z')
		return a - 'a' + 10;
	else
	{
		if (a == '@')
			return 36;
		else if (a == '_')
			return 37;
		else if (a == '.')
			return 38;
		else
			return 39;
	}
}

trie_ptr create_new_node(char * line);

void trie_update_add(trie_ptr headptr, char line[])
{
	int i, isend, j;
	int set_next;
	int pos;
	char new_part[1024];
	char old_part[1024];
	trie_ptr curptr,addnewptr;
	curptr = headptr;
	
	for (i = 0,isend = 0; isend == 0 && line[i] != '\0'; i++)
	{
		pos = getNum(line[i]);
		
		if (curptr->next[pos] == NULL)
		{
			addnewptr = (trie_ptr)malloc(sizeof(trie));
			addnewptr->s = (char *)malloc(sizeof(char)*(strlen(line) - i+1));
			strcpy(addnewptr->s, line + i);
			addnewptr->isword = 1;
			
			for (set_next = 0; set_next < VECTORSIZE; set_next++)
				addnewptr->next[set_next] = NULL;

			curptr->next[pos] = addnewptr;
			isend = 1;
		}

		else
		{
			//line为树字符串的子串
			if (strcmp(curptr->next[pos]->s, line + i) == 0)
			{
				curptr->next[pos]->isword = 1;
				return;		
			}

			for (j = 0; curptr->next[pos]->s[j] == line[i] &&line[i]!='\0'&& curptr->next[pos]->s[j] != '\0'; j++, i++)
				;

			//结点字符串元素大于等于新串
			if (curptr->next[pos]->s[j] != '\0')
			{
				//拆分字符串为前后两部分
				strncpy(old_part, curptr->next[pos]->s, j + 1);
				strcpy(new_part, (curptr->next[pos]->s + j));
				old_part[j] = '\0';

				
				strcpy(curptr->next[pos]->s, new_part);


				curptr->next[pos]->s = (char *)realloc(curptr->next[pos]->s,sizeof(char) *( strlen(new_part)+1));
				

				//建立新结点，取原先结点的前部分为节点元素

				addnewptr = (trie_ptr)malloc(sizeof(trie));
				addnewptr->s = (char *)malloc(sizeof(char)*(strlen(old_part) + 1));
				strcpy(addnewptr->s, old_part);
				addnewptr->isword = 0;
				for (set_next = 0; set_next < VECTORSIZE; set_next++)
					addnewptr->next[set_next] = NULL;
				
				//指针链接
				addnewptr->next[getNum(curptr->next[pos]->s[0])] = curptr->next[pos];
				curptr->next[pos] = addnewptr;
				
				
				if (line[i] != '\0')
				{
					//加入新分支
					addnewptr = (trie_ptr)malloc(sizeof(trie));
					addnewptr->s = (char *)malloc(sizeof(char)*(strlen(line + i) + 1));
					strcpy(addnewptr->s, line + i);
					addnewptr->isword = 1;
					for (set_next = 0; set_next < VECTORSIZE; set_next++)
						addnewptr->next[set_next] = NULL;
					curptr->next[pos]->next[getNum(addnewptr->s[0])] = addnewptr;
				}
				else
				{
					//新串为短串
					curptr->next[pos]->isword = 1;

				}
				return;
			}
			else 
				i--;
			
			curptr = curptr->next[pos];
		}
	}
}

int trie_update_search(trie_ptr headptr, char * line)
{
	int i,j;
	int pos;
	int alen;
	int isend;
	trie_ptr curptr=headptr;


	for (i = 0; line[i] != '\0'; i++)
	{
		pos = getNum(line[i]);
		if (curptr->next[pos] == NULL)
			return 0;
		else
		{
			alen = strlen(curptr->next[pos]->s);
			if (alen > strlen(line + i))
				return 0;
			else if (strcmp(curptr->next[pos]->s, line + i) == 0)
				return 1;
			else if (strncmp(curptr->next[pos]->s, line + i, alen) != 0)
				return 0;
			else
			{
				curptr = curptr->next[pos];
				i = i + alen - 1;
			}
		}
	}
	if (line[i] == '\0'&&curptr->isword == 1)
		return 1;
	else
		return 0;
}

void trie_update_check(FILE * fp_strpool, FILE *  fp_checkedstr, FILE *  fp_result)
{
	int i;
	int count;
	int county;
	char line[1024];
	clock_t start,create, end;
    double d1,d2,d3;
	int temp;

	trie_ptr headptr;

	headptr = (trie_ptr)malloc(sizeof(trie));
	headptr->isword = 0;
	for (i = 0; i < VECTORSIZE; i++)
		headptr->next[i] = NULL;

	start = clock();
	count = 0;
	

	while (fgets(line, 1024, fp_strpool))
	{
		line[strlen(line)-1] = '\0';
		strcpy(line, string_reverse(line));
		trie_update_add(headptr,line);
	
	}

	create = clock();
	count = 0;
	county = 0;
	while (fgets(line, 1024, fp_checkedstr))
	{
		line[strlen(line)-2] = '\0';
		strcpy(line, string_reverse(line));
		count++;
		if (trie_update_search(headptr, line) == 1)
		{
			fprintf(fp_result, "Yes, %d:\t %s\n", count, line);
			county++;
		}
		else
		{
			fprintf(fp_result, "No, %d:\t %s\n", count, line);
		}
	}
	fprintf(fp_result,"Yes: %d ,  \n", county);
   
	return;
}
