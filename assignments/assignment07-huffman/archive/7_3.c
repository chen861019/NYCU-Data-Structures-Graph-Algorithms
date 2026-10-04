#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

// 節點結構
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// 最小堆結構
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

// 創建新節點
struct MinHeapNode* createNode(char data, unsigned freq) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// 創建最小堆
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* heap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (struct MinHeapNode**)malloc(heap->capacity * sizeof(struct MinHeapNode*));
    return heap;
}

// 交換節點
void swapNodes(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// 最小堆化
void minHeapify(struct MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->array[left]->freq < heap->array[smallest]->freq)
        smallest = left;

    if (right < heap->size && heap->array[right]->freq < heap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&heap->array[smallest], &heap->array[idx]);
        minHeapify(heap, smallest);
    }
}

// 提取最小節點
struct MinHeapNode* extractMin(struct MinHeap* heap) {
    struct MinHeapNode* temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
    return temp;
}

// 插入節點到最小堆
void insertMinHeap(struct MinHeap* heap, struct MinHeapNode* node) {
    heap->size++;
    int i = heap->size - 1;
    while (i && node->freq < heap->array[(i - 1) / 2]->freq) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

// 建立霍夫曼樹
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeap* heap = createMinHeap(size);
    for (int i = 0; i < size; i++) {
        heap->array[i] = createNode(data[i], freq[i]);
    }
    heap->size = size;
    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        minHeapify(heap, i);
    
    while (heap->size > 1) {
        struct MinHeapNode* left = extractMin(heap);
        struct MinHeapNode* right = extractMin(heap);
        struct MinHeapNode* top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(heap, top);
    }
    return extractMin(heap);
}

// 生成霍夫曼編碼表
void generateCodes(struct MinHeapNode* root, char* code, int top, char codes[][MAX_TREE_HT]) {
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1, codes);
    }
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1, codes);
    }
    if (!root->left && !root->right) {
        code[top] = '\0';
        strcpy(codes[root->data], code);
    }
}

// 計算壓縮比
void calculateCompressionRatio(const char* inputFile, const char* compressedFile) {
    FILE *input = fopen(inputFile, "rb");
    FILE *compressed = fopen(compressedFile, "rb");

    fseek(input, 0L, SEEK_END);
    fseek(compressed, 0L, SEEK_END);

    long originalSize = ftell(input);
    long compressedSize = ftell(compressed);

    fclose(input);
    fclose(compressed);

    printf("壓縮比：%.2f%%\n", 100.0 * (1.0 - (double)compressedSize / originalSize));
}

// 壓縮文件
void compressFile(FILE* input, FILE* output, char codes[][MAX_TREE_HT]) {
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        fputs(codes[ch], output);
    }
}

// 解壓縮文件
void decompressFile(FILE* input, FILE* output, struct MinHeapNode* root) {
    struct MinHeapNode* current = root;
    int bit;
    while ((bit = fgetc(input)) != EOF) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            fputc(current->data, output);
            current = root;
        }
    }
}

// 驗證兩個文件是否一致
int verifyFiles(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");

    int ch1, ch2;
    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    }
    fclose(f1);
    fclose(f2);
    return (ch1 == EOF && ch2 == EOF);
}

int main() {
    FILE* input = fopen("input.txt", "r");
    FILE* compressed = fopen("compressed.bin", "w");
    FILE* decompressed = fopen("output.txt", "w");

    if (!input || !compressed || !decompressed) {
        printf("文件打開失敗！\n");
        return 1;
    }

    int freq[256] = {0};
    int ch;
    while ((ch = fgetc(input)) != EOF) freq[ch]++;

    char data[256];
    int dataFreq[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            data[size] = i;
            dataFreq[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(data, dataFreq, size);
    char codes[256][MAX_TREE_HT] = {0};
    char code[MAX_TREE_HT];
    generateCodes(root, code, 0, codes);

    rewind(input);
    compressFile(input, compressed, codes);
    fclose(compressed);

    compressed = fopen("compressed.bin", "r");
    decompressFile(compressed, decompressed, root);
    fclose(decompressed);

    if (verifyFiles("input.txt", "output.txt")) {
        printf("壓縮與解壓縮成功。\n");
    } else {
        printf("文件不一致！\n");
    }

    calculateCompressionRatio("input.txt", "compressed.bin");

    fclose(input);
    fclose(compressed);
    return 0;
}

