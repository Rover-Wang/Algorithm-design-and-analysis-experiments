#include <stdio.h>
#define MAX 1000

int BinarySearch(int num ,int* a,int low,int high)
{
    int mid = 0;

    if (low > high) //递归边界
    {
        printf("没找到%d",num);
        return -1;
    }
    mid = (low + high) / 2;
    //若mid指向目标
    if (a[mid] == num)  
    {
        printf("找到了是%d",a[mid]);
        return a[mid];
    }
        
    //否则递归
    else if (a[mid] < num)
    {
        return BinarySearch(num, a,mid+1 ,high);
    }
    else
    {
        return BinarySearch(num, a, low ,mid-1);
    }

}

int main()
{
    int n = 0;
    int num = 0;
    printf("请输入有序数的个数：");
    scanf_s("%d", &n);
    int a[MAX] = {};//规定了数组最大长度
    printf("请从小到大依次输入有序数据：");//建立有序数组
    for (int i = 0; i < n; i++)
    {
        scanf_s("%d", &a[i]);
    }
    printf("请输入要二分查找的数：");
    scanf_s("%d",&num);//目标
    BinarySearch(num,a,1,n);
    return  0;
}
