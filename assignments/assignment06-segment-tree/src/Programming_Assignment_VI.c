//Programming Assignment VI 
//Segment Tree
//author : 陳沛安 
//date: 2024/11/20
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
        segTree->tree[node] = arr[start];
    } else {
        int mid = (start + end) / 2;
        buildTree(arr, segTree, start, mid, 2 * node + 1);
        buildTree(arr, segTree, mid + 1, end, 2 * node + 2);
        segTree->tree[node] = (segTree->tree[2 * node + 1] > segTree->tree[2 * node + 2]) 
                                ? segTree->tree[2 * node + 1] : segTree->tree[2 * node + 2];
    }
}

// 區間最大值查詢
int queryTree(SegmentTree *segTree, int start, int end, int l, int r, int node) {
    if (l > end || r < start) return INT_MIN;
    if (l <= start && r >= end) return segTree->tree[node];

    int mid = (start + end) / 2;
    int leftMax = queryTree(segTree, start, mid, l, r, 2 * node + 1);
    int rightMax = queryTree(segTree, mid + 1, end, l, r, 2 * node + 2);
    return (leftMax > rightMax) ? leftMax : rightMax;
}

// 暴力法查詢
int straightforwardMax(int *arr, int l, int r) {
    int maxVal = INT_MIN;
    for (int i = l; i <= r; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    return maxVal;
}

// 計算時間差
struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void performanceTest(SegmentTree *segTree, int *arr, int n) {
    int querySizes[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
    int numTests = sizeof(querySizes) / sizeof(querySizes[0]);

    for (int i = 0; i < numTests; i++) {
        int m = querySizes[i];
        struct timespec start, end;
        double segTreeTime, bruteForceTime;

        // 測量區間樹查詢時間
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int j = 0; j < m; j++) {
            int l = rand() % n;
            int r = rand() % n;
            if (l > r) { int temp = l; l = r; r = temp; }
            queryTree(segTree, 0, n - 1, l, r, 0);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        segTreeTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Segment Tree: %d queries in %.9f seconds\n", m, segTreeTime);

        // 測量暴力法查詢時間
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int j = 0; j < m; j++) {
            int l = rand() % n;
            int r = rand() % n;
            if (l > r) { int temp = l; l = r; r = temp; }
            straightforwardMax(arr, l, r);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        bruteForceTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Brute Force: %d queries in %.9f seconds\n", m, bruteForceTime);

        // 顯示時間差異
        printf("Time Difference (Segment Tree vs Brute Force): %.9f seconds\n", bruteForceTime - segTreeTime);
    }
}


int main() {
    int n, m;
    srand(time(NULL));
    printf("Enter size of array (n): ");
    scanf("%d", &n);
    if (n <= 0) { printf("Array size must be positive.\n"); return 1; }

    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) { printf("Memory allocation failed.\n"); return 1; }
    printf("Randomly generated array: ");
    for (int i = 0; i < n; i++) { arr[i] = rand() % 100 - 50; printf("%d ", arr[i]); }
    printf("\n");

    SegmentTree segTree;
    int treeSize = 2 * (1 << ((int)ceil(log2(n)))) - 1;
    segTree.tree = (int *)malloc(treeSize * sizeof(int));
    if (!segTree.tree) { printf("Memory allocation failed.\n"); free(arr); return 1; }

    buildTree(arr, &segTree, 0, n - 1, 0);

#ifdef DEBUG
    performanceTest(&segTree, arr, n);
#else
    printf("Enter number of queries (m): ");
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        int l, r;
        printf("Enter query (l r): ");
        scanf("%d %d", &l, &r);
        int segTreeMax = queryTree(&segTree, 0, n - 1, l, r, 0);
        int bruteForceMax = straightforwardMax(arr, l, r);
        printf("Segment Tree Max: %d, Brute Force Max: %d\n", segTreeMax, bruteForceMax);
        if (segTreeMax != bruteForceMax) { printf("Error: Results do not match!\n"); }
    }
#endif
    free(segTree.tree);
    free(arr);
    return 0;
}

