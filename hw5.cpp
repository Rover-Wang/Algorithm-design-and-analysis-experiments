#include <stdio.h>
#include <limits.h>
#define MAX 100

// 多段图结构体
typedef struct {
    int edges[MAX][MAX];
    int n; // 顶点数
} MGraph;

MGraph topography; // 保存邻接矩阵
int path[MAX] = {}; // 保存到该顶点的最短路径对应的前驱
int min_cost[MAX] = {}; // 保存到每个顶点的最短路径长

// 创建多段图
MGraph creatgraphy(int num) 
{
    MGraph graph;
    int n = num;
    int point1, point2, value;

    // 初始化邻接矩阵为INT_MAX（表示边不存在）
    for (int i = 1; i <= n; i++) 
    {
        for (int j = 1; j <= n; j++) 
        {
            graph.edges[i][j] = INT_MAX;
        }
    }

    printf("请输入边数：\n");
    scanf_s("%d", &n);
    printf("请填图（格式：起点 终点 权值，例如：1 2 9）：\n");
    for (int i = 0; i < n; i++) 
    {
        scanf_s("%d %d %d", &point1, &point2, &value);
        if (point1 >= 1 && point1 <= num && point2 >= 1 && point2 <= num) 
        {
            graph.edges[point1][point2] = value; // 填图
        }
        else 
        {
            printf("输入的边无效，跳过。\n");
        }
    }

    printf("\n建立的邻接矩阵为:\n");
    // 打印邻接矩阵
    for (int i = 1; i <= num; i++) 
    {
        for (int j = 1; j <= num; j++) 
        {
            printf("%d ", (graph.edges[i][j] == INT_MAX) ? 0 : graph.edges[i][j]);
        }
        printf("\n");
    }
    graph.n = num;
    return graph;
}

// 动态规划求多段图最短路径
void MultiStageGragh(MGraph graph, int k, int source) 
{
    int cost[MAX];
    int path[MAX];
    int n = graph.n;

    // 初始化
    for (int i = 1; i <= n; i++) 
    {
        path[i] = -1;
        cost[i] = INT_MAX; // 避免整数溢出
    }
    cost[k] = 0; // 终点的最短路径为0

    // 自后向前计算最短路径和成本
    for (int i = n - 1; i >= 1; i--) 
    {
        for (int j = i + 1; j <= n; j++) 
        {
            if (graph.edges[i][j] != INT_MAX) 
            { // 如果存在边
                int temp = graph.edges[i][j] + cost[j];
                if (temp < cost[i]) {//小于已经求出的解，就更新cost[i]
                    cost[i] = temp;
                    path[i] = j;
                }
            }
        }
    }

    // 输出结果
    if (cost[source] == INT_MAX) 
    {
        printf("无路径到达终点。\n");
    }
    else 
    {
        printf("最小成本：%d\n", cost[source]);
        printf("最小路径：");
        int a = source;
        while (a != k) 
        {
            printf("%d -> ", a);
            a = path[a];
            if (a == -1) 
            {
                printf("无路径到达终点。\n");
                return;
            }
        }
        printf("%d\n", k);
    }
}

int main() 
{
    int num = 0;
    int source = 0;
    int k = 0;

    printf("请输入多段图结点数：");
    scanf_s("%d", &num);
    MGraph topography = creatgraphy(num);

    printf("请输入起点（1-%d）：", num);
    scanf_s("%d", &source);
    while (source < 1 || source > num) 
    {
        printf("起点无效，请重新输入：");
        scanf_s("%d", &source);
    }

    printf("请输入终点（1-%d）：", num);
    scanf_s("%d", &k);
    while (k < 1 || k > num) 
    {
        printf("终点无效，请重新输入：");
        scanf_s("%d", &k);
    }

    MultiStageGragh(topography, k, source);
    return 0;
}