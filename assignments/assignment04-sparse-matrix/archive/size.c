#include <stdio.h>
#include <stdlib.h>

struct Node {
    int col;
    int value;
    struct Node *Link;
};

int main() {
    struct Node *node1 = (struct Node *)malloc(sizeof(struct Node)); // 分配一個節點的記憶體

    if (node1 != NULL) {
        printf("Size of a Node: %lu bytes\n", sizeof(struct Node));  // 每個節點的記憶體
        free(node1);
    }
    return 0;
}

