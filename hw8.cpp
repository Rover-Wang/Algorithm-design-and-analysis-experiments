#include <stdio.h>
#include <stdlib.h>
#define max 100

int w[max];          // 物品体积
int v[max];          // 物品价值
int selected[max] = { 0 };  // 记录原始物品的选择情况

// 物品结构
typedef struct {
    int w;      // 体积
    int v;      // 价值
    double r;   // 价值率
    int index;  // 原始索引
} Item;

// 搜索树节点（包含路径记录）
typedef struct Node {
    int level;     // 当前处理的物品层级
    int cw;        // 当前总重量
    int cv;        // 当前总价值
    double bestv;  // 上界估计值
    int path[max]; // 从根到当前节点的选择路径
    struct Node* next; // 链表指针
} Node;

// 优先队列结构
typedef struct {
    Node* front;  // 队列前端
} PriorityQueue;

// 创建新节点
Node* createNode(int level, int weight, int value, double bestv, Node* parent, int include) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->level = level;
    node->cw = weight;
    node->cv = value;
    node->bestv = bestv;
    node->next = NULL;

    // 复制父节点的路径
    if (parent != NULL) 
    {
        for (int i = 0; i < max; i++) 
        {
            node->path[i] = parent->path[i];
        }
    }
    else 
    {
        // 根节点初始化为全0
        for (int i = 0; i < max; i++) 
        {
            node->path[i] = 0;
        }
    }

    // 如果当前物品被选择，记录到路径中
    if (level >= 0 && include) 
    {
        node->path[level] = 1;
    }

    return node;
}

// 初始化优先队列
PriorityQueue* initQueue() 
{
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->front = NULL;
    return queue;
}

// 判断队列是否为空
int isEmpty(PriorityQueue* queue) 
{
    return queue->front == NULL;
}

// 入队
void enqueue(PriorityQueue* queue, Node* node) 
{
    if (isEmpty(queue) || node->bestv > queue->front->bestv) 
    {
        node->next = queue->front;
        queue->front = node;
    }
    else 
    {
        Node* current = queue->front;
        while (current->next != NULL && current->next->bestv > node->bestv) 
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
}

// 计算节点上界
double calculateUpperBound(Item* items, int n, int level, int weight, int value, int capacity) 
{
    if (weight >= capacity) return 0;
    double bound = value;
    int remainingWeight = capacity - weight;
    int i = level + 1;

    // 按价值率降序装入剩余物品
    while (i < n && items[i].w <= remainingWeight) 
    {
        bound += items[i].v;
        remainingWeight -= items[i].w;
        i++;
    }

}

// 出队
Node* dequeue(PriorityQueue* queue) 
{
    if (isEmpty(queue)) return NULL;
    Node* temp = queue->front;
    queue->front = queue->front->next;
    return temp;
}

// 交换两个物品
void swap(Item* e, Item* f) 
{
    Item temp = *e;
    *e = *f;
    *f = temp;
}

// 分支限界法求解0-1背包问题
int knapsackBranch(Item* items, int n, int capacity) 
{
    PriorityQueue* queue = initQueue();
    int maxValue = 0;
    int bestPath[max] = { 0 }; // 记录最优路径

    // 初始化根节点
    Node* root = createNode(-1, 0, 0, calculateUpperBound(items, n, -1, 0, 0, capacity), NULL, 0);
    enqueue(queue, root);

    while (!isEmpty(queue)) 
    {
        Node* current = dequeue(queue);

        // 剪枝
        if (current->bestv <= maxValue) 
        {
            free(current);
            continue;
        }

        int nextLevel = current->level + 1;

        // 处理完所有物品，更新最优解
        if (nextLevel == n) 
        {
            if (current->cv > maxValue) 
            { // 只有当当前价值更大时才更新
                maxValue = current->cv;
                // 复制当前路径到最优路径
                for (int i = 0; i < n; i++) 
                {
                    bestPath[i] = current->path[i];
                }
            }
            free(current);
            continue;
        }

        // 分支1：选择当前物品
        int newWeight = current->cw + items[nextLevel].w;
        int newValue = current->cv + items[nextLevel].v;

        if (newWeight <= capacity) 
        {
            double bound = calculateUpperBound(items, n, nextLevel, newWeight, newValue, capacity);
            if (bound > maxValue) 
            { // 只有可能更好的才入队
                Node* left = createNode(nextLevel, newWeight, newValue, bound, current, 1);
                enqueue(queue, left);
            }
        }

        // 分支2：不选择当前物品
        double bound = calculateUpperBound(items, n, nextLevel, current->cw, current->cv, capacity);
        if (bound > maxValue) 
        { // 只有可能更好的才入队
            Node* right = createNode(nextLevel, current->cw, current->cv, bound, current, 0);
            enqueue(queue, right);
        }

        free(current);
    }

    // 将最优路径映射到原始索引
    for (int i = 0; i < n; i++) 
    {
        if (bestPath[i]) 
        {
            selected[items[i].index] = 1; // 使用原始索引记录选择情况
        }
    }

    free(queue);
    return maxValue;
}

int main() 
{
    int n, capacity;
    Item items[max] = {};

    printf("请输入物品数目n：");
    scanf_s("%d", &n);

    printf("请输入背包容量c：");
    scanf_s("%d", &capacity);

    printf("请输入每个物品的体积和价值（格式：w v）：\n");
    for (int i = 0; i < n; i++) 
    {
        items[i].index = i;
        scanf_s("%d %d", &items[i].w, &items[i].v);
        items[i].r = (double)items[i].v / items[i].w;
    }

    // 按价值率降序排序
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (items[j].r < items[j + 1].r) 
            {
                swap(&items[j], &items[j + 1]);
            }
        }
    }

    int maxValue = knapsackBranch(items, n, capacity);

    printf("\n最大价值为：%d\n", maxValue);
    printf("选择的物品（原始索引）：");
    int first = 1;
    for (int i = 0; i < n; i++) 
    {
        if (selected[i]) 
        {
            if (!first) printf(", ");
            printf("%d", i + 1);
            first = 0;
        }
    }
    if (first) printf("无");
    printf("\n");

    return 0;
}