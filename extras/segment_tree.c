#include <stdio.h>
#include <stdlib.h>

// 建立 Segment Tree
void buildTree(int *tree, int *data, int n) {
    // 初始化樹葉節點
    for (int i = 0; i < n; i++) {
        tree[n + i] = data[i];
    }
    // 建構樹的內部節點
    for (int i = n - 1; i > 0; i--) {
        tree[i] = tree[2 * i] + tree[2 * i + 1];
    }
}

// 更新 Segment Tree
void update(int *tree, int n, int idx, int value) {
    // 更新樹葉節點
    int pos = idx + n;
    tree[pos] = value;
    // 更新父節點
    while (pos > 1) {
        pos /= 2;
        tree[pos] = tree[2 * pos] + tree[2 * pos + 1];
    }
}

// 區間查詢（求和）
int sumRange(int *tree, int n, int left, int right) {
    int result = 0;
    left += n;
    right += n;
    while (left <= right) {
        if (left % 2 == 1) {
            result += tree[left];
            left++;
        }
        if (right % 2 == 0) {
            result += tree[right];
            right--;
        }
        left /= 2;
        right /= 2;
    }
    return result;
}

int main() {
    int data[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(data) / sizeof(data[0]);

    // 建立 Segment Tree，需要 2 * n 的空間
    int *tree = (int *)malloc(2 * n * sizeof(int));
    
    // 建構 Segment Tree
    buildTree(tree, data, n);

    // 查詢範圍 1 到 3 的總和
    printf("Sum of range [1, 3]: %d\n", sumRange(tree, n, 1, 3)); // 輸出 15

    // 更新索引 2 的值為 6
    update(tree, n, 2, 6);

    // 再次查詢範圍 1 到 3 的總和
    printf("Sum of range [1, 3] after update: %d\n", sumRange(tree, n, 1, 3)); // 輸出 16

    // 釋放記憶體
    free(tree);
    
    return 0;
}

