#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TREE_HT 256

// 霍夫曼樹節點結構
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// 創建新的霍夫曼樹節點
struct MinHeapNode* createNode(char data, unsigned freq) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// 最小堆結構
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

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

// 壓縮文件
void compressFile(FILE* input, FILE* output, char codes[][MAX_TREE_HT]) {
    int ch;
    int bitBuffer = 0;
    int bitCount = 0;
    int totalBits = 0;

    while ((ch = fgetc(input)) != EOF) {
        char* code = codes[ch];
        for (int i = 0; code[i] != '\0'; i++) {
            bitBuffer = (bitBuffer << 1) | (code[i] - '0');
            bitCount++;
            totalBits++;
            if (bitCount == 8) {
                fputc(bitBuffer, output);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        bitBuffer = (bitBuffer << (8 - bitCount)) & 0xFF;  // 補齊最後一個位元組
        fputc(bitBuffer, output);
    }
    fwrite(&totalBits, sizeof(int), 1, output);  // 儲存總位元數
}

// 解壓縮文件
void decompressFile(FILE* input, FILE* output, struct MinHeapNode* root) {
    fseek(input, -(long)sizeof(int), SEEK_END);

    int totalBits;
    fread(&totalBits, sizeof(int), 1, input);
    rewind(input);

    struct MinHeapNode* current = root;
    unsigned char byte;  // 使用 unsigned char 來處理 8 位元資料
    int bitsRead = 0;

    while (bitsRead < totalBits) {
        byte = fgetc(input);  // 此處讀入 byte
        for (int i = 7; i >= 0 && bitsRead < totalBits; i--) {
            current = (byte & (1 << i)) ? current->right : current->left;
            bitsRead++;
            if (!current->left && !current->right) {
                fputc(current->data, output);
                current = root;
            }
        }
    }
}

// 釋放霍夫曼樹佔用的記憶體
void freeHuffmanTree(struct MinHeapNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

// 生成指定大小的測試文件
void generateFile(int size_in_kb) {
    FILE *file = fopen("input.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int size_in_bytes = size_in_kb * 1024;
    srand(time(NULL));
    while (size_in_bytes--) {
        fputc('a' + rand() % 26, file);
    }
    fclose(file);
    printf("Test file created.\n");
}

int main() {
    generateFile(10);  // 生成 10 KB 的測試文件
    FILE* input = fopen("input.txt", "r");
    FILE* compressed = fopen("compressed.bin", "wb");
    FILE* decompressed = fopen("output.txt", "w");

    int freq[256] = {0};
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        freq[ch]++;
    }
    rewind(input);

    char symbols[256];
    int freqs[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            symbols[size] = i;
            freqs[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(symbols, freqs, size);
    char codes[256][MAX_TREE_HT] = {0};
    char code[MAX_TREE_HT];
    generateCodes(root, code, 0, codes);

    compressFile(input, compressed, codes);
    fclose(input);
    fclose(compressed);

    compressed = fopen("compressed.bin", "rb");
    decompressFile(compressed, decompressed, root);
    fclose(compressed);
    fclose(decompressed);

    // 釋放記憶體
    freeHuffmanTree(root);

    printf("Compression and decompression completed.\n");
    return 0;
}

