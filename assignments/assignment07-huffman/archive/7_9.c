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

// 列印霍夫曼編碼
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; i++)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// 壓縮文件
void compressFile(FILE* input, FILE* output, char codes[][MAX_TREE_HT]) {
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        fputs(codes[ch], output);  // 使用霍夫曼編碼壓縮文件
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

// 驗證文件一致性
int verifyFiles(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");

    // 檢查文件是否成功打開
    if (f1 == NULL || f2 == NULL) {
        printf("Error opening files!\n");
        return 0;  // 若無法打開文件，返回錯誤
    }

    int ch1, ch2;
    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        if (ch1 != ch2) {  // 比較文件的每一個字節
            fclose(f1);
            fclose(f2);
            return 0;  // 只要有不相等的字節，就返回不匹配
        }
    }

    // 確保兩個文件都已經讀取完畢
    if (fgetc(f1) != EOF || fgetc(f2) != EOF) {
        fclose(f1);
        fclose(f2);
        return 0;  // 文件大小不相等
    }

    fclose(f1);
    fclose(f2);
    return 1;  // 如果兩個文件的所有字節都相等，返回匹配
}

// 計算壓縮比
void calculateCompressionRatio(const char* inputFile, const char* compressedFile) {
    FILE* input = fopen(inputFile, "rb");
    FILE* compressed = fopen(compressedFile, "rb");

    if (input == NULL || compressed == NULL) {
        printf("Error opening files for compression ratio calculation.\n");
        return;
    }

    fseek(input, 0L, SEEK_END);
    fseek(compressed, 0L, SEEK_END);

    long originalSize = ftell(input);
    long compressedSize = ftell(compressed);

    fclose(input);
    fclose(compressed);

    printf("Original file size: %ld bytes\n", originalSize);
    printf("Compressed file size: %ld bytes\n", compressedSize);
    printf("Compression ratio: %.2f%%\n", 100.0 * (1.0 - (double)compressedSize / originalSize));
}

// 生成指定大小的文件
void generateFile(int size_in_kb) {
    FILE *file = fopen("input.txt", "w");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int size_in_bytes = size_in_kb * 1024;  // 轉換為字節
    char random_char;
    
    // 設定隨機數種子
    srand(time(NULL));

    // 生成隨機小寫字符並寫入文件
    while (size_in_bytes > 0) {
        // 隨機生成一個小寫字母（a-z）
        random_char = (rand() % 26) + 'a';  // 生成 'a' 到 'z' 的隨機小寫字母

        fputc(random_char, file);
        size_in_bytes--;
    }

    fclose(file);
    printf("File 'input.txt' of size %dKB created successfully with random content.\n", size_in_kb);
}

int main() {

#ifdef DEBUG
    // Debug 模式: 使用測試數據
    char symbols[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(symbols) / sizeof(symbols[0]);

    // 創建霍夫曼樹並生成霍夫曼編碼
    struct MinHeapNode* root = buildHuffmanTree(symbols, freq, size);
    char codes[256][MAX_TREE_HT] = {0};  // 用來存放霍夫曼編碼
    char code[MAX_TREE_HT];
    generateCodes(root, code, 0, codes);  // 生成霍夫曼編碼

    // 列印霍夫曼編碼
    printf("Huffman Codes:\n");
    int arr[MAX_TREE_HT];  // 定義為 int 類型數組
    printCodes(root, arr, 0);  // 列印霍夫曼編碼
#else
	generateFile(1000);
    // Release 模式: 讀取文件進行壓縮與解壓
    FILE* input = fopen("input.txt", "r");
    FILE* compressed = fopen("compressed.bin", "w");
    FILE* decompressed = fopen("output.txt", "w");

    if (!input || !compressed || !decompressed) {
        printf("File opening failed!\n");
        return 1;
    }

    // 讀取 input.txt 並生成霍夫曼樹
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, input);
    buffer[file_size] = '\0';
    
    int freq[256] = {0};
    for (long i = 0; i < file_size; i++) {
        freq[(unsigned char)buffer[i]]++;
    }
    
    // 構建霍夫曼樹
    char symbols[256];
    int freq_sorted[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            symbols[size] = (char)i;
            freq_sorted[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(symbols, freq_sorted, size);
    char codes[256][MAX_TREE_HT] = {0};
    char code[MAX_TREE_HT];
    generateCodes(root, code, 0, codes);  // 生成霍夫曼編碼

    // 壓縮文件
    rewind(input);
    compressFile(input, compressed, codes);  // 使用霍夫曼編碼壓縮文件
    fclose(compressed);

    // 計算並顯示壓縮比
    calculateCompressionRatio("input.txt", "compressed.bin");

    // 解壓縮文件
    compressed = fopen("compressed.bin", "r");
    decompressFile(compressed, decompressed, root);  // 解壓縮
    fclose(decompressed);

    if (verifyFiles("input.txt", "output.txt")) {
        printf("Compression and decompression successful.\n");
    } else {
        printf("Files do not match!\n");
    }

    fclose(input);
    fclose(compressed);
    free(buffer);
#endif

    return 0;
}

