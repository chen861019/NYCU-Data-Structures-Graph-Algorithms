#include <stdio.h>
#include <stdlib.h>

typedef struct TNode{
    int num;
    struct TNode *Link;
} NODE, *PNODE;

int main(void) {
    int i, num, in;
    PNODE pHead, pTail, pGet;
    pHead = pTail = NULL;
    
    printf("幾個node : ");
    scanf("%d", &num);
    
    if ((pHead = (PNODE) malloc(sizeof(NODE))) == NULL) {
        printf("記憶體不足\n");
        exit(0);
    }

    printf("請輸入第1個: ");
    scanf("%d", &in);
    pHead->num = in;
    pHead->Link = NULL;
    pTail = pHead;
    
    for (i = 1; i < num; i++) {
        if ((pGet = (PNODE) malloc(sizeof(NODE))) == NULL) {
            printf("記憶體不足\n");
            exit(0);
        }

        printf("請輸入第%d個: ", i+1);
        scanf("%d", &in);
        pGet->num = in;
        pGet->Link = NULL;
        pTail->Link = pGet;  // 正確連接新節點
        pTail = pGet;        // 更新尾節點
    }

    pGet = pHead;
    i = 1;
    
    while (pGet != NULL) {
        printf("第%d個node為: %d\n", i, pGet->num);
        pGet = pGet->Link;
        i++;
    }
    
    return 0;
}

