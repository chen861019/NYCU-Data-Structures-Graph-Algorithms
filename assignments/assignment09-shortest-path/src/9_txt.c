// Programming Assignment IX 
// Shortest Path for TXT Output
// author : 陳沛安
// date: 2024/12/31

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 隨機生成圖並寫入 graph.txt
void generateGraphFile(const char* filename, int V, int E) {
    // 開啟檔案以寫入模式
    FILE* file = fopen(filename, "w");
    if (!file) {
        // 若檔案無法開啟，輸出錯誤訊息並結束程式
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // 在檔案中寫入頂點數 (V) 和邊數 (E)
    fprintf(file, "%d %d\n", V, E);

    // 初始化隨機數種子，使每次執行生成不同的圖
    srand(time(NULL));
    
    // 生成 E 條邊，隨機選擇起點與終點
    for (int i = 0; i < E; i++) {
        int src = rand() % V;  // 隨機生成起點
        int dest = rand() % V; // 隨機生成終點
        
        // 避免自環：若起點和終點相同，則重新生成終點
        while (dest == src) { 
            dest = rand() % V;
        }
        
        // 權重範圍設為 1 到 50 之間
        int weight = rand() % 50 + 1; 
        
        // 將邊的資訊 (src, dest, weight) 寫入檔案
        fprintf(file, "%d %d %d\n", src, dest, weight);
    }

    // 關閉檔案
    fclose(file);
    
    // 顯示生成成功訊息
    printf("Graph with %d vertices and %d edges saved to %s\n", V, E, filename);
}

int main() {
    int V, E;  // 頂點數與邊數

    // 提示使用者輸入頂點數
    printf("Enter number of vertices: ");
    scanf("%d", &V);

    // 提示使用者輸入邊數
    printf("Enter number of edges: ");
    scanf("%d", &E);

    // 檢查邊數是否超過無向圖的最大可能邊數 V*(V-1)/2
    if (E > V * (V - 1) / 2) {
        // 若邊數超過最大可能值，提示錯誤並結束程式
        printf("Too many edges for %d vertices. Maximum possible is %d.\n", V, V * (V - 1) / 2);
        return 1;
    }

    // 調用函式生成圖並輸出到 graph.txt
    generateGraphFile("graph.txt", V, E);
    
    // 暫停程式，等待使用者按鍵後結束 (僅在 Windows 有效)
    system("pause");
    
    return 0;
}

