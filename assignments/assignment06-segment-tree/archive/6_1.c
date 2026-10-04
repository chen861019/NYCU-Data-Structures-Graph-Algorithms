#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

// 定義區間樹節點結構
typedef struct SegmentTreeNode {
    int start, end;     // 節點表示的區間範圍 [start, end]
    int maxValue;       // 節點範圍內的最大值
    struct SegmentTreeNode *left, *right; // 左子樹和右子樹指標
} SegmentTreeNode;

// 建構區間樹節點
SegmentTreeNode* buildTree(int* arr, int start, int end) {
    SegmentTreeNode* node = (SegmentTreeNode*)malloc(sizeof(SegmentTreeNode));
    node->start = start;
    node->end = end;

    if (start == end) {
        // 葉節點
        node->maxValue = arr[start];
        node->left = node->right = NULL;
    } else {
        // 分裂區間，遞迴建構左右子樹
        int mid = (start + end) / 2;
        node->left = buildTree(arr, start, mid);
        node->right = buildTree(arr, mid + 1, end);
        node->maxValue = (node->left->maxValue > node->right->maxValue) ? node->left->maxValue : node->right->maxValue;
    }

    return node;
}

// 區間最大值查詢
int queryTree(SegmentTreeNode* root, int l, int r) {
    if (root == NULL || l > root->end || r < root->start) {
        // 查詢範圍與節點無交集
        return INT_MIN;
    }
    if (l <= root->start && r >= root->end) {
        // 查詢範圍完全包含節點
        return root->maxValue;
    }
    // 查詢左右子樹
    int leftMax = queryTree(root->left, l, r);
    int rightMax = queryTree(root->right, l, r);
    return (leftMax > rightMax) ? leftMax : rightMax;
}

// 釋放區間樹
void freeTree(SegmentTreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// 直接方法查詢最大值 (用於驗證)
int straightforwardMax(int* arr, int l, int r) {
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

    // 輸入陣列大小和元素
    printf("Enter size of array (n): ");
    scanf("%d", &n);
    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter array elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // 建構區間樹
    SegmentTreeNode* root = buildTree(arr, 0, n - 1);

    // 輸入查詢次數
    printf("Enter number of queries (m): ");
    scanf("%d", &m);

    // 處理每個查詢，並比較直接方法的結果
    for (int i = 0; i < m; i++) {
        int l, r;
        printf("Enter query (l r): ");
        scanf("%d %d", &l, &r);
        int segTreeMax = queryTree(root, l, r);
        int straightforwardMaxVal = straightforwardMax(arr, l, r);

        printf("Segment Tree Max: %d, Direct Max: %d\n", segTreeMax, straightforwardMaxVal);

        // 驗證結果
        if (segTreeMax != straightforwardMaxVal) {
            printf("Error: Results do not match!\n");
        }
    }

    // 性能測試
    clock_t start, end;
    start = clock();
    for (int i = 0; i < 10000; i++) { // 模擬大量查詢
        queryTree(root, rand() % n, rand() % n);
    }
    end = clock();
    printf("Segment Tree Query Time for 10000 queries: %.2f ms\n", (double)(end - start) * 1000 / CLOCKS_PER_SEC);

    // 釋放記憶體
    freeTree(root);
    free(arr);

    return 0;
}

