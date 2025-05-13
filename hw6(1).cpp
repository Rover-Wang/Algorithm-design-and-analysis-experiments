#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTICES 1000
#define INF INT_MAX

// 边的结构体
typedef struct Edge {
    int to;//目的结点
    int weight;
    Edge* next;//指向下一条边的指针
} Edge;

// 顶点的邻接表头节点
typedef struct Vertex {
    Edge* head;
} Vertex;

// 图的结构体
typedef struct Graph {
    Vertex vertices[MAX_VERTICES + 1]; // 顶点编号从1开始
    int numVertices;//顶点数
} Graph;

// 最小堆节点
typedef struct HeapNode {
    int vertex;
    int key; // 到MST的最小权值
} HeapNode;

typedef struct MinHeap {
    HeapNode* array;  // 存储堆节点的数组
    int size;         // 当前堆的大小
    int capacity;     // 堆的最大容量
    int* pos;         // 顶点在堆数组中的位置映射表
} MinHeap;

// 创建新边
Edge* createEdge(int to, int weight) 
{
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->to = to;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

// 创建图
Graph* createGraph(int numVertices) 
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    for (int i = 1; i <= numVertices; i++) 
    {
        graph->vertices[i].head = NULL;//初始化图没有边的连接
    }
    return graph;
}

// 添加边（无向图）
void addEdge(Graph* graph, int src, int dest, int weight) 
{
    // 添加从src到dest的边
    Edge* newEdge = createEdge(dest, weight);
    newEdge->next = graph->vertices[src].head;
    graph->vertices[src].head = newEdge;

    // 添加从dest到src的边
    newEdge = createEdge(src, weight);
    newEdge->next = graph->vertices[dest].head;
    graph->vertices[dest].head = newEdge;
}

// 创建最小堆节点
HeapNode* createHeapNode(int vertex, int key) 
{
    HeapNode* node = (HeapNode*)malloc(sizeof(HeapNode));
    node->vertex = vertex;
    node->key = key;
    return node;
}

// 创建最小堆
MinHeap* createMinHeap(int capacity) 
{
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    //初始化
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->array = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    return minHeap;
}

// 交换两个堆节点
void swapHeapNode(HeapNode* a, HeapNode* b) 
{
    HeapNode t = *a;
    *a = *b;
    *b = t;
}

// 最小堆化
void minHeapify(MinHeap* minHeap, int idx) 
{
    int smallest, left, right;//表示位置
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    //比较当前节点 idx 与其左右子节点的权值，找出权值最小的节点
    if (left < minHeap->size && minHeap->array[left].key < minHeap->array[smallest].key)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right].key < minHeap->array[smallest].key)
        smallest = right;

    if (smallest != idx) 
    {
        // 交换位置
        minHeap->pos[minHeap->array[smallest].vertex] = idx;
        minHeap->pos[minHeap->array[idx].vertex] = smallest;

        // 交换节点
        swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// 判断堆是否为空
int isEmpty(MinHeap* minHeap) 
{
    return minHeap->size == 0;
}

// 提取最小值
HeapNode extractMin(MinHeap* minHeap) 
{
    if (isEmpty(minHeap)) return { -1, -1 };//堆为空的情况

    // 存储根节点
    HeapNode root = minHeap->array[0];

    // 替换根节点为最后一个节点
    HeapNode lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // 更新位置
    minHeap->pos[root.vertex] = minHeap->size - 1;//将最后一个结点的指针指向root
    minHeap->pos[lastNode.vertex] = 0;

    // 减少堆的大小并调整
    (minHeap->size)--;
    minHeapify(minHeap, 0);

    return root;
}

// 减小键值
void decreaseKey(MinHeap* minHeap, int vertex, int key) 
{
    // 获取顶点在堆中的位置
    int i = minHeap->pos[vertex];

    // 更新键值
    minHeap->array[i].key = key;

    // 向上调整堆
    while (i && minHeap->array[i].key < minHeap->array[(i - 1) / 2].key) //更新后的结点小于其父节点的值
    {
        // 交换当前节点和父节点的位置
        minHeap->pos[minHeap->array[i].vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2].vertex] = i;

        // 交换节点
        swapHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // 移动到父节点
        i = (i - 1) / 2;
    }
}

// 判断顶点是否在堆中
int isInMinHeap(MinHeap* minHeap, int vertex) 
{
    if (minHeap->pos[vertex] < minHeap->size)
        return 1;
    return 0;
}

// Prim算法堆优化版本
void Prim(Graph* graph) 
{
    int V = graph->numVertices; // 顶点数
    int parent[MAX_VERTICES + 1]; // 存储MST中每个顶点的父节点
    int key[MAX_VERTICES + 1];    // 存储每个顶点到MST的最小权值

    // 创建最小堆
    MinHeap* minHeap = createMinHeap(V);

    // 初始化
    for (int v = 1; v <= V; v++) 
    {
        parent[v] = -1;        // 初始化父节点为-1，表示未被访问
        key[v] = INF;          // 初始化到MST的距离为无穷大
        minHeap->array[v - 1] = { v, key[v] };  // 将顶点v加入堆中，权值为INF
        minHeap->pos[v] = v - 1;  // 记录顶点v在堆数组中的位置
    }

    // 确保第一个顶点被首先处理
    key[1] = 0;
    decreaseKey(minHeap, 1, key[1]);

    // 堆的大小初始化为V
    minHeap->size = V;

    // 构建MST
    while (!isEmpty(minHeap)) 
    {
        // 提取权值最小的顶点
        HeapNode minNode = extractMin(minHeap);
        int u = minNode.vertex;

        // 遍历所有邻接顶点
        Edge* pCrawl = graph->vertices[u].head;
        while (pCrawl != NULL) 
        {
            int v = pCrawl->to;

            // 如果v仍在堆中，且边(u,v)的权值小于当前v的key值
            if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v]) 
            {
                key[v] = pCrawl->weight;
                parent[v] = u;
                decreaseKey(minHeap, v, key[v]);//更新k值
            }
            pCrawl = pCrawl->next;
        }
    }

    // 输出MST的边
    printf("\nPrim算法最小生成树的边:\n");
    for (int i = 2; i <= V; i++) 
    {
        printf("Edge: (%d, %d) Cost: %d\n", parent[i], i, key[i]);
    }

    // 释放内存
    free(minHeap->pos);
    free(minHeap->array);
    free(minHeap);
}

// 释放图的内存
void freeGraph(Graph* graph) 
{
    for (int i = 1; i <= graph->numVertices; i++) 
    {
        Edge* current = graph->vertices[i].head;
        while (current != NULL) 
        {
            Edge* next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph);
}


int main() 
{
    int n ,s;
    int src, des, weight;
    printf("请输入顶点数目：\n");
    scanf_s("%d", &n);
    Graph* graph = createGraph(n);

    //添加边
    printf("请输入边数：\n");
    scanf_s("%d", &s);
    printf("请输入边值：源点，终点，权值 eg.（1 2 2）\n");
    for (int i = s; i > 0; i--)
    {
        scanf_s("%d %d %d",&src, &des,&weight);
        addEdge(graph, src, des, weight);
    }

   

    // 运行Prim算法
    Prim(graph);

    // 释放内存
    freeGraph(graph);

    return 0;
}