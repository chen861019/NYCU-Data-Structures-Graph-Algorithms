#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

// 定義區間樹節點結構
typedef struct SegmentTree {
    int *tree; // 儲存區間樹的陣列
    int size;  // 區間樹的大小
} SegmentTree;

// 建構區間樹 (遞迴)
void buildTree(int *arr, SegmentTree *segTree, int start, int end, int node) {
    if (start == end) {
        // 葉節點
        segTree->tree[node] = arr[start];
    } else {
        // 建構左右子樹
        int mid = (start + end) / 2;
        buildTree(arr, segTree, start, mid, 2 * node + 1);
        buildTree(arr, segTree, mid + 1, end, 2 * node + 2);
        // 內部節點的值為子節點的最大值
        segTree->tree[node] = (segTree->tree[2 * node + 1] > segTree->tree[2 * node + 2]) 
                                ? segTree->tree[2 * node + 1] : segTree->tree[2 * node + 2];
    }
}

// 區間最大值查詢 (遞迴)
int queryTree(SegmentTree *segTree, int start, int end, int l, int r, int node) {
    if (l > end || r < start) {
        // 無交集，返回負無窮大
        return INT_MIN;
    }
    if (l <= start && r >= end) {
        // 完全包含
        return segTree->tree[node];
    }

    // 部分重疊，查詢左右子樹
    int mid = (start + end) / 2;
    int leftMax = queryTree(segTree, start, mid, l, r, 2 * node + 1);
    int rightMax = queryTree(segTree, mid + 1, end, l, r, 2 * node + 2);
    return (leftMax > rightMax) ? leftMax : rightMax;
}

// 直接方法查詢最大值 (暴力法)
int straightforwardMax(int *arr, int l, int r) {
    int maxVal = INT_MIN;
    for (int i = l; i <= r; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int main() {
    int n, m;

    // 設定隨機種子
    srand(time(NULL));

    // 隨機生成陣列大小和查詢次數
    printf("Enter size of array (n): ");
    scanf("%d", &n);
    printf("Enter number of queries (m): ");
    scanf("%d", &m);

    // 隨機生成陣列元素，範圍為 -50 到 49
    int *arr = (int *)malloc(n * sizeof(int));
    printf("Randomly generated array: ");
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100 - 50; // 隨機生成 -50 到 49 的數字
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 初始化區間樹
    SegmentTree segTree;
    int treeSize = 2 * (1 << ((int)ceil(log2(n)))) - 1;
    segTree.tree = (int *)malloc(treeSize * sizeof(int));
    segTree.size = treeSize;

    // 建構區間樹
    buildTree(arr, &segTree, 0, n - 1, 0);

    // 處理每個查詢並比較結果
    for (int i = 0; i < m; i++) {
        int l, r;
        printf("Enter query (l r): ");
        scanf("%d %d", &l, &r);

        // 使用區間樹查詢
        int segTreeMax = queryTree(&segTree, 0, n - 1, l, r, 0);

        // 使用直接方法查詢
        int bruteForceMax = straightforwardMax(arr, l, r);

        printf("Segment Tree Max: %d, Brute Force Max: %d\n", segTreeMax, bruteForceMax);

        // 驗證結果
        if (segTreeMax != bruteForceMax) {
            printf("Error: Results do not match!\n");
        }
    }

    // 性能測試
    clock_t start, end;
    start = clock();
    for (int i = 0; i < 10000; i++) {
        int l = rand() % n;
        int r = rand() % n;
        if (l > r) {
            int temp = l;
            l = r;
            r = temp;
        }
        queryTree(&segTree, 0, n - 1, l, r, 0);
    }
    end = clock();
    printf("Segment Tree Query Time for 10000 queries: %.2f ms\n", (double)(end - start) * 1000 / CLOCKS_PER_SEC);

    // 釋放記憶體
    free(segTree.tree);
    free(arr);

    return 0;
}

