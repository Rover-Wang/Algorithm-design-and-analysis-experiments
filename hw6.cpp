#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#define MAX 100

// 带权图结构体
typedef struct {
    int edges[MAX][MAX];//边的顶点及权值
    int n; // 顶点数
} MST;

MST topography; // 保存邻接矩阵
int path[MAX] = {}; // 保存到该顶点的最短路径对应的前驱
int min_cost[MAX] = {}; // 保存到每个顶点的最短路径长

// 创建带权图
MST creatgraphy(int num,int s)
{
    MST graph;
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
    
    printf("请填图（格式：起点 终点 权值，例如：1 2 9）：\n");
    for (int i = 1; i <= s; i++)//s条边
    {
        scanf_s("%d %d %d", &point1, &point2, &value);
        if (point1 >= 1 && point1 <= num && point2 >= 1 && point2 <= num)
        {
            graph.edges[point1][point2] = value;// 填图
            graph.edges[point2][point1] = value; // 无向图需要双向赋值
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


// Prim算法求最小生成树
void Prim(MST graph, int n)
{
    printf("\nPrim算法最小生成树的边:\n");
    // 初始化集合
    int visited[MAX] = { 0 };
    visited[1] = 1;  // 从顶点1开始

    // 开始构建最小生成树
    for (int i = 0; i < n - 1; i++) //由于最小生成树的边数为 n - 1，所以进行 n - 1 次循环
    {
        int cost = INT_MAX;
        int min_u = -1, min_v = -1;

        // 遍历已访问的顶点
        for (int u = 1; u <= n; u++)
        {
            if (visited[u]) //如果第u个结点已经访问
            {
                // 遍历未访问的顶点
                for (int v = 1; v <= n; v++)
                {
                    if (!visited[v] && graph.edges[u][v] < cost && graph.edges[u][v] != 0) //v结点未访问且权值小于cost且不等于0，更新cost
                    {
                        cost = graph.edges[u][v];
                        min_u = u;
                        min_v = v;
                    }
                }
            }
        }

        // 检查是否找到最小边
        if (min_u == -1 || min_v == -1) {
            printf("无法找到更多边，图可能不连通。\n");
            break;
        }

        // 将找到的最小边的顶点加入已访问集合
        visited[min_v] = 1;

        // 输出最小边
        
        printf("Edge: (%d, %d) Cost: %d\n", min_u, min_v, cost);
    }
}

// 边结构体
typedef struct {
    int src, dest, weight;
} Edge;

// 为Kruskal算法准备的图结构体
typedef struct {
    int V, E;  // 顶点数和边数
    Edge* edges;  // 边的数组
} Graph;

// 从邻接矩阵创建用于Kruskal算法的边列表
Graph* createEdgeList(MST mst, int n, int s) 
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = n;
    graph->E = s;
    graph->edges = (Edge*)malloc(s * sizeof(Edge));

    int edgeCount = 0;
    // 只遍历上三角矩阵，避免重复添加边
    for (int i = 1; i <= n; i++) 
    {
        for (int j = i + 1; j <= n; j++) 
        {
            if (mst.edges[i][j] != INT_MAX) 
            {
                //边结构体数组
                graph->edges[edgeCount].src = i;
                graph->edges[edgeCount].dest = j;
                graph->edges[edgeCount].weight = mst.edges[i][j];
                edgeCount++;
            }
        }
    }

    return graph;
}

// 并查集数据结构的辅助函数：查找元素所在集合（带路径压缩）
int find(int parent[], int i) 
{
    if (parent[i] != i)//元素的父节点不是本身
        parent[i] = find(parent, parent[i]);  // 递归查找集合根节点
    return parent[i];
}

// 并查集数据结构的辅助函数：合并两个集合
void unionSets(int parent[], int x, int y) 
{
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    parent[xroot] = yroot;//x所在集合的根节点作为y所在集合的根节点的子节点并入y所在集合
}

// 比较函数，用于qsort排序边
int compareEdges(const void* a, const void* b) 
{
    Edge* edge1 = (Edge*)a;
    Edge* edge2 = (Edge*)b;
    return edge1->weight - edge2->weight;
}


// Kruskal算法实现
void Kruskal(MST mst, int n, int s) 
{
    // 从邻接矩阵创建边列表
    Graph* graph = createEdgeList(mst, n, s);

    // 用于存储最小生成树的边
    Edge result[MAX];
    int e = 0;  // 结果数组的索引

    //按权值升序排序边
    qsort(graph->edges, graph->E, sizeof(Edge), compareEdges);

    // 为每个顶点分配内存用于并查集
    int* parent = (int*)malloc((n + 1) * sizeof(int));  // 顶点编号从1开始
    for (int v = 1; v <= n; ++v)
        parent[v] = v;  // 初始化每个顶点的父节点为自身

    // 步骤2: 从小到大选择边
    for (int i = 0; i < graph->E && e < n - 1; i++) 
    {
        // 选择当前最小边
        Edge next_edge = graph->edges[i];

        int x = find(parent, next_edge.src);
        int y = find(parent, next_edge.dest);

        // 如果选择这条边不会形成环，则包含它
        if (x != y) //即两者根节点不同
        {
            result[e++] = next_edge;
            unionSets(parent, x, y);
        }
        // 否则丢弃这条边
    }

    // 输出结果
    printf("\nKruskal算法最小生成树的边:\n");
    for (int i = 0; i < e; ++i)
        printf("Edge: (%d, %d) 权值: %d\n", result[i].src, result[i].dest, result[i].weight);

    free(parent);
    free(graph->edges);
    free(graph);
}

int main() 
{
    int n = 0;  // 顶点数
    int s = 0;  // 边数

    printf("请输入结点数：\n");
    scanf_s("%d", &n);
    printf("请输入边数：\n");
    scanf_s("%d", &s);

    MST graph = creatgraphy(n, s);

    // 执行Prim算法
    Prim(graph, n);

    // 执行Kruskal算法
    Kruskal(graph, n, s);

    return 0;
}