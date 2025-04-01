#include <stdio.h>

const int N = 1000;
int a[N];
//冒泡排序
void bubble_sort(int n) {
    int temp;
    for (int i = 0; i < n - 1;i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j+1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
           }
        }
    }
}

//快速排序
void quick_sort(int i,int j) {
    if (i >= j)
    {
        return;
    }
    //base
    int x = a[(i+j)/2];
    //两个指针
    int l = i;
    int r = j;
    int temp;
    //进行当前轮次的排序
    while (l <= r ) {
        while (l < r && a[l] < x)
        {
            l++;
        }
        
        while (l < r && a[r] > x) 
        { 
            r--; 
        }

        if (l < r) 
        {
            temp = a[l];
            a[l] = a[r]; 
            a[r] = temp;
        }

    }
    a[l] = x;

    //递归排序两侧数据
    quick_sort(i , r);
    quick_sort(l , j);
}

int main()
{
    int b = 0;
    int n = 0;
    
    printf("输入待排序数据个数:\n");
    scanf_s("%d" , &n);
    //提供input
    printf("依次输入待排序数据:\n");
    for (int i = 0; i < n; i++) {
        scanf_s("%d",&a[i]);
    }
    //选择排序方法
    printf("输入排序方法：（1.冒泡排序 2.快速排序）");
    scanf_s("%d", &b);
    if (b == 1) {
        bubble_sort(n);
        for (int i = 0; i < n; i++) {
            printf("%d",a[i]);
        }
    }
    else if (b == 2) {
        quick_sort(0,n-1);
        for (int i = 0; i < n; i++) {
            printf("%d", a[i]);
        }
    }
    else {
        printf("非法输入");
    }
    return 0;
}

