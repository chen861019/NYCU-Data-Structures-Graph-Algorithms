//Programming Assignment VII
//Huffman Code
//author : 陳沛安 
//date: 2024/12/04
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

// 計算字符頻率
void calculateFrequency(const char* str, char* symbols, int* freq, int* size) {
    int freqArr[256] = {0};
    for (int i = 0; str[i] != '\0'; i++) {
        freqArr[(unsigned char)str[i]]++;
    }

    int idx = 0;
    for (int i = 0; i < 256; i++) {
        if (freqArr[i] > 0) {
            symbols[idx] = (char)i;
            freq[idx] = freqArr[i];
            idx++;
        }
    }
    *size = idx;
}

// 顯示字符頻率
void displayFrequencies(char* symbols, int* freq, int size) {
    printf("Character Frequencies:\n");
    for (int i = 0; i < size; i++) {
        printf("%c: %d\n", symbols[i], freq[i]);
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

// 計算文件大小
long getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

// 計算壓縮比並顯示兩個檔案的大小
void calculateCompressionRatio(const char* originalFile, const char* compressedFile) {
    FILE* input = fopen(originalFile, "rb");
    FILE* compressed = fopen(compressedFile, "rb");

    if (input == NULL || compressed == NULL) {
        printf("Error opening files!\n");
        return;
    }

    long originalSize = getFileSize(input);
    long compressedSize = getFileSize(compressed);

    fclose(input);
    fclose(compressed);

    // 顯示原始檔案和壓縮檔案的大小
    printf("Original file size: %ld bytes\n", originalSize);
    printf("Compressed file size: %ld bytes\n", compressedSize);

	// 計算減少的字節數
    long decreaseInSize = originalSize - compressedSize;
    printf("Reduction in size: %ld bytes\n", decreaseInSize);
	
    if (originalSize == 0) {
        printf("Original file is empty!\n");
        return;
    }

    double ratio = (double)originalSize / compressedSize;
    printf("Compression ratio: %.2f%%\n", ratio * 100);
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
    printf("File 'input.txt' of size %dKB created successfully with random content.\n", size_in_kb);
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

int main() {
#ifdef DEBUG
    // Debug 模式: 使用測試數據
    const char* inputString = "abcabdcc";

    char symbols[256];
    int freq[256];
    int size = 0;

    // 計算頻率
    calculateFrequency(inputString, symbols, freq, &size);

    // 顯示頻率
    displayFrequencies(symbols, freq, size);

    struct MinHeapNode* root = buildHuffmanTree(symbols, freq, size);
    char codes[256][MAX_TREE_HT] = {0};
    char code[MAX_TREE_HT];
    generateCodes(root, code, 0, codes);

    // 顯示每個符號的霍夫曼編碼
    printf("Huffman Codes:\n");
    for (int i = 0; i < size; i++) {
        printf("%c: %s\n", symbols[i], codes[(unsigned char)symbols[i]]);
    }
#else   
    generateFile(100);  // 生成 10 KB 的測試文件
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
        if (freq[i] > 0) {
            symbols[size] = (char)i;
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

    // 驗證壓縮與解壓縮文件是否一致
	printf("\033[1;32mAre the files different? Compression and decompression check:\033[0m\n");
	if (verifyFiles("input.txt", "output.txt")) {
	    printf("\033[1;32mThe files are identical. Compression and decompression were successful.\033[0m\n");
	} else {
	    printf("\033[1;31mThe files are different! Compression and decompression failed.\033[0m\n");
	}
	
	// 計算壓縮比
    calculateCompressionRatio("input.txt", "compressed.bin");
#endif	
    freeHuffmanTree(root);
    return 0;
}

