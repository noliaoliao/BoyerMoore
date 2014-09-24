#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#define MAX_CHAR_LEN 256
const unsigned long  MAX_BUF_LEN = 1<<30;
#define MAX(x, y) (x) > (y) ? (x) : (y)
char g_szText[MAX_BUF_LEN] = {0};

int debugPrint(int *info,int len)
{
    int i = 0;
    for(i = 0; i < len; i++)
    {
        printf("%d  ",info[i]);
    }
	printf("\n");
    return 0;
}

int calcBC(char *subdir, int bc[])
{
    int i = 0;
    int len = strlen(subdir);
    
    for(i = 0; i < MAX_CHAR_LEN; i++)
    {
        bc[i] = len;
    }
    for(i = 0; i < len - 1; i++)
    {
        bc[subdir[i]] = len - 1 - i;
    }

    return 0;
}

int calcSuffix(char *subdir, int suffix[])
{
    int i = 0;
    int j = 0;
    int k = 0;
    int count = 0;
    int len = strlen(subdir);

    suffix[len-1] = len;
    for(i = len-2; i >= 0; i--)
    {
        j = i;
        k = len - 1;
        count = 0;
        while(subdir[j--] == subdir[k--])
        {
            count++;
        }
        suffix[i] = count;
    }
  
    return 0;
}

int calcGS(char *subdir, int gs[])
{
    int i = 0;
    int j = 0;
    int len = strlen(subdir);

    int *suffix = (int *)malloc(sizeof(int)*len);
    if(!suffix)
    {
        printf("malloc error!\n");
        return 0;
    }

    calcSuffix(subdir,suffix);

	//debugPrint(suffix,len);
    for(i = 0; i < len; i++)
    {
        gs[i] = len;
    }

    j = 0;
    for(i = len-2; i >= 0; --i)
    {
        if(suffix[i] == i+1)
        {
            for(;j < len-1; j++)
            {
                if(gs[j] == len)
                {
                    gs[j] = len - 1 -i;
                }
            }
        }
    }

    for(i = 0; i <= len -2; i++)
    {
        gs[len-1-suffix[i]] = len - 1 - i;
    }

    if(suffix)
    {
        free(suffix);
        suffix = NULL;
    }

    return 0;
}

int BoyerMoore(char *szText, int nTextLen, char *szSubstr, int nStrLen)
{
	//int textlen = strlen(text);
    //int sublen = strlen(subdir);
    int i = 0;
	int j = 0;

	int *bc = (int *)malloc(sizeof(int)*MAX_CHAR_LEN);
    int *gs = (int *)malloc(sizeof(int)*nStrLen);

	calcBC(szSubstr,bc);
    calcGS(szSubstr,gs);
    
	//printf("%s\n",szText);
	//printf("%s\n",szSubstr);

	debugPrint(gs,nStrLen);
    //searching
	while(j <= nTextLen-nStrLen)
	{
		for(i = nStrLen - 1; i >= 0 && szSubstr[i] == szText[j+i]; i--) ;
		
		//find
		if( i < 0)
		{
			printf("find the substr,pos:%d\n",j);
			//j += gs[0];
			//break;
			return 0;
		}
		else
		{
			j += MAX(bc[szText[j+i]] - nStrLen + 1 + i,gs[i]);
		}
	}

	printf("not foud str:%s\n",szSubstr);
	return 0;
}

int main(void)
{

	//生成随机字符串文本
	/*
	unsigned long i = 0;
	srand(time(0));
	for(i = 0; i < MAX_BUF_LEN;i++)
	{
		g_szText[i] = (rand() % ('Z' - 'A'))+'A';
	}
	FILE *file = fopen("d:\\1.txt","w+");
	fwrite(g_szText,MAX_BUF_LEN,1,file);
	*/
	//读取随机字符串文本
	
	FILE *file = fopen("d:\\1.txt","r");
	fread(g_szText,MAX_BUF_LEN,1,file);
	fclose(file);
	char *szStr = "VKOHEVHDBLYXWEMKOCKJFLY";
	//printf("%s\n",g_szText);

	clock_t start = 0;
	int end = 0;
	//begin time
	start = clock();
	//search
	BoyerMoore(g_szText,MAX_BUF_LEN,szStr,strlen(szStr));
	//end time
	end = clock();
	printf("BM time: %f",(double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	printf("%s\n",strstr(g_szText,szStr));
	end = clock();
	printf("strstr time: %f",(double)(end - start) / CLOCKS_PER_SEC);
    
	return 0;
}
