#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 生成隨機數據並將其存入檔案
void generateRandomArrayToFile(const char *filename, int n, int seed) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("無法打開檔案 %s\n", filename);
        exit(1);
    }

    srand(seed);
    for (int i = 0; i < n; i++) {
        int random_num = rand();
        fprintf(file, "%d\n", random_num);  // 每個數寫入檔案
    }

    fclose(file);
    printf("已生成數據並存入檔案: %s\n", filename);
}

int main() {
    int n = 100000;  // 要生成的隨機數數量
    int seed = 42;  // 隨機數種子
    const char *filename = "random_numbers.txt";  // 檔案名稱

    generateRandomArrayToFile(filename, n, seed);
    return 0;
}

