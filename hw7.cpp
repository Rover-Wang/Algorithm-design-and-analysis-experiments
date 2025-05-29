#include <stdio.h>
#define max 100

bool x[max];//利用bool类型保存物品是否存放在背包中
int cw, cv, bestv;//分别定义当前背包空间，当前背包物品价值，当前最优价值
int w[max];
int bestx[max]; //当前最优价值方案
int v[max];

//添加限界函数Bound()
double Bound(int i, int n, int c) 
{
    double cleft = c - cw;  // 剩余容量
    double b = cv;
    // 以物品单位重量价值递减序装入物品
    while (i < n && w[i] <= cleft) 
    {
        cleft -= w[i];
        b += v[i];
        i++;
    }
    
    return b;
}

//打印结果
void output(int n)
{
    printf("最优价值：%d\n",bestv);
    printf("选择的物品：");
    //打印当前最优价值方案
    for (int i = 0; i < n; i++)
    {
        if (bestx[i]) printf("%d ",i+1);
    }
}

//0-1背包问题的回溯法实现
void Backtrack(int n,int c,int i)
{
    if(i >= n)//是否已经到达叶子结点
    {
        if (bestv < cv)//是否出现了更优解
        {
            bestv = cv;
            for (int j = 0; j < n; j++)
            {
                bestx[j] = x[j];
            }
            
        }
        
        output(n);//输出结果
    }
    else
    {
        if (cw + w[i] <= c)//可以容纳当前物品，左子树
        {
            x[i] = 1;//表示第i个物品放入背包
            //更新背包信息
            cv += v[i];
            cw += w[i];
            Backtrack(n, c,i + 1);
            //回溯
            cv -= v[i];
            cw -= w[i];
        }
        
        // 计算右子树的上界，若上界大于当前最优值才继续
        if (Bound(i + 1, n, c) > bestv) 
        {
            x[i] = 0;
            Backtrack(n, c, i + 1);
        }
    }
}


int main()
{
    int n = 0;
    int c = 0;

    printf("请输入可以放入背包的物品数目n：");
    scanf_s("%d", &n);

    printf("\n请输入背包的容量c：");
    scanf_s("%d", &c);
    //初始化物品信息
    printf("\n请输入可以放入背包的物品信息：eg:体积w 价值v\n");
    for (int i = 0; i < n; i++)
    {
        scanf_s("%d %d\n",&w[i] ,&v[i]);
    }


    // 冒泡排序（按单位价值降序）
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            if ((double)v[i] / w[i] < (double)v[j] / w[j]) 
            {
                int tempw = w[i], tempv = v[i];
                w[i] = w[j], v[i] = v[j];
                w[j] = tempw, v[j] = tempv;
            }
        }
    }

    //调用回溯法函数
    Backtrack(n,c,1);

    return 0;
}
