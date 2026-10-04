#include <stdio.h>

int verifyFiles(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");  // 以二進制模式開啟文件1
    FILE* f2 = fopen(file2, "rb");  // 以二進制模式開啟文件2

    // 檢查文件是否成功打開
    if (f1 == NULL || f2 == NULL) {
        printf("Error opening files!\n");
        return 0;  // 若無法打開文件，返回錯誤
    }

    int ch1, ch2;
    // 比較兩個文件的每一個字節
    while ((ch1 = fgetc(f1)) != EOF && (ch2 = fgetc(f2)) != EOF) {
        if (ch1 != ch2) {  // 如果兩個文件的字節不一致，返回不匹配
            fclose(f1);
            fclose(f2);
            return 0;  // 文件不一致
        }
    }

    // 確保兩個文件都已經讀取完畢
    if (fgetc(f1) != EOF || fgetc(f2) != EOF) {
        fclose(f1);
        fclose(f2);
        return 0;  // 如果一個文件結束，另一個文件還有內容，則大小不同，返回不匹配
    }

    fclose(f1);
    fclose(f2);
    return 1;  // 如果所有字節都一致，返回匹配
}

int main() {
    const char* file1 = "input.txt";  // 原始文件
    const char* file2 = "output.txt"; // 解壓縮後的文件

    // 驗證兩個文件是否一致
    if (verifyFiles(file1, file2)) {
        printf("Files match!\n");  // 如果文件一致
    } else {
        printf("Files do not match!\n");  // 如果文件不一致
    }

    return 0;
}

