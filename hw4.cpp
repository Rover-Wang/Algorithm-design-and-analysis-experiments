/*
实验（四）：求最大子段和
最大子段和：给定由n个整数(可能为负整数)组成的序列a1，a2， a3… ， an， 寻找它的某个连续子段，使得其和最大。例如( -2,11,-4,13,-5,-2 )最大子段是{ 11,-4,13 }其和为20。
*/

//1.枚举法
/* a1开始有n个，a2开始有（n-1）个...an开始有1个*/
#define MAX_SIZE 1000
#include <stdio.h>
int maxmum(int n ,int*a)//求最大子段和函数(传入序列中整数的个数，传入数组指针)
{
    int i = 0;
    int j = 0;
    int maxsum = 0;
    int sum = 0;
    //双层循环找到字段的始末位置
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
        {
            sum = sum + a[j];
            if (maxsum < sum)//假如当前子段和大于已知最大子段和，更新
            {
                maxsum = sum;
            }
        }
        sum = 0;//完成a[i]开始后，sum清零
    }
    return maxsum;
}

int main()
{
    int n = 0;
    int a[MAX_SIZE] = {};
    printf("请输入字段中数字个数：");
    scanf_s("%d",&n);
    printf("请输入字段：（如：6 -2 11 -4 13 -5 -2）\n");
    for (int i = 0; i < n; i++)
    {
        scanf_s("%d",&a[i]);
    }

    int b = maxmum(n, a);
    printf("\n");
    printf("最大子段和为：%d", b);

}

//2.分治法
/*  分治：将待求A划分为a[1...n/2]与a[n/2+1...n]
    case1：最大字段完全落在a[1...n/2]；
    case2：最大字段完全落在a[n/2+1...n]；
    case3：最大字段跨在a[1...n/2]与a[n/2+1...n]之间；
    
    case1与case2可以通过递归到case3或递归出现最大子段和就是某一项的情况来求解；
    case3:由于[n/2]与a[n/2+1]必定在最大子段和中，可分别在两段中计算最大子段和s1与s2,s1+s2即为所求
*/
#define MAX_SIZE 1000
#include <stdio.h>
int maxmum(int left ,int right, int* a)//求最大子段和函数(传入字段的始末位置，传入数组指针)
{
    int sum = 0;
    if (left == right)
    {
        sum = a[right] > 0 ? a[left] : 0;
    }
    else
    {
        int mid = (left + right) / 2;
        //将A划分为a[1...n/2]与a[n/2+1...n]并递归
        int leftmaxsum = maxmum(left, mid, a);
        int rightmaxsum = maxmum(mid + 1, right, a);

        //case3
        int s1 = 0;
        int leftsum = 0;
        for (int i = mid; i >= left; i--)
        {
            leftsum += a[i];
            if (s1 < leftsum)
            {
                s1 = leftsum;
            }
        }
        int s2 = 0;
        int rightsum = 0;
        for (int i = mid+1; i <= right; i++)
        {
            rightsum += a[i];
            if (s2 < rightsum)
            {
                s2 = rightsum;
            }
        }
        sum = s1 + s2;
        //比较哪种情况子段和更大
        if (sum < leftmaxsum) sum = leftmaxsum;
        if (sum < rightmaxsum) sum = rightmaxsum;
    }
    return sum;
}
int main()
{
    int n = 0;
    int a[MAX_SIZE] = {};
    printf("请输入字段中数字个数：");
    scanf_s("%d",&n);
    
    printf("请输入字段：（如：6 -2 11 -4 13 -5 -2）\n");
    for (int i = 0; i < n; i++)
    {
        scanf_s("%d",&a[i]);
    }

    int b = maxmum(0,n-1,a);
    printf("\n");
    printf("最大子段和为：%d", b);

}


//3.动态规划

/*
    子问题定义：考虑所有下标以j结尾的子段的最大子段和
    原问题则为：1<=j<=n的情况下，哪个子问题具有最大值
*/
#define MAX_SIZE 1000
#include <stdio.h>

int maxmum(int n, int* a)//求最大子段和函数(传入序列中整数的个数，传入数组指针)
{
    int maxsum = 0;//存储当前最大的b[j]
    int sum = 0;//存储当前的b[j]
    for (int j = 0; j <= n-1; j++)
    {
        sum += a[j];//如sum = sum + a[1]:若a[1]>=0,sum = a[1],maxsum = a[1];否则令sum = 0;maxmum不变
        if (sum < 0) sum = 0;//作用是：若sum < 0,后面的子段和加上前面子段和会更小，所以清零，从下一项即第j+1项开始
        if (maxsum < sum) maxsum = sum;

    }
    return maxsum;

}

int main()
{
    int n = 0;
    int a[MAX_SIZE] = {};
    printf("请输入字段中数字个数：");
    scanf_s("%d",&n);
    printf("请输入字段：（如：6 -2 11 -4 13 -5 -2）\n");
    for (int i = 0; i < n; i++)
    {
        scanf_s("%d",&a[i]);
    }

    int b = maxmum(n, a);
    printf("\n");
    printf("最大子段和为：%d", b);

}
